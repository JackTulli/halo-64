/*
FOLLOWING_CAMERA.C

symbols in this file:
00077EC0 0060:
	_following_camera_new (0000)
00077F20 0080:
	_code_00077f20 (0000)
00077FA0 0010:
	_arcsine (0000)
00077FB0 00e0:
	_uniform_cubic_spline (0000)
00078090 0090:
	_uniform_cubic_spline_vector3d (0000)
00078120 0150:
	_code_00078120 (0000)
00078270 00d0:
	_following_camera_deterministic (0000)
00078340 0630:
	_following_camera_update (0000)
00256E5C 0029:
	??_C@_0CJ@CPNCEFIH@c?3?2halo?2SOURCE?2camera?2following_@ (0000)
00256E88 001c:
	??_C@_0BM@HKHJJNLK@t?5?$DO?$DN?5t0?5?$CG?$CG?5t?5?$DM?$DN?5t0?5?$CL?53?40f?$CKh?$AA@ (0000)
00256EA4 0009:
	??_C@_08OMLDILI@h?5?$DO?50?40f?$AA@ (0000)
00256EB0 0028:
	??_C@_0CI@JCGGGL@camera_track?9?$DOcontrol_points?4cou@ (0000)
00256ED8 0004:
	__real@3ea2f983 (0000)
00256EE0 0053:
	??_C@_0FD@PCPIEKIH@magnitude3d?$CI?$CGresult?9?$DOforward?$CJ?5?$DO?5@ (0000)
00256F34 0004:
	__real@3f800347 (0000)
00256F38 0004:
	__real@3f7ff972 (0000)
002DCC5C 0010:
	_following_camera_zoom_levels (0000)
*/

/* ---------- headers */

#include "following_camera.h"
#include "observer.h"

#include "static_camera.h"

#include "game/game_globals.h"
#include "game/player_control.h"
#include "objects/objects.h"
#include "scenario/scenario.h"
#include "tag_files/tag_groups.h"
#include "units/unit_definitions.h"
#include "units/units.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

/* ---------- constants */

enum
{
	CAMERA_TRACK_DEFINITION_TAG = 'trak'
};

/* ---------- macros */

/* ---------- structures */

struct following_camera_control
{
	short local_player_index;
	boolean active;
	byte pad3[5];
	real_euler_angles2d facing_delta;
};

struct following_camera_result
{
	struct camera_command command;
	byte parameter_flags[5];
	byte pad51[3];
	real parameter_timers[5];
};

struct camera_track_control_point
{
	real_vector3d position;
	real_quaternion orientation;
	long unused[8];
};

struct camera_track_definition
{
	unsigned long flags;
	struct tag_block control_points;
	long unused[8];
};

struct unit_camera_track
{
	struct tag_reference track;
	long unused[3];
};

typedef char camera_track_control_point_size_assert[
	sizeof(struct camera_track_control_point) == 0x3C ? 1 : -1];

typedef char camera_track_definition_control_points_offset_assert[
	offsetof(struct camera_track_definition, control_points) == 0x4 ? 1 : -1];
typedef char unit_camera_track_size_assert[
	sizeof(struct unit_camera_track) == 0x1C ? 1 : -1];

/* ---------- prototypes */

static struct unit_camera const *unit_camera_get(
	long unit_index);
static void camera_track_splut(
	struct unit_camera const *camera,
	real pitch,
	real_vector3d *offset);

/* ---------- globals */

real following_camera_zoom_levels[4] =
{
	31.29f,
	12.78f,
	5.13f,
	2.05f
};

/* ---------- public code */

void following_camera_new(
	struct following_camera *camera)
{
	match_assert("c:\\halo\\SOURCE\\camera\\following_camera.c", 19, camera);
	camera->initialized = FALSE;
	camera->confined = FALSE;
	camera->crouched = FALSE;
	camera->zoomed = FALSE;
	camera->zoom_level = 0;
	camera->facing_offset.pitch = 0.f;
	camera->facing_offset.yaw = 0.f;
	camera->unit_index = NONE;
	camera->seat_index = NONE;
	camera->distance_scale = 1.f;
	return;
}

