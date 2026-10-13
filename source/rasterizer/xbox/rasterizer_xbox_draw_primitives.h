/*
RASTERIZER_XBOX_DRAW_PRIMITIVES.H
*/

#ifndef __RASTERIZER_XBOX_DRAW_PRIMITIVES_H
#define __RASTERIZER_XBOX_DRAW_PRIMITIVES_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct triangle_buffer;
struct vertex_buffer;

/* ---------- prototypes/RASTERIZER_XBOX_DRAW_PRIMITIVES.C */

boolean rasterizer_dynamic_geometry_initialize(
	void);
void rasterizer_dynamic_geometry_begin(
	void);
void rasterizer_dynamic_geometry_end(
	void);
void rasterizer_dynamic_geometry_dispose(
	void);
long _rasterizer_dynamic_triangles_new(
	long count);
short *_rasterizer_dynamic_triangles_lock(
	long dynamic_triangle_buffer_index);
void _rasterizer_dynamic_triangles_unlock(
	long dynamic_triangle_buffer_index);
void _rasterizer_dynamic_triangles_delete(
	long dynamic_triangle_buffer_index);
long _rasterizer_dynamic_vertices_new(
	short type,
	long count);
short _rasterizer_dynamic_vertices_get_type(
	long dynamic_vertex_buffer_index);
void *_rasterizer_dynamic_vertices_lock(
	long dynamic_vertex_buffer_index);
void _rasterizer_dynamic_vertices_unlock(
	long dynamic_vertex_buffer_index);
void _rasterizer_dynamic_vertices_delete(
	long dynamic_vertex_buffer_index);
void rasterizer_draw_dynamic_vertices(
	long first_primitive_index,
	long primitive_count,
	long dynamic_vertex_buffer_index,
	short vertices_per_primitive);
void rasterizer_draw_dynamic_triangles_dynamic_vertices(
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	long dynamic_vertex_buffer_index);
void rasterizer_draw_dynamic_triangles_static_vertices(
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer);
void rasterizer_draw_dynamic_triangles_static_vertices2(
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer0,
	struct vertex_buffer const *vertex_buffer1);
void rasterizer_draw_static_triangles_dynamic_vertices(
	struct triangle_buffer const *triangle_buffer,
	long first_triangle_index,
	long triangle_count,
	long dynamic_vertex_buffer_index);
void rasterizer_draw_static_triangles_static_vertices(
	struct triangle_buffer const *triangle_buffer,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer);
void rasterizer_draw(
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index);

#endif // __RASTERIZER_XBOX_DRAW_PRIMITIVES_H
