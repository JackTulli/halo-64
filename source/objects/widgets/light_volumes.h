/*
LIGHT_VOLUMES.H

header included in hcex build.
*/

#ifndef __LIGHT_VOLUMES_H
#define __LIGHT_VOLUMES_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/widgets/widget_types.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	_light_volume_color_interpolate_in_hsv_bit = 0,
	_light_volume_color_interpolate_along_farthest_hue_path_bit,
	NUMBER_OF_LIGHT_VOLUME_FLAGS
};

/* ---------- macros */

#define light_volume_definition_get(definition_index) \
	((struct light_volume_definition *)tag_get('mgs2', (definition_index)))

/* ---------- structures */

struct light_volume_datum
{
	struct datum_header header;
	short __unknown2;
	long definition_index;
};

struct light_volume_frame
{
	long unknown0[4];
	real offset_from_marker;
	real offset_exponent;
	real length;
	long unknown1[8];
	real radius_hither;
	real radius_yon;
	real radius_exponent;
	long unknown48[8];
	real_argb_color color_hither;
	real_argb_color color_yon;
	real color_exponent;
	real brightness_exponent;
	long unknown90[8];
};

struct light_volume_definition
{
	char attachment_marker[32];
	short type;
	word flags;
	long unknown24[4];
	real near_fade_distance;
	real far_fade_distance;
	real perpendicular_brightness_scale;
	real parallel_brightness_scale;
	short brightness_scale_source;
	word pad46;
	long unknown48[5];
	struct tag_reference map;
	short sequence_index;
	short count;
	long unknown70[18];
	short frame_animation_source;
	word padBA;
	long unknownBC[9];
	long unknownE0[16];
	struct tag_block frames;
	long unknown12C[8];
};

struct light_volume_globals
{
	struct light_volume_frame frame_storage;
	struct data_array *light_volume_data;
};

typedef char light_volume_datum_size_assert[
	sizeof(struct light_volume_datum) == 0x8 ? 1 : -1];
typedef char light_volume_frame_size_assert[
	sizeof(struct light_volume_frame) == 0xB0 ? 1 : -1];
typedef char light_volume_definition_size_assert[
	sizeof(struct light_volume_definition) == 0x14C ? 1 : -1];
typedef char light_volume_globals_size_assert[
	sizeof(struct light_volume_globals) == 0xB4 ? 1 : -1];
typedef char light_volume_datum_definition_index_offset_assert[
	offsetof(struct light_volume_datum, definition_index) == 0x4 ? 1 : -1];
typedef char light_volume_globals_light_volume_data_offset_assert[
	offsetof(struct light_volume_globals, light_volume_data) == 0xB0 ? 1 : -1];
typedef char light_volume_definition_attachment_marker_offset_assert[
	offsetof(struct light_volume_definition, attachment_marker) == 0x0 ? 1 : -1];
typedef char light_volume_definition_far_fade_distance_offset_assert[
	offsetof(struct light_volume_definition, far_fade_distance) == 0x38 ? 1 : -1];
typedef char light_volume_definition_brightness_scale_source_offset_assert[
	offsetof(struct light_volume_definition, brightness_scale_source) == 0x44 ? 1 : -1];
typedef char light_volume_definition_count_offset_assert[
	offsetof(struct light_volume_definition, count) == 0x6E ? 1 : -1];
typedef char light_volume_definition_frames_offset_assert[
	offsetof(struct light_volume_definition, frames) == 0x120 ? 1 : -1];
typedef char light_volume_definition_frames_count_offset_assert[
	offsetof(struct light_volume_definition, frames) +
		offsetof(struct tag_block, count) == 0x120 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

void light_volumes_initialize(
	void);
void light_volumes_dispose(
	void);
void light_volumes_initialize_for_new_map(
	void);
void light_volumes_dispose_from_old_map(
	void);

long light_volume_new(
	long definition_index);

void light_volume_delete(
	long light_volume_index);
void light_volume_render(
	long object_index,
	long light_volume_index);
void light_volume_submit(
	long object_index,
	long light_volume_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

/* ---------- public code */

#endif // __LIGHT_VOLUMES_H
