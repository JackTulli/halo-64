/*
LEAF_MAP.C

symbols in this file:
00181320 0050:
	_node_stack_push (0000)
00181370 0040:
	_node_stack_pop (0000)
001813B0 0040:
	_node_stack_read (0000)
001813F0 0030:
	_leaf_map_delete (0000)
00181420 00a0:
	_find_like_crossing (0000)
001814C0 0040:
	_map_leaf_find_face_on_node (0000)
00181500 00d0:
	_leaf_map_close_portal (0000)
001815D0 0060:
	_leaf_map_leaf_is_closed (0000)
00181630 00b0:
	_leaf_map_family_mark (0000)
001816E0 0160:
	_render_debug_leaf_portal (0000)
00181840 0060:
	_render_debug_leaf_portals (0000)
001818A0 0340:
	_leaf_map_build_portal_from_leaves (0000)
00181BE0 0310:
	_leaf_map_get_leaf_bounds (0000)
00181EF0 0070:
	_leaf_map_closure (0000)
00181F60 0290:
	_leaf_map_leaf_spans_polygon (0000)
001821F0 00b0:
	_leaf_face_get_vertex3d (0000)
001822A0 0160:
	_render_debug_leaf_faces (0000)
00182400 0080:
	_normalize_three_dee (0000)
00182480 0170:
	_intersect_planes3d (0000)
001825F0 01e0:
	_leaf_map_build_portals_from_leaf (0000)
001827D0 0260:
	_leaf_map_build_leaf_face_for_leaf_on_node (0000)
00182A30 00f0:
	_leaf_map_build_portals (0000)
00182B20 0070:
	_leaf_map_build_leaf_faces_for_leaf (0000)
00182B90 00e0:
	_leaf_map_build_leaf_faces (0000)
00182C70 0130:
	_leaf_map_initialize_from_bsp (0000)
002A0C84 0012:
	??_C@_0BC@EGCHAPG@front?5leaf?5index?$CK?$AA@ (0000)
002A0C98 0011:
	??_C@_0BB@LINKKPJD@back?5leaf?5index?$CK?$AA@ (0000)
002A0CAC 000d:
	??_C@_0N@KPMCHLJG@plane?5index?$CK?$AA@ (0000)
002A0CBC 0019:
	??_C@_0BJ@FMOPLLFD@global_leaf_portal_block?$AA@ (0000)
002A0CD8 0019:
	??_C@_0BJ@IBJPLNPO@leaf_portal_vertex_block?$AA@ (0000)
002A0CF4 0010:
	??_C@_0BA@IDKJGEOA@portal?5indices?$CK?$AA@ (0000)
002A0D04 0007:
	??_C@_06KCOLJFCM@faces?$CK?$AA@ (0000)
002A0D0C 0016:
	??_C@_0BG@MIFDMMKE@global_map_leaf_block?$AA@ (0000)
002A0D24 000c:
	??_C@_0M@MIPIKCKP@node?5index?$CK?$AA@ (0000)
002A0D30 0014:
	??_C@_0BE@JGACHFLM@map_leaf_face_block?$AA@ (0000)
002A0D44 000e:
	??_C@_0O@GPDFDPME@portal?5index?$CK?$AA@ (0000)
002A0D54 001c:
	??_C@_0BM@OMCNINBM@map_leaf_portal_index_block?$AA@ (0000)
002A0D70 0008:
	??_C@_07FKKFMDPH@vertex?$CK?$AA@ (0000)
002A0D78 001b:
	??_C@_0BL@MDBPGNIK@map_leaf_face_vertex_block?$AA@ (0000)
002A0D94 0014:
	??_C@_0BE@CCPHNOCB@leaf_map_initialize?$AA@ (0000)
002A0DA8 003b:
	??_C@_0DL@KLHAKAHD@leaf_map_globals?4node_stack_coun@ (0000)
002A0DE4 0025:
	??_C@_0CF@JDBJPHEO@c?3?2halo?2SOURCE?2structures?2leaf_m@ (0000)
002A0E0C 0024:
	??_C@_0CE@INFHLEED@leaf_map_globals?4node_stack_coun@ (0000)
002A0E30 003c:
	??_C@_0DM@JGHBMKCF@levels_up?$DO?$DN0?5?$CG?$CG?5levels_up?$DMleaf_m@ (0000)
002A0E6C 0038:
	??_C@_0DI@KHGJOGO@portal_designator_index?$CB?$DNleaf?9?$DOp@ (0000)
002A0EA4 002e:
	??_C@_0CO@EGKEMBBF@couldn?8t?5allocate?5leaf?5map?5porta@ (0000)
002A0ED4 0022:
	??_C@_0CC@GKACLMKE@couldn?8t?5allocate?5leaf?5map?5porta@ (0000)
002A0EF8 0004:
	__real@3b23d70a (0000)
002A0EFC 001c:
	??_C@_0BM@NDHGCOBM@too?5many?5vertices?5in?5portal?$AA@ (0000)
002A0F18 0019:
	??_C@_0BJ@NHOJIIIJ@leaf_index0?$CB?$DNleaf_index1?$AA@ (0000)
002A0F34 0009:
	??_C@_08JHHHNNE@map?9?$DObsp?$AA@ (0000)
002A0F40 0026:
	??_C@_0CG@HMEMFMPL@reference_plane?9?$DOn?4n?$FLprojection?$FN@ (0000)
002A0F68 004f:
	??_C@_0EP@LAJCFBMA@ancestor_node_index?$CB?$DNNONE?5?$HM?$HM?5ind@ (0000)
002A0FB8 001d:
	??_C@_0BN@LDAFHDBA@couldn?8t?5allocate?5leaf?5face?4?$AA@ (0000)
002A0FD8 0021:
	??_C@_0CB@LPMMAOJD@couldn?8t?5allocate?5leaf?5vertices?4@ (0000)
002A0FFC 001a:
	??_C@_0BK@OEBNDAAE@result?4vertex_count?$CB?$DNNONE?$AA@ (0000)
002A1018 0023:
	??_C@_0CD@KPKMOOLF@couldn?8t?5allocate?5leaf_map?5leave@ (0000)
002A103C 0025:
	??_C@_0CF@CKHDENNA@leaf_map_globals?4node_stack_coun@ (0000)
002A1064 0004:
	??_C@_03DGBMLAJE@bsp?$AA@ (0000)
002A1068 0009:
	??_C@_08NNODDDAN@leaf_map?$AA@ (0000)
0030E970 09d0:
	_leaf_map_initialize_section (0000)
	_global_leaf_face_polygon (05f8)
	_global_map_leaf_block (08f8)
	_global_leaf_portal_block (09a4)
004C08B0 040a:
	_leaf_map_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "structures/leaf_map.h"
#include "cache/cache_files.h"
#include "cseries/errors.h"
#include "cseries/profile.h"
#include "physics/bsp3d.h"
#include "math/geometry.h"
#include "render/render_debug_geometry.h"

/* ---------- constants */

