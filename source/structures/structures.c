/*
STRUCTURES.C

symbols in this file:
00187C10 0010:
	_structures_initialize (0000)
00187C20 0010:
	_structures_initialize_for_new_map (0000)
00187C30 0010:
	_structures_dispose_from_old_map (0000)
00187C40 0010:
	_structures_dispose (0000)
00187C50 0040:
	_structure_cluster_marker_begin (0000)
00187C90 0080:
	_structure_cluster_unmarked (0000)
00187D10 0080:
	_structure_cluster_mark (0000)
00187D90 0040:
	_structure_cluster_marker_end (0000)
00187DD0 01b0:
	_structure_render_surface_from_point_and_leaf (0000)
00187F80 00d0:
	_structure_get_planar_fog_definition_index (0000)
00188050 01b0:
	_sphere_intersects_cluster_portal (0000)
00188200 0120:
	_code_00188200 (0000)
00188320 01e0:
	_structure_clusters_in_cone (0000)
00188500 0260:
	_structure_test_vector (0000)
00188760 01c0:
	_structure_get_planar_fog (0000)
00188920 0160:
	_render_debug_fog_planes (0000)
00188A80 0130:
	_structure_clusters_in_sphere (0000)
002A1E54 002e:
	??_C@_0CO@IAFCMJGA@?$CBstructure_globals?4cluster_marke@ (0000)
002A1E84 0027:
	??_C@_0CH@MEPFLBCJ@c?3?2halo?2SOURCE?2structures?2struct@ (0000)
002A1EAC 002d:
	??_C@_0CN@MDKOILJF@structure_globals?4cluster_marker@ (0000)
002A1EDC 002b:
	??_C@_0CL@OOHBKJHI@stack_depth?$DMMAXIMUM_CLUSTERS_PER@ (0000)
002A1F08 0002:
	??_C@_01LKDEMHDF@s?$AA@ (0000)
002A1F0C 000e:
	??_C@_0O@GNJLHEFA@surface_index?$AA@ (0000)
002A1F1C 000f:
	??_C@_0P@MLEINILI@material_index?$AA@ (0000)
002A1F2C 0014:
	??_C@_0BE@GKJHHJJB@intersected_indices?$AA@ (0000)
004C1100 0808:
	_bss_004c1100 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "math/geometry.h"
#include "physics/collision_bsp_definitions.h"
#include "physics/collisions.h"
#include "physics/collision_usage.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_debug.h"
#include "rasterizer/rasterizer_geometry_environment.h"
#include "render/render.h"
#include "scenario/scenario.h"
#include "scenario/fog_definitions.h"
#include "scenario/sky_definitions.h"
#include "structure_bsp_definitions.h"
#include "structure_vector_tests.h"
#include "structures.h"

/* ---------- constants */

enum
{
	_render_planar_fog_mode_off = 0,
	_render_planar_fog_mode_normal = 1,
	_render_planar_fog_mode_fully_fogged,
	_render_fog_runtime_screen_use_sky_interpolator_bit = 0,
	_collision_surface_two_sided_bit = 0,
};

/* ---------- macros */

#define structure_material_get_vertex(material, vertex_index) \
	((struct environment_vertex_compressed const *) \
		(material)->compressed_vertex_data.address + (vertex_index))

/* ---------- structures */

struct structure_runtime_globals
{
	boolean initialized;
	boolean cluster_marker_initialized;
	byte reserved0002[2];
	long cluster_marker;
	long cluster_magic_numbers[MAXIMUM_CLUSTERS_PER_STRUCTURE];
};

struct structure_fog_plane_render
{
	short region_index;
	word pad;
	real_plane3d plane;
	struct tag_block vertices;
};

struct structure_cluster_graph
{
	short sky_index;
	short fog_designator;
	short background_sound_palette_index;
	short sound_environment_palette_index;
	short weather_palette_index;
	short transition_structure_bsp_index;
	short first_runtime_decal_index;
	word runtime_decal_count;
	long unused[6];
	struct tag_block predicted_resources;
	struct tag_block subclusters;
	word first_lens_flare_marker_index;
	word lens_flare_marker_count;
	struct tag_block surface_indices;
	struct tag_block mirrors;
	struct tag_block portal_indices;
};

