/*
DAMAGE.C

symbols in this file:
00125DD0 0010:
	_damage_initialize (0000)
00125DE0 0010:
	_damage_dispose (0000)
00125DF0 0010:
	_damage_initialize_for_new_map (0000)
00125E00 0010:
	_damage_dispose_from_old_map (0000)
00125E10 0010:
	_damage_render_debug (0000)
00125E20 00e0:
	_object_initialize_vitality (0000)
00125F00 0050:
	_object_get_maximum_body_vitality (0000)
00125F50 0050:
	_object_get_maximum_shield_vitality (0000)
00125FA0 0040:
	_damage_data_new (0000)
00125FE0 0050:
	_object_restore_body (0000)
00126030 0060:
	_object_double_charge_shield (0000)
00126090 0050:
	_object_destroy_notify_children (0000)
001260E0 0050:
	_get_player_index_from_object_or_parents (0000)
00126130 0050:
	_object_can_take_damage (0000)
00126180 0050:
	_object_cannot_take_damage (0000)
001261D0 0030:
	_object_set_ranged_attack_inhibited (0000)
00126200 0030:
	_object_set_melee_attack_inhibited (0000)
00126230 0020:
	_damage_effect_new_on_object (0000)
00126250 0080:
	_object_permutation_shield_regions (0000)
001262D0 0060:
	_object_get_actual_body_vitality (0000)
00126330 0060:
	_object_get_actual_shield_vitality (0000)
00126390 0080:
	_object_deplete_shield (0000)
00126410 0380:
	_object_damage_shield (0000)
00126790 0230:
	_object_damage_aftermath (0000)
001269C0 0200:
	_damage_effect_new_at_location (0000)
00126BC0 01d0:
	_render_debug_object_damage (0000)
00126D90 00e0:
	_object_deplete_body (0000)
00126E70 0070:
	_object_destroy (0000)
00126EE0 0140:
	_object_destroy_region (0000)
00127020 0550:
	_object_damage_body (0000)
00127570 07c0:
	_object_cause_damage (0000)
00127D30 0420:
	_object_damage_update (0000)
00128150 0530:
	_area_of_effect_cause_damage_to_object (0000)
00128680 0080:
	_area_of_effect_cause_damage (0000)
00289410 006f:
	??_C@_0GP@MMLJIPGP@damage_resistance?9?$DOshield_materi@ (0000)
00289480 0020:
	??_C@_0CA@CANDACNM@c?3?2halo?2SOURCE?2objects?2damage?4c?$AA@ (0000)
002894A0 0029:
	??_C@_0CJ@DEGDFOIJ@collision?4type?$DN?$DN_collision_resul@ (0000)
002894D0 0061:
	??_C@_0GB@CBCJEGGG@?$CFs?$HMnbody?5?$CF0?43f?$HMn?5?5current?5?$CF0?43f?$HM@ (0000)
00289534 002c:
	??_C@_0CM@MBIMGLAN@no?5object?5to?5debug?$HMn?$CIpoint?5and?5p@ (0000)
00289560 0009:
	??_C@_08KBLDDAKN@?$HOdamaged?$AA@ (0000)
0028956C 003b:
	??_C@_0DL@NODEPHOL@region_index?$DO?$DN0?5?$CG?$CG?5region_index?$DM@ (0000)
002895A8 004b:
	??_C@_0EL@KFMBAJDI@damage_material?9?$DOtype?$DO?$DN0?5?$CG?$CG?5dama@ (0000)
002895F4 0028:
	??_C@_0CI@HMLKKOKN@?$CFs?3?5?$CC?$CFs?$CC?5?$CC?$CFs?$CC?5k?$DN?$CF0?42f?5S?$FL?$CF3?42f?$FN?5B@ (0000)
00289620 0041:
	??_C@_0EB@BEIFMKJC@damaged_object_count?$DMsizeof?$CIdama@ (0000)
00289668 0053:
	??_C@_0FD@MEFHNJOJ@region_index?$DN?$DNNONE?5?$HM?$HM?5?$CIregion_in@ (0000)
002896BC 0004:
	__real@3a422e45 (0000)
00456E48 004c:
	?default_damage_material@?1??object_cause_damage@@9@9 (0000)
	_global_debug_damage_object_index (0048)
*/

/* ---------- headers */

#include "cseries.h"
#include "damage.h"
#include "ai/ai.h"
#include "effects/effects.h"
#include "effects/player_effects.h"
#include "game/cheats.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/game_engine.h"
#include "game/game_globals.h"
#include "game/game_statistics_internal.h"
#include "game/players.h"
#include "hs/object_lists.h"
#include "input/input.h"
#include "interface/hud_unit.h"
#include "items/items.h"
#include "items/projectiles.h"
#include "main/console.h"
#include "math/periodic_functions.h"
#include "damage_effect_definitions.h"
#include "object_definitions.h"
#include "object_types.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_bsp_definitions.h"
#include "physics/collision_model_definitions.h"
#include "physics/collisions.h"
#include "physics/collision_usage.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "scenario/scenario.h"
#include "tag_files/tag_files.h"
#include "text/draw_string.h"
#include "units/unit_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/vehicles.h"

#ifdef HALO_LINUX
/* network_game_globals.c's */
boolean network_game_distributed_client(void);
/* port/linux/game/network_damage.c's */
boolean network_damage_deals(struct damage_data const *damage, long object_index, short node_index,
	short region_index, short material_index, real_vector3d const *object_normal, boolean authorized);
boolean network_damage_replaying_kill(void);
void network_damage_player_effect(long player_index, struct damage_data const *damage, real total_damage);
void network_damage_aftermath(long object_index, struct damage_data const *damage, unsigned long being_damaged_flags,
	real shield_damage, real body_damage, real body_damage_multiplier, short body_part, short node_index,
	short region_index, short material_index);

/* set while a distributed client carries out a kill it does not decide (an
act of god: the host's word, network_distributed.c, or the world's) */
static boolean distributed_damage_authorized;

/* whether this machine runs objects' shields (their stun, recharge and
overcharge): not a client of the distributed netcode, which has the host's
(damage_set_network_state) */
#define objects_update_shields() (!network_game_distributed_client())
#else
#define objects_update_shields() TRUE
#endif

/* ---------- constants */

enum
{
	_object_region_lives_until_object_dies_bit = 0,
	_object_region_forces_object_to_die_bit,
	_object_region_dies_when_object_dies_bit,
	_object_region_dies_when_object_is_damaged_bit,
	_object_region_missing_when_shield_is_zero_bit,
	_object_region_inhibits_melee_attack_bit,
	_object_region_inhibits_ranged_attack_bit,
	_object_region_inhibits_walking_bit,
	_object_region_forces_drop_weapon_bit,
	_object_region_head_destroyed_scream_bit,
	NUMBER_OF_DAMAGE_REGION_FLAGS,
};

enum
{
	_damage_material_head_bit = 0,
};

enum
{
	_damage_category_none = 0,
	_damage_category_falling,
	_damage_category_bullet,
	_damage_category_grenade,
	_damage_category_highexplosive,
	_damage_category_sniper,
	_damage_category_melee,
	_damage_category_flame,
	_damage_category_mountedweapon,
	_damage_category_vehicle,
	_damage_category_plasma,
	_damage_category_needle,
	_damage_category_shotgun,
	NUMBER_OF_DAMAGE_CATEGORIES,
};

enum
{
	_effect_vector_normal = 0,
	_effect_vector_incident,
	_effect_vector_negative_incident,
	_effect_vector_reflected,
	_effect_vector_gravity,
	NUMBER_OF_EFFECT_MARKERS,
};

enum
{
	_damage_effect_dont_scale_damage_by_distance_bit = 0,
};

enum
{
	_damage_does_not_hurt_owner_bit = 0,
	_damage_can_cause_headshots_bit,
	_damage_pings_resistant_units_bit,
	_damage_does_not_hurt_friends_bit = 3,
	_damage_does_not_ping_units_bit,
	_damage_detonates_explosives_bit,
	_damage_only_hurts_shields_bit,
	_damage_causes_flaming_death_bit,
	_damage_draw_indicators_down_bit,
	_damage_skips_shields_bit,
	_damage_does_not_hurt_infection_forms_bit = 10,
	_damage_can_cause_multiplayer_headshots_bit,
	_damage_infection_form_pop_bit = 12,
};

enum
{
	_damage_resistance_takes_shield_damage_for_children_bit = 0,
	_damage_resistance_takes_body_damage_for_children_bit,
	_damage_resistance_always_shields_friendly_damage_bit,
	_damage_resistance_children_take_area_damage_bit = 3,
	_damage_resistance_parent_never_takes_body_damage_for_us_bit,
	_damage_resistance_only_hurt_by_explosives_bit,
	_damage_resistance_only_hurt_while_occupied_bit,
};

enum
{
	_damage_side_effect_none = 0,
	_damage_side_effect_harmless,
	_damage_side_effect_lethal_to_the_unsuspecting = 2,
	_damage_side_effect_emp,
};

enum
{
	_damage_area_of_effect_collision_flags =
		FLAG(_collision_test_front_facing_surfaces_bit) |
		FLAG(_collision_test_structure_bit) |
		FLAG(_collision_test_objects_vehicles_bit) |
		FLAG(_collision_test_objects_scenery_bit) |
		FLAG(_collision_test_objects_machines_bit),
};

/* ---------- macros */

/* ---------- structures */

struct damage_region
{
	char name[TAG_STRING_LENGTH+1];
	unsigned long flags;
	long unused0;
	real damage_threshold;
	long unused1[3];
	struct tag_reference destroyed_effect;
	struct tag_block permutations;
};

struct game_globals_falling_damage
{
	long unused0[2];
	real falling_distance_lower_bound;
	real falling_distance_upper_bound;
	struct tag_reference falling_damage;
	long terminal_velocity_unused[2];
	real maximum_distance;
	struct tag_reference maximum_distance_damage;
	struct tag_reference vehicle_hit_environment_damage_effect;
	struct tag_reference vehicle_killed_unit_damage_effect;
	struct tag_reference vehicle_collision_damage;
	struct tag_reference flaming_death_damage;
	long unused7c[4];
	real runtime_maximum_falling_velocity;
	real runtime_minimum_damage_velocity;
	real runtime_maximum_damage_velocity;
};

