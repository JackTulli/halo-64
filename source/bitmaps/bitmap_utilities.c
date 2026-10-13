/*
BITMAP_UTILITIES.C

symbols in this file:
00065B80 0030:
	_bitmap_fill (0000)
00065BB0 0050:
	_bitmap_alpha_to_rgb (0000)
00065C00 0190:
	_bitmap_clone (0000)
00065D90 0290:
	_bitmap_2d_shrink (0000)
00066020 0320:
	_bitmap_3d_shrink (0000)
00066340 0190:
	_bitmap_cm_shrink (0000)
000664D0 0190:
	_bitmap_fade (0000)
00066660 0470:
	_bitmap_2d_smooth (0000)
00066AD0 0720:
	_bitmap_3d_smooth (0000)
000671F0 00b0:
	_bitmap_cm_smooth (0000)
000672A0 0480:
	_bitmap_2d_sharpen (0000)
00067720 00d0:
	_bitmap_3d_sharpen (0000)
000677F0 00d0:
	_bitmap_cm_sharpen (0000)
000678C0 0230:
	_bitmap_2d_alpha_bleed (0000)
00067AF0 0110:
	_bitmap_3d_alpha_bleed (0000)
00067C00 00a0:
	_bitmap_cm_alpha_bleed (0000)
00067CA0 00b0:
	_bitmap_cm_height_map (0000)
00067D50 0200:
	_bitmap_2d_compress_to_mipmap (0000)
00067F50 02d0:
	_bitmap_3d_compress_to_mipmap (0000)
00068220 02c0:
	_bitmap_cm_compress_to_mipmap (0000)
000684E0 0370:
	_bitmap_2d_uncompress_from_mipmap (0000)
00068850 02c0:
	_bitmap_3d_uncompress_from_mipmap (0000)
00068B10 02b0:
	_bitmap_cm_uncompress_from_mipmap (0000)
00068DC0 0030:
	_real_rgb_color_brightness (0000)
00068DF0 01f0:
	_rgb_color_to_hsv_color (0000)
00068FE0 01e0:
	_hsv_color_to_rgb_color (0000)
000691C0 0190:
	_real_rgb_color_to_real_hsv_color (0000)
00069350 0190:
	_real_hsv_color_to_real_rgb_color (0000)
000694E0 0060:
	_argb_color_to_real_argb_color (0000)
00069540 0050:
	_rgb_color_to_real_rgb_color (0000)
00069590 0090:
	_pixel32_to_real_argb_color (0000)
00069620 0070:
	_pixel32_to_real_rgb_color (0000)
00069690 00c0:
	_valid_real_rgb_color (0000)
00069750 00d0:
	_bitmap_shrink (0000)
00069820 0160:
	_bitmap_smooth (0000)
00069980 0160:
	_bitmap_sharpen (0000)
00069AE0 00a0:
	_bitmap_alpha_bleed (0000)
00069B80 0430:
	_bitmap_2d_height_map (0000)
00069FB0 0110:
	_bitmap_3d_height_map (0000)
0006A0C0 0260:
	_bitmap_2d_vector_map (0000)
0006A320 00e0:
	_bitmap_3d_vector_map (0000)
0006A400 00d0:
	_bitmap_cm_vector_map (0000)
0006A4D0 0210:
	_bitmap_compress_to_mipmap (0000)
0006A6E0 0200:
	_bitmap_uncompress_from_mipmap (0000)
0006A8E0 0220:
	_rgb_colors_interpolate (0000)
0006AB00 0160:
	_rgb_colors_interpolate_and_scale (0000)
0006AC60 00d0:
	_bitmap_height_map (0000)
0006AD30 0090:
	_bitmap_vector_map (0000)
00253D04 0004:
	_oo_unsigned_short_max (0000)
00253D08 003b:
	??_C@_0DL@NLJHJPHM@bitmap_get_pixel_data_size?$CIclone@ (0000)
00253D44 002e:
	??_C@_0CO@GFGADFMM@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5tem@ (0000)
00253D74 001c:
	??_C@_0BM@JIFNGHMH@source_bitmap?9?$DObase_address?$AA@ (0000)
00253D90 000e:
	??_C@_0O@LLKKCKIH@source_bitmap?$AA@ (0000)
00253DA0 002a:
	??_C@_0CK@FAKBAENJ@c?3?2halo?2SOURCE?2bitmaps?2bitmap_ut@ (0000)
00253DCC 0008:
	??_C@_07HJGGGODH@scale?$DO1?$AA@ (0000)
00253DD4 0025:
	??_C@_0CF@KNCPFPIC@source_bitmap?9?$DOtype?$DN?$DN_bitmap_typ@ (0000)
00253DFC 0025:
	??_C@_0CF@KMONDFLF@source_bitmap?9?$DOtype?$DN?$DN_bitmap_typ@ (0000)
00253E24 002b:
	??_C@_0CL@EEMHJEBC@source_bitmap?9?$DOtype?$DN?$DN_bitmap_typ@ (0000)
00253E50 0004:
	__real@43800000 (0000)
00253E58 0046:
	??_C@_0EG@HMKKNAIC@?$CD?$CD?$CD?5WARNING?5tried?5to?5smooth?5a?5bi@ (0000)
00253EA0 002e:
	??_C@_0CO@OJGKHOJB@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5tem@ (0000)
00253ED0 0014:
	??_C@_0BE@CCCNMADO@filter_coefficients?$AA@ (0000)
00253EE4 001e:
	??_C@_0BO@HBJILGDD@bitmap?9?$DOtype?$DN?$DN_bitmap_type_2d?$AA@ (0000)
00253F04 001e:
	??_C@_0BO@HAFKNMAE@bitmap?9?$DOtype?$DN?$DN_bitmap_type_3d?$AA@ (0000)
00253F24 0027:
	??_C@_0CH@PCFOMNKE@?$CD?$CD?$CD?5WARNING?5tried?5to?5smooth?5a?5cu@ (0000)
00253F4C 0024:
	??_C@_0CE@LEKDELBL@bitmap?9?$DOtype?$DN?$DN_bitmap_type_cube_@ (0000)
00253F70 000f:
	??_C@_0P@BOMHLMLK@negative_table?$AA@ (0000)
00253F80 000f:
	??_C@_0P@BHFDFLKJ@positive_table?$AA@ (0000)
00253F90 0029:
	??_C@_0CJ@IOADGLLE@?$CD?$CD?$CD?5WARNING?5tried?5to?5sharpen?5a?53@ (0000)
00253FBC 0028:
	??_C@_0CI@MDGCOAND@?$CD?$CD?$CD?5WARNING?5tried?5to?5sharpen?5a?5c@ (0000)
00253FE4 0009:
	??_C@_08HMMJDEFO@passes?$DO0?$AA@ (0000)
00253FF0 0037:
	??_C@_0DH@PFEBJHLO@?$CD?$CD?$CD?5WARNING?5tried?5to?5alpha?9bleed@ (0000)
00254028 0036:
	??_C@_0DG@LNAIIMEN@?$CD?$CD?$CD?5WARNING?5tried?5to?5use?5a?5cube?5@ (0000)
00254060 0011:
	??_C@_0BB@IHCHECOA@bump_height?$DO0?40f?$AA@ (0000)
00254074 003d:
	??_C@_0DN@DPHCPDGM@TEST_FLAG?$CIdestination_bitmap?9?$DOfl@ (0000)
002540B8 0053:
	??_C@_0FD@ENNJNLHL@MAX?$CI1?0?5destination_bitmap?9?$DOdepth@ (0000)
00254110 0054:
	??_C@_0FE@IAGIFAMH@MAX?$CI1?0?5destination_bitmap?9?$DOheigh@ (0000)
00254168 0053:
	??_C@_0FD@KJCBOGPG@MAX?$CI1?0?5destination_bitmap?9?$DOwidth@ (0000)
002541BC 002a:
	??_C@_0CK@NKHGKAJF@destination_bitmap?9?$DOtype?$DN?$DN_bitma@ (0000)
002541E8 002a:
	??_C@_0CK@NLLEMKKC@destination_bitmap?9?$DOtype?$DN?$DN_bitma@ (0000)
00254214 0030:
	??_C@_0DA@HHMLFPEA@destination_bitmap?9?$DOtype?$DN?$DN_bitma@ (0000)
00254244 0038:
	??_C@_0DI@JCJJEBL@TEST_FLAG?$CIsource_bitmap?9?$DOflags?0?5@ (0000)
00254280 004e:
	??_C@_0EO@HMAHIFHF@MAX?$CI1?0?5source_bitmap?9?$DOdepth?5?$DO?$DOso@ (0000)
002542D0 004f:
	??_C@_0EP@GHOBKDJO@MAX?$CI1?0?5source_bitmap?9?$DOheight?$DO?$DOso@ (0000)
00254320 004e:
	??_C@_0EO@JIPPLIPI@MAX?$CI1?0?5source_bitmap?9?$DOwidth?5?$DO?$DOso@ (0000)
00254370 0004:
	__real@3e991687 (0000)
00254374 0004:
	__real@3f1645a2 (0000)
00254378 0004:
	__real@3de978d5 (0000)
0025437C 0004:
	__real@477fff00 (0000)
00254380 0004:
	__real@47800000 (0000)
00254384 0004:
	__real@3e2aaaab (0000)
00254388 0016:
	??_C@_0BG@EIAAKENO@rgb?$CB?$DN?$CIrgb_color?5?$CK?$CJhsv?$AA@ (0000)
002543A0 0004:
	??_C@_03PPLPHHG@hsv?$AA@ (0000)
002543A4 0004:
	__real@37800080 (0000)
002543A8 0004:
	??_C@_03FNOJMHE@rgb?$AA@ (0000)
002543AC 0004:
	__real@37800000 (0000)
002543B0 001b:
	??_C@_0BL@CJFFBABE@rgb?$CB?$DN?$CIreal_rgb_color?5?$CK?$CJhsv?$AA@ (0000)
002543CC 0021:
	??_C@_0CB@IFIPCKKG@?$CD?$CD?$CD?5ERROR?5unupported?5bitmap?5type@ (0000)
002543F0 0028:
	??_C@_0CI@JOAHHOB@filter_size?$DM?$DN?$CIfloat?$CJMAXIMUM_FILT@ (0000)
00254418 0004:
	__real@42ff0000 (0000)
0025441C 0004:
	__real@3c008081 (0000)
00254420 000b:
	??_C@_0L@DEFNOJIM@rgb_result?$AA@ (0000)
0025442C 0010:
	??_C@_0BA@JANHNNE@rgb_upper_bound?$AA@ (0000)
0025443C 002c:
	??_C@_0CM@MJIBIDD@?$CFs?3?5assert_valid_real_rgb_color?$CI@ (0000)
00254468 0010:
	??_C@_0BA@EHGOIBMN@rgb_lower_bound?$AA@ (0000)
00254478 000a:
	??_C@_09BIGBFPOL@rgb_scale?$AA@ (0000)
00254488 0041:
	??_C@_0EB@GJHMFKH@?$CD?$CD?$CD?5WARNING?5importing?5special?9ef@ (0000)
0031BF00 0200:
	_bitmap_sharpen_negative_table (0000)
0031C100 0200:
	_bitmap_sharpen_positive_table (0000)
0031C300 0014:
	_bitmap_smooth_filter_coefficients (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "bitmaps/bitmaps_internal.h"
#include "bitmaps/bitmap_utilities.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/s3tc/s3tc.h"
#include "cseries/errors.h"

/* ---------- constants */

