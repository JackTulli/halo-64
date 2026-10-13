/*
RASTERIZER_GEOMETRY_ENVIRONMENT.H

Narrow cross-translation-unit environment-vertex interface owned by
RASTERIZER_GEOMETRY.C.
*/

#ifndef __RASTERIZER_GEOMETRY_ENVIRONMENT_H
#define __RASTERIZER_GEOMETRY_ENVIRONMENT_H
#pragma once

/* ---------- structures */

union real_point2d;
union real_point3d;
union real_vector3d;
struct environment_lightmap_vertex_compressed;
struct environment_vertex_compressed;

/* ---------- prototypes/RASTERIZER_GEOMETRY.C */

void environment_vertex_compressed_get_point(
	struct environment_vertex_compressed const *vertex,
	union real_point3d *point);
void environment_vertex_compressed_get_normal(
	struct environment_vertex_compressed const *vertex,
	union real_vector3d *normal);
void environment_vertex_compressed_get_texcoord(
	struct environment_vertex_compressed const *vertex,
	union real_point2d *texcoord);
void environment_lightmap_vertex_compressed_get_incident_radiosity(
	struct environment_lightmap_vertex_compressed const *vertex,
	union real_vector3d *incident_radiosity);
void environment_lightmap_vertex_compressed_get_texcoord(
	struct environment_lightmap_vertex_compressed const *vertex,
	union real_point2d *texcoord);

#endif /* __RASTERIZER_GEOMETRY_ENVIRONMENT_H */
