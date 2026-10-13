/*
BITMAPS_INTERNAL.H

Narrow cross-translation-unit interface owned by BITMAPS.C.
*/

#ifndef __BITMAPS_INTERNAL_H
#define __BITMAPS_INTERNAL_H
#pragma once

#include "bitmaps/bitmaps.h"

extern pixel32 global_vector_palette[];

char const *bitmap_format_get_string(
	short format);
pixel32 bitmap_format_to_a8r8g8b8(
	short format,
	void const *mipmap_address,
	long pixel_index);
byte palette_find_closest_match(
	pixel32 const *palette,
	pixel32 color);
void *bitmap_3d_address(
	struct bitmap_data *bitmap,
	short x,
	short y,
	short z,
	short mipmap_index);
void *bitmap_mipmap_address(
	struct bitmap_data *bitmap,
	short mipmap_index);
short bitmap_mipmap_get_width(
	struct bitmap_data *bitmap,
	short mipmap_index);
short bitmap_mipmap_get_height(
	struct bitmap_data *bitmap,
	short mipmap_index);
long bitmap_get_pixel_count(
	struct bitmap_data *bitmap);
long bitmap_get_pixel_data_size(
	struct bitmap_data *bitmap);
struct bitmap_data *bitmap_2d_new(
	short width,
	short height,
	short mipmap_count,
	short format);
struct bitmap_data *bitmap_3d_new(
	short width,
	short height,
	short depth,
	short mipmap_count,
	short format);
struct bitmap_data *bitmap_cube_map_new(
	short width,
	short mipmap_count,
	short format);
void bitmap_3d_slice_extract(
	struct bitmap_data *bitmap,
	short mipmap_index,
	short slice_index,
	struct bitmap_data *slice_bitmap);
void bitmap_3d_slice_insert(
	struct bitmap_data *slice_bitmap,
	struct bitmap_data *bitmap,
	short mipmap_index,
	short slice_index);
void bitmap_cube_map_face_extract(
	struct bitmap_data *bitmap,
	short mipmap_index,
	short face_index,
	struct bitmap_data *face_bitmap);
void bitmap_cube_map_face_insert(
	struct bitmap_data *face_bitmap,
	struct bitmap_data *bitmap,
	short mipmap_index,
	short face_index);

#endif /* __BITMAPS_INTERNAL_H */
