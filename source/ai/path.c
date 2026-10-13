/*
PATH.C

symbols in this file:
0004C5F0 0010:
	_paths_initialize (0000)
0004C600 0010:
	_paths_dispose (0000)
0004C610 0010:
	_paths_initialize_for_new_map (0000)
0004C620 0010:
	_paths_dispose_from_old_map (0000)
0004C630 0030:
	_path_input_new (0000)
0004C660 0010:
	_path_input_set_target_object (0000)
0004C670 0030:
	_path_input_set_start (0000)
0004C6A0 0040:
	_path_input_set_attractor (0000)
0004C6E0 0020:
	_path_input_set_search_bounds (0000)
0004C700 0040:
	_path_state_new (0000)
0004C740 0030:
	_path_state_destination (0000)
0004C770 0040:
	_code_0004c770 (0000)
0004C7B0 0010:
	_code_0004c7b0 (0000)
0004C7C0 01e0:
	_code_0004c7c0 (0000)
0004C9A0 0230:
	_code_0004c9a0 (0000)
0004CBD0 0120:
	_code_0004cbd0 (0000)
0004CCF0 0080:
	_code_0004ccf0 (0000)
0004CD70 0060:
	_code_0004cd70 (0000)
0004CDD0 0080:
	_path_get_node (0000)
0004CE50 0050:
	_path_node_from_hash_table (0000)
0004CEA0 00f0:
	_path_3d_available (0000)
0004CF90 0090:
	_path_3d_build_path (0000)
0004D020 0130:
	_path_state_approach_point (0000)
0004D150 04a0:
	_path_state_build_path (0000)
0004D5F0 0250:
	_code_0004d5f0 (0000)
0004D840 0070:
	_code_0004d840 (0000)
0004D8B0 0180:
	_code_0004d8b0 (0000)
0004DA30 00d0:
	_closest_point_to_attractor (0000)
0004DB00 00c0:
	_path_attractor_weight (0000)
0004DBC0 01f0:
	_path_state_estimated_distance (0000)
0004DDB0 0830:
	_code_0004ddb0 (0000)
0004E5E0 0100:
	_path_state_find (0000)
0024DAB8 0054:
	??_C@_0FE@EPMGNKHK@state?9?$DOnode_list?$FLparent_node_ind@ (0000)
0024DB10 0045:
	??_C@_0EF@LADBNEPJ@state?9?$DOnode_list?$FLparent_node_ind@ (0000)
0024DB58 0046:
	??_C@_0EG@NGIKIECC@?$CIparent_node_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIpar@ (0000)
0024DBA0 0042:
	??_C@_0EC@FLANNGD@state?9?$DOnode_list?$FLnode_index?$FN?4qua@ (0000)
0024DBE4 0038:
	??_C@_0DI@LCDBFPAM@?$CInode_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CInode_index@ (0000)
0024DC1C 003f:
	??_C@_0DP@ONCFDIOO@?$CIheap_location?5?$DO?$DN?51?$CJ?5?$CG?$CG?5?$CIheap_lo@ (0000)
0024DC5C 0019:
	??_C@_0BJ@IILBBPFP@c?3?2halo?2SOURCE?2ai?2path?4c?$AA@ (0000)
0024DC78 0049:
	??_C@_0EJ@NPIFJABE@state?9?$DOnode_list?$FLchild_node_inde@ (0000)
0024DCC8 0048:
	??_C@_0EI@JGENJHLI@state?9?$DOnode_list?$FLchild_node_inde@ (0000)
0024DD10 0044:
	??_C@_0EE@LMKECELL@?$CIchild_node_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIchil@ (0000)
0024DD58 005f:
	??_C@_0FP@HHDGGKGA@state?9?$DOnode_list?$FLnode_index?$FN?4qua@ (0000)
0024DDB8 0030:
	??_C@_0DA@HHBOKDMG@state?9?$DOnode_list?$FLnode_index?$FN?4hea@ (0000)
0024DDE8 0017:
	??_C@_0BH@BOHECKL@state?9?$DOheap_count?5?$DO?$DN?51?$AA@ (0000)
0024DE00 002e:
	??_C@_0CO@NBKNGICB@path_heap_insert?3?5overflowed?5sta@ (0000)
0024DE30 0036:
	??_C@_0DG@MBNMAAEM@?$CInode_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CInode_index@ (0000)
0024DE68 0013:
	??_C@_0BD@MBAKELBN@node_index?5?$CB?$DN?5NONE?$AA@ (0000)
0024DE7C 0018:
	??_C@_0BI@PECADNDG@straight_line_reference?$AA@ (0000)
0024DE94 0019:
	??_C@_0BJ@GLKEJCKE@approach_point_reference?$AA@ (0000)
0024DEB0 003b:
	??_C@_0DL@MMNOICOM@state?9?$DOdebug?9?$DOpath_build_result?5@ (0000)
0024DEEC 0017:
	??_C@_0BH@ICOJEHHO@child_node?9?$DOdepth?5?$DN?$DN?50?$AA@ (0000)
0024DF04 0019:
	??_C@_0BJ@CFIDHICE@child_node_index?5?$CB?$DN?5NONE?$AA@ (0000)
0024DF20 0025:
	??_C@_0CF@EODIHIDD@node?9?$DOdepth?5?$DN?$DN?5child_node?9?$DOdepth@ (0000)
0024DF48 0035:
	??_C@_0DF@MJJJFNJI@?$CInode?9?$DOdepth?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CInode?9?$DOdep@ (0000)
0024DF80 005a:
	??_C@_0FK@GBFFKFKH@?$CIinitial_node?9?$DOsurface_index?5?$DO?$DN?5@ (0000)
0024DFDC 0017:
	??_C@_0BH@EGEJGGGM@state?9?$DOnode_count?5?$DN?$DN?50?$AA@ (0000)
0024DFF8 0078:
	??_C@_0HI@IDMLJPKG@pathfinding?3?5attempted?5to?5build?5@ (0000)
0024E070 0064:
	??_C@_0GE@LNDFFALH@?$CIstate?9?$DOinput?4start_surface_inde@ (0000)
0024E0D8 005c:
	??_C@_0FM@NOKAIMOC@?$CIedge?9?$DOadjacent_surface_index?5?$DO?$DN@ (0000)
0024E134 003e:
	??_C@_0DO@PCGCNINJ@?$CIsurface_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIsurface@ (0000)
0024E174 0013:
	??_C@_0BD@GFDHLODO@distance_reference?$AA@ (0000)
0024E188 0058:
	??_C@_0FI@OHAECMC@quantized_cost_estimate?5?$DM?$DN?5state@ (0000)
0024E1E0 0050:
	??_C@_0FA@CJCEFIEA@?$CInew_node?9?$DOheap_location?5?$DO?$DN?51?$CJ?5?$CG@ (0000)
0024E230 0054:
	??_C@_0FE@CHKDPLDB@state?9?$DOheap?$FLheap_location?$FN?4quant@ (0000)
0024E284 0034:
	??_C@_0DE@MIONFOJA@state?9?$DOheap?$FLheap_location?$FN?4node_@ (0000)
0024E2B8 003c:
	??_C@_0DM@DFJMOLG@?$CIheap_location?5?$DO?$DN?51?$CJ?5?$CG?$CG?5?$CIheap_lo@ (0000)
0024E2F4 003c:
	??_C@_0DM@PHFOIMMP@path_state_traverse?3?5found?5a?5?8be@ (0000)
0024E330 0052:
	??_C@_0FC@GGCMLBII@?$CInew_node?9?$DOsurface_index?5?$DO?$DN?50?$CJ?5?$CG@ (0000)
0024E388 004e:
	??_C@_0EO@KKHPGEKA@path_state_traverse?3?5cost?5?$CF?41f?5e@ (0000)
0024E3D8 005c:
	??_C@_0FM@KGGDAHPH@total_cost_estimate?5?9?5cheapest_n@ (0000)
0024E434 0004:
	__real@bdcccccd (0000)
0024E438 005c:
	??_C@_0FM@PLCDNLNA@?$CIcheapest_node?9?$DOsurface_index?5?$DO?$DN@ (0000)
0024E498 0041:
	??_C@_0EB@OLCMDDAG@state?9?$DOdebug?9?$DOpath_traverse_resu@ (0000)
*/

