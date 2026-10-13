/*
TIFF_FILE.C

symbols in this file:
0006DBE0 0070:
	_tiff_get_bounds (0000)
0006DC50 0420:
	_tiff_export (0000)
0006E070 0540:
	_tiff_import (0000)
00255454 0002:
	??_C@_01KDCPPGHE@r?$AA@ (0000)
00255458 0014:
	??_C@_0BE@FPJPBLAF@failed?5to?5open?5tiff?$AA@ (0000)
0025546C 000e:
	??_C@_0O@BNNCBLEN@out?5of?5memory?$AA@ (0000)
0025547C 0019:
	??_C@_0BJ@EDBMJEAG@failed?5to?5write?5scanline?$AA@ (0000)
00255498 0029:
	??_C@_0CJ@CNFFKPJI@invalid?5bitmap?5encoding?5for?5tiff@ (0000)
002554C4 0023:
	??_C@_0CD@MEPHMAHO@c?3?2halo?2SOURCE?2bitmaps?2tiff_file@ (0000)
002554E8 0002:
	??_C@_01NOFIACDB@w?$AA@ (0000)
002554EC 0014:
	??_C@_0BE@DABJOKOK@file?5does?5not?5exist?$AA@ (0000)
00255500 0010:
	??_C@_0BA@GNLHBLFA@not?5a?5TIFF?5file?$AA@ (0000)
00255510 0030:
	??_C@_0DA@NCFNHNBH@unsupported?5TIFF?5orientation?5?$CImu@ (0000)
00255540 0036:
	??_C@_0DG@CEDAGLPJ@unsupported?5bits?5per?5sample?5?$CI?$CFd?$CJ@ (0000)
00255578 0033:
	??_C@_0DD@OCPJGOKH@unsupported?5TIFF?5photometric?0?5pl@ (0000)
002555AC 000f:
	??_C@_0P@HCMCPNNJ@TIFF?5too?5large?$AA@ (0000)
002555BC 001e:
	??_C@_0BO@DJPDCIEM@failed?5to?5read?5TIFF?5scan?5line?$AA@ (0000)
002555DC 0013:
	??_C@_0BD@NKJFFLKL@unsupported?5format?$AA@ (0000)
0031C320 0200:
	_bss_0031c320 (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps_internal.h"
#include "bitmaps/libtiff/tiffio.h"
#include "bitmaps/tiff_file.h"
#include "math/integer_math.h"
#include "tag_files/files.h"

/* ---------- constants */

enum
{
	_bitmap_format_a8 = 0,
	_bitmap_format_y8 = 1,
	_bitmap_format_ay8 = 2,
	_bitmap_format_r5g6b5 = 6,
	_bitmap_format_a1r5g5b5 = 8,
	_bitmap_format_a4r4g4b4 = 9,
	_bitmap_format_x8r8g8b8 = 10,
	_bitmap_format_a8r8g8b8 = 11,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

static char error_message_buffer[512] = {0};

/* ---------- public code */

boolean
tiff_get_bounds(
	struct file_reference *file,
	unsigned long *width,
	unsigned long *height)
{
	char path[256];
	TIFF *tiff;
	boolean result = FALSE;

	tiff = TIFFOpen(
		file_reference_get_name(file, 0xD, path),
		"r");
	if (tiff)
	{
		TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, width);
		TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, height);
		TIFFClose(tiff);
		result = TRUE;
	}

	return result;
}

char const *
tiff_export(
	struct file_reference *file,
	struct bitmap_data *bitmap)
{
	char const *error_message = NULL;
	short tiff_format;
	short photometric;
	short samples_per_pixel;

	switch (bitmap->format)
	{
	case _bitmap_format_a8:
	case _bitmap_format_y8:
	case _bitmap_format_ay8:
		tiff_format = _bitmap_format_a8;
		photometric = PHOTOMETRIC_MINISBLACK;
		samples_per_pixel = 1;
		break;
	case _bitmap_format_r5g6b5:
	case _bitmap_format_a1r5g5b5:
	case _bitmap_format_a4r4g4b4:
	case _bitmap_format_x8r8g8b8:
	case _bitmap_format_a8r8g8b8:
		tiff_format = _bitmap_format_a8r8g8b8;
		photometric = PHOTOMETRIC_RGB;
		samples_per_pixel = 4;
		break;
	default:
		error_message = "invalid bitmap encoding for tiff export.";
		break;
	}

