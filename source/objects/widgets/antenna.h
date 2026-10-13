/*
ANTENNA.H

header included in hcex build.
*/

#ifndef __ANTENNA_H
#define __ANTENNA_H
#pragma once

/* ---------- headers */

#include "objects/widgets/widget_types.h"
#include "memory/data.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	ANTENNA_TAG = 'ant!',
#ifdef HALO_N64
	/* (no widgets are made on the N64, widgets.c) */
	MAXIMUM_ANTENNAS = 1,
#else
	MAXIMUM_ANTENNAS = 12,
#endif
	MAXIMUM_ANTENNA_VERTICES = 20,
};

/* ---------- macros */

#define antenna_get(index) \
	((struct antenna_datum *)datum_get(antenna_data, (index)))
#define antenna_definition_get(index) \
	((struct antenna_definition *)tag_get(ANTENNA_TAG, (index)))

/* ---------- structures */

struct antenna_vertex_definition
{
	real spring_coefficient;
	long unknown004[6];
	real_euler_angles2d angle_to_next;
	real length_to_next;
	short sequence_index;
	word pad2A;
	real_argb_color color;
	real_argb_color lod_color;
	long unknown04C[10];
	real_vector3d vector_to_next;
};

struct antenna_definition
{
	char attachment_marker[32];
	struct tag_reference texture;
	struct tag_reference physics;
	long unknown040[20];
	real spring_coefficient;
	real falloff_pixels;
	real cutoff_pixels;
	real runtime_total_length;
	long unknown0A0[9];
	struct tag_block vertices;
};

struct antenna_vertex_datum
{
	real_point3d position;
	real_vector3d velocity;
	real sprite_scale;
	short sprite_index;
	word pad1E;
};

struct antenna_datum
{
	short identifier;
	word pad02;
	boolean initialized;
	boolean disabled;
	short updates_since_last_render;
	long definition_index;
	long object_index;
	real_point3d last_attachment_location;
	struct antenna_vertex_datum vertices[MAXIMUM_ANTENNA_VERTICES + 1];
};

typedef char antenna_vertex_definition_size_assert[
	sizeof(struct antenna_vertex_definition) == 0x80 ? 1 : -1];
typedef char antenna_definition_size_assert[
	sizeof(struct antenna_definition) == 0xD0 ? 1 : -1];
typedef char antenna_vertex_datum_size_assert[
	sizeof(struct antenna_vertex_datum) == 0x20 ? 1 : -1];
typedef char antenna_datum_size_assert[
	sizeof(struct antenna_datum) == 0x2BC ? 1 : -1];

/* ---------- prototypes/ANTENNA.C */

void antennas_initialize(
	void);
void antennas_initialize_for_new_map(
	void);
void antennas_dispose_from_old_map(
	void);
void antennas_dispose(
	void);

long antenna_new(
	long definition_index);
void antenna_delete(
	long antenna_index);
void antennas_update(
	real delta);
void antenna_render(
	long object_index,
	long antenna_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

/* ---------- globals */

extern struct data_array *antenna_data;

/* ---------- public code */

#endif // __ANTENNA_H