/* ---------- headers */

#include "ai/path.h"

#ifdef HALO_N64
/* the distance still to go counts this much more than the way come
(weighted A*): the bots' and Covenant's searches reach their goal opening
far fewer surfaces around an obstacle, for a way at most this much longer
than the shortest before smoothing straightens it */
#define PATH_HEURISTIC_WEIGHT 1.5f
#else
#define PATH_HEURISTIC_WEIGHT 1.0f
#endif

#ifdef HALO_N64
/* a search in parts (path_state_find_continue): the surfaces it may still
open this call (-1: no limit), and whether it stopped for that */
static short n64_traverse_budget = -1;
static boolean n64_traverse_paused;
#endif
#include "ai/ai_debug.h"
#include "ai/ai_profile.h"
#include "ai/path_structure_bsp.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_bsp.h"
#include "physics/collision_bsp_definitions.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"

#include <stddef.h>

/* ---------- constants */

enum
{
	_collision_surface_breakable_bit = 3,
	_pathfinding_surface_walkable_bit = 6,
	_pathfinding_surface_breakable_bit = 7,
};

#define PATH_COST_ESTIMATE_GRANULARITY 0.1f

/* ---------- macros */

/* ---------- structures */

struct path_edge
{
	long adjacent_surface_index;
	byte adjacent_pathfinding_surface;
	byte pad05[3];
	real_point3d base_point;
	real_vector3d edge_vector;
};

typedef char path_edge_size_assert[
	sizeof(struct path_edge) == 0x20 ? 1 : -1];

typedef char path_input_size_assert[
	sizeof(struct path_input) == 0x48 ? 1 : -1];
typedef char path_input_ignore_broken_surfaces_offset_assert[
	offsetof(struct path_input, ignore_broken_surfaces) == 0x4 ? 1 : -1];
typedef char path_input_ignore_source_object_index_offset_assert[
	offsetof(struct path_input, ignore_source_object_index) == 0x8 ? 1 : -1];
typedef char path_input_ignore_target_object_index_offset_assert[
	offsetof(struct path_input, ignore_target_object_index) == 0xC ? 1 : -1];
typedef char path_input_start_valid_offset_assert[
	offsetof(struct path_input, start_valid) == 0x10 ? 1 : -1];
typedef char path_input_start_point_offset_assert[
	offsetof(struct path_input, start_point) == 0x14 ? 1 : -1];
typedef char path_input_start_surface_index_offset_assert[
	offsetof(struct path_input, start_surface_index) == 0x20 ? 1 : -1];
typedef char path_state_destination_valid_offset_assert[
	offsetof(struct path_state, destination_valid) == 0x4C ? 1 : -1];
typedef char path_state_destination_offset_assert[
	offsetof(struct path_state, destination) == 0x50 ? 1 : -1];
typedef char path_state_debug_offset_assert[
	offsetof(struct path_state, debug) == 0x48 ? 1 : -1];
typedef char path_state_structure_offset_assert[
	offsetof(struct path_state, structure) == 0x64 ? 1 : -1];
typedef char path_destination_surface_index_offset_assert[
	offsetof(struct path_destination, surface_index) == 0xC ? 1 : -1];
typedef char path_destination_target_radius_offset_assert[
	offsetof(struct path_destination, target_radius) == 0x10 ? 1 : -1];
typedef char path_node_surface_index_offset_assert[
	offsetof(struct path_node, surface_index) == 0x8 ? 1 : -1];

/* ---------- prototypes */

static void path_state_reset(
	struct path_state *state);
static boolean path_heap_verify(
	struct path_state *state);
static void path_heap_bubble_up(
	struct path_state *state,
	short heap_location);
static void path_heap_bubble_down(
	struct path_state *state,
	short heap_location);
static short path_heap_pop_cheapest_node(
	struct path_state *state);
static void path_heap_insert(
	struct path_state *state,
	short node_index,
	short quantized_cost_estimate);
static boolean surface_is_broken(
	struct structure_bsp const *structure,
	long surface_index);
static boolean path_state_begin(
	struct path_state *state);
static real closest_available_point_on_surface(
	struct structure_bsp const *structure,
	long surface_index,
	real_point3d const *point,
	real_point3d *closest_point);
static short build_path_edges_for_surface(
	struct structure_bsp const *structure,
	long surface_index,
	struct path_edge *edges);
static boolean path_state_traverse(
	struct path_state *state);

/* ---------- globals */

/* ---------- public code */

void paths_initialize(
	void)
{
	return;
}

void paths_dispose(
	void)
{
	return;
}

void paths_initialize_for_new_map(
	void)
{
	return;
}

void paths_dispose_from_old_map(
	void)
{
	return;
}

void path_input_new(
	struct path_input *input,
	real pathfinding_radius,
	boolean ignore_broken_surfaces,
	long ignore_source_object_index)
{
	csmemset(input, 0, sizeof(*input));
	input->pathfinding_radius = pathfinding_radius;
	input->ignore_broken_surfaces = ignore_broken_surfaces;
	input->ignore_source_object_index = ignore_source_object_index;
	input->ignore_target_object_index = NONE;
	return;
}

void path_input_set_target_object(
	struct path_input *input,
	long target_object_index)
{
	input->ignore_target_object_index = target_object_index;
	return;
}

