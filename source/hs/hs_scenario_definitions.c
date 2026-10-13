/*
HS_SCENARIO_DEFINITIONS.C

symbols in this file:
000BD310 00e0:
	_hs_syntax_data_byte_swap (0000)
0026F0D0 001a:
	??_C@_0BK@HCHIMFOO@hs_string_data_definition?$AA@ (0000)
0026F0EC 001a:
	??_C@_0BK@DBIJMIDJ@hs_syntax_data_definition?$AA@ (0000)
0026F108 000c:
	??_C@_0M@BMPNPGNE@syntax_node?$AA@ (0000)
0026F114 0012:
	??_C@_0BC@PBNDBBAC@data_array_header?$AA@ (0000)
0026F128 0007:
	??_C@_06OEKOLKIL@source?$AA@ (0000)
0026F130 0016:
	??_C@_0BG@FBLJNDKH@hs_source_files_block?$AA@ (0000)
0026F148 001a:
	??_C@_0BK@NCBIEJFG@hs_source_data_definition?$AA@ (0000)
0026F164 000c:
	??_C@_0M@GBFIIOJI@reference?$CK?$FO?$AA@ (0000)
0026F170 0014:
	??_C@_0BE@GFKIJMCD@hs_references_block?$AA@ (0000)
0026F184 0021:
	??_C@_0CB@CJODAJDG@initialization?5expression?5index?$CK@ (0000)
0026F1A8 0006:
	??_C@_05GAAEPGEK@type?$CK?$AA@ (0000)
0026F1B0 0011:
	??_C@_0BB@GFGOKKNE@hs_globals_block?$AA@ (0000)
0026F1C4 0017:
	??_C@_0BH@MDEEMKLE@root?5expression?5index?$CK?$AA@ (0000)
0026F1DC 000d:
	??_C@_0N@DOPJMMFH@return?5type?$CK?$AA@ (0000)
0026F1EC 000d:
	??_C@_0N@BKPIBOHC@script?5type?$CK?$AA@ (0000)
0026F1FC 0006:
	??_C@_05DAKKIELG@name?$CK?$AA@ (0000)
0026F204 0011:
	??_C@_0BB@FDOCKNHP@hs_scripts_block?$AA@ (0000)
0026F218 003d:
	??_C@_0DN@NFFJFPKM@data_size?$DO?$DN0?5?$CG?$CG?5?$CIdata_size?$CFsizeo@ (0000)
0026F258 000d:
	??_C@_0N@LFDKIDJD@csirtpn?5?$AAedo?$AA@ (0000)
0026F268 0020:
	??_C@_0CA@JPLDHLPF@size?$DO?$DNsizeof?$CIstruct?5data_array?$CJ?$AA@ (0000)
0026F288 002c:
	??_C@_0CM@EMEIFNAD@c?3?2halo?2SOURCE?2hs?2hs_scenario_de@ (0000)
002E49C0 0274:
	_hs_script_types_enum (0000)
	_hs_types_enum (000c)
	_hs_scripts_block (0060)
	_hs_globals_block (00e4)
	_hs_references_block (0140)
	_hs_source_data_definition (016c)
	_hs_source_files_block (01a0)
	_hs_syntax_data_definition (0254)
	_hs_string_data_definition (0264)
*/

/* ---------- headers */

#include "cseries.h"
#include "hs/hs_scenario_definitions.h"
#include "memory/byte_swapping.h"
#include "memory/data.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

extern char *hs_script_type_names[];
extern char *hs_type_names[];
static void hs_syntax_data_byte_swap(
	void *owner,
	void *data,
	long size);

static struct tag_field hs_script_fields[6];
static struct tag_field hs_global_fields[7];
static unsigned long hs_reference_group_tags[3];
static struct tag_field hs_reference_fields[3];
static struct tag_field hs_source_fields[3];
static byte_swap_code hs_data_array_codes[14];
static struct byte_swap_definition hs_data_array_definition;
static byte_swap_code hs_syntax_node_codes[10];
static struct byte_swap_definition hs_syntax_node_definition;

/* ---------- globals */

struct tag_enum_definition hs_script_types_enum =
{
	5,
	hs_script_type_names,
	NULL,
};

struct tag_enum_definition hs_types_enum =
{
	49,
	hs_type_names,
	NULL,
};

