/*
RASTERIZER_DEBUG.C

symbols in this file:
0016D760 0070:
	_rasterizer_debug_new_primitive (0000)
0016D7D0 0090:
	_rasterizer_debug_initialize (0000)
0016D860 0020:
	_rasterizer_debug_begin (0000)
0016D880 0010:
	_rasterizer_debug_end (0000)
0016D890 00e0:
	_rasterizer_debug_dispose (0000)
0016D970 0010:
	_rasterizer_debug_test (0000)
0016D980 0060:
	_code_0016d980 (0000)
0016D9E0 0420:
	_rasterizer_debug_draw (0000)
0016DE00 0250:
	_rasterizer_debug_line_shaded (0000)
0016E050 0310:
	_rasterizer_debug_triangle_shaded (0000)
0016E360 0020:
	_rasterizer_debug_line (0000)
0016E380 0020:
	_rasterizer_debug_triangle (0000)
0029D990 002b:
	??_C@_0CL@IMCIMOMN@?$CD?$CD?$CD?5WARNING?5debug?5geometry?5buffe@ (0000)
0029D9BC 002b:
	??_C@_0CL@HKCMJLKE@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5deb@ (0000)
0029D9E8 002d:
	??_C@_0CN@HHLFKIED@c?3?2halo?2SOURCE?2rasterizer?2raster@ (0000)
0029DA18 0021:
	??_C@_0CB@NDEAGFJM@debug_data?4non_opaque_primitives@ (0000)
0029DA3C 0018:
	??_C@_0BI@INNMENHJ@debug_data?4opaque_lines?$AA@ (0000)
0029DA54 001c:
	??_C@_0BM@JLDDGFOM@debug_data?4opaque_triangles?$AA@ (0000)
0029DA70 0045:
	??_C@_0EF@HLJMNMGB@?$CD?$CD?$CD?5ERROR?5failed?5to?5lock?5dynamic@ (0000)
0029DAB8 0041:
	??_C@_0EB@BGLDJEMO@debug_data?4primitive_count?5?$DM?$DNRAS@ (0000)
0029DB00 004b:
	??_C@_0EL@KLLHFFMI@debug_data?4non_opaque_primitive_@ (0000)
0029DB50 0043:
	??_C@_0ED@KEPEIIND@debug_data?4opaque_line_count?5?$DM?$DNR@ (0000)
0029DB98 0047:
	??_C@_0EH@DHPFKGPL@debug_data?4opaque_triangle_count@ (0000)
0029DBE0 001d:
	??_C@_0BN@KMMBIEHE@p0?5?$CG?$CG?5p1?5?$CG?$CG?5color0?5?$CG?$CG?5color1?$AA@ (0000)
0029DC00 002d:
	??_C@_0CN@JOAFNCEE@p0?5?$CG?$CG?5p1?5?$CG?$CG?5p2?5?$CG?$CG?5color0?5?$CG?$CG?5colo@ (0000)
004662F8 0021:
	_bss_004662f8 (0000)
*/

/* ---------- headers */

#include "rasterizer/rasterizer_frame_statistics.h"
#include <stdlib.h>

#include "cseries.h"
#include "errors.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "rasterizer.h"
#include "rasterizer_debug.h"
#include "rasterizer_geometry.h"
#include "rasterizer/xbox/rasterizer_xbox_debug.h"
#include "rasterizer/xbox/rasterizer_xbox_draw_primitives.h"

/* ---------- constants */

enum
{
#ifdef HALO_XBOX
	/* the console's memory: 1024 debug primitives, a fifth of the memory */
	MAXIMUM_DEBUG_PRIMITIVES = 1024,
#else
	MAXIMUM_DEBUG_PRIMITIVES = 8192,
#endif
	MAXIMUM_VERTICES_PER_DEBUG_PRIMITIVE = 3
};

enum
{
	RASTERIZER_DEBUG_LINE_ZBIAS = 16
};

enum
{
	_rasterizer_stats_none = 0,
	_rasterizer_stats_summary,
	_rasterizer_stats_geometry,
	NUMBER_OF_RASTERIZER_STATS_MODES
};

/* ---------- macros */

/* ---------- structures */