	if (!error_message)
	{
		char path[256];
		TIFF *tiff = TIFFOpen(file_reference_get_name(file, 0xD, path), "w");

		if (tiff)
		{
			short row_size = bitmap_format_get_bits_per_pixel(tiff_format) * bitmap->width / 8;
			byte *row_buffer = match_malloc("c:\\halo\\SOURCE\\bitmaps\\tiff_file.c", 107, row_size);

			if (row_buffer)
			{
				short y;

				TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, bitmap->width);
				TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, bitmap->height);
				TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
				TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, photometric);
				TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
				TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
				TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);
				TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

				for (y = 0; y < bitmap->height; y++)
				{
					void *source_row = bitmap_2d_address(bitmap, 0, y, 0);

					// TIFF rows are stored red, green, blue, alpha
					switch (bitmap->format)
					{
					case _bitmap_format_a1r5g5b5:
						{
							short x;

							for (x = 0; x < bitmap->width; x++)
							{
								word pixel = ((word *)source_row)[x];

								row_buffer[x * 4 + 3] = 0xFF;
								row_buffer[x * 4 + 2] = ((pixel & 0x1F) << 3) | ((pixel & 0x1F) << 2);
								row_buffer[x * 4 + 1] = (((pixel >> 5) & 0x1F) << 3) | (((pixel >> 5) & 0x1F) << 2);
								row_buffer[x * 4] = ((pixel >> 10) << 3) | ((pixel >> 10) << 2);
							}
						}
						break;

					case _bitmap_format_r5g6b5:
						{
							short x;

							for (x = 0; x < bitmap->width; x++)
							{
								word pixel = ((word *)source_row)[x];

								row_buffer[x * 4 + 3] = 0xFF;
								row_buffer[x * 4 + 2] = ((pixel & 0x1F) << 3) | ((pixel & 0x1F) >> 2);
								row_buffer[x * 4 + 1] = (((pixel >> 5) & 0x3F) << 2) | (((pixel >> 5) & 0x3F) >> 4);
								row_buffer[x * 4] = ((pixel >> 11) << 3) | ((pixel >> 11) >> 2);
							}
						}
						break;

					case _bitmap_format_a4r4g4b4:
						{
							short x;

							for (x = 0; x < bitmap->width; x++)
							{
								word pixel = ((word *)source_row)[x];

								row_buffer[x * 4 + 3] = (((pixel >> 12) & 0xF) << 4) | ((pixel >> 12) & 0xF);
								row_buffer[x * 4 + 2] = ((pixel & 0xF) << 4) | (pixel & 0xF);
								row_buffer[x * 4 + 1] = (((pixel >> 4) & 0xF) << 4) | ((pixel >> 4) & 0xF);
								row_buffer[x * 4] = (((pixel >> 8) & 0xF) << 4) | ((pixel >> 8) & 0xF);
							}
						}
						break;

					case _bitmap_format_x8r8g8b8:
						{
							short x;

							for (x = 0; x < bitmap->width; x++)
							{
								pixel32 pixel = ((pixel32 *)source_row)[x];

								row_buffer[x * 4 + 3] = 0xFF;
								row_buffer[x * 4 + 2] = (byte)pixel;
								row_buffer[x * 4 + 1] = (byte)(pixel >> 8);
								row_buffer[x * 4] = (byte)(pixel >> 16);
							}
						}
						break;

					case _bitmap_format_a8r8g8b8:
						{
							short x;

							for (x = 0; x < bitmap->width; x++)
							{
								pixel32 pixel = ((pixel32 *)source_row)[x];

								row_buffer[x * 4 + 3] = (byte)(pixel >> 24);
								row_buffer[x * 4 + 2] = (byte)pixel;
								row_buffer[x * 4 + 1] = (byte)(pixel >> 8);
								row_buffer[x * 4] = (byte)(pixel >> 16);
							}
						}
						break;

					default:
						csmemcpy(row_buffer, source_row, row_size);
						break;
					}

					if (TIFFWriteScanline(tiff, row_buffer, y, 0) < 0)
					{
						error_message = "failed to write scanline";
						break;
					}
				}

				match_free("c:\\halo\\SOURCE\\bitmaps\\tiff_file.c", 231, row_buffer);
			}
			else
			{
				error_message = "out of memory";
			}

			TIFFClose(tiff);
		}
		else
		{
			error_message = "failed to open tiff";
		}
	}

	return error_message;
}

