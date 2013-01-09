#pragma once


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// A mesh object stores the data for mesh triangles:
//
//			vertices
//			normals
//			uv texture coordinates
//			lists of triangles that share each vertex
//
// Mesh triangles are stored in a grid, which has a pointer to the mesh
// Each mesh triangle also has a pointer to the mesh
// The Mesh class does not inherit from GeometricObject

class ALIGN_8 Mesh {										  	
	public:

		int 					num_vertices; 			// number of vertices
		float3 *vertices_cached;
		float3 *normals_cached;
		int 					num_triangles; 			// number of triangles
		 Mesh(void);  
		~Mesh(void); 			
		
	void packCPU(bool pinned);
	void packGPU();
	void unpackGPU();
		
		
	private:
		bool pinnedVertices, pinnedNormals;
		void delete_vectors(void);
		void unpackCPU(float3 *temp_vertices, float3 *temp_normals);
		
};

