/*
BITMAPS_QUANTITIZE.C

symbols in this file:
0006D5D0 0020:
	_bitmap_quantitize_read_row (0000)
0006D5F0 01d0:
	_row_dither (0000)
0006D7C0 0250:
	_bitmap_quantitize (0000)
00255338 0054:
	??_C@_0FE@EDPMNCJK@bits_per_channel?$FLchannel_index?$FN?$DO@ (0000)
0025538C 002c:
	??_C@_0CM@EPJOEJBJ@c?3?2halo?2SOURCE?2bitmaps?2bitmaps_q@ (0000)
002DCABC 0018:
	_bits_per_channel_r5g6b5 (0000)
	_bits_per_channel_a1r5g5b5 (0008)
	_bits_per_channel_a4r4g4b4 (0010)
0031C314 0008:
	_bits_per_channel_reversed (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmaps_quantitize_internal.h"

/* ---------- constants */

enum
{
	CHANNEL_COUNT = 4,
	CHANNEL_BITS = 8,
	_bitmap_type_2d = 0,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void bitmap_quantitize_read_row(
	short *destination,
	byte const *source,
	short pixel_count);
static void row_dither(
	short const *bits_per_channel,
	short const *minimum_error,
	short pixel_count,
	short *source,
	short *next_source,
	byte *destination);

/* ---------- globals */

short bits_per_channel_r5g6b5[CHANNEL_COUNT] = { 0, 5, 6, 5 };
short bits_per_channel_a1r5g5b5[CHANNEL_COUNT] = { 8, 5, 5, 5 };
short bits_per_channel_a4r4g4b4[CHANNEL_COUNT] = { 4, 4, 4, 4 };
static short bits_per_channel_reversed[CHANNEL_COUNT] = { 0 };

/* ---------- public code */

void bitmap_quantitize(
	struct bitmap_data *bitmap,
	short const *bits_per_channel)
{
	short minimum_error[CHANNEL_COUNT];
	short *source;
	short *next_source;
	short channel_index;
	short y;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 48, bitmap_verify(bitmap, TRUE));

	if (bits_per_channel && bitmap->type == _bitmap_type_2d)
	{
		source = match_malloc("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 52, bitmap->width * CHANNEL_COUNT * sizeof(short));
		next_source = match_malloc("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 53, bitmap->width * CHANNEL_COUNT * sizeof(short));

		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
		{
			match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 63, bits_per_channel[channel_index]>=0 && bits_per_channel[channel_index]<=CHANNEL_BITS);
		}
		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
			bits_per_channel_reversed[CHANNEL_COUNT - 1 - channel_index] = bits_per_channel[channel_index];

		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
			minimum_error[channel_index] = (short)((1 << (CHANNEL_BITS - bits_per_channel_reversed[channel_index])) * 0.25f);

		/* Original behavior: if exactly one allocation fails, the successful
		 * allocation is leaked. A corrected implementation would free either
		 * non-NULL buffer before leaving this block. */
		if (source && next_source)
		{
			bitmap_quantitize_read_row(source, bitmap->base_address, bitmap->width);

			for (y = 0; y < bitmap->height - 1; y++)
			{
				bitmap_quantitize_read_row(
					next_source,
					bitmap_2d_address(bitmap, 0, (short)(y + 1), 0),
					bitmap->width);
				row_dither(
					bits_per_channel_reversed,
					minimum_error,
					bitmap->width,
					source,
					next_source,
					bitmap_2d_address(bitmap, 0, y, 0));
				{
					short *swap = source;
					source = next_source;
					next_source = swap;
				}
			}

			row_dither(
				bits_per_channel_reversed,
				minimum_error,
				bitmap->width,
				source,
				NULL,
				bitmap_2d_address(bitmap, 0, (short)(bitmap->height - 1), 0));
			match_free("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 115, source);
			match_free("c:\\halo\\SOURCE\\bitmaps\\bitmaps_quantitize.c", 116, next_source);
		}
	}
	return;
}

/* ---------- private code */

static void bitmap_quantitize_read_row(
	short *destination,
	byte const *source,
	short pixel_count)
{
	long channel_count = pixel_count * CHANNEL_COUNT;

	while (channel_count-- > 0)
		*destination++ = *source++;
	return;
}

static void row_dither(
	short const *bits_per_channel,
	short const *minimum_error,
	short pixel_count,
	short *source,
	short *next_source,
	byte *destination)
{
	short pixel_index;

	for (pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		byte original[CHANNEL_COUNT];
		byte quantized[CHANNEL_COUNT];
		short channel_index;

		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
		{
			original[channel_index] = PIN(source[channel_index], 0, 255);
		}

		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
		{
			destination[channel_index] = quantized[channel_index] = bits_per_channel[channel_index] ?
				((original[channel_index] >> (CHANNEL_BITS - bits_per_channel[channel_index])) * 255) /
					((1 << bits_per_channel[channel_index]) - 1) :
				0;
		}

		for (channel_index = 0; channel_index < CHANNEL_COUNT; channel_index++)
		{
			short error = original[channel_index] - quantized[channel_index];

			if (pixel_index < pixel_count - 1 && source[CHANNEL_COUNT + channel_index] > minimum_error[channel_index])
				source[CHANNEL_COUNT + channel_index] += (error * 7) / 16;

			if (next_source)
			{
				if (pixel_index && next_source[-CHANNEL_COUNT + channel_index] > minimum_error[channel_index])
					next_source[-CHANNEL_COUNT + channel_index] += (error * 3) / 16;
				if (next_source[channel_index] > minimum_error[channel_index])
					next_source[channel_index] += (error * 5) / 16;
				if (pixel_index < pixel_count - 1 && next_source[CHANNEL_COUNT + channel_index] > minimum_error[channel_index])
					next_source[CHANNEL_COUNT + channel_index] += error / 16;
			}
		}

		source += CHANNEL_COUNT;
		destination += CHANNEL_COUNT;
		if (next_source)
			next_source += CHANNEL_COUNT;
	}
	return;
}
