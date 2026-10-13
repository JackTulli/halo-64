/*
DRAW_STRING.C

symbols in this file:
0018AB70 0010:
	_draw_string_initialize (0000)
0018AB80 0070:
	_draw_string_initialize_for_new_map (0000)
0018ABF0 0010:
	_draw_string_dispose_from_old_map (0000)
0018AC00 0010:
	_draw_string_dispose (0000)
0018AC10 0070:
	_text_bounds_draw_character (0000)
0018AC80 00e0:
	_text_pick_draw_character (0000)
0018AD60 0050:
	_draw_string_get_string (0000)
0018ADB0 0070:
	_draw_string_set_tab_stops (0000)
0018AE20 0070:
	_draw_string_set_indents (0000)
0018AE90 0150:
	_draw_string_set_color (0000)
0018AFE0 0050:
	_draw_string_get_color (0000)
0018B030 0020:
	_draw_string_set_font (0000)
0018B050 00b0:
	_draw_string_set_format (0000)
0018B100 0040:
	_draw_string_set_draw_mode (0000)
0018B140 0020:
	_draw_string_set_highlight (0000)
0018B160 03b0:
	_bitmap_draw_character (0000)
0018B510 0070:
	_styled_font_get (0000)
0018B580 0100:
	_parse_string_new (0000)
0018B680 0270:
	_parse_string (0000)
0018B8F0 0110:
	_parse_unicode_string (0000)
0018BA00 0210:
	_draw_string_partial (0000)
0018BC10 0210:
	_draw_unicode_string_partial (0000)
0018BE20 0390:
	_draw_string (0000)
0018C1B0 0390:
	_draw_unicode_string (0000)
0018C540 00c0:
	_draw_string_compute_bounds (0000)
0018C600 00c0:
	_draw_unicode_string_compute_bounds (0000)
0018C6C0 0050:
	_draw_string_pick (0000)
0018C710 0130:
	_bitmap_draw_string (0000)
002A26E4 002d:
	??_C@_0CN@NJAONNEI@internal?5string?5localization?5tag@ (0000)
002A2714 0029:
	??_C@_0CJ@LDLNPDDM@index?$DO?$DN0?5?$CG?$CG?5index?$DMNUMBER_OF_TEXT@ (0000)
002A2740 0022:
	??_C@_0CC@EBHGCODL@c?3?2halo?2SOURCE?2text?2draw_string?4@ (0000)
002A2764 002e:
	??_C@_0CO@LAOAKNHK@count?$DO?$DN0?5?$CG?$CG?5count?$DMMAXIMUM_NUMBER@ (0000)
002A2794 0014:
	??_C@_0BE@FGPECGKA@paragraph_indent?$DO?$DN0?$AA@ (0000)
002A27A8 0012:
	??_C@_0BC@DNGFHNA@initial_indent?$DO?$DN0?$AA@ (0000)
002A27BC 002d:
	??_C@_0CN@CGOGANEE@?$CIcolor?9?$DOblue?5?$DO?$DN?50?4f?$CJ?5?$CG?$CG?5?$CIcolor?9?$DO@ (0000)
002A27EC 002f:
	??_C@_0CP@FADKAPOG@?$CIcolor?9?$DOgreen?5?$DO?$DN?50?4f?$CJ?5?$CG?$CG?5?$CIcolor?9@ (0000)
002A281C 002b:
	??_C@_0CL@COFHKNKB@?$CIcolor?9?$DOred?5?$DO?$DN?50?4f?$CJ?5?$CG?$CG?5?$CIcolor?9?$DOr@ (0000)
002A2848 002f:
	??_C@_0CP@DFGKLIFB@?$CIcolor?9?$DOalpha?5?$DO?$DN?50?4f?$CJ?5?$CG?$CG?5?$CIcolor?9@ (0000)
002A2878 0040:
	??_C@_0EA@DLFMIHAM@justification?$DO?$DN0?5?$CG?$CG?5justificatio@ (0000)
002A28B8 0046:
	??_C@_0EG@IPKHNIBH@style?$DN?$DN_text_style_plain?5?$HM?$HM?5?$CIsty@ (0000)
002A2900 0029:
	??_C@_0CJ@CIJKMAAN@VALID_FLAGS?$CIflags?0?5NUMBER_OF_TEX@ (0000)
002A292C 003e:
	??_C@_0DO@NHIHGJAE@x0?$DO?$DN0?5?$CG?$CG?5x0?$CLdx?$DM?$DNdraw_character_s@ (0000)
002A296C 003c:
	??_C@_0DM@BMLHMCCB@y0?$DO?$DN0?5?$CG?$CG?5y0?$DM?$DNdraw_character_soft@ (0000)
002A29A8 000d:
	??_C@_0N@HNKPGPMM@result?$CB?$DNNONE?$AA@ (0000)
002A29B8 0049:
	??_C@_0EJ@CJKOFANJ@tab_stop_index?$DO?$DN0?5?$CG?$CG?5tab_stop_in@ (0000)
004C1908 0008:
	_draw_character_software_globals (0000)
004C1910 000c:
	_text_pick_globals (0000)
004C191C 000c:
	_text_bounds_globals (0000)
004C1928 00d8:
	_font_drawing_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "interface/interface.h"
#include "math/integer_math.h"
#include "math/real_math.h"
#include "rasterizer/rasterizer.h"
#include "text/draw_string.h"
#include "text/font_group.h"
#include "text/international_strings.h"
#include "text/text_group.h"

#include <ctype.h>

/* ---------- constants */

