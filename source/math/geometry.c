/*
GEOMETRY.C

symbols in this file:
000F4F00 00b0:
	_geosphere_dispose (0000)
000F4FB0 0010:
	_convex_hull3d_verify (0000)
000F4FC0 0020:
	_plane2d_distance_to_point (0000)
000F4FE0 0030:
	_vector_intersect_plane2d (0000)
000F5010 0040:
	_vector_intersect_plane3d (0000)
000F5050 0150:
	_code_000f5050 (0000)
000F51A0 0300:
	_build_torus (0000)
000F54A0 00a0:
	_code_000f54a0 (0000)
000F5540 0310:
	_convex_hull2d (0000)
000F5850 0100:
	_convex_hull2d_verify (0000)
000F5950 00d0:
	_convex_hull2d_test_circle (0000)
000F5A20 0090:
	_convex_hull2d_test_point (0000)
000F5AB0 00a0:
	_convex_hull2d_test_point_indexed (0000)
000F5B50 0060:
	_convex_hull2d_area (0000)
000F5BB0 0060:
	_convex_hull2d_perimeter (0000)
000F5C10 0120:
	_convex_hull2d_test_vector (0000)
000F5D30 03f0:
	_convex_polygon2d_clip_to_plane (0000)
000F6120 0060:
	_convex_polygon2d_verify (0000)
000F6180 0460:
	_convex_polygon3d_clip_to_plane (0000)
000F65E0 0190:
	_convex_polygon3d_verify (0000)
000F6770 05d0:
	_convex_hull3d_begin (0000)
000F6D40 0710:
	_convex_hull3d_expand (0000)
000F7450 0110:
	_convex_hull3d (0000)
000F7560 0070:
	_convex_hull3d_test_point (0000)
000F75D0 0110:
	_convex_hull3d_test_vector (0000)
000F76E0 01a0:
	_code_000f76e0 (0000)
000F7880 0210:
	_convex_hull2d_intersect (0000)
000F7A90 0190:
	_code_000f7a90 (0000)
000F7C20 03b0:
	_code_000f7c20 (0000)
000F7FD0 01f0:
	_geosphere_new (0000)
0027A2B8 0078:
	_rdata_0027a2b8 (0000)
0027A330 0026:
	??_C@_0CG@DDHFMONE@sphere?9?$DOtriangle_strip_vertex_in@ (0000)
0027A358 0011:
	??_C@_0BB@NLDBGHK@sphere?9?$DOvertices?$AA@ (0000)
0027A36C 0007:
	??_C@_06FOHEPPJH@sphere?$AA@ (0000)
0027A374 001f:
	??_C@_0BP@BJAGONOE@c?3?2halo?2SOURCE?2math?2geometry?4c?$AA@ (0000)
0027A394 0035:
	??_C@_0DF@KKNLNKMO@new_vertex?5?$DO?$DN0?5?$CG?$CG?5new_vertex?5?$DM?$DN?5@ (0000)
0027A3CC 002f:
	??_C@_0CP@CNADNFO@parent2?5?$DO?$DN0?5?$CG?$CG?5parent2?5?$DM?$DN?5sphere@ (0000)
0027A3FC 002f:
	??_C@_0CP@BDMOGHAE@parent1?5?$DO?$DN0?5?$CG?$CG?5parent1?5?$DM?$DN?5sphere@ (0000)
0027A42C 003f:
	??_C@_0DP@FFEHBALB@subdivision_index?5?$DO?50?5?$CG?$CG?5subdivi@ (0000)
0027A46C 0019:
	??_C@_0BJ@ENKKOHHL@cylinder_segment_count?$DO2?$AA@ (0000)
0027A488 0015:
	??_C@_0BF@GOKGNGOE@ring_segment_count?$DO2?$AA@ (0000)
0027A4A0 0015:
	??_C@_0BF@HMLCHDNC@start_vertex_index?$DO0?$AA@ (0000)
0027A4B8 002d:
	??_C@_0CN@HIIJMMI@start_vertex_index?$CLvertex_index?$DM@ (0000)
0027A4E8 001a:
	??_C@_0BK@MBDLNMGM@vertex_index?$DMvertex_count?$AA@ (0000)
0027A504 0021:
	??_C@_0CB@IMGLMJJD@count?$DO?$DN0?5?$CG?$CG?5count?$DM?$DNmaximum_count@ (0000)
0027A528 0018:
	??_C@_0BI@ONCCMPIK@count?$DM?$DNCLIP_BUFFER_SIZE?$AA@ (0000)
0027A540 0027:
	??_C@_0CH@GFCOCAKD@count?$DO?$DNNUMBER_OF_VERTICES_PER_TR@ (0000)
0027A568 0004:
	__real@b58637bd (0000)
0027A56C 0009:
	??_C@_08FGPNIDBO@surfaces?$AA@ (0000)
0027A578 0006:
	??_C@_05JGEJPNHM@edges?$AA@ (0000)
0027A580 0009:
	??_C@_08BOLPBPCL@vertices?$AA@ (0000)
0027A58C 003c:
	??_C@_0DM@LMGOJFMO@vertex2?9?$DOpoint_index?$DO?$DN0?5?$CG?$CG?5verte@ (0000)
0027A5C8 0010:
	??_C@_0BA@FMIEEKIK@vertex2?9?$DOextant?$AA@ (0000)
0027A5D8 003c:
	??_C@_0DM@NBEECNH@vertex1?9?$DOpoint_index?$DO?$DN0?5?$CG?$CG?5verte@ (0000)
0027A614 0010:
	??_C@_0BA@LHLDPBIJ@vertex1?9?$DOextant?$AA@ (0000)
0027A628 0043:
	??_C@_0ED@EPDBMIEH@?$CIedge?9?$DOsurface_indices?$FL0?$FN?$DN?$DNNONE?$CJ@ (0000)
0027A670 0043:
	??_C@_0ED@EMHNLENG@edge?9?$DOvertex_indices?$FL1?$FN?$DO?$DN0?5?$CG?$CG?5ed@ (0000)
0027A6B8 0043:
	??_C@_0ED@JNNMLONE@edge?9?$DOvertex_indices?$FL0?$FN?$DO?$DN0?5?$CG?$CG?5ed@ (0000)
0027A6FC 000d:
	??_C@_0N@NNPGIPCM@edge?9?$DOextant?$AA@ (0000)
0027A710 004f:
	??_C@_0EP@NMDNFEOD@edge?9?$DOvertex_indices?$FL0?$FN?$DN?$DNvertex_@ (0000)
0027A760 0027:
	??_C@_0CH@PNIAGAON@edge_index?$DO?$DN0?5?$CG?$CG?5edge_index?$DMedge@ (0000)
0027A788 0020:
	??_C@_0CA@OJBIMDIM@first_boundary_edge_index?$CB?$DNNONE?$AA@ (0000)
0027A7A8 0009:
	??_C@_08NONFIEMK@vb?5?$CB?$DN?5va?$AA@ (0000)
0027A7B4 0025:
	??_C@_0CF@PLABFBEI@vb?5?$DO?$DN?50?5?$CG?$CG?5vb?5?$DM?5sphere?9?$DOvertex_c@ (0000)
0027A7DC 0025:
	??_C@_0CF@KKAMJMOF@va?5?$DO?$DN?50?5?$CG?$CG?5va?5?$DM?5sphere?9?$DOvertex_c@ (0000)
0027A804 002f:
	??_C@_0CP@IEJEPOMO@result_count?$DO?$DN0?5?$CG?$CG?5result_count?$DM@ (0000)
0027A834 0017:
	??_C@_0BH@GHJAIPGF@p?$CB?$DNresult?5?$CG?$CG?5q?$CB?$DNresult?$AA@ (0000)
0027A84C 0008:
	??_C@_07OPEDLPNI@q_count?$AA@ (0000)
0027A854 0002:
	??_C@_01IIACKFLH@q?$AA@ (0000)
0027A858 0008:
	??_C@_07CDOJLPEG@p_count?$AA@ (0000)
0027A860 0020:
	??_C@_0CA@GNDAHJOD@maximum_count?$DM?$DNCLIP_BUFFER_SIZE?$AA@ (0000)
0027A880 0025:
	??_C@_0CF@POILEFLF@v3?5?$DO?$DN0?5?$CG?$CG?5v3?5?$DM?$DN?5sphere?9?$DOvertex_c@ (0000)
0027A8A8 0025:
	??_C@_0CF@IMOGDHLH@v2?5?$DO?$DN0?5?$CG?$CG?5v2?5?$DM?$DN?5sphere?9?$DOvertex_c@ (0000)
0027A8D0 0025:
	??_C@_0CF@BKFBKBLB@v1?5?$DO?$DN0?5?$CG?$CG?5v1?5?$DM?$DN?5sphere?9?$DOvertex_c@ (0000)
0027A8F8 0040:
	??_C@_0EA@DLEJKPCO@topright_vertex?5?$DO?$DN?50?5?$CG?$CG?5topright@ (0000)
0027A938 003a:
	??_C@_0DK@CHGNILGK@right_vertex?5?$DO?$DN?50?5?$CG?$CG?5right_verte@ (0000)
0027A974 0038:
	??_C@_0DI@CELCELPM@left_vertex?5?$DO?$DN?50?5?$CG?$CG?5left_vertex?5@ (0000)
0027A9AC 0036:
	??_C@_0DG@HPBCBMGH@top_vertex?5?$DO?$DN?50?5?$CG?$CG?5top_vertex?5?$DM?$DN@ (0000)
0027A9E8 0066:
	??_C@_0GG@CNBBDAKE@?$CKtriangle_strip_vertex_indices_i@ (0000)
0027AA50 0025:
	??_C@_0CF@PKGGEOOK@v3?5?$DO?$DN?50?5?$CG?$CG?5v3?5?$DM?5sphere?9?$DOvertex_c@ (0000)
0027AA78 0025:
	??_C@_0CF@MKJNAKHB@v2?5?$DO?$DN?50?5?$CG?$CG?5v2?5?$DM?5sphere?9?$DOvertex_c@ (0000)
0027AAA0 0025:
	??_C@_0CF@JLJAMHNM@v1?5?$DO?$DN?50?5?$CG?$CG?5v1?5?$DM?5sphere?9?$DOvertex_c@ (0000)
0027AAC8 001b:
	??_C@_0BL@OKBFAGJI@vertex_subdivision_indices?$AA@ (0000)
0027AAE4 0024:
	??_C@_0CE@LEDAAIBG@triangle_strip_vertex_indices_in@ (0000)
0027AB08 000d:
	??_C@_0N@KLBMBFKD@vertex_index?$AA@ (0000)
0027AB18 0025:
	??_C@_0CF@JFAFJJEF@vertex_index?5?$DN?$DN?5result?9?$DOvertex_c@ (0000)
0027AB40 0065:
	??_C@_0GF@PMJOGKBA@triangle_strip_vertex_indices_in@ (0000)
003078C4 0008:
	_global_convex_hull3d_delta (0000)
	_global_convex_hull3d_epsilon (0004)
*/

