#pragma once
// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


//not static since this will be used in Grid as a global var (yuck!)
TFloat3s 		sg_mesh_vertices;				// mesh vertices 
TFloat3s 		 sg_mesh_normals;				// average normal at each vertex;
TIntsofInts 	sg_mesh_vertex_faces;			// the triangles shared by each vertex


// ----------------------------------------------------------------  default constructor
Mesh::Mesh(void)
	: num_vertices(0),
		vertices_cached(NULL),
		normals_cached(NULL),
		pinnedVertices(false),
		pinnedNormals(false),
		num_triangles(0)
{
}

Mesh::~Mesh(void)
{
	delete_vectors();		
}

void Mesh::delete_vectors(void)
{
	sg_mesh_vertices.clear();
	sg_mesh_normals.clear();
	sg_mesh_vertex_faces.clear();
}

