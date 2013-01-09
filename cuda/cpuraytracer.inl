#pragma once

void World::cpu_render_scene(uchar4 *canvas)  
{		
	printf("Starting CPU ray tracing...\n");
	fflush(NULL);
	//unsigned int cpu_timer = 0.0f;
	//CUT_SAFE_CALL(cutCreateTimer(&cpu_timer));
	//CUT_SAFE_CALL(cutStartTimer(cpu_timer));

	ShadeRec sr;
	sr.init();
	float3	pixel_color = make_float3(0.0f);
	float2 	pp = make_float2(0.0f);		// sample point on a pixel
	Ray			ray;
	ray.init();
	
	float s = 1.0f/ropt.camera_ptr.zoom;
	int n = (int)sqrtf(ropt.vp.num_samples);
	
	ray.o = ropt.camera_ptr.eye;
	
	for (int r = 0; r < ropt.vp.vres; r++)			
	{
		for (int c = 0; c < ropt.vp.hres; c++) 
		{		
			pixel_color = make_float3(0.0f); 
	
			for (int p = 0; p < n; p++)			
			{
				for (int q = 0; q < n; q++) 
				{	
					pp.x = s * (c - 0.5f * ropt.vp.hres + (q + 0.5f) * 1.0f/n); 
					pp.y = s * (r - 0.5f * ropt.vp.vres + (p + 0.5f) * 1.0f/n);
					ray.d = ropt.camera_ptr.get_direction(&pp);
					pixel_color += trace_ray(&ray, &sr);
				}
			}
			pixel_color *= 1.0f/ropt.vp.num_samples;
			save_pixel(r, c, &pixel_color,canvas);
		}
	}
	//CUT_SAFE_CALL(cutStopTimer(cpu_timer));  

	//ropt.cpuTime = cutGetTimerValue(cpu_timer);
	//CUT_SAFE_CALL(cutDeleteTimer(cpu_timer));  
	//printf("  CPU processing time   : %f (ms)\n",ropt.cpuTime);
	fflush(NULL);
	
}

void World::packCPU()
{
	printf("Reorganizing Data Structures...\n");
	usePinnedLights = ropt.usePinned;	
	usePinnedMaterials = ropt.usePinned;

	if (!ropt.usePinned)
	{
		printf("  Not going to use host pinned memory...\n");
	} else
	{
		printf("  Using pinned memory...\n");
	}
	
	//first pack the special materials
	packMaterials();
	
	//create the static light buffer
	numLights = sg_lights.size();
	if (!usePinnedLights || (usePinnedLights && cudaMallocHost((void**)&lights_cached,numLights*sizeof(Light)) != cudaSuccess))
	{
		//either we requested not to use pinned memory or we requested to use it and our request was denided
		//so fall back to regular heap memory
		lights_cached = (Light*)malloc(numLights*sizeof(Light));
		usePinnedLights = false;
	} 
	
	//copy the elements over...
	int j = 0;
	for(TLightsIter iter = sg_lights.begin(); 
	    iter != sg_lights.end();
	    ++iter,++j)
	{
		lights_cached[j] = **iter;
	}
	
	//time to package the grid
	grid.packCPU(&ropt);

	printf("Finished Consolidation.\n");
	
	int totalBytes = 
	grid.count_total_geos*sizeof(GeometricObject) +
	2*grid.mesh_ptr.num_vertices*sizeof(float3) + sizeof(World);
	
	
	printf("World Stats: \n");
	printf("  Total # of GeometricObjects: %i\n",grid.count_total_geos);
	printf("  Approximate bytes of data: %i bytes\n",totalBytes);
	if (grid.mesh_ptr.num_triangles)
		printf("  # of triangles in mesh: %i\n", grid.mesh_ptr.num_triangles);
	fflush(NULL);

	//what we do here is release all the memory allocated for the STL vector since
	//we have prepped a copy for ourselves for both CPU & GPU usage.  This is ok since
	//our app only runs once through the world (GPU & CPU)
	delete_vectors();	
}

