// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// ----------------------------------------------------------------------  default constructor

Plane::Plane(void)	:
		a(make_float3(0.0f)),
		n(make_float3(0.0f, 1.0f, 0.0f))						
{}

// ----------------------------------------------------------------------  constructor

void Plane::init(const float3& point, const float3& normal)
{
	a = point;
	n = normal;
	Maths::normalize_float3(n);
}

// ----------------------------------------------------------------- hit


bool Plane::hit(const Ray *ray, ShadeRec *sr) const {
	
	float t = dot((a - ray->o), n) * 1.0f/dot(ray->d,n);
														
	if (t > EPSILON) 
	{
		sr->the_t 				= t;
		
		if (sr->only_calc_hit)
		{
			return true;
		}
		
		sr->normal 			= n;
		
		return (true);	
	}

	return (false);
}




