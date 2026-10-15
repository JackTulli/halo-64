/*
COLLISION_BSP.C

symbols in this file:
00136C00 0060:
	_collision_surface_edge_count (0000)
00136C60 00e0:
	_collision_surface_polygon (0000)
00136D40 0030:
	_add_feature (0000)
00136D70 0050:
	_render_debug_collision_vertex (0000)
00136DC0 0080:
	_render_debug_collision_edge (0000)
00136E40 0070:
	_render_debug_collision_surface (0000)
00136EB0 0040:
	_render_debug_collision_bsp (0000)
00136EF0 0070:
	_collision_edge_length (0000)
00136F60 00e0:
	_collision_surface_perimeter (0000)
00137040 01a0:
	_collision_surface_area (0000)
001371E0 0050:
	_collision_surface_project_point2d (0000)
00137230 0100:
	_collision_surface_test_point2d (0000)
00137330 0230:
	_collision_surface_find_closest_point2d (0000)
00137560 01c0:
	_collision_surface_test_line2d (0000)
00137720 0370:
	_collision_surface_test_sphere (0000)
00137A90 0130:
	_code_00137a90 (0000)
00137BC0 00d0:
	_code_00137bc0 (0000)
00137C90 02a0:
	_code_00137c90 (0000)
00137F30 00a0:
	_bsp2d_test_sphere_recursive (0000)
00137FD0 0190:
	_code_00137fd0 (0000)
00138160 0210:
	_code_00138160 (0000)
00138370 0070:
	_collision_bsp_test_pill_new (0000)
001383E0 0320:
	_bsp3d_test_sphere_recursive (0000)
00138700 0320:
	_code_00138700 (0000)
00138A20 01e0:
	_code_00138a20 (0000)
00138C00 00d0:
	_collision_bsp_test_sphere (0000)
00138CD0 00f0:
	_collision_bsp_test_vector (0000)
00138DC0 0110:
	_code_00138dc0 (0000)
00138ED0 05e0:
	_code_00138ed0 (0000)
001394B0 0080:
	_collision_bsp_test_pill (0000)
0028AFF8 0033:
	??_C@_0DD@MEMDNOLF@point_count?$DMMAXIMUM_VERTICES_PER@ (0000)
0028B02C 0027:
	??_C@_0CH@BOKFNJDK@c?3?2halo?2SOURCE?2physics?2collision@ (0000)
0028B054 003e:
	??_C@_0DO@KILIPDDJ@data?9?$DOstack_depth?$DO?$DN0?5?$CG?$CG?5data?9?$DOst@ (0000)
0028B094 0004:
	__real@39000000 (0000)
00456EB0 0010:
	_collision_bsp_usage_times (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "collision_bsp.h"
#include "collision_bsp_definitions.h"
#include "collision_usage.h"
#include "collisions.h"
#include "render/render_debug.h"
#include "scenario/scenario.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MAXIMUM_BSP3D_DEPTH = 128,
};

enum collision_surface_flags
{
	_collision_surface_two_sided_bit,
	_collision_surface_invisible_bit,
	_collision_surface_climbable_bit,
	_collision_surface_breakable_bit,
};

enum collision_leaf_flags
{
	_collision_leaf_contains_two_sided_bit,
};

enum contents
{
	_contents_unknown,
	_contents_empty,
	_contents_semi_empty,
	_contents_solid,
};

/* ---------- macros */

#define BSP3D_TEST_PILL_EPSILON (1.f/4096.f)
#define BSP2D_TEST_PILL_EPSILON (1.f/8192.f)

/* ---------- structures */

struct collision_bsp_test_pill_new_context
{
	struct collision_bsp const *bsp;
	short breakable_surface_count;
	short pad;
	byte const *breakable_surface_flags;
	real_point3d const *point;
	real_vector3d const *vector;
	real radius;
	real *t;
	real_vector3d *normal;
	long last_leaf_index;
	byte last_contents;
	byte pad2[3];
	long last_plane_designator;
};

typedef char collision_bsp_test_pill_new_context_size_assert[
	sizeof(struct collision_bsp_test_pill_new_context) == 0x2C ? 1 : -1];

struct collision_bsp_test_pill_context
{
	struct collision_bsp const *bsp;
	real_point3d const *point;
	real_vector3d const *vector;
	real radius;
	struct collision_bsp_test_pill_result *result;
	long stack_depth;
	long plane_stack[MAXIMUM_BSP3D_DEPTH];
	short projection_axis;
	boolean projection_sign;
	byte pad;
	real_point2d point2d;
	real_vector2d vector2d;
};

typedef char collision_bsp_test_pill_context_size_assert[
	sizeof(struct collision_bsp_test_pill_context) == 0x22C ? 1 : -1];

struct collision_bsp_test_sphere_context
{
	struct collision_bsp const *bsp;
	short breakable_surface_count;
	short pad;
	byte const *breakable_surface_flags;
	real_point3d const *center;
	real radius;
	struct collision_bsp_test_sphere_result *result;
	long stack_depth;
	long plane_stack[MAXIMUM_BSP3D_DEPTH];
	short projection_axis;
	boolean projection_sign;
	byte pad2;
	real_point2d center2d;
};

typedef char collision_bsp_test_sphere_context_size_assert[
	sizeof(struct collision_bsp_test_sphere_context) == 0x228 ? 1 : -1];
typedef char collision_bsp_test_sphere_context_plane_stack_offset_assert[
	offsetof(struct collision_bsp_test_sphere_context, plane_stack) == 0x1C ? 1 : -1];
typedef char collision_bsp_test_sphere_context_projection_axis_offset_assert[
	offsetof(struct collision_bsp_test_sphere_context, projection_axis) == 0x21C ? 1 : -1];
typedef char collision_bsp_test_sphere_context_center2d_offset_assert[
	offsetof(struct collision_bsp_test_sphere_context, center2d) == 0x220 ? 1 : -1];

#ifdef HALO_N64_CAMPAIGN
/* (the campaign cartridge's, 4 bytes each: bsp3d.h) */
struct collision_leaf
{
	byte flags;
	byte bsp2d_reference_count;
	unsigned short first_bsp2d_reference_index;
};

struct bsp2d_reference
{
	bsp_flagged_index plane_designator;
	bsp_flagged_index root_index;
};

typedef char collision_leaf_size_assert[
	sizeof(struct collision_leaf) == 0x04 ? 1 : -1];
typedef char bsp2d_reference_size_assert[
	sizeof(struct bsp2d_reference) == 0x04 ? 1 : -1];
#else
struct collision_leaf
{
	word flags;
	short bsp2d_reference_count;
	long first_bsp2d_reference_index;
};

struct bsp2d_reference
{
	long plane_designator;
	long root_index;
};

typedef char collision_leaf_size_assert[
	sizeof(struct collision_leaf) == 0x08 ? 1 : -1];
typedef char bsp2d_reference_size_assert[
	sizeof(struct bsp2d_reference) == 0x08 ? 1 : -1];
#endif

struct collision_bsp_test_vector_context
{
	unsigned long flags;
	struct collision_bsp const *bsp;
	short breakable_surface_count;
	short pad;
	byte const *breakable_surface_flags;
	real_point3d const *point;
	real_vector3d const *vector;
	struct collision_bsp_test_vector_result *result;
	long last_leaf_index;
	byte last_contents;
	byte pad2[3];
	long last_plane_index;
};

