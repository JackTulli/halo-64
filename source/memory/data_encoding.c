/*
DATA_ENCODING.C

symbols in this file:
00109470 0070:
	_data_encode_new (0000)
001094E0 0130:
	_data_encode_memory (0000)
00109610 0100:
	_data_encode_integer (0000)
00109710 0100:
	_data_encode_structures (0000)
00109810 0240:
	_data_encode_array (0000)
00109A50 00a0:
	_data_encode_string (0000)
00109AF0 0070:
	_data_decode_new (0000)
00109B60 00f0:
	_data_decode_structures (0000)
00109C50 0130:
	_data_decode_memory (0000)
00109D80 0070:
	_data_decode_byte (0000)
00109DF0 0080:
	_data_decode_short (0000)
00109E70 0080:
	_data_decode_long (0000)
00109EF0 0030:
	_data_decode_int64 (0000)
00109F20 0070:
	_data_decode_integer (0000)
00109F90 0170:
	_data_decode_array (0000)
0010A100 0050:
	_data_decode_string (0000)
0027D428 0026:
	??_C@_0CG@DKDENLGI@c?3?2halo?2SOURCE?2memory?2data_encod@ (0000)
0027D450 004f:
	??_C@_0EP@IBADKJMK@state?5?$CG?$CG?5state?9?$DObuffer?5?$CG?$CG?5state?9@ (0000)
0027D4A0 0010:
	??_C@_0BA@MMBBIMAK@maximum_value?$DO0?$AA@ (0000)
0027D4B0 000e:
	??_C@_0O@DLMDCMCD@bs_definition?$AA@ (0000)
0027D4C0 0012:
	??_C@_0BC@JKGHEGPN@source_structures?$AA@ (0000)
0027D4D4 0022:
	??_C@_0CC@ENPGOLI@element_count?$DM?$DNUNSIGNED_SHORT_MA@ (0000)
0027D4F8 0021:
	??_C@_0CB@ICNNLDBF@element_count?$DM?$DNUNSIGNED_CHAR_MAX@ (0000)
0027D51C 0011:
	??_C@_0BB@LOCFNCAH@element_count?$DO?$DN0?$AA@ (0000)
0027D530 000d:
	??_C@_0N@MOKGBPFF@source_array?$AA@ (0000)
0027D540 0032:
	??_C@_0DC@POGPGOIK@state?9?$DOoffset?$CLstring_length?$CL1?$DM?$DNs@ (0000)
0027D574 0013:
	??_C@_0BD@IAHGFBAA@structure_count?$DO?$DN0?$AA@ (0000)
0027D588 0050:
	??_C@_0FA@EJBJLMFJ@state?5?$CG?$CG?5state?9?$DObuffer?5?$CG?$CG?5state?9@ (0000)
0027D5D8 0018:
	??_C@_0BI@EADHMJMM@maximum_element_count?$DO0?$AA@ (0000)
0027D5F0 0018:
	??_C@_0BI@PJCDPPLJ@element_count_reference?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "memory/byte_swapping.h"
#include "memory/data_encoding.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void data_encode_new(
	struct data_encoding_state *state,
	void *buffer,
	long buffer_size)
{
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 25, buffer);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 26, buffer_size>=0);

	csmemset(state, 0, sizeof(*state));
	state->buffer = buffer;
	state->buffer_size = buffer_size;

	return;
}

boolean data_encode_memory(
	struct data_encoding_state *state,
	void const *source,
	short element_count,
	long element_size)
{
	long memory_size;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		43,
		state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);

	switch (element_size)
	{
	case 1:
		memory_size = element_count;
		break;
	case -2:
		memory_size = element_count<<1;
		break;
	case -4:
		memory_size = element_count<<2;
		break;
	case -8:
		memory_size = element_count<<3;
		break;
	default:
		/* BUG (original): if system_exit returns, memory_size remains
		 * uninitialized. All known callers use a valid element size.
		 */
		display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_encoding.c", 51, TRUE);
		system_exit(-1);
		break;
	}

	if (state->offset+memory_size<=state->buffer_size && !state->overflow)
	{
		void *destination;

		destination = state->buffer+state->offset;
		if (source)
			csmemcpy(destination, source, memory_size);
		else
			csmemset(destination, 0, memory_size);

		if (element_size != 1)
			byte_swap_memory(destination, element_count, element_size);
		state->offset += memory_size;
	}
	else
	{
		state->overflow = TRUE;
	}

	return !state->overflow;
}

