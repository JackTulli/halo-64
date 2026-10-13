/*
STRUCTURE_RENDER.C

symbols in this file:
00184D30 0050:
	_structure_render_set_fog_offset (0000)
00184D80 0020:
	_compare_surface_indices (0000)
00184DA0 0100:
	_structure_render_dynamic_triangles_from_bitvector (0000)
00184EA0 0080:
	_structure_render_dynamic_triangles_from_indices (0000)
00184F20 00c0:
	_structure_render_build_dynamic_triangles (0000)
00184FE0 0260:
	_structure_render_pass (0000)
00185240 0120:
	_structure_render_preprocess (0000)
00185360 00b0:
	_structure_render_lightmaps (0000)
00185410 0080:
	_structure_render_diffuse_texture (0000)
00185490 0070:
	_structure_render_specular_lightmaps (0000)
00185500 0050:
	_structure_render_reflection_lightmap_masks (0000)
00185550 0040:
	_structure_render_reflection_mirrors (0000)
00185590 0080:
	_structure_render_reflections (0000)
00185610 0080:
	_structure_render_transparent_geometry (0000)
00185690 0080:
	_structure_render_fog (0000)
00185710 0070:
	_structure_render_fog_screen (0000)
00185780 0130:
	_structure_render_specular_light (0000)
001858B0 0130:
	_structure_render_diffuse_light (0000)
001859E0 0100:
	_structure_render_shadow (0000)
002A17E8 001e:
	??_C@_0BO@IFCJOOII@render_structure_shadows_draw?$AA@ (0000)
002A1808 0019:
	??_C@_0BJ@LHNNMHCN@render_structure_shadows?$AA@ (0000)
002A1824 0025:
	??_C@_0CF@BNBKIEIJ@render_structure_diffuse_lights_@ (0000)
002A184C 0020:
	??_C@_0CA@HHIKFCNA@render_structure_diffuse_lights?$AA@ (0000)
002A186C 0026:
	??_C@_0CG@DKGBAEP@render_structure_specular_lights@ (0000)
002A1894 0021:
	??_C@_0CB@HDMNCMAJ@render_structure_specular_lights@ (0000)
002A18B8 0015:
	??_C@_0BF@FGONPFDO@render_structure_fog?$AA@ (0000)
002A18D0 0026:
	??_C@_0CG@INKKMOOJ@render_structure_transparent_geo@ (0000)
002A18F8 001d:
	??_C@_0BN@HGHIIJ@render_structure_reflections?$AA@ (0000)
002A1918 0022:
	??_C@_0CC@GOLKDEIH@render_structure_diffuse_texture@ (0000)
002A193C 001b:
	??_C@_0BL@MIJLHPO@render_structure_lightmaps?$AA@ (0000)
002A1958 0027:
	??_C@_0CH@CNKEDOPG@render_structure_build_triangle_@ (0000)
002A1980 002b:
	??_C@_0CL@FLJBJLNG@?$CBstructure_render_globals?4fog_of@ (0000)
002A19AC 002d:
	??_C@_0CN@IJHJEMLI@c?3?2halo?2SOURCE?2structures?2struct@ (0000)
002A19DC 003c:
	??_C@_0DM@OPJNGOOB@surface_index_index?$DO?$DN0?5?$CG?$CG?5surfac@ (0000)
002A1A18 0030:
	??_C@_0DA@GELBPONP@unable?5to?5allocate?5dynamic?5struc@ (0000)
002A1A48 0044:
	??_C@_0EE@DHHGDPGL@there?5are?5more?5surfaces?5than?5mat@ (0000)
0030F340 47aa:
	_debug_leaf_index (0000)
	_debug_leaf_portal_index (0004)
004C0CD0 0018:
	_structure_render_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "structure_render.h"

#include "bitmaps/bitmap_group_runtime.h"
#include "cseries/errors.h"
#include "cseries/profile.h"
#include "cseries/sort.h"
#include "physics/breakable_surfaces.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_environment.h"
#include "render/render.h"
#include "scenario/scenario.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"
#include "structures/leaf_map.h"
#include "structures/structure_bsp_definitions.h"
#include "structures/structure_visibility.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MAXIMUM_LOCALLY_RENDERED_SURFACES = 4096,
};

enum
{
	_structure_material_coplanar_bit = 0,
	_structure_material_fog_plane_bit,
};

/* ---------- macros */

