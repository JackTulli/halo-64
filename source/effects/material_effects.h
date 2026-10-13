/*
MATERIAL_EFFECTS.H

header included in hcex build.
*/

#ifndef __MATERIAL_EFFECTS_H
#define __MATERIAL_EFFECTS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct location;

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

extern boolean debug_material_effects;

/* ---------- public code */

boolean material_effect_visible(
	real_point3d const *position);
void material_effect_new(
	long definition_index,
	short effect_index,
	short material_index,
	real_point3d const *position,
	real_vector3d const *normal,
	struct location const *location,
	real scale);
void material_effect_new_from_point(
	long definition_index,
	short effect_index,
	real_point3d const *position,
	real scale);

#endif // __MATERIAL_EFFECTS_H
