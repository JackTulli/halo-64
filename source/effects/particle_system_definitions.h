/*
PARTICLE_SYSTEM_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __PARTICLE_SYSTEM_DEFINITIONS_H
#define __PARTICLE_SYSTEM_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "shaders/shader_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	PARTICLE_SYSTEM_DEFINITION_TAG = 'pctl',
	PARTICLE_SYSTEM_DEFINITION_VERSION = 4,
};

enum particle_system_type_flags
{
	_particle_system_type_type_states_loop_bit = 0,
	_particle_system_type_type_states_loop_forward_backward_bit,
	_particle_system_type_particle_states_loop_bit,
	_particle_system_type_particle_states_loop_forward_backward_bit,
	_particle_system_type_dies_in_water_bit,
	_particle_system_type_dies_in_air_bit,
	_particle_system_type_dies_on_ground_bit,
	_particle_system_type_rotational_sprites_animate_sideways_bit,
	_particle_system_type_disabled_bit,
	_particle_system_type_tint_by_effect_color_bit,
	_particle_system_type_initial_count_scales_bit,
	_particle_system_type_minimum_count_scales_bit,
	_particle_system_type_creation_rate_scales_bit,
	_particle_system_type_scale_scales_bit,
	_particle_system_type_animation_rate_scales_bit,
	_particle_system_type_rotation_rate_scales_bit,
	_particle_system_type_nodraw_first_person_bit,
	_particle_system_type_nodraw_third_person_bit,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_FLAGS,
};

enum explosion_type_definition_physics_constant
{
	_explosion_type_definition_physics_constant_xy_spread,
	_explosion_type_definition_physics_constant_z_spread,
	_explosion_type_definition_physics_constant_intensity,
	NUMBER_OF_EXPLOSION_TYPE_DEFINITION_PHYSICS_CONSTANTS,
};

enum jet_type_definition_physics_constant
{
	_jet_type_definition_physics_constant_velocity,
	_jet_type_definition_physics_constant_spread_fraction,
	_jet_type_definition_physics_constant_rotates_up,
	NUMBER_OF_JET_TYPE_DEFINITION_PHYSICS_CONSTANTS,
};

enum particle_system_update_physics
{
	_particle_system_update_physics_default,
	_particle_system_update_physics_explosion,
	NUMBER_OF_PARTICLE_SYSTEM_UPDATE_PHYSICS,
};

enum particle_system_type_update_physics
{
	_particle_system_type_update_physics_default,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_UPDATE_PHYSICS,
};

enum particle_system_type_create
{
	_particle_system_type_create_default,
	_particle_system_type_create_explosion,
	_particle_system_type_create_jet,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_CREATION_PHYSICS,
};

enum particle_system_type_complex_sprite_render_mode
{
	_particle_system_type_complex_sprite_render_mode_simple,
	_particle_system_type_complex_sprite_render_mode_rotational,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_COMPLEX_SPRITE_RENDER_MODES,
};

/* ---------- macros */

#define particle_system_definition_get(index) ((struct particle_system_definition *)tag_get(PARTICLE_SYSTEM_DEFINITION_TAG, (index)))

/* ---------- structures */

struct particle_system_physics_constant
{
	real k;
};

struct particle_system_type_particle_state_interpolated_randomized_variables
{
	real scale;
	real animation_rate;
	real rotation_rate;
	real_argb_color color;
};

struct particle_system_type_particle_state_interpolated_variables
{
	real radius;
};

struct particle_system_type_state_interpolated_variables
{
	struct particle_system_type_particle_state_interpolated_randomized_variables particle_state_randomized_multipliers;
	struct particle_system_type_particle_state_interpolated_variables particle_state_multipliers;
	real minimum_particle_count;
	real particle_creation_rate;
};

struct particle_system_type_state
{
	char name[32];
	real duration_lower_bound;
	real duration_upper_bound;
	real transition_time_lower_bound;
	real transition_time_upper_bound;
	unsigned long flags;
	struct particle_system_type_state_interpolated_variables variables;
	long unused5C[21];
	short particle_creation_physics;
	short particle_update_physics;
	struct tag_block physics_constants;
};

struct particle_system_type_particle_state
{
	char name[32];
	real duration_lower_bound;
	real duration_upper_bound;
	real transition_time_lower_bound;
	real transition_time_upper_bound;
	struct tag_reference bitmaps;
	short sequence_index;
	word pad42;
	long unused44;
	real scale_lower_bound;
	real scale_upper_bound;
	real animation_rate_lower_bound;
	real animation_rate_upper_bound;
	real rotation_rate_lower_bound;
	real rotation_rate_upper_bound;
	real_argb_color color_lower_bound;
	real_argb_color color_upper_bound;
	struct particle_system_type_particle_state_interpolated_variables variables;
	struct tag_reference point_physics;
	byte reserved94[0x24];
	struct shader_effect_definition shader;
	byte reserved16C[0xC];
};

struct old_particle_system_type
{
	byte reserved00[0x68];
	struct tag_block type_states;
	byte reserved74[0xC];
};

