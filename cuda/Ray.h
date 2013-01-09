#pragma once

class ALIGN_8 Ray {
	public:
		float3			o;  	// origin 
		float3		d; 		// direction 
		
		HOST_AND_DEVICE void init();
		
};