enum
{
	MAXIMUM_NODE_STACK_COUNT = 256,
	MAXIMUM_LEAF_FACE_VERTICES = 64,
	MAXIMUM_PORTAL_VERTICES = 64
};

/* ---------- declaration that belongs in tag_files/tag_groups.h
   _tag_field_real_point2d is a shared tag-system constant, not leaf_map's own;
   it is held here because this lane may not edit shared headers.  Same
   precedent as bitmaps/bitmap_group.c.
   ---------- */

enum
{
	_tag_field_real_point2d = 16,
};

enum projected_plane_result
{
	_projected_plane_empty,
	_projected_plane_valid,
	_projected_plane_full
};

/* ---------- macros */

#define index_from_node(node) ((node) & LONG_MAX)

/* ---------- structures */

struct map_leaf_face
{
	long node_index;
	struct tag_block vertices;			// real_point2d
};

struct map_leaf
{
	struct tag_block faces;				// map_leaf_face
	struct tag_block portal_designators;		// long
};

struct leaf_map_polygon
{
	short vertex_count;
	real_point2d vertices[MAXIMUM_LEAF_FACE_VERTICES];
};

typedef char map_leaf_face_size_assert[
	sizeof(struct map_leaf_face) == 0x10 ? 1 : -1];
typedef char map_leaf_size_assert[
	sizeof(struct map_leaf) == 0x18 ? 1 : -1];

/* ---------- prototypes */

static void node_stack_push(
	long node_index);
static long node_stack_pop(
	void);
static long node_stack_read(
	short levels_up);
static boolean find_like_crossing(
	struct leaf_map const *leaf_map,
	long plane_designator,
	boolean *side);
static short map_leaf_find_face_on_node(
	struct leaf_map const *leaf_map,
	struct map_leaf const *leaf,
	long node_index);
static real normalize_three_dee(
	real_vector3d *vector);
static short intersect_planes3d(
	real_plane3d const *reference_plane,
	real_plane3d const *plane,
	real_plane2d *result);
static void leaf_map_build_leaf_faces(
	struct leaf_map *leaf_map,
	long node_index);
static void leaf_map_build_leaf_faces_for_leaf(
	struct leaf_map *leaf_map,
	long leaf_index);
static void leaf_map_build_leaf_face_for_leaf_on_node(
	struct leaf_map *leaf_map,
	long leaf_index,
	long node_designator);
static void leaf_map_build_portals(
	struct leaf_map *leaf_map,
	long node_index);
static void leaf_map_build_portals_from_leaf(
	struct leaf_map *leaf_map,
	long ancestor_node_index,
	long leaf_index,
	long node_index,
	short levels_up);
static void leaf_map_build_portal_from_leaves(
	struct leaf_map *leaf_map,
	long node_index,
	long leaf_index0,
	long leaf_index1);

static void leaf_map_family_mark(
	const struct leaf_map *leaf_map,
	long *marked_leaves,
	long leaf_index);

static void leaf_face_get_vertex3d(
	const struct leaf_map *map,
	struct map_leaf_face *face,
	short vertex_index,
	real_point3d *result);

/* ---------- globals */

static struct profile_section leaf_map_initialize_section = {"leaf_map_initialize", NONE, TRUE};

static struct leaf_map_polygon global_leaf_face_polygon =
{
	4,
	{
		{{-1536.f, -1536.f}},
		{{1536.f, -1536.f}},
		{{1536.f, 1536.f}},
		{{-1536.f, 1536.f}}
	}
};

