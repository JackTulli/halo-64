/*
PROJECTILES.C

symbols in this file:
000E7460 0010:
	_projectiles_initialize (0000)
000E7470 0010:
	_projectiles_initialize_for_new_map (0000)
000E7480 0010:
	_projectiles_dispose_from_old_map (0000)
000E7490 0010:
	_projectiles_dispose (0000)
000E74A0 0030:
	_projectile_kill_tracer (0000)
000E74D0 0040:
	_projectiles_delete_all (0000)
000E7510 0010:
	_projectile_delete (0000)
000E7520 0020:
	_projectile_set_target_object_index (0000)
000E7540 0030:
	_projectile_make_tracer (0000)
000E7570 0020:
	_projectile_get_ballistic_acceleration (0000)
000E7590 0030:
	_projectile_estimate_time_to_target (0000)
000E75C0 0040:
	_dangerous_projectiles_near_player (0000)
000E7600 0030:
	_projectile_handle_deleted_object (0000)
000E7630 0020:
	_code_000e7630 (0000)
000E7650 0060:
	_code_000e7650 (0000)
000E76B0 00e0:
	_projectile_export_function_values (0000)
000E7790 0060:
	_code_000e7790 (0000)
000E77F0 0070:
	_projectile_handle_parent_destroyed (0000)
000E7860 0030:
	_random_vector_in_cone3d (0000)
000E7890 0370:
	_projectile_aim_ballistic (0000)
000E7C00 00c0:
	_projectile_aim_linear (0000)
000E7CC0 00c0:
	_projectile_aim (0000)
000E7D80 00b0:
	_code_000e7d80 (0000)
000E7E30 00e0:
	_code_000e7e30 (0000)
000E7F10 0200:
	_code_000e7f10 (0000)
000E8110 0410:
	_code_000e8110 (0000)
000E8520 01b0:
	_projectile_new (0000)
000E86D0 01f0:
	_projectile_accelerate (0000)
000E88C0 0b70:
	_code_000e88c0 (0000)
000E9430 0fe0:
	_projectile_update (0000)
00279028 000b:
	??_C@_0L@IKCANML@reflection?$AA@ (0000)
00279034 0012:
	??_C@_0BC@MIGMEKLO@negative?5incident?$AA@ (0000)
00279048 0009:
	??_C@_08NAKCOKKJ@incident?$AA@ (0000)
00279054 0012:
	??_C@_0BC@IEANEAFD@projectile_update?$AA@ (0000)
00279068 0004:
	_rdata_00279068 (0000)
0027906C 0023:
	??_C@_0CD@LFGANAAE@c?3?2halo?2SOURCE?2items?2projectiles@ (0000)
00279090 002f:
	??_C@_0CP@JLELNEII@projectile?9?$DOobject?4parent_object@ (0000)
002790C0 0012:
	??_C@_0BC@KMKCONEA@result_aim_vector?$AA@ (0000)
002790D4 0014:
	??_C@_0BE@NNDLGLKI@v_desired_sq?5?$DO?50?40f?$AA@ (0000)
002790E8 0016:
	??_C@_0BG@HPLEINHD@t_squared_max?5?$DO?$DN?50?40f?$AA@ (0000)
00279100 0014:
	??_C@_0BE@BBPDBBBA@4?40f?5?$CK?5a?5?$CK?5c?5?$DO?50?40f?$AA@ (0000)
00279114 002b:
	??_C@_0CL@KDJFFEJL@?$CGprojectile?9?$DOobject?4translationa@ (0000)
00279140 000d:
	??_C@_0N@OKFAMOHD@acceleration?$AA@ (0000)
00279150 0004:
	__real@3c088889 (0000)
00279154 0004:
	__real@3f22f983 (0000)
00279158 001e:
	??_C@_0BO@CBMNHFJH@collision?9?$DOobject_index?$CB?$DNNONE?$AA@ (0000)
00279178 001c:
	??_C@_0BM@JNPELLAN@?$CGprojectile?9?$DOobject?4forward?$AA@ (0000)
00279194 0017:
	??_C@_0BH@CAHIPIOP@?$CGprojectile?9?$DOobject?4up?$AA@ (0000)
002791AC 000e:
	??_C@_0O@DJJKDDEB@?$CGnew_position?$AA@ (0000)
002791BC 002a:
	??_C@_0CK@HFBLHMOF@projectile?5velocity?5is?5bad?5after@ (0000)
002791E8 002f:
	??_C@_0CP@HBPNPJKN@projectile?5velocity?5is?5bad?5after@ (0000)
00279218 0004:
	__real@3f7d70a4 (0000)
0027921C 002b:
	??_C@_0CL@BHNFKMBE@projectile?5velocity?5is?5bad?5after@ (0000)
00306B30 060c:
	_data_00306b30 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "projectiles.h"

#include "projectiles_callbacks.h"
#include "projectile_definitions.h"

#include "ai/ai.h"
#include "cseries/profile.h"
#include "effects/contrail_definitions.h"
#include "effects/contrails.h"
#include "effects/effects.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/game_globals.h"
#include "game/players.h"
#include "math/periodic_functions.h"
#include "objects/damage.h"
#include "objects/objects.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "physics/physics.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"

/* ---------- constants */

enum projectile_datum_flags
{
	_projectile_has_nonzero_angular_velocity_bit = 0,
	_projectile_tracer_bit = 1,
	_projectile_collided_once_bit = 2,
	_projectile_attached_bit = 3,
	_projectile_stopped_after_collision_bit = 4,
	_projectile_counting_down_bit = 5,
	_projectile_already_super_exploded_bit = 6,
	_projectile_will_super_explode_bit = 7,
	NUMBER_OF_PROJECTILE_DATUM_FLAGS,
};

enum projectile_definition_flags
{
	_projectile_oriented_along_velocity_bit = 0,
	_projectile_aim_ballistic_bit = 1,
	_projectile_detonation_max_time_if_attached_bit = 2,
	_projectile_super_combining_explosion_bit = 3,
	_projectile_combine_initial_velocity_with_parent_velocity_bit = 4,
	_projectile_random_detonation_time_when_attached_bit = 5,
	_projectile_minimum_unattached_detonation_time = 6,
	NUMBER_OF_PROJECTILE_DEFINITION_FLAGS,
};

enum projectile_export_function_mode
{
	_projectile_export_function_none = 0,
	_projectile_export_function_range_remaining,
	_projectile_export_function_time_remaining,
	_projectile_export_function_tracer,
	NUMBER_OF_PROJECTILE_EXPORT_FUNCTION_MODES,
};

enum projectile_action
{
	_projectile_action_none = 0,
	_projectile_action_detonate,
	_projectile_action_disappear,
	NUMBER_OF_PROJECTILE_ACTIONS,
};

enum projectile_detonation_timer_starts
{
	_projectile_detonation_timer_starts_immediately = 0,
	_projectile_detonation_timer_starts_after_first_bounce,
	_projectile_detonation_timer_starts_when_at_rest,
	NUMBER_OF_PROJECTILE_DETONATION_TIMER_STARTS,
};

enum projectile_potential_response_flags
{
	_projectile_potential_response_only_against_units_bit = 0,
	NUMBER_OF_PROJECTILE_POTENTIAL_RESPONSE_FLAGS,
};

/* TU-local copies: no shared header declares these yet.  The effect vector
 * enum duplicates objects/damage.c, the spatial effect enum duplicates
 * ai/actors.c and ai/ai.c, the surface flag duplicates ai/path.c and
 * ai/path_smoothing.c, and the periodic function enum duplicates
 * math/periodic_functions.c. */
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
	_ai_spatial_effect_environmental_noise = 0,
	_ai_spatial_effect_weapon_impact,
	_ai_spatial_effect_weapon_detonation,
	NUMBER_OF_AI_SPATIAL_EFFECTS,
};

enum
{
	_collision_surface_breakable_bit = 3,
};

enum
{
	_periodic_function_one = 0,
	_periodic_function_zero,
	_periodic_function_cosine,
	_periodic_function_cosine_variable_period,
	_periodic_function_diagonal_wave,
	_periodic_function_diagonal_wave_variable_period,
	_periodic_function_slide,
	_periodic_function_slide_variable_period,
	_periodic_function_noise,
	_periodic_function_jitter,
	_periodic_function_wander,
	_periodic_function_spark,
	NUMBER_OF_PERIODIC_FUNCTIONS,
};

