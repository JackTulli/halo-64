/*
LEAF_MAP.H

header included in hcex build.
*/

#ifndef __LEAF_MAP_H
#define __LEAF_MAP_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bsp3d;

struct leaf_map
{
	const struct bsp3d *bsp;
	struct tag_block leaves;
	struct tag_block portals;
};

struct leaf_portal
{
	long plane_index;
	long leaf_indices[2];
	struct tag_block vertices;			// real_point3d
};

typedef char leaf_portal_size_assert[
	sizeof(struct leaf_portal) == 0x18 ? 1 : -1];

/* ---------- prototypes/LEAF_MAP.C */

boolean leaf_map_initialize_from_bsp(
	struct leaf_map *leaf_map,
	struct bsp3d const *bsp,
	long leaf_count);
void leaf_map_delete(
	struct leaf_map *leaf_map);
void leaf_map_close_portal(
	struct leaf_map *leaf_map,
	long portal_index);
boolean leaf_map_leaf_is_closed(
	struct leaf_map const *leaf_map,
	long leaf_index);
void render_debug_leaf_portal(
	struct leaf_map const *leaf_map,
	long portal_designator);
void render_debug_leaf_portals(
	struct leaf_map const *leaf_map,
	long leaf_index);
void render_debug_leaf_faces(
	struct leaf_map const *leaf_map,
	long leaf_index);
boolean leaf_map_leaf_spans_polygon(
	struct leaf_map const *leaf_map,
	long leaf_index,
	real_plane3d const *plane,
	short projection,
	boolean sign,
	long vertex_count,
	real_point2d const *vertices);
void leaf_map_get_leaf_bounds(
	struct leaf_map const *leaf_map,
	long leaf_index,
	real_point3d *center,
	real *radius);
boolean leaf_map_closure(
	struct leaf_map const *leaf_map,
	long *marked_leaves,
	long *result);

/* ---------- globals */

extern boolean debug_leaf_portals;

/* ---------- public code */

#endif // __LEAF_MAP_H