/* ---------- headers */

#include "cseries.h"

#include "real_math.h"

#include "geometry.h"

/* ---------- constants */

enum
{
	GEOSPHERE_PRIMITIVE_VERTEX_COUNT= 6,
	GEOSPHERE_PRIMITIVE_EDGE_COUNT= 12,
	GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT= 8,

	MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT= 8,

	CLIP_BUFFER_SIZE= 512
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static short points_dimension2d(
	short count,
	real_point2d const *points);

static void subdivide_triangle(
	struct geosphere *sphere,
	short v1,
	short v2,
	short v3,
	short *vertex_index,
	short *triangle_strip_vertex_indices_index,
	short *vertex_subdivision_indices);
static short get_face_vertex(
	struct geosphere *sphere,
	short v1,
	short v2,
	short v3,
	short row,
	short column,
	short *vertex_index,
	short *vertex_subdivision_indices,
	short *vertex_face_indices);
static short get_edge_vertex(
	struct geosphere *sphere,
	short v1,
	short v2,
	short subdivision_index,
	short *vertex_index,
	short *vertex_subdivision_indices);
static void calculate_vertex(
	struct geosphere *sphere,
	short new_vertex,
	short parent1,
	short parent2,
	short subdivision_count,
	short subdivision_index);

/* ---------- globals */

real global_convex_hull3d_delta = 0.01f;
real global_convex_hull3d_epsilon = 0.001f;

static real_point3d const geosphere_primitive_vertices[GEOSPHERE_PRIMITIVE_VERTEX_COUNT]=
{
	{0.f, 0.f, 1.f},
	{0.f, 1.f, 0.f},
	{1.f, 0.f, 0.f},
	{0.f, -1.f, 0.f},
	{-1.f, 0.f, 0.f},
	{0.f, 0.f, -1.f}
};

static short const geosphere_primitive_triangles[GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT][NUMBER_OF_VERTICES_PER_TRIANGLE]=
{
	{0, 1, 2},
	{0, 2, 3},
	{0, 3, 4},
	{0, 4, 1},
	{5, 1, 4},
	{5, 4, 3},
	{5, 3, 2},
	{5, 2, 1}
};

/* ---------- public code */

struct geosphere *geosphere_new(
	short segment_count)
{
	struct geosphere *result = match_malloc("c:\\halo\\SOURCE\\math\\geometry.c", 58, sizeof(struct geosphere));

	if (result)
	{
		short *vertex_subdivision_indices;

		result->segment_count = segment_count;
		result->triangle_count = GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT*segment_count*segment_count;
		result->vertex_count = GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT*(segment_count - 2)*(segment_count - 1)/2 +
			GEOSPHERE_PRIMITIVE_EDGE_COUNT*(segment_count - 1) + GEOSPHERE_PRIMITIVE_VERTEX_COUNT;
		result->vertices = match_malloc("c:\\halo\\SOURCE\\math\\geometry.c", 66, sizeof(real_point3d)*result->vertex_count);
		result->triangle_strip_vertex_indices = match_malloc("c:\\halo\\SOURCE\\math\\geometry.c", 67,
			sizeof(short)*(NUMBER_OF_VERTICES_PER_TRIANGLE + 1)*result->triangle_count);
		result->triangle_strip_count = 0;
		vertex_subdivision_indices = match_malloc("c:\\halo\\SOURCE\\math\\geometry.c", 69,
			sizeof(short)*MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT*MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT);

		if (result->vertices && result->triangle_strip_vertex_indices && vertex_subdivision_indices)
		{
			short vertex_index = 0;
			short triangle_strip_vertex_indices_index = 0;
			short triangle_index;
			short index;

			for (index = 0; index < MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT*MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT; index++)
			{
				vertex_subdivision_indices[index] = NONE;
			}

			for (index = 0; index < GEOSPHERE_PRIMITIVE_VERTEX_COUNT; index++)
			{
				result->vertices[vertex_index++] = geosphere_primitive_vertices[index];
			}

			for (triangle_index = 0; triangle_index < GEOSPHERE_PRIMITIVE_TRIANGLE_COUNT; triangle_index++)
			{
				subdivide_triangle(result, geosphere_primitive_triangles[triangle_index][0],
					geosphere_primitive_triangles[triangle_index][1], geosphere_primitive_triangles[triangle_index][2],
					&vertex_index, &triangle_strip_vertex_indices_index, vertex_subdivision_indices);
			}

			/* BUG (preserved for exact matching): at segment_count == 1,
			 * January fills the allocated strip buffer but rejects equality.
			 * A corrected build should allow a fully filled valid buffer.
			 */
			match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 98,
				triangle_strip_vertex_indices_index < (NUMBER_OF_VERTICES_PER_TRIANGLE + 1) * result->triangle_count);
			match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 99, vertex_index == result->vertex_count);
		}
		else
		{
			/* BUG (preserved for exact matching): January frees allocated
			 * children but returns the owner with dangling member pointers.
			 * A corrected build should free the owner and return NULL.
			 */
			if (result->vertices)
			{
				match_free("c:\\halo\\SOURCE\\math\\geometry.c", 103, result->vertices);
			}
			if (result->triangle_strip_vertex_indices)
			{
				match_free("c:\\halo\\SOURCE\\math\\geometry.c", 104, result->triangle_strip_vertex_indices);
			}
		}