static struct tag_field map_leaf_face_vertex_block_fields[2] =
{
	{ _tag_field_real_point2d, 0, "vertex*", NULL },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition map_leaf_face_vertex_block =
{
	"map_leaf_face_vertex_block",
	0,
	MAXIMUM_LEAF_FACE_VERTICES,
	sizeof(real_point2d),
	NULL,
	map_leaf_face_vertex_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field map_leaf_portal_index_block_fields[2] =
{
	{ _tag_field_long_integer, 0, "portal index*", NULL },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition map_leaf_portal_index_block =
{
	"map_leaf_portal_index_block",
	0,
	256,
	sizeof(long),
	NULL,
	map_leaf_portal_index_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field map_leaf_face_block_fields[3] =
{
	{ _tag_field_long_integer, 0, "node index*", NULL },
	{ _tag_field_block, 0, "vertices*", &map_leaf_face_vertex_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition map_leaf_face_block =
{
	"map_leaf_face_block",
	0,
	256,
	sizeof(struct map_leaf_face),
	NULL,
	map_leaf_face_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field global_map_leaf_block_fields[3] =
{
	{ _tag_field_block, 0, "faces*", &map_leaf_face_block },
	{ _tag_field_block, 0, "portal indices*", &map_leaf_portal_index_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition global_map_leaf_block =
{
	"global_map_leaf_block",
	0,
	0x10000,
	sizeof(struct map_leaf),
	NULL,
	global_map_leaf_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field leaf_portal_vertex_block_fields[2] =
{
	{ _tag_field_real_point3d, 0, "point*", NULL },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition leaf_portal_vertex_block =
{
	"leaf_portal_vertex_block",
	0,
	MAXIMUM_PORTAL_VERTICES,
	sizeof(real_point3d),
	NULL,
	leaf_portal_vertex_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field global_leaf_portal_block_fields[5] =
{
	{ _tag_field_long_integer, 0, "plane index*", NULL },
	{ _tag_field_long_integer, 0, "back leaf index*", NULL },
	{ _tag_field_long_integer, 0, "front leaf index*", NULL },
	{ _tag_field_block, 0, "vertices*", &leaf_portal_vertex_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition global_leaf_portal_block =
{
	"global_leaf_portal_block",
	0,
	0x80000,
	sizeof(struct leaf_portal),
	NULL,
	global_leaf_portal_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct
{
	long node_stack[MAXIMUM_NODE_STACK_COUNT];
	short node_stack_count;
	char const *error;
} leaf_map_globals;

static short portal_count = 0;

/* ---------- public code */

boolean leaf_map_initialize_from_bsp(
	struct leaf_map *leaf_map,
	struct bsp3d const *bsp,
	long leaf_count)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		86,
		leaf_map);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		87,
		bsp);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		88,
		leaf_map_globals.node_stack_count==0);

	leaf_map_globals.error = NULL;

	profile_enter(leaf_map_initialize_section);

	leaf_map->bsp = bsp;

	if (tag_block_resize(&leaf_map->leaves, leaf_count))
	{
		if (bsp->nodes.count > 0)
		{
			leaf_map_build_leaf_faces(leaf_map, 0);
			leaf_map_build_portals(leaf_map, 0);
		}
	}
	else if (!leaf_map_globals.error)
	{
		leaf_map_globals.error = "couldn't allocate leaf_map leaves.";
	}

	if (leaf_map_globals.error)
	{
		error(_error_delayed, leaf_map_globals.error);
	}

	profile_exit(leaf_map_initialize_section);

	return leaf_map_globals.error == NULL;
}

void leaf_map_delete(
	struct leaf_map *leaf_map)
{
	tag_block_resize(&leaf_map->leaves, 0);
	tag_block_resize(&leaf_map->portals, 0);
	return;
}

void leaf_map_close_portal(
	struct leaf_map *leaf_map,
	long portal_index)
{
	struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(&leaf_map->portals, portal_index, struct leaf_portal);
	long side;

	for (side = 0; side < 2; side++)
	{
		struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, portal->leaf_indices[side] & LONG_MAX, struct map_leaf);
		short portal_designator_index;

		for (portal_designator_index = 0;
			portal_designator_index < leaf->portal_designators.count;
			portal_designator_index++)
		{
			long *portal_designator = TAG_BLOCK_GET_ELEMENT(&leaf->portal_designators, portal_designator_index, long);

			if ((*portal_designator & LONG_MAX) == portal_index)
			{
				*portal_designator |= LONG_MIN;
				break;
			}
		}

		match_assert(
			"c:\\halo\\SOURCE\\structures\\leaf_map.c",
			673,
			portal_designator_index!=leaf->portal_designators.count);
	}

	return;
}

boolean leaf_map_leaf_is_closed(
	const struct leaf_map *leaf_map,
	long leaf_index)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	short portal_designator_index;

	for (portal_designator_index = 0;
		portal_designator_index < leaf->portal_designators.count;
		portal_designator_index++)
	{
		long portal_designator = *TAG_BLOCK_GET_ELEMENT(&leaf->portal_designators, portal_designator_index, long);

		if (!(portal_designator & LONG_MIN))
		{
			return FALSE;
		}
	}

	return (boolean)leaf->portal_designators.count;
}

void render_debug_leaf_portal(
	const struct leaf_map *leaf_map,
	long portal_designator)
{
	struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(&leaf_map->portals, portal_designator & LONG_MAX, struct leaf_portal);
	boolean closed = (portal_designator & LONG_MIN) != 0;
	real_argb_color colors[2] = {{{0.1f, 0.0f, 1.0f, 0.0f}}, {{0.1f, 1.0f, 0.0f, 0.0f}}};
	short vertex_index;

	for (vertex_index = 2; vertex_index < portal->vertices.count; vertex_index++)
	{
		render_debug_triangle(
			TRUE,
			TAG_BLOCK_GET_ELEMENT(&portal->vertices, 0, real_point3d),
			TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index - 1, real_point3d),
			TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index, real_point3d),
			&colors[closed]);
		render_debug_line(
			TRUE,
			TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index - 1, real_point3d),
			TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index, real_point3d),
			global_real_argb_red);
	}

	render_debug_line(
		TRUE,
		TAG_BLOCK_GET_ELEMENT(&portal->vertices, 0, real_point3d),
		TAG_BLOCK_GET_ELEMENT(&portal->vertices, 1, real_point3d),
		global_real_argb_red);
	render_debug_line(
		TRUE,
		TAG_BLOCK_GET_ELEMENT(&portal->vertices, 0, real_point3d),
		TAG_BLOCK_GET_ELEMENT(&portal->vertices, portal->vertices.count - 1, real_point3d),
		global_real_argb_red);

	return;
}

boolean leaf_map_leaf_spans_polygon(
	const struct leaf_map *leaf_map,
	long leaf_index,
	const real_plane3d *plane,
	short projection,
	boolean sign,
	long vertex_count,
	const real_point2d *vertices)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	short face_index;

	for (face_index = 0; face_index < leaf->faces.count; face_index++)
	{
		struct map_leaf_face *face = TAG_BLOCK_GET_ELEMENT(&leaf->faces, face_index, struct map_leaf_face);
		struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, face->node_index, struct bsp3d_node);
		real_plane3d *face_plane = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->planes, node->plane_designator, real_plane3d);
		short face_projection = projection_from_vector3d(&face_plane->n);
		boolean face_sign = projection_sign_from_vector3d(&face_plane->n, face_projection);
		short vertex_index;

		for (vertex_index = 0; vertex_index < face->vertices.count; vertex_index++)
		{
			real_point2d point;
			real_point3d point0;
			real_point3d point1;
			real_point3d intersection;
			real_vector3d delta;
			long next_vertex_index;
			real distance0, distance1;

			project_point2d(
				TAG_BLOCK_GET_ELEMENT(&face->vertices, vertex_index, real_point2d),
				face_plane,
				face_projection,
				face_sign,
				&point0);

			next_vertex_index = (vertex_index + 1 == face->vertices.count) ? 0 : vertex_index + 1;

			project_point2d(
				TAG_BLOCK_GET_ELEMENT(&face->vertices, next_vertex_index, real_point2d),
				face_plane,
				face_projection,
				face_sign,
				&point1);

			distance0 = plane3d_distance_to_point(plane, &point0);
			distance1 = plane3d_distance_to_point(plane, &point1);

			if ((distance0 < -0.03f && distance1 > 0.03f) ||
				(distance0 > 0.03f && distance1 < -0.03f))
			{
				real intersection_t;

				vector_from_points3d(&point0, &point1, &delta);
				intersection_t= vector_intersect_plane3d(&point0, &delta, plane);
				point_from_line3d(&point0, &delta, intersection_t, &intersection);
				project_point3d(&intersection, projection, sign, &point);

				if (convex_hull2d_test_point(vertex_count, vertices, &point, 0.05f))
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void leaf_map_get_leaf_bounds(
	const struct leaf_map *leaf_map,
	long leaf_index,
	real_point3d *center,
	real *radius)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	real_rectangle3d bounds = *global_null_rectangle3d;
	real_point3d corner;

	if (leaf->portal_designators.count != 0)
	{
		short portal_designator_index;

		for (portal_designator_index = 0;
			portal_designator_index < leaf->portal_designators.count;
			portal_designator_index++)
		{
			struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(&leaf_map->portals,
				*TAG_BLOCK_GET_ELEMENT(&leaf->portal_designators, portal_designator_index, long) & LONG_MAX,
				struct leaf_portal);
			short vertex_index;

			for (vertex_index = 0; vertex_index < portal->vertices.count; vertex_index++)
			{
				real_point3d *vertex = TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index, real_point3d);

				if (vertex->x < bounds.x0)
				{
					bounds.x0 = vertex->x;
				}
				if (vertex->x > bounds.x1)
				{
					bounds.x1 = vertex->x;
				}
				if (vertex->y < bounds.y0)
				{
					bounds.y0 = vertex->y;
				}
				if (vertex->y > bounds.y1)
				{
					bounds.y1 = vertex->y;
				}
				if (vertex->z < bounds.z0)
				{
					bounds.z0 = vertex->z;
				}
				if (vertex->z > bounds.z1)
				{
					bounds.z1 = vertex->z;
				}
			}
		}
	}
	else if (leaf->faces.count != 0)
	{
		short face_index;

		for (face_index = 0; face_index < leaf->faces.count; face_index++)
		{
			struct map_leaf_face *face = TAG_BLOCK_GET_ELEMENT(&leaf->faces, face_index, struct map_leaf_face);
			struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, face->node_index, struct bsp3d_node);
			real_plane3d *plane = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->planes, node->plane_designator, real_plane3d);
			short projection = projection_from_vector3d(&plane->n);
			boolean sign = projection_sign_from_vector3d(&plane->n, projection);
			real_point3d point;
			short vertex_index;

			for (vertex_index = 0; vertex_index < face->vertices.count; vertex_index++)
			{
				project_point2d(
					TAG_BLOCK_GET_ELEMENT(&face->vertices, vertex_index, real_point2d),
					plane,
					projection,
					sign,
					&point);

				if (point.x < bounds.x0)
				{
					bounds.x0 = point.x;
				}
				if (point.x > bounds.x1)
				{
					bounds.x1 = point.x;
				}
				if (point.y < bounds.y0)
				{
					bounds.y0 = point.y;
				}
				if (point.y > bounds.y1)
				{
					bounds.y1 = point.y;
				}
				if (point.z < bounds.z0)
				{
					bounds.z0 = point.z;
				}
				if (point.z > bounds.z1)
				{
					bounds.z1 = point.z;
				}
			}
		}
	}

	center->x = (bounds.x1 + bounds.x0) * 0.5f;
	center->y = (bounds.y1 + bounds.y0) * 0.5f;
	center->z = (bounds.z1 + bounds.z0) * 0.5f;

	corner.x = bounds.x1;
	corner.y = bounds.y1;
	corner.z = bounds.z1;

	*radius = distance3d(center, &corner);

	return;
}

void render_debug_leaf_portals(
	const struct leaf_map *leaf_map,
	long leaf_index)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	short portal_designator_index;

	for (portal_designator_index = 0;
		portal_designator_index < leaf->portal_designators.count;
		portal_designator_index++)
	{
		long portal_designator = *TAG_BLOCK_GET_ELEMENT(&leaf->portal_designators, portal_designator_index, long);

		render_debug_leaf_portal(leaf_map, portal_designator);
	}

	return;
}

void render_debug_leaf_faces(
	const struct leaf_map *map,
	long leaf_index)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	real_argb_color color = {{0.2f, 1.0f, 0.0f, 0.0f}};
	real_point3d first_vertex;
	real_point3d second_vertex;
	real_point3d next_vertex;
	real_point3d previous_vertex;
	short face_index;

	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		940,
		map->bsp);

	for (face_index = 0; face_index < leaf->faces.count; face_index++)
	{
		struct map_leaf_face *face = TAG_BLOCK_GET_ELEMENT(&leaf->faces, face_index, struct map_leaf_face);
		short vertex_index;

		leaf_face_get_vertex3d(map, face, 0, &first_vertex);
		leaf_face_get_vertex3d(map, face, 1, &second_vertex);
		previous_vertex = second_vertex;
		render_debug_line(TRUE, &first_vertex, &second_vertex, global_real_argb_white);

		for (vertex_index = 2; vertex_index < face->vertices.count; vertex_index++)
		{
			leaf_face_get_vertex3d(map, face, vertex_index, &next_vertex);
			render_debug_triangle(TRUE, &first_vertex, &previous_vertex, &next_vertex, &color);
			render_debug_line(TRUE, &previous_vertex, &next_vertex, global_real_argb_white);
			previous_vertex = next_vertex;
		}
	}

	return;
}

