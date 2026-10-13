/*
SOUND_DSOUND.H

file has inline function assertions.
*/

#ifndef __SOUND_DSOUND_H
#define __SOUND_DSOUND_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct IDirectSound;
struct platform_sound_channel_properties;
struct platform_sound_listener_properties;
struct sound_location;
struct sound_permutation;
struct sound_platform_definition;
struct sound_preferences;

/* ---------- prototypes/SOUND_DSOUND_XBOX.C */

unsigned long sound_samples_per_second(
	short sample_rate);
long dsound_volume_from_gain(
	real gain,
	long maximum_volume);
long dsound_frequency_from_pitch(
	long samples_per_second,
	real pitch);
long dsound_angle_from_angle(
	real angle);
long dsound_occlusion_from_occlusion(
	real occlusion);
long dsound_obstruction_from_obstruction(
	real obstruction);
struct IDirectSound *dsound_get(
	void);
short dsound_virtual_get_state(
	short virtual_channel_index);
void dsound_virtual_stop(
	short virtual_channel_index);
void dsound_channel_update(
	short channel_index);
void dsound_begin_scene(
	void);
void dsound_end_scene(
	void);
void dsound_dispose(
	void);
void dsound_set_paused(
	boolean paused);
void dsound_flush(
	void);
void dsound_set_listener_properties(
	struct platform_sound_listener_properties const *properties);
boolean dsound_initialize(
	struct sound_preferences *preferences);
void dsound_virtual_set_location(
	short virtual_channel_index,
	boolean spatialized,
	struct sound_location const *location,
	real occlusion,
	real obstruction,
	boolean attenuate_direct_path);
void dsound_virtual_set_properties(
	short virtual_channel_index,
	struct platform_sound_channel_properties const *properties,
	boolean gain_only);
void dsound_virtual_queue_sound(
	short virtual_channel_index,
	struct sound_permutation *sound);

/* ---------- globals */

extern struct sound_platform_definition platform_sound_dsound;

/* ---------- public code */

#endif // __SOUND_DSOUND_H
