/*
ACTOR_COMBAT.C

symbols in this file:
0000F590 0090:
	_code_0000f590 (0000)
0000F620 0030:
	_actor_combat_fire_wildly (0000)
0000F650 0040:
	_actor_combat_disable_bursts (0000)
0000F690 0030:
	_actor_firing_blindly (0000)
0000F6C0 0040:
	_actor_combat_currently_firing_burst (0000)
0000F700 0040:
	_actor_get_weapon_definition (0000)
0000F740 00c0:
	_actor_get_weapon_vector (0000)
0000F800 0080:
	_actor_combat_get_firing_variant_definition (0000)
0000F880 00a0:
	_code_0000f880 (0000)
0000F920 0040:
	_code_0000f920 (0000)
0000F960 0020:
	_fast_ftol (0000)
0000F980 0010:
	_sine (0000)
0000F990 0010:
	_cosine (0000)
0000F9A0 0010:
	_tangent (0000)
0000F9B0 0020:
	_cross_product2d (0000)
0000F9D0 0030:
	_add_vectors3d (0000)
0000FA00 0020:
	_random_boolean (0000)
0000FA20 0020:
	_valid_real (0000)
0000FA40 0160:
	_actor_combat_find_nearby_target (0000)
0000FBA0 00b0:
	_code_0000fba0 (0000)
0000FC50 00d0:
	_code_0000fc50 (0000)
0000FD20 01c0:
	_actor_combat_reaim_grenade (0000)
0000FEE0 0110:
	_actor_combat_build_grenade_trajectory (0000)
0000FFF0 0100:
	_actor_combat_find_grenade_target (0000)
000100F0 0370:
	_actor_combat_check_collateral_damage (0000)
00010460 0120:
	_actor_combat_plan_grenade_trajectory (0000)
00010580 0040:
	_valid_realcmp (0000)
000105C0 0060:
	_valid_real_normal3d (0000)
00010620 00b0:
	_code_00010620 (0000)
000106D0 02d0:
	_actor_aim_projectile (0000)
000109A0 07b0:
	_code_000109a0 (0000)
00011150 0060:
	_actor_combat_retarget_grenade (0000)
000111B0 0220:
	_actor_aim_grenade (0000)
000113D0 1240:
	_actor_combat_update (0000)
002444A0 000e:
	??_C@_0O@HEGILGAC@weapon_vector?$AA@ (0000)
002444B0 0021:
	??_C@_0CB@NKOAMEKE@c?3?2halo?2SOURCE?2ai?2actor_combat?4c@ (0000)
002444D4 0035:
	??_C@_0DF@COLBENAC@burst_geometry_reference?5?$CG?$CG?5firi@ (0000)
0024450C 0004:
	__real@3dcccccd (0000)
00244510 0004:
	__real@3fd9999a (0000)
00244514 0016:
	??_C@_0BG@ODFAEBCF@projectile_definition?$AA@ (0000)
00244530 0042:
	??_C@_0EC@CDIDOCJH@grenade_origin?5?$CG?$CG?5desired_impact@ (0000)
00244574 0004:
	__real@3e4ccccd (0000)
00244578 0008:
	__real@3f50624de0000000 (0000)
00244580 0043:
	??_C@_0ED@MKICAFGB@actor?9?$DOcontrol?4current_fire_targ@ (0000)
002445C4 002b:
	??_C@_0CL@GJIEKICC@?$CFs?3?5assert_valid_real_normal3d?$CI?$CF@ (0000)
002445F0 0007:
	??_C@_06NBFPPGEG@vector?$AA@ (0000)
002445F8 0004:
	__real@3f490fdb (0000)
002445FC 003b:
	??_C@_0DL@NNPMKEAI@?$CFs?3?5proj?5?$CF?41f?5rof?5?$CF?41f?5dmg?1s?5?$CF?41@ (0000)
00244638 0020:
	??_C@_0CA@FHOKFHD@?$CFs?3?5manual?5damage?5modifier?5?$CF?42f?$AA@ (0000)
00244658 0010:
	??_C@_0BA@FHPPJCFM@?$CGnew_aim_vector?$AA@ (0000)
00244668 0021:
	??_C@_0CB@CPANGHMG@?$CGactor?9?$DOcontrol?4burst_aim_vector@ (0000)
0024468C 000f:
	??_C@_0P@DBEDAMI@aiming_success?$AA@ (0000)
0024469C 0012:
	??_C@_0BC@CHLPMLFC@?$CBhold_burst_start?$AA@ (0000)
002446B0 0025:
	??_C@_0CF@EHKIDLPJ@new_state?$CB?$DNactor?9?$DOcontrol?4fire_s@ (0000)
002446D8 0018:
	??_C@_0BI@MMOBPCCM@firing_decision?5?$DN?$DN?5NONE?$AA@ (0000)
002446F0 0004:
	__real@3f3504f3 (0000)
002446F4 0004:
	__real@3e3377ed (0000)
002446F8 004b:
	??_C@_0EL@FHBLJBEG@actor?9?$DOcontrol?4current_fire_targ@ (0000)
00244744 0015:
	??_C@_0BF@GLLAMKHP@weapon_index?5?$CB?$DN?5NONE?$AA@ (0000)
0024475C 0036:
	??_C@_0DG@EHGBJHNC@actor?9?$DOcontrol?4current_fire_targ@ (0000)
*/

/* ---------- headers */

#define normalize3d normalize3d_inline
#define point_from_line3d actor_combat_point_from_line3d_inline
#define vector_from_points3d actor_combat_vector_from_points3d_inline
#define magnitude_squared3d actor_combat_magnitude_squared3d_inline
#define distance_squared3d actor_combat_distance_squared3d_inline
#define distance3d actor_combat_distance3d_inline
#define real_random_range actor_combat_real_random_range_inline
#define collision_test_line actor_combat_collision_test_line_inline
#include "cseries.h"

#include "ai.h"
#include "ai_communication.h"
#include "ai_debug.h"
#include "actor_definitions.h"
#include "actors.h"
#include "encounters.h"
#include "props.h"
#include "items/projectile_definitions.h"
#include "items/projectiles.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "game/game_globals.h"
#include "game/game.h"
#include "game/players.h"
#include "main/console.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "tag_files/tag_files.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"
#undef collision_test_line
#undef real_random_range
#undef distance_squared3d
#undef distance3d
#undef magnitude_squared3d
#undef vector_from_points3d
#undef point_from_line3d
#undef normalize3d

#include "math/real_math_declarations.h"

/* ---------- constants */

enum
{
	_actor_fire_target_none = 0,
	_actor_fire_target_prop,
	_actor_fire_target_manual_point,

	_vehicle_ai_weapon_cannot_rotate_bit = 8,

	MAXIMUM_COLLATERAL_DAMAGE_ACTORS = 32,
};

/* actor_definition.flags2 has no owner enumeration in actor_definitions.h yet;
 * actor_firing_position.c, actor_moving.c, actor_stimulus.c and encounters.c
 * carry partial TU-local copies of the same bit names. */
enum
{
	_actor_definition_flags2_avoid_all_enemy_attack_vectors_bit = 0,
	_actor_definition_flags2_must_stand_to_fire_bit,
	_actor_definition_flags2_must_stop_to_fire_bit,
	_actor_definition_flags2_disallow_vehicle_combat_bit,
	_actor_definition_flags2_pathfinding_ignores_danger_bit,
	_actor_definition_flags2_panic_in_groups_bit,
	_actor_definition_flags2_no_corpse_shooting_bit,
	NUMBER_OF_ACTOR_DEFINITION_FLAGS2
};

/* No shared header owns these tag/actor domains yet; action_charge.c carries a
 * strafing-only copy, and actors.c, action_obey.c and actor_perception.c carry
 * combat-status copies. */
enum
{
	_actor_special_fire_mode_none = 0,
	_actor_special_fire_mode_overcharge,
	_actor_special_fire_mode_secondary,
	NUMBER_OF_ACTOR_SPECIAL_FIRE_MODES,
};

enum
{
	_actor_special_fire_situation_never = 0,
	_actor_special_fire_situation_enemy_visible,
	_actor_special_fire_situation_enemy_out_of_sight,
	_actor_special_fire_situation_strafing,
	NUMBER_OF_ACTOR_SPECIAL_FIRE_SITUATIONS,
};

enum
{
	_actor_combat_status_none = 0,
	_actor_combat_status_wary,
	_actor_combat_status_investigate,
	_actor_combat_status_definite,
	_actor_combat_status_certain,
	_actor_combat_status_clear_los,
	_actor_combat_status_dangerous,
	_actor_combat_status_visible,
	NUMBER_OF_ACTOR_COMBAT_STATUS_LEVELS,
};

/* ---------- macros */

#define GRENADE_AIMING_ANGLE_COSINE 0.8660254f
#define GRENADE_AIMING_ANGLE_SINE 0.5f
#define actor_combat_vehicle_definition_get(index) \
	((struct actor_combat_vehicle_definition_view *)vehicle_specific_definition_get(index))

/* ---------- structures */

