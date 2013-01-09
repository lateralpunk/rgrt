#pragma once

// This file contains the definition the ViewPlane class
	
ViewPlane::ViewPlane(void)							
	: 	hres(400), 
		vres(400),
		num_samples(1)
{}

void 													
ViewPlane::set_hres(const int h_res) {
	hres = h_res;
}

void 													
ViewPlane::set_vres(const int v_res) {
	vres = v_res;
}

inline void
ViewPlane::set_samples(const int n) {
	num_samples = n;
}

	





