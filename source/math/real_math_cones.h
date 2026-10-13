#ifndef __REAL_MATH_CONES_H
#define __REAL_MATH_CONES_H
#pragma once

#include "math/real_math.h"

boolean pin_normal_to_cone3d(
	real_vector3d const *normal,
	real_vector3d const *direction,
	real sine,
	real cosine,
	real_vector3d *result);

#endif /* __REAL_MATH_CONES_H */
