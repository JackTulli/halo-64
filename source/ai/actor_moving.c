/*
ACTOR_MOVING.C

symbols in this file:
00018940 0030:
	_actor_move_keep_moving_past_destination (0000)
00018970 0040:
	_actor_move_animation_busy (0000)
000189B0 0030:
	_actor_path_clear (0000)
000189E0 0020:
	_actor_path_has_path (0000)
00018A00 0040:
	_actor_path_at_destination (0000)
00018A40 0040:
	_actor_path_get_destination_firing_position_index (0000)
00018A80 00c0:
	_actor_path_input_new (0000)
00018B40 0010:
	_arccosine (0000)
00018B50 0040:
	_midpoint3d (0000)
00018B90 0090:
	_actor_test_destination (0000)
00018C20 01d0:
	_actor_get_stopping_distances (0000)
00018DF0 0080:
	_actor_move_animation_impulse (0000)
00018E70 0090:
	_actor_move_force_stop (0000)
00018F00 0250:
	_actor_move_try_evasion_vector (0000)
00019150 01a0:
	_actor_move_try_evasion_direction (0000)
000192F0 0100:
	_actor_aim_jump (0000)
000193F0 0240:
	_code_000193f0 (0000)
00019630 02f0:
	_code_00019630 (0000)
00019920 00f0:
	_code_00019920 (0000)
00019A10 0090:
	_actor_move_transform_avoidance_vector (0000)
00019AA0 0140:
	_actor_move_get_avoidance_direction (0000)
00019BE0 0150:
	_actor_move_initialize (0000)
00019D30 0110:
	_actor_path_3d_available (0000)
00019E40 0280:
	_code_00019e40 (0000)
0001A0C0 02d0:
	_code_0001a0c0 (0000)
0001A390 1030:
	_code_0001a390 (0000)
0001B3C0 05a0:
	_actor_path_refresh (0000)
0001B960 03d0:
	_actor_destination_update (0000)
0001BD30 0130:
	_actor_move_to_point (0000)
0001BE60 00b0:
	_actor_move_to_move_position (0000)
0001BF10 00b0:
	_actor_move_to_firing_position (0000)
0001BFC0 00f0:
	_actor_move_to_prop (0000)
0001C0B0 0ac0:
	_code_0001c0b0 (0000)
0001CB70 0c40:
	_actor_move_update (0000)
0001D7B0 0090:
	_actor_move_halt (0000)
0001D840 0080:
	_actor_move_halt_at_firing_position (0000)
00245318 01e8:
	_sense_ray_length (0000)
	_sense_ray_offset (0004)
	_sense_ray_divergence (0008)
	_sense_ray_offsets (000c)
	_sense_ray_divergences (0030)
	_sense_ray_angles (0054)
	_avoidance_ray_length (0078)
	_avoidance_ray_angles (007c)
	_avoidance_ray_offsets (009c)
	_avoidance_ray_divergence (00a4)
	_sense_ray_avoidance_weights (00b0)
	_avoid_ray_avoidance_weights (01d0)
	_avoid_ray_clear_bias_time (01d8)
	_avoid_ray_adjacent_fractions (01dc)
	_avoid_ray_fully_obstructed_t (01e4)
00245500 0004:
	__real@3daaaaab (0000)
00245504 0004:
	__real@bf000000 (0000)
00245508 0019:
	??_C@_0BJ@MLAKEMMB@evasion_vector?5?$CG?$CG?5result?$AA@ (0000)
00245524 0021:
	??_C@_0CB@NNMAEIGB@c?3?2halo?2SOURCE?2ai?2actor_moving?4c@ (0000)
00245548 0038:
	??_C@_0DI@MLEEHIMI@alignment_vector?5?$CG?$CG?5evade_direct@ (0000)
00245580 000c:
	??_C@_0M@LILBOJLP@collision_t?$AA@ (0000)
0024558C 001c:
	??_C@_0BM@PBPJGODL@ray_origin?5?$CG?$CG?5ray_direction?$AA@ (0000)
002455A8 0020:
	??_C@_0CA@DELIOAJE@avoidance_data?5?$CG?$CG?5avoidance_ray?$AA@ (0000)
002455C8 002a:
	??_C@_0CK@ELKMJFEA@?$CIangle?5?$DO?$DN?50?40f?$CJ?5?$CG?$CG?5?$CIangle?5?$DM?5_ful@ (0000)
002455F4 0004:
	__real@40c90fdb (0000)
002455F8 0054:
	??_C@_0FE@HKFOPCHI@warning?3?5actor_move_get_avoidanc@ (0000)
0024564C 0016:
	??_C@_0BG@BFFHAOOA@desired_facing_vector?$AA@ (0000)
00245664 0014:
	??_C@_0BE@NFPBALFN@facing_direction?9?$DOk?$AA@ (0000)
00245678 0025:
	??_C@_0CF@KDINJLFK@?$CFs?0?5?$CFs?3?5assert_valid_realcmp?$CI?$CFf?0@ (0000)
002456A0 0016:
	??_C@_0BG@DNKOIPNK@movement_direction?9?$DOk?$AA@ (0000)
002456B8 0005:
	??_C@_04MPNNEBNI@0?40f?$AA@ (0000)
002456C0 0023:
	??_C@_0CD@NOAIDOJA@?$CIreal_vector2d?5?$CK?$CJ?5facing_directi@ (0000)
002456E4 0025:
	??_C@_0CF@IPKJEDPE@?$CIreal_vector2d?5?$CK?$CJ?5movement_direc@ (0000)
0024570C 0011:
	??_C@_0BB@CEGHPIDG@facing_direction?$AA@ (0000)
00245720 0013:
	??_C@_0BD@FPDMFHB@movement_direction?$AA@ (0000)
00245734 0004:
	__real@3f860a92 (0000)
00245738 0004:
	__real@3f44ec4f (0000)
0024573C 0004:
	__real@3fa66666 (0000)
00245740 0004:
	__real@3b23d70b (0000)
00245744 0004:
	__real@be4ccccd (0000)
00245748 0004:
	__real@40200001 (0000)
00245750 0080:
	??_C@_0IA@DPLAEGLA@?$CImovement_direction_approximatio@ (0000)
002457D0 0066:
	??_C@_0GG@CIIOBGBO@?$CIbest_avoidance_direction?5?$DO?$DN?50?$CJ?5@ (0000)
00245838 0004:
	__real@ff7fffff (0000)
0024583C 0004:
	__real@41480000 (0000)
00245840 0004:
	__real@3ca3d70a (0000)
00245844 0004:
	__real@42960000 (0000)
00245848 0027:
	??_C@_0CH@FHDECA@avoidance_rotation?5?$CG?$CG?5emergency_@ (0000)
00245870 004a:
	??_C@_0EK@EPCHDGAN@actor?9?$DOcontrol?4path?4destination_@ (0000)
002458BC 0004:
	__real@3c23d70b (0000)
002458C0 002e:
	??_C@_0CO@CIMGKAKP@pathfinding?5is?5attempting?5to?5wal@ (0000)
002458F0 0004:
	__real@49742400 (0000)
002458F4 002a:
	??_C@_0CK@FMFBKFJI@?$CFs?3?5fell?5off?5end?5of?5unfinished?5p@ (0000)
00245920 000b:
	??_C@_0L@DMJIGJL@final_step?$AA@ (0000)
0024592C 0004:
	__real@3cb851ec (0000)
00245930 0004:
	__real@3d800000 (0000)
00245934 000c:
	??_C@_0M@BLMAJMLK@destination?$AA@ (0000)
00245940 0058:
	??_C@_0FI@KHOOINPJ@?$CIfacing_direction?5?$DN?$DN?5_actor_faci@ (0000)
00245998 0021:
	??_C@_0CB@CFOCDAMI@adjust?5angle?5?$CF?44f?5?9?$DO?5?$CF?44f?5?$CI?$CF?44f?$CJ@ (0000)
002459BC 001f:
	??_C@_0BP@KEBEJPIO@steer?5?$CF?44f?5?9?5oversteer?5to?5?$CF?44f?$AA@ (0000)
002459DC 0029:
	??_C@_0CJ@GAOIDHOG@steer?5?$CF?44f?5?$DM?5?$CF?44f?5?9?5clear?5overst@ (0000)
00245A08 000b:
	??_C@_0L@KHLHAIBH@steer?5?$CF?44f?$AA@ (0000)
00245A14 0020:
	??_C@_0CA@KJFKJOKH@steer?5?$CF?44f?5?$CIset?5oversteer?5?$CF?44f?$CJ?$AA@ (0000)
00245A34 0004:
	__real@bf800000 (0000)
00245A38 0039:
	??_C@_0DJ@NBFPGAMC@?$CImaximum_throttle?5?$DO?$DN?50?40f?$CJ?5?$CG?$CG?5?$CIm@ (0000)
00245A74 0004:
	__real@3f733333 (0000)
00245A78 000f:
	??_C@_0P@JJNKCIHG@?$CGfacing_vector?$AA@ (0000)
00245A88 000f:
	??_C@_0P@NHABDOCD@?$CB?$CCunreachable?$CC?$AA@ (0000)
00245A98 0004:
	__real@3a83126f (0000)
00245A9C 001c:
	??_C@_0BM@IMMKLGND@?$CGactor?9?$DOinput?4facing_vector?$AA@ (0000)
*/

/* ---------- headers */

#define REAL_MATH_EXTERNAL_POINT_FROM_LINE3D
#include "cseries/cseries.h"
#include "cseries/errors.h"
#define PATH_EXTERNAL_FLEE_ROUTINES
#include "ai/actions.h"
#include "ai/actor_definitions.h"
#include "ai/actor_looking.h"
#include "ai/actor_types.h"
#include "ai/actors.h"
#include "ai/ai_communication.h"
#include "ai/ai_debug.h"
#include "ai/ai_scenario_definitions.h"
#include "ai/path_structure_bsp.h"
#include "ai/props.h"
#include "game/game.h"
#include "main/console.h"
#include "physics/collision_bsp.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/biped_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/vehicle_datum.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"
#include "objects/object_definitions.h"
#include "objects/object_types.h"
#include "objects/objects.h"
#include "physics/collision_model_definitions.h"
#undef PATH_EXTERNAL_FLEE_ROUTINES
#undef REAL_MATH_EXTERNAL_POINT_FROM_LINE3D

/* ---------- constants */

enum
{
	VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS = 8,
};

enum
{
	_actor_facing_forward = 0,
	_actor_facing_backward,
	_actor_facing_left,
	_actor_facing_right,
	_actor_facing_free,
	NUMBER_OF_ACTOR_FACINGS,
};

enum
{
	_actor_evade_left = 0,
	_actor_evade_right,
	_actor_evade_forward,
	_actor_evade_back,
	_actor_evade_random_side,
};

enum
{
	_actor_definition_flags2_pathfinding_ignores_danger_bit = 4,
};

enum
{
	_actor_mode_asleep = 1,
	_actor_mode_alert,
	_actor_mode_combat,
};

/* ---------- macros */

#define _full_circle (2.f*_pi)

/* ---------- structures */

/* The shared vehicle tag layout remains opaque in the public header. Actor
 * movement reads this January-authenticated speed and AI-driving fragment, so
 * keep that layout fragment local to this translation unit. */
struct vehicle_definition
{
	byte __unknown0[0x2F8];
	real maximum_forward_speed;
	real maximum_reverse_speed;
	real speed_acceleration;
	real speed_deceleration;
	byte __unknown308[0x78];
	real ai_sideslip_distance;
	byte __unknown384[4];
	real ai_avoidance_distance;
	real ai_pathfinding_radius;
	byte __unknown390[8];
	real ai_oversteer_angle_lower_bound;
	real ai_oversteer_angle_upper_bound;
	real ai_steering_max_angle;
	real ai_steering_max_throttle;
};

struct vector_avoidance_ray
{
	real length;
	real_vector3d offset;
	real_vector3d divergence;
};

typedef char actor_moving_vehicle_maximum_forward_speed_offset_assert[
	offsetof(struct vehicle_definition, maximum_forward_speed) == 0x2F8 ? 1 : -1];
typedef char actor_moving_vehicle_speed_acceleration_offset_assert[
	offsetof(struct vehicle_definition, speed_acceleration) == 0x300 ? 1 : -1];
typedef char actor_moving_vehicle_sideslip_distance_offset_assert[
	offsetof(struct vehicle_definition, ai_sideslip_distance) == 0x380 ? 1 : -1];
typedef char actor_moving_vehicle_avoidance_distance_offset_assert[
	offsetof(struct vehicle_definition, ai_avoidance_distance) == 0x388 ? 1 : -1];
typedef char actor_moving_vehicle_pathfinding_radius_offset_assert[
	offsetof(struct vehicle_definition, ai_pathfinding_radius) == 0x38C ? 1 : -1];
typedef char actor_moving_vehicle_oversteer_lower_bound_offset_assert[
	offsetof(struct vehicle_definition, ai_oversteer_angle_lower_bound) == 0x398 ? 1 : -1];
typedef char actor_moving_vehicle_oversteer_upper_bound_offset_assert[
	offsetof(struct vehicle_definition, ai_oversteer_angle_upper_bound) == 0x39C ? 1 : -1];
typedef char actor_moving_vehicle_steering_max_angle_offset_assert[
	offsetof(struct vehicle_definition, ai_steering_max_angle) == 0x3A0 ? 1 : -1];