/* ---------- structures */

struct structure_render_globals
{
	boolean environment_geometry_valid;
	long environment_triangles_index;
	boolean fog_offset_valid;
	real_vector3d fog_offset_vector;
};

typedef char verify_structure_render_environment_geometry_valid_offset[
	offsetof(
		struct structure_render_globals,
		environment_geometry_valid) == 0x0 ? 1 : -1];
typedef char verify_structure_render_environment_triangles_index_offset[
	offsetof(
		struct structure_render_globals,
		environment_triangles_index) == 0x4 ? 1 : -1];
typedef char verify_structure_render_fog_offset_valid_offset[
	offsetof(
		struct structure_render_globals,
		fog_offset_valid) == 0x8 ? 1 : -1];
typedef char verify_structure_render_fog_offset_vector_offset[
	offsetof(
		struct structure_render_globals,
		fog_offset_vector) == 0xC ? 1 : -1];
typedef char verify_structure_render_globals_size[
	sizeof(struct structure_render_globals) == 0x18 ? 1 : -1];

/* ---------- prototypes */

static boolean compare_surface_indices(
	long surface_index1,
	long surface_index2);
static void structure_render_dynamic_triangles_from_bitvector(
	short surface_count,
	long *surface_indices,
	unsigned long *surface_flags,
	short *triangles);
static void structure_render_dynamic_triangles_from_indices(
	short surface_count,
	long *surface_indices,
	short *triangles);
static long structure_render_build_dynamic_triangles(
	long *surface_indices,
	short surface_count,
	unsigned long *surface_flags);
static void structure_render_pass(
	long *surface_indices,
	short surface_count,
	long dynamic_triangles_index,
	void (*begin_lightmap)(struct bitmap_data const *lightmap_bitmap),
	void (*draw_triangles)(
		struct shader const *shader,
		short shader_permutation_index,
		long dynamic_triangle_buffer_index,
		long first_triangle_index,
		long triangle_count,
		struct vertex_buffer const *vertex_buffer),
	void (*end_lightmap)(void),
	void (*draw_transparent_triangles)(
		struct shader const *shader,
		short shader_permutation_index,
		struct bitmap_data const *lightmap,
		long dynamic_triangle_buffer_index,
		long first_triangle_index,
		long triangle_count,
		struct vertex_buffer const *vertex_buffers,
		real_point3d const *centroid,
		real_plane3d const *plane,
		real_vector3d const *offset,
		void const *lighting,
		unsigned long geometry_flags));

/* ---------- globals */

static struct structure_render_globals structure_render_globals;

long debug_leaf_index = NONE;
long debug_leaf_portal_index = NONE;

static struct profile_section render_structure_build_triangle_arrays_section =
	{ "render_structure_build_triangle_arrays", NONE, TRUE };
static struct profile_section render_structure_lightmaps_section =
	{ "render_structure_lightmaps", NONE, TRUE };
static struct profile_section render_structure_diffuse_textures_section =
	{ "render_structure_diffuse_textures", NONE, TRUE };
static struct profile_section render_structure_reflections_section =
	{ "render_structure_reflections", NONE, TRUE };
static struct profile_section render_structure_transparent_geometry_section =
	{ "render_structure_transparent_geometry", NONE, TRUE };
static struct profile_section render_structure_fog_section =
	{ "render_structure_fog", NONE, TRUE };
static struct profile_section render_structure_specular_lights_section =
	{ "render_structure_specular_lights", NONE, TRUE };
static struct profile_section render_structure_specular_lights_draw_section =
	{ "render_structure_specular_lights_draw", NONE, TRUE };
