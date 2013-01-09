#pragma once

class ALIGN_8 CompoundSpec {	
public:
	bool usePinned;

	CompoundSpec(void);   										
	HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *s) const;
	void packGPU();
	void unpackGPU();

	GeometricObject *objects;
	int numObjects;

private:	
	void unpackCPU(GeometricObject *temp_objects);
	
	

};
