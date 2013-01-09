#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// This file contains the definition of the Grid class

extern TFloat3s 		sg_mesh_vertices;				// mesh vertices 
extern TFloat3s 		 sg_mesh_normals;				// average normal at each vertex;
extern TIntsofInts 	sg_mesh_vertex_faces;			// the triangles shared by each vertex

TGeometricObjects	sg_cells;			// grid of cells
TGeometricObjects	sg_objects;		

#include "ply.h"

// ----------------------------------------------------------------  default constructor

Grid::Grid(void)	
	: 
		nx(0),
		ny(0),
		nz(0),
		reverse_normal(false),
		cells_cached(NULL),
		num_cells(0),
		usePinned(false),
		numSMTs(0),
		temp_cells_cached(NULL),
		count_total_geos(0)
{
}

Grid::~Grid(void) 
{	
	delete_vectors();	
}

void Grid::add_object(GeometricObject* object_ptr) {
	sg_objects.push_back(object_ptr);	
}


void Grid::delete_vectors(void) 
{
	for(TGeometricObjectsIter iter = sg_objects.begin(); 
	    iter != sg_objects.end();
	    ++iter)
	{
		SAFE_DELETE(*iter);
	}
	sg_objects.clear();
}

// 
//------------------------------------------------------------------ find_min_bounds

// find the minimum grid coordinates, based on the bounding boxes of all the sg_objects

float3 
Grid::find_min_bounds(void) {
	float3 p0(make_float3(FLT_MAX));

	for(TGeometricObjectsIter iter = sg_objects.begin(); 
	    iter != sg_objects.end();
	    ++iter)
	{
		GeometricObject *curr_obj = (*iter);
		const BBox &object_box = curr_obj->get_bounding_box();
				
		if (object_box.x0 < p0.x)
			p0.x = object_box.x0;
		if (object_box.y0 < p0.y)
			p0.y = object_box.y0;
		if (object_box.z0 < p0.z)
			p0.z = object_box.z0;
	}
	
	p0 = p0 - GRID_BBOX_DELTA;
		
	return (p0);
}


//------------------------------------------------------------------ find_max_bounds

// find the maximum grid coordinates, based on the bounding boxes of the sg_objects

float3 
Grid::find_max_bounds(void) {
	BBox object_box;
	float3 p1(make_float3(-FLT_MAX));

	for(TGeometricObjectsIter iter = sg_objects.begin(); 
	    iter != sg_objects.end();
	    ++iter)
	{
		GeometricObject *curr_obj = (*iter);
		const BBox &object_box = curr_obj->get_bounding_box();
				
		if (object_box.x1 > p1.x)
			p1.x = object_box.x1;
		if (object_box.y1 > p1.y)
			p1.y = object_box.y1;
		if (object_box.z1 > p1.z)
			p1.z = object_box.z1;
	}
	
	p1 = p1 + GRID_BBOX_DELTA;
		
	return (p1);
}

//------------------------------------------------------------------ setup_cells