enum
{
	_bitmap_type_2d,
	_bitmap_type_3d,
	_bitmap_type_cube_map,
	NUMBER_OF_BITMAP_TYPES,
};

enum
{
	_bitmap_format_a8,
	_bitmap_format_y8,
	_bitmap_format_ay8,
	_bitmap_format_a8y8,
	_bitmap_format_unused1,
	_bitmap_format_unused2,
	_bitmap_format_r5g6b5,
	_bitmap_format_unused3,
	_bitmap_format_a1r5g5b5,
	_bitmap_format_a4r4g4b4,
	_bitmap_format_x8r8g8b8,
	_bitmap_format_a8r8g8b8,
	_bitmap_format_unused4,
	_bitmap_format_unused5,
	_bitmap_format_dxt1,
	_bitmap_format_dxt3,
	_bitmap_format_dxt5,
	_bitmap_format_p8_bump,
	NUMBER_OF_BITMAP_FORMATS,

	FIRST_COMPRESSED_BITMAP_FORMAT = _bitmap_format_dxt1,
	LAST_COMPRESSED_BITMAP_FORMAT = _bitmap_format_dxt5,
};

enum
{
	_bitmap_has_power_of_two_dimensions_bit,
	_bitmap_compressed_bit,
	_bitmap_palettized_bit,
	_bitmap_swizzled_bit,
	_bitmap_linear_bit,
	_bitmap_v16u16_bit,
	_bitmap_allocated_bit,
	_bitmap_cached_bit,
	NUMBER_OF_BITMAP_FLAGS,
};

enum
{
	MAXIMUM_FILTER_SIZE = 10,
};

enum
{
	_rgb_color_interpolation_hsv_bit = 0,
	_rgb_color_interpolation_hsv_reverse_bit,
};

/* ---------- macros */

#define match_assert_valid_real_rgb_color(file, line, rgb) \
match_vassert( \
	file, \
	line, \
	valid_real_rgb_color(rgb), \
	csprintf( \
		temporary, \
		"%s: assert_valid_real_rgb_color(%f, %f, %f)", \
		#rgb, (*rgb).red, (*rgb).green, (*rgb).blue \
	) \
)

/* ---------- structures */

/* ---------- prototypes */

static struct bitmap_data *bitmap_2d_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels);
static struct bitmap_data *bitmap_3d_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels);
static struct bitmap_data *bitmap_cm_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels);
static void bitmap_3d_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients);
static void bitmap_cm_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients);
static void bitmap_2d_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table);
static void bitmap_3d_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table);
static void bitmap_cm_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table);
static void bitmap_2d_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes);
static void bitmap_3d_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes);
static void bitmap_cm_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes);
static void bitmap_2d_height_map(
	struct bitmap_data *bitmap,
	real bump_height);
static void bitmap_3d_height_map(
	struct bitmap_data *bitmap,
	real bump_height);
static void bitmap_cm_height_map(
	struct bitmap_data *bitmap,
	real bump_height);
static void bitmap_2d_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color);
static void bitmap_3d_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color);
static void bitmap_cm_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color);
static void bitmap_2d_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index);
static void bitmap_3d_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index);
static void bitmap_cm_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index);

static void bitmap_2d_vector_map(
	struct bitmap_data *bitmap);
static void bitmap_3d_vector_map(
	struct bitmap_data *bitmap);
static void bitmap_cm_vector_map(
	struct bitmap_data *bitmap);

/* ---------- globals */

real const oo_unsigned_short_max = 1.0f / UNSIGNED_SHORT_MAX;
short bitmap_sharpen_negative_table[256]= {0};
short bitmap_sharpen_positive_table[256]= {0};
short bitmap_smooth_filter_coefficients[MAXIMUM_FILTER_SIZE]= {0};

/* ---------- public code */

void bitmap_fill(
	struct bitmap_data *bitmap,
	pixel32 fill_color)
{
	pixel32 *pixels = bitmap_2d_address(bitmap, 0, 0, 0);
	long pixel_count = bitmap_get_pixel_count(bitmap);

	while (pixel_count-- > 0)
		*pixels++ = fill_color;
	return;
}

void bitmap_alpha_to_rgb(
	struct bitmap_data *bitmap)
{
	pixel32 *pixels = bitmap_2d_address(bitmap, 0, 0, 0);
	long pixel_count = bitmap_get_pixel_count(bitmap);

	while (pixel_count-- > 0)
	{
		/* byte is a character type, so this representation access is defined. */
		byte alpha = ((byte *)pixels)[3];
		unsigned long expanded = alpha;
		expanded = (expanded << 8) | alpha;
		expanded = (expanded << 8) | alpha;
		expanded = (expanded << 8) | alpha;
		*pixels++ = expanded;
	}
	return;
}

struct bitmap_data *bitmap_clone(
	struct bitmap_data *source_bitmap)
{
	struct bitmap_data *cloned_bitmap = NULL;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x67, source_bitmap);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x68, source_bitmap->base_address);

	switch (source_bitmap->type)
	{
	case _bitmap_type_2d:
		cloned_bitmap = bitmap_2d_new(
			source_bitmap->width,
			source_bitmap->height,
			source_bitmap->mipmap_count,
			source_bitmap->format);
		break;

	case _bitmap_type_3d:
		cloned_bitmap = bitmap_3d_new(
			source_bitmap->width,
			source_bitmap->height,
			source_bitmap->depth,
			source_bitmap->mipmap_count,
			source_bitmap->format);
		break;

	case _bitmap_type_cube_map:
		cloned_bitmap = bitmap_cube_map_new(
			source_bitmap->width,
			source_bitmap->mipmap_count,
			source_bitmap->format);
		break;

	default:
		match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x83, FALSE, "### ERROR unsupported bitmap type");
		break;
	}

	if (cloned_bitmap && cloned_bitmap->base_address)
	{
		void *source_pixels = bitmap_mipmap_address(source_bitmap, 0);
		void *cloned_pixels = bitmap_mipmap_address(cloned_bitmap, 0);
		long pixel_data_size = bitmap_get_pixel_data_size(source_bitmap);

		match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x8D,
			bitmap_get_pixel_data_size(cloned_bitmap)==pixel_data_size);

		csmemcpy(cloned_pixels, source_pixels, pixel_data_size);
		cloned_bitmap->flags = source_bitmap->flags;
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	return cloned_bitmap;
}

struct bitmap_data *bitmap_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels)
{
	struct bitmap_data *destination_bitmap = NULL;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0xE1, bitmap_verify(source_bitmap, TRUE));

	if (scale <= 1)
	{
		destination_bitmap = bitmap_clone(source_bitmap);
	}
	else
	{
		switch (source_bitmap->type)
		{
		case _bitmap_type_2d:
			destination_bitmap = bitmap_2d_shrink(
				source_bitmap,
				scale,
				alpha_bias,
				ignore_transparent_pixels);
			break;

		case _bitmap_type_3d:
			destination_bitmap = bitmap_3d_shrink(
				source_bitmap,
				scale,
				alpha_bias,
				ignore_transparent_pixels);
			break;

		case _bitmap_type_cube_map:
			destination_bitmap = bitmap_cm_shrink(
				source_bitmap,
				scale,
				alpha_bias,
				ignore_transparent_pixels);
			break;

		default:
			match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0xF3, FALSE, "### ERROR unupported bitmap type");
			break;
		}
	}

	return destination_bitmap;
}

static struct bitmap_data *bitmap_2d_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels)
{
	struct bitmap_data *destination_bitmap;
	short x_step, y_step;
	short width, height;
	short x, y;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x105, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x106, source_bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x107, scale>1);

	x_step = MIN(scale, source_bitmap->width);
	y_step = MIN(scale, source_bitmap->height);
	width = source_bitmap->width / x_step;
	height = source_bitmap->height / y_step;

	destination_bitmap = bitmap_2d_new(width, height, 0, _bitmap_format_a8r8g8b8);
	if (destination_bitmap && destination_bitmap->base_address)
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				long alpha = 0;
				long red = 0;
				long green = 0;
				long blue = 0;
				long count = 0;
				short i, j;
				pixel32 *destination_pixel = bitmap_2d_address(
					destination_bitmap, x, y, 0);

				for (j = 0; j < y_step; j++)
				{
					for (i = 0; i < x_step; i++)
					{
						pixel32 pixel = *(pixel32 *)bitmap_2d_address(
							source_bitmap, x * x_step + i, y * y_step + j, 0);
						long pixel_alpha = pixel >> 24;

						if (pixel_alpha || !ignore_transparent_pixels)
						{
							alpha += pixel_alpha;
							red += (pixel >> 16) & 0xFF;
							green += (pixel >> 8) & 0xFF;
							blue += pixel & 0xFF;
							count++;
						}
					}
				}

				if (count)
				{
					*destination_pixel =
						(PIN((alpha + count / 2) / count + alpha_bias, 0, 255) << 24) |
						(((red + count / 2) / count) << 16) |
						(((green + count / 2) / count) << 8) |
						((blue + count / 2) / count);
				}
				else
				{
					*destination_pixel = 0;
				}
			}
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	return destination_bitmap;
}