enum
{
	MAXIMUM_PROJECTILE_COLLISIONS_PER_UPDATE = 10,
	MAXIMUM_COMBINING_PROJECTILES = 6,
};

/* ---------- macros */

#define projectile_runtime_get(index) \
	((struct projectile_runtime_datum *)object_get_and_verify_type( \
		(index), \
		_object_mask_projectile))

/* ---------- structures */

struct _projectile_runtime_datum
{
	unsigned long flags;
	short action;
	short hit_material_type;
	long ignore_object_index;
	long target_object_index;
	long tracer_attachment_index;
	real detonation_timer;
	real detonation_timer_delta;
	real arming_time;
	real arming_time_delta;
	real odometer;
	real deceleration_timer;
	real deceleration_timer_delta;
	real deceleration;
	real maximum_damage_distance;
	real_vector3d rotation_axis;
	real rotation_sine;
	real rotation_cosine;
};

struct projectile_runtime_datum
{
	long definition_index;
	struct _object_datum object;
	struct _item_datum item;
	struct _projectile_runtime_datum projectile;
};

typedef char projectile_runtime_arming_time_delta_offset_assert[
	offsetof(struct projectile_runtime_datum, projectile.arming_time_delta) == 0x1FC
		? 1
		: -1];
typedef char projectile_runtime_odometer_offset_assert[
	offsetof(struct projectile_runtime_datum, projectile.odometer) == 0x200
		? 1
		: -1];
typedef char projectile_runtime_deceleration_offset_assert[
	offsetof(struct projectile_runtime_datum, projectile.deceleration) == 0x20C
		? 1
		: -1];
typedef char projectile_runtime_rotation_axis_offset_assert[
	offsetof(struct projectile_runtime_datum, projectile.rotation_axis) == 0x214
		? 1
		: -1];
typedef char projectile_runtime_rotation_cosine_offset_assert[
	offsetof(struct projectile_runtime_datum, projectile.rotation_cosine) == 0x224
		? 1
		: -1];

/* ---------- prototypes */

boolean projectile_aim_linear(
	real base_velocity,
	real_point3d const *origin,
	real_point3d const *target_point,
	real_vector3d *result_aim_vector,
	real *result_velocity,
	real *result_ticks,
	real *result_distance);

static real projectile_calculate_deceleration_from_distances(
	struct projectile_definition const *definition,
	real minimum_distance,
	real maximum_distance);
static void projectile_adjust_for_angular_velocity_change(
	long projectile_index);
static void projectile_calculate_deceleration(
	long projectile_index);
static void projectile_set_action(
	long projectile_index,
	short action);
static void projectile_effect_new(
	long projectile_index,
	long definition_index,
	struct collision_result const *collision,
	real_point3d const *marker_points,
	real_vector3d const *marker_forwards,
	real scale,
	real material_effect_scale);
static boolean projectile_collision_test_line(
	long projectile_index,
	real_point3d const *new_position,
	struct collision_result *collision);
static void projectile_detonate(
	long projectile_index,
	boolean first_collision,
	real time_left);
static void projectile_collision(
	long projectile_index,
	struct collision_result *collision,
	real_point3d *new_position,
	real_vector3d *new_velocity,
	real time_left);

/* ---------- globals */

static real const seconds_per_tick = 1.0f / TICKS_PER_SECOND;

static struct profile_section projectile_update_section = {"projectile_update", NONE, TRUE};

static char const *effect_marker_names[NUMBER_OF_EFFECT_MARKERS] =
{
	"normal",
	"incident",
	"negative incident",
	"reflection",
	"gravity"
};

/* ---------- public code */

void projectiles_initialize(
	void)
{
	return;
}

void projectiles_initialize_for_new_map(
	void)
{
	return;
}

void projectiles_dispose_from_old_map(
	void)
{
	return;
}

void projectiles_dispose(
	void)
{
	return;
}

void projectile_kill_tracer(
	long projectile_index)
{
	struct projectile_datum *projectile;

	projectile = projectile_get(projectile_index);
	SET_FLAG(projectile->projectile.flags, _projectile_tracer_bit, FALSE);

	return;
}

void projectiles_delete_all(
	void)
{
	struct object_iterator iterator;

	object_iterator_new(&iterator, _object_mask_projectile, 0);
	while (object_iterator_next(&iterator))
	{
		object_delete(iterator.index);
	}

	return;
}

void projectile_delete(
	long projectile_index)
{
	return;
}

void projectile_set_target_object_index(
	long projectile_index,
	long target_object_index)
{
	struct projectile_datum *projectile;

	projectile = projectile_get(projectile_index);
	projectile->projectile.target_object_index = target_object_index;

	return;
}

void projectile_make_tracer(
	long projectile_index)
{
	struct projectile_datum *projectile;

	projectile = projectile_get(projectile_index);
	SET_FLAG(projectile->projectile.flags, _projectile_tracer_bit, TRUE);

	return;
}

void projectile_handle_deleted_object(
	long projectile_index,
	long deleted_object_index)
{
	struct projectile_datum *projectile;

	projectile = projectile_get(projectile_index);
	if (projectile->projectile.target_object_index == deleted_object_index)
		projectile->projectile.target_object_index = NONE;

	return;
}

boolean projectile_handle_parent_destroyed(
	long projectile_index)
{
	struct projectile_datum *projectile;

	projectile = projectile_get(projectile_index);
	match_assert(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		1845,
		projectile->object.parent_object_index != NONE);
	projectile->projectile.arming_time = 1.0f;
	projectile->projectile.detonation_timer = 1.0f;
	SET_FLAG(projectile->projectile.flags, _projectile_attached_bit, FALSE);
	object_detach(projectile_index);

	return TRUE;
}

boolean dangerous_projectiles_near_player(
	void)
{
	struct object_iterator iterator;
	struct object_datum *projectile;

	object_iterator_new(&iterator, _object_mask_projectile, 0);
	projectile = (struct object_datum *)object_iterator_next(&iterator);
	if (projectile)
	{
		projectile_definition_get(projectile->definition_index);
		return TRUE;
	}

	return FALSE;
}

void projectile_export_function_values(
	long projectile_index)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;
	short function_index;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);

	for (function_index = 0; function_index < NUMBEROF(definition->projectile.function_inputs); function_index++)
	{
		real value;
		short function_input = definition->projectile.function_inputs[function_index];

		if (function_input != _projectile_export_function_none)
		{
			switch (function_input)
			{
			case _projectile_export_function_range_remaining:
				if (definition->projectile.maximum_range != 0.f)
					value = projectile->projectile.odometer / definition->projectile.maximum_range;
				else
					value = 0.f;
				break;

			case _projectile_export_function_time_remaining:
				value = projectile->projectile.detonation_timer;
				break;

			case _projectile_export_function_tracer:
				if (TEST_FLAG(projectile->projectile.flags, _projectile_tracer_bit))
					value = 1.f;
				else
					value = 0.f;
				break;

			/* value is left unassigned only by this default arm. Not reached unassigned: the
			 * arm's assertion failure calls system_exit, which does not return in January
			 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
			 * Source-policy approval pending (2026-09-27 audit). */
			default:
				display_assert(
					NULL,
					"c:\\halo\\SOURCE\\items\\projectiles.c",
					1570,
					TRUE);
				system_exit(NONE);
				break;
			}

			projectile->object.incoming_function_values[function_index] = value;
		}
	}

	return;
}

