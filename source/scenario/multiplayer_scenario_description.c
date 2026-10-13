/*
MULTIPLAYER_SCENARIO_DESCRIPTION.C

symbols in this file:
0017D990 0090:
	_multiplayer_scenario_description_get_list (0000)
0017DA20 0070:
	_map_name_from_multiplayer_scenario_description_item (0000)
002A04E8 000e:
	??_C@_0O@BBNKPBIN@scenario_list?$AA@ (0000)
002A04F8 003b:
	??_C@_0DL@KHFPDEF@c?3?2halo?2SOURCE?2scenario?2multipla@ (0000)
002A0534 0006:
	??_C@_05BMGBEOOC@?$CFs?2?$CFs?$AA@ (0000)
*/

/* ---------- headers */

#include <stdio.h>

#include "cseries.h"
#include "scenario/multiplayer_scenario_description.h"

#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

struct multiplayer_scenario_description_item *multiplayer_scenario_description_get_list(
	short *count)
{
	long scenario_list_index;
	struct multiplayer_scenario_description_item *result;

	match_assert("c:\\halo\\SOURCE\\scenario\\multiplayer_scenario_description.c", 60, count);
	scenario_list_index = tag_loaded(MULTIPLAYER_SCENARIO_DESCRIPTION_TAG, "ui\\multiplayer_scenarios");
	if (scenario_list_index != NONE)
	{
		struct multiplayer_scenario_description *scenario_list =
			multiplayer_scenario_description_get(scenario_list_index);

		match_assert("c:\\halo\\SOURCE\\scenario\\multiplayer_scenario_description.c", 66, scenario_list);
		result = scenario_list->scenarios.address;
		*count = (short)scenario_list->scenarios.count;
	}
	else
	{
		result = NULL;
		*count = 0;
	}

	return result;
}

boolean map_name_from_multiplayer_scenario_description_item(
	struct multiplayer_scenario_description_item const *item,
	char *map_name,
	unsigned long map_name_size)
{
	char const *scenario_tag_path;
	long character_index;

	match_assert("c:\\halo\\SOURCE\\scenario\\multiplayer_scenario_description.c", 88, item);
	scenario_tag_path = item->scenario_tag_path;
	*map_name = 0;
	character_index = csstrlen(scenario_tag_path);
	if (character_index >= 0)
	{
		do
		{
			if (item->scenario_tag_path[character_index] == '\\')
			{
				_snprintf(
					map_name,
					map_name_size,
					"%s\\%s",
					scenario_tag_path,
					&item->scenario_tag_path[character_index + 1]);
				break;
			}
			character_index--;
		}
		while (character_index >= 0);
	}

	return TRUE;
}

/* ---------- private code */