enum
{
	NUMBER_OF_TEXT_STRINGS = 1,
	NUMBER_OF_TEXT_FLAGS = 4,
	MAXIMUM_NUMBER_OF_TAB_STOPS = 16,

	_text_justification_left = 0,
	_text_justification_right,
	_text_justification_center,
	NUMBER_OF_TEXT_JUSTIFICATIONS,

	_draw_text_wrap_horizontally_bit = 0,
	_draw_text_wrap_vertically_bit,

	_parsed_end_of_string = 0,
	_parsed_end_of_line = 1,
	_parsed_end_of_word = 2,
	_parsed_end_of_column = 3,
	_parsed_justification_change = 4,
	_parsed_color_change = 5,
	_parsed_character = 6,
	_parsed_style_change = 7,

	_string_index_can_end_words = 4,
	_string_index_cannot_end_words = 5,
	_string_index_cannot_begin_words = 6,

	_bitmap_format_a8 = 0,
	_bitmap_format_y8 = 1,
	_bitmap_format_ay8 = 2,
	_bitmap_format_r5g6b5 = 6,
	_bitmap_format_a8r8g8b8 = 11,
};

/* ---------- structures */

struct font_drawing_globals
{
	long string_list_index;
	long current_font_index;
	unsigned long current_flags;
	short current_style;
	short current_justification;
	real_argb_color current_color;
	short tab_stop_count;
	short tab_stops[MAXIMUM_NUMBER_OF_TAB_STOPS];
	short highlight_start_index;
	short highlight_stop_index;
	short initial_indent;
	short paragraph_indent;
	struct rasterizer_dynamic_screen_geometry_parameters multitexture_params;
};

struct font_character
{
	word character;
	short character_width;
	short bitmap_width;
	short bitmap_height;
	short bitmap_origin_x;
	short bitmap_origin_y;
	short hardware_character_index;
	word pad;
	long pixels_offset;
};

struct parse_string_state
{
	long base_font_index;
	struct font_header *font_header;
	byte *string;
	short string_index;
	short style;
	short justification;
	word character;
	short result;
	word pad;
	pixel32 color;
};

/* ---------- prototypes */

static struct font_header *styled_font_get(
	long font_index,
	short style);
static void parse_string_new(
	struct parse_string_state *state,
	char const *string,
	long font_index,
	short style,
	short justification,
	real_argb_color const *color);
static short parse_string(
	struct parse_string_state *state);
static word parse_unicode_string(
	struct parse_string_state *state);
static void draw_string_partial(
	draw_character_proc draw_character,
	point2d *cursor,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	pixel32 color,
	char const *string,
	short string_index,
	short string_length);
static void draw_unicode_string_partial(
	draw_character_proc draw_character,
	point2d *cursor,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	pixel32 color,
	wchar_t const *string,
	short string_index,
	short string_length);

/* ---------- globals */

static struct
{
	struct bitmap_data *bitmap;
	short encoding_shift;
} draw_character_software_globals = { 0 };

static struct
{
	point2d pick_point;
	short best_pick_string_index;
	short best_pick_distance;
	short last_string_index;
} text_pick_globals = { 0 };

static struct
{
	rectangle2d bounds;
	struct font_header *last_font_header;
} text_bounds_globals = { 0 };

static struct font_drawing_globals font_drawing_globals = { 0 };


/* ---------- public code */

void draw_string_initialize(
	void)
{
	return;
}

void draw_string_initialize_for_new_map(
	void)
{
	long localization_string_list_index = interface_get_tag_index(_interface_string_list_localization);
	font_drawing_globals.string_list_index = localization_string_list_index;

	if (localization_string_list_index != NONE)
	{
		set_language_code((short)atoi(string_list_get_string(localization_string_list_index, 0)));
		font_drawing_globals.tab_stop_count = 0;
		font_drawing_globals.current_flags = 0;
		font_drawing_globals.current_justification = _text_justification_left;
		font_drawing_globals.initial_indent = 0;
		font_drawing_globals.paragraph_indent = 0;
		font_drawing_globals.current_font_index = NONE;
	}
	else
	{
		error(_error_immediate, "internal string localization tag is missing.");
	}

	return;
}

void draw_string_dispose_from_old_map(
	void)
{
	font_drawing_globals.string_list_index = NONE;
	return;
}

void draw_string_dispose(
	void)
{
	return;
}

static void text_bounds_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *character,
	pixel32 color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy)
{
	short x1 = (short)(x0 + dx);
	short y1 = (short)(y0 + dy);

	if (x0 < text_bounds_globals.bounds.x0)
		text_bounds_globals.bounds.x0 = x0;
	if (y0 < text_bounds_globals.bounds.y0)
		text_bounds_globals.bounds.y0 = y0;
	if (x1 > text_bounds_globals.bounds.x1)
		text_bounds_globals.bounds.x1 = x1;
	if (y1 > text_bounds_globals.bounds.y1)
		text_bounds_globals.bounds.y1 = y1;
	text_bounds_globals.last_font_header = font;

	return;
}

