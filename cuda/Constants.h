#pragma once

#define EPSILON 1e-3f

#define RECT_BBOX_DELTA 0.5f 
#define SPHERE_BBOX_DELTA 2.0f
#define SMT_BBOX_DELTA 1e-3f
#define GRID_BBOX_DELTA 1e-3f

#define 	PI 			  3.1415926535897932384f
#define 	TWO_PI 		6.2831853071795864769f
#define 	PI_ON_180 0.0174532925199432957f
#define 	invPI 		0.3183098861837906715f
#define 	invTWO_PI 0.1591549430918953358f
#define 	invRAND_MAX (1.0f / (float)RAND_MAX)


const float3	black(make_float3(0.0f));
const float3	white(make_float3(1.0f));
const float3	red(make_float3(1.0f, 0.0f, 0.0f));
const	float3 yellow(make_float3(1.0f, 1.0f, 0.0f));										// yellow
const	float3 brown(make_float3(0.71f, 0.40f, 0.16f));								// brown
const	float3 darkGreen(make_float3(0.0f, 0.41f, 0.41f));							// darkGreen
const	float3 orange(make_float3(1.0f, 0.75f, 0.0f));									// orange
const	float3 green(make_float3(0.0f, 0.6f, 0.3f));									// green
const	float3 lightGreen(make_float3(0.65f, 1.0f, 0.30f));								// light green
const	float3 darkYellow(make_float3(0.61f, 0.61f, 0.0f));								// dark yellow
const	float3 lightPurple(make_float3(0.65f, 0.3f, 1.0f));								// light purple
const	float3 darkPurple(make_float3(0.5f, 0.0f, 1.0f));									// dark purple
const	float3 grey(make_float3(0.25f));											// grey