boolean data_encode_integer(
	struct data_encoding_state *state,
	long value,
	long maximum_value)
{
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 84, maximum_value>0);

	if (maximum_value <= UNSIGNED_CHAR_MAX)
	{
		byte byte_value;

		byte_value = (byte)value;
		match_assert(
			"c:\\halo\\SOURCE\\memory\\data_encoding.c",
			43,
			state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);
		if (state->offset + 1 <= state->buffer_size && !state->overflow)
		{
			csmemcpy(state->buffer + state->offset, &byte_value, 1);
			state->offset++;
		}
		else
		{
			state->overflow = TRUE;
		}
	}
	else if (maximum_value <= UNSIGNED_SHORT_MAX)
	{
		short short_value;

		short_value = (short)value;
		data_encode_memory(state, &short_value, 1, -sizeof(short_value));
	}
	else
	{
		long long_value;

		long_value = value;
		data_encode_memory(state, &long_value, 1, -sizeof(long_value));
	}

	return !state->overflow;
}

boolean data_encode_structures(
	struct data_encoding_state *state,
	void const *source_structures,
	short structure_count,
	struct byte_swap_definition *bs_definition)
{
	short memory_size;
	void *destination;

	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 110, state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 111, source_structures);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 112, bs_definition);

	memory_size = (short)(bs_definition->size * structure_count);
	if (memory_size > 0)
	{
		if (state->offset + memory_size <= state->buffer_size && !state->overflow)
		{
			destination = state->buffer + state->offset;
			csmemcpy(destination, source_structures, memory_size);
			byte_swap_data(bs_definition, destination, structure_count);
			state->offset += memory_size;
		}
		else
		{
			state->overflow = TRUE;
		}
	}

	return !state->overflow;
}

boolean data_encode_array(
	struct data_encoding_state *state,
	long element_size,
	void const *source_array,
	long element_count,
	struct byte_swap_definition *bs_definition)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		141,
		state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 142, source_array);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 143, bs_definition);
	match_vassert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		144,
		element_count>=0,
		"element_count>=0");

	switch (element_size)
	{
	case 1:
	{
		byte byte_count;

		match_vassert(
			"c:\\halo\\SOURCE\\memory\\data_encoding.c",
			150,
			element_count<=UNSIGNED_CHAR_MAX,
			"element_count<=UNSIGNED_CHAR_MAX");
		byte_count = (byte)element_count;
		match_assert(
			"c:\\halo\\SOURCE\\memory\\data_encoding.c",
			43,
			state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);
		if (state->offset+1<=state->buffer_size && !state->overflow)
		{
			csmemcpy(state->buffer+state->offset, &byte_count, 1);
			state->offset++;
		}
		else
		{
			state->overflow = TRUE;
		}
		break;
	}
	case -2:
	{
		short short_count;

		match_vassert(
			"c:\\halo\\SOURCE\\memory\\data_encoding.c",
			154,
			element_count<=UNSIGNED_SHORT_MAX,
			"element_count<=UNSIGNED_SHORT_MAX");
		short_count = (short)element_count;
		data_encode_memory(state, &short_count, 1, -sizeof(short_count));
		break;
	}
	case -4:
	{
		long long_count;

		long_count = element_count;
		data_encode_memory(state, &long_count, 1, -sizeof(long_count));
		break;
	}
	case -8:
	{
		__int64 int64_count = element_count;

		data_encode_memory(state, &int64_count, 1, -sizeof(int64_count));
		break;
	}
	default:
		display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_encoding.c", 165, TRUE);
		system_exit(-1);
		break;
	}

	data_encode_structures(
		state,
		source_array,
		(short)element_count,
		bs_definition);

	return !state->overflow;
}

boolean data_encode_string(
	struct data_encoding_state *state,
	char const *string,
	short maximum_length)
{
	short string_length = (short)strnlen(string, maximum_length);
	char *destination = state->buffer + state->offset;

	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 182, state->offset+string_length+1<=state->buffer_size);

	if (state->offset + string_length + 1 <= state->buffer_size && !state->overflow)
	{
		csstrncpy(destination, string, string_length);
		destination[string_length] = 0;
		state->offset += string_length + 1;
	}
	else
	{
		state->overflow = TRUE;
	}

	return !state->overflow;
}

void data_decode_new(
	struct data_encoding_state *state,
	void const *buffer,
	long buffer_size)
{
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 204, buffer);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 205, buffer_size>=0);

	csmemset(state, 0, sizeof(*state));
	state->buffer = (byte *)buffer;
	state->buffer_size = buffer_size;

	return;
}

