/*
PARTICLE_SYSTEMS.H

header included in hcex build.
*/

#ifndef __PARTICLE_SYSTEMS_H
#define __PARTICLE_SYSTEMS_H
#pragma once

#include "cseries/cseries.h"
#include "effects/particle_system_definitions.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/objects.h"

/* ---------- constants */

enum
{
	_particle_system_active_bit,
	_particle_system_initializing_bit,
};

enum
{
	MAXIMUM_PARTICLE_SYSTEM_TYPES_PER_SYSTEM = 4,
};

/* ---------- macros */

#define particle_system_get(system_index) ((struct particle_system_datum *)datum_get(particle_systems, (system_index)))
#define ps_particle_get(particle_index) ((struct ps_particle_datum *)datum_get(system_particles, (particle_index)))

/* ---------- structures */

struct explosion_system_physics_state
{
	boolean grounded;
};

union particle_system_physics_state
{
	struct explosion_system_physics_state explosion;
	long unused;
};

struct particle_type
{
	short state_index;
	short transition_state_index;
	real time_left_in_state;
	real state_length;
	struct particle_system_type_state_interpolated_variables variables;
	real fractional_particle_count;
	boolean states_moving_forward;
	byte reserved39;
	short particle_count;
	long first_particle_index;
};

struct particle_system_datum
{
	struct datum_header header;
	short pad;
	unsigned long flags;
	long definition_index;
	long object_index;
	short attachment_index;
	short function_index;
	real scale;
	struct location location;
	real_point3d position;
	real_vector3d velocity;
	real_argb_color color;
	real_rgb_color lighting;
	union particle_system_physics_state physics_state;
	struct particle_type types[MAXIMUM_PARTICLE_SYSTEM_TYPES_PER_SYSTEM];
};

struct ps_particle_datum
{
	struct datum_header header;
	boolean states_moving_forward;
	boolean valid;
	long next_particle_index;
	short state_index;
	short transition_state_index;
	real time_left_in_state;
	real state_length;
	struct location location;
	real_point3d position;
	real_vector3d velocity;
	real_vector3d axis;
	real rotation;
	real sprite_index;
	struct particle_system_type_particle_state_interpolated_randomized_variables randomized_variables;
	struct particle_system_type_particle_state_interpolated_randomized_variables transition_randomized_variables;
};

typedef char particle_type_size_assert[
	sizeof(struct particle_type) == 0x40 ? 1 : -1];
typedef char particle_type_state_index_offset_assert[
	offsetof(struct particle_type, state_index) == 0x0 ? 1 : -1];
typedef char particle_type_transition_state_index_offset_assert[
	offsetof(struct particle_type, transition_state_index) == 0x2 ? 1 : -1];
typedef char particle_type_time_left_in_state_offset_assert[
	offsetof(struct particle_type, time_left_in_state) == 0x4 ? 1 : -1];
typedef char particle_type_state_length_offset_assert[
	offsetof(struct particle_type, state_length) == 0x8 ? 1 : -1];
typedef char particle_type_states_moving_forward_offset_assert[
	offsetof(struct particle_type, states_moving_forward) == 0x38 ? 1 : -1];
typedef char particle_type_variables_offset_assert[
	offsetof(struct particle_type, variables) == 0xC ? 1 : -1];
typedef char particle_type_fractional_particle_count_offset_assert[
	offsetof(struct particle_type, fractional_particle_count) == 0x34 ? 1 : -1];
typedef char particle_type_particle_count_offset_assert[
	offsetof(struct particle_type, particle_count) == 0x3A ? 1 : -1];
typedef char particle_type_first_particle_index_offset_assert[
	offsetof(struct particle_type, first_particle_index) == 0x3C ? 1 : -1];
typedef char particle_system_datum_size_assert[
	sizeof(struct particle_system_datum) == 0x158 ? 1 : -1];
typedef char particle_system_datum_flags_offset_assert[
	offsetof(struct particle_system_datum, flags) == 0x4 ? 1 : -1];
typedef char particle_system_datum_definition_index_offset_assert[
	offsetof(struct particle_system_datum, definition_index) == 0x8 ? 1 : -1];