typedef char actor_moving_vehicle_steering_max_throttle_offset_assert[
	offsetof(struct vehicle_definition, ai_steering_max_throttle) == 0x3A4 ? 1 : -1];

/* ---------- prototypes */

static void actor_move_vector_avoidance(
	long actor_index,
	real_vector3d const *movement_direction,
	real_vector3d *avoidance_rotation,
	real *emergency_amount);

static void actor_move_calculate_controlled_by_aiming(
	boolean move_in_3d,
	real_vector3d const *movement_vector,
	real_vector3d const *forced_aim_direction,
	real_vector3d const *current_facing_vector,
	real_vector3d *desired_facing_vector,
	short *desired_facing_direction);

static void actor_move_calculate_free(
	boolean move_in_3d,
	real_vector3d const *movement_direction,
	real_vector3d const *facing_direction,
	real_vector3d *throttle);

static void actor_move_calculate_movement(
	long actor_index,
	boolean move_in_3d,
	short override_facing,
	boolean free_movement,
	real free_movement_distance_squared,
	boolean allow_all_moving_turns,
	real steering_maximum_angle,
	real oversteer_minimum_angle,
	real oversteer_maximum_angle,
	real rotation_emergency_amount,
	real maximum_throttle,
	real_vector3d const *desired_movement_vector,
	real_vector3d *desired_facing_vector,
	short *desired_facing_direction,
	real_vector3d *desired_throttle,
	boolean *movement_thwarted,
	boolean *movement_complete);

/* ---------- globals */

real const sense_ray_length = 1.f;
real const sense_ray_offset = 0.7f;
real const sense_ray_divergence = 0.052359879f;
real const sense_ray_offsets[9] =
{
	0.f,
	0.5f,
	0.5f,
	0.5f,
	0.5f,
	1.f,
	1.f,
	1.f,
	1.f,
};
real const sense_ray_divergences[9] =
{
	0.f,
	0.3f,
	0.3f,
	0.3f,
	0.3f,
	1.f,
	1.f,
	1.f,
	1.f,
};
real const sense_ray_angles[9] =
{
	0.f,
	0.f,
	1.5707964f,
	3.1415927f,
	4.712389f,
	0.f,
	1.5707964f,
	3.1415927f,
	4.712389f,
};
real const avoidance_ray_length = 0.7f;
real const avoidance_ray_angles[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS] =
{
	0.f,
	0.78539819f,
	1.5707964f,
	2.3561945f,
	3.1415927f,
	3.9269909f,
	4.712389f,
	5.4977875f,
};
real const avoidance_ray_offsets[2] =
{
	0.7f,
	1.f,
};
real const avoidance_ray_divergence[2] =
{
	0.5235988f,
	0.95993108f,
};
real const sense_ray_avoidance_weights[9][VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS] =
{
	{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f },
	{ -0.5f, -0.5f, 0.f, 0.7f, 1.f, 0.7f, 0.f, -0.5f },
	{ 0.f, -0.5f, -0.5f, -0.5f, 0.f, 0.7f, 1.f, 0.7f },
	{ 1.f, 0.7f, 0.f, -0.5f, -0.5f, -0.5f, 0.f, 0.7f },
	{ 0.f, 0.7f, 1.f, 0.7f, 0.f, -0.5f, -0.5f, -0.5f },
};
real const avoid_ray_avoidance_weights[2] =
{
	0.8f,
	1.2f,
};
long const avoid_ray_clear_bias_time = 75;
real const avoid_ray_adjacent_fractions[2] =
{
	0.8f,
	0.5f,
};
real const avoid_ray_fully_obstructed_t = 0.5f;

extern struct vector_avoidance_ray avoidance_rays[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS][2];
extern real_vector3d avoidance_directions[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS];
extern struct vector_avoidance_ray sense_rays[9];

/* ---------- public code */

void actor_move_keep_moving_past_destination(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->orders.move.destination.keep_moving = TRUE;
	actor->control.path.destination_orders.keep_moving = TRUE;

	return;
}

boolean actor_move_animation_busy(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->orders.move.animation.impulse == NONE)
	{
		long unit_index = actor->meta.unit_index;
		if (unit_index == NONE || !unit_is_busy(unit_index))
			return FALSE;
	}

	return TRUE;
}

void actor_path_clear(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->control.path.path.valid = FALSE;
	actor->control.path.at_destination = TRUE;
	actor->control.path.destination_original_distance = 0.0f;

	return;
}

boolean actor_path_has_path(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	return actor->control.path.path.valid;
}

boolean actor_path_at_destination(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	return !actor->control.path.path.valid || actor->control.path.at_destination;
}

short actor_path_get_destination_firing_position_index(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	short firing_position_index = NONE;

	if (actor->control.path.destination_orders.destination_type == _destination_firing_position)
		firing_position_index = actor->control.path.destination_orders.firing_position_index;

	return firing_position_index;
}

void actor_path_input_new(
	long actor_index,
	struct path_input *input)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	real pathfinding_radius = definition->moving.pathfinding_radius;
	long ignore_source_object_index = actor->meta.unit_index;

	if (actor->input.vehicle_driver_type > _actor_vehicle_driver_none)
	{
		struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);
		struct vehicle_definition *vehicle_definition =
			vehicle_specific_definition_get(vehicle->definition_index);

		ignore_source_object_index = actor->input.vehicle_index;
		if (vehicle_definition->ai_pathfinding_radius > 0.f)
			pathfinding_radius = vehicle_definition->ai_pathfinding_radius;
	}

	actor_find_pathfinding_location(actor_index);
	path_input_new(
		input,
		pathfinding_radius,
		actor->emotions.ignorant_of_broken_surfaces,
		ignore_source_object_index);
	path_input_set_start(
		input,
		&actor->input.pathfinding_point,
		actor->input.pathfinding_surface_index);

	return;
}

boolean actor_move_animation_impulse(
	long actor_index,
	short impulse,
	real_vector2d const *alignment_vector)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	actor_set_dormant(actor_index, FALSE);

	if (!actor_move_animation_busy(actor_index))
	{
		actor->orders.move.animation.impulse = impulse;
		actor->orders.move.animation.alignment_vector = *alignment_vector;
		result = TRUE;
	}

	return result;
}

boolean actor_move_force_stop(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (!actor_move_animation_busy(actor_index) &&
		!actor_action_deny_transition(actor_index))
	{
		actor->control.moving = FALSE;
		actor->output.throttle = *global_zero_vector3d;
		actor_unit_control_stop_animation_impulse(actor_index);
		result = TRUE;
	}

	return result;
}

boolean actor_aim_jump(
	long actor_index,
	long unit_index,
	boolean leap,
	real jump_magnitude,
	real_vector3d *jump_velocity)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->input.vehicle_index == NONE)
	{
		if (actor->meta.swarm)
		{
			actor_type_swarm_aim_jump(
				actor_index,
				unit_index,
				jump_magnitude,
				jump_velocity);
		}
		else if (actor->control.jumping_targeted)
		{
			real magnitude;

			if (actor->state.action == _actor_action_charge &&
				actor->state.action_data.charge.goal == _charge_goal_melee_leaping)
			{
				leap = TRUE;
			}

			set_real_vector3d(
				jump_velocity,
				actor->control.jump_alignment_vector.i * actor->control.jump_target_horizontal_vel,
				actor->control.jump_alignment_vector.j * actor->control.jump_target_horizontal_vel,
				actor->control.jump_target_vertical_vel);
			magnitude = magnitude3d(jump_velocity);

			if (!leap && magnitude > jump_magnitude)
			{
				scale_vector3d(jump_velocity, jump_magnitude / magnitude, jump_velocity);
			}
		}
	}

	actor->control.jumping_targeted = FALSE;
	return TRUE;
}

void actor_move_transform_avoidance_vector(
	struct vector_avoidance_data *avoidance_data,
	real_vector3d const *avoidance_vector,
	real_vector3d *direction_vector)
{
	real component;

	*direction_vector = *global_zero_vector3d;

	component = avoidance_vector->i;
	direction_vector->i += component * avoidance_data->forward.i;
	direction_vector->j += component * avoidance_data->forward.j;
	direction_vector->k += component * avoidance_data->forward.k;

	component = avoidance_vector->j;
	direction_vector->i += component * avoidance_data->left.i;
	direction_vector->j += component * avoidance_data->left.j;
	direction_vector->k += component * avoidance_data->left.k;

	component = avoidance_vector->k;
	direction_vector->i += component * avoidance_data->up.i;
	direction_vector->j += component * avoidance_data->up.j;
	direction_vector->k += component * avoidance_data->up.k;

	return;
}

void actor_move_get_avoidance_direction(
	struct vector_avoidance_data *avoidance_data,
	real direction,
	real_vector3d *direction_vector)
{
	real_vector3d avoidance_vector;
	real angle = REAL_MAX;
	short direction_index;

	if (direction < 0.f || direction >= (real)VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS)
		direction = 0.f;

	for (direction_index = 0;
		direction_index < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
		direction_index++)
	{
		if ((real)direction_index + 1.f > direction)
		{
			real fraction = direction - (real)direction_index;
			real angle0 = avoidance_ray_angles[direction_index];
			real angle1 =
				direction_index == VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS - 1
					? avoidance_ray_angles[0]
					: avoidance_ray_angles[direction_index + 1];

			angle = angle0*(1.f - fraction) + angle1*fraction;
			break;
		}
	}

	if (angle == REAL_MAX)
	{
		angle = 0.f;
		error(
			_error_silent,
			"warning: actor_move_get_avoidance_vector couldn't find out-of-bounds direction %.4f",
			direction);
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		2743,
		(angle >= 0.0f) && (angle < _full_circle));

	avoidance_vector.i = 0.f;
	avoidance_vector.j = cosine(angle);
	avoidance_vector.k = sine(angle);
	actor_move_transform_avoidance_vector(
		avoidance_data,
		&avoidance_vector,
		direction_vector);

	return;
}

static void actor_move_avoidance_setup(
	struct vector_avoidance_data *avoidance_data)
{
	long object_indices[2048];
	struct object_datum *object = object_get(avoidance_data->object_index);
	short object_count = objects_in_sphere(
		_object_mask_biped,
		_object_mask_sightblocking,
		&object->object.location,
		&avoidance_data->origin,
		MAX(avoidance_ray_length, sense_ray_length) * avoidance_data->avoid_distance,
		object_indices,
		NUMBEROF(object_indices));
	short object_number;

	avoidance_data->avoidance_object_count = 0;
	for (object_number = 0; object_number < object_count; object_number++)
	{
		long object_index = object_indices[object_number];
		struct object_datum *avoid_object = object_get(object_index);

		if (object_index != NONE &&
			object_index != avoidance_data->object_index)
		{
			struct object_definition *definition =
				object_definition_get(avoid_object->definition_index);
			struct collision_model *collision_model =
				collision_model_definition_get(definition->object.collision_model.index);

			if (collision_model->pathfinding_spheres.count > 0)
			{
				real_matrix4x3 world_matrix;
				real_point3d center;
				real radius;
				real maximum_radius = 0.f;
				short sphere_index;
				short avoidance_object_index;

				object_get_bounding_sphere(object_index, &center, &radius);
				object_get_world_matrix(object_index, &world_matrix);
				for (sphere_index = 0;
					sphere_index < collision_model->pathfinding_spheres.count;
					sphere_index++)
				{
					struct pathfinding_sphere *sphere = TAG_BLOCK_GET_ELEMENT(
						&collision_model->pathfinding_spheres,
						sphere_index,
						struct pathfinding_sphere);
					real_point3d sphere_center;
					real sphere_radius;
					real sphere_distance;

					if (sphere->node_index != NONE)
					{
						real_matrix4x3 *node_matrix =
							object_get_node_matrix(object_index, sphere->node_index);

						matrix4x3_transform_point(node_matrix, &sphere->center, &sphere_center);
						sphere_radius = sphere->radius*node_matrix->scale;
					}
					else
					{
						matrix4x3_transform_point(&world_matrix, &sphere->center, &sphere_center);
						sphere_radius = world_matrix.scale*sphere->radius;
					}

					sphere_distance = distance2d(
						(real_point2d const *)&center,
						(real_point2d const *)&sphere_center) + sphere_radius;
					maximum_radius = MAX(maximum_radius, sphere_distance);
				}

				avoidance_object_index = avoidance_data->avoidance_object_count;
				if (avoidance_object_index < MAXIMUM_NUMBER_OF_AVOIDANCE_OBJECTS)
				{
					avoidance_data->avoidance_object_count = avoidance_object_index + 1;
					avoidance_data->avoidance_objects[avoidance_object_index].width = maximum_radius;
					avoidance_data->avoidance_objects[avoidance_object_index].base = center;
					avoidance_data->avoidance_objects[avoidance_object_index].object_index = object_index;
					avoidance_data->avoidance_objects[avoidance_object_index].base.z -= radius - maximum_radius;
					avoidance_data->avoidance_objects[avoidance_object_index].height =
						MAX(0.f, 2.f*radius - 2.f*maximum_radius);
				}
			}
		}
	}

	return;
}

