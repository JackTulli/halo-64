/*
DATA_COMPRESS.C

symbols in this file:
00109360 0070:
	_data_compress (0000)
001093D0 0040:
	_data_decompressed_size (0000)
00109410 0060:
	_data_decompress (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "data_compress.h"

#include "memory/byte_swapping.h"
#include "memory/zlib/zlib.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

boolean data_compress(
	void const *source,
	unsigned long source_size,
	void *compressed_data,
	unsigned long *compressed_size,
	unsigned long compressed_capacity)
{
	boolean result = FALSE;

	if (compressed_capacity >= sizeof(struct compressed_data_header))
	{
		struct compressed_data_header *header = compressed_data;

		header->decompressed_size = SWAP4(source_size);
		*compressed_size = compressed_capacity - sizeof(struct compressed_data_header);

		if (compress2(
			(Bytef *)(header + 1),
			compressed_size,
			(Bytef const *)source,
			source_size,
			Z_BEST_COMPRESSION) == Z_OK)
		{
			*compressed_size += sizeof(struct compressed_data_header);
			result = TRUE;
		}
	}

	return result;
}

unsigned long data_decompressed_size(
	void const *compressed_data,
	unsigned long compressed_size)
{
	unsigned long result = 0;

	if (compressed_size >= sizeof(struct compressed_data_header))
	{
		struct compressed_data_header const *header = compressed_data;
		result = SWAP4(header->decompressed_size);
	}

	return result;
}

boolean data_decompress(
	void const *compressed_data,
	unsigned long compressed_size,
	void *destination,
	unsigned long *destination_size,
	unsigned long destination_capacity)
{
	boolean result = FALSE;
	(void)destination_capacity;

	*destination_size = data_decompressed_size(compressed_data, compressed_size);
	if (uncompress(
		(Bytef *)destination,
		destination_size,
		(Bytef const *)compressed_data + sizeof(struct compressed_data_header),
		compressed_size) == Z_OK)
	{
		result = TRUE;
	}

	return result;
}

/* ---------- private code */