typedef char particle_system_datum_object_index_offset_assert[
	offsetof(struct particle_system_datum, object_index) == 0xC ? 1 : -1];
typedef char particle_system_datum_scale_offset_assert[
	offsetof(struct particle_system_datum, scale) == 0x14 ? 1 : -1];
typedef char particle_system_datum_position_offset_assert[
	offsetof(struct particle_system_datum, position) == 0x20 ? 1 : -1];
typedef char particle_system_datum_location_offset_assert[
	offsetof(struct particle_system_datum, location) == 0x18 ? 1 : -1];
typedef char particle_system_datum_velocity_offset_assert[
	offsetof(struct particle_system_datum, velocity) == 0x2C ? 1 : -1];
typedef char particle_system_datum_color_offset_assert[
	offsetof(struct particle_system_datum, color) == 0x38 ? 1 : -1];
typedef char particle_system_datum_lighting_offset_assert[
	offsetof(struct particle_system_datum, lighting) == 0x48 ? 1 : -1];
typedef char particle_system_datum_types_offset_assert[
	offsetof(struct particle_system_datum, types) == 0x58 ? 1 : -1];
typedef char particle_system_datum_physics_state_offset_assert[
	offsetof(struct particle_system_datum, physics_state) == 0x54 ? 1 : -1];
typedef char ps_particle_datum_size_assert[
	sizeof(struct ps_particle_datum) == 0x80 ? 1 : -1];
typedef char ps_particle_datum_next_particle_index_offset_assert[
	offsetof(struct ps_particle_datum, next_particle_index) == 0x4 ? 1 : -1];
typedef char ps_particle_datum_state_index_offset_assert[
	offsetof(struct ps_particle_datum, state_index) == 0x8 ? 1 : -1];
typedef char ps_particle_datum_transition_state_index_offset_assert[
	offsetof(struct ps_particle_datum, transition_state_index) == 0xA ? 1 : -1];
typedef char ps_particle_datum_time_left_in_state_offset_assert[
	offsetof(struct ps_particle_datum, time_left_in_state) == 0xC ? 1 : -1];
typedef char ps_particle_datum_state_length_offset_assert[
	offsetof(struct ps_particle_datum, state_length) == 0x10 ? 1 : -1];
typedef char ps_particle_datum_location_offset_assert[
	offsetof(struct ps_particle_datum, location) == 0x14 ? 1 : -1];
typedef char ps_particle_datum_position_offset_assert[
	offsetof(struct ps_particle_datum, position) == 0x1C ? 1 : -1];
typedef char ps_particle_datum_velocity_offset_assert[
	offsetof(struct ps_particle_datum, velocity) == 0x28 ? 1 : -1];
typedef char ps_particle_datum_axis_offset_assert[
	offsetof(struct ps_particle_datum, axis) == 0x34 ? 1 : -1];
typedef char ps_particle_datum_randomized_variables_offset_assert[
	offsetof(struct ps_particle_datum, randomized_variables) == 0x48 ? 1 : -1];
typedef char ps_particle_datum_transition_randomized_variables_offset_assert[
	offsetof(struct ps_particle_datum, transition_randomized_variables) == 0x64 ? 1 : -1];

/* ---------- prototypes/PARTICLE_SYSTEMS.C */

void particle_systems_initialize(
	void);
void particle_systems_initialize_for_new_map(
	void);
void particle_systems_dispose_from_old_map(
	void);
void particle_systems_dispose(
	void);
void particle_systems_disconnect_from_structure_bsp(
	void);
void particle_systems_reconnect_to_structure_bsp(
	void);
void particle_systems_update(
	real delta_time);
void particle_systems_render(
	void);
long particle_system_new_attached(
	long definition_index,
	long object_index,
	short attachment_index);
long particle_system_new_unattached(
	long definition_index,
	real_point3d const *position,
	real_vector3d const *velocity,
	real_argb_color const *color,
	real scale);

void particle_system_orphan(
	long system_index);

/* ---------- globals */

extern struct data_array *particle_systems;
extern struct data_array *system_particles;

/* ---------- public code */

#endif // __PARTICLE_SYSTEMS_H
