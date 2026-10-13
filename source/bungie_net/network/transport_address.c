/*
TRANSPORT_ADDRESS.C

symbols in this file:
0006FFF0 00a0:
	_create_transport_address (0000)
00070090 0070:
	_delete_transport_address (0000)
00070100 0100:
	_transport_address_equivalent (0000)
00070200 00f0:
	_transport_address_to_string (0000)
000702F0 0130:
	_transport_error_to_string (0000)
00255B8C 0008:
	??_C@_07LHEPONKL@address?$AA@ (0000)
00255B94 0016:
	??_C@_0BG@GPECFFFI@transport_initialized?$AA@ (0000)
00255BAC 0036:
	??_C@_0DG@MOOGDGPH@c?3?2halo?2SOURCE?2bungie_net?2networ@ (0000)
00255BE4 0029:
	??_C@_0CJ@JIAALALG@IPV4_ADDRESS_LENGTH?5?$DN?$DN?5b?9?$DOaddres@ (0000)
00255C10 0029:
	??_C@_0CJ@HCIGGNNE@IPV4_ADDRESS_LENGTH?5?$DN?$DN?5a?9?$DOaddres@ (0000)
00255C3C 0002:
	??_C@_01OJONOECF@b?$AA@ (0000)
00255C40 0002:
	??_C@_01MCMALHOG@a?$AA@ (0000)
00255C44 0024:
	??_C@_0CE@BKPDKIMM@?$CF4X?4?$CF4X?4?$CF4X?4?$CF4X?4?$CF4X?4?$CF4X?4?$CF4X?4?$CF4X?3@ (0000)
00255C68 0014:
	??_C@_0BE@EFJFBANP@?$CFhd?4?$CFhd?4?$CFhd?4?$CFhd?3?$CFhd?$AA@ (0000)
00255C7C 002c:
	??_C@_0CM@HJNAPKHL@IPV4_ADDRESS_LENGTH?5?$DN?$DN?5addr?9?$DOadd@ (0000)
00255CA8 0005:
	??_C@_04HLBMEOMD@addr?$AA@ (0000)
00255CB0 001a:
	??_C@_0BK@DKILIOMH@?$DMunknown?5transport?5error?$DO?$AA@ (0000)
00255CCC 0026:
	??_C@_0CG@GJOAJKPP@_transport_result_connect_in_pro@ (0000)
00255CF4 0029:
	??_C@_0CJ@KLDGMKNJ@_transport_result_dns_lookup_in_@ (0000)
00255D20 001c:
	??_C@_0BM@MOAMJPHC@_transport_error_poll_error?$AA@ (0000)
00255D3C 0023:
	??_C@_0CD@OLHPGLJL@_transport_error_endpoint_set_fu@ (0000)
00255D60 0025:
	??_C@_0CF@EOHIKLGM@_transport_error_endpoint_not_in@ (0000)
00255D88 0020:
	??_C@_0CA@KOHMPNKI@_transport_error_options_failed?$AA@ (0000)
00255DA8 001f:
	??_C@_0BP@DBCACINH@_transport_error_listen_failed?$AA@ (0000)
00255DC8 0020:
	??_C@_0CA@FCFPELFI@_transport_error_connect_failed?$AA@ (0000)
00255DE8 0021:
	??_C@_0CB@DFMBBADG@_transport_error_address_unknown@ (0000)
00255E0C 001f:
	??_C@_0BP@NDAFLCHE@_transport_error_bind_endpoint?$AA@ (0000)
00255E2C 001f:
	??_C@_0BP@KLAPLPBJ@_transport_result_poll_timeout?$AA@ (0000)
00255E4C 001e:
	??_C@_0BO@FIGFJHPP@_transport_error_bad_endpoint?$AA@ (0000)
00255E6C 001e:
	??_C@_0BO@HCCGKGOJ@_transport_error_buffers_full?$AA@ (0000)
00255E8C 001b:
	??_C@_0BL@JNCEOCJP@_transport_error_seg_fault?$AA@ (0000)
00255EA8 001f:
	??_C@_0BP@IGIFAHML@_transport_error_out_of_memory?$AA@ (0000)
00255EC8 0024:
	??_C@_0CE@BJDLEGKH@_transport_error_dns_lookup_fail@ (0000)
00255EEC 0026:
	??_C@_0CG@DFIHKINE@_transport_error_bad_input_param@ (0000)
00255F14 0026:
	??_C@_0CG@FOCINHIK@_transport_result_already_initia@ (0000)
00255F3C 0021:
	??_C@_0CB@FHIMIPBL@_transport_error_not_initialized@ (0000)
00255F60 0028:
	??_C@_0CI@PBPCPNIC@_transport_result_operation_woul@ (0000)
00255F88 0021:
	??_C@_0CB@DOGKFJME@_transport_error_connection_lost@ (0000)
00255FAC 001d:
	??_C@_0BN@JNHGFCD@_transport_error_endpoint_io?$AA@ (0000)
00255FCC 0019:
	??_C@_0BJ@ILGMKEDA@_transport_error_unknown?$AA@ (0000)
00255FE8 0016:
	??_C@_0BG@MJBCLCLD@_transport_error_none?$AA@ (0000)
0031CD30 0100:
	_transport_address_string (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"

#include "bungie_net/network/transport.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

#ifndef HALO_ANDROID /* Mach-O section names differ; the default is .bss anyway */
#pragma bss_seg(".bss")
#endif
char transport_address_string[256];
#ifndef HALO_ANDROID
#pragma bss_seg()
#endif

