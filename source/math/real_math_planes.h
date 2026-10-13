/*
REAL_MATH_PLANES.H

focused owner header for real_math.c plane-intersection routines that
real_math.h does not declare; adding them to the broad real_math.h regresses
exact consumers through declaration-count sensitivity.
*/

#ifndef __REAL_MATH_PLANES_H
#define __REAL_MATH_PLANES_H
#pragma once

#include "cseries/cseries.h"
#include "math/real_math.h"

boolean line_from_planes3d(
	real_plane3d const *plane0,
	real_plane3d const *plane1,
	real_point3d *point,
	real_vector3d *direction);

#endif /* __REAL_MATH_PLANES_H */
