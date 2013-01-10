#pragma once

//./bin/darwin/release/rgrt-cuda -w=2000 -h=2000 -n=1 -g=1 -m=1

#include "build/build01.inl"
#include "build/build02.inl"
#include "build/build03.inl"
#include "build/build04.inl"
#include "build/build05.inl"
#include "build/build06.inl"
#include "build/build07.inl"
#include "build/build08.inl"
#include "build/build09.inl"

static TLights sg_lights;
static TMaterials sg_materials;


#define DEFAULT_BUILD_FILE "build01"
#define MAX_IMAGE_DIM 16384
#define MAX_SAMPLES 256

#include "gpuraytracer.inl"
#include "cpuraytracer.inl"

World::World(void) : 
lights_cached(NULL),
background_color(make_float3(0.0f)),
numLights(0),
usePinnedLights(false),usePinnedMaterials(false),
materials_cached(NULL),
numMaterials(0)
{
}

World::~World()
{
	delete_vectors();
}

void World::delete_vectors(void)
{
	for(TLightsIter iter = sg_lights.begin(); 
	    iter != sg_lights.end();
	    ++iter)
	{
		SAFE_DELETE(*iter);
	}
	sg_lights.clear();

	for(TMaterialsIter iter = sg_materials.begin(); 
	    iter != sg_materials.end();
	    ++iter)
	{
		SAFE_DELETE(*iter);
	}
	sg_materials.clear();
}


inline void World::add_light(Light *light_ptr) {  
	sg_lights.push_back(light_ptr);
}

inline void World::add_material(Material *mat_ptr)
{
	//first ensure that the material exists in our list, and if not
	//then we should add it to it
 if(sg_materials.end() == find(sg_materials.begin(),sg_materials.end(),mat_ptr)) 
 {
 		sg_materials.push_back(mat_ptr);
 }

}

void World::set_material(Material *mat_ptr,GeometricObject *geo)
{
	//add material if it don't already exist...
	add_material(mat_ptr);
	//ok now let the geo point to this material
	geo->set_material(mat_ptr);
}

float3 World::trace_ray(const Ray *ray, ShadeRec *sr) const
{
	sr->the_t = FLT_MAX;
	sr->material = ULONG_MAX;
	
	if (grid.hit(ray,sr) && (sr->material != ULONG_MAX) && sr->material < numMaterials)
	{
		Material *mat = &materials_cached[sr->material];
		return mat->shade(this,ray, sr);
	}

	return background_color;
}


void World::render()
{
	printf("Starting ray-tracing...\n");
	//package up the world
	packCPU();
	
	uchar4 *c_canvas = NULL;
	uchar4 *h_canvas = NULL;
	int numPixels = ropt.vp.hres * ropt.vp.vres;
	
	//-mode: (0,1,2) 0 to render on both GPU & CPU. 1 to render on GPU only. 2 to render on CPU only. 1 & 2 imply no error checking. 0 by default.
	
	if (ropt.mode == 0 || ropt.mode == 2)
	{
		c_canvas = (uchar4*)malloc(numPixels*sizeof(uchar4));
		cpu_render_scene(c_canvas);
	}
	
	if (ropt.mode == 0 || ropt.mode == 1)
	{
		h_canvas = (uchar4*)malloc(numPixels*sizeof(uchar4));
		launchKernel(h_canvas);
	}
		
	if (ropt.mode == 0)
	{
		float maxErr = 0.0f;
		printf("Calculating accuracy...\n");
		float err = 0.0f;
		for(int idx=0; idx<numPixels; idx++)
		{
			err += sqrt((float)
			 (( (float)h_canvas[idx].x -  (float)c_canvas[idx].x)*( (float)h_canvas[idx].x -  (float)c_canvas[idx].x) +
				( (float)h_canvas[idx].y -  (float)c_canvas[idx].y)*( (float)h_canvas[idx].y -  (float)c_canvas[idx].y) +
				( (float)h_canvas[idx].z -  (float)c_canvas[idx].z)*( (float)h_canvas[idx].z -  (float)c_canvas[idx].z) +
				( (float)h_canvas[idx].w -  (float)c_canvas[idx].w)*( (float)h_canvas[idx].w -  (float)c_canvas[idx].w) ) );
		}
		maxErr = Maths::max(maxErr, err/(float)(numPixels));
		printf("  Error : %f\n",  maxErr);
		ropt.error = maxErr;
	}

	if (ropt.mode == 0 || ropt.mode == 1)
	{
		ropt.save_canvas(h_canvas);
	} else if (ropt.mode == 2)
	{
		ropt.save_canvas(c_canvas);
	}

	if (ropt.mode == 0)
	{
		free(h_canvas);
		free(c_canvas);
	} else if (ropt.mode == 1)
	{
		free(h_canvas);
	} else if (ropt.mode == 2)
	{
		free(c_canvas);
	}
	printf("Finished ray-tracing...\n");
}

