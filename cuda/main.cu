#include "common.h"




int main(int argc, char **argv)
{
	
	// printf("sizeof(ShadeRec): %i\n",sizeof(ShadeRec));
	// printf("sizeof(Ray): %i\n",sizeof(Ray));
	//printf("sizeof(World): %i\n",sizeof(World));
	// printf("sizeof(float3): %i\n",sizeof(float3));
	//printf("sizeof(Compound): %i\n",sizeof(Compound));
	// printf("sizeof(GeometricObject): %i\n",sizeof(GeometricObject));
	//printf("sizeof(Grid): %i\n",sizeof(Grid));
	//printf("sizeof(Material): %i\n",sizeof(Material));
	
	// Material *mat = new Material();
	// uint mat_id = (uint)mat;
	// printf("0x%x, 0x%x\n",mat,mat_id);
	
	// exit(0);
	
	World world;
	if (!world.processArgs(argc,argv))
	{
		exit(0);
	}
	
	//CUT_DEVICE_INIT(argc,argv);  
	// world.ropt.useGrid = true;
	world.render();

	return 0;
}



