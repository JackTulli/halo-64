/*
MULTIPLAYER_SCENARIO_DESCRIPTION.H

header included in hcex build.
*/

#ifndef __MULTIPLAYER_SCENARIO_DESCRIPTION_H
#define __MULTIPLAYER_SCENARIO_DESCRIPTION_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MULTIPLAYER_SCENARIO_DESCRIPTION_TAG = 'mply',
};

/* ---------- macros */

#define multiplayer_scenario_description_get(index) ((struct multiplayer_scenario_description *)tag_get(MULTIPLAYER_SCENARIO_DESCRIPTION_TAG, (index)))

/* ---------- structures */

struct multiplayer_scenario_description_item
{
	struct tag_reference descriptive_bitmap;
	struct tag_reference displayed_map_name;
	char scenario_tag_path[1];
};

struct multiplayer_scenario_description
{
	struct tag_block scenarios;
};

typedef char multiplayer_scenario_description_item_path_offset_assert[
	offsetof(struct multiplayer_scenario_description_item, scenario_tag_path) == 0x20 ? 1 : -1];

/* ---------- prototypes/MULTIPLAYER_SCENARIO_DESCRIPTION.C */

struct multiplayer_scenario_description_item *multiplayer_scenario_description_get_list(
	short *count);
boolean map_name_from_multiplayer_scenario_description_item(
	struct multiplayer_scenario_description_item const *item,
	char *map_name,
	unsigned long map_name_size);

/* ---------- globals */

/* ---------- public code */

#endif // __MULTIPLAYER_SCENARIO_DESCRIPTION_H
