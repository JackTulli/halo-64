/*
KEY_AGREEMENT.C

symbols in this file:
0006E9A0 0050:
	_key_agreement_get_packet_type (0000)
0006E9F0 0030:
	_key_agreement_decode_packet (0000)
0006EA20 0020:
	_key_agreement_encode_packet (0000)
0006EA40 00a0:
	_key_agreement_build_message (0000)
0006EAE0 0070:
	_build_initiate_key_agreement_message (0000)
0006EB50 0050:
	_build_finalize_key_agreement_message (0000)
0006EBA0 0070:
	_is_message_encryption_key_message (0000)
0006EC10 0080:
	_initiate_key_exchange (0000)
0006EC90 01a0:
	_complete_key_exchange (0000)
0006EE30 0010:
	_initialize_key_agreement_packets (0000)
0025565C 001c:
	??_C@_0BM@CDLIOKHM@key_agreement_packets_group?$AA@ (0000)
00255678 0026:
	??_C@_0CG@MLKFMCKI@message_finalize_key_agreement_p@ (0000)
002556A0 0026:
	??_C@_0CG@GCJFJJDO@message_initiate_key_agreement_p@ (0000)
002556C8 0042:
	??_C@_0EC@PKJCEKIE@?$CImessage_type?$DN?5GET_MESSAGE_TYPE?$CI@ (0000)
0025570C 0031:
	??_C@_0DB@CMOIKFEE@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
00255740 0012:
	??_C@_0BC@PHJNCMNB@prime?5?$CG?$CG?5g?5?$CG?$CG?5key?$AA@ (0000)
00255754 0004:
	??_C@_03ICHNJLJF@key?$AA@ (0000)
00255758 0016:
	??_C@_0BG@DBBMADFN@msgptr?5?$CG?$CG?5packet_type?$AA@ (0000)
00255770 0029:
	??_C@_0CJ@IKOFDDIL@msgptr?5?$CG?$CG?5prime?5?$CG?$CG?5secret?5?$CG?$CG?5pri@ (0000)
002DCAD4 0088:
	_key_agreement_packets (0000)
0031C520 0200:
	_key_agreement_message_buffer (0000)
*/

/* ---------- headers */

#include "bungie_net/common/key_agreement.h"

#include "bungie_net/common/message_header.h"
#include "bungie_net/common/public_key_crypt.h"
#include "bungie_net/common/random_numbers.h"
#include "bungie_net/network/transport_endpoint_winsock.h"
#include "memory/data_packet_groups.h"

/* ---------- constants */

enum
{
	KEY_AGREEMENT_ENCODED_PACKET_SIZE = 0x80,
	KEY_AGREEMENT_MESSAGE_BUFFER_SIZE = 0x200,
	KEY_AGREEMENT_PACKET_VERSION = 1,
};

enum key_agreement_packet_type
{
	_key_agreement_packet_type_initiate = 0,
	_key_agreement_packet_type_finalize,
	NUMBER_OF_KEY_AGREEMENT_PACKET_TYPES,
};

/* ---------- macros */

