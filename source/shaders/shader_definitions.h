/*
SHADER_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __SHADER_DEFINITIONS_H
#define __SHADER_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "real_math.h"
#include "shaders/shader_texture_animation.h"
#include "tag_groups.h"

/* ---------- constants */

/* ---------- macros */

#define SHADER_DEFINITION_TAG 'shdr'
#define shader_definition_get(index) \
	((struct shader *)tag_get(SHADER_DEFINITION_TAG, (index)))

/* ---------- structures */

struct shader_radiosity_properties
{
	unsigned short flags;
	short detail_level;
	real power;
	real_rgb_color color_of_emitted_light;
	real_rgb_color tint_color;
};

struct shader_physics_properties
{
	unsigned short flags;
	short material_type;
};

struct shader_base
{
	struct shader_radiosity_properties radiosity;
	struct shader_physics_properties physics;
	short type;
	short pad;
};

struct shader
{
	struct shader_base base;
};

struct shader_effect_definition
{
	struct shader shader;
	unsigned short flags;
	short framebuffer_blend_function;
	short framebuffer_fade_mode;
	unsigned short primary_map_flags;
	byte reserved_before_secondary_map[28];
	struct tag_reference secondary_map;
	short secondary_map_anchor;
	unsigned short secondary_map_flags;
	struct shader_texture_animation secondary_map_animation;
	real secondary_map_radius;
	real secondary_map_zsprite_radius_scale;
	byte reserved_after_secondary_map_zsprite_radius_scale[20];
};

typedef char shader_effect_secondary_map_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map) == 0x4C ? 1 : -1];
typedef char shader_effect_secondary_map_anchor_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map_anchor) == 0x5C ? 1 : -1];
typedef char shader_effect_secondary_map_flags_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map_flags) == 0x5E ? 1 : -1];
typedef char shader_effect_secondary_map_animation_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map_animation) == 0x60 ? 1 : -1];
typedef char shader_effect_secondary_map_radius_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map_radius) == 0x98 ? 1 : -1];
typedef char shader_effect_secondary_map_zsprite_radius_scale_offset_assert[
	offsetof(struct shader_effect_definition, secondary_map_zsprite_radius_scale) == 0x9C ? 1 : -1];
typedef char shader_effect_framebuffer_fade_mode_offset_assert[
	offsetof(struct shader_effect_definition, framebuffer_fade_mode) == 0x2C ? 1 : -1];
typedef char shader_effect_definition_size_assert[
	sizeof(struct shader_effect_definition) == 0xB4 ? 1 : -1];

/* ---------- prototypes/SHADER_DEFINITIONS.C */

struct shader *shader_get_and_verify_type(struct shader *shader, short shader_type);

/* ---------- globals */

extern struct shader_effect_definition global_shader_effect_additive;
extern struct shader_effect_definition global_shader_effect_alpha_blended;

/* ---------- public code */

#endif // __SHADER_DEFINITIONS_H
