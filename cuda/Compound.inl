#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// ----------------------------------------------------------------  default constructor

Compound::Compound (void) 
{
	objects = new TGeometricObjects();
	type = GeometricObject::CompSpec;
}


// ---------------------------------------------------------------- destructor

Compound::~Compound(void) 
{	
	SAFE_DELETE(objects);
}

// ---------------------------------------------------------------- add_object


void 
Compound::add_object(GeometricObject* object_ptr) {
	objects->push_back(object_ptr);	
}


