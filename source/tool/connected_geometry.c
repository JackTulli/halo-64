/*
CONNECTED_GEOMETRY.C

symbols in this file:
000F2CD0 0030:
	_connected_geometry_new (0000)
000F2D00 0050:
	_connected_geometry_delete (0000)
000F2D50 00d0:
	_connected_geometry_group_recursive (0000)
000F2E20 00c0:
	_connected_geometry_find_or_add_vertex (0000)
000F2EE0 00f0:
	_connected_geometry_find_or_add_edge (0000)
000F2FD0 00b0:
	_plane3d_from_points (0000)
000F3080 01a0:
	_connected_geometry_add_triangle (0000)
000F3220 0180:
	_triangle_coplanar (0000)
000F33A0 0080:
	_connected_geometry_add_intermediate_triangle (0000)
000F3420 0130:
	_connected_geometry_group_coplanar (0000)
00279CB0 0079:
	??_C@_0HJ@CCKALPEO@?$CD?$CD?$CD?5WARNING?3?5found?5duplicate?5tri@ (0000)
00279D30 0008:
	__real@3f847ae140000000 (0000)
004561B3 0001:
	_warned_about_duplicate_triangles (0000)
*/

/* ---------- headers */

#include "tool/connected_geometry.h"
#include "tool/error_geometry.h"

/* ---------- constants */

enum
{
	_connected_geometry_edge_designator_forward_bit = 31
};

#define CONNECTED_GEOMETRY_POINT_EPSILON 0.001f

/* ---------- macros */

/* ---------- structures */

struct intermediate_geometry_triangle
{
	long unused_before_vertex_indices[2];
	long vertex_indices[3];
	long unused_after_vertex_indices[8];
};

struct intermediate_geometry_vertex
{
	long unused_before_point[2];
	real_point3d point;
	long unused_after_point[15];
};

struct intermediate_geometry
{
	long unused_before_triangles[77];
	struct dynamic_array triangles;
	struct dynamic_array vertices;
};

typedef boolean (*connected_geometry_group_predicate)(
	void *predicate_data,
	struct connected_geometry *geometry,
	struct connected_geometry_triangle *triangle,
	long group_index);

typedef char connected_geometry_dynamic_array_size_assert[
	sizeof(struct dynamic_array) == 0xC ? 1 : -1];
typedef char connected_geometry_point_size_assert[
	sizeof(real_point3d) == 0xC ? 1 : -1];
typedef char connected_geometry_plane_size_assert[
	sizeof(real_plane3d) == 0x10 ? 1 : -1];
typedef char connected_geometry_edge_size_assert[
	sizeof(struct connected_geometry_edge) == 0x1C ? 1 : -1];
typedef char connected_geometry_edge_point_indices_offset_assert[
	offsetof(struct connected_geometry_edge, point_indices) == 0xC ? 1 : -1];
typedef char connected_geometry_triangle_size_assert[
	sizeof(struct connected_geometry_triangle) == 0x18 ? 1 : -1];
typedef char connected_geometry_size_assert[
	sizeof(struct connected_geometry) == 0x24 ? 1 : -1];
typedef char connected_geometry_edges_offset_assert[
	offsetof(struct connected_geometry, edges) == 0xC ? 1 : -1];
typedef char connected_geometry_triangles_offset_assert[
	offsetof(struct connected_geometry, triangles) == 0x18 ? 1 : -1];
typedef char intermediate_geometry_triangle_size_assert[
	sizeof(struct intermediate_geometry_triangle) == 0x34 ? 1 : -1];
typedef char intermediate_geometry_triangle_vertex_indices_offset_assert[
	offsetof(struct intermediate_geometry_triangle, vertex_indices) == 0x8 ? 1 : -1];
typedef char intermediate_geometry_vertex_size_assert[
	sizeof(struct intermediate_geometry_vertex) == 0x50 ? 1 : -1];
typedef char intermediate_geometry_vertex_point_offset_assert[
	offsetof(struct intermediate_geometry_vertex, point) == 0x8 ? 1 : -1];
