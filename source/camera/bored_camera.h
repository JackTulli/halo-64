/*
BORED_CAMERA.H

header included in hcex build.
*/

#ifndef __BORED_CAMERA_H
#define __BORED_CAMERA_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bored_camera
{
	unsigned long last_update_milliseconds;
	long timer_milliseconds;
	long boredom_count;
};

struct camera_action;
struct camera_command;

/* ---------- prototypes/BORED_CAMERA.C */

void bored_camera_new(
	struct bored_camera *camera);
void bored_camera_update(
	struct bored_camera *camera,
	struct camera_action const *action,
	struct camera_command *result);

boolean is_bored(
	void);

boolean is_still_bored(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __BORED_CAMERA_H
