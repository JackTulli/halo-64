/*
WIDGET_TYPES.H

file has inline function assertions.
*/

#ifndef __WIDGET_TYPES_H
#define __WIDGET_TYPES_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

enum
{
	_widget_type_flag = 0,
	_widget_type_antenna,
	_widget_type_glow,
	_widget_type_light_volume,
	_widget_type_lightning,
	NUMBER_OF_WIDGET_TYPES,

	_widget_type_internal_sprite = NUMBER_OF_WIDGET_TYPES
};

enum
{
	_rasterizer_widget_z_enable_bit = 0,
	_rasterizer_widget_z_write_enable_bit,
	NUMBER_OF_RASTERIZER_WIDGET_FLAGS
};

/* ---------- macros */

/* ---------- structures */

struct render_animation;
struct render_lighting;

typedef void (*widget_type_lifecycle_proc)(
	void);
typedef long (*widget_type_new_proc)(
	long definition_index);
typedef void (*widget_type_delete_proc)(
	long datum_index);
typedef void (*widget_type_update_proc)(
	real delta);
typedef void (*widget_type_render_proc)(
	long object_index,
	long type_datum_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

struct widget_type_definition
{
	unsigned long group_tag;
	boolean needs_lighting;
	byte pad[3];
	widget_type_lifecycle_proc initialize;
	widget_type_lifecycle_proc initialize_for_new_map;
	widget_type_lifecycle_proc dispose_from_old_map;
	widget_type_lifecycle_proc dispose;
	widget_type_new_proc new_proc;
	widget_type_delete_proc delete_proc;
	widget_type_update_proc update;
	widget_type_render_proc render_proc;
};

typedef char widget_type_definition_size_assert[
	sizeof(struct widget_type_definition) == 0x28 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __WIDGET_TYPES_H