boolean projectile_new(
	long projectile_index)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;
	struct tag_block const *attachments;
	real detonation_ticks;
	real arming_ticks;
	real initial_velocity;
	short attachment_index;
	boolean underwater;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);

	SET_FLAG(projectile->object.flags, _object_dynamic_lighting_recompute_bit, TRUE);
	projectile->projectile.flags = FLAG(_projectile_tracer_bit);
	projectile->projectile.target_object_index = NONE;
	projectile->projectile.action = 0;
	projectile->projectile.hit_material_type = NONE;
	projectile->projectile.ignore_object_index =
		object_get_ultimate_parent(projectile->object.owner_object_index);

	if (TEST_FLAG(
		definition->projectile.flags,
		_projectile_detonation_max_time_if_attached_bit))
	{
		detonation_ticks = definition->projectile.timer_lower_bound;
	}
	else
	{
		detonation_ticks = real_random_range(
			definition->projectile.timer_lower_bound,
			definition->projectile.timer_upper_bound);
	}
	detonation_ticks *= TICKS_PER_SECOND;
	if (detonation_ticks >= 1.f)
		projectile->projectile.detonation_timer_delta = 1.f / detonation_ticks;

	arming_ticks = definition->projectile.arming_time * TICKS_PER_SECOND;
	if (arming_ticks >= 1.f)
		projectile->projectile.arming_time_delta = 1.f / arming_ticks;

	attachments = &definition->object.attachments;
	projectile->projectile.tracer_attachment_index = NONE;
	for (attachment_index = 0;
		attachment_index < attachments->count;
		attachment_index++)
	{
		struct object_attachment_definition const *attachment = TAG_BLOCK_GET_ELEMENT(
			attachments,
			attachment_index,
			struct object_attachment_definition);

		if (attachment->type.group_tag == CONTRAIL_DEFINITION_TAG)
		{
			projectile->projectile.tracer_attachment_index = attachment_index;
			break;
		}
	}

	initial_velocity = definition->projectile.initial_velocity;
	projectile->object.translational_velocity.i =
		projectile->object.forward.i * initial_velocity +
		projectile->object.translational_velocity.i;
	projectile->object.translational_velocity.j =
		projectile->object.forward.j * initial_velocity +
		projectile->object.translational_velocity.j;
	projectile->object.translational_velocity.k =
		projectile->object.forward.k * initial_velocity +
		projectile->object.translational_velocity.k;

	underwater = scenario_location_underwater(
		&projectile->object.location,
		&projectile->object.bounding_sphere_center,
		NULL);
	SET_FLAG(projectile->object.flags, _object_wholly_under_media_bit, underwater);

	projectile_adjust_for_angular_velocity_change(projectile_index);
	projectile_export_function_values(projectile_index);
	projectile_calculate_deceleration(projectile_index);

	SET_FLAG(projectile->object.flags, _object_shadowless_bit, TRUE);
	SET_FLAG(projectile->object.flags, _object_deleted_when_deactivated_bit, TRUE);

	return TRUE;
}

void projectile_accelerate(
	long projectile_index,
	real_vector3d const *acceleration)
{
	struct projectile_runtime_datum *projectile;

	projectile = projectile_runtime_get(projectile_index);
	projectile_definition_get(projectile->definition_index);

	match_assert_valid_real_vector3d(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		1007,
		acceleration);

	if (projectile->object.parent_object_index != NONE)
		return;

	match_assert_valid_real_vector3d(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		1011,
		&projectile->object.translational_velocity);

	add_vectors3d(
		&projectile->object.translational_velocity,
		acceleration,
		&projectile->object.translational_velocity);

	{
		real_vector3d rotation_axis;
		real rotation_magnitude;

		random_direction3d(&rotation_axis);
		rotation_magnitude =
			magnitude3d(acceleration) *
			real_random() *
			1.5707964f;
		scale_vector3d(&rotation_axis, rotation_magnitude, &rotation_axis);
		add_vectors3d(
			&projectile->object.angular_velocity,
			&rotation_axis,
			&projectile->object.angular_velocity);
	}

	projectile_adjust_for_angular_velocity_change(projectile_index);
	SET_FLAG(projectile->object.flags, _object_at_rest_bit, FALSE);

	match_assert_valid_real_vector3d(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		1029,
		&projectile->object.translational_velocity);

	return;
}

