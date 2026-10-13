/*
DECAL_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __DECAL_DEFINITIONS_H
#define __DECAL_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	DECAL_GROUP_TAG = 'deca',
};

enum decal_definition_flag
{
	_decal_definition_geometry_inherited_by_next_decal_in_chain_bit,
	_decal_definition_color_interpolate_in_hsv_bit,
	_decal_definition_color_interpolate_along_farthest_hue_path_bit,
	_decal_definition_no_random_rotation_bit,
	_decal_definition_water_effect_bit,
	_decal_definition_SAPIEN_ONLY_snap_to_axis_bit,
	_decal_definition_SAPIEN_ONLY_incremental_counter_bit,
	_decal_definition_animation_loop_bit,
	_decal_definition_preserve_aspect_bit,
	NUMBER_OF_DECAL_DEFINITION_FLAGS
};

enum decal_type
{
	_decal_type_scratch,
	_decal_type_splatter,
	_decal_type_burn,
	_decal_type_painted_sign,
	NUMBER_OF_DECAL_TYPES
};

/* ---------- macros */

#define decal_definition_get(index) ((struct decal_definition *)tag_get(DECAL_GROUP_TAG, (index)))

/* ---------- structures */

struct decal_definition;

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __DECAL_DEFINITIONS_H