void World::save_pixel(const int row, const int column, float3 *raw_color,uchar4 *canvas) const 
{
	*raw_color = clamp(*raw_color,0.0f,1.0f);
	uchar4 *ucharPixel = &canvas[row*ropt.vp.hres+column];
	ucharPixel->x = (uchar)(raw_color->x * 255.0f);
	ucharPixel->y = (uchar)(raw_color->y * 255.0f);
	ucharPixel->z = (uchar)(raw_color->z * 255.0f);
	ucharPixel->w = 0;
}

bool World::processArgs(int argc, char **argv)
{
	char *buildName, *m_fname;
	int useGrid=1;

	
	// Read command-line args
  char *usage=" "
    "gpuraytracer {-b=build01} {-o=build01.bmp} {-g=1} "
    "{-w=400} {-h=400} {-n=1} {-z=1.0} {-m=0} {-s=1} {-p=1} {-help}\n"
    "All arugments are optional. By default, build01 test scene is executed. \n"
		"The defaults are determined by the build scene. \n"
		"-b  The build files supported by the sytem and overridden by the ones below.\n"
		"-o: output (GPU) image saved to disk.  By default the naming scheme:\n"
		"      \"build01_g-1_p-0_wh-600x600_n-1_z-1.0_cpu-64.74_gpumem-1.30_gpu-0.01_gputotal-1.31_error-0.000050.bmp\"\n"
		"      which represents the build name, if grid was used, the resolution, \n"
		"      number of samples, zoom, cpu time, gpu memory time, gpu computation time, gpu total time, error.\n"
		"      NOTE: Your directory may become full of BMPs :)\n"
		"-g: (0,1) representing whether or not grid acceleration is used. 1 by default. \n"
		"-w: width of final image (max=16,384).\n"
		"-h: height of final image (max=16,384).\n"
		"-n: number of samples used per pixel for anti-aliasing (integer, max=256).\n"
		"-z: camera zoom factor (float).\n"
		"-m: (0,1,2) 0 to render on both GPU & CPU. 1 to render on GPU only. 2 to render on CPU only. 1 & 2 imply no error checking. 0 by default. \n"
		"-s: (0,1) 0 to not save to file, 1 to save to file.  Note, that either way, we do create a GPU/CPU buffers to store pixel data so as to be a fair test. 1 by default. \n"
		"-p: (0,1) 0 to not use host pinned memory, 1 to use pinned memory.  Default 0. Since lots of our scenes have a lot of mesh data, resonance really chokes on asking for too much pinned memory.\n"
		"-help: this menu.\n"; 
	
	if (checkCmdLineFlag(argc,(const char**)argv,"help"))
	{
		printf("USAGE: %s\n",usage);
		return false;
	}

	if (getCmdLineArgumentString(argc,(const char**)argv,"o",&m_fname))
	{
		ropt.m_fname = m_fname;
	}
	
	if (checkCmdLineFlag( argc, (const char **)argv, "g"))
	{
		useGrid = getCmdLineArgumentInt(argc,(const char**)argv,"g");
	
		if (useGrid < 0 || useGrid > 1)
		{
			useGrid = 1;
		}
	}
	ropt.useGrid = useGrid;

	if (getCmdLineArgumentString(argc,(const char**)argv,"b",&buildName))
	{
		ropt.buildName = buildName;
	} else
	{
		ropt.buildName = DEFAULT_BUILD_FILE;
	}

	//ok before proceeding with the other arguments, let's create the scene
	//with the suggested build paramaters, and then process the argumetns to customize
	if (!strcmp(ropt.buildName,"build01"))
	{
		build01(this);
	} else if (!strcmp(ropt.buildName,"build02"))
	{
		build02(this);	
	}	else if (!strcmp(ropt.buildName,"build03"))
	{
		build03(this);	
	}	else if (!strcmp(ropt.buildName,"build04"))
	{
		build04(this);	
	}	else if (!strcmp(ropt.buildName,"build05"))
	{
		build05(this);	
	}	else if (!strcmp(ropt.buildName,"build06"))
	{
		build06(this);	
	}	else if (!strcmp(ropt.buildName,"build07"))
	{
		build07(this);	
	}	else if (!strcmp(ropt.buildName,"build08"))
	{
		build08(this);	
	}			
	else if (!strcmp(ropt.buildName,"build09"))
	{
		build09(this);	
	}	
	else
	{
		printf("Invalid build name %s provided.\n\n",ropt.buildName);
		printf("USAGE: %s\n",usage);
		return false;
	}
	
	int width,height,ns,mode,save,usePinned;
	float zoom;
	
	//now proceed with customization
	if (checkCmdLineFlag( argc, (const char **)argv, "w"))
	{
		width = getCmdLineArgumentInt(argc,(const char**)argv,"w");
		ropt.vp.hres = clamp(width,4,MAX_IMAGE_DIM);
	}
	
	if (checkCmdLineFlag( argc, (const char **)argv, "h"))
	{
		height = getCmdLineArgumentInt(argc,(const char**)argv,"h");
		ropt.vp.vres = clamp(height,4,MAX_IMAGE_DIM);
	}
	
	if (checkCmdLineFlag( argc, (const char **)argv, "n"))
	{
		ns = getCmdLineArgumentInt(argc,(const char**)argv,"n");
		ropt.vp.num_samples = clamp(ns,1,MAX_SAMPLES);
	}
	
	if (checkCmdLineFlag( argc, (const char **)argv, "z"))
	{
		zoom = getCmdLineArgumentInt(argc,(const char**)argv,"z");
		ropt.camera_ptr.zoom = zoom;
	}
	
	if (ropt.camera_ptr.zoom == 0.0f)
		ropt.camera_ptr.zoom = 1.0f;
	
	if (checkCmdLineFlag( argc, (const char **)argv, "m"))
	{
		mode = getCmdLineArgumentInt(argc,(const char**)argv,"m");
		if (mode < 0 || mode > 2)
		{
			mode = 0;
		}
		ropt.mode = mode;
	}

	if (checkCmdLineFlag( argc, (const char **)argv, "s"))
	{
		save = getCmdLineArgumentInt(argc,(const char**)argv,"s");
		if (save < 0 || save > 1)
		{
			save = 1;
		}
		ropt.save = save;
	}

	if (checkCmdLineFlag( argc, (const char **)argv, "p"))
	{
		usePinned = getCmdLineArgumentInt(argc,(const char**)argv,"p");
		if (usePinned < 0 || usePinned > 1)
		{
			usePinned = 0;
		}
		ropt.usePinned = usePinned;
	}
	

	printf("*** Configuration - b: %s, o: %s, g: %i, w: %i, h: %i, n: %i, z: %.2f, m: %i, s: %i, p: %i ****\n",
					ropt.buildName, ropt.m_fname ? ropt.m_fname : "auto-gen", ropt.useGrid, ropt.vp.hres, 
					ropt.vp.vres, ropt.vp.num_samples, ropt.camera_ptr.zoom,ropt.mode,ropt.save,ropt.usePinned);
	
	return true;
}




