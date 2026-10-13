/*
STRUCTURE_LENS_FLARES.C

symbols in this file:
00183BB0 0020:
	_compare_temp_markers (0000)
00183BD0 0060:
	_cluster_index_from_point (0000)
00183C30 10f0:
	_build_structure_lens_flares (0000)
00184D20 0010:
	_structure_lens_flares_place (0000)
002A1370 0048:
	??_C@_0EI@BOLLHBJG@?$CD?$CD?$CD?5ERROR?5failed?5to?5clear?5lens?5f@ (0000)
002A13B8 0007:
	??_C@_06DMAFEADM@done?$AN?6?$AA@ (0000)
002A13C0 0048:
	??_C@_0EI@OPILBFAL@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5tem@ (0000)
002A1408 0047:
	??_C@_0EH@EENPIBPH@?$CD?$CD?$CD?5ERROR?5failed?5to?5sort?5lens?5fl@ (0000)
002A1450 0033:
	??_C@_0DD@CDKGJELE@?$CD?$CD?$CD?5ERROR?5failed?5to?5resize?5lens?5@ (0000)
002A1488 0047:
	??_C@_0EH@GEEEEHH@?$CD?$CD?$CD?5WARNING?5failed?5to?5add?5lens?5f@ (0000)
002A14D0 0066:
	??_C@_0GG@JDHLIDMF@point_count?$CL2?$DMMAXIMUM_TRIANGLES_@ (0000)
002A1538 0071:
	??_C@_0HB@PCJNLAFB@cluster?9?$DOfirst_lens_flare_marker@ (0000)
002A15B0 004f:
	??_C@_0EP@HDCOMOMJ@cluster?9?$DOfirst_lens_flare_marker@ (0000)
002A1600 0037:
	??_C@_0DH@MGKKDPBA@cluster_index?$DMtemp_markers?$FLmarke@ (0000)
002A1638 0040:
	??_C@_0EA@NCAOCOMC@?$CD?$CD?$CD?5WARNING?5failed?5to?5add?5lens?5f@ (0000)
002A1678 0055:
	??_C@_0FF@CDJGMDIB@surface?9?$DOvertex_indices?$FL2?$FN?$DO?$DN0?5?$CG?$CG@ (0000)
002A16D0 0055:
	??_C@_0FF@KHBPBDOH@surface?9?$DOvertex_indices?$FL1?$FN?$DO?$DN0?5?$CG?$CG@ (0000)
002A1728 0055:
	??_C@_0FF@NLGHKDMF@surface?9?$DOvertex_indices?$FL0?$FN?$DO?$DN0?5?$CG?$CG@ (0000)
002A1780 0022:
	??_C@_0CC@BLKMGOAC@building?5structure?5lens?5flares?4?4@ (0000)
002A17A4 000e:
	??_C@_0O@IHAGMCLG@structure_bsp?$AA@ (0000)
002A17B4 0032:
	??_C@_0DC@KJLLBBH@c?3?2halo?2SOURCE?2structures?2struct@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"

#include "structure_bsp_definitions.h"
#include "structures/structure_lens_flares.h"
#include "cache/cache_files.h"
#include "math/geometry.h"
#include "memory/array.h"
#include "physics/collision_bsp_definitions.h"
#include "scenario/scenario.h"
#include "shaders/shader_definitions.h"
#include "tool/connected_geometry.h"

/* ---------- constants */

enum
{
	_shader_type_environment = 3,
	_shader_type_transparent_generic = 5,
	_shader_type_transparent_chicago = 6,
	MAXIMUM_TRIANGLES_PER_CONNECTED_GEOMETRY_COPLANAR_GROUP = 20000,
};

/* ---------- macros */

#define POINT_DOT_VECTOR3D(point, vector) \
	((point)->x*(vector)->i + (point)->y*(vector)->j + (point)->z*(vector)->k)

/* ---------- structures */

struct temporary_lens_flare_marker
{
	real_point3d position;
	char direction[3];
	byte lens_flare_index;
	short cluster_index;
	word pad;
};

struct structure_lens_flare
{
	struct tag_reference lens_flare;
};

struct structure_lens_flare_marker
{
	real_point3d position;
	char direction[3];
	byte lens_flare_index;
};

struct structure_cluster_lens_flare_data
{
	byte reserved[0x40];
	word first_lens_flare_marker_index;
	word lens_flare_marker_count;
	byte trailing[0x24];
};

