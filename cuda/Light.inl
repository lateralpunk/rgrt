#pragma once

// ---------------------------------------------------------------------- default constructor

Light::Light(void) : 
type(Ambient),
ls(1.0f), 
color(make_float3(1.0f)), 
dir(make_float3(0.0f, 1.0f, 0.0f)), 
castsShadows(false),
location(make_float3(0.0f))
{
	
}


// ---------------------------------------------------------------------- get_direction	

float3								
Light::get_direction(const ShadeRec *s) const {
	
	if (type == Directional)
	{
		return dir;
	} else if (type == Point)
	{
		return normalize(location - s->hit_point);
	}
	
	return make_float3(0.0f);
}


// ---------------------------------------------------------------------- L

float3
Light::L(const ShadeRec *sr) const {	
	return (ls * color);
}



// ------------------------------------------------------------------------------- scale_radiance

	inline void
	Light::scale_radiance(const float b) { 
		ls = b;
	}

// ------------------------------------------------------------------------------- set_color

	inline void
	Light::set_color(const float c) {
		color.x = c; color.y = c; color.z = c;
	}


// ------------------------------------------------------------------------------- set_color

	inline void
	Light::set_color(const float3& c) {
		color = c;
	}


// ------------------------------------------------------------------------------- set_color

	inline void
	Light::set_color(const float r, const float g, const float b) {
		color.x = r; color.y = g; color.z = b; 
	}


// ---------------------------------------------------------------------- set_direction

	inline void
	Light::set_direction(float3 d) {
		dir = d;
		Maths::normalize_float3(dir);
	}


// ---------------------------------------------------------------------- set_direction 

	inline void
	Light::set_direction(float dx, float dy, float dz) {
		dir.x = dx; dir.y = dy; dir.z = dz;
		Maths::normalize_float3(dir);
	}

	void Light::set_location(float dx, float dy, float dz)
	{
		location.x = dx;
		location.y = dy;
		location.z = dz;
	}

