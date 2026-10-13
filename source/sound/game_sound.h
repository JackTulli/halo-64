/*
GAME_SOUND.H

header included in hcex build.
*/

#ifndef __GAME_SOUND_H
#define __GAME_SOUND_H
#pragma once

/* ---------- headers */

#include "objects/objects.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct sound_source;

struct sound_location
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d translational_velocity;
	struct location game_location;
};

/* ---------- prototypes/GAME_SOUND.C */

void game_sound_initialize(
	void);
void game_sound_dispose(
	void);
void game_sound_initialize_for_new_map(
	void);
void game_sound_dispose_from_old_map(
	void);
void game_sound_clear(
	void);
void game_sound_restore(
	void);
void game_sound_update(
	real dt);
long game_looping_sound_new(
	long object_index,
	long definition_index,
	char const *marker_name,
	short function_index);
void game_looping_sound_delete(
	long looping_sound_index);
long object_impulse_sound_new(
	long object_index,
	long definition_index,
	short node_index,
	real_point3d const *position,
	real_vector3d const *forward,
	real scale);
long unattached_impulse_sound_new(
	long definition_index,
	struct sound_location const *location,
	real scale);
long unspatialized_impulse_sound_new(
	long definition_index,
	real scale);
long unattached_looping_sound_start(
	long definition_index,
	long source_object_index,
	real scale);
void unattached_looping_sound_stop(
	long looping_sound_index);
void scripted_sound_new(
	long definition_index,
	long source_object_index,
	real scale);
void scripted_sound_stop(
	long sound_index);
long scripted_sound_time(
	long sound_index);
void scripted_foley_predict(
	long definition_index);
void scripted_looping_sound_start(
	long sound_index,
	long source_object_index,
	real gain);
void scripted_looping_sound_stop(
	long sound_index);
void scripted_looping_sound_set_scale(
	long sound_index,
	real scale);
void scripted_looping_sound_set_alternate(
	long sound_index,
	boolean alternate);
void compute_sound_obstruction(
	short local_player_index,
	struct sound_source *source,
	real distance);
void game_sound_set_mouth_aperture(
	long object_index,
	real mouth_aperture);
boolean track_object_impulse_sound(
	long object_index,
	void const *attachment_data,
	struct sound_source *source);

/* ---------- globals */

/* ---------- public code */

#endif // __GAME_SOUND_H