struct encounter_actor_iterator
{
	long encounter_index;
	long index;
	long next_index;
};

struct actor_combat_vehicle_definition_view
{
	byte __unknown0[0x2F0];
	unsigned long flags;
};

typedef char actor_combat_vehicle_definition_flags_offset_assert[
	offsetof(struct actor_combat_vehicle_definition_view, flags) == 0x2F0 ? 1 : -1];

/* ---------- prototypes */

static boolean actor_combat_enable_special_fire_situation(
	long actor_index,
	short special_fire_situation);
static void actor_combat_get_firing_parameters(
	long actor_index,
	struct actor_variant_definition *firing_variant_definition,
	struct actor_burst_geometry **burst_geometry_reference,
	struct actor_firing_pattern **firing_pattern_reference);
static void actor_start_pause(
	long actor_index);
static boolean actor_start_first_burst_delay(
	long actor_index,
	struct actor_variant_definition *firing_variant_definition);
static boolean actor_combat_allow_special_fire_situation(
	long actor_index,
	short special_fire_situation);
static void actor_start_burst(
	long actor_index);
static void actor_combat_find_nearby_target(
	real_point3d *target_point,
	real miss_distance);
static void actor_get_weapon_vector(
	long actor_index,
	real_vector3d *weapon_vector);
static struct projectile_definition *actor_get_grenade_definition(
	short grenade_type_index);
static boolean actor_combat_build_grenade_trajectory(
	short grenade_type_index,
	real_point3d const *grenade_origin,
	real velocity_max,
	real_point3d const *desired_impact_point,
	real *target_ballistic_fraction_min,
	boolean lob,
	real_vector3d *aim_vector,
	real *aim_speed,
	real *aim_ticks,
	real_vector3d *aim_velocity,
	real *aim_gravity);
static boolean actor_combat_reaim_grenade(
	long actor_index,
	real_point3d const *grenade_origin);
static boolean actor_combat_retarget_grenade(
	long actor_index,
	real_point3d const *desired_grenade_target);

/* ---------- globals */

/* ---------- public code */

static boolean actor_combat_enable_special_fire_situation(
	long actor_index,
	short special_fire_situation)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	switch (special_fire_situation)
	{
	case _actor_special_fire_situation_enemy_visible:
		result =
			actor->control.current_fire_target_type == _actor_fire_target_prop &&
			actor->target.target_type >= _actor_target_clear_line_of_sight_enemy;
		break;

	case _actor_special_fire_situation_enemy_out_of_sight:
		result =
			actor->control.current_fire_target_type == _actor_fire_target_none &&
			actor->target.target_type >= _actor_target_uninspected_orphan &&
			actor->target.target_really_alive &&
			actor->target.since_any_target_visible_timer >= 75;
		break;

	case _actor_special_fire_situation_strafing:
		result =
			actor->control.current_fire_target_type == _actor_fire_target_prop &&
			actor->target.target_type >= _actor_target_clear_line_of_sight_enemy &&
			actor->input.vehicle_gunner;
		break;
	}

	return result;
}

void actor_combat_fire_wildly(
	long actor_index,
	short fire_ticks)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->control.fire_state = _actor_fire_state_wild;
	actor->control.fire_state_timer = fire_ticks;

	return;
}

void actor_combat_disable_bursts(
	long actor_index,
	long disable_timer)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->control.burst_disable_timer =
		MAX(actor->control.burst_disable_timer, disable_timer);

	return;
}

boolean actor_firing_blindly(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	return actor->control.fire_state == _actor_fire_state_wild;
}

boolean actor_combat_currently_firing_burst(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->control.current_fire_target_type > _actor_fire_target_none)
	{
		result = actor->control.fire_state == _actor_fire_state_bursting;
	}

	return result;
}

struct weapon_definition *actor_get_weapon_definition(
	long actor_index)
{
	struct weapon_definition *result = NULL;
	long weapon_index = actor_get_weapon(actor_index);

	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		result = weapon_definition_get(weapon->definition_index);
	}

	return result;
}

static void actor_get_weapon_vector(
	long actor_index,
	real_vector3d *weapon_vector)
{
	struct actor_datum *actor = actor_get(actor_index);
	long unit_index = actor->meta.unit_index;
	boolean have_vector = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_combat.c",
		1166,
		weapon_vector);

	if (actor->input.vehicle_gunner)
	{
		struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);
		struct actor_combat_vehicle_definition_view *vehicle_definition =
			actor_combat_vehicle_definition_get(vehicle->definition_index);

		unit_index = actor->input.vehicle_index;
		if (TEST_FLAG(
			vehicle_definition->flags,
			_vehicle_ai_weapon_cannot_rotate_bit))
		{
			*weapon_vector = vehicle->object.forward;
			have_vector = TRUE;
		}
	}

	if (!have_vector)
	{
		unit_get(unit_index);
		unit_get_aiming_vector(unit_index, weapon_vector);
		unit_clip_to_aiming_bounds(unit_index, weapon_vector, TRUE);
	}

	return;
}

struct actor_variant_definition *actor_combat_get_firing_variant_definition(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *firing_variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	long weapon_index = actor_get_weapon(actor_index);

	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *weapon_definition =
			weapon_definition_get(weapon->definition_index);

		if (weapon_definition != NULL &&
			weapon_definition->weapon.ai_firing_parameters.index != NONE)
		{
			firing_variant_definition = actor_variant_definition_get(
				weapon_definition->weapon.ai_firing_parameters.index);
		}
	}

	return firing_variant_definition;
}

static void actor_combat_get_firing_parameters(
	long actor_index,
	struct actor_variant_definition *firing_variant_definition,
	struct actor_burst_geometry **burst_geometry_reference,
	struct actor_firing_pattern **firing_pattern_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_burst_geometry *burst_geometry =
		&firing_variant_definition->ranged_combat.burst_geometry;
	struct actor_firing_pattern *firing_pattern = NULL;

	if (actor->emotions.berserk)
	{
		firing_pattern = &firing_variant_definition->ranged_combat.berserk_pattern;
	}
	else if (actor->control.firing_at_new_target)
	{
		firing_pattern = &firing_variant_definition->ranged_combat.new_target_pattern;
	}
	else if (actor->control.firing_while_moving)
	{
		firing_pattern = &firing_variant_definition->ranged_combat.moving_pattern;
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_combat.c",
		1677,
		burst_geometry_reference && firing_pattern_reference);

	*burst_geometry_reference = burst_geometry;
	*firing_pattern_reference = firing_pattern;

	return;
}

static struct projectile_definition *actor_get_grenade_definition(
	short grenade_type_index)
{
	struct game_globals_grenade *grenade = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->grenades,
		grenade_type_index,
		struct game_globals_grenade);
	struct projectile_definition *projectile_definition = NULL;

	if (grenade && grenade->projectile.index != NONE)
	{
		projectile_definition =
			projectile_definition_get(grenade->projectile.index);
	}

	return projectile_definition;
}

static void actor_combat_find_nearby_target(
	real_point3d *target_point,
	real miss_distance)
{
	real_point3d above;
	real_point3d target;
	real_vector3d direction;
	struct collision_result collision;

	/* Keep January's inlined real-math schedule without materializing the
	 * header helpers as actor_combat-owned COMDATs. */
	above.x = global_up3d->i*1.5f + target_point->x;
	above.y = global_up3d->j*1.5f + target_point->y;
	above.z = global_up3d->k*1.5f + target_point->z;
	vector3d_from_angle(
		&direction,
		real_seed_random_range(get_global_random_seed_address(), -_pi, _pi));
	target.x = direction.i*miss_distance + above.x;
	target.y = direction.j*miss_distance + above.y;
	target.z = direction.k*miss_distance + above.z;

	{
		real_vector3d collision_vector;

		collision_vector.i = above.x - target_point->x;
		collision_vector.j = above.y - target_point->y;
		collision_vector.k = above.z - target_point->z;
		if (collision_test_vector(
			FLAG(_collision_test_front_facing_surfaces_bit) |
				FLAG(_collision_test_back_facing_surfaces_bit) |
				FLAG(_collision_test_structure_bit),
			target_point,
			&collision_vector,
			NONE,
			&collision))
		{
			above = *target_point;
		}
	}

	{
		real_vector3d collision_vector;

		collision_vector.i = target.x - above.x;
		collision_vector.j = target.y - above.y;
		collision_vector.k = target.z - above.z;
		if (collision_test_vector(
			FLAG(_collision_test_front_facing_surfaces_bit) |
				FLAG(_collision_test_back_facing_surfaces_bit) |
				FLAG(_collision_test_structure_bit),
			&above,
			&collision_vector,
			NONE,
			&collision))
		{
			miss_distance = MAX(0.0f, collision.t*miss_distance - 0.1f);
			target.x = direction.i*miss_distance + above.x;
			target.y = direction.j*miss_distance + above.y;
			target.z = direction.k*miss_distance + above.z;
		}
	}

	*target_point = target;

	return;
}

