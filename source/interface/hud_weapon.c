/*
HUD_WEAPON.C

symbols in this file:
000C7E70 0040:
	_hud_initialize_weapon_interface (0000)
000C7EB0 0040:
	_hud_initialize_weapon_interface_for_new_map (0000)
000C7EF0 0010:
	_hud_dispose_weapon_interface_from_old_map (0000)
000C7F00 0010:
	_hud_dispose_weapon_interface (0000)
000C7F10 0030:
	_scripted_hud_show_crosshair (0000)
000C7F40 0070:
	_get_hud_state (0000)
000C7FB0 0070:
	_get_crosshair_state (0000)
000C8020 0050:
	_code_000c8020 (0000)
000C8070 02e0:
	_code_000c8070 (0000)
000C8350 0020:
	_strip_path_name (0000)
000C8370 08d0:
	_code_000c8370 (0000)
000C8C40 00a0:
	_hud_fix_weapon_data (0000)
000C8CE0 05c0:
	_code_000c8ce0 (0000)
000C92A0 0a60:
	_code_000c92a0 (0000)
000C9D00 0270:
	_hud_update_weapon (0000)
000C9F70 01f0:
	_hud_render_weapon_interface (0000)
002702F8 0013:
	??_C@_0BD@NOPIHEPB@weapon_hud_globals?$AA@ (0000)
0027030C 0026:
	??_C@_0CG@DOLNIFA@c?3?2halo?2SOURCE?2interface?2hud_wea@ (0000)
00270334 0015:
	??_C@_0BF@HCPMNCNB@hud?5weapon?5interface?$AA@ (0000)
00270350 0008:
	__real@3ff4000000000000 (0000)
00270358 0052:
	??_C@_0FC@PHPDNACJ@frame?5index?5NONE?5when?5drawing?5cr@ (0000)
002703AC 0030:
	??_C@_0DA@GANDHJMM@too?5many?5levels?5in?5current?5weapo@ (0000)
00453AC4 0004:
	_weapon_hud_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "cache/texture_cache.h"
#include "game/game.h"
#include "game/game_globals.h"
#include "game/player_control.h"
#include "game/players.h"
#include "interface/hud_draw.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_weapon.h"
#include "interface/unit_hud_interface_definition.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "objects/objects.h"
#include "render/render.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"
#include "units/unit_definitions.h"
#include "units/units.h"

#include <math.h>
#include <string.h>

/* ---------- constants */

enum hud_crosshair_show_flags
{
	_hud_crosshair_show_bit = 0
};

enum
{
	GRENADE_HUD_INTERFACE_TAG = 'grhi',
	WEAPON_HUD_INTERFACE_TAG = 'wphi',
	MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH = 16,
	NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES = 8,
	NUMBER_OF_WEAPON_HUD_CROSSHAIR_STATES = 19,
};

enum weapon_crosshair_type
{
	_crosshair_state_aim = 0,
	_crosshair_state_zoom,
	_crosshair_state_charge,
	_crosshair_state_flash_ammo,
	_crosshair_state_flash_heat,
	_crosshair_state_flash_total_ammo,
	_crosshair_state_flash_total_battery,
	_crosshair_state_reload,
	_crosshair_state_fired_with_no_ammo,
	_crosshair_state_threw_with_no_grenade,
	_crosshair_state_flash_ammo_none_for_reload,
	_crosshair_state_flash_secondary_ammo,
	_crosshair_state_flash_secondary_total_ammo,
	_crosshair_state_secondary_reload,
	_crosshair_state_fired_secondary_with_no_ammo,
	_crosshair_state_flash_secondary_ammo_none_for_reload,
	_crosshair_state_primary_trigger_ready,
	_crosshair_state_secondary_trigger_ready,
	_crosshair_state_flash_fired_battery_depleted,
	NUMBER_OF_CROSSHAIR_STATES,
};

enum hud_scaling_flags
{
	_hud_dont_scale_offset_bit = 0,
	_hud_dont_scale_size_bit,
	_hud_use_high_resolution_scale_bit,
};

enum hud_crosshair_flags
{
	_hud_crosshair_flashes_bit = 0,
	_hud_crosshair_not_a_sprite_bit,
	_hud_crosshair_not_on_default_zoom_bit,
	_hud_crosshair_show_sniper_data_bit,
	_hud_crosshair_hide_outside_area_bit,
	_hud_crosshair_one_zoom_level_bit,
	_hud_crosshair_only_on_default_zoom_bit,
	_hud_crosshair_runtime_invalid_bit,
};

/* TU-local copy: bitmap_group.h does not own the bitmap group type enum; the existing
   copies are in bitmaps/bitmap_extract.c (complete), bitmaps/bitmap_group.c and
   interface/hud_draw.c. */
enum bitmap_group_type
{
	_bitmap_group_type_2d_textures = 0,
	_bitmap_group_type_3d_textures,
	_bitmap_group_type_cube_maps,
	_bitmap_group_type_sprites,
	_bitmap_group_type_interface_bitmaps,
	NUMBER_OF_BITMAP_GROUP_TYPES
};

enum weapon_overlay_on_flags
{
	_weapon_overlay_on_flashing_bit = 0,
	_weapon_overlay_on_empty_bit,
	_weapon_overlay_on_reload_bit,
	_weapon_overlay_on_default_bit,
	_weapon_overlay_on_always_bit,
};

enum grenade_overlay_on_flags
{
	_grenade_overlay_on_flashing_bit = 0,
	_grenade_overlay_on_empty_bit,
	_grenade_overlay_on_default_bit,
	_grenade_overlay_on_always_bit,
};

enum
{
	_hud_use_parent_flash_parameters_bit = 0,
	_hud_element_runtime_invalid_bit = 0,
	_weapon_number_divide_by_clip_size_bit = 0,
};

/* ---------- macros */

#define grenade_hud_interface_definition_get(index) \
	((struct grenade_hud_interface_definition *)tag_get( \
		GRENADE_HUD_INTERFACE_TAG, \
		(index)))
#define weapon_hud_interface_definition_get(index) \
	((struct weapon_hud_interface_definition *)tag_get( \
		WEAPON_HUD_INTERFACE_TAG, \
		(index)))

/* ---------- structures */

struct weapon_hud_state
{
	long last_weapon_flash_time[NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES];
	long last_weapon_index;
	long last_grenade_flash_time;
};

union crosshair_state_value
{
	long reference_data;
	real frame_index;
};

struct crosshair_state
{
	union crosshair_state_value value;
};

struct crosshair_hud_state
{
	struct crosshair_state states[NUMBER_OF_WEAPON_HUD_CROSSHAIR_STATES];
	unsigned long render_flags;
};

