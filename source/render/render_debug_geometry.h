/*
RENDER_DEBUG_GEOMETRY.H

Narrow geometry interface owned by RENDER_DEBUG.C.
*/

#ifndef __RENDER_DEBUG_GEOMETRY_H
#define __RENDER_DEBUG_GEOMETRY_H
#pragma once

/* ---------- prototypes/RENDER_DEBUG.C */

void render_debug_triangle(
	boolean immediate,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2,
	real_argb_color const *color);
void render_debug_quadrilateral(
	boolean immediate,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2,
	real_point3d const *point3,
	real_argb_color const *color);
void render_debug_box(
	boolean immediate,
	real_rectangle3d const *bounds,
	real_argb_color const *color);
void render_debug_box2d_outline(
	boolean immediate,
	real_rectangle2d const *bounds,
	real_argb_color const *color);
void render_debug_polygon(
	real_point3d const *points,
	short point_count,
	real_argb_color const *color);
void render_debug_polygon_edges(
	real_point3d const *points,
	short point_count,
	real_argb_color const *color);
void render_debug_point2d(
	boolean immediate,
	real_plane3d const *plane,
	short projection,
	boolean sign,
	real_point2d const *point,
	real size,
	real_argb_color const *color,
	real offset);
void render_debug_line2d(
	boolean immediate,
	real_plane3d const *plane,
	short projection,
	boolean sign,
	real_point2d const *p0,
	real_point2d const *p1,
	real_argb_color const *color,
	real offset);
void render_debug_vector2d(
	boolean immediate,
	real_plane3d const *plane,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_vector2d const *vector,
	real size,
	real_argb_color const *color,
	real offset);

#endif // __RENDER_DEBUG_GEOMETRY_H