boolean projectile_update(
	long projectile_index)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;
	real time_remaining;
	short collision_count;
	boolean flyby_sound_played;
	boolean detonation_timer_running;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);
	time_remaining = 1.0f;
	collision_count = 0;
	flyby_sound_played = FALSE;

	profile_enter(projectile_update_section);

	if (!TEST_FLAG(projectile->projectile.flags, _projectile_tracer_bit) &&
		projectile->projectile.tracer_attachment_index != NONE)
	{
		if (projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index] != NONE)
			contrail_delete(projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index]);
		projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index] = NONE;
		projectile->projectile.tracer_attachment_index = NONE;
	}

	projectile->projectile.arming_time += projectile->projectile.arming_time_delta;
	projectile->projectile.deceleration_timer += projectile->projectile.deceleration_timer_delta;

	switch (definition->projectile.detonation_timer_starts)
	{
	case _projectile_detonation_timer_starts_after_first_bounce:
		detonation_timer_running = TEST_FLAG(projectile->projectile.flags, _projectile_collided_once_bit);

	case _projectile_detonation_timer_starts_when_at_rest:
		detonation_timer_running = TEST_FLAG(projectile->projectile.flags, _projectile_stopped_after_collision_bit);
		break;

	default:
		detonation_timer_running = TRUE;
		break;
	}

	if (TEST_FLAG(projectile->projectile.flags, _projectile_counting_down_bit) ||
		TEST_FLAG(projectile->projectile.flags, _projectile_attached_bit))
	{
		detonation_timer_running = TRUE;
	}

	if (detonation_timer_running)
	{
		if (!TEST_FLAG(projectile->projectile.flags, _projectile_counting_down_bit))
			SET_FLAG(projectile->projectile.flags, _projectile_counting_down_bit, TRUE);

		projectile->projectile.detonation_timer += projectile->projectile.detonation_timer_delta;
		if (projectile->projectile.detonation_timer >= 1.0f)
			projectile_set_action(projectile_index, _projectile_action_detonate);
	}

	projectile_export_function_values(projectile_index);

	while (time_remaining > 0.0f &&
		(projectile->projectile.action == _projectile_action_none ||
			(projectile->projectile.action == _projectile_action_detonate &&
				projectile->projectile.arming_time_delta != 0.0f &&
				projectile->projectile.arming_time < 1.0f)) &&
		!TEST_FLAG(projectile->projectile.flags, _projectile_attached_bit) &&
		!TEST_FLAG(projectile->object.flags, _object_at_rest_bit) &&
		projectile->object.parent_object_index == NONE)
	{
		real speed = magnitude3d(&projectile->object.translational_velocity);
		real_vector3d new_velocity = projectile->object.translational_velocity;
		real_point3d new_position;
		struct collision_result collision;
		boolean moved = FALSE;
		real gravity_acceleration;
		real final_speed = speed;
		real average_speed = speed;
		real_vector3d average_velocity = projectile->object.translational_velocity;
		long ignore_object_index = projectile->projectile.ignore_object_index;
		real distance_fraction;

		match_assert_valid_real_vector3d(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			326,
			&projectile->object.translational_velocity);

		if (projectile->projectile.target_object_index != NONE &&
			definition->projectile.guided_angular_velocity > 0.0f)
		{
			struct object_datum *target = object_get(projectile->projectile.target_object_index);
			real angular_velocity = definition->projectile.guided_angular_velocity * (1.0f / TICKS_PER_SECOND);
			real target_distance;
			real wander_scale;
			real_point3d target_point;
			real_euler_angles2d wander_angles;
			real_vector3d wander_direction;
			real_vector3d target_vector;
			real_vector3d rotation_axis;

			if (TEST_FLAG(_object_mask_unit, target->object.type) &&
				unit_get(projectile->projectile.target_object_index)->unit.player_index != NONE)
			{
				angular_velocity *= game_difficulty_get_value(_game_difficulty_value_guidance_vs_player);
			}

			target_distance = distance3d(
				&target->object.bounding_sphere_center,
				&projectile->object.bounding_sphere_center);
			if (target_distance > 10.0f)
			{
				wander_scale = 1.0f;
			}
			else if (target_distance > 2.0f)
			{
				wander_scale = (target_distance - 2.0f) * 0.125f;
				wander_scale = PIN(wander_scale, 0.0f, 1.0f);
			}
			else
			{
				wander_scale = 0.0f;
			}

			unit_get_center_of_mass(projectile->projectile.target_object_index, &target_point);

			set_real_euler_angles2d(
				&wander_angles,
				_pi - periodic_function_evaluate(
					_periodic_function_wander,
					((game_time_get() + 3 * DATUM_INDEX_TO_IDENTIFIER(projectile_index)) & UNSIGNED_SHORT_MAX) * (1.0f / 90.0f)) * (_pi / 2.f),
				periodic_function_evaluate(
					_periodic_function_wander,
					((game_time_get() + 7 * DATUM_INDEX_TO_IDENTIFIER(projectile_index)) & UNSIGNED_SHORT_MAX) * (1.0f / 90.0f)) * (_pi * 2.f));
			vector3d_from_euler_angles2d(&wander_direction, &wander_angles);

			point_from_line3d(&target_point, &wander_direction, wander_scale, &target_point);

			vector_from_points3d(&projectile->object.position, &target_point, &target_vector);
			cross_product3d(&projectile->object.translational_velocity, &target_vector, &rotation_axis);
			if (dot_product3d(&target_vector, &projectile->object.translational_velocity) > 0.0f &&
				normalize3d(&rotation_axis) > 0.0f)
			{
				rotate_vector_about_axis(
					&new_velocity,
					&rotation_axis,
					sine(angular_velocity),
					cosine(angular_velocity));
			}
		}

		match_vassert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			413,
			valid_real_vector3d(&new_velocity),
			"projectile velocity is bad after steering.");

		if (projectile->projectile.deceleration_timer >= 1.0f)
		{
			if (speed > definition->projectile.final_velocity &&
				projectile->projectile.deceleration != 0.0f)
			{
				final_speed = speed - projectile->projectile.deceleration * time_remaining;
				if (final_speed <= definition->projectile.final_velocity)
				{
					real fraction = (speed - definition->projectile.final_velocity) / (projectile->projectile.deceleration * time_remaining);

					final_speed = definition->projectile.final_velocity * 0.99f;
					average_speed = (final_speed + speed) * fraction * 0.5f +
						(1.0f - fraction) * definition->projectile.final_velocity;
					scale_vector3d(&new_velocity, final_speed / speed, &new_velocity);
					average_velocity.i = (new_velocity.i + projectile->object.translational_velocity.i) * fraction * 0.5f + (1.0f - fraction) * new_velocity.i;
					average_velocity.j = (new_velocity.j + projectile->object.translational_velocity.j) * fraction * 0.5f + (1.0f - fraction) * new_velocity.j;
					average_velocity.k = (new_velocity.k + projectile->object.translational_velocity.k) * fraction * 0.5f + (1.0f - fraction) * new_velocity.k;
				}
				else
				{
					average_speed = speed - projectile->projectile.deceleration * time_remaining * 0.5f;
					scale_vector3d(&new_velocity, final_speed / speed, &new_velocity);
					average_velocity.i = (new_velocity.i + projectile->object.translational_velocity.i) * 0.5f;
					average_velocity.j = (new_velocity.j + projectile->object.translational_velocity.j) * 0.5f;
					average_velocity.k = (new_velocity.k + projectile->object.translational_velocity.k) * 0.5f;
				}
			}
			else if (definition->projectile.maximum_range == 0.0f &&
				definition->projectile.timer_upper_bound == 0.0f &&
				definition->projectile.minimum_velocity <= definition->projectile.final_velocity &&
				(projectile->projectile.deceleration != 0.0f ||
					projectile->projectile.odometer >= projectile->projectile.maximum_damage_distance))
			{
				projectile_set_action(projectile_index, _projectile_action_disappear);
			}
			else if (speed < definition->projectile.final_velocity && speed > 0.0f)
			{
				scale_vector3d(
					&new_velocity,
					definition->projectile.final_velocity / speed * 0.99f,
					&new_velocity);
			}
		}

		match_vassert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			461,
			valid_real_vector3d(&new_velocity),
			"projectile velocity is bad after deceleration.");

		if (TEST_FLAG(projectile->object.flags, _object_wholly_under_media_bit))
			gravity_acceleration = global_gravity * definition->projectile.water_gravity_scale;
		else
			gravity_acceleration = global_gravity * definition->projectile.air_gravity_scale;
		new_velocity.k -= gravity_acceleration * time_remaining;
		average_velocity.k -= gravity_acceleration * time_remaining * 0.5f;

		match_vassert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			476,
			valid_real_vector3d(&new_velocity),
			"projectile velocity is bad after gravity.");

		if (definition->projectile.maximum_range != 0.0f &&
			average_speed * time_remaining + projectile->projectile.odometer > definition->projectile.maximum_range)
		{
			if (average_speed != 0.0f)
			{
				distance_fraction = (definition->projectile.maximum_range - projectile->projectile.odometer) /
					average_speed * time_remaining;
			}
			else
			{
				distance_fraction = 0.0f;
			}
			projectile_set_action(projectile_index, _projectile_action_detonate);
		}
		else
		{
			distance_fraction = 1.0f;
		}

		point_from_line3d(&projectile->object.position, &average_velocity, distance_fraction * time_remaining, &new_position);

		match_assert_valid_real_point3d(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			505,
			&new_position);

		match_assert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			507,
			global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
		global_current_collision_users[global_current_collision_user_depth++] =
			_collision_user_projectiles;

		if (collision_count != MAXIMUM_PROJECTILE_COLLISIONS_PER_UPDATE &&
			projectile->projectile.action != _projectile_action_disappear &&
			(moved = TRUE) &&
			projectile_collision_test_line(projectile_index, &new_position, &collision))
		{
			time_remaining = 1.0f - collision.t;
			new_velocity.k += gravity_acceleration * time_remaining;
			if (final_speed != 0.0f)
			{
				real restored_speed = time_remaining * projectile->projectile.deceleration + final_speed;

				restored_speed = MIN(restored_speed, speed);
				scale_vector3d(&new_velocity, restored_speed / final_speed, &new_velocity);
			}

			if (collision.plane.n.k > 0.3f)
				SET_FLAG(projectile->projectile.flags, _projectile_collided_once_bit, TRUE);

			projectile->projectile.ignore_object_index = NONE;
			projectile_collision(
				projectile_index,
				&collision,
				&new_position,
				&new_velocity,
				time_remaining);
			collision_count++;
			ai_handle_spatial_effect(
				projectile_index,
				&collision.point,
				_ai_spatial_effect_weapon_impact,
				definition->projectile.impact_noise,
				1);

			if (TEST_FLAG(projectile->projectile.flags, _projectile_attached_bit))
				moved = FALSE;
		}
		else
		{
			if (collision_count == MAXIMUM_PROJECTILE_COLLISIONS_PER_UPDATE)
				projectile_set_action(projectile_index, _projectile_action_detonate);
			time_remaining = 0.0f;
		}

		match_assert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			560,
			global_current_collision_user_depth > 1);
		--global_current_collision_user_depth;

		if (moved)
		{
			real_vector3d movement;

			vector_from_points3d(&projectile->object.position, &new_position, &movement);
			projectile->projectile.odometer += magnitude3d(&movement);

			if (!flyby_sound_played &&
				definition->projectile.flyby_sound.index != NONE)
			{
				short local_player_index;

				for (local_player_index = 0; local_player_index < MAXIMUM_LOCAL_PLAYERS; local_player_index++)
				{
					if (local_player_get_player_index(local_player_index) != NONE)
					{
						long unit_index = player_get(local_player_get_player_index(local_player_index))->unit_index;

						if (unit_index != NONE && unit_index != ignore_object_index)
						{
							real_point3d const *listener_position = &object_get(unit_index)->object.bounding_sphere_center;
							real maximum_distance = sound_definition_get_maximum_distance(definition->projectile.flyby_sound.index);
							real_vector3d listener_vector;
							real_vector3d parallel;
							real_vector3d perpendicular;
							real distance_along_path;

							listener_vector.i = listener_position->x - projectile->object.position.x;
							listener_vector.j = listener_position->y - projectile->object.position.y;
							listener_vector.k = listener_position->z - projectile->object.position.z;
							component_vectors_from_direction3d(&listener_vector, &movement, &parallel, &perpendicular);
							distance_along_path = dot_product3d(&parallel, &movement);
							if (distance_along_path >= 0.0f &&
								distance_along_path < magnitude_squared3d(&movement) &&
								magnitude_squared3d(&perpendicular) < maximum_distance * maximum_distance)
							{
								struct sound_location location;

								point_from_line3d(listener_position, &perpendicular, -1.0f, &location.position);
								location.forward = movement;
								normalize3d(&location.forward);
								location.translational_velocity = *global_zero_vector3d;
								location.game_location = collision.location;
								unattached_impulse_sound_new(definition->projectile.flyby_sound.index, &location, 1.0f);
								flyby_sound_played = TRUE;
							}
						}
					}
				}
			}

			if (TEST_FLAG(definition->projectile.flags, _projectile_oriented_along_velocity_bit) &&
				(projectile->object.translational_velocity.i != 0.0f || projectile->object.translational_velocity.j != 0.0f || projectile->object.translational_velocity.k != 0.0f))
			{
				real_vector3d forward = projectile->object.translational_velocity;

				if (normalize3d(&forward) > 0.0f)
				{
					real_vector3d left;

					projectile->object.forward = forward;
					cross_product3d(&projectile->object.up, &projectile->object.forward, &left);
					if (normalize3d(cross_product3d(&projectile->object.forward, &left, &projectile->object.up)) == 0.0f)
						normalize3d(perpendicular3d(&projectile->object.forward, &projectile->object.up));
				}

				rotate_vector_about_axis(
					&projectile->object.up,
					&projectile->object.forward,
					projectile->projectile.rotation_sine,
					projectile->projectile.rotation_cosine);
			}
			else if (TEST_FLAG(projectile->projectile.flags, _projectile_has_nonzero_angular_velocity_bit))
			{
				real_vector3d left;

				rotate_vector_about_axis(
					&projectile->object.forward,
					&projectile->projectile.rotation_axis,
					projectile->projectile.rotation_sine,
					projectile->projectile.rotation_cosine);
				rotate_vector_about_axis(
					&projectile->object.up,
					&projectile->projectile.rotation_axis,
					projectile->projectile.rotation_sine,
					projectile->projectile.rotation_cosine);
				normalize3d(&projectile->object.forward);
				cross_product3d(&projectile->object.up, &projectile->object.forward, &left);
				cross_product3d(&projectile->object.forward, &left, &projectile->object.up);
				normalize3d(&projectile->object.up);
			}

			object_translate(projectile_index, &new_position, &collision.location);
			projectile->object.translational_velocity = new_velocity;

			if (time_remaining != 0.0f &&
				collision_count != 0 &&
				projectile->projectile.tracer_attachment_index != NONE &&
				projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index] != NONE)
			{
				object_compute_node_matrices(projectile_index);
				contrail_owner_collision(
					projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index],
					FALSE,
					(1.0f - time_remaining) * seconds_per_tick);
			}
		}

		match_assert_valid_real_vector3d(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			672,
			&projectile->object.translational_velocity);
	}

	switch (projectile->projectile.action)
	{
	case _projectile_action_detonate:
		if (projectile->projectile.arming_time_delta != 0.0f &&
			projectile->projectile.arming_time < 1.0f)
		{
			break;
		}
		projectile_detonate(projectile_index, collision_count == 0, time_remaining);
		/* the detonated projectile is deleted below */

	case _projectile_action_disappear:
		object_delete(projectile_index);
		break;
	}

	match_assert_valid_real_vector3d_axes2(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		688,
		&projectile->object.forward,
		&projectile->object.up);

	profile_exit(projectile_update_section);

	return TRUE;
}