struct weapon_hud_globals
{
	struct weapon_hud_state hud_states[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	struct crosshair_hud_state crosshair_states[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	long script_flags;
};

struct weapon_interface_magazine_state
{
	boolean reloading;
	boolean can_fire;
	short rounds_loaded;
	short rounds_loaded_maximum;
	short rounds_remaining;
	short rounds_remaining_maximum;
};

struct weapon_interface_state
{
	real heat;
	real age;
	boolean overheated;
	byte pad09;
	short magazine_count;
	struct weapon_interface_magazine_state magazines[2];
};

struct number_hud_element_definition
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	char digits;
	byte number_flags;
	char fractional_digits;
	byte pad;
	long unused[3];
};

struct weapon_hud_overlay_definition
{
	struct tag_reference bitmap;
	struct tag_block items;
};

struct grenade_count_panel_definition
{
	struct static_hud_element_definition background;
	struct number_hud_element_definition numbers;
	short flash_cutoff;
	short pad;
};

struct grenade_hud_interface_definition
{
	struct hud_absolute_placement_definition absolute_placement;
	struct static_hud_element_definition background;
	struct grenade_count_panel_definition grenade_count_panel;
	struct weapon_hud_overlay_definition overlays;
	struct tag_block warning_sounds;
	long unused0[17];
	struct tag_reference messaging_icon_bitmap;
	long unused1[12];
};

struct weapon_flash_state_definition
{
	short flags;
	short pad;
	short total_ammo;
	short loaded_ammo;
	short heat;
	short age;
	long unused[8];
};

struct weapon_hud_interface_definition
{
	struct tag_reference parent_hud;
	struct weapon_flash_state_definition flash_cutoffs;
	struct hud_absolute_placement_definition absolute_placement;
	struct tag_block statics;
	struct tag_block meters;
	struct tag_block numbers;
	struct tag_block crosshairs;
	struct tag_block overlays;
	unsigned long valid_crosshair_types_flags;
	struct tag_block warning_sounds;
	struct tag_block screen_effects;
	long unused1[33];
	byte messaging_icon[0x10];
	long unused2[12];
};

struct weapon_hud_element_header
{
	short state_type;
	short runtime_flags;
	short use_on_map_type;
	short pad;
	long unused[7];
};

struct weapon_hud_static_element
{
	struct weapon_hud_element_header header;
	struct static_hud_element_definition static_element;
	long unused[10];
};

struct weapon_hud_meter_element
{
	struct weapon_hud_element_header header;
	struct meter_hud_element_definition meter_element;
	long unused[10];
};

struct weapon_hud_number_element
{
	struct weapon_hud_element_header header;
	struct number_hud_element_definition number_element;
	word weapon_flags;
	short pad;
	long unused[9];
};

struct weapon_hud_overlays_element
{
	short state_type;
	short runtime_flags;
	short use_on_map_type;
	short pad;
	long unused[7];
	struct weapon_hud_overlay_definition overlays;
	long unused2[10];
};

struct weapon_hud_crosshair_definition
{
	struct tag_reference bitmap;
	struct tag_block items;
};

struct weapon_hud_crosshairs_element
{
	short crosshair_type;
	short runtime_flags;
	short use_on_map_type;
	short pad;
	long unused[7];
	struct weapon_hud_crosshair_definition crosshairs;
	long unused2[10];
};

struct weapon_hud_crosshair_item
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	short frame_rate;
	short sequence_index;
	unsigned long flags;
	long unused[8];
};

typedef char weapon_hud_state_size_assert[
	sizeof(struct weapon_hud_state) == 0x28 ? 1 : -1];
typedef char crosshair_state_size_assert[
	sizeof(struct crosshair_state) == 0x4 ? 1 : -1];
typedef char crosshair_hud_state_size_assert[
	sizeof(struct crosshair_hud_state) == 0x50 ? 1 : -1];
typedef char weapon_hud_globals_script_flags_offset_assert[
	offsetof(struct weapon_hud_globals, script_flags) == 0x1E0 ? 1 : -1];
typedef char weapon_hud_globals_size_assert[
	sizeof(struct weapon_hud_globals) == 0x1E4 ? 1 : -1];
typedef char weapon_interface_state_size_assert[
	sizeof(struct weapon_interface_state) == 0x20 ? 1 : -1];
typedef char number_hud_element_definition_size_assert[
	sizeof(struct number_hud_element_definition) == 0x54 ? 1 : -1];
typedef char grenade_hud_interface_definition_size_assert[
	sizeof(struct grenade_hud_interface_definition) == 0x1F8 ? 1 : -1];
typedef char weapon_hud_interface_definition_size_assert[
	sizeof(struct weapon_hud_interface_definition) == 0x17C ? 1 : -1];
typedef char weapon_hud_crosshairs_element_size_assert[
	sizeof(struct weapon_hud_crosshairs_element) == 0x68 ? 1 : -1];
typedef char weapon_hud_crosshair_item_size_assert[
	sizeof(struct weapon_hud_crosshair_item) == 0x6C ? 1 : -1];
typedef char hud_weapon_globals_default_weapon_hud_index_offset_assert[
	offsetof(struct hud_globals_definition, defaults.default_weapon_hud.index) == 0x2CC ? 1 : -1];

/* ---------- prototypes */

static struct weapon_hud_state *get_hud_state(
	short local_player_index);
char *strip_path_name(
	char *path);
static struct crosshair_hud_state *get_crosshair_state(
	short local_player_index);
static void play_weapon_hud_sounds(
	short local_player_index,
	long hud_index,
	struct weapon_interface_state *weapon_state);
static void render_grenade_hud(
	short local_player_index,
	long unit_index);
static void crosshairs_draw(
	struct player_datum *player,
	long weapon_index,
	long hud_index,
	struct weapon_interface_state *weapon_state);
static void hud_update_weapon_local_player(
	short local_player_index,
	long weapon_index,
	long hud_index,
	struct weapon_interface_state *weapon_state);
static void render_weapon_hud(
	long hud_index,
	short local_player_index,
	struct weapon_definition const *weapon_definition,
	struct weapon_interface_state const *weapon_state,
	short const *new_state_flags,
	short const *new_overlay_flags,
	short const *new_numbers);

/* ---------- globals */

static struct weapon_hud_globals *weapon_hud_globals = NULL;

/* ---------- public code */

void hud_initialize_weapon_interface(
	void)
{
	weapon_hud_globals = game_state_malloc(
		"hud weapon interface",
		NULL,
		sizeof(*weapon_hud_globals));

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x6B,
		weapon_hud_globals);

	return;
}

void hud_initialize_weapon_interface_for_new_map(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x73,
		weapon_hud_globals);

	csmemset(
		weapon_hud_globals,
		NONE,
		sizeof(*weapon_hud_globals));

	return;
}

void hud_dispose_weapon_interface_from_old_map(
	void)
{
	return;
}

void hud_dispose_weapon_interface(
	void)
{
	return;
}

void scripted_hud_show_crosshair(
	boolean show)
{
	SET_FLAG(
		weapon_hud_globals->script_flags,
		_hud_crosshair_show_bit,
		show);

	return;
}

static struct weapon_hud_state *get_hud_state(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x1AE,
		local_player_index>=0 &&
			local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x1AF,
		weapon_hud_globals);

	return &weapon_hud_globals->hud_states[local_player_index];
}