		if (vertex_subdivision_indices)
		{
			match_free("c:\\halo\\SOURCE\\math\\geometry.c", 107, vertex_subdivision_indices);
		}
	}

	return result;
}

void geosphere_dispose(
	struct geosphere *sphere)
{
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 117, sphere);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 118, sphere->vertices);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 119, sphere->triangle_strip_vertex_indices);

	match_free("c:\\halo\\SOURCE\\math\\geometry.c", 121, sphere->vertices);
	match_free("c:\\halo\\SOURCE\\math\\geometry.c", 122, sphere->triangle_strip_vertex_indices);
	match_free("c:\\halo\\SOURCE\\math\\geometry.c", 123, sphere);
	return;
}

static void subdivide_triangle(
	struct geosphere *sphere,
	short v1,
	short v2,
	short v3,
	short *vertex_index,
	short *triangle_strip_vertex_indices_index,
	short *vertex_subdivision_indices)
{
	short face_count = (sphere->segment_count + 1)*(sphere->segment_count + 1);
	short *vertex_face_indices;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 146, vertex_index);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 147, triangle_strip_vertex_indices_index);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 148, vertex_subdivision_indices);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 149, v1 >= 0 && v1 < sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 150, v2 >= 0 && v2 < sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 151, v3 >= 0 && v3 < sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 152,
		*triangle_strip_vertex_indices_index < (NUMBER_OF_VERTICES_PER_TRIANGLE + 1) * sphere->triangle_count);

	vertex_face_indices = match_malloc("c:\\halo\\SOURCE\\math\\geometry.c", 154, sizeof(short)*face_count);
	if (vertex_face_indices)
	{
		short face_index;
		short row;

		for (face_index = 0; face_index < face_count; face_index++)
		{
			vertex_face_indices[face_index] = NONE;
		}

		for (row = 1; row <= sphere->segment_count; row++)
		{
			short column;

			sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = 2*row + 1;
			sphere->triangle_strip_count++;

			for (column = 1; column <= row; column++)
			{
				short top_vertex = get_face_vertex(sphere, v1, v2, v3, row - 1, column - 1,
					vertex_index, vertex_subdivision_indices, vertex_face_indices);
				short left_vertex = get_face_vertex(sphere, v1, v2, v3, row, column - 1,
					vertex_index, vertex_subdivision_indices, vertex_face_indices);
				short right_vertex = get_face_vertex(sphere, v1, v2, v3, row, column,
					vertex_index, vertex_subdivision_indices, vertex_face_indices);

				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 176, top_vertex >= 0 && top_vertex <= sphere->vertex_count);
				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 177, left_vertex >= 0 && left_vertex <= sphere->vertex_count);
				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 178, right_vertex >= 0 && right_vertex <= sphere->vertex_count);

				if (column == 1)
				{
					sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = left_vertex;
					sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = top_vertex;
				}
				sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = right_vertex;

				if (column < row)
				{
					short topright_vertex = get_face_vertex(sphere, v1, v2, v3, row - 1, column,
						vertex_index, vertex_subdivision_indices, vertex_face_indices);

					match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 194, topright_vertex >= 0 && topright_vertex <= sphere->vertex_count);
					sphere->triangle_strip_vertex_indices[(*triangle_strip_vertex_indices_index)++] = topright_vertex;
				}
			}
		}

		match_free("c:\\halo\\SOURCE\\math\\geometry.c", 200, vertex_face_indices);
	}

	return;
}

static short get_face_vertex(
	struct geosphere *sphere,
	short v1,
	short v2,
	short v3,
	short row,
	short column,
	short *vertex_index,
	short *vertex_subdivision_indices,
	short *vertex_face_indices)
{
	short face_index = row*(sphere->segment_count + 1) + column;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 222, v1 >=0 && v1 <= sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 223, v2 >=0 && v2 <= sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 224, v3 >=0 && v3 <= sphere->vertex_count);

	if (vertex_face_indices[face_index] == NONE)
	{
		if (column == 0)
		{
			vertex_face_indices[face_index] = get_edge_vertex(sphere, v1, v3, row, vertex_index, vertex_subdivision_indices);
		}
		else if (row == sphere->segment_count)
		{
			vertex_face_indices[face_index] = get_edge_vertex(sphere, v3, v2, column, vertex_index, vertex_subdivision_indices);
		}
		else if (column == row)
		{
			vertex_face_indices[face_index] = get_edge_vertex(sphere, v1, v2, row, vertex_index, vertex_subdivision_indices);
		}
		else
		{
			short new_vertex = (*vertex_index)++;
			short parent1 = get_edge_vertex(sphere, v1, v3, row, vertex_index, vertex_subdivision_indices);
			short parent2 = get_edge_vertex(sphere, v1, v2, row, vertex_index, vertex_subdivision_indices);

			vertex_face_indices[face_index] = new_vertex;
			calculate_vertex(sphere, new_vertex, parent1, parent2, row, column);
		}
	}

	return vertex_face_indices[face_index];
}

static short get_edge_vertex(
	struct geosphere *sphere,
	short v1,
	short v2,
	short subdivision_index,
	short *vertex_index,
	short *vertex_subdivision_indices)
{
	short va = MIN(v1, v2);
	short vb = MAX(v1, v2);
	boolean reversed = v1 > v2;
	short vertex;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 269, sphere);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 270, va >= 0 && va < sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 271, vb >= 0 && vb < sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 272, vb != va);

	if (subdivision_index == 0)
	{
		vertex = v1;
	}
	else if (subdivision_index == sphere->segment_count)
	{
		vertex = v2;
	}
	else
	{
		short *edge_vertex_index = vertex_subdivision_indices + va*MAXIMUM_GEOSPHERE_PRIMITIVE_VERTEX_COUNT + vb;
		long offset;

		if (*edge_vertex_index == NONE)
		{
			short index;

			*edge_vertex_index = *vertex_index;
			for (index = 1; index < sphere->segment_count; index++)
			{
				short new_vertex = (*vertex_index)++;

				calculate_vertex(sphere, new_vertex, va, vb, sphere->segment_count, index);
			}
		}

		offset = reversed ? sphere->segment_count - subdivision_index : subdivision_index;
		vertex = *edge_vertex_index + offset - 1;
	}

	return vertex;
}

boolean convex_hull3d_verify(void)
{
	return TRUE;
}

real vector_intersect_plane2d(
	real_point2d const *point,
	real_vector2d const *vector,
	real_plane2d const *plane)
{
	return ((plane->n.i*point->x + plane->n.j*point->y) - plane->d) /
		-(plane->n.i*vector->i + plane->n.j*vector->j);
}

static void calculate_vertex(
	struct geosphere *sphere,
	short new_vertex,
	short parent1,
	short parent2,
	short subdivision_count,
	short subdivision_index)
{
	real t = (real)subdivision_index/subdivision_count;
	real one_minus_t = 1.f - t;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 315, subdivision_index > 0 && subdivision_index < subdivision_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 316, parent1 >=0 && parent1 <= sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 317, parent2 >=0 && parent2 <= sphere->vertex_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 318, new_vertex >=0 && new_vertex <= sphere->vertex_count);

	set_real_point3d(&sphere->vertices[new_vertex],
		one_minus_t*sphere->vertices[parent1].x + t*sphere->vertices[parent2].x,
		one_minus_t*sphere->vertices[parent1].y + t*sphere->vertices[parent2].y,
		one_minus_t*sphere->vertices[parent1].z + t*sphere->vertices[parent2].z);
	normalize3d((real_vector3d *)&sphere->vertices[new_vertex]);

	return;
}

