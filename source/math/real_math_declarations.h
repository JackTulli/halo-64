/*
REAL_MATH_DECLARATIONS.H
*/

#ifndef __REAL_MATH_DECLARATIONS_H
#define __REAL_MATH_DECLARATIONS_H
#pragma once

/* External declarations for real-math helpers that are normally supplied as
 * non-static inline definitions by real_math.h. */

real normalize2d(
	real_vector2d *vector);
real normalize3d(
	real_vector3d *vector);
real real_random_range(
	real lower_bound,
	real upper_bound);
real distance_squared3d(
	real_point3d const *a,
	real_point3d const *b);
real distance3d(
	real_point3d const *a,
	real_point3d const *b);
real_vector3d *vector_from_points3d(
	real_point3d const *a,
	real_point3d const *b,
	real_vector3d *result);
real_point3d *point_from_line3d(
	real_point3d const *p,
	real_vector3d const *v,
	real t,
	real_point3d *result);
real magnitude_squared3d(
	real_vector3d const *v);

#endif /* __REAL_MATH_DECLARATIONS_H */
