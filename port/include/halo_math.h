/*
HALO_MATH.H

The same maths functions on every native port (included by the game's
<math.h>: port/linux/include/math.h, port/windows/include/crt/math.h).

System link games run in lockstep: every machine simulates the whole game
from the same inputs, and the host checks each machine's random seed every
tick, so a machine whose floating point results differ in the last bit
goes out of sync within ticks. The C libraries' sin, pow and the rest are
not the same bit for bit (glibc on Linux, the UCRT on Windows, musl on
Android), and the compiler folds calls with constant arguments using the
build machine's own. So the game calls halo_ versions instead, built on
every port from the same source (musl's, in port/third_party/musl-math),
and never known to the compiler as builtins.

Every port also compiles with -ffp-contract=off: fusing a multiply and an
add into one FMA instruction (which -march=native and ARM64 do by default)
rounds once instead of twice.

The macros are function-like, so they rename calls (and musl's definitions)
but not variables or fields of the same names. The functions that are
exact in IEEE arithmetic (sqrt, fabs, floor, ceil, fmod) need nothing.
*/

#ifndef __HALO_MATH_H
#define __HALO_MATH_H

double halo_sin(double x);
double halo_cos(double x);
double halo_tan(double x);
double halo_asin(double x);
double halo_acos(double x);
double halo_atan(double x);
double halo_atan2(double y, double x);
double halo_exp(double x);
double halo_log(double x);
double halo_log2(double x);
double halo_log10(double x);
double halo_pow(double x, double y);

#define sin(x) halo_sin(x)
#define cos(x) halo_cos(x)
#define tan(x) halo_tan(x)
#define asin(x) halo_asin(x)
#define acos(x) halo_acos(x)
#define atan(x) halo_atan(x)
#define atan2(y, x) halo_atan2(y, x)
#define exp(x) halo_exp(x)
#define log(x) halo_log(x)
#define log2(x) halo_log2(x)
#define log10(x) halo_log10(x)
#define pow(x, y) halo_pow(x, y)

#if defined(HALO_N64) && !defined(__HALO_MUSL_MATH_MATH_H)
/* The N64 plays on one machine (split screen), so nothing has to match
another machine to the bit, and the VR4300 does double precision slowly and
musl's routines in software: the game's sines, cosines, powers and
exponentials there are single precision (source/math/real_math.c, Cephes'
polynomials, within a few units in the last place of float), still double
to the expressions around them. musl's own sources keep their names. */
float n64_sinf(float angle);
float n64_cosf(float angle);
float n64_atan2f(float y, float x);
float n64_acosf(float x);
float n64_powf(float x, float y);
float n64_expf(float x);

#undef sin
#undef cos
#undef atan2
#undef acos
#undef exp
#undef pow
#define sin(x) ((double)n64_sinf((float)(x)))
#define cos(x) ((double)n64_cosf((float)(x)))
#define atan2(y, x) ((double)n64_atan2f((float)(y), (float)(x)))
#define acos(x) ((double)n64_acosf((float)(x)))
#define exp(x) ((double)n64_expf((float)(x)))
#define pow(x, y) ((double)n64_powf((float)(x), (float)(y)))
#endif

#endif