void path_input_set_start(
	struct path_input *input,
	const real_point3d *start_point,
	long start_surface_index)
{
	input->start_valid = TRUE;
	input->start_point = *start_point;
	input->start_surface_index = start_surface_index;
	return;
}

void path_input_set_attractor(
	struct path_input *input,
	real_point3d const *attractor_point,
	real attractor_radius,
	long attractor_object_index,
	real attractor_weight)
{
	input->attractor_valid = TRUE;
	input->attractor_point = *attractor_point;
	input->attractor_radius = attractor_radius;
	input->attractor_object_index = attractor_object_index;
	input->attractor_weight = attractor_weight;
	return;
}

void path_input_set_search_bounds(
	struct path_input *input,
	real search_maximum_distance)
{
	input->search_bounded = TRUE;
	input->search_maximum_distance = search_maximum_distance;
	return;
}

void path_state_new(
	struct path_input const *input,
	struct path_state *state,
	struct path_debug_storage *debug)
{
#ifdef HALO_N64
	/* the header only: a node's fields are all written when it is taken
	(path_state_begin, path_state_traverse), the heap is read below its
	count and the hash table is cleared by path_state_reset -- clearing the
	whole 47 KB for every search was ~2.5 ms of cache line fills */
	csmemset(state, 0, offsetof(struct path_state, node_list));
#else
	csmemset(state, 0, sizeof(*state));
#endif
	state->structure = global_structure_bsp_get();
	state->input = *input;
	state->debug = debug;

	return;
}

void path_state_destination(
	struct path_state *state,
	real_point3d const *destination_point,
	long destination_surface_index,
	real destination_accept_radius)
{
	state->destination_valid = TRUE;
	state->destination.point = *destination_point;
	state->destination.surface_index = destination_surface_index;
	state->destination.target_radius = destination_accept_radius;

	return;
}

static void path_state_reset(
	struct path_state *state)
{
	state->node_count = 0;
	state->heap_count = 1;
	csmemset(state->hash_table, NONE, sizeof(state->hash_table));
	state->closest_node_index = NONE;
	state->closest_distance = REAL_MAX;
	state->closest_cost_estimate = REAL_MAX;

	return;
}

/* January retains this private heap check with a body that reduces to TRUE
 * (name from the 2001-09-25 Xbox linker map, same link slot).  Callers check
 * the heap on entry and exit, as path_obstacle_avoidance.c's heap_verify does.
 */
static boolean path_heap_verify(
	struct path_state *state)
{
	return TRUE;
}

static void path_heap_bubble_up(
	struct path_state *state,
	short heap_location)
{
	short node_index;
	short node_cost;
	struct path_node *node;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x4EA,
		(heap_location >= 1) && (heap_location <= PATH_NODE_LIST_SIZE));

	node_index = state->heap[heap_location].node_index;
	node_cost = state->heap[heap_location].quantized_cost_estimate;
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x4EF,
		(node_index >= 0) && (node_index < PATH_NODE_LIST_SIZE));
	node = &state->node_list[node_index];
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x4F0,
		state->node_list[node_index].quantized_cost_estimate == node_cost);

	if (heap_location > 1)
	{
		short parent_location;
		short parent_node_index;
		short parent_cost_estimate;
		struct path_node *parent_node;

		do
		{
			parent_location = heap_location >> 1;
			parent_node_index = state->heap[parent_location].node_index;
			parent_cost_estimate = state->heap[parent_location].quantized_cost_estimate;
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x4FF,
				(parent_node_index >= 0) && (parent_node_index < PATH_NODE_LIST_SIZE));
			parent_node = &state->node_list[parent_node_index];
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x500,
				state->node_list[parent_node_index].heap_location == parent_location);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x501,
				state->node_list[parent_node_index].quantized_cost_estimate == parent_cost_estimate);

			if (node_cost >= parent_cost_estimate)
			{
				break;
			}

			state->heap[heap_location].node_index = parent_node_index;
			state->heap[heap_location].quantized_cost_estimate = parent_cost_estimate;
			parent_node->heap_location = heap_location;
			heap_location = parent_location;
		}
		while (parent_location > 1);
	}

	state->heap[heap_location].node_index = node_index;
	state->heap[heap_location].quantized_cost_estimate = node_cost;
	node->heap_location = heap_location;

	return;
}

static void path_heap_bubble_down(
	struct path_state *state,
	short heap_location)
{
	short node_index;
	short node_cost;
	struct path_node *node;
	short new_node_index;
	short new_heap_location;
	short new_cost;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x524,
		(heap_location >= 1) && (heap_location <= PATH_NODE_LIST_SIZE));

	node_index = state->heap[heap_location].node_index;
	node_cost = state->heap[heap_location].quantized_cost_estimate;
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x529,
		(node_index >= 0) && (node_index < PATH_NODE_LIST_SIZE));
	node = &state->node_list[node_index];
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x52A,
		state->node_list[node_index].quantized_cost_estimate == node_cost);

	while (TRUE)
	{
		short child_heap_location;
		short child_number;

		new_node_index = node_index;
		new_heap_location = heap_location;
		new_cost = node_cost;
		child_number = 0;
		child_heap_location = (short)(heap_location << 1);

		while (child_number < 2 && child_heap_location < state->heap_count)
		{
			short child_node_index = state->heap[child_heap_location].node_index;
			short child_cost = state->heap[child_heap_location].quantized_cost_estimate;
			struct path_node *child_node;

			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x53E,
				(child_node_index >= 0) && (child_node_index < PATH_NODE_LIST_SIZE));
			child_node = &state->node_list[child_node_index];
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x53F,
				state->node_list[child_node_index].heap_location == child_heap_location);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x540,
				state->node_list[child_node_index].quantized_cost_estimate == child_cost);

			if (child_cost < new_cost)
			{
				new_heap_location = child_heap_location;
				new_node_index = child_node_index;
				new_cost = child_cost;
			}

			child_number++;
			child_heap_location++;
		}

		if (new_heap_location == heap_location)
		{
			break;
		}

		state->heap[heap_location].node_index = new_node_index;
		state->heap[heap_location].quantized_cost_estimate = new_cost;
		state->node_list[new_node_index].heap_location = heap_location;
		heap_location = new_heap_location;
	}

	state->heap[heap_location].node_index = node_index;
	state->heap[heap_location].quantized_cost_estimate = node_cost;
	node->heap_location = heap_location;

	return;
}

static short path_heap_pop_cheapest_node(
	struct path_state *state)
{
	short node_index = NONE;

