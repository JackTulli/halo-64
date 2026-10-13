/*
RASTERIZER_TRANSPARENT_GEOMETRY.C

symbols in this file:
00173AB0 00a0:
	_rasterizer_transparent_geometry_initialize (0000)
00173B50 0030:
	_rasterizer_transparent_geometry_begin (0000)
00173B80 0030:
	_rasterizer_transparent_geometry_new_group (0000)
00173BB0 0030:
	_rasterizer_transparent_geometry_new_group2 (0000)
00173BE0 0020:
	_rasterizer_transparent_geometry_get_groups2 (0000)
00173C00 00a0:
	_rasterizer_transparent_geometry_next_group (0000)
00173CA0 0010:
	_rasterizer_transparent_geometry_get_groups (0000)
00173CB0 0050:
	_rasterizer_transparent_geometry_get_group_from_presorted_index (0000)
00173D00 00c0:
	_rasterizer_transparent_geometry_get_group_presorted_index (0000)
00173DC0 0040:
	_rasterizer_transparent_geometry_get_group_pending_status (0000)
00173E00 0060:
	_rasterizer_transparent_geometry_set_group_pending_status (0000)
00173E60 0070:
	_rasterizer_transparent_geometry_get_primary_vertex_type (0000)
00173ED0 0010:
	_rasterizer_transparent_geometry_end (0000)
00173EE0 0080:
	_rasterizer_transparent_geometry_dispose (0000)
00173F60 0010:
	_rasterizer_transparent_geometry_stop (0000)
00173F70 0030:
	_rasterizer_sort_internal (0000)
00173FA0 0180:
	_group_sorted_indices_cmpfn (0000)
00174120 00b0:
	_rasterizer_sort_external (0000)
001741D0 01e0:
	_rasterizer_transparent_geometry_draw (0000)
0029F19C 0039:
	??_C@_0DJ@FPHMGBGH@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5tra@ (0000)
0029F1D8 003c:
	??_C@_0DM@PHFAKDJD@c?3?2halo?2SOURCE?2rasterizer?2raster@ (0000)
0029F214 001b:
	??_C@_0BL@MKAECLFF@next_group_sorted_index?$DO?$DN0?$AA@ (0000)
0029F230 004f:
	??_C@_0EP@FBJFLNLL@group?9?$DOsorted_index?$DO?$DN0?5?$CG?$CG?5group?9@ (0000)
0029F280 0053:
	??_C@_0FD@BILDLJMA@group_presorted_index?$DO?$DN0?5?$CG?$CG?5grou@ (0000)
0029F2D8 006f:
	??_C@_0GP@EGPBNBIE@?$CI?$CIunsigned?5long?$CJgroup?9?$CIunsigned?5@ (0000)
0029F348 0035:
	??_C@_0DF@OBMEADDC@?$CD?$CD?$CD?5ERROR?5transparent?5geometry?5g@ (0000)
0029F380 0057:
	??_C@_0FH@DJGEIAGF@group_index2?5?$CG?$CG?5?$CI?$CKgroup_index2?$CJ?$DO@ (0000)
0029F3D8 0057:
	??_C@_0FH@MLHLDOCN@group_index1?5?$CG?$CG?5?$CI?$CKgroup_index1?$CJ?$DO@ (0000)
0029F430 0013:
	??_C@_0BD@KIEBCHPI@?$CBfirst_person_flag?$AA@ (0000)
0029F444 0007:
	??_C@_06OGEFEFJ@?$CBwater?$AA@ (0000)
004B8AD8 0002:
	_transparent_geometry_group_index (0000)
004B8ADC 0030:
	_transparent_geometry_group_pending_flags (0000)
004B8B0C 0004:
	_transparent_geometry_groups (0000)
004B8B10 0004:
	_transparent_geometry_groups2 (0000)
004B8B14 0004:
	_transparent_geometry_group_count (0000)
004B8B18 0004:
	_transparent_geometry_group_count2 (0000)
004B8B1C 0004:
	_transparent_geometry_group_sorted_indices (0000)
004B8B20 0002:
	_transparent_geometry_next_group_sorted_index (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "rasterizer.h"
#include "rasterizer_debug_options.h"
#include "rasterizer_geometry.h"
#include "rasterizer_transparent_geometry.h"
#include "rasterizer/xbox/rasterizer_xbox_internal.h"
#include "rasterizer/xbox/rasterizer_xbox_state.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"

/* ---------- constants */