//Pre: we never allow of adding Grids into sg_cells.
void Grid::setup_cells(void) {
	printf("    Organizing grid cells...\n");
	// find the minimum and maximum coordinates of the grid
	
	float3 p0 = find_min_bounds();
	float3 p1 = find_max_bounds();
	
	bbox.x0 = p0.x;
	bbox.y0 = p0.y;
	bbox.z0 = p0.z;
	bbox.x1 = p1.x;
	bbox.y1 = p1.y;
	bbox.z1 = p1.z;
		
	// compute the number of grid cells in the x, y, and z directions
	
	int num_objects = sg_objects.size();
	
	// dimensions of the grid in the x, y, and z directions
	
	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;  
	
	float multiplier = 1.0f;  	// multiplyer scales the number of grid cells relative to the number of sg_objects
								
	float s = powf(wx * wy * wz * 1.0f/num_objects, 0.3333333f);    
	float s_over = 1.0f/s;
	nx = multiplier * wx * s_over + 1;
	ny = multiplier * wy * s_over + 1;
	nz = multiplier * wz * s_over + 1;

	// set up the array of grid cells with null pointers
	
	sg_cells.reserve(num_objects);
	
	int num_cells = nx * ny * nz;	
	for (int j = 0; j < num_cells; j++)
		sg_cells.push_back(NULL);
		
				
	// set up a temporary array to hold the number of sg_objects stored in each cell
	
	vector<int> counts;
	counts.reserve(num_cells);
		
	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);
		

	// put the sg_objects into the sg_cells
	 
	int index;  	// cell's array index

	for(TGeometricObjectsIter iter = sg_objects.begin(); iter != sg_objects.end(); ++iter)
	{
		GeometricObject *curr_obj = (*iter);
		const BBox &obj_bBox =  curr_obj->get_bounding_box();
				
		// compute the cell indices at the corners of the bounding box of the object
		int ixmin = Maths::clamp(((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x)), 0.0f, (nx - 1));
		int iymin = Maths::clamp(((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y)), 0.0f, (ny - 1));
		int izmin = Maths::clamp(((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z)), 0.0f, (nz - 1));
		int ixmax = Maths::clamp(((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x)), 0.0f, (nx - 1));
		int iymax = Maths::clamp(((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y)), 0.0f, (ny - 1));
		int izmax = Maths::clamp(((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z)), 0.0f, (nz - 1));
				
		// add the object to the sg_cells
				
		for (int iz = izmin; iz <= izmax; iz++) 					// sg_cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// sg_cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) 
				{			// cells in x direction
					index = ix + nx * iy + nx * ny * iz;
															
					if (counts[index] == 0) 
					{
						sg_cells[index] = curr_obj;
						counts[index] += 1;  						// now = 1
					}
					else 
					{
						if (counts[index] == 1) 
						{
							// pay very careful attention here! we are adding raw Compounds here
							//Later we will need in packCPU to convert these to native GeometricObject with type
							//CompSpec
							Compound* compound_ptr = new Compound;	// construct a compound object
							compound_ptr->add_object(sg_cells[index]); // add object already in cell
							compound_ptr->add_object(curr_obj);  	// add the new object
							sg_cells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}						
						else 
						{										// counts[index] > 1
							Compound* compound_ptr = dynamic_cast<Compound*>(sg_cells[index]);
							if (compound_ptr)
							{
								compound_ptr->add_object(curr_obj);	// just add current object
								counts[index] += 1;						// for statistics only
							} else
							{
								printf("Error - Grid::setup_cells: expecting a Compound* object, but not recevied!\n");
							}
						}
					}
				}	
	}  
	
	// display some statistics on counts
	// this is useful for finding out how many sg_cells have no sg_objects, one object, etc
	// comment this out if you don't want to use it
	
		int num_zeroes 	= 0;
		int num_ones 	= 0;
		int num_twos 	= 0;
		int num_threes 	= 0;
		int num_greater = 0;
	
		for (int j = 0; j < num_cells; j++) {
			if (counts[j] == 0)
				num_zeroes += 1;
			if (counts[j] == 1)
				num_ones += 1;
			if (counts[j] == 2)
				num_twos += 1;
			if (counts[j] == 3)
				num_threes += 1;
			if (counts[j] > 3)
				num_greater += 1;
		}
	
		printf("      Stats: # of cells that have 0 objects, 1 object, 2 objects, etc...\n");
		printf("      num_cells = %i\n",num_cells);
		printf("      numZeroes = %i, numOnes = %i, numTwos = %i\n",num_zeroes,num_ones,num_twos);
		printf("      numThrees = %i numGreater = %i\n",num_threes, num_greater);

		printf("    Finished grid cell organization.\n");
			
	counts.erase (counts.begin(), counts.end());  
}



bool Grid::hit(const Ray *ray, ShadeRec *sr) const
{		 		
	bool hit = false;		
	if (ropt.useGrid)
	{
		hit = accel_hit(ray,sr);
	} else
	{
		hit = no_accel_hit(ray,sr);
	}
	
	if (hit)
	{
		sr->hit_point = ray->o + sr->the_t * ray->d;
		sr->normal = normalize(sr->normal);
	}
	
	return hit;
}