struct structure_cluster_portal
{
	short cluster_indices[2];
	long plane_index;
	real_point3d centroid;
	real bounding_radius;
	unsigned long flags;
	long unused[6];
	struct tag_block vertices;
};

struct structure_surface_reference
{
	long surface_index;
	long bsp3d_node_index;
};

struct environment_vertex_compressed
{
	real_point3d position;
	unsigned long normal;
	unsigned long binormal;
	unsigned long tangent;
	real_point2d texcoord;
};

struct fog_screen
{
	word flags;
	short layer_count;
	real near_distance;
	real far_distance;
	real near_density;
	real far_density;
	real start_distance_from_fog_plane;
	byte reserved18[4];
	pixel32 color;
	real rotation_multiplier;
	real strafing_multiplier;
	real zoom_multiplier;
	byte reserved2C[8];
	real map_scale;
	struct tag_reference map;
};

struct structure_planar_fog_definition
{
	word flags;
	word pad;
	real animation_distance;
	byte unused08[0x50];
	real maximum_density;
	byte unused5C[4];
	real maximum_distance;
	byte unused64[4];
	real maximum_depth;
	byte unused6C[0xC];
	real_rgb_color color;
	struct fog_screen screen;
};

typedef char verify_structure_fog_plane_render_size[
	sizeof(struct structure_fog_plane_render) == 0x20 ? 1 : -1];
typedef char verify_structure_cluster_graph_size[
	sizeof(struct structure_cluster_graph) == 0x68 ? 1 : -1];
typedef char verify_structure_cluster_graph_portal_indices_offset[
	offsetof(struct structure_cluster_graph, portal_indices) == 0x5C ? 1 : -1];
typedef char verify_structure_cluster_portal_size[
	sizeof(struct structure_cluster_portal) == 0x40 ? 1 : -1];
typedef char verify_structure_surface_reference_size[
	sizeof(struct structure_surface_reference) == 0x8 ? 1 : -1];
typedef char verify_environment_vertex_compressed_size[
	sizeof(struct environment_vertex_compressed) == 0x20 ? 1 : -1];
typedef char verify_structure_planar_fog_definition_screen_offset[
	offsetof(struct structure_planar_fog_definition, screen) == 0x84 ? 1 : -1];

typedef char verify_structure_cluster_marker_initialized_offset[
	offsetof(
		struct structure_runtime_globals,
		cluster_marker_initialized) == 0x1 ? 1 : -1];
typedef char verify_structure_cluster_marker_offset[
	offsetof(
		struct structure_runtime_globals,
		cluster_marker) == 0x4 ? 1 : -1];
typedef char verify_structure_cluster_magic_numbers_offset[
	offsetof(
		struct structure_runtime_globals,
		cluster_magic_numbers) == 0x8 ? 1 : -1];
typedef char verify_structure_runtime_globals_size[
	sizeof(struct structure_runtime_globals) == 0x808 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

static struct structure_runtime_globals structure_globals;

extern boolean debug_fog_planes;

/* ---------- public code */

void structures_initialize(
	void)
{
	structure_detail_objects_initialize();
	structure_decals_initialize();
	return;
}

void structures_initialize_for_new_map(
	void)
{
	structure_detail_objects_initialize_for_new_map();
	structure_decals_initialize_for_new_map();
	return;
}

void structures_dispose_from_old_map(
	void)
{
	structure_decals_dispose_from_old_map();
	structure_detail_objects_dispose_from_old_map();
	return;
}

void structures_dispose(
	void)
{
	structure_decals_dispose();
	structure_detail_objects_dispose();
	return;
}

void structure_cluster_marker_begin(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x103,
		!structure_globals.cluster_marker_initialized);
	structure_globals.cluster_marker++;
	structure_globals.cluster_marker_initialized = TRUE;

	return;
}

void structure_cluster_marker_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x130,
		structure_globals.cluster_marker_initialized);
	structure_globals.cluster_marker_initialized = FALSE;

	return;
}

