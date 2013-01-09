void 												
build09(World *world) {
		

	world->ropt.vp.set_samples(16);

	world->background_color = make_float3(0.13,0.68,0.75);

	world->ropt.vp.set_hres(2400);
	world->ropt.vp.set_vres(2400);

	// camera
	world->ropt.camera_ptr.set_eye(75, 20, 80);
	world->ropt.camera_ptr.set_lookat(-0.05, -0.5, 0);
	world->ropt.camera_ptr.set_view_distance(1600);
	world->ropt.camera_ptr.set_zoom(7.0);
	world->ropt.camera_ptr.compute_uvw();     


	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(0.75, 0.5, -0.15);
	light_ptr1->scale_radiance(2.0); 	
	light_ptr1->type = Light::Directional;
	world->add_light(light_ptr1);

	Material* phong_ptr1 = new Material;			
	phong_ptr1->set_ka(0.4); 
	phong_ptr1->set_kd(0.8);
	phong_ptr1->set_cd(1.0, 0.2, 0.0); 
	phong_ptr1->set_ks(0.5); 
	phong_ptr1->set_cs(1.0, 1.0, 0.0);
	phong_ptr1->set_exp(50.0);
	world->add_material(phong_ptr1);

	char* file_name = "models/goldfish_high_res.ply"; 		

	//this automatically adds it to the grid
	world->grid.read_ply_file(file_name,phong_ptr1);

}