void World::packMaterials()
{
	//ok so here's the deal with materials.  To keep the amount of data that needs to be stored in each
	//GeometricObject as low as possible, we keep a pointer to a material in GeoObj.  Well actually it's not
	//a pointer, but an uint.  Ok so this is what happens.  A material is created, and added to the world.  A Geo
	//says they want to use that material.  in GeoObj::set_material, we cast the material pointer to an uint
	//called mat_ptr_id.  Calling get_material basically casts that back to a Material*.  The reason we store 
	//a uint as oppose to an actual Material* is that on the GPU we rather use the mat_ptr_id as an index
	//into an array of materials_cached (which is the purpose of this function).  This avoids dealing with actual
	//pointers on the device which is really useful since we dont' have to reassign device pointers and such as
	//we do have to do with SMTs (see gpuraytracer.inl).  Actually now that I think about it, I could have handled
	//SMTs similarily..but since we only support one Mesh, it's no biggie there...
	//I must iterate that this is a huge saving since potentially meshes & other geos can share materials which
	//obviusly is very common.  I was originally storing a copy of the material in each geo which was resulting
	//in sizeof(GeometricObject) to be 240 bytes, and now it's down to 128 bytes!  GeometricObject is the biggest
	//factor in the required memory bandwidth for our applicaiton since it stores all the objects in the world...
	
	//ok so iterate over all the materials and over all objects and reassign material indices...
	//this is the mat_idx which is basically the index into materials_cached that we will form later...
	int mat_idx = 0;
	for(TMaterialsIter iter = sg_materials.begin(); 
	    iter != sg_materials.end();
	    ++iter,++mat_idx)
	{
		Material *material = *iter;
		
		//iterate over all objects in the world...
		for(TGeometricObjectsIter iter2 = sg_objects.begin(); 
		    iter2 != sg_objects.end();
		    ++iter2)
		{
			GeometricObject *geoObj = *iter2;
			//and for each object matching the current material
			if (geoObj->get_material() == material)
			{
				//ok so right here, we are assinging an actual index number for the 
				//the future material as it will be stored in materials_cached..cool heh?
				geoObj->set_material(mat_idx);
			}
		}
	}
	
	//ok time for the materials_cached array to be created.  remember it has to be lined up exactly
	//as the above mat_idx or the objects would be referencing wrong elements
	//create the static light buffer
	numMaterials = sg_materials.size();
	if (!usePinnedMaterials || (usePinnedMaterials && cudaMallocHost((void**)&materials_cached,numMaterials*sizeof(Material)) != cudaSuccess))
	{
		//either we requested not to use pinned memory or we requested to use it and our request was denided
		//so fall back to regular heap memory
		materials_cached = (Material*)malloc(numMaterials*sizeof(Material));
		usePinnedMaterials = false;
	}
	
	//copy the elements over...
	int j = 0;
	for(TMaterialsIter iter = sg_materials.begin(); 
	    iter != sg_materials.end();
	    ++iter,++j)
	{
		materials_cached[j] = **iter;
	}
}

void World::unpackCPU(Light *temp_lights, Material *temp_materials)
{
	//we need to ensure that we free the host memory correctly depending on how it 
	//was allocated
	if (usePinnedLights)
	{
		checkCudaErrors(cudaFreeHost(temp_lights));
	} else
	{
		SAFE_FREE(temp_lights);
	}

	if (usePinnedMaterials)
	{
		checkCudaErrors(cudaFreeHost(temp_materials));
	} else
	{
		SAFE_FREE(temp_materials);
	}
}

