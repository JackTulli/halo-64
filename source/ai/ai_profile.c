/*
AI_PROFILE.C

symbols in this file:
00041C90 0020:
	_ai_profile_initialize (0000)
00041CB0 0010:
	_ai_profile_dispose (0000)
00041CC0 0020:
	_ai_profile_initialize_for_new_map (0000)
00041CE0 0010:
	_ai_profile_dispose_from_old_map (0000)
00041CF0 0110:
	_ai_profile_update (0000)
00041E00 0070:
	_ai_profile_display (0000)
00041E70 0090:
	_code_00041e70 (0000)
00041F00 0040:
	_ai_profile_change_render_spray (0000)
00041F40 0010:
	_code_00041f40 (0000)
00041F50 0010:
	_code_00041f50 (0000)
00041F60 0050:
	_code_00041f60 (0000)
00041FB0 0010:
	_code_00041fb0 (0000)
00041FC0 0010:
	_code_00041fc0 (0000)
00041FD0 0050:
	_code_00041fd0 (0000)
00042020 0010:
	_code_00042020 (0000)
00042030 0060:
	_code_00042030 (0000)
00042090 0070:
	_code_00042090 (0000)
00042100 0060:
	_code_00042100 (0000)
00042160 0090:
	_code_00042160 (0000)
000421F0 0070:
	_code_000421f0 (0000)
00042260 0060:
	_code_00042260 (0000)
000422C0 0150:
	_code_000422c0 (0000)
00042410 0080:
	_ai_profile_render (0000)
0024BBC8 0012:
	??_C@_0BC@FECLANMB@activation?5status?$AA@ (0000)
0024BBE0 0059:
	??_C@_0FJ@HFLBIAGN@?$CImeter?9?$DOhistory_next_index?5?$DO?$DN?50?$CJ@ (0000)
0024BC3C 001e:
	??_C@_0BO@JBPKCAGJ@definition?9?$DOmeter_id?5?$DN?$DN?5index?$AA@ (0000)
0024BC5C 001f:
	??_C@_0BP@IDLEFHAL@c?3?2halo?2SOURCE?2ai?2ai_profile?4c?$AA@ (0000)
0024BC80 004f:
	??_C@_0EP@EALMABPN@ai?5enc?5?$CF?52d?1?$CF?53d?0?5actor?5?$CF?53d?1?$CF?53@ (0000)
0024BCD0 0012:
	??_C@_0BC@LJCLCOPD@AI?5line?9spray?3?5?$CFs?$AA@ (0000)
0024BCE4 001e:
	??_C@_0BO@PNNEPAEO@encounters?5?$CFd?1?$CFd?$HMtprops?5?$CFd?1?$CFd?$AA@ (0000)
0024BD04 001e:
	??_C@_0BO@GPBPAFNH@actors?5?$CFd?1?$CFd?1?$CFd?$HMunits?5?$CFd?1?$CFd?$CFd?$AA@ (0000)
0024BD24 0022:
	??_C@_0CC@KKENFKIE@swarms?5?$CFd?1?$CFd?1?$CFd?$HMtcomponents?5?$CFd?1?$CF@ (0000)
0024BD48 003d:
	??_C@_0DN@KDGEGKJF@props?5a?1o?1u?3?$HMtenemy?5?$CFd?1?$CFd?1?$CFd?$HMtfr@ (0000)
0024BD88 003d:
	??_C@_0DN@CMELKBKH@collisions?5?$CFd?$HMtlineofsight?5?$CFd?$HMtl@ (0000)
0024BDC8 002e:
	??_C@_0CO@HHCJMMCK@path_flood?5?$CFd?$HMtpath_find?5?$CFd?$HMtact@ (0000)
002B7458 00ec:
	_global_ai_meter_definitions (0000)
*/

/* ---------- headers */

#include "ai/ai_profile.h"

#include "cseries/cseries.h"

