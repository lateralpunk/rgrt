#pragma once

// ----------------------------------------------------------------- default constructor

Camera::Camera(void)		
	:	eye(make_float3(0.0f, 0.0f, 500.0f)),
		lookat(make_float3(0.0f)),
		ra(0.0f),
		up(make_float3(0.0f, 1.0f, 0.0f)),
		u(make_float3(1.0f, 0.0f, 0.0f)),
		v(make_float3(0.0f, 1.0f, 0.0f)),
		w(make_float3(0.0f, 0.0f, 1.0f)),
		d(500.0f),
		zoom(1.0f)
{}

//-------------------------------------------------------------- compute_uvw

// This computes an orthornormal basis given the view point, lookat point, and up vector

void
Camera::compute_uvw(void) {
	w = eye - lookat;
	Maths::normalize_float3(w);
	u = cross(up,w);
	Maths::normalize_float3(u);
	v = cross(w,u);

	// take care of the singularity by hardwiring in specific camera orientations
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) { // camera looking vertically down
		u = make_float3(0.0f, 0.0f, 1.0f);
		v = make_float3(1.0f, 0.0f, 0.0f);
		w = make_float3(0.0f, 1.0f, 0.0f);	
	}
	
	if (eye.x == lookat.x && eye.z == lookat.z && eye.y < lookat.y) { // camera looking vertically up
		u = make_float3(1.0f, 0.0f, 0.0f);
		v = make_float3(0.0f, 0.0f, 1.0f);
		w = make_float3(0.0f, -1.0f, 0.0f);
	}
}

// ----------------------------------------------------------------------------- get_direction

float3
Camera::get_direction(const float2 *p) const {
	float3 dir = p->x * u + p->y * v - d * w;
	Maths::normalize_float3(dir);
	return(dir);
}



// ----------------------------------------------------------------------------- render_scene

// inlined access functions


// ----------------------------------------------------------------- set_eye

inline void
Camera::set_eye(const float3& p) {
	eye = p;
}


// ----------------------------------------------------------------- set_eye

inline void
Camera::set_eye(const float x, const float y, const float z) {
	eye.x = x; eye.y = y; eye.z = z;
}


// ----------------------------------------------------------------- set_lookat

inline void
Camera::set_lookat(const float3& p) {
	lookat = p;
}


// ----------------------------------------------------------------- set_lookat

inline void
Camera::set_lookat(const float x, const float y, const float z) {
	lookat.x = x; lookat.y = y; lookat.z = z;
}


// ----------------------------------------------------------------- set_up_vector

inline void
Camera::set_up_vector(const float3& u) {
	up = u;
}


// ----------------------------------------------------------------- set_up_vector

inline void
Camera::set_up_vector(const float x, const float y, const float z) {
	up.x = x; up.y = y; up.z = z;
}


// ----------------------------------------------------------------- set_roll

inline void
Camera::set_roll(const float r) { 
	ra = r;
}


//-------------------------------------------------------------------------- set_vpd

inline void
Camera::set_view_distance(float _d) {
	d = _d;
}	



//-------------------------------------------------------------------------- set_zoom

inline void
Camera::set_zoom(float zoom_factor) {
	zoom = zoom_factor;
}