static short actor_move_test_avoidance_vector(
	struct vector_avoidance_data *avoidance_data,
	struct vector_avoidance_ray const *avoidance_ray,
	real_point3d *ray_origin,
	real_vector3d *ray_direction,
	real *collision_t,
	byte *collision_timer)
{
	struct collision_bsp_test_vector_result collision;
	real_vector3d offset;
	real_vector3d divergence;
	real scale;
	short result = _actor_vector_avoidance_clear;
	short object_index;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		2039,
		avoidance_data && avoidance_ray);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		2040,
		ray_origin && ray_direction);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		2041,
		collision_t);

	*collision_t = REAL_MAX;

	actor_move_transform_avoidance_vector(avoidance_data, &avoidance_ray->offset, &offset);
	actor_move_transform_avoidance_vector(avoidance_data, &avoidance_ray->divergence, &divergence);

	ray_origin->x = offset.i*avoidance_data->avoid_width + avoidance_data->origin.x;
	ray_origin->y = offset.j*avoidance_data->avoid_width + avoidance_data->origin.y;
	ray_origin->z = offset.k*avoidance_data->avoid_width + avoidance_data->origin.z;

	scale = avoidance_data->avoid_distance*avoidance_ray->length;
	ray_direction->i = divergence.i*scale;
	ray_direction->j = divergence.j*scale;
	ray_direction->k = divergence.k*scale;

	vector_from_points3d(&avoidance_data->origin, ray_origin, &offset);
	if (collision_bsp_test_vector(
		FLAG(_collision_test_front_facing_surfaces_bit) |
			FLAG(_collision_test_back_facing_surfaces_bit),
		avoidance_data->bsp,
		0,
		NULL,
		&avoidance_data->origin,
		&offset,
		1.f,
		&collision))
	{
		result = _actor_vector_avoidance_obstructed_structure;
		*collision_t = 0.f;
	}
	else if (collision_bsp_test_vector(
		FLAG(_collision_test_front_facing_surfaces_bit) |
			FLAG(_collision_test_back_facing_surfaces_bit),
		avoidance_data->bsp,
		0,
		NULL,
		ray_origin,
		ray_direction,
		1.f,
		&collision))
	{
		result = _actor_vector_avoidance_obstructed_structure;
		*collision_t = collision.t;
	}

	for (object_index = 0;
		object_index < avoidance_data->avoidance_object_count;
		object_index++)
	{
		struct vehicle_avoidance_cylinder *cylinder =
			&avoidance_data->avoidance_objects[object_index];
		real object_t;

		if (pill_test_vector3d(
			&cylinder->base,
			cylinder->height,
			cylinder->width,
			ray_origin,
			ray_direction,
			&object_t,
			&offset) &&
			object_t < *collision_t)
		{
			result = _actor_vector_avoidance_obstructed_object;
			*collision_t = object_t;
		}
	}

	if (collision_timer)
	{
		if (result > _actor_vector_avoidance_clear)
		{
			*collision_timer = 0;
		}
		else if (*collision_timer < UNSIGNED_CHAR_MAX)
		{
			(*collision_timer)++;
		}
	}

	return result;
}

static boolean actor_move_vector_avoidance_find_direction(
	short direction_count,
	real_vector3d const *directions,
	real const *weights,
	real_vector3d const *direction_vector,
	real *approximate_direction_reference,
	real *approximate_weight_reference)
{
	short previous_index = direction_count - 1;
	real previous_cross =
		directions[previous_index].j*direction_vector->k -
		directions[previous_index].k*direction_vector->j;
	short direction_index;

	for (direction_index = 0;
		direction_index < direction_count;
		direction_index++)
	{
		real cross =
			directions[direction_index].j*direction_vector->k -
			directions[direction_index].k*direction_vector->j;

		if (previous_cross*cross <= 0.f &&
			dot_product3d(direction_vector, &directions[direction_index]) > 0.f)
		{
			*approximate_direction_reference =
				((real)previous_index*cross -
					(real)(direction_index == 0 ? direction_count : direction_index)*previous_cross) /
				(cross - previous_cross);
			*approximate_weight_reference =
				(weights[previous_index]*cross - weights[direction_index]*previous_cross) /
				(cross - previous_cross);
			return TRUE;
		}

		previous_index = direction_index;
		previous_cross = cross;
	}

	return FALSE;
}

void actor_move_initialize(
	void)
{
	short sense_ray_index;

	for (sense_ray_index = 0;
		sense_ray_index < NUMBEROF(sense_rays);
		sense_ray_index++)
	{
		real ray_sine = sine(sense_ray_angles[sense_ray_index]);
		real ray_cosine = cosine(sense_ray_angles[sense_ray_index]);
		real divergence = sense_ray_divergence *
			sense_ray_divergences[sense_ray_index];
		real divergence_sine = sine(divergence);
		real divergence_cosine = cosine(divergence);
		real offset = sense_ray_offset * sense_ray_offsets[sense_ray_index];

		sense_rays[sense_ray_index].length = sense_ray_length;
		sense_rays[sense_ray_index].offset.i = 0.f;
		sense_rays[sense_ray_index].offset.j = offset * ray_cosine;
		sense_rays[sense_ray_index].offset.k = offset * ray_sine;
		sense_rays[sense_ray_index].divergence.i = divergence_cosine;
		sense_rays[sense_ray_index].divergence.j = divergence_sine * ray_cosine;
		sense_rays[sense_ray_index].divergence.k = divergence_sine * ray_sine;
	}

	{
		short avoidance_ray_index;

		for (avoidance_ray_index = 0;
			avoidance_ray_index < NUMBEROF(avoidance_ray_offsets);
			avoidance_ray_index++)
		{
			real divergence_sine = sine(
				avoidance_ray_divergence[avoidance_ray_index]);
			real divergence_cosine = cosine(
				avoidance_ray_divergence[avoidance_ray_index]);
			real offset = avoidance_ray_offsets[avoidance_ray_index];
			long direction_index;

			for (direction_index = 0;
				direction_index < NUMBEROF(avoidance_directions);
				direction_index++)
			{
				set_real_vector3d(
					&avoidance_directions[direction_index],
					0.f,
					cosine(avoidance_ray_angles[direction_index]),
					sine(avoidance_ray_angles[direction_index]));
				avoidance_rays[direction_index][avoidance_ray_index].length = avoidance_ray_length;
				scale_vector3d(
					&avoidance_directions[direction_index],
					offset,
					&avoidance_rays[direction_index][avoidance_ray_index].offset);
				scale_vector3d(
					&avoidance_directions[direction_index],
					divergence_sine,
					&avoidance_rays[direction_index][avoidance_ray_index].divergence);
				avoidance_rays[direction_index][avoidance_ray_index].divergence.i = divergence_cosine;
			}
		}
	}

	return;
}

boolean actor_path_3d_available(
	long actor_index,
	real_point3d const *destination,
	real *avoidance_distance_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	real avoidance_distance = 0.f;
	boolean available = TRUE;

	if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying)
	{
		struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);
		struct vehicle_definition *vehicle_definition =
			vehicle_specific_definition_get(vehicle->definition_index);

		avoidance_distance = vehicle_definition->ai_avoidance_distance;
		if (avoidance_distance > 0.f &&
			actor->control.vector_avoidance_rotation_emergency_instantaneous > 0.9f)
		{
			real_vector3d direction;

			if (normalize3d(vector_from_points3d(
					&actor->input.position.body_position,
					destination,
					&direction)) > 0.f &&
				dot_product3d(&direction, &actor->input.facing_vector) > 0.984f)
			{
				available = FALSE;
			}
		}
	}

	if (avoidance_distance_reference)
		*avoidance_distance_reference = avoidance_distance;

	return available;
}

/* ---------- private code */

boolean actor_test_destination(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->control.path.destination_orders.destination_type == _destination_none ||
		actor->control.path.destination_orders.destination_type == _destination_halt)
	{
		actor->control.path.at_destination = TRUE;
	}
	else
	{
		real tolerance = actor_destination_tolerance(actor_index);

		if (distance_squared3d(
			&actor->input.position.body_position,
			&actor->control.path.destination.point) < tolerance*tolerance)
		{
			actor->control.path.at_destination = TRUE;
		}
	}

	return actor->control.path.at_destination;
}

void actor_get_stopping_distances(
	long actor_index,
	real *current_stopping_distance_reference,
	real *maximum_stopping_distance_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	real reference_speed = 0.083333336f;
	real current_speed = 0.0f;
	real deceleration = 0.016666668f;
	real stopping_deceleration = 0.026666667f;

	if (actor->input.vehicle_index != NONE)
	{
		switch (actor->input.vehicle_driver_type)
		{
		case _actor_vehicle_driver_hovering_ground:
		case _actor_vehicle_driver_nondirectional_ground:
		{
			struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);
			struct vehicle_definition *definition =
				vehicle_specific_definition_get(vehicle->definition_index);

			current_speed = dot_product3d(
				&vehicle->object.translational_velocity,
				&vehicle->object.forward);
			reference_speed = definition->maximum_forward_speed;
			deceleration = definition->speed_acceleration;
			stopping_deceleration = deceleration;
			break;
		}
		}
	}
	else if (actor->meta.unit_index != NONE)
	{
		struct biped_datum *biped = biped_try_and_get(actor->meta.unit_index);

		if (biped)
		{
			struct biped_definition *definition = biped_definition_get(biped->definition_index);

			current_speed = dot_product3d(
				&biped->object.translational_velocity,
				&biped->object.forward);
			if (TEST_FLAG(definition->biped.flags, _biped_flying_bit))
			{
				reference_speed = definition->biped.flying_velocity *
					(1.0f / TICKS_PER_SECOND);
				deceleration = definition->biped.flying_acceleration *
					(1.0f / TICKS_PER_SECOND);
				stopping_deceleration = definition->biped.flying_deceleration *
					(1.0f / TICKS_PER_SECOND);

				if (actor->control.crouching &&
					definition->biped.flying_crouch_velocity_modifier > 0.0f)
				{
					reference_speed *= definition->biped.flying_crouch_velocity_modifier;
					deceleration *= definition->biped.flying_crouch_velocity_modifier;
					stopping_deceleration *= definition->biped.flying_crouch_velocity_modifier;
				}
			}
		}
	}

	if (current_stopping_distance_reference)
	{
		*current_stopping_distance_reference = current_speed * current_speed /
			(stopping_deceleration * 2.0f);
	}

	if (maximum_stopping_distance_reference)
	{
		if (current_speed > reference_speed)
			reference_speed = current_speed;

		*maximum_stopping_distance_reference =
			(reference_speed * reference_speed - current_speed * current_speed) /
				(deceleration * 2.0f) +
			reference_speed * reference_speed / (stopping_deceleration * 2.0f);
	}

	return;
}

boolean actor_move_try_evasion_vector(
	long actor_index,
	real_vector2d *evasion_vector,
	real evade_distance,
	real maximum_ledge_height,
	boolean *evasion_is_ledge_reference,
	struct path_collision_result *result)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	boolean found = FALSE;
	boolean evasion_is_ledge = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		0x484,
		evasion_vector && result);

	if (!actor->state.flying)
	{
		real_point2d evasion_point;

		evasion_point.x = evade_distance * evasion_vector->i +
			actor->input.position.body_position.x;
		evasion_point.y = evade_distance * evasion_vector->j +
			actor->input.position.body_position.y;

		actor_find_pathfinding_location(actor_index);
		if (!structure_test_pill2d(
			global_structure_bsp_get(),
			actor->emotions.ignorant_of_broken_surfaces,
			(real_point2d const *)&actor->input.pathfinding_point,
			actor->input.pathfinding_surface_index,
			&evasion_point,
			NONE,
			definition->moving.pathfinding_radius,
			0,
			result))
		{
			real height_delta = result->point.z - actor->input.position.body_position.z;

			found = TRUE;
			if (height_delta > evade_distance * 0.5f)
			{
				found = FALSE;
			}
			else if (maximum_ledge_height == 0.0f &&
				height_delta < evade_distance * -0.5f)
			{
				found = FALSE;
			}
		}

		if (!found && maximum_ledge_height > 0.0f)
		{
			struct collision_bsp *collision_bsp = global_collision_bsp_get();
			struct collision_bsp_test_vector_result collision_result;
			real_point3d actor_midpoint;
			real_vector3d collision_vector;

			midpoint3d(
				&actor->input.position.head_position,
				&actor->input.position.body_position,
				&actor_midpoint);
			collision_vector.i = evade_distance * evasion_vector->i;
			collision_vector.j = evade_distance * evasion_vector->j;
			collision_vector.k = 0.0f;

			if (!collision_bsp_test_vector(
				FLAG(_collision_test_front_facing_surfaces_bit) |
					FLAG(_collision_test_back_facing_surfaces_bit),
				collision_bsp,
				0,
				NULL,
				&actor_midpoint,
				&collision_vector,
				REAL_MAX,
				&collision_result))
			{
				found = TRUE;
				evasion_is_ledge = TRUE;
				if (maximum_ledge_height < REAL_MAX)
				{
					real_point3d down_ray_start;
					real_vector3d down_ray_vector;

					point_from_line3d(
						&actor_midpoint,
						&collision_vector,
						1.0f,
						&down_ray_start);
					scale_vector3d(
						global_down3d,
						maximum_ledge_height,
						&down_ray_vector);
					if (!collision_bsp_test_vector(
						FLAG(_collision_test_front_facing_surfaces_bit) |
							FLAG(_collision_test_back_facing_surfaces_bit),
						collision_bsp,
						0,
						NULL,
						&down_ray_start,
						&down_ray_vector,
						REAL_MAX,
						&collision_result))
					{
						found = FALSE;
					}
				}
			}
		}
	}

	if (evasion_is_ledge_reference)
		*evasion_is_ledge_reference = evasion_is_ledge;

	return found;
}

