/*
FLYING_CAMERA.H

header included in hcex build.
*/

#ifndef __FLYING_CAMERA_H
#define __FLYING_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct flying_camera
{
	real_point3d position;
	real_euler_angles2d facing;
	real roll;
	real field_of_view;
};

struct flying_camera_action
{
	short local_player_index;
	boolean active;
	byte pad3[5];
	real_euler_angles3d facing_delta;
	real_vector3d translation;
	real wheel_delta;
};

struct camera_command;

/* ---------- prototypes/FLYING_CAMERA.C */

void flying_camera_new(
	struct flying_camera *camera);
void flying_camera_new_from_point_and_vector(
	struct flying_camera *camera,
	real_point3d const *position,
	real_vector3d const *forward);
void flying_camera_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result);

/* ---------- globals */

/* ---------- public code */

#endif // __FLYING_CAMERA_H
