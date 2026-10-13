/*
MATH.H

The <math.h> the musl sources in ../src see, instead of the system's: only
what they use, with the same types on every port (port/include/halo_math.h
says why). Written for this port, not musl's.
*/

#ifndef __HALO_MUSL_MATH_MATH_H
#define __HALO_MUSL_MATH_MATH_H

/* sin, pow and the rest are defined as the halo_ functions */
#include "../../../include/halo_math.h"

/* the evaluation method: the compiler's when the C library's <float.h>
does not say (nxdk's); the ports compile without excess precision
(-ffp-eval-method=source on the Xbox's x87), so this is 0 everywhere */
#include <float.h>
#if !defined(FLT_EVAL_METHOD) || FLT_EVAL_METHOD < 0
#undef FLT_EVAL_METHOD
#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__
#endif
#if FLT_EVAL_METHOD == 2
typedef long double double_t;
typedef long double float_t;
#else
typedef double double_t;
typedef float float_t;
#endif

/* musl chooses an FMA instruction where the target has one; one code path
for every port */
#undef __FP_FAST_FMA
#undef __FP_FAST_FMAF

#define INFINITY __builtin_inff()
#define NAN __builtin_nanf("")
#define HUGE_VAL __builtin_huge_val()

#define isnan(x) __builtin_isnan(x)
#define isinf(x) __builtin_isinf(x)
#define signbit(x) __builtin_signbit(x)

/* exact in IEEE arithmetic, so any implementation gives the same result */
#define fabs(x) __builtin_fabs(x)
#define sqrt(x) __builtin_sqrt(x)
#define floor(x) __builtin_floor(x)
#define scalbn(x, n) __builtin_scalbn(x, n)

#endif
