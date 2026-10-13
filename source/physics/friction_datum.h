/*
FRICTION_DATUM.H
*/

#ifndef __FRICTION_DATUM_H
#define __FRICTION_DATUM_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- structures */

struct friction_datum
{
	real_vector3d friction;
	real_vector3d parallel;
	real_vector3d perpendicular;
};

#endif // __FRICTION_DATUM_H