boolean leaf_map_closure(
	const struct leaf_map *leaf_map,
	long *marked_leaves,
	long *result)
{
	long leaf_index;

	if (marked_leaves != result)
	{
		csmemcpy(result, marked_leaves, BIT_VECTOR_SIZE_IN_BYTES(leaf_map->leaves.count));
	}

	for (leaf_index = 0; leaf_index < leaf_map->leaves.count; leaf_index++)
	{
		if (BIT_VECTOR_TEST_FLAG(result, leaf_index))
		{
			leaf_map_family_mark(leaf_map, result, leaf_index);
		}
	}

	return TRUE;
}

/* ---------- private code */

static void node_stack_push(
	long node_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		42,
		leaf_map_globals.node_stack_count<MAXIMUM_NODE_STACK_COUNT);

	leaf_map_globals.node_stack[leaf_map_globals.node_stack_count++] = node_index;

	return;
}

static long node_stack_pop(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		51,
		leaf_map_globals.node_stack_count>0);

	return leaf_map_globals.node_stack[--leaf_map_globals.node_stack_count];
}

static long node_stack_read(
	short levels_up)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		59,
		levels_up>=0 && levels_up<leaf_map_globals.node_stack_count);

	return leaf_map_globals.node_stack[leaf_map_globals.node_stack_count - levels_up - 1];
}

