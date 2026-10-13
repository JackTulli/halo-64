/*
GLOW.H

header included in hcex build.
*/

#ifndef __GLOW_H
#define __GLOW_H
#pragma once

/* ---------- headers */

#include "objects/widgets/widget_types.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct glow_globals
{
	struct data_array *glow_data;
	struct data_array *glow_particle_data;
};

/* ---------- prototypes/EXAMPLE.C */

void glow_initialize(
	void);
void glow_initialize_for_new_map(
	void);
void glow_dispose_from_old_map(
	void);
void glow_dispose(
	void);
long glow_new(
	long definition_index);
void glow_delete(
	long glow_index);
void glow_submit(
	long object_index,
	long glow_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

/* ---------- globals */

extern struct glow_globals glow_globals;

/* ---------- public code */

#endif // __GLOW_H