typedef char damage_region_size_assert[
	sizeof(struct damage_region) == 0x54 ? 1 : -1];
typedef char game_globals_falling_damage_size_assert[
	sizeof(struct game_globals_falling_damage) == 0x98 ? 1 : -1];
typedef char game_globals_falling_damage_effect_offset_assert[
	offsetof(struct game_globals_falling_damage, falling_damage) +
		offsetof(struct tag_reference, index) == 0x1C ? 1 : -1];

typedef char object_deplete_body_definition_index_offset_assert[
	offsetof(struct object_datum, definition_index) == 0x00 ? 1 : -1];
typedef char object_deplete_body_type_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, type) == 0x64 ? 1 : -1];
typedef char object_deplete_body_damage_flags_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, damage_flags) == 0xB6 ? 1 : -1];
typedef char object_deplete_body_next_object_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, next_object_index) == 0xC4 ? 1 : -1];
typedef char object_deplete_body_first_child_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, first_child_object_index) == 0xC8 ? 1 : -1];
typedef char object_deplete_body_player_index_offset_assert[
	offsetof(struct unit_datum, unit) + offsetof(struct _unit_datum, player_index) == 0x1C8 ? 1 : -1];
typedef char object_deplete_body_parent_seat_offset_assert[
	offsetof(struct unit_datum, unit) + offsetof(struct _unit_datum, parent_seat_index) == 0x2A0 ? 1 : -1];
typedef char object_deplete_body_collision_model_offset_assert[
	offsetof(struct object_definition, object) + offsetof(struct _object_definition, collision_model) + offsetof(struct tag_reference, index) == 0x7C ? 1 : -1];
typedef char object_deplete_body_effect_offset_assert[
	offsetof(struct collision_model, resistance) + offsetof(struct damage_resistance, body_depleted_effect) + offsetof(struct tag_reference, index) == 0xB4 ? 1 : -1];
typedef char object_destroy_effect_offset_assert[
	offsetof(struct collision_model, resistance) + offsetof(struct damage_resistance, body_destroyed_effect) + offsetof(struct tag_reference, index) == 0xC8 ? 1 : -1];
typedef char object_destroy_region_regions_destroyed_flags_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, regions_destroyed_flags) == 0x124 ? 1 : -1];
typedef char object_destroy_region_regions_block_offset_assert[
	offsetof(struct collision_model, resistance) + offsetof(struct damage_resistance, regions) == 0x240 ? 1 : -1];
typedef char object_destroy_region_flags_offset_assert[
	offsetof(struct damage_region, flags) == 0x20 ? 1 : -1];
typedef char object_destroy_region_destroyed_effect_offset_assert[
	offsetof(struct damage_region, destroyed_effect) + offsetof(struct tag_reference, index) == 0x44 ? 1 : -1];
typedef char object_damage_body_region_damage_offset_assert[
	offsetof(struct object_datum, object) + offsetof(struct _object_datum, region_damage) == 0x128 ? 1 : -1];
typedef char object_damage_body_damage_threshold_offset_assert[
	offsetof(struct damage_region, damage_threshold) == 0x28 ? 1 : -1];
typedef char object_damage_body_driver_offset_assert[
	offsetof(struct unit_datum, unit) + offsetof(struct _unit_datum, driver_object_index) == 0x2D4 ? 1 : -1];
typedef char object_damage_body_localized_effect_offset_assert[
	offsetof(struct damage_resistance, localized_damage_effect) + offsetof(struct tag_reference, index) == 0x7C ? 1 : -1];
typedef char object_damage_body_body_destroyed_threshold_offset_assert[
	offsetof(struct damage_resistance, body_destroyed_threshold) == 0xB8 ? 1 : -1];

/* ---------- prototypes */

static long get_player_index_from_object_or_parents(
	long object_index);

boolean unit_unsuspecting(
	long unit_index,
	real_point3d const *point);

static void object_permutation_shield_regions(
	long object_index,
	boolean active);
static void area_of_effect_cause_damage_to_object(
	struct damage_data *damage,
	long object_index,
	boolean damage_next_object);

static void damage_effect_new_on_object(
	long effect_definition_index,
	long object_index);

static void damage_effect_new_at_location(
	long effect_definition_index,
	long object_index,
	short node_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real_vector3d const *normal);

static void object_destroy_region(
	long object_index,
	short region_index);

/* ---------- globals */

extern boolean debug_damage;

/* Name and type from the 2003 PC demo PDB ONLY (file static long); HCEX has no such static, so
   the name is singly attested. January corroborates the storage: .bss +0x48, after
   object_cause_damage's default material. Both statics are uninitialised and VC7 orders
   uninitialised statics by a hash of their names, so this name takes part in producing
   January's order; no other name was tested. */
static long global_debug_damage_object_index;

/* ---------- public code */

void damage_initialize(void)
{
	return;
}

void damage_dispose(void)
{
	return;
}

void damage_initialize_for_new_map(void)
{
	global_debug_damage_object_index = NONE;
	return;
}

void damage_dispose_from_old_map(void)
{
	return;
}

void damage_render_debug(void)
{
	return;
}

void object_initialize_vitality(
	long object_index,
	real *custom_body_vitality,
	real *custom_shield_vitality)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	real maximum_body_vitality = 0.f;
	real maximum_shield_vitality = 0.f;

	if (definition->object.collision_model.index != NONE)
	{
		struct collision_model *collision_model =
			collision_model_definition_get(definition->object.collision_model.index);

		if (collision_model)
		{
			maximum_body_vitality = collision_model->resistance.maximum_body_vitality;
			maximum_shield_vitality = collision_model->resistance.maximum_shield_vitality;
		}
	}

	if (custom_body_vitality)
		maximum_body_vitality = *custom_body_vitality;

	if (custom_shield_vitality)
		maximum_shield_vitality = *custom_shield_vitality;

	object->object.maximum_body_vitality = maximum_body_vitality;
	object->object.maximum_shield_vitality = maximum_shield_vitality;
	object->object.body_vitality = maximum_body_vitality > 0.f ? 1.f : 0.f;
	object->object.shield_vitality = maximum_shield_vitality > 0.f ? 1.f : 0.f;
	return;
}

real object_get_maximum_body_vitality(
	long object_index,
	boolean ignore_difficulty)
{
	struct object_datum *object = object_get(object_index);
	real result = object->object.maximum_body_vitality;

	if (!ignore_difficulty)
		result *= game_difficulty_get_team_value(1, object->object.owner_team_index);

	return result;
}

real object_get_maximum_shield_vitality(
	long object_index,
	boolean ignore_difficulty)
{
	struct object_datum *object = object_get(object_index);
	real result = object->object.maximum_shield_vitality;

	if (!ignore_difficulty)
		result *= game_difficulty_get_team_value(2, object->object.owner_team_index);

	return result;
}

real object_get_actual_body_vitality(
	long object_index,
	boolean ignore_difficulty)
{
	real body_vitality = object_get(object_index)->object.body_vitality;
	real result = object_get_maximum_body_vitality(object_index, ignore_difficulty);

	result *= body_vitality;

	return result;
}

real object_get_actual_shield_vitality(
	long object_index,
	boolean ignore_difficulty)
{
	real shield_vitality = object_get(object_index)->object.shield_vitality;
	real result = object_get_maximum_shield_vitality(object_index, ignore_difficulty);

	result *= shield_vitality;

	return result;
}

void damage_data_new(
	struct damage_data *damage_data,
	long definition_index)
{
	csmemset(damage_data, 0, sizeof(*damage_data));
	damage_data->definition_index = definition_index;
	damage_data->material_type = NONE;
	damage_data->owner_player_index = NONE;
	damage_data->owner_object_index = NONE;
	damage_data->owner_team_index = NONE;
	damage_data->location.cluster_index = NONE;
	damage_data->scale = 1.f;
	damage_data->multiplier = 1.f;
	return;
}

boolean object_restore_body(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	boolean restored = FALSE;

	if (!TEST_FLAG(object->object.damage_flags, _object_dead_bit) &&
		object->object.body_vitality < 1.f)
	{
		object->object.body_vitality = 1.f;
		restored = TRUE;
	}

	return restored;
}

boolean object_double_charge_shield(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	boolean charged = object->object.shield_vitality <= 1.f;

	if (charged)
	{
		SET_FLAG(object->object.damage_flags, _object_shield_over_charging_bit, TRUE);
		if (object->object.shield_vitality == 0.f)
			object->object.shield_vitality = 0.01f;

		object->object.shield_stun_ticks = 0;
	}

	return charged;
}

void object_deplete_body(
	long object_index)
{
	struct object_datum *object;
	word damage_flags;

	object = object_get(object_index);
	damage_flags = object->object.damage_flags;
	if (!TEST_FLAG(damage_flags, _object_dead_bit))
	{
		struct object_definition *definition;
		long collision_model_index;

		SET_FLAG(damage_flags, _object_dead_bit, TRUE);
		object->object.damage_flags = damage_flags;

		definition = object_definition_get(object->definition_index);
		collision_model_index = definition->object.collision_model.index;
		if (collision_model_index != NONE)
		{
			struct collision_model *collision_model;

			collision_model = collision_model_definition_get(collision_model_index);
			damage_effect_new_on_object(
				collision_model->resistance.body_depleted_effect.index,
				object_index);
		}

		if (object->object.type == _object_type_vehicle)
		{
			long unit_index;

			unit_index = object->object.first_child_object_index;
			while (unit_index != NONE)
			{
				struct unit_datum *unit;

				unit = (struct unit_datum *)object_get(unit_index);
				if (unit->object.type == _object_type_biped &&
					(unit->unit.player_index == NONE || !cheat.deathless_player) &&
					unit->unit.parent_seat_index != NONE)
				{
					unit_kill(unit_index);
				}

				unit_index = unit->object.next_object_index;
			}
		}

		object_deplete_shield(object_index);
	}

	return;
}