static struct crosshair_hud_state *get_crosshair_state(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x1B7,
		local_player_index>=0 &&
			local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x1B8,
		weapon_hud_globals);

	return &weapon_hud_globals->crosshair_states[local_player_index];
}

static void play_weapon_hud_sounds(
	short local_player_index,
	long hud_index,
	struct weapon_interface_state *weapon_state)
{
	(void)weapon_state;

	if (hud_index != NONE)
	{
		unit_try_and_get(
			local_player_get_player_index(local_player_index) == NONE ?
				NONE :
				player_get(
					local_player_get_player_index(local_player_index))->unit_index);
	}

	return;
}

static void render_grenade_hud(
	short local_player_index,
	long unit_index)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct unit_datum *unit;
	long weapon_index;
	long parent_index;
	struct unit_datum *parent_unit;
	short grenade_type;
	struct game_globals_grenade *grenade;
	struct weapon_hud_state *hud_state;
	long hud_index;
	struct grenade_hud_interface_definition *definition;
	short draw_flags;

	csmemset(
		stack_buffer,
		0x62,
		sizeof(stack_buffer));

	unit = unit_get(unit_index);
	weapon_index = unit_inventory_get_weapon(
		unit_index,
		unit->unit.current_weapon_index);
	unit = unit_get(unit_index);
	(void)unit_definition_get(unit->definition_index);
	if (weapon_prevents_grenade_throwing(weapon_index) ||
		unit->unit.current_grenade_index == NONE)
	{
		goto finished;
	}

	parent_index = unit->object.parent_object_index;
	parent_unit = unit_try_and_get(parent_index);
	if (parent_unit &&
		(parent_unit->unit.driver_object_index == unit_index ||
			parent_unit->unit.gunner_object_index == unit_index))
	{
		goto finished;
	}

	grenade_type = unit_get_current_grenade_type(unit_index);
	if (grenade_type == NONE)
		goto finished;

	grenade = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->grenades,
		grenade_type,
		struct game_globals_grenade);
	hud_index = grenade->hud_interface.index;
	hud_state = get_hud_state(local_player_index);
	if (hud_index == NONE)
		goto finished;

	definition = grenade_hud_interface_definition_get(hud_index);
	draw_flags = unit->unit.grenade_counts[unit->unit.current_grenade_index] <=
		definition->grenade_count_panel.flash_cutoff;
	SET_FLAG(
		draw_flags,
		_hud_draw_disabled_bit,
		unit->unit.grenade_counts[unit->unit.current_grenade_index] == 0);
	SET_FLAG(draw_flags, _hud_draw_in_multiplayer_bit, local_player_count() > 1);

	if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
	{
		if (hud_state->last_grenade_flash_time == NONE)
			hud_state->last_grenade_flash_time = game_time_get();
	}
	else
	{
		hud_state->last_grenade_flash_time = NONE;
	}

	if (definition->background.interface_bitmap.index != NONE)
	{
		hud_draw_static_element(
			local_player_index,
			&definition->absolute_placement,
			&definition->background,
			draw_flags,
			hud_state->last_grenade_flash_time);
	}

	if (definition->grenade_count_panel.background.interface_bitmap.index != NONE)
	{
		hud_draw_static_element(
			local_player_index,
			&definition->absolute_placement,
			&definition->grenade_count_panel.background,
			draw_flags,
			hud_state->last_grenade_flash_time);
	}

	if (definition->grenade_count_panel.numbers.digits)
	{
		hud_draw_numbers(
			local_player_index,
			&definition->absolute_placement,
			&definition->grenade_count_panel.numbers,
			unit_get_grenade_count(
				unit_index,
				unit_get_current_grenade_type(unit_index)),
			NONE,
			draw_flags,
			hud_state->last_grenade_flash_time,
			0.0f);
	}

	if (definition->overlays.bitmap.index != NONE)
	{
		short overlay_flags =
			unit->unit.grenade_counts[unit->unit.current_grenade_index] <=
			definition->grenade_count_panel.flash_cutoff;

		SET_FLAG(
			overlay_flags,
			_grenade_overlay_on_empty_bit,
			unit->unit.grenade_counts[unit->unit.current_grenade_index] == 0);
		SET_FLAG(overlay_flags, _grenade_overlay_on_default_bit, overlay_flags == 0);
		SET_FLAG(overlay_flags, _grenade_overlay_on_always_bit, TRUE);
		hud_draw_weapon_overlays(
			local_player_index,
			&definition->absolute_placement,
			&definition->overlays,
			overlay_flags,
			hud_state->last_grenade_flash_time,
			draw_flags,
			local_player_count() > 1);
	}

finished:
	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_weapon.c", 0x3A2);
	return;
}