static struct tag_field hs_script_fields[6] =
{
	{ _tag_field_string, 0, "name*", NULL },
	{ _tag_field_enum, 0, "script type*", &hs_script_types_enum },
	{ _tag_field_enum, 0, "return type*", &hs_types_enum },
	{ _tag_field_long_integer, 0, "root expression index*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)52 },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition hs_scripts_block =
{
	"hs_scripts_block",
	0,
	512,
	sizeof(struct hs_script),
	NULL,
	hs_script_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field hs_global_fields[7] =
{
	{ _tag_field_string, 0, "name*", NULL },
	{ _tag_field_enum, 0, "type*", &hs_types_enum },
	{ _tag_field_pad, 0, NULL, (void *)2 },
	{ _tag_field_pad, 0, NULL, (void *)4 },
	{ _tag_field_long_integer, 0, "initialization expression index*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)48 },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition hs_globals_block =
{
	"hs_globals_block",
	0,
	128,
	sizeof(struct hs_global),
	NULL,
	hs_global_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static unsigned long hs_reference_group_tags[3] = { 0, NONE, 0 };

static struct tag_field hs_reference_fields[3] =
{
	{ _tag_field_pad, 0, NULL, (void *)24 },
	{ _tag_field_tag_reference, 0, "reference*^", hs_reference_group_tags },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition hs_references_block =
{
	"hs_references_block",
	0,
	256,
	sizeof(struct hs_reference),
	NULL,
	hs_reference_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct tag_data_definition hs_source_data_definition =
{
	"hs_source_data_definition",
	0,
	0x40000,
	NULL,
};

static struct tag_field hs_source_fields[3] =
{
	{ _tag_field_string, 0, "name*", NULL },
	{ _tag_field_data, 0, "source", &hs_source_data_definition },
	{ _tag_field_terminator, 0, NULL, NULL },
};

struct tag_block_definition hs_source_files_block =
{
	"hs_source_files_block",
	0,
	8,
	sizeof(struct hs_source_file),
	NULL,
	hs_source_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static byte_swap_code hs_data_array_codes[14] =
{
	_begin_bs_array,
	1,
	32,
	_2byte,
	_2byte,
	1,
	3,
	_4byte,
	_2byte,
	_2byte,
	_2byte,
	_2byte,
	_4byte,
	_end_bs_array,
};

static struct byte_swap_definition hs_data_array_definition =
{
	"data_array_header",
	sizeof(struct data_array),
	hs_data_array_codes,
	BYTE_SWAP_DEFINITION_SIGNATURE,
	FALSE,
};

static byte_swap_code hs_syntax_node_codes[10] =
{
	_begin_bs_array,
	1,
	_2byte,
	_2byte,
	_2byte,
	_2byte,
	_4byte,
	_4byte,
	_4byte,
	_end_bs_array,
};

static struct byte_swap_definition hs_syntax_node_definition =
{
	"syntax_node",
	sizeof(struct hs_syntax_node),
	hs_syntax_node_codes,
	BYTE_SWAP_DEFINITION_SIGNATURE,
	FALSE,
};

struct tag_data_definition hs_syntax_data_definition =
{
	"hs_syntax_data_definition",
	0,
	380076,
	hs_syntax_data_byte_swap,
};

struct tag_data_definition hs_string_data_definition =
{
	"hs_string_data_definition",
	0,
	0x40000,
	NULL,
};

/* ---------- public code */

static void hs_syntax_data_byte_swap(
	void *owner,
	void *data,
	long size)
{
	long data_size;

	if (size)
	{
		match_assert("c:\\halo\\SOURCE\\hs\\hs_scenario_definitions.c", 109,
			size>=sizeof(struct data_array));

		if (!memcmp(data, "csirtpn \0edo", 12))
		{
			byte_swap_data(&hs_data_array_definition, data, 1);
			byte_swap_data(&hs_syntax_node_definition, data, 0x4000);
			return;
		}

		data_size= size-sizeof(struct data_array);
		match_assert("c:\\halo\\SOURCE\\hs\\hs_scenario_definitions.c", 121,
			data_size>=0 && (data_size%sizeof(struct hs_syntax_node))==0);

		if (data_size>=0 && (data_size%sizeof(struct hs_syntax_node))==0)
		{
			long syntax_node_count= data_size/sizeof(struct hs_syntax_node);

			byte_swap_data(&hs_data_array_definition, data, 1);
			data= (byte *)data+sizeof(struct data_array);
			byte_swap_data(&hs_syntax_node_definition, data, syntax_node_count);
		}
	}

	return;
}
