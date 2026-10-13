/*
CONTRAIL_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __CONTRAIL_DEFINITIONS_H
#define __CONTRAIL_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"
#include "shaders/shader_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	CONTRAIL_DEFINITION_TAG = 'cont',
	CONTRAIL_DEFINITION_VERSION = 3,
	MAXIMUM_CONTRAIL_POINT_STATES_PER_CONTRAIL = 16,
};

enum
{
	_contrail_first_point_unfaded_bit = 0,
	_contrail_last_point_unfaded_bit,
	_contrail_fades_slowly_bit = 6,
};

enum
{
	_contrail_state_width_bit = 4,
	_contrail_state_color_bit,
};

enum
{
	_contrail_texture_repeats_u_bit = 6,
	_contrail_texture_repeats_v_bit,
};

enum
{
	_contrail_render_type_vertical = 0,
	_contrail_render_type_horizontal,
	_contrail_render_type_media,
	_contrail_render_type_ground,
	_contrail_render_type_viewer,
};

/* ---------- macros */

#define contrail_definition_get(index) \
	((struct contrail_definition *)tag_get(CONTRAIL_DEFINITION_TAG, (index)))

/* ---------- structures */

struct contrail_point_state
{
	real duration_lower_bound;
	real duration_upper_bound;
	real transition_duration_lower_bound;
	real transition_duration_upper_bound;
	struct tag_reference physics;
	long reserved20[8];
	real width;
	real_argb_color color_lower_bound;
	real_argb_color color_upper_bound;
	unsigned long scale_flags;
};

struct contrail_shader
{
	struct shader shader;
	byte reserved28[4];
	short framebuffer_fade_mode;
	byte reserved2E[0x86];
};

struct contrail_definition
{
	word flags;
	word scale_flags;
	real point_generation_rate;
	real point_velocity_lower_bound;
	real point_velocity_upper_bound;
	real point_velocity_cone_angle;
	real point_inherited_velocity_fraction;
	short render_type;
	word pad1A;
	real texture_repeats_u;
	real texture_repeats_v;
	real texture_animation_u;
	real texture_animation_v;
	real frames_per_second;
	struct tag_reference bitmap;
	short first_sequence_index;
	short sequence_count;
	long reserved44[16];
	struct contrail_shader shader;
	struct tag_block states;
};

typedef char verify_contrail_point_state_size[
	sizeof(struct contrail_point_state) == 0x68 ? 1 : -1];
typedef char verify_contrail_definition_size[
	sizeof(struct contrail_definition) == 0x144 ? 1 : -1];
typedef char verify_contrail_definition_states_offset[
	offsetof(struct contrail_definition, states) == 0x138 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __CONTRAIL_DEFINITIONS_H
