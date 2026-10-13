/*
EDITOR_FLYING_CAMERA.H

header included in hcex build.
*/

#ifndef __EDITOR_FLYING_CAMERA_H
#define __EDITOR_FLYING_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct flying_camera;
struct flying_camera_action;
struct camera_command;

/* ---------- prototypes/EDITOR_FLYING_CAMERA.C */

void editor_camera_new(
	struct flying_camera *camera,
	short local_player_index);
void editor_camera_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result);

void editor_camera_get_focus(
	real_point3d *position,
	real_euler_angles2d *angles);
void editor_camera_set_focus(
	real_point3d const *position,
	real_euler_angles2d const *angles);
void editor_camera_set_position(
	real_point3d const *point,
	real_euler_angles2d const *angles);
void editor_camera_move_to_point(
	real_point3d const *point);
void editor_camera_set_position_and_roll(
	real_point3d const *point,
	real_euler_angles3d const *angles);
void editor_camera_set_unit_focus(
	long unit_index);
void editor_camera_bump_speed(
	void);
boolean editor_camera_use_roll(
	boolean new_use_roll);
void editor_camera_set_mode(
	short mode);

long editor_camera_get_speed(
	void);

long editor_camera_get_unit_focus(
	void);

short editor_camera_get_mode(
	void);

boolean editor_camera_get_scripted(
	void);
void editor_camera_set_scripted(
	boolean scripted);

real editor_camera_get_field_of_view(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __EDITOR_FLYING_CAMERA_H