/* ---------- private code */

static real projectile_calculate_deceleration_from_distances(
	struct projectile_definition const *definition,
	real minimum_distance,
	real maximum_distance)
{
	real distance_delta = maximum_distance - minimum_distance;
	real deceleration = 0.f;

	if (definition->projectile.initial_velocity != definition->projectile.final_velocity &&
		distance_delta != 0.f)
	{
		deceleration =
			(definition->projectile.initial_velocity * definition->projectile.initial_velocity -
			 definition->projectile.final_velocity * definition->projectile.final_velocity) /
			(2.f * distance_delta);
	}

	return deceleration;
}

static void projectile_adjust_for_angular_velocity_change(
	long projectile_index)
{
	struct projectile_runtime_datum *projectile;
	real angular_velocity_magnitude;

	projectile = projectile_runtime_get(projectile_index);
	angular_velocity_magnitude = magnitude3d(&projectile->object.angular_velocity);

	if (angular_velocity_magnitude != 0.f)
	{
		SET_FLAG(
			projectile->projectile.flags,
			_projectile_has_nonzero_angular_velocity_bit,
			TRUE);
		scale_vector3d(
			&projectile->object.angular_velocity,
			1.f / angular_velocity_magnitude,
			&projectile->projectile.rotation_axis);
		projectile->projectile.rotation_sine = sine(angular_velocity_magnitude);
		projectile->projectile.rotation_cosine = cosine(angular_velocity_magnitude);
	}
	else
	{
		SET_FLAG(
			projectile->projectile.flags,
			_projectile_has_nonzero_angular_velocity_bit,
			FALSE);
		projectile->projectile.rotation_sine = 0.f;
		projectile->projectile.rotation_cosine = 1.f;
	}

	return;
}

static void projectile_calculate_deceleration(
	long projectile_index)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);

	if (TEST_FLAG(projectile->object.flags, _object_wholly_under_media_bit))
	{
		projectile->projectile.deceleration = projectile_calculate_deceleration_from_distances(
			definition,
			definition->projectile.water_damage_range_lower_bound,
			definition->projectile.water_damage_range_upper_bound);
		projectile->projectile.maximum_damage_distance =
			definition->projectile.water_damage_range_upper_bound;

		if (definition->projectile.water_damage_range_lower_bound > 0.f)
		{
			projectile->projectile.deceleration_timer_delta =
				definition->projectile.water_damage_range_lower_bound /
				definition->projectile.initial_velocity;
			return;
		}
	}
	else
	{
		projectile->projectile.deceleration = projectile_calculate_deceleration_from_distances(
			definition,
			definition->projectile.air_damage_range_lower_bound,
			definition->projectile.air_damage_range_upper_bound);
		/* BUG (preserved for exact matching): January loads the water upper bound
		 * in the air branch. A corrected build should use
		 * definition->projectile.air_damage_range_upper_bound. */
		projectile->projectile.maximum_damage_distance =
			definition->projectile.water_damage_range_upper_bound;

		if (definition->projectile.air_damage_range_lower_bound > 0.f)
		{
			projectile->projectile.deceleration_timer_delta =
				definition->projectile.air_damage_range_lower_bound /
				definition->projectile.initial_velocity;
			return;
		}
	}

	projectile->projectile.deceleration_timer = 1.f;
	projectile->projectile.deceleration_timer_delta = 0.f;

	return;
}

static void projectile_set_action(
	long projectile_index,
	short action)
{
	struct projectile_runtime_datum *projectile;

	projectile = projectile_runtime_get(projectile_index);
	if (action > projectile->projectile.action)
		projectile->projectile.action = action;

	return;
}

static void projectile_effect_new(
	long projectile_index,
	long definition_index,
	struct collision_result const *collision,
	real_point3d const *marker_points,
	real_vector3d const *marker_forwards,
	real scale,
	real material_effect_scale)
{
	if (collision->type == _collision_result_object)
	{
		effect_new_attached_from_markers(
			definition_index,
			projectile_index,
			collision->object_index,
			collision->node_index,
			NUMBER_OF_EFFECT_MARKERS,
			effect_marker_names,
			marker_points,
			marker_forwards,
			scale,
			material_effect_scale,
			NULL,
			NULL);
	}
	else
	{
		effect_new_unattached_from_markers(
			definition_index,
			projectile_index,
			NULL,
			NUMBER_OF_EFFECT_MARKERS,
			effect_marker_names,
			marker_points,
			marker_forwards,
			scale,
			material_effect_scale,
			NULL,
			NULL,
			TRUE);
	}

	return;
}

static boolean projectile_collision_test_line(
	long projectile_index,
	real_point3d const *new_position,
	struct collision_result *collision)
{
	struct projectile_runtime_datum *projectile = projectile_runtime_get(projectile_index);
	struct projectile_definition const *definition = projectile_definition_get(projectile->definition_index);

	if (collision_test_line(
		_collision_test_for_projectiles_flags,
		&projectile->object.position,
		new_position,
		projectile->projectile.ignore_object_index,
		collision))
	{
		return TRUE;
	}
	else if (definition->projectile.collision_radius < _real_epsilon)
	{
		return FALSE;
	}
	else
	{
		real_vector3d forward;
		real_vector3d left;
		real_point3d p0_left;
		real_point3d p1_left;
		real_point3d p0_right;
		real_point3d p1_right;

		vector_from_points3d(&projectile->object.position, new_position, &forward);
		if (normalize3d(cross_product3d(global_up3d, &forward, &left)) == 0.0f)
		{
			left = *global_left3d;
		}

		point_from_line3d(&projectile->object.position, &left, definition->projectile.collision_radius, &p0_left);
		point_from_line3d(new_position, &left, definition->projectile.collision_radius, &p1_left);
		point_from_line3d(&projectile->object.position, &left, -definition->projectile.collision_radius, &p0_right);
		point_from_line3d(new_position, &left, -definition->projectile.collision_radius, &p1_right);

		if (collision_test_line(
				_collision_test_for_projectiles_fat_flags,
				&p0_left,
				&p1_left,
				projectile->projectile.ignore_object_index,
				collision) ||
			collision_test_line(
				_collision_test_for_projectiles_fat_flags,
				&p0_right,
				&p1_right,
				projectile->projectile.ignore_object_index,
				collision))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

static void projectile_detonate(
	long projectile_index,
	boolean first_collision,
	real time_left)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;
	char const *marker_names[2];
	real_point3d marker_points[2];
	real_vector3d marker_forwards[2];
	real_vector3d up;
	long effect_definition_index;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);

