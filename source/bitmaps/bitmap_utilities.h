/*
BITMAP_UTILITIES.H

Recovered public interface additions owned by BITMAP_UTILITIES.C.
*/

#ifndef __BITMAP_UTILITIES_H
#define __BITMAP_UTILITIES_H
#pragma once

#include "bitmaps/bitmaps.h"

void bitmap_fill(
	struct bitmap_data *bitmap,
	pixel32 fill_color);
void bitmap_alpha_to_rgb(
	struct bitmap_data *bitmap);
struct bitmap_data *bitmap_clone(
	struct bitmap_data *source_bitmap);
void bitmap_fade(
	struct bitmap_data *bitmap,
	pixel32 fade_color,
	real fade_amount);
void bitmap_smooth(
	struct bitmap_data *bitmap,
	real filter_size);
void bitmap_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount);
struct bitmap_data *bitmap_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels);
void bitmap_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes);
void bitmap_height_map(
	struct bitmap_data *bitmap,
	real bump_height);
void bitmap_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color);
void bitmap_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index);
union real_rgb_color *pixel32_to_real_rgb_color(
	pixel32 color,
	union real_rgb_color *result);

struct rgb_color
{
	word red;
	word green;
	word blue;
};

struct hsv_color
{
	word hue;
	word saturation;
	word value;
};

struct argb_color
{
	word alpha;
	word red;
	word green;
	word blue;
};

union real_hsv_color
{
	real n[3];
	struct
	{
		real hue;
		real saturation;
		real value;
	};
};

struct hsv_color *rgb_color_to_hsv_color(
	struct rgb_color const *rgb,
	struct hsv_color *hsv);
struct rgb_color *hsv_color_to_rgb_color(
	struct hsv_color const *hsv,
	struct rgb_color *rgb);
union real_hsv_color *real_rgb_color_to_real_hsv_color(
	union real_rgb_color const *rgb,
	union real_hsv_color *hsv);
union real_rgb_color *real_hsv_color_to_real_rgb_color(
	union real_hsv_color *hsv,
	union real_rgb_color *rgb);
union real_argb_color *argb_color_to_real_argb_color(
	struct argb_color const *source,
	union real_argb_color *result);
union real_rgb_color *rgb_color_to_real_rgb_color(
	struct rgb_color const *source,
	union real_rgb_color *result);

#endif /* __BITMAP_UTILITIES_H */