static struct bitmap_data *bitmap_3d_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels)
{
	struct bitmap_data *destination_bitmap;
	short x_step, y_step, z_step;
	short width, height, depth;
	short x, y, z;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x15D, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x15E, source_bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x15F, scale>1);

	x_step = MIN(scale, source_bitmap->width);
	y_step = MIN(scale, source_bitmap->height);
	z_step = MIN(scale, (short)source_bitmap->depth);
	width = source_bitmap->width / x_step;
	height = source_bitmap->height / y_step;
	depth = (short)source_bitmap->depth / z_step;

	destination_bitmap = bitmap_3d_new(width, height, depth, 0, _bitmap_format_a8r8g8b8);
	if (destination_bitmap && destination_bitmap->base_address)
	{
		for (z = 0; z < depth; z++)
		{
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					long alpha = 0;
					long red = 0;
					long green = 0;
					long blue = 0;
					long count = 0;
					short i, j, k;
					pixel32 *destination_pixel = bitmap_3d_address(
						destination_bitmap, x, y, z, 0);

					for (k = 0; k < z_step; k++)
					{
						for (j = 0; j < y_step; j++)
						{
							for (i = 0; i < x_step; i++)
							{
								pixel32 pixel = *(pixel32 *)bitmap_3d_address(
									source_bitmap, x * x_step + i, y * y_step + j, z * z_step + k, 0);
								long pixel_alpha = pixel >> 24;

								if (pixel_alpha || !ignore_transparent_pixels)
								{
									alpha += pixel_alpha;
									red += (pixel >> 16) & 0xFF;
									green += (pixel >> 8) & 0xFF;
									blue += pixel & 0xFF;
									count++;
								}
							}
						}
					}

					if (count)
					{
						*destination_pixel =
							(PIN((alpha + count / 2) / count + alpha_bias, 0, 255) << 24) |
							(((red + count / 2) / count) << 16) |
							(((green + count / 2) / count) << 8) |
							((blue + count / 2) / count);
					}
					else
					{
						*destination_pixel = 0;
					}
				}
			}
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	return destination_bitmap;
}

static struct bitmap_data *bitmap_cm_shrink(
	struct bitmap_data *source_bitmap,
	short scale,
	short alpha_bias,
	boolean ignore_transparent_pixels)
{
	struct bitmap_data *destination_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x1BB, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x1BC, source_bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x1BD, scale>1);

	destination_bitmap = bitmap_cube_map_new(
		source_bitmap->width / MIN(scale, source_bitmap->width),
		0,
		_bitmap_format_a8r8g8b8);
	if (destination_bitmap && destination_bitmap->base_address)
	{
		struct bitmap_data *face_bitmap = bitmap_2d_new(
			source_bitmap->width,
			source_bitmap->height,
			0,
			_bitmap_format_a8r8g8b8);

		if (face_bitmap && face_bitmap->base_address)
		{
			short face_index;

			for (face_index = 0; face_index < NUMBER_OF_FACES_PER_CUBE; face_index++)
			{
				struct bitmap_data *shrunk_bitmap;

				bitmap_cube_map_face_extract(source_bitmap, 0, face_index, face_bitmap);
				shrunk_bitmap = bitmap_2d_shrink(
					face_bitmap,
					scale,
					alpha_bias,
					ignore_transparent_pixels);
				if (shrunk_bitmap && shrunk_bitmap->base_address)
				{
					bitmap_cube_map_face_insert(
						shrunk_bitmap, destination_bitmap, 0, face_index);
				}

				bitmap_delete(shrunk_bitmap);
			}
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate temporary bitmap");
		}

		bitmap_delete(face_bitmap);
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	return destination_bitmap;
}

void bitmap_fade(
	struct bitmap_data *bitmap,
	pixel32 fade_color,
	real fade_amount)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x1F5,
		bitmap_verify(bitmap, TRUE));

	if (fade_amount > 0.f)
	{
		long alpha = (long)floor(PIN(fade_amount, 0.f, 1.f) * 256.f + 0.5f);
		long inverse_alpha = 256 - alpha;
		long faded_alpha = ((fade_color >> 24) & 0xFF) * alpha;
		long faded_red = ((fade_color >> 16) & 0xFF) * alpha;
		long faded_green = ((fade_color >> 8) & 0xFF) * alpha;
		long faded_blue = (fade_color & 0xFF) * alpha;
		pixel32 *pixels = bitmap_mipmap_address(bitmap, 0);
		long pixel_count = bitmap_get_pixel_count(bitmap);
		long index;

		for (index = 0; index < pixel_count; index++)
		{
			pixel32 pixel = pixels[index];

			pixels[index] =
				((((pixel >> 24) * inverse_alpha + faded_alpha + 0x7F) >> 8) << 24) |
				(((((pixel >> 16) & 0xFF) * inverse_alpha + faded_red + 0x7F) >> 8) << 16) |
				(((((pixel >> 8) & 0xFF) * inverse_alpha + faded_green + 0x7F) >> 8) << 8) |
				((((pixel & 0xFF) * inverse_alpha + faded_blue + 0x7F) >> 8));
		}
	}

	return;
}

static void bitmap_2d_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients);

void bitmap_smooth(
	struct bitmap_data *bitmap,
	real filter_size)
{
	short integral_filter_size = (short)floor(filter_size);

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x21E,
		bitmap_verify(bitmap, TRUE));
	match_vassert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
		0x21F,
		filter_size <= (real)MAXIMUM_FILTER_SIZE,
		"filter_size<=(float)MAXIMUM_FILTER_SIZE");

	if (integral_filter_size > 0.f)
	{
		short pass;

		csmemset(
			bitmap_smooth_filter_coefficients,
			0,
			sizeof(bitmap_smooth_filter_coefficients));

		for (pass = 2 * integral_filter_size; pass >= 0; pass--)
		{
			short coefficient_index;

			for (coefficient_index = MAXIMUM_FILTER_SIZE - 1;
				coefficient_index > 0;
				coefficient_index--)
			{
				bitmap_smooth_filter_coefficients[coefficient_index] +=
					bitmap_smooth_filter_coefficients[coefficient_index - 1];
			}

			bitmap_smooth_filter_coefficients[0] = 1;
		}

		switch (bitmap->type)
		{
		case _bitmap_type_2d:
			bitmap_2d_smooth(
				bitmap,
				integral_filter_size,
				bitmap_smooth_filter_coefficients);
			break;

		case _bitmap_type_3d:
			bitmap_3d_smooth(
				bitmap,
				integral_filter_size,
				bitmap_smooth_filter_coefficients);
			break;

		case _bitmap_type_cube_map:
			bitmap_cm_smooth(
				bitmap,
				integral_filter_size,
				bitmap_smooth_filter_coefficients);
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
				0x244,
				FALSE,
				"### ERROR unsupported bitmap type");
			break;
		}
	}

	return;
}

