/*
DATA_PACKET_GROUPS.H
*/

#ifndef __DATA_PACKET_GROUPS_H
#define __DATA_PACKET_GROUPS_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

enum data_packet_field_type
{
	_data_packet_field_pad = 0,
	_data_packet_field_bytes,
	_data_packet_field_shorts,
	_data_packet_field_longs,
	_data_packet_field_int64s,
	_data_packet_field_string,
	_data_packet_field_data,
	_data_packet_field_array,
	_data_packet_field_raw,
	_data_packet_field_end,
	_data_packet_field_type_count
};

struct data_packet_field
{
	short type;
	short count;
	short minimum_version;
	short maximum_version;
	short size;
};

struct data_packet_definition
{
	char const *name;
	long flags;
	short size;
	short version;
	struct data_packet_field *fields;
	boolean initialized;
};

struct data_packet_entry
{
	short packet_class;
	short flags;
	struct data_packet_definition *definition;
};

struct data_packet_group_definition
{
	char const *name;
	short packet_type_count;
	short packet_class_count;
	long maximum_decoded_packet_size;
	long maximum_encoded_packet_size;
	struct data_packet_entry *packets;
};

/* ---------- prototypes/DATA_PACKET_GROUPS.C */

void data_packet_group_initialize(
	struct data_packet_group_definition *group_definition);

char const *data_packet_groups_get_error(
	void);

boolean data_packet_group_encode_packet(
	struct data_packet_group_definition *group_definition,
	const void *decoded_packet,
	void *encoded_packet,
	short *encoded_packet_size,
	short packet_type,
	long packet_version);

boolean data_packet_group_decode_packet(
	struct data_packet_group_definition *group_definition,
	void *decoded_packet,
	const void *encoded_packet,
	short *encoded_packet_size,
	short *packet_type,
	short *packet_version,
	short expected_packet_class);

#endif // __DATA_PACKET_GROUPS_H
