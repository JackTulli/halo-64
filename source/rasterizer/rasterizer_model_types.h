/*
RASTERIZER_MODEL_TYPES.H

Shared model geometry and draw-packet layouts.
*/

#ifndef __RASTERIZER_MODEL_TYPES_H
#define __RASTERIZER_MODEL_TYPES_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "render/render.h"
#include "shaders/shaders.h"

/* ---------- structures */

struct shader;

struct model_vertex_uncompressed
{
	real_point3d position;
	real_vector3d normal;
	real_vector3d binormal;
	real_vector3d tangent;
	real_point2d texcoord;
	short nodes[2];
	real node_weights[2];
};

struct model_vertex_compressed
{
	real_point3d position;
	unsigned long normal;
	unsigned long binormal;
	unsigned long tangent;
	point2d texcoord;
	byte nodes[2];
	short node_weight;
};

struct rasterizer_model_skinning
{
	real_matrix4x3 const *node_matrices;
	short node_matrix_count;
	word pad;
};

struct render_model_effect
{
	short type;
	word pad;
	real intensity;
	real parameter;
	long source_object_index;
	real_point3d source_object_centroid;
	struct shader const *modifier_shader;
	struct render_animation modifier_animation;
};

struct rasterizer_model_begin_parameters
{
	unsigned long geometry_flags;
	long unique_identifier;
	struct rasterizer_model_skinning skinning;
	struct render_lighting lighting;
	struct render_animation animation;
	struct render_model_effect effect;
	real_point3d centroid;
	real radius;
	real_vector2d base_map_scale;
};

typedef char verify_model_vertex_uncompressed_size[
	sizeof(struct model_vertex_uncompressed) == 0x44 ? 1 : -1];
typedef char verify_model_vertex_compressed_size[
	sizeof(struct model_vertex_compressed) == 0x20 ? 1 : -1];
typedef char verify_rasterizer_model_skinning_size[
	sizeof(struct rasterizer_model_skinning) == 0x08 ? 1 : -1];
typedef char verify_render_model_effect_size[
	sizeof(struct render_model_effect) == 0x28 ? 1 : -1];
typedef char verify_rasterizer_model_begin_parameters_size[
	sizeof(struct rasterizer_model_begin_parameters) == 0xCC ? 1 : -1];

#endif // __RASTERIZER_MODEL_TYPES_H
