/*
RENDER_OBJECTS.H
*/

#ifndef __RENDER_OBJECTS_H
#define __RENDER_OBJECTS_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct render_lighting;

/* ---------- prototypes/RENDER_OBJECTS.C */

void render_objects_initialize(
	void);
void render_objects_initialize_for_new_map(
	void);
void render_objects_dispose_from_old_map(
	void);
void render_objects_dispose(
	void);
struct render_lighting *object_get_cached_render_lighting(
	long object_index,
	real level_of_detail_pixels);
void render_objects(
	void);
void render_object_shadows(
	void);

#endif // __RENDER_OBJECTS_H