static void bitmap_2d_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients)
{
	long pixel_data_size;
	pixel32 *source_pixels;
	pixel32 *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x250, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x251, bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x252, filter_coefficients);

	if (bitmap->width >= filter_size && bitmap->height >= filter_size)
	{
		short x, y;

		pixel_data_size = bitmap_get_pixel_data_size(bitmap);
		source_pixels = bitmap_mipmap_address(bitmap, 0);
		temporary_pixels = match_malloc(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x25D,
			pixel_data_size);
		if (temporary_pixels)
		{
			for (y = 0; y < bitmap->height; y++)
			{
				for (x = 0; x < bitmap->width; x++)
				{
					long alpha = 0;
					long red = 0;
					long green = 0;
					long blue = 0;
					short filter_index;

					for (filter_index = -filter_size;
						filter_index <= filter_size;
						filter_index++)
					{
						short source_x =
							(short)((bitmap->width + filter_index + x) % bitmap->width);
						pixel32 pixel = source_pixels[y * bitmap->width + source_x];
						short coefficient = filter_coefficients[filter_index + filter_size];

						alpha += (pixel >> 24) * coefficient;
						red += ((pixel >> 16) & 0xFF) * coefficient;
						green += ((pixel >> 8) & 0xFF) * coefficient;
						blue += (pixel & 0xFF) * coefficient;
					}

					temporary_pixels[y * bitmap->width + x] =
						((((alpha + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 24) |
						(((red + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 16) |
						(((green + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 8) |
						((blue + (1 << (2 * filter_size - 1))) >> (2 * filter_size)));
				}
			}

			for (y = 0; y < bitmap->height; y++)
			{
				for (x = 0; x < bitmap->width; x++)
				{
					long alpha = 0;
					long red = 0;
					long green = 0;
					long blue = 0;
					short filter_index;

					for (filter_index = -filter_size;
						filter_index <= filter_size;
						filter_index++)
					{
						short source_y =
							(short)((bitmap->height + filter_index + y) % bitmap->height);
						pixel32 pixel = temporary_pixels[source_y * bitmap->width + x];
						short coefficient = filter_coefficients[filter_index + filter_size];

						alpha += (pixel >> 24) * coefficient;
						red += ((pixel >> 16) & 0xFF) * coefficient;
						green += ((pixel >> 8) & 0xFF) * coefficient;
						blue += (pixel & 0xFF) * coefficient;
					}

					source_pixels[y * bitmap->width + x] =
						((((alpha + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 24) |
						(((red + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 16) |
						(((green + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 8) |
						((blue + (1 << (2 * filter_size - 1))) >> (2 * filter_size)));
				}
			}

			match_free(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
				0x2A5,
				temporary_pixels);
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate temporary buffer");
		}
	}
	else
	{
		/* BUG (preserved): January supplies an unused newline vararg. */
		fprintf(
			stdout,
			"### WARNING tried to smooth a bitmap with a filter which is too large",
			"\r\n");
		fflush(stdout);
	}

	return;
}

static void bitmap_3d_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients)
{
	long pixel_data_size;
	pixel32 *source_pixels;
	pixel32 *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x2BA, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x2BB, bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x2BC, filter_coefficients);

	if (bitmap->width >= filter_size &&
		bitmap->height >= filter_size &&
		bitmap->depth >= filter_size)
	{
		short x, y, z;

		pixel_data_size = bitmap_get_pixel_data_size(bitmap);
		source_pixels = bitmap_mipmap_address(bitmap, 0);
		temporary_pixels = match_malloc(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x2C7,
			pixel_data_size);
		if (temporary_pixels)
		{
			for (z = 0; z < bitmap->depth; z++)
			{
				for (y = 0; y < bitmap->height; y++)
				{
					for (x = 0; x < bitmap->width; x++)
					{
						long alpha = 0;
						long red = 0;
						long green = 0;
						long blue = 0;
						short filter_index;

						for (filter_index = -filter_size;
							filter_index <= filter_size;
							filter_index++)
						{
							short source_x =
								(short)((bitmap->width + filter_index + x) % bitmap->width);
							pixel32 pixel = source_pixels[
								(bitmap->height * z + y) * bitmap->width + source_x];
							short coefficient = filter_coefficients[filter_index + filter_size];

							alpha += (pixel >> 24) * coefficient;
							red += ((pixel >> 16) & 0xFF) * coefficient;
							green += ((pixel >> 8) & 0xFF) * coefficient;
							blue += (pixel & 0xFF) * coefficient;
						}

						temporary_pixels[(bitmap->height * z + y) * bitmap->width + x] =
							((((alpha + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 24) |
							(((red + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 16) |
							(((green + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 8) |
							((blue + (1 << (2 * filter_size - 1))) >> (2 * filter_size)));
					}
				}
			}

			for (z = 0; z < bitmap->depth; z++)
			{
				for (y = 0; y < bitmap->height; y++)
				{
					for (x = 0; x < bitmap->width; x++)
					{
						long alpha = 0;
						long red = 0;
						long green = 0;
						long blue = 0;
						short filter_index;

						for (filter_index = -filter_size;
							filter_index <= filter_size;
							filter_index++)
						{
							short source_y =
								(short)((bitmap->height + filter_index + y) % bitmap->height);
							pixel32 pixel = temporary_pixels[
								(bitmap->height * z + source_y) * bitmap->width + x];
							short coefficient = filter_coefficients[filter_index + filter_size];

							alpha += (pixel >> 24) * coefficient;
							red += ((pixel >> 16) & 0xFF) * coefficient;
							green += ((pixel >> 8) & 0xFF) * coefficient;
							blue += (pixel & 0xFF) * coefficient;
						}

						source_pixels[(bitmap->height * z + y) * bitmap->width + x] =
							((((alpha + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 24) |
							(((red + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 16) |
							(((green + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 8) |
							((blue + (1 << (2 * filter_size - 1))) >> (2 * filter_size)));
					}
				}
			}

			for (z = 0; z < bitmap->depth; z++)
			{
				for (y = 0; y < bitmap->height; y++)
				{
					for (x = 0; x < bitmap->width; x++)
					{
						long alpha = 0;
						long red = 0;
						long green = 0;
						long blue = 0;
						short filter_index;

						for (filter_index = -filter_size;
							filter_index <= filter_size;
							filter_index++)
						{
							short source_z =
								(short)((bitmap->depth + filter_index + z) % bitmap->depth);
							pixel32 pixel = source_pixels[
								(bitmap->height * source_z + y) * bitmap->width + x];
							short coefficient = filter_coefficients[filter_index + filter_size];

							alpha += (pixel >> 24) * coefficient;
							red += ((pixel >> 16) & 0xFF) * coefficient;
							green += ((pixel >> 8) & 0xFF) * coefficient;
							blue += (pixel & 0xFF) * coefficient;
						}

						temporary_pixels[(bitmap->height * z + y) * bitmap->width + x] =
							((((alpha + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 24) |
							(((red + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 16) |
							(((green + (1 << (2 * filter_size - 1))) >> (2 * filter_size)) << 8) |
							((blue + (1 << (2 * filter_size - 1))) >> (2 * filter_size)));
					}
				}
			}

			csmemcpy(source_pixels, temporary_pixels, pixel_data_size);
			match_free(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
				0x33E,
				temporary_pixels);
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate temporary buffer");
		}
	}
	else
	{
		/* BUG (preserved): January supplies an unused newline vararg. */
		fprintf(
			stdout,
			"### WARNING tried to smooth a bitmap with a filter which is too large",
			"\r\n");
		fflush(stdout);
	}

	return;
}

static void bitmap_cm_smooth(
	struct bitmap_data *bitmap,
	short filter_size,
	short const *filter_coefficients)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x353, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x354, bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x355, filter_coefficients);

	/* BUG (preserved for exact matching): January passes a newline as an
	 * unused vararg even though the format has no conversion. A corrected
	 * build should append the newline to the format string instead.
	 */
	fprintf(stdout, "### WARNING tried to smooth a cube map", "\r\n");
	fflush(stdout);

	return;
}

void bitmap_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x361,
		bitmap_verify(bitmap, TRUE));

	if (sharpen_amount > 0.f)
	{
		short sharpen_value = (short)(sharpen_amount * 100.f);
		short falloff;
		short value;

		sharpen_value = PIN(sharpen_value, 0, 100);
		falloff = MAX(1, 100 - sharpen_value);

		for (value = 0; value < NUMBEROF(bitmap_sharpen_positive_table); value++)
		{
			bitmap_sharpen_positive_table[value] =
				(short)(100 * value / falloff);
			bitmap_sharpen_negative_table[value] =
				(short)(value * sharpen_value / 8 / falloff);
		}

		switch (bitmap->type)
		{
		case _bitmap_type_2d:
			bitmap_2d_sharpen(
				bitmap,
				sharpen_amount,
				bitmap_sharpen_positive_table,
				bitmap_sharpen_negative_table);
			break;

		case _bitmap_type_3d:
			bitmap_3d_sharpen(
				bitmap,
				sharpen_amount,
				bitmap_sharpen_positive_table,
				bitmap_sharpen_negative_table);
			break;

		case _bitmap_type_cube_map:
			bitmap_cm_sharpen(
				bitmap,
				sharpen_amount,
				bitmap_sharpen_positive_table,
				bitmap_sharpen_negative_table);
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
				0x37F,
				FALSE,
				"### ERROR unsupported bitmap type");
			break;
		}
	}

	return;
}

static void bitmap_2d_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table)
{
	long pixel_data_size;
	byte *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x38C, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x38D, bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x38E, positive_table);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x38F, negative_table);

	if (bitmap->width >= 3 && bitmap->height >= 3)
	{
		short y;

		pixel_data_size = bitmap_get_pixel_data_size(bitmap);
		temporary_pixels = match_malloc(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x398,
			pixel_data_size);
		if (temporary_pixels)
		{
			for (y = 0; y < bitmap->height; y++)
			{
				short previous_y = y > 0 ? y - 1 : bitmap->height - 1;
				short next_y = y < bitmap->height - 1 ? y + 1 : 0;
				byte *previous_row = bitmap_2d_address(bitmap, 0, previous_y, 0);
				byte *current_row = bitmap_2d_address(bitmap, 0, y, 0);
				byte *next_row = bitmap_2d_address(bitmap, 0, next_y, 0);
				byte *destination_row = temporary_pixels + 4 * bitmap->width * y;
				short byte_index = 0;
				short last_byte_index;

				do
				{
					short wrapped_left = 4 * bitmap->width + byte_index;

					destination_row[byte_index] = (byte)PIN(
						positive_table[current_row[byte_index]] -
						negative_table[previous_row[wrapped_left - 4]] -
						negative_table[previous_row[byte_index]] -
						negative_table[previous_row[byte_index + 4]] -
						negative_table[current_row[wrapped_left - 4]] -
						negative_table[current_row[byte_index + 4]] -
						negative_table[next_row[wrapped_left - 4]] -
						negative_table[next_row[byte_index]] -
						negative_table[next_row[byte_index + 4]], 0, 255);
					byte_index++;
				}
				while (byte_index < 4);

				for (last_byte_index = 4 * bitmap->width - 4; byte_index < last_byte_index; byte_index++)
				{
					destination_row[byte_index] = (byte)PIN(
						positive_table[current_row[byte_index]] -
						negative_table[previous_row[byte_index - 4]] -
						negative_table[previous_row[byte_index]] -
						negative_table[previous_row[byte_index + 4]] -
						negative_table[current_row[byte_index - 4]] -
						negative_table[current_row[byte_index + 4]] -
						negative_table[next_row[byte_index - 4]] -
						negative_table[next_row[byte_index]] -
						negative_table[next_row[byte_index + 4]], 0, 255);
				}

				for (last_byte_index += 4; byte_index < last_byte_index; byte_index++)
				{
					short wrapped_right = byte_index - 4 * bitmap->width;

					destination_row[byte_index] = (byte)PIN(
						positive_table[current_row[byte_index]] -
						negative_table[previous_row[byte_index - 4]] -
						negative_table[previous_row[byte_index]] -
						negative_table[previous_row[wrapped_right + 4]] -
						negative_table[current_row[byte_index - 4]] -
						negative_table[current_row[wrapped_right + 4]] -
						negative_table[next_row[byte_index - 4]] -
						negative_table[next_row[byte_index]] -
						negative_table[next_row[wrapped_right + 4]], 0, 255);
				}
			}

			csmemcpy(
				bitmap_mipmap_address(bitmap, 0),
				temporary_pixels,
				pixel_data_size);
			match_free(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
				0x3D1,
				temporary_pixels);
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate temporary buffer");
		}
	}

	return;
}

static void bitmap_3d_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3E2, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3E3, bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3E4, positive_table);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3E5, negative_table);

	/* BUG (preserved for exact matching): see bitmap_cm_smooth. */
	fprintf(stdout, "### WARNING tried to sharpen a 3d bitmap", "\r\n");
	fflush(stdout);

	return;
}

static void bitmap_cm_sharpen(
	struct bitmap_data *bitmap,
	real sharpen_amount,
	short const *positive_table,
	short const *negative_table)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3F3, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3F4, bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3F5, positive_table);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x3F6, negative_table);

	/* BUG (preserved for exact matching): see bitmap_cm_smooth. */
	fprintf(stdout, "### WARNING tried to sharpen a cube map", "\r\n");
	fflush(stdout);

	return;
}

void bitmap_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x402, bitmap_verify(bitmap, TRUE));

	if (passes > 0)
	{
		switch (bitmap->type)
		{
		case _bitmap_type_2d:
			bitmap_2d_alpha_bleed(bitmap, passes);
			break;

		case _bitmap_type_3d:
			bitmap_3d_alpha_bleed(bitmap, passes);
			break;

		case _bitmap_type_cube_map:
			bitmap_cm_alpha_bleed(bitmap, passes);
			break;

		default:
			match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x410, FALSE, "### ERROR unsupported bitmap type");
			break;
		}
	}

	return;
}

static void bitmap_2d_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes)
{
	long pixel_data_size;
	pixel32 *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x41D, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x41E, bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x41F, passes>0);

	pixel_data_size = bitmap_get_pixel_data_size(bitmap);
	temporary_pixels = match_malloc("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x422, pixel_data_size);
	if (temporary_pixels)
	{
		short pass;

		for (pass = 0; pass < passes; pass++)
		{
			short x, y;

			for (y = 0; y < bitmap->height; y++)
			{
				pixel32 *source_pixels = bitmap_2d_address(bitmap, 0, y, 0);
				pixel32 *destination_pixels = temporary_pixels + y * bitmap->width;

				for (x = 0; x < bitmap->width; x++)
				{
					pixel32 pixel = source_pixels[x];

					if (!(pixel & 0xFF000000))
					{
						boolean found = FALSE;
						short i, j;

						for (j = -1; !found && j <= 1; j++)
						{
							for (i = -1; !found && i <= 1; i++)
							{
								short neighbor_x = x + i;
								short neighbor_y = y + j;

								if (neighbor_x >= 0 && neighbor_y >= 0 &&
									neighbor_x < bitmap->width && neighbor_y < bitmap->height)
								{
									pixel32 neighbor = *(pixel32 *)bitmap_2d_address(
										bitmap, neighbor_x, neighbor_y, 0);

									if (neighbor)
									{
										pixel = neighbor & 0x00FFFFFF;
										found = TRUE;
									}
								}
							}
						}
					}

					destination_pixels[x] = pixel;
				}
			}

			csmemcpy(bitmap_mipmap_address(bitmap, 0), temporary_pixels, pixel_data_size);
		}

		match_free("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x462, temporary_pixels);
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary buffer");
	}

	return;
}

static void bitmap_3d_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes)
{
	struct bitmap_data *slice_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x472, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x473, bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x474, passes>0);

	slice_bitmap = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
	if (slice_bitmap && slice_bitmap->base_address)
	{
		short slice_index;

		for (slice_index = 0; slice_index < (short)bitmap->depth; slice_index++)
		{
			bitmap_3d_slice_extract(bitmap, 0, slice_index, slice_bitmap);
			bitmap_2d_alpha_bleed(slice_bitmap, passes);
			bitmap_3d_slice_insert(slice_bitmap, bitmap, 0, slice_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(slice_bitmap);

	return;
}

static void bitmap_cm_alpha_bleed(
	struct bitmap_data *bitmap,
	short passes)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4A1, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4A2, bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4A3, passes>0);

	fprintf(stdout, "### WARNING tried to alpha-bleed a cube map (skipping)");
	fflush(stdout);

	return;
}

void bitmap_height_map(
	struct bitmap_data *bitmap,
	real bump_height)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4AF, bitmap_verify(bitmap, TRUE));

	if (bump_height > 0.f)
	{
		switch (bitmap->type)
		{
		case _bitmap_type_2d:
			bitmap_2d_height_map(bitmap, bump_height);
			break;

		case _bitmap_type_3d:
			bitmap_3d_height_map(bitmap, bump_height);
			break;

		case _bitmap_type_cube_map:
			bitmap_cm_height_map(bitmap, bump_height);
			break;

		default:
			match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4BD, FALSE, "### ERROR unsupported bitmap type");
			break;
		}
	}
	else
	{
		fprintf(stdout, "### WARNING importing special-effect bump map with zero-height\r\n");
		fflush(stdout);
	}

	return;
}

static void bitmap_2d_height_map(
	struct bitmap_data *bitmap,
	real bump_height)
{
	long pixel_data_size;
	pixel32 *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4CF, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4D0, bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4D1, bump_height>0.0f);

	pixel_data_size = bitmap_get_pixel_data_size(bitmap);
	temporary_pixels = match_malloc("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x4D4, pixel_data_size);
	if (temporary_pixels)
	{
		real scale = bitmap->height * bump_height * (1.f / 255.f);
		short x, y;

		for (y = 0; y < bitmap->height; y++)
		{
			for (x = 0; x < bitmap->width; x++)
			{
				pixel32 pixel = *(pixel32 *)bitmap_2d_address(bitmap, x, y, 0);
				pixel32 left_pixel = *(pixel32 *)bitmap_2d_address(bitmap,
					x == 0 ? bitmap->width - 1 : x - 1, y, 0);
				pixel32 right_pixel = *(pixel32 *)bitmap_2d_address(bitmap,
					x == bitmap->width - 1 ? 0 : x + 1, y, 0);
				pixel32 up_pixel = *(pixel32 *)bitmap_2d_address(bitmap, x,
					y == 0 ? bitmap->height - 1 : y - 1, 0);
				pixel32 down_pixel = *(pixel32 *)bitmap_2d_address(bitmap, x,
					y == bitmap->height - 1 ? 0 : y + 1, 0);
				real center_height = ((pixel >> 16) & 0xFF) * scale;
				real left_height = ((left_pixel >> 16) & 0xFF) * scale;
				real right_height = ((right_pixel >> 16) & 0xFF) * scale;
				real up_height = ((up_pixel >> 16) & 0xFF) * scale;
				real down_height = ((down_pixel >> 16) & 0xFF) * scale;
				real_vector3d x_vector, y_vector, normal;

				x_vector.j = 0.f;
				if (center_height > left_height && center_height > right_height)
				{
					x_vector.i = 1.f;
					x_vector.k = 0.f;
				}
				else if (left_height > right_height)
				{
					x_vector.i = -1.f;
					x_vector.k = left_height - center_height;
				}
				else
				{
					x_vector.i = 1.f;
					x_vector.k = right_height - center_height;
				}

				y_vector.i = 0.f;
				if (center_height > up_height && center_height > down_height)
				{
					y_vector.j = 1.f;
					y_vector.k = 0.f;
				}
				else if (up_height > down_height)
				{
					y_vector.j = -1.f;
					y_vector.k = up_height - center_height;
				}
				else
				{
					y_vector.j = 1.f;
					y_vector.k = down_height - center_height;
				}

				cross_product3d(&x_vector, &y_vector, &normal);
				if (normal.k < 0.f)
				{
					normal.i = -normal.i;
					normal.j = -normal.j;
					normal.k = -normal.k;
				}
				normalize3d(&normal);

				temporary_pixels[y * bitmap->width + x] =
					(pixel & 0xFF000000) |
					(fast_ftol((normal.i + 1.f) * 127.5f) << 16) |
					(fast_ftol((normal.j + 1.f) * 127.5f) << 8) |
					fast_ftol((normal.k + 1.f) * 127.5f);
			}
		}

		csmemcpy(bitmap_mipmap_address(bitmap, 0), temporary_pixels, pixel_data_size);
		match_free("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x51D, temporary_pixels);
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary buffer");
	}

	return;
}

static void bitmap_3d_height_map(
	struct bitmap_data *bitmap,
	real bump_height)
{
	struct bitmap_data *slice_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x52D, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x52E, bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x52F, bump_height>0.0f);

	slice_bitmap = bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
	if (slice_bitmap && slice_bitmap->base_address)
	{
		short slice_index;

		for (slice_index = 0; slice_index < (short)bitmap->depth; slice_index++)
		{
			bitmap_3d_slice_extract(bitmap, 0, slice_index, slice_bitmap);
			bitmap_2d_height_map(slice_bitmap, bump_height);
			bitmap_3d_slice_insert(slice_bitmap, bitmap, 0, slice_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(slice_bitmap);

	return;
}

static void bitmap_cm_height_map(
	struct bitmap_data *bitmap,
	real bump_height)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x55C, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x55D, bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x55E, bump_height>0.0f);

	fprintf(stdout, "### WARNING tried to use a cube map as a height map\r\n");
	fflush(stdout);

	return;
}

void bitmap_vector_map(
	struct bitmap_data *bitmap)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x569, bitmap_verify(bitmap, TRUE));

	switch (bitmap->type)
	{
		case _bitmap_type_2d:
			bitmap_2d_vector_map(bitmap);
			break;

		case _bitmap_type_3d:
			bitmap_3d_vector_map(bitmap);
			break;

		case _bitmap_type_cube_map:
			bitmap_cm_vector_map(bitmap);
			break;

		default:
			match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x577, FALSE, "### ERROR unsupported bitmap type");
			break;
	}

	return;
}

static void bitmap_2d_vector_map(
	struct bitmap_data *bitmap)
{
	long pixel_data_size;
	pixel32 *temporary_pixels;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x583, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x584, bitmap->type==_bitmap_type_2d);

	pixel_data_size= bitmap_get_pixel_data_size(bitmap);
	temporary_pixels= match_malloc("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x587, pixel_data_size);
	if (temporary_pixels)
	{
		short x, y;

		for (y= 0; y<bitmap->height; y++)
		{
			for (x= 0; x<bitmap->width; x++)
			{
				pixel32 pixel= *(pixel32 *)bitmap_2d_address(bitmap, x, y, 0);
				real_vector3d vector;

				vector.i= ((pixel>>16)&0xFF)*(2.f/255.f) - 1.f;
				vector.j= ((pixel>>8)&0xFF)*(2.f/255.f) - 1.f;
				vector.k= (pixel&0xFF)*(2.f/255.f) - 1.f;
				normalize3d(&vector);

				temporary_pixels[y*bitmap->width + x]=
					(pixel&0xFF000000) |
					(fast_ftol((vector.i+1.f)*127.5f + 0.5f)<<16) |
					(fast_ftol((vector.j+1.f)*127.5f + 0.5f)<<8) |
					fast_ftol((vector.k+1.f)*127.5f + 0.5f);
			}
		}

		csmemcpy(bitmap_mipmap_address(bitmap, 0), temporary_pixels, pixel_data_size);
		match_free("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x5A8, temporary_pixels);
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary buffer");
	}

	return;
}

static void bitmap_3d_vector_map(
	struct bitmap_data *bitmap)
{
	struct bitmap_data *slice_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x5B7, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x5B8, bitmap->type==_bitmap_type_3d);

	slice_bitmap= bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
	if (slice_bitmap && slice_bitmap->base_address)
	{
		short slice_index;

		for (slice_index= 0; slice_index<(short)bitmap->depth; slice_index++)
		{
			bitmap_3d_slice_extract(bitmap, 0, slice_index, slice_bitmap);
			bitmap_2d_vector_map(slice_bitmap);
			bitmap_3d_slice_insert(slice_bitmap, bitmap, 0, slice_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(slice_bitmap);

	return;
}

static void bitmap_cm_vector_map(
	struct bitmap_data *bitmap)
{
	struct bitmap_data *face_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x5E5, bitmap_verify(bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x5E6, bitmap->type==_bitmap_type_cube_map);

	face_bitmap= bitmap_2d_new(bitmap->width, bitmap->height, 0, bitmap->format);
	if (face_bitmap && face_bitmap->base_address)
	{
		short face_index;

		for (face_index= 0; face_index<NUMBER_OF_FACES_PER_CUBE; face_index++)
		{
			bitmap_cube_map_face_extract(bitmap, 0, face_index, face_bitmap);
			bitmap_2d_vector_map(face_bitmap);
			bitmap_cube_map_face_insert(face_bitmap, bitmap, 0, face_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(face_bitmap);

	return;
}

void bitmap_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x619, bitmap_verify(source_bitmap, TRUE));

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x61B, bitmap_verify(destination_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x61C, destination_mipmap_index>=0 && destination_mipmap_index<=destination_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x61D, MAX(1, destination_bitmap->width >>destination_mipmap_index)==source_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x61E, MAX(1, destination_bitmap->height>>destination_mipmap_index)==source_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x61F, MAX(1, destination_bitmap->depth >>destination_mipmap_index)==source_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x620, TEST_FLAG(destination_bitmap->flags, _bitmap_compressed_bit));

	switch (source_bitmap->type)
	{
	case _bitmap_type_2d:
		bitmap_2d_compress_to_mipmap(
			source_bitmap,
			destination_bitmap,
			destination_mipmap_index,
			transparent_color);
		break;

	case _bitmap_type_3d:
		bitmap_3d_compress_to_mipmap(
			source_bitmap,
			destination_bitmap,
			destination_mipmap_index,
			transparent_color);
		break;

	case _bitmap_type_cube_map:
		bitmap_cm_compress_to_mipmap(
			source_bitmap,
			destination_bitmap,
			destination_mipmap_index,
			transparent_color);
		break;

	default:
		match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x630, FALSE, "### ERROR unsupported bitmap type");
		break;
	}

	return;
}

static void bitmap_2d_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x63C, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x63D, source_bitmap->type==_bitmap_type_2d);

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x63F, bitmap_verify(destination_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x640, destination_bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x641, destination_mipmap_index>=0 && destination_mipmap_index<=destination_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x642, MAX(1, destination_bitmap->width >>destination_mipmap_index)==source_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x643, MAX(1, destination_bitmap->height>>destination_mipmap_index)==source_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x644, MAX(1, destination_bitmap->depth >>destination_mipmap_index)==source_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x645, TEST_FLAG(destination_bitmap->flags, _bitmap_compressed_bit));

	/* The Xbox tool build does not compress bitmaps at runtime. */
	display_assert(NULL, "c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x69F, TRUE);
	system_exit(-1);

	return;
}

static void bitmap_3d_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color)
{
	struct bitmap_data *source_slice_bitmap;
	struct bitmap_data *destination_slice_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6AF, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B0, source_bitmap->type==_bitmap_type_3d);

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B2, bitmap_verify(destination_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B3, destination_bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B4, destination_mipmap_index>=0 && destination_mipmap_index<=destination_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B5, MAX(1, destination_bitmap->width >>destination_mipmap_index)==source_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B6, MAX(1, destination_bitmap->height>>destination_mipmap_index)==source_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B7, MAX(1, destination_bitmap->depth >>destination_mipmap_index)==source_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6B8, TEST_FLAG(destination_bitmap->flags, _bitmap_compressed_bit));

	source_slice_bitmap = bitmap_2d_new(
		source_bitmap->width,
		source_bitmap->height,
		0,
		source_bitmap->format);
	destination_slice_bitmap = bitmap_2d_new(
		source_bitmap->width,
		source_bitmap->height,
		0,
		destination_bitmap->format);
	if (source_slice_bitmap && source_slice_bitmap->base_address &&
		destination_slice_bitmap && destination_slice_bitmap->base_address)
	{
		short slice_index;

		for (slice_index = 0; slice_index < (short)source_bitmap->depth; slice_index++)
		{
			bitmap_3d_slice_extract(source_bitmap, 0, slice_index, source_slice_bitmap);
			bitmap_2d_compress_to_mipmap(source_slice_bitmap, destination_slice_bitmap, 0, transparent_color);
			bitmap_3d_slice_insert(
				destination_slice_bitmap,
				destination_bitmap,
				destination_mipmap_index,
				slice_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(source_slice_bitmap);
	bitmap_delete(destination_slice_bitmap);

	return;
}

static void bitmap_cm_compress_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index,
	pixel32 const *transparent_color)
{
	struct bitmap_data *source_face_bitmap;
	struct bitmap_data *destination_face_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6FA, bitmap_verify(source_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6FB, source_bitmap->type==_bitmap_type_cube_map);

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6FD, bitmap_verify(destination_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6FE, destination_bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x6FF, destination_mipmap_index>=0 && destination_mipmap_index<=destination_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x700, MAX(1, destination_bitmap->width >>destination_mipmap_index)==source_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x701, MAX(1, destination_bitmap->height>>destination_mipmap_index)==source_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x702, MAX(1, destination_bitmap->depth >>destination_mipmap_index)==source_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x703, TEST_FLAG(destination_bitmap->flags, _bitmap_compressed_bit));

	source_face_bitmap = bitmap_2d_new(
		source_bitmap->width,
		source_bitmap->height,
		0,
		source_bitmap->format);
	destination_face_bitmap = bitmap_2d_new(
		source_bitmap->width,
		source_bitmap->height,
		0,
		destination_bitmap->format);
	if (source_face_bitmap && source_face_bitmap->base_address &&
		destination_face_bitmap && destination_face_bitmap->base_address)
	{
		short face_index;

		for (face_index = 0; face_index < NUMBER_OF_FACES_PER_CUBE; face_index++)
		{
			bitmap_cube_map_face_extract(source_bitmap, 0, face_index, source_face_bitmap);
			bitmap_2d_compress_to_mipmap(source_face_bitmap, destination_face_bitmap, 0, transparent_color);
			bitmap_cube_map_face_insert(
				destination_face_bitmap,
				destination_bitmap,
				destination_mipmap_index,
				face_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(source_face_bitmap);
	bitmap_delete(destination_face_bitmap);

	return;
}

void bitmap_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index)
{
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x746, bitmap_verify(source_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x747, source_mipmap_index>=0 && source_mipmap_index<=source_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x748, MAX(1, source_bitmap->width >>source_mipmap_index)==destination_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x749, MAX(1, source_bitmap->height>>source_mipmap_index)==destination_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x74A, MAX(1, source_bitmap->depth >>source_mipmap_index)==destination_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x74B, TEST_FLAG(source_bitmap->flags, _bitmap_compressed_bit));

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x74D, bitmap_verify(destination_bitmap, TRUE));

	switch (source_bitmap->type)
	{
	case _bitmap_type_2d:
		bitmap_2d_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
		break;

	case _bitmap_type_3d:
		bitmap_3d_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
		break;

	case _bitmap_type_cube_map:
		bitmap_cm_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
		break;

	default:
		match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x75B, FALSE, "### ERROR unsupported bitmap type");
		break;
	}

	return;
}

static void bitmap_2d_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index)
{
	pixel32 colors[S3TC_BLOCK_PIXELS];
	byte *block;
	short height;
	short x, y;
	short destination_x, destination_y;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x766, bitmap_verify(source_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x767, source_bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x768, source_mipmap_index>=0 && source_mipmap_index<=source_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x769, MAX(1, source_bitmap->width >>source_mipmap_index)==destination_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x76A, MAX(1, source_bitmap->height>>source_mipmap_index)==destination_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x76B, MAX(1, source_bitmap->depth >>source_mipmap_index)==destination_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x76C, TEST_FLAG(source_bitmap->flags, _bitmap_compressed_bit));

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x76E, bitmap_verify(destination_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x76F, destination_bitmap->type==_bitmap_type_2d);

	block = bitmap_mipmap_address(source_bitmap, source_mipmap_index);
	height = bitmap_mipmap_get_height(source_bitmap, source_mipmap_index);

	for (y = 0, destination_y = 0; y < height; y += 4, destination_y += 4)
	{
		short width = bitmap_mipmap_get_width(source_bitmap, source_mipmap_index);

		for (x = 0, destination_x = 0; x < width; x += 4, destination_x += 4)
		{
			short color_index = 0;
			short i, j;

			switch (source_bitmap->format)
			{
			case _bitmap_format_dxt1:
				DecodeBlockRGB(
					(struct s3tc_block_rgb *)block,
					(struct s3tc_color *)colors);
				block += sizeof(struct s3tc_block_rgb);
				break;

			case _bitmap_format_dxt3:
				DecodeBlockAlpha4(
					(struct s3tc_block_alpha4 *)block,
					(struct s3tc_color *)colors);
				block += sizeof(struct s3tc_block_alpha4);
				break;

			case _bitmap_format_dxt5:
				DecodeBlockAlpha3(
					(struct s3tc_block_alpha3 *)block,
					(struct s3tc_color *)colors);
				block += sizeof(struct s3tc_block_alpha3);
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x78C, FALSE, "### ERROR unsupported bitmap format");
				break;
			}

			for (j = 0; j < 4; j++)
			{
				for (i = 0; i < 4; i++)
				{
					if (destination_x + i < destination_bitmap->width &&
						destination_y + j < destination_bitmap->height)
					{
						*(pixel32 *)bitmap_2d_address(destination_bitmap, x + i, y + j, 0) =
							colors[color_index++];
					}
				}
			}
		}
	}

	return;
}

static void bitmap_3d_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index)
{
	struct bitmap_data *source_slice_bitmap;
	struct bitmap_data *destination_slice_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B0, bitmap_verify(source_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B1, source_bitmap->type==_bitmap_type_3d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B2, source_mipmap_index>=0 && source_mipmap_index<=source_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B3, MAX(1, source_bitmap->width >>source_mipmap_index)==destination_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B4, MAX(1, source_bitmap->height>>source_mipmap_index)==destination_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B5, MAX(1, source_bitmap->depth >>source_mipmap_index)==destination_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B6, TEST_FLAG(source_bitmap->flags, _bitmap_compressed_bit));

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B8, bitmap_verify(destination_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7B9, destination_bitmap->type==_bitmap_type_3d);

	source_slice_bitmap = bitmap_2d_new(
		destination_bitmap->width,
		destination_bitmap->height,
		0,
		source_bitmap->format);
	destination_slice_bitmap = bitmap_2d_new(
		destination_bitmap->width,
		destination_bitmap->height,
		0,
		destination_bitmap->format);
	if (source_slice_bitmap && source_slice_bitmap->base_address &&
		destination_slice_bitmap && destination_slice_bitmap->base_address)
	{
		short slice_index;

		for (slice_index = 0; slice_index < (short)source_bitmap->depth; slice_index++)
		{
			bitmap_3d_slice_extract(
				source_bitmap, source_mipmap_index, slice_index, source_slice_bitmap);
			bitmap_2d_uncompress_from_mipmap(source_slice_bitmap, destination_slice_bitmap, 0);
			bitmap_3d_slice_insert(
				destination_slice_bitmap, destination_bitmap, 0, slice_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(source_slice_bitmap);
	bitmap_delete(destination_slice_bitmap);

	return;
}

static void bitmap_cm_uncompress_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index)
{
	struct bitmap_data *source_face_bitmap;
	struct bitmap_data *destination_face_bitmap;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7F9, bitmap_verify(source_bitmap, FALSE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FA, source_bitmap->type==_bitmap_type_cube_map);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FB, source_mipmap_index>=0 && source_mipmap_index<=source_bitmap->mipmap_count);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FC, MAX(1, source_bitmap->width >>source_mipmap_index)==destination_bitmap->width);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FD, MAX(1, source_bitmap->height>>source_mipmap_index)==destination_bitmap->height);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FE, MAX(1, source_bitmap->depth >>source_mipmap_index)==destination_bitmap->depth);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x7FF, TEST_FLAG(source_bitmap->flags, _bitmap_compressed_bit));

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x801, bitmap_verify(destination_bitmap, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x802, destination_bitmap->type==_bitmap_type_cube_map);

	source_face_bitmap = bitmap_2d_new(
		destination_bitmap->width,
		destination_bitmap->height,
		0,
		source_bitmap->format);
	destination_face_bitmap = bitmap_2d_new(
		destination_bitmap->width,
		destination_bitmap->height,
		0,
		destination_bitmap->format);
	if (source_face_bitmap && source_face_bitmap->base_address &&
		destination_face_bitmap && destination_face_bitmap->base_address)
	{
		short face_index;

		for (face_index = 0; face_index < NUMBER_OF_FACES_PER_CUBE; face_index++)
		{
			bitmap_cube_map_face_extract(
				source_bitmap, source_mipmap_index, face_index, source_face_bitmap);
			bitmap_2d_uncompress_from_mipmap(source_face_bitmap, destination_face_bitmap, 0);
			bitmap_cube_map_face_insert(
				destination_face_bitmap, destination_bitmap, 0, face_index);
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate temporary bitmap");
	}

	bitmap_delete(source_face_bitmap);
	bitmap_delete(destination_face_bitmap);

	return;
}

