/*
RASTERIZER_SWIZZLE.H

header included in hcex build.
*/

#ifndef __RASTERIZER_SWIZZLE_H
#define __RASTERIZER_SWIZZLE_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;

/* ---------- prototypes/RASTERIZER_SWIZZLE.C */

void bitmap_swizzle_vector2d(
	short dimension_x,
	short dimension_y,
	short x,
	short y,
	long *swizzled);
void bitmap_swizzle_vector3d(
	short dimension_x,
	short dimension_y,
	short dimension_z,
	short x,
	short y,
	short z,
	long *swizzled);
void rasterizer_xbox_bitmap_swizzle2d_byte(
	void *destination,
	void const *source,
	short width,
	short height);
void rasterizer_xbox_bitmap_swizzle2d_word(
	void *destination,
	void const *source,
	short width,
	short height);
void rasterizer_xbox_bitmap_swizzle2d_long(
	void *destination,
	void const *source,
	short width,
	short height);
void rasterizer_xbox_bitmap_swizzle3d_byte(
	void *destination,
	void const *source,
	short width,
	short height,
	short depth);
void rasterizer_xbox_bitmap_swizzle3d_word(
	void *destination,
	void const *source,
	short width,
	short height,
	short depth);
void rasterizer_xbox_bitmap_swizzle3d_long(
	void *destination,
	void const *source,
	short width,
	short height,
	short depth);
void rasterizer_xbox_bitmap_swizzle(
	struct bitmap_data *bitmap);
short rasterizer_xbox_bitmap_get_max_mipmap_count(
	struct bitmap_data *bitmap);
long rasterizer_xbox_bitmap_get_pixel_data_size(
	struct bitmap_data *bitmap);
boolean rasterizer_xbox_bitmap_rebuild_hardware_format(
	struct bitmap_data *bitmap);

/* ---------- globals */

/* ---------- public code */

#endif // __RASTERIZER_SWIZZLE_H