struct rasterizer_debug_vertex
{
	real_point3d point;
	pixel32 color;
};

struct rasterizer_debug_primitive
{
	struct rasterizer_debug_vertex vertices[MAXIMUM_VERTICES_PER_DEBUG_PRIMITIVE];
	short vertex_count;
	real distance;
	boolean opaque;
};

#pragma pack(push, 1)
struct rasterizer_debug_data
{
	boolean initialized;
	byte pad1[3];
	struct rasterizer_debug_primitive *opaque_triangles;
	long opaque_triangle_count;
	struct rasterizer_debug_primitive *opaque_lines;
	long opaque_line_count;
	struct rasterizer_debug_primitive *non_opaque_primitives;
	long non_opaque_primitive_count;
	long primitive_count;
	boolean geometry_buffer_full_warning;
};
#pragma pack(pop)

struct rasterizer_debug_options_prefix
{
	byte reserved00[2];
	short stats;
	byte reserved04[0x21];
	boolean debug_geometry;
};

typedef char rasterizer_debug_data_size_assert[
	sizeof(struct rasterizer_debug_data) == 0x21 ? 1 : -1];
typedef char rasterizer_debug_primitive_size_assert[
	sizeof(struct rasterizer_debug_primitive) == 0x3C ? 1 : -1];
typedef char rasterizer_debug_options_debug_geometry_offset_assert[
	offsetof(struct rasterizer_debug_options_prefix, debug_geometry) == 0x25 ? 1 : -1];

/* ---------- prototypes */

static int __cdecl rasterizer_debug_compare_primitives(
	void const *primitive0_pointer,
	void const *primitive1_pointer);
long rasterizer_debug_new_primitive(
	long *primitive_count);
void rasterizer_debug_line_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_argb_color const *color0,
	real_argb_color const *color1);
void rasterizer_debug_triangle_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_argb_color const *color0,
	real_argb_color const *color1,
	real_argb_color const *color2);

/* ---------- globals */

static struct rasterizer_debug_data debug_data;

extern struct rasterizer_debug_options_prefix rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

long rasterizer_debug_new_primitive(
	long *primitive_count)
{
	long primitive_index = NONE;

	if (*primitive_count<MAXIMUM_DEBUG_PRIMITIVES &&
		debug_data.primitive_count<MAXIMUM_DEBUG_PRIMITIVES)
	{
		primitive_index = (*primitive_count)++;
		debug_data.primitive_count++;

		if (rasterizer_debug_options.stats==_rasterizer_stats_geometry)
		{
			rasterizer_frame_statistics.debug_primitive_count++;
		}
	}
	else if (!debug_data.geometry_buffer_full_warning)
	{
		error(_error_silent, "### WARNING debug geometry buffer overflow");
		debug_data.geometry_buffer_full_warning = TRUE;
	}

	return primitive_index;
}

boolean rasterizer_debug_initialize(
	void)
{
	boolean success = TRUE;

	debug_data.opaque_triangles = match_malloc(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
		96,
		sizeof(struct rasterizer_debug_primitive)*MAXIMUM_DEBUG_PRIMITIVES);
	debug_data.opaque_lines = match_malloc(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
		97,
		sizeof(struct rasterizer_debug_primitive)*MAXIMUM_DEBUG_PRIMITIVES);
	debug_data.non_opaque_primitives = match_malloc(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
		98,
		sizeof(struct rasterizer_debug_primitive)*MAXIMUM_DEBUG_PRIMITIVES);

	if (debug_data.opaque_triangles &&
		debug_data.opaque_lines &&
		debug_data.non_opaque_primitives)
	{
		debug_data.initialized = success;
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate debug buffers");
		success = FALSE;
		debug_data.initialized = success;
	}

	return success;
}

void rasterizer_debug_begin(
	void)
{
	debug_data.opaque_triangle_count = 0;
	debug_data.opaque_line_count = 0;
	debug_data.non_opaque_primitive_count = 0;
	debug_data.primitive_count = 0;

	return;
}

void rasterizer_debug_end(
	void)
{
	return;
}

