#ifndef __ACTOR_PLACEMENT_H
#define __ACTOR_PLACEMENT_H
#pragma once

#include "cseries.h"

struct actor_starting_location;

long actor_place(
	long actor_variant_definition_index,
	long encounter_index,
	long squad_index,
	struct actor_starting_location *starting_location,
	boolean upgrade_major,
	short initial_variant);

#endif // __ACTOR_PLACEMENT_H