void build_torus(
	real_matrix4x3 const *matrix,
	short *vertex_count_reference,
	short *triangle_strip_count_reference,
	real_point3d *points,
	real_point2d *texture_uvs,
	short *triangle_strip_vertex_indices,
	short ring_segment_count,
	real ring_radius,
	short cylinder_segment_count,
	real cylinder_radius)
{
	short triangle_strip_count = 0;
	short vertex_count = 0;
	short ring_index;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 346, ring_segment_count>2);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 347, cylinder_segment_count>2);

	for (ring_index = 0; ring_index <= ring_segment_count; ring_index++)
	{
		real ring_fraction = (real)ring_index / (real)ring_segment_count;
		real ring_angle = ring_fraction * 2.f*_pi;
		real_vector3d radial;
		real_vector3d axis;
		real_point3d cylinder_point;
		short cylinder_index;

		set_real_vector3d(&radial, cosine(ring_angle)*ring_radius, sine(ring_angle)*ring_radius, 0.f);
		cross_product3d(&radial, global_up3d, &axis);
		normalize3d(&axis);

		set_real_point3d(&cylinder_point, cosine(ring_angle)*cylinder_radius, sine(ring_angle)*cylinder_radius, 0.f);

		for (cylinder_index = 0; cylinder_index <= cylinder_segment_count; cylinder_index++)
		{
			texture_uvs->y = ring_fraction*2.f;

			if (ring_index > 0)
			{
				if (cylinder_index == 0)
				{
					*triangle_strip_vertex_indices++ = 2*(cylinder_segment_count + 1);
					triangle_strip_count++;
				}

				*triangle_strip_vertex_indices++ = vertex_count;
				*triangle_strip_vertex_indices++ = vertex_count - cylinder_segment_count - 1;
			}

			if (ring_index == ring_segment_count)
			{
				long wrapped_index = (cylinder_segment_count + 1)*ring_segment_count;

				*points = points[-wrapped_index];
				texture_uvs->x = texture_uvs[-wrapped_index].x;
			}
			else
			{
				texture_uvs->x = ((real)cylinder_index / (real)cylinder_segment_count)*2.f;

				if (cylinder_index == cylinder_segment_count)
				{
					*points = points[-cylinder_segment_count];
				}
				else
				{
					real cylinder_angle = ((real)cylinder_index / (real)cylinder_segment_count)*2.f*_pi;

					*points = cylinder_point;
					rotate_vector_about_axis(
						(real_vector3d *)points,
						&axis,
						sine(cylinder_angle),
						cosine(cylinder_angle));
					points->x += radial.i;
					points->y += radial.j;
					points->z += radial.k;
					matrix4x3_transform_point(matrix, points, points);
				}
			}

			points++;
			texture_uvs++;
			vertex_count++;
		}
	}

	*vertex_count_reference = vertex_count;
	*triangle_strip_count_reference = triangle_strip_count;

	return;
}

static short points_dimension2d(
	short count,
	real_point2d const *points)
{
	short dimension = NONE;
	short index = 0;
	real_point2d reference_point;
	real_plane2d line;

	do
	{
		if (index >= count)
		{
			break;
		}

		switch (dimension)
		{
		case NONE:
			reference_point = points[index];
			dimension = 0;
			break;

		case 0:
			if (plane2d_from_points(&line, &points[index], &reference_point))
			{
				dimension = 1;
			}
			break;

		case 1:
			if (!(fabs(plane2d_distance_to_point(&line, &points[index])) < _real_epsilon))
			{
				dimension = 2;
			}
			break;
		}

		index++;
	}
	while (dimension < 2);

	return dimension;
}

short convex_hull2d(
	short vertex_count,
	real_point2d const *points,
	short *hull_indices)
{
	short hull_count = 0;

	if (points_dimension2d(vertex_count, points) == 2)
	{
		boolean nondegenerate = FALSE;
		real accumulated_angle = 0.f;
		real minimum_x = REAL_MAX;
		real minimum_y = REAL_MAX;
		short current_index;
		short best_index;
		short index;

		for (index = 0; index < vertex_count; index++)
		{
			if (points[index].y < minimum_y - _real_epsilon ||
				(points[index].y < minimum_y && points[index].x < minimum_x + _real_epsilon) ||
				(points[index].y < minimum_y + _real_epsilon && points[index].x < minimum_x - _real_epsilon))
			{
				minimum_x = points[index].x;
				current_index = index;
				minimum_y = points[index].y;
			}
		}

		do
		{
			real minimum_angle_increment = REAL_MAX;

			if (hull_count >= vertex_count)
			{
				short last_vertex_index = hull_count - 1;
				short start_vertex_index;
				short vertex_index;

				for (start_vertex_index = last_vertex_index - 1; start_vertex_index > 0; start_vertex_index--)
				{
					if (hull_indices[start_vertex_index] == hull_indices[last_vertex_index])
					{
						hull_count = last_vertex_index - start_vertex_index;
						for (vertex_index = 0; vertex_index < hull_count; vertex_index++)
						{
							match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 633, vertex_index<vertex_count);
							match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 634, start_vertex_index+vertex_index<vertex_count);
							hull_indices[vertex_index] = hull_indices[start_vertex_index + vertex_index];
						}
						break;
					}
				}
				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 642, start_vertex_index>0);
				break;
			}

			/* current_index and best_index are assigned only inside the scans. For finite
			 * points with dimension 2 both scans assign them; a NaN or infinite coordinate can
			 * leave either unassigned. Not shown reachable: the callers' points (king hill
			 * markers, lens flare projections) were not traced for non-finite values.
			 * Source-policy approval pending (2026-09-27 audit). */
			hull_indices[hull_count++] = current_index;

			for (index = 0; index < vertex_count; index++)
			{
				if (points[index].x != points[current_index].x || points[index].y != points[current_index].y)
				{
					real angle_increment = arctangent(points[index].y - points[current_index].y, points[index].x - points[current_index].x) - accumulated_angle;

					while (angle_increment < -_real_epsilon)
					{
						angle_increment += 2.f*_pi;
					}
					if (angle_increment < minimum_angle_increment)
					{
						minimum_angle_increment = angle_increment;
						best_index = index;
					}
				}
			}

			accumulated_angle += minimum_angle_increment;
			current_index = best_index;

			if (!nondegenerate)
			{
				nondegenerate = !(realcmp(points[best_index].x, points[hull_indices[0]].x) &&
					realcmp(points[best_index].y, points[hull_indices[0]].y));
			}
		}
		while (best_index != hull_indices[0] &&
			!(nondegenerate &&
				realcmp(points[best_index].x, points[hull_indices[0]].x) &&
				realcmp(points[best_index].y, points[hull_indices[0]].y)));
	}

	return hull_count;
}

boolean convex_hull2d_verify(
	short vertex_count,
	real_point2d const *vertices,
	short index_count,
	short const *indices)
{
	real total_angle = 0.f;
	short index;
	real_point2d const *point_base = vertices;
	short const *index_base = indices;

	for (index = 0; index < index_count; index++)
	{
		long previous_index = index - 1 >= 0 ? index - 1 : index_count - 1;
		real_point2d const *previous = point_base + index_base[previous_index];
		real_point2d const *current = point_base + index_base[index];
		real_vector2d edge_previous;
		real_vector2d edge_next;
		long next_index;
		real_point2d const *next;

		next_index = index + 1 < index_count ? index + 1 : 0;
		next = point_base + index_base[next_index];
		edge_previous.i = current->x - previous->x;
		edge_previous.j = current->y - previous->y;
		edge_next.i = next->x - current->x;
		edge_next.j = next->y - current->y;
		if (cross_product2d(&edge_previous, &edge_next) < 0.f)
		{
			return FALSE;
		}
		total_angle += angle_between_vectors2d(&edge_previous, &edge_next);
	}

	return fabs(total_angle - 2.f*_pi) < 0.001f;
}

boolean convex_hull2d_test_circle(
	short count,
	real_point2d const *points,
	real_point2d const *center,
	real radius)
{
	short index;
	boolean result = TRUE;
	real_point2d const *point_base = points;
	real_point2d const *circle_center = center;
	real radius_squared = radius*radius;

	for (index = 0; index < count; index++)
	{
		long next_index = index + 1 < count ? index + 1 : 0;
		real_vector2d edge;
		real_vector2d offset;
		real edge_length_squared;
		real cross;

		edge.i = point_base[next_index].x - point_base[index].x;
		edge.j = point_base[next_index].y - point_base[index].y;
		offset.i = circle_center->x - point_base[index].x;
		offset.j = circle_center->y - point_base[index].y;
		edge_length_squared = edge.i*edge.i;
		edge_length_squared += edge.j*edge.j;
		if (edge_length_squared != 0.f)
		{
			cross = cross_product2d(&offset, &edge);
			if (cross > 0.f && cross*cross > edge_length_squared*radius_squared)
			{
				result = FALSE;
				break;
			}
		}
	}

	return result;
}

