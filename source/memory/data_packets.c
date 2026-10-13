/*
DATA_PACKETS.C

symbols in this file:
0010A5F0 01d0:
	_code_0010a5f0 (0000)
0010A7C0 0300:
	_code_0010a7c0 (0000)
0010AAC0 02a0:
	_code_0010aac0 (0000)
0010AD60 0110:
	_data_packet_verify (0000)
0010AE70 0100:
	_data_packet_encode (0000)
0010AF70 0100:
	_data_packet_decode (0000)
0027D94C 0030:
	??_C@_0DA@CDEMHOHB@field?5has?5negative?5or?5zero?5count@ (0000)
0027D980 0047:
	??_C@_0EH@IJFFLBHG@unknown?5field?5type?5in?5packet?5?8?$CFs@ (0000)
0027D9C8 0025:
	??_C@_0CF@HEIDLPDO@c?3?2halo?2SOURCE?2memory?2data_packe@ (0000)
0027D9F0 0030:
	??_C@_0DA@LCBBKELK@element_count?$DO?$DN0?5?$CG?$CG?5element_coun@ (0000)
0027DA20 0028:
	??_C@_0CI@PGBBKFEI@data_size?$DO?$DN0?5?$CG?$CG?5data_size?$DM?$DNfield@ (0000)
0027DA48 0049:
	??_C@_0EJ@PJKAPNNJ@packet?5?8?$CFs?8?5fields?5added?5up?5to?5?$CD@ (0000)
0027DA94 0035:
	??_C@_0DF@PHJLJBNP@packet_definition?9?$DOname?5?$CG?$CG?5packe@ (0000)
0027DACC 001e:
	??_C@_0BO@OPEJACHI@packet_definition?9?$DOversion?$DO?$DN0?$AA@ (0000)
0027DAEC 001b:
	??_C@_0BL@FEHKLID@packet_definition?9?$DOsize?$DO?$DN0?$AA@ (0000)
0027DB08 0012:
	??_C@_0BC@CELFOEFM@packet_definition?$AA@ (0000)
0027DB1C 0017:
	??_C@_0BH@HPAHFLDG@maximum_buffer_size?$DO?$DN0?$AA@ (0000)
0027DB34 0016:
	??_C@_0BG@FEKHCMCH@buffer?5?$CG?$CG?5buffer_size?$AA@ (0000)
0027DB4C 0017:
	??_C@_0BH@LODNBOPH@encoded_packet_size?$DO?$DN0?$AA@ (0000)
0027DB64 000f:
	??_C@_0P@LFFMEHAO@encoded_packet?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "memory/data_encoding.h"
#include "memory/data_packet_groups.h"
#include "memory/data_packets.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void _data_packet_verify(
	struct data_packet_definition *packet_definition,
	short *packet_size,
	struct data_packet_field *fields,
	short *field_count);

void _data_packet_encode(
	struct data_packet_definition *packet_definition,
	struct data_encoding_state *state,
	short packet_version,
	void const *decoded_packet,
	short *encoded_packet_size,
	struct data_packet_field *fields,
	short *field_count);

void _data_packet_decode(
	struct data_packet_definition *packet_definition,
	struct data_encoding_state *state,
	short packet_version,
	void *decoded_packet,
	short *encoded_packet_size,
	struct data_packet_field *fields,
	short *field_count);

/* ---------- globals */

/* ---------- public code */

void data_packet_verify(
	struct data_packet_definition *packet_definition)
{
	short packet_size;
	short field_count;

	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 32, packet_definition);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 33, packet_definition->size>=0);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 34, packet_definition->version>=0);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 35, packet_definition->name && packet_definition->fields);

	if (!packet_definition->initialized)
	{
		_data_packet_verify(packet_definition, &packet_size, packet_definition->fields, &field_count);
		if (packet_size != packet_definition->size)
		{
			display_assert(
				csprintf(
					temporary,
					"packet '%s' fields added up to #%d bytes but should have been #%d bytes.",
					packet_definition->name,
					packet_size,
					packet_definition->size),
				"c:\\halo\\SOURCE\\memory\\data_packets.c",
				43,
				TRUE);
			system_exit(-1);
		}
		packet_definition->initialized = TRUE;
	}

	return;
}

