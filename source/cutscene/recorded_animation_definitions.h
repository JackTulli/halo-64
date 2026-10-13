/*
RECORDED_ANIMATION_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __RECORDED_ANIMATION_DEFINITIONS_H
#define __RECORDED_ANIMATION_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct scenario;

struct recorded_animation_definition
{
	char name[TAG_STRING_LENGTH+1];
	byte version;
	char raw_animation_data;
	byte unit_control_data_version;
	byte pad;
	short length_in_ticks;
	word pad2;
	unsigned long pad3;
	struct tag_data event_stream;
};

/* ---------- prototypes/RECORDED_ANIMATION_DEFINITIONS.C */

short scenario_get_animation_by_name(struct scenario const *scenario, char const *name);

/* ---------- globals */

/* ---------- public code */

#endif // __RECORDED_ANIMATION_DEFINITIONS_H
