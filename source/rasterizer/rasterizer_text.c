/*
RASTERIZER_TEXT.C

symbols in this file:
00172E80 0010:
	_code_00172e80 (0000)
00172E90 0010:
	_code_00172e90 (0000)
00172EA0 0090:
	_rasterizer_text_cache_initialize (0000)
00172F30 0010:
	_rasterizer_text_set_shadow_color (0000)
00172F40 0030:
	_rasterizer_text_cache_flush (0000)
00172F70 0030:
	_rasterizer_text_cache_dispose (0000)
00172FA0 0020:
	_code_00172fa0 (0000)
00172FC0 00b0:
	_code_00172fc0 (0000)
00173070 0060:
	_code_00173070 (0000)
001730D0 0380:
	_code_001730d0 (0000)
00173450 00f0:
	_code_00173450 (0000)
00173540 0170:
	_code_00173540 (0000)
001736B0 0200:
	_rasterizer_draw_string (0000)
001738B0 0200:
	_rasterizer_draw_unicode_string (0000)
0029EEE0 0033:
	??_C@_0DD@DKOHMJNA@?$CD?$CD?$CD?5ERROR?5failed?5to?5initialize?5h@ (0000)
0029EF14 0026:
	??_C@_0CG@HPKDNNGC@?$CBhardware_character_cache?4initia@ (0000)
0029EF3C 002c:
	??_C@_0CM@KJINBGGM@c?3?2halo?2SOURCE?2rasterizer?2raster@ (0000)
0029EF68 0009:
	??_C@_08KDNNBGOA@x0?5?$CG?$CG?5y0?$AA@ (0000)
0029EF78 0054:
	??_C@_0FE@BCPFIEIE@hardware_character_index?$DO?$DN0?5?$CG?$CG?5h@ (0000)
0029EFCC 0025:
	??_C@_0CF@POBHCEMM@hardware_character_cache?4initial@ (0000)
0029EFF4 0026:
	??_C@_0CG@JHCOKPHL@font?5cache?5overwrote?5character?5i@ (0000)
0029F01C 0013:
	??_C@_0BD@PIEBJAO@hardware_character?$AA@ (0000)
0029F030 0046:
	??_C@_0EG@CBPAFGHN@font_character?9?$DObitmap_height?$DM?$DNH@ (0000)
0029F078 0044:
	??_C@_0EE@KGLMBKON@font_character?9?$DObitmap_width?$DM?$DNHA@ (0000)
0029F0C0 0068:
	??_C@_0GI@IDEJCHPO@font_character?$DN?$DNhardware_charact@ (0000)
0029F128 0074:
	??_C@_0HE@KFKECHAF@font_character?9?$DOhardware_charact@ (0000)
0030D4D0 0002:
	_data_0030d4d0 (0000)
004B82C0 0816:
	_bss_004b82c0 (0000)
*/


/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps_internal.h"
#include "math/integer_math.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_debug_options.h"
#include "rasterizer/rasterizer_text.h"
#include "rasterizer/xbox/rasterizer_xbox_hardware_bitmaps.h"
#include "render/render.h"
#include "text/font_group.h"
#include "text/unicode.h"

/* ---------- constants */

enum
{
#ifdef HALO_XBOX
	/* four times January's cache: the emulator's GPU runs frames behind
	the CPU, and a slot recycled within that lag draws the wrong glyph; a
	frame's glyphs (the debug console's included) now fit without churn */
	HARDWARE_CHARACTER_CACHE_BITMAP_WIDTH = 256,
	HARDWARE_CHARACTER_CACHE_BITMAP_HEIGHT = 256,
	MAXIMUM_HARDWARE_CHARACTERS = 1024,
#else
	HARDWARE_CHARACTER_CACHE_BITMAP_WIDTH = 128,
	HARDWARE_CHARACTER_CACHE_BITMAP_HEIGHT = 128,
	MAXIMUM_HARDWARE_CHARACTERS = 256,
#endif
};

enum
{
	_bitmap_format_a4r4g4b4 = 9,
};

enum
{
	_rasterizer_target_render_primary = 0,
	_shader_framebuffer_blend_function_alpha_blend = 0,
};

/* ---------- macros */

/* ---------- structures */

struct font_character
{
	word character;
	short character_width;
	short bitmap_width;
	short bitmap_height;
	short bitmap_origin_x;
	short bitmap_origin_y;
	short hardware_character_index;
	short pad;
	long pixels_offset;
};

