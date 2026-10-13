/*
WEATHER_PARTICLE_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __WEATHER_PARTICLE_DEFINITIONS_H
#define __WEATHER_PARTICLE_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	WEATHER_PARTICLE_SYSTEM_DEFINITION_TAG = 'rain',
};

/* ---------- macros */

#define weather_particle_system_definition_get(index) ((struct weather_particle_system_definition *)tag_get(WEATHER_PARTICLE_SYSTEM_DEFINITION_TAG, (index)))

/* ---------- structures */

struct weather_particle_system_definition
{
	unsigned long flags;
	long unused[8];
	struct tag_block particle_types;
};

typedef char weather_particle_system_definition_size_assert[
	sizeof(struct weather_particle_system_definition) == 0x30 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __WEATHER_PARTICLE_DEFINITIONS_H
