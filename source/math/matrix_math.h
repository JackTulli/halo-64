/*
MATRIX_MATH.H

focused owner header for matrix_math.c routines that real_math.h does not
declare; adding them to the broad real_math.h regresses exact consumers
through declaration-count sensitivity.
*/

#ifndef __MATRIX_MATH_H
#define __MATRIX_MATH_H
#pragma once

#include "cseries/cseries.h"
#include "math/real_math.h"

real_matrix3x3 *matrix3x3_multiply(
	real_matrix3x3 const *a,
	real_matrix3x3 const *b,
	real_matrix3x3 *result);

#endif /* __MATRIX_MATH_H */
