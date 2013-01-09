
void 												
build08(World *world) {
	
	world->ropt.vp.set_samples(8);

	world->ropt.vp.set_hres(3000);
	world->ropt.vp.set_vres(3000);
	world->background_color = white;


	world->ropt.camera_ptr.set_eye(0, 0, 5);
	world->ropt.camera_ptr.set_lookat(make_float3(0.0f));
	world->ropt.camera_ptr.set_view_distance(3600);
	world->ropt.camera_ptr.set_zoom(5.0);
	world->ropt.camera_ptr.compute_uvw();     


	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(120, 140, 220);
	light_ptr1->scale_radiance(1.5); 	
	light_ptr1->type = Light::Directional;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);

	Material* dielectric_ptr = new Material;
	dielectric_ptr->set_ka(0.1);		
	dielectric_ptr->set_kd(0.8);
	dielectric_ptr->set_ks(1);    
	dielectric_ptr->set_exp(40.0);
	dielectric_ptr->set_cd(0.65, 0.65, 0.5);   // lemon
	world->add_material(dielectric_ptr);

	char* file_name = "models/Horse97K.ply"; 		

	//this automatically adds it to the grid
	world->grid.read_ply_file(file_name,dielectric_ptr);
	
}


