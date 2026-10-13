/*
ACTION_CHARGE.C

symbols in this file:
00001080 0050:
	_action_charge_begin (0000)
000010D0 0110:
	_action_charge_find_target_range (0000)
000011E0 0050:
	_action_charge_update (0000)
00001230 0220:
	_action_charge_control (0000)
00001450 0050:
	_action_charge_is_leaping (0000)
000014A0 0010:
	_square_root (0000)
000014B0 0020:
	_scale_vector2d (0000)
000014D0 0020:
	_magnitude_squared2d (0000)
000014F0 0020:
	_magnitude2d (0000)
00001510 0050:
	_normalize2d (0000)
00001560 0020:
	_dot_product2d (0000)
00001580 0030:
	_point_from_line3d (0000)
000015B0 0030:
	_scale_vector3d (0000)
000015E0 0030:
	_magnitude3d (0000)
00001610 0060:
	_normalize3d (0000)
00001670 0020:
	_dot_product3d (0000)
00001690 0030:
	_subtract_vectors3d (0000)
000016C0 0010:
	_real_random (0000)
000016D0 0050:
	_collision_test_line (0000)
00001720 0cb0:
	_action_charge_perform (0000)
000023D0 0120:
	_action_charge_valid_melee_destination (0000)
000024F0 03b0:
	_action_charge_setup (0000)
00242F60 0004:
	__real@00000000 (0000)
00242F64 0004:
	__real@3f333333 (0000)
00242F68 0004:
	__real@3f800000 (0000)
00242F70 0008:
	__real@3f1a36e2e0000000 (0000)
00242F78 0004:
	__real@40800000 (0000)
00242F7C 0004:
	__real@3f5db3d7 (0000)
00242F80 0004:
	__real@3cbf258c (0000)
00242F84 0004:
	__real@3e99999a (0000)
00242F88 0004:
	__real@3d4ccccd (0000)
00242F8C 0004:
	__real@3fc00000 (0000)
00242F90 0004:
	__real@3f4ccccd (0000)
00242F94 0022:
	??_C@_0CC@DPKBMBNB@c?3?2halo?2SOURCE?2ai?2action_charge?4@ (0000)
00242FB8 0028:
	??_C@_0CI@OJMAHDFE@global_current_collision_user_de@ (0000)
00242FE0 0049:
	??_C@_0EJ@EEKGOMEC@global_current_collision_user_de@ (0000)
0024302C 0030:
	??_C@_0DA@BMPMGKCG@actor?5?$CFs?5melee?5animation?5has?5no?5@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"

#include "actions.h"

#include "actor_definitions.h"
#include "actor_perception.h"
#include "actors.h"
#include "ai.h"
#include "ai_communication.h"
#include "props.h"
#include "game/game.h"
#include "items/projectiles.h"
#include "items/weapon_definitions.h"
#include "math/real_math.h"
#include "objects/objects.h"
#include "physics/collisions.h"
#include "physics/collision_usage.h"
#include "units/units.h"
#include "ai_debug.h"

/* ---------- constants */

enum
{
	_action_charge_idle_look_type = 4,
	_action_charge_primary_priority_exact_facing = 4,
	_action_charge_primary_priority_aiming = 5,
	_action_charge_primary_priority_locked_aiming = 7,
	_actor_special_fire_situation_strafing = 3,
	_action_charge_combat_status_clear_line_of_sight = 5,
	_action_charge_prop_facing_central = 2,
	_action_charge_prop_closing_speed_slow = 1,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

boolean unit_get_melee_range_and_ticks(
	long unit_index,
	boolean secondary,
	short *melee_tick,
	real *attack_time,
	short *frame_count,
	real *damage_time);

static real action_charge_find_target_range(
	long actor_index,
	short goal,
	struct charge_state_data *state_data);

static boolean action_charge_valid_melee_destination(
	long actor_index,
	short goal,
	real_point3d *melee_target_point);

/* ---------- globals */

/* ---------- public code */

void action_charge_begin(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->state.action_data.charge.goal == _charge_goal_vehicle_strafing &&
		actor_combat_get_firing_variant_definition(actor_index)->ranged_combat.special_fire_situation ==
			_actor_special_fire_situation_strafing)
	{
		short charge_remaining = actor->control.special_fire_deny_attempts;

		if (charge_remaining > 0)
		{
			actor->control.special_fire_deny_attempts = charge_remaining - 1;
		}
	}

