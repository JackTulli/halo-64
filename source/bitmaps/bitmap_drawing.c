/*
BITMAP_DRAWING.C

symbols in this file:
00060610 00c0:
	_bitmap_initialize_line (0000)
000606D0 0140:
	_bitmap_step_line (0000)
00060810 02f0:
	_bitmap_fill_rectangle (0000)
00060B00 0ae0:
	_bitmap_copy (0000)
000615E0 0420:
	_bitmap_tile_and_bevel_rectangle (0000)
00061A00 03e0:
	_bitmap_draw_line (0000)
00061DE0 00c0:
	_bitmap_frame_rectangle (0000)
00251114 0012:
	_bitmap_bevel_sequence_indices (0000)
00251128 0035:
	??_C@_0DF@JBBFIHJO@bitmap?5?$EA?$CFp?5has?5bad?5encoding?5?$CD?$CFd?5@ (0000)
00251160 0028:
	??_C@_0CI@IJDPNIHH@c?3?2halo?2SOURCE?2bitmaps?2bitmap_dr@ (0000)
00251188 003f:
	??_C@_0DP@BDCJPGKD@no?5copy?5bitmap?5translation?5exist@ (0000)
002511C8 0016:
	??_C@_0BG@KLCGHOAK@?$CD?$CFd?$CL?$CD?$CFd?$CI?$CD?$CFd?0?$CD?$CFd?$CJ?$DM?$DN?$CD?$CFd?$AA@ (0000)
002511E0 0020:
	??_C@_0CA@IKDBNOCM@adjusted_destination_point?4y?$DO?$DN0?$AA@ (0000)
00251200 0020:
	??_C@_0CA@LHFBPHJM@adjusted_destination_point?4x?$DO?$DN0?$AA@ (0000)
00251220 0016:
	??_C@_0BG@OAHFNKOM@destination?5?$CG?$CG?5source?$AA@ (0000)
00251238 0039:
	??_C@_0DJ@ODLKIEJD@bitmap?5?$EA?$CFp?5has?5bad?5encoding?5?$CD?$CFd?5@ (0000)
00251274 001a:
	??_C@_0BK@LAJPHKOM@destination?9?$DObase_address?$AA@ (0000)
002DB678 0a54:
	_bitmap_copy_translation_table_data (0000)
	_translation_table (0010)
*/

/* ---------- headers */

#include "bitmaps/bitmap_drawing.h"

#include "bitmaps/bitmaps_internal.h"
#include "bitmaps/bitmap_group.h"
#include "cseries/errors.h"

/* ---------- constants */

enum
{
	NUMBER_OF_BITMAP_FORMATS = 18,
	NUMBER_OF_BITMAP_COPY_FLAG_COMBINATIONS = 4,
};

enum
{
	_bitmap_copy_blend_bit = 0,
	_bitmap_copy_modulate_bit,
};

enum
{
	_bitmap_copy_none = 0,
	_bitmap_copy_16bit,
	_bitmap_copy_a1r5g5b5_to_r5g6b5,
	_bitmap_copy_r5g6b5_to_a1r5g5b5,
	_bitmap_copy_r5g6b5_to_a8r8g8b8,
	_bitmap_copy_a4r4g4b4_to_a8r8g8b8,
	_bitmap_copy_a8r8g8b8,
	_bitmap_copy_a8r8g8b8_to_r5g6b5,
	_bitmap_copy_a8r8g8b8_to_a4r4g4b4,
	_bitmap_copy_a8r8g8b8_blend,
	_bitmap_copy_a4r4g4b4_to_a8r8g8b8_blend,
	_bitmap_copy_a8r8g8b8_modulate,
	_bitmap_copy_a4r4g4b4_to_a8r8g8b8_modulate_blend,
	_bitmap_copy_a8r8g8b8_modulate_blend,
};

enum
{
	_bitmap_format_a8 = 0,
	_bitmap_format_y8,
	_bitmap_format_ay8,
	_bitmap_format_r5g6b5 = 6,
	_bitmap_format_a8r8g8b8 = 11,
};

enum
{
	_bitmap_line_step_pixel = 0,
	_bitmap_line_step_vertical,
	_bitmap_line_step_horizontal,
};

enum
{
	_bitmap_fill_write_16bit = 0,
	_bitmap_fill_blend_a1r5g5b5,
	_bitmap_fill_blend_r5g6b5,
};

enum
{
	_bitmap_pixel_write_a8 = 0,
	_bitmap_pixel_write_16bit,
	_bitmap_pixel_write_32bit,
	_bitmap_pixel_blend_a1r5g5b5,
	_bitmap_pixel_blend_r5g6b5,
	_bitmap_pixel_blend_a8r8g8b8,
};

enum
{
	NUMBER_OF_BITMAP_BEVEL_PIECES = 9,
	NUMBER_OF_BITMAP_TILE_FLAGS_PER_PART = 2,
};

enum
{
	_bitmap_bevel_part_center = 0,
	_bitmap_bevel_part_corner,
	_bitmap_bevel_part_edge,
};

