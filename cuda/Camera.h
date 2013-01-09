#pragma once



//--------------------------------------------------------------------- class Camera

class ALIGN_8 Camera {
public:

	float3			eye;				// eye point
	float3			lookat; 			// lookat point
	float			ra;					// roll angle
	float3		u, v, w;			// orthonormal basis vectors
	float3		up;					// up vector
	float	d;		// view plane distance
	float	zoom;	// zoom factor

	void set_eye(const float3& p);
	void set_eye(const float x, const float y, const float z);
	void set_lookat(const float3& p);
	void set_lookat(const float x, const float y, const float z);
	void set_up_vector(const float3& u);
	void set_up_vector(const float x, const float y, const float z);
	void set_roll(const float ra);
	void compute_uvw(void);
	void set_view_distance(const float vpd);
	void set_zoom(const float zoom_factor);

	Camera();   							// default constructor
	HOST_AND_DEVICE float3 get_direction(const float2 *p) const;
};