static void actor_start_pause(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_burst_geometry *burst_geometry;
	struct actor_firing_pattern *firing_pattern;
	real burst_separation;

	actor_combat_get_firing_parameters(
		actor_index,
		actor_combat_get_firing_variant_definition(actor_index),
		&burst_geometry,
		&firing_pattern);

	burst_separation = actor_combat_real_random_range_inline(
		burst_geometry->burst_separation_lower_bound,
		burst_geometry->burst_separation_upper_bound)*
		game_difficulty_get_team_value(
			_game_difficulty_value_burst_separation,
			actor->meta.team_index);

	if (firing_pattern && firing_pattern->burst_separation_modifier != 0.0f)
	{
		burst_separation *= firing_pattern->burst_separation_modifier;
	}

	if (actor->external_orders.playfighting)
	{
		burst_separation *= 1.7f;
	}

	actor->control.fire_state_timer = (short)(burst_separation*TICKS_PER_SECOND);

	return;
}

static boolean actor_start_first_burst_delay(
	long actor_index,
	struct actor_variant_definition *firing_variant_definition)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean fire_immediately = actor->orders.combat.override_firing_restrictions;

	if (actor->control.current_fire_target_type == _actor_fire_target_prop)
	{
		struct prop_datum *prop =
			prop_get(actor->control.current_fire_target_prop_index);

		if (prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan)
		{
			actor->firing_positions.pursuit_fired_at_orphan = TRUE;
			fire_immediately = TRUE;
		}
	}

	if (fire_immediately)
	{
		actor->control.fire_state_timer = 0;
	}
	else
	{
		actor->control.fire_state_timer = (short)(actor_combat_real_random_range_inline(
			firing_variant_definition->ranged_combat.first_burst_delay_lower_bound,
			firing_variant_definition->ranged_combat.first_burst_delay_upper_bound)*
			TICKS_PER_SECOND);
	}

	return !fire_immediately;
}

static boolean actor_combat_reaim_grenade(
	long actor_index,
	real_point3d const *grenade_origin)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct projectile_definition *projectile_definition =
		actor_get_grenade_definition(
			variant_definition->grenade_combat.grenade_type);
	real_vector3d aim_vector;
	real aim_speed;
	real aim_ticks;
	boolean linear;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_combat.c",
		1731,
		projectile_definition);

	if (projectile_aim(
		projectile_definition,
		grenade_origin,
		&actor->control.grenade_current_target,
		NULL,
		NULL,
		NULL,
		&actor->control.grenade_current_aim_speed,
		actor->control.grenade_current_lob,
		&aim_vector,
		&aim_speed,
		&aim_ticks,
		NULL,
		&linear))
	{
		boolean within_aiming_angle;

		{
			real_vector2d aim_vector2d;

			aim_vector2d.i = aim_vector.i;
			aim_vector2d.j = aim_vector.j;
			within_aiming_angle = normalize2d(&aim_vector2d) > 0.0f &&
				aim_vector2d.i*actor->input.facing_vector.i +
					aim_vector2d.j*actor->input.facing_vector.j >
						GRENADE_AIMING_ANGLE_COSINE;
		}

		if (within_aiming_angle)
		{
			real_vector3d aim_velocity;
			real aim_gravity;

			aim_velocity.i = aim_vector.i*aim_speed;
			aim_velocity.j = aim_vector.j*aim_speed;
			aim_velocity.k = aim_vector.k*aim_speed;

			aim_gravity = linear ?
				0.0f :
				projectile_get_ballistic_acceleration(projectile_definition);

			if (ai_test_ballistic_line_of_fire(
				actor_index,
				grenade_origin,
				aim_ticks,
				&aim_velocity,
				aim_gravity,
				actor->control.grenade_current_ignore_object_index,
				actor->input.vehicle_index != NONE))
			{
				actor->control.grenade_current_aim_vector = aim_vector;
				actor->control.grenade_current_aim_speed = aim_speed;
				result = TRUE;
			}
		}
	}

	return result;
}

static boolean actor_combat_build_grenade_trajectory(
	short grenade_type_index,
	real_point3d const *grenade_origin,
	real velocity_max,
	real_point3d const *desired_impact_point,
	real *target_ballistic_fraction_min,
	boolean lob,
	real_vector3d *aim_vector,
	real *aim_speed,
	real *aim_ticks,
	real_vector3d *aim_velocity,
	real *aim_gravity)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_combat.c",
		1780,
		grenade_origin && desired_impact_point && aim_vector && aim_speed);

	{
		struct projectile_definition *projectile_definition =
			actor_get_grenade_definition(grenade_type_index);

		if (projectile_definition)
		{
			boolean linear = FALSE;

			if (projectile_aim(
				projectile_definition,
				grenade_origin,
				desired_impact_point,
				&velocity_max,
				NULL,
				target_ballistic_fraction_min,
				NULL,
				FALSE,
				aim_vector,
				aim_speed,
				aim_ticks,
				NULL,
				&linear))
			{
				result = TRUE;

				if (aim_velocity)
				{
					real speed = *aim_speed;

					aim_velocity->i = speed*aim_vector->i;
					aim_velocity->j = speed*aim_vector->j;
					aim_velocity->k = speed*aim_vector->k;
				}

				if (aim_gravity)
				{
					*aim_gravity = linear ?
						0.0f :
						projectile_get_ballistic_acceleration(projectile_definition);
				}
			}
		}
	}

	return result;
}

boolean actor_combat_find_grenade_target(
	long actor_index,
	real_point3d *grenade_target,
	long *grenade_target_prop_index,
	long *grenade_ignore_object_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	boolean result = FALSE;

	if (actor->target.target_prop_index != NONE)
	{
		struct prop_datum *prop = prop_get(actor->target.target_prop_index);

		if (prop->enemy &&
			!prop->dead &&
			((prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged) ||
				prop->state == _prop_state_uninspected_orphan) &&
			prop->distance > variant_definition->grenade_combat.grenade_range_lower_bound &&
			prop->distance < variant_definition->grenade_combat.grenade_range_upper_bound)
		{
			*grenade_target = prop->body_position;
			grenade_target->z += 0.2f;
			*grenade_target_prop_index = actor->target.target_prop_index;
			*grenade_ignore_object_index = prop->vehicle_index;
			result = TRUE;

			if (actor->external_orders.playfighting)
			{
				actor_combat_find_nearby_target(grenade_target, 1.5f);
			}
		}
	}

	return result;
}

boolean actor_combat_check_collateral_damage(
	long actor_index,
	real enemy_radius,
	real collateral_damage_radius,
	real_point3d const *test_point,
	short *threat_count)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result;
	short threats;
	short counted_actor_count;
	long counted_actor_indices[MAXIMUM_COLLATERAL_DAMAGE_ACTORS];

	/* Preserve the original tag validation before iterating combat state. */
	actor_variant_definition_get(actor->meta.variant_definition_index);
	result = TRUE;
	threats = 0;
	counted_actor_count = 0;

	{
		struct prop_iterator prop_iterator;
		struct prop_datum *prop;

		prop_iterator_new(&prop_iterator, actor_index);
		for (prop = prop_iterator_next(&prop_iterator);
			prop;
			prop = prop_iterator_next(&prop_iterator))
		{
			if (prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged &&
				!prop->dead)
			{
				if (prop->enemy)
				{
					if (actor_combat_distance_squared3d_inline(&prop->body_position, test_point) <
						enemy_radius*enemy_radius)
					{
						if (prop->player)
						{
							threats += 10;
						}
						else if (prop->vehicle_index != NONE)
						{
							threats += 5;
						}
						else if (prop->actor_index != NONE)
						{
							if (counted_actor_count < NUMBEROF(counted_actor_indices))
							{
								counted_actor_indices[counted_actor_count++] = prop->actor_index;
							}

							if (prop->swarm)
							{
								threats += actor_get(prop->actor_index)->meta.swarm_unit_count;
							}
							else
							{
								threats++;
							}
						}
					}
				}
				else if (collateral_damage_radius > 0.0f &&
					actor_combat_distance_squared3d_inline(&prop->body_position, test_point) <
						collateral_damage_radius*collateral_damage_radius)
				{
					result = FALSE;
					break;
				}
			}
		}
	}

	if (enemy_radius > 0.0f && actor->target.target_prop_index != NONE)
	{
		struct prop_datum *target_prop = prop_get(actor->target.target_prop_index);

		if (target_prop->actor_index != NONE)
		{
			struct actor_datum *target_actor = actor_get(target_prop->actor_index);

			if (target_actor->meta.encounter_index != NONE)
			{
				struct encounter_actor_iterator actor_iterator;
				struct actor_datum *enemy_actor;

				encounter_actor_iterator_new(
					&actor_iterator,
					target_actor->meta.encounter_index);
				for (enemy_actor = encounter_actor_iterator_next(&actor_iterator);
					enemy_actor;
					enemy_actor = encounter_actor_iterator_next(&actor_iterator))
				{
					short index;
					boolean already_counted = FALSE;

					for (index = 0; index < counted_actor_count; index++)
					{
						if (counted_actor_indices[index] == actor_iterator.index)
						{
							already_counted = TRUE;
							break;
						}
					}

					if (!already_counted &&
						actor_combat_distance_squared3d_inline(
							&enemy_actor->input.position.body_position,
							test_point) < enemy_radius*enemy_radius)
					{
						if (enemy_actor->meta.swarm)
						{
							threats += enemy_actor->meta.swarm_unit_count;
						}
						else
						{
							threats++;
						}
					}
				}
			}
		}
	}

	if (result && actor->meta.encounter_index != NONE && collateral_damage_radius > 0.0f)
	{
		struct encounter_actor_iterator actor_iterator;
		struct actor_datum *friend_actor;

		encounter_actor_iterator_new(
			&actor_iterator,
			actor->meta.encounter_index);
		for (friend_actor = encounter_actor_iterator_next(&actor_iterator);
			friend_actor;
			friend_actor = encounter_actor_iterator_next(&actor_iterator))
		{
			if (actor_combat_distance_squared3d_inline(
					&friend_actor->input.position.body_position,
					test_point) < collateral_damage_radius*collateral_damage_radius)
			{
				result = FALSE;
				break;
			}
		}
	}

	if (threat_count)
	{
		*threat_count = threats;
	}

	return result;
}