void *data_decode_structures(
	struct data_encoding_state *state,
	short structure_count,
	struct byte_swap_definition *bs_definition)
{
	short memory_size;
	void *structures = NULL;

	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 222, state && state->buffer && state->offset>=0 && state->offset<=state->buffer_size);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 223, structure_count>=0);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 224, bs_definition);

	memory_size = (short)(bs_definition->size * structure_count);
	if (state->offset + memory_size <= state->buffer_size && !state->overflow)
	{
		structures = state->buffer + state->offset;
		if (memory_size)
		{
			byte_swap_data(bs_definition, structures, structure_count);
			state->offset += memory_size;
		}
	}
	else
	{
		state->overflow = TRUE;
	}

	return structures;
}

void *data_decode_memory(
	struct data_encoding_state *state,
	short count,
	long element_size)
{
	void *memory = NULL;
	long memory_size;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		256,
		state && state->buffer && state->offset>=0 && state->offset<=state->buffer_size);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 257, count>=0);

	switch (element_size)
	{
	case 1:
		memory_size = count;
		break;
	case -2:
		memory_size = count<<1;
		break;
	case -4:
		memory_size = count<<2;
		break;
	case -8:
		memory_size = count<<3;
		break;
	default:
		/* BUG (original): if system_exit returns, memory_size remains
		 * uninitialized. A corrected build should assign memory_size = count
		 * before leaving this case.
		 */
		display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_encoding.c", 265, TRUE);
		system_exit(-1);
		break;
	}

	if (state->offset+memory_size<=state->buffer_size && !state->overflow)
	{
		memory = state->buffer+state->offset;
		if (element_size!=1)
		{
			byte_swap_memory(memory, count, element_size);
		}
		state->offset += memory_size;
	}
	else
	{
		state->overflow = TRUE;
	}

	return memory;
}

byte data_decode_byte(
	struct data_encoding_state *state)
{
	byte *value;

	return (value = data_decode_memory(state, 1, sizeof(*value))) ? *value : 0;
}

short data_decode_short(
	struct data_encoding_state *state)
{
	short *value;

	return (value = data_decode_memory(state, 1, -sizeof(*value))) ? *value : 0;
}

long data_decode_long(
	struct data_encoding_state *state)
{
	long *value;

	return (value = data_decode_memory(state, 1, -sizeof(*value))) ? *value : 0;
}

__int64 data_decode_int64(
	struct data_encoding_state *state)
{
	__int64 *value;

	return (value = data_decode_memory(state, 1, -sizeof(*value))) ? *value : 0;
}

long data_decode_integer(
	struct data_encoding_state *state,
	long maximum_value)
{
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 321, maximum_value>0);

	if (maximum_value <= UNSIGNED_CHAR_MAX)
		return data_decode_byte(state);
	if (maximum_value <= UNSIGNED_SHORT_MAX)
		return data_decode_short(state);

	return data_decode_long(state);
}

void *data_decode_array(
	struct data_encoding_state *state,
	long element_size,
	long *element_count_reference,
	long maximum_element_count,
	struct byte_swap_definition *bs_definition)
{
	long element_count;
	void *array = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		348,
		state && state->buffer && state->offset>=0 && state->offset<state->buffer_size);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		349,
		element_count_reference);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_encoding.c",
		350,
		maximum_element_count>0);
	match_assert("c:\\halo\\SOURCE\\memory\\data_encoding.c", 351, bs_definition);

	switch (element_size)
	{
	case 1:
		element_count = data_decode_byte(state);
		break;
	case -2:
		element_count = data_decode_short(state);
		break;
	case -4:
		element_count = data_decode_long(state);
		break;
	case -8:
		element_count = (long)data_decode_int64(state);
		break;
	/* element_count is left unassigned only by this default arm. Not reached unassigned: the
	 * arm's assertion failure calls system_exit, which does not return in January
	 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
	 * Source-policy approval pending (2026-09-27 audit). */
	default:
		display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_encoding.c", 370, TRUE);
		system_exit(-1);
		break;
	}

	if (!state->overflow && element_count >= 0 && element_count <= maximum_element_count)
	{
		*element_count_reference = element_count;
		array = data_decode_structures(
			state,
			(short)element_count,
			bs_definition);
	}

	return array;
}

char *data_decode_string(
	struct data_encoding_state *state,
	word maximum_length)
{
	char *string = state->buffer + state->offset;
	short string_length = 0;

	while (state->offset + string_length < state->buffer_size)
	{
		if (!string[string_length])
		{
			state->offset += string_length + 1;
			return string;
		}
		string_length++;
	}

	state->overflow = TRUE;
	return NULL;
}

/* ---------- private code */