boolean structure_cluster_unmarked(
	short cluster_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x10e,
		structure_globals.cluster_marker_initialized);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x10f,
		cluster_index>=0 && cluster_index<MAXIMUM_CLUSTERS_PER_STRUCTURE);

	return (boolean)(structure_globals.cluster_magic_numbers[cluster_index]!=structure_globals.cluster_marker);
}

boolean structure_cluster_mark(
	short cluster_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x11e,
		structure_globals.cluster_marker_initialized);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x11f,
		cluster_index>=0 && cluster_index<MAXIMUM_CLUSTERS_PER_STRUCTURE);

	if (structure_globals.cluster_magic_numbers[cluster_index]!=structure_globals.cluster_marker)
	{
		structure_globals.cluster_magic_numbers[cluster_index]= structure_globals.cluster_marker;
		return TRUE;
	}

	return FALSE;
}

boolean sphere_intersects_cluster_portal(
	struct structure_bsp *structure,
	short portal_index,
	real_point3d const *position,
	real radius)
{
	struct structure_cluster_portal *portal = TAG_BLOCK_GET_ELEMENT(
		&structure->cluster_portals,
		portal_index,
		struct structure_cluster_portal);
	real_plane3d *collision_plane = TAG_BLOCK_GET_ELEMENT(
		&TAG_BLOCK_GET_ELEMENT(
			&structure->collision_bsp,
			0,
			struct collision_bsp)->bsp3d.planes,
		portal->plane_index,
		real_plane3d);
	real plane_distance = plane3d_distance_to_point(collision_plane, position);
	real_point2d projected_vertices[MAXIMUM_VERTICES_PER_CLUSTER_PORTAL];
	real_point3d projected_position;
	real_point2d projected_position2d;
	real_vector3d const *plane_normal;
	short projection;
	boolean projection_sign;
	short vertex_index;

	if (fabs(plane_distance) < radius &&
		distance_squared3d(position, &portal->centroid) <
			(portal->bounding_radius + radius) * (portal->bounding_radius + radius))
	{
		plane_normal = &TAG_BLOCK_GET_ELEMENT(
			&global_bsp3d_get()->planes,
			portal->plane_index,
			real_plane3d)->n;
		projection = projection_from_vector3d(plane_normal);
		projection_sign = projection_sign_from_vector3d(plane_normal, projection);
		point_from_line3d(
			position,
			plane_normal,
			-plane_distance,
			&projected_position);
		project_point3d(
			&projected_position,
			projection,
			projection_sign,
			&projected_position2d);

		for (vertex_index = 0;
			vertex_index < portal->vertices.count;
			vertex_index++)
		{
			project_point3d(
				TAG_BLOCK_GET_ELEMENT(
					&portal->vertices,
					vertex_index,
					real_point3d),
				projection,
				projection_sign,
				&projected_vertices[vertex_index]);
		}

		if (convex_hull2d_test_circle(
			(short)portal->vertices.count,
			projected_vertices,
			&projected_position2d,
			square_root(radius * radius - plane_distance * plane_distance)))
		{
			return TRUE;
		}
	}

	return FALSE;
}

