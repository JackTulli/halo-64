/*
RASTERIZER_FRAME_STATISTICS.H

Narrow cross-translation-unit interface owned by RASTERIZER_FRAME_STATISTICS.C.
*/

#ifndef __RASTERIZER_FRAME_STATISTICS_H
#define __RASTERIZER_FRAME_STATISTICS_H
#pragma once

#include "cseries.h"
#include <stddef.h>

/* January's 0x170-byte linker-common record. Only counters already used by
 * reconstructed writers are named here; unreviewed interiors stay reserved. */
struct rasterizer_frame_statistics_globals
{
	real frames_per_second;
	short fps_sample_count;
	short pad006;
	real average_frames_per_second;
	real minimum_frames_per_second;
	real maximum_frames_per_second;
	long fogged_object_count;
	long normal_object_count;
	long fast_object_count;
	long scenery_object_count;
	unsigned long lightmap_dynamic_vertex_count;
	unsigned long lightmap_dynamic_triangle_count;
	unsigned long lightmap_dynamic_draw_count;
	unsigned long shadow_count;
	unsigned long shadow_vertex_count;
	unsigned long shadow_triangle_count;
	unsigned long shadow_draw_count;
	unsigned long environment_dynamic_vertex_count;
	unsigned long environment_dynamic_triangle_count;
	unsigned long environment_dynamic_draw_count;
	unsigned long decal_vertex_count;
	unsigned long decal_triangle_count;
	unsigned long decal_draw_count;
	unsigned long decal_shader_change_count;
	unsigned long decal_texture_change_count;
	unsigned long diffuse_texture_dynamic_vertex_count;
	unsigned long diffuse_texture_dynamic_triangle_count;
	unsigned long diffuse_texture_dynamic_draw_count;
	unsigned long specular_light_dynamic_vertex_count;
	unsigned long specular_light_dynamic_triangle_count;
	unsigned long specular_light_dynamic_draw_count;
	unsigned long specular_lightmap_dynamic_vertex_count;
	unsigned long specular_lightmap_dynamic_triangle_count;
	unsigned long specular_lightmap_dynamic_draw_count;
	unsigned long reflection_mask_dynamic_vertex_count;
	unsigned long reflection_mask_dynamic_triangle_count;
	unsigned long reflection_mask_dynamic_draw_count;
	unsigned long reflection_dynamic_vertex_count;
	unsigned long reflection_dynamic_triangle_count;
	unsigned long reflection_dynamic_draw_count;
	unsigned long transparent_geometry_dynamic_vertex_count;
	unsigned long transparent_geometry_dynamic_triangle_count;
	long transparent_geometry_largest_dynamic_triangle_count;
	unsigned long transparent_geometry_dynamic_draw_count;
	unsigned long environment_fog_dynamic_vertex_count;
	unsigned long environment_fog_dynamic_triangle_count;
	unsigned long environment_fog_dynamic_draw_count;
	unsigned long environment_fog_screen_dynamic_vertex_count;
	unsigned long environment_fog_screen_dynamic_triangle_count;
	unsigned long environment_fog_screen_dynamic_draw_count;
	unsigned long environment_fog_screen_model_count;
	unsigned long environment_fog_screen_static_vertex_count;
	unsigned long environment_fog_screen_static_triangle_count;
	unsigned long environment_fog_screen_static_draw_count;
	unsigned long model_count;
	unsigned long model_vertex_count;
	unsigned long model_triangle_count;
	unsigned long model_draw_count;
	long transparent_model_vertex_count;
	long transparent_model_triangle_count;
	long transparent_model_maximum_triangle_count;
	long transparent_model_submit_count;
	unsigned long model_shadow_count;
	unsigned long model_shadow_vertex_count;
	unsigned long model_shadow_triangle_count;
	unsigned long model_shadow_draw_count;
	unsigned long dynamic_unlit_draw_count;
	unsigned long dynamic_unlit_triangle_count;
	long largest_dynamic_unlit_triangle_count;
	unsigned long dynamic_unlit_vertex_count;
	byte reserved114[0x1C];
	long dynamic_vertex_count;
	long dynamic_vertex_buffer_count;
	long dynamic_triangle_count;
	long dynamic_triangle_buffer_count;
	long debug_primitive_count;
	byte reserved144[4];
	long dynamic_light_count;
	long lens_flare_count;
	long vertex_shader_skinning_constant_bytes;
	long vertex_shader_lighting_constant_bytes;
	long vertex_shader_instruction_count;
	long pixel_shader_pushbuffer_bytes;
	unsigned long model_skinning_constant_bytes;
	unsigned long model_lighting_constant_bytes;
	unsigned long model_vertex_shader_work_accumulated;
	byte reserved16C[4];
};

typedef char rasterizer_frame_statistics_globals_size_assert[
	sizeof(struct rasterizer_frame_statistics_globals) == 0x170 ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_dynamic_vertex_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_dynamic_vertex_count) == 0xB8 ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_dynamic_triangle_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_dynamic_triangle_count) == 0xBC ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_dynamic_draw_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_dynamic_draw_count) == 0xC0 ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_model_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_model_count) == 0xC4 ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_static_vertex_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_static_vertex_count) == 0xC8 ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_static_triangle_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_static_triangle_count) == 0xCC ? 1 : -1];
typedef char rasterizer_frame_statistics_environment_fog_screen_static_draw_count_offset_assert[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		environment_fog_screen_static_draw_count) == 0xD0 ? 1 : -1];

extern struct rasterizer_frame_statistics_globals rasterizer_frame_statistics;

struct triangle_buffer;
struct vertex_buffer;

void rasterizer_frame_statistics_get_fps(
	struct rasterizer_frame_statistics_globals *frame_statistics);
void rasterizer_frame_statistics_draw(
	void);

long rasterizer_frame_statistics_count_static_vertices(
	struct triangle_buffer const *triangle_buffer,
	struct vertex_buffer const *vertex_buffer);
long rasterizer_frame_statistics_count_dynamic_vertices(
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count);

#endif /* __RASTERIZER_FRAME_STATISTICS_H */
