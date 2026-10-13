/*
MATERIAL_EFFECT_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __MATERIAL_EFFECT_DEFINITIONS_H
#define __MATERIAL_EFFECT_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "real_math.h"
#include "tag_groups.h"

/* ---------- constants */

enum
{
	MATERIAL_EFFECTS_TAG = 'foot',
};

/* ---------- macros */

#define material_effects_definition_get(index) ((struct material_effects_definition *)tag_get(MATERIAL_EFFECTS_TAG, (index)))

/* ---------- structures */

struct material_effect_material
{
	struct tag_reference effect;
	struct tag_reference sound;
	byte pad20[0x10];
};

struct material_effect
{
	struct tag_block materials;
	byte padC[0x10];
};

struct material_effects_definition
{
	struct tag_block effects;
};

typedef char material_effect_material_size_assert[
	sizeof(struct material_effect_material) == 0x30 ? 1 : -1];
typedef char material_effect_material_effect_offset_assert[
	offsetof(struct material_effect_material, effect) == 0x00 ? 1 : -1];
typedef char material_effect_material_sound_offset_assert[
	offsetof(struct material_effect_material, sound) == 0x10 ? 1 : -1];
typedef char material_effect_size_assert[
	sizeof(struct material_effect) == 0x1C ? 1 : -1];
typedef char material_effect_materials_offset_assert[
	offsetof(struct material_effect, materials) == 0x00 ? 1 : -1];
typedef char material_effects_definition_effects_offset_assert[
	offsetof(struct material_effects_definition, effects) == 0x00 ? 1 : -1];

struct breakable_surface
{
	real maximum_vitality; 
	long unused1[2]; 
	unsigned long flags; 
	struct tag_reference effect; 
	struct tag_reference sound; 
	long unused2[6]; 
	struct tag_block particle_effects; 
};

struct material_definition
{
	unsigned long flags;
	long modifiers_unused[24];
	unsigned long biped_flags;
	real biped_maximum_acceleration;
	real biped_slip_angle;
	real biped_slow_angle;
	long biped_unused[8];
	real physics_ground_friction_scale;
	real physics_ground_friction_normal_k1_scale;
	real physics_ground_friction_normal_k0_scale;
	real physics_ground_depth_scale;
	real physics_ground_damp_fraction_scale;
	real physics_unused[19];
	long unused[120];
	struct breakable_surface breakable_surface;
	long unused2[15];
	struct tag_reference melee_hit_sound;
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __MATERIAL_EFFECT_DEFINITIONS_H