	path_heap_verify(state);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x572,
		state->heap_count >= 1);

	if (state->heap_count > 1)
	{
		struct path_node *node;

		node_index = state->heap[1].node_index;
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x577,
			(node_index >= 0) && (node_index < PATH_NODE_LIST_SIZE));
		node = &state->node_list[node_index];
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x579,
			state->node_list[node_index].heap_location == 1);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x57A,
			state->node_list[node_index].quantized_cost_estimate == state->heap[1].quantized_cost_estimate);

		node->heap_location = NONE;
		state->heap_count--;
		if (state->heap_count > 1)
		{
			state->heap[1] = state->heap[state->heap_count];
			path_heap_bubble_down(state, 1);
		}
		path_heap_verify(state);
	}

	return node_index;
}

static void path_heap_insert(
	struct path_state *state,
	short node_index,
	short quantized_cost_estimate)
{
	short heap_location;

	path_heap_verify(state);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x594,
		state->heap_count >= 1);

	heap_location = state->heap_count;
	if (heap_location < PATH_NODE_LIST_SIZE)
	{
		state->heap_count++;
		state->heap[heap_location].node_index = node_index;
		state->heap[heap_location].quantized_cost_estimate = quantized_cost_estimate;
		path_heap_bubble_up(state, heap_location);
		path_heap_verify(state);
	}
	else
	{
		error(_error_silent, "path_heap_" "insert: overflowed static size heap");
	}

	return;
}

static boolean surface_is_broken(
	struct structure_bsp const *structure,
	long surface_index)
{
	struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(
		&structure->collision_bsp,
		0,
		struct collision_bsp);
	struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	boolean broken;

	broken = FALSE;
	if (TEST_FLAG(surface->flags, _collision_surface_breakable_bit))
	{
		long const *breakable_surface_flags =
			(long const *)breakable_surface_flags_get();

		broken = !BIT_VECTOR_TEST_FLAG(
			breakable_surface_flags,
			surface->breakable_surface_index);
	}

	return broken;
}

struct path_node *path_get_node(
	struct path_state *state,
	short node_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		1553,
		node_index != NONE);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		1554,
		(node_index >= 0) && (node_index < state->node_count));

	return &state->node_list[node_index];
}

short path_node_from_hash_table(
	struct path_state *state,
	long surface_index)
{
	short node_index;
	short slot;

	slot = (short)((surface_index & PATH_HASH_KEY_MASK) << 3);
	do
	{
		node_index = state->hash_table[slot];
		slot = (short)((slot + 1) & PATH_HASH_TABLE_MASK);
	}
	while (node_index != NONE &&
		state->node_list[node_index].surface_index != surface_index);

	return node_index;
}

boolean path_3d_available(
	struct structure_bsp *structure,
	real_point3d const *start_point,
	real avoidance_distance,
	real_point3d const *end_point,
	boolean *finishing_path_reference,
	real_point3d *path_endpoint)
{
	boolean available = FALSE;
	boolean finishing_path = FALSE;
	real_point3d endpoint = *end_point;
	real_vector3d vector;
	struct collision_bsp_test_vector_result result;

	vector_from_points3d(start_point, end_point, &vector);
	if (!collision_bsp_test_vector(
			FLAG(_collision_test_front_facing_surfaces_bit),
			TAG_BLOCK_GET_ELEMENT(
				&structure->collision_bsp,
				0,
				struct collision_bsp),
			0,
			NULL,
			start_point,
			&vector,
			REAL_MAX,
			&result) ||
		result.t >= 1.0f ||
		magnitude_squared3d(&vector)*((1.0f - result.t)*(1.0f - result.t)) < 0.1f)
	{
		available = TRUE;
		finishing_path = TRUE;
	}

	if (finishing_path_reference)
	{
		*finishing_path_reference = finishing_path;
	}
	if (path_endpoint)
	{
		*path_endpoint = endpoint;
	}

	return available;
}

boolean path_3d_build_path(
	struct structure_bsp *structure,
	real_point3d const *start_point,
	real avoidance_distance,
	real_point3d const *end_point,
	struct path_result *path)
{
	real_point3d endpoint;
	boolean finishing_path;

	csmemset(path, 0, sizeof(*path));
	if (path_3d_available(
			structure,
			start_point,
			avoidance_distance,
			end_point,
			&finishing_path,
			&endpoint))
	{
		path->steps[0].point = endpoint;
		path->step_count = 1;
		path->steps[0].surface_index = NONE;
		path->step_index = 0;
		path->steps_finish_path = finishing_path;
		path->endpoint.point = *end_point;
		path->endpoint.surface_index = NONE;
		path->endpoint.target_radius = 0.0f;
		path->valid = TRUE;
	}

	return path->valid;
}

boolean path_state_approach_point(
	struct path_state *state,
	real_point2d const *end_point,
	long end_surface_index,
	boolean *straight_line_reference,
	real_point3d *approach_point_reference)
{
	short node_index = path_node_from_hash_table(state, end_surface_index);
	boolean result = FALSE;

	if (node_index != NONE)
	{
		struct path_node *node = path_get_node(state, node_index);

		while (node->parent_node_index != NONE)
		{
			struct path_node *parent_node = path_get_node(
				state,
				node->parent_node_index);
			struct path_collision_result collision_result;

			if (structure_test_line2d(
					state->structure,
					state->input.ignore_broken_surfaces,
					end_point,
					end_surface_index,
					(real_point2d const *)&parent_node->entry_point,
					parent_node->surface_index,
					&collision_result))
			{
				break;
			}

			node_index = node->parent_node_index;
			node = path_get_node(state, node_index);
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x12D,
			approach_point_reference);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x12E,
			straight_line_reference);

		if (node->parent_node_index == NONE)
		{
			*straight_line_reference = TRUE;
			*approach_point_reference = state->input.start_point;
		}
		else
		{
			*straight_line_reference = FALSE;
			*approach_point_reference = node->entry_point;
		}

		result = TRUE;
	}

	return result;
}

boolean path_state_build_path(
	struct path_state *state,
	struct path_result *path)
{
	short node_index;
	struct path_node *node;
	short raw_step_count;
	short smoothed_step_count;
	short avoided_step_count;
	boolean steps_finish_path;
	struct path_step raw_steps[64];
	struct path_step smoothed_steps[4];
	struct path_step avoided_steps[4];
	short child_node_index;
	struct path_node *child_node;

	if (state->debug)
	{
		state->debug->path_build_result = _path_build_result_none;
	}

