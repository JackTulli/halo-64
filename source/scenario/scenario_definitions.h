/*
SCENARIO_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __SCENARIO_DEFINITIONS_H
#define __SCENARIO_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	SCENARIO_TAG = 'scnr',

	_scenario_type_solo = 0,
	_scenario_type_multiplayer,
	_scenario_type_main_menu,

	_scenario_object_placement_not_automatic_bit = 0,
	_scenario_object_placement_not_on_easy_bit,
	_scenario_object_placement_not_on_normal_bit,
	_scenario_object_placement_not_on_hard_bit,
	NUMBER_OF_SCENARIO_OBJECT_LOCATION_PLACEMENT_FLAGS,
};

enum
{
	_scenario_trigger_volume_type_axis_aligned = 0,
	_scenario_trigger_volume_type_oriented,
};

enum
{
	_weapon_created_at_rest_bit = 0,
	_weapon_obsolete_bit,
	_weapon_does_accelerate_bit,
	NUMBER_OF_SCENARIO_WEAPON_FLAGS,
};

enum netgame_flag_type
{
	_netgame_flag_ctf_flag = 0,
	_netgame_flag_ctf_vehicle,
	_netgame_flag_oddball_ball_spawn,
	_netgame_flag_race_track,
	_netgame_flag_race_vehicle,
	_netgame_flag_vegas_bank,
	_netgame_flag_teleporter_source,
	_netgame_flag_teleporter_target,
	_netgame_flag_hill,
	NUMBER_OF_NETGAME_FLAG_TYPES,
};

/* ---------- macros */

#define scenario_definition_get(index) ((struct scenario *)tag_get(SCENARIO_TAG, (index)))

/* ---------- structures */

struct scenario_object_palette_entry
{
	struct tag_reference reference;
	unsigned long unused[8];
};

struct scenario_object_datum
{
	short palette_entry_index;
	short name_index;
	word placement_flags;
	short variant_number;
	real_point3d position;
	real_euler_angles3d rotation;
	word on_bsp_flags;
	word misc_flags;
	unsigned long unused;
};

struct scenario_placeholder_datum
{
	struct scenario_object_datum object;
};

struct scenario_object_permutation
{
	unsigned long change_colors[4];
	byte region_permutations[8];
	unsigned long unused2[2];
};

struct scenario_weapon_datum
{
	struct scenario_object_datum object;
	struct scenario_object_permutation permutation;
	short rounds_total;
	short rounds_loaded;
	word flags;
	word pad;
	unsigned long unused[3];
};

struct scenario_object_name
{
	char name[TAG_STRING_LENGTH+1];
	short runtime_object_type;
	short runtime_scenario_datum_index;
};

struct player_starting_location
{
	real_point3d position;
	real facing;
	short team_index;
	short structure_bsp_reference_index;
	short game_types[4];
	byte unused1C[0x18];
};

typedef char player_starting_location_size_assert[
	sizeof(struct player_starting_location) == 0x34 ? 1 : -1];
typedef char player_starting_location_game_types_offset_assert[
	offsetof(struct player_starting_location, game_types) == 0x14 ? 1 : -1];

struct scenario_cutscene_camera_point
{
	long flags;
	char name[TAG_STRING_LENGTH+1];
	long pad;
	real_point3d position;
	real_euler_angles3d orientation;
	real field_of_view;
	long unused[9];
};

struct scenario_decal_palette_entry
{
	struct tag_reference reference;
};

struct scenario_structure_bsp_reference
{
	long file_offset;
	long file_size;
	void *base_address;
	byte unusedC[4];
	struct tag_reference structure_bsp;
};

typedef char scenario_structure_bsp_reference_size_assert[
	sizeof(struct scenario_structure_bsp_reference) == 0x20 ? 1 : -1];

struct scenario_trigger_volume
{
	short type;
	word pad;
	char name[TAG_STRING_LENGTH+1];
	byte unused[0xC];
	real_vector3d forward;
	real_vector3d up;
	union
	{
		struct
		{
			real_point3d position;
			real_vector3d extents;
		};
		real_rectangle3d bounds;
	};
};

typedef char scenario_trigger_volume_size_assert[
	sizeof(struct scenario_trigger_volume) == 0x60 ? 1 : -1];

