/*
ACTOR_TYPE_INFECTION.C

symbols in this file:
00027280 0130:
	_infection_decide_action (0000)
000273B0 0060:
	_code_000273b0 (0000)
00027410 0060:
	_code_00027410 (0000)
00027470 0e20:
	_code_00027470 (0000)
00028290 02b0:
	_infection_swarm_aim_jump (0000)
0024644C 000a:
	??_C@_09BGMFGBDN@infection?$AA@ (0000)
00246458 0004:
	__real@bfc90fdb (0000)
0024645C 0004:
	__real@3fc90fdb (0000)
00246460 0004:
	__real@bf666666 (0000)
00246464 003d:
	??_C@_0DN@KOBFBIIN@TEST_FLAG?$CIswarm_component?9?$DOflags@ (0000)
002464A8 004a:
	??_C@_0EK@OLPJENC@swarm_component?9?$DOwander?4move_tic@ (0000)
002464F4 0029:
	??_C@_0CJ@BNPOEG@c?3?2halo?2SOURCE?2ai?2actor_type_inf@ (0000)
00246520 0004:
	__real@40490fdb (0000)
00246524 0004:
	__real@bd888889 (0000)
00246528 0004:
	__real@3d99999a (0000)
0024652C 0004:
	__real@3df5c28f (0000)
002B6BC4 0020:
	_actor_type_infection (0000)
*/

/* ---------- headers */

#define real_random_range real_random_range_inline
#include "cseries.h"
#include "actions.h"
#include "math/real_math.h"
#undef real_random_range

#include "actor_definitions.h"
#include "actors.h"
#include "ai_scenario_definitions.h"
#include "actor_types.h"
#include "props.h"

#include "game/game.h"
#include "items/projectiles.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/unit_definitions.h"

/* ---------- constants */

enum
{
	_actor_combat_status_definite = 3,
};

enum
{
	_actor_panic_none = 0,
};

enum
{
	_swarm_movement_none = 0,
	_swarm_movement_wander_noncombat,
	_swarm_movement_wander_idle,
	_swarm_movement_wander_combat,
	_swarm_movement_towards_prop,
	_swarm_movement_away_from_prop,
	_swarm_movement_obey,
};

enum
{
	_unit_melee_attack_none = 0,
	_unit_melee_attack_starting,
	_unit_melee_attack_dangerous,
	_unit_melee_attack_impact,
	_unit_melee_attack_continuous,
};

/* ---------- macros */

/* ---------- structures */

struct unit_control_data
{
	char animation_state;
	char aiming_speed;
	word control_flags;
	short weapon_index;
	short grenade_index;
	short zoom_level;
	short pad;
	real_vector3d throttle;
	real primary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
};

union projectile_aim_direction
{
	real_vector3d vector;
	struct
	{
		real_vector2d horizontal;
		real vertical;
	};
};

typedef char swarm_component_datum_size_check[
	sizeof(struct swarm_component_datum) == 0x40 ? 1 : -1];
typedef char swarm_wander_control_size_check[
	sizeof(struct swarm_wander_control) == 0x14 ? 1 : -1];
typedef char swarm_component_target_prop_offset_check[
	offsetof(struct swarm_component_datum, combat_target_prop_index) == 0x14 ? 1 : -1];
typedef char swarm_component_wander_offset_check[
	offsetof(struct swarm_component_datum, wander) == 0x1C ? 1 : -1];
typedef char swarm_component_obey_vector_offset_check[
	offsetof(struct swarm_component_datum, obey.directmovement.vector) == 0x28 ? 1 : -1];
typedef char unit_control_data_size_check[
	sizeof(struct unit_control_data) == 0x40 ? 1 : -1];
typedef char unit_control_data_throttle_offset_check[
	offsetof(struct unit_control_data, throttle) == 0x0C ? 1 : -1];
typedef char unit_control_data_facing_offset_check[
	offsetof(struct unit_control_data, facing_vector) == 0x1C ? 1 : -1];
typedef char projectile_aim_direction_size_check[
	sizeof(union projectile_aim_direction) == sizeof(real_vector3d) ? 1 : -1];

/* ---------- prototypes */

