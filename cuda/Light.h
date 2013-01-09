#pragma once


class ALIGN_8 Light 
{	
public:

enum Type
{
		Ambient,
		Directional,
		Point
	};

	Type type;
	float		ls;
	float3	color;
	float3	dir;		// direction the light comes from
	bool castsShadows;
	float3 location;


	void set_color(const float c);
	void set_color(const float3& c);
	void set_color(const float r, const float g, const float b); 

	void set_direction(float3 d);						
	void set_direction(float dx, float dy, float dz);
	void set_location(float dx, float dy, float dz);
	void scale_radiance(const float b);

	Light(void);
	HOST_AND_DEVICE float3 L(const ShadeRec *sr) const;			
	HOST_AND_DEVICE float3 get_direction(const ShadeRec *sr) const;


};