boolean actor_combat_plan_grenade_trajectory(
	long actor_index,
	short trajectory_type,
	real_point3d const *grenade_target,
	long grenade_target_prop_index,
	long grenade_ignore_object_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	real_point3d grenade_origin = actor->input.position.head_position;
	real_vector3d aim_vector;
	real_vector3d aim_velocity;
	real aim_speed;
	real aim_ticks;
	real aim_gravity;
	boolean result = FALSE;

	if (actor_combat_build_grenade_trajectory(
			variant_definition->grenade_combat.grenade_type,
			&grenade_origin,
			variant_definition->grenade_combat.grenade_maximum_velocity,
			grenade_target,
			NULL,
			FALSE,
			&aim_vector,
			&aim_speed,
			&aim_ticks,
			&aim_velocity,
			&aim_gravity) &&
		ai_test_ballistic_line_of_fire(
			actor_index,
			&grenade_origin,
			aim_ticks,
			&aim_velocity,
			aim_gravity,
			grenade_ignore_object_index,
			actor->input.vehicle_index != NONE))
	{
		actor->control.grenade_current_target = *grenade_target;
		actor->control.grenade_current_prop_index = grenade_target_prop_index;
		actor->control.grenade_current_ignore_object_index = grenade_ignore_object_index;
		actor->control.grenade_current_aim_vector = aim_vector;
		actor->control.grenade_current_aim_speed = aim_speed;
		actor->control.grenade_current_lob = FALSE;

		result = TRUE;
	}

	return result;
}

static boolean actor_combat_allow_special_fire_situation(
	long actor_index,
	short special_fire_situation)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	if (special_fire_situation == _actor_special_fire_situation_strafing)
	{
		struct prop_datum *prop;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_combat.c",
			970,
			actor->control.current_fire_target_type == _actor_fire_target_prop);

		prop = prop_get(actor->control.current_fire_target_prop_index);
		if (prop->vehicle_index != NONE)
		{
			result = TRUE;
		}
		else if (prop->player)
		{
			result = FALSE;
		}
		else
		{
			short enemy_count = 0;

			actor_combat_check_collateral_damage(
				actor_index,
				6.0f,
				0.0f,
				&prop->body_position,
				&enemy_count);
			result = enemy_count >= 3;
		}
	}

	return result;
}

long actor_aim_projectile(
	long actor_index,
	real_point3d const *origin,
	real_vector3d *vector,
	real *error_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	long target_unit_index = NONE;

	if (actor->control.fire_state == _actor_fire_state_bursting)
	{
		struct actor_debug_info *actor_debug_info =
			&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

		actor_debug_info->last_projectile_aiming_time = game_time_get();

		if (actor->control.current_fire_target_type == _actor_fire_target_prop &&
			actor->control.current_fire_target_prop_index != NONE)
		{
			struct prop_datum *prop =
				prop_get(actor->control.current_fire_target_prop_index);

			if (prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged)
			{
				target_unit_index = prop->unit_index;
			}
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_combat.c",
			1071,
			vector);

		actor_debug_info->field_60 = actor->control.burst_aim_by_vector;
		actor_debug_info->field_64 = *origin;

		if (actor->control.burst_aim_by_vector)
		{
			*vector = actor->control.burst_aim_vector;
		}
		else
		{
			real_point3d const *burst_target = &actor->control.burst_target;

			actor_debug_info->field_7C = *burst_target;
			vector->i = burst_target->x - origin->x;
			vector->j = burst_target->y - origin->y;
			vector->k = burst_target->z - origin->z;
			normalize3d(vector);
		}

		match_assert_valid_real_normal3d(
			"c:\\halo\\SOURCE\\ai\\actor_combat.c",
			1090,
			vector);
		actor_debug_info->field_70 = *vector;

		{
			real_vector3d weapon_vector;

			actor_get_weapon_vector(actor_index, &weapon_vector);
			if (dot_product3d(&weapon_vector, vector) < GRENADE_AIMING_ANGLE_COSINE)
			{
				real_vector3d rotation_axis;
				boolean aiming_success = TRUE;

				cross_product3d(&weapon_vector, vector, &rotation_axis);
				if (normalize3d(&rotation_axis) == 0.0f)
				{
					perpendicular3d(&weapon_vector, &rotation_axis);
					if (normalize3d(&rotation_axis) == 0.0f)
					{
						aiming_success = FALSE;
					}
				}

				*vector = weapon_vector;
				if (aiming_success)
				{
					rotate_vector_about_axis(
						vector,
						&rotation_axis,
						GRENADE_AIMING_ANGLE_SINE,
						GRENADE_AIMING_ANGLE_COSINE);
				}

				actor_debug_info->field_88 = TRUE;
				actor_debug_info->field_98 = weapon_vector;
				actor_debug_info->field_8C = *vector;
			}
			else
			{
				actor_debug_info->field_88 = FALSE;
			}
		}

		*error_reference = actor->control.burst_error;
	}

	return target_unit_index;
}