typedef char collision_bsp_test_vector_context_size_assert[
	sizeof(struct collision_bsp_test_vector_context) == 0x28 ? 1 : -1];

struct collision_bsp_usage_times
{
	LARGE_INTEGER vector;
	LARGE_INTEGER sphere;
};

/* ---------- prototypes */

static boolean collision_surface_test_point(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point);
static boolean collision_bsp_test_pill_new_recursive(
	struct collision_bsp_test_pill_new_context *data,
	long node_index,
	real t0,
	real t1);
static long collision_leaf_test_vector(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long leaf_index,
	long plane_index,
	real t,
	boolean test_surface);
static boolean collision_bsp_test_vector_recursive(
	struct collision_bsp_test_vector_context *data,
	long node_index,
	real t0,
	real t1);
static boolean sphere_test_vector(
	real_point3d const *center,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t);
static boolean pill_test_vector(
	real_point3d const *base,
	real_vector3d const *edge,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t,
	real *edge_t);
static boolean collision_surface_test_pill(
	struct collision_bsp_test_pill_context *data,
	long surface_index);
static boolean bsp2d_test_pill_recursive(
	struct collision_bsp_test_pill_context *data,
	long child_index);
static boolean bsp3d_test_pill_recursive(
	struct collision_bsp_test_pill_context *data,
	long node_index);
static void add_feature(
	long *count,
	long *indices,
	long index);
#ifdef HALO_N64
static void n64_feature_hash_begin(
	void);
#endif
static void collision_surface_test_sphere(
	struct collision_bsp_test_sphere_context *data,
	long surface_index);
static void bsp2d_test_sphere_recursive(
	struct collision_bsp_test_sphere_context *data,
	long child_index);
static void bsp3d_test_sphere_recursive(
	struct collision_bsp_test_sphere_context *data,
	long node_index);
void render_debug_collision_edge(
	struct collision_bsp *bsp,
	long edge_index,
	real_matrix4x3 const *matrix,
	real_argb_color const *color);

/* ---------- globals */

static struct collision_bsp_usage_times collision_bsp_usage_times = { 0 };

/* ---------- public code */

short collision_surface_edge_count(
	struct collision_bsp const *bsp,
	long surface_index)
{
	short edge_count = 0;
	struct collision_surface const *surface;
	long first_edge_index;
	long edge_index;

	surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	first_edge_index = surface->first_edge_index;
	edge_index = first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;

		edge_count++;
		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return edge_count;
}

short collision_surface_polygon(
	struct collision_bsp const *bsp,
	long surface_index,
	real_point3d *points)
{
	short point_count = 0;
	struct collision_surface const *surface;
	long first_edge_index;
	long edge_index;

	surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	first_edge_index = surface->first_edge_index;
	edge_index = first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);

		match_assert(
			"c:\\halo\\SOURCE\\physics\\collision_bsp.c",
			0xe1,
			point_count<MAXIMUM_VERTICES_PER_COLLISION_SURFACE);

		points[point_count++] = vertex->point;
		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return point_count;
}

void render_debug_collision_vertex(
	struct collision_bsp *bsp,
	long vertex_index,
	real_matrix4x3 const *matrix,
	real scale,
	real_argb_color const *color)
{
	struct collision_vertex const *vertex;
	real_point3d const *point;
	real_point3d transformed_point;

	vertex = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		vertex_index,
		struct collision_vertex);
	point = &vertex->point;

	if (matrix)
	{
		point = matrix4x3_transform_point(
			matrix,
			point,
			&transformed_point);
	}

	render_debug_point(TRUE, point, scale, color);

	return;
}

void render_debug_collision_edge(
	struct collision_bsp *bsp,
	long edge_index,
	real_matrix4x3 const *matrix,
	real_argb_color const *color)
{
	struct collision_edge const *edge;
	struct collision_vertex const *vertex0;
	struct collision_vertex const *vertex1;
	real_point3d const *point0;
	real_point3d const *point1;
	real_point3d transformed_point0;
	real_point3d transformed_point1;

	edge = TAG_BLOCK_GET_ELEMENT(
		&bsp->edges,
		edge_index,
		struct collision_edge);
	vertex0 = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		edge->vertex_indices[0],
		struct collision_vertex);
	vertex1 = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		edge->vertex_indices[1],
		struct collision_vertex);
	point0 = &vertex0->point;
	point1 = &vertex1->point;

	if (matrix)
	{
		point0 = matrix4x3_transform_point(
			matrix,
			point0,
			&transformed_point0);
		point1 = matrix4x3_transform_point(
			matrix,
			point1,
			&transformed_point1);
	}

	render_debug_line(TRUE, point0, point1, color);

	return;
}

void render_debug_collision_surface(
	struct collision_bsp *bsp,
	long surface_index,
	real_matrix4x3 const *matrix,
	real_argb_color const *color)
{
	struct collision_surface const *surface;
	long first_edge_index;
	long edge_index;

	surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	first_edge_index = surface->first_edge_index;
	edge_index = first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;

		render_debug_collision_edge(
			bsp,
			edge_index,
			matrix,
			color);
		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return;
}

void render_debug_collision_bsp(
	struct collision_bsp *bsp,
	real_matrix4x3 const *matrix)
{
	long edge_index;

	for (edge_index = 0; edge_index < bsp->edges.count; edge_index++)
	{
		render_debug_collision_edge(
			bsp,
			edge_index,
			matrix,
			global_real_argb_green);
	}

	return;
}

real collision_edge_length(
	struct collision_bsp const *bsp,
	long edge_index)
{
	struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
		&bsp->edges,
		edge_index,
		struct collision_edge);
	struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		edge->vertex_indices[0],
		struct collision_vertex);
	struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		edge->vertex_indices[1],
		struct collision_vertex);

	return distance3d(&vertex0->point, &vertex1->point);
}

real collision_surface_perimeter(
	struct collision_bsp const *bsp,
	long surface_index)
{
	real perimeter = 0.f;
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	long const first_edge_index = surface->first_edge_index;
	long edge_index = first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);

		perimeter += distance3d(&vertex0->point, &vertex1->point);
		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return perimeter;
}

