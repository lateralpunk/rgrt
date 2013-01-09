#pragma once


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This is a header file for the mathematics utility functions
// The prototypes of any inlined functions must be placed here

// prototypes
// 
// 

// You may have to experiment with EQN_EPS
// The original was 1e-9, but I use 1e-90  KS Dec 3, 2007

#define	IsZero(x)	((x) > -EPSILON && (x) < EPSILON)


namespace Maths
{
	HOST_AND_DEVICE float
	min(float x0, float x1);

	HOST_AND_DEVICE float
	max(float x0, float x1);

	int							
	rand_int(void);

	float
	rand_float(void);

	void
	set_rand_seed(const int seed);

	float 						// for multi-jittered sampling
	rand_float(int l, float h);	

	int							// for multi-jittered sampling
	rand_int(int l, int h);   

	HOST_AND_DEVICE float
	clamp(const float x, const float min, const float max);

	HOST_AND_DEVICE int
	SolveQuadric(float c[3], float s[2]);

	HOST_AND_DEVICE int 
	SolveCubic(float c[4], float s[3]);

	HOST_AND_DEVICE int 
	SolveQuartic(float c[5], float s[4]);

	HOST_AND_DEVICE void multiple_by_float_float3(float3 &a, float s);
	HOST_AND_DEVICE void normalize_float3(float3 &v);
	HOST_AND_DEVICE float distance_float3(const float3 &a, const float3 &b);
	HOST_AND_DEVICE bool equalsZero(float x);

	//Round a / b to nearest higher integer value
	int iDivUp(int a, int b);

	//Round a / b to nearest lower integer value
	int iDivDown(int a, int b);

	//Align a to nearest higher multiple of b
	int iAlignUp(int a, int b);

	//Align a to nearest lower multiple of b
	int iAlignDown(int a, int b);

	// inlined functions

	// ----------------------------------------------------------------- min

	inline float
	min(float x0, float x1) {
		return ((x0 < x1) ? x0 : x1);
	}


	// ----------------------------------------------------------------- max

	inline float
	max(float x0, float x1) {
		return ((x0 > x1) ? x0 : x1);
	}


	// ---------------------------------------------------- rand_int
	// a wrapper for rand()

	inline int
	rand_int(void) {
		return(rand());
	}


	// ---------------------------------------------------- rand_float

	inline float
	rand_float(void) {
		return((float)rand_int() * invRAND_MAX);
	}


	// ---------------------------------------------------- set_rand_seed

	inline void
	set_rand_seed(const int seed) {
		srand(seed);
	}


	// The following two functions are only for multi-jittered sampling
	// I had trouble getting them to compile in the MultiJittered class
	// The overloading confused my compiler

	// ---------------------------------------------------- rand_float with arguments

	inline float 
	rand_float(int l, float h) {
		return (rand_float() * (h - l) + l);
	}

	// ---------------------------------------------------- rand_int with arguments

	inline int
	rand_int(int l, int h) {
		return ((int) (rand_float(0, h - l + 1) + l));
	}


	// ---------------------------------------------------- clamp

	inline float
	clamp(const float x, const float min, const float max) {
		return (x < min ? min : (x > max ? max : x));
	}

	//Round a / b to nearest higher integer value
	inline int iDivUp(int a, int b){
	    return (a % b != 0) ? (a / b + 1) : (a / b);
	}

	//Round a / b to nearest lower integer value
	inline int iDivDown(int a, int b){
	    return a / b;
	}

	//Align a to nearest higher multiple of b
	inline int iAlignUp(int a, int b){
	    return (a % b != 0) ?  (a - a % b + b) : a;
	}

	//Align a to nearest lower multiple of b
	inline int iAlignDown(int a, int b){
	    return a - a % b;
	}
}



#include "Maths.inl"
