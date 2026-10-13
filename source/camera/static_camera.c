/*
STATIC_CAMERA.C

symbols in this file:
0007BA10 0070:
	_static_camera_new (0000)
0007BA80 0420:
	_static_camera_update (0000)
00257288 0026:
	??_C@_0CG@CBDIIJJN@c?3?2halo?2SOURCE?2camera?2static_cam@ (0000)
*/

/* ---------- headers */

#include "static_camera.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void static_camera_new(
	struct static_camera *camera,
	real_point3d const *position,
	long location,
	real_vector3d const *forward,
	real_vector3d const *up,
	real field_of_view,
	long timer,
	long flags)
{
	camera->position = *position;
	camera->location = location;
	camera->forward = *forward;
	camera->up = *up;
	camera->field_of_view = field_of_view;
	camera->timer = timer;
	camera->initialized = FALSE;
	camera->flags = flags;
	return;
}

void static_camera_update(
	struct static_camera *camera,
	void const *action,
	struct camera_command *result)
{
	match_assert("c:\\halo\\SOURCE\\camera\\static_camera.c", 36, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\static_camera.c", 37, action);
	match_assert("c:\\halo\\SOURCE\\camera\\static_camera.c", 38, result);

	if (!camera->initialized)
	{
		result->position = camera->position;
		result->forward = camera->forward;
		result->up = camera->up;
		result->field_of_view = camera->field_of_view;
		result->timer = (real)camera->timer;
		result->velocity.i = result->velocity.j = result->velocity.k = 0.f;
		result->flags = camera->flags | FLAG(0);
		result->offset = *global_zero_vector3d;
		camera->initialized = TRUE;

		match_vassert(
			"c:\\halo\\SOURCE\\camera\\static_camera.c",
			53,
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
	}

	return;
}

/* ---------- private code */