real collision_surface_area(
	struct collision_bsp const *bsp,
	long surface_index)
{
	real_plane3d plane;
	real_vector3d p_vector;
	real_vector3d q_vector;
	real_vector3d cross;
	struct collision_vertex const *anchor;
	struct collision_vertex const *p;
	struct collision_vertex const *q;
	struct collision_surface const *surface;
	struct collision_edge const *edge;
	boolean reverse;
	boolean owner;
	real area = 0.f;

	surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	edge = TAG_BLOCK_GET_ELEMENT(
		&bsp->edges,
		surface->first_edge_index,
		struct collision_edge);
	reverse = edge->surface_indices[1] == surface_index;
	anchor = TAG_BLOCK_GET_ELEMENT(
		&bsp->vertices,
		edge->vertex_indices[reverse],
		struct collision_vertex);
	bsp3d_get_plane_from_designator(
		&bsp->bsp3d,
		surface->plane_designator,
		&plane);
	edge = TAG_BLOCK_GET_ELEMENT(
		&bsp->edges,
		edge->edge_indices[reverse],
		struct collision_edge);
	owner = edge->surface_indices[1] == surface_index;
	reverse = owner;

	if (edge->edge_indices[reverse] != surface->first_edge_index)
	{
		do
		{
			p = TAG_BLOCK_GET_ELEMENT(
				&bsp->vertices,
				edge->vertex_indices[reverse],
				struct collision_vertex);
			q = TAG_BLOCK_GET_ELEMENT(
				&bsp->vertices,
				edge->vertex_indices[!owner],
				struct collision_vertex);
			vector_from_points3d(&anchor->point, &p->point, &p_vector);
			vector_from_points3d(&anchor->point, &q->point, &q_vector);
			cross_product3d(&p_vector, &q_vector, &cross);
			area += dot_product3d(&cross, &plane.n);

			edge = TAG_BLOCK_GET_ELEMENT(
				&bsp->edges,
				edge->edge_indices[reverse],
				struct collision_edge);
			owner = edge->surface_indices[1] == surface_index;
			reverse = owner;
		}
		while (edge->edge_indices[reverse] != surface->first_edge_index);

		if (area > 0.f)
		{
			return area;
		}
	}

	return 0.f;
}

real_point3d *collision_surface_project_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_point3d *result)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
		&bsp->bsp3d.planes,
		surface->plane_designator & LONG_MAX,
		real_plane3d);

	project_point2d(point, plane, projection, sign, result);
	return result;
}

boolean collision_surface_test_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	long const first_edge_index = surface->first_edge_index;
	long edge_index = first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);
		real_point2d point0;
		real_point2d point1;
		real_vector2d vector0;
		real_vector2d vector1;

		project_point3d(&vertex0->point, projection, sign, &point0);
		project_point3d(&vertex1->point, projection, sign, &point1);
		vector_from_points2d(&point0, point, &vector0);
		vector_from_points2d(&point1, point, &vector1);

		if (cross_product2d(&vector0, &vector1) > 0.f)
		{
			return FALSE;
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return TRUE;
}

boolean collision_surface_find_closest_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_point2d *result)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	long first_edge_index = surface->first_edge_index;
	long edge_index = first_edge_index;
	boolean previous_before;
	boolean previous_after;
	boolean first_before;
	boolean first_after;
	boolean before;
	boolean after;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *origin = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *target = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);
		real_point2d origin2d;
		real_point2d target2d;
		real_vector2d edge_vector;
		real_vector2d point_vector;
		real dot;

		before = FALSE;
		after = FALSE;
		project_point3d(&origin->point, projection, sign, &origin2d);
		project_point3d(&target->point, projection, sign, &target2d);
		vector_from_points2d(&origin2d, point, &point_vector);
		vector_from_points2d(&origin2d, &target2d, &edge_vector);

		if (cross_product2d(&point_vector, &edge_vector) > 0.f)
		{
			dot = dot_product2d(&point_vector, &edge_vector);
			if (dot < 0.f)
			{
				before = TRUE;
			}
			else
			{
				real length_squared = magnitude_squared2d(&edge_vector);

				if (dot > length_squared)
				{
					after = TRUE;
				}
				else
				{
					point_from_line2d(
						&origin2d,
						&edge_vector,
						dot/length_squared,
						result);
					return FALSE;
				}
			}
		}

		if (edge_index != first_edge_index)
		{
			if ((previous_after && (before || !after)) ||
				(before && (previous_after || !previous_before)))
			{
				*result = origin2d;
				return FALSE;
			}
		}
		else
		{
			first_before = before;
			first_after = after;
		}

		previous_before = before;
		previous_after = after;
		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	if ((after && (first_before || !first_after)) ||
		(first_before && (after || !before)))
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *origin = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);

		project_point3d(&origin->point, projection, sign, result);
		return FALSE;
	}

	*result = *point;
	return TRUE;
}

boolean collision_surface_test_line2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_vector2d const *direction,
	struct collision_surface_test_line2d_result *result)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	long const first_edge_index = surface->first_edge_index;
	long edge_index = first_edge_index;

	result->enter_t = REAL_MIN;
	result->enter_edge_index = NONE;
	result->enter_surface_index = NONE;
	result->exit_t = REAL_MAX;
	result->exit_edge_index = NONE;
	result->exit_surface_index = NONE;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[0],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[1],
			struct collision_vertex);
		real_vector2d edge_vector;
		real_vector2d point_vector;
		volatile real edge_cross;
		real point_cross;

		edge_vector.i = vertex1->point.x-vertex0->point.x;
		edge_vector.j = vertex1->point.y-vertex0->point.y;
		point_vector.i = point->x-vertex0->point.x;
		point_vector.j = point->y-vertex0->point.y;
		edge_cross = cross_product2d(direction, &edge_vector);
		point_cross = cross_product2d(&edge_vector, &point_vector);

		if (edge_cross != 0.f)
		{
			point_cross /= edge_cross;
			if ((edge_cross < 0.f) != reverse)
			{
				if (point_cross > result->enter_t)
				{
					result->enter_t = point_cross;
					result->enter_edge_index = edge_index;
					result->enter_surface_index = edge->surface_indices[!reverse];
				}
			}
			else if (point_cross < result->exit_t)
			{
				result->exit_t = point_cross;
				result->exit_edge_index = edge_index;
				result->exit_surface_index = edge->surface_indices[!reverse];
			}
		}
		else if ((point_cross < 0.f) != reverse)
		{
			result->enter_t = REAL_MAX;
			result->enter_edge_index = edge_index;
			result->enter_surface_index = edge->surface_indices[!reverse];
			result->exit_t = REAL_MIN;
			result->exit_edge_index = edge_index;
			result->exit_surface_index = edge->surface_indices[!reverse];
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != first_edge_index);

	return result->enter_t > result->exit_t;
}

boolean collision_bsp_test_pill_new(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	real *t,
	real_vector3d *normal)
{
	struct collision_bsp_test_pill_new_context context;

	context.bsp = bsp;
	context.breakable_surface_count = breakable_surface_count;
	context.breakable_surface_flags = breakable_surface_flags;
	context.point = point;
	context.vector = vector;
	context.radius = radius;
	context.t = t;
	context.normal = normal;
	context.last_leaf_index = NONE;
	context.last_contents = _contents_unknown;
	context.last_plane_designator = NONE;
	*t = REAL_MAX;

	return collision_bsp_test_pill_new_recursive(&context, 0, 0.f, 1.f);
}

boolean collision_bsp_test_pill(
	struct collision_bsp const *bsp,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	real maximum_t,
	struct collision_bsp_test_pill_result *result)
{
	struct collision_bsp_test_pill_context context;

	context.bsp = bsp;
	context.point = point;
	context.vector = vector;
	context.radius = radius;
	context.result = result;
	context.stack_depth = 0;
	result->t = maximum_t < 0.f ? 0.f : maximum_t;
	result->leaf_count = 0;

	return bsp3d_test_pill_recursive(&context, 0);
}

boolean collision_bsp_test_sphere(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *center,
	real radius,
	struct collision_bsp_test_sphere_result *result)
{
	struct collision_bsp_test_sphere_context context;
	short collision_function = 6 + (bsp == global_collision_bsp);