struct parse_string_state;

typedef void (*draw_character_proc)(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy);

struct hardware_character
{
	struct font_character *character;
	short x0;
	short y0;
};

struct hardware_character_cache
{
	boolean initialized;
	byte unused1;
	short read_index;
	short write_index;
	short x0;
	short y0;
	short maximum_character_height;
	struct bitmap_data *bitmap;
	struct hardware_character characters[MAXIMUM_HARDWARE_CHARACTERS];
};

/* ---------- prototypes */

struct bitmap_data *hardware_character_cache_get_bitmap(
	void);
void draw_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	char const *string);
void draw_unicode_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	wchar_t const *string);
static void hardware_character_cache_get_origin(
	short hardware_character_index,
	short *x0,
	short *y0);
static void flush_hardware_character(
	struct hardware_character *hardware_character);
static void cache_hardware_format_character(
	struct font_header *font,
	struct font_character *font_character);
void rasterizer_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy);
static void rasterizer_draw_character_with_dropshadow(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy);

/* ---------- globals */

#ifdef HALO_XBOX
/* the Xbox build's Direct3D device (port/xbox/src/d3d8_nv2a.c): waits for
the GPU to finish everything queued */
void halo_xbox_gpu_drain(void);
#endif

extern struct rasterizer_window_begin_parameters global_window_parameters;

static struct hardware_character_cache hardware_character_cache;
static pixel32 global_shadow_color = 0;
static short rasterizer_text_unused = 0;
static short magic_number= 12;

/* ---------- public code */

void lock_rasterizer_text_data(
	void)
{
	return;
}

void unlock_rasterizer_text_data(
	void)
{
	return;
}

boolean
rasterizer_text_cache_initialize(
	void)
{
	struct bitmap_data *bitmap;
	boolean success = TRUE;

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 118, !hardware_character_cache.initialized);

	bitmap = bitmap_2d_new(
		HARDWARE_CHARACTER_CACHE_BITMAP_WIDTH,
		HARDWARE_CHARACTER_CACHE_BITMAP_HEIGHT,
		0,
		_bitmap_format_a4r4g4b4);

	if (bitmap)
	{
		memset(&hardware_character_cache, 0, sizeof(hardware_character_cache));

		if (rasterizer_bitmap_new(bitmap))
		{
			hardware_character_cache.bitmap = bitmap;
			hardware_character_cache.initialized = TRUE;
		}
		else
		{
			error(_error_silent, "### ERROR failed to initialize hardware text cache");
			success = FALSE;
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to initialize hardware text cache");
		success = FALSE;
	}

	return success;
}

void
rasterizer_text_set_shadow_color(
	pixel32 shadow_color)
{
	global_shadow_color = shadow_color;

	return;
}

void
rasterizer_text_cache_flush(
	void)
{
	struct hardware_character *hardware_character;
	long hardware_character_count;

	if (hardware_character_cache.initialized)
	{
		hardware_character = hardware_character_cache.characters;
		hardware_character_count = MAXIMUM_HARDWARE_CHARACTERS;
		do
		{
			if (hardware_character->character)
				hardware_character->character->hardware_character_index = NONE;
			hardware_character->character = NULL;
			hardware_character++;
		} while (--hardware_character_count);
	}

	return;
}

void
rasterizer_text_cache_dispose(
	void)
{
	if (hardware_character_cache.initialized)
	{
		rasterizer_text_cache_flush();
		bitmap_delete(hardware_character_cache.bitmap);
		hardware_character_cache.initialized = FALSE;
	}

	return;
}

void
rasterizer_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy)
{
	cache_hardware_format_character(font, font_character);

	if (font_character->hardware_character_index != NONE)
	{
		struct dynamic_screen_vertex vertices[NUMBER_OF_VERTICES_PER_QUADRILATERAL];
		short u0, v0;

		hardware_character_cache_get_origin(font_character->hardware_character_index, &u0, &v0);

		u0 += x;
		v0 += y;

		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

		vertices[0].position.x = vertices[3].position.x = (real)x0;
		vertices[1].position.x = vertices[2].position.x = (real)(x0 + dx);
		vertices[0].position.y = vertices[1].position.y = (real)y0;
		vertices[2].position.y = vertices[3].position.y = (real)(y0 + dy);

		vertices[0].texture_coordinates.x = vertices[3].texture_coordinates.x = (real)u0;
		vertices[1].texture_coordinates.x = vertices[2].texture_coordinates.x = (real)(u0 + dx);
		vertices[0].texture_coordinates.y = vertices[1].texture_coordinates.y = (real)v0;
		vertices[2].texture_coordinates.y = vertices[3].texture_coordinates.y = (real)(v0 + dy);

		rasterizer_text_draw_character(vertices);
	}

	return;
}