static void text_pick_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *character,
	pixel32 color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy)
{
	short x1 = x0 + dx;
	short y1 = y0 + dy;
	short distance_x0 = x0 - text_pick_globals.pick_point.x;
	short distance_x1 = x1 - text_pick_globals.pick_point.x;
	short distance_y0 = y0 - text_pick_globals.pick_point.y;
	short distance_y1 = y1 - text_pick_globals.pick_point.y;
	short distance;

	distance_x0 = ABS(distance_x0);
	distance_x1 = ABS(distance_x1);
	distance_y0 = ABS(distance_y0);
	distance_y1 = ABS(distance_y1);
	distance = MAX(distance_x0, distance_x1);
	distance = MAX(distance, distance_y0);
	distance = MAX(distance, distance_y1);

	if (distance < text_pick_globals.best_pick_distance)
	{
		text_pick_globals.best_pick_distance = distance;
		if (text_pick_globals.pick_point.x - x0 < ((x1 - x0) >> 1))
		{
			text_pick_globals.best_pick_string_index = text_pick_globals.last_string_index;
			text_pick_globals.last_string_index = state->string_index;
			return;
		}

		text_pick_globals.best_pick_string_index = state->string_index;
		text_pick_globals.last_string_index = state->string_index;
		return;
	}

	text_pick_globals.last_string_index = state->string_index;

	return;
}

char *draw_string_get_string(
	short index)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 341, index>=0 && index<NUMBER_OF_TEXT_STRINGS);

	return string_list_get_string(font_drawing_globals.string_list_index, index + 7);
}

void draw_string_set_tab_stops(
	short const *tab_stops,
	short count)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 350, count>=0 && count<MAXIMUM_NUMBER_OF_TAB_STOPS);

	font_drawing_globals.tab_stop_count = MIN(count, MAXIMUM_NUMBER_OF_TAB_STOPS);

	if (font_drawing_globals.tab_stop_count > 0)
	{
		csmemcpy(font_drawing_globals.tab_stops, tab_stops, font_drawing_globals.tab_stop_count * sizeof(short));
	}

	return;
}

void draw_string_set_indents(
	short initial_indent,
	short paragraph_indent)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 366, initial_indent>=0);
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 367, paragraph_indent>=0);

	font_drawing_globals.paragraph_indent = paragraph_indent;
	font_drawing_globals.initial_indent = initial_indent;

	return;
}

void draw_string_set_color(
	real_argb_color const *color)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 378, color);
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 379, (color->alpha >= 0.f) && (color->alpha <= 1.f));
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 380, (color->red >= 0.f) && (color->red <= 1.f));
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 381, (color->green >= 0.f) && (color->green <= 1.f));
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 382, (color->blue >= 0.f) && (color->blue <= 1.f));

	font_drawing_globals.current_color = *color;

	return;
}

void draw_string_get_color(
	real_argb_color *color)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 392, color);

	*color = font_drawing_globals.current_color;

	return;
}

void draw_string_set_font(
	long font_index)
{
	font_definition_get(font_index);
	font_drawing_globals.current_font_index = font_index;

	return;
}

void draw_string_set_format(
	short style,
	short justification,
	unsigned long flags)
{
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 410, VALID_FLAGS(flags, NUMBER_OF_TEXT_FLAGS));
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 411, style==_text_style_plain || (style>=0 && style<NUMBER_OF_TEXT_STYLES));
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 412, justification>=0 && justification<NUMBER_OF_TEXT_JUSTIFICATIONS);

	font_drawing_globals.current_style = style;
	font_drawing_globals.current_justification = justification;
	font_drawing_globals.current_flags = flags;

	return;
}

void draw_string_set_draw_mode(
	long font_index,
	short style,
	short justification,
	unsigned long flags,
	real_argb_color const *color)
{
	draw_string_set_font(font_index);
	draw_string_set_color(color);
	draw_string_set_format(style, justification, flags);

	return;
}

void draw_string_set_highlight(
	short start,
	short end)
{
	font_drawing_globals.highlight_start_index = start;
	font_drawing_globals.highlight_stop_index = end;

	return;
}

/* ---------- private code */

