// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#pragma once

extern TFloat3s 		sg_mesh_vertices;				// mesh vertices 
extern TFloat3s 		 sg_mesh_normals;				// average normal at each vertex;
extern TIntsofInts 	sg_mesh_vertex_faces;			// the triangles shared by each vertex


// // ----------------------------------------------------------------  default constructor
// 
SmoothMeshTriangle::SmoothMeshTriangle(void) :
	mesh_ptr(NULL),
	index0(0), index1(0), index2(0)
{
}

BBox SmoothMeshTriangle::init(Mesh* _mesh_ptr, const int i1, const int i2, const int i3)
{
	mesh_ptr = _mesh_ptr;
	index0 = i1;
	index1 = i2;
	index2 = i3;
	
	float3 v1(sg_mesh_vertices.at(index0));
	float3 v2(sg_mesh_vertices.at(index1));
	float3 v3(sg_mesh_vertices.at(index2));
	
	return (BBox(Maths::min(Maths::min(v1.x, v2.x), v3.x) - SMT_BBOX_DELTA, Maths::max(Maths::max(v1.x, v2.x), v3.x) + SMT_BBOX_DELTA, 
				Maths::min(Maths::min(v1.y, v2.y), v3.y) - SMT_BBOX_DELTA, Maths::max(Maths::max(v1.y, v2.y), v3.y) + SMT_BBOX_DELTA, 
				Maths::min(Maths::min(v1.z, v2.z), v3.z) - SMT_BBOX_DELTA, Maths::max(Maths::max(v1.z, v2.z), v3.z) + SMT_BBOX_DELTA));
}	

float3 SmoothMeshTriangle::interpolate_normal(const float beta, const float gamma) const {	
	float3 norm((1.0f - beta - gamma) * mesh_ptr->normals_cached[index0] 
						+ beta * mesh_ptr->normals_cached[index1] 
								+ gamma * mesh_ptr->normals_cached[index2]);
	Maths::normalize_float3(norm);
	return(norm);
}

float3 SmoothMeshTriangle::compute_normal(const bool reverse_normal) {
	float3 normal = cross((sg_mesh_vertices.at(index1) - sg_mesh_vertices.at(index0)), 
								 (sg_mesh_vertices.at(index2) - sg_mesh_vertices.at(index0)));
	Maths::normalize_float3(normal);
	
	if (reverse_normal)
		normal = -1.0f * normal;
	
	return normal;	
}

bool SmoothMeshTriangle::hit(const Ray *ray, ShadeRec *sr) const
{	
	float3 v0(mesh_ptr->vertices_cached[index0]);
	float3 v1(mesh_ptr->vertices_cached[index1]);
	float3 v2(mesh_ptr->vertices_cached[index2]);

	float a = v0.x - v1.x, b = v0.x - v2.x, c = ray->d.x, d = v0.x - ray->o.x; 
	float e = v0.y - v1.y, f = v0.y - v2.y, g = ray->d.y, h = v0.y - ray->o.y;
	float i = v0.z - v1.z, j = v0.z - v2.z, k = ray->d.z, l = v0.z - ray->o.z;
		
	float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float inv_denom  = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * inv_denom;
	
	if (beta < 0.0f)
	 	return (false);
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * inv_denom;
	
	if (gamma < 0.0f )
	 	return (false);
	
	if (beta + gamma > 1.0f)
		return (false);
			
	float e3 = a * p - b * r + d * s;
	float t = e3 * inv_denom;
	
	if (t < EPSILON) 
		return (false);
                                                                                                       					
	sr->the_t = t;
	
	if (sr->only_calc_hit)
	{
		return true;
	}
	
	sr->normal 			= interpolate_normal(beta, gamma); // for smooth shading

	return (true);
		
}

