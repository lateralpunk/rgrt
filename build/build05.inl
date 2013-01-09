// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This builds the scene for Figure 22.11(e)
// The figures in the book were antialiased with 16 samples per pixel,
// but you need to compare the render times with one sample per pixel
// You should also render the images without the grid and compare the times

//CPU not avail for g=0
// -bash-3.2$ ./gpuraytracer -b=build05 -g=0 -m=1
// *** Configuration - b: build05, o: auto-gen, g: 0, w: 2400, h: 2400, n: 8, z: 6.00, m: 1, s: 1, p: 0 ****
// Using device 0: Tesla C1060
// Starting ray-tracing...
// Reorganizing Data Structures...
//   Not going to use host pinned memory...
//   Not using Grid accelration...
// Finished Consolidation.
// World Stats: 
//   Total # of GeometricObjects: 3000
//   Approximate bytes of data: 480504 bytes
// Starting GPU ray tracing...
//   # of threads in a block: 16 x 16 (256)
//   # of blocks in a grid  : 150 x 150 (22500)
//   GPU memory access time: 93.487000 (ms)
//   GPU computation time  : 24981.437500 (ms)
//   GPU processing time   : 25074.923828 (ms)
// Finished ray-tracing...

// -bash-3.2$ make build05-g1
// ./gpuraytracer -b=build05 -g=1
// *** Configuration - b: build05, o: auto-gen, g: 1, w: 2400, h: 2400, n: 8, z: 6.00, m: 0, s: 1, p: 0 ****
// Using device 0: Tesla C1060
// Starting ray-tracing...
// Reorganizing Data Structures...
//   Not going to use host pinned memory...
//   Using Grid acceleration...
//     Organizing grid cells...
//       Stats: # of cells that have 0 objects, 1 object, 2 objects, etc...
//       num_cells = 3375
//       numZeroes = 0, numOnes = 3, numTwos = 3
//       numThrees = 8 numGreater = 3361
//     Finished grid cell organization.
// Finished Consolidation.
// World Stats: 
//   Total # of GeometricObjects: 440481
//   Approximate bytes of data: 70477464 bytes
// Starting CPU ray tracing...
//   CPU processing time   : 141253.453125 (ms)
// Starting GPU ray tracing...
//   # of threads in a block: 16 x 16 (256)
//   # of blocks in a grid  : 150 x 150 (22500)
//   GPU memory access time: 553.368958 (ms)
//   GPU computation time  : 7443.520996 (ms)
//   GPU processing time   : 7996.890137 (ms)
// Calculating accuracy...
//   Error : 0.266910
// Finished ray-tracing...


void 												
build05(World *world) {
	
	world->ropt.vp.set_samples(8);
	
	world->ropt.vp.set_hres(2400);
	world->ropt.vp.set_vres(2400);
	
	// camera
	world->ropt.camera_ptr.set_eye(10, 15, 30);
	world->ropt.camera_ptr.set_lookat(0, 0, 0);
	world->ropt.camera_ptr.set_view_distance(4000);
	world->ropt.camera_ptr.set_zoom(6.0);
	world->ropt.camera_ptr.compute_uvw();     
	
	
	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(-10, 20, 20);
	light_ptr1->scale_radiance(3.0); 	
	light_ptr1->type = Light::Directional;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);

	
	int num_spheres = 3000;
	
	float volume = 0.1 / num_spheres;
	float radius = 10.0 * pow(0.75 * volume / 3.14159, 0.333333);  	// for Figure 22.17(a)
//	float radius = 250.0 * pow(0.75 * volume / 3.14159, 0.333333);  // for Figure 22.17(b)
	
	Maths::set_rand_seed(15);
	
	for (int j = 0; j < num_spheres; j++) {
		Material* matte_ptr = new Material;
		matte_ptr->set_ka(0.25);
		matte_ptr->set_kd(0.75);
		matte_ptr->set_cd(Maths::rand_float(), Maths::rand_float(), Maths::rand_float());
		matte_ptr->set_exp(20.0f);
		matte_ptr->set_ks(0.12f);
		
		GeometricObject*	sphere_ptr = new GeometricObject(make_float3(1.0 - 2.0 * Maths::rand_float(),1.0 - 2.0 * Maths::rand_float(),1.0 - 2.0 * Maths::rand_float()),radius); 
		world->set_material(matte_ptr,sphere_ptr);
		world->grid.add_object(sphere_ptr);
	}	
}


