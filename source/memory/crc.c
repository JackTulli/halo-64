/*
CRC.C

symbols in this file:
001088D0 0010:
	_crc_new (0000)
001088E0 0040:
	_build_crc_table (0000)
00108920 0080:
	_crc_checksum_buffer (0000)
0027D2E4 000f:
	??_C@_0P@JPGOHOCM@buffer_size?$DO?$DN0?$AA@ (0000)
0027D2F4 001c:
	??_C@_0BM@FPJPBIIF@c?3?2halo?2SOURCE?2memory?2crc?4c?$AA@ (0000)
00456220 0401:
	_crc_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "memory/crc.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

#pragma pack(push, 1)
struct crc_globals
{
	unsigned long table[256];
	boolean initialized;
};
#pragma pack(pop)

/* ---------- prototypes */

/* ---------- globals */

#ifndef HALO_ANDROID /* Mach-O section names differ; the default is .bss anyway */
#pragma bss_seg(".bss")
#endif
struct crc_globals crc_globals;
#ifndef HALO_ANDROID
#pragma bss_seg()
#endif

/* ---------- public code */

void crc_new(
	unsigned long *crc_reference)
{
	*crc_reference = 0xFFFFFFFF;
	return;
}

/* The descriptive private name follows the recovered cross-build CRC implementation. */
static void build_crc_table(
	unsigned long *crc_table)
{
	unsigned long byte_index;
	long byte_count;

	byte_index = 0;
	byte_count = 256;
	do
	{
		unsigned long crc = byte_index;
		long bit_count;

		bit_count = 8;
		do
		{
			if (crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;
			else
				crc >>= 1;
		} while (--bit_count);

		*crc_table = crc;
		byte_index++;
		crc_table++;
	} while (--byte_count);

	return;
}

void crc_checksum_buffer(
	unsigned long *crc_reference,
	void const *buffer,
	long buffer_size)
{
	unsigned long crc;
	unsigned long table_index;

	match_assert("c:\\halo\\SOURCE\\memory\\crc.c", 42, buffer_size>=0);

	if (!crc_globals.initialized)
	{
		build_crc_table(crc_globals.table);
		crc_globals.initialized = TRUE;
	}

	crc = *crc_reference;
	if (buffer_size > 0)
	{
		do
		{
			table_index = (*(byte const *)buffer ^ crc) & 0xFF;
			table_index = crc_globals.table[table_index];
			crc >>= 8;
			buffer = (byte const *)buffer + 1;
			crc ^= table_index;
		} while (--buffer_size);
	}

	*crc_reference = crc;
	return;
}

/* ---------- private code */