struct particle_system_type
{
	char name[32];
	unsigned long flags;
	short initial_particle_count;
	word pad26;
	short complex_sprite_render_mode;
	short sprite_render_mode;
	struct particle_system_type_particle_state_interpolated_variables variables;
	byte reserved30[0x24];
	short initial_particle_creation_physics;
	word pad56;
	unsigned long physics_flags;
	struct tag_block physics_constants;
	struct tag_block type_states;
	struct tag_block particle_states;
};

struct particle_system_definition
{
	byte reserved00[0x38];
	struct tag_reference system_update_point_physics;
	short system_update_physics;
	word pad4A;
	unsigned long physics_flags;
	struct tag_block physics_constants;
	struct tag_block types;
};

typedef char particle_system_definition_size_assert[
	sizeof(struct particle_system_definition) == 0x68 ? 1 : -1];
typedef char particle_system_definition_system_update_point_physics_offset_assert[
	offsetof(struct particle_system_definition, system_update_point_physics) == 0x38 ? 1 : -1];
typedef char particle_system_definition_system_update_point_physics_index_offset_assert[
	offsetof(struct particle_system_definition, system_update_point_physics.index) == 0x44 ? 1 : -1];
typedef char particle_system_definition_system_update_physics_offset_assert[
	offsetof(struct particle_system_definition, system_update_physics) == 0x48 ? 1 : -1];
typedef char particle_system_definition_types_offset_assert[
	offsetof(struct particle_system_definition, types) == 0x5C ? 1 : -1];
typedef char old_particle_system_type_size_assert[
	sizeof(struct old_particle_system_type) == 0x80 ? 1 : -1];
typedef char old_particle_system_type_type_states_offset_assert[
	offsetof(struct old_particle_system_type, type_states) == 0x68 ? 1 : -1];
typedef char particle_system_type_size_assert[
	sizeof(struct particle_system_type) == 0x80 ? 1 : -1];
typedef char particle_system_type_flags_offset_assert[
	offsetof(struct particle_system_type, flags) == 0x20 ? 1 : -1];
typedef char particle_system_type_physics_constants_offset_assert[
	offsetof(struct particle_system_type, physics_constants) == 0x5C ? 1 : -1];
typedef char particle_system_type_type_states_offset_assert[
	offsetof(struct particle_system_type, type_states) == 0x68 ? 1 : -1];
typedef char particle_system_type_particle_states_offset_assert[
	offsetof(struct particle_system_type, particle_states) == 0x74 ? 1 : -1];
typedef char particle_system_physics_constant_size_assert[
	sizeof(struct particle_system_physics_constant) == sizeof(real) ? 1 : -1];
typedef char particle_system_type_particle_state_interpolated_randomized_variables_size_assert[
	sizeof(struct particle_system_type_particle_state_interpolated_randomized_variables) == 0x1C ? 1 : -1];
typedef char particle_system_type_particle_state_interpolated_variables_size_assert[
	sizeof(struct particle_system_type_particle_state_interpolated_variables) == sizeof(real) ? 1 : -1];
typedef char particle_system_type_state_interpolated_variables_size_assert[
	sizeof(struct particle_system_type_state_interpolated_variables) == 0x28 ? 1 : -1];
typedef char particle_system_type_particle_state_size_assert[
	sizeof(struct particle_system_type_particle_state) == 0x178 ? 1 : -1];
typedef char particle_system_type_particle_state_scale_lower_bound_offset_assert[
	offsetof(struct particle_system_type_particle_state, scale_lower_bound) == 0x48 ? 1 : -1];
typedef char particle_system_type_particle_state_color_lower_bound_offset_assert[
	offsetof(struct particle_system_type_particle_state, color_lower_bound) == 0x60 ? 1 : -1];
typedef char particle_system_type_particle_state_variables_offset_assert[
	offsetof(struct particle_system_type_particle_state, variables) == 0x80 ? 1 : -1];
typedef char particle_system_type_particle_state_point_physics_offset_assert[
	offsetof(struct particle_system_type_particle_state, point_physics) == 0x84 ? 1 : -1];
typedef char particle_system_type_particle_state_shader_offset_assert[
	offsetof(struct particle_system_type_particle_state, shader) == 0xB8 ? 1 : -1];
typedef char particle_system_type_state_size_assert[
	sizeof(struct particle_system_type_state) == 0xC0 ? 1 : -1];
typedef char particle_system_type_state_duration_lower_bound_offset_assert[
	offsetof(struct particle_system_type_state, duration_lower_bound) == 0x20 ? 1 : -1];
typedef char particle_system_type_state_duration_upper_bound_offset_assert[
	offsetof(struct particle_system_type_state, duration_upper_bound) == 0x24 ? 1 : -1];
typedef char particle_system_type_state_variables_offset_assert[
	offsetof(struct particle_system_type_state, variables) == 0x34 ? 1 : -1];
typedef char particle_system_type_state_particle_update_physics_offset_assert[
	offsetof(struct particle_system_type_state, particle_update_physics) == 0xB2 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __PARTICLE_SYSTEM_DEFINITIONS_H
