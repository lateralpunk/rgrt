void build01(World *world)	
{	 

	world->ropt.vp.set_samples(16);

	world->ropt.vp.set_hres(4000);
	world->ropt.vp.set_vres(4000);

	// camera
	world->ropt.camera_ptr.set_eye(0.0f, 0.0f, 10000.0f);
	world->ropt.camera_ptr.set_lookat(make_float3(0.0f));
	world->ropt.camera_ptr.set_view_distance(15000.0f);
	world->ropt.camera_ptr.set_zoom(13.0f);
	world->ropt.camera_ptr.compute_uvw();     


	Light *light_ptr1 = new Light();
	light_ptr1->set_location(100, 100, 200);
	light_ptr1->scale_radiance(3.0); 	
	light_ptr1->type = Light::Point;
	light_ptr1->castsShadows = true;
	world->add_light(light_ptr1);

	// Material materials' reflection coefficients

	float ka 	= 0.25f;
	float kd 	= 0.75f;
	float ks 	= 0.12f;
	float exp 	= 20.0f;	

	// spheres

	Material* phong_ptr1 = new Material;
	phong_ptr1->set_ka(ka);	
	phong_ptr1->set_kd(kd);
	phong_ptr1->set_ks(ks);
	phong_ptr1->set_exp(exp);
	phong_ptr1->set_cd(yellow);
	//world->set_material(matte_ptr4,rectangle_ptr3);

	GeometricObject*	sphere_ptr1 = new GeometricObject(make_float3(5, 3, 0), 30); 
	world->set_material(phong_ptr1,sphere_ptr1);	   							// yellow
	world->grid.add_object(sphere_ptr1);


	Material* phong_ptr2 = new Material;
	phong_ptr2->set_ka(ka);	
	phong_ptr2->set_kd(kd);
	phong_ptr2->set_ks(ks);
	phong_ptr2->set_exp(exp);
	phong_ptr2->set_cd(brown);

	GeometricObject*	sphere_ptr2 = new GeometricObject(make_float3(45, -7, -60), 20); 
	world->set_material(phong_ptr2,sphere_ptr2);								// brown
	world->grid.add_object(sphere_ptr2);


	Material* phong_ptr3 = new Material;
	phong_ptr3->set_ka(ka);	
	phong_ptr3->set_kd(kd);
	phong_ptr3->set_ks(ks);
	phong_ptr3->set_exp(exp);
	phong_ptr3->set_cd(darkGreen);

	GeometricObject*	sphere_ptr3 = new GeometricObject(make_float3(40, 43, -100), 17); 
	world->set_material(phong_ptr3,sphere_ptr3);								// dark green
	world->grid.add_object(sphere_ptr3);


	Material* phong_ptr4 = new Material;
	phong_ptr4->set_ka(ka);	
	phong_ptr4->set_kd(kd);
	phong_ptr4->set_ks(ks);
	phong_ptr4->set_exp(exp);
	phong_ptr4->set_cd(orange);

	GeometricObject*	sphere_ptr4 = new GeometricObject(make_float3(-20, 28, -15), 20); 
	world->set_material(phong_ptr4,sphere_ptr4);								// orange
	world->grid.add_object(sphere_ptr4);


	Material* phong_ptr5 = new Material;
	phong_ptr5->set_ka(ka);	
	phong_ptr5->set_kd(kd);
	phong_ptr5->set_ks(ks);
	phong_ptr5->set_exp(exp);
	phong_ptr5->set_cd(green);

	GeometricObject*	sphere_ptr5 = new GeometricObject(make_float3(-25, -7, -35), 27); 			
	world->set_material(phong_ptr5,sphere_ptr5);								// green
	world->grid.add_object(sphere_ptr5);


	Material* phong_ptr6 = new Material;
	phong_ptr6->set_ka(ka);	
	phong_ptr6->set_kd(kd);
	phong_ptr6->set_ks(ks);
	phong_ptr6->set_exp(exp);
	phong_ptr6->set_cd(lightGreen);

	GeometricObject*	sphere_ptr6 = new GeometricObject(make_float3(20, -27, -35), 25); 
	world->set_material(phong_ptr6,sphere_ptr6);								// light green
	world->grid.add_object(sphere_ptr6);


	Material* phong_ptr7 = new Material;
	phong_ptr7->set_ka(ka);	
	phong_ptr7->set_kd(kd);
	phong_ptr7->set_ks(ks);
	phong_ptr7->set_exp(exp);
	phong_ptr7->set_cd(green);

	GeometricObject*	sphere_ptr7 = new GeometricObject(make_float3(35, 18, -35), 22); 
	world->set_material(phong_ptr7,sphere_ptr7);   							// green
	world->grid.add_object(sphere_ptr7);


	Material* phong_ptr8 = new Material;
	phong_ptr8->set_ka(ka);	
	phong_ptr8->set_kd(kd);
	phong_ptr8->set_ks(ks);
	phong_ptr8->set_exp(exp);
	phong_ptr8->set_cd(brown);

	GeometricObject*	sphere_ptr8 = new GeometricObject(make_float3(-57, -17, -50), 15);  
	world->set_material(phong_ptr8,sphere_ptr8);								// brown
	world->grid.add_object(sphere_ptr8);


	Material* phong_ptr9 = new Material;
	phong_ptr9->set_ka(ka);	
	phong_ptr9->set_kd(kd);
	phong_ptr9->set_ks(ks);
	phong_ptr9->set_exp(exp);
	phong_ptr9->set_cd(lightGreen);

	GeometricObject*	sphere_ptr9 = new GeometricObject(make_float3(-47, 16, -80), 23); 
	world->set_material(phong_ptr9,sphere_ptr9);								// light green
	world->grid.add_object(sphere_ptr9);


	Material* phong_ptr10 = new Material;
	phong_ptr10->set_ka(ka);	
	phong_ptr10->set_kd(kd);
	phong_ptr10->set_ks(ks);
	phong_ptr10->set_exp(exp);
	phong_ptr10->set_cd(darkGreen);

	GeometricObject*	sphere_ptr10 = new GeometricObject(make_float3(-15, -32, -60), 22); 
	world->set_material(phong_ptr10,sphere_ptr10);     						// dark green
	world->grid.add_object(sphere_ptr10);


	Material* phong_ptr11 = new Material;
	phong_ptr11->set_ka(ka);	
	phong_ptr11->set_kd(kd);
	phong_ptr11->set_ks(ks);
	phong_ptr11->set_exp(exp);
	phong_ptr11->set_cd(darkYellow);

	GeometricObject*	sphere_ptr11 = new GeometricObject(make_float3(-35, -37, -80), 22); 
	world->set_material(phong_ptr11,sphere_ptr11);							// dark yellow
	world->grid.add_object(sphere_ptr11);


	Material* phong_ptr12 = new Material;
	phong_ptr12->set_ka(ka);	
	phong_ptr12->set_kd(kd);
	phong_ptr12->set_ks(ks);
	phong_ptr12->set_exp(exp);
	phong_ptr12->set_cd(darkYellow);

	GeometricObject*	sphere_ptr12 = new GeometricObject(make_float3(10, 43, -80), 22); 
	world->set_material(phong_ptr12,sphere_ptr12);							// dark yellow
	world->grid.add_object(sphere_ptr12);


	Material* phong_ptr13 = new Material;
	phong_ptr13->set_ka(ka);	
	phong_ptr13->set_kd(kd);
	phong_ptr13->set_ks(ks);
	phong_ptr13->set_exp(exp);
	phong_ptr13->set_cd(darkYellow);

	GeometricObject*	sphere_ptr13 = new GeometricObject(make_float3(30, -7, -80), 10); 
	world->set_material(phong_ptr13,sphere_ptr13);
	world->grid.add_object(sphere_ptr13);											// dark yellow (hidden)


	Material* phong_ptr14 = new Material;
	phong_ptr14->set_ka(ka);	
	phong_ptr14->set_kd(kd);
	phong_ptr14->set_ks(ks);
	phong_ptr14->set_exp(exp);
	phong_ptr14->set_cd(darkGreen);

	GeometricObject*	sphere_ptr14 = new GeometricObject(make_float3(-40, 48, -110), 18); 
	world->set_material(phong_ptr14,sphere_ptr14); 							// dark green
	world->grid.add_object(sphere_ptr14);


	Material* phong_ptr15 = new Material;
	phong_ptr15->set_ka(ka);	
	phong_ptr15->set_kd(kd);
	phong_ptr15->set_ks(ks);
	phong_ptr15->set_exp(exp);
	phong_ptr15->set_cd(brown);

	GeometricObject*	sphere_ptr15 = new GeometricObject(make_float3(-10, 53, -120), 18); 
	world->set_material(phong_ptr15,sphere_ptr15); 							// brown
	world->grid.add_object(sphere_ptr15);


	Material* phong_ptr16 = new Material;
	phong_ptr16->set_ka(ka);	
	phong_ptr16->set_kd(kd);
	phong_ptr16->set_ks(ks);
	phong_ptr16->set_exp(exp);
	phong_ptr16->set_cd(lightPurple);

	GeometricObject*	sphere_ptr16 = new GeometricObject(make_float3(-55, -52, -100), 10); 
	world->set_material(phong_ptr16,sphere_ptr16);							// light purple
	world->grid.add_object(sphere_ptr16);


	Material* phong_ptr17 = new Material;
	phong_ptr17->set_ka(ka);	
	phong_ptr17->set_kd(kd);
	phong_ptr17->set_ks(ks);
	phong_ptr17->set_exp(exp);
	phong_ptr17->set_cd(brown);

	GeometricObject*	sphere_ptr17 = new GeometricObject(make_float3(5, -52, -100), 15); 		
	world->set_material(phong_ptr17,sphere_ptr17);							// browm
	world->grid.add_object(sphere_ptr17);


	Material* phong_ptr18 = new Material;
	phong_ptr18->set_ka(ka);	
	phong_ptr18->set_kd(kd);
	phong_ptr18->set_ks(ks);
	phong_ptr18->set_exp(exp);
	phong_ptr18->set_cd(darkPurple);

	GeometricObject*	sphere_ptr18 = new GeometricObject(make_float3(-20, -57, -120), 15); 
	world->set_material(phong_ptr18,sphere_ptr18);							// dark purple
	world->grid.add_object(sphere_ptr18);


	Material* phong_ptr19 = new Material;
	phong_ptr19->set_ka(ka);	
	phong_ptr19->set_kd(kd);
	phong_ptr19->set_ks(ks);
	phong_ptr19->set_exp(exp);
	phong_ptr19->set_cd(darkGreen);

	GeometricObject*	sphere_ptr19 = new GeometricObject(make_float3(55, -27, -100), 17); 
	world->set_material(phong_ptr19,sphere_ptr19);							// dark green
	world->grid.add_object(sphere_ptr19);


	Material* phong_ptr20 = new Material;
	phong_ptr20->set_ka(ka);	
	phong_ptr20->set_kd(kd);
	phong_ptr20->set_ks(ks);
	phong_ptr20->set_exp(exp);
	phong_ptr20->set_cd(brown);

	GeometricObject*	sphere_ptr20 = new GeometricObject(make_float3(50, -47, -120), 15); 
	world->set_material(phong_ptr20,sphere_ptr20);							// browm
	world->grid.add_object(sphere_ptr20);


	Material* phong_ptr21 = new Material;
	phong_ptr21->set_ka(ka);	
	phong_ptr21->set_kd(kd);
	phong_ptr21->set_ks(ks);
	phong_ptr21->set_exp(exp);
	phong_ptr21->set_cd(lightPurple);

	GeometricObject*	sphere_ptr21 = new GeometricObject(make_float3(70, -42, -150), 10); 
	world->set_material(phong_ptr21,sphere_ptr21);							// light purple
	world->grid.add_object(sphere_ptr21);


	Material* phong_ptr22 = new Material;
	phong_ptr22->set_ka(ka);	
	phong_ptr22->set_kd(kd);
	phong_ptr22->set_ks(ks);
	phong_ptr22->set_exp(exp);
	phong_ptr22->set_cd(lightPurple);

	GeometricObject*	sphere_ptr22 = new GeometricObject(make_float3(5, 73, -130), 12); 
	world->set_material(phong_ptr22,sphere_ptr22);							// light purple
	world->grid.add_object(sphere_ptr22);


	Material* phong_ptr23 = new Material;
	phong_ptr23->set_ka(ka);	
	phong_ptr23->set_kd(kd);
	phong_ptr23->set_ks(ks);
	phong_ptr23->set_exp(exp);
	phong_ptr23->set_cd(darkPurple);

	GeometricObject*	sphere_ptr23 = new GeometricObject(make_float3(66, 21, -130), 13); 			
	world->set_material(phong_ptr23,sphere_ptr23);							// dark purple
	world->grid.add_object(sphere_ptr23);	


	Material* phong_ptr24 = new Material;
	phong_ptr24->set_ka(ka);	
	phong_ptr24->set_kd(kd);
	phong_ptr24->set_ks(ks);
	phong_ptr24->set_exp(exp);
	phong_ptr24->set_cd(lightPurple);

	GeometricObject*	sphere_ptr24 = new GeometricObject(make_float3(72, -12, -140), 12); 
	world->set_material(phong_ptr24,sphere_ptr24);							// light purple
	world->grid.add_object(sphere_ptr24);


	Material* phong_ptr25 = new Material;
	phong_ptr25->set_ka(ka);	
	phong_ptr25->set_kd(kd);
	phong_ptr25->set_ks(ks);
	phong_ptr25->set_exp(exp);
	phong_ptr25->set_cd(green);

	GeometricObject*	sphere_ptr25 = new GeometricObject(make_float3(64, 5, -160), 11); 			
	world->set_material(phong_ptr25,sphere_ptr25);					 		// green
	world->grid.add_object(sphere_ptr25);


	Material* phong_ptr26 = new Material;
	phong_ptr26->set_ka(ka);	
	phong_ptr26->set_kd(kd);
	phong_ptr26->set_ks(ks);
	phong_ptr26->set_exp(exp);
	phong_ptr26->set_cd(lightPurple);

	GeometricObject*	sphere_ptr26 = new GeometricObject(make_float3(55, 38, -160), 12); 		
	world->set_material(phong_ptr26,sphere_ptr26);							// light purple
	world->grid.add_object(sphere_ptr26);


	Material* phong_ptr27 = new Material;
	phong_ptr27->set_ka(ka);	
	phong_ptr27->set_kd(kd);
	phong_ptr27->set_ks(ks);
	phong_ptr27->set_exp(exp);
	phong_ptr27->set_cd(lightPurple);

	GeometricObject*	sphere_ptr27 = new GeometricObject(make_float3(-73, -2, -160), 12); 		
	world->set_material(phong_ptr27,sphere_ptr27);							// light purple
	world->grid.add_object(sphere_ptr27);


	Material* phong_ptr28 = new Material;
	phong_ptr28->set_ka(ka);	
	phong_ptr28->set_kd(kd);
	phong_ptr28->set_ks(ks);
	phong_ptr28->set_exp(exp);
	phong_ptr28->set_cd(darkPurple);

	GeometricObject*	sphere_ptr28 = new GeometricObject(make_float3(30, -62, -140), 15); 
	world->set_material(phong_ptr28,sphere_ptr28); 							// dark purple
	world->grid.add_object(sphere_ptr28);



	Material* phong_ptr29 = new Material;
	phong_ptr29->set_ka(ka);	
	phong_ptr29->set_kd(kd);
	phong_ptr29->set_ks(ks);
	phong_ptr29->set_exp(exp);
	phong_ptr29->set_cd(darkPurple);

	GeometricObject*	sphere_ptr29 = new GeometricObject(make_float3(25, 63, -140), 15); 
	world->set_material(phong_ptr29,sphere_ptr29);							// dark purple
	world->grid.add_object(sphere_ptr29);


	Material* phong_ptr30 = new Material;
	phong_ptr30->set_ka(ka);	
	phong_ptr30->set_kd(kd);
	phong_ptr30->set_ks(ks);
	phong_ptr30->set_exp(exp);
	phong_ptr30->set_cd(darkPurple);

	GeometricObject*	sphere_ptr30 = new GeometricObject(make_float3(-60, 46, -140), 15);  
	world->set_material(phong_ptr30,sphere_ptr30); 							// dark purple
	world->grid.add_object(sphere_ptr30);


	Material* phong_ptr31 = new Material;
	phong_ptr31->set_ka(ka);	
	phong_ptr31->set_kd(kd);
	phong_ptr31->set_ks(ks);
	phong_ptr31->set_exp(exp);
	phong_ptr31->set_cd(lightPurple);

	GeometricObject*	sphere_ptr31 = new GeometricObject(make_float3(-30, 68, -130), 12); 
	world->set_material(phong_ptr31,sphere_ptr31); 							// light purple
	world->grid.add_object(sphere_ptr31);


	Material* phong_ptr32 = new Material;
	phong_ptr32->set_ka(ka);	
	phong_ptr32->set_kd(kd);
	phong_ptr32->set_ks(ks);
	phong_ptr32->set_exp(exp);
	phong_ptr32->set_cd(green);

	GeometricObject*	sphere_ptr32 = new GeometricObject(make_float3(58, 56, -180), 11);   
	world->set_material(phong_ptr32,sphere_ptr32);							//  green
	world->grid.add_object(sphere_ptr32);


	Material* phong_ptr33 = new Material;
	phong_ptr33->set_ka(ka);	
	phong_ptr33->set_kd(kd);
	phong_ptr33->set_ks(ks);
	phong_ptr33->set_exp(exp);
	phong_ptr33->set_cd(green);

	GeometricObject*	sphere_ptr33 = new GeometricObject(make_float3(-63, -39, -180), 11); 
	world->set_material(phong_ptr33,sphere_ptr33);							// green 
	world->grid.add_object(sphere_ptr33);


	Material* phong_ptr34 = new Material;
	phong_ptr34->set_ka(ka);	
	phong_ptr34->set_kd(kd);
	phong_ptr34->set_ks(ks);
	phong_ptr34->set_exp(exp);
	phong_ptr34->set_cd(lightPurple);

	GeometricObject*	sphere_ptr34 = new GeometricObject(make_float3(46, 68, -200), 10); 	
	world->set_material(phong_ptr34,sphere_ptr34);							// light purple
	world->grid.add_object(sphere_ptr34);


	Material* phong_ptr35 = new Material;
	phong_ptr35->set_ka(ka);	
	phong_ptr35->set_kd(kd);
	phong_ptr35->set_ks(ks);
	phong_ptr35->set_exp(exp);
	phong_ptr35->set_cd(lightPurple);

	GeometricObject*	sphere_ptr35 = new GeometricObject(make_float3(-3, -72, -130), 12); 
	world->set_material(phong_ptr35,sphere_ptr35);							// light purple
	world->grid.add_object(sphere_ptr35);
}