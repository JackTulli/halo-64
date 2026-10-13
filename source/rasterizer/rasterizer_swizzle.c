/*
RASTERIZER_SWIZZLE.C
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "math/real_math.h"
#include "math/integer_math.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps_mipmap.h"
#include "rasterizer/rasterizer_swizzle.h"

/* ---------- constants */

enum
{
	NUMBER_OF_SWIZZLE_TABLE_ENTRIES = 64,
};

enum
{
	_bitmap_type_2d = 0,
	_bitmap_type_3d,
	_bitmap_type_cube_map,
};

enum
{
	_bitmap_has_power_of_two_dimensions_bit = 0,
	_bitmap_compressed_bit = 1,
	_bitmap_swizzled_bit = 3,
	_bitmap_linear_bit = 4,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void compute_swizzle_masks(
	short width,
	short height,
	short depth);

/* ---------- globals */

/* interleaves the low 6 bits of an index into the even bit positions */
static const word swizzle_table[NUMBER_OF_SWIZZLE_TABLE_ENTRIES]=
{
	0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
	0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
	0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
	0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
	0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
	0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
	0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
	0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555
};

/* maps a bitmap cube-map face index to the hardware face order */
static const short face_mapping_inverse_table[NUMBER_OF_FACES_PER_CUBE]=
{
	0, 2, 1, 3, 4, 5
};

static unsigned long rasterizer_swizzle_z_mask;
static unsigned long rasterizer_swizzle_y_mask;
static unsigned long rasterizer_swizzle_x_mask;

/* ---------- public code */

short rasterizer_xbox_bitmap_get_max_mipmap_count(
	struct bitmap_data *bitmap)
{
	short mipmap_count = 0;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 459, bitmap_verify(bitmap, FALSE));

	if (TEST_FLAG(bitmap->flags, _bitmap_has_power_of_two_dimensions_bit) &&
		!TEST_FLAG(bitmap->flags, _bitmap_linear_bit))
	{
		if (TEST_FLAG(bitmap->flags, _bitmap_compressed_bit))
		{
			mipmap_count = MIN((short)bitmap->mipmap_count,
				floor_log2(MAX(bitmap->width/4, MAX(bitmap->height/4, (short)bitmap->depth))));
		}
		else
		{
			mipmap_count = MIN((short)bitmap->mipmap_count,
				floor_log2(MAX(bitmap->width, MAX(bitmap->height, (short)bitmap->depth))));
		}
	}

	return mipmap_count;
}

long rasterizer_xbox_bitmap_get_pixel_data_size(
	struct bitmap_data *bitmap)
{
	long offset = 0;
	short maximum_mipmap_index = rasterizer_xbox_bitmap_get_max_mipmap_count(bitmap);
	short mipmap_index;

	for (mipmap_index = 0; mipmap_index <= maximum_mipmap_index; mipmap_index++)
	{
		long size = bitmap_mipmap_get_pixel_data_size(bitmap, mipmap_index);

		if (TEST_FLAG(bitmap->flags, _bitmap_linear_bit))
		{
			long padding;

			match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 506, mipmap_index==0);
			match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 507, !TEST_FLAG(bitmap->flags, _bitmap_compressed_bit));

			padding = -bitmap_mipmap_get_row_pitch(bitmap, mipmap_index) & 63;
			size += bitmap_mipmap_get_height(bitmap, mipmap_index) * padding;
		}

		if (bitmap->type == _bitmap_type_cube_map)
		{
			size /= 6;
		}

		offset += size;
	}

	offset += -offset & 127;

	if (bitmap->type == _bitmap_type_cube_map)
	{
		offset *= 6;
	}

	return offset;
}