static void object_destroy_notify_children(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	long child_object_index = object->object.first_child_object_index;

	while (child_object_index != NONE)
	{
		long next_object_index = object_get(child_object_index)->object.next_object_index;

		if (!object_type_handle_parent_destroyed(child_object_index))
			object_destroy_notify_children(child_object_index);

		child_object_index = next_object_index;
	}

	return;
}

void object_destroy(
	long object_index)
{
	struct object_datum *object;
	struct object_definition *definition;
	long collision_model_index;

	object = object_get(object_index);
	definition = object_definition_get(object->definition_index);

	object_deplete_body(object_index);

	collision_model_index = definition->object.collision_model.index;
	if (collision_model_index != NONE)
	{
		struct collision_model *collision_model;

		collision_model = collision_model_definition_get(collision_model_index);
		damage_effect_new_on_object(
			collision_model->resistance.body_destroyed_effect.index,
			object_index);
	}

	object_destroy_notify_children(object_index);
	object_delete(object_index);

	return;
}

void object_deplete_shield(
	long object_index)
{
	struct object_datum *object = object_get(object_index);

	if (!TEST_FLAG(object->object.damage_flags, _object_shield_depleted_bit))
	{
		struct object_definition *definition = object_definition_get(object->definition_index);
		long collision_model_index = definition->object.collision_model.index;

		if (collision_model_index != NONE)
		{
			struct collision_model *collision_model =
				collision_model_definition_get(collision_model_index);

			damage_effect_new_on_object(
				collision_model->resistance.shield_depleted_effect.index,
				object_index);
		}

		object->object.current_shield_damage = 0.f;
		SET_FLAG(object->object.damage_flags, _object_shield_depleted_bit, TRUE);
		object_permutation_shield_regions(object_index, FALSE);
	}

	return;
}

void render_debug_object_damage(
	void)
{
	if (debug_damage)
	{
		char buffer[2048];
		struct collision_result collision;
		real_vector3d direction;
		rectangle2d bounds = render.camera.window_bounds;

		bounds.x0 += 320;
		if (global_debug_damage_object_index == NONE)
		{
			_snprintf(
				buffer,
				sizeof(buffer),
				"no object to debug|n(point and press space)");
		}
		else
		{
			struct object_datum *object =
				object_try_and_get(global_debug_damage_object_index);

			if (object)
			{
				_snprintf(
					buffer,
					sizeof(buffer),
					"%s|nbody %0.3f|n  current %0.3f|n  recent %0.3f|nshield %0.3f|n  current %0.3f|n  recent %0.3f|n",
					strrchr(tag_get_name(object->definition_index), '\\'),
					(double)object->object.body_vitality,
					(double)object->object.current_body_damage,
					(double)object->object.recent_body_damage,
					(double)object->object.shield_vitality,
					(double)object->object.current_shield_damage,
					(double)object->object.recent_shield_damage);
			}
			else
			{
				global_debug_damage_object_index = NONE;
			}
		}

		draw_string_set_format(NONE, 0, 0);
		draw_string_set_color(global_real_argb_white);
		rasterizer_draw_string(&bounds, NULL, NULL, 0, buffer);

		if (input_key_is_down(_key_space))
		{
			long unit_index = NONE;

			if (render.local_player_index != NONE)
			{
				unit_index = player_get(
					local_player_get_player_index(render.local_player_index))->unit_index;
			}

			scale_vector3d(&render.camera.forward, 50.f, &direction);
			if (collision_test_vector(
				FLAG(_collision_test_front_facing_surfaces_bit) |
					FLAG(_collision_test_objects_bit),
				&render.camera.position,
				&direction,
				unit_index,
				&collision))
			{
				match_assert(
					"c:\\halo\\SOURCE\\objects\\damage.c",
					0x794,
					collision.type==_collision_result_object);
				global_debug_damage_object_index = collision.object_index;
			}
		}
	}

	return;
}

void object_can_take_damage(
	long object_list_index)
{
	long reference_index;
	long object_index = object_list_get_first(object_list_index, &reference_index);

	while (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);
		SET_FLAG(object->object.damage_flags, _object_cannot_take_damage_bit, FALSE);
		object_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void object_cannot_take_damage(
	long object_list_index)
{
	long reference_index;
	long object_index = object_list_get_first(object_list_index, &reference_index);

	while (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);
		SET_FLAG(object->object.damage_flags, _object_cannot_take_damage_bit, TRUE);
		object_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void object_set_ranged_attack_inhibited(
	long object_index,
	boolean inhibited)
{
	if (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);
		SET_FLAG(object->object.damage_flags, _object_ranged_attack_inhibited_bit, inhibited);
	}

	return;
}

void object_set_melee_attack_inhibited(
	long object_index,
	boolean inhibited)
{
	if (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);
		SET_FLAG(object->object.damage_flags, _object_melee_attack_inhibited_bit, inhibited);
	}

	return;
}

static void object_damage_shield(
	long object_index,
	struct damage_resistance const *damage_resistance,
	struct damage_resistance_material const *damage_material,
	struct damage_definition const *damage_definition,
	struct damage_data *damage,
	unsigned long *being_damaged_flags,
	real *shield_damage_reference,
	real *total_damage_reference)
{
	struct object_datum *object = object_get(object_index);
	real total_damage = *total_damage_reference;
	boolean negligible_damage = FALSE;
	real shield_damage = total_damage;
	boolean ignore_difficulty = FALSE;
	real maximum_shield_vitality;
	real inverse_maximum_shield_vitality;

	(void)damage;

	if (!game_engine_running() &&
		damage_definition->category == _damage_category_falling &&
		object->object.owner_team_index == _game_team_player)
	{
		ignore_difficulty = TRUE;
	}

	if (object->object.shield_vitality > 0.f)
	{
		maximum_shield_vitality =
			object_get_maximum_shield_vitality(object_index, ignore_difficulty);
		inverse_maximum_shield_vitality = maximum_shield_vitality > 0.f
			? 1.f / maximum_shield_vitality
			: 0.f;

		if (!TEST_FLAG(
				*being_damaged_flags,
				_object_being_damaged_by_friendly_bit) ||
			!TEST_FLAG(
				damage_resistance->flags,
				_damage_resistance_always_shields_friendly_damage_bit))
		{
			shield_damage =
				(1.f - damage_material->shield_leak_fraction) * total_damage;
			if (object->object.shield_vitality <=
					damage_resistance->shield_failure_threshold &&
				damage_resistance->shield_failure_threshold > 0.f)
			{
				real shield_failure = transition_function_evaluate(
					damage_resistance->shield_failure_function,
					object->object.shield_vitality /
						damage_resistance->shield_failure_threshold);

				shield_damage *=
					(1.f - damage_resistance->maximum_shield_failure) *
						shield_failure +
					damage_resistance->maximum_shield_failure;
			}
		}

		if (TEST_FLAG(
				object->object.damage_flags,
				_object_shield_over_charging_bit))
		{
			shield_damage = total_damage;
			total_damage = 0.f;
		}
		else
		{
			real actual_shield_damage;
			real normalized_shield_damage;

			if (shield_damage < 0.f)
				shield_damage = 0.f;
			total_damage -= shield_damage;

			if (TEST_FLAG(
					*being_damaged_flags,
					_object_being_damaged_by_friendly_bit) &&
				TEST_FLAG(
					*being_damaged_flags,
					_object_being_damaged_multiplied_by_difficulty_bit))
			{
				real difficulty =
					game_difficulty_get_value(_game_difficulty_value_enemy_damage);

				if (difficulty > 0.f)
					shield_damage /= difficulty;
			}

			actual_shield_damage =
				damage_material->shield_damage_multiplier * shield_damage;
			match_vassert(
				"c:\\halo\\SOURCE\\objects\\damage.c",
				1550,
				damage_resistance->shield_material_type >= 0 &&
					damage_resistance->shield_material_type < NUMBER_OF_MATERIAL_TYPES,
				"damage_resistance->shield_material_type>=0 && damage_resistance->shield_material_type<NUMBER_OF_MATERIAL_TYPES");
			actual_shield_damage *= damage_definition->material_modifiers[
				damage_resistance->shield_material_type];
			if (actual_shield_damage < _real_epsilon)
				negligible_damage = TRUE;

			normalized_shield_damage =
				actual_shield_damage * inverse_maximum_shield_vitality;
			if (normalized_shield_damage > object->object.shield_vitality ||
				damage_definition->side_effect == _damage_side_effect_emp)
			{
				real excess_damage =
					actual_shield_damage -
					maximum_shield_vitality * object->object.shield_vitality;

				if (excess_damage > 0.f)
					total_damage += excess_damage;
				object->object.shield_vitality = 0.f;
				if (!TEST_FLAG(
						object->object.damage_flags,
						_object_shield_depleted_bit))
				{
					object_deplete_shield(object_index);
					SET_FLAG(
						*being_damaged_flags,
						_object_being_damaged_shield_depleted_bit,
						TRUE);
				}
			}
			else
			{
				if (!TEST_FLAG(
						object->object.damage_flags,
						_object_cannot_take_damage_bit))
				{
					object->object.shield_vitality -= normalized_shield_damage;
				}

				if (!TEST_FLAG(
						object->object.damage_flags,
						_object_passed_shield_damage_threshold_bit) &&
					object->object.shield_vitality <
						damage_resistance->shield_damaged_effect_threshold)
				{
					damage_effect_new_on_object(
						damage_resistance->shield_damaged_effect.index,
						object_index);
					SET_FLAG(
						object->object.damage_flags,
						_object_passed_shield_damage_threshold_bit,
						TRUE);
				}
			}
		}

		if (!negligible_damage)
		{
			real normalized_damage =
				(*total_damage_reference - total_damage) *
				inverse_maximum_shield_vitality;

			object->object.shield_damage_decay_timer = 0;
			if (!TEST_FLAG(
					object->object.damage_flags,
					_object_shield_depleted_bit))
			{
				object->object.current_shield_damage = 1.f;
			}
			object->object.recent_shield_damage += normalized_damage;
			if (object->object.current_shield_damage > 1.f)
				object->object.current_shield_damage = 1.f;
			if (object->object.recent_shield_damage > 1.f)
				object->object.recent_shield_damage = 1.f;
		}
	}
	else
	{
		shield_damage = 0.f;
		object->object.shield_vitality = 0.f;
	}

	if (shield_damage >= damage_resistance->minimum_shield_stun_damage ||
		object->object.shield_vitality == 0.f)
	{
		object->object.shield_stun_ticks =
			(short)(damage_resistance->shield_stun_time * TICKS_PER_SECOND);
	}

	*shield_damage_reference = shield_damage;
	*total_damage_reference = total_damage;
	return;
}