static void hud_update_weapon_local_player(
	short local_player_index,
	long weapon_index,
	long hud_index,
	struct weapon_interface_state *weapon_state)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct player_datum *player;
	long unit_index;
	struct unit_datum *unit;
	struct crosshair_hud_state *crosshair;
	struct weapon_hud_interface_definition *root_definition;

	csmemset(
		stack_buffer,
		0x62,
		sizeof(stack_buffer));
	player = player_get(local_player_get_player_index(local_player_index));
	unit_index = player->unit_index;
	unit = unit_try_and_get(unit_index);
	if (!unit)
		goto finished;

	crosshair = get_crosshair_state(player->local_player_index);
	root_definition = weapon_hud_interface_definition_get(hud_index);
	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		(void)weapon_definition_get(weapon->definition_index);
	}

	{
		struct weapon_hud_interface_definition *weapon_hud_hierarchy[MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH] =
			{ root_definition };
		long weapon_hud_indices[MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH] = { hud_index };
		unsigned long valid_crosshair_types = weapon_hud_hierarchy[0]->valid_crosshair_types_flags;
		unsigned long render_flags = 0;
		short definition_count = 1;
		short crosshair_index;

		if (weapon_index != get_hud_state(local_player_index)->last_weapon_index &&
			weapon_index == NONE)
		{
			csmemset(crosshair, 0, sizeof(*crosshair));
		}

		do
		{
			if (weapon_hud_hierarchy[definition_count - 1]->parent_hud.index == NONE)
				break;
			weapon_hud_indices[definition_count] =
				weapon_hud_hierarchy[definition_count - 1]->parent_hud.index;
			weapon_hud_hierarchy[definition_count] = weapon_hud_interface_definition_get(
				weapon_hud_indices[definition_count]);
			valid_crosshair_types |= weapon_hud_hierarchy[definition_count]->valid_crosshair_types_flags;
			definition_count++;
		}
		while (definition_count < MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH);

		if (definition_count == MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH)
		{
			error(
				_error_silent,
				"too many levels in current weapon hud hierarchy");
		}

		for (crosshair_index = 0;
			crosshair_index < NUMBER_OF_CROSSHAIR_STATES;
			crosshair_index++)
		{
			if (TEST_FLAG(valid_crosshair_types, crosshair_index))
			{
				struct crosshair_state *state = &crosshair->states[crosshair_index];
				short result;

				switch (crosshair_index)
				{
				case _crosshair_state_primary_trigger_ready:
					result = weapon_state->magazines[0].can_fire;
					break;

				case _crosshair_state_secondary_trigger_ready:
					result = weapon_state->magazines[1].can_fire;
					break;

				case _crosshair_state_aim:
					result = weapon_index == NONE ?
						FALSE :
						player_control_get_autoaim_level(player->local_player_index) == 1.0f;
					break;

				case _crosshair_state_zoom:
					result = player_control_get_zoom_level(player->local_player_index) != NONE ?
						player_control_get_zoom_level(player->local_player_index) + 2 :
						1;
					break;

				case _crosshair_state_reload:
					result = weapon_state->magazines[0].reloading;
					break;

				case _crosshair_state_flash_ammo:
					/* The redundant (remaining || loaded) && remaining tests of the four ammo states are first-party:
					   January tests rounds_remaining twice (T+0x205..0x21b) and the later /Od build spells them the
					   same way (0x638534..0x638599). */
					result = (weapon_state->magazines[0].rounds_remaining || weapon_state->magazines[0].rounds_loaded) &&
						weapon_state->magazines[0].rounds_remaining &&
						weapon_state->magazines[0].rounds_loaded <= root_definition->flash_cutoffs.loaded_ammo;
					break;

				case _crosshair_state_flash_heat:
					result = weapon_state->heat * 100.0f >=
						(real)root_definition->flash_cutoffs.heat;
					break;

				case _crosshair_state_flash_total_ammo:
					result = weapon_state->magazines[0].rounds_remaining <=
							root_definition->flash_cutoffs.total_ammo &&
						!weapon_state->magazines[0].reloading;
					break;

				case _crosshair_state_flash_total_battery:
					result = weapon_state->age < 1.0f &&
						(1.0f - weapon_state->age) * 100.0f <=
							(real)root_definition->flash_cutoffs.age;
					break;

				case _crosshair_state_charge:
					result = FALSE;
					break;

				case _crosshair_state_flash_fired_battery_depleted:
					result = (weapon_state->age == 1.0f &&
							TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit)) ||
						state->value.reference_data != NONE;
					break;

				case _crosshair_state_fired_with_no_ammo:
					result = (!weapon_state->magazines[0].rounds_loaded &&
							!weapon_state->magazines[0].rounds_remaining &&
							TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit)) ||
						state->value.reference_data != NONE;
					break;

				case _crosshair_state_threw_with_no_grenade:
				{
					boolean no_grenades = TRUE;
					short grenade_type;

					for (grenade_type = 0; grenade_type < NUMBER_OF_UNIT_GRENADE_TYPES; grenade_type++)
					{
						no_grenades = no_grenades && !unit->unit.grenade_counts[grenade_type];
					}
					result = (no_grenades &&
							!unit->unit.grenade_throw_state &&
							TEST_FLAG(unit->unit.control_flags, _unit_control_throw_grenade_bit)) ||
						state->value.reference_data != NONE;
					break;
				}

				case _crosshair_state_flash_ammo_none_for_reload:
					result = (weapon_state->magazines[0].rounds_remaining || weapon_state->magazines[0].rounds_loaded) &&
						!weapon_state->magazines[0].rounds_remaining &&
						weapon_state->magazines[0].rounds_loaded <= root_definition->flash_cutoffs.loaded_ammo;
					break;

				case _crosshair_state_flash_secondary_ammo:
					result = (weapon_state->magazines[1].rounds_remaining || weapon_state->magazines[1].rounds_loaded) &&
						weapon_state->magazines[1].rounds_remaining &&
						weapon_state->magazines[1].rounds_loaded <= root_definition->flash_cutoffs.loaded_ammo;
					break;

				case _crosshair_state_flash_secondary_total_ammo:
					result = weapon_state->magazines[1].rounds_remaining <=
							root_definition->flash_cutoffs.total_ammo &&
						!weapon_state->magazines[1].reloading;
					break;

				case _crosshair_state_secondary_reload:
					result = weapon_state->magazines[1].reloading;
					break;

				case _crosshair_state_fired_secondary_with_no_ammo:
					/* January (T+0x3b7 shared tail `test ch,8`) and the later /Od build (0x638a07
					   `and edx,0x800`) both test the primary trigger for this secondary state. */
					result = (!weapon_state->magazines[1].rounds_loaded &&
							!weapon_state->magazines[1].rounds_remaining &&
							TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit)) ||
						state->value.reference_data != NONE;
					break;

				case _crosshair_state_flash_secondary_ammo_none_for_reload:
					result = (weapon_state->magazines[1].rounds_remaining || weapon_state->magazines[1].rounds_loaded) &&
						!weapon_state->magazines[1].rounds_remaining &&
						weapon_state->magazines[1].rounds_loaded <= root_definition->flash_cutoffs.loaded_ammo;
					break;

				default:
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
						0x16E,
						!"unreachable");
					/* BUG (preserved for exact matching): January (T+0x404) and the later /Od build (0x638ac2)
					   leave result unassigned in this arm. The arm is unreachable in defined execution:
					   crosshair_index only takes the values 0..NUMBER_OF_CROSSHAIR_STATES-1 (0..18), and each of
					   those 19 states has a case above that assigns result. */
					break;
				}

				SET_FLAG(
					render_flags,
					crosshair_index,
					result > 0 || crosshair_index == _crosshair_state_aim);
				switch (crosshair_index)
				{
				case _crosshair_state_aim:
					state->value.reference_data = result;
					break;

				case _crosshair_state_zoom:
					state->value.reference_data = result - 1;
					break;

				case _crosshair_state_charge:
				case _crosshair_state_flash_ammo:
				case _crosshair_state_flash_heat:
				case _crosshair_state_flash_total_ammo:
				case _crosshair_state_flash_total_battery:
				case _crosshair_state_reload:
				case _crosshair_state_fired_with_no_ammo:
				case _crosshair_state_threw_with_no_grenade:
				case _crosshair_state_flash_ammo_none_for_reload:
				case _crosshair_state_flash_secondary_ammo:
				case _crosshair_state_flash_secondary_total_ammo:
				case _crosshair_state_secondary_reload:
				case _crosshair_state_fired_secondary_with_no_ammo:
				case _crosshair_state_flash_secondary_ammo_none_for_reload:
				case _crosshair_state_primary_trigger_ready:
				case _crosshair_state_secondary_trigger_ready:
				case _crosshair_state_flash_fired_battery_depleted:
					if (!result)
					{
						state->value.reference_data = NONE;
					}
					else if (state->value.reference_data == NONE)
					{
						state->value.reference_data = game_time_get();
					}
					break;

				default:
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
						0x197,
						!"unreachable");
					break;
				}
			}
		}

		crosshair->render_flags = render_flags;
	}

finished:
	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_weapon.c", 0x19E);
	return;
}