void following_camera_deterministic(
	long unit_index,
	real_point3d *position,
	real_vector3d *forward)
{
	struct unit_datum *unit;
	struct unit_camera const *camera;
	real_vector3d offset;
	real pitch;
	real forward_x;
	real forward_y;
	real horizontal_magnitude;

	unit = unit_get(unit_index);
	camera = unit_camera_get(unit_index);
	unit_get_camera_position(unit_index, position);
	*forward = unit->unit.aiming_vector;

	pitch = arcsine(forward->k);
	camera_track_splut(camera, pitch, &offset);

	forward_x = forward->i;
	forward_y = forward->j;
	horizontal_magnitude = square_root(
		forward_y * forward_y + forward_x * forward_x);
	if (!(0.0001f > fabs(horizontal_magnitude)))
	{
		horizontal_magnitude = 1.f / horizontal_magnitude;
		forward_x *= horizontal_magnitude;
		forward_y *= horizontal_magnitude;
	}

	position->x = offset.j * forward_y + offset.i * forward_x + position->x;
	position->y = offset.i * forward_y - offset.j * forward_x + position->y;
	position->z += offset.k;

	return;
}

void following_camera_update(
	struct following_camera *camera,
	struct following_camera_control const *controls,
	struct following_camera_result *result)
{
	struct player_control_unit_camera_info camera_info;
	struct camera_command *command;

	match_assert("c:\\halo\\SOURCE\\camera\\following_camera.c", 138, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\following_camera.c", 139, result);

	player_control_get_unit_camera_info(controls->local_player_index, &camera_info);
	command = &result->command;

	command->position = camera_info.position;
	command->timer = 0.f;
	command->flags = 0;
	command->field_of_view = DEGREES_TO_RADIANS(70.f);

	if (camera->initialized &&
		(camera_info.unit_index != camera->unit_index ||
			camera_info.seat_index != camera->seat_index))
	{
		command->timer = 1.f;
	}
	camera->unit_index = camera_info.unit_index;
	camera->seat_index = camera_info.seat_index;

	if (camera_info.camera)
	{
		struct unit_datum *unit;
		boolean crouched;
		real_euler_angles2d facing;
		real_vector3d track_offset;

		unit = unit_get(camera_info.unit_index);
		crouched =
			TEST_FLAG(unit->unit.control_flags, _unit_control_crouch_modifier_bit) ||
			TEST_FLAG(unit->unit.control_flags, _unit_control_jump_bit);
		if (crouched != camera->crouched)
		{
			result->parameter_flags[1] = TRUE;
			result->parameter_timers[1] = MAX(0.5f, result->parameter_timers[1]);
			camera->crouched = crouched;
		}

		if (controls->active)
		{
			camera->facing_offset.yaw += controls->facing_delta.yaw;
			camera->facing_offset.pitch += controls->facing_delta.pitch;
			result->parameter_flags[4] = TRUE;
			result->parameter_timers[4] = MAX(0.4f, result->parameter_timers[4]);
		}
		else if (camera->facing_offset.yaw != 0.f || camera->facing_offset.pitch != 0.f)
		{
			camera->facing_offset.pitch = 0.f;
			camera->facing_offset.yaw = 0.f;
		}

		facing = *player_control_get_facing_angles(controls->local_player_index);
		facing.yaw += camera->facing_offset.yaw;
		facing.pitch = PIN(
			facing.pitch + camera->facing_offset.pitch,
			-_pi / 2.f,
			_pi / 2.f);
		vector3d_from_euler_angles2d(&command->forward, &facing);

		match_vassert(
			"c:\\halo\\SOURCE\\camera\\following_camera.c",
			212,
			magnitude3d(&command->forward) > 0.9999f &&
			magnitude3d(&command->forward) < 1.0001f,
			"magnitude3d(&result->forward) > 0.9999f && magnitude3d(&result->forward) < 1.0001f");

		camera_track_splut(camera_info.camera, facing.pitch, &track_offset);
		command->depth = magnitude3d(&track_offset);
		command->offset.i =
			(command->depth * cosine(facing.pitch) + track_offset.i) * camera->distance_scale;
		command->offset.j = -track_offset.j * camera->distance_scale;
		command->offset.k =
			(command->depth * sine(facing.pitch) + track_offset.k) * camera->distance_scale;
		command->depth = MAX(
			(command->depth - 0.6f) * camera->distance_scale + 0.6f,
			0.6f);

		object_get_velocities(camera_info.unit_index, &command->velocity, NULL);
		SET_FLAG(command->flags, 0, TRUE);
	}

	observer_up_from_forward(&command->forward, &command->up);

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\following_camera.c",
		238,
		!(command->flags & FLAG(0)) ||
		(valid_real_vector3d_axes2(&command->forward, &command->up) &&
			valid_real(command->position.x) && command->position.x>=-5000.f && command->position.x<=5000.f &&
			valid_real(command->position.y) && command->position.y>=-5000.f && command->position.y<=5000.f &&
			valid_real(command->position.z) && command->position.z>=-5000.f && command->position.z<=5000.f &&
			valid_real(command->offset.i) && command->offset.i>=-5000.f && command->offset.i<=5000.f &&
			valid_real(command->offset.j) && command->offset.j>=-5000.f && command->offset.j<=5000.f &&
			valid_real(command->offset.k) && command->offset.k>=-5000.f && command->offset.k<=5000.f &&
			valid_real_vector3d(&command->velocity) &&
			valid_real(command->depth) && command->depth>=0.f && command->depth<=5000.f &&
			valid_real(command->field_of_view) && command->field_of_view>=0.001f && command->field_of_view<=_pi / 2.f &&
			valid_real(command->timer) && command->timer>=0.f && command->timer<=3600.f),
		csprintf(
			temporary,
			"Invalid camera command.\nF: (%f, %f, %f) U: (%f, %f, %f)\nP: (%f, %f, %f) O: (%f, %f, %f)\nD: %f V: (%f, %f, %f), FOV: %f, T: %f, FL: %ld",
			command->forward.i,
			command->forward.j,
			command->forward.k,
			command->up.i,
			command->up.j,
			command->up.k,
			command->position.x,
			command->position.y,
			command->position.z,
			command->offset.i,
			command->offset.j,
			command->offset.k,
			command->depth,
			command->velocity.i,
			command->velocity.j,
			command->velocity.k,
			command->field_of_view,
			command->timer,
			command->flags));

