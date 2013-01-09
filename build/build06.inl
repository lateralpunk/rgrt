void build06(World *world) {

	world->ropt.vp.set_samples(4);

	world->ropt.vp.set_hres(2400);
	world->ropt.vp.set_vres(2400);

	// camera
	world->ropt.camera_ptr.set_eye(20, 10, 40);
	world->ropt.camera_ptr.set_lookat(-0.025, 0.11, 0.0);
	world->ropt.camera_ptr.set_view_distance(70000);
	world->ropt.camera_ptr.set_zoom(6.0);
	world->ropt.camera_ptr.compute_uvw();     


	Light *light_ptr1 = new Light();
	light_ptr1->set_direction(20, 40, 20);
	light_ptr1->scale_radiance(1.5); 	
	light_ptr1->type = Light::Directional;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);


	float ka = 0.5;		// used for all materials

	Material *matte_ptr1 = new Material();
	matte_ptr1->set_ka(ka);		
	matte_ptr1->set_kd(0.5);
	matte_ptr1->set_cd(0.5, 0.75, 1);   // pale blue for bunny
	matte_ptr1->set_exp(50.0f);
	matte_ptr1->set_ks(0.12f);
	world->add_material(matte_ptr1);

	char* file_name = "models/Bunny4K.ply"; 		// 4000 triangles

	//this automatically adds it to the grid
	world->grid.read_ply_file(file_name,matte_ptr1);

	// rectangle parameters

	float3 p0(make_float3(-0.13, 0.033, -0.1));  	// common corner
	float height = 0.25;  				// y direction
	float width = 0.45;  				// x direction	
	float depth = 0.45;   				// z direction	

	// horizontal rectangle 

	Material *matte_ptr2 = new Material;
	matte_ptr2->set_ka(ka);		
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(white);

	GeometricObject*	rectangle_ptr1= new GeometricObject(p0, make_float3(0, 0,depth), make_float3(width, 0, 0)); 			
	world->set_material(matte_ptr2,rectangle_ptr1);
	world->grid.add_object(rectangle_ptr1);	

	// rectangle perpendicular to x axis
	
	Material *matte_ptr3 = new Material;
	matte_ptr3->set_ka(ka);		
	matte_ptr3->set_kd(0.75);
	matte_ptr3->set_cd(0.5, 1, 0.75);

	GeometricObject*	rectangle_ptr2= new GeometricObject(p0, make_float3(0, height, 0), make_float3(0, 0, depth)); 			
	world->set_material(matte_ptr3,rectangle_ptr2);
	world->grid.add_object(rectangle_ptr2);	
	
	// rectangle perpendicular to w axis
	
	Material *matte_ptr4 = new Material;	
	matte_ptr4->set_ka(ka);		
	matte_ptr4->set_kd(0.5);
	matte_ptr4->set_cd(1, 1, 0.5);
	
	GeometricObject*	rectangle_ptr3= new GeometricObject(p0, make_float3(width, 0, 0), make_float3(0, height, 0)); 			
	world->set_material(matte_ptr4,rectangle_ptr3);
	world->grid.add_object(rectangle_ptr3);
}


