#pragma once

class ALIGN_8 World {	
	public:
	
		ulong numLights,numMaterials;
		Light *lights_cached;
		Material *materials_cached;
		float3 background_color;
		Light ambient_ptr;
		RenderOpt ropt;
		bool usePinnedLights,usePinnedMaterials;
		Grid grid;
		
		 World(void);				
		 ~World();								
		
		bool processArgs(int argc, char **argv);
		void add_light(Light *light_ptr); 
		void add_material(Material *mat_ptr);
		void set_material(Material *mat_ptr,GeometricObject *geo);
		void render();
		HOST_AND_DEVICE float3 trace_ray(const Ray *ray, ShadeRec *sr) const;
		HOST_AND_DEVICE void save_pixel(const int row, const int column, float3 *pixel_color,uchar4 *canvas) const;

private:

	void packCPU();
	void packMaterials(); //materials are special ;)
	void unpackCPU(Light *temp_lights,Material *temp_materials);
	void packGPU(World **d_ws);
	void unpackGPU(World *d_ws);
	void cpu_render_scene(uchar4 *canvas);
	void launchKernel(uchar4 *h_canvas);

	void delete_vectors(void);
	
		
};