void
rasterizer_draw_string(
	rectangle2d const *bounds,
	rectangle2d const *clip,
	point2d *cursor_reference,
	short height_adjust,
	char const *string)
{
	if (rasterizer_debug_options.dynamic_screen_geometry
		&& global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)
	{
		struct bitmap_data *bitmap;
		struct rasterizer_dynamic_screen_geometry_parameters parameters;
		rectangle2d window_bounds;
		rectangle2d viewport_bounds;

		magic_number++;

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 180, string);

		bitmap = hardware_character_cache_get_bitmap();

		if (bitmap && string[0])
		{
			/* January calls strlen() here and discards the result; the call is
			part of the object and is reproduced. */
			strlen(string);

			if (!bounds)
			{
				window_bounds = render.camera.window_bounds;
				offset_rectangle2d(
					&window_bounds,
					-render.camera.viewport_bounds.x0,
					-render.camera.viewport_bounds.y0);
			}
			else
			{
				window_bounds = *bounds;
			}

			if (!clip)
			{
				viewport_bounds = render.camera.viewport_bounds;
				offset_rectangle2d(
					&viewport_bounds,
					-render.camera.viewport_bounds.x0,
					-render.camera.viewport_bounds.y0);
			}
			else
			{
				set_rectangle2d(
					&viewport_bounds,
					FLOOR(clip->x0, 0),
					FLOOR(clip->y0, 0),
					MIN(render.camera.viewport_bounds.x1 - render.camera.viewport_bounds.x0, clip->x1),
					MIN(render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0, clip->y1));
			}

			memset(&parameters, 0, sizeof(parameters));
			parameters.map_texture_scale[0].i = 1.0f / (real)bitmap->width;
			parameters.map_texture_scale[0].j = 1.0f / (real)bitmap->height;
			parameters.map_scale[0].i = parameters.map_scale[0].j = 1.0f;
			parameters.meter_parameters = NULL;
			parameters.point_sampled = FALSE;
			parameters.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_blend;
			parameters.map[0] = bitmap;

			rasterizer_text_begin(&parameters);
			draw_string(
				rasterizer_draw_character_with_dropshadow,
				&window_bounds,
				cursor_reference,
				&viewport_bounds,
				height_adjust,
				string);
			rasterizer_text_end();
		}
	}

	return;
}

void
rasterizer_draw_unicode_string(
	rectangle2d const *bounds,
	rectangle2d const *clip,
	point2d *cursor_reference,
	short height_adjust,
	wchar_t const *string)
{
	if (rasterizer_debug_options.dynamic_screen_geometry
		&& global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)
	{
		struct bitmap_data *bitmap;
		struct rasterizer_dynamic_screen_geometry_parameters parameters;
		rectangle2d window_bounds;
		rectangle2d viewport_bounds;

		magic_number++;

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 310, string);

		bitmap = hardware_character_cache_get_bitmap();

		if (bitmap && string[0])
		{
			/* January calls ustrlen() here and discards the result; the call is
			part of the object and is reproduced. */
			ustrlen(string);

			if (!bounds)
			{
				window_bounds = render.camera.window_bounds;
				offset_rectangle2d(
					&window_bounds,
					-render.camera.viewport_bounds.x0,
					-render.camera.viewport_bounds.y0);
			}
			else
			{
				window_bounds = *bounds;
			}

			if (!clip)
			{
				viewport_bounds = render.camera.viewport_bounds;
				offset_rectangle2d(
					&viewport_bounds,
					-render.camera.viewport_bounds.x0,
					-render.camera.viewport_bounds.y0);
			}
			else
			{
				set_rectangle2d(
					&viewport_bounds,
					FLOOR(clip->x0, 0),
					FLOOR(clip->y0, 0),
					MIN(render.camera.viewport_bounds.x1 - render.camera.viewport_bounds.x0, clip->x1),
					MIN(render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0, clip->y1));
			}

			memset(&parameters, 0, sizeof(parameters));
			parameters.map_texture_scale[0].i = 1.0f / (real)bitmap->width;
			parameters.map_texture_scale[0].j = 1.0f / (real)bitmap->height;
			parameters.map_scale[0].i = parameters.map_scale[0].j = 1.0f;
			parameters.meter_parameters = NULL;
			parameters.point_sampled = FALSE;
			parameters.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_blend;
			parameters.map[0] = bitmap;

			rasterizer_text_begin(&parameters);
			draw_unicode_string(
				rasterizer_draw_character_with_dropshadow,
				&window_bounds,
				cursor_reference,
				&viewport_bounds,
				height_adjust,
				string);
			rasterizer_text_end();
		}
	}

	return;
}