char const *
tiff_import(
	struct file_reference *file,
	struct bitmap_data **bitmap_result,
	rectangle2d const *requested_bounds,
	short format)
{
	char const *error_message = NULL;

	if (file_exists(file))
	{
		char path[256];
		TIFF *tiff = TIFFOpen(file_reference_get_name(file, 0xD, path), "r");

		if (tiff)
		{
			long scanline_size = TIFFScanlineSize(tiff);
			word bits_per_sample;
			word orientation;
			word samples_per_pixel;
			word planar_config;
			word photometric;
			unsigned long image_width;
			unsigned long image_height;
			rectangle2d bounds;

			TIFFGetFieldDefaulted(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
			TIFFGetFieldDefaulted(tiff, TIFFTAG_ORIENTATION, &orientation);
			TIFFGetFieldDefaulted(tiff, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
			TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &planar_config);
			TIFFGetField(tiff, TIFFTAG_PHOTOMETRIC, &photometric);
			TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &image_width);
			TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &image_height);

			if (requested_bounds)
			{
				bounds = *requested_bounds;
			}
			else
			{
				bounds.y0 = 0;
				bounds.x0 = 0;
				bounds.x1 = (short)image_width;
				bounds.y1 = (short)image_height;
			}

			if (orientation == ORIENTATION_TOPLEFT)
			{
				if (bits_per_sample == 8 &&
					(samples_per_pixel == 4 || samples_per_pixel == 3 || samples_per_pixel == 2 || samples_per_pixel == 1))
				{
					if (format != NONE && format != _bitmap_format_a8r8g8b8)
					{
						error_message = "unsupported format";
					}
					else if (planar_config == PLANARCONFIG_CONTIG)
					{
						short width = rectangle2d_width(&bounds);
						short height = rectangle2d_height(&bounds);

						if (width >= 0 && width <= 30000 && height >= 0 && height <= 30000)
						{
							struct bitmap_data *bitmap = bitmap_2d_new(width, height, 0, _bitmap_format_a8r8g8b8);
							byte *scanline = match_malloc("c:\\halo\\SOURCE\\bitmaps\\tiff_file.c", 319, scanline_size);

							if (bitmap && scanline)
							{
								short y;

								*bitmap_result = bitmap;
								for (y = bounds.y0; y < bounds.y1; y++)
								{
									if (TIFFReadScanline(tiff, scanline, PIN(y, 0, image_height - 1), 0) < 0)
									{
										error_message = "failed to read TIFF scan line";
										break;
									}

									switch (samples_per_pixel)
									{
									case 1:
										{
											pixel32 *destination = bitmap_2d_address(bitmap, 0, (short)(y - bounds.y0), 0);
											short x;

											for (x = bounds.x0; x < bounds.x1; x++)
											{
												short source_x = PIN(x, 0, image_width - 1);
												byte value = scanline[source_x];

												destination[x - bounds.x0] = (value << 24) | (value << 16) | (value << 8) | value;
											}
										}
										break;
									case 2:
										{
											pixel32 *destination = bitmap_2d_address(bitmap, 0, (short)(y - bounds.y0), 0);
											short x;

											for (x = bounds.x0; x < bounds.x1; x++)
											{
												short source_x = PIN(x, 0, image_width - 1);
												byte *pixel = scanline + source_x * 2;

												destination[x - bounds.x0] = (pixel[1] << 24) | (pixel[0] << 16) | (pixel[0] << 8) | pixel[0];
											}
										}
										break;
									case 3:
										{
											pixel32 *destination = bitmap_2d_address(bitmap, 0, (short)(y - bounds.y0), 0);
											short x;

											for (x = bounds.x0; x < bounds.x1; x++)
											{
												short source_x = PIN(x, 0, image_width - 1);
												byte *pixel = scanline + source_x * 3;

												destination[x - bounds.x0] = 0xFF000000 | (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
											}
										}
										break;
									case 4:
										{
											pixel32 *destination = bitmap_2d_address(bitmap, 0, (short)(y - bounds.y0), 0);
											short x;

											for (x = bounds.x0; x < bounds.x1; x++)
											{
												short source_x = PIN(x, 0, image_width - 1);
												byte *pixel = scanline + source_x * 4;

												destination[x - bounds.x0] = (pixel[3] << 24) | (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
											}
										}
										break;
									default:
										match_vassert("c:\\halo\\SOURCE\\bitmaps\\tiff_file.c", 406, FALSE, NULL);
									}
								}
							}
							else
							{
								error_message = "out of memory";
							}

							// a failed scan line still leaves *bitmap_result pointing at the deleted bitmap
							if (error_message && bitmap)
							{
								bitmap_delete(bitmap);
							}
							if (scanline)
							{
								match_free("c:\\halo\\SOURCE\\bitmaps\\tiff_file.c", 422, scanline);
							}
						}
						else
						{
							error_message = "TIFF too large";
						}
					}
					else
					{
						error_message = "unsupported TIFF photometric, planar configuration";
					}
				}
				else
				{
					_snprintf(
						error_message_buffer,
						NUMBEROF(error_message_buffer),
						"unsupported bits per sample (%d) or sample count (%d)",
						bits_per_sample,
						samples_per_pixel);
					error_message = error_message_buffer;
				}
			}
			else
			{
				error_message = "unsupported TIFF orientation (must be top left)";
			}

			TIFFClose(tiff);
		}
		else
		{
			error_message = "not a TIFF file";
		}
	}
	else
	{
		error_message = "file does not exist";
	}

	return error_message;
}

/* ---------- private code */
