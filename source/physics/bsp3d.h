/*
BSP3D.H

header included in hcex build.
*/

#ifndef __BSP3D_H
#define __BSP3D_H
#pragma once


/* ---------- headers */

#include "tag_files/tag_groups.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

#ifdef HALO_N64_CAMPAIGN
/* The campaign cartridge's collision is half the size (tools/n64/
collision.py): 16-bit indices. A BSP's child (a node, a leaf, NONE: solid),
a 2D root (a node or a surface) and a reference's plane designator (flipped
or not) keep their flag in bit 15 and NONE is 0xFFFF; BSP_FLAGGED_INDEX
gives the 32-bit form the rest of the code reads (the flag in bit 31) */
typedef unsigned short bsp_flagged_index;
#define BSP_FLAGGED_INDEX(value) \
	((unsigned short)(value) == 0xFFFF ? (long)NONE : \
	((unsigned short)(value) & 0x8000) ? (long)(LONG_MIN | (long)((unsigned short)(value) & 0x7FFF)) : \
	(long)(unsigned short)(value))
#else
typedef long bsp_flagged_index;
#define BSP_FLAGGED_INDEX(value) ((long)(value))
#endif

/* ---------- structures */

struct bsp3d
{
	struct tag_block nodes;
	struct tag_block planes;
};

#ifdef HALO_N64_CAMPAIGN
struct bsp3d_node
{
	short plane_designator;
	bsp_flagged_index children[2];
};

typedef char bsp3d_node_size_check[
	sizeof(struct bsp3d_node) == 0x6 ? 1 : -1];
#else
struct bsp3d_node
{
	long plane_designator;
	long children[2];
};

typedef char bsp3d_node_size_check[
	sizeof(struct bsp3d_node) == 0xC ? 1 : -1];
#endif

/* ---------- prototypes/BSP3D.C */

long bsp3d_test_point(struct bsp3d const *bsp, long node_index, union real_point3d const *point);

/* ---------- globals */

/* ---------- public code */

__inline real_plane3d *bsp3d_get_plane_from_designator(
	struct bsp3d const *bsp,
	long plane_designator,
	real_plane3d *result)
{
	real_plane3d* plane = TAG_BLOCK_GET_ELEMENT(&bsp->planes, plane_designator & LONG_MAX, real_plane3d);

	if (plane_designator & LONG_MIN)
	{
		plane3d_negate(plane, result);
	}
	else
	{
		*result = *plane;
	}

	return result;
}

#endif // __BSP3D_H