void bitmap_swizzle_vector2d(
	short dimension_x,
	short dimension_y,
	short x,
	short y,
	long *swizzled)
{
	short log2_x = floor_log2(dimension_x);
	short log2_y = floor_log2(dimension_y);
	short minimum_log2 = log2_y;
	short mask;
	long swizzled_x, swizzled_y;

	if (log2_x <= log2_y)
	{
		minimum_log2 = log2_x;
	}

	mask = (short)((1 << minimum_log2) - 1);

	if (mask <= 63)
	{
		swizzled_x = swizzle_table[x & mask];
		swizzled_y = 2 * swizzle_table[y & mask];
	}
	else
	{
		long upper_mask = mask >> 6;

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 86, upper_mask<=63);

		swizzled_x = (swizzle_table[(x >> 6) & upper_mask] << 12) | swizzle_table[x & 63];
		swizzled_y = 2 * ((swizzle_table[(y >> 6) & upper_mask] << 12) | swizzle_table[y & 63]);
	}

	if (log2_x > minimum_log2)
	{
		swizzled_x |= (x >> minimum_log2) << (2 * minimum_log2);
	}
	else if (log2_y > minimum_log2)
	{
		swizzled_y |= (y >> minimum_log2) << (2 * minimum_log2);
	}

	swizzled[0] = swizzled_x;
	swizzled[1] = swizzled_y;

	return;
}
void rasterizer_xbox_bitmap_swizzle2d_byte(
	void *dst,
	void const *src,
	short width,
	short height)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	short x, y;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 147, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 148, src);

	compute_swizzle_masks(width, height, 1);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			((byte *)dst)[y_swizzle | x_swizzle] = ((byte const *)src)[source_index];
			source_index++;
			x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
		}

		y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle2d_word(
	void *dst,
	void const *src,
	short width,
	short height)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	short x, y;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 176, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 177, src);

	compute_swizzle_masks(width, height, 1);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			((word *)dst)[y_swizzle | x_swizzle] = ((word const *)src)[source_index];
			source_index++;
			x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
		}

		y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle2d_long(
	void *dst,
	void const *src,
	short width,
	short height)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	short x, y;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 205, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 206, src);

	compute_swizzle_masks(width, height, 1);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			((long *)dst)[y_swizzle | x_swizzle] = ((long const *)src)[source_index];
			source_index++;
			x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
		}

		y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle3d_byte(
	void *dst,
	void const *src,
	short width,
	short height,
	short depth)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	unsigned long z_swizzle = 0;
	short x, y, z;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 235, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 236, src);

	compute_swizzle_masks(width, height, depth);

	for (z = 0; z < depth; z++)
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				((byte *)dst)[z_swizzle | y_swizzle | x_swizzle] = ((byte const *)src)[source_index];
				source_index++;
				x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
			}

			y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
		}

		z_swizzle = (z_swizzle - rasterizer_swizzle_z_mask) & rasterizer_swizzle_z_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle3d_word(
	void *dst,
	void const *src,
	short width,
	short height,
	short depth)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	unsigned long z_swizzle = 0;
	short x, y, z;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 270, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 271, src);

	compute_swizzle_masks(width, height, depth);

	for (z = 0; z < depth; z++)
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				((word *)dst)[z_swizzle | y_swizzle | x_swizzle] = ((word const *)src)[source_index];
				source_index++;
				x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
			}

			y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
		}

		z_swizzle = (z_swizzle - rasterizer_swizzle_z_mask) & rasterizer_swizzle_z_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle3d_long(
	void *dst,
	void const *src,
	short width,
	short height,
	short depth)
{
	long source_index = 0;
	unsigned long x_swizzle = 0;
	unsigned long y_swizzle = 0;
	unsigned long z_swizzle = 0;
	short x, y, z;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 305, dst);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 306, src);

	compute_swizzle_masks(width, height, depth);

	for (z = 0; z < depth; z++)
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				((long *)dst)[z_swizzle | y_swizzle | x_swizzle] = ((long const *)src)[source_index];
				source_index++;
				x_swizzle = (x_swizzle - rasterizer_swizzle_x_mask) & rasterizer_swizzle_x_mask;
			}

			y_swizzle = (y_swizzle - rasterizer_swizzle_y_mask) & rasterizer_swizzle_y_mask;
		}

		z_swizzle = (z_swizzle - rasterizer_swizzle_z_mask) & rasterizer_swizzle_z_mask;
	}

	return;
}

