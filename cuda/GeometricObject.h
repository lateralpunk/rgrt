#pragma once

//#define GEO_PLANES 1

class ALIGN_8 GeometricObject 
{	

protected:
	GeometricObject(void);	

private:

	ulong mat_ptr_id;
	void set_material(const Material *mPtr); 	
	void set_material(ulong mat_idx); 
	
public:	
	friend class World;	
	friend class Grid;
	

	enum Type
	{
		Undefined,
		SphereSpec,
		RectSpec,
		CompSpec,
		SmooMesTriSpec,
		PlaneSpec
	};

	Type type;
	Sphere sp;
	Rectangle rect;
	SmoothMeshTriangle smt;
	CompoundSpec cs;
#ifdef GEO_PLANES
	Plane plane;
#endif
	BBox bbox;
	float3		normal;	

//sphere
	GeometricObject(const float3 &center, float r);
//rectangle
	GeometricObject(const float3& _p0, const float3& _a, const float3& _b);
//smt
	GeometricObject(Mesh* _mesh_ptr, const int i1, const int i2, const int i3,bool reverse_normal);
#ifdef GEO_PLANES	
//plane
	GeometricObject(const float3& point, const float3& normal);
#endif

//notice virtual ;) no we don't use it on CUDA don't worry
	virtual ~GeometricObject();

	const Material*	get_material(void) const;		
	HOST_AND_DEVICE ulong	get_material_idx(void) const;		
	
	HOST_AND_DEVICE bool hit(const Ray *ray, ShadeRec *sr) const;
	HOST_AND_DEVICE const BBox& get_bounding_box(void);
	const float3& get_normal(void) const; 	
	

};


