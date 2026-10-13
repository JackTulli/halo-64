/*
BITMAPS_SAMPLING.H

Narrow cross-translation-unit sampling interface owned by BITMAPS.C.
*/

#ifndef __BITMAPS_SAMPLING_H
#define __BITMAPS_SAMPLING_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/integer_math.h"

/* ---------- structures */

struct bitmap_data;
union real_point2d;

/* ---------- prototypes/BITMAPS.C */

pixel32 bitmap_2d_get_pixel(
	struct bitmap_data *bitmap,
	union real_point2d const *texcoord,
	real lod);

#endif /* __BITMAPS_SAMPLING_H */