static void bitmap_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *character,
	pixel32 color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy)
{
	short format = draw_character_software_globals.bitmap->format;
	short coverage_scale = (short)(color >> 24);
	byte *glyph_pixels = (byte *)font->pixels.address + character->pixels_offset;
	word destination_color;
	short row;

	if (format == _bitmap_format_r5g6b5)
	{
		destination_color = (word)(((((color >> 16) & 0xFF) >> 3) << 11) |
			((((color >> 8) & 0xFF) >> 2) << 5) |
			((color & 0xFF) >> 3));
	}

	if (dy <= 0)
		return;

	for (row = dy; row > 0; row--, y++, y0++)
	{
		long row_pitch = bitmap_format_get_bits_per_pixel(
			draw_character_software_globals.bitmap->format) *
			draw_character_software_globals.bitmap->width / 8;
		byte *destination_pixel = (byte *)draw_character_software_globals.bitmap->base_address +
			(x0 << draw_character_software_globals.encoding_shift) +
			y0 * row_pitch;
		byte *source_pixel = &glyph_pixels[character->bitmap_width * y + x];
		short column;

		match_assert(
			"c:\\halo\\SOURCE\\text\\draw_string.c",
			495,
			y0>=0 && y0<=draw_character_software_globals.bitmap->height);
		match_assert(
			"c:\\halo\\SOURCE\\text\\draw_string.c",
			496,
			x0>=0 && x0+dx<=draw_character_software_globals.bitmap->width);

		switch (format)
		{
		case _bitmap_format_a8:
		case _bitmap_format_y8:
		case _bitmap_format_ay8:
			for (column = dx; column > 0; column--, source_pixel++, destination_pixel++)
			{
				short coverage = *source_pixel;

				if (coverage)
				{
					byte destination_value = *destination_pixel;

					coverage = (coverage * coverage_scale) >> 8;
					*destination_pixel = (byte)MIN(coverage, destination_value);
				}
			}
			break;

		case _bitmap_format_r5g6b5:
			for (column = dx; column > 0; column--, source_pixel++, destination_pixel += 2)
			{
				if (*source_pixel)
				{
					word destination = *(word *)destination_pixel;
					short coverage = (*source_pixel * coverage_scale) >> 8;
					short inverse = 255 - coverage;

					*(word *)destination_pixel = (word)(
						(((destination_color * coverage + destination * inverse) >> 8) & 0xF800) |
						((((destination_color & 0x7FF) * coverage + (destination & 0x7FF) * inverse) >> 8) & 0x7E0) |
						((((destination_color & 0x1F) * coverage + (destination & 0x1F) * inverse) >> 8) & 0x1F));
				}
			}
			break;

		case _bitmap_format_a8r8g8b8:
			for (column = dx; column > 0; column--, source_pixel++, destination_pixel += 4)
			{
				short source_value = *source_pixel;

				if (source_value)
				{
					pixel32 destination = *(pixel32 *)destination_pixel;
					short coverage = (source_value * coverage_scale) >> 8;
					short inverse = 255 - coverage;

					*(pixel32 *)destination_pixel =
						((((((destination >> 16) & 0xFF) * inverse) >> 8) + ((((color >> 16) & 0xFF) * coverage) >> 8)) << 16) |
						((((((destination >> 8) & 0xFF) * inverse) >> 8) + ((((color >> 8) & 0xFF) * coverage) >> 8)) << 8) |
						((((destination & 0xFF) * inverse) >> 8) + (((color & 0xFF) * coverage) >> 8)) |
						(MAX((pixel32)coverage, destination >> 24) << 24);
				}
			}
			break;

		default:
			display_assert(
				"### ERROR unsupported bitmap format",
				"c:\\halo\\SOURCE\\text\\draw_string.c",
				565,
				TRUE);
			system_exit(-1);
			break;
		}
	}

	return;
}

static struct font_header *styled_font_get(
	long font_index,
	short style)
{
	long styled_font_index;

	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 1039, style==_text_style_plain || (style>=0 && style<NUMBER_OF_TEXT_STYLES));

	styled_font_index = style == _text_style_plain ? font_index : font_definition_get(font_index)->style_fonts[style].index;
	if (styled_font_index == NONE)
		styled_font_index = font_index;

	return font_definition_get(styled_font_index);
}

static void parse_string_new(
	struct parse_string_state *state,
	char const *string,
	long font_index,
	short style,
	short justification,
	real_argb_color const *color)
{
	pixel32 packed_color;

	match_assert(
		"c:\\halo\\SOURCE\\text\\draw_string.c",
		1054,
		style==_text_style_plain || (style>=0 && style<NUMBER_OF_TEXT_STYLES));
	match_assert(
		"c:\\halo\\SOURCE\\text\\draw_string.c",
		1055,
		justification>=0 && justification<NUMBER_OF_TEXT_JUSTIFICATIONS);

	state->base_font_index = font_index;
	state->string = (byte *)string;
	state->justification = justification;
	state->string_index = 0;
	state->style = style;

	packed_color = (long)(color->alpha * 255.f);
	packed_color = (packed_color << 8) | (long)(color->red * 255.f);
	packed_color = (packed_color << 8) | (long)(color->green * 255.f);
	packed_color = (packed_color << 8) | (long)(color->blue * 255.f);
	state->color = packed_color;
	state->font_header = styled_font_get(font_index, style);

	return;
}

static short parse_string(
	struct parse_string_state *state)
{
	short *string_index = &state->string_index;
	word character;
	short result;

	do
	{
		character = get_next_character(state->string, string_index);
		result = NONE;

		if ((character & 0xFF00) == 0x7C00)
		{
			switch (tolower(character & 0xFF))
			{
			case 'p': state->style = _text_style_plain; result = _parsed_style_change; break;
			case 'i': state->style = _text_style_italic; result = _parsed_style_change; break;
			case 'b': state->style = _text_style_bold; result = _parsed_style_change; break;
			case 'k': state->style = _text_style_condense; result = _parsed_style_change; break;
			case 'u': state->style = _text_style_underline; result = _parsed_style_change; break;
			case 'l': state->justification = _text_justification_left; result = _parsed_justification_change; break;
			case 'r': state->justification = _text_justification_right; result = _parsed_justification_change; break;
			case 'c': state->justification = _text_justification_center; result = _parsed_justification_change; break;
			case 'n': result = _parsed_end_of_line; break;
			case 't': result = _parsed_end_of_column; break;
			}
		}

		switch (result)
		{
		case NONE:
			switch (character)
			{
			case 0:
				result = _parsed_end_of_string;
				break;
			case '\t':
				result = _parsed_end_of_column;
				break;
			case '\r':
				result = _parsed_end_of_line;
				break;
			default:
				{
					short next_string_index = *string_index;
					long next_character = get_next_character(state->string, &next_string_index);
					char *can_end_words = string_list_get_string(font_drawing_globals.string_list_index, _string_index_can_end_words);
					char *cannot_end_words = string_list_get_string(font_drawing_globals.string_list_index, _string_index_cannot_end_words);
					char *cannot_begin_words = string_list_get_string(font_drawing_globals.string_list_index, _string_index_cannot_begin_words);

					if (((character & 0xFF00) || character_in_pattern(character, can_end_words)) &&
						(!(character & 0xFF00) || !character_in_pattern(character, cannot_end_words)) &&
						!character_in_pattern(next_character, cannot_begin_words))
					{
						result = _parsed_end_of_word;
					}
					else
					{
						result = _parsed_character;
					}
				}
				break;
			}
			break;

		case _parsed_style_change:
			state->font_header = styled_font_get(state->base_font_index, state->style);
			break;
		}
	}
	while (result == _parsed_style_change || result == _parsed_color_change);

	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 1203, result!=NONE);

	state->result = result;
	state->character = character;

	return result;
}