static void object_damage_aftermath(
	long object_index,
	struct damage_data *damage,
	unsigned long being_damaged_flags,
	real shield_damage,
	real body_damage,
	real body_damage_multiplier,
	short body_part)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *object_definition =
		object_definition_get(object->definition_index);
	struct damage_effect_definition *damage_effect =
		damage_effect_definition_get(damage->definition_index);

	if (object_definition->object.acceleration_scale > _real_epsilon)
	{
		real_vector3d direction = damage->direction;
		real_vector3d acceleration;
		real acceleration_scale;

		direction.k += 0.45f;
		normalize3d(&direction);
		acceleration_scale =
			damage_effect->damage.instantaneous_acceleration *
			object_definition->object.acceleration_scale *
			0.033333335f;
		scale_vector3d(&direction, acceleration_scale, &acceleration);

		switch (object->object.type)
		{
		case _object_type_projectile:
			projectile_accelerate(object_index, &acceleration);
			break;

		case _object_type_weapon:
		case _object_type_equipment:
		case _object_type_garbage:
			item_accelerate(
				object_index,
				&acceleration,
				damage->scale > 0.5f &&
					TEST_FLAG(
						damage_effect->damage.flags,
						_damage_detonates_explosives_bit));
			break;

		case _object_type_biped:
		case _object_type_vehicle:
			{
				struct unit_datum const *unit = (struct unit_datum const *)object;

				if (damage_effect->damage.instantaneous_acceleration > _real_epsilon &&
					!TEST_FLAG(unit->unit.flags, _unit_impervious_bit))
				{
					if (object->object.type == _object_type_biped)
					{
						biped_accelerate(object_index, &acceleration);
					}
					else if (object->object.type == _object_type_vehicle)
					{
						if (TEST_FLAG(
								damage_effect->damage.flags,
								_damage_detonates_explosives_bit))
						{
							scale_vector3d(&acceleration, 2.f, &acceleration);
						}
						vehicle_accelerate(object_index, &acceleration);
					}
				}
			}
			break;

		default:
			break;
		}
	}

	if (game_engine_can_score() &&
		!TEST_FLAG(damage->flags, _damage_no_statistics_bit))
	{
		game_statistics_record_damage(
			object_index,
			shield_damage + body_damage,
			damage->owner_player_index,
			damage->owner_object_index,
			damage->owner_team_index);
		if (TEST_FLAG(
				being_damaged_flags,
				_object_being_damaged_body_depleted_bit))
		{
			game_statistics_record_kill(
				object_index,
				damage->owner_player_index,
				damage->owner_object_index,
				damage->owner_team_index);
		}
	}
	else if (game_engine_can_score())
	{
		long player_index = player_index_from_unit_index(object_index);

		game_engine_player_killed(
			player_index,
			object_index,
			player_index,
			TRUE);
	}

	if (TEST_FLAG(_object_mask_unit, object->object.type))
	{
		unit_damage_aftermath(
			object_index,
			damage,
			being_damaged_flags,
			shield_damage,
			body_damage,
			body_damage_multiplier,
			body_part);
	}

	return;
}

static void object_damage_body(
	long object_index,
	short region_index,
	short node_index,
	real_vector3d const *object_normal,
	struct damage_resistance const *damage_resistance,
	struct damage_resistance_material const *damage_material,
	struct damage_definition const *damage_definition,
	struct damage_data *damage,
	unsigned long *being_damaged_flags,
	real *body_damage,
	real *body_damage_multiplier,
	real total_damage)
{
	struct object_datum *object = object_get(object_index);
	real damage_amount = damage_material->body_damage_multiplier*total_damage;
	boolean ignore_difficulty = FALSE;
	real maximum_body_vitality;
	real inverse_maximum_body_vitality;
	real actual_damage;

	if (TEST_FLAG(damage_resistance->flags, _damage_resistance_only_hurt_while_occupied_bit) &&
		object->object.type == _object_type_vehicle &&
		unit_get(object_index)->unit.driver_object_index == NONE)
	{
		damage_amount = 0.f;
	}

	if (!game_engine_running() &&
		damage_definition->category == _damage_category_falling &&
		object->object.owner_team_index == _game_team_player)
	{
		ignore_difficulty = TRUE;
	}

	maximum_body_vitality = object_get_maximum_body_vitality(object_index, ignore_difficulty);
	if (maximum_body_vitality > 0.f)
		inverse_maximum_body_vitality = 1.f/maximum_body_vitality;
	else
		inverse_maximum_body_vitality = 0.f;

	actual_damage = damage_amount;
	if (TEST_FLAG(*being_damaged_flags, _object_being_damaged_by_friendly_bit))
	{
		actual_damage = (1.f - damage_resistance->friendly_damage_resistance)*damage_amount;
		if (TEST_FLAG(*being_damaged_flags, _object_being_damaged_multiplied_by_difficulty_bit))
		{
			real difficulty = game_difficulty_get_value(_game_difficulty_value_enemy_damage);

			if (difficulty > 0.f)
				actual_damage /= difficulty;
		}
	}
	actual_damage *= inverse_maximum_body_vitality;

	match_vassert(
		"c:\\halo\\SOURCE\\objects\\damage.c",
		1295,
		damage_material->material_type>=0 && damage_material->material_type<NUMBER_OF_MATERIAL_TYPES,
		"damage_material->type>=0 && damage_material->type<NUMBER_OF_MATERIAL_TYPES");
	actual_damage *= damage_definition->material_modifiers[damage_material->material_type];

	if (!TEST_FLAG(object->object.damage_flags, _object_cannot_take_damage_bit))
	{
		if (damage_amount > 0.f && TEST_FLAG(damage_material->flags, _damage_material_head_bit))
		{
			if (TEST_FLAG(damage_definition->flags, _damage_can_cause_headshots_bit))
			{
				if (game_engine_running() ||
					object->object.type != _object_type_biped ||
					unit_get(object_index)->unit.player_index == NONE)
				{
					object->object.body_vitality = 0.f;
					SET_FLAG(*being_damaged_flags, _object_being_damaged_killed_instantly_bit, TRUE);
					if (game_engine_running())
						SET_FLAG(*being_damaged_flags, _object_being_damaged_force_hard_ping_bit, TRUE);
				}
			}
			else if (TEST_FLAG(damage_definition->flags, _damage_can_cause_multiplayer_headshots_bit) &&
				game_engine_running())
			{
				actual_damage *= 2.f;
				if (actual_damage > object->object.body_vitality)
					SET_FLAG(*being_damaged_flags, _object_being_damaged_force_hard_ping_bit, TRUE);
			}
		}

		object->object.body_vitality -= actual_damage;
	}

	if (region_index != NONE &&
		!TEST_FLAG(object->object.regions_destroyed_flags, region_index))
	{
		struct damage_region const *region = TAG_BLOCK_GET_ELEMENT(
			&damage_resistance->regions,
			region_index,
			struct damage_region);

		object->object.region_damage[region_index] = (byte)(actual_damage*255.f + object->object.region_damage[region_index]);
		if (region->damage_threshold > 0.f &&
			object->object.region_damage[region_index]*(1.f/255.f) > region->damage_threshold)
		{
			object_destroy_region(object_index, region_index);
			SET_FLAG(*being_damaged_flags, _object_being_damaged_region_destroyed_bit, TRUE);
		}
	}

	object->object.body_damage_decay_timer = 0;
	object->object.current_body_damage += actual_damage;
	object->object.recent_body_damage += actual_damage;
	if (object->object.current_body_damage > 1.f)
		object->object.current_body_damage = 1.f;
	if (object->object.recent_body_damage > 1.f)
		object->object.recent_body_damage = 1.f;

	if (cheat.deathless_player && object->object.body_vitality < 0.f)
	{
		if (TEST_FLAG(_object_mask_unit, object->object.type))
		{
			boolean player_controlled = unit_get(object_index)->unit.player_index != NONE;

			if (!player_controlled && object->object.type == _object_type_vehicle)
			{
				long child_object_index = object->object.first_child_object_index;

				while (child_object_index != NONE)
				{
					struct object_datum *child_object = object_get(child_object_index);

					if (TEST_FLAG(_object_mask_unit, child_object->object.type))
					{
						struct unit_datum *child_unit = (struct unit_datum *)child_object;

						if (child_unit->unit.player_index != NONE)
						{
							player_controlled = TRUE;
							break;
						}
					}

					child_object_index = child_object->object.next_object_index;
				}
			}

			if (player_controlled)
				object->object.body_vitality = 0.f;
		}
	}

	{
		real body_vitality = object_get_actual_body_vitality(object_index, FALSE);

		if (damage_resistance->body_destroyed_threshold < 0.f &&
			body_vitality < damage_resistance->body_destroyed_threshold)
		{
			object_destroy(object_index);
			*being_damaged_flags |=
				FLAG(_object_being_damaged_body_depleted_bit) |
				FLAG(_object_being_damaged_body_destroyed_bit);
		}
		else if (body_vitality < 0.f)
		{
			if (!TEST_FLAG(object->object.damage_flags, _object_dead_bit))
			{
				short dying_region_index;

				for (dying_region_index = 0;
					dying_region_index < damage_resistance->regions.count;
					dying_region_index++)
				{
					struct damage_region const *region = TAG_BLOCK_GET_ELEMENT(
						&damage_resistance->regions,
						dying_region_index,
						struct damage_region);

					if (TEST_FLAG(region->flags, _object_region_dies_when_object_dies_bit))
						object_destroy_region(object_index, dying_region_index);
				}

				object_deplete_body(object_index);
				SET_FLAG(*being_damaged_flags, _object_being_damaged_body_depleted_bit, TRUE);
			}
		}
		else if (body_vitality < damage_resistance->body_damaged_effect_threshold &&
			!TEST_FLAG(object->object.damage_flags, _object_passed_body_damage_threshold_bit))
		{
			damage_effect_new_on_object(
				damage_resistance->body_damaged_effect.index,
				object_index);
			SET_FLAG(object->object.damage_flags, _object_passed_body_damage_threshold_bit, TRUE);
		}
	}

	if (TEST_FLAG(damage->flags, _damage_create_localized_effect_bit) &&
		damage_resistance->localized_damage_effect.index != NONE)
	{
		damage_effect_new_at_location(
			damage_resistance->localized_damage_effect.index,
			object_index,
			node_index,
			&damage->epicenter,
			&damage->direction,
			object_normal);
	}

	if (TEST_FLAG(damage->flags, _damage_area_of_effect_bit) &&
		damage_amount > damage_resistance->area_damage_effect_threshold &&
		damage_resistance->area_damage_effect.index != NONE &&
		damage_definition->category != _damage_category_flame)
	{
		damage_effect_new_on_object(
			damage_resistance->area_damage_effect.index,
			object_index);
	}

	*body_damage = damage_amount;
	*body_damage_multiplier = damage_material->body_damage_multiplier;

	return;
}

