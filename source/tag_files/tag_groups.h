/*
TAG_GROUPS.H

header included in hcex build.
*/

#ifndef __TAG_GROUPS_H
#define __TAG_GROUPS_H
#pragma once

/* ---------- headers */

#include "memory/byte_swapping.h"

/* ---------- constants */

enum tag_field_type
{
	_tag_field_string = 0,
	_tag_field_char_integer = 1,
	_tag_field_short_integer = 2,
	_tag_field_long_integer = 3,
	_tag_field_enum = 6,
	_tag_field_word_flags = 8,
	_tag_field_byte_flags = 9,
	_tag_field_real_point3d = 17,
	_tag_field_real_plane2d = 23,
	_tag_field_real_plane3d = 24,
	_tag_field_tag_reference = 33,
	_tag_field_block = 34,
	_tag_field_data = 37,
	_tag_field_pad = 40,
	_tag_field_terminator = 44,
};

/* ---------- macros */

#if defined(HALO_N64) && defined(HALO_RELEASE)
/* (what tag_block_get_element_with_size is with its assertions compiled
out, without the call: the game reaches for block elements constantly) */
#define TAG_BLOCK_GET_ELEMENT(block_address, index, type) \
	((type *)((byte *)(block_address)->address + (long)(index) * (long)sizeof(type)))
#else
#define TAG_BLOCK_GET_ELEMENT(block_address, index, type) ((type *)tag_block_get_element_with_size((block_address), (index), sizeof(type)))
#endif

/* ---------- structures */

typedef void (*byte_swap_block_proc)(void *);
typedef boolean (*postprocess_block_proc)(void *, boolean);
typedef byte *(*format_block_proc)(long, struct tag_block *, long, byte *);
typedef void (*delete_block_proc)(struct tag_block *, long);
typedef void (*byte_swap_data_proc)(void *, void *, long);

struct tag_enum_definition
{
	long count;
	char **names;
	void *unused;
};

struct tag_field
{
	short type;
	word pad;
	char *name;
	void *definition;
};

struct tag_data_definition
{
	char *name;
	unsigned long flags;
	long maximum_size;
	byte_swap_data_proc byte_swap_data;
};

struct tag_block_definition
{
	char *name;
	unsigned long flags;
	long maximum_element_count;
	long element_size;
	void *default_element;
	struct tag_field *fields;
	byte_swap_block_proc byte_swap_block;
	postprocess_block_proc postprocess_block;
	format_block_proc format_block;
	delete_block_proc delete_block;
	byte_swap_code *byte_swap_codes;
};

struct tag_block
{
	long count;
	void *address;
	struct tag_block_definition *definition;
};

struct tag_reference
{
	unsigned long group_tag;
	char *name;
	long name_length;
	long index;
};

struct tag_reference_definition
{
	unsigned long flags;
	unsigned long group_tag;
	unsigned long *group_tags;
};

typedef char tag_reference_definition_size_assert[
	sizeof(struct tag_reference_definition) == 0xC ? 1 : -1];

struct tag_data
{
	long size;
	unsigned long pad;
	long file_offset;
	void *address;
	struct tag_data_definition *definition;
};

/* ---------- prototypes/TAG_GROUPS.C */

long verify_tag_reference(struct tag_reference const *reference);
void *tag_data_get_pointer(struct tag_data const *data, long offset, long size);
void *tag_block_get_element_with_size(struct tag_block const *block, long index, long element_size);

/* ---------- prototypes/CACHE_FILES.C */

long tag_loaded(long group_tag, const char *name);

void *tag_get(long group_tag, long tag_index);

#if defined(HALO_LINUX) || defined(HALO_N64)
boolean tag_index_is_group(long tag_index, long group_tag);
#endif

/* ---------- globals */

/* ---------- public code */

#endif // __TAG_GROUPS_H
