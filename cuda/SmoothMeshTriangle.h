#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// SmoothMeshTriangle is a MeshTriangle that uses smooth shading with an interpolated normal at each hit point


class ALIGN_8 SmoothMeshTriangle  {		
public:
	Mesh 		*mesh_ptr;					// stores all the data
	int			index0, index1, index2;  	// indices into the vertices array in the mesh

	SmoothMeshTriangle(void);   	
	BBox init(Mesh* _mesh_ptr, const int i1, const int i2, const int i3);	
	float3 compute_normal(const bool reverse_normal);
	HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr) const;

private:
	HOST_AND_DEVICE float3 interpolate_normal(const float beta, const float gamma) const;
	
};