boolean actor_move_try_evasion_direction(
	long actor_index,
	real_vector2d *alignment_vector,
	real evade_distance,
	short *evade_direction_reference,
	real maximum_ledge_height,
	boolean *evasion_is_ledge,
	struct path_collision_result *result)
{
	real_vector2d evasion_vector;
	short attempt_count;
	short attempt_index;
	short evade_direction;
	boolean success = FALSE;

	actor_get(actor_index);
	attempt_count = 1;
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		0x4E4,
		alignment_vector && evade_direction_reference && result);

	evade_direction = *evade_direction_reference;
	switch (evade_direction)
	{
	case _actor_evade_left:
		set_real_vector2d(&evasion_vector, -alignment_vector->j, alignment_vector->i);
		break;

	case _actor_evade_right:
		set_real_vector2d(&evasion_vector, alignment_vector->j, -alignment_vector->i);
		break;

	case _actor_evade_forward:
		set_real_vector2d(&evasion_vector, alignment_vector->i, alignment_vector->j);
		break;

	case _actor_evade_back:
		set_real_vector2d(&evasion_vector, -alignment_vector->i, -alignment_vector->j);
		break;

	case _actor_evade_random_side:
		if (seed_random(get_global_random_seed_address()) > 0x8000)
		{
			evade_direction = _actor_evade_left;
			set_real_vector2d(&evasion_vector, -alignment_vector->j, alignment_vector->i);
		}
		else
		{
			evade_direction = _actor_evade_right;
			set_real_vector2d(&evasion_vector, alignment_vector->j, -alignment_vector->i);
		}
		attempt_count = 2;
		break;

	default:
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			0x508,
			FALSE,
			NULL);
		break;
	}

	for (attempt_index = 0; attempt_index < attempt_count; attempt_index++, evade_direction ^= 1)
	{
		if (actor_move_try_evasion_vector(
			actor_index,
			&evasion_vector,
			evade_distance,
			maximum_ledge_height,
			evasion_is_ledge,
			result))
		{
			success = TRUE;
			break;
		}

		evasion_vector.i = -evasion_vector.i;
		evasion_vector.j = -evasion_vector.j;
	}

	*evade_direction_reference = success ? evade_direction : NONE;
	return success;
}

static void actor_move_vector_avoidance(
	long actor_index,
	real_vector3d const *movement_direction,
	real_vector3d *avoidance_rotation,
	real *emergency_amount)
{
	struct actor_datum *actor = actor_get(actor_index);
	real emergency = 0.f;
	real_vector3d rotation = *global_zero_vector3d;
	long object_index = actor->input.vehicle_index;

	if (object_index == NONE)
		object_index = actor->meta.unit_index;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		2183,
		avoidance_rotation && emergency_amount);

	if (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);
		boolean direction_chosen = FALSE;
		struct actor_debug_info *debug_info =
			&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
		struct vector_avoidance_data avoidance_data;
		real weights[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS];
		real_vector3d movement_vector;
		real_vector3d local_movement_direction;
		real maximum_sense_emergency;
		real angular_speed;
		real best_weight;
		real movement_direction_approximation;
		real movement_approximate_weight;
		real forward_dot;
		real weight_difference;
		real emergency_scale;
		short best_avoidance_direction;
		short direction_index;
		short ray_index;
		boolean sharp_turn = FALSE;

		debug_info->field_19C = game_time_get();
		avoidance_data.structure = global_structure_bsp_get();
		avoidance_data.bsp = global_collision_bsp_get();
		avoidance_data.object_index = object_index;
		object_get_origin(object_index, &avoidance_data.origin);
		avoidance_data.forward = object->object.forward;
		avoidance_data.up = object->object.up;
		cross_product3d(&object->object.up, &object->object.forward, &avoidance_data.left);
		avoidance_data.avoid_distance = 12.f;
		avoidance_data.avoid_width = 1.f;
		actor_move_avoidance_setup(&avoidance_data);

		maximum_sense_emergency = 0.f;
		csmemset(weights, 0, sizeof(weights));

		{
			short current_direction = actor->control.vector_avoidance_current_direction;

			if (VALID_INDEX(current_direction, VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS))
			{
				short next_direction = (current_direction + 1) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short second_next_direction = (current_direction + 2) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short previous_direction = (current_direction + VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS - 1) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short second_previous_direction = (current_direction + VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS - 2) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;

				weights[current_direction] += 0.4f;
				weights[next_direction] += avoid_ray_adjacent_fractions[0]*0.4f;
				weights[second_next_direction] += avoid_ray_adjacent_fractions[1]*0.4f;
				weights[previous_direction] += avoid_ray_adjacent_fractions[0]*0.4f;
				weights[second_previous_direction] += avoid_ray_adjacent_fractions[1]*0.4f;
			}
		}

		for (ray_index = 0; ray_index < NUMBEROF(sense_rays); ray_index++)
		{
			real_point3d ray_origin;
			real_vector3d ray_direction;
			real collision_t;
			short avoidance_type = actor_move_test_avoidance_vector(
				&avoidance_data,
				&sense_rays[ray_index],
				&ray_origin,
				&ray_direction,
				&collision_t,
				NULL);

			debug_info->ray_origin[ray_index] = ray_origin;
			debug_info->ray_direction[ray_index] = ray_direction;
			debug_info->avoidance_type[ray_index] = avoidance_type;
			debug_info->collision_t[ray_index] = collision_t;
			if (avoidance_type > _actor_vector_avoidance_clear)
			{
				real sense_emergency = 1.f - collision_t;
				real sense_weight = 2.f*sense_emergency;

				for (direction_index = 0;
					direction_index < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
					direction_index++)
				{
					weights[direction_index] +=
						sense_ray_avoidance_weights[ray_index][direction_index]*MIN(sense_weight, 1.f);
				}
				maximum_sense_emergency = MAX(maximum_sense_emergency, sense_emergency);
			}
		}

		for (direction_index = 0;
			direction_index < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
			direction_index++)
		{
			short avoidance_types[NUMBEROF(avoid_ray_avoidance_weights)];
			real avoidance_t[NUMBEROF(avoid_ray_avoidance_weights)];
			real direction_weight;
			boolean obstructed;

			for (ray_index = 0; ray_index < NUMBEROF(avoid_ray_avoidance_weights); ray_index++)
			{
				real_point3d ray_origin;
				real_vector3d ray_direction;

				avoidance_types[ray_index] = actor_move_test_avoidance_vector(
					&avoidance_data,
					&avoidance_rays[direction_index][ray_index],
					&ray_origin,
					&ray_direction,
					&avoidance_t[ray_index],
					&actor->control.vector_avoidance_clear_times[direction_index][ray_index]);
				debug_info->field_6358[direction_index][ray_index] = ray_origin;
				debug_info->field_6418[direction_index][ray_index] = ray_direction;
				debug_info->field_62F8[direction_index][ray_index] = avoidance_types[ray_index];
				debug_info->avoid_t[direction_index][ray_index] = avoidance_t[ray_index];
			}

			direction_weight = 0.f;
			obstructed = FALSE;

			for (ray_index = NUMBEROF(avoid_ray_avoidance_weights) - 1; ray_index >= 0; ray_index--)
			{
				if (avoidance_types[ray_index] == _actor_vector_avoidance_clear)
				{
					real clear_fraction = 1.f;

					if (!obstructed)
					{
						byte clear_time = actor->control.vector_avoidance_clear_times[direction_index][ray_index];

						if (clear_time < avoid_ray_clear_bias_time)
						{
							clear_fraction = 0.f;
						}
						else
						{
							clear_fraction = PIN(
								1.f - (real)avoid_ray_clear_bias_time/(real)clear_time,
								0.f,
								1.f);
						}
					}
					direction_weight += avoid_ray_avoidance_weights[ray_index]*clear_fraction;
				}
				else
				{
					direction_weight -=
						avoid_ray_avoidance_weights[ray_index]*MIN(2.f*(1.f - avoidance_t[ray_index]), 1.f);
					obstructed = TRUE;
				}
			}

			{
				short next_direction = (direction_index + 1) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short second_next_direction = (direction_index + 2) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short previous_direction = (direction_index + VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS - 1) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
				short second_previous_direction = (direction_index + VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS - 2) % VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;

				weights[direction_index] += direction_weight;
				weights[next_direction] += avoid_ray_adjacent_fractions[0]*direction_weight;
				weights[second_next_direction] += avoid_ray_adjacent_fractions[1]*direction_weight;
				weights[previous_direction] += avoid_ray_adjacent_fractions[0]*direction_weight;
				weights[second_previous_direction] += avoid_ray_adjacent_fractions[1]*direction_weight;
			}
		}

		angular_speed = magnitude3d(&object->object.angular_velocity);
		debug_info->field_6551 = FALSE;
		if (angular_speed > 0.02f)
		{
			real velocity_weight = MIN((angular_speed - 0.02f)*12.5f, 1.f)*0.8f;
			real_vector3d velocity_direction;
			real velocity_approximate_direction;
			real velocity_approximate_weight = 0.f;

			velocity_direction.i = 0.f;
			velocity_direction.j = dot_product3d(&avoidance_data.up, &object->object.angular_velocity);
			velocity_direction.k = -dot_product3d(&avoidance_data.left, &object->object.angular_velocity);
			if (normalize3d(&velocity_direction) > 0.f &&
				actor_move_vector_avoidance_find_direction(
					VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS,
					avoidance_directions,
					weights,
					&velocity_direction,
					&velocity_approximate_direction,
					&velocity_approximate_weight) &&
				velocity_approximate_weight > 0.5f)
			{
				for (direction_index = 0;
					direction_index < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
					direction_index++)
				{
					real velocity_dot =
						dot_product3d(&avoidance_directions[direction_index], &velocity_direction);

					if (velocity_dot < 0.f)
						weights[direction_index] += velocity_dot*velocity_weight;
				}
			}
			else
			{
				velocity_weight = 0.f;
			}

			debug_info->field_6554 = velocity_weight;
			debug_info->field_6558 = angular_speed;
			debug_info->field_6551 = TRUE;
			debug_info->avoidance_vector = velocity_direction;
			debug_info->field_6568 = velocity_approximate_weight;
		}

		best_weight = -REAL_MAX;
		best_avoidance_direction = NONE;
		for (direction_index = 0;
			direction_index < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS;
			direction_index++)
		{
			if (weights[direction_index] > best_weight)
			{
				best_weight = weights[direction_index];
				best_avoidance_direction = direction_index;
			}
		}
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			2435,
			(best_avoidance_direction >= 0) && (best_avoidance_direction < VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS));
		csmemcpy(debug_info->field_64D8, weights, sizeof(weights));

		movement_vector = *movement_direction;
		forward_dot = 1.f;
		local_movement_direction = *global_zero_vector3d;
		movement_direction_approximation = 0.f;
		movement_approximate_weight = 0.f;
		if (normalize3d(&movement_vector) > 0.f)
		{
			local_movement_direction.i = 0.f;
			forward_dot = dot_product3d(&avoidance_data.forward, &movement_vector);
			local_movement_direction.j = dot_product3d(&avoidance_data.left, &movement_vector);
			local_movement_direction.k = dot_product3d(&avoidance_data.up, &movement_vector);
			if (normalize3d(&local_movement_direction) > 0.f)
			{
				actor_move_vector_avoidance_find_direction(
					VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS,
					avoidance_directions,
					weights,
					&local_movement_direction,
					&movement_direction_approximation,
					&movement_approximate_weight);
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_moving.c",
					2466,
					(movement_direction_approximation >= 0) && (movement_direction_approximation <= ((real) VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS)));
			}
		}

		weight_difference = best_weight - movement_approximate_weight;
		debug_info->field_6524 = avoidance_data.forward;
		debug_info->field_6530 = *movement_direction;
		debug_info->field_6504 = movement_direction_approximation;
		debug_info->field_64FC = best_weight;
		debug_info->field_6500 = best_avoidance_direction;
		debug_info->field_6508 = movement_approximate_weight;
		if (maximum_sense_emergency > 0.6f)
		{
			emergency_scale = 1.f + MIN(1.f, (maximum_sense_emergency - 0.6f)/(1.f - 0.6f));
		}
		else
		{
			emergency_scale = MIN(1.f, maximum_sense_emergency/0.3f);
		}
		debug_info->field_6510 = forward_dot;
		debug_info->sign_no_danger = weight_difference;

		if (forward_dot < -0.2f)
		{
			if (actor->control.vector_avoidance_sharp_turn_timer != NONE &&
				actor->control.vector_avoidance_sharp_turn_timer < 90)
			{
				debug_info->field_653C = 7;
				sharp_turn = TRUE;
			}
			else if (magnitude_squared3d(&object->object.angular_velocity) > 0.05f*0.05f)
			{
				if (weight_difference > 2.f && best_weight > 2.f)
				{
					debug_info->field_653C = 6;
					sharp_turn = TRUE;
				}
			}
			else if (emergency_scale > 0.5f)
			{
				debug_info->field_653C = 5;
				sharp_turn = TRUE;
			}
		}

		if (sharp_turn)
		{
			real_vector3d best_direction;
			real_vector3d rotation_axis;

			if (actor->control.vector_avoidance_sharp_turn_timer == NONE)
				actor->control.vector_avoidance_sharp_turn_timer = 0;
			else
				actor->control.vector_avoidance_sharp_turn_timer++;

			actor_move_transform_avoidance_vector(
				&avoidance_data,
				&avoidance_directions[best_avoidance_direction],
				&best_direction);
			rotation_axis.i = movement_direction->j*best_direction.k - movement_direction->k*best_direction.j;
			rotation_axis.j = movement_direction->k*best_direction.i - movement_direction->i*best_direction.k;
			rotation_axis.k = movement_direction->i*best_direction.j - movement_direction->j*best_direction.i;
			if (normalize3d(&rotation_axis) > 0.f)
			{
				real rotation_angle = angle_between_vectors3d(movement_direction, &best_direction);

				rotation.i = rotation_axis.i*rotation_angle;
				rotation.j = rotation_axis.j*rotation_angle;
				rotation.k = rotation_axis.k*rotation_angle;
			}

			emergency = PIN((2.f - movement_approximate_weight)*0.5f - 0.5f, 0.f, 1.f);
			emergency = MAX(emergency, emergency_scale);
			direction_chosen = TRUE;
		}
		else
		{
			actor->control.vector_avoidance_sharp_turn_timer = NONE;
			if (forward_dot < 0.5f)
			{
				if (weight_difference > 1.3f)
				{
					real direction_dot = dot_product3d(
						&avoidance_directions[best_avoidance_direction],
						&local_movement_direction);

					if (direction_dot > 0.5f)
					{
						real rotation_angle;

						emergency = PIN(weight_difference/1.3f - 0.5f, 0.f, 1.f);
						emergency = MAX(emergency, emergency_scale);
						rotation_angle = emergency*(_pi/3.f);
						if (local_movement_direction.k*avoidance_directions[best_avoidance_direction].j -
							local_movement_direction.j*avoidance_directions[best_avoidance_direction].k > 0.f)
						{
							rotation_angle = -rotation_angle;
						}
						rotation.i = avoidance_data.forward.i*rotation_angle;
						rotation.j = avoidance_data.forward.j*rotation_angle;
						rotation.k = avoidance_data.forward.k*rotation_angle;
						direction_chosen = TRUE;
						debug_info->field_653C = 4;
						debug_info->sign_rotated = rotation_angle;
					}
					else
					{
						debug_info->sign_too_far_cosangle = direction_dot;
						debug_info->field_653C = 3;
					}
				}
				else
				{
					debug_info->field_653C = 2;
				}
			}
			else if (maximum_sense_emergency > 0.f)
			{
				real_vector3d perpendicular;
				real rotation_angle = 0.f;

				emergency = emergency_scale;

				/* perpendicular has no forward component, so only its left and up
				   components are accumulated (January has no forward-axis term and no
				   call to actor_move_transform_avoidance_vector at this site) */
				rotation = *global_zero_vector3d;
				perpendicular.j = -avoidance_directions[best_avoidance_direction].k;
				rotation.i += perpendicular.j*avoidance_data.left.i;
				rotation.j += perpendicular.j*avoidance_data.left.j;
				rotation.k += perpendicular.j*avoidance_data.left.k;
				perpendicular.k = avoidance_directions[best_avoidance_direction].j;
				rotation.i += perpendicular.k*avoidance_data.up.i;
				rotation.j += perpendicular.k*avoidance_data.up.j;
				rotation.k += perpendicular.k*avoidance_data.up.k;
				if (normalize3d(&rotation) > 0.f)
				{
					rotation_angle = emergency*(_pi/3.f);
					rotation.i *= rotation_angle;
					rotation.j *= rotation_angle;
					rotation.k *= rotation_angle;
				}
				debug_info->field_6520 = rotation_angle;
				direction_chosen = TRUE;
				debug_info->field_653C = 1;
				debug_info->field_651C = maximum_sense_emergency;
			}
			else
			{
				debug_info->field_653C = 0;
			}
		}

		debug_info->field_6550 = direction_chosen;
		if (direction_chosen)
			actor->control.vector_avoidance_current_direction = best_avoidance_direction;
		else
			actor->control.vector_avoidance_current_direction = NONE;
		debug_info->avoidance_data = avoidance_data;
		debug_info->field_654C = emergency;
		debug_info->field_6540 = rotation;
	}

	*avoidance_rotation = rotation;
	*emergency_amount = emergency;

	return;
}