void rasterizer_debug_dispose(
	void)
{
	if (debug_data.initialized)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			137,
			debug_data.opaque_triangles);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			138,
			debug_data.opaque_lines);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			139,
			debug_data.non_opaque_primitives);

		match_free(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			141,
			debug_data.opaque_triangles);
		match_free(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			142,
			debug_data.opaque_lines);
		match_free(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			143,
			debug_data.non_opaque_primitives);

		debug_data.initialized = FALSE;
	}

	return;
}

void rasterizer_debug_test(
	void)
{
	return;
}

static int __cdecl rasterizer_debug_compare_primitives(
	void const *primitive0_pointer,
	void const *primitive1_pointer)
{
	struct rasterizer_debug_primitive const *primitive0 = primitive0_pointer;
	struct rasterizer_debug_primitive const *primitive1 = primitive1_pointer;
	long result = 0;

	if (!primitive0->opaque && !primitive1->opaque)
	{
		if (primitive0->distance>primitive1->distance)
		{
			result = 1;
		}

		if (primitive0->distance<primitive1->distance)
		{
			result = -1;
		}
	}
	else
	{
		if (primitive0->opaque)
		{
			result-= primitive0->vertex_count;
		}

		if (primitive1->opaque)
		{
			result+= primitive1->vertex_count;
		}
	}

	return result;
}