static short structure_clusters_in_sphere_recursive(
	short cluster_index,
	real_point3d const *position,
	real radius,
	short maximum_count,
	short *intersected_indices)
{
	struct structure_bsp *structure = global_structure_bsp_get();
	struct structure_cluster_graph *cluster = TAG_BLOCK_GET_ELEMENT(
		&structure->clusters,
		cluster_index,
		struct structure_cluster_graph);
	short cluster_count;
	short portal_list_index;

	if (maximum_count-- > 0)
	{
		*intersected_indices++ = cluster_index;
	}

	structure_cluster_mark(cluster_index);
	cluster_count = 1;

	for (portal_list_index = 0;
		portal_list_index < cluster->portal_indices.count;
		portal_list_index++)
	{
		short portal_index = *TAG_BLOCK_GET_ELEMENT(
			&cluster->portal_indices,
			portal_list_index,
			short);
		struct structure_cluster_portal *portal = TAG_BLOCK_GET_ELEMENT(
			&structure->cluster_portals,
			portal_index,
			struct structure_cluster_portal);
		short adjacent_cluster_index =
			portal->cluster_indices[0] == cluster_index
				? portal->cluster_indices[1]
				: portal->cluster_indices[0];

		if (structure_cluster_unmarked(adjacent_cluster_index) &&
			sphere_intersects_cluster_portal(
				structure,
				portal_index,
				position,
				radius))
		{
			short added_count = structure_clusters_in_sphere_recursive(
				adjacent_cluster_index,
				position,
				radius,
				maximum_count,
				intersected_indices);

			cluster_count += added_count;
			maximum_count -= added_count;
			/* BUG (original): January and HCEA advance by every visited cluster,
			 * even after the output capacity is exhausted, which can form an
			 * out-of-bounds pointer. A corrected build should advance only by
			 * the number actually written, clamped to the remaining capacity. */
			intersected_indices += added_count;
		}
	}

	return cluster_count;
}

boolean structure_render_surface_from_point_and_leaf(
	real_point3d const *point,
	long leaf_index,
	long plane_index,
	short *lightmap_index,
	short *material_index,
	long *surface_index,
	real *s,
	real *t)
{
#if defined(HALO_N64) || defined(HALO_N64_CAMPAIGN)
	/* Native MP and campaign leaves keep their cluster only; their render
	surface references are not part of the cartridge format. */
	return FALSE;
#else
	struct structure_bsp *structure = global_structure_bsp_get();
	struct structure_leaf *leaf = TAG_BLOCK_GET_ELEMENT(
		&structure->leaves,
		leaf_index & LONG_MAX,
		struct structure_leaf);
	long surface_reference_index;
	boolean found = FALSE;

	for (surface_reference_index = leaf->first_surface_reference_index;
		surface_reference_index <
			leaf->first_surface_reference_index + leaf->surface_reference_count;
		surface_reference_index++)
	{
		struct structure_surface_reference *surface_reference = TAG_BLOCK_GET_ELEMENT(
			&structure->surface_references,
			surface_reference_index,
			struct structure_surface_reference);

		if (surface_reference->bsp3d_node_index != NONE)
		{
			struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(
				&TAG_BLOCK_GET_ELEMENT(
					&structure->collision_bsp,
					0,
					struct collision_bsp)->bsp3d.nodes,
				surface_reference->bsp3d_node_index,
				struct bsp3d_node);

			if (node->plane_designator == plane_index)
			{
				struct structure_surface *surface = TAG_BLOCK_GET_ELEMENT(
					&structure->surfaces,
					surface_reference->surface_index,
					struct structure_surface);
				struct structure_lightmap *lightmap;
				struct structure_material *material;

				structure_bsp_find_material_for_surface(
					structure,
					surface_reference->surface_index,
					lightmap_index,
					material_index);
				lightmap = TAG_BLOCK_GET_ELEMENT(
					&structure->lightmaps,
					*lightmap_index,
					struct structure_lightmap);
				material = TAG_BLOCK_GET_ELEMENT(
					&lightmap->materials,
					*material_index,
					struct structure_material);

				if (material->vertices.type ==
						_rasterizer_vertex_type_environment_uncompressed ||
					material->vertices.type ==
						_rasterizer_vertex_type_environment_compressed)
				{
					real_point3d triangle_points[NUMBER_OF_VERTICES_PER_TRIANGLE];

					environment_vertex_compressed_get_point(
						structure_material_get_vertex(
							material,
							surface->vertex_indices[0]),
						&triangle_points[0]);
					environment_vertex_compressed_get_point(
						structure_material_get_vertex(
							material,
							surface->vertex_indices[1]),
						&triangle_points[1]);
					environment_vertex_compressed_get_point(
						structure_material_get_vertex(
							material,
							surface->vertex_indices[2]),
						&triangle_points[2]);

					if (point_in_triangle3d(
						point,
						&triangle_points[0],
						&triangle_points[1],
						&triangle_points[2],
						s,
						t))
					{
						*surface_index = surface_reference->surface_index;
						found = TRUE;
						break;
					}
				}
			}
		}
	}

	return found;
#endif
}

