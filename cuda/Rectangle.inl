// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// ----------------------------------------------------------------  default constructor

Rectangle::Rectangle(void)
	:
		p0(make_float3(-1.0f, 0.0f, -1.0f)), 
		a(make_float3(0.0f, 0.0f, 2.0f)), b(make_float3(2.0f, 0.0f, 0.0f))
{}


BBox Rectangle::init(const float3& _p0, const float3& _a, const float3& _b)
{
	p0 = _p0;
	a = _a;
	b = _b;
	
	return (BBox(Maths::min(p0.x, p0.x + a.x + b.x) - RECT_BBOX_DELTA, Maths::max(p0.x, p0.x + a.x + b.x) + RECT_BBOX_DELTA,
				Maths::min(p0.y, p0.y + a.y + b.y) - RECT_BBOX_DELTA, Maths::max(p0.y, p0.y + a.y + b.y) + RECT_BBOX_DELTA, 
				Maths::min(p0.z, p0.z + a.z + b.z) - RECT_BBOX_DELTA, Maths::max(p0.z, p0.z + a.z + b.z) + RECT_BBOX_DELTA));
}

//------------------------------------------------------------------ get_bounding_box 

float3 Rectangle::compute_normal() const 
{
	float3 normal = cross(a, b);
	Maths::normalize_float3(normal);
	return (normal);
}

bool Rectangle::hit(const Ray *ray, ShadeRec *sr, const float3 *normal) const {

	float t = dot((p0 - ray->o),*normal) * 1.0f/dot(ray->d,*normal); 

	if (t <= EPSILON)
		return (false);
			
	float3 p = ray->o + t * ray->d;
	float3 d = p - p0;
	
	float ddota = dot(d,a);
	
	if (ddota < 0.0f || ddota > dot(a,a))
		return (false);
		
	float ddotb = dot(d,b);
	
	if (ddotb < 0.0f || ddotb > dot(b,b))
		return (false);
		
	sr->the_t 				= t;
	
	if (sr->only_calc_hit)
	{
		return true;
	}
	
	sr->normal 			= *normal;
	
	return (true);
}