#include "ai/actors.h"
#include "ai/actor_iterators.h"
#include "ai/encounters.h"
#include "ai/props.h"

#include "camera/observer.h"
#include "interface/interface.h"
#include "main/console.h"
#include "memory/data.h"
#include "rasterizer/rasterizer.h"
#include "render/render_debug.h"
#include "text/draw_string.h"
#include "text/text_group.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	AI_PROFILE_STRING_LENGTH = 2048,
};

enum
{
	_ai_render_spray_none = 0,
	_ai_render_spray_actions,
	_ai_render_spray_activation_status,
	NUMBER_OF_AI_RENDER_SPRAYS,
};

/* ---------- macros */

/* ---------- structures */

/* The iterator's public API is in actor_iterators.h. This concrete layout
 * agrees with its ACTORS.C owner and the January 28-byte caller frame. */
struct actor_iterator
{
	struct data_iterator encounter_iterator;
	boolean iterated_encounterless_list;
	boolean active_only;
	byte pad[2];
	long index;
	long next_index;
};

typedef short (*ai_meter_sample_proc)(
	void);

typedef char ai_meter_size_assert[
	sizeof(struct ai_meter) == 0x88 ? 1 : -1];
typedef char ai_profile_globals_meters_offset_assert[
	offsetof(struct ai_profile_globals, meters) == 0x0C ? 1 : -1];
typedef char ai_profile_globals_size_assert[
	sizeof(struct ai_profile_globals) == 0xEEC ? 1 : -1];

struct ai_meter_definition
{
	short meter_id;
	ai_meter_sample_proc sample_proc;
};

/* ---------- prototypes */

static short ai_profile_sample_encounters(
	void);
static short ai_profile_sample_actors(
	void);
static short ai_profile_sample_units(
	void);
static short ai_profile_sample_props(
	void);
static short ai_profile_sample_swarms(
	void);
static short ai_profile_sample_swarm_actors(
	void);
static short ai_profile_sample_swarm_components(
	void);
static void ai_profile_draw_string(
	char const *string,
	short tab_count,
	short const *tab_stops,
	real_argb_color const *color);
static void ai_profile_render_encounters(
	void);
static void ai_profile_render_actors(
	void);
static void ai_profile_render_swarms(
	void);
static void ai_profile_render_props(
	void);
static void ai_profile_render_collisions(
	void);
static void ai_profile_render_paths(
	void);
static void ai_profile_render_spray(
	void);

/* ---------- globals */

struct ai_profile_globals ai_profile;
char profilestring[AI_PROFILE_STRING_LENGTH];
short global_ai_profile_string_position;
struct ai_meter_definition global_ai_meter_definitions[NUMBER_OF_AI_METERS] =
{
	{_ai_meter_encounters, ai_profile_sample_encounters},
	{_ai_meter_encounters_updated, NULL},
	{_ai_meter_actors, ai_profile_sample_actors},
	{_ai_meter_actors_updated, NULL},
	{_ai_meter_actors_active, NULL},
	{_ai_meter_units, ai_profile_sample_units},
	{_ai_meter_units_updated, NULL},
	{_ai_meter_units_active, NULL},
	{_ai_meter_props, ai_profile_sample_props},
	{_ai_meter_dead_props_acknowledged, NULL},
	{_ai_meter_dead_props_orphaned, NULL},
	{_ai_meter_dead_props_unacknowledged, NULL},
	{_ai_meter_enemy_props_acknowledged, NULL},
	{_ai_meter_enemy_props_orphaned, NULL},
	{_ai_meter_enemy_props_unacknowledged, NULL},
	{_ai_meter_friendly_props_acknowledged, NULL},
	{_ai_meter_friendly_props_orphaned, NULL},
	{_ai_meter_friendly_props_unacknowledged, NULL},
	{_ai_meter_swarm_actors, ai_profile_sample_swarm_actors},
	{_ai_meter_swarms, ai_profile_sample_swarms},
	{_ai_meter_swarm_components, ai_profile_sample_swarm_components},
	{_ai_meter_collisions, NULL},
	{_ai_meter_line_of_sight, NULL},
	{_ai_meter_line_of_fire, NULL},
	{_ai_meter_path_flood, NULL},
	{_ai_meter_path_find, NULL},
	{_ai_meter_action_change, NULL},
	{_ai_meter_firing_point, NULL},
};
static char const *ai_profile_render_spray_names[NUMBER_OF_AI_RENDER_SPRAYS] =
{
	"none", "actions", "activation status",
};