static short infection_wander_pause_time(
	short movement_type);
static short infection_wander_move_time(
	short movement_type);
real real_random_range(
	real lower_bound,
	real upper_bound);

void infection_decide_action(
	long actor_index);
void infection_swarm_control(
	long actor_index);
void infection_swarm_aim_jump(
	long actor_index,
	long unit_index,
	real jump_magnitude,
	real_vector3d *jump_velocity);

void unit_detach_from_parent(
	long unit_index);

/* ---------- globals */

struct actor_type_definition actor_type_infection =
{
	"infection",
	32,
	2,
	2,
	2,
	0,
	TRUE,
	{ 0, 0 },
	NULL,
	infection_decide_action,
	infection_swarm_control,
	infection_swarm_aim_jump
};

/* ---------- public code */

/* ---------- private code */

void infection_decide_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor_definition_get(actor->meta.definition_index);
	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	if (!actor_action_deny_transition(actor_index))
		actor_action_handle_combat_transition(actor_index);

	switch (actor->state.action)
	{
	case _actor_action_fight:
	case _actor_action_charge:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
		{
			actor_action_handle_combat_failure(actor_index);
			return;
		}
		break;

	case _actor_action_flee:
		if (actor->state.action_data.flee.unable_to_flee)
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		else
			actor_action_handle_done_fleeing(actor_index);
		return;

	case _actor_action_guard:
		actor_action_handle_combat_status(
			actor_index,
			actor_action_can_stop_guarding(actor_index, _actor_action_fight, _actor_action_guard),
			FALSE);
		return;

	case _actor_action_uncover:
	case _actor_action_search:
	case _actor_action_wait:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
			actor_action_handle_exit_pursuit(actor_index);
		return;

	case _actor_action_obey:
		actor_action_handle_combat_status(
			actor_index,
			actor->state.action_data.obey.initiative,
			actor->state.action_data.obey.finished);
		break;
	}

	return;
}

static short infection_wander_pause_time(
	short movement_type)
{
	real scale = 1.f;
	short result;

	switch (movement_type)
	{
	case 1:
		scale = real_seed_random_range(get_global_random_seed_address(), 4.f, 5.f);
		break;

	case 2:
		scale = real_seed_random_range(get_global_random_seed_address(), 2.f, 2.8f);
		break;

	case 3:
		scale = real_seed_random_range(get_global_random_seed_address(), 0.4f, 1.f);
		break;
	}

	result = (short)(scale * 30.f);
	return MIN(result, 255);
}

static short infection_wander_move_time(
	short movement_type)
{
	real scale = 1.f;
	short result;

	switch (movement_type)
	{
	case 1:
		scale = real_seed_random_range(get_global_random_seed_address(), 1.f, 2.5f);
		break;

	case 2:
	case 3:
		scale = real_seed_random_range(get_global_random_seed_address(), 0.6f, 1.8f);
		break;
	}

	result = (short)(scale * 30.f);
	return MIN(result, 255);
}

