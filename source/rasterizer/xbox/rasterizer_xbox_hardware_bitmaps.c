/*
RASTERIZER_XBOX_HARDWARE_BITMAPS.C

symbols in this file:
00157A80 0020:
	_IDirect3DDevice8_CreateTexture@32 (0000)
00157AA0 0030:
	_IDirect3DDevice8_CreateVolumeTexture@36 (0000)
00157AD0 0020:
	_IDirect3DDevice8_CreateCubeTexture@28 (0000)
00157AF0 0190:
	_rasterizer_bitmap_new (0000)
00157C80 0010:
	_IDirect3DBaseTexture8_Release@4 (0000)
00157C90 0010:
	_D3DTexture_UnlockRect@8 (0000)
00157CA0 0020:
	_IDirect3DTexture8_LockRect@20 (0000)
00157CC0 0010:
	_IDirect3DTexture8_UnlockRect@8 (0000)
00157CD0 0010:
	_D3DVolumeTexture_UnlockBox@8 (0000)
00157CE0 0020:
	_IDirect3DVolumeTexture8_LockBox@20 (0000)
00157D00 0010:
	_IDirect3DVolumeTexture8_UnlockBox@8 (0000)
00157D10 0010:
	_D3DCubeTexture_UnlockRect@12 (0000)
00157D20 0020:
	_IDirect3DCubeTexture8_LockRect@24 (0000)
00157D40 0010:
	_IDirect3DCubeTexture8_UnlockRect@12 (0000)
00157D50 01c0:
	_rasterizer_bitmap_2d_changed (0000)
00157F10 0210:
	_rasterizer_bitmap_3d_changed (0000)
00158120 0210:
	_rasterizer_bitmap_cm_changed (0000)
00158330 0030:
	_rasterizer_bitmap_delete (0000)
00158360 00b0:
	_rasterizer_bitmap_changed (0000)
00290958 0048:
	_rasterizer_bitmap_format_table (0000)
002909A0 000c:
	_face_mapping_table (0000)
002909AC 0032:
	??_C@_0DC@CBFADDNN@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5bitma@ (0000)
002909E0 00db:
	??_C@_0NL@DPJLAHAC@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
00290AC0 00f6:
	??_C@_0PG@MPKBLAC@IDirect3DDevice8_CreateVolumeTex@ (0000)
00290BB8 00d3:
	??_C@_0ND@BNAAHDML@IDirect3DDevice8_CreateCubeTextu@ (0000)
00290C90 0042:
	??_C@_0EC@OHIAJMON@TEST_FLAG?$CIbitmap?9?$DOflags?0?5_bitmap@ (0000)
00290CD8 0042:
	??_C@_0EC@HHGHOJEI@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
00290D1C 0032:
	??_C@_0DC@IBMOBLMK@?$CD?$CD?$CD?5ERROR?5failed?5to?5change?5bitma@ (0000)
00290D50 004c:
	??_C@_0EM@EMDOPEPF@?$CD?$CD?$CD?5ERROR?5uncompressed?5bitmap?5fo@ (0000)
00290D9C 0021:
	??_C@_0CB@OPBPBKN@?$CD?$CD?$CD?5ERROR?5failed?5to?5lock?5surface@ (0000)
00290DC0 0083:
	??_C@_0ID@FMIMKLDC@IDirect3DTexture8_LockRect?$CI?$CIIDir@ (0000)
00290E48 008d:
	??_C@_0IN@DNCNKHPL@IDirect3DVolumeTexture8_LockBox?$CI@ (0000)
00290ED8 00ab:
	??_C@_0KL@IPKDNHEL@IDirect3DCubeTexture8_LockRect?$CI?$CI@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps_mipmap.h"
#include "cache/texture_cache.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_swizzle.h"
#include "rasterizer/xbox/rasterizer_xbox_hardware_bitmaps.h"
#include <xtl.h>
#include "rasterizer/xbox/rasterizer_xbox.h"

/* ---------- constants */

