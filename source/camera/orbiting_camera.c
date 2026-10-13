/*
ORBITING_CAMERA.C

symbols in this file:
0007B580 0020:
	_orbiting_camera_new (0000)
0007B5A0 0470:
	_orbiting_camera_update (0000)
0025724C 0014:
	_rdata_0025724c (0000)
00257260 0028:
	??_C@_0CI@JLFPHGKF@c?3?2halo?2SOURCE?2camera?2orbiting_c@ (0000)
*/

/* ---------- headers */

#include "orbiting_camera.h"
#include "observer.h"
#include "static_camera.h"
#include "camera/director.h"
#include "objects/objects.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct camera_action
{
	short local_player_index;
	boolean inhibit_input;
	byte pad3[5];
	real yaw_delta;
	real pitch_delta;
	byte pad10[0x10];
	real zoom_delta;
};

struct unit_camera_info
{
	long unit_index;
	byte pad4[8];
	real_point3d position;
};

struct orbiting_camera_constants
{
	real field_of_view;
	real unknown4;
	real unknown8;
	real timer;
	real vertical_offset;
};

/* ---------- prototypes */

void player_control_get_unit_camera_info(
	short local_player_index,
	struct unit_camera_info *camera_info);

/* ---------- globals */

static struct orbiting_camera_constants const rdata_0025724c =
{
	DEGREES_TO_RADIANS(50.f),
	1.f,
	0.5f,
	0.5f,
	0.52f
};

/* ---------- public code */

void orbiting_camera_new(
	struct orbiting_camera *camera,
	real distance,
	real_vector3d const *forward)
{
	camera->distance = distance;
	euler_angles2d_from_vector3d(&camera->facing, forward);
	return;
}

void orbiting_camera_update(
	struct orbiting_camera *camera,
	struct camera_action const *action,
	struct camera_command *result)
{
	struct unit_camera_info camera_info;

	player_control_get_unit_camera_info(action->local_player_index, &camera_info);
	result->position = camera_info.position;

	if (action->inhibit_input)
	{
		camera->facing.yaw -= action->yaw_delta;
		camera->facing.pitch = PIN(
			camera->facing.pitch - action->pitch_delta,
			-DEGREES_TO_RADIANS(72.f),
			DEGREES_TO_RADIANS(72.f));
		director_inhibit_input(action->local_player_index);
	}

	camera->distance = MAX(camera->distance - action->zoom_delta / 3.f, 0.6f);

	if (camera_info.unit_index != NONE)
	{
		vector3d_from_euler_angles2d(&result->forward, &camera->facing);
		observer_up_from_forward(&result->forward, &result->up);
		object_get_velocities(camera_info.unit_index, &result->velocity, NULL);
		result->position.z += rdata_0025724c.vertical_offset;
		result->flags = FLAG(0);
	}

	result->offset = *global_zero_vector3d;
	result->depth = camera->distance;
	result->field_of_view = rdata_0025724c.field_of_view;
	result->timer = rdata_0025724c.timer;

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\orbiting_camera.c",
		71,
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
			valid_real(rdata_0025724c.field_of_view) && rdata_0025724c.field_of_view>=0.001f && rdata_0025724c.field_of_view<=_pi / 2.f &&
			valid_real(rdata_0025724c.timer) && rdata_0025724c.timer>=0.f && rdata_0025724c.timer<=3600.f),
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
			rdata_0025724c.field_of_view,
			rdata_0025724c.timer,
			result->flags));

	return;
}

/* ---------- private code */
