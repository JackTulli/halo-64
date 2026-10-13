/*
FIRST_PERSON_CAMERA.C

symbols in this file:
000772B0 0040:
	_first_person_camera_new (0000)
000772F0 00d0:
	_first_person_camera_deterministic (0000)
000773C0 04f0:
	_code_000773c0 (0000)
000778B0 0030:
	_first_person_camera_fake (0000)
000778E0 00c0:
	_first_person_camera_update (0000)
00256DBC 002c:
	??_C@_0CM@BADBGAHA@c?3?2halo?2SOURCE?2camera?2first_pers@ (0000)
00256DE8 0010:
	??_C@_0BA@DGFIDHMB@primary?5trigger?$AA@ (0000)
00256DF8 0039:
	??_C@_0DJ@GGFKGPIO@valid_real_vector3d_axes2?$CI?$CGresul@ (0000)
*/

/* ---------- headers */

#include "first_person_camera.h"
#include "observer.h"
#include "static_camera.h"

#include "objects/objects.h"
#include "units/unit_definitions.h"
#include "units/units.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct first_person_camera_action
{
	short local_player_index;
};

struct first_person_camera_result
{
	struct camera_command command;
	byte reserved4C[3];
	boolean field_4F;
	byte reserved50[0x10];
	real transition_time;
};

/* ---------- prototypes */

long player_control_get_unit_index(
	short local_player_index);
void player_control_get_facing_direction(
	short local_player_index,
	real_vector3d *facing_direction);
real player_control_get_field_of_view(
	short local_player_index);

static void first_person_camera_for_unit_and_vector(
	long unit_index,
	real_vector3d const *forward,
	struct camera_command *result);

/* ---------- globals */

/* ---------- public code */

void first_person_camera_new(
	struct first_person_camera *camera)
{
	match_assert("c:\\halo\\SOURCE\\camera\\first_person_camera.c", 24, camera);
	camera->field_of_view = 0.f;
	return;
}

void first_person_camera_deterministic(
	long unit_index,
	real_point3d *position,
	real_vector3d *forward)
{
	struct unit_datum *unit;

	unit = unit_get(unit_index);
	unit_get_camera_position(unit_index, position);
	*forward = unit->unit.aiming_vector;

	if (unit->object.parent_object_index != NONE)
	{
		struct unit_datum *vehicle;

		vehicle = vehicle_try_and_get(unit->object.parent_object_index);
		if (vehicle)
		{
			struct unit_definition *definition;
			struct unit_seat *seat;

			definition = vehicle_definition_get(vehicle->definition_index);
			seat = TAG_BLOCK_GET_ELEMENT(
				&definition->unit.seats,
				unit->unit.parent_seat_index,
				struct unit_seat);

			if (TEST_FLAG(seat->flags, _unit_seat_first_person_camera_bit))
			{
				struct object_marker marker;

				if (object_get_marker_by_name(
					unit->object.parent_object_index,
					"primary trigger",
					&marker,
					1))
				{
					*position = marker.matrix.position;
					*forward = marker.matrix.forward;
				}
			}
		}
	}

	return;
}

void first_person_camera_fake(
	long unit_index,
	struct camera_command *result)
{
	struct unit_datum *unit;

	unit = unit_get(unit_index);
	first_person_camera_for_unit_and_vector(unit_index, &unit->unit.aiming_vector, result);
	return;
}

void first_person_camera_update(
	struct first_person_camera *camera,
	struct first_person_camera_action const *action,
	struct first_person_camera_result *result)
{
	long unit_index;
	real_vector3d facing_direction;

