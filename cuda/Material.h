#pragma once

class World;

class ALIGN_8 Material {	
public:

	void set_ka(const float k);
	void set_kd(const float k);
	void set_ks(const float k);
	void set_exp(const float exp);
	void set_cd(const float3& c);
	void	set_cd(const float r, const float g, const float b);
	void set_cd(const float c);
	void set_cs(const float r, const float g, const float b);
	
	 Material(void);						
	HOST_AND_DEVICE float3 shade(const World *ws, const Ray *ray, const ShadeRec *sr) const;
	
private:
	BRDF		ambient_brdf;
	BRDF		diffuse_brdf;
	BRDF	  glossy_specular_brdf;
};


