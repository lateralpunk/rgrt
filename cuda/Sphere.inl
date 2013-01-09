#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// ---------------------------------------------------------------- default constructor

Sphere::Sphere(void)	
	: 	
		center(make_float3(0.0f)),
		radius(1.0f)
{
}

BBox Sphere::init(const float3 &c1, float r1)
{
	center = c1;
	radius = r1;
	
	float3 minp(make_float3(-radius*SPHERE_BBOX_DELTA));
	float3 maxp(make_float3(radius*SPHERE_BBOX_DELTA));

	return BBox(minp+center,maxp+center);
}

bool
Sphere::hit(const Ray *ray, ShadeRec *sr) const {
	
	float 		a 		= dot(ray->d,ray->d);
	
	if (IsZero(a))
	{
		return false;
	}
	
	float 		t;
	float3	temp 	= ray->o - center;
	float 		b 		= 2.0f * dot(temp,ray->d);
	float 		c 		= dot(temp,temp) - radius * radius;
	float 		disc	= b * b - 4.0f * a * c;
	
	if (disc < EPSILON)
		return(false);
	else 
	{	
		float e = sqrtf(disc);
		float denom = 1.0f/(2.0f * a);
		
		t = Maths::min(-b - e, -b + e);
		t *= denom;
	
		if (t < EPSILON) 
		{
			return (false);
		} 
	}

	sr->the_t = t;

	if (sr->only_calc_hit)
	{
		return true;
	}
	
	sr->normal   = (temp + t * ray->d) * (1.0f/radius);

	return (true);
}

