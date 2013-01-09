#pragma once

//these represent the block dimensions for the CUDA ray tracer renderer below.  
//NOTE: change to 16x8 on Mac...16x16 only works on Resonance...
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 8

//main GPU ray tracer starting kernel. 
__global__ void gpu_render_scene(uchar4 *canvas,const World *world, uint srtns)
{
	//to alleviate some register pressure, I allocte shared memory per thread in a block (which is 128 max)
	//for things such as rays since it's is used most often..
	__shared__  ALIGN_8 Ray sharedRays[BLOCKDIM_X*BLOCKDIM_Y];
		
		//input block indicies
	const uint c = __umul24(blockIdx.x,blockDim.x) + threadIdx.x;
	const uint r = __umul24(blockIdx.y,blockDim.y) + threadIdx.y;
	
	if (c>=world->ropt.vp.hres || r>=world->ropt.vp.vres)
	{
		return;
	}
	
	const uint shrdId = __umul24(threadIdx.y,blockDim.x) + threadIdx.x;
	
	float2 pp;
	pp.x = pp.y = 0.0f;
	
	float3 pixel_color;
	pixel_color.x = pixel_color.y = pixel_color.z = 0.0f;
	
	Ray *ray = &sharedRays[shrdId];
	ray->init();
	ray->o = world->ropt.camera_ptr.eye;
	
	ShadeRec sr;
	sr.init();
	float s = __fdividef(1.0f,world->ropt.camera_ptr.zoom);
	float over_n = __fdividef(1.0f,srtns);
	
	for (uint p = 0; p < srtns; p++)			// up pixel
	{
		for (uint q = 0; q < srtns; q++) 	// across pixel
		{
			pp.x = s * (c - 0.5f * world->ropt.vp.hres + (q + 0.5f) * over_n); 
			pp.y = s * (r - 0.5f * world->ropt.vp.vres + (p + 0.5f) * over_n);
			ray->d = world->ropt.camera_ptr.get_direction(&pp);
			pixel_color += world->trace_ray(ray,&sr);
		}
	}	
	
	pixel_color *= __fdividef(1.0f,world->ropt.vp.num_samples);
	world->save_pixel(r, c, &pixel_color,canvas);
}

//look for a description of this below when we call it.  Basic idea is to reassign some 
//device pointers, since there is no way to access device member variables on the host (at least
//none that I could of come up with!)
__global__ void arrange_smt_pointers(World *world)
{
	//512 threads per bock
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	
	//max of num of objects in the world
	if (idx >= world->grid.num_cells)
	{
		return;
	}
	
	//each thread processes an individual object from the cell so we are
	//coalescing device memory
	GeometricObject *geoObj = &world->grid.cells_cached[idx];
	if (geoObj->type == GeometricObject::SmooMesTriSpec)
	{
		//ok we have a SMT, go ahead and do the necessary
		geoObj->smt.mesh_ptr = &world->grid.mesh_ptr;
	} else if (geoObj->type == GeometricObject::CompSpec)
	{
		//oh look, we are using grid acceleration and thus have a cell here with more than
		//one GeometricObject in it, we better iterate over those objects and do the
		//necessary too!!!
		for (int i=0;i<geoObj->cs.numObjects;i++)
		{
			GeometricObject *g2 = &geoObj->cs.objects[i];
			if (g2->type == GeometricObject::SmooMesTriSpec)
			{
				g2->smt.mesh_ptr = &world->grid.mesh_ptr;
			}
		}
	}
}