static void actor_start_burst(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *firing_variant_definition =
		actor_combat_get_firing_variant_definition(actor_index);
	boolean use_nearby_target = FALSE;
	struct actor_burst_geometry *burst_geometry;
	struct actor_firing_pattern *firing_pattern;
	real new_target_time;

	if (actor->control.next_burst_secondary &&
		!actor_combat_allow_special_fire_situation(
			actor_index,
			firing_variant_definition->ranged_combat.special_fire_situation))
	{
		actor->control.next_burst_secondary = FALSE;
	}
	actor->control.fire_burst_secondary = actor->control.next_burst_secondary;
	actor->control.next_burst_secondary = FALSE;

	if (actor->input.vehicle_index == NONE)
	{
		actor->control.firing_while_moving =
			actor->input.in_midair || actor->control.moving;
	}
	else
	{
		struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);

		actor->control.firing_while_moving =
			actor_combat_magnitude_squared3d_inline(&vehicle->object.translational_velocity) > 1.0f;
	}

	new_target_time = firing_variant_definition->ranged_combat.new_target_pattern_time;
	new_target_time *= game_difficulty_get_team_value(
		_game_difficulty_value_new_target_delay,
		actor->meta.team_index);
	actor->control.firing_at_new_target =
		actor->control.current_fire_target_timer < new_target_time*TICKS_PER_SECOND;

	actor_combat_get_firing_parameters(
		actor_index,
		firing_variant_definition,
		&burst_geometry,
		&firing_pattern);

	{
		real burst_duration;

		if (actor->orders.combat.override_burst_duration > 0.0f)
		{
			burst_duration = actor->orders.combat.override_burst_duration;
		}
		else
		{
			burst_duration = actor_combat_real_random_range_inline(
				burst_geometry->burst_duration_lower_bound,
				burst_geometry->burst_duration_upper_bound);
			if (firing_pattern && firing_pattern->burst_duration_modifier != 0.0f)
			{
				burst_duration *= firing_pattern->burst_duration_modifier;
			}
			if (actor->external_orders.playfighting)
			{
				burst_duration *= 0.6f;
			}
		}
		actor->control.fire_state_timer = (short)(burst_duration*TICKS_PER_SECOND);
	}

	{
		real burst_error = firing_variant_definition->ranged_combat.projectile_error_angle;

		burst_error *= game_difficulty_get_team_value(
			_game_difficulty_value_projectile_error,
			actor->meta.team_index);

		if (firing_pattern && firing_pattern->error_angle_modifier != 0.0f)
		{
			burst_error *= firing_pattern->error_angle_modifier;
		}
		if (actor->external_orders.playfighting)
		{
			burst_error = 2.0f*burst_error + DEGREES_TO_RADIANS(1.0f);
		}
		actor->control.burst_error = burst_error;
	}

	actor->control.burst_damage_modifier = 0.0f;
	if (firing_variant_definition->ranged_combat.weapon_damage_modifier > 0.0f)
	{
		actor->control.burst_damage_modifier =
			firing_variant_definition->ranged_combat.weapon_damage_modifier;
		if (ai_debug.print_damage_modifiers)
		{
			console_printf(
				FALSE,
				"%s: manual damage modifier %.2f",
				tag_name_strip_path(tag_get_name(actor->meta.variant_definition_index)),
				actor->control.burst_damage_modifier);
		}
	}
	else if (firing_variant_definition->ranged_combat.damage_per_second > 0.0f)
	{
		long weapon_index = actor_get_weapon(actor_index);

		if (weapon_index != NONE)
		{
			struct weapon_datum *weapon = weapon_get(weapon_index);
			real rate_of_fire;
			real projectile_damage = weapon_definition_get_damage_potential(
				weapon->definition_index,
				&rate_of_fire);
			real damage_per_second;

			if (firing_variant_definition->ranged_combat.rate_of_fire > 0.0f &&
				rate_of_fire > firing_variant_definition->ranged_combat.rate_of_fire)
			{
				rate_of_fire = firing_variant_definition->ranged_combat.rate_of_fire;
			}

			damage_per_second = rate_of_fire*projectile_damage;
			if (damage_per_second > 0.0f)
			{
				actor->control.burst_damage_modifier =
					firing_variant_definition->ranged_combat.damage_per_second/damage_per_second;
				if (ai_debug.print_damage_modifiers)
				{
					console_printf(
						FALSE,
						"%s: proj %.1f rof %.1f dmg/s %.1f -> to get %.1f mod= %.2f",
						tag_name_strip_path(tag_get_name(actor->meta.variant_definition_index)),
						projectile_damage,
						rate_of_fire,
						damage_per_second,
						firing_variant_definition->ranged_combat.damage_per_second,
						actor->control.burst_damage_modifier);
				}
			}
		}
	}

	if (actor->control.fire_burst_secondary || actor->control.overcharging_weapon)
	{
		if (firing_variant_definition->ranged_combat.special_damage_modifier > 0.0f)
		{
			actor->control.burst_damage_modifier *=
				firing_variant_definition->ranged_combat.special_damage_modifier;
		}
		actor->control.burst_error +=
			firing_variant_definition->ranged_combat.special_projectile_error;
	}

	if (firing_variant_definition->ranged_combat.weapon_bombardment_range > 0.0f &&
		actor->control.current_fire_target_type == _actor_fire_target_prop)
	{
		struct prop_datum *prop = prop_get(actor->control.current_fire_target_prop_index);

		use_nearby_target =
			prop->state < _prop_state_becoming_unacknowledged ||
			prop->state > _prop_state_acknowledged ||
			prop->visibility == _actor_perception_none;
	}

	{
		real_point3d target = actor->control.current_fire_target_position;
		real_vector3d aim_vector;
		real_vector3d burst_relative_position;
		real_vector3d burst_adjustment;
		real origin_angle;
		real return_angle;
		real origin_radius;
		real return_radius;

		if (use_nearby_target)
		{
			actor_combat_find_nearby_target(
				&target,
				firing_variant_definition->ranged_combat.weapon_bombardment_range);
		}

		{
			real_vector3d target_direction;
			real_vector3d const up = {0.0f, 0.0f, 1.0f};

			actor_combat_vector_from_points3d_inline(
				&actor->input.position.head_position,
				&target,
				&target_direction);
			cross_product3d(&target_direction, &up, &aim_vector);
			normalize3d(&aim_vector);
			if (random_boolean())
			{
				negate_vector3d(&aim_vector, &aim_vector);
			}
		}

		origin_angle = actor_combat_real_random_range_inline(
			-burst_geometry->burst_origin_angle,
			burst_geometry->burst_origin_angle);
		return_angle = actor_combat_real_random_range_inline(
			-burst_geometry->burst_return_angle,
			burst_geometry->burst_return_angle) + origin_angle;
		origin_radius = game_difficulty_get_team_value(
			_game_difficulty_value_burst_error,
			actor->meta.team_index)*burst_geometry->burst_origin_radius;
		return_radius = actor_combat_real_random_range_inline(
			burst_geometry->burst_return_length_lower_bound,
			burst_geometry->burst_return_length_upper_bound);
		return_radius *= game_difficulty_get_team_value(
			_game_difficulty_value_burst_error,
			actor->meta.team_index);
		if (actor->external_orders.playfighting)
		{
			origin_radius *= 2.0f;
			return_radius *= 2.0f;
		}

		if (actor->control.fire_state_timer > 0 &&
			burst_geometry->burst_maximum_angular_velocity > 0.0f)
		{
			real burst_ticks = actor->control.fire_state_timer;
			real sweep_angle = MIN(
				burst_ticks*burst_geometry->burst_maximum_angular_velocity*(1.0f/TICKS_PER_SECOND),
				_pi/4.0f);
			real maximum_origin_radius = tangent(sweep_angle)*actor->control.current_fire_target_range;

			if (origin_radius > maximum_origin_radius)
			{
				if (origin_radius < maximum_origin_radius*1.5f)
				{
					actor->control.fire_state_timer =
						(short)fast_ftol(burst_ticks*(origin_radius/maximum_origin_radius));
				}
				else
				{
					actor->control.fire_state_timer = (short)fast_ftol(burst_ticks*1.5f);
					maximum_origin_radius *= 1.5f;
					return_radius = maximum_origin_radius/origin_radius*return_radius;
					origin_radius = maximum_origin_radius;
				}
			}
		}

		{
			real origin_cosine = cosine(origin_angle);
			real origin_sine = sine(origin_angle);
			real return_cosine = cosine(return_angle);
			real return_sine = sine(return_angle);

			burst_relative_position.i = (origin_cosine*aim_vector.i + 0.0f*origin_sine)*origin_radius;
			burst_relative_position.j = (origin_cosine*aim_vector.j + 0.0f*origin_sine)*origin_radius;
			burst_relative_position.k = (origin_cosine*aim_vector.k + origin_sine)*origin_radius;
			burst_adjustment.i = -((return_cosine*aim_vector.i + return_sine*0.0f)*return_radius);
			burst_adjustment.j = -((return_cosine*aim_vector.j + return_sine*0.0f)*return_radius);
			burst_adjustment.k = -((return_cosine*aim_vector.k + return_sine)*return_radius);
		}

		if (actor->control.fire_state_timer > 0)
		{
			real inverse_ticks = 1.0f/actor->control.fire_state_timer;

			burst_adjustment.i *= inverse_ticks;
			burst_adjustment.j *= inverse_ticks;
			burst_adjustment.k *= inverse_ticks;
		}

		actor->control.burst_initial_position = target;
		actor->control.burst_relative_position = burst_relative_position;
		actor->control.burst_adjustment = burst_adjustment;
		add_vectors3d(
			&actor->control.burst_initial_position,
			&actor->control.burst_relative_position,
			&actor->control.burst_target);
	}

	if (actor->state.combat_status >= _actor_combat_status_visible)
	{
		boolean traitor = FALSE;
		long cause_unit_index = NONE;
		short communication_type;

		if (actor->control.current_fire_target_type == _actor_fire_target_prop)
		{
			struct prop_datum *prop = prop_get(actor->control.current_fire_target_prop_index);

			traitor = prop->ally;
			cause_unit_index = prop->unit_index;
		}

		if (actor->emotions.berserk)
		{
			communication_type = _ai_communication_shooting_berserk;
		}
		else if (traitor)
		{
			communication_type = _ai_communication_shooting_traitor;
		}
		else if (actor->situation.cumulative_threats[_actor_threat_visible] >= 5)
		{
			communication_type = _ai_communication_shooting_group;
		}
		else
		{
			communication_type = actor->input.vehicle_gunner ?
				_ai_communication_shooting_vehicle :
				_ai_communication_shooting;
		}

		ai_communication_event(
			communication_type,
			actor->meta.unit_index,
			cause_unit_index,
			_comm_hostility_enemy,
			NONE,
			NONE,
			NULL);
	}

	return;
}

static boolean actor_combat_retarget_grenade(
	long actor_index,
	real_point3d const *desired_grenade_target)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	boolean result = FALSE;

	if (actor_combat_check_collateral_damage(
		actor_index,
		variant_definition->grenade_combat.enemy_radius,
		variant_definition->grenade_combat.collateral_damage_radius,
		desired_grenade_target,
		NULL))
	{
		actor->control.grenade_current_target = *desired_grenade_target;
		result = TRUE;
	}

	return result;
}

