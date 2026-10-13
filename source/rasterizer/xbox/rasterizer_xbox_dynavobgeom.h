/*
RASTERIZER_XBOX_DYNAVOBGEOM.H
*/

#ifndef __RASTERIZER_XBOX_DYNAVOBGEOM_H
#define __RASTERIZER_XBOX_DYNAVOBGEOM_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct bitmap_data;
struct dynamic_screen_vertex;
struct rasterizer_dynamic_screen_geometry_parameters;
struct render_animation;
struct shader;

/* ---------- prototypes/RASTERIZER_XBOX_DYNAVOBGEOM.C */

void _rasterizer_hud_begin(
	void);
void _rasterizer_hud_end(
	void);
void _rasterizer_dynamic_screen_geometry_draw(
	long primitive_type,
	long vertex_type,
	long dynamic_vertex_buffer_index,
	long vertex_count);
void _rasterizer_dynamic_screen_geometry_add_multitexture_params_to_base(
	struct rasterizer_dynamic_screen_geometry_parameters *base,
	struct rasterizer_dynamic_screen_geometry_parameters const *multitex_params);
void _rasterizer_dynamic_unlit_geometry_draw(
	struct shader const *shader,
	struct bitmap_data const *bitmap,
	struct render_animation const *animation,
	long dynamic_triangle_buffer_index,
	long dynamic_vertex_buffer_index,
	long vertex_count,
	real_point3d const *centroid,
	unsigned long geometry_flags);
void _rasterizer_dynamic_lit_geometry_draw(
	void const *vertices,
	void const *parameters);
void _rasterizer_psuedo_dynamic_screen_quad_draw(
	struct rasterizer_dynamic_screen_geometry_parameters *parameters,
	struct dynamic_screen_vertex *vertices);

#endif // __RASTERIZER_XBOX_DYNAVOBGEOM_H