	collision_log_usage(collision_function);
	collision_log_start_time(&collision_bsp_usage_times.sphere);

	context.bsp = bsp;
	context.breakable_surface_count = breakable_surface_count;
	context.breakable_surface_flags = breakable_surface_flags;
	context.center = center;
	context.radius = radius;
	context.result = result;
	context.stack_depth = 0;
	result->leaf_count = 0;
	result->surface_count = 0;
	result->edge_count = 0;
	result->vertex_count = 0;
#ifdef HALO_N64
	n64_feature_hash_begin();
#endif

	bsp3d_test_sphere_recursive(&context, 0);
	collision_log_end_time(
		collision_function,
		collision_bsp_usage_times.sphere.QuadPart);

	return result->surface_count > 0 || result->edge_count > 0;
}

boolean collision_bsp_test_vector(
	unsigned long flags,
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real maximum_t,
	struct collision_bsp_test_vector_result *result)
{
	struct collision_bsp_test_vector_context context;
	short collision_function = 4 + (bsp == global_collision_bsp);
	boolean return_value;
	real t;

	collision_log_usage(collision_function);
	collision_log_start_time(&collision_bsp_usage_times.vector);

	context.flags = flags;
	context.bsp = bsp;
	context.breakable_surface_count = breakable_surface_count;
	context.breakable_surface_flags = breakable_surface_flags;
	context.point = point;
	context.vector = vector;
	context.result = result;
	result->t = maximum_t < 0.f ? 0.f : maximum_t;
	context.last_leaf_index = NONE;
	context.last_plane_index = NONE;
	result->leaf_count = 0;
	context.last_contents = _contents_unknown;

	t = PIN(maximum_t, 0.f, 1.f);
	return_value = collision_bsp_test_vector_recursive(&context, 0, 0.f, t);
	collision_log_end_time(
		collision_function,
		collision_bsp_usage_times.vector.QuadPart);

	return return_value;
}

/* ---------- private code */

#ifdef HALO_N64
/* A sphere test lists each vertex, edge and surface once, and add_feature
read the whole list to find out whether one was there already: thousands of
compares for a biped's pill every tick, as each surface is reached from
several leaves and each vertex and edge from several surfaces. A hash of the
entries answers instead (stamped with the test, so it never needs clearing),
and a surface already listed is not walked again: its vertices and edges were
listed with it, as the same sphere hits them again. The lists come out the
same, in the same order. */
enum
{
	N64_FEATURE_HASH_BITS = 10, /* the three lists hold 768 at most */
	N64_FEATURE_HASH_SIZE = 1 << N64_FEATURE_HASH_BITS,
};

static unsigned long n64_feature_hash[N64_FEATURE_HASH_SIZE]; /* stamp << 16 | position + 1 */
static unsigned long n64_feature_stamp;

static void n64_feature_hash_begin(
	void)
{
	n64_feature_stamp += 0x10000;
	if (!n64_feature_stamp)
	{
		csmemset(n64_feature_hash, 0, sizeof(n64_feature_hash));
		n64_feature_stamp = 0x10000;
	}

	return;
}

/* the slot holding the index in the list, or the free one ending its run */
static unsigned long *n64_feature_slot(
	long const *count,
	long const *indices,
	long index)
{
	unsigned long slot =
		(((unsigned long)index ^ ((unsigned long)indices >> 4)) * 2654435761UL) >>
		(32 - N64_FEATURE_HASH_BITS);

	for (;;)
	{
		unsigned long entry = n64_feature_hash[slot];
		long position = (long)(entry & 0xFFFF) - 1;

		if ((entry & 0xFFFF0000) != n64_feature_stamp ||
			(position < *count && indices[position] == index))
		{
			return &n64_feature_hash[slot];
		}
		slot = (slot + 1) & (N64_FEATURE_HASH_SIZE - 1);
	}
}

static void add_feature(
	long *count,
	long *indices,
	long index)
{
	unsigned long *slot = n64_feature_slot(count, indices, index);

	if ((*slot & 0xFFFF0000) != n64_feature_stamp && *count < 256)
	{
		indices[*count] = index;
		(*count)++;
		*slot = n64_feature_stamp | (unsigned long)*count;
	}

	return;
}
#else
static void add_feature(
	long *count,
	long *indices,
	long index)
{
	short feature_index;

	for (feature_index = 0; feature_index < *count; feature_index++)
	{
		if (indices[feature_index] == index)
			return;
	}

	if (*count < 256)
	{
		indices[*count] = index;
		(*count)++;
	}

	return;
}
#endif

static void collision_surface_test_sphere(
	struct collision_bsp_test_sphere_context *data,
	long surface_index)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&data->bsp->surfaces,
		surface_index,
		struct collision_surface);
	byte breakable_surface_index;
	long edge_index;
	real radius_squared;
	boolean hit_feature = FALSE;

#ifdef HALO_N64
	if ((*n64_feature_slot(
		&data->result->surface_count,
		data->result->surface_indices,
		surface_index) & 0xFFFF0000) == n64_feature_stamp)
	{
		return;
	}
#endif
	if (TEST_FLAG(surface->flags, _collision_surface_breakable_bit))
	{
		breakable_surface_index = surface->breakable_surface_index;
		if (breakable_surface_index < data->breakable_surface_count &&
			!BIT_VECTOR_TEST_FLAG(
				(long const *)data->breakable_surface_flags,
				breakable_surface_index))
		{
			return;
		}
	}

	radius_squared = data->radius * data->radius;
	edge_index = surface->first_edge_index;
	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->edges,
			edge_index,
			struct collision_edge);
		boolean reverse = edge->surface_indices[1] == surface_index;
		long vertex_index = edge->vertex_indices[reverse];
		struct collision_vertex const *vertex = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->vertices,
			vertex_index,
			struct collision_vertex);
		real delta_x = vertex->point.x - data->center->x;
		real delta_y = vertex->point.y - data->center->y;
		real delta_z = vertex->point.z - data->center->z;
		real distance_squared =
			delta_x * delta_x +
			delta_y * delta_y +
			delta_z * delta_z;

		if (distance_squared <= radius_squared)
		{
			add_feature(
				&data->result->vertex_count,
				data->result->vertex_indices,
				vertex_index);
			hit_feature = TRUE;
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != surface->first_edge_index);

	edge_index = surface->first_edge_index;
	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->edges,
			edge_index,
			struct collision_edge);
		boolean reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);
		real_vector3d edge_vector;

		vector_from_points3d(&vertex0->point, &vertex1->point, &edge_vector);
		if (fast_vector_intersects_sphere(
			&vertex0->point,
			&edge_vector,
			data->center,
			data->radius))
		{
			add_feature(
				&data->result->edge_count,
				data->result->edge_indices,
				edge_index);
			hit_feature = TRUE;
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != surface->first_edge_index);

	if (!hit_feature)
	{
		edge_index = surface->first_edge_index;
		do
		{
			struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
				&data->bsp->edges,
				edge_index,
				struct collision_edge);
			boolean reverse = edge->surface_indices[1] == surface_index;
			struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
				&data->bsp->vertices,
				edge->vertex_indices[reverse],
				struct collision_vertex);
			struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
				&data->bsp->vertices,
				edge->vertex_indices[!reverse],
				struct collision_vertex);
			real_point2d point0;
			real_point2d point1;
			real_vector2d vector0;
			real_vector2d vector1;

			project_point3d(
				&vertex0->point,
				data->projection_axis,
				data->projection_sign,
				&point0);
			project_point3d(
				&vertex1->point,
				data->projection_axis,
				data->projection_sign,
				&point1);
			vector_from_points2d(&data->center2d, &point0, &vector0);
			vector_from_points2d(&data->center2d, &point1, &vector1);
			if (cross_product2d(&vector0, &vector1) < 0.f)
				return;

			edge_index = edge->edge_indices[reverse];
		}
		while (edge_index != surface->first_edge_index);
	}

	add_feature(
		&data->result->surface_count,
		data->result->surface_indices,
		surface_index);

	return;
}