static word parse_unicode_string(
	struct parse_string_state *state)
{
	wchar_t const *string = (wchar_t const *)state->string;
	short string_index = state->string_index;
	word character = string[string_index];

	state->character = character;
	state->string_index = string_index + 1;

	switch (character)
	{
	case '|':
		if (string[state->string_index++] == 'n')
		{
			state->character = '\r';
			state->result = _parsed_end_of_line;
		}
		break;
	case 0:
		state->result = _parsed_end_of_string;
		break;
	case '\t':
		state->result = _parsed_end_of_column;
		break;
	case '\r':
		state->result = _parsed_end_of_line;
		break;
	default:
		state->result = _parsed_character;
		break;
	}

	return state->result;
}

static void draw_string_partial(
	draw_character_proc draw_character,
	point2d *cursor,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	pixel32 color,
	char const *string,
	short string_index,
	short string_length)
{
	short clip_left = SHORT_MIN;
	short clip_top = SHORT_MIN;
	short clip_right = SHORT_MAX;
	short clip_bottom = SHORT_MAX;

	if (bounds)
	{
		if (bounds->x0 > clip_left)
			clip_left = bounds->x0;
		if (bounds->x1 < clip_right)
			clip_right = bounds->x1;
		if (bounds->y0 > clip_top)
			clip_top = bounds->y0;
		if (bounds->y1 < clip_bottom)
			clip_bottom = bounds->y1;
	}

	if (clip)
	{
		if (clip->x0 > clip_left)
			clip_left = clip->x0;
		if (clip->x1 < clip_right)
			clip_right = clip->x1;
		if (clip->y0 > clip_top)
			clip_top = clip->y0;
		if (clip->y1 < clip_bottom)
			clip_bottom = clip->y1;
	}

	if (clip_left < clip_right && clip_top < clip_bottom)
	{
		struct parse_string_state state;

		parse_string_new(
			&state,
			string,
			font_drawing_globals.current_font_index,
			font_drawing_globals.current_style,
			font_drawing_globals.current_justification,
			&font_drawing_globals.current_color);

		for (state.string_index = string_index; state.string_index < string_length; )
		{
			pixel32 character_color =
				(state.string_index >= font_drawing_globals.highlight_start_index && state.string_index < font_drawing_globals.highlight_stop_index) ?
				color ^ 0xFFFFFF :
				color;
			struct font_character *character;

			parse_string(&state);
			character = font_get_character_by_ascii_code(state.font_header, state.character);
			if (character)
			{
				short x = cursor->x - character->bitmap_origin_x;
				short y = cursor->y - character->bitmap_origin_y;
				short width = character->bitmap_width;
				short source_x = 0;
				short source_y = 0;
				short height = character->bitmap_height;

				cursor->x += character->character_width;

				if (x + width > clip_right)
				{
					width = clip_right - x;
				}
				if (x < clip_left)
				{
					source_x = clip_left - x;
					x = clip_left;
					width -= source_x;
				}

				if (y + height > clip_bottom)
				{
					height = clip_bottom - y;
				}
				if (y < clip_top)
				{
					source_y = clip_top - y;
					y = clip_top;
					height -= source_y;
				}

				if (width > 0 && height > 0)
				{
					draw_character(&state, state.font_header, character, character_color, x, y, source_x, source_y, width, height);
				}
			}
		}
	}

	return;
}