	path->valid = FALSE;
	if (state->destination_valid)
	{
		node_index = path_node_from_hash_table(
			state,
			state->destination.surface_index);

		if (node_index != NONE)
		{
			node = path_get_node(state, node_index);
			path->endpoint = state->destination;
			path->endpoint.target_radius = 0.0f;
		}
		else
		{
			if (state->closest_distance < state->destination.target_radius)
			{
				node_index = state->closest_node_index;
				node = path_get_node(state, node_index);
				path->endpoint.point = state->closest_point;
				path->endpoint.surface_index = node->surface_index;
				path->endpoint.target_radius = state->closest_distance;
			}
		}

		if (node_index != NONE)
		{
			long depth_plus_one = node->depth + 1;
			boolean path_build_success;

			smoothed_step_count = 0;
			avoided_step_count = 0;
			steps_finish_path = TRUE;
			child_node_index = NONE;
			child_node = NULL;
			raw_step_count = MIN(depth_plus_one, 64);

			do
			{
				node = path_get_node(state, node_index);

				if (node->depth >= 64)
				{
					steps_finish_path = FALSE;
				}
				else
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\path.c",
						0x1E8,
						(node->depth >= 0) && (node->depth < raw_step_count));

					raw_steps[node->depth].surface_index = node->surface_index;
					if (child_node_index == NONE)
					{
						raw_steps[node->depth].point = path->endpoint.point;
					}
					else
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\path.c",
							0x1F0,
							node->depth == child_node->depth - 1);
						raw_steps[node->depth].point = child_node->entry_point;
					}
				}
				child_node_index = node_index;
				child_node = node;
				node_index = node->parent_node_index;
			} while (node_index != NONE);

			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x1FB,
				child_node_index != NONE);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x1FC,
				child_node->depth == 0);

			if (game_connection() == _game_connection_local &&
				ai_debug.path_disable_smoothing)
			{
				smoothed_step_count = MIN(raw_step_count, 4);
				csmemcpy(
					smoothed_steps,
					raw_steps,
					smoothed_step_count * sizeof(struct path_step));
			}
			else
			{
				path_smooth(
					state,
					raw_step_count,
					raw_steps,
					&smoothed_step_count,
					smoothed_steps,
					&steps_finish_path);
			}

			if (game_connection() == _game_connection_local &&
				ai_debug.path_disable_obstacle_avoidance)
			{
				avoided_step_count = MIN(smoothed_step_count, 4);
				csmemcpy(
					avoided_steps,
					smoothed_steps,
					avoided_step_count * sizeof(struct path_step));
				path_build_success = TRUE;
			}
			else
			{
				path_build_success = path_avoid_obstacles(
					state,
					smoothed_step_count,
					smoothed_steps,
					&avoided_step_count,
					avoided_steps,
					&steps_finish_path);

				if (state->debug && !path_build_success)
				{
					state->debug->path_build_result =
						_path_build_result_obstacle_avoidance_failed;
				}
			}

			if (path_build_success)
			{
				path->step_count = (char)avoided_step_count;
				path->steps_finish_path = steps_finish_path;
				path->valid = TRUE;
				path->step_index = 0;
				csmemcpy(
					path->steps,
					avoided_steps,
					avoided_step_count * sizeof(struct path_step));

				if (path->steps_finish_path)
				{
					path->endpoint.point =
						path->steps[path->step_count - 1].point;
					path->endpoint.surface_index =
						path->steps[path->step_count - 1].surface_index;
					path->endpoint.target_radius = distance3d(
						&path->endpoint.point,
						&state->destination.point);
				}

				if (state->debug)
				{
					state->debug->path_build_result = _path_build_result_success;
				}
			}

			if (state->debug)
			{
				state->debug->raw_step_count = raw_step_count;
				csmemcpy(
					state->debug->raw_steps,
					raw_steps,
					raw_step_count * sizeof(struct path_step));
				state->debug->smoothed_step_count = smoothed_step_count;
				csmemcpy(
					state->debug->smoothed_steps,
					smoothed_steps,
					smoothed_step_count * sizeof(struct path_step));
				state->debug->avoided_step_count = avoided_step_count;
				csmemcpy(
					state->debug->avoided_steps,
					avoided_steps,
					avoided_step_count * sizeof(struct path_step));
			}
		}
		else if (state->debug)
		{
			state->debug->path_build_result =
				(state->closest_node_index != NONE) +
				_path_build_result_cached_node_missing;
		}
	}
	else if (state->debug)
	{
		state->debug->path_build_result = _path_build_result_no_destination;
	}

	if (state->debug)
	{
		state->debug->result = *path;

		if (state->debug->path_build_result != _path_build_result_success)
		{
			state->debug->failure = TRUE;
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x265,
			state->debug->path_build_result != _path_build_result_none);
	}

	return path->valid;
}

static boolean path_state_begin(
	struct path_state *state)
{
	boolean result = FALSE;

	if (state->input.start_surface_index != NONE &&
		state->input.start_point.z > -1000.0f)
	{
		struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(
			&state->structure->collision_bsp,
			0,
			struct collision_bsp);
		real distance_to_destination;
		long quantized_cost_estimate;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x2B1,
			(state->input.start_surface_index >= 0) && (state->input.start_surface_index < bsp->surfaces.count));

		if (state->destination_valid)
		{
			struct path_destination const *destination = &state->destination;
			real distance_squared = distance_squared3d(
				&state->input.start_point,
				&destination->point);

			distance_to_destination = square_root(distance_squared);
			quantized_cost_estimate = (long)(PATH_HEURISTIC_WEIGHT * distance_to_destination / PATH_COST_ESTIMATE_GRANULARITY);
			if (quantized_cost_estimate >= SHORT_MAX)
			{
				error(
					_error_silent,
					"pathfinding: attempted to build path from (%.1f %.1f %.1f) to (%.1f %.1f %.1f) ... distance %.1f > maximum allowed %.1f",
					state->input.start_point.x,
					state->input.start_point.y,
					state->input.start_point.z,
					destination->point.x,
					destination->point.y,
					destination->point.z,
					distance_to_destination,
					SHORT_MAX * PATH_COST_ESTIMATE_GRANULARITY);
			}
			else
			{
				result = TRUE;
			}
		}
		else
		{
			distance_to_destination = 0.0f;
			quantized_cost_estimate = 0;
			result = TRUE;
		}

		if (result)
		{
			short node_index;
			struct path_node *initial_node;

			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x2CC,
				state->node_count == 0);
			node_index = state->node_count++;
			initial_node = &state->node_list[node_index];
			initial_node->parent_node_index = NONE;
			initial_node->parent_node_surface_index = NONE;
			initial_node->surface_index = state->input.start_surface_index;
			initial_node->entry_point = state->input.start_point;
			initial_node->linear_distance_to_entry_point = 0.0f;
			initial_node->closest_approach_to_attractor = REAL_MAX;
			initial_node->path_distance_from_origin = 0.0f;
			initial_node->cumulative_cost = 0.0f;
			initial_node->total_cost_estimate = PATH_HEURISTIC_WEIGHT * distance_to_destination;
			initial_node->quantized_cost_estimate = (short)quantized_cost_estimate;
			initial_node->depth = 0;
#ifndef HALO_N64
			initial_node->last_render_id = NONE;
#endif

