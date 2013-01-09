#pragma once

//-------------------------------------------------------------------------------------- class ViewPlane

class ALIGN_8 ViewPlane {
public:
	int 			hres;   					// horizontal image resolution 
	int 			vres;   					// vertical image resolution
	int				num_samples;				// number of samples per pixel

	ViewPlane();   								// default Constructor
	void set_samples(const int n);			
	void set_hres(const int h_res);
	void set_vres(const int v_res);

	

};