void render_debug_fog_planes(
	void)
{
	if (debug_fog_planes &&
		render.fog.planar_mode == _render_planar_fog_mode_normal &&
		render.cluster_index != NONE)
	{
		struct structure_bsp *structure = global_structure_bsp_get();
		struct structure_cluster *cluster = TAG_BLOCK_GET_ELEMENT(
			&structure->clusters,
			render.cluster_index,
			struct structure_cluster);
		struct structure_fog_plane_render *fog_plane = TAG_BLOCK_GET_ELEMENT(
			&structure->fog_planes,
			cluster->fog_reference & SHORT_MAX,
			struct structure_fog_plane_render);
		short vertex_index;

		for (vertex_index = 0;
			vertex_index < fog_plane->vertices.count;
			vertex_index++)
		{
			short next_vertex_index = (short)((vertex_index + 1) % fog_plane->vertices.count);
			real_point3d *point0 = TAG_BLOCK_GET_ELEMENT(
				&fog_plane->vertices,
				vertex_index,
				real_point3d);
			real_point3d *point1 = TAG_BLOCK_GET_ELEMENT(
				&fog_plane->vertices,
				next_vertex_index,
				real_point3d);
			real_point3d offset_point0;
			real_point3d offset_point1;

			point_from_line3d(point0, &fog_plane->plane.n, -render.fog.planar_maximum_distance, &offset_point0);
			point_from_line3d(point1, &fog_plane->plane.n, -render.fog.planar_maximum_distance, &offset_point1);

			rasterizer_debug_line(point0, point1, global_real_argb_white);
			rasterizer_debug_line(
				&offset_point0,
				&offset_point1,
				global_real_argb_black);
			rasterizer_debug_line_shaded(
				point0,
				&offset_point0,
				global_real_argb_white,
				global_real_argb_black);
			rasterizer_debug_line_shaded(
				point1,
				&offset_point1,
				global_real_argb_white,
				global_real_argb_black);
		}
	}

	return;
}

short structure_clusters_in_sphere(
	short cluster_index,
	real_point3d const *position,
	real radius,
	short maximum_count,
	short *intersected_indices)
{
	short cluster_count = 0;

	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x86,
		position);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x87,
		radius>=0.f);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x88,
		maximum_count>0);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x89,
		intersected_indices);

	if (cluster_index != NONE)
	{
		if (radius > 0.0f)
		{
			structure_cluster_marker_begin();
			cluster_count = structure_clusters_in_sphere_recursive(
				cluster_index,
				position,
				radius,
				maximum_count,
				intersected_indices);
			structure_cluster_marker_end();
		}
		else if (maximum_count > 0)
		{
			intersected_indices[0] = cluster_index;
			cluster_count = 1;
		}
	}

	return cluster_count;
}

short structure_clusters_in_cone(
	short position_cluster_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real distance,
	real sine,
	real cosine,
	short maximum_cluster_count,
	short *cluster_indices)
{
	short cluster_stack[MAXIMUM_CLUSTERS_PER_STRUCTURE];
	short stack_depth;
	short cluster_count = 0;
	struct structure_bsp *structure;

	structure_cluster_marker_begin();
	structure = global_structure_bsp_get();
	structure_cluster_mark(position_cluster_index);
	cluster_stack[0] = position_cluster_index;
	stack_depth = 1;

	while (stack_depth > 0 && cluster_count < maximum_cluster_count)
	{
		short cluster_index = cluster_stack[--stack_depth];
		struct structure_cluster_graph *cluster = TAG_BLOCK_GET_ELEMENT(
			&structure->clusters,
			cluster_index,
			struct structure_cluster_graph);
		short portal_index;

		cluster_indices[cluster_count++] = cluster_index;

		for (portal_index = 0;
			portal_index < cluster->portal_indices.count;
			portal_index++)
		{
			short structure_portal_index = *TAG_BLOCK_GET_ELEMENT(
				&cluster->portal_indices,
				portal_index,
				short);
			struct structure_cluster_portal *portal = TAG_BLOCK_GET_ELEMENT(
				&structure->cluster_portals,
				structure_portal_index,
				struct structure_cluster_portal);
			short adjacent_cluster_index =
				portal->cluster_indices[0] == cluster_index
					? portal->cluster_indices[1]
					: portal->cluster_indices[0];

			if (structure_cluster_unmarked(adjacent_cluster_index) &&
				sphere_intersects_cone3d(
					&portal->centroid,
					portal->bounding_radius,
					position,
					direction,
					distance,
					sine,
					cosine))
			{
				structure_cluster_mark(adjacent_cluster_index);
				match_assert(
					"c:\\halo\\SOURCE\\structures\\structures.c",
					0xF5,
					stack_depth<MAXIMUM_CLUSTERS_PER_STRUCTURE);
				cluster_stack[stack_depth++] = adjacent_cluster_index;
			}
		}
	}

	structure_cluster_marker_end();

	return cluster_count;
}