			bsp = TAG_BLOCK_GET_ELEMENT(
				&state->structure->collision_bsp,
				0,
				struct collision_bsp);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x2E3,
				(initial_node->surface_index >= 0) && (initial_node->surface_index < bsp->surfaces.count));

			if (state->destination_valid)
			{
				state->closest_distance = distance_to_destination;
				state->closest_node_index = node_index;
				state->closest_point = state->input.start_point;
				state->closest_cost_estimate = PATH_HEURISTIC_WEIGHT * distance_to_destination;
			}

			state->hash_table[
				(initial_node->surface_index & PATH_HASH_KEY_MASK) << 3] = node_index;
			path_heap_insert(
				state,
				node_index,
				(short)quantized_cost_estimate);
		}
	}

	return result;
}

static real closest_available_point_on_surface(
	struct structure_bsp const *structure,
	long surface_index,
	real_point3d const *point,
	real_point3d *closest_point)
{
	struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(
		&structure->collision_bsp,
		0,
		struct collision_bsp);
	real_point2d closest_point2d;

	collision_surface_find_closest_point2d(
		bsp,
		surface_index,
		_z,
		TRUE,
		(real_point2d const *)point,
		&closest_point2d);
	collision_surface_project_point2d(
		bsp,
		surface_index,
		_z,
		TRUE,
		&closest_point2d,
		closest_point);

	return distance3d(point, closest_point);
}

static short build_path_edges_for_surface(
	struct structure_bsp const *structure,
	long surface_index,
	struct path_edge *edges)
{
	byte const *pathfinding_surfaces = structure->pathfinding_surfaces.address;
	struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(
		&structure->collision_bsp,
		0,
		struct collision_bsp);
	short edge_count = 0;
	struct collision_surface const *surface;
	long edge_index;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x5D8,
		(surface_index >= 0) && (surface_index < bsp->surfaces.count));
	surface = TAG_BLOCK_GET_ELEMENT(
		&bsp->surfaces,
		surface_index,
		struct collision_surface);
	edge_index = surface->first_edge_index;

	do
	{
		struct collision_edge const *collision_edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		boolean right_surface = surface_index == collision_edge->surface_indices[1];
		struct path_edge *edge = &edges[edge_count++];
		struct collision_vertex const *start_vertex;
		struct collision_vertex const *end_vertex;

		edge->adjacent_surface_index =
			collision_edge->surface_indices[!right_surface];
		if (edge->adjacent_surface_index != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x5EE,
				(edge->adjacent_surface_index >= 0) && (edge->adjacent_surface_index < bsp->surfaces.count));
		}
		edge->adjacent_pathfinding_surface =
			pathfinding_surfaces[edge->adjacent_surface_index];

		start_vertex = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			collision_edge->vertex_indices[0],
			struct collision_vertex);
		end_vertex = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			collision_edge->vertex_indices[1],
			struct collision_vertex);
		edge->base_point = start_vertex->point;
		vector_from_points3d(
			&start_vertex->point,
			&end_vertex->point,
			&edge->edge_vector);

		if (edge_count == MAXIMUM_PATH_EDGES_PER_COLLISION_SURFACE)
		{
			break;
		}

		edge_index = collision_edge->edge_indices[right_surface];
	}
	while (edge_index != surface->first_edge_index);

	return edge_count;
}

void closest_point_to_attractor(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *q,
	real_point3d *result)
{
	real_vector3d segment;
	real_vector3d offset;
	real t;

	vector_from_points3d(p0, p1, &segment);
	vector_from_points3d(q, p0, &offset);
	t = dot_product3d(&offset, &segment)/magnitude_squared3d(&segment);
	if (t < 0.0f || t > 1.0f)
	{
		*result = *p1;
	}
	else
	{
		point_from_line3d(p0, &segment, t, result);
	}

	return;
}

real path_attractor_weight(
	struct path_state *state,
	real_point3d const *point,
	real_point3d const *previous_point,
	real *distance_reference)
{
	real_point3d closest_point;
	real distance_squared;
	real weight = 0.0f;
	real distance = REAL_MAX;

	closest_point_to_attractor(
		point,
		previous_point,
		&state->input.attractor_point,
		&closest_point);
	distance_squared = distance_squared3d(
		&state->input.attractor_point,
		&closest_point);
	if (distance_squared <
		state->input.attractor_radius*state->input.attractor_radius)
	{
		distance = square_root(distance_squared);
		weight = (1.0f - distance/state->input.attractor_radius)*
			state->input.attractor_weight;
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		1631,
		distance_reference);
	*distance_reference = distance;

	return weight;
}

boolean path_state_estimated_distance(
	struct path_state *state,
	real_point3d const *end_point,
	long end_surface_index,
	real *distance_reference,
	real *closest_approach_to_attractor_reference,
	real_vector3d *estimated_direction_reference)
{
	short node_index = path_node_from_hash_table(state, end_surface_index);
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path.c",
		0x14E,
		distance_reference);

	if (node_index != NONE)
	{
		struct path_node *node = path_get_node(state, node_index);
		real distance = distance3d(&node->entry_point, end_point) +
			node->path_distance_from_origin;
		real closest_approach_to_attractor;

		if (state->input.attractor_valid)
		{
			real_point3d closest_point;

			closest_point_to_attractor(
				&node->entry_point,
				end_point,
				&state->input.attractor_point,
				&closest_point);
			closest_approach_to_attractor = distance3d(
				&state->input.attractor_point,
				&closest_point);
			closest_approach_to_attractor = MIN(
				closest_approach_to_attractor,
				node->closest_approach_to_attractor);
		}
		else
		{
			closest_approach_to_attractor = 0.0f;
		}

		if (closest_approach_to_attractor_reference)
		{
			*closest_approach_to_attractor_reference = closest_approach_to_attractor;
		}
		*distance_reference = distance;
		result = TRUE;

		if (estimated_direction_reference)
		{
			short current_node_index = node_index;
			short child_node_index = NONE;
			real path_distance = 0.0f;
			real_point3d const *direction_point;

			do
			{
				node = path_get_node(state, current_node_index);
				node->child_node_index = child_node_index;
				child_node_index = current_node_index;
				current_node_index = node->parent_node_index;
			}
			while (current_node_index != NONE);

			current_node_index = child_node_index;
			while (current_node_index != NONE && path_distance < 0.8f)
			{
				node = path_get_node(state, current_node_index);
				path_distance += node->linear_distance_to_entry_point;
				current_node_index = node->child_node_index;
			}

			direction_point = current_node_index == NONE ?
				end_point : &node->entry_point;
			vector_from_points3d(
				&state->input.start_point,
				direction_point,
				estimated_direction_reference);
			normalize3d(estimated_direction_reference);
		}
	}
	else
	{
		if (closest_approach_to_attractor_reference)
		{
			*closest_approach_to_attractor_reference = REAL_MAX;
		}
		if (estimated_direction_reference)
		{
			*estimated_direction_reference = *global_zero_vector3d;
		}
		*distance_reference = REAL_MAX;
	}

	return result;
}

