/*
DATA_ENCODING.H

header included in hcex build.
*/

#ifndef __DATA_ENCODING_H
#define __DATA_ENCODING_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct byte_swap_definition;

struct data_encoding_state
{
	byte *buffer;
	long offset;
	long buffer_size;
	boolean overflow;
	byte pad[3];
};

/* ---------- prototypes/DATA_ENCODING.C */

void data_encode_new(
	struct data_encoding_state *state,
	void *buffer,
	long buffer_size);

boolean data_encode_memory(
	struct data_encoding_state *state,
	void const *source,
	short element_count,
	long element_size);

boolean data_encode_integer(
	struct data_encoding_state *state,
	long value,
	long maximum_value);

boolean data_encode_structures(
	struct data_encoding_state *state,
	void const *source_structures,
	short structure_count,
	struct byte_swap_definition *bs_definition);

boolean data_encode_array(
	struct data_encoding_state *state,
	long element_size,
	void const *source_array,
	long element_count,
	struct byte_swap_definition *bs_definition);

boolean data_encode_string(
	struct data_encoding_state *state,
	char const *string,
	short maximum_length);

void data_decode_new(
	struct data_encoding_state *state,
	void const *buffer,
	long buffer_size);

void *data_decode_memory(
	struct data_encoding_state *state,
	short count,
	long element_size);

void *data_decode_structures(
	struct data_encoding_state *state,
	short structure_count,
	struct byte_swap_definition *bs_definition);

byte data_decode_byte(
	struct data_encoding_state *state);

short data_decode_short(
	struct data_encoding_state *state);

long data_decode_long(
	struct data_encoding_state *state);

__int64 data_decode_int64(
	struct data_encoding_state *state);

long data_decode_integer(
	struct data_encoding_state *state,
	long maximum_value);

void *data_decode_array(
	struct data_encoding_state *state,
	long element_size,
	long *element_count_reference,
	long maximum_element_count,
	struct byte_swap_definition *bs_definition);

char *data_decode_string(
	struct data_encoding_state *state,
	word maximum_length);

/* ---------- globals */

/* ---------- public code */

#endif // __DATA_ENCODING_H