	return;
}

static real action_charge_find_target_range(
	long actor_index,
	short goal,
	struct charge_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	real range = 0.f;

	if (goal == _charge_goal_melee ||
		goal == _charge_goal_melee_leaping)
	{
		struct actor_definition *definition =
			actor_definition_get(actor->meta.definition_index);

		if (goal == _charge_goal_melee_leaping &&
			!(0.f > definition->berserk.melee_leap_range_upper_bound))
		{
			range = definition->berserk.melee_leap_range_upper_bound;
		}

		if (state_data->melee_suicide)
		{
			if (!(range > definition->berserk.melee_attack_range))
			{
				range = definition->berserk.melee_attack_range;
			}
		}
		else
		{
			real melee_range =
				definition->berserk.melee_attack_range + state_data->melee_danger_range;

			if (!(range > melee_range))
			{
				range = melee_range;
			}
		}
	}
	else if (goal == _charge_goal_vehicle_strafing || goal == _charge_goal_close_range)
	{
		if (actor_has_ranged_weapon(actor_index) &&
			actor->target.target_type >= _actor_target_acknowledged_enemy)
		{
			if (!(0.f > actor->control.weapon_maximum_range))
			{
				range = actor->control.weapon_maximum_range;
			}
		}
	}

	return range;
}

void action_charge_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->state.action_data.charge.goal == _charge_goal_melee_leaping &&
		actor->state.action_data.charge.launched_leap &&
		!actor->state.action_data.charge.launched_melee_attack &&
		!actor->input.in_midair)
	{
		++actor->state.action_data.charge.leap_failure_timer;
	}

	return;
}

void action_charge_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct charge_state_data *state_data = &actor->state.action_data.charge;
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);

	actor->orders.look.primary_direction.type = _direction_specification_target;
	actor->orders.look.idle_look_type = _action_charge_idle_look_type;

	if ((state_data->goal == _charge_goal_melee ||
		state_data->goal == _charge_goal_melee_leaping) &&
		state_data->alignment_incorrect &&
		!actor->control.moving &&
		!actor_path_has_path(actor_index))
	{
		actor->orders.look.primary_priority = _action_charge_primary_priority_exact_facing;
	}
	else
	{
		short priority = _action_charge_primary_priority_aiming;

		if (actor->state.combat_status >= 5 &&
			state_data->goal != _charge_goal_stalking)
		{
			actor->orders.look.primary_priority = _action_charge_primary_priority_locked_aiming;
		}
		else
		{
			actor->orders.look.primary_priority = priority;
		}
	}

	if (state_data->goal == _charge_goal_stalking)
	{
		actor->orders.move.stationary_crouch = !state_data->stalking_catch_target;
		actor->orders.move.moving_crouch = !state_data->stalking_catch_target;
	}
	else if (!actor->orders.move.panicked &&
		TEST_FLAG(definition->flags, _actor_definition_defensive_crouch_while_charging_bit))
	{
		actor->orders.move.stationary_crouch = actor->emotions.defensive_crouch;
		actor->orders.move.moving_crouch = actor->emotions.defensive_crouch;
	}
	else
	{
		actor->orders.move.stationary_crouch = FALSE;
		actor->orders.move.moving_crouch = FALSE;
	}

	if (state_data->leap_pending)
	{
		actor->orders.move.jump = TRUE;
		actor->orders.move.jump_leap = state_data->leap_vertical_velocity < state_data->leap_horizontal_velocity * 0.7f;
		actor->orders.move.jump_targeted = TRUE;
		actor->orders.move.jump_alignment_vector = state_data->leap_alignment_vector;
		actor->orders.move.jump_target_horizontal_vel = state_data->leap_horizontal_velocity;
		actor->orders.move.jump_target_vertical_vel = state_data->leap_vertical_velocity;
		state_data->launched_leap = TRUE;
		state_data->leap_pending = FALSE;
		state_data->leap_start_time = game_time_get();
		state_data->leap_failure_timer = 0;
	}

	if (TEST_FLAG(definition->flags, _actor_definition_berserk_use_panic_movement_bit) &&
		(actor->emotions.berserk ||
		state_data->goal == _charge_goal_melee ||
		state_data->goal == _charge_goal_melee_leaping))
	{
		actor->orders.move.panicked = state_data->advancing && !actor->orders.move.moving_crouch;
	}

	actor->orders.move.move_face_exactly = TRUE;
	actor->orders.move.dive_into_cover = FALSE;
	actor->orders.move.emerge_from_cover = FALSE;
	actor->orders.combat.shoot_at_target = state_data->goal != _charge_goal_stalking;

	return;
}

