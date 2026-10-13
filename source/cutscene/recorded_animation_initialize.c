/*
RECORDED_ANIMATION_INITIALIZE.C

symbols in this file:
00081D80 0070:
	_recorded_animation_byteswap_unit_control (0000)
00081DF0 0090:
	_recorded_animation_initialize_unit_control (0000)
00081E80 0070:
	_recorded_animation_write_unit_control (0000)
00258B44 000e:
	??_C@_0O@FLAIBBAO@real_vector3d?$AA@ (0000)
00258B54 000e:
	??_C@_0O@FKMKHLDJ@real_vector2d?$AA@ (0000)
002DCE10 0110:
	_data_002dce10 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "memory/byte_swapping.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct recorded_unit_control
{
	byte byte_field0;
	byte byte_field1;
	short word_field2;
	short word_field4;
	short version2_field;
	short version3_field;
	short unused_field10;
	real_vector2d vector2d_field12;
	long long_field20;
	long version1_field;
	real_vector3d vector3d_field28;
	real_vector3d vector3d_field40;
	real_vector3d vector3d_field52;
};

struct recorded_animation_control_field
{
	struct byte_swap_definition *definition;
	long size;
	long unit_control_offset;
};

struct recorded_animation_layout_data
{
	byte_swap_code real_vector2d_codes[2];
	struct byte_swap_definition real_vector2d_definition;
	byte_swap_code real_vector3d_codes[3];
	struct byte_swap_definition real_vector3d_definition;
	long version_table_alignment;
	struct recorded_animation_control_field version0_fields[10];
	struct recorded_animation_control_field version1_fields[2];
	struct recorded_animation_control_field version2_fields[2];
	struct recorded_animation_control_field version3_fields[2];
	struct recorded_animation_control_field *fields_by_version[4];
};

/* ---------- prototypes */

extern struct byte_swap_definition byte_bs_definition;
extern struct byte_swap_definition word_bs_definition;
extern struct byte_swap_definition long_bs_definition;

/* ---------- globals */

struct recorded_animation_layout_data real_vector2d_bs_codes =
{
	{ _4byte, _4byte },
	{ "real_vector2d", sizeof(real_vector2d), real_vector2d_bs_codes.real_vector2d_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE },
	{ _4byte, _4byte, _4byte },
	{ "real_vector3d", sizeof(real_vector3d), real_vector2d_bs_codes.real_vector3d_codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE },
	0, /* the original independently declared version table begins on an 8-byte data boundary */
	{
		{ &byte_bs_definition, sizeof(byte), offsetof(struct recorded_unit_control, byte_field0) },
		{ &byte_bs_definition, sizeof(byte), offsetof(struct recorded_unit_control, byte_field1) },
		{ &word_bs_definition, sizeof(short), offsetof(struct recorded_unit_control, word_field2) },
		{ &word_bs_definition, sizeof(short), offsetof(struct recorded_unit_control, word_field4) },
		{ &word_bs_definition, sizeof(short), NONE },
		{ &real_vector2d_bs_codes.real_vector2d_definition, sizeof(real_vector2d), offsetof(struct recorded_unit_control, vector2d_field12) },
		{ &real_vector2d_bs_codes.real_vector3d_definition, sizeof(real_vector3d), offsetof(struct recorded_unit_control, vector3d_field28) },
		{ &real_vector2d_bs_codes.real_vector3d_definition, sizeof(real_vector3d), offsetof(struct recorded_unit_control, vector3d_field40) },
		{ &real_vector2d_bs_codes.real_vector3d_definition, sizeof(real_vector3d), offsetof(struct recorded_unit_control, vector3d_field52) },
		{ NULL, NONE, NONE },
	},
	{
		{ &long_bs_definition, sizeof(long), offsetof(struct recorded_unit_control, version1_field) },
		{ NULL, NONE, NONE },
	},
	{
		{ &word_bs_definition, sizeof(short), offsetof(struct recorded_unit_control, version2_field) },
		{ NULL, NONE, NONE },
	},
	{
		{ &word_bs_definition, sizeof(short), offsetof(struct recorded_unit_control, version3_field) },
		{ NULL, NONE, NONE },
	},
	{
		real_vector2d_bs_codes.version0_fields,
		real_vector2d_bs_codes.version1_fields,
		real_vector2d_bs_codes.version2_fields,
		real_vector2d_bs_codes.version3_fields,
	},
};

/* ---------- public code */

void recorded_animation_byteswap_unit_control(byte **stream, byte unit_control_data_version)
{
	short version_index;

	for (version_index = 0; version_index < MAX(unit_control_data_version, 1); version_index++)
	{
		struct recorded_animation_control_field *field = real_vector2d_bs_codes.fields_by_version[version_index];

		while (field->size != NONE)
		{
			byte_swap_data(field->definition, *stream, 1);
			*stream += field->size;
			field++;
		}
	}
}

void recorded_animation_initialize_unit_control(
	struct recorded_unit_control *unit_control,
	byte **stream,
	byte unit_control_data_version)
{
	short version_index;

	csmemset(unit_control, 0, sizeof(*unit_control));
	unit_control->version3_field = NONE;

	for (version_index = 0; version_index < MAX(unit_control_data_version, 1); version_index++)
	{
		struct recorded_animation_control_field *field = real_vector2d_bs_codes.fields_by_version[version_index];

		while (field->size != NONE)
		{
			if (field->unit_control_offset != NONE)
			{
				csmemcpy(
					(byte *)unit_control + field->unit_control_offset,
					*stream,
					field->size);
			}

			*stream += field->size;
			field++;
		}
	}
}

void recorded_animation_write_unit_control(
	struct recorded_unit_control const *unit_control,
	byte **stream,
	byte unit_control_data_version)
{
	short version_index;

	for (version_index = 0; version_index < MAX(unit_control_data_version, 1); version_index++)
	{
		struct recorded_animation_control_field *field = real_vector2d_bs_codes.fields_by_version[version_index];

		while (field->size != NONE)
		{
			csmemcpy(
				*stream,
				(byte const *)unit_control + field->unit_control_offset,
				field->size);
			*stream += field->size;
			field++;
		}
	}
}

/* ---------- private code */