real real_rgb_color_brightness(
	union real_rgb_color const *color)
{
	return
		color->red * 0.299f +
		color->green * 0.587f +
		color->blue * 0.114f;
}

struct hsv_color *rgb_color_to_hsv_color(
	struct rgb_color const *rgb,
	struct hsv_color *hsv)
{
	real red = (real)(long)rgb->red * (1.0f / 65535.0f);
	real green = (real)(long)rgb->green * (1.0f / 65535.0f);
	real blue = (real)(long)rgb->blue * (1.0f / 65535.0f);
	real value;
	real minimum;
	real delta;
	real hue;
	real saturation;

	if (green > blue)
		value = green;
	else
		value = blue;
	if (red > value)
		value = red;
	else
	{
		if (green > blue)
			value = green;
		else
			value = blue;
	}

	if (green > blue)
		minimum = blue;
	else
		minimum = green;
	if (red > minimum)
	{
		if (green > blue)
			minimum = blue;
		else
			minimum = green;
	}
	else
		minimum = red;

	delta = value - minimum;

	if (!hsv)
	{
		display_assert(
			"hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x852,
			TRUE);
		system_exit(-1);
	}
	if ((void const *)rgb == (void const *)hsv)
	{
		display_assert(
			"rgb!=(rgb_color *)hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x853,
			TRUE);
		system_exit(-1);
	}