static void
rasterizer_draw_character_with_dropshadow(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy)
{
	cache_hardware_format_character(font, font_character);

	if (font_character->hardware_character_index != NONE)
	{
		struct dynamic_screen_vertex vertices[NUMBER_OF_VERTICES_PER_QUADRILATERAL];
		real x_offset = 1.0f;
		real y_offset = 1.0f;
		unsigned long shadow_color = global_shadow_color
			? global_shadow_color
			: (color & 0xFF000000);
		real left = (real)x0;
		real right = (real)(x0 + dx);
		real top = (real)y0;
		real bottom = (real)(y0 + dy);
		boolean shadow = TRUE;

		while (TRUE)
		{
			unsigned long vertex_color;
			short u0, v0;

			hardware_character_cache_get_origin(font_character->hardware_character_index, &u0, &v0);

			u0 += x;
			v0 += y;

			vertex_color = shadow ? shadow_color : color;

			vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = vertex_color;

			vertices[0].position.x = vertices[3].position.x = left + x_offset;
			vertices[1].position.x = vertices[2].position.x = right + x_offset;
			vertices[0].position.y = vertices[1].position.y = top + y_offset;
			vertices[2].position.y = vertices[3].position.y = bottom + y_offset;

			vertices[0].texture_coordinates.x = vertices[3].texture_coordinates.x = (real)u0;
			vertices[1].texture_coordinates.x = vertices[2].texture_coordinates.x = (real)(u0 + dx);
			vertices[0].texture_coordinates.y = vertices[1].texture_coordinates.y = (real)v0;
			vertices[2].texture_coordinates.y = vertices[3].texture_coordinates.y = (real)(v0 + dy);

			rasterizer_text_draw_character(vertices);

			if (!shadow)
				break;

			shadow = FALSE;
			x_offset = y_offset = 0.0f;
		}
	}

	return;
}

/* ---------- private code */

struct bitmap_data *
hardware_character_cache_get_bitmap(
	void)
{
	return hardware_character_cache.initialized ? hardware_character_cache.bitmap : NULL;
}

static void
hardware_character_cache_get_origin(
	short hardware_character_index,
	short *x0,
	short *y0)
{
	struct hardware_character *hardware_character =
		&hardware_character_cache.characters[hardware_character_index];

	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 597, hardware_character_cache.initialized);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 598, hardware_character_index>=0 && hardware_character_index<MAXIMUM_HARDWARE_CHARACTERS);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 599, x0 && y0);

	*x0 = hardware_character->x0;
	*y0 = hardware_character->y0;

	return;
}

static void
flush_hardware_character(
	struct hardware_character *hardware_character)
{
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 610, hardware_character);

	if (hardware_character->character)
	{
		hardware_character->character->hardware_character_index = NONE;

		if (hardware_character->character->pad == magic_number)
			error(_error_log, "font cache overwrote character in use");

		hardware_character->character = NULL;
	}

	return;
}

static void
cache_hardware_format_character(
	struct font_header *font,
	struct font_character *font_character)
{
	match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 633, hardware_character_cache.initialized);

#ifdef HALO_XBOX
	/* the index lives in the tag data, so a map file carries whatever its
	tool left there; only the assert below stood between a stale value and
	drawing another character's slot. A slot that does not hold this
	character means it is not cached. */
	if (font_character->hardware_character_index != NONE &&
		(font_character->hardware_character_index < 0 ||
			font_character->hardware_character_index >= MAXIMUM_HARDWARE_CHARACTERS ||
			hardware_character_cache.characters[font_character->hardware_character_index].character != font_character))
	{
		static int reported;

		if (reported++ < 4)
			error(_error_silent, "font character %p: stale cache index %d", font_character,
				(int)font_character->hardware_character_index);
		font_character->hardware_character_index = NONE;
	}
