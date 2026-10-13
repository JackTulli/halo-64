/*
SOUND_MANAGER.H

header included in hcex build.
*/

#ifndef __SOUND_MANAGER_H
#define __SOUND_MANAGER_H
#pragma once

/* ---------- constants */

enum looping_sound_refresh_state
{
	_looping_sound_refresh_start,
	_looping_sound_refresh_loop,
	_looping_sound_refresh_stop,
	NUMBER_OF_LOOPING_SOUND_REFRESH_STATES,
};

/* ---------- macros */

/* ---------- structures */

struct sound_source;
struct sound_environment_definition;

/* ---------- prototypes/SOUND_MANAGER.C */

boolean sound_valid_for_channel(
	short compression,
	short encoding,
	short sample_rate,
	short spatialization_mode,
	short channel_type_flags);

void sound_dispose(
	void);

void sound_initialize(
	void);
void sound_initialize_for_new_map(
	void);
void sound_dispose_from_old_map(
	void);
void sound_reconnect_to_structure_bsp(
	void);

long sound_render_time(
	void);
void sound_render(
	void);

void sound_stop_all(
	void);
void sound_stop_impulse(
	long sound_index);
void sound_stop_impulse_by_source_and_definition(
	long source_identifier,
	long definition_index);
long sound_new_impulse(
	long definition_index,
	struct sound_source *source,
	long source_identifier,
	boolean (*track_proc)(
		long source_identifier,
		void const *track_data,
		struct sound_source *source),
	void const *track_data,
	short track_data_size);
void sound_pause(
	boolean paused);
void sound_idle(
	void);
void sound_manager_set_sound_environment(
	struct sound_environment_definition const *environment);
boolean sound_refresh_looping(
	long definition_index,
	long looping_sound_index,
	struct sound_source *source,
	short refresh_state,
	boolean alternate,
	real fade_time);

/* ---------- globals */

extern boolean debug_sound;

/* ---------- public code */

#endif // __SOUND_MANAGER_H