static boolean find_like_crossing(
	struct leaf_map const *leaf_map,
	long plane_designator,
	boolean *side)
{
	boolean found = FALSE;
	short levels_up;

	for (levels_up = 0; levels_up < leaf_map_globals.node_stack_count; levels_up++)
	{
		long traversal_node = node_stack_read(levels_up);
		struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(
			&leaf_map->bsp->nodes,
			index_from_node(traversal_node),
			struct bsp3d_node);

		if (node->plane_designator == plane_designator)
		{
			*side = (traversal_node & LONG_MIN) != 0;
			found = TRUE;
			break;
		}
	}

	return found;
}

static short map_leaf_find_face_on_node(
	struct leaf_map const *leaf_map,
	struct map_leaf const *leaf,
	long node_index)
{
	short face_index;

	for (face_index = 0; face_index < leaf->faces.count; face_index++)
	{
		struct map_leaf_face *face = TAG_BLOCK_GET_ELEMENT(
			&leaf->faces,
			face_index,
			struct map_leaf_face);

		if (face->node_index == node_index)
		{
			return face_index;
		}
	}

	return NONE;
}

static real normalize_three_dee(
	real_vector3d *vector)
{
	real length = magnitude3d(vector);
	real inverse_length = 1.f / length;

	if (valid_real(inverse_length))
	{
		scale_vector3d(vector, inverse_length, vector);

		if (valid_real_normal3d(vector))
		{
			return length;
		}
	}

	return 0.f;
}