long actor_aim_grenade(
	long actor_index,
	real_point3d const *origin,
	real_vector3d *vector)
{
	struct actor_datum *actor = actor_get(actor_index);
	long target_unit_index = NONE;
	real_vector3d aim_vector;

	if (actor->control.grenade_current_prop_index != NONE)
	{
		struct prop_datum *prop = prop_get(actor->control.grenade_current_prop_index);

		if (prop->state >= _prop_state_becoming_unacknowledged &&
			prop->state <= _prop_state_acknowledged)
		{
			target_unit_index = prop->unit_index;
		}

		if (prop->state < _prop_state_unacknowledged ||
			prop->state > _prop_state_becoming_acknowledged)
		{
			real_point3d new_desired_target = prop->body_position;

			new_desired_target.z += 0.2f;
			actor_combat_retarget_grenade(
				actor_index,
				&new_desired_target);
		}
	}

	actor_combat_reaim_grenade(actor_index, origin);

	if (actor->input.vehicle_index == NONE)
	{
		real_vector2d horizontal_aim_vector;

		aim_vector = actor->control.grenade_current_aim_vector;
		horizontal_aim_vector = *(real_vector2d const *)&aim_vector;

		if (normalize2d(&horizontal_aim_vector) > 0.0f &&
			dot_product2d(
				(real_vector2d const *)&actor->input.facing_vector,
				&horizontal_aim_vector) < GRENADE_AIMING_ANGLE_COSINE)
		{
			real_vector3d new_aim_vector = actor->input.facing_vector;
			boolean counterclockwise = cross_product2d(
				(real_vector2d const *)&actor->input.facing_vector,
				&horizontal_aim_vector) > 0.0f;

			rotate_vector_about_axis(
				&new_aim_vector,
				global_up3d,
				(counterclockwise ? 1 : -1)*GRENADE_AIMING_ANGLE_SINE,
				GRENADE_AIMING_ANGLE_COSINE);
			scale_vector2d(
				(real_vector2d const *)&new_aim_vector,
				magnitude2d((real_vector2d const *)&aim_vector),
				(real_vector2d *)&new_aim_vector);
			new_aim_vector.k = aim_vector.k;

			match_assert_valid_real_normal3d(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				1865,
				&new_aim_vector);

			aim_vector = new_aim_vector;
		}
	}

	/* BUG (preserved for exact matching): aim_vector is assigned only on foot.
	 * With actor->input.vehicle_index != NONE, January branches from its vehicle
	 * test straight to this scale and reads the unassigned local; the 2011 HCEX
	 * and later /Od builds keep the same path. A corrected build should start
	 * from actor->control.grenade_current_aim_vector.
	 */
	scale_vector3d(
		&aim_vector,
		actor->control.grenade_current_aim_speed,
		vector);

	return target_unit_index;
}

/* ---------- private code */

