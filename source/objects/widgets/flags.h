/*
FLAGS.H

header included in hcex build.
*/

#ifndef __FLAGS_H
#define __FLAGS_H
#pragma once

/* ---------- headers */

#include "objects/widgets/widget_types.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/EXAMPLE.C */

void flags_initialize(
	void);
void flags_initialize_for_new_map(
	void);
void flags_dispose_from_old_map(
	void);
void flags_dispose(
	void);
long flag_new(
	long definition_index);
void flag_delete(
	long flag_index);
void flags_update(
	real delta);
void flag_render(
	long object_index,
	long flag_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

/* ---------- globals */

extern struct data_array *flag_data;

/* ---------- public code */

#endif // __FLAGS_H