static short intersect_planes3d(
	real_plane3d const *reference_plane,
	real_plane3d const *plane,
	real_plane2d *result)
{
	short projection = projection_from_vector3d(&reference_plane->n);
	real_plane3d projected_plane;
	real length;

	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		299,
		reference_plane->n.n[projection]!=0.f);
	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		300,
		result);

	if (plane->n.n[projection] == 0.f)
	{
		projected_plane = *plane;
	}
	else
	{
		real scale = plane->n.n[projection] / reference_plane->n.n[projection];

		projected_plane.n.i = plane->n.i - scale * reference_plane->n.i;
		projected_plane.n.j = plane->n.j - scale * reference_plane->n.j;
		projected_plane.n.k = plane->n.k - scale * reference_plane->n.k;
		projected_plane.d = plane->d - scale * reference_plane->d;
	}

	length = normalize_three_dee(&projected_plane.n);

	if (length != 0.f)
	{
		project_point3d(
			(real_point3d const *)&projected_plane.n,
			projection,
			projection_sign_from_vector3d(&reference_plane->n, projection),
			(real_point2d *)&result->n);
		result->d = projected_plane.d / length;

		return _projected_plane_valid;
	}

	if (projected_plane.d > 0.f)
	{
		return _projected_plane_empty;
	}

	return _projected_plane_full;
}

static void leaf_map_build_leaf_faces(
	struct leaf_map *leaf_map,
	long node_index)
{
	struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, node_index, struct bsp3d_node);
	short child_index;

	for (child_index = 0; child_index < 2; child_index++)
	{
		long child_node_index;

		node_stack_push(child_index != 0 ? node_index : (node_index | LONG_MIN));

		child_node_index = node->children[child_index];

		if (child_node_index & LONG_MIN)
		{
			if (child_node_index != NONE)
			{
				leaf_map_build_leaf_faces_for_leaf(leaf_map, child_node_index);
			}
		}
		else
		{
			leaf_map_build_leaf_faces(leaf_map, child_node_index);
		}

		node_stack_pop();
	}

	return;
}

static void leaf_map_build_leaf_faces_for_leaf(
	struct leaf_map *leaf_map,
	long leaf_index)
{
	short levels_up;

	for (levels_up = 0; levels_up < leaf_map_globals.node_stack_count; levels_up++)
	{
		leaf_map_build_leaf_face_for_leaf_on_node(leaf_map, leaf_index, node_stack_read(levels_up));
	}

	return;
}

static void leaf_map_build_leaf_face_for_leaf_on_node(
	struct leaf_map *leaf_map,
	long leaf_index,
	long node_designator)
{
	long node_index = index_from_node(node_designator);
	real_plane3d *reference_plane = TAG_BLOCK_GET_ELEMENT(
		&leaf_map->bsp->planes,
		TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, node_index, struct bsp3d_node)->plane_designator,
		real_plane3d);
	struct leaf_map_polygon result = global_leaf_face_polygon;
	short levels_up;

	for (levels_up = 0; levels_up < leaf_map_globals.node_stack_count; levels_up++)
	{
		long traversal_node;
		real_plane3d plane;
		real_plane2d projected_plane;
		short projection_result;

		if (result.vertex_count == 0)
		{
			break;
		}

		traversal_node = node_stack_read(levels_up);

		if (traversal_node != node_designator)
		{
			plane = *TAG_BLOCK_GET_ELEMENT(
				&leaf_map->bsp->planes,
				TAG_BLOCK_GET_ELEMENT(
					&leaf_map->bsp->nodes,
					index_from_node(traversal_node),
					struct bsp3d_node)->plane_designator,
				real_plane3d);

			if (traversal_node & LONG_MIN)
			{
				plane3d_negate(&plane, &plane);
			}

			projection_result = intersect_planes3d(reference_plane, &plane, &projected_plane);

			if (projection_result == _projected_plane_valid)
			{
				result.vertex_count = convex_polygon2d_clip_to_plane(
					result.vertex_count,
					result.vertices,
					&projected_plane,
					MAXIMUM_LEAF_FACE_VERTICES,
					result.vertices,
					NULL,
					NULL,
					0.000244140625f);

				match_assert(
					"c:\\halo\\SOURCE\\structures\\leaf_map.c",
					227,
					result.vertex_count!=NONE);
			}
			else if (projection_result == _projected_plane_empty)
			{
				result.vertex_count = 0;
			}
		}
	}

	if (result.vertex_count != 0)
	{
		struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(
			&leaf_map->leaves,
			index_from_node(leaf_index),
			struct map_leaf);
		short face_index = (short)tag_block_add_element(&leaf->faces);

		if (face_index != NONE)
		{
			struct map_leaf_face *face = TAG_BLOCK_GET_ELEMENT(
				&leaf->faces,
				face_index,
				struct map_leaf_face);

			face->node_index = node_index;

			if (tag_block_resize(&face->vertices, result.vertex_count))
			{
				csmemcpy(face->vertices.address, result.vertices, result.vertex_count * sizeof(real_point2d));
			}
			else if (!leaf_map_globals.error)
			{
				leaf_map_globals.error = "couldn't allocate leaf vertices.";
			}
		}
		else if (!leaf_map_globals.error)
		{
			leaf_map_globals.error = "couldn't allocate leaf face.";
		}
	}

	return;
}

