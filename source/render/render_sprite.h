/*
RENDER_SPRITE.H

header included in hcex build.
*/

#ifndef __RENDER_SPRITE_H
#define __RENDER_SPRITE_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"
#include "shaders/shader_definitions.h"

/* ---------- constants */

enum build_sprites_flags
{
	_build_sprites_screen_space_bit = 0,
	_build_sprites_first_person_bit,
	NUMBER_OF_BUILD_SPRITES_FLAGS,
};

enum build_sprite_flags
{
	_build_sprite_viewer_space_bit = 0,
	_build_sprite_u_mirror_bit,
	_build_sprite_v_mirror_bit,
	NUMBER_OF_BUILD_SPRITE_FLAGS,
};

enum build_sprite_orientation
{
	_build_sprite_normal = 0,
	_build_sprite_parallel,
	_build_sprite_perpendicular,
	NUMBER_OF_BUILD_SPRITE_ORIENTATIONS,
};

enum build_sprite_rotational_flags
{
	_build_sprite_rotational_viewer_space_bit = 0,
	_build_sprite_rotational_sideways_rotation_animates_bit,
	NUMBER_OF_BUILD_SPRITE_ROTATIONAL_FLAGS,
};

/* ---------- structures */

struct bitmap_data;

struct build_sprite_group
{
	long vertex_buffer_index;
	void *vertices;
	short sprite_count;
	word pad0A;
	struct bitmap_data *bitmap;
};

typedef char build_sprite_group_size_assert[
	sizeof(struct build_sprite_group) == 0x10 ? 1 : -1];

struct build_sprite_data
{
	long bitmap_group_index;
	short maximum_sprite_count;
	short screen_space;
	struct shader_effect_definition const *shader;
	short sprite_count;
	word pad0E;
	unsigned long flags;
	real_point3d centroid;
	short group_count;
	word pad22;
	struct build_sprite_group groups[8];
};

typedef char build_sprite_data_size_assert[
	sizeof(struct build_sprite_data) == 0xA4 ? 1 : -1];

/* ---------- prototypes/RENDER_SPRITE.C */

void build_sprites_begin(
	struct build_sprite_data *data,
	short maximum_sprite_count,
	long bitmap_group_index,
	struct shader_effect_definition const *shader,
	unsigned long flags);
void build_sprite(
	struct build_sprite_data *data,
	short mode,
	short sequence_index,
	short sprite_index,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_direction,
	real rotation,
	real scale,
	real_argb_color const *color,
	real fade,
	unsigned long flags);
void build_sprites_end(
	struct build_sprite_data *data);
void build_sprite_prepare_for_window(
	void);
real build_sprite_compute_vertex_fade(
	short fade_mode,
	real_point3d const *viewer_space_point,
	real_vector3d const *viewer_space_normal);
void build_sprite_rotational(
	struct build_sprite_data *data,
	unsigned long flags,
	short sequence_index,
	short sprite_index,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_axis_of_rotation,
	real rotation,
	real scale,
	real_argb_color const *color,
	real fade);

#endif // __RENDER_SPRITE_H
