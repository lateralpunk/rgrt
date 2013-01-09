void 												
build03(World *world){
	world->ropt.vp.set_samples(32);
	
	world->ropt.vp.set_hres(1000);
	world->ropt.vp.set_vres(1000);
	
	// camera
	world->ropt.camera_ptr.set_eye(0, 0, 20);
	world->ropt.camera_ptr.set_lookat(0, 0, 0);
	world->ropt.camera_ptr.set_view_distance(3600);
	world->ropt.camera_ptr.set_zoom(2.5);
	world->ropt.camera_ptr.compute_uvw();     
	
	
	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(-10, 20, 20);
	light_ptr1->scale_radiance(3.0); 	
	light_ptr1->type = Light::Directional;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);
	
	

	int num_spheres = 20000;		// for Figure 22.11(b)
	
	float volume = 0.1 / num_spheres;
	float radius = pow(0.75 * volume / 3.14159, 0.333333);
	radius *= 2.0f;
	
	Maths::set_rand_seed(15);
	
	
	for (int j = 0; j < num_spheres; j++) 
	{
		Material* matte_ptr = new Material;
		matte_ptr->set_ka(0.25);
		matte_ptr->set_kd(0.75);
		matte_ptr->set_cd(Maths::rand_float(), Maths::rand_float(), Maths::rand_float());
		matte_ptr->set_exp(20.0f);
		matte_ptr->set_ks(0.12f);
		
		GeometricObject*	sphere_ptr= new GeometricObject(
		make_float3(1.0 - 2.0 * Maths::rand_float(),
		1.0 - 2.0 * Maths::rand_float(), 
		1.0 - 2.0 * Maths::rand_float()), radius); 
		world->set_material(matte_ptr,sphere_ptr);
		world->grid.add_object(sphere_ptr);
	}	

}