#define KEY_AGREEMENT_FILE "c:\\halo\\SOURCE\\bungie_net\\common\\key_agreement.c"
#define DATA_PACKET_FIELD(type, count) { type, count, 0, 0, 0 }
#define DATA_PACKET_FIELD_END DATA_PACKET_FIELD(_data_packet_field_end, 0)
#define KEY_AGREEMENT_PACKET_DEFINITION(member, name, structure) \
	{ name, 0, sizeof(structure), KEY_AGREEMENT_PACKET_VERSION, key_agreement_packets.member##_fields, FALSE }

/* ---------- structures */

struct message_initiate_key_agreement
{
	struct public_key prime;
	struct public_key generator;
	struct public_key key;
};

struct message_finalize_key_agreement
{
	struct public_key key;
};

union key_agreement_packet_value
{
	long value;
	short encoded;
};

struct key_agreement_packet_definitions
{
	struct data_packet_field initiate_fields[4];
	struct data_packet_definition initiate;
	struct data_packet_field finalize_fields[2];
	struct data_packet_definition finalize;
	struct data_packet_entry packets[NUMBER_OF_KEY_AGREEMENT_PACKET_TYPES];
	struct data_packet_group_definition group;
};

/* ---------- prototypes */

static char key_agreement_get_packet_type(
	word const *msgptr);
static boolean key_agreement_decode_packet(
	void *decoded_packet,
	void const *encoded_packet,
	short *encoded_packet_size,
	short *packet_type,
	short *packet_version,
	short expected_packet_class);
static boolean key_agreement_encode_packet(
	void const *decoded_packet,
	void *encoded_packet,
	short *encoded_packet_size,
	short packet_type,
	long packet_version);
static word *key_agreement_build_message(
	short packet_type,
	void const *packet,
	void *buffer,
	word buffer_size);
static word *build_initiate_key_agreement_message(
	struct public_key const *prime,
	struct public_key const *g,
	struct public_key const *key,
	void *buffer,
	word buffer_size);
static word *build_finalize_key_agreement_message(
	struct public_key const *key,
	void *buffer,
	word buffer_size);

/* ---------- globals */

struct key_agreement_packet_definitions key_agreement_packets =
{
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 2),
		DATA_PACKET_FIELD(_data_packet_field_longs, 2),
		DATA_PACKET_FIELD(_data_packet_field_longs, 2),
		DATA_PACKET_FIELD_END,
	},
	KEY_AGREEMENT_PACKET_DEFINITION(initiate, "message_initiate_key_agreement_packet", struct message_initiate_key_agreement),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 2),
		DATA_PACKET_FIELD_END,
	},
	KEY_AGREEMENT_PACKET_DEFINITION(finalize, "message_finalize_key_agreement_packet", struct message_finalize_key_agreement),
	{
		{ 0, 0, &key_agreement_packets.initiate },
		{ 0, 0, &key_agreement_packets.finalize },
	},
	{
		"key_agreement_packets_group",
		NUMBER_OF_KEY_AGREEMENT_PACKET_TYPES,
		1,
		0x60,
		KEY_AGREEMENT_ENCODED_PACKET_SIZE,
		key_agreement_packets.packets,
	},
};

static byte key_agreement_message_buffer[KEY_AGREEMENT_MESSAGE_BUFFER_SIZE];

/* ---------- public code */

long is_message_encryption_key_message(
	word const *msgptr,
	word message_size,
	byte *packet_type)
{
	word message_flags;
	byte message_type;

	match_assert(KEY_AGREEMENT_FILE, 0xC4, msgptr && packet_type);

	message_flags = GET_MESSAGE_FLAGS(*msgptr);
	*packet_type = ((byte const *)msgptr)[message_size - 1];
	message_type = GET_MESSAGE_TYPE(*msgptr);
	if (TEST_FLAG(message_flags, 1) &&
		message_type == _message_type_packet &&
		(*packet_type == _key_agreement_packet_type_initiate ||
		*packet_type == _key_agreement_packet_type_finalize))
	{
		return TRUE;
	}

	return FALSE;
}

boolean initiate_key_exchange(
	struct transport_endpoint *endpoint,
	struct public_key *key,
	struct public_key *prime,
	struct public_key *secret)
{
	boolean success = TRUE;
	struct public_key generator;
	word *message;
	short message_size;

	generate_key_parameters(prime, secret, &generator);
	generate_public_key(prime, secret, &generator, key);
	message = build_initiate_key_agreement_message(
		prime,
		&generator,
		key,
		key_agreement_message_buffer,
		sizeof(key_agreement_message_buffer));
	if (!message)
	{
		success = FALSE;
	}
	else
	{
		message_size = GET_MESSAGE_SIZE(*message);
		byte_swap_message_header(message, _byte_order_network);
		if (write_endpoint(endpoint, message, message_size) != message_size)
			success = FALSE;
	}

	return success;
}

boolean complete_key_exchange(
	struct transport_endpoint *endpoint,
	word const *msgptr,
	struct public_key const *prime,
	struct public_key *secret,
	struct public_key *private_key)
{
	struct message_initiate_key_agreement initiate_packet;
	struct message_finalize_key_agreement finalize_packet;
	struct public_key key;
	short encoded_packet_size;
	short packet_type;
	short packet_version = KEY_AGREEMENT_PACKET_VERSION;
	word *message;
	short message_size;
	byte message_type;

	match_assert(KEY_AGREEMENT_FILE, 0x105, msgptr && prime && secret && private_key);

