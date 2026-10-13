/*
STRUCTURE_VECTOR_TESTS.H

Narrow cross-translation-unit vector-test interface owned by STRUCTURES.C.
*/

#ifndef __STRUCTURE_VECTOR_TESTS_H
#define __STRUCTURE_VECTOR_TESTS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- structures */

union real_point3d;
union real_vector3d;

/* ---------- prototypes/STRUCTURES.C */

boolean structure_test_vector(
	union real_point3d const *point,
	union real_vector3d const *vector,
	union real_point3d *collision_point,
	short *lightmap_index,
	short *material_index,
	long *surface_index,
	real *s,
	real *t);

#endif /* __STRUCTURE_VECTOR_TESTS_H */