static void draw_unicode_string_partial(
	draw_character_proc draw_character,
	point2d *cursor,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	pixel32 color,
	wchar_t const *string,
	short string_index,
	short string_length)
{
	short clip_left = SHORT_MIN;
	short clip_top = SHORT_MIN;
	short clip_right = SHORT_MAX;
	short clip_bottom = SHORT_MAX;

	if (bounds)
	{
		if (bounds->x0 > clip_left)
			clip_left = bounds->x0;
		if (bounds->x1 < clip_right)
			clip_right = bounds->x1;
		if (bounds->y0 > clip_top)
			clip_top = bounds->y0;
		if (bounds->y1 < clip_bottom)
			clip_bottom = bounds->y1;
	}

	if (clip)
	{
		if (clip->x0 > clip_left)
			clip_left = clip->x0;
		if (clip->x1 < clip_right)
			clip_right = clip->x1;
		if (clip->y0 > clip_top)
			clip_top = clip->y0;
		if (clip->y1 < clip_bottom)
			clip_bottom = clip->y1;
	}

	if (clip_left < clip_right && clip_top < clip_bottom)
	{
		struct parse_string_state state;

		parse_string_new(
			&state,
			(char const *)string,
			font_drawing_globals.current_font_index,
			font_drawing_globals.current_style,
			font_drawing_globals.current_justification,
			&font_drawing_globals.current_color);

		for (state.string_index = string_index; state.string_index < string_length; )
		{
			pixel32 character_color =
				(state.string_index >= font_drawing_globals.highlight_start_index && state.string_index < font_drawing_globals.highlight_stop_index) ?
				color ^ 0xFFFFFF :
				color;
			struct font_character *character;

			parse_unicode_string(&state);
			character = font_get_character_by_ascii_code(state.font_header, state.character);
			if (character)
			{
				short x = cursor->x - character->bitmap_origin_x;
				short y = cursor->y - character->bitmap_origin_y;
				short width = character->bitmap_width;
				short source_x = 0;
				short source_y = 0;
				short height = character->bitmap_height;

				cursor->x += character->character_width;

				if (x + width > clip_right)
				{
					width = clip_right - x;
				}
				if (x < clip_left)
				{
					source_x = clip_left - x;
					x = clip_left;
					width -= source_x;
				}

				if (y + height > clip_bottom)
				{
					height = clip_bottom - y;
				}
				if (y < clip_top)
				{
					source_y = clip_top - y;
					y = clip_top;
					height -= source_y;
				}

				if (width > 0 && height > 0)
				{
					draw_character(&state, state.font_header, character, character_color, x, y, source_x, source_y, width, height);
				}
			}
		}
	}

	return;
}

void draw_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	char const *string)
{
	short tab_stop_index = 0;
	short paragraph_line_index = 0;
	short wrapped_line_index = 0;
	short maximum_wrapped_line_index = 0;
	struct parse_string_state state;
	point2d cursor;

	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 648, bounds);
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 649, string);

	parse_string_new(
		&state,
		string,
		font_drawing_globals.current_font_index,
		font_drawing_globals.current_style,
		font_drawing_globals.current_justification,
		&font_drawing_globals.current_color);

	do
	{
		short justification = state.justification;
		short segment_start_index = state.string_index;
		short tab_stop_count = font_drawing_globals.tab_stop_count;
		short line_width = 0;
		short break_string_index = 0;
		short break_line_width;
		short previous_result = NONE;
		short segment_end_index;
		boolean done = FALSE;
		rectangle2d line_bounds = *bounds;

		if (tab_stop_count > 0)
		{
			match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 681, tab_stop_index>=0 && tab_stop_index<=font_drawing_globals.tab_stop_count);

			if (tab_stop_index != 0)
			{
				line_bounds.x0 = font_drawing_globals.tab_stops[tab_stop_index - 1];
			}
			else if (paragraph_line_index != 0)
			{
				line_bounds.x0 += font_drawing_globals.paragraph_indent;
			}
			else
			{
				line_bounds.x0 += font_drawing_globals.initial_indent;
			}

			if (tab_stop_index < font_drawing_globals.tab_stop_count)
			{
				line_bounds.x1 = font_drawing_globals.tab_stops[tab_stop_index];
			}
		}
		else if (paragraph_line_index != 0)
		{
			line_bounds.x0 += font_drawing_globals.paragraph_indent;
		}
		else
		{
			line_bounds.x0 += font_drawing_globals.initial_indent;
		}

		cursor.x = state.font_header->leading_width + line_bounds.x0;
		cursor.y = (state.font_header->leading_height + state.font_header->descending_height + state.font_header->ascending_height + height_adjust) *
			(wrapped_line_index + paragraph_line_index) +
			state.font_header->ascending_height + line_bounds.y0;

		do
		{
			boolean wrapped = FALSE;

			parse_string(&state);
			if (state.result != _parsed_end_of_word && state.result != _parsed_character)
			{
				done = TRUE;
			}
			else
			{
				struct font_character *character = font_get_character_by_ascii_code(state.font_header, state.character);

				if (character)
				{
					if (state.result != _parsed_end_of_word && previous_result == _parsed_end_of_word)
					{
						break_string_index = segment_end_index;
						break_line_width = line_width;
					}

					if (character->bitmap_width + cursor.x + line_width < line_bounds.x1)
					{
						line_width += character->character_width;
					}
					else if (TEST_FLAG(font_drawing_globals.current_flags, _draw_text_wrap_horizontally_bit))
					{
						if (break_string_index > 0)
						{
							segment_end_index = break_string_index;
							line_width = break_line_width;
							wrapped = TRUE;
						}
						done = TRUE;
					}
				}
			}

			if (!wrapped)
			{
				segment_end_index = state.string_index;
			}
			previous_result = state.result;
		}
		while (!done);

		switch (justification)
		{
		case _text_justification_center:
			cursor.x = ((rectangle2d_width(&line_bounds) - line_width) >> 1) + line_bounds.x0;
			break;

		case _text_justification_right:
			cursor.x = rectangle2d_width(&line_bounds) + line_bounds.x0 - state.font_header->leading_width - line_width;
			break;
		}

		if (TEST_FLAG(font_drawing_globals.current_flags, _draw_text_wrap_vertically_bit) || cursor.y < line_bounds.y1)
		{
			draw_string_partial(
				draw_character,
				&cursor,
				&line_bounds,
				clip,
				state.color,
				string,
				segment_start_index,
				segment_end_index);
		}

		state.string_index = segment_end_index;

		switch (state.result)
		{
		case _parsed_end_of_string:
		case _parsed_color_change:
			break;

		case _parsed_end_of_word:
		case _parsed_character:
			wrapped_line_index++;
			if (wrapped_line_index > maximum_wrapped_line_index)
			{
				maximum_wrapped_line_index = wrapped_line_index;
			}
			break;

		case _parsed_end_of_column:
			if (tab_stop_index < font_drawing_globals.tab_stop_count)
			{
				tab_stop_index++;
				wrapped_line_index = 0;
			}
			break;

		case _parsed_justification_change:
			wrapped_line_index = 0;
			break;

		case _parsed_end_of_line:
			tab_stop_index = 0;
			wrapped_line_index = 0;
			paragraph_line_index += maximum_wrapped_line_index + 1;
			break;

		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\text\\draw_string.c", 817, TRUE);
			system_exit(-1);
			break;
		}
	}
	while (state.result != _parsed_end_of_string);

	font_drawing_globals.highlight_stop_index = 0;
	font_drawing_globals.highlight_start_index = 0;
	if (cursor_reference)
	{
		*cursor_reference = cursor;
	}

	return;
}