bool Grid::no_accel_hit(const Ray *ray, ShadeRec *sr) const
{
	float3		normal(make_float3(0.0f));
	bool		hit 		= false;
	float tmin 		= FLT_MAX;
	ulong material = ULONG_MAX;

	for (int j = 0; j < num_cells; j++)
	{
		GeometricObject *geoObj = &cells_cached[j];
		if (geoObj && geoObj->hit(ray, sr) && (sr->the_t < tmin)) 
		{
			hit				= true;
			tmin 			= sr->the_t;
			normal			= sr->normal;
			material = geoObj->get_material_idx();	
		}
	}

	if (hit) 
	{
		sr->the_t				= tmin;
		sr->normal 			= normal;
		sr->material = material;
	}

	return (hit);
}

// The following grid traversal code is based on the pseudo-code in Shirley (2000)	

bool Grid::accel_hit(const Ray *ray, ShadeRec *sr) const
{
	
	float ox = ray->o.x;
	float oy = ray->o.y;
	float oz = ray->o.z;
	float dx = ray->d.x;
	float dy = ray->d.y;
	float dz = ray->d.z;
	
	float x0 = bbox.x0;
	float y0 = bbox.y0;
	float z0 = bbox.z0;
	float x1 = bbox.x1;
	float y1 = bbox.y1;
	float z1 = bbox.z1;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max; 
	
	// the following code includes modifications from Shirley and Morley (2003)
	
	float a = 1.0f / dx;
	if (a >= 0.0f) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	float b = 1.0f / dy;
	if (b >= 0.0f) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	float c = 1.0f / dz;
	if (c >= 0.0f) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	float t0, t1;
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
		
	if (tz_min > t0)
		t0 = tz_min;
		
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
		
	if (tz_max < t1)
		t1 = tz_max;
			
	if (t0 > t1)
	{
		return(false);
	}
	
			
	// initial cell coordinates
	
	int ix, iy, iz;
	
	if (bbox.inside(&ray->o)) {  			// does the ray start inside the grid?
		ix = Maths::clamp(((ox - x0) * nx / (x1 - x0)), 0.0f, (nx - 1));
		iy = Maths::clamp(((oy - y0) * ny / (y1 - y0)), 0.0f, (ny - 1));
		iz = Maths::clamp(((oz - z0) * nz / (z1 - z0)), 0.0f, (nz - 1));
	}
	else {
		float3 p = ray->o + t0 * ray->d;  // initial hit point with grid's bounding box
		ix = Maths::clamp(((p.x - x0) * nx / (x1 - x0)), 0.0f, (nx - 1));
		iy = Maths::clamp(((p.y - y0) * ny / (y1 - y0)), 0.0f, (ny - 1));
		iz = Maths::clamp(((p.z - z0) * nz / (z1 - z0)), 0.0f, (nz - 1));
	}
	
	// ray parameter increments per cell in the x, y, and z directions
	
	float dtx = (tx_max - tx_min) * 1.0f/nx;
	float dty = (ty_max - ty_min) * 1.0f/ny;
	float dtz = (tz_max - tz_min) * 1.0f/nz;
		
	float 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;
	
	if (dx > 0.0f) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}
	
	if (Maths::equalsZero(dx)) {
		tx_next = FLT_MAX;
		ix_step = -1;
		ix_stop = -1;
	}
	
	
	if (dy > 0.0f) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}
	
	if (Maths::equalsZero(dy)) {
		ty_next = FLT_MAX;
		iy_step = -1;
		iy_stop = -1;
	}
		
	if (dz > 0.0f) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}
	
	if (Maths::equalsZero(dz)) {
		tz_next = FLT_MAX;
		iz_step = -1;
		iz_stop = -1;
	}
	
	// traverse the grid
	
	while (true) {	
		GeometricObject* object_ptr = &cells_cached[ix + nx * iy + nx * ny * iz];
	
		if (tx_next < ty_next && tx_next < tz_next) 
		{
			if (object_ptr && object_ptr->type != GeometricObject::Undefined)
			{
				if (object_ptr->type == GeometricObject::CompSpec)
				{
					if (object_ptr->cs.hit(ray,sr) && sr->the_t < tx_next)
					{
						return true;
					}
				} else 
				{
					if (object_ptr->hit(ray, sr) && sr->the_t < tx_next)
					{
						return true;
					}
				}
			}
			
			tx_next += dtx;
			ix += ix_step;
						
			if (ix == ix_stop)
				return (false);
		} else 
		{ 	
			if (ty_next < tz_next) 
			{
				if (object_ptr && object_ptr->type != GeometricObject::Undefined)
				{
					if (object_ptr->type == GeometricObject::CompSpec)
					{
						if (object_ptr->cs.hit(ray,sr) && sr->the_t < ty_next)
						{
							return true;
						}
					} else 
					{
						if (object_ptr->hit(ray, sr) && sr->the_t < ty_next)
						{
							return true;
						}
					}
				}
				
				ty_next += dty;
				iy += iy_step;
								
				if (iy == iy_stop)
					return (false);
		 	} else 
			{		
				if (object_ptr && object_ptr->type != GeometricObject::Undefined)
				{
					if (object_ptr->type == GeometricObject::CompSpec)
					{
						if (object_ptr->cs.hit(ray,sr) && sr->the_t < tz_next)
						{
							return true;
						}
					} else 
					{
						if (object_ptr->hit(ray, sr) && sr->the_t < tz_next)
						{
							return true;
						}
					}
				}
				
				tz_next += dtz;
				iz += iz_step;
								
				if (iz == iz_stop)
					return (false);
		 	}
		}
	}

}



