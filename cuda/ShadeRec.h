#pragma once

// this file contains the declaration of the class ShadeRec

class ALIGN_8 ShadeRec {
	public:
	
		float3 			hit_point;			// World coordinates of intersection
		float3				normal;				// float3 at hit point
		float				the_t;					// ray parameter
		ulong material;
		bool only_calc_hit;
		
		HOST_AND_DEVICE void init();
	  
};