void draw_unicode_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	wchar_t const *string)
{
	short tab_stop_index = 0;
	short paragraph_line_index = 0;
	short wrapped_line_index = 0;
	short maximum_wrapped_line_index = 0;
	struct parse_string_state state;
	point2d cursor;

	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 848, bounds);
	match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 849, string);

	parse_string_new(
		&state,
		(char const *)string,
		font_drawing_globals.current_font_index,
		font_drawing_globals.current_style,
		font_drawing_globals.current_justification,
		&font_drawing_globals.current_color);

	do
	{
		short justification = state.justification;
		short segment_start_index = state.string_index;
		short tab_stop_count = font_drawing_globals.tab_stop_count;
		short line_width = 0;
		short break_string_index = 0;
		short break_line_width;
		short previous_result = NONE;
		short segment_end_index;
		boolean done = FALSE;
		rectangle2d line_bounds = *bounds;

		if (tab_stop_count > 0)
		{
			match_assert("c:\\halo\\SOURCE\\text\\draw_string.c", 881, tab_stop_index>=0 && tab_stop_index<=font_drawing_globals.tab_stop_count);

			if (tab_stop_index != 0)
			{
				line_bounds.x0 = font_drawing_globals.tab_stops[tab_stop_index - 1];
			}
			else if (paragraph_line_index != 0)
			{
				line_bounds.x0 += font_drawing_globals.paragraph_indent;
			}
			else
			{
				line_bounds.x0 += font_drawing_globals.initial_indent;
			}

			if (tab_stop_index < font_drawing_globals.tab_stop_count)
			{
				line_bounds.x1 = font_drawing_globals.tab_stops[tab_stop_index];
			}
		}
		else if (paragraph_line_index != 0)
		{
			line_bounds.x0 += font_drawing_globals.paragraph_indent;
		}
		else
		{
			line_bounds.x0 += font_drawing_globals.initial_indent;
		}

		cursor.x = state.font_header->leading_width + line_bounds.x0;
		cursor.y = (state.font_header->leading_height + state.font_header->descending_height + state.font_header->ascending_height + height_adjust) *
			(wrapped_line_index + paragraph_line_index) +
			state.font_header->ascending_height + line_bounds.y0;

		do
		{
			boolean wrapped = FALSE;

			parse_unicode_string(&state);
			if (state.result != _parsed_end_of_word && state.result != _parsed_character)
			{
				done = TRUE;
			}
			else
			{
				struct font_character *character = font_get_character_by_ascii_code(state.font_header, state.character);

				if (character)
				{
					if (state.result != _parsed_end_of_word && previous_result == _parsed_end_of_word)
					{
						break_string_index = segment_end_index;
						break_line_width = line_width;
					}

					if (character->bitmap_width + cursor.x + line_width < line_bounds.x1)
					{
						line_width += character->character_width;
					}
					else if (TEST_FLAG(font_drawing_globals.current_flags, _draw_text_wrap_horizontally_bit))
					{
						if (break_string_index > 0)
						{
							segment_end_index = break_string_index;
							line_width = break_line_width;
							wrapped = TRUE;
						}
						done = TRUE;
					}
				}
			}

			if (!wrapped)
			{
				segment_end_index = state.string_index;
			}
			previous_result = state.result;
		}
		while (!done);

		switch (justification)
		{
		case _text_justification_center:
			cursor.x = ((rectangle2d_width(&line_bounds) - line_width) >> 1) + line_bounds.x0;
			break;

		case _text_justification_right:
			cursor.x = rectangle2d_width(&line_bounds) + line_bounds.x0 - state.font_header->leading_width - line_width;
			break;
		}

		if (TEST_FLAG(font_drawing_globals.current_flags, _draw_text_wrap_vertically_bit) || cursor.y < line_bounds.y1)
		{
			draw_unicode_string_partial(
				draw_character,
				&cursor,
				&line_bounds,
				clip,
				state.color,
				string,
				segment_start_index,
				segment_end_index);
		}

		state.string_index = segment_end_index;

		switch (state.result)
		{
		case _parsed_end_of_string:
		case _parsed_color_change:
			break;

		case _parsed_end_of_word:
		case _parsed_character:
			wrapped_line_index++;
			if (wrapped_line_index > maximum_wrapped_line_index)
			{
				maximum_wrapped_line_index = wrapped_line_index;
			}
			break;

		case _parsed_end_of_column:
			if (tab_stop_index < font_drawing_globals.tab_stop_count)
			{
				tab_stop_index++;
				wrapped_line_index = 0;
			}
			break;

		case _parsed_justification_change:
			wrapped_line_index = 0;
			break;

		case _parsed_end_of_line:
			tab_stop_index = 0;
			wrapped_line_index = 0;
			paragraph_line_index += maximum_wrapped_line_index + 1;
			break;

		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\text\\draw_string.c", 1017, TRUE);
			system_exit(-1);
			break;
		}
	}
	while (state.result != _parsed_end_of_string);

	font_drawing_globals.highlight_stop_index = 0;
	font_drawing_globals.highlight_start_index = 0;
	if (cursor_reference)
	{
		*cursor_reference = cursor;
	}

	return;
}