static void leaf_map_build_portals(
	struct leaf_map *leaf_map,
	long node_index)
{
	struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, node_index, struct bsp3d_node);
	short child_index;

	for (child_index = 0; child_index < 2; child_index++)
	{
		long child_node_index;

		node_stack_push(child_index != 0 ? node_index : (node_index | LONG_MIN));

		child_node_index = node->children[child_index];

		if (child_node_index & LONG_MIN)
		{
			if (child_node_index != NONE)
			{
				leaf_map_build_portals_from_leaf(
					leaf_map,
					NONE,
					index_from_node(child_node_index),
					0,
					(short)(leaf_map_globals.node_stack_count - 1));
			}
		}
		else
		{
			leaf_map_build_portals(leaf_map, child_node_index);
		}

		node_stack_pop();
	}

	return;
}

static void leaf_map_build_portals_from_leaf(
	struct leaf_map *leaf_map,
	long ancestor_node_index,
	long leaf_index,
	long node_index,
	short levels_up)
{
	struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&leaf_map->bsp->nodes, node_index, struct bsp3d_node);
	long first_traversal_node = ancestor_node_index == NONE ? node_stack_read(levels_up) : NONE;
	boolean side;
	boolean plane_on_stack = find_like_crossing(leaf_map, node->plane_designator, &side);
	short child_index;

	match_assert(
		"c:\\halo\\SOURCE\\structures\\leaf_map.c",
		415,
		ancestor_node_index!=NONE || index_from_node(first_traversal_node)==node_index);

	for (child_index = 0; child_index < 2; child_index++)
	{
		boolean descend_from_this_node = ancestor_node_index == NONE && child_index != 0 && (first_traversal_node & LONG_MIN) != 0;
		long child_node_index;

		if (ancestor_node_index == NONE && child_index == 0 && !(first_traversal_node & LONG_MIN))
		{
			continue;
		}

		if (descend_from_this_node)
		{
			short face_index = map_leaf_find_face_on_node(
				leaf_map,
				TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, index_from_node(leaf_index), struct map_leaf),
				node_index);

			if (face_index == NONE)
			{
				continue;
			}
		}
		else if (ancestor_node_index != NONE && plane_on_stack && side == child_index)
		{
			continue;
		}

		child_node_index = node->children[child_index];

		if (child_node_index & LONG_MIN)
		{
			if (child_node_index != NONE && index_from_node(child_node_index) != leaf_index)
			{
				leaf_map_build_portal_from_leaves(
					leaf_map,
					descend_from_this_node ? node_index : ancestor_node_index,
					leaf_index,
					child_node_index);
			}
		}
		else
		{
			leaf_map_build_portals_from_leaf(
				leaf_map,
				descend_from_this_node ? node_index : ancestor_node_index,
				leaf_index,
				child_node_index,
				(short)(levels_up - 1));
		}
	}

	return;
}

