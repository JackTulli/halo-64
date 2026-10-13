/*
BITMAP_GROUP.H

header included in hcex build.
*/

#ifndef __BITMAP_GROUP_H
#define __BITMAP_GROUP_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	BITMAP_GROUP_TAG = 'bitm',
};

/* ---------- macros */

#define bitmap_group_get(index) ((struct bitmap_group *)tag_get(BITMAP_GROUP_TAG, (index)))

/* ---------- structures */

struct bitmap_data
{
	unsigned long signature;
	short width;
	short height;
	short depth;
	short type;
	short format;
	unsigned short flags;
	union point2d registration_point;
	short mipmap_count;
	short mipmap_pad;
	long pixels_offset;
	long pixels_size;
	long tag_index;
	long cache_block_index;
	void *hardware_format;
	void *base_address;
};

struct bitmap_group_sprite
{
	short bitmap_index;
	short bitmap_pad;
	long unused;
	real_rectangle2d bounds;
	real_point2d registration_point;
};

struct bitmap_group_sequence
{
	char name[32];
	short first_bitmap_index;
	short bitmap_count;
	long unused[4];
	struct tag_block sprites;
};

typedef char bitmap_group_sprite_size_assert[
	sizeof(struct bitmap_group_sprite) == 0x20 ? 1 : -1];
typedef char bitmap_group_sequence_size_assert[
	sizeof(struct bitmap_group_sequence) == 0x40 ? 1 : -1];

struct bitmap_group
{
	short type;
	short format;
	short usage;
	unsigned short flags;
	real detail_fade;
	real sharpen_amount;
	real bump_height;
	short sprite_budget_size;
	unsigned short sprite_budget_count;
	short import_width;
	short import_height;
	struct tag_data import_bitmap;
	struct tag_data pixel_data;
	real smoothing_filter_size;
	real alpha_bias;
	short mipmap_count;
	short sprite_usage;
	short sprite_spacing;
	unsigned short unused;
	struct tag_block sequences;
	struct tag_block bitmaps;
};

/* ---------- prototypes/BITMAP_GROUP.C */

struct bitmap_data *bitmap_group_get_bitmap_from_sequence(
	long bitmap_group_index,
	short sequence_index,
	short frame_index);

/* ---------- globals */

/* ---------- public code */

#endif // __BITMAP_GROUP_H