enum
{
	_shader_type_transparent_water = 7,
	_rasterizer_geometry_first_person_bit = 7,
	_rasterizer_target_render_primary = 0,
};

/* ---------- macros */

/* ---------- structures */

/* January's assert strings name this type and its sorted_index field, and pin
the stride: the group array is walked with a 0xA0 element size. */
struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader *shader;
	byte opaque10[0x44];
	long dynamic_vertex_buffer_index;
	struct vertex_buffer const *vertex_buffer;
	byte opaque5c[0x14];
	real z_sort;
	byte opaque74[0x1C];
	long sorted_index;
	byte opaque94[9];
	boolean cortana_hack;
	byte opaque9e[2];
};

typedef char transparent_geometry_group_size_assert[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];
typedef char transparent_geometry_group_geometry_flags_offset_assert[
	offsetof(struct transparent_geometry_group, geometry_flags) == 0x0 ? 1 : -1];
typedef char transparent_geometry_group_object_index_offset_assert[
	offsetof(struct transparent_geometry_group, object_index) == 0x4 ? 1 : -1];
typedef char transparent_geometry_group_source_object_index_offset_assert[
	offsetof(struct transparent_geometry_group, source_object_index) == 0x8 ? 1 : -1];
typedef char transparent_geometry_group_shader_offset_assert[
	offsetof(struct transparent_geometry_group, shader) == 0xC ? 1 : -1];
typedef char transparent_geometry_group_dynamic_vertex_buffer_index_offset_assert[
	offsetof(struct transparent_geometry_group, dynamic_vertex_buffer_index) == 0x54 ? 1 : -1];
typedef char transparent_geometry_group_vertex_buffer_offset_assert[
	offsetof(struct transparent_geometry_group, vertex_buffer) == 0x58 ? 1 : -1];
typedef char transparent_geometry_group_z_sort_offset_assert[
	offsetof(struct transparent_geometry_group, z_sort) == 0x70 ? 1 : -1];
typedef char transparent_geometry_group_sorted_index_offset_assert[
	offsetof(struct transparent_geometry_group, sorted_index) == 0x90 ? 1 : -1];