struct scenario_starting_profile_weapon
{
	struct tag_reference weapon;
	short rounds_loaded;
	short rounds_total;
};

struct scenario_starting_profile
{
	char name[32];
	real starting_health_modifier;
	real starting_shield_modifier;
	struct scenario_starting_profile_weapon primary_weapon;
	struct scenario_starting_profile_weapon secondary_weapon;
	char grenade_counts[2];
	byte pad[22];
};

typedef char scenario_starting_profile_size_assert[
	sizeof(struct scenario_starting_profile) == 0x68 ? 1 : -1];

struct encounter_player_starting_location
{
	real_point3d position;
	real facing;
	short team_index;
	word pad12;
	short game_types[4];
	char __unknown1c[24];
};

struct scenario_netgame_flag
{
	real_point3d position;
	real facing;
	short type;
	short team_index;
	long unused[32];
};

struct scenario_netgame_equipment
{
	long flags;
	short game_type[4];
	short team_index;
	short spawn_time;
	long run_time_spawned_item_index;
	long unused1[11];
	real_point3d position;
	real facing;
	struct tag_reference item_collection;
	long unused2[12];
};

struct scenario_starting_equipment
{
	long flags;
	short game_type[4];
	long unused1[12];
	struct tag_reference item_collection[6];
	long unused2[12];
};

struct scenario
{
	struct tag_reference ugly_structure_bsp;
	struct tag_reference unloved_globals;
	struct tag_reference bad_sky;
	struct tag_block sky_references; // tag_reference
	short type;
	word flags;
	struct tag_block scenario_references;
	real local_north;
	unsigned long header_unused[5];
	long reference_unused[34];
	struct tag_block predicted_ui_resources;
	struct tag_block functions;
	struct tag_data editor_scenario_data;
	struct tag_block comments;
	long user_edit_unused[56];
	struct tag_block object_names;					// scenario_object_name
	struct tag_block scenery;
	struct tag_block scenery_palette;
	struct tag_block bipeds;
	struct tag_block biped_palette;
	struct tag_block vehicles;
	struct tag_block vehicle_palette;
	struct tag_block equipment;
	struct tag_block equipment_palette;
	struct tag_block weapons;
	struct tag_block weapon_palette;
	struct tag_block device_groups;
	struct tag_block machines;
	struct tag_block machine_palette;
	struct tag_block controls;
	struct tag_block control_palette;
	struct tag_block light_fixtures;
	struct tag_block light_fixtures_palette;
	struct tag_block sound_scenery;
	struct tag_block sound_scenery_palette;
	struct tag_block unused_blocks[7];
	struct tag_block starting_profiles;
	struct tag_block players;
	struct tag_block trigger_volumes; // scenario_trigger_volume
	struct tag_block recorded_animations;
	struct tag_block netgame_flags;
	struct tag_block netgame_equipment;
	struct tag_block scenario_starting_equipment;
	struct tag_block bsp_switch_trigger_volumes;
	struct tag_block decals;
	struct tag_block decal_palette;
	struct tag_block detail_object_collection_palette;
	long render_unused[21];
	struct tag_block ai_actor_palette;
	struct tag_block ai_encounters;						// encounter_definition
	struct tag_block ai_command_lists;
	struct tag_block ai_animation_references;
	struct tag_block ai_script_references;
	struct tag_block ai_recording_references;
	struct tag_block ai_conversations;
	struct tag_data hs_syntax_data;
	struct tag_data hs_string_constants;
	struct tag_block hs_scripts;
	struct tag_block hs_globals;
	struct tag_block hs_references;
	struct tag_block hs_source_files;
	long scripting_unused[6];
	struct tag_block cutscene_flags;
	struct tag_block cutscene_camera_points;			// scenario_cutscene_camera_point
	struct tag_block cutscene_chapter_titles;
	long rapidly_dwindling_unused_space[27];
	struct tag_reference custom_object_names;
	struct tag_reference ingame_help_text;
	struct tag_reference hud_messages;
	struct tag_block structure_bsp_references; // scenario_structure_bsp_reference
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __SCENARIO_DEFINITIONS_H
