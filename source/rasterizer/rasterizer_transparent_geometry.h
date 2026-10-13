/*
RASTERIZER_TRANSPARENT_GEOMETRY.H

Narrow cross-translation-unit interface owned by RASTERIZER_TRANSPARENT_GEOMETRY.C.
*/

#ifndef __RASTERIZER_TRANSPARENT_GEOMETRY_H
#define __RASTERIZER_TRANSPARENT_GEOMETRY_H
#pragma once

#include "cseries.h"

struct transparent_geometry_group;

boolean rasterizer_transparent_geometry_initialize_aux_buffer(
	void);
void rasterizer_transparent_geometry_dispose_aux_buffer(
	void);
void rasterizer_transparent_geometry_dispose(
	void);
void rasterizer_transparent_geometry_groups_begin(
	void);
void rasterizer_transparent_geometry_groups_end(
	void);
void rasterizer_sort_external(
	void);
struct transparent_geometry_group *rasterizer_transparent_geometry_new_group(
	void);
struct transparent_geometry_group *rasterizer_transparent_geometry_new_group2(
	void);
short rasterizer_transparent_geometry_get_group_presorted_index(
	struct transparent_geometry_group const *group);
void rasterizer_transparent_geometry_group_draw(
	struct transparent_geometry_group *group,
	boolean dirty);
void rasterizer_transparent_geometry_set_group_pending_status(
	struct transparent_geometry_group const *group,
	boolean pending);
boolean rasterizer_transparent_geometry_get_group_pending_status(
	struct transparent_geometry_group const *group);
void *rasterizer_transparent_geometry_get_group_from_presorted_index(
	short presorted_index);
struct transparent_geometry_group *rasterizer_transparent_geometry_next_group(
	struct transparent_geometry_group const *group);
void *rasterizer_transparent_geometry_get_groups2(
	short *group_count);
short rasterizer_transparent_geometry_get_primary_vertex_type(
	struct transparent_geometry_group const *group);
void rasterizer_transparent_geometry_group_draw__internal(
	struct transparent_geometry_group const *group,
	boolean has_lightmap);

#endif /* __RASTERIZER_TRANSPARENT_GEOMETRY_H */
