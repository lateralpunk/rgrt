#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

//---------------------------------------------------------------------- class Grid
class ALIGN_8 Grid {										  	
public:

	 Grid(void); 
	 ~Grid();		
	
	void read_ply_file(char* file_name, const Material *mPtr);
	void packCPU(const RenderOpt *ropt);
	void packGPU();
	void unpackGPU();
	void add_object(GeometricObject* object_ptr);
	

	HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr) const;		 					
	 
	int num_cells;
	GeometricObject *cells_cached;
	//we need this later in Grid::unpackGPU.  It points to the host cells
	//which we need to iterateve over and find all CompoundSpecs that we can
	//then call unpackGPU on :)
	GeometricObject *temp_cells_cached; 
	Mesh						mesh_ptr;		// holds triangle data
	int numSMTs;
	int count_total_geos;

private: 
	bool usePinned;
	int							nx, ny, nz;    	// number of cells in the x, y, and z directions
	BBox						bbox;			// bounding box
	bool						reverse_normal;	// some PLY files have normals that point inwards
	RenderOpt ropt;

	void unpackCPU(GeometricObject *temp_cells);

	
	HOST_AND_DEVICE bool no_accel_hit(const Ray *ray, ShadeRec *sr) const;	
	HOST_AND_DEVICE bool accel_hit(const Ray *ray, ShadeRec *sr) const;	
	

	float3 find_min_bounds(void);
	float3 find_max_bounds(void);
	void setup_cells(void);
	void delete_vectors(void);

	void compute_mesh_normals(void);	
	
};







