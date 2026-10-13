/*
DIALOGUE_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __DIALOGUE_DEFINITIONS_H
#define __DIALOGUE_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

enum
{
	DIALOGUE_DEFINITION_TAG = 'udlg',
	NUMBER_OF_DIALOGUE_VOCALIZATION_TYPES = 209,
};

/* ---------- macros */

#define dialogue_definition_get(index) \
	((struct dialogue_definition *)tag_get(DIALOGUE_DEFINITION_TAG, (index)))

/* ---------- structures */

struct dialogue_definition;

/* ---------- prototypes/DIALOGUE_DEFINITIONS.C */

char const *dialogue_get_vocalization_name(
	short vocalization_type,
	boolean abbreviated);
short dialogue_get_vocalization_type_by_name(
	char const *name);

/* ---------- prototypes/UNIT_DIALOGUE.C */

void unit_dialogue_determine_variant(
	long unit_index);

/* ---------- globals */

/* ---------- public code */

#endif // __DIALOGUE_DEFINITIONS_H