/* ---------- public code */

void ai_profile_initialize(
	void)
{
	csmemset(&ai_profile, 0, sizeof(ai_profile));
	ai_profile.show = TRUE;
	return;
}

void ai_profile_initialize_for_new_map(
	void)
{
	csmemset(ai_profile.meters, 0, sizeof(ai_profile.meters));
	return;
}

void ai_profile_dispose(
	void)
{
	return;
}

void ai_profile_dispose_from_old_map(
	void)
{
	return;
}

void ai_profile_update(
	void)
{
	short index;
	struct ai_meter_definition const *definition = global_ai_meter_definitions;
	struct ai_meter *meter = ai_profile.meters;

	for (index = 0; index < NUMBER_OF_AI_METERS; index++, definition++, meter++)
	{
		match_assert("c:\\halo\\SOURCE\\ai\\ai_profile.c", 140, definition->meter_id == index);
		if (definition->sample_proc)
		{
			meter->accumulator = definition->sample_proc();
		}

		meter->current_value = meter->accumulator;
		meter->accumulator = 0;
		match_assert("c:\\halo\\SOURCE\\ai\\ai_profile.c", 151, (meter->history_next_index >= 0) && (meter->history_next_index < AI_METER_HISTORY_TICKS));
		if (meter->history_next_index < meter->history_count)
		{
			meter->history_sum -= meter->history[meter->history_next_index];
		}
		meter->history[meter->history_next_index] = meter->current_value;
		/* BUG (original): January and October subtract the evicted sample but
		 * never add the new sample to history_sum. A corrected build should add
		 * current_value here before computing the average. */
		meter->history_next_index++;
		meter->history_count = MAX(meter->history_count, meter->history_next_index);
		meter->history_next_index %= AI_METER_HISTORY_TICKS;
		meter->average = (real)meter->history_sum / meter->history_count;
	}

	return;
}

void ai_profile_display(
	char *buffer)
{
	sprintf(buffer + csstrlen(buffer),
		"ai enc % 2d/% 3d, actor % 3d/% 3d/% 3d, unit % 3d/% 3d/% 3d, props % 3d/% 3d|n",
		ai_profile.meters[_ai_meter_encounters_updated].current_value,
		ai_profile.meters[_ai_meter_encounters].current_value,
		ai_profile.meters[_ai_meter_actors_active].current_value,
		ai_profile.meters[_ai_meter_actors_updated].current_value,
		ai_profile.meters[_ai_meter_actors].current_value,
		ai_profile.meters[_ai_meter_units_active].current_value,
		ai_profile.meters[_ai_meter_units_updated].current_value,
		ai_profile.meters[_ai_meter_units].current_value,
		ai_profile.meters[_ai_meter_props].current_value,
		768);
	return;
}

short ai_profile_change_render_spray(
	void)
{
	ai_profile.render_spray =
		(ai_profile.render_spray + 1) % NUMBER_OF_AI_RENDER_SPRAYS;
	console_printf(
		FALSE,
		"AI line-spray: %s",
		ai_profile_render_spray_names[ai_profile.render_spray]);

	return ai_profile.render_spray;
}