boolean data_packet_encode(
	struct data_packet_definition *packet_definition,
	long packet_version,
	void const *decoded_packet,
	void *buffer,
	short *buffer_size,
	short maximum_buffer_size)
{
	struct data_encoding_state state;
	short version;

	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 61, packet_definition);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 62, buffer && buffer_size);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 63, maximum_buffer_size>=0);

	data_packet_verify(packet_definition);
	data_encode_new(&state, buffer, maximum_buffer_size);
	version = (short)packet_version;
	if (version == NONE)
		version = packet_definition->version;
	if (packet_definition->version > 0)
	{
		byte version_byte = (byte)version;
		data_encode_memory(&state, &version_byte, 1, 1);
	}
	_data_packet_encode(
		packet_definition,
		&state,
		version,
		decoded_packet,
		NULL,
		packet_definition->fields,
		NULL);
	*buffer_size = (short)state.offset;

	return !state.overflow;
}

boolean data_packet_decode(
	struct data_packet_definition *packet_definition,
	void const *encoded_packet,
	short encoded_packet_size,
	void *decoded_packet,
	short *packet_version,
	short *decoded_packet_size)
{
	struct data_encoding_state state;
	short version;
	boolean result;

	result = FALSE;
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 95, encoded_packet);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 96, decoded_packet);
	match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 97, encoded_packet_size>=0);

	data_packet_verify(packet_definition);
	data_decode_new(&state, encoded_packet, encoded_packet_size);
	if (packet_definition->version == 0)
		version = 0;
	else
		version = data_decode_byte(&state);
	if (version <= packet_definition->version)
	{
		_data_packet_decode(
			packet_definition,
			&state,
			version,
			decoded_packet,
			NULL,
			packet_definition->fields,
			NULL);
		if (!state.overflow)
			result = TRUE;
	}
	if (packet_version)
		*packet_version = version;
	if (decoded_packet_size)
		*decoded_packet_size = (short)state.offset;

	return result;
}

/* ---------- private code */

void _data_packet_verify(
	struct data_packet_definition *packet_definition,
	short *packet_size,
	struct data_packet_field *fields,
	short *field_count)
{
	short field_size;
	short total_size;
	struct data_packet_field *field;

	field = fields;
	total_size = 0;
	/* BUG (original): a first version-ineligible field reads an indeterminate
	 * field_size; later ineligible fields reuse the preceding size. A corrected
	 * build should initialize field_size to zero before the loop. */
	while (field->type != _data_packet_field_end)
	{
		if (field->type < 0 || field->type >= _data_packet_field_type_count)
		{
			display_assert(
				csprintf(
					temporary,
					"unknown field type in packet '%s' (probably missing '__pack_pack_end')",
					packet_definition->name),
				"c:\\halo\\SOURCE\\memory\\data_packets.c",
				142,
				TRUE);
			system_exit(-1);
		}
		if (field->count <= 0)
		{
			display_assert(
				csprintf(
					temporary,
					"field has negative or zero count in packet '%s'",
					packet_definition->name),
				"c:\\halo\\SOURCE\\memory\\data_packets.c",
				144,
				TRUE);
			system_exit(-1);
		}

		if (packet_definition->version >= field->minimum_version &&
			(packet_definition->version <= field->maximum_version || field->maximum_version == 0))
		{
			switch (field->type)
			{
			case _data_packet_field_pad:
			case _data_packet_field_bytes:
			case _data_packet_field_raw:
				field_size = field->count;
				break;
			case _data_packet_field_shorts:
				field_size = field->count * sizeof(short);
				break;
			case _data_packet_field_longs:
				field_size = field->count * sizeof(long);
				break;
			case _data_packet_field_int64s:
				field_size = field->count * sizeof(__int64);
				break;
			case _data_packet_field_string:
				field_size = field->count + 1;
				break;
			case _data_packet_field_data:
				field_size = field->count + sizeof(short);
				break;
			case _data_packet_field_array:
			{
				short element_size;
				short element_field_count;

				_data_packet_verify(packet_definition, &element_size, field + 1, &element_field_count);
				field_size = sizeof(short) + field->count * element_size;
				field += element_field_count;
				break;
			}
			case _data_packet_field_end:
				field_size = 0;
				break;
			default:
				display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_packets.c", 193, TRUE);
				system_exit(-1);
				break;
			}
		}
		field->size = field_size;
		total_size += field_size;
		field++;
	}
	if (field_count)
		*field_count = (short)(field - fields + 1);
	if (packet_size)
		*packet_size = total_size;

	return;
}