static boolean path_state_traverse(
	struct path_state *state)
{
	real pathfinding_radius = MAX(0.2f, state->input.pathfinding_radius);
	struct path_edge edges[MAXIMUM_PATH_EDGES_PER_COLLISION_SURFACE];
	boolean result = TRUE;
	boolean reported_cost_overflow = FALSE;

	while (TRUE)
	{
		short cheapest_node_index;
		struct path_node *cheapest_node;
		struct collision_bsp const *bsp;
		short edge_count;
		short edge_index;

#ifdef HALO_N64
		/* (a search in parts: this call's share used up) */
		if (n64_traverse_budget >= 0)
		{
			if (n64_traverse_budget == 0)
			{
				n64_traverse_paused = TRUE;
				break;
			}
			n64_traverse_budget--;
		}
		/* the node list full and the destination none of its nodes: no node
		can be added, so the search can't reach it -- it ends here, at the
		closest node so far, instead of emptying the heap (~13 ms) */
		if (state->node_count >= PATH_NODE_LIST_SIZE && state->destination_valid &&
			path_node_from_hash_table(state, state->destination.surface_index) == NONE)
		{
			break;
		}
#endif
		cheapest_node_index = path_heap_pop_cheapest_node(state);
		if (cheapest_node_index == NONE)
		{
			if (state->debug &&
				state->debug->path_traverse_result == _path_traverse_result_none)
			{
				state->debug->path_traverse_result = _path_traverse_result_exhausted_search;
			}
			break;
		}

		cheapest_node = path_get_node(state, cheapest_node_index);
		bsp = TAG_BLOCK_GET_ELEMENT(
			&state->structure->collision_bsp,
			0,
			struct collision_bsp);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x35D,
			(cheapest_node->surface_index >= 0) && (cheapest_node->surface_index < bsp->surfaces.count));

		if (state->destination_valid)
		{
			if (cheapest_node->surface_index == state->destination.surface_index)
			{
				state->closest_node_index = cheapest_node_index;
				state->closest_distance = 0.0f;
				state->closest_point = state->destination.point;
				break;
			}

			if (cheapest_node->total_cost_estimate >
				MAX(5.0f, state->closest_distance) * 10.0f + state->closest_cost_estimate)
			{
				if (state->debug &&
					state->debug->path_traverse_result == _path_traverse_result_none)
				{
					state->debug->path_traverse_result =
						_path_traverse_result_never_close_enough;
				}
				break;
			}
		}

