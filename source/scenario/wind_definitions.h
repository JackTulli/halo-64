/*
WIND_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __WIND_DEFINITIONS_H
#define __WIND_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

enum
{
	WIND_TAG = 'wind',
};

/* ---------- macros */

#define wind_definition_get(index) ((struct wind_definition *)tag_get(WIND_TAG, (index)))

/* ---------- structures */

struct wind_definition
{
	real velocity_lower_bound;
	real velocity_upper_bound;
	real_euler_angles2d variation_area;
	real local_variation_weight;
	real local_variation_rate;
	real damping;
	long unused[9];
};

typedef char wind_definition_size_assert[
	sizeof(struct wind_definition) == 0x40 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __WIND_DEFINITIONS_H