void _data_packet_encode(
	struct data_packet_definition *packet_definition,
	struct data_encoding_state *state,
	short packet_version,
	void const *decoded_packet,
	short *encoded_packet_size,
	struct data_packet_field *fields,
	short *field_count)
{
	struct data_packet_field *field;
	byte const *decoded_data;
	byte const *decoded_start;

	field = fields;
	decoded_data = decoded_packet;
	decoded_start = decoded_data;
	while (field->type != _data_packet_field_end)
	{
		if (packet_version >= field->minimum_version &&
			(packet_version <= field->maximum_version || field->maximum_version == 0))
		{
			switch (field->type)
			{
			case _data_packet_field_pad:
				break;
			case _data_packet_field_bytes:
				data_encode_memory(state, decoded_data, field->count, 1);
				break;
			case _data_packet_field_shorts:
				data_encode_memory(state, decoded_data, field->count, -2);
				break;
			case _data_packet_field_longs:
				data_encode_memory(state, decoded_data, field->count, -4);
				break;
			case _data_packet_field_int64s:
				data_encode_memory(state, decoded_data, field->count, -8);
				break;
			case _data_packet_field_string:
				data_encode_string(state, (char const *)decoded_data, field->count);
				break;
			case _data_packet_field_data:
			{
				short data_size = *(short const *)decoded_data;
				byte const *data = decoded_data + sizeof(short);

				match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 253, data_size>=0 && data_size<=field->count);
				if (data_size < 0 || data_size > field->count)
					data_size = 0;
				data_encode_integer(state, data_size, field->count);
				data_encode_memory(state, data, data_size, 1);
				break;
			}
			case _data_packet_field_raw:
				data_encode_memory(state, decoded_data, field->count, 1);
				break;
			case _data_packet_field_array:
			{
				short element_count = *(short const *)decoded_data;
				short element_field_count;
				byte const *element = decoded_data + sizeof(short);

				_data_packet_verify(
					packet_definition,
					NULL,
					field + 1,
					&element_field_count);
				match_assert("c:\\halo\\SOURCE\\memory\\data_packets.c", 281, element_count>=0 && element_count<=field->count);
				if (element_count < 0 || element_count > field->count)
					element_count = 0;
				data_encode_integer(state, element_count, field->count);
				while (element_count-- > 0)
				{
					short element_size;

					_data_packet_encode(
						packet_definition,
						state,
						packet_version,
						element,
						&element_size,
						field + 1,
						NULL);
					element += element_size;
				}
				field += element_field_count;
				break;
			}
			default:
				display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_packets.c", 297, TRUE);
				system_exit(-1);
				break;
			}
		}
		else
		{
			switch (field->type)
			{
			case _data_packet_field_pad:
				break;
			case _data_packet_field_bytes:
			case _data_packet_field_shorts:
			case _data_packet_field_longs:
			case _data_packet_field_int64s:
			case _data_packet_field_raw:
				data_encode_memory(state, NULL, field->count, 1);
				break;
			case _data_packet_field_string:
			{
				byte zero;

				zero = 0;
				data_encode_memory(state, &zero, 1, 1);
				break;
			}
			case _data_packet_field_data:
			case _data_packet_field_array:
				data_encode_integer(state, 0, field->count);
				break;
			default:
				display_assert(NULL, "c:\\halo\\SOURCE\\memory\\data_packets.c", 324, TRUE);
				system_exit(-1);
				break;
			}
		}
		decoded_data += field->size;
		field++;
	}
	if (field_count)
		*field_count = (short)(field - fields + 1);
	if (encoded_packet_size)
		*encoded_packet_size = (short)(decoded_data - decoded_start);

	return;
}