/* ---------- public code */

struct transport_address *create_transport_address(
	struct transport_address_data const *address,
	word address_length,
	word port)
{
	struct transport_address *result;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 30, transport_initialized);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 31, address);

	result = match_malloc(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c",
		33,
		sizeof(*result));
	if (result)
	{
		result->address = *address;
		result->address_length = address_length;
		result->port = port;
		result->address_type = 0;
	}

	return result;
}

void delete_transport_address(
	struct transport_address *address)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 47, transport_initialized);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 48, address);
	match_free("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 50, address);
	return;
}

long transport_address_equivalent(
	struct transport_address const *a,
	struct transport_address const *b)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 59, a);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 60, b);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 61, transport_initialized);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 63, IPV4_ADDRESS_LENGTH == a->address_length);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 64, IPV4_ADDRESS_LENGTH == b->address_length);

	if (csmemcmp(
			&a->address,
			&b->address,
			a->address_length > b->address_length ? a->address_length : b->address_length) == 0 &&
		a->port == b->port)
		return TRUE;

	return FALSE;
}

char const *transport_address_to_string(
	struct transport_address const *addr)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 74, addr);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\network\\transport_address.c", 75, IPV4_ADDRESS_LENGTH == addr->address_length);

	transport_address_string[0] = 0;
	if (addr->address_length == IPV4_ADDRESS_LENGTH)
	{
		_snprintf(
			transport_address_string,
			NUMBEROF(transport_address_string),
			"%hd.%hd.%hd.%hd:%hd",
			addr->address.bytes[3],
			addr->address.bytes[2],
			addr->address.bytes[1],
			addr->address.bytes[0],
			addr->port);
	}
	else if (addr->address_length == IPV6_ADDRESS_LENGTH)
	{
		_snprintf(
			transport_address_string,
			NUMBEROF(transport_address_string),
			"%4X.%4X.%4X.%4X.%4X.%4X.%4X.%4X:%hd",
			addr->address.words[0],
			addr->address.words[1],
			addr->address.words[2],
			addr->address.words[3],
			addr->address.words[4],
			addr->address.words[5],
			addr->address.words[6],
			addr->address.words[7],
			addr->port);
	}

	return transport_address_string;
}

char const *transport_error_to_string(
	short error)
{
	switch (error)
	{
	case _transport_error_none:
		return "_transport_error_none";
	case _transport_error_unknown:
		return "_transport_error_unknown";
	case _transport_error_endpoint_io:
		return "_transport_error_endpoint_io";
	case _transport_error_connection_lost:
		return "_transport_error_connection_lost";
	case _transport_result_operation_would_block:
		return "_transport_result_operation_would_block";
	case _transport_error_not_initialized:
		return "_transport_error_not_initialized";
	case _transport_result_already_initialized:
		return "_transport_result_already_initialized";
	case _transport_error_bad_input_parameters:
		return "_transport_error_bad_input_parameters";
	case _transport_error_dns_lookup_failure:
		return "_transport_error_dns_lookup_failure";
	case _transport_error_out_of_memory:
		return "_transport_error_out_of_memory";
	case _transport_error_seg_fault:
		return "_transport_error_seg_fault";
	case _transport_error_buffers_full:
		return "_transport_error_buffers_full";
	case _transport_error_bad_endpoint:
		return "_transport_error_bad_endpoint";
	case _transport_result_poll_timeout:
		return "_transport_result_poll_timeout";
	case _transport_error_bind_endpoint:
		return "_transport_error_bind_endpoint";
	case _transport_error_address_unknown:
		return "_transport_error_address_unknown";
	case _transport_error_connect_failed:
		return "_transport_error_connect_failed";
	case _transport_error_listen_failed:
		return "_transport_error_listen_failed";
	case _transport_error_options_failed:
		return "_transport_error_options_failed";
	case _transport_error_endpoint_not_in_set:
		return "_transport_error_endpoint_not_in_set";
	case _transport_error_endpoint_set_full:
		return "_transport_error_endpoint_set_full";
	case _transport_error_poll_error:
		return "_transport_error_poll_error";
	case _transport_result_dns_lookup_in_progress:
		return "_transport_result_dns_lookup_in_progress";
	case _transport_result_connect_in_progress:
		return "_transport_result_connect_in_progress";
	default:
		return "<unknown transport error>";
	}
}

/* ---------- private code */