void World::launchKernel(uchar4 *h_canvas) 
{
	printf("Starting GPU ray tracing...\n");
	fflush(NULL);

	//****************** PRE KERNEL STUFF *************************
//	unsigned int timer_memory = 0;
//  CUT_SAFE_CALL(cutCreateTimer(&timer_memory));
//  CUT_SAFE_CALL(cutStartTimer(timer_memory));

	//first we need to package the world for GPU memory transfer
	World *d_world = NULL;
	//this is a big step!
	packGPU(&d_world); 
	
	//ok craete a canvas for us to draw into
	int numPixels = ropt.vp.hres * ropt.vp.vres;
	uchar4 *d_canvas;
	checkCudaErrors(cudaMalloc((void**)&d_canvas,numPixels*sizeof(uchar4)));
//	CUT_SAFE_CALL(cutStopTimer(timer_memory));
	//****************** KERNEL COMPUTATION STUFF *************************
	//ok time for the real fun.  We create one thread per pixel of output
	dim3 dimBlock(BLOCKDIM_X, BLOCKDIM_Y);
	dim3 dimGrid(Maths::iDivUp(ropt.vp.hres, BLOCKDIM_X), Maths::iDivUp(ropt.vp.vres, BLOCKDIM_Y));
	printf("  # of threads in a block: %d x %d (%d)\n",
	         dimBlock.x, dimBlock.y, dimBlock.x * dimBlock.y);
	  printf("  # of blocks in a grid  : %d x %d (%d)\n",
	         dimGrid.x, dimGrid.y, dimGrid.x * dimGrid.y);
	fflush(NULL);
	uint srtns = (uint)sqrtf(ropt.vp.num_samples);
//	unsigned int timer_compute = 0.0f;
	//start the gpu compute timer
//	CUT_SAFE_CALL(cutCreateTimer(&timer_compute));
//	CUT_SAFE_CALL(cutStartTimer(timer_compute));
	//here we go
	gpu_render_scene<<<dimGrid,dimBlock>>>(d_canvas,d_world,srtns);
	// Make sure that all threads have completed
	checkCudaErrors(cudaThreadSynchronize());
	// Stop the timer
	//we are done
//	CUT_SAFE_CALL(cutStopTimer(timer_compute));

	//****************** POST KERNEL STUFF *************************
//	CUT_SAFE_CALL(cutStartTimer(timer_memory));
	//copy the data back to the host
	checkCudaErrors(cudaMemcpy(h_canvas,d_canvas,numPixels*sizeof(uchar4),cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaFree(d_canvas));
//	CUT_SAFE_CALL(cutStopTimer(timer_memory));
	
	//ok we should do whatever clean that is necessary for the world
	unpackGPU(d_world);

	//timer stuff
//	ropt.gpuMemTime = cutGetTimerValue(timer_memory);
//	ropt.gpuCompTime = cutGetTimerValue(timer_compute);
//	CUT_SAFE_CALL(cutDeleteTimer(timer_memory));
//	CUT_SAFE_CALL(cutDeleteTimer(timer_compute));	

	//Show Timing
//	printf("  GPU memory access time: %f (ms)\n",ropt.gpuMemTime);
//	printf("  GPU computation time  : %f (ms)\n",ropt.gpuCompTime);
//	printf("  GPU processing time   : %f (ms)\n",ropt.gpuMemTime + ropt.gpuCompTime);
	fflush(NULL);
}

void World::packGPU(World **d_ws)
{
	//what packing entails is that we wrap up all of the world, grid, geometric objects, etc into their
	//individual compoentnts so that they can be processed by the gpu.  Key idea is to keep all that data
	//within the World memember variables such as grid, then trhough grid, mesh, cells, objects, etc...
	//first pack up the grid
	grid.packGPU();
	
	//now the lights
	//first keep track of what the current member variable is pointing to
	Light *temp_lights = lights_cached;
	//since we are reassigning it to something else
	checkCudaErrors(cudaMalloc((void**)&lights_cached,numLights*sizeof(Light)));
	checkCudaErrors(cudaMemcpy(lights_cached,temp_lights,numLights*sizeof(Light),cudaMemcpyHostToDevice));

	//and obviously the materials too
	Material *temp_materials = materials_cached;
	checkCudaErrors(cudaMalloc((void**)&materials_cached,numMaterials*sizeof(Material)));
	checkCudaErrors(cudaMemcpy(materials_cached,temp_materials,numMaterials*sizeof(Material),cudaMemcpyHostToDevice));

	//now we are done with the cpu version of that member variable so we can go unpack (destroy) it
	unpackCPU(temp_lights,temp_materials);
	
	//and finally us, the world
	//ok so this is the big thing.  What we are doing here is creating a device world.  which then we will
	//copy "this" pointer into.  Meaning everything we packGPU from above will have it all lined up.  Only trick
	//is that some of the pointers (e.g. SMT) will still be pointing to host memory, we deal with that later
	//in arrange_smt_pointers below.  Also it should be clearly noted here that the "this" pointer on the Host
	//has some member variables set that pont to device memory.  This is fine since we will not be using the 
	//host world any further.  Plus it works good for us since we can then use this same class to free up the
	//device pointers since we store it right this!!!
	checkCudaErrors(cudaMalloc((void**)&(*d_ws),sizeof(World)));
	//notice how we are copying "this".  Again this encapsulates everything that the GPU needs to know
	checkCudaErrors(cudaMemcpy(*d_ws,this,sizeof(World),cudaMemcpyHostToDevice));
	//so to repeat, for example lights_cached on this pointer on the Host now points to device memory...
	//we can use it later to free it instantly since we kept track of it right this!!!
	
	//ok now rearrange necessary poitners like mesh in smt and Compounds..
	//so the deal here is that we have one Mesh object in Grid (we only allow one mesh per build/scene).
	//That Mesh has a buffer of vertices & normals.  There is a SmoothMeshTriangle GeometricObject which contains
	//a pointer to the Mesh it belongs to so as to share all that vertex and normal information.  That way
	//we avoid duplicating data over into each individual SMT, and instead we just keep an 3 integer indexes.
	//Well the pointer proves to be difficult to deal with because when we packagted the world above for GPU processing
	//the SMTs still point to the Host Mesh since we basicall copied "this" over.  Remember if it were not a pointer
	//and just a regular variable, than memcpy would have copied all the relevant informatin, but it is a pointer, 
	//which is what is tricky.  We need a way in order for us to get ALL the SMTs to point to the DEVICE Mesh member
	//variable of Grid.  I tried many different things and spent at least 2 days behind this.  My original logic
	//revolved around cudaGetSymbolAddress, cudaGetSymbolSize, cudaHostGetDevicePointer, cudaMemcpyFromSymbol,
	//cudaMemcpyToSymbol and the idea being that I wanted *on* the host to be able to set certain aspects of d_ws.
	//Ofcourse I wasn't thinking correctly since d_ws was actually a device pointer.  Hence my final solution was to
	//think outside the box and basically launch a special kernel that does the reassning of the pointers where it
	//has access to them.  
	//ok so only if we have SMTs do we need to do this
	if (grid.numSMTs != 0)
	{
		//we allocate max of 512 threads
		dim3 dimBlock(512, 1);
		//key thing is that we need a grid dimension that supports up to a potential of num_cells objects.
		//I'll explain later, though num_cells can mean total number of ojbects if grid acceleration is not used.
		//Or it can mean total number of Cells if Grid is used.  Each cell then can have a list of objects...
		//nayways that's too much info for now...
		dim3 dimGrid(Maths::iDivUp(grid.num_cells, 512), 1);
		//launch this special kerenl  Also, do realize that I am taking all this into account for th eGPU memory
		//timer since it has already been started..
		arrange_smt_pointers<<<dimGrid,dimBlock>>>(*d_ws);
		// Make sure that all threads have completed
		//we need to wait obviously before we go ahead and proceed or our main kerenel would have stale data...
		checkCudaErrors(cudaThreadSynchronize());
	}
}

void World::unpackGPU(World *dws)
{
	//free up the lights
	checkCudaErrors(cudaFree(lights_cached));
	//and the materials
	checkCudaErrors(cudaFree(materials_cached));
	//and the grid...
	grid.unpackGPU();
	//and finally the world iteslef
	checkCudaErrors(cudaFree(dws));
}

void Grid::packGPU()
{
	//ok so first pack the Mesh vertex and normal buffers
	mesh_ptr.packGPU();
	
	//ok now look for any compound objects (e.g. cells with a list of ojbects), and pack em
	for (int i=0;i<num_cells;i++)
	{
		GeometricObject *geoObj = &cells_cached[i];
		if (geoObj->type == GeometricObject::CompSpec)
		{
			//so we are calling pack on the CompoundSpec member variable of GeometricObject.
			geoObj->cs.packGPU();
		}
	}
	
	//ok fun fun, now we are going to allocate device memory for all our cells...
	//notice that we are oblivious to SMTs and their Mesh pointer.  At this point, they device SMTs
	//still point to the Host Mesh.  bad, but we clean up as we explained earlier...
	temp_cells_cached = cells_cached;
	checkCudaErrors(cudaMalloc((void**)&cells_cached,num_cells*sizeof(GeometricObject)));
	checkCudaErrors(cudaMemcpy(cells_cached,temp_cells_cached,num_cells*sizeof(GeometricObject),cudaMemcpyHostToDevice));
	
	//ok so we aren't going to cealn up temp_cells_cached here as we ought to to clear up th eCPU.  the reason
	//being is that we need later in unpackGPU a reference to all the cells so that we can iterate over them
	//to find the CompSpec and call unpackGPU on those...
	
}

void Grid::unpackGPU()
{
	//again remember that this has certain data members like cells_cached that point to device
	//memory..
	checkCudaErrors(cudaFree(cells_cached));

	//ok we need to find all CompSpecs so that we can accordingly call unpackGPU on it...
	for (int i=0;i<num_cells;i++)
	{
		GeometricObject *geoObj = &temp_cells_cached[i];
		if (geoObj->type == GeometricObject::CompSpec)
		{
			geoObj->cs.unpackGPU();
		}
	}

	//cleanup CPU stuff since we are done with it that we didn't do earlier..no biggie
	unpackCPU(temp_cells_cached);
	
	mesh_ptr.unpackGPU();
}

void Mesh::packGPU()
{

	//smae logic, pack the vertices...
	float3 *temp_vertices = vertices_cached;
	checkCudaErrors(cudaMalloc((void**)&vertices_cached,num_vertices*sizeof(float3)));
	checkCudaErrors(cudaMemcpy(vertices_cached,temp_vertices,num_vertices*sizeof(float3),cudaMemcpyHostToDevice));

	//and the normals...
	float3 *temp_normals = normals_cached;
	checkCudaErrors(cudaMalloc((void**)&normals_cached,num_vertices*sizeof(float3)));
	checkCudaErrors(cudaMemcpy(normals_cached,temp_normals,num_vertices*sizeof(float3),cudaMemcpyHostToDevice));

	//clean up CPU
	unpackCPU(temp_vertices,temp_normals);
}

void Mesh::unpackGPU()
{
	checkCudaErrors(cudaFree(vertices_cached));
	checkCudaErrors(cudaFree(normals_cached));
}

void CompoundSpec::packGPU()
{
	//so for this cell, pack up all it's objects!
	GeometricObject *temp_objects = objects;
	checkCudaErrors(cudaMalloc((void**)&objects,numObjects*sizeof(GeometricObject)));
	checkCudaErrors(cudaMemcpy(objects,temp_objects,numObjects*sizeof(GeometricObject),cudaMemcpyHostToDevice));
	//cleanup CPU
	unpackCPU(temp_objects);
}

void CompoundSpec::unpackGPU()
{
	checkCudaErrors(cudaFree(objects));
}




