/*
TARGA_FILE.C

symbols in this file:
0006DA10 01d0:
	_targa_export (0000)
002553B8 0013:
	??_C@_0BD@IPEDCPOO@couldn?8t?5open?5file?$AA@ (0000)
002553CC 0016:
	??_C@_0BG@IOECLONJ@couldn?8t?5write?5header?$AA@ (0000)
002553E4 0013:
	??_C@_0BD@HEEBDIJJ@couldn?8t?5write?5row?$AA@ (0000)
002553F8 0007:
	??_C@_06CBIFBKHG@pixels?$AA@ (0000)
00255400 0028:
	??_C@_0CI@PPKFLDJK@bitmap?9?$DOformat?$DN?$DN_bitmap_format_x@ (0000)
00255428 0005:
	??_C@_04DAMGJPCA@file?$AA@ (0000)
00255430 0024:
	??_C@_0CE@ONPLCIIE@c?3?2halo?2SOURCE?2bitmaps?2targa_fil@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "bitmaps/bitmap_group.h"
#include "tag_files/files.h"

/* ---------- constants */

enum
{
	_bitmap_type_2d = 0,
	_bitmap_format_x8r8g8b8 = 10,
};

/* ---------- macros */

/* ---------- structures */

#pragma pack(push, 1)
struct targa_header
{
	byte id_length;
	byte color_map_type;
	byte image_type;
	word color_map_first_entry;
	word color_map_length;
	byte color_map_entry_size;
	word x_origin;
	word y_origin;
	word width;
	word height;
	byte pixel_depth;
	byte image_descriptor;
};
#pragma pack(pop)

/* ---------- prototypes */

void *bitmap_2d_address(struct bitmap_data *bitmap, short x, short y, short mipmap_index);

/* ---------- globals */

/* ---------- public code */

char const *
targa_export(struct file_reference *file, struct bitmap_data *bitmap)
{
	char const *error_message = NULL;
	struct targa_header header;
	long row_size;
	long y;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\targa_file.c", 36, file);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\targa_file.c", 37, bitmap);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\targa_file.c", 38, bitmap->type==_bitmap_type_2d);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\targa_file.c", 39, bitmap->format==_bitmap_format_x8r8g8b8);

	if (!file_create(file) || !file_open(file, FLAG(_permission_write_bit)))
		return "couldn't open file";

	csmemset(&header, 0, sizeof(header));
	header.id_length = 0;
	header.color_map_type = 0;
	header.image_type = 2;
	header.x_origin = 0;
	header.y_origin = 0;
	header.width = bitmap->width;
	header.height = bitmap->height;
	header.pixel_depth = 32;
	header.image_descriptor = 40;

	if (file_write(file, sizeof(header), &header))
	{
		row_size = bitmap->width * sizeof(unsigned long);
		for (y = 0; y < bitmap->height; y++)
		{
			unsigned long *pixels = bitmap_2d_address(bitmap, 0, (short)y, 0);

			match_assert("c:\\halo\\SOURCE\\bitmaps\\targa_file.c", 67, pixels);
			if (!file_write(file, row_size, pixels))
			{
				error_message = "couldn't write row";
				break;
			}
		}
	}
	else
	{
		error_message = "couldn't write header";
	}

	file_close(file);
	return error_message;
}

/* ---------- private code */
