/*
COLLISION_BSP_DEFINITIONS.C

symbols in this file:
0028B098 0007:
	??_C@_06EBBNEIN@point?$CK?$AA@ (0000)
0028B0A0 0007:
	??_C@_06HLKHCFCI@vertex?$AA@ (0000)
0028B0A8 000a:
	??_C@_09MDMJIKHH@vertices?$CK?$AA@ (0000)
0028B0B4 000f:
	??_C@_0P@NMHHINEP@right?5surface?$CK?$AA@ (0000)
0028B0C4 000e:
	??_C@_0O@HJALJEGK@left?5surface?$CK?$AA@ (0000)
0028B0D4 000e:
	??_C@_0O@DEEPODI@reverse?5edge?$CK?$AA@ (0000)
0028B0E4 000e:
	??_C@_0O@KENBEDCK@forward?5edge?$CK?$AA@ (0000)
0028B0F4 000c:
	??_C@_0M@KMPMFKHO@end?5vertex?$CK?$AA@ (0000)
0028B100 000e:
	??_C@_0O@GKJFIJMB@start?5vertex?$CK?$AA@ (0000)
0028B110 0005:
	??_C@_04IJFJNCMM@edge?$AA@ (0000)
0028B118 0007:
	??_C@_06DGEOLIMC@edges?$CK?$AA@ (0000)
0028B120 000a:
	??_C@_09PNBEKGHK@material?$CK?$AA@ (0000)
0028B12C 0013:
	??_C@_0BD@GCFCIGCE@breakable?5surface?$CK?$AA@ (0000)
0028B140 000a:
	??_C@_09KMPBHCFD@breakable?$AA@ (0000)
0028B14C 000a:
	??_C@_09JADBKOHN@climbable?$AA@ (0000)
0028B158 000a:
	??_C@_09MLILFJ@invisible?$AA@ (0000)
0028B164 000a:
	??_C@_09FDDODKJD@two?5sided?$AA@ (0000)
0028B170 000c:
	??_C@_0M@HOEBBLL@first?5edge?$CK?$AA@ (0000)
0028B17C 0008:
	??_C@_07IHELBNLD@surface?$AA@ (0000)
0028B184 000a:
	??_C@_09JFDCAMMI@surfaces?$CK?$AA@ (0000)
0028B190 000d:
	??_C@_0N@NMJAHBMO@bsp2d?5nodes?$CK?$AA@ (0000)
0028B1A0 000d:
	??_C@_0N@OLKADIIH@right?5child?$CK?$AA@ (0000)
0028B1B0 000c:
	??_C@_0M@JLKPIPMH@left?5child?$CK?$AA@ (0000)
0028B1BC 000b:
	??_C@_0L@NMLDHHAD@bsp2d?5node?$AA@ (0000)
0028B1C8 000c:
	??_C@_0M@PGLOCOOF@bsp2d?5node?$CK?$AA@ (0000)
0028B1D4 0010:
	??_C@_0BA@CPJOPKHG@bsp2d?5reference?$AA@ (0000)
0028B1E4 0012:
	??_C@_0BC@BNNBPAMN@bsp2d?5references?$CK?$AA@ (0000)
0028B1F8 0017:
	??_C@_0BH@LLALOJK@first?5bsp2d?5reference?$CK?$AA@ (0000)
0028B210 0017:
	??_C@_0BH@MBAEJDPN@bsp2d?5reference?5count?$CK?$AA@ (0000)
0028B228 001f:
	??_C@_0BP@BLGNMLGF@contains?5double?9sided?5surfaces?$AA@ (0000)
0028B248 0005:
	??_C@_04BDFFPIKJ@leaf?$AA@ (0000)
0028B250 0008:
	??_C@_07EEAJCCKA@leaves?$CK?$AA@ (0000)
0028B258 0006:
	??_C@_05MAIECAHP@plane?$AA@ (0000)
0028B260 0008:
	??_C@_07KKBFJPFE@planes?$CK?$AA@ (0000)
0028B268 000d:
	??_C@_0N@FFEJCMDL@front?5child?$CK?$AA@ (0000)
0028B278 000c:
	??_C@_0M@FHBHGJLP@back?5child?$CK?$AA@ (0000)
0028B284 0007:
	??_C@_06KPBBCEKF@plane?$CK?$AA@ (0000)
0028B28C 000b:
	??_C@_0L@BABJHHJN@bsp3d?5node?$AA@ (0000)
0028B298 000d:
	??_C@_0N@DDFCBKPA@bsp3d?5nodes?$CK?$AA@ (0000)
0030C9C0 0384:
	_data_0030c9c0 (0000)
	_global_collision_bsp_fields (0318)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "physics/collision_bsp_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct collision_bsp_flags_definition
{
	long count;
	char **names;
};

struct collision_bsp_definition_data
{
	struct tag_field bsp3d_node_fields[4];
	struct tag_block_definition bsp3d_node_block;
	struct tag_field plane_fields[2];
	struct tag_block_definition plane_block;
	char *leaf_flag_names[1];
	struct collision_bsp_flags_definition leaf_flags;
	struct tag_field leaf_fields[4];
	struct tag_block_definition leaf_block;
	struct tag_field bsp2d_node_fields[4];
	struct tag_block_definition bsp2d_node_block;
	struct tag_field bsp2d_reference_fields[3];
	struct tag_block_definition bsp2d_reference_block;
	char *surface_flag_names[4];
	struct collision_bsp_flags_definition surface_flags;
	long pad0;
	struct tag_field surface_fields[6];
	struct tag_block_definition surface_block;
	long pad1;
	struct tag_field edge_fields[7];
	struct tag_block_definition edge_block;
	struct tag_field vertex_fields[3];
	struct tag_block_definition vertex_block;
};

typedef char collision_bsp_definition_data_size_assert[
	sizeof(struct collision_bsp_definition_data) == 0x318 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

/* This object's .data is byte-exact against January once csplit resolves the
 * 22 internal pointers correctly.  config/symbols.json lists
 * _global_collision_bsp_definition_data (file_offset 3197376) ahead of
 * _debug_objects_collision_models (3197369) and
 * _breakable_surface_effect_enabled (3197370) -- the file's only descending
 * step in 23,407 entries.  csplit builds its address->symbol map from that
 * order, so the whole blob falls inside breakable_surface_effect_enabled and
 * every internal pointer is emitted as that symbol + 6 + offset.  Restoring
 * ascending order makes the regenerated split .data byte-identical to ours
 * with all 68 relocations equal, and changes no other split object. */
