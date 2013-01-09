#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


class ALIGN_8 Rectangle {	
	public:
		
		Rectangle(void);   									
		BBox init(const float3& _p0, const float3& _a, const float3& _b);
		
	
		HOST_AND_DEVICE float3 compute_normal() const;		
		
		HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr, const float3 *normal) const;
		
	private:
	
		float3 		p0;   			// corner vertex 
		float3		a;				// side
		float3		b;				// side
};