static void crosshairs_draw(
	struct player_datum *player,
	long weapon_index,
	long hud_index,
	struct weapon_interface_state *weapon_state)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	boolean firing_active = FALSE;

	csmemset(
		stack_buffer,
		0x62,
		sizeof(stack_buffer));
	if (TEST_FLAG(weapon_hud_globals->script_flags, _hud_crosshair_show_bit) &&
		hud_index != NONE)
	{
		long unit_index = player->unit_index;
		struct crosshair_hud_state *crosshair = get_crosshair_state(player->local_player_index);
		struct weapon_hud_interface_definition *root_definition = weapon_hud_interface_definition_get(hud_index);
		short map_type_flags = global_scenario_get()->type != _scenario_type_main_menu;

		SET_FLAG(map_type_flags, 1, local_player_count() == 1);
		SET_FLAG(map_type_flags, 2, local_player_count() > 1);
		if (unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(unit_index);
			struct weapon_definition *weapon_definition = weapon_index == NONE ?
				NULL :
				weapon_definition_get(weapon_get(weapon_index)->definition_index);
			struct weapon_hud_interface_definition *definitions[MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH] = { root_definition };
			long definition_indices[MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH] = { hud_index };
			unsigned long render_flags = crosshair->render_flags;
			short definition_count = 1;
			short definition_index;

			do
			{
				if (definitions[definition_count - 1]->parent_hud.index == NONE)
					break;
				definition_indices[definition_count] = definitions[definition_count - 1]->parent_hud.index;
				definitions[definition_count] = weapon_hud_interface_definition_get(definition_indices[definition_count]);
				definition_count++;
			}
			while (definition_count < MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH);

			if (definition_count == MAXIMUM_WEAPON_HUD_DEFINITION_DEPTH)
			{
				error(
					_error_silent,
					"too many levels in current weapon hud hierarchy");
			}

			for (definition_index = 0;
				definition_index < definition_count;
				definition_index++)
			{
				struct weapon_hud_interface_definition *definition = definitions[definition_index];
				struct hud_absolute_placement_definition absolute_placement = { _hud_anchor_center };
				boolean in_multiplayer = local_player_count() > 1;
				short crosshair_index;

				for (crosshair_index = 0;
					crosshair_index < definition->crosshairs.count;
					crosshair_index++)
				{
					struct weapon_hud_crosshairs_element *element = TAG_BLOCK_GET_ELEMENT(
						&definition->crosshairs,
						crosshair_index,
						struct weapon_hud_crosshairs_element);
					short state_index = element->crosshair_type;

					if (TEST_FLAG(render_flags, state_index) &&
						TEST_FLAG(map_type_flags, element->use_on_map_type))
					{
						struct crosshair_state *state = &crosshair->states[state_index];
						short item_index;

						for (item_index = 0;
							item_index < element->crosshairs.items.count;
							item_index++)
						{
							struct weapon_hud_crosshair_item *item = TAG_BLOCK_GET_ELEMENT(
								&element->crosshairs.items,
								item_index,
								struct weapon_hud_crosshair_item);

							if (!TEST_FLAG(item->flags, _hud_crosshair_runtime_invalid_bit) &&
								(!TEST_FLAG(item->flags, _hud_crosshair_not_on_default_zoom_bit) ||
									crosshair->states[_crosshair_state_zoom].value.reference_data > 0) &&
								(!TEST_FLAG(item->flags, _hud_crosshair_only_on_default_zoom_bit) ||
									crosshair->states[_crosshair_state_zoom].value.reference_data == 0))
							{
								real scale = local_player_count() > 1 &&
									!TEST_FLAG(item->placement.multiplayer_scaling_flags, _hud_dont_scale_size_bit) ?
									0.5f :
									1.0f;
								struct bitmap_group_sequence *sequence = !TEST_FLAG(item->flags, _hud_crosshair_not_a_sprite_bit) ?
									TAG_BLOCK_GET_ELEMENT(
										&bitmap_group_get(verify_tag_reference(&element->crosshairs.bitmap))->sequences,
										item->sequence_index,
										struct bitmap_group_sequence) :
									NULL;
								short frame_index;
								pixel32 color;

								switch (state_index)
								{
								case _crosshair_state_aim:
									if (TEST_FLAG(item->flags, _hud_crosshair_flashes_bit))
									{
										frame_index = 0;
										if (state->value.reference_data > 0)
										{
											color = get_flash_color(&item->colors, 0);
											goto draw_crosshair;
										}
									}
									else
									{
										frame_index = (short)state->value.reference_data;
									}
									goto use_flat_color;

								case _crosshair_state_zoom:
									if (TEST_FLAG(item->flags, _hud_crosshair_one_zoom_level_bit))
									{
										if (!state->value.reference_data)
											continue;
										frame_index = 0;
									}
									else
									{
										frame_index = (short)state->value.reference_data -
											TEST_FLAG(item->flags, _hud_crosshair_not_on_default_zoom_bit);
									}
									if (TEST_FLAG(item->flags, _hud_crosshair_flashes_bit) &&
										crosshair->states[_crosshair_state_aim].value.reference_data > 0)
									{
										color = get_flash_color(&item->colors, 0);
										goto draw_crosshair;
									}
									goto use_flat_color;

								case _crosshair_state_fired_with_no_ammo:
								case _crosshair_state_threw_with_no_grenade:
								case _crosshair_state_fired_secondary_with_no_ammo:
								case _crosshair_state_flash_fired_battery_depleted:
									if (state_index == _crosshair_state_flash_fired_battery_depleted)
									{
										firing_active = weapon_state->age == 0.0f &&
											TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit);
									}
									else if (state_index == _crosshair_state_fired_with_no_ammo)
									{
										firing_active = !weapon_state->magazines[0].rounds_loaded &&
											!weapon_state->magazines[0].rounds_remaining &&
											TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit);
									}
									else if (state_index == _crosshair_state_threw_with_no_grenade)
									{
										boolean no_grenades = TRUE;
										short grenade_type;

										for (grenade_type = 0; grenade_type < NUMBER_OF_UNIT_GRENADE_TYPES; grenade_type++)
										{
											no_grenades = no_grenades && !unit->unit.grenade_counts[grenade_type];
										}
										firing_active = no_grenades &&
											!unit->unit.grenade_throw_state &&
											TEST_FLAG(unit->unit.control_flags, _unit_control_throw_grenade_bit);
									}
									if (!firing_active &&
										game_time_get() - state->value.reference_data >= get_flash_duration(&item->colors))
									{
										state->value.reference_data = NONE;
									}
									if (state->value.reference_data == NONE)
										continue;
									/* fall through */

								case _crosshair_state_charge:
								case _crosshair_state_flash_ammo:
								case _crosshair_state_flash_heat:
								case _crosshair_state_flash_total_ammo:
								case _crosshair_state_flash_total_battery:
								case _crosshair_state_reload:
								case _crosshair_state_flash_ammo_none_for_reload:
								case _crosshair_state_flash_secondary_ammo:
								case _crosshair_state_flash_secondary_total_ammo:
								case _crosshair_state_secondary_reload:
								case _crosshair_state_flash_secondary_ammo_none_for_reload:
								case _crosshair_state_primary_trigger_ready:
								case _crosshair_state_secondary_trigger_ready:
									if (item->frame_rate > 0)
									{
										frame_index = (short)(((game_time_get() - state->value.reference_data) /
											item->frame_rate / TICKS_PER_SECOND) % sequence->sprites.count);
									}
									else
									{
										frame_index = 0;
									}
									if (TEST_FLAG(item->flags, _hud_crosshair_flashes_bit) &&
										state->value.reference_data != NONE)
									{
										color = get_flash_color(&item->colors, state->value.reference_data);
										goto draw_crosshair;
									}
								use_flat_color:
									color = item->colors.color;
									break;

								default:
									match_vassert(
										"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
										0x4A2,
										FALSE,
										NULL);
									break;
								}

							draw_crosshair:
								match_vassert(
									"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
									0x4A5,
									frame_index != NONE,
									csprintf(
										temporary,
										"frame index NONE when drawing crosshair %d, element %d, for weapon interface '%s'",
										crosshair_index,
										item_index,
										strip_path_name(tag_get_name(definition_indices[definition_index]))));
								{
									struct bitmap_group *bitmap_group = bitmap_group_get(verify_tag_reference(&element->crosshairs.bitmap));
									struct bitmap_data *bitmap = TAG_BLOCK_GET_ELEMENT(
										&bitmap_group->bitmaps,
										sequence ?
											TAG_BLOCK_GET_ELEMENT(&sequence->sprites, frame_index, struct bitmap_group_sprite)->bitmap_index :
											item->sequence_index,
										struct bitmap_data);

									if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
									{
										if (TEST_FLAG(item->flags, _hud_crosshair_hide_outside_area_bit))
										{
											real texel_scale_u = 1.0f;
											real texel_scale_v = 1.0f;
											boolean interface_bitmap = bitmap_group->type == _bitmap_group_type_interface_bitmaps;
											real_rectangle2d clip;
											real inverse_scale;
											real expand_u;
											real expand_v;

											if (sequence)
											{
												clip = TAG_BLOCK_GET_ELEMENT(&sequence->sprites, frame_index, struct bitmap_group_sprite)->bounds;
											}
											else
											{
												clip.x0 = 0.0f;
												clip.x1 = (real)(interface_bitmap ? bitmap->width : 1);
												clip.y0 = 0.0f;
												clip.y1 = (real)(interface_bitmap ? bitmap->height : 1);
												texel_scale_u = interface_bitmap ? 1.0 : (1.0f / bitmap->width) * 1.25;
												texel_scale_v = interface_bitmap ? 1.0 : (1.0f / bitmap->height) * 1.25;
											}

											inverse_scale = 1.0f / scale;
											expand_u = (bitmap->width - (render.camera.viewport_bounds.x1 - render.camera.viewport_bounds.x0) * inverse_scale) *
												texel_scale_u * -0.5f;
											expand_v = (bitmap->height - (render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0) * inverse_scale) *
												texel_scale_v * -0.5f;
											clip.x0 -= expand_u;
											clip.x1 += expand_u;
											clip.y0 -= expand_v;
											clip.y1 += expand_v;
											hud_draw_bitmap(
												bitmap,
												&absolute_placement,
												&item->placement,
												&clip,
												scale,
												0.0f,
												color,
												in_multiplayer,
												interface_bitmap,
												TRUE);
										}
										else
										{
											hud_draw_bitmap(
												bitmap,
												&absolute_placement,
												&item->placement,
												sequence ?
													&TAG_BLOCK_GET_ELEMENT(&sequence->sprites, frame_index, struct bitmap_group_sprite)->bounds :
													NULL,
												scale,
												0.0f,
												color,
												in_multiplayer,
												bitmap_group->type == _bitmap_group_type_interface_bitmaps,
												TRUE);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_weapon.c", 0x4E2);
	return;
}

static void render_weapon_hud(
	long hud_index,
	short local_player_index,
	struct weapon_definition const *weapon_definition,
	struct weapon_interface_state const *weapon_state,
	short const *new_state_flags,
	short const *new_overlay_flags,
	short const *new_numbers)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct weapon_hud_interface_definition *definition;
	struct weapon_hud_state *hud_state;
	short state_flags[NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES] = { 0 };
	short overlay_flags[NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES] = { 0 };
	short number_values[NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES] = { 0 };
	real number_fractions[NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES] = { 0.0f };
	short state_index;
	short map_type_flags;
	short element_index;

	csmemset(
		stack_buffer,
		0x62,
		sizeof(stack_buffer));
	definition = weapon_hud_interface_definition_get(hud_index);
	hud_state = get_hud_state(local_player_index);

	if (TEST_FLAG(
		definition->flash_cutoffs.flags,
		_hud_use_parent_flash_parameters_bit) &&
		new_state_flags &&
		new_overlay_flags &&
		new_numbers)
	{
		csmemcpy(
			state_flags,
			new_state_flags,
			sizeof(state_flags));
		csmemcpy(
			overlay_flags,
			new_overlay_flags,
			sizeof(overlay_flags));
		csmemcpy(
			number_values,
			new_numbers,
			sizeof(number_values));
	}
	else
	{
		struct weapon_flash_state_definition const *cutoffs =
			&definition->flash_cutoffs;
		short flags = state_flags[0];
		long age_percentage;

		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			weapon_state->magazines[0].rounds_remaining <= cutoffs->total_ammo);
		SET_FLAG(
			flags,
			_hud_draw_disabled_bit,
			weapon_state->magazines[0].rounds_remaining == 0);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[0] = flags;

		flags = state_flags[1];
		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			weapon_state->magazines[0].rounds_loaded <= cutoffs->loaded_ammo &&
				!weapon_state->magazines[0].reloading);
		SET_FLAG(flags, _hud_draw_disabled_bit, FALSE);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[1] = flags;

		flags = state_flags[2];
		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			weapon_state->heat * 100.0f >= cutoffs->heat);
		SET_FLAG(flags, _hud_draw_disabled_bit, FALSE);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[2] = flags;

		flags = state_flags[3];
		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			(1.0f - weapon_state->age) * 100.0f <= cutoffs->age);
		age_percentage = PIN(
			fast_ftol_C(weapon_state->age * 100.0f),
			0,
			100);
		SET_FLAG(
			flags,
			_hud_draw_disabled_bit,
			100 - age_percentage == 0);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[3] = flags;

		flags = state_flags[4];
		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			weapon_state->magazines[1].rounds_remaining <= cutoffs->total_ammo);
		SET_FLAG(
			flags,
			_hud_draw_disabled_bit,
			weapon_state->magazines[1].rounds_remaining == 0);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[4] = flags;

		flags = state_flags[5];
		SET_FLAG(
			flags,
			_hud_draw_flashing_bit,
			weapon_state->magazines[1].rounds_loaded <= cutoffs->loaded_ammo &&
				!weapon_state->magazines[1].reloading);
		SET_FLAG(flags, _hud_draw_disabled_bit, FALSE);
		SET_FLAG(
			flags,
			_hud_draw_in_multiplayer_bit,
			local_player_count() > 1);
		state_flags[5] = flags;

		for (state_index = 0;
			state_index < NUMBER_OF_WEAPON_HUD_FLASH_REFERENCES;
			state_index++)
		{
			if (TEST_FLAG(state_flags[state_index], _hud_draw_flashing_bit))
			{
				if (hud_state->last_weapon_flash_time[state_index] == NONE)
				{
					hud_state->last_weapon_flash_time[state_index] = game_time_get();
				}
			}
			else
			{
				hud_state->last_weapon_flash_time[state_index] = NONE;
			}
		}

		flags = overlay_flags[0];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			weapon_state->magazines[0].rounds_remaining <= cutoffs->total_ammo &&
				!weapon_state->magazines[0].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->magazines[0].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			weapon_state->magazines[0].rounds_remaining == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[0] = flags;

		flags = overlay_flags[1];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			weapon_state->magazines[0].rounds_loaded <= cutoffs->loaded_ammo);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->magazines[0].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			weapon_state->magazines[0].rounds_loaded == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[1] = flags;

		flags = overlay_flags[2];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			weapon_state->heat * 100.0f >= cutoffs->heat);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->overheated);
		age_percentage = PIN(
			fast_ftol_C(weapon_state->age * 100.0f),
			0,
			100);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			100 - age_percentage == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[2] = flags;

		flags = overlay_flags[3];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			(1.0f - weapon_state->age) * 100.0f <= cutoffs->age);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->overheated);
		age_percentage = PIN(
			fast_ftol_C(weapon_state->age * 100.0f),
			0,
			100);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			100 - age_percentage == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[3] = flags;

		flags = overlay_flags[0];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			weapon_state->magazines[1].rounds_remaining <= cutoffs->total_ammo &&
				!weapon_state->magazines[1].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->magazines[1].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			weapon_state->magazines[1].rounds_remaining == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[0] = flags;

		flags = overlay_flags[1];
		SET_FLAG(
			flags,
			_weapon_overlay_on_flashing_bit,
			weapon_state->magazines[1].rounds_loaded <= cutoffs->loaded_ammo);
		SET_FLAG(
			flags,
			_weapon_overlay_on_reload_bit,
			weapon_state->magazines[1].reloading);
		SET_FLAG(
			flags,
			_weapon_overlay_on_empty_bit,
			weapon_state->magazines[1].rounds_loaded == 0);
		SET_FLAG(
			flags,
			_weapon_overlay_on_default_bit,
			flags == 0);
		SET_FLAG(flags, _weapon_overlay_on_always_bit, TRUE);
		overlay_flags[1] = flags;

		number_values[0] = weapon_state->magazines[0].rounds_remaining;
		number_values[1] = weapon_state->magazines[0].rounds_loaded;
		number_values[2] = (short)(weapon_state->heat * 255.0f);
		number_values[3] = (short)((1.0f - weapon_state->age) * 100.0f);
		number_values[4] = weapon_state->magazines[1].rounds_remaining;
		number_values[5] = weapon_state->magazines[1].rounds_loaded;

		{
			long target_object_index =
				player_control_get_target_object_index(local_player_index);

			if (player_control_get_autoaim_level(local_player_index) == 1.0f &&
				target_object_index != NONE)
			{
				long player_index = local_player_get_player_index(local_player_index);
				long unit_index = player_index == NONE ?
					NONE :
					player_get(local_player_get_player_index(local_player_index))->unit_index;
				real_point3d camera_position;
				real_point3d target_position;
				real delta_x;
				real delta_y;
				real delta_z;

				unit_get_camera_position(unit_index, &camera_position);
				object_get_origin(target_object_index, &target_position);
				delta_x = camera_position.x - target_position.x;
				delta_y = camera_position.y - target_position.y;
				delta_z = camera_position.z - target_position.z;
				number_fractions[6] = square_root(
					delta_x * delta_x +
					delta_y * delta_y +
					delta_z * delta_z) * 3.0480001f;
				number_fractions[7] =
					(target_position.z - camera_position.z) * 3.0480001f;
			}
			else
			{
				union
				{
					unsigned long bits;
					real value;
				} no_target_value;

				no_target_value.bits = 0xFFC00000;
				number_fractions[6] = no_target_value.value;
				number_fractions[7] = no_target_value.value;
			}
		}
	}

	if (definition->parent_hud.index != NONE)
	{
		render_weapon_hud(
			definition->parent_hud.index,
			local_player_index,
			weapon_definition,
			weapon_state,
			state_flags,
			overlay_flags,
			number_values);
	}

	map_type_flags = global_scenario_get()->type != _scenario_type_main_menu;
	SET_FLAG(map_type_flags, 1, local_player_count() == 1);
	SET_FLAG(map_type_flags, 2, local_player_count() > 1);

	for (element_index = 0;
		element_index < definition->statics.count;
		element_index++)
	{
		struct weapon_hud_static_element *element = TAG_BLOCK_GET_ELEMENT(
			&definition->statics,
			element_index,
			struct weapon_hud_static_element);

		if (!TEST_FLAG(element->header.runtime_flags, _hud_element_runtime_invalid_bit) &&
			TEST_FLAG(map_type_flags, element->header.use_on_map_type))
		{
			state_index = element->header.state_type;
			hud_draw_static_element(
				local_player_index,
				&definition->absolute_placement,
				&element->static_element,
				state_flags[state_index],
				hud_state->last_weapon_flash_time[state_index]);
		}
	}

	for (element_index = 0;
		element_index < definition->meters.count;
		element_index++)
	{
		struct weapon_hud_meter_element *element = TAG_BLOCK_GET_ELEMENT(
			&definition->meters,
			element_index,
			struct weapon_hud_meter_element);

		if (!TEST_FLAG(element->header.runtime_flags, _hud_element_runtime_invalid_bit) &&
			TEST_FLAG(map_type_flags, element->header.use_on_map_type))
		{
			byte value;

			state_index = element->header.state_type;
			value = (byte)number_values[state_index];
			hud_draw_meter(
				local_player_index,
				&definition->absolute_placement,
				&element->meter_element,
				value,
				value,
				state_flags[state_index],
				(real)hud_state->last_weapon_flash_time[state_index],
				0.0f);
		}
	}

	for (element_index = 0;
		element_index < definition->numbers.count;
		element_index++)
	{
		struct weapon_hud_number_element *element = TAG_BLOCK_GET_ELEMENT(
			&definition->numbers,
			element_index,
			struct weapon_hud_number_element);

		if (!TEST_FLAG(element->header.runtime_flags, _hud_element_runtime_invalid_bit) &&
			TEST_FLAG(map_type_flags, element->header.use_on_map_type))
		{
			short magazine_size = 1;
			short value;
			short decimal_value;

			if (TEST_FLAG(
				element->weapon_flags,
				_weapon_number_divide_by_clip_size_bit))
			{
				struct weapon_magazine_definition const *magazine =
					TAG_BLOCK_GET_ELEMENT(
						&weapon_definition->weapon.magazines,
						0,
						struct weapon_magazine_definition);

				magazine_size = magazine->rounds_loaded_maximum;
			}

			state_index = element->header.state_type;
			if (element->number_element.fractional_digits)
			{
				union
				{
					unsigned long bits;
					real value;
				} fraction;
				real scale;

				fraction.value = number_fractions[state_index];
				if (fraction.bits == 0xFFC00000)
					continue;
				scale = power(10.0f, 4.0f);
				decimal_value = (short)fmod(
					fabs(number_fractions[state_index] * scale),
					scale);
				value = (short)fast_ftol_C(
					number_fractions[state_index] / magazine_size);
			}
			else
			{
				value = number_values[state_index] / magazine_size;
				decimal_value = NONE;
			}

			hud_draw_numbers(
				local_player_index,
				&definition->absolute_placement,
				&element->number_element,
				value,
				decimal_value,
				state_flags[state_index],
				hud_state->last_weapon_flash_time[state_index],
				0.0f);
		}
	}

	for (element_index = 0;
		element_index < definition->overlays.count;
		element_index++)
	{
		struct weapon_hud_overlays_element *element = TAG_BLOCK_GET_ELEMENT(
			&definition->overlays,
			element_index,
			struct weapon_hud_overlays_element);

		if (!TEST_FLAG(element->runtime_flags, _hud_element_runtime_invalid_bit) &&
			TEST_FLAG(map_type_flags, element->use_on_map_type))
		{
			state_index = element->state_type;
			hud_draw_weapon_overlays(
				local_player_index,
				&definition->absolute_placement,
				&element->overlays,
				overlay_flags[state_index],
				hud_state->last_weapon_flash_time[state_index],
				state_flags[state_index],
				local_player_count() > 1);
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_weapon.c", 0x308);
	return;
}

void hud_fix_weapon_data(
	short old_local_player_index,
	short new_local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x89,
		old_local_player_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x8A,
		new_local_player_index!=NONE);

	*get_hud_state(new_local_player_index) =
		*get_hud_state(old_local_player_index);
	*get_crosshair_state(new_local_player_index) =
		*get_crosshair_state(old_local_player_index);

	return;
}

