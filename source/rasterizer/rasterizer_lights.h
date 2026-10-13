/*
RASTERIZER_LIGHTS.H
*/

#ifndef __RASTERIZER_LIGHTS_H
#define __RASTERIZER_LIGHTS_H
#pragma once

/* ---------- structures */

struct rasterizer_lens_flare_submit_parameters;

struct rasterizer_light_submit_parameters
{
	struct point_light_definition *definition;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real_rgb_color color;
	real radius;
};

struct rasterizer_lights
{
	long light_count;
	struct rasterizer_light_submit_parameters lights[MAXIMUM_LIGHTS_PER_WINDOW];
};

/* ---------- prototypes/RASTERIZER_LIGHTS.C */

void rasterizer_lights_begin_for_new_frame(
	void);
long rasterizer_light_submit(
	struct rasterizer_light_submit_parameters const *parameters);
void rasterizer_lens_flare_submit(
	struct rasterizer_lens_flare_submit_parameters const *parameters);
void rasterizer_lens_flare_submit_for_cluster(
	short cluster_index);
void rasterizer_lens_flares_submit_occlusion_tests(
	void);
void rasterizer_lens_flares_draw(
	void);
void rasterizer_sun_glow_draw(
	struct rasterizer_lens_flare_submit_parameters const *parameters);

/* ---------- globals */

extern struct rasterizer_lights rasterizer_lights;

#endif // __RASTERIZER_LIGHTS_H