void Grid::packCPU(const RenderOpt *r)
{
	//make a copy
	ropt = *r;
	usePinned = ropt.usePinned;	
	
	//first pack up the mesh
	mesh_ptr.packCPU(ropt.usePinned);
	
	TGeometricObjects *the_objects = NULL;
	
	if (ropt.useGrid)
	{
		printf("  Using Grid acceleration...\n");
		//first setup the sg_cells!!
		setup_cells();
		num_cells = sg_cells.size();
		the_objects = &sg_cells;
	} else
	{
		printf("  Not using Grid accelration...\n");
		//we are just taking the sg_objects in the world and putting them inside the cells_cached array
		//it really is just a container and will not be utilized like a real cell 
		num_cells = sg_objects.size();
		the_objects = &sg_objects;
	}
	
	fflush(NULL);
	
	//start the count o
	count_total_geos = num_cells;
	if (!usePinned || (usePinned && cudaMallocHost((void**)&cells_cached,num_cells*sizeof(GeometricObject)) != cudaSuccess))
	{
		cells_cached = (GeometricObject*)malloc(num_cells*sizeof(GeometricObject));
		//make sure you notify that we didn't use pinned so that we can unpack accordingly
		usePinned = false;
	} 
	
	//package up the cells/list of objects
	int j=0;
	for(TGeometricObjectsIter iter = the_objects->begin(); iter != the_objects->end();++iter,++j)
	{
		GeometricObject *srcObj = *iter;
		GeometricObject *destObj = &cells_cached[j];
		if (!srcObj)
		{
			//with grid acceleation, there will be empty cells, we need to account for them since it is actually allocated.
			//we do so by just marking them as undefined
			destObj->type = GeometricObject::Undefined;
			continue;
		}
		
		//and the big line...copies over the GeometricObject data (not a deep-copy cause obviously we wouldn't care for pointers)
		//SMTs have a mesh_ptr that is handled specially...look for it in the GPU code
		*destObj = *srcObj;
		
		if (ropt.useGrid)
		{
			if (srcObj->type == GeometricObject::CompSpec)
			{
				//ok so we have a cell here that has more than one object in it, we need to create a CompoundSpec
				//which basially acts as an array of GeoObjs...
				Compound* compound_ptr = static_cast<Compound*>(srcObj);
				//update the count of geos to account for the objects in this cell...
				count_total_geos += compound_ptr->objects->size();
				compound_ptr->packCPU(&destObj->cs,ropt.usePinned);
			} 
		}
	}
	
	delete_vectors();
}

void Grid::unpackCPU(GeometricObject *temp_cells)
{
	
	if (usePinned)
	{
		checkCudaErrors(cudaFreeHost(temp_cells));
	} else
	{
		SAFE_FREE(temp_cells);
	}
}

void Mesh::packCPU(bool pinned)
{
	pinnedVertices = pinned;
	if (!pinnedVertices || (pinnedVertices && cudaMallocHost((void**)&vertices_cached,num_vertices*sizeof(float3)) != cudaSuccess))
	{
		vertices_cached = (float3*)malloc(num_vertices*sizeof(float3));
		//make sure you notify that we didn't use pinned so that we can unpack accordingly
		pinnedVertices = false;
	} 

	int j=0;
	for(TFloat3sIter iter = sg_mesh_vertices.begin(); 
			iter != sg_mesh_vertices.end();
			++iter,++j)
	{
		vertices_cached[j] = *iter;
	}

	//yes its suppose to be num_vertices don't worry!
	pinnedNormals = pinned;
	if (!pinnedNormals || (pinnedNormals && cudaMallocHost((void**)&normals_cached,num_vertices*sizeof(float3)) != cudaSuccess))
	{
		normals_cached = (float3*)malloc(num_vertices*sizeof(float3));
		//make sure you notify that we didn't use pinned so that we can unpack accordingly
		pinnedNormals = false;
	} 
	
	j=0;
	for(TFloat3sIter iter = sg_mesh_normals.begin(); 
			iter != sg_mesh_normals.end();
			++iter,++j)
	{
		normals_cached[j] = *iter;
	}
	
	delete_vectors();
}
void Mesh::unpackCPU(float3 *temp_vertices, float3 *temp_normals)
{
	if (pinnedVertices)
	{
		checkCudaErrors(cudaFreeHost(temp_vertices));
	} else
	{
		SAFE_FREE(temp_vertices);
	}
	
	if (pinnedNormals)
	{
		checkCudaErrors(cudaFreeHost(temp_normals));
	} else
	{
		SAFE_FREE(temp_normals);
	}
	
}

void Compound::packCPU(CompoundSpec *cs,bool pinned)
{
	cs->numObjects = objects->size();
	cs->usePinned = pinned;
	
	//first always try pinned memory
	if (!cs->usePinned || (cs->usePinned && cudaMallocHost((void**)&cs->objects,cs->numObjects*sizeof(GeometricObject)) != cudaSuccess))
	{
		//if not, then resort to regular heap paged locked memory
		cs->objects = (GeometricObject*)malloc(cs->numObjects*sizeof(GeometricObject));
		//make sure you notify that we didn't use pinned so that we can unpack accordingly
		cs->usePinned = false;
	} 
	
	for (int j = 0; j < cs->numObjects; j++) 
	{
		GeometricObject *srcObj = (*objects)[j];
		cs->objects[j] = *srcObj;
	}	
}

void CompoundSpec::unpackCPU(GeometricObject *temp_objects)
{
	if (usePinned)
	{
		checkCudaErrors(cudaFreeHost(temp_objects));
	} else
	{
		SAFE_FREE(temp_objects);
	}
}