	if (value == 0.0f)
		saturation = 0.0f;
	else
		saturation = delta / value;

	if (saturation == 0.0f)
		hue = 0.0f;
	else
	{
		if (red == value)
			hue = (green - blue) / delta;
		else if (green == value)
			hue = (blue - red) / delta + 2.0f;
		else
			hue = (red - green) / delta + 4.0f;

		hue *= 1.0f / 6.0f;
		if (hue < 0.0f)
			hue += 1.0f;
	}

	hsv->hue = (word)(long)(hue * 65536.0f);
	hsv->saturation = (word)(long)(saturation * 65535.0f);
	hsv->value = (word)(long)(value * 65535.0f);
	return hsv;
}

struct rgb_color *hsv_color_to_rgb_color(
	struct hsv_color const *hsv,
	struct rgb_color *rgb)
{
	real scaled_hue;
	real saturation;
	real value;
	real fraction;
	real p;
	real q;
	real t;
	real red;
	real green;
	real blue;
	long truncated_sector;
	long sector;

	scaled_hue = (real)(long)hsv->hue * (1.0f / 65536.0f);
	scaled_hue *= 6.0f;
	saturation = (real)(long)hsv->saturation * (1.0f / 65535.0f);
	value = (real)(long)hsv->value * (1.0f / 65535.0f);