	marker_names[0] = "";
	marker_names[1] = "gravity";
	effect_definition_index = definition->projectile.effect.index;

	if (TEST_FLAG(definition->projectile.flags, _projectile_super_combining_explosion_bit) &&
		!TEST_FLAG(projectile->projectile.flags, _projectile_already_super_exploded_bit) &&
		projectile->object.parent_object_index != NONE)
	{
		struct object_datum *parent = object_get(projectile->object.parent_object_index);
		struct object_datum *child;
		long child_index;
		short combining_projectile_count = 0;

		for (child_index = parent->object.first_child_object_index;
			child_index != NONE;
			child_index = child->object.next_object_index)
		{
			child = object_get(child_index);
			if (child->definition_index == projectile->definition_index &&
				!TEST_FLAG(projectile_runtime_get(child_index)->projectile.flags, _projectile_already_super_exploded_bit))
			{
				combining_projectile_count++;
			}
		}

		if (parent->object.type == _object_type_biped &&
			(biped_get(projectile->object.parent_object_index)->unit.player_index == NONE || game_engine_running()) &&
			combining_projectile_count > MAXIMUM_COMBINING_PROJECTILES)
		{
			real_point3d parent_origin;
			real_point3d known_good_point;

			for (child_index = parent->object.first_child_object_index;
				child_index != NONE;
				child_index = child->object.next_object_index)
			{
				child = object_get(child_index);
				if (child->definition_index == projectile->definition_index &&
					!TEST_FLAG(projectile_runtime_get(child_index)->projectile.flags, _projectile_already_super_exploded_bit))
				{
					struct projectile_runtime_datum *child_projectile = projectile_runtime_get(child_index);

					if (combining_projectile_count <= MAXIMUM_COMBINING_PROJECTILES)
					{
						SET_FLAG(child_projectile->projectile.flags, _projectile_already_super_exploded_bit, TRUE);
						child_projectile->projectile.detonation_timer *= real_random();
						child_projectile->projectile.arming_time *= real_random();
					}
					else
					{
						child_projectile->projectile.detonation_timer = 0.0f;
						child_projectile->projectile.arming_time = 0.0f;
					}
					combining_projectile_count--;
				}
			}

			effect_definition_index = definition->projectile.super_detonation.index;
			object_get_origin(projectile->object.parent_object_index, &parent_origin);
			object_detach(projectile_index);
			known_good_point = projectile->object.position;
			object_translate(projectile_index, &parent_origin, NULL);
			object_force_inside_bsp(projectile_index, &known_good_point);
			object_compute_node_matrices_recursive(projectile_index);
		}
	}

	if (first_collision &&
		projectile->projectile.tracer_attachment_index != NONE &&
		projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index] != NONE)
	{
		object_compute_node_matrices(projectile_index);
		contrail_owner_collision(
			projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index],
			FALSE,
			(1.0f - time_left) * seconds_per_tick);
	}

	object_get_origin(projectile_index, &marker_points[0]);
	object_get_orientation(projectile_index, &marker_forwards[0], &up);
	marker_points[1] = marker_points[0];
	marker_forwards[1] = *global_down3d;
	effect_new_unattached_from_markers(
		effect_definition_index,
		projectile->object.owner_object_index,
		NULL,
		NUMBEROF(marker_names),
		marker_names,
		marker_points,
		marker_forwards,
		0.0f,
		0.0f,
		NULL,
		NULL,
		TRUE);

	if (projectile->object.parent_object_index != NONE &&
		definition->projectile.attached_detonation_damage.index != NONE)
	{
		struct damage_data damage;

		damage_data_new(&damage, definition->projectile.attached_detonation_damage.index);
		SET_FLAG(damage.flags, _damage_from_weapon_bit, TRUE);
		object_get_orientation(projectile_index, &damage.direction, NULL);
		object_get_origin(projectile_index, &damage.origin);
		damage.epicenter = damage.origin;
		damage.owner_object_index = projectile->object.owner_object_index;
		damage.owner_player_index = projectile->object.owner_player_index;
		damage.owner_team_index = projectile->object.owner_team_index;
		object_cause_damage(&damage, projectile->object.parent_object_index, NONE, NONE, NONE, NULL);
	}

	if (projectile->projectile.hit_material_type != NONE)
	{
		struct projectile_material_response_definition const *material_response;

		material_response = VALID_INDEX(projectile->projectile.hit_material_type, definition->projectile.material_responses.count) ?
			TAG_BLOCK_GET_ELEMENT(
				&definition->projectile.material_responses,
				projectile->projectile.hit_material_type,
				struct projectile_material_response_definition) :
			&default_projectile_material_response;

		effect_new_unattached_from_markers(
			material_response->detonation_effect.index,
			projectile->object.owner_object_index,
			NULL,
			NUMBEROF(marker_names),
			marker_names,
			marker_points,
			marker_forwards,
			0.0f,
			0.0f,
			NULL,
			NULL,
			TRUE);
	}

	ai_handle_spatial_effect(
		projectile_index,
		&marker_points[0],
		_ai_spatial_effect_weapon_detonation,
		definition->projectile.detonation_noise,
		1);

	return;
}