	unit_index = player_control_get_unit_index(action->local_player_index);
	match_assert("c:\\halo\\SOURCE\\camera\\first_person_camera.c", 157, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\first_person_camera.c", 158, result);
	player_control_get_facing_direction(action->local_player_index, &facing_direction);
	first_person_camera_for_unit_and_vector(unit_index, &facing_direction, &result->command);
	result->command.field_of_view = player_control_get_field_of_view(action->local_player_index);
	if (camera->field_of_view != result->command.field_of_view)
	{
		result->transition_time = 0.18f;
		result->field_4F = TRUE;
		camera->field_of_view = result->command.field_of_view;
	}

	return;
}

/* ---------- private code */

static void first_person_camera_for_unit_and_vector(
	long unit_index,
	real_vector3d const *forward,
	struct camera_command *result)
{
	result->timer = 0.f;
	result->flags = 0;
	result->offset = *global_zero_vector3d;
	result->depth = 0.f;
	result->forward = *forward;
	result->field_of_view = DEGREES_TO_RADIANS(70.f);
	observer_up_from_forward(&result->forward, &result->up);
	match_assert(
		"c:\\halo\\SOURCE\\camera\\first_person_camera.c",
		82,
		valid_real_vector3d_axes2(&result->forward, &result->up));

	if (unit_index != NONE)
	{
		struct unit_datum *unit;

		unit = unit_get(unit_index);
		unit_get_camera_position(unit_index, &result->position);
		object_get_velocities(unit_index, &result->velocity, NULL);
		if (unit->object.parent_object_index != NONE)
		{
			struct unit_datum *vehicle;

			vehicle = vehicle_try_and_get(unit->object.parent_object_index);
			if (vehicle)
			{
				struct unit_definition *definition;
				struct unit_seat *seat;

				definition = vehicle_definition_get(vehicle->definition_index);
				seat = TAG_BLOCK_GET_ELEMENT(
					&definition->unit.seats,
					unit->unit.parent_seat_index,
					struct unit_seat);

				if (TEST_FLAG(seat->flags, _unit_seat_first_person_camera_bit))
				{
					struct object_marker marker;

					if (object_get_marker_by_name(
						unit->object.parent_object_index,
						"primary trigger",
						&marker,
						1))
					{
						result->position = marker.matrix.position;
						result->forward = marker.matrix.forward;
						result->up = marker.matrix.up;
					}
				}
				else
				{
					real_matrix4x3 matrix;

					matrix4x3_from_point_and_vectors(
						&matrix,
						&vehicle->object.position,
						&vehicle->object.forward,
						&vehicle->object.up);
					matrix4x3_inverse_transform_normal(&matrix, &result->forward, &result->forward);
					observer_up_from_forward(&result->forward, &result->up);
					matrix4x3_transform_normal(&matrix, &result->forward, &result->forward);
					matrix4x3_transform_normal(&matrix, &result->up, &result->up);
				}
			}
		}

		result->flags = FLAG(0);
	}

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\first_person_camera.c",
		133,
		!(result->flags & FLAG(0)) ||
		(valid_real_vector3d_axes2(&result->forward, &result->up) &&
			valid_real(result->position.x) && result->position.x>=-5000.f && result->position.x<=5000.f &&
			valid_real(result->position.y) && result->position.y>=-5000.f && result->position.y<=5000.f &&
			valid_real(result->position.z) && result->position.z>=-5000.f && result->position.z<=5000.f &&
			valid_real(result->offset.i) && result->offset.i>=-5000.f && result->offset.i<=5000.f &&
			valid_real(result->offset.j) && result->offset.j>=-5000.f && result->offset.j<=5000.f &&
			valid_real(result->offset.k) && result->offset.k>=-5000.f && result->offset.k<=5000.f &&
			valid_real_vector3d(&result->velocity) &&
			valid_real(result->depth) && result->depth>=0.f && result->depth<=5000.f &&
			valid_real(result->field_of_view) && result->field_of_view>=0.001f && result->field_of_view<=_pi / 2.f &&
			valid_real(result->timer) && result->timer>=0.f && result->timer<=3600.f),
		csprintf(
			temporary,
			"Invalid camera command.\nF: (%f, %f, %f) U: (%f, %f, %f)\nP: (%f, %f, %f) O: (%f, %f, %f)\nD: %f V: (%f, %f, %f), FOV: %f, T: %f, FL: %ld",
			result->forward.i,
			result->forward.j,
			result->forward.k,
			result->up.i,
			result->up.j,
			result->up.k,
			result->position.x,
			result->position.y,
			result->position.z,
			result->offset.i,
			result->offset.j,
			result->offset.k,
			result->depth,
			result->velocity.i,
			result->velocity.j,
			result->velocity.k,
			result->field_of_view,
			result->timer,
			result->flags));

	return;
}