boolean convex_hull2d_test_point(
	short count,
	real_point2d const *points,
	real_point2d const *point,
	real epsilon)
{
	short index;
	boolean result = TRUE;
	real_point2d const *point_base = points;

	for (index = 0; index < count; index++)
	{
		real_point2d const *current = point_base + index;
		long next_index = index + 1 < count ? index + 1 : 0;
		real_point2d const *next = point_base + next_index;
		real_vector2d edge;
		real_vector2d offset;

		edge.i = next->x - current->x;
		edge.j = next->y - current->y;
		offset.i = point->x - current->x;
		offset.j = point->y - current->y;
		if (cross_product2d(&edge, &offset) < -epsilon)
		{
			result = FALSE;
			break;
		}
	}

	return result;
}

boolean convex_hull2d_test_point_indexed(
	short count,
	short const *indices,
	real_point2d const *points,
	real_point2d const *point,
	real epsilon)
{
	short index;
	boolean result = TRUE;
	short const *index_base = indices;
	real_point2d const *point_base = points;

	for (index = 0; index < count; index++)
	{
		real_point2d const *current = point_base + index_base[index];
		long next_index = index + 1 < count ? index + 1 : 0;
		real_point2d const *next = point_base + index_base[next_index];
		real_vector2d edge;
		real_vector2d offset;

		edge.i = next->x - current->x;
		edge.j = next->y - current->y;
		offset.i = point->x - current->x;
		offset.j = point->y - current->y;
		if (cross_product2d(&edge, &offset) < -epsilon)
		{
			result = FALSE;
			break;
		}
	}

	return result;
}

real convex_hull2d_area(
	short count,
	real_point2d const *vertices)
{
	real area;

	area = 0.f;
	if (count > 2)
	{
		long remaining;
		real_point2d const *vertex = vertices + 1;
		remaining = (unsigned short)(count - 2);
		do
		{
			real_vector2d first;
			real_vector2d second;

			first.i = vertex->x - vertices[0].x;
			first.j = vertex->y - vertices[0].y;
			second.i = (vertex + 1)->x - vertices[0].x;
			second.j = (vertex + 1)->y - vertices[0].y;
			area += (first.i*second.j - first.j*second.i)*0.5f;
			vertex++;
			remaining--;
		}
		while (remaining != 0);
	}

	return (real)fabs(area);
}

real convex_hull2d_perimeter(
	short count,
	real_point2d const *points)
{
	real perimeter = distance2d(points + count - 1, points);
	short index;

	for (index = 1; index < count; index++)
	{
		perimeter += distance2d(points + index - 1, points + index);
	}

	return perimeter;
}

boolean convex_hull2d_test_vector(
	short count,
	real_point2d const *points,
	real_point2d const *origin,
	real_vector2d const *vector,
	real *minimum_distance,
	real *maximum_distance)
{
	real minimum = REAL_MIN;
	real maximum = REAL_MAX;
	short index = 0;

	if (count > 0)
	{
		do
		{
			long next_index = index + 1 < count ? index + 1 : 0;
			real_vector2d edge;
			real_vector2d offset;
			real denominator;
			real numerator;

			edge.i = points[next_index].x - points[index].x;
			edge.j = points[next_index].y - points[index].y;
			offset.i = origin->x - points[index].x;
			offset.j = origin->y - points[index].y;
			denominator = edge.j*vector->i - edge.i*vector->j;
			numerator = edge.i*offset.j - offset.i*edge.j;

			if (!(fabs(denominator) < _real_epsilon))
			{
				real distance = numerator / denominator;

				if (denominator > 0.f)
				{
					if (minimum < distance)
					{
						minimum = distance;
					}
				}
				else if (maximum > distance)
				{
					maximum = distance;
				}

				if (minimum > maximum)
				{
					return FALSE;
				}
			}
			else if (numerator < _real_epsilon)
			{
				return FALSE;
			}
			index++;
		}
		while (index < count);
	}

	if (minimum_distance)
	{
		*minimum_distance = minimum;
	}
	if (maximum_distance)
	{
		*maximum_distance = maximum;
	}

	return TRUE;
}

short convex_hull2d_intersect(
	short p_count,
	real_point2d const *p,
	short q_count,
	real_point2d const *q,
	short maximum_count,
	real_point2d *result,
	real epsilon)
{
	short result_count = q_count;
	real_point2d const *source = q;
	real_plane2d plane;
	real_point2d buffers[2][CLIP_BUFFER_SIZE];
	short index;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1016, maximum_count<=CLIP_BUFFER_SIZE);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1017, p);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1018, p_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1019, q);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1020, q_count);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1021, result);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1022, p!=result && q!=result);

	for (index = 0; index < p_count && result_count > 0; index++)
	{
		short previous_index = index != 0 ? index - 1 : p_count - 1;
		real_point2d *output = index == p_count - 1 ? result : buffers[index & 1];

		if (plane2d_from_points(&plane, p + index, p + previous_index))
		{
			result_count = convex_polygon2d_clip_to_plane(result_count, source, &plane, maximum_count, output, NULL, NULL, epsilon);
			if (result_count == NONE)
			{
				return NONE;
			}
		}
		else
		{
			match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1037, result_count>=0 && result_count<=maximum_count);
			csmemcpy(output, source, sizeof(real_point2d)*result_count);
		}

		source = output;
	}

	return result_count;
}

short convex_polygon2d_clip_to_plane(
	short count,
	real_point2d const *points,
	real_plane2d const *plane,
	short maximum_count,
	real_point2d *result,
	long *clip_flags,
	boolean *clipped,
	real epsilon)
{
	real_point2d local_points[CLIP_BUFFER_SIZE];
	boolean any_in_front = FALSE;
	boolean any_behind = FALSE;
	long result_clip_flags = 0;
	short result_count = 0;
	real_point2d const *previous_point;
	boolean previous_in_front;
	short point_index;

	match_assert(
		"c:\\halo\\SOURCE\\math\\geometry.c",
		1350,
		count>=NUMBER_OF_VERTICES_PER_TRIANGLE);

	if (clipped)
	{
		*clipped = FALSE;
	}

	if (points == result)
	{
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1357, count<=CLIP_BUFFER_SIZE);
		csmemcpy(local_points, points, sizeof(real_point2d)*count);
		points = local_points;
	}

	previous_point = points + count - 1;
	previous_in_front = plane2d_distance_to_point(plane, previous_point) >= 0.f;

	for (point_index = 0; point_index < count; point_index++)
	{
		real_point2d const *point = points + point_index;
		real distance = plane2d_distance_to_point(plane, point);
		boolean in_front = distance >= 0.f;

		if (distance > epsilon)
		{
			any_in_front = TRUE;
		}
		else if (distance < -epsilon)
		{
			any_behind = TRUE;
		}

		if (in_front != previous_in_front)
		{
			real_vector2d vector;
			real scale;

			if (result_count == maximum_count)
			{
				result_count = NONE;
				break;
			}

			if (clipped)
			{
				*clipped = TRUE;
			}

			vector_from_points2d(point, previous_point, &vector);
			scale = PIN(vector_intersect_plane2d(point, &vector, plane), 0.f, 1.f);
			point_from_line2d(point, &vector, scale, result + result_count);
			SET_FLAG(result_clip_flags, result_count, TRUE);
			result_count++;

			if (result_count != 1 &&
				((fabs(result[result_count - 1].x - result[0].x) < epsilon &&
					fabs(result[result_count - 1].y - result[0].y) < epsilon) ||
				(fabs(result[result_count - 1].x - result[result_count - 2].x) < epsilon &&
					fabs(result[result_count - 1].y - result[result_count - 2].y) < epsilon)))
			{
				result_count--;
			}
		}

		if (in_front)
		{
			if (result_count == maximum_count)
			{
				result_count = NONE;
				break;
			}

			result[result_count] = *point;
			SET_FLAG(result_clip_flags, result_count, clip_flags && TEST_FLAG(*clip_flags, point_index));
			result_count++;

			if (result_count != 1 &&
				((fabs(result[result_count - 1].x - result[0].x) < epsilon &&
					fabs(result[result_count - 1].y - result[0].y) < epsilon) ||
				(fabs(result[result_count - 1].x - result[result_count - 2].x) < epsilon &&
					fabs(result[result_count - 1].y - result[result_count - 2].y) < epsilon)))
			{
				result_count--;
			}
		}

		previous_point = point;
		previous_in_front = in_front;
	}

	if (result_count != NONE)
	{
		if (result_count < 3)
		{
			result_count = 0;
		}

		if (!any_in_front)
		{
			result_count = 0;
		}
		else if (!any_behind)
		{
			match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1441, count>=0 && count<=maximum_count);
			csmemcpy(result, points, sizeof(real_point2d)*count);
			result_count = count;
		}
	}
	else
	{
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1448, count>=0 && count<=maximum_count);
		csmemcpy(result, points, sizeof(real_point2d)*count);
	}

	if (clip_flags)
	{
		*clip_flags = result_clip_flags;
	}

	return result_count;
}