struct collision_bsp_definition_data global_collision_bsp_definition_data =
{
	{
		{ _tag_field_long_integer, 0, "plane*", NULL },
		{ _tag_field_long_integer, 0, "back child*", NULL },
		{ _tag_field_long_integer, 0, "front child*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"bsp3d node", 0, 0x20000, sizeof(struct bsp3d_node), NULL,
		global_collision_bsp_definition_data.bsp3d_node_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{
		{ _tag_field_real_plane3d, 0, "plane*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"plane", 0, 0x10000, sizeof(real_plane3d), NULL,
		global_collision_bsp_definition_data.plane_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{ "contains double-sided surfaces" },
	{ 1, global_collision_bsp_definition_data.leaf_flag_names },
	{
		{ _tag_field_word_flags, 0, "flags*", &global_collision_bsp_definition_data.leaf_flags },
		{ _tag_field_short_integer, 0, "bsp2d reference count*", NULL },
		{ _tag_field_long_integer, 0, "first bsp2d reference*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"leaf", 0, 0x10000, 8, NULL,
		global_collision_bsp_definition_data.leaf_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{
		{ _tag_field_real_plane2d, 0, "plane*", NULL },
		{ _tag_field_long_integer, 0, "left child*", NULL },
		{ _tag_field_long_integer, 0, "right child*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"bsp2d node", 0, 0xFFFF, sizeof(struct bsp2d_node), NULL,
		global_collision_bsp_definition_data.bsp2d_node_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{
		{ _tag_field_long_integer, 0, "plane*", &global_collision_bsp_definition_data.plane_block },
		{ _tag_field_long_integer, 0, "bsp2d node*", &global_collision_bsp_definition_data.bsp2d_node_block },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"bsp2d reference", 0, 0x20000, 8, NULL,
		global_collision_bsp_definition_data.bsp2d_reference_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{ "two sided", "invisible", "climbable", "breakable" },
	{ 4, global_collision_bsp_definition_data.surface_flag_names },
	0,
	{
		{ _tag_field_long_integer, 0, "plane*", NULL },
		{ _tag_field_long_integer, 0, "first edge*", NULL },
		{ _tag_field_byte_flags, 0, "flags*", &global_collision_bsp_definition_data.surface_flags },
		{ _tag_field_char_integer, 0, "breakable surface*", NULL },
		{ _tag_field_short_integer, 0, "material*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"surface", 0, 0x20000, sizeof(struct collision_surface), NULL,
		global_collision_bsp_definition_data.surface_fields, NULL, NULL, NULL, NULL, NULL,
	},
	0,
	{
		{ _tag_field_long_integer, 0, "start vertex*", NULL },
		{ _tag_field_long_integer, 0, "end vertex*", NULL },
		{ _tag_field_long_integer, 0, "forward edge*", NULL },
		{ _tag_field_long_integer, 0, "reverse edge*", NULL },
		{ _tag_field_long_integer, 0, "left surface*", NULL },
		{ _tag_field_long_integer, 0, "right surface*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"edge", 0, 0x40000, sizeof(struct collision_edge), NULL,
		global_collision_bsp_definition_data.edge_fields, NULL, NULL, NULL, NULL, NULL,
	},
	{
		{ _tag_field_real_point3d, 0, "point*", NULL },
		{ _tag_field_long_integer, 0, "first edge*", NULL },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
	{
		"vertex", 0, 0x20000, sizeof(struct collision_vertex), NULL,
		global_collision_bsp_definition_data.vertex_fields, NULL, NULL, NULL, NULL, NULL,
	},
};

struct tag_field global_collision_bsp_fields[9] =
{
	{ _tag_field_block, 0, "bsp3d nodes*", &global_collision_bsp_definition_data.bsp3d_node_block },
	{ _tag_field_block, 0, "planes*", &global_collision_bsp_definition_data.plane_block },
	{ _tag_field_block, 0, "leaves*", &global_collision_bsp_definition_data.leaf_block },
	{ _tag_field_block, 0, "bsp2d references*", &global_collision_bsp_definition_data.bsp2d_reference_block },
	{ _tag_field_block, 0, "bsp2d nodes*", &global_collision_bsp_definition_data.bsp2d_node_block },
	{ _tag_field_block, 0, "surfaces*", &global_collision_bsp_definition_data.surface_block },
	{ _tag_field_block, 0, "edges*", &global_collision_bsp_definition_data.edge_block },
	{ _tag_field_block, 0, "vertices*", &global_collision_bsp_definition_data.vertex_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

/* ---------- public code */

/* ---------- private code */