static struct profile_section render_structure_diffuse_lights_section =
	{ "render_structure_diffuse_lights", NONE, TRUE };
static struct profile_section render_structure_diffuse_lights_draw_section =
	{ "render_structure_diffuse_lights_draw", NONE, TRUE };
static struct profile_section render_structure_shadows_section =
	{ "render_structure_shadows", NONE, TRUE };
static struct profile_section render_structure_shadows_draw_section =
	{ "render_structure_shadows_draw", NONE, TRUE };

static short report_dynamic_structure_triangle_failure = TRUE;

/* ---------- public code */

void structure_render_set_fog_offset(
	real_vector3d *vector_offset)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structure_render.c",
		103,
		!structure_render_globals.fog_offset_valid);

	structure_render_globals.fog_offset_valid = TRUE;
	structure_render_globals.fog_offset_vector.i = vector_offset->i;
	structure_render_globals.fog_offset_vector.j = vector_offset->j;
	structure_render_globals.fog_offset_vector.k = vector_offset->k;

	return;
}

/* ---------- private code */

static boolean compare_surface_indices(
	long surface_index1,
	long surface_index2)
{
	if (surface_index1 < surface_index2)
	{
		return FALSE;
	}

	if (surface_index2 < surface_index1)
	{
		return TRUE;
	}

	return FALSE;
}

static void structure_render_dynamic_triangles_from_bitvector(
	short surface_count,
	long *surface_indices,
	unsigned long *surface_flags,
	short *triangles)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	short surface_index_index = 0;
	long surface_index = 0;

	if (structure_bsp->surfaces.count > 0)
	{
		do
		{
			if (*surface_flags)
			{
				short bit_index;

				for (bit_index = 0;
					bit_index < LONG_BITS;
					bit_index++, surface_index++)
				{
					if (surface_index >= structure_bsp->surfaces.count)
					{
						break;
					}

					if (TEST_FLAG(*surface_flags, bit_index))
					{
						struct structure_surface *surface = TAG_BLOCK_GET_ELEMENT(
							&structure_bsp->surfaces,
							surface_index,
							struct structure_surface);

						match_assert(
							"c:\\halo\\SOURCE\\structures\\structure_render.c",
							421,
							surface_index_index>=0 && surface_index_index<surface_count);

						*surface_indices++ = surface_index;
						triangles[surface_index_index * NUMBER_OF_VERTICES_PER_TRIANGLE + 0] =
							surface->vertex_indices[0];
						triangles[surface_index_index * NUMBER_OF_VERTICES_PER_TRIANGLE + 1] =
							surface->vertex_indices[1];
						triangles[surface_index_index * NUMBER_OF_VERTICES_PER_TRIANGLE + 2] =
							surface->vertex_indices[2];
						surface_index_index++;
					}
				}
			}
			else
			{
				surface_index += LONG_BITS;
			}

			surface_flags++;
		}
		while (surface_index < structure_bsp->surfaces.count);
	}

	return;
}

static void structure_render_dynamic_triangles_from_indices(
	short surface_count,
	long *surface_indices,
	short *triangles)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	short surface_index_index;

	qsort_4byte(surface_indices, surface_count, compare_surface_indices);

	for (surface_index_index = 0;
		surface_index_index < surface_count;
		surface_index_index++)
	{
		struct structure_surface *surface = TAG_BLOCK_GET_ELEMENT(
			&structure_bsp->surfaces,
			*surface_indices,
			struct structure_surface);

		triangles[0] = surface->vertex_indices[0];
		triangles[1] = surface->vertex_indices[1];
		triangles[2] = surface->vertex_indices[2];

		surface_indices++;
		triangles += NUMBER_OF_VERTICES_PER_TRIANGLE;
	}

	return;
}