void object_cause_damage(
	struct damage_data *damage,
	long object_index,
	short node_index,
	short region_index,
	short material_index,
	real_vector3d const *object_normal)
{
	/* Name and function scope from the 2003 PC demo PDB and the HCEX PDB (static local of
	   object_cause_damage; their type is named damage_material). Neither PDB records the block:
	   placing it at the top of the function is unattested. January corroborates: .bss +0, and only
	   this function takes its address. */
	static struct damage_resistance_material default_damage_material;
	struct damage_effect_definition *damage_effect;
	struct damage_definition const *damage_definition;
	boolean damage_was_modified;
	boolean parent_takes_body_damage;
	boolean material_effect_recorded;
	short damaged_object_count;
	real total_damage;
	struct object_datum *current_object;
	struct damage_resistance_material const *damage_material;
	long current_object_index;
	unsigned long being_damaged_flags;
	real shield_damage;
	real body_damage;
	real body_damage_multiplier;
	short body_part;
	short object_number;
	long damaged_object_indices[16];

#ifdef HALO_LINUX
	/* the distributed netcode (port/linux/NETCODE.md): the host deals
	damage; a client reports its own players' hits instead, and the host
	deals those (port/linux/game/network_damage.c) */
	if (!network_damage_deals(damage, object_index, node_index, region_index, material_index, object_normal,
		distributed_damage_authorized))
	{
		return;
	}
#endif

	damage_effect = damage_effect_definition_get(damage->definition_index);
	damage_definition = &damage_effect->damage;
	damage_was_modified = FALSE;
	parent_takes_body_damage = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\damage.c",
		0x338,
		region_index==NONE ||
			(region_index>=0 && region_index<MAXIMUM_REGIONS_PER_OBJECT));

	if (damage->owner_player_index != NONE)
		global_debug_damage_object_index = object_index;

	{
		real random_damage = real_random_range(
			damage_definition->damage_lower_bound,
			damage_definition->damage_upper_bound);

		total_damage =
			((1.f - damage->scale) * damage_definition->damage_minimum +
				random_damage * damage->scale) *
			damage->multiplier;
	}

	if (damage->owner_object_index != NONE)
	{
		struct unit_datum *attacker =
			unit_try_and_get(damage->owner_object_index);

		if (attacker)
		{
			long actor_index;

			if (attacker->unit.gunner_object_index != NONE)
			{
				attacker = unit_get(attacker->unit.gunner_object_index);
			}

			actor_index = attacker->unit.swarm_actor_index != NONE ?
				attacker->unit.swarm_actor_index :
				attacker->unit.actor_index;

			if (actor_index != NONE)
				ai_adjust_damage(actor_index, damage, &total_damage);
		}
	}

	if (game_engine_running())
	{
		total_damage *= game_engine_get_damage_multiplier(
			get_player_index_from_object_or_parents(damage->owner_object_index),
			get_player_index_from_object_or_parents(object_index));
	}
	else if (damage->owner_team_index != NONE &&
		game_team_is_enemy(damage->owner_team_index, _game_team_player))
	{
		total_damage *= game_difficulty_get_value(_game_difficulty_value_enemy_damage);
		damage_was_modified = TRUE;
	}

	damaged_object_count = 0;
	material_effect_recorded = FALSE;
	if (TEST_FLAG(damage->flags, _damage_area_of_effect_bit) ||
		TEST_FLAG(damage->flags, _damage_kill_instantly_bit))
	{
		damaged_object_count = 1;
		damaged_object_indices[0] = object_index;
	}
	else
	{
		long damaged_object_index = object_index;

		while (damaged_object_index != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\objects\\damage.c",
				0x37E,
				damaged_object_count<sizeof(damaged_object_indices)/sizeof(long));
			damaged_object_indices[damaged_object_count] = damaged_object_index;
			damaged_object_count++;
			damaged_object_index =
				object_get(damaged_object_index)->object.parent_object_index;
		}
	}

	{
		struct object_datum *object = object_get(object_index);
		struct object_definition const *object_definition =
			object_definition_get(object->definition_index);

		if (object_definition->object.collision_model.index != NONE)
		{
			struct collision_model const *collision_model =
				collision_model_definition_get(
					object_definition->object.collision_model.index);

			parent_takes_body_damage = !TEST_FLAG(
				collision_model->resistance.flags,
				_damage_resistance_parent_never_takes_body_damage_for_us_bit);
		}

		if (object->object.umbrella_shield_object_index != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\objects\\damage.c",
				0x397,
				damaged_object_count<sizeof(damaged_object_indices)/sizeof(long));
			damaged_object_indices[damaged_object_count] =
				object->object.umbrella_shield_object_index;
			damaged_object_count++;
		}

		if (!TEST_FLAG(damage->flags, _damage_area_of_effect_bit) &&
			object->object.type == _object_type_vehicle)
		{
			struct unit_datum *vehicle = unit_get(object_index);
			struct unit_definition const *vehicle_definition =
				unit_definition_get(vehicle->definition_index);
			long child_object_index = vehicle->object.first_child_object_index;

			damage->multiplier =
				(1.f - damage_definition->vehicle_passthrough_penalty) *
				vehicle_definition->unit.child_damage_fraction;
			while (child_object_index != NONE)
			{
				struct object_datum *child_object = object_get(child_object_index);

				match_assert(
					"c:\\halo\\SOURCE\\objects\\damage.c",
					0x3AB,
					damaged_object_count<sizeof(damaged_object_indices)/sizeof(long));
				if (child_object->object.type == _object_type_biped)
				{
					long player_index = unit_get(child_object_index)->unit.player_index;

					if (player_index != NONE ||
						child_object_index == vehicle->unit.driver_object_index)
					{
						SET_FLAG(
							damage->flags,
							_damage_bypasses_shields_bit,
							player_index == NONE);
						object_cause_damage(
							damage,
							child_object_index,
							NONE,
							NONE,
							NONE,
							NULL);
						SET_FLAG(
							damage->flags,
							_damage_bypasses_shields_bit,
							FALSE);
					}
				}

				child_object_index = child_object->object.next_object_index;
			}

			damage->multiplier = 1.f;
		}
	}

	for (object_number = 0;
		object_number < damaged_object_count;
		object_number++)
	{
		struct unit_datum *unit = unit_try_and_get(damaged_object_indices[object_number]);

		if (unit)
		{
			long player_index = unit->unit.player_index;

			if (player_index != NONE)
			{
#ifdef HALO_LINUX
				/* (the host's, for its clients; a client replaying the host's
				killing blow has had its player effect already) */
				network_damage_player_effect(player_index, damage, total_damage);
				if (!network_damage_replaying_kill())
#endif
				player_effect_start(
					player_index,
					damage,
					&damage->direction,
					damage->scale,
					total_damage);
			}
			else if (cheat.reflexive_damage_effects)
			{
				player_effect_start(
					local_player_get_player_index(0),
					damage,
					&damage->direction,
					damage->scale,
					total_damage);
			}
		}
	}

	if (total_damage > 0.f)
	{
		do
		{
			struct object_definition const *current_definition;
			long collision_model_index;

			if (damaged_object_count-- <= 0)
				return;

			current_object_index = damaged_object_indices[damaged_object_count];
			current_object = object_get(current_object_index);
			current_definition = object_definition_get(current_object->definition_index);
			collision_model_index = current_definition->object.collision_model.index;
			shield_damage = 0.f;
			body_damage = 0.f;
			body_damage_multiplier = 0.f;
			being_damaged_flags = 0;
			body_part = NONE;

			if (collision_model_index != NONE)
			{
				struct collision_model const *collision_model =
					collision_model_definition_get(collision_model_index);
				boolean force_kill = TEST_FLAG(
					damage->flags,
					_damage_kill_instantly_bit);

				if (node_index >= 0 && node_index < collision_model->nodes.count)
				{
					struct collision_node const *collision_node =
						TAG_BLOCK_GET_ELEMENT(
							&collision_model->nodes,
							node_index,
							struct collision_node);

					body_part = collision_node->runtime_damage_part;
				}

				if (damage_was_modified)
				{
					SET_FLAG(
						being_damaged_flags,
						_object_being_damaged_multiplied_by_difficulty_bit,
						TRUE);
				}

				if (damage->owner_team_index != NONE &&
					!game_team_is_enemy(
						current_object->object.owner_team_index,
						damage->owner_team_index))
				{
					SET_FLAG(
						being_damaged_flags,
						_object_being_damaged_by_friendly_bit,
						TRUE);
				}

				if (damaged_object_count == 0 &&
					material_index >= 0 &&
					material_index < collision_model->resistance.materials.count)
				{
					damage_material = TAG_BLOCK_GET_ELEMENT(
						&collision_model->resistance.materials,
						material_index,
						struct damage_resistance_material);
				}
				else if (collision_model->resistance.indirect_damage_material_index >= 0 &&
					collision_model->resistance.indirect_damage_material_index <
						collision_model->resistance.materials.count)
				{
					damage_material = TAG_BLOCK_GET_ELEMENT(
						&collision_model->resistance.materials,
						collision_model->resistance.indirect_damage_material_index,
						struct damage_resistance_material);
				}
				else
				{
					damage_material = &default_damage_material;
				}

				damage->material_type = damage_material->material_type;

				if (cheat.omnipotent && damage->owner_player_index != NONE)
					force_kill = TRUE;

				if (damage_definition->side_effect ==
						_damage_side_effect_lethal_to_the_unsuspecting &&
					unit_unsuspecting(current_object_index, &damage->epicenter) &&
					!TEST_FLAG(
						current_object->object.damage_flags,
						_object_cannot_take_damage_bit))
				{
					force_kill = TRUE;
				}

				if (force_kill &&
					!TEST_FLAG(current_object->object.damage_flags, _object_dead_bit))
				{
					current_object->object.body_vitality = 0.f;
					object_deplete_body(current_object_index);
					SET_FLAG(
						being_damaged_flags,
						_object_being_damaged_body_depleted_bit,
						TRUE);
					SET_FLAG(
						being_damaged_flags,
						_object_being_damaged_killed_instantly_bit,
						TRUE);
				}

				if (!TEST_FLAG(damage->flags, _damage_bypasses_shields_bit) &&
					!TEST_FLAG(damage_definition->flags, _damage_skips_shields_bit) &&
					current_object->object.maximum_shield_vitality > 0.f &&
					(damaged_object_count == 0 ||
						TEST_FLAG(
							collision_model->resistance.flags,
							_damage_resistance_takes_shield_damage_for_children_bit)))
				{
					object_damage_shield(
						current_object_index,
						&collision_model->resistance,
						damage_material,
						damage_definition,
						damage,
						&being_damaged_flags,
						&shield_damage,
						&total_damage);
				}

				if ((damaged_object_count == 0 ||
						(parent_takes_body_damage &&
							TEST_FLAG(
								collision_model->resistance.flags,
								_damage_resistance_takes_body_damage_for_children_bit))) &&
					!TEST_FLAG(
						damage_definition->flags,
						_damage_only_hurts_shields_bit))
				{
					if (TEST_FLAG(
							collision_model->resistance.flags,
							_damage_resistance_only_hurt_by_explosives_bit) &&
						!TEST_FLAG(
							damage_definition->flags,
							_damage_detonates_explosives_bit))
					{
						total_damage = 0.f;
					}

					object_damage_body(
						current_object_index,
						damaged_object_count == 0 ? region_index : NONE,
						damaged_object_count == 0 ? node_index : NONE,
						damaged_object_count == 0 ? object_normal : NULL,
						&collision_model->resistance,
						damage_material,
						damage_definition,
						damage,
						&being_damaged_flags,
						&body_damage,
						&body_damage_multiplier,
						total_damage);
					damaged_object_count = 0;
				}

				if (!material_effect_recorded &&
					(shield_damage > _real_epsilon || body_damage > _real_epsilon))
				{
					if (shield_damage > body_damage)
					{
						damage->material_type =
							collision_model->resistance.shield_material_type;
						damage->material_effect_scale =
							current_object->object.shield_vitality;
					}
					else
					{
						damage->material_effect_scale = PIN(
							current_object->object.body_vitality,
							0.f,
							1.f);
					}

					if (debug_damage &&
						current_object_index == global_debug_damage_object_index)
					{
						console_printf(
							FALSE,
							"%s: \"%s\" \"%s\" k=%0.2f S[%3.2f] B[%3.2f]",
							strrchr(tag_get_name(damage->definition_index), '\\') + 1,
							material_get_name(damage_material->material_type),
							damage_material->name,
							(double)damage->scale,
							(double)shield_damage,
							(double)body_damage);
					}

					material_effect_recorded = TRUE;
				}
			}

			object_damage_aftermath(
				current_object_index,
				damage,
				being_damaged_flags,
				shield_damage,
				body_damage,
				body_damage_multiplier,
				body_part);
#ifdef HALO_LINUX
			/* (the host's, for its clients) */
			network_damage_aftermath(
				current_object_index,
				damage,
				being_damaged_flags,
				shield_damage,
				body_damage,
				body_damage_multiplier,
				body_part,
				current_object_index == object_index ? node_index : NONE,
				current_object_index == object_index ? region_index : NONE,
				current_object_index == object_index ? material_index : NONE);
#endif
			if (TEST_FLAG(
				being_damaged_flags,
				_object_being_damaged_body_destroyed_bit))
			{
				object_delete(current_object_index);
			}
		}
		while (total_damage > 0.f);
	}

	return;
}

