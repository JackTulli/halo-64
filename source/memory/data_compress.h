/*
DATA_COMPRESS.H
*/

#ifndef __DATA_COMPRESS_H
#define __DATA_COMPRESS_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct compressed_data_header
{
	unsigned long decompressed_size;
};

/* ---------- prototypes/DATA_COMPRESS.C */

boolean data_compress(
	void const *source,
	unsigned long source_size,
	void *compressed_data,
	unsigned long *compressed_size,
	unsigned long compressed_capacity);

unsigned long data_decompressed_size(void const *compressed_data, unsigned long compressed_size);

boolean data_decompress(
	void const *compressed_data,
	unsigned long compressed_size,
	void *destination,
	unsigned long *destination_size,
	unsigned long destination_capacity);

#endif // __DATA_COMPRESS_H
