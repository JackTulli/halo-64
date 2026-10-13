/*
IMA_ADPCM.C

symbols in this file:
001B8090 0160:
	_compress_ima_adpcm_audio_data (0000)
001B81F0 01a0:
	_decompress_ima_adpcm_audio_data (0000)
001B8390 0020:
	_byte_swap_bungie_ima_adpcm_header (0000)
002AAC6C 0018:
	??_C@_0BI@EDCLEDDM@bungie?5ima?5adpcm?5header?$AA@ (0000)
002AAC88 01a4:
	_step_size_adjustment_table (0000)
	_step_size_table (0040)
00316A7C 002c:
	_data_00316a7c (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "memory/byte_swapping.h"

#include "ima_adpcm.h"

/* ---------- constants */

enum
{
	STEP_SIZE_TABLE_COUNT = 89,
	STEP_SIZE_ADJUSTMENT_TABLE_COUNT = 16,
};

/* ---------- macros */

/* ---------- structures */

struct bungie_ima_adpcm_header_byte_swap_data
{
	byte_swap_code codes[6];
	struct byte_swap_definition definition;
};

typedef char verify_bungie_ima_adpcm_header_byte_swap_data_size[
	sizeof(struct bungie_ima_adpcm_header_byte_swap_data) == 0x2C ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

long const step_size_adjustment_table[STEP_SIZE_ADJUSTMENT_TABLE_COUNT] =
{
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8,
};

long const step_size_table[STEP_SIZE_TABLE_COUNT] =
{
	7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
	19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
	130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
	337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
	876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
	5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
	15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
};

struct bungie_ima_adpcm_header_byte_swap_data bungie_ima_adpcm_header_bs =
{
	{
		_begin_bs_array, 1,
		_4byte, _2byte, _2byte,
		_end_bs_array,
	},
	{
		"bungie ima adpcm header",
		sizeof(struct bungie_ima_adpcm_header),
		bungie_ima_adpcm_header_bs.codes,
		BYTE_SWAP_DEFINITION_SIGNATURE,
		FALSE,
	},
};

/* ---------- public code */

long compress_ima_adpcm_audio_data(
	short *input_samples,
	long input_sample_count,
	byte *output_data,
	long output_data_size)
{
	long result = (input_sample_count>>1) + (input_sample_count&1) + sizeof(struct bungie_ima_adpcm_header);

	if (output_data)
	{
		boolean write_high_nibble = TRUE;
		long original_sample_count = input_sample_count;
		struct bungie_ima_adpcm_header *header = (struct bungie_ima_adpcm_header *)output_data;
		long sample;
		short step_size_index = 0;

		sample = input_samples[0];

		output_data += sizeof(struct bungie_ima_adpcm_header);
		output_data_size -= sizeof(struct bungie_ima_adpcm_header);

		header->initial_sample = (short)sample;

		while (input_sample_count>0 && output_data_size)
		{
			long difference = *input_samples - sample;
			long step_size = step_size_table[step_size_index];
			long sample_difference;
			long temporary_step_size;
			char mask;
			char code;

			temporary_step_size = step_size;

			if (difference<0)
			{
				code = 8;
				difference = -difference;
			}
			else
			{
				code = 0;
			}

			mask = 4;
			do
			{
				if (difference>=temporary_step_size)
				{
					code |= mask;
					difference -= temporary_step_size;
				}
				mask >>= 1;
				temporary_step_size >>= 1;
			}
			while (mask);

			temporary_step_size = step_size_table[step_size_index];
			sample_difference = temporary_step_size >> 3;
			mask = 4;
			do
			{
				if (code&mask)
				{
					sample_difference += temporary_step_size;
				}
				mask >>= 1;
				temporary_step_size >>= 1;
			}
			while (mask);

			if (code&8)
			{
				sample_difference = -sample_difference;
			}

			sample = PIN(sample + sample_difference, SHORT_MIN, SHORT_MAX);

			step_size_index = (short)PIN(
				step_size_index + step_size_adjustment_table[code],
				0,
				STEP_SIZE_TABLE_COUNT - 1);

			if (write_high_nibble)
			{
				*output_data = (byte)(code<<4);
			}
			else
			{
				*output_data |= code;
			}

			write_high_nibble = !write_high_nibble;
			if (write_high_nibble)
			{
				output_data++;
				output_data_size--;
			}

			input_sample_count--;
			input_samples++;
		}

		header->sample_count = original_sample_count - input_sample_count;
		result = input_sample_count;
	}

	return result;
}

long decompress_ima_adpcm_audio_data(
	struct bungie_ima_adpcm_header const *input_header,
	long input_data_size,
	short *output_samples,
	long output_sample_count,
	struct bungie_ima_adpcm_state *state)
{
	long sample_count = input_header->sample_count;
	char const *input = (char const *)(input_header + 1);
	long result;
	long remaining_sample_count;
	long sample;
	short step_size_index;
	boolean read_high_nibble;

	input_data_size -= sizeof(struct bungie_ima_adpcm_header);
	result = sample_count * sizeof(short);

	if (output_samples)
	{
		if (state)
		{
			if (state->sample_count==0)
			{
				state->sample_count = sample_count;
				state->sample = input_header->initial_sample;
				state->step_size_index = 0;
			}

			sample = state->sample;
			step_size_index = state->step_size_index;
			input_data_size -= state->sample_index>>1;
			input += state->sample_index>>1;
			remaining_sample_count = state->sample_count - state->sample_index;
			read_high_nibble = !(state->sample_index&1);
		}
		else
		{
			sample = input_header->initial_sample;
			step_size_index = 0;
			read_high_nibble = TRUE;
			remaining_sample_count = sample_count;
		}

		while (remaining_sample_count && output_sample_count && input_data_size)
		{
			char code;
			long step_size = step_size_table[step_size_index];
			long temporary_step_size;
			long sample_difference = step_size>>3;
			char mask = 4;

			if (read_high_nibble)
			{
				code = (*input>>4)&0xF;
			}
			else
			{
				code = *input&0xF;
			}

			temporary_step_size = step_size;
			do
			{
				if (code&mask)
				{
					sample_difference += temporary_step_size;
				}
				mask >>= 1;
				temporary_step_size >>= 1;
			}
			while (mask);

			if (code&8)
			{
				sample_difference = -sample_difference;
			}

			sample = PIN(sample + sample_difference, SHORT_MIN, SHORT_MAX);

			step_size_index = (short)PIN(
				step_size_index + step_size_adjustment_table[code],
				0,
				STEP_SIZE_TABLE_COUNT - 1);

			*output_samples = (short)sample;
			read_high_nibble = !read_high_nibble;
			if (read_high_nibble)
			{
				input++;
				input_data_size--;
			}

			output_samples++;
			remaining_sample_count--;
			output_sample_count--;
			if (state)
			{
				state->sample_index++;
			}
		}

		if (state)
		{
			state->sample = (short)sample;
			state->step_size_index = step_size_index;
		}

		result = remaining_sample_count;
	}

	return result;
}

void byte_swap_bungie_ima_adpcm_header(
	struct bungie_ima_adpcm_header *header)
{
	byte_swap_data(&bungie_ima_adpcm_header_bs.definition, header, 1);

	return;
}

/* ---------- private code */
