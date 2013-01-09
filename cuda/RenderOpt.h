#pragma once

class ALIGN_8 RenderOpt {
	public:
		ViewPlane vp;
		Camera camera_ptr;
		bool useGrid, save,usePinned;
		float cpuTime, gpuMemTime, gpuCompTime, error;
		const char *buildName;
		const char *m_fname;
		int mode;
		
		 RenderOpt();
		
		void save_canvas(const uchar4 *canvas) const;
	  
};