static boolean collision_surface_test_point(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point)
{
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	long edge_index;

	if (TEST_FLAG(surface->flags, _collision_surface_breakable_bit))
	{
		byte breakable_surface_index = surface->breakable_surface_index;

		if (breakable_surface_index < breakable_surface_count &&
			!BIT_VECTOR_TEST_FLAG(
				(long const *)breakable_surface_flags,
				breakable_surface_index))
		{
			return FALSE;
		}
	}

	edge_index = surface->first_edge_index;
	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);
		real_point2d point0;
		real_point2d point1;
		real_vector2d vector0;
		real_vector2d vector1;

		project_point3d(&vertex0->point, projection, sign, &point0);
		project_point3d(&vertex1->point, projection, sign, &point1);
		vector_from_points2d(&point0, point, &vector0);
		vector_from_points2d(&point0, &point1, &vector1);

		if (cross_product2d(&vector0, &vector1) > 0.f)
		{
			return FALSE;
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != surface->first_edge_index);

	return TRUE;
}

static boolean sphere_test_vector(
	real_point3d const *center,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t)
{
	real_vector3d delta;
	real outside;
	real projection;
	real length_squared;
	real discriminant;
	real hit_t;

	vector_from_points3d(point, center, &delta);
	outside = magnitude_squared3d(&delta) - radius*radius;
	if (outside <= 0.f)
	{
		*t = 0.f;
		return TRUE;
	}

	projection = dot_product3d(&delta, vector);
	if (projection > 0.f)
	{
		length_squared = magnitude_squared3d(vector);
		discriminant = projection*projection - length_squared*outside;
		if (discriminant >= 0.f)
		{
			hit_t = (projection - square_root(discriminant))/length_squared;
			if (hit_t <= 1.f)
			{
				*t = hit_t;
				return TRUE;
			}
		}
	}

	return FALSE;
}

static boolean collision_bsp_test_pill_new_recursive(
	struct collision_bsp_test_pill_new_context *data,
	long node_index,
	real t0,
	real t1)
{
	while (!(node_index & LONG_MIN))
	{
		struct bsp3d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.nodes,
			node_index,
			struct bsp3d_node);
		real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.planes,
			node->plane_designator,
			real_plane3d);
		real distance = plane3d_distance_to_point(plane, data->point);
		real dot = dot_product3d(data->vector, &plane->n);
		real distance0 = dot*t0 + distance;
		real distance1 = dot*t1 + distance;
		boolean reaches_back =
			distance0 < data->radius || distance1 < data->radius;
		boolean reaches_front =
			distance0 > -data->radius || distance1 > -data->radius;

		if (reaches_back && reaches_front)
		{
			boolean front = dot > 0.f;
			real near_t;
			real far_t;
			boolean near_hit;

			if (dot != 0.f)
			{
				real inverse = 1.f/dot;
				real back_t = -((distance + data->radius)*inverse);
				real front_t = -((distance - data->radius)*inverse);

				near_t = MAX(back_t, front_t);
				far_t = MIN(back_t, front_t);
				near_t = PIN(near_t, t0, t1);
				far_t = PIN(far_t, t0, t1);
			}
			else
			{
				near_t = t1;
				far_t = t0;
			}

			near_hit = collision_bsp_test_pill_new_recursive(
				data,
				BSP_FLAGGED_INDEX(node->children[!front]),
				t0,
				near_t);
			if (near_hit)
			{
				if (far_t >= *data->t)
				{
					return TRUE;
				}
				t1 = *data->t;
			}
			data->last_plane_designator = front ?
				(node->plane_designator | LONG_MIN) :
				(node->plane_designator & LONG_MAX);

			near_hit |= collision_bsp_test_pill_new_recursive(
				data,
				BSP_FLAGGED_INDEX(node->children[front]),
				far_t,
				t1);

			return near_hit;
		}

		node_index = BSP_FLAGGED_INDEX(node->children[reaches_front]);
	}

	if (node_index == NONE && data->last_plane_designator != NONE)
	{
		real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.planes,
			data->last_plane_designator & LONG_MAX,
			real_plane3d);

		*data->t = t0;
		if (data->last_plane_designator & LONG_MIN)
		{
			negate_vector3d(&plane->n, data->normal);
		}
		else
		{
			*data->normal = plane->n;
		}

		return TRUE;
	}

	return FALSE;
}

static void bsp2d_test_sphere_recursive(
	struct collision_bsp_test_sphere_context *data,
	long child_index)
{
	while (!(child_index & LONG_MIN))
	{
		struct bsp2d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp2d.nodes,
			child_index,
			struct bsp2d_node);
		real distance = plane2d_distance_to_point(&node->plane, &data->center2d);
		boolean reaches_first_child = distance <= data->radius;
		boolean reaches_second_child = distance >= -data->radius;

		if (reaches_first_child)
			bsp2d_test_sphere_recursive(data, node->child_indices[0]);
		if (!reaches_second_child)
			return;

		child_index = node->child_indices[1];
	}

	collision_surface_test_sphere(data, child_index & LONG_MAX);

	return;
}

static long collision_leaf_test_vector(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long leaf_index,
	long plane_index,
	real t,
	boolean test_surface)
{
	struct collision_leaf const *leaf = TAG_BLOCK_GET_ELEMENT(
		&bsp->leaves,
		leaf_index,
		struct collision_leaf);
	long reference_index;

	for (
		reference_index = leaf->first_bsp2d_reference_index;
		reference_index <
			leaf->first_bsp2d_reference_index + leaf->bsp2d_reference_count;
		reference_index++)
	{
		struct bsp2d_reference const *reference = TAG_BLOCK_GET_ELEMENT(
			&bsp->bsp2d_references,
			reference_index,
			struct bsp2d_reference);

		if ((BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MAX) == plane_index)
		{
			real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
				&bsp->bsp3d.planes,
				plane_index,
				real_plane3d);
			real absolute_i = fabs(plane->n.i);
			real absolute_j = fabs(plane->n.j);
			real absolute_k = fabs(plane->n.k);
			short projection;
			boolean projection_sign;
			real_point3d hit_point;
			real_point2d point2d;
			long surface_index;

			if (absolute_k >= absolute_j && absolute_k >= absolute_i)
				projection = _z;
			else
				projection = absolute_j >= absolute_i ? _y : _x;
			projection_sign =
				projection_sign_from_vector3d(&plane->n, projection) !=
				(BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MIN ? TRUE : FALSE);
			hit_point.x = vector->i*t + point->x;
			hit_point.y = vector->j*t + point->y;
			hit_point.z = vector->k*t + point->z;
			project_point3d(&hit_point, projection, projection_sign, &point2d);
			surface_index = bsp2d_test_point(
				&bsp->bsp2d.nodes,
				&point2d,
				BSP_FLAGGED_INDEX(reference->root_index));
			if (!test_surface ||
				collision_surface_test_point(
					bsp,
					breakable_surface_count,
					breakable_surface_flags,
					surface_index,
					projection,
					projection_sign,
					&point2d))
			{
				return surface_index;
			}
		}
	}

	return NONE;
}