void area_of_effect_cause_damage(
	struct damage_data *damage,
	long unused_object_index)
{
	struct damage_effect_definition *definition =
		damage_effect_definition_get(damage->definition_index);
#ifdef HALO_LINUX
	/* the native builds damage more objects per explosion
	(halo_port_capacity.h): 64 runs out in a crowd of 128 players */
	long object_indices[HALO_PORT_MAXIMUM_AREA_OF_EFFECT_OBJECTS];
#else
	long object_indices[64];
#endif
	short object_count;

	object_count = objects_in_sphere(
		0,
		0,
		&damage->location,
		&damage->origin,
		definition->cutoff_radius,
		object_indices,
		NUMBEROF(object_indices));
	if (object_count > 0)
	{
		short object_number;

		for (object_number = 0; object_number < object_count; object_number++)
		{
			area_of_effect_cause_damage_to_object(
				damage,
				object_indices[object_number],
				FALSE);
		}
	}

	breakable_surface_damage_area_of_effect(damage);
	return;
}

void object_damage_update(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *object_definition =
		object_definition_get(object->definition_index);
	long collision_model_index = object_definition->object.collision_model.index;
	struct collision_model *collision_model;
	word damage_flags;

	if (collision_model_index == NONE)
		return;

	collision_model = collision_model_definition_get(collision_model_index);
	if (!collision_model)
		return;

	damage_flags = object->object.damage_flags;
	if (TEST_FLAG(damage_flags, _object_die_act_of_god_no_statistics_bit) ||
		(damage_flags &
			(FLAG(_object_die_act_of_god_bit) |
				FLAG(_object_die_act_of_god_silent_bit))) != 0)
	{
		if (!TEST_FLAG(damage_flags, _object_dead_bit))
		{
			struct game_globals_falling_damage *falling_damage =
				TAG_BLOCK_GET_ELEMENT(
					&scenario_get_game_globals()->falling_damage,
					0,
					struct game_globals_falling_damage);
			long damage_effect_index = falling_damage->falling_damage.index;

			if (damage_effect_index != NONE)
			{
				struct damage_data damage;

				damage_data_new(&damage, damage_effect_index);
				damage.scale = 1.f;
				SET_FLAG(damage.flags, _damage_kill_instantly_bit, TRUE);
				if (TEST_FLAG(
					object->object.damage_flags,
					_object_die_act_of_god_silent_bit))
				{
					SET_FLAG(damage.flags, _damage_silent_bit, TRUE);
				}

				if (TEST_FLAG(
					object->object.damage_flags,
					_object_die_act_of_god_no_statistics_bit))
				{
					SET_FLAG(damage.flags, _damage_no_statistics_bit, TRUE);
				}

#ifdef HALO_LINUX
				distributed_damage_authorized = TRUE;
#endif
				object_cause_damage(
					&damage,
					object_index,
					NONE,
					NONE,
					NONE,
					NULL);
#ifdef HALO_LINUX
				distributed_damage_authorized = FALSE;
#endif
			}
		}

		object->object.damage_flags &=
			~(FLAG(_object_die_act_of_god_bit) |
				FLAG(_object_die_act_of_god_silent_bit) |
				FLAG(_object_die_act_of_god_no_statistics_bit));
	}

	if (objects_update_shields())
		SET_FLAG(object->object.damage_flags, _object_shield_charging_bit, FALSE);
	damage_flags = object->object.damage_flags;
	if (objects_update_shields() &&
		object->object.maximum_shield_vitality > 0.f &&
		!TEST_FLAG(damage_flags, _object_dead_bit))
	{
		if (TEST_FLAG(damage_flags, _object_shield_over_charging_bit))
		{
			real shield_vitality = object->object.shield_vitality + 0.033333335f;

			object->object.shield_vitality = shield_vitality;
			if (shield_vitality >= 3.f)
			{
				object->object.shield_vitality = 3.f;
				SET_FLAG(
					object->object.damage_flags,
					_object_shield_over_charging_bit,
					FALSE);
			}
			else
			{
				SET_FLAG(
					object->object.damage_flags,
					_object_shield_charging_bit,
					TRUE);
			}
		}
		else if (object->object.shield_vitality > 1.f && game_engine_running())
		{
			long player_index = player_index_from_unit_index(object_index);
			real overcharge = object->object.shield_vitality - 1.f;

			if (0.00074074074f > overcharge)
			{
				object->object.shield_vitality = 1.f;
				hud_tick_shield(player_index, overcharge);
			}
			else
			{
				object->object.shield_vitality -= 0.00074074074f;
				hud_tick_shield(player_index, 0.00074074074f);
			}
		}
		else if (object->object.shield_vitality < 1.f)
		{
			if (object->object.shield_stun_ticks == 0)
			{
				real shield_recharge =
					collision_model->resistance.runtime_shield_recharge_velocity;

				shield_recharge *= game_difficulty_get_team_value(
						_game_difficulty_value_enemy_recharge,
						object->object.owner_team_index);

				if (TEST_FLAG(
					object->object.damage_flags,
					_object_shield_depleted_bit))
				{
					damage_effect_new_on_object(
						collision_model->resistance.shield_recharging_effect.index,
						object_index);
					SET_FLAG(
						object->object.damage_flags,
						_object_shield_depleted_bit,
						FALSE);
					object_permutation_shield_regions(object_index, TRUE);
				}

				SET_FLAG(
					object->object.damage_flags,
					_object_shield_charging_bit,
					TRUE);
				shield_recharge += object->object.shield_vitality;
				damage_flags = object->object.damage_flags;
				object->object.shield_vitality = shield_recharge;
				if (shield_recharge > 1.f)
				{
					SET_FLAG(
						damage_flags,
						_object_shield_charging_bit,
						FALSE);
					object->object.shield_vitality = 1.f;
					object->object.damage_flags = damage_flags;
				}
			}
			else
			{
				object->object.shield_stun_ticks--;
			}
		}
	}

	if (object->object.body_damage_decay_timer != NONE)
	{
		long decay_timer = ++object->object.body_damage_decay_timer;

		if (decay_timer >= 0)
			object->object.current_body_damage -= 0.016666668f;
		if (decay_timer >= TICKS_PER_SECOND * 2)
			object->object.recent_body_damage -= 0.016666668f;

		object->object.current_body_damage =
			MAX(0.f, object->object.current_body_damage);
		object->object.recent_body_damage =
			MAX(0.f, object->object.recent_body_damage);
		if (object->object.current_body_damage == 0.f &&
			object->object.recent_body_damage == 0.f)
		{
			object->object.body_damage_decay_timer = NONE;
		}
	}

	if (object->object.shield_damage_decay_timer != NONE)
	{
		long decay_timer = ++object->object.shield_damage_decay_timer;

		if (decay_timer >= 0)
			object->object.current_shield_damage -= 0.016666668f;
		if (decay_timer >= TICKS_PER_SECOND * 2)
			object->object.recent_shield_damage -= 0.016666668f;

		object->object.current_shield_damage =
			MAX(0.f, object->object.current_shield_damage);
		object->object.recent_shield_damage =
			MAX(0.f, object->object.recent_shield_damage);
		if (object->object.current_shield_damage == 0.f &&
			object->object.recent_shield_damage == 0.f)
		{
			object->object.shield_damage_decay_timer = NONE;
		}
	}

	return;
}

