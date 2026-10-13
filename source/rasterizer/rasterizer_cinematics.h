/*
RASTERIZER_CINEMATICS.H

header included in hcex build.
*/

#ifndef __RASTERIZER_CINEMATICS_H
#define __RASTERIZER_CINEMATICS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;

struct rasterizer_cinematic_screen_effect_parameters
{
	short convolution_extra_passes;
	short convolution_type;
	real convolution_radius;
	struct bitmap_data *convolution_mask;
	real filter_light_enhancement_intensity;
	real filter_desaturation_intensity;
	real_rgb_color filter_desaturation_tint;
	boolean filter_desaturation_is_additive;
	boolean filter_light_enhancement_uses_convolution_mask;
	boolean filter_desaturation_uses_convolution_mask;
	boolean video_on;
	short video_overbright_mode;
	byte reserved26[2];
	struct bitmap_data *video_scanline_map;
	real video_noise_intensity;
	real video_noise_map_scale;
	struct bitmap_data *video_noise_map;
};

typedef char rasterizer_cinematic_screen_effect_parameters_size_assert[
	sizeof(struct rasterizer_cinematic_screen_effect_parameters) == 0x38 ? 1 : -1];

/* ---------- prototypes/RASTERIZER_CINEMATICS.C */

void rasterizer_screen_effects_initialize(
	void);
void rasterizer_screen_effects_initialize_for_new_map(
	void);
void rasterizer_screen_effects_dispose_from_old_map(
	void);
void rasterizer_screen_effects_dispose(
	void);
void rasterizer_script_screen_effect_set_value(
	word effect_index,
	real value);
real rasterizer_script_screen_effect_get_value(
	short effect_index);
void rasterizer_screen_effect_start(
	boolean clear);
void rasterizer_screen_effect_set_convolution(
	short convolution_extra_passes,
	short convolution_type,
	real convolution_radius_lower_bound,
	real convolution_radius_upper_bound,
	real convolution_time);
void rasterizer_screen_effect_set_filter(
	real filter_light_enhancement_intensity_lower_bound,
	real filter_light_enhancement_intensity_upper_bound,
	real filter_desaturation_intensity_lower_bound,
	real filter_desaturation_intensity_upper_bound,
	boolean filter_desaturation_is_additive,
	real filter_time);
void rasterizer_screen_effect_set_filter_desaturation_tint(
	real red,
	real green,
	real blue);
void rasterizer_screen_effect_set_video(
	short video_overbright_mode,
	real video_noise_intensity);
void rasterizer_screen_effect_stop(
	void);
struct rasterizer_cinematic_screen_effect_parameters *rasterizer_screen_effect_get_cinematic_parameters(
	struct rasterizer_cinematic_screen_effect_parameters *parameters);

void rasterizer_set_near_clip_distance(
	real near_clip_distance);
real rasterizer_get_near_clip_distance(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __RASTERIZER_CINEMATICS_H