void hud_update_weapon(
	void)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	short local_player_index;

	csmemset(
		stack_buffer,
		0x62,
		sizeof(stack_buffer));

	for (local_player_index = local_player_get_next(NONE);
		local_player_index != NONE;
		local_player_index = local_player_get_next(local_player_index))
	{
		long unit_index = local_player_get_player_index(local_player_index) == NONE ?
			NONE :
			player_get(local_player_get_player_index(local_player_index))->unit_index;

		if (unit_index != NONE)
		{
			long weapon_index = unit_inventory_get_weapon(
				unit_index,
				unit_get(unit_index)->unit.current_weapon_index);
			boolean clear_to_default = FALSE;

			if (weapon_index == NONE)
			{
				struct unit_datum *unit = unit_get(unit_index);

				if (unit->object.parent_object_index != NONE &&
					unit->unit.parent_seat_index != NONE)
				{
					struct unit_datum *parent = unit_get(
						unit->object.parent_object_index);
					struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
						&unit_definition_get(parent->definition_index)->unit.seats,
						unit->unit.parent_seat_index,
						struct unit_seat);

					if (TEST_FLAG(seat->flags, _unit_seat_gunner_bit))
					{
						weapon_index = unit_inventory_get_weapon(
							unit->object.parent_object_index,
							unit_get(unit->object.parent_object_index)->unit.current_weapon_index);
					}
					else
					{
						clear_to_default = TRUE;
					}
				}
			}

			if (weapon_index != NONE)
			{
				struct weapon_datum *weapon = weapon_get(weapon_index);
				struct weapon_definition *definition = weapon_definition_get(
					weapon->definition_index);
				struct weapon_interface_state weapon_state;

				weapon_build_weapon_interface_state(
					weapon_index,
					&weapon_state);
				if (definition->weapon.interface_definition.hud_interface.index != NONE)
				{
					long hud_index =
						definition->weapon.interface_definition.hud_interface.index;

					hud_update_weapon_local_player(
						local_player_index,
						weapon_index,
						hud_index,
						&weapon_state);
				}
			}
			else if (!clear_to_default && !unit_get_weapon_count(unit_index))
			{
				struct weapon_interface_state weapon_state = { 0 };
				long hud_index = hud_globals->defaults.default_weapon_hud.index;

				hud_update_weapon_local_player(
					local_player_index,
					NONE,
					hud_index,
					&weapon_state);
			}

			get_hud_state(local_player_index)->last_weapon_index = weapon_index;
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_weapon.c", 0xD4);

	return;
}

