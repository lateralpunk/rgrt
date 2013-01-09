#pragma once

// this file contains the definition of the class GeometricObject 

GeometricObject::GeometricObject(void) : 
type(Undefined),
normal(make_float3(0.0f)),
mat_ptr_id(ULONG_MAX)
{
}

GeometricObject::GeometricObject(const float3 &c, float r) : 
type(SphereSpec)
{
	bbox = sp.init(c,r);
}

GeometricObject::GeometricObject(const float3& _p0, const float3& _a, const float3& _b) :
type(RectSpec)
{
	bbox = rect.init(_p0,_a,_b);
	normal = rect.compute_normal();
}

GeometricObject::GeometricObject(Mesh* _mesh_ptr, const int i1, const int i2, const int i3,bool reverse_normal) :
type(SmooMesTriSpec)
{
	bbox = smt.init(_mesh_ptr,i1,i2,i3);
	normal = smt.compute_normal(reverse_normal);
}


#ifdef GEO_PLANES
GeometricObject::GeometricObject(const float3& point, const float3& normal) :
type(PlaneSpec)
{
	plane.init(point,normal);
}
#endif


GeometricObject::~GeometricObject()
{
	
}


// ---------------------------------------------------------------- set_material

void GeometricObject::set_material(const Material *mPtr) 
{
	mat_ptr_id = (ulong)mPtr;
}

void GeometricObject::set_material(ulong mat_idx)
{
	mat_ptr_id = mat_idx;
}


const Material* GeometricObject::get_material(void) const 
{
	return (const Material*)mat_ptr_id;
}

ulong GeometricObject::get_material_idx(void) const
{
	return mat_ptr_id;
}


// ----------------------------------------------------------------- hit

bool GeometricObject::hit(const Ray *ray, ShadeRec *sr) const
{	
	if (type == SphereSpec)
	{
		return sp.hit(ray,sr);
	} else if (type == SmooMesTriSpec)
	{
		return smt.hit(ray,sr);
	} else if (type == RectSpec)
	{
		return rect.hit(ray,sr,&normal);
	} 
#ifdef GEO_PLANES	
	else if (type == PlaneSpec)
	{
		return plane.hit(ray,sr);
	}
#endif
	
	return false;
	
}

const float3& GeometricObject::get_normal(void) const
{
	return normal;
}

const BBox& GeometricObject::get_bounding_box (void) 
{
	return bbox;
}
