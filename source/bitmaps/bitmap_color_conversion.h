/*
BITMAP_COLOR_CONVERSION.H
*/

#ifndef __BITMAP_COLOR_CONVERSION_H
#define __BITMAP_COLOR_CONVERSION_H
#pragma once

#include "cseries/cseries.h"
#include "math/integer_math.h"

pixel32 real_alpha_to_pixel32(
	real alpha);
pixel32 real_argb_color_to_pixel32(
	union real_argb_color const *color);
pixel32 real_rgb_color_to_pixel32(
	union real_rgb_color const *color);

#endif /* __BITMAP_COLOR_CONVERSION_H */
