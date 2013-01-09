#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


//-------------------------------------------------------------------------------- class Sphere

class ALIGN_8 Sphere {	
								  	
	public:
		
		float3 	center;   			// center coordinates as a point  
		float 		radius;				// the radius 

		Sphere(void);   									// Default constructor
		BBox init(const float3 &center, float r);
																					
		HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr) const;	
};


