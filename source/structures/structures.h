/*
STRUCTURES.H

header included in hcex build.
*/

#ifndef __STRUCTURES_H
#define __STRUCTURES_H
#pragma once

/* ---------- constants */

enum
{
	MAXIMUM_COLLISION_MATERIALS_PER_STRUCTURE = 512,
	MAXIMUM_SURFACE_REFERENCES_PER_STRUCTURE = 0x40000,
	MAXIMUM_LIGHTMAPS_PER_STRUCTURE = 128,
	MAXIMUM_MATERIALS_PER_STRUCTURE_LIGHTMAP = 2048,
	MAXIMUM_SURFACES_PER_STRUCTURE_MATERIAL = 20000,
	MAXIMUM_VERTICES_PER_STRUCTURE_MATERIAL = 64000,
	MAXIMUM_CLUSTER_PORTALS_PER_CLUSTER = 128,
	MAXIMUM_MIRRORS_PER_CLUSTER = 16,
	MAXIMUM_SUBCLUSTERS_PER_CLUSTER = 4096,
	MAXIMUM_SURFACES_PER_SUBCLUSTER = 128,
	MAXIMUM_SURFACES_PER_CLUSTER = SHORT_MAX+1,
	MAXIMUM_VERTICES_PER_MIRROR = 512,
	MAXIMUM_TEMPORARY_CLUSTERS_PER_STRUCTURE = 8192,
	MAXIMUM_CLUSTERS_PER_STRUCTURE = 512,
	MAXIMUM_CLUSTER_DATA_SIZE = UNSIGNED_SHORT_MAX+1,
	MAXIMUM_CLUSTER_PORTALS_PER_STRUCTURE = 512,
	MAXIMUM_VERTICES_PER_CLUSTER_PORTAL = 128,
	MAXIMUM_FOG_PLANES_PER_STRUCTURE = 32,
	MAXIMUM_VERTICES_PER_STRUCTURE_FOG_PLANE = 4096,
	MAXIMUM_FOG_REGIONS_PER_STRUCTURE = 32,
	MAXIMUM_FOG_PALETTE_ENTRIES_PER_STRUCTURE = 32,
	MAXIMUM_WEATHER_PALETTE_ENTRIES_PER_STRUCTURE = 32,
	MAXIMUM_WEATHER_POLYHEDRA_PER_STRUCTURE = 32,
	MAXIMUM_PLANES_PER_WEATHER_POLYHEDRON = 16,
	MAXIMUM_BACKGROUND_SOUND_PALETTE_ENTRIES_PER_STRUCTURE = 64,
	MAXIMUM_SOUND_ENVIRONMENT_PALETTE_ENTRIES_PER_STRUCTURE = 64,
	MAXIMUM_MARKERS_PER_STRUCTURE = 1024,
	MAXIMUM_LENS_FLARES_PER_STRUCTURE = 256,
	MAXIMUM_LENS_FLARE_MARKERS_PER_STRUCTURE = UNSIGNED_SHORT_MAX+1,
	MAXIMUM_DECALS_PER_STRUCTURE = 6144,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/STRUCTURES.C */

void structures_initialize(
	void);
void structures_initialize_for_new_map(
	void);
void structures_dispose_from_old_map(
	void);
void structures_dispose(
	void);
void structure_cluster_marker_begin(
	void);
boolean structure_cluster_mark(
	short cluster_index);
void structure_cluster_marker_end(
	void);
short structure_clusters_in_cone(
	short position_cluster_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real distance,
	real sine,
	real cosine,
	short maximum_cluster_count,
	short *cluster_indices);
short structure_clusters_in_sphere(
	short cluster_index,
	real_point3d const *position,
	real radius,
	short maximum_count,
	short *intersected_indices);
void render_debug_fog_planes(
	void);

/* ---------- prototypes/STRUCTURE_DETAIL_OBJECTS.C */

void structure_detail_objects_initialize(
	void);
void structure_detail_objects_initialize_for_new_map(
	void);
void structure_detail_objects_dispose_from_old_map(
	void);
void structure_detail_objects_dispose(
	void);
void structure_detail_objects_flush(
	void);
void detail_object_offset(
	real offset);
void structure_render_detail_objects(
	void);
void render_debug_detail_objects(
	void);

/* ---------- prototypes/STRUCTURE_LENS_FLARES.C */

long cluster_index_from_point(struct structure_bsp const *structure_bsp, union real_point3d const *point);

/* ---------- prototypes/STRUCTURE_RUNTIME_DECALS.C */

void structure_decals_initialize(
	void);
void structure_decals_initialize_for_new_map(
	void);
void structure_decals_reconnect_to_structure_bsp(
	void);
void structure_decals_disconnect_from_structure_bsp(
	void);
void structure_decals_dispose_from_old_map(
	void);
void structure_decals_dispose(
	void);
void structure_decals_update(
	unsigned long *old_combined_pvs,
	unsigned long *new_combined_pvs,
	short cluster_count);

/* ---------- globals */

/* ---------- public code */

#endif // __STRUCTURES_H