	if (!rgb)
	{
		display_assert(
			"rgb",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x886,
			TRUE);
		system_exit(-1);
	}
	if ((void const *)rgb == (void const *)hsv)
	{
		display_assert(
			"rgb!=(rgb_color *)hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x888,
			TRUE);
		system_exit(-1);
	}

	if (saturation == 0.0f)
		red = green = blue = value;
	else
	{
		truncated_sector = (long)scaled_hue;
		sector = (real)truncated_sector > scaled_hue ? truncated_sector - 1 : truncated_sector;
		fraction = scaled_hue - (real)sector;
		p = (1.0f - saturation) * value;
		q = (1.0f - saturation * fraction) * value;
		t = (1.0f - (1.0f - fraction) * saturation) * value;

		switch (sector)
		{
		case 0:
			red = value;
			green = t;
			blue = p;
			break;
		case 1:
			red = q;
			green = value;
			blue = p;
			break;
		case 2:
			red = p;
			green = value;
			blue = t;
			break;
		case 3:
			red = p;
			green = q;
			blue = value;
			break;
		case 4:
			red = t;
			green = p;
			blue = value;
			break;
		case 5:
			red = value;
			green = p;
			blue = q;
			break;
		}
	}

	/* The sector switch has no default. Not reached unassigned: for a word hue,
	 * scaled_hue = hue * 6 / 65536 is at most 393210 / 65536 < 6 (exact in real), so
	 * sector is 0..5 and each of those cases assigns red, green and blue. Source-policy
	 * approval pending (2026-09-27 audit). */
	rgb->red = (word)(long)(red * 65535.0f);
	rgb->green = (word)(long)(green * 65535.0f);
	rgb->blue = (word)(long)(blue * 65535.0f);
	return rgb;
}