boolean action_charge_is_leaping(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct charge_state_data *state_data = &actor->state.action_data.charge;
	boolean result = FALSE;

	if (state_data->goal == _charge_goal_melee_leaping &&
		state_data->launched_leap)
	{
		result = state_data->leap_start_time + 30 >= game_time_get();
	}

	return result;
}


boolean action_charge_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct actor_variant_definition *firing_variant_definition =
		actor_combat_get_firing_variant_definition(actor_index);
	struct charge_state_data *state_data = &actor->state.action_data.charge;
	struct prop_datum *prop = NULL;
	boolean result = FALSE;

	if (actor->target.target_prop_index != NONE)
	{
		prop = prop_get(actor->target.target_prop_index);

		if (actor->input.delayed_attached_projectile_index != NONE)
		{
			state_data->advancing = TRUE;
		}
		else if (state_data->goal == _charge_goal_vehicle_ramming ||
			state_data->goal == _charge_goal_vehicle_strafing)
		{
			state_data->advancing = TRUE;
		}
		else if (state_data->goal == _charge_goal_melee ||
			state_data->goal == _charge_goal_melee_leaping)
		{
			real abort_range = REAL_MAX;
			boolean check_range = TRUE;
			boolean berserk_ranges = actor->emotions.berserk;

			if (!actor_has_ranged_weapon(actor_index))
			{
				berserk_ranges = TRUE;
			}

			match_assert(
				"c:\\halo\\SOURCE\\ai\\action_charge.c",
				288,
				!actor->meta.swarm);

			if (state_data->launched_melee_attack ||
				state_data->launched_leap ||
				state_data->leap_pending)
			{
				check_range = FALSE;
			}
			else if (actor->emotions.berserk || !actor_has_ranged_weapon(actor_index))
			{
				abort_range = berserk_ranges ?
					variant_definition->ranged_combat.berserk_melee_abort_range :
					variant_definition->ranged_combat.melee_abort_range;
			}

			if (actor->external_orders.disable_charging)
			{
				real maximum_abort_range = MAX(0.f, definition->berserk.melee_attack_range) + 0.8f;

				abort_range = MIN(abort_range, maximum_abort_range);
			}

			if (check_range && prop->distance > abort_range)
			{
				state_data->aborted_melee_attack = TRUE;
			}
			else
			{
				actor->emotions.last_melee_attack_time = game_time_get();
				state_data->advancing = TRUE;

				if (check_range)
				{
					if (state_data->goal == _charge_goal_melee)
					{
						if (definition->berserk.melee_leap_range_upper_bound == 0.f ||
							definition->berserk.melee_leap_chance == 0.f)
						{
							state_data->leap_possible_if_at_range = FALSE;
						}
						else if (prop->flying || prop->unreachable_ticks > 0)
						{
							state_data->leap_possible_if_at_range = TRUE;
						}

						if (state_data->leap_possible_if_at_range &&
							(prop->unreachable_ticks > 0 ||
							prop->distance > definition->berserk.melee_leap_range_lower_bound * 1.5f))
						{
							state_data->goal = _charge_goal_melee_leaping;
						}
					}
					else if (prop->distance < definition->berserk.melee_leap_range_lower_bound)
					{
						state_data->goal = _charge_goal_melee;
						state_data->leap_possible_if_at_range = TRUE;
					}
				}
			}
		}
		else
		{
			boolean stalking =
				TEST_FLAG(definition->flags, _actor_definition_stalking_behavior_bit) &&
				actor->state.combat_status >= _action_charge_combat_status_clear_line_of_sight &&
				!actor->emotions.berserk;

			state_data->goal = stalking ? _charge_goal_stalking : _charge_goal_close_range;
			if (state_data->goal == _charge_goal_stalking)
			{
				state_data->stalking_currently_exposed =
					(prop->line_of_sight == _ai_line_of_sight_clear || prop->line_of_sight == _ai_line_of_sight_occluded) &&
					prop->quantized_facing <= _action_charge_prop_facing_central;
				if (state_data->stalking_currently_exposed &&
					TEST_FLAG(definition->flags, _actor_definition_stalking_freeze_when_exposed_bit))
				{
					state_data->advancing = FALSE;
				}
				else
				{
					state_data->advancing = TRUE;
				}
				if (state_data->stalking_currently_exposed)
				{
					state_data->stalking_discovery_timer++;
				}

				state_data->stalking_catch_target = FALSE;
				if (!state_data->stalking_currently_exposed && prop->quantized_closing_speed <= _action_charge_prop_closing_speed_slow)
				{
					state_data->stalking_catch_target = TRUE;
				}
				else if (definition->defensive.stalking_max_distance > 0.f &&
					prop->distance >= definition->defensive.stalking_max_distance)
				{
					state_data->stalking_catch_target = TRUE;
				}
			}
			else if (!actor_has_ranged_weapon(actor_index) || actor->input.underwater)
			{
				state_data->advancing = TRUE;
			}
			else
			{
				real minimum_range;
				real maximum_range;
				struct weapon_definition *weapon;

				if (actor->emotions.berserk)
				{
					minimum_range = firing_variant_definition->ranged_combat.berserk_firing_range_lower_bound;
					maximum_range = firing_variant_definition->ranged_combat.berserk_firing_range_upper_bound;
				}
				else
				{
					minimum_range = firing_variant_definition->ranged_combat.combat_range_lower_bound;
					maximum_range = firing_variant_definition->ranged_combat.combat_range_upper_bound;
				}

				weapon = actor_get_weapon_definition(actor_index);
				if (weapon && weapon->weapon.ai_minimum_target_range > 0.f)
				{
					minimum_range = MAX(minimum_range, weapon->weapon.ai_minimum_target_range);
				}

				if (state_data->advancing)
				{
					if (prop->distance < minimum_range)
					{
						state_data->advancing = FALSE;
					}
				}
				else
				{
					if (prop->distance > maximum_range)
					{
						state_data->advancing = TRUE;
					}
				}

				if (prop->distance > 0.7f &&
					prop->line_of_sight != _ai_line_of_sight_clear &&
					prop->line_of_sight != _ai_line_of_sight_occluded)
				{
					state_data->advancing = TRUE;
				}
			}
		}
	}
	else
	{
		state_data->advancing = FALSE;
	}

	if (state_data->launched_melee_attack)
	{
		long unit_index = actor->meta.unit_index;

		state_data->finished_melee_attack = unit_index == NONE || !unit_is_busy(unit_index);
	}
	else if (!state_data->leap_pending &&
		(state_data->goal == _charge_goal_melee || state_data->goal == _charge_goal_melee_leaping) &&
		prop)
	{
		boolean melee = FALSE;
		struct actor_debug_info *debug_info =
			&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
		real_vector3d direction;
		real distance;

		debug_info->last_melee_time = game_time_get();
		point_from_line3d(&actor->input.position.body_position, global_up3d, 0.05f, &debug_info->field_108);
		debug_info->field_114 = actor->input.facing_vector;
		point_from_line3d(&prop->body_position, global_up3d, 0.05f, &debug_info->field_120);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\action_charge.c",
			487,
			!actor->meta.swarm);

		if (prop->distance < 0.8f)
		{
			debug_info->field_139 = TRUE;
			direction = prop->actor_to_prop;
			melee = TRUE;
		}
		else
		{
			struct unit_datum *unit = unit_get(actor->meta.unit_index);
			real lead_fraction;
			real speed;
			real_point3d target_point;

			debug_info->field_139 = FALSE;
			lead_fraction = 0.f;
			speed = magnitude3d(&prop->velocity);
			if (speed > 0.f)
			{
				lead_fraction = (dot_product3d(&prop->velocity, &prop->actor_to_prop) / speed + 1.f) * 0.5f;
			}

			point_from_line3d(
				&prop->body_position,
				&prop->velocity,
				lead_fraction * state_data->melee_ticks_until_dangerous,
				&target_point);
			vector_from_points3d(&actor->input.position.body_position, &target_point, &direction);
			point_from_line3d(&target_point, global_up3d, 0.05f, &debug_info->field_13C);

			if (dot_product3d(&direction, &prop->actor_to_prop) < 0.f)
			{
				distance = 0.f;
				direction = prop->actor_to_prop;
			}
			else
			{
				distance = normalize3d(&direction);
				if (distance == 0.f)
				{
					direction = prop->actor_to_prop;
				}
			}

			if (state_data->goal == _charge_goal_melee_leaping && !state_data->launched_leap)
			{
				debug_info->field_148 = definition->berserk.melee_leap_range_lower_bound;
				debug_info->field_14C = definition->berserk.melee_leap_range_upper_bound;

				if (distance < definition->berserk.melee_leap_range_lower_bound &&
					prop->unreachable_ticks == 0 &&
					!prop->flying)
				{
					state_data->aborted_melee_attack = TRUE;
					actor->emotions.last_melee_attack_time = NONE;
				}
				else
				{
					if (distance < definition->berserk.melee_leap_range_upper_bound)
					{
						real target_velocity_minimum = definition->berserk.melee_leap_velocity * 0.3f;
						real_vector3d aim_vector;
						real horizontal_velocity;
						real vertical_velocity;

						if (projectile_aim_ballistic(
							definition->berserk.melee_leap_velocity,
							1.f,
							&actor->input.position.body_position,
							&prop->body_position,
							&target_velocity_minimum,
							&definition->berserk.melee_leap_ballistic,
							NULL,
							FALSE,
							&aim_vector,
							NULL,
							NULL,
							NULL,
							&vertical_velocity,
							&horizontal_velocity))
						{
							if (normalize2d((real_vector2d *)&aim_vector) == 0.f)
							{
								aim_vector = actor->input.facing_vector;
								if (normalize2d((real_vector2d *)&aim_vector) == 0.f)
								{
									aim_vector = *global_forward3d;
								}
							}

							state_data->leap_pending = TRUE;
							state_data->leap_alignment_vector.i = aim_vector.i;
							state_data->leap_alignment_vector.j = aim_vector.j;
							state_data->leap_horizontal_velocity = horizontal_velocity;
							state_data->leap_vertical_velocity = vertical_velocity;
						}
					}
				}
			}
			else if (state_data->melee_suicide)
			{
				debug_info->field_148 = 0.f;
				debug_info->field_14C = definition->berserk.melee_attack_range;

				if (distance < definition->berserk.melee_attack_range)
				{
					melee = TRUE;
				}
				else if (distance < definition->berserk.suicide_sensing_distance)
				{
					real_vector3d relative_velocity;

					subtract_vectors3d(&prop->velocity, &unit->object.translational_velocity, &relative_velocity);
					if (dot_product3d(&relative_velocity, &direction) > 0.023333333f)
					{
						melee = TRUE;
					}
				}
			}
			else
			{
				if (state_data->goal == _charge_goal_melee_leaping && state_data->launched_leap)
				{
					distance -= dot_product3d(&direction, &unit->object.translational_velocity) *
						state_data->melee_ticks_until_dangerous;
				}

				debug_info->field_148 = state_data->melee_danger_range;
				debug_info->field_14C = state_data->melee_danger_range + definition->berserk.melee_attack_range;
				if (distance < state_data->melee_danger_range + definition->berserk.melee_attack_range)
				{
					melee = TRUE;
				}
			}
		}

		if (state_data->leap_pending || (melee && !state_data->melee_suicide))
		{
			real_vector2d facing_direction;

			facing_direction.i = direction.i;
			facing_direction.j = direction.j;
			if (normalize2d(&facing_direction) > 0.f)
			{
				real minimum_alignment = state_data->launched_leap ? 0.f : 0.8660254f;

				if (dot_product2d(&facing_direction, (real_vector2d *)&actor->input.facing_vector) < minimum_alignment)
				{
					melee = FALSE;
					state_data->leap_pending = FALSE;
					state_data->alignment_incorrect = TRUE;
				}
			}
		}

		debug_info->field_12C = direction;
		debug_info->field_138 = melee;

		if (melee)
		{
			real_vector2d melee_direction;

			melee_direction.i = direction.i;
			melee_direction.j = direction.j;
			if (normalize2d(&melee_direction) == 0.f)
			{
				melee_direction.i = actor->input.facing_vector.i;
				melee_direction.j = actor->input.facing_vector.j;
			}

			if (unit_melee_attack_begin(actor->meta.unit_index, FALSE, &melee_direction))
			{
				ai_communication_event(
					_ai_communication_melee,
					actor->meta.unit_index,
					prop->unit_index,
					_comm_hostility_enemy,
					NONE,
					NONE,
					NULL);
				state_data->launched_melee_attack = TRUE;
			}
		}
	}

	{
		long time = game_time_get();
		real target_range;

		if ((state_data->goal == _charge_goal_melee || state_data->goal == _charge_goal_melee_leaping) &&
			!state_data->launched_melee_attack &&
			!state_data->leap_pending)
		{
			if (state_data->launched_leap)
			{
				if (state_data->leap_failure_timer > 15)
				{
					state_data->aborted_melee_attack = TRUE;
				}
			}
			else if (definition->berserk.melee_attack_charge_timer > 0.f &&
				time >= state_data->charge_start_time + definition->berserk.melee_attack_charge_timer * TICKS_PER_SECOND)
			{
				state_data->aborted_melee_attack = TRUE;
			}
		}

		if (state_data->goal == _charge_goal_vehicle_strafing ||
			state_data->goal == _charge_goal_vehicle_ramming)
		{
			actor->emotions.last_vehicle_charge_time = time;
		}

		target_range = action_charge_find_target_range(actor_index, state_data->goal, state_data);
		state_data->acceptable_target_range = target_range;

		if (!actor->meta.swarm && actor->meta.timeslice)
		{
			boolean moving = FALSE;

			state_data->unable_to_advance = FALSE;
			if (!state_data->launched_melee_attack &&
				!state_data->launched_leap &&
				!state_data->leap_pending &&
				state_data->advancing)
			{
				real minimum_move_range = state_data->goal == _charge_goal_melee_leaping ? 4.f : 1.5f;
				real move_range = MAX(minimum_move_range, target_range);

				if (actor_move_to_prop(actor_index, actor->target.target_prop_index, move_range))
				{
					actor_move_keep_moving_past_destination(actor_index);
					moving = TRUE;
				}
				else
				{
					state_data->unable_to_advance = TRUE;
					state_data->advancing = FALSE;
				}
			}

			if (!moving)
			{
				actor_move_halt(actor_index);
			}

			if (actor->target.target_type >= _actor_target_acknowledged_enemy)
			{
				struct prop_datum *target_prop = prop_get(actor->target.target_prop_index);
				boolean unreachable = FALSE;
				boolean out_of_range = FALSE;

				if (target_prop->distance > state_data->acceptable_target_range)
				{
					out_of_range = TRUE;
				}

				if ((state_data->goal == _charge_goal_melee || state_data->goal == _charge_goal_melee_leaping) &&
					(state_data->launched_leap || state_data->leap_pending || state_data->launched_melee_attack))
				{
					out_of_range = FALSE;
				}

				if (out_of_range)
				{
					if (state_data->unable_to_advance || !actor_path_has_path(actor_index))
					{
						unreachable = TRUE;
					}
					else if (actor->control.path.path.endpoint.target_radius > state_data->acceptable_target_range)
					{
						unreachable = TRUE;
					}
				}

				actor_perception_unreachable(actor_index, actor->target.target_prop_index, unreachable);
			}
		}
	}

	if (state_data->goal == _charge_goal_melee ||
		state_data->goal == _charge_goal_melee_leaping)
	{
		result = state_data->aborted_melee_attack ||
			state_data->finished_melee_attack ||
			state_data->unable_to_advance;
	}
	else if (state_data->goal == _charge_goal_vehicle_strafing ||
		state_data->goal == _charge_goal_vehicle_ramming)
	{
		result = state_data->unable_to_advance;
	}

	return result;
}