static long structure_render_build_dynamic_triangles(
	long *surface_indices,
	short surface_count,
	unsigned long *surface_flags)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	long dynamic_triangles_index = NONE;

	if (surface_count > 0)
	{
		dynamic_triangles_index = rasterizer_dynamic_triangles_new(surface_count);

		if (dynamic_triangles_index != NONE)
		{
			short *triangles = rasterizer_dynamic_triangles_lock(dynamic_triangles_index);

			match_assert(
				"c:\\halo\\SOURCE\\structures\\structure_render.c",
				486,
				triangles);

			if (surface_flags)
			{
				structure_render_dynamic_triangles_from_bitvector(
					surface_count,
					surface_indices,
					surface_flags,
					triangles);
			}
			else
			{
				structure_render_dynamic_triangles_from_indices(
					surface_count,
					surface_indices,
					triangles);
			}

			rasterizer_dynamic_triangles_unlock(dynamic_triangles_index);
		}
		else if (report_dynamic_structure_triangle_failure)
		{
			error(_error_silent, "unable to allocate dynamic structure triangles.");
			report_dynamic_structure_triangle_failure = FALSE;
		}
	}

	return dynamic_triangles_index;
}

static void structure_render_pass(
	long *surface_indices,
	short surface_count,
	long dynamic_triangles_index,
	void (*begin_lightmap)(struct bitmap_data const *lightmap_bitmap),
	void (*draw_triangles)(
		struct shader const *shader,
		short shader_permutation_index,
		long dynamic_triangle_buffer_index,
		long first_triangle_index,
		long triangle_count,
		struct vertex_buffer const *vertex_buffer),
	void (*end_lightmap)(void),
	void (*draw_transparent_triangles)(
		struct shader const *shader,
		short shader_permutation_index,
		struct bitmap_data const *lightmap,
		long dynamic_triangle_buffer_index,
		long first_triangle_index,
		long triangle_count,
		struct vertex_buffer const *vertex_buffers,
		real_point3d const *centroid,
		real_plane3d const *plane,
		real_vector3d const *offset,
		void const *lighting,
		unsigned long geometry_flags))
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	long *surface_index_cursor = surface_indices;
	long *surface_index_end = surface_indices + surface_count;
	long first_triangle_index = 0;
	short lightmap_index;

	for (lightmap_index = 0;
		lightmap_index < structure_bsp->lightmaps.count;
		lightmap_index++)
	{
		struct structure_lightmap *lightmap;
		struct structure_material *last_material;
		struct bitmap_data *lightmap_bitmap;
		short material_index;

		if (surface_index_cursor >= surface_index_end)
		{
			break;
		}

		lightmap = TAG_BLOCK_GET_ELEMENT(
			&structure_bsp->lightmaps,
			lightmap_index,
			struct structure_lightmap);
		last_material = TAG_BLOCK_GET_ELEMENT(
			&lightmap->materials,
			lightmap->materials.count - 1,
			struct structure_material);

		if (*surface_index_cursor >= last_material->first_surface_index + last_material->surface_count)
		{
			continue;
		}

		if (structure_bsp->lightmap_group.index == NONE)
		{
			lightmap_bitmap = NULL;
		}
		else
		{
			lightmap_bitmap = bitmap_group_try_and_get_bitmap(
				structure_bsp->lightmap_group.index,
				lightmap->bitmap_index);
		}

		if (begin_lightmap)
		{
			begin_lightmap(lightmap_bitmap);
		}

		for (material_index = 0;
			material_index < lightmap->materials.count;
			material_index++)
		{
			struct structure_material *material;
			struct shader *shader;
			long *chunk_start;
			short chunk_count;

			if (surface_index_cursor >= surface_index_end)
			{
				break;
			}

			material = TAG_BLOCK_GET_ELEMENT(
				&lightmap->materials,
				material_index,
				struct structure_material);

			if (*surface_index_cursor >= material->first_surface_index + material->surface_count)
			{
				continue;
			}

			chunk_start = surface_index_cursor;
			shader = shader_definition_get(material->shader.index);

			do
			{
				surface_index_cursor++;
			}
			while (surface_index_cursor < surface_index_end
				&& *surface_index_cursor < material->first_surface_index + material->surface_count);

			chunk_count = (short)(surface_index_cursor - chunk_start);

			if (breakable_surface_extant(material->breakable_surface_index))
			{
				if (!shader_type_is_transparent(shader->base.type))
				{
					if (draw_triangles)
					{
						draw_triangles(
							shader,
							material->permutation_index,
							dynamic_triangles_index,
							first_triangle_index,
							chunk_count,
							&material->vertices);
					}
				}
				else if (draw_transparent_triangles)
				{
					draw_transparent_triangles(
						shader,
						material->permutation_index,
						lightmap_bitmap,
						dynamic_triangles_index,
						first_triangle_index,
						chunk_count,
						&material->vertices,
						&material->centroid,
						TEST_FLAG(material->flags, _structure_material_coplanar_bit)
							? &material->plane
							: NULL,
						TEST_FLAG(material->flags, _structure_material_fog_plane_bit)
							? &structure_render_globals.fog_offset_vector
							: global_zero_vector3d,
						&material->lighting,
						0);
				}
			}

			first_triangle_index += chunk_count;
		}

		if (end_lightmap)
		{
			end_lightmap();
		}
	}

	match_vassert(
		"c:\\halo\\SOURCE\\structures\\structure_render.c",
		599,
		surface_index_cursor >= surface_index_end,
		"there are more surfaces than materials that reference them, stupid.");

	return;
}

