/*
PROJECTILE_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __PROJECTILE_DEFINITIONS_H
#define __PROJECTILE_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "objects/object_definitions.h"

/* ---------- constants */

enum
{
	PROJECTILE_DEFINITION_TAG = 'proj',
};

enum
{
	_projectile_material_response_disappear = 0,
	_projectile_material_response_detonate,
	_projectile_material_response_reflect,
	_projectile_material_response_overpenetrate,
	_projectile_material_response_attach,
	NUMBER_OF_PROJECTILE_MATERIAL_RESPONSES,
};

enum
{
	_projectile_material_effect_scale_damage = 0,
	_projectile_material_effect_scale_angle,
	NUMBER_OF_PROJECTILE_MATERIAL_EFFECT_SCALES,
};

/* ---------- macros */

#define projectile_definition_get(index) ((struct projectile_definition *)tag_get(PROJECTILE_DEFINITION_TAG, index))

/* ---------- structures */

struct projectile_material_response_definition
{
	word flags;
	short default_response;
	struct tag_reference default_effect;
	byte pad0[16];
	short potential_response;
	word potential_flags;
	real potential_skip_fraction;
	real potential_angle_lower_bound;
	real potential_angle_upper_bound;
	real potential_velocity_lower_bound;
	real potential_velocity_upper_bound;
	struct tag_reference potential_effect;
	byte pad1[16];
	short scale_effects_by;
	word pad2;
	real angular_noise;
	real velocity_noise;
	struct tag_reference detonation_effect;
	byte pad3[24];
	real initial_friction;
	real maximum_distance;
	real parallel_friction;
	real perpendicular_friction;
};

struct _projectile_definition
{
	unsigned long flags;
	short detonation_timer_starts;
	short impact_noise;
	short function_inputs[4];
	struct tag_reference super_detonation;
	real ai_perception_radius;
	real collision_radius;
	real arming_time;
	real danger_radius;
	struct tag_reference effect;
	real timer_lower_bound;
	real timer_upper_bound;
	real minimum_velocity;
	real maximum_range;
	real air_gravity_scale;
	real air_damage_range_lower_bound;
	real air_damage_range_upper_bound;
	real water_gravity_scale;
	real water_damage_range_lower_bound;
	real water_damage_range_upper_bound;
	real initial_velocity;
	real final_velocity;
	real guided_angular_velocity;
	short detonation_noise;
	word pad;
	struct tag_reference detonation_started;
	struct tag_reference flyby_sound;
	struct tag_reference attached_detonation_damage;
	struct tag_reference impact_damage;
	long unused[3];
	struct tag_block material_responses;
};

struct projectile_definition
{
	struct _object_definition object;
	struct _projectile_definition projectile;
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

extern struct projectile_material_response_definition default_projectile_material_response;

/* ---------- public code */

#endif // __PROJECTILE_DEFINITIONS_H