static boolean pill_test_vector(
	real_point3d const *base,
	real_vector3d const *edge,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t,
	real *edge_t)
{
	real_vector3d delta;
	real edge_length_squared;
	real edge_dot_vector;
	real vector_length_squared;
	real denominator;
	real delta_dot_edge;
	real delta_dot_vector;
	real linear_coefficient;
	real constant_coefficient;
	real discriminant;
	real root;
	real inverse;
	real t0;
	real t1;
	real hit_t;
	real edge_projection;

	vector_from_points3d(base, point, &delta);
	edge_length_squared = magnitude_squared3d(edge);
	edge_dot_vector = dot_product3d(edge, vector);
	vector_length_squared = magnitude_squared3d(vector);
	denominator = vector_length_squared*edge_length_squared -
		edge_dot_vector*edge_dot_vector;
	if (denominator != 0.f)
	{
		delta_dot_edge = dot_product3d(&delta, edge);
		delta_dot_vector = dot_product3d(&delta, vector);
		linear_coefficient = delta_dot_edge*edge_dot_vector -
			delta_dot_vector*edge_length_squared;
		constant_coefficient =
			(magnitude_squared3d(&delta) - radius*radius)*edge_length_squared -
			delta_dot_edge*delta_dot_edge;
		discriminant = linear_coefficient*linear_coefficient -
			constant_coefficient*denominator;
		if (discriminant >= 0.f)
		{
			root = square_root(discriminant);
			inverse = 1.f/denominator;
			t0 = -((root + linear_coefficient)*inverse);
			if (t0 <= 1.f)
			{
				t1 = -((linear_coefficient - root)*inverse);
				if (t1 >= 0.f)
				{
					hit_t = FLOOR(t0, 0.f);
					edge_projection = edge_dot_vector*hit_t + delta_dot_edge;
					if (edge_projection < 0.f)
					{
						if (sphere_test_vector(base, radius, point, vector, t))
						{
							*edge_t = 0.f;
							return TRUE;
						}
					}
					else if (edge_projection > edge_length_squared)
					{
						real_point3d end;

						end.x = base->x + edge->i;
						end.y = base->y + edge->j;
						end.z = base->z + edge->k;
						if (sphere_test_vector(&end, radius, point, vector, t))
						{
							*edge_t = 1.f;
							return TRUE;
						}
					}
					else
					{
						*t = hit_t;
						*edge_t = edge_projection/edge_length_squared;
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

static void bsp3d_test_sphere_recursive(
	struct collision_bsp_test_sphere_context *data,
	long node_index)
{
	while (!(node_index & LONG_MIN))
	{
		struct bsp3d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.nodes,
			node_index,
			struct bsp3d_node);
		real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.planes,
			node->plane_designator,
			real_plane3d);
		real distance = plane3d_distance_to_point(plane, data->center);
		boolean reaches_second_child = distance < data->radius;
		boolean child_index = distance > -data->radius;

		if (child_index && reaches_second_child)
		{
			match_assert(
				"c:\\halo\\SOURCE\\physics\\collision_bsp.c",
				0x206,
				data->stack_depth>=0 && data->stack_depth<MAXIMUM_BSP3D_DEPTH);
			data->plane_stack[data->stack_depth++] =
				node->plane_designator | LONG_MIN;
			bsp3d_test_sphere_recursive(data, BSP_FLAGGED_INDEX(node->children[0]));

			data->stack_depth--;
			match_assert(
				"c:\\halo\\SOURCE\\physics\\collision_bsp.c",
				0x210,
				data->stack_depth>=0 && data->stack_depth<MAXIMUM_BSP3D_DEPTH);
			data->plane_stack[data->stack_depth++] =
				node->plane_designator & LONG_MAX;
			bsp3d_test_sphere_recursive(data, BSP_FLAGGED_INDEX(node->children[1]));
			data->stack_depth--;

			return;
		}

		node_index = BSP_FLAGGED_INDEX(node->children[child_index]);
	}

	if (node_index != NONE)
	{
		long leaf_index = node_index & LONG_MAX;
		struct collision_leaf const *leaf = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->leaves,
			leaf_index,
			struct collision_leaf);
		long reference_index;

		if (data->result->leaf_count < 256)
		{
			data->result->leaf_indices[data->result->leaf_count] = leaf_index;
			data->result->leaf_count++;
		}

		for (
			reference_index = leaf->first_bsp2d_reference_index;
			reference_index <
				leaf->first_bsp2d_reference_index + leaf->bsp2d_reference_count;
			reference_index++)
		{
			struct bsp2d_reference const *reference = TAG_BLOCK_GET_ELEMENT(
				&data->bsp->bsp2d_references,
				reference_index,
				struct bsp2d_reference);
			short stack_index;

			for (stack_index = 0; stack_index < data->stack_depth; stack_index++)
			{
				if (data->plane_stack[stack_index] == BSP_FLAGGED_INDEX(reference->plane_designator))
				{
					real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
						&data->bsp->bsp3d.planes,
						BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MAX,
						real_plane3d);
					real plane_distance = -plane3d_distance_to_point(plane, data->center);
					real_point3d projected_center;
					real absolute_i;
					real absolute_j;
					real absolute_k;
					short projection;

					projected_center.x =
						plane->n.i * plane_distance + data->center->x;
					projected_center.y =
						plane->n.j * plane_distance + data->center->y;
					projected_center.z =
						plane->n.k * plane_distance + data->center->z;
					absolute_i = fabs(plane->n.i);
					absolute_j = fabs(plane->n.j);
					absolute_k = fabs(plane->n.k);
					if (absolute_k >= absolute_j && absolute_k >= absolute_i)
						projection = _z;
					else
						projection = absolute_j >= absolute_i ? _y : _x;
					data->projection_axis = projection;
					data->projection_sign =
						projection_sign_from_vector3d(&plane->n, projection) !=
						(BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MIN ? TRUE : FALSE);
					project_point3d(
						&projected_center,
						data->projection_axis,
						data->projection_sign,
						&data->center2d);
					bsp2d_test_sphere_recursive(data, BSP_FLAGGED_INDEX(reference->root_index));
					break;
				}
			}
		}
	}

	return;
}

static boolean collision_bsp_test_vector_recursive(
	struct collision_bsp_test_vector_context *data,
	long node_index,
	real t0,
	real t1)
{
	if (!(node_index & LONG_MIN))
	{
		struct bsp3d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.nodes,
			node_index,
			struct bsp3d_node);
		real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.planes,
			node->plane_designator,
			real_plane3d);
		real distance = plane3d_distance_to_point(plane, data->point);
		real dot = dot_product3d(data->vector, &plane->n);
		real distance0 = dot*t0 + distance;
		real distance1 = dot*t1 + distance;
		boolean reaches_back = distance0 < 0.f || distance1 < 0.f;
		boolean reaches_front = distance0 >= 0.f || distance1 >= 0.f;

		if (reaches_back && reaches_front)
		{
			boolean front = dot > 0.f;
			real t = -(distance/dot);

			if (collision_bsp_test_vector_recursive(
				data,
				BSP_FLAGGED_INDEX(node->children[!front]),
				t0,
				t))
			{
				return TRUE;
			}
			if (data->result->t <= t)
			{
				return FALSE;
			}
			data->last_plane_index = node->plane_designator;
			if (collision_bsp_test_vector_recursive(
				data,
				BSP_FLAGGED_INDEX(node->children[front]),
				t,
				t1))
			{
				return TRUE;
			}
		}
		else if (collision_bsp_test_vector_recursive(
			data,
			BSP_FLAGGED_INDEX(node->children[reaches_front]),
			t0,
			t1))
		{
			return TRUE;
		}
	}
	else
	{
		long leaf_index = NONE;
		byte contents = _contents_solid;
		boolean test_surface = FALSE;
		long test_leaf_index;

		if (node_index != NONE)
		{
			leaf_index = node_index & LONG_MAX;
			contents = TEST_FLAG(
				TAG_BLOCK_GET_ELEMENT(
					&data->bsp->leaves,
					leaf_index,
					struct collision_leaf)->flags,
				_collision_leaf_contains_two_sided_bit) ?
				_contents_semi_empty : _contents_empty;
		}

		if (TEST_FLAG(data->flags, _collision_test_front_facing_surfaces_bit) &&
			(data->last_contents == _contents_empty ||
				data->last_contents == _contents_semi_empty) &&
			contents == _contents_solid)
		{
			test_leaf_index = data->last_leaf_index;
		}
		else if (TEST_FLAG(data->flags, _collision_test_back_facing_surfaces_bit) &&
			data->last_contents == _contents_solid &&
			(contents == _contents_empty || contents == _contents_semi_empty))
		{
			test_leaf_index = leaf_index;
		}
		else if (!TEST_FLAG(data->flags, _collision_test_ignore_two_sided_surfaces_bit) &&
			data->last_contents == _contents_semi_empty &&
			contents == _contents_semi_empty)
		{
			test_leaf_index =
				TEST_FLAG(data->flags, _collision_test_front_facing_surfaces_bit) ?
				data->last_leaf_index : leaf_index;
			test_surface = TRUE;
		}
		else
		{
			test_leaf_index = NONE;
		}

		if (test_leaf_index != NONE)
		{
			long surface_index = collision_leaf_test_vector(
				data->bsp,
				data->breakable_surface_count,
				data->breakable_surface_flags,
				data->point,
				data->vector,
				test_leaf_index,
				data->last_plane_index,
				t0,
				test_surface);

			if (surface_index != NONE)
			{
				struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
					&data->bsp->surfaces,
					surface_index,
					struct collision_surface);

				if ((!TEST_FLAG(surface->flags, _collision_surface_invisible_bit) ||
						!TEST_FLAG(data->flags, _collision_test_ignore_invisible_surfaces_bit)) &&
					(!TEST_FLAG(surface->flags, _collision_surface_breakable_bit) ||
						!TEST_FLAG(data->flags, _collision_test_ignore_breakable_surfaces_bit)))
				{
					data->result->t = t0;
					data->result->plane = TAG_BLOCK_GET_ELEMENT(
						&data->bsp->bsp3d.planes,
						data->last_plane_index,
						real_plane3d);
					data->result->surface_index = surface_index;
					data->result->plane_designator = surface->plane_designator;
					data->result->flags = surface->flags;
					data->result->breakable_surface_index =
						surface->breakable_surface_index;
					data->result->material_index = surface->material_index;

					return TRUE;
				}
			}
		}

		if (leaf_index != NONE)
		{
			if (data->result->leaf_count < 256)
			{
				data->result->leaf_indices[data->result->leaf_count] = leaf_index;
				data->result->leaf_count++;
			}
			else
			{
				data->result->leaf_indices[255] = leaf_index;
			}
		}
		data->last_leaf_index = leaf_index;
		data->last_contents = contents;
	}

	return FALSE;
}