static boolean action_charge_valid_melee_destination(
	long actor_index,
	short goal,
	real_point3d *melee_target_point)
{
	struct actor_datum *actor = actor_get(actor_index);
	real_point3d start_point;
	boolean result = TRUE;

	if (actor->control.path.at_destination)
	{
		start_point = actor->input.position.head_position;
	}
	else if (actor->control.path.path.valid)
	{
		unit_estimate_position(
			actor->meta.unit_index,
			1,
			&actor->control.path.path.endpoint.point,
			NULL,
			NULL,
			&start_point);
	}
	else
	{
		result = FALSE;
	}

	if (result)
	{
		struct collision_result collision;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\action_charge.c",
			889,
			global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
		global_current_collision_users[global_current_collision_user_depth++] =
			_collision_user_ai_melee;

		collision_test_line(
			FLAG(_collision_test_front_facing_surfaces_bit) |
				FLAG(_collision_test_back_facing_surfaces_bit) |
				FLAG(_collision_test_ignore_breakable_surfaces_bit) |
				FLAG(_collision_test_structure_bit),
			&start_point,
			melee_target_point,
			NONE,
			&collision);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\action_charge.c",
			897,
			global_current_collision_user_depth > 1);
		--global_current_collision_user_depth;
	}

	return result;
}