typedef char transparent_geometry_group_cortana_hack_offset_assert[
	offsetof(struct transparent_geometry_group, cortana_hack) == 0x9D ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

static short transparent_geometry_group_index = 0;
static unsigned long transparent_geometry_group_pending_flags[
	BIT_VECTOR_SIZE_IN_LONGS(RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS)] = { 0 };
static struct transparent_geometry_group *transparent_geometry_groups = NULL;
static struct transparent_geometry_group *transparent_geometry_groups2 = NULL;
static long transparent_geometry_group_count = 0;
static long transparent_geometry_group_count2 = 0;
static short *transparent_geometry_group_sorted_indices = NULL;
static short transparent_geometry_next_group_sorted_index = 0;

extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

boolean rasterizer_transparent_geometry_initialize(
	void)
{
	boolean success = TRUE;

	transparent_geometry_groups = debug_malloc(
		RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS * sizeof(struct transparent_geometry_group),
		FALSE, "c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x29);
	transparent_geometry_group_sorted_indices = debug_malloc(
		RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS * sizeof(short),
		FALSE, "c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x2B);
	transparent_geometry_groups2 = debug_malloc(
		RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS2 * sizeof(struct transparent_geometry_group),
		FALSE, "c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x2E);

	transparent_geometry_group_count2 = 0;
	transparent_geometry_group_count = 0;

	if (!transparent_geometry_groups ||
		!transparent_geometry_group_sorted_indices ||
		!transparent_geometry_groups2)
	{
		error(_error_silent, "### ERROR failed to allocate transparent geometry buffer");
		success = FALSE;
	}

	success = success && rasterizer_transparent_geometry_initialize_aux_buffer();

	return success;
}

void rasterizer_transparent_geometry_begin(
	void)
{
	transparent_geometry_group_count = 0;
	transparent_geometry_next_group_sorted_index = 0;
	memset(transparent_geometry_group_pending_flags, 0,
		sizeof(transparent_geometry_group_pending_flags));
	transparent_geometry_group_count2 = 0;

	return;
}

void rasterizer_transparent_geometry_end(
	void)
{
	return;
}

short rasterizer_transparent_geometry_get_primary_vertex_type(
	struct transparent_geometry_group const *group)
{
	short vertex_type = NONE;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0xF4, group);

	if (group->vertex_buffer)
	{
		vertex_type = group->vertex_buffer->type;
	}
	else if (group->dynamic_vertex_buffer_index!=NONE)
	{
		vertex_type = rasterizer_dynamic_vertices_get_type(group->dynamic_vertex_buffer_index);
	}
	else
	{
		error(_error_silent, "### ERROR transparent geometry group has no vertices");
	}

	return vertex_type;
}

void rasterizer_transparent_geometry_set_group_pending_status(
	struct transparent_geometry_group const *group,
	boolean pending)
{
	short group_presorted_index = rasterizer_transparent_geometry_get_group_presorted_index(group);

	if (group_presorted_index!=NONE)
		BIT_VECTOR_SET_FLAG(transparent_geometry_group_pending_flags, group_presorted_index, !pending);

	return;
}

boolean rasterizer_transparent_geometry_get_group_pending_status(
	struct transparent_geometry_group const *group)
{
	short group_presorted_index = rasterizer_transparent_geometry_get_group_presorted_index(group);
	boolean pending = TRUE;

	if (group_presorted_index!=NONE)
		pending = !BIT_VECTOR_TEST_FLAG(transparent_geometry_group_pending_flags, group_presorted_index);

	return pending;
}

short rasterizer_transparent_geometry_get_group_presorted_index(
	struct transparent_geometry_group const *group)
{
	short group_presorted_index = NONE;

	if (group>=transparent_geometry_groups &&
		group<transparent_geometry_groups+transparent_geometry_group_count)
	{
		group_presorted_index = (short)(group-transparent_geometry_groups);

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0xCB,
			group_presorted_index>=0 && group_presorted_index<transparent_geometry_group_count);
		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0xCC,
			((unsigned long)group-(unsigned long)transparent_geometry_groups)%sizeof(struct transparent_geometry_group)==0);
	}

	return group_presorted_index;
}

void *rasterizer_transparent_geometry_get_group_from_presorted_index(
	short group_presorted_index)
{
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0xBC,
		group_presorted_index>=0 && group_presorted_index<transparent_geometry_group_count);

	return (byte *)transparent_geometry_groups + group_presorted_index*0xA0;
}

void *rasterizer_transparent_geometry_get_groups2(
	short *group_count)
{
	if (group_count)
		*group_count = (short)transparent_geometry_group_count2;

	return transparent_geometry_groups2;
}

struct transparent_geometry_group *rasterizer_transparent_geometry_next_group(
	struct transparent_geometry_group const *group)
{
	short next_group_sorted_index;
	short group_sorted_index;
	struct transparent_geometry_group *next_group = (struct transparent_geometry_group *)group;

	if (group)
	{
		group_sorted_index = (short)group->sorted_index;
		next_group_sorted_index = (short)(group_sorted_index + 1);

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x89,
			group->sorted_index>=0 && group->sorted_index<transparent_geometry_group_count);