static boolean collision_surface_test_pill(
	struct collision_bsp_test_pill_context *data,
	long surface_index)
{
	boolean hit = FALSE;
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&data->bsp->surfaces,
		surface_index,
		struct collision_surface);
	long edge_index = surface->first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->edges,
			edge_index,
			struct collision_edge);
		boolean const reverse = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->vertices,
			edge->vertex_indices[reverse],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->vertices,
			edge->vertex_indices[!reverse],
			struct collision_vertex);
		real_vector3d edge_vector;
		real t;
		real edge_t;

		vector_from_points3d(&vertex0->point, &vertex1->point, &edge_vector);
		if (pill_test_vector(
				&vertex0->point,
				&edge_vector,
				data->radius,
				data->point,
				data->vector,
				&t,
				&edge_t) &&
			data->result->t > t)
		{
			real_point3d edge_point;
			real_point3d pill_point;

			data->result->t = t;
			edge_point.x = edge_vector.i*edge_t + vertex0->point.x;
			edge_point.y = edge_vector.j*edge_t + vertex0->point.y;
			edge_point.z = edge_vector.k*edge_t + vertex0->point.z;
			pill_point.x = data->vector->i*t + data->point->x;
			pill_point.y = data->vector->j*t + data->point->y;
			pill_point.z = data->vector->k*t + data->point->z;
			vector_from_points3d(&edge_point, &pill_point, &data->result->plane.n);
			{
				real_vector3d *normal = &data->result->plane.n;
				real magnitude = square_root(
					normal->i*normal->i +
					normal->j*normal->j +
					normal->k*normal->k);

				if (!(_real_epsilon > fabs(magnitude)))
				{
					real inverse_magnitude = 1.f/magnitude;

					normal->i = inverse_magnitude*normal->i;
					normal->j = inverse_magnitude*normal->j;
					normal->k = inverse_magnitude*normal->k;
				}
			}
			data->result->plane.d = REAL_MAX;
			data->result->surface_index = surface_index;
			data->result->material_index = surface->material_index;
			hit = TRUE;
		}

		edge_index = edge->edge_indices[reverse];
	}
	while (edge_index != surface->first_edge_index);

	return hit;
}

static boolean bsp2d_test_pill_recursive(
	struct collision_bsp_test_pill_context *data,
	long child_index)
{
	boolean hit = FALSE;

	if (!(child_index & LONG_MIN))
	{
		struct bsp2d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp2d.nodes,
			child_index,
			struct bsp2d_node);
		real distance0 = plane2d_distance_to_point(&node->plane, &data->point2d);
		real distance1 =
			node->plane.n.i*data->vector2d.i +
			node->plane.n.j*data->vector2d.j +
			distance0;
		boolean reaches_back =
			distance0 <= data->radius + BSP2D_TEST_PILL_EPSILON ||
			distance1 <= data->radius + BSP2D_TEST_PILL_EPSILON;
		boolean reaches_front =
			distance0 >= -data->radius - BSP2D_TEST_PILL_EPSILON ||
			distance1 >= -data->radius - BSP2D_TEST_PILL_EPSILON;

		if ((reaches_back &&
				bsp2d_test_pill_recursive(data, node->child_indices[0])) ||
			(reaches_front &&
				bsp2d_test_pill_recursive(data, node->child_indices[1])))
		{
			hit = TRUE;
		}
	}
	else if (collision_surface_test_pill(data, child_index & LONG_MAX))
	{
		hit = TRUE;
	}

	return hit;
}