/* ---------- public code */

void structure_render_preprocess(
	void)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();

	profile_enter(render_structure_build_triangle_arrays_section);

	structure_render_globals.environment_triangles_index =
		structure_render_build_dynamic_triangles(
			render.environment_surface_indices,
			render.environment_surface_count,
			render.environment_surface_flags);

	profile_exit(render_structure_build_triangle_arrays_section);

	structure_render_globals.environment_geometry_valid =
		structure_render_globals.environment_triangles_index != NONE;

	if (debug_leaf_index >= 0 && debug_leaf_index < structure_bsp->leaf_map.leaves.count)
	{
		render_debug_leaf_portals(&structure_bsp->leaf_map, debug_leaf_index);
	}

	if (debug_leaf_portal_index >= 0 && debug_leaf_portal_index < structure_bsp->leaf_map.portals.count)
	{
		render_debug_leaf_portal(&structure_bsp->leaf_map, debug_leaf_portal_index);
	}

	if (debug_leaf_portals)
	{
		long portal_designator_index;

		for (portal_designator_index = 0;
			portal_designator_index < structure_bsp->leaf_map.portals.count;
			portal_designator_index++)
		{
			struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(
				&structure_bsp->leaf_map.portals,
				portal_designator_index,
				struct leaf_portal);

			render_debug_leaf_portal(&structure_bsp->leaf_map, portal_designator_index);
		}
	}

	structure_render_globals.fog_offset_valid = FALSE;
	structure_render_globals.fog_offset_vector = *global_zero_vector3d;

	return;
}

void structure_render_lightmaps(
	void)
{
	profile_enter(render_structure_lightmaps_section);

	if (structure_render_globals.environment_geometry_valid)
	{
		short saved_lightmap_mode = rasterizer_globals.lightmap_mode;

		if (global_structure_bsp_get()->lightmap_group.index == NONE
			&& !rasterizer_globals.lightmap_mode)
		{
			rasterizer_globals.lightmap_mode = _rasterizer_lightmap_mode_no_lightmap;
		}

		rasterizer_environment_lightmaps_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			rasterizer_environment_lightmap_begin,
			rasterizer_environment_lightmap_draw,
			rasterizer_environment_lightmap_end,
			NULL);

		rasterizer_environment_lightmaps_end();

		rasterizer_globals.lightmap_mode = saved_lightmap_mode;
	}

	profile_exit(render_structure_lightmaps_section);

	return;
}

void structure_render_diffuse_texture(
	void)
{
	profile_enter(render_structure_diffuse_textures_section);

	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_diffuse_textures_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_diffuse_texture_draw,
			NULL,
			NULL);

		rasterizer_environment_diffuse_textures_end();
	}

	profile_exit(render_structure_diffuse_textures_section);

	return;
}

