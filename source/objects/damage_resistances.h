/*
DAMAGE_RESISTANCES.H

header included in hcex build.
*/

#ifndef __DAMAGE_RESISTANCES_H
#define __DAMAGE_RESISTANCES_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct damage_resistance_material
{
	char name[TAG_STRING_LENGTH+1];
	unsigned long flags;
	short material_type;
	word pad;
	real shield_leak_fraction;
	real shield_damage_multiplier;
	real shield_unused[3];
	real body_damage_multiplier;
	long body_unused[2];
};

typedef char damage_resistance_material_size_assert[
	sizeof(struct damage_resistance_material) == 0x48 ? 1 : -1];
typedef char damage_resistance_material_type_offset_assert[
	offsetof(struct damage_resistance_material, material_type) == 0x24 ? 1 : -1];
typedef char damage_resistance_material_shield_leak_fraction_offset_assert[
	offsetof(struct damage_resistance_material, shield_leak_fraction) == 0x28 ? 1 : -1];
typedef char damage_resistance_material_body_damage_multiplier_offset_assert[
	offsetof(struct damage_resistance_material, body_damage_multiplier) == 0x3C ? 1 : -1];

struct damage_resistance
{
	unsigned long flags;
	short indirect_damage_material_index;
	word pad2;
	real maximum_body_vitality;
	real body_system_shock;
	long body_vitality_unused[6];
	real body_stun_unused[7];
	real friendly_damage_resistance;
	long friendly_damage_unused[2];
	long body_unused[8];
	struct tag_reference localized_damage_effect;
	real area_damage_effect_threshold;
	struct tag_reference area_damage_effect;
	real body_damaged_effect_threshold;
	struct tag_reference body_damaged_effect;
	struct tag_reference body_depleted_effect;
	real body_destroyed_threshold;
	struct tag_reference body_destroyed_effect;
	real maximum_shield_vitality;
	word pad0;
	short shield_material_type;
	long shield_vitality_unused[6];
	short shield_failure_function;
	word pad1;
	real shield_failure_threshold;
	real maximum_shield_failure;
	long shield_failure_unused[4];
	real minimum_shield_stun_damage;
	real shield_stun_time;
	real shield_recharge_time;
	real shield_recharge_unused[4];
	long shield_unused[24];
	real shield_damaged_effect_threshold;
	struct tag_reference shield_damaged_effect;
	struct tag_reference shield_depleted_effect;
	struct tag_reference shield_recharging_effect;
	unsigned long unused2[2];
	real runtime_shield_recharge_velocity;
	unsigned long unused[28];
	struct tag_block materials;	// damage_resistance_material
	struct tag_block regions;
	struct tag_block modifiers;
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __DAMAGE_RESISTANCES_H
