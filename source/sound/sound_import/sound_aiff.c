/*
SOUND_AIFF.C

symbols in this file:
001B6370 0080:
	_sound_file_is_aiff (0000)
001B63F0 0220:
	_sound_file_aiff_info_get (0000)
001B6610 00d0:
	_sound_file_aiff_raw_data_get (0000)
001B66E0 0010:
	_sound_file_aiff_format (0000)
002AA8A8 0014:
	??_C@_0BE@PMHCOHBH@aiff?5format?5info?5p1?$AA@ (0000)
002AA8BC 000b:
	??_C@_0L@GBBDAFEP@aiff?5chunk?$AA@ (0000)
002AA8C8 0015:
	??_C@_0BF@EADPPKIJ@aiff?5container?5chunk?$AA@ (0000)
00316944 0088:
	_aiff_byte_swap_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "sound_import.h"

#include "memory/byte_swapping.h"
#include "tag_files/files.h"

/* ---------- constants */

enum
{
	AIFF_CONTAINER_HEADER_SIZE= 12,
	AIFF_CHUNK_HEADER_SIZE= 8,
	AIFF_COMMON_CHUNK_SIZE= 18,
	AIFC_COMMON_CHUNK_SIZE= 22,
	AIFF_SOUND_DATA_HEADER_SIZE= 8
};

/* ---------- macros */

/* ---------- structures */

struct aiff_container_chunk
{
	tag signature;
	long data_size;
	tag form_type;
};

struct aiff_chunk
{
	tag signature;
	long data_size;
};

struct aiff_common_chunk
{
	short channel_count;
	byte sample_frame_count[4];
	short significant_bits_per_sample;
	byte sample_rate[10];
	byte compression_type[4];
};

struct aiff_byte_swap_globals
{
	byte_swap_code container_chunk_codes[6];
	struct byte_swap_definition container_chunk_definition;
	byte_swap_code chunk_codes[5];
	struct byte_swap_definition chunk_definition;
	byte_swap_code common_chunk_codes[8];
	struct byte_swap_definition common_chunk_definition;
};