void rasterizer_debug_draw(
	void)
{
	boolean success = TRUE;

	if (debug_data.initialized &&
		debug_data.primitive_count>0 &&
		rasterizer_debug_options.debug_geometry)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			320,
			debug_data.opaque_triangles);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			321,
			debug_data.opaque_lines);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			322,
			debug_data.non_opaque_primitives);
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			323,
			debug_data.opaque_triangle_count<=MAXIMUM_DEBUG_PRIMITIVES,
			"debug_data.opaque_triangle_count <=RASTERIZER_MAXIMUM_DEBUG_PRIMITIVES");
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			324,
			debug_data.opaque_line_count<=MAXIMUM_DEBUG_PRIMITIVES,
			"debug_data.opaque_line_count <=RASTERIZER_MAXIMUM_DEBUG_PRIMITIVES");
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			325,
			debug_data.non_opaque_primitive_count<=MAXIMUM_DEBUG_PRIMITIVES,
			"debug_data.non_opaque_primitive_count<=RASTERIZER_MAXIMUM_DEBUG_PRIMITIVES");
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			326,
			debug_data.primitive_count<=MAXIMUM_DEBUG_PRIMITIVES,
			"debug_data.primitive_count <=RASTERIZER_MAXIMUM_DEBUG_PRIMITIVES");

		qsort(
			debug_data.non_opaque_primitives,
			debug_data.non_opaque_primitive_count,
			sizeof(struct rasterizer_debug_primitive),
			rasterizer_debug_compare_primitives);

		rasterizer_globals.current_lock_operation = _rasterizer_lock_debug;

		if (debug_data.opaque_triangle_count>0)
		{
			long vertex_buffer_index = rasterizer_dynamic_vertices_new(
				_rasterizer_vertex_type_debug,
				debug_data.opaque_triangle_count*NUMBER_OF_VERTICES_PER_TRIANGLE);

			if (vertex_buffer_index!=NONE)
			{
				struct rasterizer_debug_vertex *vertices =
					rasterizer_dynamic_vertices_lock(vertex_buffer_index);

				if (vertices)
				{
					long triangle_count = debug_data.opaque_triangle_count;
					long vertex_index = 0;
					long primitive_index;

					for (primitive_index = 0; primitive_index<triangle_count; primitive_index++)
					{
						struct rasterizer_debug_primitive const *primitive =
							&debug_data.opaque_triangles[primitive_index];

						memcpy(
							&vertices[vertex_index],
							primitive->vertices,
							primitive->vertex_count*sizeof(struct rasterizer_debug_vertex));
						vertex_index+= primitive->vertex_count;
					}

					rasterizer_dynamic_vertices_unlock(vertex_buffer_index);
					rasterizer_debug_drawing_begin(TRUE, 0);
					rasterizer_draw_dynamic_vertices(
						0,
						triangle_count,
						vertex_buffer_index,
						NUMBER_OF_VERTICES_PER_TRIANGLE);
					rasterizer_debug_drawing_end();
				}
				else
				{
					error(_error_silent, "### ERROR failed to lock dynamic vertex buffers for debug primitives");
					success = FALSE;
				}

				rasterizer_dynamic_vertices_delete(vertex_buffer_index);
			}
			else
			{
				success = FALSE;
			}
		}

		if (success && debug_data.opaque_line_count>0)
		{
			long vertex_buffer_index = rasterizer_dynamic_vertices_new(
				_rasterizer_vertex_type_debug,
				debug_data.opaque_line_count*NUMBER_OF_VERTICES_PER_LINE);

			if (vertex_buffer_index!=NONE)
			{
				struct rasterizer_debug_vertex *vertices =
					rasterizer_dynamic_vertices_lock(vertex_buffer_index);

				if (vertices)
				{
					long line_count = debug_data.opaque_line_count;
					long vertex_index = 0;
					long primitive_index;

					for (primitive_index = 0; primitive_index<line_count; primitive_index++)
					{
						struct rasterizer_debug_primitive const *primitive =
							&debug_data.opaque_lines[primitive_index];

						memcpy(
							&vertices[vertex_index],
							primitive->vertices,
							primitive->vertex_count*sizeof(struct rasterizer_debug_vertex));
						vertex_index+= primitive->vertex_count;
					}

					rasterizer_dynamic_vertices_unlock(vertex_buffer_index);
					rasterizer_debug_drawing_begin(TRUE, RASTERIZER_DEBUG_LINE_ZBIAS);
					rasterizer_draw_dynamic_vertices(
						0,
						line_count,
						vertex_buffer_index,
						NUMBER_OF_VERTICES_PER_LINE);
					rasterizer_debug_drawing_end();
				}
				else
				{
					error(_error_silent, "### ERROR failed to lock dynamic vertex buffers for debug primitives");
					success = FALSE;
				}

				rasterizer_dynamic_vertices_delete(vertex_buffer_index);
			}
			else
			{
				success = FALSE;
			}
		}

		{
			long primitive_index;

			for (primitive_index = 0; success && primitive_index<debug_data.non_opaque_primitive_count; primitive_index++)
			{
				struct rasterizer_debug_primitive const *primitive =
					&debug_data.non_opaque_primitives[primitive_index];
				long vertex_buffer_index = rasterizer_dynamic_vertices_new(
					_rasterizer_vertex_type_debug,
					primitive->vertex_count);

				if (vertex_buffer_index!=NONE)
				{
					struct rasterizer_debug_vertex *vertices =
						rasterizer_dynamic_vertices_lock(vertex_buffer_index);

					if (vertices)
					{
						memcpy(
							vertices,
							primitive->vertices,
							primitive->vertex_count*sizeof(struct rasterizer_debug_vertex));
						rasterizer_dynamic_vertices_unlock(vertex_buffer_index);
						rasterizer_debug_drawing_begin(FALSE, 0);
						rasterizer_draw_dynamic_vertices(
							0,
							1,
							vertex_buffer_index,
							primitive->vertex_count);
						rasterizer_debug_drawing_end();
					}
					else
					{
						error(_error_silent, "### ERROR failed to lock dynamic vertex buffers for debug primitives");
						success = FALSE;
					}

					rasterizer_dynamic_vertices_delete(vertex_buffer_index);
				}
				else
				{
					success = FALSE;
				}
			}
		}

		rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
	}

	return;
}