/* ---------- private code */

static long get_player_index_from_object_or_parents(
	long object_index)
{
	long player_index = NONE;

	while (object_index != NONE)
	{
		if (unit_try_and_get(object_index))
		{
			player_index = player_index_from_unit_index(object_index);
			break;
		}

		object_index = object_get(object_index)->object.parent_object_index;
	}

	return player_index;
}

static void damage_effect_new_on_object(
	long effect_definition_index,
	long object_index)
{
	effect_new_from_object(
		effect_definition_index,
		object_index,
		object_index,
		NONE,
		0.f,
		0.f,
		NULL,
		NULL);
	return;
}

static void damage_effect_new_at_location(
	long effect_definition_index,
	long object_index,
	short node_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real_vector3d const *normal)
{
	char const *marker_names[NUMBER_OF_EFFECT_MARKERS];
	real_point3d marker_points[NUMBER_OF_EFFECT_MARKERS];
	real_vector3d marker_forwards[NUMBER_OF_EFFECT_MARKERS];
	real_vector3d incident;
	short marker_index;

	marker_names[_effect_vector_normal] = "normal";
	marker_names[_effect_vector_incident] = "incident";
	marker_names[_effect_vector_negative_incident] = "negative incident";
	marker_names[_effect_vector_reflected] = "reflection";
	marker_names[_effect_vector_gravity] = "gravity";

	marker_forwards[_effect_vector_gravity] = *global_down3d;

	incident = *direction;
	if (normalize3d(&incident) == 0.f)
		incident = *global_forward3d;
	scale_vector3d(&incident, -1.f, &marker_forwards[_effect_vector_incident]);
	marker_forwards[_effect_vector_negative_incident] = incident;

	if (!normal)
	{
		real_point3d origin;
		real_vector3d position_normal;

		object_get_origin(object_index, &origin);
		vector_from_points3d(&origin, position, &position_normal);
		if (normalize3d(&position_normal) == 0.f)
			position_normal = object_get(object_index)->object.forward;

		marker_forwards[_effect_vector_normal] = position_normal;
		reflect_vector3d(
			&incident,
			&position_normal,
			&marker_forwards[_effect_vector_reflected]);
	}
	else
	{
		marker_forwards[_effect_vector_normal] = *normal;
		reflect_vector3d(
			&incident,
			normal,
			&marker_forwards[_effect_vector_reflected]);
	}

	for (marker_index = 0; marker_index < NUMBER_OF_EFFECT_MARKERS; marker_index++)
		marker_points[marker_index] = *position;

	if (object_index != NONE && node_index != NONE)
	{
		effect_new_attached_from_markers(
			effect_definition_index,
			object_index,
			object_index,
			node_index,
			NUMBER_OF_EFFECT_MARKERS,
			marker_names,
			marker_points,
			marker_forwards,
			1.f,
			0.f,
			NULL,
			NULL);
	}
	else
	{
		effect_new_unattached_from_markers(
			effect_definition_index,
			object_index,
			global_zero_vector3d,
			NUMBER_OF_EFFECT_MARKERS,
			marker_names,
			marker_points,
			marker_forwards,
			1.f,
			0.f,
			NULL,
			NULL,
			FALSE);
	}

	return;
}

static void object_destroy_region(
	long object_index,
	short region_index)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	long collision_model_index = definition->object.collision_model.index;

	if (collision_model_index != NONE)
	{
		struct collision_model *collision_model =
			collision_model_definition_get(collision_model_index);

		match_assert(
			"c:\\halo\\SOURCE\\objects\\damage.c",
			1818,
			region_index>=0 && region_index<MAXIMUM_REGIONS_PER_OBJECT);

		if (!TEST_FLAG(object->object.regions_destroyed_flags, region_index))
		{
			struct damage_region *region = TAG_BLOCK_GET_ELEMENT(
				&collision_model->resistance.regions,
				region_index,
				struct damage_region);

			damage_effect_new_on_object(
				region->destroyed_effect.index,
				object_index);
			object_permute_region(object_index, "~damaged", region_index, TRUE);

			if (TEST_FLAG(region->flags, _object_region_inhibits_melee_attack_bit))
				SET_FLAG(object->object.damage_flags, _object_melee_attack_inhibited_bit, TRUE);
			if (TEST_FLAG(region->flags, _object_region_inhibits_ranged_attack_bit))
				SET_FLAG(object->object.damage_flags, _object_ranged_attack_inhibited_bit, TRUE);
			if (TEST_FLAG(region->flags, _object_region_inhibits_walking_bit))
				SET_FLAG(object->object.damage_flags, _object_walking_inhibited_bit, TRUE);
			if (TEST_FLAG(region->flags, _object_region_forces_drop_weapon_bit))
				SET_FLAG(object->object.damage_flags, _object_cannot_hold_weapon_bit, TRUE);
			if (TEST_FLAG(region->flags, _object_region_forces_object_to_die_bit))
				object_deplete_body(object_index);

			SET_FLAG(object->object.regions_destroyed_flags, region_index, TRUE);
			object_type_handle_region_destroyed(object_index, region_index, region->flags);
		}
	}

	return;
}

