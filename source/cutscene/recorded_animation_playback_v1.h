/*
RECORDED_ANIMATION_PLAYBACK_V1.H

header included in hcex build.
*/

#ifndef __RECORDED_ANIMATION_PLAYBACK_V1_H
#define __RECORDED_ANIMATION_PLAYBACK_V1_H
#pragma once

/* ---------- headers */

#include "units/unit_control_data.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/RECORDED_ANIMATION_PLAYBACK_V1.C */

void recorded_animation_initialize_event_stream_v1(
	void *animation_state,
	void *controller,
	byte **event_stream,
	byte unit_control_data_version);
boolean recorded_animation_apply_event_stream_v1(
	void *animation_state,
	struct unit_control_data *controller,
	long *relative_ticks,
	byte **event_stream);

void byte_swap_recording_stream_v1(void *stream, long stream_size, byte unit_control_data_version);

/* ---------- globals */

/* ---------- public code */

#endif // __RECORDED_ANIMATION_PLAYBACK_V1_H
