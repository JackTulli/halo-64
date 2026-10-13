/*
BSP2D.C

symbols in this file:
00136590 0070:
	_bsp2d_test_point (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bsp2d.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

long bsp2d_test_point(
	struct tag_block const *nodes,
	real_point2d const *point,
	long node_index)
{
	while (!(node_index & LONG_MIN))
	{
		struct bsp2d_node const *node = TAG_BLOCK_GET_ELEMENT(nodes, node_index, struct bsp2d_node);
		real distance = (node->plane.n.i * point->x + node->plane.n.j * point->y) - node->plane.d;
		node_index = node->child_indices[distance >= 0.0f];
	}

	return node_index != NONE ? node_index & LONG_MAX : NONE;
}

/* ---------- private code */