enum
{
	_bitmap_tile_part_draw_bit = 0,
	_bitmap_tile_part_blend_bit,
};

enum
{
	_bitmap_bevel_left_bit = 0,
	_bitmap_bevel_right_bit,
	_bitmap_bevel_top_bit,
	_bitmap_bevel_bottom_bit,
	_bitmap_bevel_corner_bit,
	_bitmap_bevel_clip_bit,
	_bitmap_bevel_center_bit,
};

/* ---------- macros */

/* ---------- structures */

struct bitmap_line_state
{
	short two_dx;
	short two_dy;
	short x_step;
	short y_step;
	short dx;
	short dy;
	short error;
	point2d point;
	point2d end_point;
};

/* ---------- prototypes */

void bitmap_initialize_line(
	struct bitmap_line_state *state,
	point2d const *p0,
	point2d const *p1);
boolean bitmap_step_line(
	struct bitmap_line_state *state,
	short step_mode);

/* ---------- globals */

short translation_table[NUMBER_OF_BITMAP_FORMATS][NUMBER_OF_BITMAP_FORMATS][NUMBER_OF_BITMAP_COPY_FLAG_COMBINATIONS] =
{
	{ 0 }, /* a8 */
	{ 0 }, /* y8 */
	{ 0 }, /* ay8 */
	{ 0 }, /* a8y8 */
	{ 0 }, /* unused1 */
	{ 0 }, /* unused2 */
	{ /* r5g6b5 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_16bit }, /* r5g6b5 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_r5g6b5_to_a8r8g8b8, _bitmap_copy_r5g6b5_to_a8r8g8b8 }, /* a8r8g8b8 */
	},
	{ 0 }, /* r6g5b5 */
	{ 0 }, /* a1r5g5b5 */
	{ /* a4r4g4b4 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_16bit }, /* a4r4g4b4 */
		{ 0 },
		{ _bitmap_copy_a4r4g4b4_to_a8r8g8b8, _bitmap_copy_a4r4g4b4_to_a8r8g8b8_blend, 0, _bitmap_copy_a4r4g4b4_to_a8r8g8b8_modulate_blend }, /* a8r8g8b8 */
	},
	{ /* x8r8g8b8 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_a8r8g8b8_to_r5g6b5 }, /* r5g6b5 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_a8r8g8b8, _bitmap_copy_a8r8g8b8, _bitmap_copy_a8r8g8b8_modulate, _bitmap_copy_a8r8g8b8_modulate }, /* a8r8g8b8 */
	},
	{ /* a8r8g8b8 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_a8r8g8b8_to_r5g6b5 }, /* r5g6b5 */
		{ _bitmap_copy_a8r8g8b8_to_a4r4g4b4 }, /* r6g5b5 */
		{ 0 },
		{ 0 },
		{ 0 },
		{ _bitmap_copy_a8r8g8b8, _bitmap_copy_a8r8g8b8_blend, _bitmap_copy_a8r8g8b8_modulate, _bitmap_copy_a8r8g8b8_modulate_blend }, /* a8r8g8b8 */
	},
};

word const bitmap_bevel_sequence_indices[NUMBER_OF_BITMAP_BEVEL_PIECES] =
{
	_bitmap_bevel_part_center,
	_bitmap_bevel_part_corner,
	_bitmap_bevel_part_corner,
	_bitmap_bevel_part_corner,
	_bitmap_bevel_part_corner,
	_bitmap_bevel_part_edge,
	_bitmap_bevel_part_edge,
	_bitmap_bevel_part_edge,
	_bitmap_bevel_part_edge,
};

static long bitmap_bevel_translation_flags[NUMBER_OF_BITMAP_BEVEL_PIECES] =
{
	FLAG(_bitmap_bevel_center_bit),
	FLAG(_bitmap_bevel_left_bit) | FLAG(_bitmap_bevel_top_bit) |
		FLAG(_bitmap_bevel_corner_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_right_bit) | FLAG(_bitmap_bevel_top_bit) |
		FLAG(_bitmap_bevel_corner_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_left_bit) | FLAG(_bitmap_bevel_bottom_bit) |
		FLAG(_bitmap_bevel_corner_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_right_bit) | FLAG(_bitmap_bevel_bottom_bit) |
		FLAG(_bitmap_bevel_corner_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_top_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_left_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_bottom_bit) | FLAG(_bitmap_bevel_clip_bit),
	FLAG(_bitmap_bevel_right_bit) | FLAG(_bitmap_bevel_clip_bit),
};

/* ---------- public code */

void bitmap_initialize_line(
	struct bitmap_line_state *state,
	point2d const *p0,
	point2d const *p1)
{
	state->dx = p1->x - p0->x;
	state->dy = p1->y - p0->y;
	state->two_dx = 2 * ABS(state->dx);
	state->two_dy = 2 * ABS(state->dy);
	state->x_step = (state->dx != 0) ? ((state->dx >= 0) ? 1 : -1) : 0;
	state->y_step = (state->dy != 0) ? ((state->dy >= 0) ? 1 : -1) : 0;
	state->point = *p0;
	state->end_point = *p1;
	state->error = (state->two_dx > state->two_dy) ?
		(state->two_dy - (state->two_dx >> 1)) :
		(state->two_dx - (state->two_dy >> 1));

	return;
}

