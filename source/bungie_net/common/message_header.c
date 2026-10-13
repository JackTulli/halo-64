/*
MESSAGE_HEADER.C

symbols in this file:
0006F1B0 00e0:
	_build_message_header (0000)
0006F290 0080:
	_byte_swap_message_header (0000)
0006F310 0090:
	_create_message (0000)
00255814 002f:
	??_C@_0CP@LMKDGHMH@?$CI0?$DM?$CItype?$CJ?$CJ?5?$CG?$CG?5?$CI?$CItype?$CJ?$DMNUMBER_OF_@ (0000)
00255844 0032:
	??_C@_0DC@OCMMPODH@?$CI0?$DM?$DN?$CIlength?$CJ?$CJ?5?$CG?$CG?5?$CI?$CIlength?$CJ?$DM?$DNMAXI@ (0000)
00255878 0004:
	??_C@_03GLPMCEFE@msg?$AA@ (0000)
0025587C 0032:
	??_C@_0DC@DDJMFODG@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
002558B0 001c:
	??_C@_0BM@KGDGDGMC@?$CB?$CCbad?5value?5for?5byte?5order?$CC?$AA@ (0000)
002558CC 0007:
	??_C@_06MBPAJAPM@header?$AA@ (0000)
002558D4 001c:
	??_C@_0BM@BHCKAPPF@buffer_size?5?$DO?$DN?5message_size?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "message_header.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void build_message_header(
	word *msg,
	word length,
	byte type,
	byte flags)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 67, msg);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 69, (0<=(length)) && ((length)<=MAXIMUM_MESSAGE_SIZE));

	*msg = (*msg & 0xF) | (length << 4);

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 70, (0<(type)) && ((type)<NUMBER_OF_MESSAGE_TYPES));
	*msg = (*msg & 0xFFF3) | ((type & 3) << 2);

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 71, (0<=flags) && ((flags)<=MESSAGE_FLAG_BITS_MASK));
	*msg = (*msg & 0xFFFC) | flags;
}

void byte_swap_message_header(
	word *header,
	enum message_header_byte_order byte_order)
{
	word value;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 80, header);

	if (byte_order == _byte_order_network)
	{
		value = *header;
		*header = (value << 8) | (value >> 8);
		return;
	}

	if (byte_order == _byte_order_host)
	{
		value = *header;
		*header = (value << 8) | (value >> 8);
		return;
	}

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 94, !"bad value for byte order");
}

void *create_message(
	long type,
	void const *data,
	unsigned long data_size,
	void *buffer,
	word buffer_size)
{
	short message_size = data_size + sizeof(word);

	if (buffer)
	{
		match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c", 41, buffer_size >= message_size);
	}
	else
	{
		buffer = debug_malloc(
			message_size,
			FALSE,
			"c:\\halo\\SOURCE\\bungie_net\\common\\message_header.c",
			46);
	}

	if (buffer)
	{
		build_message_header((word *)buffer, message_size, type, 0);

		if (data)
		{
			csmemcpy((byte *)buffer + sizeof(word), data, (word)data_size);
		}
	}

	return buffer;
}

/* ---------- private code */
