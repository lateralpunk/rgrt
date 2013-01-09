#pragma once

class ShadeRec;

class ALIGN_8 BRDF {
public:
	enum Type
	{
		Lambertian,
			GlossySpecular
		};

		Type type;

private:
	float		kd;
	float3 	cd;		
	float		exp; 		// specular exponent

public:
	
		void set_ka(const float ka);	
		void set_kd(const float kd);
		void set_cd(const float3& c);
		void set_cd(const float r, const float g, const float b);
		void set_cd(const float c);
		void set_kr(const float k);
		void set_cr(const float3& c);
		void set_cr(const float r, const float g, const float b);
		void set_cr(const float c);
		void set_ks(const float ks);
		void set_exp(const float exp);
		void set_cs(const float3& c);
		void set_cs(const float r, const float g, const float b);
		void set_cs(const float c);

		BRDF(void);						
		HOST_AND_DEVICE float3 f(const ShadeRec *sr, const float3 *wo, const float3 *wi) const;
		HOST_AND_DEVICE float3 rho(const ShadeRec *sr, const float3 *wo) const;

	};