enum
{
	NUMBER_OF_BITMAP_FORMATS = 18,
	_bitmap_type_2d = 0,
	_bitmap_type_3d,
	_bitmap_type_cube_map,
	_bitmap_has_power_of_two_dimensions_bit = 0,
	_bitmap_compressed_bit = 1,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void rasterizer_bitmap_2d_changed(
	struct bitmap_data *bitmap);
static void rasterizer_bitmap_3d_changed(
	struct bitmap_data *bitmap);
static void rasterizer_bitmap_cm_changed(
	struct bitmap_data *bitmap);

/* ---------- globals */

static D3DFORMAT const rasterizer_bitmap_format_table[NUMBER_OF_BITMAP_FORMATS] =
{
	D3DFMT_A8,
	D3DFMT_L8,
	D3DFMT_AL8,
	D3DFMT_A8L8,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_R5G6B5,
	D3DFMT_UNKNOWN,
	D3DFMT_A1R5G5B5,
	D3DFMT_A4R4G4B4,
	D3DFMT_X8R8G8B8,
	D3DFMT_A8R8G8B8,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
	D3DFMT_P8,
};

static short const face_mapping_table[NUMBER_OF_FACES_PER_CUBE] =
{
	D3DCUBEMAP_FACE_POSITIVE_X,
	D3DCUBEMAP_FACE_POSITIVE_Y,
	D3DCUBEMAP_FACE_NEGATIVE_X,
	D3DCUBEMAP_FACE_NEGATIVE_Y,
	D3DCUBEMAP_FACE_POSITIVE_Z,
	D3DCUBEMAP_FACE_NEGATIVE_Z,
};

/* ---------- public code */

boolean rasterizer_bitmap_new(
	struct bitmap_data *bitmap)
{
	boolean success = TRUE;
	long result;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		51,
		bitmap);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		52,
		TEST_FLAG(bitmap->flags, _bitmap_has_power_of_two_dimensions_bit));

	bitmap->mipmap_count =
		rasterizer_xbox_bitmap_get_max_mipmap_count(bitmap);
	if (global_d3d_device)
	{
		switch (bitmap->type)
		{
		case _bitmap_type_2d:
			result = IDirect3DDevice8_CreateTexture(
				global_d3d_device,
				bitmap->width,
				bitmap->height,
				bitmap->mipmap_count + 1,
				0,
				rasterizer_bitmap_format_table[bitmap->format],
				D3DPOOL_MANAGED,
				(IDirect3DTexture8 **)&bitmap->hardware_format);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_CreateTexture(global_d3d_device, bitmap->width, bitmap->height, bitmap->mipmap_count+1, 0, rasterizer_bitmap_format_table[bitmap->format], D3DPOOL_MANAGED, &(IDirect3DTexture8*)bitmap->hardware_format)");
			}
			break;

		case _bitmap_type_3d:
			result = IDirect3DDevice8_CreateVolumeTexture(
				global_d3d_device,
				bitmap->width,
				bitmap->height,
				bitmap->depth,
				bitmap->mipmap_count + 1,
				0,
				rasterizer_bitmap_format_table[bitmap->format],
				D3DPOOL_MANAGED,
				(IDirect3DVolumeTexture8 **)&bitmap->hardware_format);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_CreateVolumeTexture(global_d3d_device, bitmap->width, bitmap->height, bitmap->depth, bitmap->mipmap_count+1, 0, rasterizer_bitmap_format_table[bitmap->format], D3DPOOL_MANAGED, &(IDirect3DVolumeTexture8*)bitmap->hardware_format)");
			}
			break;

		case _bitmap_type_cube_map:
			result = IDirect3DDevice8_CreateCubeTexture(
				global_d3d_device,
				bitmap->width,
				bitmap->mipmap_count + 1,
				0,
				rasterizer_bitmap_format_table[bitmap->format],
				D3DPOOL_MANAGED,
				(IDirect3DCubeTexture8 **)&bitmap->hardware_format);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_CreateCubeTexture(global_d3d_device, bitmap->width, bitmap->mipmap_count+1, 0, rasterizer_bitmap_format_table[bitmap->format], D3DPOOL_MANAGED, &(IDirect3DCubeTexture8*)bitmap->hardware_format)");
			}
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
				91,
				FALSE,
				"### ERROR unsupported bitmap type");
			break;
		}

		if (!bitmap->hardware_format)
			success = FALSE;
		if (!success)
			bitmap->hardware_format = NULL;
	}
	else
	{
		bitmap->hardware_format = NULL;
	}

	if (!success)
	{
		error(
			_error_silent,
			"### ERROR failed to create bitmap hardware format");
	}
	return success;
}

/* ---------- private code */

