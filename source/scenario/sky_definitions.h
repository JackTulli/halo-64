/*
SKY_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __SKY_DEFINITIONS_H
#define __SKY_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_files.h"

/* ---------- constants */

#define SKY_DEFINITION_TAG 'sky '

/* ---------- macros */

#define sky_definition_get(index) ((struct sky *)tag_get(SKY_DEFINITION_TAG, (index)))

/* ---------- structures */

struct sky_atmospheric_fog
{
	real_rgb_color color;
	byte unused0C[8];
	real maximum_density;
	real start_distance;
	real opaque_distance;
};

typedef char sky_atmospheric_fog_size_assert[
	sizeof(struct sky_atmospheric_fog) == 0x20 ? 1 : -1];

struct sky
{
	byte unused00[0x58];
	struct sky_atmospheric_fog outdoor_fog;
	struct sky_atmospheric_fog indoor_fog;
	struct tag_reference indoor_fog_screen;
};

typedef char sky_indoor_fog_offset_assert[
	offsetof(struct sky, indoor_fog) == 0x78 ? 1 : -1];
typedef char sky_indoor_fog_screen_offset_assert[
	offsetof(struct sky, indoor_fog_screen) == 0x98 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __SKY_DEFINITIONS_H