void hud_render_weapon_interface(
	struct player_datum *player)
{
	long weapon_index = unit_inventory_get_weapon(
		player->unit_index,
		unit_get(player->unit_index)->unit.current_weapon_index);
	boolean seat_disallows_hud = FALSE;

	if (weapon_index == NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);

		if (unit->object.parent_object_index != NONE && unit->unit.parent_seat_index != NONE)
		{
			struct unit_datum *parent = unit_get(unit->object.parent_object_index);
			struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
				&unit_definition_get(parent->definition_index)->unit.seats,
				unit->unit.parent_seat_index,
				struct unit_seat);

			if (TEST_FLAG(seat->flags, _unit_seat_gunner_bit))
			{
				weapon_index = unit_inventory_get_weapon(
					unit->object.parent_object_index,
					unit_get(unit->object.parent_object_index)->unit.current_weapon_index);
			}
			else
			{
				seat_disallows_hud = TRUE;
			}
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_weapon.c",
		0x1D8,
		player->local_player_index==render.local_player_index);

	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *definition = weapon_definition_get(
			weapon->definition_index);
		struct weapon_interface_state weapon_state;
		long hud_index;

		weapon_build_weapon_interface_state(
			weapon_index,
			&weapon_state);
		hud_index = definition->weapon.interface_definition.hud_interface.index;
		if (hud_index != NONE)
		{
			crosshairs_draw(
				player,
				weapon_index,
				hud_index,
				&weapon_state);
			render_weapon_hud(
				hud_index,
				player->local_player_index,
				definition,
				&weapon_state,
				NULL,
				NULL,
				NULL);
			play_weapon_hud_sounds(
				player->local_player_index,
				hud_index,
				&weapon_state);
		}
	}
	else if (!seat_disallows_hud && !unit_get_weapon_count(player->unit_index))
	{
		struct weapon_interface_state weapon_state = { 0 };

		crosshairs_draw(
			player,
			NONE,
			hud_globals->defaults.default_weapon_hud.index,
			&weapon_state);
	}

	render_grenade_hud(
		player->local_player_index,
		player->unit_index);
	if (player->local_player_index != NONE)
	{
		get_hud_state(player->local_player_index)->last_weapon_index = weapon_index;
	}

	return;
}

/* ---------- private code */

char *strip_path_name(
	char *path)
{
	char *name = strrchr(path, '\\');

	if (name)
		return name + 1;

	return path;
}