static void area_of_effect_cause_damage_to_object(
	struct damage_data *damage,
	long object_index,
	boolean damage_next_object)
{
	boolean did_damage;
	boolean collision_blocked;
	boolean infection_form;
	struct object_datum *object;
	struct object_definition *object_definition;
	struct damage_effect_definition *damage_effect;
	real_point3d const *epicenter;
	real_vector3d to_object;
	real_vector3d perpendicular;
	real_vector3d cross;
	real_vector3d collision_vector;
	real_vector3d offset_vector;
	real_vector3d direct_vector;
	real_point3d collision_point;
	struct collision_result spread_collision;
	struct collision_result direct_collision;
	boolean can_damage;

	object = object_get(object_index);
	object_definition = object_definition_get(object->definition_index);
	damage_effect = damage_effect_definition_get(damage->definition_index);
	can_damage = !TEST_FLAG(object->object.flags, _object_invisible_bit);
	did_damage = FALSE;
	infection_form = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\damage.c",
		601,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] =
		_collision_user_area_damage;

	if (can_damage &&
		TEST_FLAG(_object_mask_unit, object->object.type) &&
		damage_effect->damage.area_of_effect_core_radius > _real_epsilon)
	{
		long ray_index;
		long rays_remaining;

		epicenter = &damage->epicenter;
		collision_blocked = TRUE;
		vector_from_points3d(
			epicenter,
			&object->object.bounding_sphere_center,
			&to_object);
		normalize3d(perpendicular3d(&to_object, &perpendicular));
		normalize3d(cross_product3d(&to_object, &perpendicular, &cross));

		for (ray_index = 0, rays_remaining = 4;
			rays_remaining;
			ray_index++, rays_remaining--)
		{
			long ultimate_parent_index;

			switch (ray_index)
			{
			case 0:
				scale_vector3d(
					&perpendicular,
					damage_effect->damage.area_of_effect_core_radius,
					&collision_vector);
				break;

			case 1:
				scale_vector3d(
					&perpendicular,
					-damage_effect->damage.area_of_effect_core_radius,
					&collision_vector);
				break;

			case 2:
				scale_vector3d(
					&cross,
					damage_effect->damage.area_of_effect_core_radius,
					&collision_vector);
				break;

			case 3:
				scale_vector3d(
					&cross,
					-damage_effect->damage.area_of_effect_core_radius,
					&collision_vector);
				break;
			}

			collision_test_vector(
				_damage_area_of_effect_collision_flags,
				epicenter,
				&collision_vector,
				object_get_ultimate_parent(object_index),
				&spread_collision);
			collision_point = spread_collision.point;
			ultimate_parent_index = object_get_ultimate_parent(object_index);
			if (!collision_test_vector(
				_damage_area_of_effect_collision_flags,
				&collision_point,
				vector_from_points3d(
					&collision_point,
					&object->object.bounding_sphere_center,
					&offset_vector),
				ultimate_parent_index,
				&spread_collision))
			{
				collision_blocked = FALSE;
			}
		}
	}
	else
	{
		long ultimate_parent_index;

		ultimate_parent_index = object_get_ultimate_parent(object_index);
		epicenter = &damage->epicenter;
		collision_blocked = collision_test_vector(
			_damage_area_of_effect_collision_flags,
			epicenter,
			vector_from_points3d(
				epicenter,
				&object->object.bounding_sphere_center,
				&direct_vector),
			ultimate_parent_index,
			&direct_collision);
	}

	if (collision_blocked)
		can_damage = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\objects\\damage.c",
		660,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	if (TEST_FLAG(damage_effect->damage.flags, _damage_does_not_hurt_owner_bit) &&
		object_index == damage->owner_object_index)
	{
		can_damage = FALSE;
	}

	if (TEST_FLAG(damage_effect->damage.flags, _damage_does_not_hurt_friends_bit) &&
		!game_team_is_enemy(object->object.owner_team_index, damage->owner_team_index))
	{
		can_damage = FALSE;
	}

	if (can_damage &&
		TEST_FLAG(damage_effect->damage.flags, _damage_infection_form_pop_bit))
	{
		can_damage = FALSE;
		if (TEST_FLAG(_object_mask_unit, object->object.type) &&
			TEST_FLAG(
				unit_definition_get(object->definition_index)->unit.flags,
				_unit_is_inconsequential_bit) &&
			object_index != damage->owner_object_index)
		{
			real infection_form_toughness = game_difficulty_get_value(
				_game_difficulty_value_infection_forms);

			can_damage = TRUE;
			if (infection_form_toughness > 0.f ||
				TEST_FLAG(
					damage_effect->damage.flags,
					_damage_does_not_hurt_infection_forms_bit))
			{
				if (TEST_FLAG(damage->flags, _damage_damaged_one_object_bit))
					can_damage = FALSE;
			}

			if (infection_form_toughness > 0.f &&
				real_seed_random(get_global_random_seed_address()) <
					infection_form_toughness * 0.25f)
			{
				can_damage = FALSE;
			}

			infection_form = TRUE;
		}
	}

	SET_FLAG(damage->flags, _damage_area_of_effect_bit, TRUE);
	if (can_damage)
	{
		real distance;
		real radius_delta;
		real scale;

		vector_from_points3d(
			epicenter,
			&object->object.bounding_sphere_center,
			&damage->direction);
		distance = normalize3d(&damage->direction);
		radius_delta = damage_effect->cutoff_radius - damage_effect->falloff_radius;
		if (radius_delta > 0.f)
		{
			scale = PIN(
				1.f -
					(distance - damage_effect->falloff_radius) /
					radius_delta,
				0.f,
				1.f);
		}
		else
		{
			scale = 1.f;
		}

		if (!TEST_FLAG(
			damage_effect->flags,
			_damage_effect_dont_scale_damage_by_distance_bit))
		{
			damage->scale = scale;
		}

		if (scale > 0.f)
		{
			object_cause_damage(
				damage,
				object_index,
				NONE,
				NONE,
				NONE,
				NULL);
				did_damage = TRUE;
		}

		if (object_definition->object.collision_model.index != NONE)
		{
			struct collision_model *collision_model =
				collision_model_definition_get(
					object_definition->object.collision_model.index);

			if (TEST_FLAG(
				collision_model->resistance.flags,
				_damage_resistance_children_take_area_damage_bit))
			{
				long child_object_index =
					object->object.first_child_object_index;

				if (child_object_index != NONE)
				{
					area_of_effect_cause_damage_to_object(
						damage,
						child_object_index,
						TRUE);
				}
			}
		}
	}

	if (infection_form && (!can_damage || did_damage))
	{
		SET_FLAG(damage->flags, _damage_damaged_one_object_bit, TRUE);
	}

	if (damage_next_object)
	{
		long next_object_index = object->object.next_object_index;

		if (next_object_index != NONE)
		{
			area_of_effect_cause_damage_to_object(
				damage,
				next_object_index,
				TRUE);
		}
	}

	return;
}

static void object_permutation_shield_regions(
	long object_index,
	boolean active)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	struct collision_model *collision_model =
		collision_model_definition_get(definition->object.collision_model.index);
	short region_index;

	for (region_index = 0;
		region_index < collision_model->resistance.regions.count;
		region_index++)
	{
		struct damage_region *region = TAG_BLOCK_GET_ELEMENT(
			&collision_model->resistance.regions,
			region_index,
			struct damage_region);

		if (TEST_FLAG(region->flags, _object_region_missing_when_shield_is_zero_bit) &&
			region->permutations.count > 1)
		{
			object->object.region_permutations[region_index] = !active;
		}
	}

	return;
}

#ifdef HALO_LINUX
/* the distributed netcode (port/linux/game/network_distributed.c): a
client's copy of an object takes the host's vitality and recent damage
(what the shields' and the HUD's effects show), with the effects of its
shields going down and coming back up as the host's do */
void damage_set_network_state(
	long object_index,
	struct damage_network_state const *state)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *object_definition =
		object_definition_get(object->definition_index);
	long collision_model_index = object_definition->object.collision_model.index;

	if (state->shield_depleted && !TEST_FLAG(object->object.damage_flags, _object_shield_depleted_bit))
	{
		object_deplete_shield(object_index);
	}
	else if (!state->shield_depleted && TEST_FLAG(object->object.damage_flags, _object_shield_depleted_bit))
	{
		/* (as object_damage_update's recharge begins) */
		if (collision_model_index != NONE)
		{
			damage_effect_new_on_object(
				collision_model_definition_get(collision_model_index)->resistance.shield_recharging_effect.index,
				object_index);
		}
		SET_FLAG(object->object.damage_flags, _object_shield_depleted_bit, FALSE);
		object_permutation_shield_regions(object_index, TRUE);
	}
	SET_FLAG(object->object.damage_flags, _object_shield_charging_bit, state->shield_charging);
	SET_FLAG(object->object.damage_flags, _object_shield_over_charging_bit, state->shield_over_charging);
	object->object.body_vitality = state->body_vitality;
	object->object.shield_vitality = state->shield_vitality;
	object->object.current_body_damage = state->current_body_damage;
	object->object.recent_body_damage = state->recent_body_damage;
	object->object.current_shield_damage = state->current_shield_damage;
	object->object.recent_shield_damage = state->recent_shield_damage;
}

/* the distributed netcode's client, replaying the damage its host dealt
(port/linux/game/network_damage.c): a player's screen shaken and flashed */
void damage_replay_player_effect(
	long player_index,
	struct damage_data *damage,
	real total_damage)
{
	player_effect_start(player_index, damage, &damage->direction, damage->scale, total_damage);
}

/* ... what the damage did to the object, but for the harm (the host's
vitality comes with its state): knockback, a unit's flinch, pain sound,
stun and unzooming, and whom it was hit by */
void damage_replay_aftermath(
	long object_index,
	struct damage_data *damage,
	unsigned long being_damaged_flags,
	real shield_damage,
	real body_damage,
	real body_damage_multiplier,
	short body_part)
{
	/* (no statistics, which object_damage_aftermath otherwise keeps: the
	host's come as they are) */
	SET_FLAG(damage->flags, _damage_no_statistics_bit, FALSE);
	object_damage_aftermath(object_index, damage, being_damaged_flags & ~FLAG(_object_being_damaged_body_depleted_bit),
		shield_damage, body_damage, body_damage_multiplier, body_part);
}

/* ... a player's killing blow, dealt as the host dealt it */
void damage_replay_kill(
	long object_index,
	struct damage_data *damage,
	short node_index,
	short region_index,
	short material_index)
{
	SET_FLAG(damage->flags, _damage_kill_instantly_bit, TRUE);
	distributed_damage_authorized = TRUE;
	object_cause_damage(damage, object_index, node_index, region_index, material_index, NULL);
	distributed_damage_authorized = FALSE;
}

/* the host's: what damage_set_network_state takes */
void damage_get_network_state(
	long object_index,
	struct damage_network_state *state)
{
	struct object_datum *object = object_get(object_index);

	state->shield_depleted = TEST_FLAG(object->object.damage_flags, _object_shield_depleted_bit);
	state->shield_charging = TEST_FLAG(object->object.damage_flags, _object_shield_charging_bit);
	state->shield_over_charging = TEST_FLAG(object->object.damage_flags, _object_shield_over_charging_bit);
	state->body_vitality = object->object.body_vitality;
	state->shield_vitality = object->object.shield_vitality;
	state->current_body_damage = object->object.current_body_damage;
	state->recent_body_damage = object->object.recent_body_damage;
	state->current_shield_damage = object->object.current_shield_damage;
	state->recent_shield_damage = object->object.recent_shield_damage;
}

/* the automated network tests (port/linux/game/network_test.c): kills the
object as falling damage does (an act of god), but credited to a player, so
that the kill counts in the game's scores */
void damage_kill_object_for_player(
	long object_index,
	long player_index)
{
	struct game_globals_falling_damage *falling_damage = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->falling_damage,
		0,
		struct game_globals_falling_damage);
	struct player_datum *player = player_get(player_index);
	struct damage_data damage;

	if (falling_damage->falling_damage.index == NONE)
		return;
	damage_data_new(&damage, falling_damage->falling_damage.index);
	damage.scale = 1.f;
	SET_FLAG(damage.flags, _damage_kill_instantly_bit, TRUE);
	damage.owner_player_index = player_index;
	damage.owner_object_index = player->unit_index;
	damage.owner_team_index = (short)player->team_index;
	object_cause_damage(&damage, object_index, NONE, NONE, NONE, NULL);
}
#endif
