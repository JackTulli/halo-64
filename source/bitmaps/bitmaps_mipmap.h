/*
BITMAPS_MIPMAP.H

Narrow cross-translation-unit mipmap interface owned by BITMAPS.C.
*/

#ifndef __BITMAPS_MIPMAP_H
#define __BITMAPS_MIPMAP_H
#pragma once

/* ---------- structures */

struct bitmap_data;

/* ---------- prototypes/BITMAPS.C */

void *bitmap_mipmap_address(
	struct bitmap_data *bitmap,
	short mipmap_index);
void *bitmap_cube_map_address(
	struct bitmap_data *bitmap,
	short x,
	short y,
	short face_index,
	short mipmap_index);
short bitmap_mipmap_get_width(
	struct bitmap_data *bitmap,
	short mipmap_index);
short bitmap_mipmap_get_height(
	struct bitmap_data *bitmap,
	short mipmap_index);
short bitmap_mipmap_get_depth(
	struct bitmap_data *bitmap,
	short mipmap_index);
long bitmap_mipmap_get_pixel_count(
	struct bitmap_data *bitmap,
	short mipmap_index);
long bitmap_mipmap_get_pixel_data_size(
	struct bitmap_data *bitmap,
	short mipmap_index);
long bitmap_mipmap_get_row_pitch(
	struct bitmap_data *bitmap,
	short mipmap_index);
short bitmap_get_max_mipmap_count(
	struct bitmap_data *bitmap);

#endif /* __BITMAPS_MIPMAP_H */