boolean convex_polygon2d_verify(
	short count,
	real *vertices)
{
	short index = 0;

	if (count > 0)
	{
		do
		{
			if (!valid_real(vertices[index*2]) || !valid_real(vertices[index*2 + 1]))
			{
				return FALSE;
			}
			index++;
		}
		while (index < count);
	}

	return TRUE;
}

short convex_polygon3d_clip_to_plane(
	short count,
	real_point3d const *points,
	real_plane3d const *plane,
	short maximum_count,
	real_point3d *result,
	boolean *clipped,
	real epsilon,
	boolean keep_degenerate)
{
	real_point3d local_points[CLIP_BUFFER_SIZE];
	boolean any_in_front = FALSE;
	boolean any_behind = FALSE;
	short result_count = 0;
	real_point3d const *previous_point;
	boolean previous_in_front;
	short point_index;

	match_assert(
		"c:\\halo\\SOURCE\\math\\geometry.c",
		1493,
		count>=NUMBER_OF_VERTICES_PER_TRIANGLE);

	if (clipped)
	{
		*clipped = FALSE;
	}

	if (points == result)
	{
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1500, count<=CLIP_BUFFER_SIZE);
		csmemcpy(local_points, points, sizeof(real_point3d)*count);
		points = local_points;
	}

	previous_point = points + count - 1;
	previous_in_front = plane3d_distance_to_point(plane, previous_point) >= 0.f;

	for (point_index = 0; point_index < count; point_index++)
	{
		real_point3d const *point = points + point_index;
		real distance = plane3d_distance_to_point(plane, point);
		boolean in_front = distance >= 0.f;

		if (distance > epsilon)
		{
			any_in_front = TRUE;
		}
		else if (distance < -epsilon)
		{
			any_behind = TRUE;
		}

		if (in_front != previous_in_front)
		{
			real_vector3d vector;
			real scale;

			if (result_count == maximum_count)
			{
				result_count = NONE;
				break;
			}

			if (clipped)
			{
				*clipped = TRUE;
			}

			vector_from_points3d(point, previous_point, &vector);
			scale = PIN(vector_intersect_plane3d(point, &vector, plane), 0.f, 1.f);
			result[result_count].x = vector.i*scale + point->x;
			result[result_count].y = vector.j*scale + point->y;
			result[result_count].z = vector.k*scale + point->z;
			result_count++;

			if (result_count != 1 &&
				((fabs(result[result_count - 1].x - result[0].x) < epsilon &&
					fabs(result[result_count - 1].y - result[0].y) < epsilon &&
					fabs(result[result_count - 1].z - result[0].z) < epsilon) ||
				(fabs(result[result_count - 1].x - result[result_count - 2].x) < epsilon &&
					fabs(result[result_count - 1].y - result[result_count - 2].y) < epsilon &&
					fabs(result[result_count - 1].z - result[result_count - 2].z) < epsilon)))
			{
				result_count--;
			}
		}

		if (in_front)
		{
			if (result_count >= maximum_count)
			{
				result_count = NONE;
				break;
			}

			result[result_count] = *point;
			result_count++;

			if (result_count != 1 &&
				((fabs(result[result_count - 1].x - result[0].x) < epsilon &&
					fabs(result[result_count - 1].y - result[0].y) < epsilon &&
					fabs(result[result_count - 1].z - result[0].z) < epsilon) ||
				(fabs(result[result_count - 1].x - result[result_count - 2].x) < epsilon &&
					fabs(result[result_count - 1].y - result[result_count - 2].y) < epsilon &&
					fabs(result[result_count - 1].z - result[result_count - 2].z) < epsilon)))
			{
				result_count--;
			}
		}

		previous_point = point;
		previous_in_front = in_front;
	}

	if (result_count == NONE)
	{
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1591, count>=0 && count<=maximum_count);
		csmemcpy(result, points, sizeof(real_point3d)*count);
	}
	else
	{
		if (result_count < 3)
		{
			result_count = 0;
		}

		if (!any_in_front)
		{
			if (any_behind || !keep_degenerate)
			{
				return 0;
			}
		}

		if (!any_behind)
		{
			match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1584, count>=0 && count<=maximum_count);
			csmemcpy(result, points, sizeof(real_point3d)*count);
			result_count = count;
		}
	}

	return result_count;
}

boolean convex_polygon3d_verify(
	short count,
	real_point3d const *points)
{
	real_vector3d edge0;
	real_vector3d edge1;
	real_vector3d normal;
	real_vector3d cross;
	short index;

	vector_from_points3d(&points[1], &points[0], &edge0);
	vector_from_points3d(&points[1], &points[2], &edge1);
	cross_product3d(&edge0, &edge1, &normal);

	for (index = 0; index < count; index++)
	{
		real_point3d const *previous = index == 0 ? points + count - 1 : points + index - 1;
		real_point3d const *current = points + index;
		real_point3d const *next = index == count - 1 ? points : current + 1;

		if (!valid_real_point3d(current))
		{
			return FALSE;
		}

		vector_from_points3d(current, previous, &edge0);
		vector_from_points3d(current, next, &edge1);
		cross_product3d(&edge0, &edge1, &cross);
		if (dot_product3d(&normal, &cross) < -0.000001f)
		{
			return FALSE;
		}
	}

	return TRUE;
}