typedef char intermediate_geometry_triangles_offset_assert[
	offsetof(struct intermediate_geometry, triangles) == 0x134 ? 1 : -1];
typedef char intermediate_geometry_vertices_offset_assert[
	offsetof(struct intermediate_geometry, vertices) == 0x140 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

static boolean warned_about_duplicate_triangles = FALSE;

/* ---------- public code */

void connected_geometry_new(
	struct connected_geometry *geometry)
{
	dynamic_array_new(&geometry->points, sizeof(real_point3d));
	dynamic_array_new(&geometry->edges, sizeof(struct connected_geometry_edge));
	dynamic_array_new(&geometry->triangles, sizeof(struct connected_geometry_triangle));

	return;
}

void connected_geometry_delete(
	struct connected_geometry *geometry)
{
	long edge_index;

	for (edge_index = 0; edge_index < geometry->edges.count; edge_index++)
	{
		struct connected_geometry_edge *edge = dynamic_array_get_element(
			&geometry->edges,
			edge_index,
			sizeof(*edge));
		dynamic_array_delete(&edge->triangle_indices);
	}

	dynamic_array_delete(&geometry->points);
	dynamic_array_delete(&geometry->edges);
	dynamic_array_delete(&geometry->triangles);

	return;
}

/* ---------- private code */

void connected_geometry_group_recursive(
	struct connected_geometry *geometry,
	connected_geometry_group_predicate predicate,
	void *predicate_data,
	long group_index,
	long triangle_index)
{
	struct connected_geometry_triangle *triangle;
	long *edge_designator;
	struct connected_geometry_edge *edge;
	long edge_triangle_index;
	short edge_triangle_counter;
	long edge_designator_count;

	triangle = dynamic_array_get_element(
		&geometry->triangles,
		triangle_index,
		sizeof(*triangle));
	if (triangle->coplanar_group_index == NONE &&
		(predicate == NULL ||
			predicate(predicate_data, geometry, triangle, group_index)))
	{
		triangle->coplanar_group_index = group_index;
		edge_designator = triangle->edge_designators;
		edge_designator_count = 3;
		do
		{
			if (*edge_designator != NONE)
			{
				edge = dynamic_array_get_element(
					&geometry->edges,
					*edge_designator & LONG_MAX,
					sizeof(*edge));
				edge_triangle_counter = 0;
				if (edge->triangle_indices.count > 0)
				{
					edge_triangle_index = 0;
					do
					{
						connected_geometry_group_recursive(
							geometry,
							predicate,
							predicate_data,
							group_index,
							*(long *)dynamic_array_get_element(
								&edge->triangle_indices,
								edge_triangle_index,
								sizeof(long)));
						edge_triangle_counter++;
						edge_triangle_index = (long)edge_triangle_counter;
					}
					while (edge_triangle_index < edge->triangle_indices.count);
				}
			}
			edge_designator++;
			edge_designator_count--;
		}
		while (edge_designator_count != 0);
	}

	return;
}

static long connected_geometry_find_or_add_vertex(
	struct connected_geometry *geometry,
	real_point3d const *point)
{
	real_point3d *existing_point;
	real pending_coordinate;
	long point_index;

	point_index = 0;
	if (geometry->points.count > 0)
	{
		do
		{
			existing_point = dynamic_array_get_element(
				&geometry->points,
				point_index,
				sizeof(*existing_point));
			if (fabs(point->x - existing_point->x) < CONNECTED_GEOMETRY_POINT_EPSILON)
			{
				pending_coordinate = point->y;
				existing_point = dynamic_array_get_element(
					&geometry->points,
					point_index,
					sizeof(*existing_point));
				if (fabs(pending_coordinate - existing_point->y) < CONNECTED_GEOMETRY_POINT_EPSILON)
				{
					pending_coordinate = point->z;
					existing_point = dynamic_array_get_element(
						&geometry->points,
						point_index,
						sizeof(*existing_point));
					if (fabs(pending_coordinate - existing_point->z) < CONNECTED_GEOMETRY_POINT_EPSILON)
					{
						break;
					}
				}
			}
			point_index++;
		}
		while (point_index < geometry->points.count);
	}

	if (point_index == geometry->points.count)
	{
		point_index = dynamic_array_add_element(&geometry->points);
		if (point_index != NONE)
		{
			existing_point = dynamic_array_get_element(
				&geometry->points,
				point_index,
				sizeof(*existing_point));
			existing_point->x = point->x;
			existing_point->y = point->y;
			existing_point->z = point->z;
		}
	}

	return point_index;
}

static long connected_geometry_find_or_add_edge(
	struct connected_geometry *geometry,
	long triangle_index,
	long point_index0,
	long point_index1)
{
	/* BUG (preserved for exact matching): January declares `direction` without an
	 * initializer. It is written only at the two matches (+0x47, +0x4d) and on edge
	 * creation (+0x67), and read once, at +0xbe. The only path that reaches that read
	 * without a write requires geometry->edges.count < 0 (a deleted array, whose count is
	 * NONE, is one way this can arise). On that path edge_index stays 0, so the
	 * dynamic_array_get_element call (+0x9b) runs first: its unconditional
	 * `array->count>=0` assertion fails, and match_assert's failure path (display_assert,
	 * then system_exit -> halt_and_catch_fire, which never returns: it loops on the error
	 * screen, or calls exit on re-entry) ends this build before `direction` is read.
	 * The later /Od+/RTC build attests the declaration with _RTC_UninitUse("direction").
	 * A corrected build should initialize direction to TRUE.
	 */
	boolean direction;
	long edge_index;

	for (edge_index = 0; edge_index < geometry->edges.count; edge_index++)
	{
		struct connected_geometry_edge *edge = dynamic_array_get_element(
			&geometry->edges,
			edge_index,
			sizeof(*edge));

		if (edge->point_indices[0] == point_index0 &&
			edge->point_indices[1] == point_index1)
		{
			direction = TRUE;
			break;
		}
		else if (edge->point_indices[0] == point_index1 &&
			edge->point_indices[1] == point_index0)
		{
			direction = FALSE;
			break;
		}
	}

	if (edge_index == geometry->edges.count)
	{
		edge_index = dynamic_array_add_element(&geometry->edges);
		direction = TRUE;
		if (edge_index != NONE)
		{
			struct connected_geometry_edge *edge = dynamic_array_get_element(
				&geometry->edges,
				edge_index,
				sizeof(*edge));

			dynamic_array_new(&edge->triangle_indices, sizeof(long));
			edge->point_indices[0] = point_index0;
			edge->point_indices[1] = point_index1;
		}
	}

	if (edge_index != NONE)
	{
		struct connected_geometry_edge *edge = dynamic_array_get_element(
			&geometry->edges,
			edge_index,
			sizeof(*edge));
		long edge_triangle_index = dynamic_array_add_element(&edge->triangle_indices);

		if (edge_triangle_index != NONE)
		{
			*(long *)dynamic_array_get_element(
				&edge->triangle_indices,
				edge_triangle_index,
				sizeof(long)) = triangle_index;
		}
		else
		{
			edge_index = NONE;
		}
	}

	return edge_index == NONE ?
		NONE :
		SET_FLAG(edge_index, _connected_geometry_edge_designator_forward_bit, direction);
}

long connected_geometry_add_triangle(
	struct connected_geometry *geometry,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2,
	boolean report_duplicates)
{
	long triangle_index = dynamic_array_add_element(&geometry->triangles);

	if (triangle_index != NONE)
	{
		struct connected_geometry_triangle *triangle = dynamic_array_get_element(
			&geometry->triangles,
			triangle_index,
			sizeof(*triangle));
		long point_indices[NUMBER_OF_EDGES_PER_TRIANGLE];
		short edge_index;

		point_indices[0] = connected_geometry_find_or_add_vertex(geometry, point0);
		point_indices[1] = connected_geometry_find_or_add_vertex(geometry, point1);
		point_indices[2] = connected_geometry_find_or_add_vertex(geometry, point2);
		if (point_indices[0] == NONE ||
			point_indices[1] == NONE ||
			point_indices[2] == NONE)
		{
			triangle_index = NONE;
		}

		for (edge_index = 0; edge_index < NUMBER_OF_EDGES_PER_TRIANGLE; edge_index++)
		{
			triangle->edge_designators[edge_index] = connected_geometry_find_or_add_edge(
				geometry,
				triangle_index,
				point_indices[edge_index],
				point_indices[(edge_index + 1) % NUMBER_OF_EDGES_PER_TRIANGLE]);
			if (triangle->edge_designators[edge_index] == NONE)
			{
				triangle_index = NONE;
			}
		}

		triangle->coplanar_group_index = NONE;
		memset(triangle->unused, 0, sizeof(triangle->unused));

		if (report_duplicates)
		{
			long existing_triangle_index;

			for (existing_triangle_index = 0;
				existing_triangle_index < geometry->triangles.count - 1;
				existing_triangle_index++)
			{
				struct connected_geometry_triangle *existing_triangle = dynamic_array_get_element(
					&geometry->triangles,
					existing_triangle_index,
					sizeof(*existing_triangle));
				short triangle_edge_index;
				short existing_triangle_edge_index;

				for (triangle_edge_index = 0;
					triangle_edge_index < NUMBER_OF_EDGES_PER_TRIANGLE;
					triangle_edge_index++)
				{
					for (existing_triangle_edge_index = 0;
						existing_triangle_edge_index < NUMBER_OF_EDGES_PER_TRIANGLE;
						existing_triangle_edge_index++)
					{
						if ((existing_triangle->edge_designators[existing_triangle_edge_index] & LONG_MAX) ==
							(triangle->edge_designators[triangle_edge_index] & LONG_MAX))
						{
							break;
						}
					}
					if (existing_triangle_edge_index == NUMBER_OF_EDGES_PER_TRIANGLE)
					{
						break;
					}
				}

				if (existing_triangle_edge_index != NUMBER_OF_EDGES_PER_TRIANGLE)
				{
					error_geometry_triangle(
						point0,
						point1,
						point2,
						global_real_argb_orange);
					if (!warned_about_duplicate_triangles)
					{
						printf("### WARNING: found duplicate triangle building connected geometry. YOU SHOULD FIX THIS. (see orange in error geometry)\r\n");
						warned_about_duplicate_triangles = TRUE;
					}

					triangle_index = NONE;
					break;
				}
			}
		}
	}

	return triangle_index;
}

static boolean triangle_coplanar(
	void *predicate_data,
	struct connected_geometry *geometry,
	struct connected_geometry_triangle *triangle,
	long group_index)
{
	real_plane3d *plane = predicate_data;
	real_point3d *point0 = dynamic_array_get_element(
		&geometry->points,
		((struct connected_geometry_edge *)dynamic_array_get_element(
			&geometry->edges,
			triangle->edge_designators[0] & LONG_MAX,
			sizeof(struct connected_geometry_edge)))->point_indices[
				(triangle->edge_designators[0] & LONG_MIN) != 0],
		sizeof(real_point3d));
	real_point3d *point1 = dynamic_array_get_element(
		&geometry->points,
		((struct connected_geometry_edge *)dynamic_array_get_element(
			&geometry->edges,
			triangle->edge_designators[1] & LONG_MAX,
			sizeof(struct connected_geometry_edge)))->point_indices[
				(triangle->edge_designators[1] & LONG_MIN) != 0],
		sizeof(real_point3d));
	real_point3d *point2 = dynamic_array_get_element(
		&geometry->points,
		((struct connected_geometry_edge *)dynamic_array_get_element(
			&geometry->edges,
			triangle->edge_designators[2] & LONG_MAX,
			sizeof(struct connected_geometry_edge)))->point_indices[
				(triangle->edge_designators[2] & LONG_MIN) != 0],
		 sizeof(real_point3d));
	real_plane3d triangle_plane;
	real point_distance;
	real point_product;
	real facing;

	if (fabs(
		point0->y*plane->n.j +
		point0->z*plane->n.k +
		point0->x*plane->n.i -
		plane->d) < 0.01f &&
		fabs(
			point1->y*plane->n.j +
			point1->z*plane->n.k +
			point1->x*plane->n.i -
			plane->d) < 0.01f)
	{
		point_distance = point2->y*plane->n.j;
		point_distance += point2->z*plane->n.k;
		point_product = point2->x*plane->n.i;
		point_distance += point_product;
		if (fabs(point_distance - plane->d) < 0.01f &&
			plane3d_from_points(&triangle_plane, point0, point2, point1) != NULL)
		{
			facing = triangle_plane.n.i*plane->n.i;
			facing += triangle_plane.n.k*plane->n.k;
			facing += triangle_plane.n.j*plane->n.j;
			if (facing > 0.0f)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void connected_geometry_add_intermediate_triangle(
	struct connected_geometry *geometry,
	struct intermediate_geometry *intermediate_geometry,
	long triangle_index,
	boolean report_duplicates)
{
	struct intermediate_geometry_triangle *triangle;

	triangle = dynamic_array_get_element(
		&intermediate_geometry->triangles,
		triangle_index,
		sizeof(*triangle));
	connected_geometry_add_triangle(
		geometry,
		&((struct intermediate_geometry_vertex *)dynamic_array_get_element(
			&intermediate_geometry->vertices,
			triangle->vertex_indices[0],
			sizeof(struct intermediate_geometry_vertex)))->point,
		&((struct intermediate_geometry_vertex *)dynamic_array_get_element(
			&intermediate_geometry->vertices,
			triangle->vertex_indices[1],
			sizeof(struct intermediate_geometry_vertex)))->point,
		&((struct intermediate_geometry_vertex *)dynamic_array_get_element(
			&intermediate_geometry->vertices,
			triangle->vertex_indices[2],
			sizeof(struct intermediate_geometry_vertex)))->point,
		report_duplicates);

	return;
}

long connected_geometry_group_coplanar(
	struct connected_geometry *geometry)
{
	struct dynamic_array *edges;
	struct connected_geometry_triangle *triangle;
	long group_count;
	long triangle_index;
	real_plane3d plane;

	group_count = 0;
	triangle_index = 0;
	if (geometry->triangles.count > 0)
	{
		edges = &geometry->edges;
		do
		{
			triangle = dynamic_array_get_element(
				&geometry->triangles,
				triangle_index,
				sizeof(*triangle));
			plane3d_from_points(
				&plane,
				dynamic_array_get_element(
					&geometry->points,
					((struct connected_geometry_edge *)dynamic_array_get_element(
						edges,
						triangle->edge_designators[0] & LONG_MAX,
						sizeof(struct connected_geometry_edge)))->point_indices[
							(triangle->edge_designators[0] & LONG_MIN) != 0],
					sizeof(real_point3d)),
				dynamic_array_get_element(
					&geometry->points,
					((struct connected_geometry_edge *)dynamic_array_get_element(
						edges,
						triangle->edge_designators[2] & LONG_MAX,
						sizeof(struct connected_geometry_edge)))->point_indices[
							(triangle->edge_designators[2] & LONG_MIN) != 0],
					sizeof(real_point3d)),
				dynamic_array_get_element(
					&geometry->points,
					((struct connected_geometry_edge *)dynamic_array_get_element(
						edges,
						triangle->edge_designators[1] & LONG_MAX,
						sizeof(struct connected_geometry_edge)))->point_indices[
							(triangle->edge_designators[1] & LONG_MIN) != 0],
					sizeof(real_point3d)));
			if (triangle->coplanar_group_index == NONE)
			{
				connected_geometry_group_recursive(
					geometry,
					triangle_coplanar,
					&plane,
					group_count,
					triangle_index);
				group_count++;
			}
			triangle_index++;
		}
		while (triangle_index < geometry->triangles.count);
	}

	return group_count;
}
