#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


class ALIGN_8 BBox {
public:
	float x0, x1, y0, y1, z0, z1;

	BBox(void);										
	BBox(	const float x0, const float x1,			
		const float y0, const float y1, 
		const float z0, const float z1);
	BBox(const float3 p0, const float3 p1);											
	BBox(const BBox& bbox);							
	HOST_AND_DEVICE bool hit(const Ray *ray) const;
	HOST_AND_DEVICE bool inside(const float3 *p) const;
};

