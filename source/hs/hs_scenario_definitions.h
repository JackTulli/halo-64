/*
HS_SCENARIO_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __HS_SCENARIO_DEFINITIONS_H
#define __HS_SCENARIO_DEFINITIONS_H
#pragma once

/* ---------- constants */

enum
{
	_hs_script_startup = 0,
	_hs_script_dormant,
	_hs_script_continuous,
	_hs_script_static,
	_hs_script_stub,
	NUMBER_OF_HS_SCRIPT_TYPES,
};

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- macros */

/* ---------- structures */

struct hs_script
{
	char name[32];
	short script_type;
	short return_type;
	long root_expression_index;
	byte unused[52];
};

struct hs_global
{
	char name[32];
	short type;
	short unused0;
	long unused1;
	long initialization_expression_index;
	byte unused2[48];
};

struct hs_reference
{
	byte unused[24];
	struct tag_reference reference;
};

struct hs_source_file
{
	char name[32];
	struct tag_data source;
};

struct hs_syntax_node
{
	short datum_header;
	union
	{
		short index;
		short constant_type;
		short function_index;
		short script_index;
	};
	short type;
	short flags;
	long next_node_index;
	union
	{
		long string_offset;
		long source_offset;
	};
	union
	{
		long data;
		boolean boolean_value;
		real real_value;
		short short_value;
	};
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __HS_SCENARIO_DEFINITIONS_H