boolean convex_hull3d_begin(
	short point_count,
	real_point3d const *points,
	short vertex_count,
	struct vertex3d *vertices,
	short edge_count,
	struct edge3d *edges,
	short surface_count,
	struct surface3d *surfaces)
{
	short minimum_x_point_index = NONE;
	short farthest_point_index = NONE;
	short farthest_line_point_index = NONE;
	short farthest_plane_point_index = NONE;
	real minimum_x;
	real maximum_distance_squared;
	real maximum_line_distance_squared;
	real maximum_plane_distance;
	real_vector3d line_direction;
	real_plane3d plane;
	short point_index;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1710, points);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1711, vertices);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1712, edges);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1713, surfaces);

	if (vertex_count < 4 || edge_count < 6 || surface_count < 4)
	{
		return FALSE;
	}

	minimum_x = REAL_MAX;
	for (point_index = 0; point_index < point_count; point_index++)
	{
		if (points[point_index].x < minimum_x)
		{
			minimum_x = points[point_index].x;
			minimum_x_point_index = point_index;
		}
	}
	if (minimum_x_point_index == NONE)
	{
		return FALSE;
	}

	maximum_distance_squared = 0.f;
	for (point_index = 0; point_index < point_count; point_index++)
	{
		if (distance_squared3d(points + point_index, points + minimum_x_point_index) > maximum_distance_squared)
		{
			farthest_point_index = point_index;
			maximum_distance_squared = distance_squared3d(points + point_index, points + minimum_x_point_index);
		}
	}
	if (farthest_point_index == NONE || maximum_distance_squared < global_convex_hull3d_delta)
	{
		return FALSE;
	}

	maximum_line_distance_squared = 0.f;
	vector_from_points3d(points + minimum_x_point_index, points + farthest_point_index, &line_direction);
	for (point_index = 0; point_index < point_count; point_index++)
	{
		real_vector3d offset;
		real_vector3d projection = line_direction;
		real_vector3d perpendicular;
		real t;
		real distance_squared;

		vector_from_points3d(points + minimum_x_point_index, points + point_index, &offset);
		t = dot_product3d(&offset, &line_direction) / magnitude_squared3d(&line_direction);
		scale_vector3d(&projection, t, &projection);
		subtract_vectors3d(&offset, &projection, &perpendicular);
		distance_squared = magnitude_squared3d(&perpendicular);

		if (distance_squared > maximum_line_distance_squared)
		{
			maximum_line_distance_squared = distance_squared;
			farthest_line_point_index = point_index;
		}
	}
	if (farthest_line_point_index == NONE || maximum_line_distance_squared < global_convex_hull3d_delta)
	{
		return FALSE;
	}

	maximum_plane_distance = 0.f;
	plane3d_from_points(&plane,
		points + minimum_x_point_index,
		points + farthest_point_index,
		points + farthest_line_point_index);
	for (point_index = 0; point_index < point_count; point_index++)
	{
		real distance = plane3d_distance_to_point(&plane, points + point_index);

		if (fabs(distance) > fabs(maximum_plane_distance))
		{
			maximum_plane_distance = distance;
			farthest_plane_point_index = point_index;
		}
	}
	if (farthest_plane_point_index == NONE || fabs(maximum_plane_distance) < global_convex_hull3d_delta)
	{
		return FALSE;
	}

	if (maximum_plane_distance > 0.f)
	{
		short swap = farthest_point_index;
		farthest_point_index = farthest_line_point_index;
		farthest_line_point_index = swap;
	}

	vertices[0].extant = TRUE;
	vertices[0].point_index = minimum_x_point_index;
	vertices[0].edge_index = 0;
	vertices[1].extant = TRUE;
	vertices[1].point_index = farthest_point_index;
	vertices[1].edge_index = 0;
	vertices[2].extant = TRUE;
	vertices[2].point_index = farthest_line_point_index;
	vertices[2].edge_index = 1;
	vertices[3].extant = TRUE;
	vertices[3].point_index = farthest_plane_point_index;
	vertices[3].edge_index = 3;

	edges[0].extant = TRUE;
	edges[0].vertex_indices[0] = 0;
	edges[0].vertex_indices[1] = 1;
	edges[0].edge_indices[0] = 1;
	edges[0].edge_indices[1] = 3;
	edges[0].surface_indices[0] = 0;
	edges[0].surface_indices[1] = 1;
	edges[1].extant = TRUE;
	edges[1].vertex_indices[0] = 1;
	edges[1].vertex_indices[1] = 2;
	edges[1].edge_indices[0] = 2;
	edges[1].edge_indices[1] = 4;
	edges[1].surface_indices[0] = 0;
	edges[1].surface_indices[1] = 2;
	edges[2].extant = TRUE;
	edges[2].vertex_indices[0] = 2;
	edges[2].vertex_indices[1] = 0;
	edges[2].edge_indices[0] = 0;
	edges[2].edge_indices[1] = 5;
	edges[2].surface_indices[0] = 0;
	edges[2].surface_indices[1] = 3;
	edges[3].extant = TRUE;
	edges[3].vertex_indices[0] = 0;
	edges[3].vertex_indices[1] = 3;
	edges[3].edge_indices[0] = 4;
	edges[3].edge_indices[1] = 2;
	edges[3].surface_indices[0] = 1;
	edges[3].surface_indices[1] = 3;
	edges[4].extant = TRUE;
	edges[4].vertex_indices[0] = 3;
	edges[4].vertex_indices[1] = 1;
	edges[4].edge_indices[0] = 0;
	edges[4].edge_indices[1] = 5;
	edges[4].surface_indices[0] = 1;
	edges[4].surface_indices[1] = 2;
	edges[5].extant = TRUE;
	edges[5].vertex_indices[0] = 3;
	edges[5].vertex_indices[1] = 2;
	edges[5].edge_indices[0] = 1;
	edges[5].edge_indices[1] = 3;
	edges[5].surface_indices[0] = 2;
	edges[5].surface_indices[1] = 3;

	surfaces[0].extant = TRUE;
	plane3d_from_points(&surfaces[0].plane,
		points + minimum_x_point_index,
		points + farthest_point_index,
		points + farthest_line_point_index);
	surfaces[0].edge_index = 0;
	surfaces[1].extant = TRUE;
	plane3d_from_points(&surfaces[1].plane,
		points + minimum_x_point_index,
		points + farthest_plane_point_index,
		points + farthest_point_index);
	surfaces[1].edge_index = 0;
	surfaces[2].extant = TRUE;
	plane3d_from_points(&surfaces[2].plane,
		points + farthest_point_index,
		points + farthest_plane_point_index,
		points + farthest_line_point_index);
	surfaces[2].edge_index = 1;
	surfaces[3].extant = TRUE;
	plane3d_from_points(&surfaces[3].plane,
		points + minimum_x_point_index,
		points + farthest_line_point_index,
		points + farthest_plane_point_index);
	surfaces[3].edge_index = 2;

	for (point_index = 4; point_index < vertex_count; point_index++)
	{
		vertices[point_index].extant = FALSE;
	}
	for (point_index = 6; point_index < edge_count; point_index++)
	{
		edges[point_index].extant = FALSE;
	}
	for (point_index = 4; point_index < surface_count; point_index++)
	{
		surfaces[point_index].extant = FALSE;
	}

	return TRUE;
}

