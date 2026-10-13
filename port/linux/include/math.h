/*
MATH.H

Host <math.h> plus the MSVC _USE_MATH_DEFINES constants. The Linux build
compiles the game in strict ISO mode (so POSIX names such as random() and
strnlen() cannot collide with the game's own), which hides glibc's M_*.
*/

#ifndef __HALO_LINUX_MATH_H
#define __HALO_LINUX_MATH_H

#include_next <math.h>

#ifndef M_PI
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.707106781186547524401
#endif

double _hypot(double x, double y);
double _copysign(double x, double y);

/* sin, pow and the rest, the same on every port */
#include "../../include/halo_math.h"

#endif
