extern void SaveBMPFile(const uchar4 *dst, int width, int height, const char *outputname);

#define NUM_CHARS 1024

RenderOpt::RenderOpt() :
useGrid(true),
cpuTime(0.0f),
gpuMemTime(0.0f),
gpuCompTime(0.0f),
error(0.0f),
buildName(NULL),
m_fname(NULL),
mode(0),
save(true),
usePinned(false)
{
}


void RenderOpt::save_canvas(const uchar4 *canvas) const
{
	if (!save)
	{
		return;
	}
	
	char fn[NUM_CHARS];
	const char *finalFileName = m_fname;

	if (!finalFileName)
	{
		if (mode == 0)
		{
			sprintf(fn,"%s_g-%i_p-%i_wh-%ix%i_n-%i_z-%.1f_cpu-%.2f_gpumem-%.2f_gpu-%.2f_gputotal-%.2f_error-%.6f.bmp",
							buildName,useGrid,usePinned,vp.hres,vp.vres,vp.num_samples,camera_ptr.zoom,cpuTime,gpuMemTime,gpuCompTime,gpuMemTime+gpuCompTime,error);
		} else if (mode == 1)
		{
			sprintf(fn,"%s_g-%i_p-%i_wh-%ix%i_n-%i_z-%.1f_gpumem-%.2f_gpu-%.2f_gputotal-%.2f.bmp",
							buildName,useGrid,usePinned,vp.hres,vp.vres,vp.num_samples,camera_ptr.zoom,gpuMemTime,gpuCompTime,gpuMemTime+gpuCompTime);			
		} else if (mode == 2)
		{
			sprintf(fn,"%s_g-%i_p-%i_wh-%ix%i_n-%i_z-%.1f_cpu-%.2f.bmp",
							buildName,useGrid,usePinned,vp.hres,vp.vres,vp.num_samples,camera_ptr.zoom,cpuTime);
		}
		
		finalFileName = fn;
	}
	
	SaveBMPFile(canvas, vp.hres, vp.vres,finalFileName);
	printf("Imaged saved: %s\n",finalFileName);
	
}