void structure_render_specular_lightmaps(
	void)
{
	if (structure_render_globals.environment_geometry_valid)
	{
		short saved_lightmap_mode = rasterizer_globals.lightmap_mode;

		if (global_structure_bsp_get()->lightmap_group.index == NONE
			&& !rasterizer_globals.lightmap_mode)
		{
			rasterizer_globals.lightmap_mode = _rasterizer_lightmap_mode_no_lightmap;
		}

		rasterizer_environment_specular_lightmaps_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			rasterizer_environment_specular_lightmap_begin,
			rasterizer_environment_specular_lightmap_draw,
			rasterizer_environment_specular_lightmap_end,
			NULL);

		rasterizer_environment_specular_lightmaps_end();

		rasterizer_globals.lightmap_mode = saved_lightmap_mode;
	}

	return;
}

void structure_render_reflection_lightmap_masks(
	void)
{
	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_reflection_lightmap_masks_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			rasterizer_environment_reflection_lightmap_mask_begin,
			rasterizer_environment_reflection_lightmap_mask_draw,
			rasterizer_environment_reflection_lightmap_mask_end,
			NULL);

		rasterizer_environment_reflection_lightmap_masks_end();
	}

	return;
}

void structure_render_reflection_mirrors(
	void)
{
	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_reflection_mirrors_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_reflection_mirror_draw,
			NULL,
			NULL);

		rasterizer_environment_reflection_mirrors_end();
	}

	return;
}

void structure_render_reflections(
	void)
{
	profile_enter(render_structure_reflections_section);

	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_reflections_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_reflection_draw,
			NULL,
			NULL);

		rasterizer_environment_reflections_end();
	}

	profile_exit(render_structure_reflections_section);

	return;
}

void structure_render_transparent_geometry(
	void)
{
	profile_enter(render_structure_transparent_geometry_section);

	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_transparent_geometry_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			NULL,
			NULL,
			rasterizer_environment_transparent_geometry_submit);

		rasterizer_environment_transparent_geometry_end();
	}

	profile_exit(render_structure_transparent_geometry_section);

	return;
}

void structure_render_fog(
	void)
{
	profile_enter(render_structure_fog_section);

	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_fog_begin();

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_fog_draw,
			NULL,
			NULL);

		rasterizer_environment_fog_end();
	}

	profile_exit(render_structure_fog_section);

	return;
}

void structure_render_fog_screen(
	void)
{
	if (structure_render_globals.environment_geometry_valid)
	{
		rasterizer_environment_fog_screen_begin(FALSE);

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_fog_screen_draw,
			NULL,
			NULL);

		rasterizer_environment_fog_screen_end();

		rasterizer_environment_fog_screen_begin(TRUE);

		structure_render_pass(
			render.environment_surface_indices,
			render.environment_surface_count,
			structure_render_globals.environment_triangles_index,
			NULL,
			rasterizer_environment_fog_screen_draw,
			NULL,
			NULL);

		rasterizer_environment_fog_screen_end();
	}

	return;
}