static boolean bsp3d_test_pill_recursive(
	struct collision_bsp_test_pill_context *data,
	long node_index)
{
	boolean hit = FALSE;

	if (!(node_index & LONG_MIN))
	{
		struct bsp3d_node const *node = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.nodes,
			node_index,
			struct bsp3d_node);
		real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->bsp3d.planes,
			node->plane_designator,
			real_plane3d);
		real distance0 = plane3d_distance_to_point(plane, data->point);
		real dot = dot_product3d(data->vector, &plane->n);
		real distance1 = dot + distance0;
		boolean reaches_back =
			distance0 <= data->radius + BSP3D_TEST_PILL_EPSILON ||
			distance1 <= data->radius + BSP3D_TEST_PILL_EPSILON;
		boolean reaches_front =
			distance0 >= -data->radius - BSP3D_TEST_PILL_EPSILON ||
			distance1 >= -data->radius - BSP3D_TEST_PILL_EPSILON;

		if (reaches_back && reaches_front)
		{
			boolean front = dot > 0.f;

			match_assert(
				"c:\\halo\\SOURCE\\physics\\collision_bsp.c",
				0x498,
				data->stack_depth>=0 && data->stack_depth<MAXIMUM_BSP3D_DEPTH);
			data->plane_stack[data->stack_depth++] = front ?
				(node->plane_designator | LONG_MIN) :
				(node->plane_designator & LONG_MAX);
			if (bsp3d_test_pill_recursive(data, BSP_FLAGGED_INDEX(node->children[!front])))
			{
				hit = TRUE;
			}
			data->stack_depth--;
			if (bsp3d_test_pill_recursive(data, BSP_FLAGGED_INDEX(node->children[front])))
			{
				hit = TRUE;
			}
		}
		else if (bsp3d_test_pill_recursive(data, BSP_FLAGGED_INDEX(node->children[reaches_front])))
		{
			hit = TRUE;
		}
	}
	else if (node_index != NONE)
	{
		long leaf_index = node_index & LONG_MAX;
		struct collision_leaf const *leaf = TAG_BLOCK_GET_ELEMENT(
			&data->bsp->leaves,
			leaf_index,
			struct collision_leaf);
		long reference_index;

		for (
			reference_index = leaf->first_bsp2d_reference_index;
			reference_index <
				leaf->first_bsp2d_reference_index + leaf->bsp2d_reference_count;
			reference_index++)
		{
			struct bsp2d_reference const *reference = TAG_BLOCK_GET_ELEMENT(
				&data->bsp->bsp2d_references,
				reference_index,
				struct bsp2d_reference);
			short stack_index;

			for (stack_index = 0; stack_index < data->stack_depth; stack_index++)
			{
				if (data->plane_stack[stack_index] == BSP_FLAGGED_INDEX(reference->plane_designator))
				{
					real_plane3d const *plane = TAG_BLOCK_GET_ELEMENT(
						&data->bsp->bsp3d.planes,
						BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MAX,
						real_plane3d);
					real distance0 = plane3d_distance_to_point(plane, data->point);
					real dot = dot_product3d(data->vector, &plane->n);
					real t = 0.f;

					if (dot != 0.f)
					{
						real inverse = 1.f/dot;

						t = -(inverse*distance0) - fabs(inverse)*data->radius;
						t = PIN(t, 0.f, 1.f);
					}

					if (data->result->t > t)
					{
						real absolute_i = fabs(plane->n.i);
						real absolute_j = fabs(plane->n.j);
						real absolute_k = fabs(plane->n.k);
						short projection;
						real_point3d pill_point;
						real plane_distance;
						real_point2d point2d;
						long surface_index;
						real_point3d start_point;
						real_point3d sweep_point;
						real negative_distance;
						real negative_dot;

						if (absolute_k >= absolute_j && absolute_k >= absolute_i)
							projection = _z;
						else
							projection = absolute_j >= absolute_i ? _y : _x;
						data->projection_axis = projection;
						data->projection_sign =
							projection_sign_from_vector3d(&plane->n, projection) !=
							(BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MIN ? TRUE : FALSE);
						pill_point.x = data->vector->i*t + data->point->x;
						pill_point.y = data->vector->j*t + data->point->y;
						pill_point.z = data->vector->k*t + data->point->z;
						plane_distance = -plane3d_distance_to_point(plane, &pill_point);
						pill_point.x = plane->n.i*plane_distance + pill_point.x;
						pill_point.y = plane->n.j*plane_distance + pill_point.y;
						pill_point.z = plane->n.k*plane_distance + pill_point.z;
						project_point3d(
							&pill_point,
							data->projection_axis,
							data->projection_sign,
							&point2d);
						surface_index = bsp2d_test_point(
							&data->bsp->bsp2d.nodes,
							&point2d,
							BSP_FLAGGED_INDEX(reference->root_index));
						if (collision_surface_test_point(
							data->bsp,
							0,
							NULL,
							surface_index,
							data->projection_axis,
							data->projection_sign,
							&point2d))
						{
							struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
								&data->bsp->surfaces,
								surface_index,
								struct collision_surface);

							data->result->t = t;
							if (BSP_FLAGGED_INDEX(reference->plane_designator) & LONG_MIN)
							{
								real_plane3d *result_plane = &data->result->plane;

								result_plane->n.i = -plane->n.i;
								result_plane->n.j = -plane->n.j;
								result_plane->n.k = -plane->n.k;
								result_plane->d = -plane->d;
							}
							else
							{
								data->result->plane = *plane;
							}
							data->result->surface_index = surface_index;
							data->result->material_index = surface->material_index;
							hit = TRUE;
						}

						negative_distance = -distance0;
						start_point.x = plane->n.i*negative_distance + data->point->x;
						start_point.y = plane->n.j*negative_distance + data->point->y;
						start_point.z = plane->n.k*negative_distance + data->point->z;
						project_point3d(
							&start_point,
							data->projection_axis,
							data->projection_sign,
							&data->point2d);
						negative_dot = -dot;
						sweep_point.x = plane->n.i*negative_dot + data->vector->i;
						sweep_point.y = plane->n.j*negative_dot + data->vector->j;
						sweep_point.z = plane->n.k*negative_dot + data->vector->k;
						project_point3d(
							&sweep_point,
							data->projection_axis,
							data->projection_sign,
							(real_point2d *)&data->vector2d);
						if (bsp2d_test_pill_recursive(data, BSP_FLAGGED_INDEX(reference->root_index)))
						{
							hit = TRUE;
						}
					}

					break;
				}
			}
		}

		if (data->result->leaf_count < 256)
		{
			data->result->leaf_indices[data->result->leaf_count] = leaf_index;
			data->result->leaf_count++;
		}
		else
		{
			data->result->leaf_indices[255] = leaf_index;
		}
	}

	return hit;
}