boolean bitmap_step_line(
	struct bitmap_line_state *state,
	short step_mode)
{
	boolean done = FALSE;

	if (state->two_dx > state->two_dy)
	{
		if (state->point.x == state->end_point.x)
		{
			done = TRUE;
		}
		else
		{
			switch (step_mode)
			{
				case _bitmap_line_step_pixel:
					if (state->error >= 0)
					{
						state->point.y += state->y_step;
						state->error -= state->two_dx;
					}
					state->point.x += state->x_step;
					state->error += state->two_dy;
					break;

				case _bitmap_line_step_horizontal:
					while (state->error < 0 && state->point.x != state->end_point.x)
					{
						state->error += state->two_dy;
						state->point.x += state->x_step;
					}
					break;
			}
		}
	}
	else
	{
		if (state->point.y == state->end_point.y)
		{
			done = TRUE;
		}
		else
		{
			switch (step_mode)
			{
				case _bitmap_line_step_pixel:
					if (state->error >= 0)
					{
						state->point.x += state->x_step;
						state->error -= state->two_dy;
					}
					state->point.y += state->y_step;
					state->error += state->two_dx;
					break;

				case _bitmap_line_step_vertical:
					while (state->error < 0 && state->point.y != state->end_point.y)
					{
						state->error += state->two_dx;
						state->point.y += state->y_step;
					}
					break;
			}
		}
	}

	return done;
}