void rasterizer_debug_line_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_argb_color const *color0,
	real_argb_color const *color1)
{
	if (debug_data.initialized && rasterizer_debug_options.debug_geometry)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			171,
			p0 && p1 && color0 && color1);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			172,
			debug_data.opaque_triangles);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			173,
			debug_data.opaque_lines);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			174,
			debug_data.non_opaque_primitives);

		if (color0->alpha>0.0f || color1->alpha>0.0f)
		{
			boolean opaque = color0->alpha==1.0f && color1->alpha==1.0f;
			long primitive_index = rasterizer_debug_new_primitive(opaque ?
				&debug_data.opaque_line_count :
				&debug_data.non_opaque_primitive_count);

			if (primitive_index!=NONE)
			{
				struct rasterizer_debug_primitive *primitive = opaque ?
					&debug_data.opaque_lines[primitive_index] :
					&debug_data.non_opaque_primitives[primitive_index];
				real_vector3d delta0;
				real_vector3d delta1;

				delta0.i = global_window_parameters.camera.position.x-p0->x;
				delta0.j = global_window_parameters.camera.position.y-p0->y;
				delta0.k = global_window_parameters.camera.position.z-p0->z;
				delta1.i = global_window_parameters.camera.position.x-p1->x;
				delta1.j = global_window_parameters.camera.position.y-p1->y;
				delta1.k = global_window_parameters.camera.position.z-p1->z;

				primitive->vertex_count = 2;
				primitive->vertices[0].point = *p0;
				primitive->vertices[1].point = *p1;
				primitive->vertices[0].color = real_argb_color_to_pixel32(color0);
				primitive->vertices[1].color = real_argb_color_to_pixel32(color1);

				primitive->distance = MIN(
					dot_product3d(&global_window_parameters.camera.forward, &delta0),
					dot_product3d(&global_window_parameters.camera.forward, &delta1));
				primitive->opaque = opaque;
			}
		}
	}

	return;
}

void rasterizer_debug_triangle_shaded(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_argb_color const *color0,
	real_argb_color const *color1,
	real_argb_color const *color2)
{
	if (debug_data.initialized && rasterizer_debug_options.debug_geometry)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			229,
			p0 && p1 && p2 && color0 && color1 && color2);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			230,
			debug_data.opaque_triangles);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			231,
			debug_data.opaque_lines);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_debug.c",
			232,
			debug_data.non_opaque_primitives);

		if (color0->alpha>0.0f || color1->alpha>0.0f || color2->alpha>0.0f)
		{
			boolean opaque = color0->alpha==1.0f && color1->alpha==1.0f && color2->alpha==1.0f;
			long primitive_index = rasterizer_debug_new_primitive(opaque ?
				&debug_data.opaque_triangle_count :
				&debug_data.non_opaque_primitive_count);

			if (primitive_index!=NONE)
			{
				struct rasterizer_debug_primitive *primitive = opaque ?
					&debug_data.opaque_triangles[primitive_index] :
					&debug_data.non_opaque_primitives[primitive_index];
				real_vector3d delta0;
				real_vector3d delta1;
				real_vector3d delta2;

				delta0.i = global_window_parameters.camera.position.x-p0->x;
				delta0.j = global_window_parameters.camera.position.y-p0->y;
				delta0.k = global_window_parameters.camera.position.z-p0->z;
				delta1.i = global_window_parameters.camera.position.x-p1->x;
				delta1.j = global_window_parameters.camera.position.y-p1->y;
				delta1.k = global_window_parameters.camera.position.z-p1->z;
				delta2.i = global_window_parameters.camera.position.x-p2->x;
				delta2.j = global_window_parameters.camera.position.y-p2->y;
				delta2.k = global_window_parameters.camera.position.z-p2->z;

				primitive->vertex_count = 3;
				primitive->vertices[0].point = *p0;
				primitive->vertices[1].point = *p1;
				primitive->vertices[2].point = *p2;
				primitive->vertices[0].color = real_argb_color_to_pixel32(color0);
				primitive->vertices[1].color = real_argb_color_to_pixel32(color1);
				primitive->vertices[2].color = real_argb_color_to_pixel32(color2);

				primitive->distance = MIN(
					dot_product3d(&global_window_parameters.camera.forward, &delta0),
					MIN(
						dot_product3d(&global_window_parameters.camera.forward, &delta1),
						dot_product3d(&global_window_parameters.camera.forward, &delta2)));
				primitive->opaque = opaque;
			}
		}
	}

	return;
}

void rasterizer_debug_line(
	real_point3d const *p0,
	real_point3d const *p1,
	real_argb_color const *color)
{
	rasterizer_debug_line_shaded(
		p0,
		p1,
		color,
		color);

	return;
}

void rasterizer_debug_triangle(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_argb_color const *color)
{
	rasterizer_debug_triangle_shaded(
		p0,
		p1,
		p2,
		color,
		color,
		color);

	return;
}

/* ---------- private code */