void ai_profile_render(
	void)
{
	global_ai_profile_string_position = rasterizer_globals.reserved04.frame_bounds.y1 - 20;
	ai_profile_render_spray();
	if (ai_profile.show)
	{
		if (ai_profile.show_prop_types)
		{
			ai_profile_render_props();
		}
		if (ai_profile.show_line_of_sight)
		{
			ai_profile_render_collisions();
		}
		if (ai_profile.show_paths)
		{
			ai_profile_render_paths();
		}
		if (ai_profile.show_swarms)
		{
			ai_profile_render_swarms();
		}
		if (ai_profile.show_actors)
		{
			ai_profile_render_actors();
		}
		if (ai_profile.show_stats)
		{
			ai_profile_render_encounters();
		}
	}

	return;
}

/* ---------- private code */

static short ai_profile_sample_encounters(
	void)
{
	return encounter_data->actual_count;
}

static short ai_profile_sample_actors(
	void)
{
	return actor_data->actual_count;
}

static short ai_profile_sample_units(
	void)
{
	struct actor_iterator iterator;
	struct actor_datum *actor;
	short count = 0;

	actor_iterator_new(&iterator, FALSE);
	while ((actor = actor_iterator_next(&iterator)) != NULL)
	{
		count += actor->meta.swarm ? actor->meta.swarm_unit_count : 1;
	}

	return count;
}

static short ai_profile_sample_swarms(
	void)
{
	return swarm_data->actual_count;
}

static short ai_profile_sample_props(
	void)
{
	return prop_data->actual_count;
}

static short ai_profile_sample_swarm_actors(
	void)
{
	struct actor_iterator iterator;
	struct actor_datum *actor;
	short count = 0;

	actor_iterator_new(&iterator, FALSE);
	while ((actor = actor_iterator_next(&iterator)) != NULL)
	{
		count += actor->meta.swarm;
	}

	return count;
}

static short ai_profile_sample_swarm_components(
	void)
{
	return swarm_component_data->actual_count;
}

static void ai_profile_draw_string(
	char const *string,
	short tab_count,
	short const *tab_stops,
	real_argb_color const *color)
{
	rectangle2d bounds;
	point2d cursor;

	bounds.y0 = global_ai_profile_string_position;
	bounds.x0 = 0;
	bounds.x1 = SHORT_MAX;
	bounds.y1 = SHORT_MAX;
	if (!color)
	{
		color = global_real_argb_white;
	}
	interface_set_bitmap_text_draw_mode(_interface_font_terminal, _text_style_plain, 0, 0, _interface_color_table_dialog, 0);
	draw_string_set_color(color);
	draw_string_set_tab_stops(tab_stops, tab_count);
	rasterizer_draw_string(&bounds, NULL, &cursor, 0, string);
	draw_string_set_tab_stops(NULL, 0);
	global_ai_profile_string_position += bounds.y0 - cursor.y;
	return;
}