static void rasterizer_bitmap_2d_changed(
	struct bitmap_data *bitmap)
{
	D3DLOCKED_RECT d3d_locked_rect;
	short width;
	short height;
	short mipmap_index;
	void *source;
	void *destination;
	boolean success = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		0x8D,
		bitmap);

	if (global_d3d_device &&
		bitmap->base_address &&
		bitmap->hardware_format)
	{
		for (mipmap_index = 0;
			success && mipmap_index <= bitmap->mipmap_count;
			mipmap_index++)
		{
			if (IDirect3DTexture8_LockRect(
					(IDirect3DTexture8 *)bitmap->hardware_format,
					mipmap_index,
					&d3d_locked_rect,
					NULL,
					D3DLOCK_NOOVERWRITE) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DTexture8_LockRect((IDirect3DTexture8*)bitmap->hardware_format, mipmap_index, &d3d_locked_rect, NULL, D3DLOCK_NOOVERWRITE)");
			}

			if (success && d3d_locked_rect.pBits)
			{
				source = bitmap_mipmap_address(bitmap, mipmap_index);
				destination = d3d_locked_rect.pBits;
				width = bitmap_mipmap_get_width(bitmap, mipmap_index);
				height = bitmap_mipmap_get_height(bitmap, mipmap_index);
				if (TEST_FLAG(bitmap->flags, _bitmap_compressed_bit))
				{
					csmemcpy(
						destination,
						source,
						bitmap_mipmap_get_pixel_data_size(
							bitmap,
							mipmap_index));
				}
				else
				{
					switch (bitmap_format_get_bits_per_pixel(
						bitmap->format) / 8)
					{
					case 4:
						rasterizer_xbox_bitmap_swizzle2d_long(
							destination,
							source,
							width,
							height);
						break;

					case 2:
						rasterizer_xbox_bitmap_swizzle2d_word(
							destination,
							source,
							width,
							height);
						break;

					case 1:
						rasterizer_xbox_bitmap_swizzle2d_byte(
							destination,
							source,
							width,
							height);
						break;

					default:
						match_vassert(
							"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
							0xB1,
							FALSE,
							"### ERROR uncompressed bitmap format does not have 1,2 or 4 bytes per pixel");
						break;
					}
				}

				if (IDirect3DTexture8_UnlockRect(
						(IDirect3DTexture8 *)bitmap->hardware_format,
						mipmap_index) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DTexture8_UnlockRect((IDirect3DTexture8*)bitmap->hardware_format, mipmap_index)");
				}
			}
			else
			{
				error(
					_error_silent,
					"### ERROR failed to lock surface");
				success = FALSE;
			}
		}

		if (!success)
		{
			error(
				_error_silent,
				"### ERROR failed to change bitmap hardware format");
		}
	}

	return;
}

static void rasterizer_bitmap_3d_changed(
	struct bitmap_data *bitmap)
{
	D3DLOCKED_BOX d3d_locked_box;
	short width;
	short height;
	short depth;
	short mipmap_index;
	short slice_index;
	long slice_size;
	byte *source;
	byte *destination;
	boolean success = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		0xCB,
		bitmap);

	if (global_d3d_device &&
		bitmap->base_address &&
		bitmap->hardware_format)
	{
		for (mipmap_index = 0;
			success && mipmap_index <= bitmap->mipmap_count;
			mipmap_index++)
		{
			if (IDirect3DVolumeTexture8_LockBox(
					(IDirect3DVolumeTexture8 *)bitmap->hardware_format,
					mipmap_index,
					&d3d_locked_box,
					NULL,
					D3DLOCK_NOOVERWRITE) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DVolumeTexture8_LockBox((IDirect3DVolumeTexture8*)bitmap->hardware_format, mipmap_index, &d3d_locked_box, NULL, D3DLOCK_NOOVERWRITE)");
			}

			if (success && d3d_locked_box.pBits)
			{
				source = bitmap_mipmap_address(bitmap, mipmap_index);
				destination = d3d_locked_box.pBits;
				width = bitmap_mipmap_get_width(bitmap, mipmap_index);
				height = bitmap_mipmap_get_height(bitmap, mipmap_index);
				depth = bitmap_mipmap_get_depth(bitmap, mipmap_index);
				if (TEST_FLAG(bitmap->flags, _bitmap_compressed_bit))
				{
					if (depth > 0)
					{
						for (slice_index = 0;
							slice_index < depth;
							slice_index++)
						{
							slice_size =
								bitmap_mipmap_get_pixel_data_size(
									bitmap,
									mipmap_index) / depth;
							csmemcpy(
								destination,
								source,
								slice_size);
							source += slice_size;
							destination += d3d_locked_box.SlicePitch;
						}
					}
				}
				else
				{
					switch (bitmap_format_get_bits_per_pixel(
						bitmap->format) / 8)
					{
					case 4:
						rasterizer_xbox_bitmap_swizzle3d_long(
							destination,
							source,
							width,
							height,
							depth);
						break;

					case 2:
						rasterizer_xbox_bitmap_swizzle3d_word(
							destination,
							source,
							width,
							height,
							depth);
						break;

					case 1:
						rasterizer_xbox_bitmap_swizzle3d_byte(
							destination,
							source,
							width,
							height,
							depth);
						break;

					default:
						match_vassert(
							"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
							0xF9,
							FALSE,
							"### ERROR uncompressed bitmap format does not have 1,2 or 4 bytes per pixel");
						break;
					}
				}

				if (IDirect3DVolumeTexture8_UnlockBox(
						(IDirect3DVolumeTexture8 *)bitmap->hardware_format,
						mipmap_index) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DVolumeTexture8_UnlockBox((IDirect3DVolumeTexture8*)bitmap->hardware_format, mipmap_index)");
				}
			}
			else
			{
				error(
					_error_silent,
					"### ERROR failed to lock surface");
				success = FALSE;
			}
		}

		if (!success)
		{
			error(
				_error_silent,
				"### ERROR failed to change bitmap hardware format");
		}
	}

	return;
}

