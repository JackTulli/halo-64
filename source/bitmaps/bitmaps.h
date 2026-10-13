/*
BITMAPS.H

header included in hcex build.
*/

#ifndef __BITMAPS_H
#define __BITMAPS_H
#pragma once

/* ---------- headers */

#include "math/integer_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;

/* ---------- prototypes/BITMAPS.C */

void bitmap_rebuild(
	struct bitmap_data *bitmap);
void bitmap_delete(
	struct bitmap_data *bitmap);

boolean bitmap_verify(
	struct bitmap_data *bitmap,
	boolean repair);

short bitmap_format_get_bits_per_pixel(
	short format);

void *bitmap_2d_address(
	struct bitmap_data *bitmap,
	short x,
	short y,
	short mipmap_index);

/* ---------- prototypes/BITMAPS_QUANTITIZE.C */

void bitmap_quantitize(
	struct bitmap_data *bitmap,
	short const *bits_per_channel);

/* ---------- prototypes/BITMAP_UTILITIES.C */

void bitmap_vector_map(
	struct bitmap_data *bitmap);

real real_rgb_color_brightness(union real_rgb_color const *color);
union real_argb_color *pixel32_to_real_argb_color(
	pixel32 color,
	union real_argb_color *result);

boolean valid_real_rgb_color(
	union real_rgb_color const *color);

union real_rgb_color *rgb_colors_interpolate(
	union real_rgb_color *rgb_result,
	unsigned long flags,
	union real_rgb_color const *rgb_lower_bound,
	union real_rgb_color const *rgb_upper_bound,
	real u);
union real_rgb_color *rgb_colors_interpolate_and_scale(
	union real_rgb_color *rgb_result,
	unsigned long flags,
	union real_argb_color const *argb_lower_bound,
	union real_argb_color const *argb_upper_bound,
	union real_rgb_color const *rgb_scale,
	real u);

/* ---------- globals */

/* ---------- public code */

#endif // __BITMAPS_H