void infection_swarm_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant = actor_variant_definition_get(
		actor->meta.variant_definition_index);
	struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);
	short intermittent_action_member = NONE;
	short member_index;

	if (swarm->swarm_intermittent_action_timer > 0)
	{
		swarm->swarm_intermittent_action_timer--;
	}
	else
	{
		switch (actor->state.action)
		{
		case _actor_action_search:
		case _actor_action_charge:
			{
				real cooldown = real_seed_random_range(
					get_global_random_seed_address(),
					6.f,
					8.f) / swarm->unit_count * TICKS_PER_SECOND;

				swarm->swarm_intermittent_action_timer = (short)MAX(cooldown, 6.f);
				intermittent_action_member = seed_random_range(
					get_global_random_seed_address(),
					0,
					swarm->unit_count);
			}
			break;
		}
	}

	for (member_index = 0; member_index < swarm->unit_count; member_index++)
	{
		long unit_index = swarm->unit_indices[member_index];
		struct unit_datum *unit = unit_get(unit_index);
		struct swarm_component_datum *swarm_component = swarm_component_get(
			swarm->component_indices[member_index]);
		struct prop_datum *target_prop;
		short movement_type = _swarm_movement_none;
		long movement_target_prop_index = NONE;
		short animation_state = _unit_animation_state_in_combat;
		short aiming_speed = _unit_aiming_speed_casual;
		boolean facing_target = FALSE;
		boolean target_in_melee_range = FALSE;
		boolean has_direction = FALSE;
		boolean should_jump = FALSE;
		boolean biped_airborne = FALSE;
		real_vector3d up;
		real_vector3d direction;

		up = unit->object.up;
		if (unit->object.type == _object_type_biped)
		{
			struct biped_datum *biped = (struct biped_datum *)unit;

			if (biped->biped.support_surface_index != NONE)
				up = biped->biped.ground_plane.n;
			biped_airborne = TEST_FLAG(biped->biped.flags, _biped_airborne_bit);
		}

		if (actor->state.combat_status >= _actor_combat_status_definite)
		{
			struct prop_iterator iterator;
			real combat_range = variant->ranged_combat.combat_range_upper_bound;
			struct prop_datum *best_prop = NULL;
			long best_prop_index = NONE;
			real best_score = 0.f;
			real best_distance = 0.f;
			struct prop_datum *prop;

			prop_iterator_new(&iterator, actor_index);
			while ((prop = prop_iterator_next(&iterator)) != NULL)
			{
				if (prop->target_weight > 0.f)
				{
					real_vector3d delta;
					real distance;
					real score;

					vector_from_points3d(
						&prop->body_position,
						&swarm_component->position,
						&delta);
					distance = square_root(magnitude_squared3d(&delta));
					score = 0.f;
					if (distance < combat_range)
						score = (1.f - distance / combat_range) * 10.f;

					if (prop->state >= _prop_state_becoming_unacknowledged &&
						prop->state <= _prop_state_acknowledged)
					{
						if (iterator.index == swarm_component->combat_target_prop_index)
							score += 7.f;
						else
							score += 5.f;

						if (!prop->child_units_attached)
							score += 5.f;
					}

					if (score > best_score)
					{
						best_score = score;
						best_prop = prop;
						best_distance = distance;
						best_prop_index = iterator.index;
					}
				}
			}

			swarm_component->combat_target_prop_index = best_prop_index;
			if (best_prop_index != NONE &&
				best_distance < variant->ranged_combat.melee_range &&
				best_prop->state >= _prop_state_becoming_unacknowledged &&
				best_prop->state <= _prop_state_acknowledged)
			{
				target_in_melee_range = TRUE;
			}
			target_prop = best_prop;
		}
		else
		{
			swarm_component->combat_target_prop_index = NONE;
			target_prop = NULL;
		}

		switch (actor->state.action)
		{
		case _actor_action_sleep:
			movement_type = _swarm_movement_none;
			animation_state = _unit_animation_state_asleep;
			break;

		case _actor_action_alert:
			movement_type = _swarm_movement_wander_noncombat;
			animation_state = _unit_animation_state_alert;
			break;

		case _actor_action_guard:
			movement_type = _swarm_movement_wander_idle;
			animation_state = _unit_animation_state_alert;
			break;

		case _actor_action_search:
			if (actor->state.action_data.flee.flee_firing_position_index == 0 &&
				actor->target.target_prop_index != NONE)
			{
				movement_type = _swarm_movement_towards_prop;
				movement_target_prop_index = actor->target.target_prop_index;
			}
			else
			{
				movement_type = _swarm_movement_wander_combat;
			}
			animation_state = _unit_animation_state_in_combat;
			break;

		case _actor_action_flee:
			animation_state = actor->state.action_data.flee.panic_type > _actor_panic_none
				? _unit_animation_state_flee
				: _unit_animation_state_in_combat;
			if (actor->state.action_data.flee.flee_prop_index != NONE)
			{
				movement_type = _swarm_movement_away_from_prop;
				movement_target_prop_index = actor->state.action_data.flee.flee_prop_index;
			}
			break;

		case _actor_action_charge:
		case _actor_action_obey:
			animation_state = _unit_animation_state_in_combat;
			if (actor->state.action == _actor_action_obey &&
				TEST_FLAG(swarm_component->flags, _swarm_component_obey_bit))
			{
				movement_type = _swarm_movement_obey;
			}
			else if (swarm_component->combat_target_prop_index != NONE)
			{
				movement_target_prop_index = swarm_component->combat_target_prop_index;
				aiming_speed = _unit_aiming_speed_alert;
				movement_type = swarm_component->attack_delay_ticks > 0
					? _swarm_movement_away_from_prop
					: _swarm_movement_towards_prop;
			}
			else
			{
				movement_type = _swarm_movement_wander_combat;
			}
			break;
		}

		if (unit->object.parent_object_index == NONE)
		{
			swarm_component->attached_to_unit_ticks = 0;
			if (swarm_component->attack_delay_ticks > 0)
				swarm_component->attack_delay_ticks--;
		}
		else
		{
			struct unit_datum *parent_unit = unit_get(unit->object.parent_object_index);
			boolean parent_is_dead = TEST_FLAG(parent_unit->object.damage_flags, _object_dead_bit);
			boolean detach = FALSE;

			if (swarm_component->attached_to_unit_ticks < 255)
				swarm_component->attached_to_unit_ticks++;

			if (parent_is_dead)
			{
				if (parent_unit->unit.time_of_death != NONE &&
					parent_unit->unit.time_of_death + 75 < game_time_get() &&
					target_prop &&
					target_prop->unit_index != unit->object.parent_object_index &&
					target_prop->state >= _prop_state_becoming_unacknowledged &&
					target_prop->state <= _prop_state_acknowledged)
				{
					detach = TRUE;
				}
			}
			else
			{
				struct unit_definition *parent_definition = unit_definition_get(
					parent_unit->definition_index);

				if ((parent_unit->object.type != _object_type_biped ||
					TEST_FLAG(
						parent_definition->unit.flags,
						_unit_melee_attackers_cannot_attach_bit)) &&
					swarm_component->attached_to_unit_ticks > 45)
				{
					swarm_component->attack_delay_ticks = 45;
					detach = TRUE;
				}
			}

			if (detach)
			{
				unit_detach_from_parent(unit_index);
				swarm_component->flags &= ~(FLAG(_swarm_component_attacking_in_melee_bit) |
					FLAG(_swarm_component_attached_to_unit_bit));
			}
			else
			{
				SET_FLAG(swarm_component->flags, _swarm_component_attached_to_unit_bit, TRUE);
				SET_FLAG(
					swarm_component->flags,
					_swarm_component_attacking_in_melee_bit,
					!parent_is_dead);
			}
		}

		if (unit->object.parent_object_index == NONE)
		{
			if (biped_airborne)
			{
				SET_FLAG(swarm_component->flags, _swarm_component_attached_to_unit_bit, FALSE);
				swarm_component->ground_ticks = 0;
			}
			else
			{
				if (swarm_component->ground_ticks < 255)
					swarm_component->ground_ticks++;

				swarm_component->flags &= ~(FLAG(_swarm_component_attacking_in_melee_bit) |
					FLAG(_swarm_component_attached_to_unit_bit));

				switch (movement_type)
				{
				case _swarm_movement_wander_noncombat:
				case _swarm_movement_wander_idle:
				case _swarm_movement_wander_combat:
					if (!TEST_FLAG(swarm_component->flags, _swarm_component_wander_bit))
					{
						csmemset(&swarm_component->wander, 0, sizeof(swarm_component->wander));
						SET_FLAG(swarm_component->flags, _swarm_component_obey_bit, FALSE);
						SET_FLAG(swarm_component->flags, _swarm_component_wander_bit, TRUE);
					}

					if (swarm_component->wander.move_ticks > 0)
					{
						swarm_component->wander.move_ticks--;
						if (swarm_component->wander.move_ticks == 0)
						{
							swarm_component->wander.pause_ticks = (byte)infection_wander_pause_time(movement_type);
						}
						else
						{
							real angle_damping = swarm_component->wander.angle * -0.06666667f;
							real angle = real_random_range(-0.020943951f, 0.020943951f) +
								swarm_component->wander.angle +
								angle_damping;

							swarm_component->wander.angle = angle;
							rotate_vector_about_axis(
								&swarm_component->wander.vector,
								&up,
								sine(angle),
								cosine(angle));
						}
					}
					else
					{
						if (swarm_component->wander.pause_ticks > 0)
							swarm_component->wander.pause_ticks--;

						if (swarm_component->wander.pause_ticks == 0)
						{
							real_vector3d to_center;
							real distance_squared;
							real angle;

							swarm_component->wander.move_ticks = (byte)infection_wander_move_time(movement_type);
							vector_from_points3d(
								&swarm_component->position,
								&swarm->swarm_center,
								&to_center);
							distance_squared = magnitude_squared3d(&to_center);
							if (distance_squared < 0.25f)
							{
								angle = real_random_range(-_pi, _pi);
								swarm_component->wander.vector = unit->object.forward;
								rotate_vector_about_axis(
									&swarm_component->wander.vector,
									&up,
									sine(angle),
									cosine(angle));
							}
							else
							{
								real angle_range = 0.5f / square_root(distance_squared) * _pi;

								angle = real_random_range(-angle_range, angle_range);
								swarm_component->wander.vector = to_center;
								rotate_vector_about_axis(
									&swarm_component->wander.vector,
									&up,
									sine(angle),
									cosine(angle));
							}
							swarm_component->wander.angle = 0.f;
						}
					}

					match_assert(
						"c:\\halo\\SOURCE\\ai\\actor_type_infection.c",
						0x1ED,
						swarm_component->wander.move_ticks || swarm_component->wander.pause_ticks);
					if (swarm_component->wander.move_ticks)
					{
						has_direction = TRUE;
						direction = swarm_component->wander.vector;
					}
					break;

				case _swarm_movement_towards_prop:
				case _swarm_movement_away_from_prop:
					{
						struct prop_datum *movement_target = prop_get(movement_target_prop_index);

						vector_from_points3d(
							&swarm_component->position,
							&movement_target->body_position,
							&direction);
						if (movement_type == _swarm_movement_away_from_prop)
						{
							negate_vector3d(&direction, &direction);
						}
						has_direction = TRUE;
					}
					break;

				case _swarm_movement_obey:
					match_assert(
						"c:\\halo\\SOURCE\\ai\\actor_type_infection.c",
						0x204,
						TEST_FLAG(swarm_component->flags, _swarm_component_obey_bit));
					if (TEST_FLAG(
						swarm_component->obey.simple_control_flags,
						_obey_simple_directmovement_bit))
					{
						has_direction = TRUE;
						switch (swarm_component->obey.directmovement.facing)
						{
						case _ai_atom_move_facing_left:
						case _ai_atom_move_facing_right:
							cross_product3d(&up, &swarm_component->obey.directmovement.vector, &direction);
							if (swarm_component->obey.directmovement.facing == _ai_atom_move_facing_right)
							{
								negate_vector3d(&direction, &direction);
							}
							break;

						default:
							direction = swarm_component->obey.directmovement.vector;
							if (swarm_component->obey.directmovement.facing == _ai_atom_move_facing_backwards)
							{
								negate_vector3d(&direction, &direction);
							}
							break;
						}
					}

					if (TEST_FLAG(
						swarm_component->obey.simple_control_flags,
						_obey_simple_jump_bit))
					{
						if (!TEST_FLAG(
							swarm_component->obey.simple_control_flags,
							_obey_simple_jump_jumped_bit) &&
							swarm_component->obey.directmovement.facing == _ai_atom_move_facing_forwards &&
							!unit_is_busy(unit_index))
						{
							SET_FLAG(
								swarm_component->flags,
								_swarm_component_obey_desire_jump_bit,
								TRUE);
							SET_FLAG(
								swarm_component->obey.simple_control_flags,
								_obey_simple_jump_jumped_bit,
								TRUE);
						}
						has_direction = TRUE;
						direction = unit->object.forward;
					}
					break;
				}
			}
		}

		if (has_direction)
		{
			real facing_dot;

			normalize3d(&direction);
			facing_dot = dot_product3d(&up, &direction);
			if (facing_dot > 0.9f)
				facing_target = TRUE;

			if (facing_dot < -0.9f)
			{
				direction = unit->object.forward;
			}
			else
			{
				real_vector3d cross;

				cross_product3d(&up, &direction, &cross);
				cross_product3d(&cross, &up, &direction);
				if (normalize3d(&direction) == 0.f)
					direction = unit->object.forward;
			}

			if (movement_type != _swarm_movement_obey)
			{
				real_point3d probe;
				real_vector3d right;
				real separation = 0.f;
				short neighbor_index;

				point_from_line3d(&swarm_component->position, &direction, -0.2f, &probe);
				cross_product3d(&up, &direction, &right);

				for (neighbor_index = 0;
					neighbor_index < swarm->unit_count;
					neighbor_index++)
				{
					if (neighbor_index != member_index)
					{
						struct swarm_component_datum *neighbor = swarm_component_get(
							swarm->component_indices[neighbor_index]);
						real_vector3d delta;
						real distance_squared;

						vector_from_points3d(&probe, &neighbor->position, &delta);
						distance_squared = magnitude_squared3d(&delta);
						if (distance_squared < 0.64000005f)
						{
							real distance = square_root(distance_squared);
							real forward_dot = dot_product3d(&delta, &direction) / distance;

							if (forward_dot > 0.5f)
							{
								if (dot_product3d(&delta, &right) > 0.f)
									separation -= (forward_dot - 0.5f) * 0.5f;
								else
									separation += (forward_dot - 0.5f) * 0.5f;
							}
						}
					}
				}

				if (separation != 0.f)
				{
					real angle;

					if (separation > 1.f)
						angle = _pi / 2.f;
					else if (separation < -1.f)
						angle = -_pi / 2.f;
					else
						angle = separation * (_pi / 2.f);

					rotate_vector_about_axis(
						&direction,
						&up,
						sine(angle),
						cosine(angle));
				}
			}

			{
				real_vector3d right;

				cross_product3d(&up, &direction, &right);
				if (normalize3d(&right) == 0.f)
					direction = unit->object.forward;
				else
					cross_product3d(&right, &up, &direction);
			}
		}

		if (TEST_FLAG(swarm_component->flags, _swarm_component_obey_desire_jump_bit))
		{
			should_jump = TRUE;
		}
		else if (has_direction && swarm_component->ground_ticks >= 45)
		{
			if (member_index == intermittent_action_member)
			{
				should_jump = TRUE;
			}
			else if (target_in_melee_range)
			{
				should_jump = TRUE;
			}
			else if (facing_target)
			{
				should_jump = TRUE;
			}
		}

		if (TEST_FLAG(swarm_component->flags, _swarm_component_attached_to_unit_bit))
		{
			if (TEST_FLAG(swarm_component->flags, _swarm_component_attacking_in_melee_bit))
			{
				unit->unit.melee_attack_state = _unit_melee_attack_continuous;
			}
			else
			{
				unit->unit.melee_attack_state = _unit_melee_attack_none;
			}
		}
		else
		{
			SET_FLAG(
				swarm_component->flags,
				_swarm_component_attacking_in_melee_bit,
				target_in_melee_range && !swarm_component->attack_delay_ticks);
			if (TEST_FLAG(swarm_component->flags, _swarm_component_attacking_in_melee_bit))
			{
				struct biped_datum *biped = (struct biped_datum *)unit;

				biped->unit.melee_attack_state = _unit_melee_attack_impact;
				biped->biped.impact_target_object_index = target_prop == NULL
					? NONE
					: target_prop->unit_index;
			}
			else
			{
				unit->unit.melee_attack_state = _unit_melee_attack_none;
			}
		}

		{
			struct unit_control_data control;
			unsigned long control_flags = 0;

			SET_FLAG(control_flags, _unit_control_jump_bit, should_jump);
			csmemset(&control, 0, sizeof(control));
			control.weapon_index = NONE;
			control.grenade_index = NONE;
			control.zoom_level = NONE;
			control.control_flags = control_flags;
			control.primary_trigger = 0.f;
			control.animation_state = animation_state;
			control.aiming_speed = aiming_speed;
			if (has_direction)
			{
				set_real_vector3d(&control.throttle, 1.f, 0.f, 0.f);
				control.facing_vector = direction;
			}
			else
			{
				control.throttle = *global_zero_vector3d;
				control.facing_vector = unit->object.forward;
			}
			control.aiming_vector = control.facing_vector;
			control.looking_vector = control.facing_vector;
			unit_control(unit_index, &control);
		}
	}

	return;
}