	camera->initialized = TRUE;
	return;
}

/* ---------- private code */

static struct unit_camera const *unit_camera_get(
	long unit_index)
{
	struct unit_datum *unit;
	struct unit_camera const *camera;

	unit = unit_get(unit_index);
	camera = NULL;
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
			if (seat->flags &
				(FLAG(_unit_seat_invisible_bit) |
					FLAG(_unit_seat_driver_bit) |
					FLAG(_unit_seat_third_person_camera_bit)))
			{
				camera = &seat->camera;
			}
		}
	}

	if (!camera)
		camera = &unit_definition_get(unit->definition_index)->unit.camera;

	return camera;
}

static void camera_track_splut(
	struct unit_camera const *camera,
	real pitch,
	real_vector3d *offset)
{
	long camera_track_index;
	struct camera_track_definition *camera_track;
	long control_point_count;
	long frame_index;
	real h;
	real t;
	short control_point_index;

	camera_track_index = NONE;
	if (camera->unit_camera_tracks.count)
	{
		struct unit_camera_track *track;
		long track_index;

		track_index = MIN(0, camera->unit_camera_tracks.count - 1);
		track = TAG_BLOCK_GET_ELEMENT(
			&camera->unit_camera_tracks,
			track_index,
			struct unit_camera_track);
		if (track)
			camera_track_index = track->track.index;
	}

	if (camera_track_index == NONE)
	{
		struct tag_reference *default_camera_track;

		default_camera_track = TAG_BLOCK_GET_ELEMENT(
			&scenario_get_game_globals()->camera,
			0,
			struct tag_reference);
		camera_track_index = default_camera_track->index;
	}

	camera_track = (struct camera_track_definition *)tag_get(
		CAMERA_TRACK_DEFINITION_TAG,
		camera_track_index);
	t = (pitch + _pi / 2.f) * (1.f / _pi);
	control_point_count = camera_track->control_points.count;
	frame_index = (long)((control_point_count - 1) * t);
	control_point_index = (short)frame_index;
	h = 1.f / (control_point_count - 1);

	match_assert(
		"c:\\halo\\SOURCE\\camera\\following_camera.c",
		86,
		camera_track->control_points.count >= 4);

	while (control_point_index > 0 &&
		(control_point_index + 4 > camera_track->control_points.count ||
			control_point_index > (short)frame_index - 1))
	{
		control_point_index--;
	}

	uniform_cubic_spline_vector3d(
		offset,
		&TAG_BLOCK_GET_ELEMENT(
			&camera_track->control_points,
			control_point_index,
			struct camera_track_control_point)->position,
		&TAG_BLOCK_GET_ELEMENT(
			&camera_track->control_points,
			control_point_index + 1,
			struct camera_track_control_point)->position,
		&TAG_BLOCK_GET_ELEMENT(
			&camera_track->control_points,
			control_point_index + 2,
			struct camera_track_control_point)->position,
		&TAG_BLOCK_GET_ELEMENT(
			&camera_track->control_points,
			control_point_index + 3,
			struct camera_track_control_point)->position,
		control_point_index * h,
		h,
		t);

	return;
}
