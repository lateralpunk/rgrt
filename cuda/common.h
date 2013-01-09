#pragma once

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>
#include <helper_cuda.h>
#include <helper_functions.h>
#include <helper_math.h>
#include <helper_string.h>
#include <helper_timer.h>
#include <float.h>
#include "Constants.h"

#ifndef __CUDACC__

// if we're not compiling with nvcc,
// #include this to define what __host__ and __device__ mean
// XXX ideally, we wouldn't require an installation of CUDA
#include <host_defines.h>

#endif // __CUDACC__

using namespace std;

#define HOST_AND_DEVICE inline __host__ __device__
//looks like doing this is unnecessary, plus we get less bytes for Geo of 224 vs 240 when not doing this
//#define ALIGN_8 __align__(8)
#define ALIGN_8

#define SAFE_FREE(x) if (x) { free(x); x=NULL; } 
#define SAFE_DELETE(x) if (x) { delete x; x=NULL; } 

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;

#include "Maths.h"

class GeometricObject;
class Light;
class Material;
typedef vector<GeometricObject*> TGeometricObjects;
typedef vector<Light*> TLights;
typedef vector<Material*> TMaterials;
typedef vector<float3> TFloat3s;
typedef vector< vector<int> > TIntsofInts;
typedef TGeometricObjects::iterator TGeometricObjectsIter;
typedef TLights::iterator TLightsIter;
typedef TMaterials::iterator TMaterialsIter;
typedef TFloat3s::iterator TFloat3sIter;
typedef TIntsofInts::iterator TTIntsofIntsIter;


//put all the declrations first
#include "Ray.h"
#include "BBox.h"
#include "Camera.h"
#include "ViewPlane.h"
#include "RenderOpt.h"
#include "BRDF.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "SmoothMeshTriangle.h"
#include "CompoundSpec.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rectangle.h"
#include "GeometricObject.h"
#include "Compound.h"
#include "Grid.h"
#include "ShadeRec.h"
#include "World.h"


//then it don't matter what order you put the defines in
#include "BBox.inl"
#include "BRDF.inl"
#include "Sphere.inl"
#include "Plane.inl"
#include "Rectangle.inl"
#include "Camera.inl"
#include "RenderOpt.inl"
#include "Compound.inl"
#include "CompoundSpec.inl"
#include "GeometricObject.inl"
#include "Mesh.inl"
#include "Light.inl"
#include "Material.inl"
#include "Ray.inl"
#include "ShadeRec.inl"
#include "SmoothMeshTriangle.inl"
#include "ViewPlane.inl"
#include "Grid.inl"
#include "World.inl"

