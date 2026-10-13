/*
RASTERIZER_XBOX_MODELS.H

Narrow interface owned by RASTERIZER_XBOX_MODELS.C for the model backends
called by the public rasterizer wrappers.
*/

#ifndef __RASTERIZER_XBOX_MODELS_H
#define __RASTERIZER_XBOX_MODELS_H
#pragma once

#include "cseries.h"

struct render_sort_filth;
struct rasterizer_model_begin_parameters;
struct shader;
struct transparent_geometry_group;
struct triangle_buffer;
struct vertex_buffer;
union real_point3d;

void _rasterizer_models_begin(
	boolean sky);
void _rasterizer_models_end(
	void);
void _rasterizer_model_begin(
	struct rasterizer_model_begin_parameters const *parameters,
	boolean do_not_change_z_stencil_states);
void _rasterizer_model_draw(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index);
struct transparent_geometry_group *_rasterizer_model_transparent_geometry_submit(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index,
	union real_point3d const *centroid,
	struct render_sort_filth *sort_filth);
void _rasterizer_model_end(
	void);

#endif /* __RASTERIZER_XBOX_MODELS_H */
