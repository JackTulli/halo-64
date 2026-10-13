/*
RASTERIZER_MODELS.H
*/

#ifndef __RASTERIZER_MODELS_H
#define __RASTERIZER_MODELS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- structures */

struct model_geometry_part;
struct rasterizer_model_begin_parameters;
struct rasterizer_model_skinning;
struct render_sort_filth;
struct shader;
struct triangle_buffer;
struct vertex_buffer;
union real_point3d;

/* ---------- prototypes/RASTERIZER.C */

void rasterizer_model_begin(
	struct rasterizer_model_begin_parameters const *parameters,
	boolean is_dynamic);
void rasterizer_model_end(
	void);
void rasterizer_environment_shadow_model_begin(
	struct rasterizer_model_begin_parameters const *parameters);
void rasterizer_environment_shadow_model_end(
	void);
void rasterizer_model_draw(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index);
void rasterizer_model_transparent_geometry_submit(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index,
	union real_point3d const *centroid,
	struct render_sort_filth *sort_filth);
void rasterizer_environment_shadow_model_draw(
	struct shader const *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	struct vertex_buffer const *vertex_buffer);
void rasterizer_debug_model_vertices(
	long object_index,
	struct rasterizer_model_skinning const *skinning,
	struct model_geometry_part const *part);

#endif // __RASTERIZER_MODELS_H