static void rasterizer_bitmap_cm_changed(
	struct bitmap_data *bitmap)
{
	D3DLOCKED_RECT d3d_locked_rect;
	short face_index;
	short width;
	short height;
	short mipmap_index;
	void *source;
	void *destination;
	boolean success = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		0x114,
		bitmap);

	if (global_d3d_device &&
		bitmap->base_address &&
		bitmap->hardware_format)
	{
		for (mipmap_index = 0;
			success && mipmap_index <= bitmap->mipmap_count;
			mipmap_index++)
		{
			for (face_index = 0;
				success && face_index < NUMBER_OF_FACES_PER_CUBE;
				face_index++)
			{
				if (IDirect3DCubeTexture8_LockRect(
						(IDirect3DCubeTexture8 *)bitmap->hardware_format,
						face_mapping_table[face_index],
						mipmap_index,
						&d3d_locked_rect,
						NULL,
						D3DLOCK_NOOVERWRITE) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DCubeTexture8_LockRect((IDirect3DCubeTexture8*)bitmap->hardware_format, face_mapping_table[face_index], mipmap_index, &d3d_locked_rect, NULL, D3DLOCK_NOOVERWRITE)");
				}

				if (success && d3d_locked_rect.pBits)
				{
					source = bitmap_cube_map_address(
						bitmap,
						0,
						0,
						face_index,
						mipmap_index);
					destination = d3d_locked_rect.pBits;
					width = bitmap_mipmap_get_width(bitmap, mipmap_index);
					height = bitmap_mipmap_get_height(bitmap, mipmap_index);
					if (TEST_FLAG(bitmap->flags, _bitmap_compressed_bit))
					{
						csmemcpy(
							destination,
							source,
							bitmap_mipmap_get_pixel_data_size(
								bitmap,
								mipmap_index) /
								NUMBER_OF_FACES_PER_CUBE);
					}
					else
					{
						switch (bitmap_format_get_bits_per_pixel(
							bitmap->format) / 8)
						{
						case 4:
							rasterizer_xbox_bitmap_swizzle2d_long(
								destination,
								source,
								width,
								height);
							break;

						case 2:
							rasterizer_xbox_bitmap_swizzle2d_word(
								destination,
								source,
								width,
								height);
							break;

						case 1:
							rasterizer_xbox_bitmap_swizzle2d_byte(
								destination,
								source,
								width,
								height);
							break;

						default:
							match_vassert(
								"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
								0x13F,
								FALSE,
								"### ERROR uncompressed bitmap format does not have 1,2 or 4 bytes per pixel");
							break;
						}
					}

					if (IDirect3DCubeTexture8_UnlockRect(
							(IDirect3DCubeTexture8 *)bitmap->hardware_format,
							face_mapping_table[face_index],
							mipmap_index) >= 0 && success)
					{
						success = TRUE;
					}
					else
					{
						success = FALSE;
						rasterizer_error(
							0,
							"IDirect3DCubeTexture8_UnlockRect((IDirect3DCubeTexture8*)bitmap->hardware_format, face_mapping_table[face_index], mipmap_index)");
					}
				}
				else
				{
					error(
						_error_silent,
						"### ERROR failed to lock surface");
					success = FALSE;
				}
			}
		}

		if (!success)
		{
			error(
				_error_silent,
				"### ERROR failed to change bitmap hardware format");
		}
	}

	return;
}

/* ---------- public code */

void rasterizer_bitmap_delete(
	struct bitmap_data *bitmap)
{
	texture_cache_bitmap_delete(bitmap);
	if (bitmap && bitmap->hardware_format)
	{
		IDirect3DBaseTexture8_Release(
			(IDirect3DBaseTexture8 *)bitmap->hardware_format);
		bitmap->hardware_format = NULL;
	}

	return;
}

void rasterizer_bitmap_changed(
	struct bitmap_data *bitmap)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
		0x70,
		bitmap);

	rasterizer_globals.current_lock_operation = _rasterizer_lock_texture_changed;
	switch (bitmap->type)
	{
	case _bitmap_type_2d:
		rasterizer_bitmap_2d_changed(bitmap);
		break;

	case _bitmap_type_3d:
		rasterizer_bitmap_3d_changed(bitmap);
		break;

	case _bitmap_type_cube_map:
		rasterizer_bitmap_cm_changed(bitmap);
		break;

	default:
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_bitmaps.c",
			0x80,
			FALSE,
			"### ERROR unsupported bitmap type");
		break;
	}
	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	return;
}