union real_hsv_color *real_rgb_color_to_real_hsv_color(
	union real_rgb_color const *rgb,
	union real_hsv_color *hsv)
{
	real value;
	real minimum;
	real delta;
	real saturation;

	if (rgb->green > rgb->blue)
		value = rgb->green;
	else
		value = rgb->blue;
	if (rgb->red > value)
		value = rgb->red;
	else
	{
		if (rgb->green > rgb->blue)
			value = rgb->green;
		else
			value = rgb->blue;
	}

	if (rgb->green > rgb->blue)
		minimum = rgb->blue;
	else
		minimum = rgb->green;
	if (rgb->red > minimum)
	{
		if (rgb->green > rgb->blue)
			minimum = rgb->blue;
		else
			minimum = rgb->green;
	}
	else
		minimum = rgb->red;

	delta = value - minimum;

	if (!hsv)
	{
		display_assert(
			"hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x8B2,
			TRUE);
		system_exit(-1);
	}
	if ((void const *)rgb == (void const *)hsv)
	{
		display_assert(
			"rgb!=(real_rgb_color *)hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x8B3,
			TRUE);
		system_exit(-1);
	}

	hsv->value = value;
	if (value == 0.0f)
		saturation = 0.0f;
	else
		saturation = delta / value;
	hsv->saturation = saturation;

	if (saturation == 0.0f)
	{
		hsv->hue = 0.0f;
		return hsv;
	}
	if (rgb->red == value)
		hsv->hue = (rgb->green - rgb->blue) / delta;
	else if (rgb->green == value)
		hsv->hue = (rgb->blue - rgb->red) / delta + 2.0f;
	else
		hsv->hue = (rgb->red - rgb->green) / delta + 4.0f;

	hsv->hue *= 1.0f / 6.0f;
	if (hsv->hue < 0.0f)
		hsv->hue += 1.0f;
	return hsv;
}

union real_rgb_color *real_hsv_color_to_real_rgb_color(
	union real_hsv_color *hsv,
	union real_rgb_color *rgb)
{
	union real_hsv_color *source = hsv;
	real scaled_hue = source->hue * 6.0f;
	real p;
	real q;
	real t;
	long truncated_sector;
	long sector;

	if (!rgb)
	{
		display_assert(
			"rgb",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x8DF,
			TRUE);
		system_exit(-1);
	}
	if ((void const *)rgb == (void const *)source)
	{
		display_assert(
			"rgb!=(real_rgb_color *)hsv",
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c",
			0x8E1,
			TRUE);
		system_exit(-1);
	}

	if (source->saturation == 0.0f)
	{
		rgb->red = rgb->green = rgb->blue = source->value;
		return rgb;
	}

	truncated_sector = (long)scaled_hue;
	sector = (real)truncated_sector > scaled_hue ? truncated_sector - 1 : truncated_sector;
	scaled_hue -= sector;
	p = (1.0f - source->saturation) * source->value;
	q = (1.0f - scaled_hue * source->saturation) * source->value;
	t = (1.0f - (1.0f - scaled_hue) * source->saturation) * source->value;

	switch (sector)
	{
	case 0:
		rgb->red = source->value;
		rgb->green = t;
		rgb->blue = p;
		return rgb;
	case 1:
		rgb->red = q;
		rgb->green = source->value;
		rgb->blue = p;
		return rgb;
	case 2:
		rgb->red = p;
		rgb->green = source->value;
		rgb->blue = t;
		return rgb;
	case 3:
		rgb->red = p;
		rgb->green = q;
		rgb->blue = source->value;
		return rgb;
	case 4:
		rgb->red = t;
		rgb->green = p;
		rgb->blue = source->value;
		return rgb;
	case 5:
		rgb->red = source->value;
		rgb->green = p;
		rgb->blue = q;
		return rgb;
	default:
		return rgb;
	}
}

union real_argb_color *argb_color_to_real_argb_color(
	struct argb_color const *source,
	union real_argb_color *result)
{
	result->alpha = (real)(long)source->alpha * oo_unsigned_short_max;
	result->red = (real)(long)source->red * oo_unsigned_short_max;
	result->green = (real)(long)source->green * oo_unsigned_short_max;
	result->blue = (real)(long)source->blue * oo_unsigned_short_max;
	return result;
}

union real_rgb_color *rgb_color_to_real_rgb_color(
	struct rgb_color const *source,
	union real_rgb_color *result)
{
	result->red = (real)(long)source->red * oo_unsigned_short_max;
	result->green = (real)(long)source->green * oo_unsigned_short_max;
	result->blue = (real)(long)source->blue * oo_unsigned_short_max;
	return result;
}

union real_argb_color *pixel32_to_real_argb_color(
	pixel32 color,
	union real_argb_color *result)
{
	unsigned long alpha = color >> 24;
	unsigned long red = (color >> 16) & 0xFF;
	unsigned long green = (color >> 8) & 0xFF;
	unsigned long blue = color & 0xFF;

	result->alpha = alpha * (1.0f / 255.0f);
	result->red = red * (1.0f / 255.0f);
	result->green = green * (1.0f / 255.0f);
	result->blue = blue * (1.0f / 255.0f);
	return result;
}

union real_rgb_color *pixel32_to_real_rgb_color(
	pixel32 color,
	union real_rgb_color *result)
{
	unsigned long red = (color >> 16) & 0xFF;
	unsigned long green = (color >> 8) & 0xFF;
	unsigned long blue = color & 0xFF;

	result->red = red * (1.0f / 255.0f);
	result->green = green * (1.0f / 255.0f);
	result->blue = blue * (1.0f / 255.0f);
	return result;
}

boolean valid_real_rgb_color(
	union real_rgb_color const *color)
{
	return
		valid_real(color->red) &&
		valid_real(color->green) &&
		valid_real(color->blue) &&
		color->red>=0.f && color->red<=1.f &&
		color->green>=0.f && color->green<=1.f &&
		color->blue>=0.f && color->blue<=1.f;
}

union real_rgb_color *rgb_colors_interpolate(
	union real_rgb_color *rgb_result,
	unsigned long flags,
	union real_rgb_color const *rgb_lower_bound,
	union real_rgb_color const *rgb_upper_bound,
	real interpolation_factor)
{
	real inverse_interpolation_factor = 1.f - interpolation_factor;

	match_assert_valid_real_rgb_color("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x939, rgb_lower_bound);
	match_assert_valid_real_rgb_color("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x93A, rgb_upper_bound);

	if (TEST_FLAG(flags, _rgb_color_interpolation_hsv_bit))
	{
		union real_hsv_color hsv_result;
		union real_hsv_color hsv_lower_bound;
		union real_hsv_color hsv_upper_bound;

		real_rgb_color_to_real_hsv_color(rgb_lower_bound, &hsv_lower_bound);
		real_rgb_color_to_real_hsv_color(rgb_upper_bound, &hsv_upper_bound);

		if ((fabs(hsv_lower_bound.hue - hsv_upper_bound.hue) > 0.5) !=
			TEST_FLAG(flags, _rgb_color_interpolation_hsv_reverse_bit))
		{
			if (hsv_lower_bound.hue < hsv_upper_bound.hue)
				hsv_lower_bound.hue += 1.f;
			else
				hsv_upper_bound.hue += 1.f;
		}

		hsv_result.hue =
			inverse_interpolation_factor * hsv_lower_bound.hue +
			interpolation_factor * hsv_upper_bound.hue;
		if (hsv_result.hue > 1.f)
			hsv_result.hue -= 1.f;
		hsv_result.saturation =
			inverse_interpolation_factor * hsv_lower_bound.saturation +
			interpolation_factor * hsv_upper_bound.saturation;
		hsv_result.value =
			inverse_interpolation_factor * hsv_lower_bound.value +
			interpolation_factor * hsv_upper_bound.value;

		real_hsv_color_to_real_rgb_color(&hsv_result, rgb_result);
	}
	else
	{
		rgb_result->red =
			inverse_interpolation_factor * rgb_lower_bound->red +
			interpolation_factor * rgb_upper_bound->red;
		rgb_result->green =
			inverse_interpolation_factor * rgb_lower_bound->green +
			interpolation_factor * rgb_upper_bound->green;
		rgb_result->blue =
			inverse_interpolation_factor * rgb_lower_bound->blue +
			interpolation_factor * rgb_upper_bound->blue;
	}

	match_assert_valid_real_rgb_color("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x95D, rgb_result);

	return rgb_result;
}

union real_rgb_color *rgb_colors_interpolate_and_scale(
	union real_rgb_color *rgb_result,
	unsigned long flags,
	union real_argb_color const *argb_lower_bound,
	union real_argb_color const *argb_upper_bound,
	union real_rgb_color const *rgb_scale,
	real u)
{
	rgb_colors_interpolate(
		rgb_result,
		flags,
		&argb_lower_bound->rgb,
		&argb_upper_bound->rgb,
		u);

	if (rgb_scale)
	{
		match_assert_valid_real_rgb_color("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x96E, rgb_scale);

		if (argb_lower_bound->alpha>_real_epsilon ||
			argb_upper_bound->alpha>_real_epsilon)
		{
			real alpha=
				(1.f-u)*argb_lower_bound->alpha +
				u*argb_upper_bound->alpha;

			rgb_result->red= alpha*rgb_result->red + (1.f-alpha)*rgb_scale->red;
			rgb_result->green= alpha*rgb_result->green + (1.f-alpha)*rgb_scale->green;
			rgb_result->blue= alpha*rgb_result->blue + (1.f-alpha)*rgb_scale->blue;
		}
		else
		{
			rgb_result->red*= rgb_scale->red;
			rgb_result->green*= rgb_scale->green;
			rgb_result->blue*= rgb_scale->blue;
		}
	}

	match_assert_valid_real_rgb_color("c:\\halo\\SOURCE\\bitmaps\\bitmap_utilities.c", 0x982, rgb_result);

	return rgb_result;
}
