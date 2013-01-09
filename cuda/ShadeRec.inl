#pragma once

void ShadeRec::init()
{
	hit_point = (make_float3(0.0f));
	normal = (make_float3(0.0f));
	the_t = (FLT_MAX);
	material = (ULONG_MAX);
	only_calc_hit = (false);
}

