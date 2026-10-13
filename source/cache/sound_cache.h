/*
SOUND_CACHE.H

header included in hcex build.
*/

#ifndef __SOUND_CACHE_H
#define __SOUND_CACHE_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct sound_permutation;

/* ---------- prototypes/XBOX_SOUND_CACHE.C */

void sound_cache_delete(
	void);
void sound_cache_open(
	void);
void sound_cache_idle(
	void);
void sound_cache_sound_new(
	long cache_tag_index,
	struct sound_permutation *sound);
void sound_cache_sound_delete(
	struct sound_permutation *sound);
void sound_cache_sound_finished(
	struct sound_permutation *sound);
void sound_cache_sound_hardware_lock(
	struct sound_permutation *sound);
void sound_cache_sound_hardware_unlock(
	struct sound_permutation *sound);
void sound_cache_new(
	void);
void sound_cache_flush(
	void);
void sound_cache_close(
	void);
boolean _sound_cache_sound_request(
	struct sound_permutation *sound,
	boolean block,
	boolean load,
	boolean reference);
void sound_cache_debug_render(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __SOUND_CACHE_H