boolean structure_test_vector(
	real_point3d const *p,
	real_vector3d const *v,
	real_point3d *collision_point,
	short *lightmap_index,
	short *material_index,
	long *surface_index,
	real *s,
	real *t)
{
	boolean found = FALSE;
	boolean done;
	struct collision_result collision;

#ifdef HALO_N64
	/* finds the lightmap texel under a point (object and particle lighting);
	the N64 build carries no lightmaps (tools/n64/convert.py) */
	return FALSE;
#endif

	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x188,
		p);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x189,
		v);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x18A,
		material_index);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x18B,
		surface_index);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x18C,
		s);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x18D,
		t);

	*collision_point = *p;
	do
	{
		done = TRUE;
		match_assert(
			"c:\\halo\\SOURCE\\structures\\structures.c",
			0x196,
			global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
		global_current_collision_users[global_current_collision_user_depth++] =
			_collision_user_structure_lighting;

		if (collision_test_vector(
			FLAG(_collision_test_front_facing_surfaces_bit) |
				FLAG(_collision_test_structure_bit),
			collision_point,
			v,
			NONE,
			&collision))
		{
			struct structure_bsp *structure = global_structure_bsp_get();

			*collision_point = collision.point;
			if (structure_render_surface_from_point_and_leaf(
				collision_point,
				collision.location.leaf_index,
				collision.plane_designator & LONG_MAX,
				lightmap_index,
				material_index,
				surface_index,
				s,
				t) &&
				TAG_BLOCK_GET_ELEMENT(
					&structure->lightmaps,
					*lightmap_index,
					struct structure_lightmap)->bitmap_index != NONE)
			{
				found = TRUE;
			}
			else if (TEST_FLAG(
				collision.flags,
				_collision_surface_two_sided_bit))
			{
				done = FALSE;
				collision_point->x += v->i * (1.0f / 4096.0f);
				collision_point->y += v->j * (1.0f / 4096.0f);
				collision_point->z += v->k * (1.0f / 4096.0f);
			}
		}

		match_assert(
			"c:\\halo\\SOURCE\\structures\\structures.c",
			0x1AA,
			global_current_collision_user_depth > 1);
		--global_current_collision_user_depth;
	}
	while (!done);

	return found;
}

