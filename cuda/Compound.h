#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

class ALIGN_8 Compound : public GeometricObject {	
	public:
		
		Compound(void);   
		~Compound (void);   

		void add_object(GeometricObject* object_ptr);
		void packCPU(CompoundSpec *cs,bool pinned);  
		

		TGeometricObjects	*objects;		
		
		
};