static void ai_profile_render_encounters(
	void)
{
	short tab_stops[] = {150, 300};

	sprintf(profilestring, "encounters %d/%d|tprops %d/%d",
		ai_profile.meters[_ai_meter_encounters_updated].current_value,
		ai_profile.meters[_ai_meter_encounters].current_value,
		ai_profile.meters[_ai_meter_props].current_value,
		768);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_actors(
	void)
{
	short tab_stops[] = {150, 300};

	/* Preserve January's missing tab marker and final unit-count separator. */
	sprintf(profilestring, "actors %d/%d/%d|units %d/%d%d",
		ai_profile.meters[_ai_meter_actors_active].current_value,
		ai_profile.meters[_ai_meter_actors_updated].current_value,
		ai_profile.meters[_ai_meter_actors].current_value,
		ai_profile.meters[_ai_meter_units_active].current_value,
		ai_profile.meters[_ai_meter_units_updated].current_value,
		ai_profile.meters[_ai_meter_units].current_value);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_swarms(
	void)
{
	short tab_stops[] = {150};

	sprintf(profilestring, "swarms %d/%d/%d|tcomponents %d/%d",
		ai_profile.meters[_ai_meter_swarms].current_value,
		ai_profile.meters[_ai_meter_swarm_actors].current_value,
		MAXIMUM_SWARMS,
		ai_profile.meters[_ai_meter_swarm_components].current_value,
		MAXIMUM_SWARM_COMPONENTS);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_props(
	void)
{
	short tab_stops[] = {150, 300, 450};

	sprintf(profilestring, "props a/o/u:|tenemy %d/%d/%d|tfriend %d/%d/%d|tdead %d/%d/%d",
		ai_profile.meters[_ai_meter_enemy_props_acknowledged].current_value,
		ai_profile.meters[_ai_meter_enemy_props_orphaned].current_value,
		ai_profile.meters[_ai_meter_enemy_props_unacknowledged].current_value,
		ai_profile.meters[_ai_meter_friendly_props_acknowledged].current_value,
		ai_profile.meters[_ai_meter_friendly_props_orphaned].current_value,
		ai_profile.meters[_ai_meter_friendly_props_unacknowledged].current_value,
		ai_profile.meters[_ai_meter_dead_props_acknowledged].current_value,
		ai_profile.meters[_ai_meter_dead_props_orphaned].current_value,
		ai_profile.meters[_ai_meter_dead_props_unacknowledged].current_value);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_collisions(
	void)
{
	short tab_stops[] = {150, 300, 450};

	sprintf(profilestring, "collisions %d|tlineofsight %d|tlineoffire %d|tfiringpoint %d",
		ai_profile.meters[_ai_meter_collisions].current_value,
		ai_profile.meters[_ai_meter_line_of_sight].current_value,
		ai_profile.meters[_ai_meter_line_of_fire].current_value,
		ai_profile.meters[_ai_meter_firing_point].current_value);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_paths(
	void)
{
	short tab_stops[] = {150, 300, 450};

	sprintf(profilestring, "path_flood %d|tpath_find %d|taction_change %d",
		ai_profile.meters[_ai_meter_path_flood].current_value,
		ai_profile.meters[_ai_meter_path_find].current_value,
		ai_profile.meters[_ai_meter_action_change].current_value);
	ai_profile_draw_string(profilestring, NUMBEROF(tab_stops), tab_stops, global_real_argb_white);
	return;
}

static void ai_profile_render_spray(
	void)
{
	struct observer_result const *camera = observer_get_camera(0);

	if (ai_profile.render_spray > _ai_render_spray_none && camera)
	{
		boolean active_only = TRUE;
		real_point3d start;
		struct actor_iterator iterator;

		switch (ai_profile.render_spray)
		{
			case _ai_render_spray_activation_status:
				active_only = FALSE;
				break;
		}

		start.x = camera->position.x + camera->forward.i * 0.05f;
		start.y = camera->position.y + camera->forward.j * 0.05f;
		start.z = camera->position.z + camera->forward.k * 0.05f;
		actor_iterator_new(&iterator, active_only);
		while (actor_iterator_next(&iterator))
		{
			struct actor_datum *actor = actor_get(iterator.index);
			real_argb_color const *color = NULL;

			switch (ai_profile.render_spray)
			{
				case _ai_render_spray_actions:
					color = actor_action_debug_color(iterator.index);
					break;
				case _ai_render_spray_activation_status:
					color = actor_activation_debug_color(iterator.index);
					break;
			}

			if (color)
			{
				if (actor->meta.swarm)
				{
					long unit_index = actor->meta.swarm_unit_index;

					while (unit_index != NONE)
					{
						struct unit_datum *unit = unit_get(unit_index);
						real_point3d head_position;

						unit_get_head_position(unit_index, &head_position);
						render_debug_line(TRUE, &start, &head_position, color);
						unit_index = unit->unit.swarm_next_unit_index;
					}
				}
				else
				{
					render_debug_line(TRUE, &start, &actor->input.position.head_position, color);
				}
			}
		}
	}

	return;
}
