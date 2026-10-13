/*
RASTERIZER_WIDGETS.H

header included in hcex build.
*/

#ifndef __RASTERIZER_WIDGETS_H
#define __RASTERIZER_WIDGETS_H
#pragma once

/* ---------- headers */

#include "cseries.h"

#define RASTERIZER_WIDGET_SIGNATURES_OWNED

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

typedef void (*rasterizer_widget_render_proc)(
	long object_index,
	long widget_index);

/* ---------- prototypes/RASTERIZER.C */

void rasterizer_widget_submit(
	long object_index,
	long widget_index,
	real_point3d const *centroid,
	rasterizer_widget_render_proc render_proc);
void rasterizer_widget_begin(
	short type,
	word flags);
boolean rasterizer_widget_set_texture(
	short stage_index,
	long bitmap_group_index,
	short sequence_index);
void rasterizer_widget_set_tint_factor(
	real tint_factor);
void rasterizer_widget_set_zbuffer_enable(
	boolean zbuffer_enable);
void rasterizer_widget_draw_sprite2d(
	real_point2d const *point,
	real radius,
	real_vector2d const *scale,
	real_vector2d const *texture_scale,
	real rotation,
	unsigned long color);
void rasterizer_widget_draw_sprite3d(
	real_point3d const *point,
	real radius,
	real_vector2d const *scale,
	real rotation,
	unsigned long color);
long rasterizer_widget_submit_occlusion_test(
	real_point3d const *point,
	real radius,
	long index);
long rasterizer_widget_get_occlusion_test_result(
	long index);

/* ---------- prototypes/RASTERIZER_XBOX_WIDGETS.C */

void _rasterizer_widget_submit(
	long object_index,
	long widget_index,
	real_point3d const *centroid,
	rasterizer_widget_render_proc render_proc);
void _rasterizer_widget_begin(
	short type,
	word flags);
boolean _rasterizer_widget_set_texture(
	short stage_index,
	long bitmap_group_index,
	short sequence_index);
void _rasterizer_widget_set_tint_factor(
	real tint_factor);
void _rasterizer_widget_set_zbuffer_enable(
	boolean zbuffer_enable);
void _rasterizer_widget_draw_sprite2d(
	real_point2d const *point,
	real radius,
	real_vector2d const *scale,
	real_vector2d const *texture_scale,
	real rotation,
	unsigned long color);
void _rasterizer_widget_draw_sprite3d(
	real_point3d const *point,
	real radius,
	real_vector2d const *scale,
	real rotation,
	unsigned long color);
long _rasterizer_widget_submit_occlusion_test(
	real_point3d const *point,
	real radius,
	long index);
long _rasterizer_widget_get_occlusion_test_result(
	long index);
void _rasterizer_widget_end(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __RASTERIZER_WIDGETS_H