#endif
	if (font_character->hardware_character_index != NONE)
	{
		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 637, font_character->hardware_character_index>=0 && font_character->hardware_character_index<MAXIMUM_HARDWARE_CHARACTERS);
		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 638, font_character==hardware_character_cache.characters[font_character->hardware_character_index].character);
	}
	else
	{
		struct hardware_character *hardware_character;
		byte *source;
		short y0, y1;
		short x, y;
		short next_write_index;

		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 645, font_character->bitmap_width<=HARDWARE_CHARACTER_CACHE_BITMAP_WIDTH);
		match_assert("c:\\halo\\SOURCE\\rasterizer\\rasterizer_text.c", 646, font_character->bitmap_height<=HARDWARE_CHARACTER_CACHE_BITMAP_HEIGHT);

		font_character->pad = magic_number;

		if (font_character->bitmap_width + hardware_character_cache.x0 > HARDWARE_CHARACTER_CACHE_BITMAP_WIDTH)
		{
			hardware_character_cache.x0 = 0;
			hardware_character_cache.y0 += hardware_character_cache.maximum_character_height;
			hardware_character_cache.maximum_character_height = 0;
		}

		if (font_character->bitmap_height + hardware_character_cache.y0 > HARDWARE_CHARACTER_CACHE_BITMAP_HEIGHT)
		{
#ifdef HALO_XBOX
			/* the cache starts over from the top: the GPU must be done with
			the glyphs still queued before their slots are rewritten */
			halo_xbox_gpu_drain();
#endif
			hardware_character_cache.y0 = 0;
			hardware_character_cache.x0 = 0;
			hardware_character_cache.maximum_character_height = 0;

			for (;
				hardware_character_cache.read_index != hardware_character_cache.write_index;
				hardware_character_cache.read_index = (hardware_character_cache.read_index + 1) & (MAXIMUM_HARDWARE_CHARACTERS - 1))
			{
				hardware_character = &hardware_character_cache.characters[hardware_character_cache.read_index];

				if (hardware_character->y0 <= 0)
					break;

				flush_hardware_character(hardware_character);
			}
		}

		if (font_character->bitmap_height > hardware_character_cache.maximum_character_height)
		{
			y0 = hardware_character_cache.y0 + hardware_character_cache.maximum_character_height;
			y1 = hardware_character_cache.y0 + font_character->bitmap_height;

			for (;
				hardware_character_cache.read_index != hardware_character_cache.write_index;
				hardware_character_cache.read_index = (hardware_character_cache.read_index + 1) & (MAXIMUM_HARDWARE_CHARACTERS - 1))
			{
				hardware_character = &hardware_character_cache.characters[hardware_character_cache.read_index];

				if (hardware_character->y0 < y0 || hardware_character->y0 >= y1)
					break;

				flush_hardware_character(hardware_character);
			}

			hardware_character_cache.maximum_character_height = font_character->bitmap_height;
		}

		next_write_index = (hardware_character_cache.write_index + 1) & (MAXIMUM_HARDWARE_CHARACTERS - 1);
		if (next_write_index == hardware_character_cache.read_index)
		{
			flush_hardware_character(&hardware_character_cache.characters[hardware_character_cache.read_index]);
			hardware_character_cache.read_index = (hardware_character_cache.read_index + 1) & (MAXIMUM_HARDWARE_CHARACTERS - 1);
		}

		hardware_character = &hardware_character_cache.characters[hardware_character_cache.write_index];
		font_character->hardware_character_index = hardware_character_cache.write_index;

		hardware_character->character = font_character;
		hardware_character->x0 = hardware_character_cache.x0;
		hardware_character->y0 = hardware_character_cache.y0;

		source = (byte *)font->pixels.address + font_character->pixels_offset;

		for (y = 0; y < font_character->bitmap_height; y++)
		{
			word *destination = (word *)bitmap_2d_address(
				hardware_character_cache.bitmap,
				hardware_character->x0,
				(short)(hardware_character->y0 + y),
				0);

			for (x = 0; x < font_character->bitmap_width; x++)
				*destination++ = (word)((*source++ << 8) | 0x0FFF);
		}

		rasterizer_bitmap_changed(hardware_character_cache.bitmap);

		hardware_character_cache.x0 += font_character->bitmap_width;
		hardware_character_cache.write_index = (hardware_character_cache.write_index + 1) & (MAXIMUM_HARDWARE_CHARACTERS - 1);
	}

	return;
}
