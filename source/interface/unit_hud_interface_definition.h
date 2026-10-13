/*
UNIT_HUD_INTERFACE_DEFINITION.H

January unit-HUD tag layout, corroborated by HCEX PDB member records.
See docs/object_matching_logs/input_hud_player_ui_followup_20260904.md.
*/

#ifndef __UNIT_HUD_INTERFACE_DEFINITION_H
#define __UNIT_HUD_INTERFACE_DEFINITION_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"
#include "interface/hud_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	UNIT_HUD_INTERFACE_DEFINITION_TAG = 'unhi',
};

enum unit_hud_sound_state
{
	_unit_hud_shield_recharging = 0,
	_unit_hud_shield_damage,
	_unit_hud_shield_low,
	_unit_hud_shield_empty,
	_unit_hud_health_low,
	_unit_hud_health_empty,
	_unit_hud_minor_damage,
	_unit_hud_major_damage,

	NUMBER_OF_UNIT_HUD_STATES
};

enum auxilary_overlay_use_team_color_flags
{
	_auxilary_overlay_use_team_color_bit = 0,

	NUMBER_OF_UNIT_AUXILARY_OVERLAY_FLAGS
};

/* ---------- macros */

#define unit_hud_interface_definition_get(index) \
	((struct unit_hud_interface_definition *)tag_get( \
		UNIT_HUD_INTERFACE_DEFINITION_TAG, \
		(index)))

/* ---------- structures */

struct static_hud_element_definition
{
	struct hud_placement_definition placement;
	struct tag_reference interface_bitmap;
	struct hud_color_definition colors;
	short sequence_index;
	short pad;
	struct tag_block multitexture_overlays;
	long unused0[1];
};

struct meter_hud_element_definition
{
	struct hud_placement_definition placement;
	struct tag_reference meter_bitmap;
	unsigned long min_color;
	unsigned long max_color;
	unsigned long flash_color;
	unsigned long empty_color;
	byte meter_flags;
	byte minimum_value;
	short sequence_index;
	byte alpha_multiplier;
	byte alpha_bias;
	short value_scale;
	real opacity;
	real fade;
	unsigned long disabled_color;
	struct tag_block multitexture_overlays;
	long unused0[1];
};

struct shield_meter_extras_definition
{
	unsigned long overcharge_min_color;
	unsigned long overcharge_max_color;
	unsigned long overcharge_flash_color;
	unsigned long overcharge_empty_color;
};

struct health_meter_extras_definition
{
	unsigned long mid_color;
	real max_cutoff;
	real min_cutoff;
	long pad;
};

struct auxilary_meter_extras_definition
{
	real min_cutoff;
	unsigned long flags;
	long pad[2];
};

struct metered_panel_definition
{
	struct static_hud_element_definition background;
	struct meter_hud_element_definition meter;
	union
	{
		struct shield_meter_extras_definition shield_extras;
		struct health_meter_extras_definition health_extras;
		struct auxilary_meter_extras_definition aux_extras;
	};
	long unused[4];
};

struct motion_sensor_panel_definition
{
	struct static_hud_element_definition background;
	struct static_hud_element_definition foreground;
	long unused[8];
};

struct auxilary_panel_definition
{
	struct hud_absolute_placement_definition absolute_placement;
	struct tag_block auxilary_overlays;
	long unused[4];
};

struct auxilary_overlay_definition
{
	struct static_hud_element_definition static_element;
	short type;
	word flags;
	long unused0[6];
};

struct auxilary_meter_definition
{
	short type;
	short pad;
	long unused0[4];
	struct metered_panel_definition panel;
	long unused1[16];
};

struct unit_hud_interface_definition
{
	struct hud_absolute_placement_definition absolute_placement;
	struct static_hud_element_definition background;
	struct metered_panel_definition shield_meter;
	struct metered_panel_definition health_meter;
	struct motion_sensor_panel_definition motion_sensor;
	struct hud_placement_definition blip_placement;
	struct auxilary_panel_definition auxilary_panel;
	struct tag_block warning_sounds;
	struct tag_block auxilary_meters;
	long unused1[89];
	long unused2[12];
};

typedef char static_hud_element_definition_size_assert[
	sizeof(struct static_hud_element_definition) == 0x68 ? 1 : -1];
typedef char meter_hud_element_definition_size_assert[
	sizeof(struct meter_hud_element_definition) == 0x68 ? 1 : -1];
typedef char metered_panel_definition_shield_extras_offset_assert[
	offsetof(struct metered_panel_definition, shield_extras) == 0xD0 ? 1 : -1];
typedef char metered_panel_definition_size_assert[
	sizeof(struct metered_panel_definition) == 0xF0 ? 1 : -1];
typedef char motion_sensor_panel_definition_size_assert[
	sizeof(struct motion_sensor_panel_definition) == 0xF0 ? 1 : -1];
typedef char auxilary_panel_definition_size_assert[
	sizeof(struct auxilary_panel_definition) == 0x40 ? 1 : -1];
typedef char auxilary_overlay_definition_type_offset_assert[
	offsetof(struct auxilary_overlay_definition, type) == 0x68 ? 1 : -1];
typedef char auxilary_overlay_definition_flags_offset_assert[
	offsetof(struct auxilary_overlay_definition, flags) == 0x6A ? 1 : -1];
typedef char auxilary_overlay_definition_size_assert[
	sizeof(struct auxilary_overlay_definition) == 0x84 ? 1 : -1];
typedef char auxilary_meter_definition_panel_offset_assert[
	offsetof(struct auxilary_meter_definition, panel) == 0x14 ? 1 : -1];
typedef char auxilary_meter_definition_size_assert[
	sizeof(struct auxilary_meter_definition) == 0x144 ? 1 : -1];
typedef char unit_hud_interface_definition_warning_sounds_offset_assert[
	offsetof(struct unit_hud_interface_definition, warning_sounds) == 0x3C0 ? 1 : -1];
typedef char unit_hud_interface_definition_auxilary_meters_offset_assert[
	offsetof(struct unit_hud_interface_definition, auxilary_meters) == 0x3CC ? 1 : -1];
typedef char unit_hud_interface_definition_size_assert[
	sizeof(struct unit_hud_interface_definition) == 0x56C ? 1 : -1];

#endif // __UNIT_HUD_INTERFACE_DEFINITION_H