		if (next_group_sorted_index<transparent_geometry_group_count)
		{
			match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x8D,
				next_group_sorted_index>=0);

			next_group = transparent_geometry_groups +
				transparent_geometry_group_sorted_indices[next_group_sorted_index];
		}
		else
		{
			next_group = NULL;
		}
	}

	return next_group;
}

void *rasterizer_transparent_geometry_get_groups(
	void)
{
	return transparent_geometry_groups;
}

struct transparent_geometry_group *rasterizer_transparent_geometry_new_group(
	void)
{
	struct transparent_geometry_group *group = NULL;
	long group_index = transparent_geometry_group_count;

	if (group_index<RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS)
	{
		group = transparent_geometry_groups + group_index;
		group->sorted_index = group_index;
		transparent_geometry_group_count = group_index + 1;
	}

	return group;
}

struct transparent_geometry_group *rasterizer_transparent_geometry_new_group2(
	void)
{
	struct transparent_geometry_group *group = NULL;
	long group_index = transparent_geometry_group_count2;

	if (group_index<RASTERIZER_MAXIMUM_TRANSPARENT_GEOMETRY_GROUPS2)
	{
		group = transparent_geometry_groups2 + group_index;
		group->sorted_index = group_index;
		transparent_geometry_group_count2 = group_index + 1;
	}

	return group;
}

void rasterizer_transparent_geometry_stop(
	void)
{
	rasterizer_set_stencil_mode(0);

	return;
}

void rasterizer_transparent_geometry_dispose(
	void)
{
	rasterizer_transparent_geometry_dispose_aux_buffer();

	if (transparent_geometry_groups)
		debug_free(transparent_geometry_groups,
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x111);
	transparent_geometry_groups = NULL;

	if (transparent_geometry_group_sorted_indices)
		debug_free(transparent_geometry_group_sorted_indices,
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x114);
	transparent_geometry_group_sorted_indices = NULL;

	if (transparent_geometry_groups2)
		debug_free(transparent_geometry_groups2,
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c", 0x118);
	transparent_geometry_groups2 = NULL;

	transparent_geometry_group_count2 = 0;
	transparent_geometry_group_count = 0;

	return;
}

/* ---------- private code */

static void rasterizer_sort_internal(
	struct transparent_geometry_group *group)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
		0x192,
		group);

	return;
}

int __cdecl group_sorted_indices_cmpfn(
	void const *group_index1_pointer,
	void const *group_index2_pointer)
{
	short const *group_index1 = group_index1_pointer;
	short const *group_index2 = group_index2_pointer;
	struct transparent_geometry_group *group1;
	struct transparent_geometry_group *group2;
	long comparison = 0;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
		0x1AA,
		group_index1 && (*group_index1)>=0 && (*group_index1)<transparent_geometry_group_count);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
		0x1AB,
		group_index2 && (*group_index2)>=0 && (*group_index2)<transparent_geometry_group_count);

	group1 = transparent_geometry_groups + *group_index1;
	group2 = transparent_geometry_groups + *group_index2;

	if (shader_is_water_decal(group1->shader))
	{
		comparison = -1;
		goto cortana_tiebreak;
	}
	if (shader_is_water_decal(group2->shader))
	{
		comparison = 1;
		goto cortana_tiebreak;
	}

	if (!group1->shader || group1->shader->base.type!=_shader_type_transparent_water)
	{
		if (group2->shader && group2->shader->base.type==_shader_type_transparent_water)
		{
			comparison = 1;
			goto cortana_tiebreak;
		}

		if ((group1->geometry_flags & FLAG(_rasterizer_geometry_first_person_bit)) &&
			!(group2->geometry_flags & FLAG(_rasterizer_geometry_first_person_bit)))
		{
			comparison = 1;
			goto cortana_tiebreak;
		}

		if (!(group2->geometry_flags & FLAG(_rasterizer_geometry_first_person_bit)) ||
			(group1->geometry_flags & FLAG(_rasterizer_geometry_first_person_bit)))
		{
			if (group1->z_sort>group2->z_sort)
			{
				comparison = 1;
				goto cortana_tiebreak;
			}
			if (!(group1->z_sort<group2->z_sort))
			{
				if (group1->source_object_index>group2->source_object_index)
				{
					comparison = 1;
					goto cortana_tiebreak;
				}
				if (group1->source_object_index>=group2->source_object_index)
				{
					goto cortana_tiebreak;
				}
			}
		}
	}
	comparison = -1;

	cortana_tiebreak:
	if (group1->cortana_hack && !group2->cortana_hack)
	{
		return 1;
	}
	if (group2->cortana_hack && !group1->cortana_hack)
	{
		return -1;
	}

	return comparison;
}

