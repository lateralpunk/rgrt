#pragma once

CompoundSpec::CompoundSpec (void) :
	objects(NULL),
	numObjects(0),
	usePinned(false)
{
	
}

bool CompoundSpec::hit(const Ray *ray, ShadeRec *sr) const 
{
	float3		normal(make_float3(0.0f));
	bool		hit 		= false;
	float tmin 		= FLT_MAX;
	ulong material = ULONG_MAX;
	
	for (int j = 0; j < numObjects; j++)
	{
		GeometricObject *geoObj = &objects[j];
		if (geoObj && geoObj->hit(ray, sr) && (sr->the_t < tmin)) 
		{
			hit				= true;
			tmin 			= sr->the_t;
			material = geoObj->get_material_idx();	
			normal			= sr->normal;
		}
	}
	
	if (hit) 
	{
		sr->the_t				= tmin;
		sr->normal 			= normal;
		sr->material = material;
	}
	
	return (hit);
}

