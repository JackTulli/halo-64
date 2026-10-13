/*
DATA_PACKET_GROUPS.C

symbols in this file:
0010A150 00d0:
	_data_packet_group_initialize (0000)
0010A220 0040:
	_data_packet_groups_get_error (0000)
0010A260 0170:
	_data_packet_group_decode_packet (0000)
0010A3D0 00f0:
	_data_packet_group_append_packet_header (0000)
0010A4C0 0130:
	_data_packet_group_encode_packet (0000)
0027D608 000e:
	??_C@_0O@IOJDJFNI@packet_header?$AA@ (0000)
0027D618 0067:
	??_C@_0GH@CHKGHAIM@packet?9?$DOdefinition?9?$DOsize?5?$CL?5sizeo@ (0000)
0027D680 0048:
	??_C@_0EI@IKJFLLBL@packet?9?$DOdefinition?9?$DOsize?$DM?$DNgroup_@ (0000)
0027D6C8 0055:
	??_C@_0FF@OGBKODEN@packet?9?$DOpacket_class?$DO?$DN0?5?$CG?$CG?5packe@ (0000)
0027D720 002b:
	??_C@_0CL@ENPIFLLL@c?3?2halo?2SOURCE?2memory?2data_packe@ (0000)
0027D74C 001a:
	??_C@_0BK@MIAMKHEE@got?5packet?5with?5no?5header?$AA@ (0000)
0027D768 0019:
	??_C@_0BJ@BGAOCGIB@got?5packet?5with?5bad?5type?$AA@ (0000)
0027D784 0021:
	??_C@_0CB@BPPNEFM@got?5packet?5with?5mismatched?5class@ (0000)
0027D7A8 0021:
	??_C@_0CB@OKKGPBBC@got?5packet?5which?5wouldn?8t?5decode@ (0000)
0027D7D0 0057:
	??_C@_0FH@FIMIEPDH@expected_packet_class?$DO?$DN0?5?$CG?$CG?5expe@ (0000)
0027D828 001e:
	??_C@_0BO@KIMBAFNK@packet_type?5?$CG?$CG?5packet_version?$AA@ (0000)
0027D848 0026:
	??_C@_0CG@PNFBKFHL@encoded_packet?5?$CG?$CG?5encoded_packet@ (0000)
0027D870 000f:
	??_C@_0P@EDJPKHAL@decoded_packet?$AA@ (0000)
0027D880 0029:
	??_C@_0CJ@LABNGHPB@couldn?8t?5append?5header?5to?5encode@ (0000)
0027D8B0 0042:
	??_C@_0EC@JCNCOIDD@packet_type?$DO?$DN0?5?$CG?$CG?5packet_type?$DMgr@ (0000)
0027D8F4 0018:
	??_C@_0BI@JKLMPOFI@?$CKencoded_packet_size?$DO?$DN0?$AA@ (0000)
0027D90C 0017:
	??_C@_0BH@IFJMIBKD@couldn?8t?5encode?5packet?$AA@ (0000)
0027D924 0013:
	??_C@_0BD@KMJAHOKI@packet?9?$DOdefinition?$AA@ (0000)
0027D938 0011:
	??_C@_0BB@IACFGFHK@group_definition?$AA@ (0000)
00309E38 0024:
	_data_00309e38 (0000)
00456624 0004:
	_bss_00456624 (0000)
*/

/* ---------- headers */

#include "memory/data_packet_groups.h"
#include "memory/data_packets.h"
#include "memory/byte_swapping.h"

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct packet_header
{
	char packet_type;
};

struct packet_header_byte_swap_data
{
	byte_swap_code codes[4];
	struct byte_swap_definition definition;
};

/* ---------- prototypes */

/* ---------- globals */

char const *global_data_packet_groups_error_string = NULL;

struct packet_header_byte_swap_data packet_header_bs =
{
	{ _begin_bs_array, 1, _1byte, _end_bs_array },
	{ "packet_header", sizeof(struct packet_header), packet_header_bs.codes, BYTE_SWAP_DEFINITION_SIGNATURE, FALSE }
};

/* ---------- public code */

void data_packet_group_initialize(
	struct data_packet_group_definition *group_definition)
{
	short packet_index;

	for (packet_index = 0; packet_index < group_definition->packet_type_count; packet_index++)
	{
		struct data_packet_entry *packet = &group_definition->packets[packet_index];

		if (packet->definition)
		{
			match_assert(
				"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
				40,
				packet->packet_class>=0 && packet->packet_class<group_definition->packet_class_count);
			match_assert(
				"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
				41,
				packet->definition->size<=group_definition->maximum_decoded_packet_size);
			match_assert(
				"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
				42,
				packet->definition->size + sizeof(struct packet_header)<=group_definition->maximum_encoded_packet_size);

			data_packet_verify(packet->definition);
		}
	}

