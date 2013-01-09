// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <math.h>

/*
 *  Roots3And4.c
 *
 *  Utility functions to find cubic and quartic roots,
 *  coefficients are passed like this:
 *
 *      c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 + c[4]*x^4 = 0
 *
 *  The functions return the number of non-complex roots and
 *  put the values into the s array.
 *
 *  Author:         Jochen Schwarze (schwarze@isa.de)
 *
 *  Jan 26, 1990    Version for Graphics Gems
 *  Oct 11, 1990    Fixed sign problem for negative q's in SolveQuartic
 *  	    	    		(reported by Mark Podlipec),
 *  	    	    		Old-style function definitions,
 *  	    	    		IsZero() as a macro
 *  Nov 23, 1990  Some systems do not declare acos() and cbrtf() in
 *                <math.h>, though the functions exist in the library.
 *                If large coefficients are used, EQN_EPS should be
 *                reduced considerably (e.g. to 1E-30), results will be
 *                correct but multiple roots might be reported more
 *                than once.
 */

 
#ifndef M_PI
#define M_PI PI
#endif

namespace Maths
{
	int SolveQuadric(float c[3], float s[2]) {
		float p, q, D;

			/* normal form: x^2 + px + q = 0 */

		p = c[ 1 ] / (2.0f * c[ 2 ]);
		q = c[ 0 ] / c[ 2 ];

		D = p * p - q;

		if (IsZero(D)) 
		{
			s[ 0 ] = -p;
			return 1;
		}
		else if (D > 0.0f) 
		{
			float sqrt_D = sqrtf(D);

			s[ 0 ] =  sqrt_D - p;
			s[ 1 ] = -sqrt_D - p;
			return 2;
		}
		else /* if (D < 0) */
			return 0;
	}


	int SolveCubic(float c[4], float s[3]) {
		int     i, num;
		float  sub;
		float  A, B, C;
		float  sq_A, p, q;
		float  cb_p, D;

			/* normal form: x^3 + Ax^2 + Bx + C = 0 */

		A = c[ 2 ] / c[ 3 ];
		B = c[ 1 ] / c[ 3 ];
		C = c[ 0 ] / c[ 3 ];

		/*  substitute x = y - A/3 to eliminate quadric term:
		x^3 +px + q = 0 */

		sq_A = A * A;
		p = 1.0f/3.0f * (-1.0f/3.0f * sq_A + B);
		q = 1.0f/2.0f * (2.0f/27.0f * A * sq_A - 1.0f/3.0f * A * B + C);

		/* use Cardano's formula */

		cb_p = p * p * p;
		D = q * q + cb_p;

		if (IsZero(D)) {
			if (IsZero(q)) { /* one triple solution */
				s[ 0 ] = 0.0f;
				num = 1;
			}
			else { /* one single and one float solution */
				float u = cbrtf(-q);
				s[ 0 ] = 2.0f * u;
				s[ 1 ] = - u;
				num = 2;
			}
		}
		else if (D < 0.0f) { /* Casus irreducibilis: three real solutions */
			float phi = 1.0f/3.0f * acosf(-q / sqrtf(-cb_p));
			float t = 2.0f * sqrtf(-p);

			s[ 0 ] =   t * cosf(phi);
			s[ 1 ] = - t * cosf(phi + M_PI / 3.0f);
			s[ 2 ] = - t * cosf(phi - M_PI / 3.0f);
			num = 3;
		}
		else { /* one real solution */
			float sqrt_D = sqrtf(D);
			float u = cbrtf(sqrt_D - q);
			float v = - cbrtf(sqrt_D + q);

			s[ 0 ] = u + v;
			num = 1;
		}

		/* resubstitute */

		sub = 1.0f/3.0f * A;

		for (i = 0; i < num; ++i)
			s[ i ] -= sub;

		return num;
	}



	int SolveQuartic(float c[5], float s[4]) {
		float  coeffs[4];
		float  z, u, v, sub;
		float  A, B, C, D;
		float  sq_A, p, q, r;
		int     i, num;

			/* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

		A = c[ 3 ] / c[ 4 ];
		B = c[ 2 ] / c[ 4 ];
		C = c[ 1 ] / c[ 4 ];
		D = c[ 0 ] / c[ 4 ];

		/*  substitute x = y - A/4 to eliminate cubic term:
		x^4 + px^2 + qx + r = 0 */

		sq_A = A * A;
		p = - 3.0f/8.0f * sq_A + B;
		q = 1.0f/8.0f * sq_A * A - 1.0f/2.0f * A * B + C;
		r = - 3.0f/256.0f*sq_A*sq_A + 1.0f/16.0f*sq_A*B - 1.0f/4.0f*A*C + D;

		if (IsZero(r)) {
		/* no absolute term: y(y^3 + py + q) = 0 */

			coeffs[ 0 ] = q;
			coeffs[ 1 ] = p;
			coeffs[ 2 ] = 0.0f;
			coeffs[ 3 ] = 1.0f;

			num = SolveCubic(coeffs, s);

			s[ num++ ] = 0.0f;
		}
		else {
		/* solve the resolvent cubic ... */

			coeffs[ 0 ] = 1.0f/2.0f * r * p - 1.0f/8.0f * q * q;
			coeffs[ 1 ] = - r;
			coeffs[ 2 ] = - 1.0f/2.0f * p;
			coeffs[ 3 ] = 1.0f;

			(void) SolveCubic(coeffs, s);

		/* ... and take the one real solution ... */

			z = s[ 0 ];

		/* ... to build two quadric equations */

			u = z * z - r;
			v = 2.0f * z - p;

			if (IsZero(u))
				u = 0.0f;
			else if (u > 0.0f)
				u = sqrtf(u);
			else
				return 0;

			if (IsZero(v))
				v = 0.0f;
			else if (v > 0.0f)
				v = sqrtf(v);
			else
				return 0;

			coeffs[ 0 ] = z - u;
			coeffs[ 1 ] = q < 0.0f ? -v : v;
			coeffs[ 2 ] = 1.0f;

			num = SolveQuadric(coeffs, s);

			coeffs[ 0 ]= z + u;
			coeffs[ 1 ] = q < 0.0f ? v : -v;
			coeffs[ 2 ] = 1.0f;

			num += SolveQuadric(coeffs, s + num);
		}

		/* resubstitute */

		sub = 1.0f/4.0f * A;

		for (i = 0; i < num; ++i)
			s[ i ] -= sub;

		return num;
	}

	void multiple_by_float_float3(float3 &a, float s)
	{
	    a.x *= s; a.y *= s; a.z *= s;
	}

	// normalize_this
	void normalize_float3(float3 &v)
	{
	    float invLen = rsqrtf(dot(v, v));
			multiple_by_float_float3(v,invLen);
	}

	float distance_float3(const float3 &a, const float3 &b)
	{
		return length(a-b);
	}

	bool equalsZero(float x) {
		return x <= EPSILON && x >= -EPSILON;
	}	
}

