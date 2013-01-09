void build02(World *world)	
{
	world->ropt.vp.set_samples(32);
	
	world->ropt.vp.set_hres(3000);
	world->ropt.vp.set_vres(3000);
	
	// camera
	world->ropt.camera_ptr.set_eye(0.0, 0.0, 0.9);
	world->ropt.camera_ptr.set_lookat(0, 0, -100);
	world->ropt.camera_ptr.set_view_distance(150);
	world->ropt.camera_ptr.compute_uvw();     
	
	
	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(0, 0, 1);
	light_ptr1->scale_radiance(3.0); 	
	light_ptr1->type = Light::Directional;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);
	
	
	int num_spheres = 5000;
	float volume = 0.1 / num_spheres;
	float radius = pow(0.75 * volume / 3.14159, 0.333333);
	
	Maths::set_rand_seed(15);
	
	for (int j = 0; j < num_spheres; j++) 
	{
		Material *matte_ptr = new Material();
		matte_ptr->set_ka(0.25);
		matte_ptr->set_kd(0.75);
		matte_ptr->set_cd(make_float3(Maths::rand_float(), Maths::rand_float(), Maths::rand_float()));
		
		GeometricObject*	sphere_ptr= new GeometricObject(make_float3(1.0 - 2.0 * Maths::rand_float(),1.0 - 2.0 * Maths::rand_float(), 1.0 - 2.0 * Maths::rand_float()), radius); 
		world->set_material(matte_ptr,sphere_ptr);							// light purple
		world->grid.add_object(sphere_ptr);
	}	
	
}