void structure_render_specular_light(
	long rasterizer_light_index,
	real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	short cluster_count,
	short *cluster_indices)
{
	long surface_indices[MAXIMUM_LOCALLY_RENDERED_SURFACES];
	long *environment_surface_indices = surface_indices;
	long environment_surface_count;
	long environment_triangles_index;

	profile_enter(render_structure_specular_lights_section);

	if (cluster_indices)
	{
		environment_surface_count = structure_visibility_build_surfaces(
			surface_indices,
			MAXIMUM_LOCALLY_RENDERED_SURFACES,
			bounding_sphere_center,
			bounding_sphere_radius,
			NULL,
			0,
			NULL,
			cluster_count,
			cluster_indices);
		environment_triangles_index = structure_render_build_dynamic_triangles(
			surface_indices,
			environment_surface_count,
			NULL);
	}
	else
	{
		environment_triangles_index = structure_render_globals.environment_triangles_index;
		environment_surface_count = render.environment_surface_count;
		environment_surface_indices = render.environment_surface_indices;
	}

	if (environment_triangles_index != NONE)
	{
		profile_enter(render_structure_specular_lights_draw_section);

		rasterizer_environment_specular_light_begin(rasterizer_light_index);

		structure_render_pass(
			environment_surface_indices,
			environment_surface_count,
			environment_triangles_index,
			NULL,
			rasterizer_environment_specular_light_draw,
			NULL,
			NULL);

		rasterizer_environment_specular_light_end();

		if (cluster_indices)
		{
			rasterizer_dynamic_triangles_delete(environment_triangles_index);
		}

		profile_exit(render_structure_specular_lights_draw_section);
	}

	profile_exit(render_structure_specular_lights_section);

	return;
}

void structure_render_diffuse_light(
	long rasterizer_light_index,
	real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	short cluster_count,
	short *cluster_indices)
{
	long surface_indices[MAXIMUM_LOCALLY_RENDERED_SURFACES];
	long *environment_surface_indices = surface_indices;
	long environment_surface_count;
	long environment_triangles_index;

	profile_enter(render_structure_diffuse_lights_section);

	if (cluster_indices)
	{
		environment_surface_count = structure_visibility_build_surfaces(
			surface_indices,
			MAXIMUM_LOCALLY_RENDERED_SURFACES,
			bounding_sphere_center,
			bounding_sphere_radius,
			NULL,
			0,
			NULL,
			cluster_count,
			cluster_indices);
		environment_triangles_index = structure_render_build_dynamic_triangles(
			surface_indices,
			environment_surface_count,
			NULL);
	}
	else
	{
		environment_triangles_index = structure_render_globals.environment_triangles_index;
		environment_surface_count = render.environment_surface_count;
		environment_surface_indices = render.environment_surface_indices;
	}

	if (environment_triangles_index != NONE)
	{
		profile_enter(render_structure_diffuse_lights_draw_section);

		rasterizer_environment_diffuse_light_begin(rasterizer_light_index);

		structure_render_pass(
			environment_surface_indices,
			environment_surface_count,
			environment_triangles_index,
			NULL,
			rasterizer_environment_diffuse_light_draw,
			NULL,
			NULL);

		if (cluster_indices)
		{
			rasterizer_dynamic_triangles_delete(environment_triangles_index);
		}

		rasterizer_environment_diffuse_light_end();

		profile_exit(render_structure_diffuse_lights_draw_section);
	}

	profile_exit(render_structure_diffuse_lights_section);

	return;
}

void structure_render_shadow(
	real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	real_rectangle3d const *bounding_box,
	short bounding_surface_count,
	real_plane3d const *bounding_surfaces)
{
	long surface_indices[MAXIMUM_LOCALLY_RENDERED_SURFACES];
	long surface_count;
	long dynamic_triangles_index;

	profile_enter(render_structure_shadows_section);

	surface_count = structure_visibility_build_surfaces(
		surface_indices,
		MAXIMUM_LOCALLY_RENDERED_SURFACES,
		bounding_sphere_center,
		bounding_sphere_radius,
		bounding_box,
		bounding_surface_count,
		bounding_surfaces,
		0,
		NULL);

	dynamic_triangles_index = structure_render_build_dynamic_triangles(
		surface_indices,
		surface_count,
		NULL);

	if (dynamic_triangles_index != NONE)
	{
		profile_enter(render_structure_shadows_draw_section);

		structure_render_pass(
			surface_indices,
			surface_count,
			dynamic_triangles_index,
			NULL,
			rasterizer_environment_shadow_draw,
			NULL,
			NULL);

		rasterizer_dynamic_triangles_delete(dynamic_triangles_index);

		profile_exit(render_structure_shadows_draw_section);
	}

	profile_exit(render_structure_shadows_section);

	return;
}
