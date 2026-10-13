/*
RASTERIZER_DEBUG.H
*/

#ifndef __RASTERIZER_DEBUG_H
#define __RASTERIZER_DEBUG_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "math/real_math.h"

/* ---------- prototypes/RASTERIZER_DEBUG.C */

boolean rasterizer_debug_initialize(
	void);
long rasterizer_debug_new_primitive(
	long *primitive_count);
void rasterizer_debug_dispose(
	void);
void rasterizer_debug_test(
	void);
void rasterizer_debug_draw(
	void);
void rasterizer_debug_line_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_argb_color const *color0,
	real_argb_color const *color1);
void rasterizer_debug_triangle_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_argb_color const *color0,
	real_argb_color const *color1,
	real_argb_color const *color2);
void rasterizer_debug_triangle(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_argb_color const *color);

#endif // __RASTERIZER_DEBUG_H
