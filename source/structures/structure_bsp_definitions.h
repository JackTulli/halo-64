/*
STRUCTURE_BSP_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __STRUCTURE_BSP_DEFINITIONS_H
#define __STRUCTURE_BSP_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "leaf_map.h"

#include "math/integer_math.h"
#include "rasterizer/rasterizer_geometry.h"
#include "render/render.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	STRUCTURE_BSP_TAG = 'sbsp',
};

/* ---------- macros */

#define structure_bsp_definition_get(index) ((struct structure_bsp *)tag_get(STRUCTURE_BSP_TAG, (index)))

/* ---------- structures */

struct structure_leaf
{
	byte_rectangle3d bounds;
	word pad;
	short cluster_index;
	short surface_reference_count;
	long first_surface_reference_index;
};

struct structure_surface
{
	word vertex_indices[NUMBER_OF_VERTICES_PER_TRIANGLE];
};

typedef char structure_surface_size_assert[
	sizeof(struct structure_surface) == 0x6 ? 1 : -1];

struct structure_material
{
	struct tag_reference shader;
	short permutation_index;
	word flags;
	long first_surface_index;
	long surface_count;
	real_point3d centroid;
	struct render_lighting lighting;
	real_plane3d plane;
	short breakable_surface_index;
	word copious_unused_space;
	struct vertex_buffer vertices;
	struct vertex_buffer lightmap_vertices;
	struct tag_data uncompressed_vertex_data;
	struct tag_data compressed_vertex_data;
};

struct structure_lightmap
{
	short bitmap_index;
	word pad;
	long unused[4];
	struct tag_block materials;	// structure_material
};

struct structure_cluster_runtime_decals
{
	byte unused[0xC];
	short first_decal_index;
	word decal_count;
	byte unused2[0x58];
};

/*
 * The January scenario code indexes this block with a 0x68-byte stride and
 * reads the background-sound palette index at +0x4.  The surrounding fields
 * remain unnamed until independently evidenced.
 */
struct structure_cluster
{
	short sky_index;
	short fog_reference;
	short background_sound_palette_index;
	short sound_environment_palette_index;
	short weather_palette_index;
	byte unusedA[0x1E];
	struct tag_block predicted_resources;
	byte unused34[0x34];
};

typedef char structure_cluster_size_assert[
	sizeof(struct structure_cluster) == 0x68 ? 1 : -1];
typedef char structure_cluster_fog_reference_offset_assert[
	offsetof(struct structure_cluster, fog_reference) == 0x02 ? 1 : -1];
typedef char structure_cluster_background_sound_offset_assert[
	offsetof(struct structure_cluster, background_sound_palette_index) == 0x04 ? 1 : -1];
typedef char structure_cluster_weather_offset_assert[
	offsetof(struct structure_cluster, weather_palette_index) == 0x08 ? 1 : -1];
typedef char structure_cluster_predicted_resources_offset_assert[
	offsetof(struct structure_cluster, predicted_resources) == 0x28 ? 1 : -1];

/*
 * The January scenario code indexes this palette with a 0x74-byte stride and
 * resolves the looping-sound reference at +0x20.
 */
struct structure_background_sound_palette_entry
{
	byte unused0[0x20];
	struct tag_reference background_sound;
	byte unused30[0x44];
};

typedef char structure_background_sound_palette_entry_size_assert[
	sizeof(struct structure_background_sound_palette_entry) == 0x74 ? 1 : -1];

struct structure_sound_environment_palette_entry
{
	byte unused0[0x20];
	struct tag_reference sound_environment;
	byte unused30[0x20];
};

typedef char structure_sound_environment_palette_entry_size_assert[
	sizeof(struct structure_sound_environment_palette_entry) == 0x50 ? 1 : -1];

struct structure_runtime_decal
{
	real_point3d position;
	byte palette_index;
	byte unused;
	char yaw;
	char pitch;
};

struct structure_fog_region
{
	byte unused[0x24];
	short fog_palette_index;
	short weather_palette_index;
};

struct structure_fog_plane
{
	short region_index;
	short runtime_material_type;
	real_plane3d plane;
	byte unused14[0xC];
};

typedef char structure_fog_plane_size_assert[
	sizeof(struct structure_fog_plane) == 0x20 ? 1 : -1];

typedef char structure_fog_region_size_assert[
	sizeof(struct structure_fog_region) == 0x28 ? 1 : -1];
typedef char structure_fog_region_weather_offset_assert[
	offsetof(struct structure_fog_region, weather_palette_index) == 0x26 ? 1 : -1];

struct structure_fog_palette_entry
{
	byte unused[0x20];
	struct tag_reference fog;
	byte unused2[0x58];
};

typedef char structure_fog_palette_entry_size_assert[
	sizeof(struct structure_fog_palette_entry) == 0x88 ? 1 : -1];

struct structure_bsp
{
	struct tag_reference lightmap_group;
	real vehicle_floor;
	real vehicle_ceiling;
	long sad_unused[5];
	struct render_lighting default_lighting;
	long lonely_unused;
	struct tag_block collision_materials;
	struct tag_block collision_bsp;			// collision_bsp
	struct tag_block nodes;
	real_rectangle3d world_bounds;
	struct tag_block leaves;				// structure_leaf
	struct tag_block surface_references;
	struct tag_block surfaces;
	struct tag_block lightmaps;				// structure_lightmap
	long render_unused[3];
	struct tag_block lens_flares;
	struct tag_block lens_flare_markers;
	struct tag_block clusters;
	struct tag_data cluster_data;
	struct tag_block cluster_portals;
	long cluster_unused[3];
	struct tag_block breakable_surfaces;
	struct tag_block fog_planes;
	struct tag_block fog_regions; // structure_fog_region
	struct tag_block fog_palette; // structure_fog_palette_entry
	long fog_unused[6];
	struct tag_block weather_palette;
	struct tag_block weather_polyhedra;
	long weather_unused[6];
	struct tag_block pathfinding_surfaces;
	struct tag_block pathfinding_edges;
	struct tag_block background_sound_palette;
	struct tag_block sound_environment_palette;
	struct tag_data sound_cluster_data;
	long sound_unused[6];
	struct tag_block markers;
	struct tag_block detail_object_data;
	struct tag_block runtime_decals;
	long diminishing_misc_unused[2];
	struct leaf_map leaf_map;
};

struct structure_collision_material
{
	struct tag_reference shader;
	word pad;
	short runtime_physics_material_type;
};

/* ---------- prototypes/STRUCTURE_BSP_DEFINITIONS.C */

unsigned long *structure_bsp_get_cluster_pvs(struct structure_bsp *structure_bsp, short cluster_index);
void structure_bsp_find_material_for_surface(struct structure_bsp *structure, long surface_index, short *lightmap_index, short *material_index);
void vertex_type_from_shader_tag(unsigned long group_tag, short *vertex_type, short *lightmap_vertex_type, boolean compressed);
byte *structure_bsp_get_cluster_encoded_sound_data(struct structure_bsp *structure_bsp, short row_index, short column_index);
byte structure_bsp_get_cluster_encoded_sound_distance(struct structure_bsp *structure_bsp, short from_cluster_index, short to_cluster_index);

/* ---------- globals */

/* ---------- public code */

#endif // __STRUCTURE_BSP_DEFINITIONS_H