typedef char verify_aiff_byte_swap_globals_size[
	sizeof(struct aiff_byte_swap_globals) == 0x88 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

struct aiff_byte_swap_globals aiff_byte_swap_globals =
{
	{
		_begin_bs_array, 1,
		_4byte, _4byte, _4byte,
		_end_bs_array,
	},
	{
		"aiff container chunk",
		sizeof(struct aiff_container_chunk),
		aiff_byte_swap_globals.container_chunk_codes,
		BYTE_SWAP_DEFINITION_SIGNATURE,
		FALSE,
	},
	{
		_begin_bs_array, 1,
		_4byte, _4byte,
		_end_bs_array,
	},
	{
		"aiff chunk",
		sizeof(struct aiff_chunk),
		aiff_byte_swap_globals.chunk_codes,
		BYTE_SWAP_DEFINITION_SIGNATURE,
		FALSE,
	},
	{
		_begin_bs_array, 1,
		_2byte, _4byte, _2byte, 10, _4byte,
		_end_bs_array,
	},
	{
		"aiff format info p1",
		AIFC_COMMON_CHUNK_SIZE,
		aiff_byte_swap_globals.common_chunk_codes,
		BYTE_SWAP_DEFINITION_SIGNATURE,
		FALSE,
	},
};

#define aiff_container_chunk_byte_swap_definition aiff_byte_swap_globals.container_chunk_definition
#define aiff_chunk_byte_swap_definition aiff_byte_swap_globals.chunk_definition
#define aiff_common_chunk_byte_swap_definition aiff_byte_swap_globals.common_chunk_definition

/* ---------- public code */

boolean sound_file_is_aiff(
	struct file_reference const *file)
{
	struct aiff_container_chunk container;
	boolean result= FALSE;

	if (file_open((struct file_reference *)file, FLAG(_permission_read_bit)))
	{
		if (file_read_from_position(file, 0, sizeof(container), &container))
		{
			byte_swap_data(&aiff_container_chunk_byte_swap_definition, &container, 1);
			if (container.signature=='FORM' &&
				(container.form_type=='AIFF' || container.form_type=='AIFC'))
			{
				result= TRUE;
			}
		}
		file_close((struct file_reference *)file);
	}

	return result;
}

boolean sound_file_aiff_info_get(
	struct file_reference const *file,
	struct sound_file_info *info)
{
	struct aiff_chunk chunk;
	struct aiff_common_chunk common;
	boolean result= FALSE;
	long position= AIFF_CONTAINER_HEADER_SIZE;

	if (file_open((struct file_reference *)file, FLAG(_permission_read_bit)))
	{
		while (file_read_from_position(file, position, sizeof(chunk), &chunk))
		{
			byte_swap_data(&aiff_chunk_byte_swap_definition, &chunk, 1);
			if (chunk.signature=='COMM')
			{
				position+= sizeof(chunk);
				if (!file_read_from_position(file, position, sizeof(common), &common))
					break;

				{
					byte sample_rate_11025[10]= { 0x40, 0x0c, 0xac, 0x44, 0, 0, 0, 0, 0, 0 };
					byte sample_rate_22050[10]= { 0x40, 0x0d, 0xac, 0x44, 0, 0, 0, 0, 0, 0 };
					byte sample_rate_44100[10]= { 0x40, 0x0e, 0xac, 0x44, 0, 0, 0, 0, 0, 0 };

				byte_swap_data(&aiff_common_chunk_byte_swap_definition, &common, 1);
				if (csmemcmp(sample_rate_11025, common.sample_rate, sizeof(common.sample_rate))==0)
				{
					info->samples_per_second= 11025;
					info->significant_bits_per_sample= common.significant_bits_per_sample;
					info->channel_count= common.channel_count;
					if (chunk.data_size==AIFF_COMMON_CHUNK_SIZE || *(tag *)common.compression_type=='NONE')
						result= TRUE;
				}
				else if (csmemcmp(sample_rate_22050, common.sample_rate, sizeof(common.sample_rate))==0)
				{
					info->samples_per_second= 22050;
					info->significant_bits_per_sample= common.significant_bits_per_sample;
					info->channel_count= common.channel_count;
					if (chunk.data_size==AIFF_COMMON_CHUNK_SIZE || *(tag *)common.compression_type=='NONE')
						result= TRUE;
				}
				else if (csmemcmp(sample_rate_44100, common.sample_rate, sizeof(common.sample_rate))==0)
				{
					info->samples_per_second= 44100;
					info->significant_bits_per_sample= common.significant_bits_per_sample;
					info->channel_count= common.channel_count;
					if (chunk.data_size==AIFF_COMMON_CHUNK_SIZE || *(tag *)common.compression_type=='NONE')
						result= TRUE;
				}
				else
					info->samples_per_second= NONE;
				}
				break;
			}

			position+= sizeof(chunk)+(chunk.data_size&1 ? chunk.data_size+1 : chunk.data_size);
		}
		file_close((struct file_reference *)file);
	}

	return result;
}

boolean sound_file_aiff_raw_data_get(
	struct file_reference const *file,
	long *size,
	void *data)
{
	struct aiff_chunk chunk;
	boolean result= FALSE;
	long position= AIFF_CONTAINER_HEADER_SIZE;

	if (file_open((struct file_reference *)file, FLAG(_permission_read_bit)))
	{
		while (file_read_from_position(file, position, sizeof(chunk), &chunk))
		{
			byte_swap_data(&aiff_chunk_byte_swap_definition, &chunk, 1);
			if (chunk.signature=='SSND')
			{
				*size= chunk.data_size-AIFF_SOUND_DATA_HEADER_SIZE;
				position+= sizeof(chunk)+AIFF_SOUND_DATA_HEADER_SIZE;
				if (file_read_from_position(file, position, *size, data))
					result= TRUE;
				break;
			}

			position+= sizeof(chunk)+(chunk.data_size&1 ? chunk.data_size+1 : chunk.data_size);
		}
		file_close((struct file_reference *)file);
	}

	if (result)
		byte_swap_memory(data, *size>>1, _2byte);

	return result;
}

void sound_file_aiff_format(
	struct sound_file_info const *info,
	long *size,
	void *data)
{
	return;
}

/* ---------- private code */