	message_size = GET_MESSAGE_SIZE(*msgptr);
	encoded_packet_size = message_size - sizeof(word);
	message_type = GET_MESSAGE_TYPE(*msgptr);
	if (message_type == _message_type_packet)
	{
		packet_type = key_agreement_get_packet_type(msgptr);

		switch (packet_type)
		{
		case _key_agreement_packet_type_initiate:
			{
				if (!key_agreement_decode_packet(
					&initiate_packet,
					msgptr + 1,
					&encoded_packet_size,
					&packet_type,
					&packet_version,
					0))
				{
					return FALSE;
				}

				secret->dwords[0] = randomrange(0xFF, initiate_packet.prime.dwords[0] - 2);
				secret->dwords[1] = randomrange(0xFF, initiate_packet.prime.dwords[1] - 2);
				generate_public_key(&initiate_packet.prime, secret, &initiate_packet.generator, &key);
				message = build_finalize_key_agreement_message(
					&key,
					key_agreement_message_buffer,
					sizeof(key_agreement_message_buffer));
				if (!message)
					return FALSE;

				message_size = GET_MESSAGE_SIZE(*message);
				byte_swap_message_header(message, _byte_order_network);
				if (write_endpoint(endpoint, message, message_size) != message_size)
					return FALSE;

				generate_private_key(&initiate_packet.key, &initiate_packet.prime, secret, private_key);
				return TRUE;
			}

		case _key_agreement_packet_type_finalize:
			{
				if (!key_agreement_decode_packet(
					&finalize_packet,
					msgptr + 1,
					&encoded_packet_size,
					&packet_type,
					&packet_version,
					0))
				{
					return FALSE;
				}

				generate_private_key(&finalize_packet.key, prime, secret, private_key);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void initialize_key_agreement_packets(
	void)
{
	data_packet_group_initialize(&key_agreement_packets.group);

	return;
}

/* ---------- private code */

static char key_agreement_get_packet_type(
	word const *msgptr)
{
	byte message_type;
	word message_size = GET_MESSAGE_SIZE(*msgptr);

	match_assert(
		KEY_AGREEMENT_FILE,
		0x4D,
		(message_type= GET_MESSAGE_TYPE(*msgptr)) == _message_type_packet);

	return ((char const *)msgptr)[message_size - 1];
}

static boolean key_agreement_decode_packet(
	void *decoded_packet,
	void const *encoded_packet,
	short *encoded_packet_size,
	short *packet_type,
	short *packet_version,
	short expected_packet_class)
{
	return data_packet_group_decode_packet(
		&key_agreement_packets.group,
		decoded_packet,
		encoded_packet,
		encoded_packet_size,
		packet_type,
		packet_version,
		expected_packet_class);
}

static boolean key_agreement_encode_packet(
	void const *decoded_packet,
	void *encoded_packet,
	short *encoded_packet_size,
	short packet_type,
	long packet_version)
{
	return data_packet_group_encode_packet(
		&key_agreement_packets.group,
		decoded_packet,
		encoded_packet,
		encoded_packet_size,
		packet_type,
		packet_version);
}

static word *key_agreement_build_message(
	short packet_type,
	void const *packet,
	void *buffer,
	word buffer_size)
{
	byte encoded_packet[KEY_AGREEMENT_ENCODED_PACKET_SIZE] = { 0 };
	union key_agreement_packet_value encoded_packet_size;
	word *message = NULL;

	encoded_packet_size.value = sizeof(encoded_packet);
	if (key_agreement_encode_packet(
		packet,
		encoded_packet,
		&encoded_packet_size.encoded,
		packet_type,
		KEY_AGREEMENT_PACKET_VERSION))
	{
		message = create_message(
			_message_type_packet,
			encoded_packet,
			encoded_packet_size.value,
			buffer,
			buffer_size);
		if (message)
			SET_MESSAGE_FLAGS(*message, FLAG(1));
	}

	return message;
}

static word *build_initiate_key_agreement_message(
	struct public_key const *prime,
	struct public_key const *g,
	struct public_key const *key,
	void *buffer,
	word buffer_size)
{
	struct message_initiate_key_agreement packet;

	match_assert(KEY_AGREEMENT_FILE, 0xA2, prime && g && key);

	packet.prime = *prime;
	packet.generator = *g;
	packet.key = *key;

	return key_agreement_build_message(
		_key_agreement_packet_type_initiate,
		&packet,
		buffer,
		buffer_size);
}

static word *build_finalize_key_agreement_message(
	struct public_key const *key,
	void *buffer,
	word buffer_size)
{
	struct message_finalize_key_agreement packet;

	match_assert(KEY_AGREEMENT_FILE, 0xB3, key);

	packet.key = *key;

	return key_agreement_build_message(
		_key_agreement_packet_type_finalize,
		&packet,
		buffer,
		buffer_size);
}