boolean actor_path_refresh(
	long actor_index,
	boolean new_destination,
	struct path_state *cached_path_state)
{
	struct actor_datum *actor = actor_get(actor_index);
	short destination_type = actor->control.path.destination_orders.destination_type;
	boolean have_previous_destination = FALSE;
	boolean success = TRUE;
	real_point3d previous_destination;

	if (destination_type != _destination_none &&
		destination_type != _destination_halt)
	{
		have_previous_destination = TRUE;
		previous_destination = actor->control.path.destination.point;
	}

	if (actor->input.vehicle_passenger ||
		destination_type == _destination_none ||
		destination_type == _destination_halt ||
		(destination_type == _destination_firing_position &&
			actor->firing_positions.moved_away_from_firing_position))
	{
		actor_path_clear(actor_index);
	}
	else
	{
		actor->control.path.path.valid = FALSE;
		actor->control.path.at_destination = FALSE;
		actor->control.path.destination_original_distance = 0.f;
		actor->control.movement_complete = FALSE;

		switch (destination_type)
		{
		case _destination_raw_location:
			actor->control.path.destination.point =
				actor->control.path.destination_orders.raw.point;
			actor->control.path.destination.surface_index =
				actor->control.path.destination_orders.raw.surface_index;
			actor->control.path.destination.target_radius = 0.f;
			break;

		case _destination_move_position:
			success = FALSE;
			if (actor->meta.encounter_index != NONE)
			{
				struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
					struct encounter_definition);
				struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
					&encounter->squads,
					actor->meta.squad_index,
					struct squad_definition);
				short move_position_index = actor->control.path.destination_orders.move_position_index;

				if (move_position_index >= 0 &&
					move_position_index < squad->move_positions.count)
				{
					struct move_position_definition *move_position = TAG_BLOCK_GET_ELEMENT(
						&squad->move_positions,
						move_position_index,
						struct move_position_definition);

					success = TRUE;
					actor->control.path.destination.point = move_position->position;
					actor->control.path.destination.surface_index = move_position->surface_index;
					actor->control.path.destination.target_radius = 0.f;
				}
			}
			break;

		case _destination_firing_position:
			if (actor->meta.encounter_index == NONE)
			{
				success = FALSE;
			}
			else
			{
				struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
					struct encounter_definition);
				struct firing_position_definition *firing_position = TAG_BLOCK_GET_ELEMENT(
					&encounter->firing_positions,
					actor->control.path.destination_orders.firing_position_index,
					struct firing_position_definition);

				actor->control.path.destination.point = firing_position->position;
				actor->control.path.destination.surface_index = firing_position->surface_index;
				actor->control.path.destination.target_radius = 0.f;
			}
			break;

		case _destination_prop:
			{
				struct prop_datum *prop = prop_get(actor->control.path.destination_orders.prop.prop_index);

				if (prop->state < _prop_state_uninspected_orphan ||
					prop->state > _prop_state_inspected_orphan)
				{
					actor_perception_find_prop_pathfinding_location(
						actor_index,
						actor->control.path.destination_orders.prop.prop_index);
				}

				if (actor->state.flying)
					actor->control.path.destination.point = prop->center_of_mass;
				else
					actor->control.path.destination.point = prop->pathfinding_point;
				actor->control.path.destination.surface_index = prop->pathfinding_surface_index;
				actor->control.path.destination.target_radius =
					actor->control.path.destination_orders.prop.accept_radius;
			}
			break;

		default:
			success = FALSE;
			match_vassert(
				"c:\\halo\\SOURCE\\ai\\actor_moving.c",
				2943,
				FALSE,
				NULL);
			break;
		}

		if (success)
		{
			boolean path_available = TRUE;

			if (actor->state.flying)
			{
				real avoidance_distance;

				path_available = actor_path_3d_available(
					actor_index,
					&actor->control.path.destination.point,
					&avoidance_distance);
			}
			else if (actor->control.path.destination.target_radius == 0.f)
			{
				path_available = actor->control.path.destination.surface_index != NONE;
			}

			if (!path_available)
				success = FALSE;
		}

		if (success)
		{
			boolean build_path = TRUE;

			if (actor_test_destination(actor_index))
			{
				build_path = have_previous_destination &&
					distance_squared3d(
						&previous_destination,
						&actor->control.path.destination.point) > 0.1f*0.1f;
			}

			if (build_path)
			{
				struct actor_definition *definition =
					actor_definition_get(actor->meta.definition_index);
				real distance = distance3d(
					&actor->input.position.body_position,
					&actor->control.path.destination.point);
				struct actor_debug_info *debug_info =
					&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

				debug_info->last_path_refresh = game_time_get();

				if (actor->state.flying)
				{
					success = path_3d_build_path(
						global_structure_bsp_get(),
						&actor->input.position.body_position,
						0.f,
						&actor->control.path.destination.point,
						&actor->control.path.path);
				}
				else if (cached_path_state)
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\actor_moving.c",
						3004,
						actor->control.path.destination_orders.ignore_target_object_index == NONE);
					path_state_destination(
						cached_path_state,
						&actor->control.path.destination.point,
						actor->control.path.destination.surface_index,
						actor->control.path.destination.target_radius);
					success = path_state_build_path(
						cached_path_state,
						&actor->control.path.path);
				}
				else
				{
					struct path_input input;
					struct path_state path_state;

					actor_path_input_new(actor_index, &input);
					if (actor->control.path.destination_orders.ignore_target_object_index != NONE)
					{
						path_input_set_target_object(
							&input,
							actor->control.path.destination_orders.ignore_target_object_index);
					}
					if (actor->danger_zone.danger_type > _actor_danger_zone_none &&
						!actor->danger_zone.attached_to_us &&
						!TEST_FLAG(
							definition->flags2,
							_actor_definition_flags2_pathfinding_ignores_danger_bit))
					{
						path_input_set_attractor(
							&input,
							&actor->danger_zone.position,
							actor->danger_zone.danger_radius,
							actor->danger_zone.object_index,
							10.f);
					}
					path_state_new(
						&input,
						&path_state,
						ai_debug_get_path_storage(actor_index));
					path_state_destination(
						&path_state,
						&actor->control.path.destination.point,
						actor->control.path.destination.surface_index,
						actor->control.path.destination.target_radius);
					success = FALSE;
					if (path_state_find(&path_state) &&
						path_state_build_path(&path_state, &actor->control.path.path))
					{
						success = TRUE;
					}
				}

				actor->control.path.refreshed_this_tick = TRUE;
				if (new_destination)
					actor->control.path.destination_original_distance = distance;

				if (success &&
					actor->control.path.path.endpoint.target_radius > 0.f &&
					distance < actor->control.path.destination.target_radius &&
					distance - actor->control.path.path.endpoint.target_radius < 0.5f)
				{
					actor_path_clear(actor_index);
				}
			}
		}

		if (!success)
			actor_path_clear(actor_index);
	}

	return success;
}

void actor_destination_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->meta.timeslice &&
		!actor->control.path.refreshed_this_tick &&
		!actor->meta.dormant)
	{
		actor_path_refresh(actor_index, FALSE, NULL);
	}
	actor_test_destination(actor_index);

	if (actor->control.path.path.valid)
	{
		struct path_result *path = &actor->control.path.path;
		boolean final_step = FALSE;
		boolean step_reached;

		do
		{
			char step_index = path->step_index;

			step_reached = FALSE;
			if (step_index + 1 < path->step_count)
			{
				real_vector2d actor_to_point;
				real_vector2d next_step;

				vector_from_points2d(
					(real_point2d const *)&actor->input.position.body_position,
					(real_point2d const *)&path->steps[step_index].point,
					&actor_to_point);
				vector_from_points2d(
					(real_point2d const *)&path->steps[step_index].point,
					(real_point2d const *)&path->steps[step_index + 1].point,
					&next_step);

				if (actor->control.movement_complete)
				{
					step_reached = TRUE;
				}
				else if (actor->control.moving && actor->control.movement_thwarted)
				{
					real facing_dot = dot_product2d((real_vector2d const *)&actor->input.facing_vector, &next_step);
					real distance_along_step = dot_product2d(&actor_to_point, &next_step);

					if (facing_dot > 0.f && distance_along_step < 0.f)
					{
						point_from_line2d(
							(real_point2d const *)&actor_to_point,
							&next_step,
							-distance_along_step,
							(real_point2d *)&actor_to_point);
						step_reached = magnitude_squared2d(&actor_to_point) < 0.25f*0.25f;
					}
				}
				else
				{
					step_reached = magnitude_squared2d(&actor_to_point) < 0.15f*0.15f;
				}

				if (step_reached)
				{
					path->step_index = step_index + 1;
					actor->control.movement_complete = FALSE;
				}
			}
			else
			{
				final_step = TRUE;
			}
		}
		while (step_reached);

		if (actor->control.movement_complete)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_moving.c",
				180,
				final_step);

			if (actor->control.path.path.steps_finish_path)
			{
				actor_path_clear(actor_index);
			}
			else if (ai_debug.print_unfinished_paths)
			{
				char actor_description[512];

				ai_debug_describe_actor(
					actor_index,
					NONE,
					TRUE,
					actor_description,
					sizeof(actor_description));
				error(
					_error_silent,
					"%s: fell off end of unfinished path %d/%d",
					actor_description,
					actor->control.path.path.step_count,
					NUMBEROF(actor->control.path.path.steps));
			}
		}
	}

	if (actor->control.path.path.valid &&
		(actor->control.moving || !actor->control.path.at_destination))
	{
		actor->control.moving = TRUE;
		actor->control.moving_towards_point = actor->control.path.path.steps[actor->control.path.path.step_index].point;
		vector_from_points3d(
			&actor->input.position.body_position,
			&actor->control.moving_towards_point,
			&actor->control.moving_towards_vector);
		if (magnitude3d(&actor->control.moving_towards_vector) >= 1000000.f)
		{
			error(
				_error_silent,
				"pathfinding is attempting to walk to tau ceti");
			actor->control.path.path.valid = FALSE;
		}
	}
	else if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying)
	{
		boolean reverse =
			actor->control.vector_avoidance_rotation_emergency_instantaneous > 0.9f;
		real distance;

		actor->control.moving = TRUE;
		actor->control.movement_complete = FALSE;
		distance = (real)(reverse ? -1 : 1) * 3.f;
		scale_vector3d(
			&actor->input.facing_vector,
			distance,
			&actor->control.moving_towards_vector);
		add_vectors3d(
			(real_vector3d const *)&actor->input.position.body_position,
			&actor->control.moving_towards_vector,
			(real_vector3d *)&actor->control.moving_towards_point);
	}
	else
	{
		actor->control.moving = FALSE;
		actor->control.movement_complete = FALSE;
		actor->control.path.at_destination = TRUE;
		actor_path_clear(actor_index);
	}

	return;
}