// ---------------------------------------------------------------- hit


// ----------------------------------------------------------------------------- read_ply_file

// Most of this function was written by Greg Turk and is released under the licence agreement 
// at the start of the PLY.h and PLY.c files
// The PLY.h file is #included at the start of this file
// It still has some of his printf statements for debugging
// I've made changes to construct mesh triangles and store them in the grid
// mesh_ptr is a data member of Grid
// sg_objects is a data member of Compound
// triangle_type is either flat or smooth
// Using the one function construct to flat and smooth triangles saves a lot of repeated code
// The ply file is the same for flat and smooth triangles


void
Grid::read_ply_file(char* file_name, const Material *mPtr) {
	// Vertex definition 

typedef struct Vertex {
	float x,y,z;      // space coordinates       
	} Vertex;

// Face definition. This is the same for all files but is placed here to keep all the definitions together

	typedef struct Face {
		unsigned char nverts;    // number of vertex indices in list
		int* verts;              // vertex index list 
		} Face;

// list of property information for a vertex
// this varies depending on what you are reading from the file

		PlyProperty vert_props[] = {
			{"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
				{"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
				{"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
		};

// list of property information for a face. 
// there is a single property, which is a list
// this is the same for all files

		PlyProperty face_props[] = { 
			{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
				1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)}
		};

// local variables

		int 			i,j;
		PlyFile*		ply;
		int 			nelems;		// number of element types: 2 in our case - vertices and faces
		char**			elist;
		int 			file_type;
		float 			version;
		int 			nprops;		// number of properties each element has
		int 			num_elems;	// number of each type of element: number of vertices or number of faces
		// PlyProperty**	plist;
//Vertex**		vlist;
//Face**			flist;
		char*			elem_name;
		// int				num_comments;
		// char**			comments;
		// int 			num_obj_info;
		// char**			obj_info;

	// open a ply file for reading
	
		printf("Parsing PLY model: %s\n",file_name);

		ply = ply_open_for_reading(file_name, &nelems, &elist, &file_type, &version);

	// print what we found out about the file

		// printf ("version %f\n", version);
		// printf ("type %d\n", file_type);
  	
  	// go through each kind of element that we learned is in the file and read them 

	// there are only two elements in our files: vertices and faces
  	for (i = 0; i < nelems; i++) 
		{  
			// get the description of the first element 

			elem_name = elist[i];
			/*plist = */ ply_get_element_description (ply, elem_name, &num_elems, &nprops);

			// print the name of the element, for debugging

			// printf("elem name: %s, num elems: %i, num props: %i\n",elem_name,num_elems,nprops);

			// if we're on vertex elements, read in the properties

			if (equal_strings ("vertex", elem_name)) 
			{
					// set up for getting vertex elements
					// the three properties are the vertex coordinates

				ply_get_property (ply, elem_name, &vert_props[0]);
				ply_get_property (ply, elem_name, &vert_props[1]);
				ply_get_property (ply, elem_name, &vert_props[2]);

				// reserve mesh elements

				mesh_ptr.num_vertices = num_elems;
				sg_mesh_vertices.reserve(num_elems);

				// grab all the vertex elements
				Vertex* vertex_ptr = new Vertex;
				for (j = 0; j < num_elems; j++) 
				{
					

						// grab an element from the file

					ply_get_element (ply, (void *)vertex_ptr);
					sg_mesh_vertices.push_back(make_float3(vertex_ptr->x, vertex_ptr->y, vertex_ptr->z));
					// printf("vertex_ptr->x: %f\n",vertex_ptr->x);
				}
				SAFE_DELETE(vertex_ptr);
			}

			// if we're on face elements, read them in 

			if (equal_strings ("face", elem_name)) 
			{	
				// set up for getting face elements

				ply_get_property (ply, elem_name, &face_props[0]);   // only one property - a list

				mesh_ptr.num_triangles = num_elems;
				sg_objects.reserve(num_elems);  // triangles will be stored in Compound::sg_objects

			// the following code stores the face numbers that are shared by each vertex

				sg_mesh_vertex_faces.reserve(mesh_ptr.num_vertices);
				vector<int> faceList;

				for (j = 0; j < mesh_ptr.num_vertices; j++) 
					sg_mesh_vertex_faces.push_back(faceList); // store empty lists so that we can use the [] notation below

			// grab all the face elements

				int count = 0; // the number of faces read
				Face* face_ptr = new Face;
				for (j = 0; j < num_elems; j++) 
				{
					// grab an element from the file 

					ply_get_element (ply, (void *) face_ptr);

					// construct a mesh triangle of the specified type

					//OK so here we are.  What we do is create a GeoObject of type smooth mesh triangle.  we initialize the smt accordingly,
					//and finally add it to the Grids sg_objects array.  Note that this newly created GeoObj does not have a material applied, we will
					//expose set_material to grid, and let the user be able to set the material.  OH yeah, and an important detail here is that a GeoObj
					//is created for all triangles that make up the mesh.  so that's potentially thousdnds of GeoObjs which are each 200 bytes..
					GeometricObject *geoObj = new GeometricObject(&mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2],reverse_normal);
					geoObj->type = GeometricObject::SmooMesTriSpec;
					geoObj->set_material(mPtr);
					sg_objects.push_back(geoObj); 				// it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals
					numSMTs += 1;
					
					// the following code stores a list of all faces that share a vertex
					// it's used for computing the average normal at each vertex in order(num_vertices) time
					
					sg_mesh_vertex_faces.at(face_ptr->verts[0]).push_back(count);
					sg_mesh_vertex_faces.at(face_ptr->verts[1]).push_back(count);
					sg_mesh_vertex_faces.at(face_ptr->verts[2]).push_back(count);
					count++;
				}
				SAFE_DELETE(face_ptr);
	    }
	    // print out the properties we got, for debugging
	    
	    // for (j = 0; j < nprops; j++)
	    // 	printf ("property %s\n", plist[j]->name);
	}  


	// // grab and print out the comments in the file
	//   
	// comments = ply_get_comments (ply, &num_comments);
	//   
	// for (i = 0; i < num_comments; i++)
	//     printf ("comment = '%s'\n", comments[i]);
	// 
	// // grab and print out the object information
	//   
	// obj_info = ply_get_obj_info (ply, &num_obj_info);
	//   
	// for (i = 0; i < num_obj_info; i++)
	//     printf ("obj_info = '%s'\n", obj_info[i]);

	// close the ply file 
	  
	ply_close (ply);
	
	//calculate the smooth mesh normals
	compute_mesh_normals();
	
}



// this computes the average normal at each vertex
// the calculation is of order(num_vertices)
// some triangles in ply files are not defined properly

void
Grid::compute_mesh_normals(void) {
	sg_mesh_normals.reserve(mesh_ptr.num_vertices);
	
	for (int index = 0; index < mesh_ptr.num_vertices; index++) {   // for each vertex
		float3 normal;    // is zero at this point	

		for (int j = 0; j < sg_mesh_vertex_faces.at(index).size(); j++)
		{
			normal += sg_objects.at(sg_mesh_vertex_faces.at(index)[j])->get_normal();  
		}
	
		// The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0
		
		if (IsZero(normal.x)&& IsZero(normal.y) && IsZero(normal.z))
			normal.y = 1.0;
		else 
			Maths::normalize_float3(normal);
		
		sg_mesh_normals.push_back(normal);
	}
}