void infection_swarm_aim_jump(
	long actor_index,
	long unit_index,
	real jump_magnitude,
	real_vector3d *jump_velocity)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct swarm_datum *swarm;
	short member_index;

	if (actor->meta.swarm_cache_index == NONE)
	{
		return;
	}

	swarm = swarm_get(actor->meta.swarm_cache_index);
	for (member_index = 0; member_index < swarm->unit_count; member_index++)
	{
		if (swarm->unit_indices[member_index] == unit_index)
		{
			struct unit_datum *unit = unit_get(unit_index);
			struct swarm_component_datum *swarm_component = swarm_component_get(
				swarm->component_indices[member_index]);
			word component_flags = swarm_component->flags;

			if (TEST_FLAG(component_flags, _swarm_component_attacking_in_melee_bit) &&
				swarm_component->combat_target_prop_index != NONE)
			{
				struct prop_datum *prop = prop_get(
					swarm_component->combat_target_prop_index);
				real target_velocity_min = 0.06f;
				real target_ballistic_fraction_min = 0.8f;
				union projectile_aim_direction aim_direction;
				real vertical_velocity;
				real horizontal_velocity;

				jump_magnitude = MAX(jump_magnitude, 0.12f);

				if (projectile_aim_ballistic(
					jump_magnitude,
					1.f,
					&swarm_component->position,
					&prop->center_of_mass,
					&target_velocity_min,
					&target_ballistic_fraction_min,
					NULL,
					FALSE,
					&aim_direction.vector,
					NULL,
					NULL,
					NULL,
					&vertical_velocity,
					&horizontal_velocity))
				{
					real magnitude_squared;

					if (normalize2d(&aim_direction.horizontal) == 0.f)
					{
						aim_direction.vector = actor->input.facing_vector;
						if (normalize2d(&aim_direction.horizontal) == 0.f)
						{
							aim_direction.vector = *global_forward3d;
						}
					}

					if (!prop->flying && vertical_velocity > 0.075f)
					{
						vertical_velocity = 0.075f;
					}

					set_real_vector3d(
						jump_velocity,
						aim_direction.horizontal.i * horizontal_velocity,
						aim_direction.horizontal.j * horizontal_velocity,
						vertical_velocity);
					magnitude_squared =
						jump_velocity->i * jump_velocity->i +
						jump_velocity->j * jump_velocity->j +
						jump_velocity->k * jump_velocity->k;
					if (magnitude_squared > jump_magnitude * jump_magnitude)
					{
						real scale = jump_magnitude / square_root(magnitude_squared);

						scale_vector3d(jump_velocity, scale, jump_velocity);
					}
				}
			}
			else if (TEST_FLAG(component_flags, _swarm_component_obey_bit) &&
				TEST_FLAG(component_flags, _swarm_component_obey_desire_jump_bit))
			{
				if (TEST_FLAG(
						swarm_component->obey.simple_control_flags,
						_obey_simple_jump_bit) &&
					TEST_FLAG(
						swarm_component->obey.simple_control_flags,
						_obey_simple_jump_targeted_bit))
				{
					real_vector2d flat_direction;

					flat_direction.i = unit->object.forward.i;
					flat_direction.j = unit->object.forward.j;
					if (normalize2d(&flat_direction) == 0.f)
					{
						flat_direction.i = unit->object.up.i;
						flat_direction.j = unit->object.up.j;
						if (normalize2d(&flat_direction) == 0.f)
						{
							flat_direction = *global_forward2d;
						}
					}

					set_real_vector3d(
						jump_velocity,
						swarm_component->obey.jump.target_horizontal_vel * flat_direction.i,
						swarm_component->obey.jump.target_horizontal_vel * flat_direction.j,
						swarm_component->obey.jump.target_vertical_vel);
				}

				SET_FLAG(
					swarm_component->flags,
					_swarm_component_obey_desire_jump_bit,
					FALSE);
			}
		}
	}

	return;
}