boolean actor_move_to_point(
	long actor_index,
	real_point3d const *destination,
	long surface_index,
	long ignore_target_object_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		951,
		destination);

	actor->firing_positions.current_position_index = NONE;
	actor_set_dormant(actor_index, FALSE);

	if (actor->control.path.destination_orders.destination_type != _destination_raw_location ||
		actor->control.path.destination_orders.raw.surface_index != surface_index ||
		distance_squared3d(destination, &actor->control.path.destination_orders.raw.point) > 0.1f*0.1f)
	{
		actor->orders.move.destination.destination_type = _destination_raw_location;
		actor->orders.move.destination.keep_moving = FALSE;
		actor->orders.move.destination.raw.point = *destination;
		actor->orders.move.destination.raw.surface_index = surface_index;
		actor->orders.move.destination.ignore_target_object_index = ignore_target_object_index;
		actor->control.path.destination_orders = actor->orders.move.destination;

		result = actor_path_refresh(actor_index, TRUE, NULL);
	}
	else if (actor->meta.timeslice && !actor->control.path.refreshed_this_tick)
	{
		result = actor_path_refresh(actor_index, FALSE, NULL);
	}

	return result;
}

boolean actor_move_to_move_position(
	long actor_index,
	short move_position_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	actor->firing_positions.current_position_index = NONE;
	actor_set_dormant(actor_index, FALSE);

	if (actor->control.path.destination_orders.destination_type != _destination_move_position ||
		actor->control.path.destination_orders.move_position_index != move_position_index)
	{
		actor->orders.move.destination.destination_type = _destination_move_position;
		actor->orders.move.destination.keep_moving = FALSE;
		actor->orders.move.destination.move_position_index = move_position_index;
		actor->orders.move.destination.ignore_target_object_index = NONE;
		actor->control.path.destination_orders = actor->orders.move.destination;

		result = actor_path_refresh(actor_index, TRUE, NULL);
	}
	else if (actor->meta.timeslice && !actor->control.path.refreshed_this_tick)
	{
		result = actor_path_refresh(actor_index, FALSE, NULL);
	}

	return result;
}

boolean actor_move_to_firing_position(
	long actor_index,
	short firing_position_index,
	struct path_state *cached_path_state)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	actor_set_dormant(actor_index, FALSE);

	if (actor->control.path.destination_orders.destination_type != _destination_firing_position ||
		actor->control.path.destination_orders.firing_position_index != firing_position_index)
	{
		actor->orders.move.destination.destination_type = _destination_firing_position;
		actor->orders.move.destination.keep_moving = FALSE;
		actor->orders.move.destination.firing_position_index = firing_position_index;
		actor->orders.move.destination.ignore_target_object_index = NONE;
		actor->firing_positions.moved_away_from_firing_position = FALSE;
		actor->control.path.destination_orders = actor->orders.move.destination;

		result = actor_path_refresh(actor_index, TRUE, cached_path_state);
	}
	else if (actor->meta.timeslice && !actor->control.path.refreshed_this_tick)
	{
		result = actor_path_refresh(actor_index, FALSE, cached_path_state);
	}

	return result;
}

boolean actor_move_to_prop(
	long actor_index,
	long prop_index,
	real accept_radius)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	actor->firing_positions.current_position_index = NONE;
	actor_set_dormant(actor_index, FALSE);

	if (actor->control.path.destination_orders.destination_type != _destination_prop ||
		actor->control.path.destination_orders.prop.prop_index != prop_index ||
		actor->control.path.destination_orders.prop.accept_radius != accept_radius)
	{
		struct prop_datum *prop = prop_get(prop_index);

		actor->orders.move.destination.destination_type = _destination_prop;
		actor->orders.move.destination.keep_moving = FALSE;
		actor->orders.move.destination.prop.prop_index = prop_index;
		actor->orders.move.destination.prop.accept_radius = accept_radius;
		actor->orders.move.destination.ignore_target_object_index =
			prop->vehicle_index == NONE ? prop->unit_index : prop->vehicle_index;
		actor->control.path.destination_orders = actor->orders.move.destination;

		result = actor_path_refresh(actor_index, TRUE, NULL);
	}
	else if (actor->meta.timeslice && !actor->control.path.refreshed_this_tick)
	{
		result = actor_path_refresh(actor_index, FALSE, NULL);
	}

	return result;
}

static void actor_move_calculate_movement(
	long actor_index,
	boolean move_in_3d,
	short override_facing,
	boolean free_movement,
	real free_movement_distance_squared,
	boolean allow_all_moving_turns,
	real steering_maximum_angle,
	real oversteer_minimum_angle,
	real oversteer_maximum_angle,
	real rotation_emergency_amount,
	real maximum_throttle,
	real_vector3d const *desired_movement_vector,
	real_vector3d *desired_facing_vector,
	short *desired_facing_direction,
	real_vector3d *desired_throttle,
	boolean *movement_thwarted,
	boolean *movement_complete)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	real_vector3d facing_vector;
	real_vector3d throttle;
	real_vector3d free_throttle;
	real minimum_facing_dot = 0.8660254f;
	real facing_dot;
	real movement_distance_squared;
	real current_stopping_distance;
	real maximum_stopping_distance;
	short facing_direction = NONE;
	boolean face_actor_facing;
	boolean facing_allows_movement;

	if (actor->orders.move.move_face_exactly)
		actor->control.face_exactly = TRUE;

	if (override_facing >= _actor_facing_forward && override_facing <= _actor_facing_right)
	{
		real_vector3d movement_vector;

		facing_direction = override_facing;
		movement_vector = *desired_movement_vector;
		if (!move_in_3d)
			movement_vector.k = 0.f;
		if (normalize3d(&movement_vector) == 0.f)
			movement_vector = actor->input.facing_vector;

		switch (override_facing)
		{
		case _actor_facing_forward:
			facing_vector = movement_vector;
			break;

		case _actor_facing_backward:
			set_real_vector3d(
				&facing_vector,
				-movement_vector.i,
				-movement_vector.j,
				movement_vector.k);
			break;

		case _actor_facing_left:
			set_real_vector3d(
				&facing_vector,
				-movement_vector.j,
				movement_vector.i,
				movement_vector.k);
			break;

		case _actor_facing_right:
			set_real_vector3d(
				&facing_vector,
				movement_vector.j,
				-movement_vector.i,
				movement_vector.k);
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_moving.c",
				1432,
				!"unreachable");
			break;
		}

		if (free_movement)
		{
			actor_move_calculate_free(move_in_3d, &movement_vector, &facing_vector, &free_throttle);
			facing_direction = _actor_facing_free;
		}
	}
	else
	{
		movement_distance_squared = magnitude_squared3d(desired_movement_vector);
		if (movement_distance_squared > 0.8f*0.8f)
			minimum_facing_dot = definition->moving.runtime_begin_movement_angle_cosine;

		if (free_movement && movement_distance_squared < free_movement_distance_squared)
		{
			real_vector3d movement_vector = *desired_movement_vector;
			real_vector3d actor_facing_vector;
			real_vector3d const *free_facing_vector;

			face_actor_facing = FALSE;

			if (actor->control.moving_forced_by_aiming)
			{
				facing_vector = actor->control.moving_forced_aim_direction;
				if (actor->input.vehicle_driver_type > _actor_vehicle_driver_none)
					face_actor_facing = TRUE;
			}
			else
			{
				facing_vector = actor->input.facing_vector;
			}

			if (!move_in_3d)
			{
				movement_vector.k = 0.f;
				facing_vector.k = 0.f;
			}
			if (normalize3d(&facing_vector) == 0.f)
				facing_vector = actor->input.facing_vector;
			if (normalize3d(&movement_vector) == 0.f)
				movement_vector = facing_vector;

			if (face_actor_facing)
			{
				actor_facing_vector = actor->input.facing_vector;
				if (!move_in_3d)
					actor_facing_vector.k = 0.f;
				if (normalize3d(&actor_facing_vector) == 0.f)
					actor_facing_vector = facing_vector;
				free_facing_vector = &actor_facing_vector;
			}
			else
			{
				free_facing_vector = &facing_vector;
			}
			actor_move_calculate_free(move_in_3d, &movement_vector, free_facing_vector, &free_throttle);
			facing_direction = _actor_facing_free;
		}
		else if (actor->control.moving_forced_by_aiming)
		{
			actor_move_calculate_controlled_by_aiming(
				move_in_3d,
				desired_movement_vector,
				&actor->control.moving_forced_aim_direction,
				&actor->input.facing_vector,
				&facing_vector,
				&facing_direction);
		}
		else
		{
			facing_vector = *desired_movement_vector;
			if (!move_in_3d)
				facing_vector.k = 0.f;
			if (normalize3d(&facing_vector) == 0.f)
				facing_vector = actor->input.facing_vector;
			facing_direction = _actor_facing_forward;
		}
	}

	match_assert_valid_real_normal3d(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		1532,
		&facing_vector);
	facing_dot = dot_product3d(&facing_vector, &actor->input.facing_vector);

	if (allow_all_moving_turns ||
		actor->output.movement_type == _actor_movement_type_flaming)
	{
		facing_allows_movement = TRUE;
	}
	else
	{
		if (!actor->state.flying)
		{
			long pathfinding_surface_index;

			actor_find_pathfinding_location(actor_index);
			pathfinding_surface_index = actor->input.pathfinding_surface_index;
			if (pathfinding_surface_index != NONE)
			{
				real_vector3d movement_direction;
				boolean test_movement_direction = TRUE;

				switch (facing_direction)
				{
				case _actor_facing_forward:
					set_real_vector3d(
						&movement_direction,
						actor->input.facing_vector.i,
						actor->input.facing_vector.j,
						actor->input.facing_vector.k);
					break;

				case _actor_facing_backward:
					set_real_vector3d(
						&movement_direction,
						-actor->input.facing_vector.i,
						-actor->input.facing_vector.j,
						actor->input.facing_vector.k);
					break;

				case _actor_facing_left:
					set_real_vector3d(
						&movement_direction,
						actor->input.facing_vector.j,
						-actor->input.facing_vector.i,
						actor->input.facing_vector.k);
					break;

				case _actor_facing_right:
					set_real_vector3d(
						&movement_direction,
						-actor->input.facing_vector.j,
						actor->input.facing_vector.i,
						actor->input.facing_vector.k);
					break;

				default:
					test_movement_direction = FALSE;
					break;
				}

				if (test_movement_direction)
				{
					if (normalize2d((real_vector2d *)&movement_direction) > 0.f)
					{
						struct path_collision_result collision;
						real_point3d test_point;

						movement_direction.k = 0.f;
						point_from_line3d(
							&actor->input.position.body_position,
							&movement_direction,
							0.4f,
							&test_point);
						if (structure_test_line2d(
							global_structure_bsp_get(),
							actor->emotions.ignorant_of_broken_surfaces,
							(real_point2d const *)&actor->input.position.body_position,
							pathfinding_surface_index,
							(real_point2d const *)&test_point,
							NONE,
							&collision))
						{
							minimum_facing_dot = MAX(minimum_facing_dot, 0.95f);
						}
					}
				}
			}
		}

		facing_allows_movement = facing_dot > minimum_facing_dot;
	}

	{
		real destination_tolerance = actor_destination_tolerance(actor_index);

		movement_distance_squared = magnitude_squared3d(desired_movement_vector);
		*movement_complete =
			movement_distance_squared < destination_tolerance*destination_tolerance;
	}

	actor_get_stopping_distances(
		actor_index,
		&current_stopping_distance,
		&maximum_stopping_distance);
	if (!actor->control.path.destination_orders.keep_moving &&
		movement_distance_squared < current_stopping_distance*current_stopping_distance)
	{
		real movement_distance = square_root(movement_distance_squared);

		if (movement_distance > maximum_stopping_distance + 0.05f &&
			current_stopping_distance > maximum_stopping_distance)
		{
			maximum_throttle = MIN(
				maximum_throttle,
				(movement_distance - maximum_stopping_distance) /
					(current_stopping_distance - maximum_stopping_distance));
		}
		else
		{
			maximum_throttle = 0.f;
		}
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1634,
			(maximum_throttle >= 0.0f) && (maximum_throttle <= 1.0f));
	}

	throttle = *global_zero_vector3d;
	if (facing_allows_movement)
	{
		switch (facing_direction)
		{
		case _actor_facing_forward:
			throttle.i = 1.f;
			break;

		case _actor_facing_backward:
			throttle.i = -1.f;
			break;

		case _actor_facing_left:
			throttle.j = -1.f;
			break;

		case _actor_facing_right:
			throttle.j = 1.f;
			break;

		case _actor_facing_free:
			throttle = free_throttle;
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\ai\\actor_moving.c",
				1649,
				FALSE,
				NULL);
			break;
		}

		throttle.i *= maximum_throttle;
		throttle.j *= maximum_throttle;
		throttle.k *= maximum_throttle;
		*movement_thwarted = FALSE;
	}
	else
	{
		actor->control.face_exactly = TRUE;
		*movement_thwarted = TRUE;
	}

	if (game_connection() == _game_connection_local && ai_debug.oversteer_disable)
	{
		oversteer_maximum_angle = 0.f;
		oversteer_minimum_angle = 0.f;
	}

	if (steering_maximum_angle > 0.f || oversteer_maximum_angle > 0.f)
	{
		real angle;
		real steering_angle;
		real angle_adjustment;

		if (facing_dot >= 1.0f)
		{
			angle = 0.f;
		}
		else if (facing_dot <= -1.0f)
		{
			angle = _pi;
		}
		else
		{
			angle = arccosine(facing_dot);
		}

		steering_angle = angle;
		if (steering_maximum_angle > 0.f)
		{
			real minimum_steering_angle = steering_maximum_angle*rotation_emergency_amount;
			real maximum_steering_angle = steering_maximum_angle;

			if (rotation_emergency_amount > 1.0f)
				maximum_steering_angle = MIN(rotation_emergency_amount, 1.5f)*steering_maximum_angle;
			minimum_steering_angle = MIN(angle*3.0f, minimum_steering_angle);
			steering_angle = PIN(steering_angle, minimum_steering_angle, maximum_steering_angle);
		}

		if (steering_angle > actor->control.face_exactly_oversteer_angle)
		{
			if (actor->control.face_exactly && steering_angle > oversteer_minimum_angle)
			{
				actor->control.face_exactly_oversteer_angle = MIN(steering_angle, oversteer_maximum_angle);
				if (ai_debug.print_oversteer)
				{
					console_printf(
						FALSE,
						"steer %.4f (set oversteer %.4f)",
						steering_angle,
						actor->control.face_exactly_oversteer_angle);
				}
			}
			else if (ai_debug.print_oversteer)
			{
				console_printf(FALSE, "steer %.4f", steering_angle);
			}
		}
		else if (actor->control.face_exactly_oversteer_angle > 0.f)
		{
			if (steering_angle < oversteer_minimum_angle)
			{
				if (ai_debug.print_oversteer)
				{
					console_printf(
						FALSE,
						"steer %.4f < %.4f - clear oversteer %.4f",
						steering_angle,
						oversteer_minimum_angle,
						actor->control.face_exactly_oversteer_angle);
				}
				actor->control.face_exactly_oversteer_angle = 0.f;
			}
			else
			{
				if (ai_debug.print_oversteer)
				{
					console_printf(
						FALSE,
						"steer %.4f - oversteer to %.4f",
						steering_angle,
						actor->control.face_exactly_oversteer_angle);
				}
				steering_angle = actor->control.face_exactly_oversteer_angle;
			}
		}

		angle_adjustment = steering_angle - angle;
		if (fabs(angle_adjustment) > _real_epsilon)
		{
			real_vector3d rotation_axis;

			rotation_axis.i = actor->input.facing_vector.j*facing_vector.k - actor->input.facing_vector.k*facing_vector.j;
			rotation_axis.j = actor->input.facing_vector.k*facing_vector.i - actor->input.facing_vector.i*facing_vector.k;
			rotation_axis.k = actor->input.facing_vector.i*facing_vector.j - actor->input.facing_vector.j*facing_vector.i;
			if (normalize3d(&rotation_axis) > 0.f)
			{
				rotate_vector_about_axis(
					&facing_vector,
					&rotation_axis,
					sine(angle_adjustment),
					cosine(angle_adjustment));
				if (ai_debug.print_oversteer)
				{
					console_printf(
						FALSE,
						"adjust angle %.4f -> %.4f (%.4f)",
						angle,
						steering_angle,
						angle_adjustment);
				}
			}
		}
	}

	if (!actor->control.moving_forced_by_aiming &&
		actor->orders.move.override_movement_facing == NONE)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1777,
			(facing_direction == _actor_facing_forward) || (facing_direction == _actor_facing_free));
	}

	*desired_facing_direction = facing_direction;
	*desired_facing_vector = facing_vector;
	*desired_throttle = throttle;

	return;
}