boolean action_charge_setup(
	long actor_index,
	short goal,
	struct charge_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = TRUE;
	struct actor_debug_info *debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

	debug_info->charge_last_time = game_time_get();

	match_assert(
		"c:\\halo\\SOURCE\\ai\\action_charge.c",
		47,
		state_data);
	csmemset(state_data, 0, sizeof(*state_data));
	state_data->charge_start_time = game_time_get();

	if (goal == _charge_goal_vehicle_ramming ||
		goal == _charge_goal_vehicle_strafing)
	{
		result = actor->input.vehicle_driver_type > 1;
		debug_info->charge_decision = result ?
			_charge_vehicle_success : _charge_vehicle_not_driver;
	}
	else if (goal == _charge_goal_melee)
	{
		if (actor->meta.swarm)
		{
			result = FALSE;
			debug_info->charge_decision = _charge_melee_swarm_cant;
		}
		else
		{
			struct unit_datum *unit = unit_get(actor->meta.unit_index);

			result = FALSE;
			if (TEST_FLAG(unit->object.damage_flags, _object_melee_attack_inhibited_bit))
			{
				result = FALSE;
				debug_info->charge_decision = _charge_melee_inhibited;
			}
			else if (actor->target.target_prop_index == NONE)
			{
				result = FALSE;
				debug_info->charge_decision = _charge_melee_notarget;
			}
			else
			{
				struct prop_datum *prop = prop_get(actor->target.target_prop_index);
				boolean leap;

				if (definition->berserk.melee_leap_range_upper_bound == 0.f ||
					definition->berserk.melee_leap_chance == 0.f)
				{
					state_data->leap_possible_if_at_range = FALSE;
					leap = FALSE;
				}
				else if (prop->flying || prop->unreachable_ticks > 0)
				{
					state_data->leap_possible_if_at_range = TRUE;
					leap = TRUE;
				}
				else
				{
					state_data->leap_possible_if_at_range = real_random() < definition->berserk.melee_leap_chance;
					leap = state_data->leap_possible_if_at_range;
					if (prop->distance < definition->berserk.melee_leap_range_lower_bound)
					{
						leap = FALSE;
					}
				}

				if (leap)
				{
					goal = _charge_goal_melee_leaping;
				}
				debug_info->field_198 = leap;

				{
					short start_tick;
					real start_range;
					short end_tick;
					real end_range;

					if (unit_get_melee_range_and_ticks(
						actor->meta.unit_index,
						leap,
						&start_tick,
						&start_range,
						&end_tick,
						&end_range))
					{
						boolean valid = FALSE;
						real target_range;
						real minimum_move_range;
						real move_range;

						if (TEST_FLAG(definition->flags, _actor_definition_suicidal_melee_attack_bit))
						{
							state_data->melee_ticks_until_dangerous = end_tick;
							state_data->melee_danger_range = 0.f;
							state_data->melee_suicide = TRUE;
						}
						else
						{
							if (start_tick == 0)
							{
								char const *name = actor_variant_definition_get(
									actor->meta.variant_definition_index)->actor_reference.name;

								if (name)
								{
									error(_error_silent, "actor %s melee animation has no damage keyframe", name);
								}
								start_range = end_range * 0.5f;
								start_tick = end_tick / 2;
							}
							state_data->melee_ticks_until_dangerous = start_tick;
							state_data->melee_danger_range = end_range - start_range;
						}

						target_range = action_charge_find_target_range(actor_index, goal, state_data);
						state_data->acceptable_target_range = target_range;
						minimum_move_range = goal == _charge_goal_melee_leaping ? 4.f : 1.5f;
						move_range = MAX(minimum_move_range, target_range);

						if (actor_move_to_prop(actor_index, actor->target.target_prop_index, move_range))
						{
							actor_move_keep_moving_past_destination(actor_index);
							valid = action_charge_valid_melee_destination(actor_index, goal, &prop->center_of_mass);
						}

						if (valid)
						{
							result = TRUE;
							debug_info->charge_decision = _charge_melee_success;
						}
						else
						{
							debug_info->charge_decision = _charge_melee_cannot_move;
							debug_info->field_194 = move_range;
						}
					}
					else
					{
						debug_info->charge_decision = _charge_melee_no_animation;
					}
				}
			}
		}
	}
	else if (goal == _charge_goal_close_range &&
		TEST_FLAG(definition->flags, _actor_definition_stalking_behavior_bit) &&
		actor->state.combat_status >= _action_charge_combat_status_clear_line_of_sight &&
		!actor->emotions.berserk)
	{
		goal = _charge_goal_stalking;
		debug_info->charge_decision = _charge_stalking_success;
	}
	else
	{
		debug_info->charge_decision = _charge_close_success;
	}

	state_data->goal = goal;

	return result;
}

/* ---------- private code */
