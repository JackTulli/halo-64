/*
SOUND_ENVIRONMENT_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __SOUND_ENVIRONMENT_DEFINITIONS_H
#define __SOUND_ENVIRONMENT_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"

/* ---------- constants */

enum
{
	SOUND_ENVIRONMENT_TAG = 'snde',
};

/* ---------- macros */

#define sound_environment_definition_get(index) \
	((struct sound_environment_definition *)tag_get(SOUND_ENVIRONMENT_TAG, (index)))


/* ---------- structures */

struct sound_environment_definition
{
	long unknown;
	short priority;
	short unused0;
	real room_intensity;
	real room_intensity_hf;
	real room_rolloff;
	real decay_time;
	real decay_hf_ratio;
	real reflections_intensity;
	real reflections_delay;
	real reverb_intensity;
	real reverb_delay;
	real diffusion;
	real density;
	real hf_reference;
	byte unused1[16];
};

typedef char verify_sound_environment_definition_size[
	sizeof(struct sound_environment_definition) == 0x48 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

extern struct sound_environment_definition const default_sound_environment;

/* ---------- public code */

#endif // __SOUND_ENVIRONMENT_DEFINITIONS_H