static void leaf_map_build_portal_from_leaves(
	struct leaf_map *leaf_map,
	long node_index,
	long leaf_index0,
	long leaf_index1)
{
	struct map_leaf *leaf0 = TAG_BLOCK_GET_ELEMENT(
		&leaf_map->leaves,
		index_from_node(leaf_index0),
		struct map_leaf);
	struct map_leaf *leaf1 = TAG_BLOCK_GET_ELEMENT(
		&leaf_map->leaves,
		index_from_node(leaf_index1),
		struct map_leaf);
	short face_index0 = map_leaf_find_face_on_node(leaf_map, leaf0, node_index);
	short face_index1 = map_leaf_find_face_on_node(leaf_map, leaf1, node_index);
	real_point2d vertices[MAXIMUM_PORTAL_VERTICES];

	if (face_index0 != NONE && face_index1 != NONE)
	{
		struct map_leaf_face *face0 = TAG_BLOCK_GET_ELEMENT(&leaf0->faces, face_index0, struct map_leaf_face);
		struct map_leaf_face *face1 = TAG_BLOCK_GET_ELEMENT(&leaf1->faces, face_index1, struct map_leaf_face);
		short vertex_count;

		match_assert(
			"c:\\halo\\SOURCE\\structures\\leaf_map.c",
			478,
			leaf_index0!=leaf_index1);

		vertex_count = convex_hull2d_intersect(
			(word)face0->vertices.count,
			face0->vertices.address,
			(word)face1->vertices.count,
			face1->vertices.address,
			MAXIMUM_PORTAL_VERTICES,
			vertices,
			0.00048828125f);

		if (vertex_count > 0)
		{
			long portal_index = tag_block_add_element(&leaf_map->portals);
			short portal_designator_index0 = (short)tag_block_add_element(&leaf0->portal_designators);
			short portal_designator_index1 = (short)tag_block_add_element(&leaf1->portal_designators);

			/* BUG (preserved for exact matching): January checks vertex_count
			 * against NONE only after entering the positive-count branch, where
			 * the assertion cannot fail. A corrected build should perform this
			 * check immediately after convex_hull2d_intersect returns. */
			match_vassert(
				"c:\\halo\\SOURCE\\structures\\leaf_map.c",
				487,
				vertex_count!=NONE,
				"too many vertices in portal");

			if (portal_index != NONE && portal_designator_index0 != NONE && portal_designator_index1 != NONE)
			{
				struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(
					&leaf_map->portals,
					portal_index,
					struct leaf_portal);
				real_plane3d *plane = TAG_BLOCK_GET_ELEMENT(
					&leaf_map->bsp->planes,
					TAG_BLOCK_GET_ELEMENT(
						&leaf_map->bsp->nodes,
						node_index,
						struct bsp3d_node)->plane_designator,
					real_plane3d);
				short projection;
				boolean sign;
				real area;

				portal->plane_index = TAG_BLOCK_GET_ELEMENT(
					&leaf_map->bsp->nodes,
					node_index,
					struct bsp3d_node)->plane_designator;

				projection = projection_from_vector3d(&plane->n);
				sign = projection_sign_from_vector3d(&plane->n, projection);

				portal->leaf_indices[0] = index_from_node(leaf_index0);
				portal->leaf_indices[1] = index_from_node(leaf_index1);

				*TAG_BLOCK_GET_ELEMENT(&leaf0->portal_designators, portal_designator_index0, long) = portal_index;
				*TAG_BLOCK_GET_ELEMENT(&leaf1->portal_designators, portal_designator_index1, long) = portal_index;

				if (tag_block_resize(&portal->vertices, vertex_count))
				{
					short vertex_index;

					for (vertex_index = 0; vertex_index < vertex_count; vertex_index++)
					{
						project_point2d(
							&vertices[vertex_index],
							plane,
							projection,
							sign,
							TAG_BLOCK_GET_ELEMENT(&portal->vertices, vertex_index, real_point3d));
					}
				}

				area = convex_hull2d_area(vertex_count, vertices);

				if (area < 0.0025f ||
					square_root(area) / convex_hull2d_perimeter(vertex_count, vertices) < 0.01f)
				{
					leaf_map_close_portal(leaf_map, portal_index);
				}
			}
			else if (!leaf_map_globals.error)
			{
				leaf_map_globals.error = portal_index == NONE
					? "couldn't allocate leaf map portal"
					: "couldn't allocate leaf map portal designator.";
			}
		}
		else
		{
			match_vassert(
				"c:\\halo\\SOURCE\\structures\\leaf_map.c",
				541,
				vertex_count!=NONE,
				NULL);
		}
	}

	return;
}

static void leaf_face_get_vertex3d(
	const struct leaf_map *map,
	struct map_leaf_face *face,
	short vertex_index,
	real_point3d *result)
{
	struct bsp3d_node *node = TAG_BLOCK_GET_ELEMENT(&map->bsp->nodes, face->node_index, struct bsp3d_node);
	real_plane3d *plane = TAG_BLOCK_GET_ELEMENT(&map->bsp->planes, node->plane_designator, real_plane3d);
	short projection = projection_from_vector3d(&plane->n);
	boolean sign = projection_sign_from_vector3d(&plane->n, projection);

	project_point2d(
		TAG_BLOCK_GET_ELEMENT(&face->vertices, vertex_index, real_point2d),
		plane,
		projection,
		sign,
		result);

	return;
}

static void leaf_map_family_mark(
	const struct leaf_map *leaf_map,
	long *marked_leaves,
	long leaf_index)
{
	struct map_leaf *leaf = TAG_BLOCK_GET_ELEMENT(&leaf_map->leaves, leaf_index & LONG_MAX, struct map_leaf);
	short portal_designator_index;

	for (portal_designator_index = 0;
		portal_designator_index < leaf->portal_designators.count;
		portal_designator_index++)
	{
		struct leaf_portal *portal = TAG_BLOCK_GET_ELEMENT(&leaf_map->portals,
			*TAG_BLOCK_GET_ELEMENT(&leaf->portal_designators, portal_designator_index, long) & LONG_MAX,
			struct leaf_portal);
		long neighbor_leaf_index = (portal->leaf_indices[0] != leaf_index)
			? portal->leaf_indices[0]
			: portal->leaf_indices[1];

		if (!BIT_VECTOR_TEST_FLAG(marked_leaves, neighbor_leaf_index))
		{
			BIT_VECTOR_SET_FLAG(marked_leaves, neighbor_leaf_index, TRUE);
			leaf_map_family_mark(leaf_map, marked_leaves, neighbor_leaf_index);
		}
	}

	return;
}