boolean convex_hull3d_expand(
	short point_count,
	real_point3d const *points,
	short vertex_count,
	struct vertex3d *vertices,
	short edge_count,
	struct edge3d *edges,
	short surface_count,
	struct surface3d *surfaces,
	short point_index)
{
	real_point3d const *new_point;
	boolean already_contained = TRUE;
	short first_boundary_edge_index;
	short surface_index;
	short edge_index;
	short vertex_index;
	short new_vertex_index;
	short previous_new_edge_index;
	short first_new_edge_index;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1973, points);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1974, vertices);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1975, edges);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 1976, surfaces);

	if (point_index < 0 || point_index >= point_count)
	{
		return FALSE;
	}

	new_point = points + point_index;
	first_boundary_edge_index = NONE;
	for (surface_index = 0; surface_index < surface_count; surface_index++)
	{
		struct surface3d const *surface = surfaces + surface_index;

		if (surface->extant &&
			surface->plane.n.i*new_point->x + surface->plane.n.j*new_point->y +
			surface->plane.n.k*new_point->z - surface->plane.d > global_convex_hull3d_delta)
		{
			already_contained = FALSE;
			break;
		}
	}
	if (already_contained)
	{
		return TRUE;
	}

	for (surface_index = 0; surface_index < surface_count; surface_index++)
	{
		struct surface3d *surface = surfaces + surface_index;

		if (surface->extant)
		{
			real distance = surface->plane.n.i*new_point->x +
				surface->plane.n.j*new_point->y + surface->plane.n.k*new_point->z - surface->plane.d;
			surface->extant = !(distance > -global_convex_hull3d_epsilon);
		}
	}

	for (edge_index = 0; edge_index < edge_count; edge_index++)
	{
		struct edge3d *edge = edges + edge_index;

		if (edge->extant)
		{
			struct surface3d *surface0;
			struct surface3d *surface1;

			if (edge->surface_indices[0] < 0 || edge->surface_indices[0] >= surface_count ||
				edge->surface_indices[1] < 0 || edge->surface_indices[1] >= surface_count)
			{
				return FALSE;
			}
			surface0 = surfaces + edge->surface_indices[0];
			surface1 = surfaces + edge->surface_indices[1];

			edge->extant = surface0->extant || surface1->extant;
			if (edge->extant && surface0->extant != surface1->extant)
			{
				if (!surface0->extant)
				{
					edge->surface_indices[0] = NONE;
				}
				else
				{
					edge->surface_indices[1] = NONE;
				}
				if (first_boundary_edge_index == NONE)
				{
					first_boundary_edge_index = edge_index;
				}
			}
		}
	}
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2076, first_boundary_edge_index!=NONE);

	for (vertex_index = 0; vertex_index < vertex_count; vertex_index++)
	{
		struct vertex3d *vertex = vertices + vertex_index;

		if (vertex->extant)
		{
			short first_edge_index = NONE;
			short previous_edge_index = NONE;
			short edge_index = (short)vertex->edge_index;

			do
			{
				struct edge3d *edge;

				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2098, edge_index>=0 && edge_index<edge_count);
				edge = edges + edge_index;
				match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2099,
					edge->vertex_indices[0]==vertex_index || edge->vertex_indices[1]==vertex_index);

				if (edge->extant)
				{
					if (previous_edge_index == NONE)
					{
						first_edge_index = edge_index;
					}
					else
					{
						struct edge3d *previous_edge = edges + previous_edge_index;
						previous_edge->edge_indices[previous_edge->vertex_indices[0]==vertex_index] = edge_index;
					}
					previous_edge_index = edge_index;
				}
				edge_index = (short)edge->edge_indices[edge->vertex_indices[0]==vertex_index];
			}
			while (edge_index != vertex->edge_index);

			if (previous_edge_index == NONE)
			{
				vertex->extant = FALSE;
			}
			else
			{
				struct edge3d *previous_edge = edges + previous_edge_index;
				vertex->edge_index = first_edge_index;
				previous_edge->edge_indices[previous_edge->vertex_indices[0]==vertex_index] = first_edge_index;
			}
		}
	}

	new_vertex_index = 0;
	first_new_edge_index = NONE;
	previous_new_edge_index = NONE;
	edge_index = first_boundary_edge_index;
	while (new_vertex_index < vertex_count && vertices[new_vertex_index].extant)
	{
		new_vertex_index++;
	}
	if (new_vertex_index >= vertex_count)
	{
		return FALSE;
	}

	for (;;)
	{
		struct edge3d *edge;
		struct vertex3d *vertex1;
		struct vertex3d *vertex2;
		struct surface3d *new_surface;
		struct edge3d *new_edge;
		short new_surface_index;
		short new_edge_index;

		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2167, edge_index>=0 && edge_index<edge_count);
		edge = edges + edge_index;
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2168, edge->extant);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2169,
			edge->vertex_indices[0]>=0 && edge->vertex_indices[0]<vertex_count);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2170,
			edge->vertex_indices[1]>=0 && edge->vertex_indices[1]<vertex_count);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2171,
			(edge->surface_indices[0]==NONE)!=(edge->surface_indices[1]==NONE));

		for (new_surface_index = 0;
			new_surface_index < surface_count && surfaces[new_surface_index].extant;
			new_surface_index++)
		{
		}
		if (new_surface_index >= surface_count)
		{
			break;
		}

		for (new_edge_index = 0;
			new_edge_index < edge_count && edges[new_edge_index].extant;
			new_edge_index++)
		{
		}
		if (new_edge_index >= edge_count)
		{
			break;
		}

		new_surface = surfaces + new_surface_index;
		vertex1 = vertices + edge->vertex_indices[edge->surface_indices[0]!=NONE];
		vertex2 = vertices + edge->vertex_indices[edge->surface_indices[1]!=NONE];

		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2206, vertex1->extant);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2207,
			vertex1->point_index>=0 && vertex1->point_index<point_count);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2209, vertex2->extant);
		match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2210,
			vertex2->point_index>=0 && vertex2->point_index<point_count);

		new_surface->extant = TRUE;
		plane3d_from_points(&new_surface->plane,
			new_point,
			points + vertex1->point_index,
			points + vertex2->point_index);
		new_surface->edge_index = edge_index;

		new_edge = edges + new_edge_index;
		new_edge->extant = TRUE;
		new_edge->vertex_indices[0] = edge->vertex_indices[edge->surface_indices[1]!=NONE];
		new_edge->vertex_indices[1] = new_vertex_index;
		new_edge->edge_indices[0] = previous_new_edge_index;
		new_edge->edge_indices[1] = edge->edge_indices[edge->surface_indices[0]!=NONE];
		new_edge->surface_indices[0] = new_surface_index;
		new_edge->surface_indices[1] = NONE;

		edge->edge_indices[edge->surface_indices[0]!=NONE] = new_edge_index;
		edge->surface_indices[edge->surface_indices[0]!=NONE] = new_surface_index;
		if (previous_new_edge_index == NONE)
		{
			first_new_edge_index = new_edge_index;
		}
		else
		{
			edges[previous_new_edge_index].surface_indices[1] = new_surface_index;
		}
		previous_new_edge_index = new_edge_index;

		edge_index = (short)new_edge->edge_indices[1];
		if (edge_index == first_boundary_edge_index)
		{
			vertices[new_vertex_index].extant = TRUE;
			vertices[new_vertex_index].point_index = point_index;
			vertices[new_vertex_index].edge_index = first_new_edge_index;
			edges[first_new_edge_index].edge_indices[0] = new_edge_index;
			new_edge->surface_indices[1] = edges[first_new_edge_index].surface_indices[0];
			return TRUE;
		}
	}

	return FALSE;
}

boolean convex_hull3d(
	short point_count,
	real_point3d const *points,
	short vertex_count,
	struct vertex3d *vertices,
	short edge_count,
	struct edge3d *edges,
	short surface_count,
	struct surface3d *surfaces)
{
	short point_index;

	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2284, points);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2285, vertices);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2286, edges);
	match_assert("c:\\halo\\SOURCE\\math\\geometry.c", 2287, surfaces);

	if (!convex_hull3d_begin(point_count, points, vertex_count, vertices, edge_count, edges, surface_count, surfaces))
	{
		return FALSE;
	}

	for (point_index = 0; point_index < point_count; point_index++)
	{
		if (!convex_hull3d_expand(point_count, points, vertex_count, vertices, edge_count, edges, surface_count, surfaces, point_index))
		{
			return FALSE;
		}
	}

	return TRUE;
}

boolean convex_hull3d_test_point(
	short point_count,
	real_point3d const *points,
	short vertex_count,
	struct vertex3d const *vertices,
	short edge_count,
	struct edge3d const *edges,
	short surface_count,
	struct surface3d const *surfaces,
	real_point3d const *point)
{
	short surface_index;
	boolean result = TRUE;

	for (surface_index = 0; surface_index < surface_count; surface_index++)
	{
		struct surface3d const *surface = surfaces + surface_index;

		if (surface->extant)
		{
			if (surface->plane.n.i*point->x + surface->plane.n.j*point->y + surface->plane.n.k*point->z - surface->plane.d >
				global_convex_hull3d_epsilon)
			{
				result = FALSE;
				break;
			}
		}
	}

	return result;
}

boolean convex_hull3d_test_vector(
	short point_count,
	real_point3d const *points,
	short vertex_count,
	struct vertex3d const *vertices,
	short edge_count,
	struct edge3d const *edges,
	short surface_count,
	struct surface3d const *surfaces,
	real_point3d const *point,
	real_vector3d const *vector,
	real *minimum_distance,
	real *maximum_distance)
{
	real minimum = REAL_MIN;
	real maximum = REAL_MAX;
	short surface_index;

	for (surface_index = 0; surface_index < surface_count; surface_index++)
	{
		struct surface3d const *surface = surfaces + surface_index;

		if (surface->extant)
		{
			real numerator = plane3d_distance_to_point(&surface->plane, point);
			real denominator = dot_product3d(vector, &surface->plane.n);

			if (!(fabs(denominator) < _real_epsilon))
			{
				real distance = -(numerator / denominator);

				if (denominator > 0.f)
				{
					if (minimum < distance)
					{
						minimum = distance;
					}
				}
				else if (maximum > distance)
				{
					maximum = distance;
				}

				if (minimum > maximum)
				{
					return FALSE;
				}
			}
			else if (numerator > global_convex_hull3d_epsilon)
			{
				return FALSE;
			}
		}
	}

	if (minimum_distance)
	{
		*minimum_distance = minimum;
	}
	if (maximum_distance)
	{
		*maximum_distance = maximum;
	}

	return TRUE;
}