void draw_string_compute_bounds(
	rectangle2d const *bounds,
	char const *string,
	rectangle2d *text_bounds,
	rectangle2d *cursor_bounds)
{
	point2d cursor;

	text_bounds_globals.bounds.y0 = SHORT_MAX;
	text_bounds_globals.bounds.x0 = SHORT_MAX;
	text_bounds_globals.bounds.y1 = SHORT_MIN;
	text_bounds_globals.bounds.x1 = SHORT_MIN;

	text_bounds_globals.last_font_header = styled_font_get(
		font_drawing_globals.current_font_index,
		font_drawing_globals.current_style);
	draw_string(text_bounds_draw_character, bounds, &cursor, NULL, 0, string);

	cursor_bounds->x0 = cursor.x;
	cursor_bounds->x1 = (short)(cursor.x + 1);
	cursor_bounds->y0 = (short)(
		cursor.y - text_bounds_globals.last_font_header->ascending_height);
	cursor_bounds->y1 = (short)(
		text_bounds_globals.last_font_header->descending_height + cursor.y);

	text_bounds->x0 = text_bounds_globals.bounds.x0;
	text_bounds->y0 = bounds->y0;
	text_bounds->x1 = text_bounds_globals.bounds.x1;
	text_bounds->y1 = cursor_bounds->y1;

	return;
}

void draw_unicode_string_compute_bounds(
	rectangle2d const *bounds,
	wchar_t const *string,
	rectangle2d *text_bounds,
	rectangle2d *cursor_bounds)
{
	point2d cursor;

	text_bounds_globals.bounds.y0 = SHORT_MAX;
	text_bounds_globals.bounds.x0 = SHORT_MAX;
	text_bounds_globals.bounds.y1 = SHORT_MIN;
	text_bounds_globals.bounds.x1 = SHORT_MIN;

	text_bounds_globals.last_font_header = styled_font_get(
		font_drawing_globals.current_font_index,
		font_drawing_globals.current_style);
	draw_unicode_string(text_bounds_draw_character, bounds, &cursor, NULL, 0, string);

	cursor_bounds->x0 = cursor.x;
	cursor_bounds->x1 = (short)(cursor.x + 1);
	cursor_bounds->y0 = (short)(
		cursor.y - text_bounds_globals.last_font_header->ascending_height);
	cursor_bounds->y1 = (short)(
		text_bounds_globals.last_font_header->descending_height + cursor.y);

	text_bounds->x0 = text_bounds_globals.bounds.x0;
	text_bounds->y0 = bounds->y0;
	text_bounds->x1 = text_bounds_globals.bounds.x1;
	text_bounds->y1 = cursor_bounds->y1;

	return;
}

short draw_string_pick(
	rectangle2d const *bounds,
	char const *string,
	point2d const *point)
{
	text_pick_globals.pick_point = *point;
	text_pick_globals.best_pick_distance = SHORT_MAX;
	text_pick_globals.best_pick_string_index = 0;
	text_pick_globals.last_string_index = 0;

	draw_string(text_pick_draw_character, bounds, NULL, NULL, 0, string);
	return (word)text_pick_globals.best_pick_string_index;
}

void bitmap_draw_string(
	struct bitmap_data *bitmap,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	char const *string)
{
	rectangle2d bitmap_bounds;
	rectangle2d bitmap_clip;
	rectangle2d const *effective_bounds = bounds;
	short format = bitmap->format;

	draw_character_software_globals.bitmap = bitmap;

	switch (format)
	{
	case _bitmap_format_a8:
	case _bitmap_format_y8:
	case _bitmap_format_ay8:
	case _bitmap_format_r5g6b5:
	case _bitmap_format_a8r8g8b8:
		break;

	default:
		return;
	}

	if (!effective_bounds)
	{
		set_rectangle2d(
			&bitmap_bounds,
			0,
			0,
			bitmap->width,
			bitmap->height);
		effective_bounds = &bitmap_bounds;
	}

	if (clip)
	{
		set_rectangle2d(
			&bitmap_clip,
			MAX(clip->x0, 0),
			MAX(clip->y0, 0),
			MIN(clip->x1, bitmap->width),
			MIN(clip->y1, bitmap->height));
		clip = &bitmap_clip;
	}

	draw_string(
		bitmap_draw_character,
		effective_bounds,
		NULL,
		clip,
		0,
		string);

	return;
}
