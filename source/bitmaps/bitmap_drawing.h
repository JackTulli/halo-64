/*
BITMAP_DRAWING.H

header included in hcex build.
*/

#ifndef __BITMAP_DRAWING_H
#define __BITMAP_DRAWING_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/integer_math.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;

/* ---------- prototypes/BITMAP_DRAWING.C */

void bitmap_fill_rectangle(
	struct bitmap_data *destination,
	pixel32 color,
	rectangle2d const *rectangle,
	rectangle2d const *clip_rectangle);
void bitmap_copy(
	struct bitmap_data *destination,
	point2d const *destination_point,
	rectangle2d const *destination_clip_rectangle,
	struct bitmap_data *source,
	rectangle2d const *source_rectangle,
	pixel32 color,
	short flags);
void bitmap_tile_and_bevel_rectangle(
	struct bitmap_data *destination,
	long bitmap_tag_index,
	short sequence_index,
	rectangle2d const *rectangle,
	rectangle2d const *clip_rectangle,
	pixel32 color,
	long flags);
void bitmap_draw_line(
	struct bitmap_data *destination,
	pixel32 color,
	rectangle2d const *clip_rectangle,
	real_point2d const *p0,
	real_point2d const *p1);
void bitmap_frame_rectangle(
	struct bitmap_data *destination,
	pixel32 color,
	real_rectangle2d const *bounds,
	rectangle2d const *clip_rectangle);

/* ---------- globals */

/* ---------- public code */

#endif // __BITMAP_DRAWING_H