struct structure_environment_vertex
{
	real_point3d point;
	byte remaining[0x2C];
};

struct shader_lens_flare_fields_environment
{
	struct shader shader;
	long reserved;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
};

struct shader_lens_flare_fields_transparent
{
	struct shader shader;
	long reserved[3];
	real lens_flare_spacing;
	struct tag_reference lens_flare;
};

/* ---------- prototypes */

long compare_temp_markers(
	struct temporary_lens_flare_marker const *a,
	struct temporary_lens_flare_marker const *b);

/* ---------- globals */

/* ---------- public code */

void structure_lens_flares_place(
	void)
{
	build_structure_lens_flares(global_structure_bsp_get());

	return;
}

long cluster_index_from_point(
	struct structure_bsp const *structure_bsp,
	real_point3d const *point)
{
	long test_result = bsp3d_test_point(&TAG_BLOCK_GET_ELEMENT(&structure_bsp->collision_bsp, 0, struct collision_bsp)->bsp3d, 0, point);
	long result = NONE;
	
	if (test_result != NONE)
	{
		result = TAG_BLOCK_GET_ELEMENT(&structure_bsp->leaves, test_result & LONG_MAX, struct structure_leaf)->cluster_index;
	}

	return result;
}

boolean build_structure_lens_flares(
	struct structure_bsp *structure_bsp)
{
	boolean result = TRUE;
	real_point2d *projected_points = NULL;
	real_point3d *points = NULL;
	short *hull_indices = NULL;
	struct temporary_lens_flare_marker *temp_markers = NULL;
	struct tag_block *clusters;
	short lightmap_index;
	short cluster_index;

