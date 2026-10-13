/*
COLLISION_DEBUG.H

header included in hcex build.
*/

#ifndef __COLLISION_DEBUG_H
#define __COLLISION_DEBUG_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/COLLISION_DEBUG.C */

void collision_debug_render(
	void);

/* ---------- globals */

extern real_point3d collision_debug_point;
extern boolean collision_debug_repeat;
extern real_vector3d collision_debug_vector;
extern real collision_debug_length;
extern real collision_debug_width;
extern real collision_debug_height;

/* ---------- public code */

#endif // __COLLISION_DEBUG_H