void actor_combat_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *actor_definition =
		actor_definition_get(actor->meta.definition_index);
	struct actor_variant_definition *actor_variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct actor_variant_definition *firing_variant_definition =
		actor_combat_get_firing_variant_definition(actor_index);
	struct weapon_definition *weapon_definition = actor_get_weapon_definition(actor_index);
	struct unit_datum *unit = unit_get(actor->meta.unit_index);
	boolean hold_burst_start = FALSE;
	long weapon_index = actor_get_weapon(actor_index);
	struct actor_debug_info *actor_debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
	short firing_decision = NONE;
	boolean fire;

	if (actor->control.fire_state_timer > 0)
	{
		actor->control.fire_state_timer--;
	}
	if (actor->control.burst_disable_timer > 0)
	{
		actor->control.burst_disable_timer--;
	}
	if (actor->control.trigger_delay_timer > 0)
	{
		actor->control.trigger_delay_timer--;
	}
	if (actor->control.special_fire_delay > 0)
	{
		actor->control.special_fire_delay--;
	}
	if (actor->control.current_fire_target_type > _actor_fire_target_none)
	{
		actor->control.current_fire_target_timer++;
	}

	if (actor->control.fire_state != _actor_fire_state_bursting)
	{
		short fire_target_type = _actor_fire_target_none;
		boolean target_changed;

		if (actor->orders.combat.shoot_at_target)
		{
			if (actor->orders.combat.use_manual_target_point)
			{
				fire_target_type = _actor_fire_target_manual_point;
			}
			else if (actor->target.target_prop_index != NONE)
			{
				fire_target_type = _actor_fire_target_prop;
			}
		}

		if (fire_target_type != actor->control.current_fire_target_type)
		{
			target_changed = TRUE;
		}
		else if (fire_target_type == _actor_fire_target_prop)
		{
			target_changed = actor->target.target_prop_index != actor->control.current_fire_target_prop_index;
		}
		else if (fire_target_type == _actor_fire_target_manual_point)
		{
			target_changed = distance_squared3d(
				&actor->orders.combat.target_point,
				&actor->control.current_fire_target_manual_point) > 0.25f;
		}
		else
		{
			target_changed = FALSE;
		}

		if (target_changed)
		{
			actor->control.current_fire_target_timer = 0;
		}

		actor->control.current_fire_target_type = fire_target_type;
		if (fire_target_type == _actor_fire_target_prop)
		{
			actor->control.current_fire_target_prop_index = actor->target.target_prop_index;
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				133,
				actor->control.current_fire_target_prop_index != NONE);
		}
		else if (fire_target_type == _actor_fire_target_manual_point)
		{
			actor->control.current_fire_target_manual_point = actor->orders.combat.target_point;
		}
	}

	actor->control.aiming_at_fire_target = FALSE;
	actor->control.weapon_maximum_range = actor_has_ranged_weapon(actor_index) ?
		firing_variant_definition->ranged_combat.maximum_firing_range :
		0.0f;

	if (actor->orders.combat.throw_grenade)
	{
		if (actor_variant_definition->grenade_combat.grenade_type != NONE &&
			!unit_get_grenade_count(
				actor->meta.unit_index,
				actor_variant_definition->grenade_combat.grenade_type))
		{
			unit_add_grenade_type_to_inventory(
				actor->meta.unit_index,
				actor_variant_definition->grenade_combat.grenade_type,
				1);
		}
		actor_unit_control_throw_grenade(actor_index);
		ai_communication_event(
			_ai_communication_grenade_throwing,
			actor->meta.unit_index,
			NONE,
			NONE,
			NONE,
			NONE,
			NULL);
		fire = FALSE;
	}
	else if (!actor_has_ranged_weapon(actor_index))
	{
		fire = FALSE;
	}
	else if (actor->control.fire_state == _actor_fire_state_wild)
	{
		fire = TRUE;
	}
	else
	{
		boolean linear = TRUE;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_combat.c",
			173,
			weapon_index != NONE);

		if (firing_variant_definition->ranged_combat.special_fire_mode > _actor_special_fire_mode_none &&
			actor->control.fire_state != _actor_fire_state_bursting &&
			actor->control.fire_state != _actor_fire_state_wild &&
			actor->control.special_fire_delay <= 0 &&
			actor->control.special_fire_deny_attempts <= 0)
		{
			struct weapon_definition *special_weapon_definition =
				weapon_definition_get(weapon_get(weapon_index)->definition_index);
			real special_fire_delay_scale = game_difficulty_get_team_value(
				_game_difficulty_value_special_fire_delay,
				actor->meta.team_index);
			boolean special_fire = TRUE;

			switch (firing_variant_definition->ranged_combat.special_fire_mode)
			{
			case _actor_special_fire_mode_overcharge:
			{
				real overcharge_chance = game_difficulty_get_team_value(
					_game_difficulty_value_overcharge_chance,
					actor->meta.team_index);

				if (special_weapon_definition->weapon.triggers.count > 0)
				{
					struct weapon_trigger_definition *trigger = TAG_BLOCK_GET_ELEMENT(
						&special_weapon_definition->weapon.triggers,
						0,
						struct weapon_trigger_definition);
				}
				else
				{
					special_fire = FALSE;
				}
				break;
			}
			case _actor_special_fire_mode_secondary:
				special_fire = special_weapon_definition->weapon.triggers.count > 1;
				break;
			}

			if (special_fire &&
				actor_combat_enable_special_fire_situation(
					actor_index,
					firing_variant_definition->ranged_combat.special_fire_situation))
			{
				real special_fire_delay =
					firing_variant_definition->ranged_combat.special_fire_delay + real_random_range(0.0f, 1.5f);
				real special_fire_random = real_random();

				actor->control.special_fire_delay = (short)(special_fire_delay*TICKS_PER_SECOND);
				if (special_fire_random < firing_variant_definition->ranged_combat.special_fire_chance &&
					actor_combat_allow_special_fire_situation(
						actor_index,
						firing_variant_definition->ranged_combat.special_fire_situation))
				{
					if (firing_variant_definition->ranged_combat.special_fire_situation ==
						_actor_special_fire_situation_strafing)
					{
						actor->control.special_fire_deny_attempts = 3;
					}

					switch (firing_variant_definition->ranged_combat.special_fire_mode)
					{
					case _actor_special_fire_mode_overcharge:
						actor->control.overcharging_weapon = TRUE;
						break;
					case _actor_special_fire_mode_secondary:
						actor->control.next_burst_secondary = TRUE;
						break;
					}
				}
			}
		}

		if (actor->control.current_fire_target_type > _actor_fire_target_none)
		{
			if (actor->control.current_fire_target_type == _actor_fire_target_prop)
			{
				struct prop_datum *prop = prop_get(actor->control.current_fire_target_prop_index);
				unsigned long const *combined_pvs;

				actor->control.current_fire_target_range = prop->distance;
				actor->control.current_fire_target_position = prop->center_of_mass;
				actor->control.current_fire_target_line_of_sight = prop->line_of_sight;
				actor->control.current_fire_target_underwater = prop->underwater;
				combined_pvs = players_get_combined_pvs();
				actor->control.current_fire_target_outside_active_area = TRUE;
				if (prop->body_location.cluster_index != NONE)
				{
					actor->control.current_fire_target_outside_active_area =
						!BIT_VECTOR_TEST_FLAG(combined_pvs, prop->body_location.cluster_index);
				}
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_combat.c",
					296,
					actor->control.current_fire_target_type == _actor_fire_target_manual_point);
				actor->control.current_fire_target_position = actor->control.current_fire_target_manual_point;
				actor->control.current_fire_target_range = distance3d(
					&actor->input.position.head_position,
					&actor->control.current_fire_target_manual_point);
				actor->control.current_fire_target_underwater = FALSE;
				actor->control.current_fire_target_outside_active_area = FALSE;
				if ((actor->control.current_fire_target_timer % 10) == 0)
				{
					actor->control.current_fire_target_line_of_sight = ai_test_line_of_sight(
						&actor->input.position.head_position,
						actor->input.position.body_location.cluster_index,
						&actor->control.current_fire_target_position,
						NONE,
						_ai_line_of_sight_normal,
						FALSE,
						NONE,
						actor->input.vehicle_index != NONE);
				}
			}

			actor->control.current_fire_target_superballistic = FALSE;
			if (firing_variant_definition->ranged_combat.weapon_super_ballistic_range > 0.0f &&
				actor->control.current_fire_target_range >
					firing_variant_definition->ranged_combat.weapon_super_ballistic_range)
			{
				actor->control.current_fire_target_superballistic = TRUE;
			}
			actor->control.current_fire_target_bombardment =
				actor->orders.combat.bombard_target &&
				firing_variant_definition->ranged_combat.weapon_bombardment_range > 0.0f;

			if (!weapon_aim(
				weapon_index,
				0,
				&actor->input.position.head_position,
				&actor->control.current_fire_target_position,
				actor->control.current_fire_target_superballistic,
				&actor->control.current_fire_target_aim_vector,
				NULL,
				&actor->control.current_fire_target_distance,
				&linear))
			{
				actor->control.current_fire_target_type = _actor_fire_target_none;
			}
		}

		if (actor->control.current_fire_target_type == _actor_fire_target_none)
		{
			firing_decision = _firing_no_target;
			fire = FALSE;
		}
		else if (actor->control.current_fire_target_outside_active_area)
		{
			firing_decision = _firing_outside_active_area;
			fire = FALSE;
		}
		else if (!actor->orders.combat.override_firing_restrictions &&
			actor->control.burst_disable_timer > 0)
		{
			firing_decision = _firing_disabled;
			fire = FALSE;
		}
		else if (actor_move_animation_busy(actor_index))
		{
			firing_decision = _firing_busy;
			fire = FALSE;
		}
		else if (!actor->orders.combat.override_firing_restrictions &&
			actor->input.in_midair &&
			!actor->state.flying &&
			!TEST_FLAG(actor_variant_definition->flags, _actor_variant_definition_can_shoot_while_flying_bit))
		{
			firing_decision = _firing_not_in_midair;
			fire = FALSE;
		}
		else if (!actor->orders.combat.override_firing_restrictions &&
			TEST_FLAG(actor_definition->flags, _actor_definition_must_crouch_to_fire_bit) &&
			!actor->control.crouching)
		{
			firing_decision = _firing_not_crouching;
			fire = FALSE;
		}
		else if (!actor->orders.combat.override_firing_restrictions &&
			TEST_FLAG(actor_definition->flags2, _actor_definition_flags2_must_stand_to_fire_bit) &&
			actor->control.crouching)
		{
			firing_decision = _firing_not_standing;
			fire = FALSE;
		}
		else if (!actor->orders.combat.override_firing_restrictions &&
			TEST_FLAG(actor_definition->flags2, _actor_definition_flags2_must_stop_to_fire_bit) &&
			actor->control.moving)
		{
			firing_decision = _firing_not_stationary;
			fire = FALSE;
		}
		else if (actor->control.current_fire_target_underwater || actor->input.underwater)
		{
			firing_decision = _firing_underwater;
			fire = FALSE;
		}
		else if (weapon_definition &&
			weapon_definition->weapon.ai_minimum_target_range > 0.0f &&
			actor->control.current_fire_target_range < weapon_definition->weapon.ai_minimum_target_range)
		{
			firing_decision = _firing_min_range;
			fire = FALSE;
		}
		else if (actor->orders.look.primary_priority == _primary_priority_none ||
			actor->orders.look.primary_direction.type != _direction_specification_target ||
			actor->control.aiming_away_from_primary)
		{
			firing_decision = _firing_wrong_target;
			fire = FALSE;
		}
		else if (actor->control.fire_state == _actor_fire_state_bursting)
		{
			fire = TRUE;
		}
		else
		{
			actor->control.current_fire_target_visible =
				actor->control.current_fire_target_line_of_sight == _ai_line_of_sight_clear ||
				actor->control.current_fire_target_line_of_sight == _ai_line_of_sight_occluded;

			if (!actor->control.current_fire_target_visible &&
				!actor->control.current_fire_target_bombardment)
			{
				firing_decision = _firing_not_visible;
				fire = FALSE;
			}
			else if (!actor->orders.combat.override_firing_restrictions &&
				actor->control.current_fire_target_range >= actor->control.weapon_maximum_range)
			{
				firing_decision = _firing_outside_range;
				fire = FALSE;
			}
			else
			{
				fire = TRUE;
				actor->control.aiming_at_fire_target = TRUE;
				actor_debug_info->burst_alignment.time = game_time_get();
				if (TEST_FLAG(actor_definition->flags, _actor_definition_start_firing_before_aligned_bit))
				{
					actor_debug_info->burst_alignment.aligned = TRUE;
					actor_debug_info->burst_alignment.aligned_immediately = TRUE;
				}
				else
				{
					real alignment_threshold = actor->control.current_fire_target_range < 1.5f ?
						actor->control.current_fire_target_range*((0.97f - 0.70710677f)/1.5f) + 0.70710677f :
						0.97f;
					real_vector3d weapon_vector;
					real alignment;

					actor_get_weapon_vector(actor_index, &weapon_vector);
					alignment = dot_product3d(&weapon_vector, &actor->control.current_fire_target_aim_vector);
					if (alignment < alignment_threshold)
					{
						hold_burst_start = TRUE;
					}
					actor_debug_info->burst_alignment.weapon_vector = weapon_vector;
					actor_debug_info->burst_alignment.aim_vector = actor->control.current_fire_target_aim_vector;
					actor_debug_info->burst_alignment.alignment = alignment;
					actor_debug_info->burst_alignment.threshold = alignment_threshold;
					actor_debug_info->burst_alignment.aligned = alignment >= alignment_threshold;
					actor_debug_info->burst_alignment.aligned_immediately = FALSE;
				}
			}
		}
	}

	if (!fire)
	{
		actor->control.fire_state = _actor_fire_state_none;
	}

	if (actor->control.fire_state != _actor_fire_state_none)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_combat.c",
			517,
			firing_decision == NONE);

		switch (actor->control.fire_state)
		{
		case _actor_fire_state_wild:
			firing_decision = _firing_wild;
			break;
		case _actor_fire_state_holding:
			firing_decision = actor->control.fire_state_timer ? _firing_holding : _firing_holding_for_line;
			break;
		case _actor_fire_state_bursting:
			firing_decision = _firing_burst;
			break;
		case _actor_fire_state_pausing:
			firing_decision = actor->control.fire_state_timer ? _firing_pausing : _firing_pausing_for_line;
			break;
		default:
			match_vassert("c:\\halo\\SOURCE\\ai\\actor_combat.c", 538, FALSE, NULL);
			break;
		}
	}

	{
		short new_state = NONE;

		switch (actor->control.fire_state)
		{
		case _actor_fire_state_none:
			if (fire)
			{
				new_state = _actor_fire_state_holding;
			}
			break;
		case _actor_fire_state_bursting:
			if (actor->control.fire_state_timer == 0)
			{
				new_state = _actor_fire_state_pausing;
			}
			break;
		case _actor_fire_state_holding:
		case _actor_fire_state_pausing:
			if (!hold_burst_start && actor->control.fire_state_timer == 0)
			{
				new_state = _actor_fire_state_bursting;
			}
			break;
		case _actor_fire_state_wild:
			if (actor->control.fire_state_timer == 0)
			{
				new_state = _actor_fire_state_none;
			}
			break;
		default:
			match_vassert("c:\\halo\\SOURCE\\ai\\actor_combat.c", 582, FALSE, NULL);
			break;
		}

		if (new_state != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				588,
				new_state!=actor->control.fire_state);

			switch (new_state)
			{
			case _actor_fire_state_holding:
				if (!actor_start_first_burst_delay(actor_index, firing_variant_definition) &&
					!hold_burst_start)
				{
					new_state = _actor_fire_state_bursting;
					actor_start_burst(actor_index);
				}
				break;
			case _actor_fire_state_bursting:
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_combat.c",
					611,
					!hold_burst_start);
				actor_start_burst(actor_index);
				break;
			case _actor_fire_state_pausing:
				actor_start_pause(actor_index);
				break;
			}

			actor->control.fire_state = new_state;
		}
	}

	{
		boolean fire_primary = FALSE;
		boolean fire_secondary = FALSE;

		actor->control.burst_aim_by_vector = FALSE;
		if (actor->control.fire_state == _actor_fire_state_wild)
		{
			fire_primary = TRUE;
		}
		else if (actor->control.fire_state == _actor_fire_state_bursting)
		{
			long ignore_unit_index = NONE;
			real_point3d origin;
			real_vector3d fire_vector;
			long blocking_prop_index;
			boolean linear;
			boolean aiming_success;

			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				640,
				weapon_index != NONE);

			actor->control.burst_origin = actor->control.burst_initial_position;
			actor_debug_info->burst_tracked_position = actor->control.burst_initial_position;
			actor_debug_info->burst_lead_vector = *global_zero_vector3d;

			if (actor->control.current_fire_target_type == _actor_fire_target_prop)
			{
				struct prop_datum *prop = prop_get(actor->control.current_fire_target_prop_index);
				real target_tracking;
				real target_leading;

				actor_debug_info->burst_last_known_position = prop->center_of_mass;
				ignore_unit_index = prop->attached_to_unit_index;

				target_tracking = firing_variant_definition->ranged_combat.target_track_fraction;
				target_tracking += game_difficulty_get_team_value(
					_game_difficulty_value_target_tracking,
					actor->meta.team_index);
				if (MIN(1.0f, target_tracking) > 0.0f &&
					!actor->control.current_fire_target_bombardment)
				{
					real_vector3d tracking_vector;

					vector_from_points3d(
						&actor->control.burst_initial_position,
						&prop->center_of_mass,
						&tracking_vector);
					point_from_line3d(
						&actor->control.burst_origin,
						&tracking_vector,
						firing_variant_definition->ranged_combat.target_track_fraction,
						&actor->control.burst_origin);
				}
				actor_debug_info->burst_tracked_position = actor->control.burst_origin;

				target_leading = firing_variant_definition->ranged_combat.target_lead_fraction;
				target_leading += game_difficulty_get_team_value(
					_game_difficulty_value_target_leading,
					actor->meta.team_index);
				if (MIN(1.0f, target_leading) > 0.0f)
				{
					real time_to_target = weapon_estimate_time_to_target(
						weapon_index,
						actor->control.fire_burst_secondary != FALSE,
						actor->control.current_fire_target_distance);
					real_vector3d lead_vector;

					lead_vector.i = time_to_target*prop->velocity.i;
					lead_vector.j = time_to_target*prop->velocity.j;
					lead_vector.k = time_to_target*prop->velocity.k;
					actor_debug_info->burst_lead_vector = lead_vector;
					point_from_line3d(
						&actor->control.burst_origin,
						&lead_vector,
						firing_variant_definition->ranged_combat.target_lead_fraction,
						&actor->control.burst_origin);
				}
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_combat.c",
					707,
					actor->control.current_fire_target_type == _actor_fire_target_manual_point);
				actor_debug_info->burst_last_known_position = actor->control.burst_origin;
			}

			{
				real_vector3d *burst_relative_position = &actor->control.burst_relative_position;

				add_vectors3d(
					&actor->control.burst_adjustment,
					burst_relative_position,
					burst_relative_position);
				add_vectors3d(
					burst_relative_position,
					&actor->control.burst_origin,
					&actor->control.burst_target);
			}

			if (actor->input.vehicle_index != NONE)
			{
				unit_get_camera_position(actor->meta.unit_index, &origin);
			}
			else
			{
				real_vector3d *gun_offset = NULL;

				if (actor->control.crouching)
				{
					if (magnitude_squared3d(&firing_variant_definition->ranged_combat.gun_offset_crouch) > _real_epsilon)
					{
						gun_offset = &firing_variant_definition->ranged_combat.gun_offset_crouch;
					}
					else if (magnitude_squared3d(&actor_definition->perception.gun_offset_crouch) > _real_epsilon)
					{
						gun_offset = &actor_definition->perception.gun_offset_crouch;
					}
				}
				else
				{
					if (magnitude_squared3d(&firing_variant_definition->ranged_combat.gun_offset_stand) > _real_epsilon)
					{
						gun_offset = &firing_variant_definition->ranged_combat.gun_offset_stand;
					}
					else if (magnitude_squared3d(&actor_definition->perception.gun_offset_stand) > _real_epsilon)
					{
						gun_offset = &actor_definition->perception.gun_offset_stand;
					}
				}

				if (!gun_offset)
				{
					origin = actor->input.position.head_position;
				}
				else
				{
					real_vector3d facing;

					actor_combat_vector_from_points3d_inline(
						&actor->control.burst_target,
						&actor->input.position.body_position,
						&facing);
					if (normalize2d((real_vector2d *)&facing) > 0.0f)
					{
						facing.k = 0.0f;
					}
					else
					{
						facing = actor->input.facing_vector;
					}
					unit_estimate_position(
						actor->meta.unit_index,
						_unit_estimate_gun_position,
						&actor->input.position.body_position,
						&facing,
						gun_offset,
						&origin);
				}
			}

			aiming_success = weapon_aim(
				weapon_index,
				actor->control.fire_burst_secondary != FALSE,
				&origin,
				&actor->control.burst_target,
				actor->control.current_fire_target_superballistic,
				&actor->control.burst_aim_vector,
				NULL,
				NULL,
				&linear);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				784,
				aiming_success);
			match_assert_valid_real_normal3d(
				"c:\\halo\\SOURCE\\ai\\actor_combat.c",
				785,
				&actor->control.burst_aim_vector);
			actor->control.burst_aim_by_vector = !linear;

			actor_combat_vector_from_points3d_inline(
				&origin,
				&actor->control.burst_target,
				&fire_vector);
			if (ai_test_line_of_fire(
				actor_index,
				ignore_unit_index,
				&origin,
				&fire_vector,
				&blocking_prop_index))
			{
				actor->control.blocked_communication_timer = 0;
				if (actor->control.fire_burst_secondary)
				{
					fire_secondary = TRUE;
				}
				else
				{
					fire_primary = TRUE;
				}
			}
			else
			{
				actor->control.blocked_communication_timer++;
				actor->control.fire_state_timer++;
				firing_decision = _firing_blocked;
				if (actor->control.blocked_communication_timer >= 45 &&
					actor->target.target_type >= _actor_target_acknowledged_enemy)
				{
					long cause_unit_index = NONE;

					if (blocking_prop_index != NONE)
					{
						cause_unit_index = prop_get(blocking_prop_index)->unit_index;
					}
					ai_communication_event(
						_ai_communication_blocked,
						actor->meta.unit_index,
						cause_unit_index,
						_comm_hostility_friend,
						NONE,
						NONE,
						NULL);
					actor->control.blocked_communication_timer = 0;
				}
			}
		}

		{
			boolean primary_trigger = FALSE;
			boolean secondary_trigger = FALSE;
			real primary_trigger_value = 0.0f;

			if (fire_primary)
			{
				real rate_of_fire = firing_variant_definition->ranged_combat.rate_of_fire;

				if (actor->control.overcharging_weapon)
				{
					actor->control.overcharging_weapon = FALSE;
				}
				else if (rate_of_fire == 0.0f)
				{
					primary_trigger = TRUE;
					primary_trigger_value = 1.0f;
				}
				else if (actor->control.trigger_delay_timer == 0)
				{
					struct actor_burst_geometry *burst_geometry;
					struct actor_firing_pattern *firing_pattern;
					short trigger_delay;

					primary_trigger = TRUE;
					primary_trigger_value = 1.0f;
					rate_of_fire *= game_difficulty_get_team_value(
						_game_difficulty_value_rate_of_fire,
						actor->meta.team_index);
					actor_combat_get_firing_parameters(
						actor_index,
						firing_variant_definition,
						&burst_geometry,
						&firing_pattern);
					if (firing_pattern && firing_pattern->rate_of_fire_modifier > 0.0f)
					{
						rate_of_fire *= firing_pattern->rate_of_fire_modifier;
					}
					actor_debug_info->shooting_rof = rate_of_fire;
					trigger_delay = (short)fast_ftol(TICKS_PER_SECOND/rate_of_fire);
					actor->control.trigger_delay_timer = MAX(2, trigger_delay);
				}
			}
			else if (fire_secondary)
			{
				if (actor->control.overcharging_weapon)
				{
					actor->control.overcharging_weapon = FALSE;
				}
				else
				{
					secondary_trigger = TRUE;
				}
			}
			else if (actor->control.overcharging_weapon)
			{
				primary_trigger = TRUE;
				primary_trigger_value = 1.0f;
			}

			actor_unit_control_primary_trigger(actor_index, primary_trigger, primary_trigger_value);
			actor_unit_control_secondary_trigger(actor_index, secondary_trigger);
		}
	}

	actor_debug_info->firing_decision = firing_decision;

	return;
}