void rasterizer_sort_external(
	void)
{
	short group_index;

	for (group_index = 0; group_index<transparent_geometry_group_count; group_index++)
	{
		rasterizer_sort_internal(transparent_geometry_groups + group_index);
		transparent_geometry_group_sorted_indices[group_index] = group_index;
	}

	qsort(
		transparent_geometry_group_sorted_indices,
		transparent_geometry_group_count,
		sizeof(*transparent_geometry_group_sorted_indices),
		group_sorted_indices_cmpfn);

	for (group_index = 0; group_index<transparent_geometry_group_count; group_index++)
	{
		transparent_geometry_groups[transparent_geometry_group_sorted_indices[group_index]].sorted_index = group_index;
	}

	return;
}

void rasterizer_transparent_geometry_draw(
	boolean water)
{
	long profile = water
		? _rasterizer_profile_water
		: _rasterizer_profile_queued_transparents;

	rasterizer_profile_begin((short)profile);
	if (transparent_geometry_group_count > 0)
	{
		boolean first_person_flag = FALSE;

		if (water)
		{
			rasterizer_sort_external();
			transparent_geometry_group_index = 0;
			if (global_window_parameters.window_index != NONE)
			{
				rasterizer_debug_options.transparent_pixel_counter_active = TRUE;
			}
		}

		rasterizer_transparent_geometry_groups_begin();
		rasterizer_debug_options.transparent_pixel_counter_active = FALSE;
		while (transparent_geometry_group_index < transparent_geometry_group_count)
		{
			struct transparent_geometry_group *group =
				transparent_geometry_groups +
				transparent_geometry_group_sorted_indices[
					transparent_geometry_group_index];

			if (water)
			{
				struct shader *shader = group->shader;

				if (!shader ||
					(shader->base.type != _shader_type_transparent_water &&
						!shader_is_water_decal(shader)))
				{
					break;
				}
			}

			if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
			{
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
					0x154,
					!water);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
					0x155,
					global_window_parameters.rasterizer_target==_rasterizer_target_render_primary);
				if (!first_person_flag)
				{
					rasterizer_set_stencil_mode(0);
					rasterizer_set_frustum_z(
						rasterizer_globals.first_person_weapon_near_clip_distance,
						rasterizer_globals.first_person_weapon_far_clip_distance);
					first_person_flag = TRUE;
				}
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\rasterizer_transparent_geometry.c",
					0x163,
					!first_person_flag);
			}

			rasterizer_transparent_geometry_group_draw(group, FALSE);
			transparent_geometry_group_index++;
		}

		if (!water && global_window_parameters.window_index != NONE)
		{
			rasterizer_debug_options.transparent_pixel_counter_active = TRUE;
		}
		rasterizer_transparent_geometry_groups_end();
		rasterizer_debug_options.transparent_pixel_counter_active = FALSE;
		if (first_person_flag)
		{
			rasterizer_set_frustum_z(0.0f, 0.0f);
		}
	}
	rasterizer_profile_end((short)profile);

	return;
}
