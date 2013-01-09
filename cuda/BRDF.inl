#pragma once


BRDF::BRDF(void) 
	: kd(0.0f), 
	exp(1.0f),
	type(Lambertian),
	cd(make_float3(1.0f))
{
}

// ---------------------------------------------------------------------- f

float3
BRDF::f(const ShadeRec *sr, const float3 *wo, const float3 *wi) const {

	if (type == Lambertian)
	{
		return (kd * cd * invPI);
		} else if (type == GlossySpecular)
		{
			float3 	L = make_float3(0.0f);  				
			float 		ndotwi = dot(sr->normal,*wi);
			float3 	r = -1.0f * *wi + 2.0f * sr->normal * ndotwi; // mirror reflection direction
			float 		rdotwo = dot(r, *wo); 		

			if (rdotwo > 0.0f)
				L = kd * cd * powf(rdotwo, exp); 

			return (L);
		}

		return make_float3(0.0f);
	}

// ---------------------------------------------------------------------- rho

	float3
	BRDF::rho(const ShadeRec *sr, const float3 *wo) const {

		if (type == Lambertian)
		{
			return (kd * cd);
		}

		return make_float3(0.0f);
	}

// -------------------------------------------------------------- set_ka

	inline void
	BRDF::set_ka(const float k) {
		kd = k;
	}



// -------------------------------------------------------------- set_kd

	inline void
	BRDF::set_kd(const float k) {
		kd = k;
	}


// -------------------------------------------------------------- set_cd

	inline void
	BRDF::set_cd(const float3& c) {
		cd = c;
	}


// ---------------------------------------------------------------- set_cd

	inline void													
	BRDF::set_cd(const float r, const float g, const float b) {
		cd.x = r; cd.y = g; cd.z = b; 
	}


// ---------------------------------------------------------------- set_cd

	inline void													
	BRDF::set_cd(const float c) {
		cd.x = c; cd.y = c; cd.z = c;
	}

// -------------------------------------------------------------- set_kr

	inline void
	BRDF::set_kr(const float k) {
		set_kd(k);
	}


// -------------------------------------------------------------- set_cr

	inline void
	BRDF::set_cr(const float3& c) {
		set_cd(c);
	}


// ---------------------------------------------------------------- set_cr

	inline void													
	BRDF::set_cr(const float r, const float g, const float b) {
		set_cd(r,g,b);
	}


// ---------------------------------------------------------------- set_cr

	inline void													
	BRDF::set_cr(const float c) {
		set_cd(c);
	}

// -------------------------------------------------------------- set_ks

	inline void
	BRDF::set_ks(const float k) {
		set_kd(k);
	}


// -------------------------------------------------------------- set_exp

	inline void
	BRDF::set_exp(const float e) {
		exp= e;
	}


// -------------------------------------------------------------- set_cs

	inline void
	BRDF::set_cs(const float3& c) {
		set_cd(c);
	}


// ---------------------------------------------------------------- set_cs

	inline void													
	BRDF::set_cs(const float r, const float g, const float b) {
		set_cd(r,g,b);
	}


// ---------------------------------------------------------------- set_cs

	inline void													
	BRDF::set_cs(const float c) {
		set_cd(c);
	}


