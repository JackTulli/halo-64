/*
FIRST_PERSON_CAMERA.H

header included in hcex build.
*/

#ifndef __FIRST_PERSON_CAMERA_H
#define __FIRST_PERSON_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct first_person_camera
{
	real field_of_view;
};

struct camera_command;
struct first_person_camera_action;
struct first_person_camera_result;

/* ---------- prototypes/FIRST_PERSON_CAMERA.C */

void first_person_camera_new(
	struct first_person_camera *camera);
void first_person_camera_deterministic(
	long unit_index,
	real_point3d *position,
	real_vector3d *forward);
void first_person_camera_fake(
	long unit_index,
	struct camera_command *result);
void first_person_camera_update(
	struct first_person_camera *camera,
	struct first_person_camera_action const *action,
	struct first_person_camera_result *result);

/* ---------- globals */

/* ---------- public code */

#endif // __FIRST_PERSON_CAMERA_H