void _data_packet_decode(
	struct data_packet_definition *packet_definition,
	struct data_encoding_state *state,
	short packet_version,
	void *decoded_packet,
	short *encoded_packet_size,
	struct data_packet_field *fields,
	short *field_count)
{
	struct data_packet_field *field;
	byte *decoded_data;
	byte *decoded_start;

	field = fields;
	decoded_data = decoded_packet;
	decoded_start = decoded_data;
	while (field->type != _data_packet_field_end)
	{
		if (packet_version >= field->minimum_version &&
			(packet_version <= field->maximum_version || field->maximum_version == 0))
		{
			switch (field->type)
			{
			case _data_packet_field_bytes:
			{
				void *source = data_decode_memory(state, field->count, 1);
				if (source)
					csmemcpy(decoded_data, source, field->count);
				break;
			}
			case _data_packet_field_shorts:
			{
				void *source = data_decode_memory(state, field->count, -2);
				if (source)
					csmemcpy(decoded_data, source, field->count * sizeof(short));
				break;
			}
			case _data_packet_field_longs:
			{
				void *source = data_decode_memory(state, field->count, -4);
				if (source)
					csmemcpy(decoded_data, source, field->count * sizeof(long));
				break;
			}
			case _data_packet_field_int64s:
			{
				void *source = data_decode_memory(state, field->count, -8);
				if (source)
					csmemcpy(decoded_data, source, field->count * sizeof(__int64));
				break;
			}
			case _data_packet_field_string:
			{
				char *string = data_decode_string(state, field->count);
				if (string)
					csstrcpy((char *)decoded_data, string);
				break;
			}
			case _data_packet_field_data:
			{
				short data_size;
				void *source;

				data_size = (short)data_decode_integer(state, field->count);
				*(short *)decoded_data = data_size;
				source = data_decode_memory(state, data_size, 1);
				if (source)
					csmemcpy(decoded_data + sizeof(short), source, data_size);
				break;
			}
			case _data_packet_field_raw:
			{
				void *source = data_decode_memory(state, field->count, 1);
				if (source)
					csmemcpy(decoded_data, source, field->count);
				break;
			}
			case _data_packet_field_array:
			{
				short element_count;
				short element_size;
				short element_field_count;
				byte *element;

				element_count = (short)data_decode_integer(state, field->count);
				_data_packet_verify(packet_definition, NULL, field + 1, &element_field_count);
				if (element_count < 0 || element_count > field->count)
					element_count = 0;
				*(short *)decoded_data = element_count;
				element = decoded_data + sizeof(short);
				while (element_count-- > 0)
				{
					_data_packet_decode(
						packet_definition,
						state,
						packet_version,
						element,
						&element_size,
						field + 1,
						NULL);
					element += element_size;
				}
				field += element_field_count;
				break;
			}
			case _data_packet_field_pad:
			default:
				break;
			}
		}
		else
			csmemset(decoded_data, 0, field->size);
		decoded_data += field->size;
		field++;
	}
	if (field_count)
		*field_count = (short)(field - fields + 1);
	if (encoded_packet_size)
		*encoded_packet_size = (short)(decoded_data - decoded_start);

	return;
}