void bitmap_fill_rectangle(
	struct bitmap_data *destination,
	pixel32 color,
	rectangle2d const *rectangle,
	rectangle2d const *clip_rectangle)
{
	rectangle2d bounds = *rectangle;
	short alpha, inverse_alpha;
	short x_minimum, x_maximum, y_minimum, y_maximum;
	short width, y;
	short mode;
	long pixel;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		275,
		destination);

	if (clip_rectangle && !intersect_rectangles2d(rectangle, clip_rectangle, &bounds))
	{
		return;
	}

	alpha = color >> 24;
	inverse_alpha = 255 - alpha;

	switch (destination->format)
	{
		case _bitmap_format_r5g6b5:
			pixel = ((((color >> 16) & 0xf8) << 5) | ((color >> 8) & 0xfc)) << 3 | ((color >> 3) & 0x1f);
			mode = (alpha != 255) ? _bitmap_fill_blend_r5g6b5 : _bitmap_fill_write_16bit;
			break;

		/* mode and pixel are left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
				301,
				FALSE,
				csprintf(
					temporary,
					"bitmap @%p has bad encoding #%d for fill_rectangle()",
					destination,
					destination->format));
			break;
	}

	x_minimum = PIN(bounds.x0, 0, destination->width);
	x_maximum = PIN(bounds.x1, 0, destination->width);
	y_minimum = PIN(bounds.y0, 0, destination->height);
	y_maximum = PIN(bounds.y1, 0, destination->height);
	width = x_maximum - x_minimum;

	for (y = y_minimum; y < y_maximum; y++)
	{
		word *address = bitmap_2d_address(destination, x_minimum, y, 0);
		short x;

		switch (mode)
		{
			case _bitmap_fill_write_16bit:
				for (x = 0; x < width; x++)
				{
					*address++ = (word)pixel;
				}
				break;

			case _bitmap_fill_blend_a1r5g5b5:
				for (x = 0; x < width; x++)
				{
					word destination_pixel = *address;

					*address = (word)
						(((((destination_pixel & 0x1f) * inverse_alpha + (pixel & 0x1f) * alpha) >> 8) & 0x1f) |
						((((destination_pixel & 0x3ff) * inverse_alpha + (pixel & 0x3ff) * alpha) >> 8) & 0x3e0) |
						(((destination_pixel * inverse_alpha + pixel * alpha) >> 8) & 0x7c00));
				}
				break;

			case _bitmap_fill_blend_r5g6b5:
				for (x = 0; x < width; x++)
				{
					word destination_pixel = *address;

					*address = (word)
						(((((destination_pixel & 0x1f) * inverse_alpha + (pixel & 0x1f) * alpha) >> 8) & 0x1f) |
						((((destination_pixel & 0x7ff) * inverse_alpha + (pixel & 0x7ff) * alpha) >> 8) & 0x7e0) |
						(((destination_pixel * inverse_alpha + pixel * alpha) >> 8) & 0xf800));
				}
				break;
		}
	}

	return;
}

void bitmap_copy(
	struct bitmap_data *destination,
	point2d const *destination_point,
	rectangle2d const *destination_clip_rectangle,
	struct bitmap_data *source,
	rectangle2d const *source_rectangle,
	pixel32 color,
	short flags)
{
	rectangle2d source_bounds;
	rectangle2d destination_bounds;
	point2d source_point;
	point2d adjusted_destination_point;
	short translation;
	pixel32 converted_color;
	short y;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		458,
		destination && source);

	set_rectangle2d(&source_bounds, 0, 0, source->width, source->height);
	if (source_rectangle)
	{
		intersect_rectangles2d(source_rectangle, &source_bounds, &source_bounds);
	}
	set_point2d(&source_point, source_bounds.x0, source_bounds.y0);

	set_rectangle2d(&destination_bounds, 0, 0, destination->width, destination->height);
	if (destination_clip_rectangle)
	{
		intersect_rectangles2d(destination_clip_rectangle, &destination_bounds, &destination_bounds);
	}

	if (destination_point)
	{
		offset_rectangle2d(&source_bounds, destination_point->x, destination_point->y);
	}
	offset_rectangle2d(&source_bounds, -source_point.x, -source_point.y);

	if (!intersect_rectangles2d(&source_bounds, &destination_bounds, &source_bounds))
	{
		return;
	}

	set_point2d(&adjusted_destination_point, source_bounds.x0, source_bounds.y0);

	if (destination_point)
	{
		offset_rectangle2d(&source_bounds, -destination_point->x, -destination_point->y);
	}
	offset_rectangle2d(&source_bounds, source_point.x, source_point.y);

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		480,
		adjusted_destination_point.x>=0);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		481,
		adjusted_destination_point.y>=0);
	match_vassert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		484,
		adjusted_destination_point.x + rectangle2d_width(&source_bounds) <= destination->width,
		csprintf(
			temporary,
			"#%d+#%d(#%d,#%d)<=#%d",
			adjusted_destination_point.x,
			rectangle2d_width(&source_bounds),
			source_bounds.x0,
			source_bounds.x1,
			destination->width));
	match_vassert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		487,
		adjusted_destination_point.y + rectangle2d_height(&source_bounds) <= destination->height,
		csprintf(
			temporary,
			"#%d+#%d(#%d,#%d)<=#%d",
			adjusted_destination_point.y,
			rectangle2d_height(&source_bounds),
			source_bounds.y0,
			source_bounds.y1,
			destination->height));

	translation = translation_table[source->format][destination->format][flags];
	if (!translation)
	{
		match_vassert(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
			492,
			FALSE,
			csprintf(
				temporary,
				"no copy bitmap translation exists to copy %s to %s. (flags=%d)",
				bitmap_format_get_string(source->format),
				bitmap_format_get_string(destination->format),
				flags));

		return;
	}

	bitmap_format_get_bits_per_pixel(source->format);

	switch (translation)
	{
		case _bitmap_copy_a8r8g8b8_modulate:
		case _bitmap_copy_a8r8g8b8_modulate_blend:
			converted_color = color;
			break;

		case _bitmap_copy_a4r4g4b4_to_a8r8g8b8_modulate_blend:
			converted_color =
				((color >> 16) & 0xf000) | ((color >> 12) & 0xf00) | ((color >> 8) & 0xf0) | ((color >> 4) & 0xf);
			break;
	}

	for (y = source_bounds.y0; y < source_bounds.y1; y++)
	{
		void *source_address = bitmap_2d_address(source, source_bounds.x0, y, 0);
		void *destination_address = bitmap_2d_address(
			destination,
			adjusted_destination_point.x,
			y - source_bounds.y0 + adjusted_destination_point.y,
			0);
		short width = rectangle2d_width(&source_bounds);
		short x;

		switch (translation)
		{
			case _bitmap_copy_16bit:
				for (x = 0; x < width; x++)
				{
					*(*(word **)&destination_address)++ = *(*(word **)&source_address)++;
				}
				break;

			case _bitmap_copy_a1r5g5b5_to_r5g6b5:
				for (x = 0; x < width; x++)
				{
					word source_pixel = *(*(word **)&source_address)++;

					*(*(word **)&destination_address)++ = (word)(((source_pixel << 1) & 0xffc0) | (source_pixel & 0x3f));
				}
				break;

			case _bitmap_copy_r5g6b5_to_a1r5g5b5:
				for (x = 0; x < width; x++)
				{
					word source_pixel = *(*(word **)&source_address)++;

					*(*(word **)&destination_address)++ = (word)(((source_pixel >> 1) & 0x7fe0) | (source_pixel & 0x1f));
				}
				break;

			case _bitmap_copy_r5g6b5_to_a8r8g8b8:
				for (x = 0; x < width; x++)
				{
					word source_pixel = *(*(word **)&source_address)++;

					*(*(pixel32 **)&destination_address)++ = 0xff000000 |
						(((((source_pixel >> 11) & 0x1f) << 3) | (((source_pixel >> 11) & 0x1f) >> 2)) << 16) |
						(((((source_pixel >> 5) & 0x3f) << 2) | (((source_pixel >> 5) & 0x3f) >> 4)) << 8) |
						(((source_pixel & 0x1f) << 3) | ((source_pixel & 0x1f) >> 2));
				}
				break;

			case _bitmap_copy_a4r4g4b4_to_a8r8g8b8:
				for (x = 0; x < width; x++)
				{
					word source_pixel = *(*(word **)&source_address)++;

					*(*(pixel32 **)&destination_address)++ =
						(((((source_pixel >> 12) & 0xf) << 4) | ((source_pixel >> 12) & 0xf)) << 24) |
						(((((source_pixel >> 8) & 0xf) << 4) | ((source_pixel >> 8) & 0xf)) << 16) |
						(((((source_pixel >> 4) & 0xf) << 4) | ((source_pixel >> 4) & 0xf)) << 8) |
						(((source_pixel & 0xf) << 4) | (source_pixel & 0xf));
				}
				break;

			case _bitmap_copy_a4r4g4b4_to_a8r8g8b8_blend:
				for (x = 0; x < width; x++)
				{
					word pixel = *(*(word **)&source_address)++;
					pixel32 source_pixel =
						(((((pixel >> 12) & 0xf) << 4) | ((pixel >> 12) & 0xf)) << 24) |
						(((((pixel >> 8) & 0xf) << 4) | ((pixel >> 8) & 0xf)) << 16) |
						(((((pixel >> 4) & 0xf) << 4) | ((pixel >> 4) & 0xf)) << 8) |
						(((pixel & 0xf) << 4) | (pixel & 0xf));
					byte alpha = source_pixel >> 24;
					byte inverse_alpha = 255 - alpha;

					*(pixel32 *)destination_address =
						(((((source_pixel >> 16) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 16) & 0xff) * inverse_alpha) >> 8)) << 16 |
						(((((source_pixel >> 8) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 8) & 0xff) * inverse_alpha) >> 8)) << 8 |
						((((source_pixel & 0xff) * alpha) >> 8) + (((*(pixel32 *)destination_address & 0xff) * inverse_alpha) >> 8)) |
						MAX(alpha, *(pixel32 *)destination_address >> 24) << 24;
					(*(pixel32 **)&destination_address)++;
				}
				break;

			case _bitmap_copy_a4r4g4b4_to_a8r8g8b8_modulate_blend:
				for (x = 0; x < width; x++)
				{
					word pixel = *(*(word **)&source_address)++;
					pixel32 source_pixel =
						(((pixel >> 12) & 0xf) * ((converted_color >> 12) & 0xf)) << 24 |
						(((pixel >> 8) & 0xf) * ((converted_color >> 8) & 0xf)) << 16 |
						(((pixel >> 4) & 0xf) * ((converted_color >> 4) & 0xf)) << 8 |
						((pixel & 0xf) * (converted_color & 0xf));
					byte alpha = source_pixel >> 24;
					byte inverse_alpha = 255 - alpha;

					*(pixel32 *)destination_address =
						(((((source_pixel >> 16) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 16) & 0xff) * inverse_alpha) >> 8)) << 16 |
						(((((source_pixel >> 8) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 8) & 0xff) * inverse_alpha) >> 8)) << 8 |
						((((source_pixel & 0xff) * alpha) >> 8) + (((*(pixel32 *)destination_address & 0xff) * inverse_alpha) >> 8)) |
						MAX(alpha, *(pixel32 *)destination_address >> 24) << 24;
					(*(pixel32 **)&destination_address)++;
				}
				break;

			case _bitmap_copy_a8r8g8b8:
				for (x = 0; x < width; x++)
				{
					*(*(pixel32 **)&destination_address)++ = *(*(pixel32 **)&source_address)++;
				}
				break;

			case _bitmap_copy_a8r8g8b8_blend:
				for (x = 0; x < width; x++)
				{
					pixel32 source_pixel = *(*(pixel32 **)&source_address)++;
					byte alpha = source_pixel >> 24;
					byte inverse_alpha = 255 - alpha;

					*(pixel32 *)destination_address =
						(((((source_pixel >> 16) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 16) & 0xff) * inverse_alpha) >> 8)) << 16 |
						(((((source_pixel >> 8) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 8) & 0xff) * inverse_alpha) >> 8)) << 8 |
						((((source_pixel & 0xff) * alpha) >> 8) + (((*(pixel32 *)destination_address & 0xff) * inverse_alpha) >> 8)) |
						MAX(alpha, *(pixel32 *)destination_address >> 24) << 24;
					(*(pixel32 **)&destination_address)++;
				}
				break;

			case _bitmap_copy_a8r8g8b8_modulate:
				for (x = 0; x < width; x++)
				{
					pixel32 pixel = *(*(pixel32 **)&source_address)++;

					*(*(pixel32 **)&destination_address)++ =
						((((pixel >> 24) & 0xff) * ((converted_color >> 24) & 0xff) >> 8) << 24) |
						((((pixel >> 16) & 0xff) * ((converted_color >> 16) & 0xff) >> 8) << 16) |
						((((pixel >> 8) & 0xff) * ((converted_color >> 8) & 0xff) >> 8) << 8) |
						((pixel & 0xff) * (converted_color & 0xff) >> 8);
				}
				break;

			case _bitmap_copy_a8r8g8b8_modulate_blend:
				for (x = 0; x < width; x++)
				{
					pixel32 source_pixel = *(*(pixel32 **)&source_address)++;
					byte alpha;
					byte inverse_alpha;

					source_pixel =
						((((source_pixel >> 24) & 0xff) * ((converted_color >> 24) & 0xff) >> 8) << 24) |
						((((source_pixel >> 16) & 0xff) * ((converted_color >> 16) & 0xff) >> 8) << 16) |
						((((source_pixel >> 8) & 0xff) * ((converted_color >> 8) & 0xff) >> 8) << 8) |
						((source_pixel & 0xff) * (converted_color & 0xff) >> 8);
					alpha = source_pixel >> 24;
					inverse_alpha = 255 - alpha;

					*(pixel32 *)destination_address =
						(((((source_pixel >> 16) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 16) & 0xff) * inverse_alpha) >> 8)) << 16 |
						(((((source_pixel >> 8) & 0xff) * alpha) >> 8) + ((((*(pixel32 *)destination_address >> 8) & 0xff) * inverse_alpha) >> 8)) << 8 |
						((((source_pixel & 0xff) * alpha) >> 8) + (((*(pixel32 *)destination_address & 0xff) * inverse_alpha) >> 8)) |
						MAX(alpha, *(pixel32 *)destination_address >> 24) << 24;
					(*(pixel32 **)&destination_address)++;
				}
				break;

			case _bitmap_copy_a8r8g8b8_to_r5g6b5:
				for (x = 0; x < width; x++)
				{
					pixel32 source_pixel = *(*(pixel32 **)&source_address)++;

					*(*(word **)&destination_address)++ = (word)(
						((((source_pixel >> 16) & 0xff) >> 3) << 11) |
						((((source_pixel >> 8) & 0xff) >> 2) << 5) |
						((source_pixel & 0xff) >> 3));
				}
				break;

			case _bitmap_copy_a8r8g8b8_to_a4r4g4b4:
				for (x = 0; x < width; x++)
				{
					pixel32 source_pixel = *(*(pixel32 **)&source_address)++;

					*(*(word **)&destination_address)++ = (word)(
						(((source_pixel >> 24) >> 4) << 12) |
						((((source_pixel >> 16) & 0xff) >> 4) << 8) |
						((((source_pixel >> 8) & 0xff) >> 4) << 4) |
						((source_pixel & 0xff) >> 4));
				}
				break;

			default:
				match_vassert(
					"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
					640,
					FALSE,
					NULL);
				break;
		}
	}

	return;
}

void bitmap_tile_and_bevel_rectangle(
	struct bitmap_data *destination,
	long bitmap_tag_index,
	short sequence_index,
	rectangle2d const *rectangle,
	rectangle2d const *clip_rectangle,
	pixel32 color,
	long flags)
{
	rectangle2d destination_rectangle;
	rectangle2d bevel_bounds;
	rectangle2d bounds;
	point2d maximum[2];
	point2d minimum[2];
	rectangle2d bevel_rectangle;
	point2d destination_point;
	short bevel_index, bitmap_index;

	if (!rectangle)
	{
		destination_rectangle.x0 = 0;
		destination_rectangle.y0 = 0;
		destination_rectangle.x1 = destination->width;
		destination_rectangle.y1 = destination->height;
		rectangle = &destination_rectangle;
	}

	bounds = *rectangle;
	minimum[0].x = minimum[1].x = rectangle->x0;
	maximum[0].x = maximum[1].x = rectangle->x1;
	minimum[0].y = minimum[1].y = rectangle->y0;
	maximum[0].y = maximum[1].y = rectangle->y1;

	if (bitmap_tag_index != NONE)
	{
		struct bitmap_group *bitmap_group;
		struct bitmap_group_sequence *sequence;

		if (clip_rectangle &&
			!intersect_rectangles2d(clip_rectangle, &bounds, &bounds))
		{
			return;
		}

		bitmap_group = bitmap_group_get(bitmap_tag_index);
		if (sequence_index >= bitmap_group->sequences.count)
		{
			return;
		}
		sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group->sequences,
			sequence_index,
			struct bitmap_group_sequence);

		for (bevel_index = 0, bitmap_index = 0;
			bevel_index < NUMBER_OF_BITMAP_BEVEL_PIECES;
			bevel_index++)
		{
			struct bitmap_data *bitmap;

			if (bitmap_index >= sequence->bitmap_count)
			{
				break;
			}

			if (TEST_FLAG(
				flags,
				NUMBER_OF_BITMAP_TILE_FLAGS_PER_PART *
					bitmap_bevel_sequence_indices[bevel_index] +
					_bitmap_tile_part_draw_bit))
			{
				bitmap = bitmap_group_get_bitmap_from_sequence(
					bitmap_tag_index,
					sequence_index,
					bitmap_index++);
				if (bitmap)
				{
					rectangle2d *clip_bounds;

					bevel_rectangle = *rectangle;
					bevel_bounds = bounds;

					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_top_bit))
					{
						bevel_rectangle.y1 = rectangle->y0 + bitmap->height;
					}
					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_bottom_bit))
					{
						bevel_rectangle.y0 = rectangle->y1 - bitmap->height;
					}
					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_left_bit))
					{
						bevel_rectangle.x1 = rectangle->x0 + bitmap->width;
					}
					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_right_bit))
					{
						bevel_rectangle.x0 = rectangle->x1 - bitmap->width;
					}

					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_clip_bit))
					{
						clip_bounds = TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_corner_bit) ?
							&bevel_bounds :
							&bevel_rectangle;

						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_top_bit))
						{
							clip_bounds->x0 = MAX(minimum[0].x, clip_bounds->x0);
							clip_bounds->x1 = MIN(maximum[0].x, clip_bounds->x1);
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_bottom_bit))
						{
							clip_bounds->x0 = MAX(minimum[1].x, clip_bounds->x0);
							clip_bounds->x1 = MIN(maximum[1].x, clip_bounds->x1);
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_left_bit))
						{
							clip_bounds->y0 = MAX(minimum[0].y, clip_bounds->y0);
							clip_bounds->y1 = MIN(maximum[0].y, clip_bounds->y1);
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_right_bit))
						{
							clip_bounds->y0 = MAX(minimum[1].y, clip_bounds->y0);
							clip_bounds->y1 = MIN(maximum[1].y, clip_bounds->y1);
						}
					}

					if (TEST_FLAG(
						bitmap_bevel_translation_flags[bevel_index],
						_bitmap_bevel_corner_bit))
					{
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_top_bit) &&
							TEST_FLAG(
								bitmap_bevel_translation_flags[bevel_index],
								_bitmap_bevel_left_bit))
						{
							minimum[0].x = bevel_rectangle.x1;
							minimum[0].y = bevel_rectangle.y1;
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_bottom_bit) &&
							TEST_FLAG(
								bitmap_bevel_translation_flags[bevel_index],
								_bitmap_bevel_left_bit))
						{
							minimum[1].x = bevel_rectangle.x1;
							maximum[0].y = bevel_rectangle.y0;
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_top_bit) &&
							TEST_FLAG(
								bitmap_bevel_translation_flags[bevel_index],
								_bitmap_bevel_right_bit))
						{
							maximum[0].x = bevel_rectangle.x0;
							minimum[1].y = bevel_rectangle.y1;
						}
						if (TEST_FLAG(
							bitmap_bevel_translation_flags[bevel_index],
							_bitmap_bevel_bottom_bit) &&
							TEST_FLAG(
								bitmap_bevel_translation_flags[bevel_index],
								_bitmap_bevel_right_bit))
						{
							maximum[1].x = bevel_rectangle.x0;
							maximum[1].y = bevel_rectangle.y0;
						}
					}

					if (intersect_rectangles2d(
						&bevel_rectangle,
						&bevel_bounds,
						&bevel_bounds))
					{
						short bevel_width = rectangle2d_width(&bevel_rectangle);
						short bevel_height = rectangle2d_height(&bevel_rectangle);
						short columns = (short)(
							(bevel_width + bitmap->width - 1) / bitmap->width);
						short rows = (short)(
							(bevel_height + bitmap->height - 1) / bitmap->height);
						long copy_flags = 0;
						short x, y;

						if (TEST_FLAG(
							flags,
							NUMBER_OF_BITMAP_TILE_FLAGS_PER_PART *
								bitmap_bevel_sequence_indices[bevel_index] +
								_bitmap_tile_part_blend_bit))
						{
							SET_FLAG(copy_flags, _bitmap_copy_blend_bit, TRUE);
						}
						if (color)
						{
							SET_FLAG(copy_flags, _bitmap_copy_modulate_bit, TRUE);
						}

						for (y = 0; y < rows; y++)
						{
							for (x = 0; x < columns; x++)
							{
								set_point2d(
									&destination_point,
									(short)(bitmap->width * x + bevel_rectangle.x0),
									(short)(bitmap->height * y + bevel_rectangle.y0));
								bitmap_copy(
									destination,
									&destination_point,
									&bevel_bounds,
									bitmap,
									NULL,
									color,
									(short)copy_flags);
							}
						}
					}
				}
			}
		}
	}

	return;
}

void bitmap_draw_line(
	struct bitmap_data *destination,
	pixel32 color,
	rectangle2d const *clip_rectangle,
	real_point2d const *p0,
	real_point2d const *p1)
{
	short x_minimum = 0;
	short x_maximum = destination->width;
	short y_minimum = 0;
	short y_maximum = destination->height;
	short alpha = color >> 24;
	short inverse_alpha = 255 - alpha;
	short mode;
	long pixel;
	point2d point0, point1;
	struct bitmap_line_state state;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
		56,
		destination->base_address);

	switch (destination->format)
	{
		case _bitmap_format_a8:
		case _bitmap_format_y8:
		case _bitmap_format_ay8:
			pixel = color >> 24;
			mode = _bitmap_pixel_write_a8;
			break;

		case _bitmap_format_r5g6b5:
			pixel = ((((color >> 16) & 0xf8) << 5) | ((color >> 8) & 0xfc)) << 3 | ((color >> 3) & 0x1f);
			mode = (alpha != 255) ? _bitmap_pixel_blend_r5g6b5 : _bitmap_pixel_write_16bit;
			break;

		case _bitmap_format_a8r8g8b8:
			pixel = color;
			mode = (alpha != 255) ? _bitmap_pixel_blend_a8r8g8b8 : _bitmap_pixel_write_32bit;
			break;

		/* mode and pixel are left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_drawing.c",
				82,
				FALSE,
				csprintf(
					temporary,
					"bitmap @%p has bad encoding #%d for draw_line_software()",
					destination,
					destination->format));
			break;
	}

	if (clip_rectangle)
	{
		if (clip_rectangle->x0 > x_minimum)
		{
			x_minimum = clip_rectangle->x0;
		}
		if (clip_rectangle->x1 < x_maximum)
		{
			x_maximum = clip_rectangle->x1;
		}
		if (clip_rectangle->y0 > y_minimum)
		{
			y_minimum = clip_rectangle->y0;
		}
		if (clip_rectangle->y1 < y_maximum)
		{
			y_maximum = clip_rectangle->y1;
		}
	}

	if (p0->y > p1->y)
	{
		real_point2d const *swap = p0;

		p0 = p1;
		p1 = swap;
	}

	point0.x = (short)p0->x;
	point0.y = (short)p0->y;
	point1.x = (short)p1->x;
	point1.y = (short)p1->y;

	bitmap_initialize_line(&state, &point0, &point1);

	do
	{
		if (state.point.x >= x_minimum && state.point.x < x_maximum &&
			state.point.y >= y_minimum && state.point.y < y_maximum)
		{
			void *address = bitmap_2d_address(destination, state.point.x, state.point.y, 0);

			switch (mode)
			{
				case _bitmap_pixel_write_a8:
					*(byte *)address = (byte)pixel;
					break;

				case _bitmap_pixel_write_16bit:
					*(word *)address = (word)pixel;
					break;

				case _bitmap_pixel_write_32bit:
					*(pixel32 *)address = (pixel32)pixel;
					break;

				case _bitmap_pixel_blend_a1r5g5b5:
				{
					word destination_pixel = *(word *)address;

					*(word *)address = (word)
						(((((destination_pixel & 0x1f) * inverse_alpha + (pixel & 0x1f) * alpha) >> 8) & 0x1f) |
						((((destination_pixel & 0x3ff) * inverse_alpha + (pixel & 0x3ff) * alpha) >> 8) & 0x3e0) |
						(((destination_pixel * inverse_alpha + pixel * alpha) >> 8) & 0x7c00));
					break;
				}

				case _bitmap_pixel_blend_r5g6b5:
				{
					word destination_pixel = *(word *)address;

					*(word *)address = (word)
						(((((destination_pixel & 0x1f) * inverse_alpha + (pixel & 0x1f) * alpha) >> 8) & 0x1f) |
						((((destination_pixel & 0x7ff) * inverse_alpha + (pixel & 0x7ff) * alpha) >> 8) & 0x7e0) |
						(((destination_pixel * inverse_alpha + pixel * alpha) >> 8) & 0xf800));
					break;
				}

				case _bitmap_pixel_blend_a8r8g8b8:
				{
					pixel32 destination_pixel = *(pixel32 *)address;

					*(pixel32 *)address =
						(((((pixel >> 16) & 0xff) * alpha) >> 8) + ((((destination_pixel >> 16) & 0xff) * inverse_alpha) >> 8)) << 16 |
						(((((pixel >> 8) & 0xff) * alpha) >> 8) + ((((destination_pixel >> 8) & 0xff) * inverse_alpha) >> 8)) << 8 |
						((((pixel & 0xff) * alpha) >> 8) + (((destination_pixel & 0xff) * inverse_alpha) >> 8)) |
						MAX(alpha, destination_pixel >> 24) << 24;
					break;
				}
			}
		}
	}
	while (!bitmap_step_line(&state, _bitmap_line_step_pixel));

	return;
}

void bitmap_frame_rectangle(
	struct bitmap_data *destination,
	pixel32 color,
	real_rectangle2d const *bounds,
	rectangle2d const *clip_rectangle)
{
	real_point2d p0, p1;

	p0.x = bounds->x0;
	p0.y = bounds->y0;
	p1.x = bounds->x1 - 1.0f;
	p1.y = bounds->y0;
	bitmap_draw_line(destination, color, clip_rectangle, &p0, &p1);

	p0.x = bounds->x1 - 1.0f;
	p0.y = bounds->y1 - 1.0f;
	bitmap_draw_line(destination, color, clip_rectangle, &p1, &p0);

	p1.x = bounds->x0;
	p1.y = bounds->y1 - 1.0f;
	bitmap_draw_line(destination, color, clip_rectangle, &p0, &p1);

	p0.x = bounds->x0;
	p0.y = bounds->y0;
	bitmap_draw_line(destination, color, clip_rectangle, &p1, &p0);

	return;
}

/* ---------- private code */