		edge_count = build_path_edges_for_surface(
			state->structure,
			cheapest_node->surface_index,
			edges);
		for (edge_index = 0; edge_index < edge_count; edge_index++)
		{
			struct path_edge const *edge = &edges[edge_index];
			boolean passable = TRUE;
			long adjacent_surface_index = edge->adjacent_surface_index;
			real_point3d entry_point;
			real linear_distance;
			real path_distance_from_origin;
			real closest_approach_to_attractor;
			real cost;
			real cumulative_cost;
			real total_cost_estimate;
			real distance_to_destination;
			long quantized_cost_estimate;
			short new_node_index = NONE;
			struct path_node *new_node;
			struct path_node previous_node_values;

			if (adjacent_surface_index == cheapest_node->parent_node_surface_index)
			{
				passable = FALSE;
			}
			if (!TEST_FLAG(edge->adjacent_pathfinding_surface, _pathfinding_surface_walkable_bit))
			{
				passable = FALSE;
			}
			if (!state->input.ignore_broken_surfaces &&
				TEST_FLAG(edge->adjacent_pathfinding_surface, _pathfinding_surface_breakable_bit) &&
				surface_is_broken(state->structure, adjacent_surface_index))
			{
				passable = FALSE;
			}
			if (!passable)
			{
				continue;
			}

			point_from_line3d(&edge->base_point, &edge->edge_vector, 0.5f, &entry_point);

			if (state->destination_valid)
			{
				real edge_length_squared = magnitude_squared3d(&edge->edge_vector);

				if (edge_length_squared > 16.0f &&
					edge_length_squared > (2.0f * pathfinding_radius) * (2.0f * pathfinding_radius))
				{
					real edge_length = square_root(edge_length_squared);
					real_vector3d edge_to_destination;
					real t;

					vector_from_points3d(&edge->base_point, &state->destination.point, &edge_to_destination);
					t = dot_product3d(&edge_to_destination, &edge->edge_vector) / magnitude_squared3d(&edge->edge_vector);
					t = PIN(t, pathfinding_radius / edge_length, 1.0f - pathfinding_radius / edge_length);
					point_from_line3d(&edge->base_point, &edge->edge_vector, t, &entry_point);
				}
			}

			linear_distance = distance3d(&cheapest_node->entry_point, &entry_point);
			path_distance_from_origin =
				cheapest_node->path_distance_from_origin + linear_distance;
			if (state->input.attractor_valid)
			{
				cost = (path_attractor_weight(
					state,
					&cheapest_node->entry_point,
					&entry_point,
					&closest_approach_to_attractor) + 1.0f) * linear_distance;
				closest_approach_to_attractor = MIN(
					cheapest_node->closest_approach_to_attractor,
					closest_approach_to_attractor);
			}
			else
			{
				cost = linear_distance;
				closest_approach_to_attractor = 0.0f;
			}
			cumulative_cost = cheapest_node->cumulative_cost + cost;

			total_cost_estimate = cumulative_cost;
			if (state->destination_valid)
			{
				distance_to_destination = distance3d(&entry_point, &state->destination.point);
				total_cost_estimate += PATH_HEURISTIC_WEIGHT * distance_to_destination;
			}

			match_assert(
				"c:\\halo\\SOURCE\\ai\\path.c",
				0x3ED,
				total_cost_estimate - cheapest_node->total_cost_estimate >= -PATH_COST_ESTIMATE_GRANULARITY);
			quantized_cost_estimate =
				(long)(total_cost_estimate / PATH_COST_ESTIMATE_GRANULARITY);
			if (quantized_cost_estimate >= SHORT_MAX)
			{
				if (!reported_cost_overflow)
				{
					error(
						_error_log,
						"path_state_" "traverse: cost %.1f exceeded maximum allowed %.1f, discarding node",
						total_cost_estimate,
						SHORT_MAX * PATH_COST_ESTIMATE_GRANULARITY);
					reported_cost_overflow = TRUE;
				}
				continue;
			}
			if (state->input.search_bounded &&
				path_distance_from_origin > state->input.search_maximum_distance)
			{
				continue;
			}

			{
				short hash_slot = (short)(
					(edge->adjacent_surface_index & PATH_HASH_KEY_MASK) << 3);

				while (TRUE)
				{
					short node_index = state->hash_table[hash_slot];

					if (node_index != NONE)
					{
						struct path_node *node = path_get_node(state, node_index);

						if (node->surface_index == adjacent_surface_index)
						{
							if (quantized_cost_estimate < node->quantized_cost_estimate)
							{
								short heap_location = node->heap_location;

								if (heap_location == NONE)
								{
#ifndef HALO_N64
									/* (weighted, the N64's search finds these often:
									no message formatted each time) */
									error(
										_error_silent,
										"path_state_" "traverse: found a 'better' path to a closed node");
#endif
								}
								else
								{
									new_node_index = node_index;
									match_assert(
										"c:\\halo\\SOURCE\\ai\\path.c",
										0x423,
										(heap_location >= 1) && (heap_location < state->heap_count));
									match_assert(
										"c:\\halo\\SOURCE\\ai\\path.c",
										0x424,
										state->heap[heap_location].node_index == node_index);
									match_assert(
										"c:\\halo\\SOURCE\\ai\\path.c",
										0x425,
										state->heap[heap_location].quantized_cost_estimate == node->quantized_cost_estimate);
								}
							}
							break;
						}

						hash_slot = (short)((hash_slot + 1) & PATH_HASH_TABLE_MASK);
					}
					else
					{
						if (state->node_count < PATH_NODE_LIST_SIZE)
						{
							new_node_index = state->node_count++;
							state->hash_table[hash_slot] = new_node_index;
							state->node_list[new_node_index].heap_location = NONE;
						}
						else if (state->debug &&
							state->debug->path_traverse_result == _path_traverse_result_none)
						{
							state->debug->path_traverse_result =
								_path_traverse_result_overflowed_nodes;
						}
						break;
					}
				}
			}

			if (new_node_index == NONE)
			{
				continue;
			}

			new_node = path_get_node(state, new_node_index);
			previous_node_values = *new_node;
			new_node->parent_node_index = cheapest_node_index;
			new_node->parent_node_surface_index = cheapest_node->surface_index;
			new_node->surface_index = edge->adjacent_surface_index;
			new_node->entry_point = entry_point;
			new_node->linear_distance_to_entry_point = linear_distance;
			new_node->closest_approach_to_attractor = closest_approach_to_attractor;
			new_node->path_distance_from_origin = path_distance_from_origin;
			new_node->cumulative_cost = cumulative_cost;
			new_node->total_cost_estimate = total_cost_estimate;
			new_node->quantized_cost_estimate = (short)quantized_cost_estimate;
			new_node->depth = cheapest_node->depth + 1;
#ifndef HALO_N64
			new_node->last_render_id = NONE;
			new_node->closest_distance_to_attractor = REAL_MAX;
#endif

			{
				struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(
					&state->structure->collision_bsp,
					0,
					struct collision_bsp);

				match_assert(
					"c:\\halo\\SOURCE\\ai\\path.c",
					0x466,
					(new_node->surface_index >= 0) && (new_node->surface_index < bsp->surfaces.count));
			}

			if (new_node->heap_location == NONE)
			{
				path_heap_insert(
					state,
					new_node_index,
					(short)quantized_cost_estimate);
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path.c",
					0x471,
					(new_node->heap_location >= 1) && (new_node->heap_location < state->heap_count));
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path.c",
					0x472,
					quantized_cost_estimate <= state->heap[new_node->heap_location].quantized_cost_estimate);
				state->heap[new_node->heap_location].quantized_cost_estimate =
					(short)quantized_cost_estimate;
				path_heap_bubble_up(state, new_node->heap_location);
			}

			if (state->destination_valid)
			{
				real closest_distance = distance_to_destination;
				real_point3d closest_point = new_node->entry_point;

				if (closest_distance < 4.0f)
				{
					closest_distance = closest_available_point_on_surface(
						state->structure,
						edge->adjacent_surface_index,
						&state->destination.point,
						&closest_point);
				}
#ifndef HALO_N64
				new_node->closest_distance_to_attractor = closest_distance;
				new_node->closest_point_to_attractor = closest_point;
#endif

				if (closest_distance < state->closest_distance)
				{
					state->closest_distance = closest_distance;
					state->closest_node_index = new_node_index;
					state->closest_point = closest_point;
					state->closest_cost_estimate = total_cost_estimate;
				}
			}
		}
	}

#ifdef HALO_N64
	if (n64_traverse_paused)
		return FALSE;
#endif
	if (state->destination_valid)
	{
		result = state->closest_distance <= state->destination.target_radius;
	}
	if (state->debug && result)
	{
		state->debug->path_traverse_result = _path_traverse_result_success;
	}

	return result;
}

#ifdef HALO_N64
/* A search in parts (the N64's bots and Covenant: one search was several
milliseconds of a single tick): begun -- FALSE if the start can't be
pathfound from, the search over -- then carried on, at most budget
surfaces opened a call, until path_state_find_continue says it is over
(TRUE; *found as path_state_find's answer). The state is the search's
alone meanwhile; path_state_build_path after, as after path_state_find. */
boolean path_state_find_begin(
	struct path_state *state)
{
	path_state_reset(state);
	return path_state_begin(state);
}

boolean path_state_find_continue(
	struct path_state *state,
	short budget,
	boolean *found)
{
	boolean result;

	n64_traverse_budget = budget;
	n64_traverse_paused = FALSE;
	result = path_state_traverse(state);
	n64_traverse_budget = -1;
	if (n64_traverse_paused)
		return FALSE;
	if (found)
		*found = result;
	return TRUE;
}
#endif

boolean path_state_find(
	struct path_state *state)
{
	boolean result = FALSE;

	if (state->destination_valid)
	{
		ai_profile.meters[_ai_meter_path_find].accumulator++;
	}
	else
	{
		ai_profile.meters[_ai_meter_path_flood].accumulator++;
	}

	path_state_reset(state);
	if (state->debug)
	{
		state->debug->path_traverse_result = _path_traverse_result_none;
	}

	if (path_state_begin(state))
	{
		result = path_state_traverse(state);
	}
	else if (state->debug)
	{
		state->debug->path_traverse_result =
			_path_traverse_result_initial_not_pathfindable;
	}

	if (state->debug)
	{
		state->debug->path_state = *state;
		state->debug->structure_bsp_index = global_structure_bsp_index_get();
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path.c",
			0x32D,
			state->debug->path_traverse_result != _path_traverse_result_none);
		if (state->debug->path_traverse_result != _path_traverse_result_success)
		{
			state->debug->failure = TRUE;
		}
	}

	return result;
}

/* ---------- private code */
