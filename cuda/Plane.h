#pragma once


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


//-------------------------------------------------------------------- class Plane

class ALIGN_8 Plane {
	
	public:
	
		float3 	a;   				// point through which plane passes 
		float3 		n;					// normal to the plane.

		Plane(void);   												// default constructor
		
		void init(const float3& point, const float3& normal);			// constructor	
	
		HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr) const;
	
				
};