	return;
}

char const *data_packet_groups_get_error(
	void)
{
	char const *result = global_data_packet_groups_error_string;
	global_data_packet_groups_error_string = NULL;

	match_assert("c:\\halo\\SOURCE\\memory\\data_packet_groups.c", 57, result);

	return result;
}

boolean data_packet_group_append_packet_header(
	struct data_packet_group_definition *group_definition,
	void *encoded_packet,
	short *encoded_packet_size,
	short packet_type)
{
	struct packet_header *packet_header = (struct packet_header *)((byte *)encoded_packet + *encoded_packet_size);
	char const *error = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		172,
		encoded_packet && encoded_packet_size);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		173,
		*encoded_packet_size>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		174,
		packet_type>=0 && packet_type<group_definition->packet_type_count);

	if (*encoded_packet_size + sizeof(struct packet_header) < group_definition->maximum_encoded_packet_size)
	{
		packet_header->packet_type = (char)packet_type;
		byte_swap_data(&packet_header_bs.definition, packet_header, 1);
		++*encoded_packet_size;
	}
	else
	{
		error = "couldn't append header to encoded packet";
	}

	global_data_packet_groups_error_string = error;

	return error==NULL;
}

boolean data_packet_group_encode_packet(
	struct data_packet_group_definition *group_definition,
	void const *decoded_packet,
	void *encoded_packet,
	short *encoded_packet_size,
	short packet_type,
	long packet_version)
{
	char const *error = NULL;
	struct data_packet_entry *packet;

	match_assert("c:\\halo\\SOURCE\\memory\\data_packet_groups.c", 132, group_definition);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		133,
		packet_type>=0 && packet_type<group_definition->packet_type_count);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		134,
		encoded_packet && encoded_packet_size);

	packet = &group_definition->packets[packet_type];
	match_assert("c:\\halo\\SOURCE\\memory\\data_packet_groups.c", 139, packet->definition);

	if (data_packet_encode(
		packet->definition,
		packet_version,
		decoded_packet,
		encoded_packet,
		encoded_packet_size,
		(short)group_definition->maximum_encoded_packet_size))
	{
		if (!data_packet_group_append_packet_header(
			group_definition,
			encoded_packet,
			encoded_packet_size,
			(short)packet_type))
		{
			error = global_data_packet_groups_error_string;
		}
	}
	else
	{
		error = "couldn't encode packet";
	}

	global_data_packet_groups_error_string = error;

	return error==NULL;
}

boolean data_packet_group_decode_packet(
	struct data_packet_group_definition *group_definition,
	void *decoded_packet,
	void const *encoded_packet,
	short *encoded_packet_size,
	short *packet_type,
	short *packet_version,
	short expected_packet_class)
{
	char const *error = NULL;

	match_assert("c:\\halo\\SOURCE\\memory\\data_packet_groups.c", 73, decoded_packet);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		74,
		encoded_packet && encoded_packet_size);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		75,
		packet_type && packet_version);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\data_packet_groups.c",
		77,
		expected_packet_class>=0 && expected_packet_class<group_definition->packet_class_count);

	if (*encoded_packet_size >= sizeof(struct packet_header))
	{
		struct packet_header *packet_header = (struct packet_header *)((byte const *)encoded_packet + *encoded_packet_size - sizeof(struct packet_header));

		byte_swap_data(&packet_header_bs.definition, packet_header, 1);

		if (packet_header->packet_type>=0 && packet_header->packet_type<group_definition->packet_type_count)
		{
			struct data_packet_entry *packet = &group_definition->packets[packet_header->packet_type];

			if (packet->packet_class == expected_packet_class)
			{
				--*encoded_packet_size;

				if (!packet->definition ||
					data_packet_decode(
						packet->definition,
						encoded_packet,
						*encoded_packet_size,
						decoded_packet,
						packet_version,
						FALSE))
				{
					*packet_type = packet_header->packet_type;
				}
				else
				{
					error = "got packet which wouldn't decode";
				}
			}
			else
			{
				error = "got packet with mismatched class";
			}
		}
		else
		{
			error = "got packet with bad type";
		}
	}
	else
	{
		error = "got packet with no header";
	}

	global_data_packet_groups_error_string = error;

	return error==NULL;
}

/* ---------- private code */