	match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 66, structure_bsp);

	fprintf(stdout, "building structure lens flares...");
	fflush(stdout);

	if (!tag_block_resize(&structure_bsp->lens_flares, 0) ||
		!tag_block_resize(&structure_bsp->lens_flare_markers, 0))
	{
		error(_error_silent, "### ERROR failed to clear lens flares and/or markers from structure_bsp");
		/* January has a distinct cold failure epilogue here. Keep this early
		 * return instead of folding it into the ordinary cleanup return. */
		return FALSE;
	}

	projected_points = match_malloc("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 74, sizeof(*projected_points) * MAXIMUM_TRIANGLES_PER_CONNECTED_GEOMETRY_COPLANAR_GROUP * NUMBER_OF_VERTICES_PER_TRIANGLE);
	points = match_malloc("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 75, sizeof(*points) * MAXIMUM_TRIANGLES_PER_CONNECTED_GEOMETRY_COPLANAR_GROUP * NUMBER_OF_VERTICES_PER_TRIANGLE);
	hull_indices = match_malloc("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 76, sizeof(*hull_indices) * MAXIMUM_TRIANGLES_PER_CONNECTED_GEOMETRY_COPLANAR_GROUP * NUMBER_OF_VERTICES_PER_TRIANGLE);

	if (structure_bsp && projected_points && points && hull_indices)
	{
	clusters = &structure_bsp->clusters;
	for (cluster_index = 0; cluster_index < clusters->count; cluster_index++)
	{
		TAG_BLOCK_GET_ELEMENT(
			clusters,
			cluster_index,
			struct structure_cluster_lens_flare_data)->first_lens_flare_marker_index = 0;
		TAG_BLOCK_GET_ELEMENT(
			clusters,
			cluster_index,
			struct structure_cluster_lens_flare_data)->lens_flare_marker_count = 0;
	}

	for (lightmap_index = 0; lightmap_index < structure_bsp->lightmaps.count; lightmap_index++)
	{
		struct structure_lightmap *lightmap = TAG_BLOCK_GET_ELEMENT(
			&structure_bsp->lightmaps,
			lightmap_index,
			struct structure_lightmap);
		short material_index;

		for (material_index = 0; material_index < lightmap->materials.count; material_index++)
		{
			struct structure_material *material = TAG_BLOCK_GET_ELEMENT(
				&lightmap->materials,
				material_index,
				struct structure_material);
			struct shader *shader;
			struct tag_reference *lens_flare_reference = NULL;
			real lens_flare_spacing = 0.f;
			short lens_flare_index = 0;
			struct connected_geometry geometry;
			long surface_offset;
			long coplanar_group_count;
			long coplanar_group_index;

			if (material->shader.index == NONE)
				continue;

			shader = tag_get('shdr', material->shader.index);
			switch (shader->base.type)
			{
			case _shader_type_environment:
			{
				struct shader_lens_flare_fields_environment *lens_flare_fields =
					(struct shader_lens_flare_fields_environment *)shader_get_and_verify_type(shader, _shader_type_environment);
				lens_flare_reference = &lens_flare_fields->lens_flare;
				lens_flare_spacing = ((struct shader_lens_flare_fields_environment *)shader_get_and_verify_type(shader, _shader_type_environment))->lens_flare_spacing;
				break;
			}
			case _shader_type_transparent_generic:
			{
				struct shader_lens_flare_fields_transparent *lens_flare_fields =
					(struct shader_lens_flare_fields_transparent *)shader_get_and_verify_type(shader, _shader_type_transparent_generic);
				lens_flare_reference = &lens_flare_fields->lens_flare;
				lens_flare_spacing = ((struct shader_lens_flare_fields_transparent *)shader_get_and_verify_type(shader, _shader_type_transparent_generic))->lens_flare_spacing;
				break;
			}
			case _shader_type_transparent_chicago:
			{
				struct shader_lens_flare_fields_transparent *lens_flare_fields =
					(struct shader_lens_flare_fields_transparent *)shader_get_and_verify_type(shader, _shader_type_transparent_chicago);
				lens_flare_reference = &lens_flare_fields->lens_flare;
				lens_flare_spacing = ((struct shader_lens_flare_fields_transparent *)shader_get_and_verify_type(shader, _shader_type_transparent_chicago))->lens_flare_spacing;
				break;
			}
			default:
				break;
			}

			if (!lens_flare_reference || lens_flare_reference->index == NONE)
				continue;

			lens_flare_index = 0;
			{
				long lens_flare_element_index = 0;

				if (structure_bsp->lens_flares.count > 0)
				{
					do
					{
						struct structure_lens_flare *lens_flare = TAG_BLOCK_GET_ELEMENT(
							&structure_bsp->lens_flares,
							lens_flare_element_index,
							struct structure_lens_flare);
						if (lens_flare->lens_flare.index == lens_flare_reference->index)
							break;
						++lens_flare_index;
						lens_flare_element_index = lens_flare_index;
					}
					while (lens_flare_element_index < structure_bsp->lens_flares.count);
				}
			}

			if (structure_bsp->lens_flares.count == lens_flare_index)
			{
				struct structure_lens_flare *lens_flare;

				if (structure_bsp->lens_flares.count < 256)
				{
					/* Original bug: the returned index is dereferenced without a
					 * NONE check. A hardened build should test the return value
					 * before TAG_BLOCK_GET_ELEMENT. */
					lens_flare = TAG_BLOCK_GET_ELEMENT(
						&structure_bsp->lens_flares,
						tag_block_add_element(&structure_bsp->lens_flares),
						struct structure_lens_flare);
					tag_reference_set(
						&lens_flare->lens_flare,
						lens_flare_reference->group_tag,
						lens_flare_reference->name);
					lens_flare->lens_flare.index = lens_flare_reference->index;
				}
				else
				{
					error(_error_silent, "### WARNING failed to add lens flare to structure_bsp (max=#%d)", 256);
					lens_flare_index = NONE;
				}
			}

			if (lens_flare_index != NONE)
			{
			struct structure_environment_vertex *vertices = material->uncompressed_vertex_data.address;

			connected_geometry_new(&geometry);
			for (surface_offset = 0; surface_offset < material->surface_count; surface_offset++)
			{
				struct structure_surface *surface = TAG_BLOCK_GET_ELEMENT(
					&structure_bsp->surfaces,
					material->first_surface_index + surface_offset,
					struct structure_surface);
				match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 179,
					surface->vertex_indices[0]>=0 && surface->vertex_indices[0]<material->vertices.count);
				match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 180,
					surface->vertex_indices[1]>=0 && surface->vertex_indices[1]<material->vertices.count);
				match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 181,
					surface->vertex_indices[2]>=0 && surface->vertex_indices[2]<material->vertices.count);

				connected_geometry_add_triangle(
					&geometry,
					&vertices[surface->vertex_indices[0]].point,
					&vertices[surface->vertex_indices[1]].point,
					&vertices[surface->vertex_indices[2]].point,
					FALSE);
			}

			coplanar_group_count = connected_geometry_group_coplanar(&geometry);
			for (coplanar_group_index = 0; coplanar_group_index < coplanar_group_count; coplanar_group_index++)
			{
				short point_count = 0;
				short triangle_index;
				short projection_axis;
				boolean projection_sign;
				real_plane3d plane;
				short hull_count;

				for (triangle_index = 0; triangle_index < geometry.triangles.count; triangle_index++)
				{
					struct connected_geometry_triangle *triangle = dynamic_array_get_element(
						&geometry.triangles,
						triangle_index,
						sizeof(*triangle));
					real_point3d *triangle_point0;
					real_point3d *triangle_point1;
					real_point3d *triangle_point2;

					if (triangle->coplanar_group_index != coplanar_group_index)
						continue;

					triangle_point0 = dynamic_array_get_element(
						&geometry.points,
						((struct connected_geometry_edge *)dynamic_array_get_element(
							&geometry.edges,
							triangle->edge_designators[0] & LONG_MAX,
							sizeof(struct connected_geometry_edge)))->point_indices[(triangle->edge_designators[0] & LONG_MIN) != 0],
						sizeof(*triangle_point0));

					triangle_point1 = dynamic_array_get_element(
						&geometry.points,
						((struct connected_geometry_edge *)dynamic_array_get_element(
							&geometry.edges,
							triangle->edge_designators[1] & LONG_MAX,
							sizeof(struct connected_geometry_edge)))->point_indices[(triangle->edge_designators[1] & LONG_MIN) != 0],
						sizeof(*triangle_point1));

					triangle_point2 = dynamic_array_get_element(
						&geometry.points,
						((struct connected_geometry_edge *)dynamic_array_get_element(
							&geometry.edges,
							triangle->edge_designators[2] & LONG_MAX,
							sizeof(struct connected_geometry_edge)))->point_indices[(triangle->edge_designators[2] & LONG_MIN) != 0],
						sizeof(*triangle_point2));

					if (point_count == 0)
					{
						real i;
						real j;
						real k;

						plane3d_from_points(
							&plane,
							triangle_point2,
							triangle_point1,
							triangle_point0);
						i = fabs(plane.n.i);
						j = fabs(plane.n.j);
						k = fabs(plane.n.k);
						if (!(k >= j) || !(k >= i))
							projection_axis = j >= i;
						else
							projection_axis = _z;
						projection_sign = plane.n.n[projection_axis] > 0.f;
					}

					match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 218,
						point_count+2<MAXIMUM_TRIANGLES_PER_CONNECTED_GEOMETRY_COPLANAR_GROUP*NUMBER_OF_VERTICES_PER_TRIANGLE);
					points[point_count] = *triangle_point0;
					points[point_count + 1] = *triangle_point1;
					points[point_count + 2] = *triangle_point2;
					project_point3d(
						triangle_point0,
						projection_axis,
						projection_sign,
						&projected_points[point_count]);
					project_point3d(
						triangle_point1,
						projection_axis,
						projection_sign,
						&projected_points[point_count + 1]);
					project_point3d(
						triangle_point2,
						projection_axis,
						projection_sign,
						&projected_points[point_count + 2]);
					point_count += 3;
				}

				hull_count = convex_hull2d(point_count, projected_points, hull_indices);
				if (hull_count >= 3)
				{
					real_point3d origin = *global_origin3d;
					real_plane3d s_plane;
					real_plane3d t_plane;
					real_rectangle2d bounds;
					short hull_index;
					rectangle2d grid_bounds;
					long t_count;
					long t_grid;

					for (hull_index = 0; hull_index < hull_count; hull_index++)
					{
						origin.x += points[hull_indices[hull_index]].x;
						origin.y += points[hull_indices[hull_index]].y;
						origin.z += points[hull_indices[hull_index]].z;
					}
					origin.x = origin.x * (1.f / (real)hull_count);
					origin.y = origin.y * (1.f / (real)hull_count);
					origin.z = origin.z * (1.f / (real)hull_count);

					{
						real_plane3d s_temp;

						vector_from_points3d(&points[0], &points[1], &s_temp.n);
						normalize3d(&s_temp.n);
						cross_product3d(&plane.n, &s_temp.n, &t_plane.n);
						plane3d_from_point_and_normal(&s_temp, &origin, &s_temp.n);
						s_plane = s_temp;
					}

					{
						plane3d_from_point_and_normal(&t_plane, &origin, &t_plane.n);

						bounds.x0 = bounds.x1 = plane3d_distance_to_point(&s_plane, &points[hull_indices[0]]);
						bounds.y0 = bounds.y1 = plane3d_distance_to_point(&t_plane, &points[hull_indices[0]]);
						for (hull_index = 0; hull_index < hull_count; hull_index++)
						{
							real_point3d const *point = &points[hull_indices[hull_index]];
							real s = plane3d_distance_to_point(&s_plane, point);
							real t_coordinate = plane3d_distance_to_point(&t_plane, point);

							bounds.x0 = MIN(s, bounds.x0);
							bounds.y0 = MIN(t_coordinate, bounds.y0);
							bounds.x1 = MAX(s, bounds.x1);
							bounds.y1 = MAX(t_coordinate, bounds.y1);
						}
					}

					if (lens_flare_spacing != 0.f)
					{
						grid_bounds.x0 = (short)fast_ftol((real)ceil(PIN(bounds.x0 / lens_flare_spacing, -1000.f, 1000.f)));
						grid_bounds.y0 = (short)fast_ftol((real)ceil(PIN(bounds.y0 / lens_flare_spacing, -1000.f, 1000.f)));
						grid_bounds.x1 = (short)fast_ftol((real)floor(PIN(bounds.x1 / lens_flare_spacing, -1000.f, 1000.f)));
						grid_bounds.y1 = (short)fast_ftol((real)floor(PIN(bounds.y1 / lens_flare_spacing, -1000.f, 1000.f)));
					}
					else
					{
						grid_bounds.x0 = grid_bounds.y0 = grid_bounds.x1 = grid_bounds.y1 = 0;
					}

					if (grid_bounds.y0 <= grid_bounds.y1)
					{
						t_grid = grid_bounds.y0;
						t_count = (unsigned short)(grid_bounds.y1 - grid_bounds.y0 + 1);
						do
						{
							if (grid_bounds.x0 <= grid_bounds.x1)
							{
								long s_grid = grid_bounds.x0;
								long s_count = (unsigned short)(grid_bounds.x1 - grid_bounds.x0 + 1);
								real t_distance = (real)t_grid * lens_flare_spacing;
								do
								{
									real_point3d position = origin;
									real_point2d projected_position;
									real s_distance = (real)s_grid * lens_flare_spacing;
									long marker_index;
									struct structure_lens_flare_marker *marker;

									{
										real ds_x = s_plane.n.i * s_distance;

										position.x = position.x + ds_x;
									}
									position.y = position.y + s_plane.n.j * s_distance;
									position.z = position.z + s_plane.n.k * s_distance;
									position.x = t_plane.n.i * t_distance + position.x;
									position.y = t_plane.n.j * t_distance + position.y;
									position.z = t_plane.n.k * t_distance + position.z;
									project_point3d(&position, projection_axis, projection_sign, &projected_position);
									if (convex_hull2d_test_point_indexed(
										hull_count,
										hull_indices,
										projected_points,
										&projected_position,
										0.f))
									{
										marker_index = tag_block_add_element(&structure_bsp->lens_flare_markers);
										if (marker_index != NONE)
										{
											marker = TAG_BLOCK_GET_ELEMENT(
												&structure_bsp->lens_flare_markers,
												marker_index,
												struct structure_lens_flare_marker);
											marker->position = position;
											marker->direction[0] = (char)fast_ftol((real)floor(plane.n.i * 127.5f));
											marker->direction[1] = (char)fast_ftol((real)floor(plane.n.j * 127.5f));
											marker->direction[2] = (char)fast_ftol((real)floor(plane.n.k * 127.5f));
											marker->lens_flare_index = (byte)lens_flare_index;
										}
										else
										{
											error(_error_silent, "### WARNING failed to add lens flare marker to structure_bsp (max=#%d)", 65536);
										}
									}

									s_grid++;
									s_count--;
								}
								while (s_count != 0);
							}

							t_grid++;
							t_count--;
						}
						while (t_count != 0);
					}
			}
			}
			connected_geometry_delete(&geometry);
			}
			else
			{
				error(_error_silent, "### WARNING failed to add lens flare to structure_bsp (max=#%d)", 256);
			}
		}
	}

	temp_markers = match_malloc(
		"c:\\halo\\SOURCE\\structures\\structure_lens_flares.c",
		363,
		sizeof(*temp_markers) * structure_bsp->lens_flare_markers.count);
	if (temp_markers)
	{
	long marker_index;
	long marker_write_index = 0;

	cluster_index = NONE;
	{
		for (marker_index = 0; marker_index < structure_bsp->lens_flare_markers.count; marker_index++)
		{
			struct structure_lens_flare_marker *marker = TAG_BLOCK_GET_ELEMENT(
				&structure_bsp->lens_flare_markers,
				marker_index,
				struct structure_lens_flare_marker);
			real_vector3d direction;
			real_point3d test_point = marker->position;
			real offset;

			direction.i = (real)marker->direction[0] * (1.f / 127.f);
			direction.j = (real)marker->direction[1] * (1.f / 127.f);
			direction.k = (real)marker->direction[2] * (1.f / 127.f);
			offset = 1.f / 65536.f;
			csmemcpy(&temp_markers[marker_index], marker, sizeof(*marker));

			do
			{
				temp_markers[marker_index].cluster_index = cluster_index_from_point(structure_bsp, &test_point);
				/* Preserve the January inline schedule without emitting a
				 * point_from_line3d COMDAT. */
				test_point.x = direction.i * offset + test_point.x;
				test_point.y = direction.j * offset + test_point.y;
				test_point.z = direction.k * offset + test_point.z;
				offset += offset;
			}
			while (temp_markers[marker_index].cluster_index == NONE && offset < 1.f);
		}
	}

	qsort(
		temp_markers,
		structure_bsp->lens_flare_markers.count,
		sizeof(*temp_markers),
		(int (__cdecl *)(const void *, const void *))compare_temp_markers);

	{
		for (marker_index = 0; marker_index < structure_bsp->lens_flare_markers.count; marker_index++)
		{
			struct temporary_lens_flare_marker *temporary_marker = &temp_markers[marker_index];
			struct structure_cluster_lens_flare_data *cluster;
			struct structure_lens_flare_marker *marker;

			if (temporary_marker->cluster_index == NONE)
				continue;

			cluster = TAG_BLOCK_GET_ELEMENT(
				clusters,
				temporary_marker->cluster_index,
				struct structure_cluster_lens_flare_data);
			if (temporary_marker->cluster_index != cluster_index)
			{
				match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 415,
					cluster_index<temp_markers[marker_index].cluster_index);
				cluster_index = temporary_marker->cluster_index;
				cluster->first_lens_flare_marker_index = marker_write_index;
			}

			marker = TAG_BLOCK_GET_ELEMENT(
				&structure_bsp->lens_flare_markers,
				marker_write_index,
				struct structure_lens_flare_marker);
			csmemcpy(marker, temporary_marker, sizeof(*marker));
			marker_write_index++;
			cluster->lens_flare_marker_count++;
		}

		if (tag_block_resize(&structure_bsp->lens_flare_markers, marker_write_index))
		{
			for (cluster_index = 0; cluster_index < clusters->count; cluster_index++)
			{
				struct structure_cluster_lens_flare_data *cluster = TAG_BLOCK_GET_ELEMENT(
					clusters,
					cluster_index,
					struct structure_cluster_lens_flare_data);
				if (cluster->lens_flare_marker_count > 0)
				{
					match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 444,
						cluster->first_lens_flare_marker_index<structure_bsp->lens_flare_markers.count);
					match_assert("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 445,
						cluster->first_lens_flare_marker_index+cluster->lens_flare_marker_count<=structure_bsp->lens_flare_markers.count);
				}
			}
		}
		else
		{
			error(_error_silent, "### ERROR failed to resize lens flare marker block");
			result = FALSE;
		}
	}

	match_free("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 455, temp_markers);
	}
	else
	{
		error(_error_silent, "### ERROR failed to sort lens flare markers by cluster (out of memory)");
		result = FALSE;
	}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary buffers for lens flare placement");
		result = FALSE;
	}

	if (projected_points)
		match_free("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 470, projected_points);
	if (points)
		match_free("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 471, points);
	if (hull_indices)
		match_free("c:\\halo\\SOURCE\\structures\\structure_lens_flares.c", 472, hull_indices);

	if (result)
		fprintf(stdout, "done\r\n");

	return result;
}

/* ---------- private code */

long compare_temp_markers(
	struct temporary_lens_flare_marker const *a,
	struct temporary_lens_flare_marker const *b)
{
	return a->cluster_index > b->cluster_index ? 1 : -1;
}