static void projectile_collision(
	long projectile_index,
	struct collision_result *collision,
	real_point3d *new_position,
	real_vector3d *new_velocity,
	real time_left)
{
	struct projectile_runtime_datum *projectile;
	struct projectile_definition const *definition;
	struct projectile_material_response_definition const *material_response;
	real_vector3d direction;
	real speed;
	real damage_scale;
	real effect_scale;
	real material_effect_scale;
	real impact_velocity;
	real impact_angle;
	real speed_squared;
	long effect_definition_index;
	short material_type;
	short response;

	projectile = projectile_runtime_get(projectile_index);
	definition = projectile_definition_get(projectile->definition_index);
	material_type = collision->material_type;
	effect_scale = 1.0f;
	material_effect_scale = 0.0f;

	direction = *new_velocity;
	speed = normalize3d(&direction);
	if (speed == 0.0f)
		direction = *global_up3d;

	if (definition->projectile.final_velocity != definition->projectile.initial_velocity)
	{
		damage_scale = (speed - definition->projectile.final_velocity) /
			(definition->projectile.initial_velocity - definition->projectile.final_velocity);
		damage_scale = PIN(damage_scale, 0.0f, 1.0f);
	}
	else
	{
		damage_scale = 1.0f;
	}

	if (collision->type == _collision_result_object &&
		definition->projectile.impact_damage.index != NONE)
	{
		struct damage_data damage;

		match_assert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			1148,
			collision->object_index!=NONE);

		damage_data_new(&damage, definition->projectile.impact_damage.index);
		SET_FLAG(damage.flags, _damage_from_weapon_bit, TRUE);
		damage.scale = damage_scale;
		damage.owner_player_index = projectile->object.owner_player_index;
		damage.owner_object_index = projectile->object.owner_object_index;
		damage.owner_team_index = projectile->object.owner_team_index;
		damage.epicenter = collision->point;
		damage.origin = collision->point;
		damage.direction = *new_velocity;
		normalize3d(&damage.direction);
		object_cause_damage(
			&damage,
			collision->object_index,
			collision->node_index,
			collision->region_index,
			collision->material_index,
			&collision->plane.n);

		if (damage.material_type != NONE)
			material_type = damage.material_type;
		material_effect_scale = damage.material_effect_scale;
	}

	projectile->projectile.hit_material_type = material_type;
	if (VALID_INDEX(material_type, definition->projectile.material_responses.count))
	{
		material_response = TAG_BLOCK_GET_ELEMENT(
			&definition->projectile.material_responses,
			material_type,
			struct projectile_material_response_definition);
	}
	else
	{
		material_response = &default_projectile_material_response;
	}

	impact_velocity = -dot_product3d(new_velocity, &collision->plane.n) + real_random_range(-material_response->velocity_noise, 0.0f);
	impact_angle = angle_between_vectors3d(new_velocity, &collision->plane.n) - _pi / 2.f +
		real_random_range(-material_response->angular_noise, material_response->angular_noise);

	if (material_response->potential_response &&
		(material_response->potential_angle_upper_bound == 0.0f ||
			(impact_angle >= material_response->potential_angle_lower_bound &&
				impact_angle <= material_response->potential_angle_upper_bound)) &&
		(material_response->potential_velocity_upper_bound == 0.0f ||
			(impact_velocity >= material_response->potential_velocity_lower_bound &&
				impact_velocity <= material_response->potential_velocity_upper_bound)) &&
		(!TEST_FLAG(material_response->potential_flags, _projectile_potential_response_only_against_units_bit) ||
			(collision->type == _collision_result_object &&
				object_try_and_get_and_verify_type(collision->object_index, _object_mask_unit))) &&
		real_random() >= material_response->potential_skip_fraction)
	{
		response = material_response->potential_response;
		effect_definition_index = material_response->potential_effect.index;
	}
	else
	{
		response = material_response->default_response;
		effect_definition_index = material_response->default_effect.index;
	}

	if (collision->type == _collision_result_structure &&
		TEST_FLAG(collision->flags, _collision_surface_breakable_bit))
	{
		struct damage_data damage;

		damage_data_new(&damage, definition->projectile.impact_damage.index);
		SET_FLAG(damage.flags, _damage_from_weapon_bit, TRUE);
		damage.epicenter = collision->point;
		damage.origin = collision->point;
		damage.direction = *new_velocity;
		normalize3d(&damage.direction);
		damage.material_type = collision->material_type;
		if (VALID_INDEX(damage.material_type, definition->projectile.material_responses.count))
		{
			damage.material_response = TAG_BLOCK_GET_ELEMENT(
				&definition->projectile.material_responses,
				damage.material_type,
				struct projectile_material_response_definition);
		}
		else
		{
			damage.material_response = &default_projectile_material_response;
		}
		damage.location = collision->location;
		breakable_surface_damage(collision->breakable_surface_index, &damage, collision->surface_index);
	}

	*new_position = collision->point;

	if (response == _projectile_material_response_overpenetrate)
	{
		if (collision->type == _collision_result_media)
		{
			SET_FLAG(
				projectile->object.flags,
				_object_wholly_under_media_bit,
				!TEST_FLAG(projectile->object.flags, _object_wholly_under_media_bit));
			projectile_calculate_deceleration(projectile_index);
			new_position->x -= collision->plane.n.i * 0.001f;
			new_position->y -= collision->plane.n.j * 0.001f;
			new_position->z -= collision->plane.n.k * 0.001f;
		}
		else if (collision->type == _collision_result_object)
		{
			scale_vector3d(new_velocity, 1.0f - material_response->initial_friction, new_velocity);
			projectile->projectile.ignore_object_index = collision->object_index;
		}
		else if (definition->projectile.timer_upper_bound != 0.0f)
		{
			SET_FLAG(projectile->projectile.flags, _projectile_collided_once_bit, TRUE);
			SET_FLAG(projectile->projectile.flags, _projectile_stopped_after_collision_bit, TRUE);
			response = _projectile_material_response_attach;
		}
		else
		{
			response = _projectile_material_response_detonate;
		}
	}

	if (response == _projectile_material_response_reflect)
	{
		real_vector3d parallel;
		real_vector3d perpendicular;

		component_vectors_from_direction3d(new_velocity, &collision->plane.n, &parallel, &perpendicular);
		new_velocity->i = (1.0f - material_response->perpendicular_friction) * perpendicular.i -
			(1.0f - material_response->parallel_friction) * parallel.i;
		new_velocity->j = (1.0f - material_response->perpendicular_friction) * perpendicular.j -
			(1.0f - material_response->parallel_friction) * parallel.j;
		new_velocity->k = (1.0f - material_response->perpendicular_friction) * perpendicular.k -
			(1.0f - material_response->parallel_friction) * parallel.k;
	}
	else if (response != _projectile_material_response_overpenetrate)
	{
		*new_velocity = *global_zero_vector3d;
	}

	if (material_response->angular_noise != 0.0f)
	{
		random_vector_in_cone3d(
			new_velocity,
			0.0f,
			material_response->angular_noise,
			new_velocity);
	}

	if (material_response->velocity_noise != 0.0f)
	{
		real magnitude = normalize3d(new_velocity);

		if (magnitude != 0.0f)
		{
			scale_vector3d(
				new_velocity,
				real_random_range(-material_response->velocity_noise, material_response->velocity_noise) + magnitude,
				new_velocity);
		}
	}

	speed_squared = magnitude_squared3d(new_velocity);
	if (response != _projectile_material_response_attach &&
		magnitude_squared3d(new_velocity) < definition->projectile.minimum_velocity * definition->projectile.minimum_velocity)
	{
		projectile_set_action(projectile_index, _projectile_action_detonate);
	}

	if (speed_squared < _real_epsilon)
	{
		SET_FLAG(projectile->projectile.flags, _projectile_stopped_after_collision_bit, TRUE);
		if (collision->plane.n.k > 0.3f)
		{
			SET_FLAG(projectile->object.flags, _object_at_rest_bit, TRUE);
			if (definition->projectile.timer_upper_bound == 0.0f)
				projectile_set_action(projectile_index, _projectile_action_detonate);
		}
	}

	switch (material_response->scale_effects_by)
	{
	case _projectile_material_effect_scale_angle:
		effect_scale = impact_angle * (2.f / _pi);
		break;

	case _projectile_material_effect_scale_damage:
		effect_scale = damage_scale;
		break;
	}
	effect_scale = PIN(effect_scale, 0.0f, 1.0f);
	material_effect_scale = PIN(material_effect_scale, 0.0f, 1.0f);

	{
		real_point3d marker_points[NUMBER_OF_EFFECT_MARKERS];
		real_vector3d marker_forwards[NUMBER_OF_EFFECT_MARKERS];
		short marker_index;

		scale_vector3d(&direction, -1.0f, &marker_forwards[_effect_vector_incident]);
		marker_forwards[_effect_vector_negative_incident] = direction;
		marker_forwards[_effect_vector_gravity] = *global_down3d;
		marker_forwards[_effect_vector_normal] = collision->plane.n;
		reflect_vector3d(&direction, &collision->plane.n, &marker_forwards[_effect_vector_reflected]);
		for (marker_index = 0; marker_index < NUMBER_OF_EFFECT_MARKERS; marker_index++)
			marker_points[marker_index] = collision->point;

		if (impact_velocity > 1.0f / 120.0f)
		{
			projectile_effect_new(
				projectile_index,
				effect_definition_index,
				collision,
				marker_points,
				marker_forwards,
				effect_scale,
				material_effect_scale);
		}

		if (!TEST_FLAG(projectile->projectile.flags, _projectile_counting_down_bit) &&
			(TEST_FLAG(projectile->projectile.flags, _projectile_stopped_after_collision_bit) ||
				response == _projectile_material_response_attach))
		{
			projectile_effect_new(
				projectile_index,
				definition->projectile.detonation_started.index,
				collision,
				marker_points,
				marker_forwards,
				effect_scale,
				material_effect_scale);
		}
	}

	switch (response)
	{
	case _projectile_material_response_disappear:
		projectile_set_action(projectile_index, _projectile_action_disappear);
		break;

	case _projectile_material_response_detonate:
		projectile_set_action(projectile_index, _projectile_action_detonate);
		break;

	case _projectile_material_response_reflect:
	case _projectile_material_response_overpenetrate:
		break;

	case _projectile_material_response_attach:
		if (collision->type == _collision_result_object &&
			TEST_FLAG(definition->projectile.flags, _projectile_super_combining_explosion_bit))
		{
			long child_index = object_get(collision->object_index)->object.first_child_object_index;
			short combining_projectile_count = 0;

			while (child_index != NONE)
			{
				struct object_datum *child = object_get(child_index);

				if (child->definition_index == projectile->definition_index &&
					!TEST_FLAG(projectile_runtime_get(child_index)->projectile.flags, _projectile_already_super_exploded_bit))
				{
					struct projectile_runtime_datum *child_projectile = projectile_runtime_get(child_index);

					child_projectile->projectile.arming_time = 0.0f;
					child_projectile->projectile.detonation_timer = 0.0f;
					combining_projectile_count++;
				}

				if (combining_projectile_count >= MAXIMUM_COMBINING_PROJECTILES)
				{
					SET_FLAG(projectile->projectile.flags, _projectile_will_super_explode_bit, TRUE);
					break;
				}

				child_index = child->object.next_object_index;
			}
		}

		projectile->object.translational_velocity = *global_zero_vector3d;
		projectile->object.angular_velocity = *global_zero_vector3d;
		SET_FLAG(projectile->projectile.flags, _projectile_attached_bit, TRUE);
		SET_FLAG(projectile->object.flags, _object_at_rest_bit, TRUE);
		object_translate(projectile_index, new_position, &collision->location);
		if (collision->type == _collision_result_object)
			object_attach_to_node(collision->object_index, projectile_index, collision->node_index);

		if (TEST_FLAG(definition->projectile.flags, _projectile_detonation_max_time_if_attached_bit))
		{
			real detonation_ticks = definition->projectile.timer_upper_bound * TICKS_PER_SECOND;

			if (detonation_ticks >= 1.0f)
				projectile->projectile.detonation_timer_delta = 1.0f / detonation_ticks;
		}
		break;

	default:
		match_vassert(
			"c:\\halo\\SOURCE\\items\\projectiles.c",
			1487,
			FALSE,
			NULL);
		break;
	}

	return;
}

