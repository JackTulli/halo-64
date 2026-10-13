/*
DEAD_CAMERA.H

header included in hcex build.
*/

#ifndef __DEAD_CAMERA_H
#define __DEAD_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct dead_camera
{
	real_point3d position;
	real_euler_angles2d facing;
	real distance;
	real field_of_view;
	real timer;
	long player_index;
	long current_player_index;
	long unit_index;
	real switch_timer;
};

typedef char dead_camera_size_assert[
	sizeof(struct dead_camera) == 0x30 ? 1 : -1];

struct camera_control;
struct dead_camera_command;

/* ---------- prototypes/DEAD_CAMERA.C */

void dead_camera_new(
	struct dead_camera *camera,
	short local_player_index,
	long unit_index);
void dead_camera_update(
	struct dead_camera *camera,
	struct camera_control const *controls,
	struct dead_camera_command *result);

/* ---------- globals */

/* ---------- public code */

#endif // __DEAD_CAMERA_H