void rasterizer_xbox_bitmap_swizzle(
	struct bitmap_data *bitmap)
{
	short mipmap_index;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 334, bitmap);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 335, bitmap->base_address);

	if (TEST_FLAG(bitmap->flags, _bitmap_compressed_bit) ||
		TEST_FLAG(bitmap->flags, _bitmap_linear_bit))
	{
		return;
	}

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 345, TEST_FLAG(bitmap->flags, _bitmap_has_power_of_two_dimensions_bit));

	for (mipmap_index = 0; mipmap_index <= (short)bitmap->mipmap_count; mipmap_index++)
	{
		long pixel_data_size = bitmap_mipmap_get_pixel_data_size(bitmap, mipmap_index);
		byte *pixels = bitmap_mipmap_address(bitmap, mipmap_index);
		byte *buffer = match_malloc("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 351, pixel_data_size);
		short width = bitmap_mipmap_get_width(bitmap, mipmap_index);
		short height = bitmap_mipmap_get_height(bitmap, mipmap_index);
		short depth = bitmap_mipmap_get_depth(bitmap, mipmap_index);

		if (buffer)
		{
			short bytes_per_pixel = bitmap_format_get_bits_per_pixel(bitmap->format)/8;

			compute_swizzle_masks(width, height, depth);

			switch (bitmap->type)
			{
				case _bitmap_type_2d:
					switch (bytes_per_pixel)
					{
						case 1:
							rasterizer_xbox_bitmap_swizzle2d_byte(buffer, pixels, width, height);
							break;
						case 2:
							rasterizer_xbox_bitmap_swizzle2d_word(buffer, pixels, width, height);
							break;
						case 4:
							rasterizer_xbox_bitmap_swizzle2d_long(buffer, pixels, width, height);
							break;
						default:
							match_vassert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 379, FALSE, "### ERROR unsupported bitmap format (bytes per pixel)");
							break;
					}
					break;

				case _bitmap_type_3d:
					switch (bytes_per_pixel)
					{
						case 1:
							rasterizer_xbox_bitmap_swizzle3d_byte(buffer, pixels, width, height, depth);
							break;
						case 2:
							rasterizer_xbox_bitmap_swizzle3d_word(buffer, pixels, width, height, depth);
							break;
						case 4:
							rasterizer_xbox_bitmap_swizzle3d_long(buffer, pixels, width, height, depth);
							break;
						default:
							match_vassert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 399, FALSE, "### ERROR unsupported bitmap format (bytes per pixel)");
							break;
					}
					break;

				case _bitmap_type_cube_map:
				{
					byte *source = pixels;
					byte *destination = buffer;
					short face_index;

					for (face_index = 0; face_index < NUMBER_OF_FACES_PER_CUBE; face_index++)
					{
						switch (bytes_per_pixel)
						{
							case 1:
								rasterizer_xbox_bitmap_swizzle2d_byte(destination, source, width, height);
								break;
							case 2:
								rasterizer_xbox_bitmap_swizzle2d_word(destination, source, width, height);
								break;
							case 4:
								rasterizer_xbox_bitmap_swizzle2d_long(destination, source, width, height);
								break;
							default:
								match_vassert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 425, FALSE, "### ERROR unsupported bitmap format (bytes per pixel)");
								break;
						}

						destination += pixel_data_size/NUMBER_OF_FACES_PER_CUBE;
						source += pixel_data_size/NUMBER_OF_FACES_PER_CUBE;
					}
					break;
				}

				default:
					match_vassert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 436, FALSE, "### ERROR unsupported bitmap type");
					break;
			}

			memcpy(pixels, buffer, pixel_data_size);
			match_free("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 440, buffer);
			SET_FLAG(bitmap->flags, _bitmap_swizzled_bit, TRUE);
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate temporary buffer for swizzling");
		}
	}

	return;
}

