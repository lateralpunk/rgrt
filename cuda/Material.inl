#pragma once

// ---------------------------------------------------------------- default constructor

Material::Material(void)
{
	glossy_specular_brdf.type = BRDF::GlossySpecular;
}

// ---------------------------------------------------------------- shade
float3 Material::shade(const World *ws, const Ray *ray, const ShadeRec *sr)	const
{

	float3	L = make_float3(0.0f);
	
	//first Phong
	const float3 wo = -1.0f * ray->d;
	L = ambient_brdf.rho(sr, &wo) * ws->ambient_ptr.L(sr);
	const int num_lights = ws->numLights;
	
	Ray shadowRay;
	shadowRay.init();
	shadowRay.o = sr->hit_point;
	ShadeRec shadowSr = *sr;
	//notify that we only interested in a shadow hit
	//and now info required for shading
	shadowSr.only_calc_hit = true;
	bool in_shadow = false;
	
	for (int j = 0; j < num_lights; j++) 
	{
		Light *lig = &ws->lights_cached[j];
		float3 wi = lig->get_direction(sr);    
		float ndotwi = dot(sr->normal,wi);
		
		if (ndotwi < EPSILON)
		{
			continue;
		}
			
		in_shadow = false;
		if (lig->castsShadows) 
		{
			shadowRay.d = wi;
			shadowSr.the_t = FLT_MAX;
			float d = Maths::distance_float3(lig->location,shadowRay.o);
			in_shadow = (ws->grid.hit(&shadowRay,&shadowSr) && shadowSr.the_t < d);
		}
		
		if (!in_shadow) 
		{
			L += (diffuse_brdf.f(sr, &wo, &wi) + glossy_specular_brdf.f(sr, &wo, &wi)) * 
						lig->L(sr) * 
						ndotwi;
		}
	}	
	
	return (L);
	
}


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
Material::set_ka(const float ka) {
	ambient_brdf.set_kd(ka);
}

// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
Material::set_kd (const float kd) {
	diffuse_brdf.set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void												
Material::set_cd(const float3& c) {
	ambient_brdf.set_cd(c);
	diffuse_brdf.set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void													
Material::set_cd(const float r, const float g, const float b) {
	ambient_brdf.set_cd(r, g, b);
	diffuse_brdf.set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void													
Material::set_cd(const float c) {
	ambient_brdf.set_cd(c);
	diffuse_brdf.set_cd(c);
}


// ---------------------------------------------------------------- set_kr


inline void								
Material::set_ks(const float ka) {
	glossy_specular_brdf.set_ks(ka);
}

inline void
Material::set_exp(const float e) {
	glossy_specular_brdf.set_exp(e);
}


void Material::set_cs(const float r, const float g, const float b)
{
	glossy_specular_brdf.set_cs(r,g,b);
}


