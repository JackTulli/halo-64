/*
SOUND_CLASSES.H

file has inline function assertions.
*/

#ifndef __SOUND_CLASSES_H
#define __SOUND_CLASSES_H
#pragma once

#include "real_math.h"
#include "objects.h"

/* ---------- constants */

enum
{
	MAXIMUM_SOUND_INSTANCES_PER_DEFINITION = 16,
	MAXIMUM_SOUND_INSTANCES_PER_OBJECT_PER_DEFINITION = 16
};

enum sound_class
{
	_sound_class_unit_dialog = 19,
	_sound_class_music = 32,
	_sound_class_scripted_dialog_to_player = 44,
	_sound_class_scripted_other = 45,
	_sound_class_scripted_dialog_to_other = 46,
	_sound_class_scripted_dialog_force_unspatialized = 47,
};

/* ---------- macros */

/* ---------- structures */

struct sound_class_definition
{
	short maximum_number_per_definition;
	short maximum_number_per_object;
	long preemption_time;
	boolean speech;
	byte pad_9;
	short priority;
	short cache_miss_mode;
	short pad_E;
	real wet_gain;
	real pad_14;
	real minimum_distance;
	real maximum_distance;
	real unknown_gain;
	real unknown_scale;
	boolean disabled;
	byte pad_29[3];
};

/* ---------- prototypes/SOUND_CLASSES.C */

void sound_classes_initialize(
	void);
void sound_classes_initialize_for_new_map(
	void);
struct sound_class_definition *sound_class_get(
	short class_index);

real sound_class_get_gain(
	short class_index);

void sound_classes_dispose_from_old_map(
	void);
void sound_classes_dispose(
	void);
void sound_classes_update(
	long ticks);

/* ---------- globals */

/* ---------- public code */

#endif // __SOUND_CLASSES_H