boolean rasterizer_xbox_bitmap_rebuild_hardware_format(
	struct bitmap_data *bitmap)
{
	boolean success = TRUE;
	unsigned long size = rasterizer_xbox_bitmap_get_pixel_data_size(bitmap);
	unsigned long offset = 0;
	short face_count = (bitmap->type == _bitmap_type_cube_map) ? NUMBER_OF_FACES_PER_CUBE : 1;
	byte *buffer;
	short face_index;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 549, bitmap->base_address);

	buffer = match_malloc("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 552, size);

	if (buffer)
	{
		rasterizer_xbox_bitmap_swizzle(bitmap);

		for (face_index = 0; face_index < face_count; face_index++)
		{
			short maximum_mipmap_index = rasterizer_xbox_bitmap_get_max_mipmap_count(bitmap);
			short mipmap_index;

			for (mipmap_index = 0; mipmap_index <= maximum_mipmap_index; mipmap_index++)
			{
				byte *source = bitmap_mipmap_address(bitmap, mipmap_index);
				long mipmap_size = bitmap_mipmap_get_pixel_data_size(bitmap, mipmap_index);
				short adjusted_face_index = face_mapping_inverse_table[face_index];

				if (bitmap->type == _bitmap_type_cube_map)
				{
					mipmap_size /= NUMBER_OF_FACES_PER_CUBE;
				}

				if (TEST_FLAG(bitmap->flags, _bitmap_linear_bit))
				{
					long row_pitch;
					long row_padding;
					short y;

					match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 588, face_index==0 && adjusted_face_index==0);
					match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 589, mipmap_index==0);
					match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 590, !TEST_FLAG(bitmap->flags, _bitmap_compressed_bit));

					row_pitch = bitmap_mipmap_get_row_pitch(bitmap, mipmap_index);
					row_padding = -row_pitch & 63;

					for (y = 0; y < bitmap->height; y++)
					{
						memcpy(buffer+offset, source, row_pitch);
						offset += row_pitch;
						memset(buffer+offset, 0, row_padding);
						source += row_pitch;
						offset += row_padding;
					}
				}
				else
				{
					memcpy(buffer+offset, source+adjusted_face_index*mipmap_size, mipmap_size);
					offset += mipmap_size;
				}
			}

			memset(buffer+offset, 0, -offset & 127);
			offset += -offset & 127;
		}

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 625, offset==size);

		memcpy(bitmap->base_address, buffer, size);
		match_free("c:\\halo\\SOURCE\\rasterizer\\rasterizer_swizzle.c", 629, buffer);
	}
	else
	{
		error(_error_silent, "### ERROR rasterizer_xbox_bitmap_rebuild_hardware_format failed (out of memory)");
		success = FALSE;
	}

	return success;
}

void bitmap_swizzle_vector3d(
	short dimension_x,
	short dimension_y,
	short dimension_z,
	short x,
	short y,
	short z,
	long *swizzled)
{
	long swizzled_x = 0;
	long swizzled_y = 0;
	long swizzled_z = 0;
	short bit_position = 0;
	short previous_bit_position;
	short axis_bit;

	axis_bit = 1;

	do
	{
		previous_bit_position = bit_position;

		if (axis_bit < dimension_x)
		{
			swizzled_x |= (x & 1) << bit_position;
			x >>= 1;
			bit_position++;
		}

		if (axis_bit < dimension_y)
		{
			swizzled_y |= (y & 1) << bit_position;
			y >>= 1;
			bit_position++;
		}

		if (axis_bit < dimension_z)
		{
			swizzled_z |= (z & 1) << bit_position;
			z >>= 1;
			bit_position++;
		}

		axis_bit <<= 1;
	}
	while (previous_bit_position != bit_position);

	swizzled[0] = swizzled_x;
	swizzled[1] = swizzled_y;
	swizzled[2] = swizzled_z;

	return;
}

/* ---------- private code */

static void compute_swizzle_masks(
	short width,
	short height,
	short depth)
{
	unsigned long size = 1;
	unsigned long bit = 1;
	unsigned long advanced;

	rasterizer_swizzle_z_mask = 0;
	rasterizer_swizzle_y_mask = 0;
	rasterizer_swizzle_x_mask = 0;

	do
	{
		advanced = 0;

		if (size < width)
		{
			rasterizer_swizzle_x_mask |= bit;
			bit <<= 1;
			advanced = bit;
		}

		if (size < height)
		{
			rasterizer_swizzle_y_mask |= bit;
			bit <<= 1;
			advanced = bit;
		}

		if (size < depth)
		{
			rasterizer_swizzle_z_mask |= bit;
			bit <<= 1;
			advanced = bit;
		}

		size <<= 1;
	}
	while (advanced);

	return;
}
