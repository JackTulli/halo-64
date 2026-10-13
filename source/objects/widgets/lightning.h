/*
LIGHTNING.H

header included in hcex build.
*/

#ifndef __LIGHTNING_H
#define __LIGHTNING_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "memory/data.h"
#include "objects/widgets/widget_types.h"

/* ---------- constants */

/* ---------- macros */

#define lightning_get(lightning_index) \
	((struct lightning_datum *)datum_get(lightning_globals.lightning_data, (lightning_index)))

/* ---------- structures */

struct lightning_globals
{
	struct data_array *lightning_data;
};

struct lightning_datum
{
	struct datum_header header;
	short __unknown2;
	long definition_index;
};

typedef char lightning_globals_size_assert[
	sizeof(struct lightning_globals) == 0x4 ? 1 : -1];
typedef char lightning_datum_size_assert[
	sizeof(struct lightning_datum) == 0x8 ? 1 : -1];

/* ---------- prototypes/LIGHTNING.C */

void lightnings_initialize(
	void);

void lightnings_dispose(
	void);
void lightnings_initialize_for_new_map(
	void);
void lightnings_dispose_from_old_map(
	void);

long lightning_new(
	long definition_index);

void lightning_delete(
	long lightning_index);

void lightning_submit(
	long object_index,
	long lightning_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation);

void lightning_render(
	void);

/* ---------- public code */

#endif // __LIGHTNING_H