static void actor_move_calculate_controlled_by_aiming(
	boolean move_in_3d,
	real_vector3d const *movement_vector,
	real_vector3d const *forced_aim_direction,
	real_vector3d const *current_facing_vector,
	real_vector3d *desired_facing_vector,
	short *desired_facing_direction)
{
	real_vector3d directions[_actor_facing_free];
	short best_direction;
	real best_aim_dot;
	real best_facing_dot;
	short direction;

	if (move_in_3d)
	{
		directions[0] = *movement_vector;
		if (normalize3d(&directions[0]) == 0.f)
			directions[0] = *current_facing_vector;
		directions[2] = *global_zero_vector3d;
	}
	else
	{
		directions[0] = *movement_vector;
		directions[0].k = 0.f;
		if (normalize3d(&directions[0]) == 0.f)
			directions[0] = *current_facing_vector;
		directions[2].i = -directions[0].j;
		directions[2].j = directions[0].i;
		directions[2].k = 0.f;
	}
	directions[1].i = -directions[0].i;
	directions[1].j = -directions[0].j;
	directions[1].k = -directions[0].k;
	directions[3].i = -directions[2].i;
	directions[3].j = -directions[2].j;
	directions[3].k = -directions[2].k;

	best_direction = NONE;
	for (direction = _actor_facing_forward; direction < _actor_facing_free; direction++)
	{
		real_vector3d const *direction_vector = &directions[direction];
		real aim_dot;
		real facing_dot;

		if (move_in_3d)
		{
			aim_dot = dot_product3d(direction_vector, forced_aim_direction);
			facing_dot = dot_product3d(direction_vector, current_facing_vector);
		}
		else
		{
			aim_dot = direction_vector->i*forced_aim_direction->i + direction_vector->j*forced_aim_direction->j;
			facing_dot = direction_vector->i*current_facing_vector->i + direction_vector->j*current_facing_vector->j;
		}

		if (best_direction == NONE ||
			(aim_dot > best_aim_dot ?
				(facing_dot > best_facing_dot || best_facing_dot < 0.5f) :
				(facing_dot > best_facing_dot && aim_dot > 0.5f)))
		{
			best_direction = direction;
			best_aim_dot = aim_dot;
			best_facing_dot = facing_dot;
		}
	}

	*desired_facing_direction = best_direction;
	*desired_facing_vector = directions[best_direction];
	match_assert_valid_real_normal3d(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		1892,
		desired_facing_vector);

	return;
}

static void actor_move_calculate_free(
	boolean move_in_3d,
	real_vector3d const *movement_direction,
	real_vector3d const *facing_direction,
	real_vector3d *throttle)
{
	if (move_in_3d)
	{
		real_vector3d left;
		real_vector3d up;

		match_assert_valid_real_normal3d(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1909,
			movement_direction);
		match_assert_valid_real_normal3d(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1910,
			facing_direction);

		biped_build_flying_axes(facing_direction, &left, &up);
		throttle->i = dot_product3d(movement_direction, facing_direction);
		throttle->j = dot_product3d(movement_direction, &left);
		throttle->k = dot_product3d(movement_direction, &up);
		normalize3d(throttle);
	}
	else
	{
		real_vector2d perpendicular;

		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1925,
			valid_real_normal2d((real_vector2d const *)movement_direction),
			csprintf(
				temporary,
				"%s: assert_valid_real_normal2d(%f, %f)",
				"(real_vector2d *) movement_direction",
				movement_direction->i,
				movement_direction->j));
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1926,
			valid_real_normal2d((real_vector2d const *)facing_direction),
			csprintf(
				temporary,
				"%s: assert_valid_real_normal2d(%f, %f)",
				"(real_vector2d *) facing_direction",
				facing_direction->i,
				facing_direction->j));
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1927,
			valid_realcmp(movement_direction->k, 0.0f),
			csprintf(
				temporary,
				"%s, %s: assert_valid_realcmp(%f, %f)",
				"movement_direction->k",
				"0.0f",
				movement_direction->k,
				0.0f));
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_moving.c",
			1928,
			valid_realcmp(facing_direction->k, 0.0f),
			csprintf(
				temporary,
				"%s, %s: assert_valid_realcmp(%f, %f)",
				"facing_direction->k",
				"0.0f",
				facing_direction->k,
				0.0f));

		perpendicular.i = -facing_direction->j;
		perpendicular.j = facing_direction->i;
		throttle->i = movement_direction->i*facing_direction->i + movement_direction->j*facing_direction->j;
		throttle->j = movement_direction->i*perpendicular.i + movement_direction->j*perpendicular.j;
		throttle->k = 0.f;
		normalize3d(throttle);
	}

	return;
}

