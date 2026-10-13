/*
RASTERIZER_XBOX_INTERNAL.H

Narrow backend interface owned by RASTERIZER_XBOX.C and the Xbox rasterizer
family.  Keeping these declarations out of the broad public header preserves
January's compiler-sensitive declaration context for unrelated consumers.
*/

#ifndef __RASTERIZER_XBOX_INTERNAL_H
#define __RASTERIZER_XBOX_INTERNAL_H
#pragma once

#include "cseries.h"
#include "math/integer_math.h"

struct bitmap_data;
struct rasterizer_model_begin_parameters;
struct shader;
struct triangle_buffer;
struct vertex_buffer;
struct render_distant_light;
struct render_fog;
struct render_lighting;
struct rasterizer_frame_begin_parameters;
struct rasterizer_model_lighting_constants;
struct rasterizer_window_begin_parameters;

boolean _rasterizer_initialize(
	void);
void _rasterizer_reset_state(
	void);
void _rasterizer_frame_begin(
	struct rasterizer_frame_begin_parameters const *parameters);
void _rasterizer_frame_end(
	void);
void _rasterizer_present(
	struct bitmap_data *screenshot_bitmap,
	point2d const *screenshot_index);
void _rasterizer_windows_begin(
	void);
void _rasterizer_window_begin(
	struct rasterizer_window_begin_parameters const *parameters);
void _rasterizer_window_get_fog(
	struct render_fog *fog);
void _rasterizer_window_set_fog(
	struct render_fog const *fog);
void _rasterizer_window_end(
	void);
void _rasterizer_windows_end(
	void);
void _rasterizer_dispose(
	void);
void _rasterizer_set_vblank_callback(
	void (*callback)(unsigned long));

short rasterizer_get_stencil_mode(
	void);
void rasterizer_set_model_lighting(
	struct render_lighting const *lighting);
void rasterizer_set_model_lighting_point_light(
	long light_index,
	short constant_index,
	struct rasterizer_model_lighting_constants *lighting_constants);
void rasterizer_set_model_lighting_distant_light(
	struct render_distant_light const *light,
	short light_index,
	struct rasterizer_model_lighting_constants *lighting_constants);
void rasterizer_set_frustum_z(
	real z_near,
	real z_far);
void rasterizer_set_vertex_shader(
	short vertex_shader_index);
void *rasterizer_get_bitmap_default_hardware_format(
	struct bitmap_data const *bitmap);
void rasterizer_filthy_bitmap_default_initialize(
	void);
void rasterizer_secondary_render_target_debug(
	rectangle2d *bounds);
void rasterizer_water_set_visibility_for_frame(
	boolean visibility);
void rasterizer_decal_vertices_begin_update(
	void);
void rasterizer_decal_vertices_end_update(
	void);
boolean rasterizer_detail_objects_initialize(
	void);
void rasterizer_detail_objects_dispose(
	void);
boolean rasterizer_environment_fog_screen_initialize(
	void);
void rasterizer_environment_fog_screen_window_begin(
	void);
void rasterizer_environment_fog_screen_window_end(
	void);
void _rasterizer_environment_fog_screen_draw(
	struct shader const *shader,
	short bitmap_index,
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer);
void _rasterizer_environment_fog_screen_end(
	void);
boolean rasterizer_environment_fog_screen_model_begin(
	struct rasterizer_model_begin_parameters const *parameters);
void rasterizer_environment_fog_screen_model_end(
	void);
void rasterizer_environment_fog_screen_model_submit(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index);
void rasterizer_environment_fog_screen_dispose(
	void);

void _rasterizer_environment_reflection_mirrors_begin(
	void);
void _rasterizer_environment_reflection_mirror_draw(
	struct shader const *shader,
	short bitmap_index,
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer);
void _rasterizer_environment_reflection_mirrors_end(
	void);
void _rasterizer_environment_reflections_begin(
	void);
void _rasterizer_environment_reflection_draw(
	struct shader const *shader,
	short bitmap_index,
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer);
void _rasterizer_environment_reflections_end(
	void);

#endif /* __RASTERIZER_XBOX_INTERNAL_H */
