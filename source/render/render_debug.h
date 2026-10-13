/*
RENDER_DEBUG.H

header included in hcex build.
*/

#ifndef __RENDER_DEBUG_H
#define __RENDER_DEBUG_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/RENDER_DEBUG.C */

void render_debug(
	void);
void render_debug_string(boolean immediate, const char *string);

void render_debug_string_at_point(
	boolean immediate,
	real_point3d const *point,
	const char *string,
	real_argb_color const *color);

void render_debug_point(boolean immediate, real_point3d const *point, real size, real_argb_color const *color);
void render_debug_line(boolean immediate, real_point3d const *point0, real_point3d const *point1, real_argb_color const *color);

void render_debug_vector(
	boolean immediate,
	real_point3d const *point,
	real_vector3d const *vector,
	real size,
	real_argb_color const *color);
void render_debug_vectors(
	boolean immediate,
	real_point3d const *point,
	real_vector3d const *forward,
	real_vector3d const *up,
	real size);
void render_debug_tick(
	boolean immediate,
	real_point3d const *point,
	real_vector3d const *tick_vector,
	real tick_size,
	real_argb_color const *color);

void render_debug_line_offset(
	boolean immediate,
	real_point3d const *p0,
	real_point3d const *p1,
	real_argb_color const *color,
	real offset);

void render_debug_matrix(
	boolean immediate,
	struct real_matrix4x3 const *matrix,
	real size);

void render_debug_sphere(
	boolean immediate,
	real_point3d const *center,
	real radius,
	real_argb_color const *color);
void render_debug_circle(
	boolean immediate,
	real_plane3d const *plane,
	short projection,
	boolean sign,
	real_point2d const *center,
	real radius,
	real_argb_color const *color,
	real offset);
void render_debug_cylinder(
	boolean immediate,
	real_point3d const *base,
	real_vector3d const *height,
	real width,
	real_argb_color const *color);
void render_debug_pill(
	boolean immediate,
	real_point3d const *base,
	real_vector3d const *height,
	real width,
	real_argb_color const *color);
void render_debug_box(
	boolean immediate,
	real_rectangle3d const *bounds,
	real_argb_color const *color);
void render_debug_box_outline(
	boolean immediate,
	real_rectangle3d const *bounds,
	real_argb_color const *color);
void render_debug_polygon(
	real_point3d const *points,
	short point_count,
	real_argb_color const *color);
void render_debug_polygon_edges(
	real_point3d const *points,
	short point_count,
	real_argb_color const *color);

/* ---------- globals */

/* ---------- public code */

#endif // __RENDER_DEBUG_H