real projectile_get_ballistic_acceleration(
	struct projectile_definition const *definition)
{
	return -(definition->projectile.air_gravity_scale * global_gravity);
}

real projectile_estimate_time_to_target(
	struct projectile_definition const *definition,
	real target_distance)
{
	real time_to_target = 0.0f;

	if (definition->projectile.initial_velocity > 0.0f)
		time_to_target = target_distance / definition->projectile.initial_velocity;

	return time_to_target;
}

boolean projectile_aim_ballistic(
	real base_velocity,
	real gravity_scale,
	real_point3d const *origin,
	real_point3d const *target_point,
	real *target_velocity_min,
	real *target_ballistic_fraction_min,
	real *forced_velocity,
	boolean lob,
	real_vector3d *result_aim_vector,
	real *result_velocity,
	real *result_ticks,
	real *result_distance,
	real *result_vertical_velocity,
	real *result_horizontal_velocity)
{
	boolean result = TRUE;
	boolean solution = FALSE;
	real_vector3d delta;
	real_vector3d aim_vector;
	real acceleration, a, b, c;
	real acceleration_height;
	real horizontal_distance_squared;
	real t_squared_max, t_max, v_min, t;
	real v_max = base_velocity;
	real v_desired, v_desired_sq;
	real distance, vertical_velocity, horizontal_velocity;

	delta.i = target_point->x - origin->x;
	delta.j = target_point->y - origin->y;
	delta.k = target_point->z - origin->z;

	horizontal_distance_squared = delta.i * delta.i + delta.j * delta.j;
	acceleration = MAX(0.f, global_gravity * gravity_scale);
	a = acceleration * acceleration * 0.25f;
	c = horizontal_distance_squared + delta.k * delta.k;

	match_assert(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		760,
		4.0f * a * c > 0.0f);
	b = -square_root(4.0f * a * c);
	t_squared_max = -(b / (2.0f * a));
	match_assert(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		764,
		t_squared_max >= 0.0f);
	t_max = square_root(t_squared_max);
	acceleration_height = acceleration * delta.k;
	v_min = acceleration_height - b < 0.0f
		? 0.0f
		: square_root(acceleration_height - b);

	if (forced_velocity)
	{
		v_desired = *forced_velocity;
	}
	else
	{
		v_desired = v_max;

		if (target_ballistic_fraction_min)
		{
			if (*target_ballistic_fraction_min > 0.0f)
			{
				real t_desired = t_max * *target_ballistic_fraction_min;
				real t_desired_squared = t_desired * t_desired;
				real b_desired =
					-(c / t_desired_squared + a * t_desired_squared);

				v_desired_sq = acceleration_height - b_desired;
				match_assert(
					"c:\\halo\\SOURCE\\items\\projectiles.c",
					806,
					v_desired_sq > 0.0f);
				if (v_max > square_root(v_desired_sq))
					v_desired = square_root(v_desired_sq);
			}
		}
	}

	if (v_desired >= v_min)
	{
		real b_desired = acceleration_height - v_desired * v_desired;
		real discriminant = b_desired * b_desired - 4.0f * a * c;

		if (b_desired < 0.0f && discriminant >= 0.0f)
		{
			real t_squared =
				(square_root(discriminant) * (lob ? 1 : -1) - b_desired) /
				(2.0f * a);

			if (t_squared > 0.0f)
			{
				t = square_root(t_squared);
				solution = TRUE;
			}
		}
	}

	if (!solution)
	{
		result = FALSE;
		t = t_max;
		v_desired = v_min;
	}

	aim_vector.i = delta.i / t;
	aim_vector.j = delta.j / t;
	aim_vector.k =
		delta.k / t + t * acceleration * 0.5f;
	horizontal_velocity = square_root(
		aim_vector.i * aim_vector.i + aim_vector.j * aim_vector.j);
	vertical_velocity = aim_vector.k;
	distance = t * v_desired;

	if (normalize3d(&aim_vector) == 0.0f)
	{
		result = FALSE;
		aim_vector = delta;
		if (normalize3d(&aim_vector) == 0.0f)
			aim_vector = *global_up3d;
	}

	match_assert(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		867,
		result_aim_vector);
	*result_aim_vector = aim_vector;

	if (result_distance)
		*result_distance = distance;
	if (result_velocity)
		*result_velocity = v_desired;
	if (result_vertical_velocity)
		*result_vertical_velocity = vertical_velocity;
	if (result_horizontal_velocity)
		*result_horizontal_velocity = horizontal_velocity;
	if (result_ticks)
		*result_ticks = t;

	return result;
}

boolean projectile_aim_linear(
	real base_velocity,
	real_point3d const *origin,
	real_point3d const *target_point,
	real_vector3d *result_aim_vector,
	real *result_velocity,
	real *result_ticks,
	real *result_distance)
{
	real_vector3d aim_vector;
	real distance;
	real ticks;

	aim_vector.i = target_point->x - origin->x;
	aim_vector.j = target_point->y - origin->y;
	aim_vector.k = target_point->z - origin->z;
	distance = normalize3d(&aim_vector);

	if (base_velocity > 0.0f)
		ticks = distance / base_velocity;
	else
		ticks = 0.0f;

	match_assert(
		"c:\\halo\\SOURCE\\items\\projectiles.c",
		921,
		result_aim_vector);
	*result_aim_vector = aim_vector;

	if (result_distance)
		*result_distance = distance;
	if (result_velocity)
		*result_velocity = base_velocity;
	if (result_ticks)
		*result_ticks = ticks;

	return TRUE;
}

boolean projectile_aim(
	struct projectile_definition const *definition,
	real_point3d const *origin,
	real_point3d const *target_point,
	real const *override_velocity_max,
	real *target_velocity_min,
	real *target_ballistic_fraction_min,
	real *forced_velocity,
	boolean lob,
	real_vector3d *result_aim_vector,
	real *result_velocity,
	real *result_ticks,
	real *result_distance,
	boolean *result_linear)
{
	real base_velocity;
	boolean result;

	if (!override_velocity_max)
		base_velocity = definition->projectile.initial_velocity;
	else
		base_velocity = *override_velocity_max;

	if (TEST_FLAG(definition->projectile.flags, _projectile_aim_ballistic_bit) &&
		definition->projectile.air_gravity_scale > 0.0f)
	{
		result = projectile_aim_ballistic(
			base_velocity,
			definition->projectile.air_gravity_scale,
			origin,
			target_point,
			target_velocity_min,
			target_ballistic_fraction_min,
			forced_velocity,
			lob,
			result_aim_vector,
			result_velocity,
			result_ticks,
			result_distance,
			NULL,
			NULL);

		if (result_linear)
			*result_linear = FALSE;
	}
	else
	{
		result = projectile_aim_linear(
			base_velocity,
			origin,
			target_point,
			result_aim_vector,
			result_velocity,
			result_ticks,
			result_distance);

		if (result_linear)
			*result_linear = TRUE;
	}

	return result;
}