long structure_get_planar_fog_definition_index(
	struct structure_bsp const *structure,
	short cluster_index,
	boolean get_fog_from_sky)
{
	long fog_definition_index = NONE;

	if (cluster_index != NONE)
	{
		struct structure_cluster *cluster = TAG_BLOCK_GET_ELEMENT(
			&structure->clusters,
			cluster_index,
			struct structure_cluster);

		if (get_fog_from_sky)
		{
			struct sky *sky = scenario_get_sky(0);

			if (sky)
			{
				fog_definition_index = sky->indoor_fog_screen.index;
			}
		}
		else
		{
			short fog_designator = cluster->fog_reference;

			if (fog_designator != NONE)
			{
				short fog_region_index;

				if (TEST_FLAG((word)fog_designator, SHORT_BITS - 1))
				{
					struct structure_fog_plane_render *fog_plane = TAG_BLOCK_GET_ELEMENT(
						&structure->fog_planes,
						fog_designator & SHORT_MAX,
						struct structure_fog_plane_render);

					fog_region_index = fog_plane->region_index;
				}
				else
				{
					fog_region_index = fog_designator & SHORT_MAX;
				}

				if (fog_region_index != NONE)
				{
					struct structure_fog_region *fog_region = TAG_BLOCK_GET_ELEMENT(
						&structure->fog_regions,
						fog_region_index,
						struct structure_fog_region);

					if (fog_region->fog_palette_index != NONE)
					{
						struct structure_fog_palette_entry *fog_palette =
							TAG_BLOCK_GET_ELEMENT(
								&structure->fog_palette,
								fog_region->fog_palette_index,
								struct structure_fog_palette_entry);

						fog_definition_index = fog_palette->fog.index;
					}
				}
			}
		}
	}

	return fog_definition_index;
}

void structure_get_planar_fog(
	short cluster_index,
	struct render_fog *fog)
{
	struct structure_bsp *structure = global_structure_bsp_get();
	boolean screen_fog = FALSE;
	long fog_definition_index;

	match_assert(
		"c:\\halo\\SOURCE\\structures\\structures.c",
		0x1F1,
		fog);

	fog->planar_mode = _render_planar_fog_mode_off;
	fog->fog_definition_flags = 0;
	fog->screen = NULL;

	fog_definition_index = structure_get_planar_fog_definition_index(
		structure,
		cluster_index,
		FALSE);
	if (fog_definition_index == NONE)
	{
		fog_definition_index = structure_get_planar_fog_definition_index(
			structure,
			cluster_index,
			TRUE);
		screen_fog = TRUE;
	}

	if (fog_definition_index != NONE)
	{
		struct structure_cluster *cluster;
		struct structure_planar_fog_definition *definition;

		structure = global_structure_bsp_get();
		cluster = TAG_BLOCK_GET_ELEMENT(
			&structure->clusters,
			cluster_index,
			struct structure_cluster);
		definition = (struct structure_planar_fog_definition *)fog_definition_get(
			fog_definition_index);

		if (screen_fog)
		{
			SET_FLAG(
				fog->runtime_flags,
				_render_fog_runtime_screen_use_sky_interpolator_bit,
				TRUE);
			fog->fog_definition_flags = definition->flags;
			fog->screen = &definition->screen;
		}
		else
		{
			if (TEST_FLAG((word)cluster->fog_reference, SHORT_BITS - 1))
			{
				fog->planar_mode = _render_planar_fog_mode_normal;
				fog->plane = TAG_BLOCK_GET_ELEMENT(
					&structure->fog_planes,
					cluster->fog_reference & SHORT_MAX,
					struct structure_fog_plane_render)->plane;
			}
			else
			{
				fog->planar_mode = _render_planar_fog_mode_fully_fogged;
			}

			fog->planar_color = definition->color;
			fog->planar_maximum_density = definition->maximum_density;
			fog->planar_maximum_depth = definition->maximum_depth;
			fog->planar_maximum_distance = definition->maximum_distance;

			if (TEST_FLAG((word)cluster->fog_reference, SHORT_BITS - 1))
			{
				real offset;
				real_vector3d vector_offset;

				TAG_BLOCK_GET_ELEMENT(
					&structure->fog_planes,
					cluster->fog_reference & SHORT_MAX,
					struct structure_fog_plane_render);
				/* BUG: January and two independent later reconstructions scale
				 * the authored animation distance by a literal zero, disabling
				 * planar-fog motion. */
				offset = definition->animation_distance * 0.0f;

				fog->plane.d += offset;
				scale_vector3d(&fog->plane.n, offset, &vector_offset);
				structure_render_set_fog_offset(&vector_offset);
			}

			fog->fog_definition_flags = definition->flags;
			fog->screen = &definition->screen;
		}
	}

	return;
}

/* ---------- private code */
