/*
FLYING_CAMERA.C

symbols in this file:
000779A0 0020:
	_flying_camera_new (0000)
000779C0 0050:
	_flying_camera_new_from_point_and_vector (0000)
00077A10 04b0:
	_flying_camera_update (0000)
00256E34 0026:
	??_C@_0CG@FECMEHCO@c?3?2halo?2SOURCE?2camera?2flying_cam@ (0000)
*/

/* ---------- headers */

#include "flying_camera.h"
#include "observer.h"
#include "static_camera.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct rasterizer_debug_options
{
	byte reserved[0x5E];
	short flying_camera_reset_ticks;
	byte trailing[8];
};

/* ---------- prototypes */

/* ---------- globals */

extern struct rasterizer_debug_options rasterizer_debug_options;

/* ---------- public code */

void flying_camera_new(
	struct flying_camera *camera)
{
	camera->position.y = 0.f;
	camera->position.x = 0.f;
	camera->facing.yaw = 0.f;
	camera->facing.pitch = 0.f;
	camera->roll = 0.f;
	camera->field_of_view = DEGREES_TO_RADIANS(70);
	return;
}

void flying_camera_new_from_point_and_vector(
	struct flying_camera *camera,
	real_point3d const *position,
	real_vector3d const *forward)
{
	flying_camera_new(camera);
	camera->position = *position;
	euler_angles2d_from_vector3d(&camera->facing, forward);
	return;
}

void flying_camera_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result)
{
	match_assert("c:\\halo\\SOURCE\\camera\\flying_camera.c", 41, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\flying_camera.c", 42, controls);
	match_assert("c:\\halo\\SOURCE\\camera\\flying_camera.c", 43, result);

	if (controls->active)
	{
		camera->facing.yaw += controls->facing_delta.yaw;
		camera->facing.pitch = PIN(
			camera->facing.pitch + controls->facing_delta.pitch,
			-1.56765485f,
			1.56765485f);
		camera->roll += controls->facing_delta.roll;
	}

	if (rasterizer_debug_options.flying_camera_reset_ticks > 0)
	{
		camera->facing.yaw = 0.f;
		camera->facing.pitch = 0.f;
		camera->roll = 0.f;
		rasterizer_debug_options.flying_camera_reset_ticks--;
	}

	result->timer = 0.3f;
	vector3d_from_euler_angles2d(&result->forward, &camera->facing);
	observer_up_from_forward(&result->forward, &result->up);
	rotate_vector_about_axis(
		&result->up,
		&result->forward,
		sine(camera->roll),
		cosine(camera->roll));

	if (controls->active)
	{
		real cosine_yaw = cosine(camera->facing.yaw);
		real sine_yaw = sine(camera->facing.yaw);
		real_vector3d translation;
		real_point3d position;

		set_real_vector3d(
			&translation,
			cosine_yaw*controls->translation.i - sine_yaw*controls->translation.j,
			cosine_yaw*controls->translation.j + sine_yaw*controls->translation.i,
			controls->translation.k);
		position.x = camera->position.x + translation.i;
		position.y = camera->position.y + translation.j;
		position.z = camera->position.z + translation.k;
		camera->position = position;
	}

	result->position = camera->position;
	result->offset = *global_zero_vector3d;
	result->depth = 0.f;
	result->field_of_view = camera->field_of_view;
	result->flags = FLAG(_observer_command_valid_bit);

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\flying_camera.c",
		149,
		!TEST_FLAG(result->flags, _observer_command_valid_bit) ||
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

/* ---------- private code */