void actor_move_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	short movement_type;
	short override_facing;
	boolean free_movement = FALSE;
	boolean move_in_3d = FALSE;
	boolean allow_all_moving_turns = FALSE;
	boolean clear_firing_positions = FALSE;
	boolean force_stationary_facing = FALSE;
	boolean allow_jump = FALSE;
	boolean crouch;
	real free_movement_distance_squared = 0.f;
	real steering_maximum_angle = 0.f;
	real oversteer_maximum_angle = 0.f;
	real oversteer_minimum_angle = 0.f;
	real rotation_emergency_amount = 0.f;
	real maximum_throttle = 1.f;

	match_assert_valid_real_normal3d(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		286,
		&actor->input.facing_vector);

	actor->control.desired_facing_vector = actor->input.facing_vector;
	actor->control.face_exactly = FALSE;
	actor->control.free_facing_vector = TRUE;
	actor->control.optional_facing_vector = TRUE;

	if (actor->orders.move.override_movement_direction)
	{
		actor->control.moving = TRUE;
		actor->control.free_facing_vector = FALSE;
		actor->control.moving_towards_vector =
			actor->orders.move.override_movement_direction_vector;
		actor->control.vector_avoidance_rotation = *global_zero_vector3d;
		actor->control.vector_avoidance_rotation_emergency = 0.f;
		actor->control.vector_avoidance_rotation_emergency_instantaneous = 0.f;
	}
	else if (actor->input.vehicle_driver_type ==
		_actor_vehicle_driver_directional_flying)
	{
		real_vector3d scaled_facing;
		real_vector3d avoidance_rotation;
		real_vector3d const *movement_direction;
		real emergency_amount;
		real rotation_magnitude_squared;
		real previous_magnitude_squared;
		real blend;

		if (actor->control.moving)
		{
			movement_direction = &actor->control.moving_towards_vector;
		}
		else
		{
			scale_vector3d(
				&actor->input.facing_vector,
				3.f,
				&scaled_facing);
			movement_direction = &scaled_facing;
		}

		actor_move_vector_avoidance(
			actor_index,
			movement_direction,
			&avoidance_rotation,
			&emergency_amount);

		rotation_magnitude_squared = magnitude_squared3d(&avoidance_rotation);
		previous_magnitude_squared =
			magnitude_squared3d(&actor->control.vector_avoidance_rotation);
		blend = rotation_magnitude_squared > previous_magnitude_squared
			? 0.3f
			: 0.05f;

		scale_vector3d(
			&actor->control.vector_avoidance_rotation,
			1.f - blend,
			&actor->control.vector_avoidance_rotation);
		actor->control.vector_avoidance_rotation.i += avoidance_rotation.i * blend;
		actor->control.vector_avoidance_rotation.j += avoidance_rotation.j * blend;
		actor->control.vector_avoidance_rotation.k += avoidance_rotation.k * blend;

		if (magnitude_squared3d(&actor->control.vector_avoidance_rotation) <
			_real_epsilon)
		{
			actor->control.vector_avoidance_rotation = *global_zero_vector3d;
		}

		actor->control.vector_avoidance_rotation_emergency_instantaneous =
			emergency_amount;
		actor->control.vector_avoidance_rotation_emergency =
			actor->control.vector_avoidance_rotation_emergency * (1.f - blend) +
			emergency_amount * blend;
		if (actor->control.vector_avoidance_rotation_emergency < 0.001f)
			actor->control.vector_avoidance_rotation_emergency = 0.f;

		if (actor->control.moving)
		{
			real_vector3d rotation =
				actor->control.vector_avoidance_rotation;
			real angle_squared = magnitude_squared3d(&rotation);

			if (angle_squared > _real_epsilon)
			{
				real angle = square_root(angle_squared);
				real inverse_angle = 1.f / angle;

				scale_vector3d(&rotation, inverse_angle, &rotation);
				rotate_vector_about_axis(
					&actor->control.moving_towards_vector,
					&rotation,
					sine(angle),
					cosine(angle));
			}

			rotation_emergency_amount =
				actor->control.vector_avoidance_rotation_emergency;
		}
	}

	movement_type = actor->orders.move.override_movement_type;
	if (movement_type == NONE)
	{
		movement_type = _actor_movement_type_combat;
		if (actor->orders.move.flaming)
		{
			movement_type = _actor_movement_type_flaming;
		}
		else if (actor->orders.move.panicked)
		{
			movement_type = _actor_movement_type_panic;
		}
		else
		{
			switch (actor->state.mode)
			{
			case _actor_mode_asleep:
				movement_type = _actor_movement_type_asleep;
				break;

			case _actor_mode_alert:
				movement_type = _actor_movement_type_alert;
				break;

			case _actor_mode_combat:
				movement_type = _actor_movement_type_combat;
				break;
			}
		}
	}
	actor->output.movement_type = movement_type;

	override_facing = actor->orders.move.override_movement_facing;
	if (actor_path_has_path(actor_index) &&
		actor->control.path.destination_original_distance >=
			definition->moving.stationary_moving_distance)
	{
		crouch = actor->orders.move.moving_crouch;
	}
	else
	{
		crouch = actor->orders.move.stationary_crouch;
	}

	if (actor->input.vehicle_driver_type > _actor_vehicle_driver_none)
	{
		struct vehicle_datum *vehicle =
			vehicle_datum_get(actor->input.vehicle_index);
		struct vehicle_definition *vehicle_definition =
			vehicle_specific_definition_get(vehicle->definition_index);

		steering_maximum_angle =
			vehicle_definition->ai_steering_max_angle;
		if (vehicle_definition->ai_steering_max_throttle > 0.f)
		{
			maximum_throttle =
				vehicle_definition->ai_steering_max_throttle;
		}
		oversteer_minimum_angle =
			vehicle_definition->ai_oversteer_angle_lower_bound;
		oversteer_maximum_angle =
			vehicle_definition->ai_oversteer_angle_upper_bound;

		switch (actor->input.vehicle_driver_type)
		{
		case _actor_vehicle_driver_hovering_ground:
			if (vehicle->vehicle.airborne_ticks > 0)
			{
				allow_jump = TRUE;
				actor->control.moving = FALSE;
				actor->control.free_facing_vector = TRUE;
				crouch = FALSE;
				break;
			}

			if (vehicle->vehicle.hover < 0.7f)
			{
				allow_jump = TRUE;
				if (vehicle->object.up.k < 0.8f)
				{
					real_vector3d escape_direction;

					escape_direction = vehicle->object.up;
					escape_direction.k = 0.f;
					if (normalize3d(&escape_direction) > 0.f)
					{
						actor->control.moving = TRUE;
						scale_vector3d(
							&escape_direction,
							3.f,
							&actor->control.moving_towards_vector);
					}
					else
					{
						actor->control.moving = FALSE;
					}
					crouch = FALSE;
					break;
				}
			}

			/* fall through */

		case _actor_vehicle_driver_nondirectional_ground:
			free_movement = TRUE;
			free_movement_distance_squared =
				vehicle_definition->ai_sideslip_distance *
				vehicle_definition->ai_sideslip_distance;
			crouch = FALSE;
			break;

		case _actor_vehicle_driver_directional_flying:
			{
				real_vector3d stuck_direction;

				if (vehicle_stuck(
					actor->input.vehicle_index,
					&stuck_direction))
				{
					actor->control.moving = FALSE;
					actor->control.free_facing_vector = FALSE;
					actor->control.optional_facing_vector = FALSE;
					actor->control.desired_facing_vector.i =
						-stuck_direction.i;
					actor->control.desired_facing_vector.j =
						-stuck_direction.j;
					actor->control.desired_facing_vector.k =
						-stuck_direction.k;
				}
				else
				{
					override_facing = 0;
					move_in_3d = TRUE;
					allow_all_moving_turns = TRUE;
				}
				crouch = FALSE;
			}
			break;

		default:
			actor->control.moving = FALSE;
			actor->control.moving_facing_direction = 0;
			actor->control.free_facing_vector =
				actor->meta.type == _actor_mounted_weapon ||
				actor->input.vehicle_gunner;
			crouch = FALSE;
			break;
		}
	}
	else if (actor->input.vehicle_passenger)
	{
		actor->control.moving = FALSE;
		actor->control.moving_facing_direction = 0;
		actor->control.free_facing_vector =
			actor->meta.type == _actor_mounted_weapon ||
			actor->input.vehicle_gunner;
		actor->control.optional_facing_vector = FALSE;
		crouch = FALSE;
	}
	else if (actor->orders.move.animation.impulse != NONE)
	{
		actor->control.moving = FALSE;
		actor->control.free_facing_vector = FALSE;
		actor->control.optional_facing_vector = FALSE;
		crouch = FALSE;
	}
	else if (actor->output.movement_type == _actor_movement_type_asleep)
	{
		actor->control.moving = FALSE;
		actor->control.free_facing_vector = FALSE;
		actor->control.optional_facing_vector = FALSE;
		force_stationary_facing = TRUE;
		crouch = FALSE;
	}
	else if (actor->input.in_midair && !actor->state.flying)
	{
		actor->control.moving = FALSE;
		actor->control.free_facing_vector = TRUE;
		crouch = FALSE;
	}
	else if (actor->control.grenade_trying_to_throw)
	{
		real_vector3d grenade_direction;

		actor->control.moving = FALSE;
		vector_from_points3d(
			&actor->input.position.body_position,
			&actor->control.grenade_current_target,
			&grenade_direction);
		crouch = FALSE;
		if (normalize3d(&grenade_direction) != 0.f)
		{
			actor->control.desired_facing_vector = grenade_direction;
			actor->control.free_facing_vector = FALSE;
			actor->control.optional_facing_vector = FALSE;
			actor->control.face_exactly = TRUE;
		}
		else
		{
			actor->control.free_facing_vector = TRUE;
		}
	}
	else if (actor->emotions.moving_into_fire_timer > 0)
	{
		actor->control.moving = FALSE;
		actor->control.free_facing_vector = TRUE;
		crouch = TEST_FLAG(
			definition->flags,
			_actor_definition_crouch_in_line_of_fire_bit);
	}
	else
	{
		clear_firing_positions = TRUE;
		if (actor->output.movement_type != _actor_movement_type_combat ||
			TEST_FLAG(
				definition->flags,
				crouch
					? _actor_definition_crouching_must_move_forward_bit
					: _actor_definition_standing_must_move_forward_bit))
		{
			actor->control.moving_forced_by_aiming = FALSE;
		}

		if (actor->output.movement_type == _actor_movement_type_flaming)
			force_stationary_facing = TRUE;

		if (TEST_FLAG(definition->flags, _actor_definition_flying_bit))
		{
			move_in_3d = TRUE;
			free_movement = TRUE;
			free_movement_distance_squared =
				definition->moving.free_flying_sidestep_distance *
				definition->moving.free_flying_sidestep_distance;
			if (actor->control.moving_forced_by_aiming)
				free_movement_distance_squared *= 4.f;
		}
	}

	if (actor->control.moving && !actor->control.movement_complete)
	{
		actor_move_calculate_movement(
			actor_index,
			move_in_3d,
			override_facing,
			free_movement,
			free_movement_distance_squared,
			allow_all_moving_turns,
			steering_maximum_angle,
			oversteer_minimum_angle,
			oversteer_maximum_angle,
			rotation_emergency_amount,
			maximum_throttle,
			&actor->control.moving_towards_vector,
			&actor->control.desired_facing_vector,
			&actor->control.moving_facing_direction,
			&actor->output.throttle,
			&actor->control.movement_thwarted,
			&actor->control.movement_complete);
		if (actor->control.movement_complete)
			actor->control.moving = FALSE;
	}

	if (actor->control.moving)
	{
		actor->control.optional_facing_vector = FALSE;
		actor->control.free_facing_vector = FALSE;
	}
	else if (force_stationary_facing)
	{
		actor->control.desired_facing_vector = actor->input.facing_vector;
		actor->control.moving_facing_direction = 0;
		actor->control.optional_facing_vector = FALSE;
		actor->control.free_facing_vector = FALSE;
	}
	else if (actor->control.fixed_stationary_facing)
	{
		actor->control.desired_facing_vector =
			actor->control.fixed_stationary_facing_vector;
		actor->control.moving_facing_direction = 0;
		actor->control.optional_facing_vector = TRUE;
		actor->control.free_facing_vector = FALSE;
	}

	match_assert_valid_real_normal3d(
		"c:\\halo\\SOURCE\\ai\\actor_moving.c",
		655,
		&actor->control.desired_facing_vector);

	if (clear_firing_positions && !actor->control.moving)
		actor_clear_discarded_firing_positions(actor_index, TRUE);

	if (actor->control.moving &&
		TEST_FLAG(
			definition->flags,
			_actor_definition_cannot_move_crouching_bit))
	{
		crouch = FALSE;
	}

	if (game_connection() == _game_connection_local && ai_debug.force_crouch)
		crouch = TRUE;

	actor->control.desire_stationary_facing = FALSE;
	if (crouch &&
		TEST_FLAG(
			definition->flags,
			_actor_definition_fixed_crouch_facing_bit))
	{
		actor->control.desire_stationary_facing = TRUE;
	}
	actor->control.crouching = crouch;
	actor_unit_control_crouch(actor_index, crouch);

	if (!actor_move_animation_busy(actor_index) &&
		actor->input.vehicle_index == NONE &&
		!actor->input.in_midair &&
		actor->emotions.berserk &&
		!actor->emotions.played_berserk_sound)
	{
		long cause_unit_index = NONE;
		real_vector2d alignment;

		alignment.i = actor->input.facing_vector.i;
		alignment.j = actor->input.facing_vector.j;
		if (actor->target.target_prop_index != NONE)
		{
			struct prop_datum *prop =
				prop_get(actor->target.target_prop_index);

			cause_unit_index = prop->unit_index;
			alignment.i = prop->actor_to_prop.i;
			alignment.j = prop->actor_to_prop.j;
			if (normalize2d(&alignment) == 0.f)
			{
				alignment.i = actor->input.facing_vector.i;
				alignment.j = actor->input.facing_vector.j;
			}
		}

		actor_move_animation_impulse(actor_index, 0, &alignment);
		ai_communication_event(
			_ai_communication_berserk,
			actor->meta.unit_index,
			cause_unit_index,
			_comm_hostility_enemy,
			NONE,
			NONE,
			NULL);
		actor->emotions.played_berserk_sound = TRUE;
	}

	if (allow_jump)
	{
		actor_unit_control_jump(actor_index);
	}
	else if (actor->input.in_midair || actor->input.vehicle_index != NONE)
	{
		actor->control.jumping_targeted = FALSE;
	}
	else if (!actor_move_animation_busy(actor_index) &&
		actor->orders.move.jump)
	{
		boolean leaped = FALSE;

		if (actor->orders.move.jump_leap)
		{
			real_vector2d alignment;

			if (actor->orders.move.jump_targeted)
			{
				alignment = actor->orders.move.jump_alignment_vector;
			}
			else
			{
				alignment.i = actor->input.facing_vector.i;
				alignment.j = actor->input.facing_vector.j;
				if (normalize2d(&alignment) == 0.f)
					alignment = *global_forward2d;
			}

			leaped = unit_leap_begin(
				actor->meta.unit_index,
				&alignment);
		}

		if (leaped)
		{
			ai_communication_event(
				_ai_communication_leap,
				actor->meta.unit_index,
				NONE,
				NONE,
				NONE,
				NONE,
				NULL);
		}
		else
		{
			actor_unit_control_jump(actor_index);
		}

		if (actor->orders.move.jump_targeted)
		{
			actor->control.jumping_targeted = TRUE;
			actor->control.jump_alignment_vector =
				actor->orders.move.jump_alignment_vector;
			actor->control.jump_target_horizontal_vel =
				actor->orders.move.jump_target_horizontal_vel;
			actor->control.jump_target_vertical_vel =
				actor->orders.move.jump_target_vertical_vel;
		}
	}

	actor->output.animation = actor->orders.move.animation;

	return;
}

boolean actor_move_halt(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying &&
		actor->control.moving)
	{
		return actor_move_to_point(
			actor_index,
			&actor->input.position.body_position,
			actor->input.pathfinding_surface_index,
			NONE);
	}

	actor->firing_positions.current_position_index = NONE;
	if (actor->control.path.destination_orders.destination_type != _destination_halt)
	{
		actor->orders.move.destination.destination_type = _destination_halt;
		actor->control.path.destination_orders = actor->orders.move.destination;
	}

	return actor_path_refresh(actor_index, TRUE, NULL);
}

boolean actor_move_halt_at_firing_position(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying)
	{
		short firing_position_index = actor->firing_positions.current_position_index;

		if (firing_position_index == NONE)
			return actor_move_halt(actor_index);

		return actor_move_to_firing_position(
			actor_index,
			firing_position_index,
			NULL);
	}

	if (actor->control.path.destination_orders.destination_type != _destination_halt)
	{
		actor->orders.move.destination.destination_type = _destination_halt;
		actor->control.path.destination_orders = actor->orders.move.destination;
	}

	return actor_path_refresh(actor_index, TRUE, NULL);
}
