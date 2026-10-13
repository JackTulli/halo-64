/*
VIRTUAL_KEYBOARD.C

symbols in this file:
000E4B50 0100:
	_virtual_keyboard_initialize (0000)
000E4C50 0060:
	_virtual_keyboard_dispose (0000)
000E4CB0 0140:
	_virtual_keyboard_launch (0000)
000E4DF0 0010:
	_virtual_keyboard_active (0000)
000E4E00 0010:
	_virtual_keyboard_last_exit_saved_text (0000)
000E4E10 0050:
	_virtual_keyboard_tab_left (0000)
000E4E60 0050:
	_virtual_keyboard_tab_right (0000)
000E4EB0 0050:
	_virtual_keyboard_tab_up (0000)
000E4F00 0050:
	_virtual_keyboard_tab_down (0000)
000E4F50 0060:
	_virtual_keyboard_cancel (0000)
000E4FB0 00d0:
	_virtual_keyboard_get_character (0000)
000E5080 0030:
	_virtual_keyboard_get_current_character (0000)
000E50B0 0650:
	_virtual_keyboard_render_internal (0000)
000E5700 0020:
	_virtual_keyboard_free_space_in_text_buffer (0000)
000E5720 0060:
	_virtual_keyboard_backspace (0000)
000E5780 0010:
	_virtual_keyboard_close (0000)
000E5790 0010:
	_virtual_keyboard_render (0000)
000E57A0 0440:
	_virtual_keyboard_select (0000)
000E5BE0 0350:
	_virtual_keyboard_process_internal (0000)
000E5F30 0010:
	_virtual_keyboard_process (0000)
00278CC0 0037:
	_virtual_keyboard_key_layout (0000)
00278CF8 0032:
	??_C@_0DC@DOACNNNI@failed?5to?5load?5virtual?5keyboard?5@ (0000)
00278D2C 0032:
	??_C@_0DC@DKJLNFKM@failed?5to?5load?5virtual?5keyboard?5@ (0000)
00278D60 0022:
	??_C@_0CC@BINJJDCB@virtual_keyboard_globals?4keyboar@ (0000)
00278D84 002c:
	??_C@_0CM@FNHOMJL@c?3?2halo?2SOURCE?2interface?2virtual@ (0000)
00278DB0 0073:
	??_C@_0HD@GDAACNMN@?$CIcaption_index?$DO?$DNFIRST_VIRTUAL_KE@ (0000)
00278E28 0053:
	??_C@_0FD@HLHLLBE@text_buffer?5?$CG?$CG?5buffer_size?5?$CG?$CG?5?$CB?$CI@ (0000)
00278E7C 002e:
	??_C@_0CO@ODBBPCAJ@keycode?5?$DM?5NUMBER_OF_CONFIGURABLE@ (0000)
00278EAC 002a:
	??_C@_0CK@MJDENDBH@virtual_keyboard_globals?4keyboar@ (0000)
00278ED8 0015:
	??_C@_0BF@LICBHHNC@?$CB?$CCwhat?5key?5is?5this?$DP?$CC?$AA@ (0000)
00278EF0 0015:
	??_C@_0BF@LNHKMLGP@keyboard_font_header?$AA@ (0000)
00278F08 006b:
	??_C@_0GL@GAMGNLLK@?$CIvirtual_keyboard_globals?4keyboa@ (0000)
00278F74 0012:
	??_C@_1BC@IBHDIJNB@?$AA?4?$AAf?$AAo?$AAr?$AAt?$AAu?$AAn?$AAe?$AA?$AA@ (0000)
00278F88 0027:
	??_C@_0CH@KEKEOCJD@virtual_keyboard_globals?4buffer_@ (0000)
00306338 0160:
	_keyboard_rect (0000)
00454D10 0068:
	_virtual_keyboard_globals (0000)
00454D78 0004:
	?time_of_last_tab@?1??virtual_keyboard_process_internal@@9@9 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "interface/event_manager.h"
#include "interface/ui_widget.h"
#include "interface/virtual_keyboard.h"
#include "input/input.h"
#include "rasterizer/rasterizer.h"
#include "saved games/saved_game_files.h"
#include "tag_files/tag_groups.h"
#include "text/draw_string.h"
#include "text/font_group.h"
#include "text/text_group.h"
#include "text/unicode.h"

/* ---------- constants */

enum
{
	VIRTUAL_KEYBOARD_ROW_COUNT = 5,
	VIRTUAL_KEYBOARD_COLUMN_COUNT = 11,
	FIRST_VIRTUAL_KEYBOARD_CAPTION_STRING_INDEX = 8,
	NUMBER_OF_VIRTUAL_KEYBOARD_STRINGS = 11,
	MAXIMUM_VIRTUAL_KEYBOARD_BUFFER_SIZE = 0x40,
	MAXIMUM_VIRTUAL_KEYBOARD_SAVED_TEXT_LENGTH = 0x20,
	VIRTUAL_KEYBOARD_TAB_REPEAT_MILLISECONDS = 250,
	FIRST_VIRTUAL_KEYBOARD_FORTUNE_STRING_INDEX = 11,
	NUMBER_OF_VIRTUAL_KEYBOARD_FORTUNES = 10,
};

enum virtual_key_code
{
	_vkey_1,
	_vkey_2,
	_vkey_3,
	_vkey_4,
	_vkey_5,
	_vkey_6,
	_vkey_7,
	_vkey_8,
	_vkey_9,
	_vkey_0,
	_vkey_a,
	_vkey_b,
	_vkey_c,
	_vkey_d,
	_vkey_e,
	_vkey_f,
	_vkey_g,
	_vkey_h,
	_vkey_i,
	_vkey_j,
	_vkey_k,
	_vkey_l,
	_vkey_m,
	_vkey_n,
	_vkey_o,
	_vkey_p,
	_vkey_q,
	_vkey_r,
	_vkey_s,
	_vkey_t,
	_vkey_u,
	_vkey_v,
	_vkey_w,
	_vkey_x,
	_vkey_y,
	_vkey_z,
	NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS,
	_vkey_done = NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS,
	_vkey_shift,
	_vkey_caps,
	_vkey_symbols,
	_vkey_backspace,
	_vkey_left,
	_vkey_right,
	_vkey_space,
	NUMBER_OF_VIRTUAL_KEYS,
};

enum
{
	_event_tab_left,
	_event_tab_right,
	_event_tab_up,
	_event_tab_down,
	_event_key_select,
	_event_cancel,
	NUMBER_OF_VIRTUAL_KEYBOARD_EVENTS,
};

/* event_manager.c keeps these private; see header request in the ledger */
enum
{
	_event_type_null,
	_event_type_left_stick,
	_event_type_right_stick,
	_event_type_button,
};

/* ui_widget.c owns the same private enum. */
enum ui_audio_feedback_sound
{
	_ui_audio_feedback_none,
	_ui_audio_feedback_cursor,
	_ui_audio_feedback_forward,
	_ui_audio_feedback_back,
	_ui_audio_feedback_flag_failure,
};

/* ---------- macros */

#define VIRTUAL_KEYBOARD_TAG 'vcky'

#define virtual_keyboard_definition_get(index) \
	((struct virtual_keyboard_definition *)tag_get(VIRTUAL_KEYBOARD_TAG, (index)))
#define virtual_keyboard_key_get(definition, index) \
	((struct virtual_keyboard_key *)(definition)->keys.address + (index))

/* ---------- structures */

struct virtual_keyboard_key
{
	short keycode;
	wchar_t character;
	wchar_t shift_character;
	wchar_t caps_character;
	wchar_t symbols_character;
	wchar_t shift_caps_character;
	wchar_t shift_symbols_character;
	wchar_t caps_symbols_character;
	struct tag_reference unselected_background_bitmap_tag;
	struct tag_reference selected_background_bitmap_tag;
	struct tag_reference active_background_bitmap_tag;
	struct tag_reference sticky_background_bitmap_tag;
};

struct virtual_keyboard_definition
{
	struct tag_reference font_tag;
	struct tag_reference background_bitmap_tag;
	struct tag_reference special_key_labels_string_list_tag;
	struct tag_block keys;
};

/* font_group.h leaves this incomplete; draw_string.c and rasterizer_text.c define it the same way */
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

struct virtual_keyboard_globals
{
	boolean active;
	boolean shift_active;
	boolean caps_active;
	boolean symbols_active;
	struct virtual_keyboard_definition *keyboard;
	short row;
	short column;
	word buffer_size;
	short last_event;
	short last_key;
	short number_of_event_repeats;
	short caption_index;
	boolean last_exit_saved_text;
	boolean first_key_replaces_buffer;
	wchar_t *text_buffer;
	wchar_t *cursor;
	unsigned long time_of_last_event;
	long caret_bitmap_index;
	wchar_t saved_text[MAXIMUM_VIRTUAL_KEYBOARD_SAVED_TEXT_LENGTH];
};

typedef char verify_virtual_keyboard_globals_size[
	sizeof(struct virtual_keyboard_globals) == 0x68 ? 1 : -1];
typedef char verify_virtual_keyboard_key_size[
	sizeof(struct virtual_keyboard_key) == 0x50 ? 1 : -1];

/* ---------- prototypes */

static boolean virtual_keyboard_cancel(
	void);
static wchar_t virtual_keyboard_get_character(
	word keycode);
wchar_t virtual_keyboard_get_current_character(
	void);
static void virtual_keyboard_render_internal(
	void);
static boolean virtual_keyboard_select(
	void);
static void virtual_keyboard_process_internal(
	void);

/* ---------- globals */

char const virtual_keyboard_key_layout[VIRTUAL_KEYBOARD_ROW_COUNT][VIRTUAL_KEYBOARD_COLUMN_COUNT] =
{
	{ _vkey_done, _vkey_1, _vkey_2, _vkey_3, _vkey_4, _vkey_5, _vkey_6, _vkey_7, _vkey_8, _vkey_9, _vkey_0 },
	{ _vkey_shift, _vkey_a, _vkey_b, _vkey_c, _vkey_d, _vkey_e, _vkey_f, _vkey_g, _vkey_h, _vkey_i, _vkey_j },
	{ _vkey_caps, _vkey_k, _vkey_l, _vkey_m, _vkey_n, _vkey_o, _vkey_p, _vkey_q, _vkey_r, _vkey_s, _vkey_t },
	{ _vkey_symbols, _vkey_u, _vkey_v, _vkey_w, _vkey_x, _vkey_y, _vkey_z, _vkey_backspace, _vkey_backspace, _vkey_backspace, _vkey_backspace },
	{ _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_space, _vkey_left, _vkey_left, _vkey_right, _vkey_right },
};

static rectangle2d keyboard_rect[NUMBER_OF_VIRTUAL_KEYS] =
{
	{ 158, 208, 190, 240 }, /* 1 */
	{ 158, 240, 190, 272 }, /* 2 */
	{ 158, 272, 190, 304 }, /* 3 */
	{ 158, 304, 190, 336 }, /* 4 */
	{ 158, 336, 190, 368 }, /* 5 */
	{ 158, 368, 190, 400 }, /* 6 */
	{ 158, 400, 190, 432 }, /* 7 */
	{ 158, 432, 190, 464 }, /* 8 */
	{ 158, 464, 190, 496 }, /* 9 */
	{ 158, 496, 190, 528 }, /* 0 */
	{ 190, 208, 222, 240 }, /* a */
	{ 190, 240, 222, 272 }, /* b */
	{ 190, 272, 222, 304 }, /* c */
	{ 190, 304, 222, 336 }, /* d */
	{ 190, 336, 222, 368 }, /* e */
	{ 190, 368, 222, 400 }, /* f */
	{ 190, 400, 222, 432 }, /* g */
	{ 190, 432, 222, 464 }, /* h */
	{ 190, 464, 222, 496 }, /* i */
	{ 190, 496, 222, 528 }, /* j */
	{ 222, 208, 254, 240 }, /* k */
	{ 222, 240, 254, 272 }, /* l */
	{ 222, 272, 254, 304 }, /* m */
	{ 222, 304, 254, 336 }, /* n */
	{ 222, 336, 254, 368 }, /* o */
	{ 222, 368, 254, 400 }, /* p */
	{ 222, 400, 254, 432 }, /* q */
	{ 222, 432, 254, 464 }, /* r */
	{ 222, 464, 254, 496 }, /* s */
	{ 222, 496, 254, 528 }, /* t */
	{ 254, 208, 286, 240 }, /* u */
	{ 254, 240, 286, 272 }, /* v */
	{ 254, 272, 286, 304 }, /* w */
	{ 254, 304, 286, 336 }, /* x */
	{ 254, 336, 286, 368 }, /* y */
	{ 254, 368, 286, 400 }, /* z */
	{ 158, 112, 190, 208 }, /* done */
	{ 190, 112, 222, 208 }, /* shift */
	{ 222, 112, 254, 208 }, /* caps */
	{ 254, 112, 286, 208 }, /* symbols */
	{ 254, 400, 286, 528 }, /* backspace */
	{ 286, 400, 318, 464 }, /* left */
	{ 286, 464, 318, 528 }, /* right */
	{ 286, 208, 318, 400 }, /* space */
};

struct virtual_keyboard_globals virtual_keyboard_globals= {0};

/* ---------- public code */

boolean virtual_keyboard_initialize(
	void)
{
	long keyboard_index;

	virtual_keyboard_globals.active = FALSE;
	virtual_keyboard_globals.shift_active = FALSE;
	virtual_keyboard_globals.caps_active = FALSE;
	virtual_keyboard_globals.symbols_active = FALSE;

	keyboard_index = tag_loaded(VIRTUAL_KEYBOARD_TAG, "ui\\english");
	if (keyboard_index != NONE)
	{
		virtual_keyboard_globals.keyboard = virtual_keyboard_definition_get(keyboard_index);
		match_assert(
			"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
			364,
			virtual_keyboard_globals.keyboard);

		virtual_keyboard_globals.row = 0;
		virtual_keyboard_globals.column = 0;
		virtual_keyboard_globals.buffer_size = 0;
		virtual_keyboard_globals.last_event = NONE;
		virtual_keyboard_globals.last_key = NONE;
		virtual_keyboard_globals.number_of_event_repeats = 0;
		virtual_keyboard_globals.text_buffer = NULL;
		virtual_keyboard_globals.cursor = NULL;
		virtual_keyboard_globals.time_of_last_event = 0;
	}
	else
	{
		error(2, "failed to load virtual keyboard for '%s' language", "<unknown>");
	}

	virtual_keyboard_globals.caret_bitmap_index =
		tag_loaded(BITMAP_GROUP_TAG, "ui\\shell\\bitmaps\\white");
	if (virtual_keyboard_globals.caret_bitmap_index == NONE)
	{
		error(
			2,
			"failed to load virtual keyboard caret bitmap '%s'",
			"ui\\shell\\bitmaps\\white");
	}

	return virtual_keyboard_globals.keyboard != NULL;
}

void virtual_keyboard_dispose(
	void)
{
	virtual_keyboard_globals.active = FALSE;
	virtual_keyboard_globals.shift_active = FALSE;
	virtual_keyboard_globals.caps_active = FALSE;
	virtual_keyboard_globals.symbols_active = FALSE;
	virtual_keyboard_globals.keyboard = NULL;
	virtual_keyboard_globals.row = 0;
	virtual_keyboard_globals.column = 0;
	virtual_keyboard_globals.buffer_size = 0;
	virtual_keyboard_globals.last_event = NONE;
	virtual_keyboard_globals.last_key = NONE;
	virtual_keyboard_globals.number_of_event_repeats = 0;
	virtual_keyboard_globals.text_buffer = NULL;
	virtual_keyboard_globals.cursor = NULL;
	virtual_keyboard_globals.time_of_last_event = 0;

	event_manager_flush();

	return;
}

boolean virtual_keyboard_launch(
	wchar_t *text_buffer,
	word buffer_size,
	short caption_index)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		418,
		text_buffer && buffer_size && !(buffer_size&1) && !virtual_keyboard_globals.active);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		419,
		(caption_index>=FIRST_VIRTUAL_KEYBOARD_CAPTION_STRING_INDEX) && (caption_index<NUMBER_OF_VIRTUAL_KEYBOARD_STRINGS));

	if (!virtual_keyboard_globals.active && virtual_keyboard_globals.keyboard)
	{
		event_manager_flush();
		virtual_keyboard_globals.row = 0;
		virtual_keyboard_globals.column = 0;
		virtual_keyboard_globals.active = TRUE;
		virtual_keyboard_globals.text_buffer = text_buffer;
		virtual_keyboard_globals.cursor = text_buffer + ustrlen(text_buffer);
		virtual_keyboard_globals.buffer_size = buffer_size;
		if (virtual_keyboard_globals.buffer_size >= MAXIMUM_VIRTUAL_KEYBOARD_BUFFER_SIZE)
			virtual_keyboard_globals.buffer_size = MAXIMUM_VIRTUAL_KEYBOARD_BUFFER_SIZE;
		virtual_keyboard_globals.last_event = NONE;
		virtual_keyboard_globals.time_of_last_event = system_milliseconds();
		virtual_keyboard_globals.caption_index = caption_index;
		virtual_keyboard_globals.shift_active = FALSE;
		virtual_keyboard_globals.caps_active = FALSE;
		virtual_keyboard_globals.symbols_active = FALSE;
		virtual_keyboard_globals.first_key_replaces_buffer = TRUE;
		ustrncpy(
			virtual_keyboard_globals.saved_text,
			text_buffer,
			MAXIMUM_VIRTUAL_KEYBOARD_SAVED_TEXT_LENGTH);
		virtual_keyboard_globals.saved_text[MAXIMUM_VIRTUAL_KEYBOARD_SAVED_TEXT_LENGTH - 1] = L'\0';
		virtual_keyboard_globals.last_exit_saved_text = FALSE;
		ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
		result = TRUE;
	}

	return result;
}

boolean virtual_keyboard_active(
	void)
{
	return virtual_keyboard_globals.active;
}

boolean virtual_keyboard_last_exit_saved_text(
	void)
{
	return virtual_keyboard_globals.last_exit_saved_text;
}

static boolean virtual_keyboard_tab_left(
	void)
{
	char keycode = virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column];

	do
	{
		virtual_keyboard_globals.column--;
		if (virtual_keyboard_globals.column < 0)
			virtual_keyboard_globals.column = VIRTUAL_KEYBOARD_COLUMN_COUNT - 1;
	}
	while (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == keycode);

	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return TRUE;
}

static boolean virtual_keyboard_tab_right(
	void)
{
	char keycode = virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column];

	do
	{
		virtual_keyboard_globals.column++;
		if (virtual_keyboard_globals.column == VIRTUAL_KEYBOARD_COLUMN_COUNT)
			virtual_keyboard_globals.column = 0;
	}
	while (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == keycode);

	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return TRUE;
}

static boolean virtual_keyboard_tab_up(
	void)
{
	char keycode = virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column];

	do
	{
		virtual_keyboard_globals.row--;
		if (virtual_keyboard_globals.row < 0)
			virtual_keyboard_globals.row = VIRTUAL_KEYBOARD_ROW_COUNT - 1;
	}
	while (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == keycode);

	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return TRUE;
}

static boolean virtual_keyboard_tab_down(
	void)
{
	char keycode = virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column];

	do
	{
		virtual_keyboard_globals.row++;
		if (virtual_keyboard_globals.row == VIRTUAL_KEYBOARD_ROW_COUNT)
			virtual_keyboard_globals.row = 0;
	}
	while (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == keycode);

	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return TRUE;
}

static boolean virtual_keyboard_cancel(
	void)
{
	virtual_keyboard_globals.active = FALSE;
	if (virtual_keyboard_globals.text_buffer)
	{
		ustrncpy(
			virtual_keyboard_globals.text_buffer,
			virtual_keyboard_globals.saved_text,
			virtual_keyboard_globals.buffer_size / sizeof(wchar_t));
		virtual_keyboard_globals.text_buffer[
			virtual_keyboard_globals.buffer_size / sizeof(wchar_t) - 1] = L'\0';
	}

	virtual_keyboard_globals.text_buffer = NULL;
	virtual_keyboard_globals.saved_text[0] = L'\0';
	virtual_keyboard_globals.last_exit_saved_text = FALSE;
	ui_play_audio_feedback_sound(_ui_audio_feedback_back);

	return TRUE;
}

static wchar_t virtual_keyboard_get_character(
	word keycode)
{
	struct virtual_keyboard_key *key;
	wchar_t character;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		987,
		virtual_keyboard_globals.keyboard != NULL);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		988,
		keycode < NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS);

	key = virtual_keyboard_key_get(virtual_keyboard_globals.keyboard, keycode);
	if (virtual_keyboard_globals.shift_active)
	{
		if (virtual_keyboard_globals.caps_active)
			character = key->shift_caps_character;
		else if (virtual_keyboard_globals.symbols_active)
			character = key->shift_symbols_character;
		else
			character = key->shift_character;
	}
	else
	{
		if (virtual_keyboard_globals.caps_active)
		{
			if (virtual_keyboard_globals.symbols_active)
				character = key->caps_symbols_character;
			else
				character = key->caps_character;
		}
		else if (virtual_keyboard_globals.symbols_active)
			character = key->symbols_character;
		else
			character = key->character;
	}

	if (!character)
		character = 0x7F;

	return character;
}

wchar_t virtual_keyboard_get_current_character(
	void)
{
	return virtual_keyboard_get_character(virtual_keyboard_key_layout[
		virtual_keyboard_globals.row][virtual_keyboard_globals.column]);
}

static void virtual_keyboard_render_internal(
	void)
{
	real_argb_color caption_color;
	real_argb_color text_color;
	struct font_header *keyboard_font_header;

	text_color.alpha = 1.0f;
	text_color.red = 0.9f;
	text_color.green = 0.9f;
	text_color.blue = 0.9f;
	caption_color = text_color;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		1036,
		(virtual_keyboard_globals.keyboard != NULL) && (virtual_keyboard_globals.keyboard->font_tag.index != NONE));
	keyboard_font_header = font_definition_get(virtual_keyboard_globals.keyboard->font_tag.index);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
		1038,
		keyboard_font_header);

	if (virtual_keyboard_globals.keyboard->background_bitmap_tag.index != NONE)
	{
		rectangle2d bounds;
		struct bitmap_data *bitmap;

		bounds.y0 = 0;
		bounds.x0 = 0;
		bounds.y1 = 480;
		bounds.x1 = 640;
		bitmap = bitmap_group_get_bitmap_from_sequence(virtual_keyboard_globals.keyboard->background_bitmap_tag.index, 0, 0);
		match_assert(
			"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
			1046,
			bitmap);
		draw_bitmap_in_rect(bitmap, &bounds, &bounds, NULL, 0xffffffff, NULL, FALSE);
	}

	draw_string_set_draw_mode(virtual_keyboard_globals.keyboard->font_tag.index, NONE, 0, 0, &caption_color);
	if (virtual_keyboard_globals.keyboard->special_key_labels_string_list_tag.index != NONE)
	{
		rectangle2d bounds;
		wchar_t *caption = unicode_string_list_get_string(
			virtual_keyboard_globals.keyboard->special_key_labels_string_list_tag.index,
			virtual_keyboard_globals.caption_index);

		bounds.y0 = 78;
		bounds.x0 = 114;
		bounds.y1 = 110;
		bounds.x1 = 640;
		rasterizer_draw_unicode_string(&bounds, &bounds, NULL, 0, caption);
	}

	draw_string_set_draw_mode(virtual_keyboard_globals.keyboard->font_tag.index, NONE, 2, 0, &text_color);
	{
		rectangle2d bounds;

		bounds.y0 = 118;
		bounds.x0 = 220;
		bounds.y1 = 143;
		bounds.x1 = 420;
		if (virtual_keyboard_globals.first_key_replaces_buffer == TRUE)
		{
			struct bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(virtual_keyboard_globals.caret_bitmap_index, 0, 0);

			if (bitmap)
			{
				rectangle2d text_bounds;
				rectangle2d cursor_bounds;

				draw_unicode_string_compute_bounds(&bounds, virtual_keyboard_globals.text_buffer, &text_bounds, &cursor_bounds);
				text_bounds.x0 -= 2;
				text_bounds.x1 += 2;
				draw_bitmap_in_rect(bitmap, &text_bounds, &bounds, NULL, 0x7f7f7f7f, NULL, FALSE);
			}
		}
		rasterizer_draw_unicode_string(&bounds, &bounds, NULL, 0, virtual_keyboard_globals.text_buffer);
	}

	if (!virtual_keyboard_globals.first_key_replaces_buffer &&
		virtual_keyboard_globals.caret_bitmap_index != NONE &&
		(system_milliseconds() / 1000) & 1)
	{
		wchar_t *character = virtual_keyboard_globals.text_buffer;
		short height = keyboard_font_header->descending_height + keyboard_font_header->ascending_height;
		short cursor_offset = 0;
		short width = 0;
		struct bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(virtual_keyboard_globals.caret_bitmap_index, 0, 0);
		rectangle2d caret_bounds;

		if (bitmap)
		{
			/* January tests the pointer, not the character; the terminator ends the walk because the font has no glyph for it */
			while (character)
			{
				struct font_character *font_character = font_get_character_by_ascii_code(keyboard_font_header, *character);

				if (!font_character)
					break;
				if (character < virtual_keyboard_globals.cursor)
					cursor_offset += font_character->character_width;
				width += font_character->character_width;
				character++;
			}

			/* 320 is the horizontal center of the 220..420 text box */
			caret_bounds.x0 = 320 - (width >> 1) + cursor_offset;
			caret_bounds.x1 = caret_bounds.x0 + 1;
			caret_bounds.y0 = 120;
			caret_bounds.y1 = height + 120;
			draw_bitmap_in_rect(bitmap, &caret_bounds, NULL, NULL, 0xffffffff, NULL, FALSE);
		}
	}

	draw_string_set_draw_mode(virtual_keyboard_globals.keyboard->font_tag.index, NONE, 2, 0, &text_color);
	{
		struct virtual_keyboard_key *keys = virtual_keyboard_globals.keyboard->keys.address;
		wchar_t string[24] = {0};
		long key_index;

		for (key_index = 0; key_index < NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS; key_index++)
		{
			struct virtual_keyboard_key *key = &keys[key_index];
			struct font_character *font_character;
			long bitmap_index;
			struct bitmap_data *bitmap;

			string[0] = virtual_keyboard_get_character((word)key_index);
			font_character = font_get_character_by_ascii_code(keyboard_font_header, string[0]);
			if (!font_character)
			{
				string[0] = 0x7F;
				font_character = font_get_character_by_ascii_code(keyboard_font_header, string[0]);
			}
			if (font_character)
			{
				rectangle2d key_bounds = keyboard_rect[key_index];

				key_bounds.x0 += 2;
				key_bounds.x1 += 2;
				key_bounds.y0 += 5;
				key_bounds.y1 += 5;
				rasterizer_draw_unicode_string(&key_bounds, NULL, NULL, 0, string);
			}

			if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == key_index)
			{
				if (virtual_keyboard_globals.last_event == _event_key_select)
					bitmap_index = key->active_background_bitmap_tag.index;
				else
					bitmap_index = key->selected_background_bitmap_tag.index;
			}
			else
			{
				bitmap_index = key->unselected_background_bitmap_tag.index;
			}

			if (bitmap_index != NONE)
			{
				bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_index, 0, 0);
				if (bitmap)
					draw_bitmap_in_rect(bitmap, &keyboard_rect[key_index], NULL, NULL, 0xffffffff, NULL, FALSE);
			}
		}

		if (virtual_keyboard_globals.keyboard->special_key_labels_string_list_tag.index != NONE)
		{
			for (; key_index < NUMBER_OF_VIRTUAL_KEYS; key_index++)
			{
				struct virtual_keyboard_key *key = &keys[key_index];
				wchar_t *label = unicode_string_list_get_string(
					virtual_keyboard_globals.keyboard->special_key_labels_string_list_tag.index,
					(short)(key_index - NUMBER_OF_CONFIGURABLE_VIRTUAL_KEYS));
				long bitmap_index = NONE;
				struct bitmap_data *bitmap = NULL;

				switch (key_index)
				{
				case _vkey_done:
				case _vkey_backspace:
				case _vkey_left:
				case _vkey_right:
				case _vkey_space:
					if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == key_index)
					{
						if (virtual_keyboard_globals.last_event == _event_key_select)
							bitmap_index = key->active_background_bitmap_tag.index;
						else
							bitmap_index = key->selected_background_bitmap_tag.index;
					}
					else
					{
						bitmap_index = key->unselected_background_bitmap_tag.index;
					}
					break;

				case _vkey_shift:
					if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == key_index)
					{
						if (virtual_keyboard_globals.last_event == _event_key_select)
							bitmap_index = key->active_background_bitmap_tag.index;
						else
							bitmap_index = key->selected_background_bitmap_tag.index;
					}
					else if (virtual_keyboard_globals.shift_active)
					{
						bitmap_index = key->sticky_background_bitmap_tag.index;
					}
					else
					{
						bitmap_index = key->unselected_background_bitmap_tag.index;
					}
					break;

				case _vkey_caps:
					if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == key_index)
					{
						if (virtual_keyboard_globals.last_event == _event_key_select)
							bitmap_index = key->active_background_bitmap_tag.index;
						else
							bitmap_index = key->selected_background_bitmap_tag.index;
					}
					else if (virtual_keyboard_globals.caps_active)
					{
						bitmap_index = key->sticky_background_bitmap_tag.index;
					}
					else
					{
						bitmap_index = key->unselected_background_bitmap_tag.index;
					}
					break;

				case _vkey_symbols:
					if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] == key_index)
					{
						if (virtual_keyboard_globals.last_event == _event_key_select)
							bitmap_index = key->active_background_bitmap_tag.index;
						else
							bitmap_index = key->selected_background_bitmap_tag.index;
					}
					else if (virtual_keyboard_globals.symbols_active)
					{
						bitmap_index = key->sticky_background_bitmap_tag.index;
					}
					else
					{
						bitmap_index = key->unselected_background_bitmap_tag.index;
					}
					break;

				default:
					match_assert(
						"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
						1408,
						!"what key is this?");
				}

				if (bitmap_index != NONE)
					bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_index, 0, 0);

				if (label)
				{
					rectangle2d label_bounds = keyboard_rect[key_index];

					label_bounds.x0 += 2;
					label_bounds.x1 += 2;
					label_bounds.y0 += 5;
					label_bounds.y1 += 5;
					rasterizer_draw_unicode_string(&label_bounds, NULL, NULL, 0, label);
				}
				if (bitmap)
					draw_bitmap_in_rect(bitmap, &keyboard_rect[key_index], &keyboard_rect[key_index], NULL, 0xffffffff, NULL, FALSE);
			}
		}
	}

	return;
}

long virtual_keyboard_free_space_in_text_buffer(
	void)
{
	return virtual_keyboard_globals.buffer_size -
		2 * (ustrlen(virtual_keyboard_globals.text_buffer) + 1);
}

static void virtual_keyboard_backspace(
	void)
{
	if (virtual_keyboard_globals.cursor > virtual_keyboard_globals.text_buffer)
	{
		long remaining_size = virtual_keyboard_globals.buffer_size -
			((byte *)virtual_keyboard_globals.cursor - (byte *)virtual_keyboard_globals.text_buffer);

		if (remaining_size >= 0)
		{
			csmemmove(
				virtual_keyboard_globals.cursor - 1,
				virtual_keyboard_globals.cursor,
				remaining_size);
			virtual_keyboard_globals.text_buffer[virtual_keyboard_globals.buffer_size / 2 - 1] = L'\0';
			virtual_keyboard_globals.cursor--;
		}
	}

	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return;
}

void virtual_keyboard_close(
	void)
{
	virtual_keyboard_cancel();

	return;
}

void virtual_keyboard_render(
	void)
{
	if (virtual_keyboard_globals.active)
		virtual_keyboard_render_internal();

	return;
}

void virtual_keyboard_process(
	void)
{
	if (virtual_keyboard_globals.active)
		virtual_keyboard_process_internal();

	return;
}

/* ---------- private code */

static boolean virtual_keyboard_select(
	void)
{
	short keycode = virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column];

	switch (keycode)
	{
	case _vkey_done:
		if (ustrcmp(virtual_keyboard_globals.saved_text, virtual_keyboard_globals.text_buffer) != 0)
		{
			if (virtual_keyboard_globals.text_buffer[0])
			{
				if (saved_game_file_name_unique(virtual_keyboard_globals.text_buffer))
				{
					virtual_keyboard_globals.last_exit_saved_text = TRUE;
				}
				else
				{
					display_error(_error_already_a_saved_game_file_with_that_name, NONE, TRUE, FALSE);
					virtual_keyboard_cancel();
				}
			}
			else
			{
				display_error(_error_cannot_create_saved_game_file_with_empty_name, NONE, TRUE, FALSE);
				virtual_keyboard_cancel();
			}
		}
		else
		{
			virtual_keyboard_globals.last_exit_saved_text = TRUE;
		}
		ui_play_audio_feedback_sound(_ui_audio_feedback_back);
		virtual_keyboard_globals.active = FALSE;
		event_manager_flush();
		break;

	case _vkey_shift:
		ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
		virtual_keyboard_globals.shift_active = !virtual_keyboard_globals.shift_active;
		break;

	case _vkey_caps:
		ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
		virtual_keyboard_globals.caps_active = !virtual_keyboard_globals.caps_active;
		break;

	case _vkey_symbols:
		ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
		virtual_keyboard_globals.symbols_active = !virtual_keyboard_globals.symbols_active;
		break;

	case _vkey_backspace:
		if (virtual_keyboard_globals.first_key_replaces_buffer == TRUE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
				854,
				virtual_keyboard_globals.buffer_size>0);
			csmemset(
				virtual_keyboard_globals.text_buffer,
				0,
				virtual_keyboard_globals.buffer_size);
			virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer;
			virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
		}
		else
		{
			virtual_keyboard_backspace();
		}
		break;

	case _vkey_left:
		if (virtual_keyboard_globals.cursor > virtual_keyboard_globals.text_buffer)
			virtual_keyboard_globals.cursor--;
		virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
		ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
		break;

	case _vkey_right:
		if (*virtual_keyboard_globals.cursor)
			virtual_keyboard_globals.cursor++;
		virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
		ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
		break;

	case _vkey_space:
		if (virtual_keyboard_globals.first_key_replaces_buffer == TRUE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
				889,
				virtual_keyboard_globals.buffer_size>0);
			csmemset(
				virtual_keyboard_globals.text_buffer,
				0,
				virtual_keyboard_globals.buffer_size);
			virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer;
			virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
		}
		if (virtual_keyboard_free_space_in_text_buffer() >= 2)
		{
			csmemmove(
				virtual_keyboard_globals.cursor + 1,
				virtual_keyboard_globals.cursor,
				virtual_keyboard_globals.buffer_size - ((byte *)virtual_keyboard_globals.cursor - (byte *)virtual_keyboard_globals.text_buffer) - sizeof(wchar_t));
			*virtual_keyboard_globals.cursor++ = L' ';
			ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
		}
		else
		{
			ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
		}
		break;

	default:
		if (virtual_keyboard_globals.first_key_replaces_buffer == TRUE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
				914,
				virtual_keyboard_globals.buffer_size>0);
			csmemset(
				virtual_keyboard_globals.text_buffer,
				0,
				virtual_keyboard_globals.buffer_size);
			virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer;
			virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
		}
		{
			long buffer_size = virtual_keyboard_globals.buffer_size;
			long free_space = buffer_size - 2 * (ustrlen(virtual_keyboard_globals.text_buffer) + 1);

			if (free_space >= 2)
			{
				csmemmove(
					virtual_keyboard_globals.cursor + 1,
					virtual_keyboard_globals.cursor,
					buffer_size - ((byte *)virtual_keyboard_globals.cursor - (byte *)virtual_keyboard_globals.text_buffer) - sizeof(wchar_t));
				*virtual_keyboard_globals.cursor++ = virtual_keyboard_get_character(
					virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column]);
				if (ustrcmp(virtual_keyboard_globals.text_buffer, L".fortune") == 0)
				{
					unsigned long fortune_index = system_milliseconds() % NUMBER_OF_VIRTUAL_KEYBOARD_FORTUNES;
					fortune_index = MIN(fortune_index, NUMBER_OF_VIRTUAL_KEYBOARD_FORTUNES - 1);
					virtual_keyboard_globals.caption_index = FIRST_VIRTUAL_KEYBOARD_FORTUNE_STRING_INDEX +
						fortune_index;
					if (virtual_keyboard_globals.saved_text[0])
					{
						ustrcpy(virtual_keyboard_globals.text_buffer, virtual_keyboard_globals.saved_text);
						virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer +
							ustrlen(virtual_keyboard_globals.text_buffer);
					}
					else
					{
						csmemset(
							virtual_keyboard_globals.text_buffer,
							0,
							virtual_keyboard_globals.buffer_size);
						virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer;
					}
				}
				ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
			}
			else
			{
				ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
			}
		}
		break;
	}

	if (virtual_keyboard_key_layout[virtual_keyboard_globals.row][virtual_keyboard_globals.column] != _vkey_shift)
		virtual_keyboard_globals.shift_active = FALSE;

	return TRUE;
}

static void virtual_keyboard_process_internal(
	void)
{
	static unsigned long time_of_last_tab= {0};
	unsigned long time = system_milliseconds();
	struct event_record event;
	long action = NONE;
	boolean handled = FALSE;

	while (get_next_event(&event, NONE))
	{
		switch (event.type)
		{
		case _event_type_left_stick:
			if (event.data.stick.y == SHORT_MAX)
				action = _event_tab_up;
			else if (event.data.stick.y == SHORT_MIN)
				action = _event_tab_down;
			else if (event.data.stick.x == SHORT_MIN)
				action = _event_tab_left;
			else if (event.data.stick.x == SHORT_MAX)
				action = _event_tab_right;
			break;

		case _event_type_button:
			switch (event.data.button.index)
			{
			case _gamepad_analog_button_a:
				if (event.data.button.value == 1)
					action = _event_key_select;
				break;

			case _gamepad_analog_button_b:
			case _gamepad_binary_button_back:
				if (event.data.button.value == 1)
					action = _event_cancel;
				break;

			case _gamepad_binary_button_dpad_up:
				if (virtual_keyboard_globals.last_event != _event_tab_up ||
					time - time_of_last_tab >= VIRTUAL_KEYBOARD_TAB_REPEAT_MILLISECONDS ||
					event.data.button.value == 1)
				{
					action = _event_tab_up;
					time_of_last_tab = time;
				}
				break;

			case _gamepad_binary_button_dpad_left:
				if (virtual_keyboard_globals.last_event != _event_tab_left ||
					time - time_of_last_tab >= VIRTUAL_KEYBOARD_TAB_REPEAT_MILLISECONDS ||
					event.data.button.value == 1)
				{
					action = _event_tab_left;
					time_of_last_tab = time;
				}
				break;

			case _gamepad_binary_button_dpad_down:
				if (virtual_keyboard_globals.last_event != _event_tab_down ||
					time - time_of_last_tab >= VIRTUAL_KEYBOARD_TAB_REPEAT_MILLISECONDS ||
					event.data.button.value == 1)
				{
					action = _event_tab_down;
					time_of_last_tab = time;
				}
				break;

			case _gamepad_binary_button_dpad_right:
				if (virtual_keyboard_globals.last_event != _event_tab_right ||
					time - time_of_last_tab >= VIRTUAL_KEYBOARD_TAB_REPEAT_MILLISECONDS ||
					event.data.button.value == 1)
				{
					action = _event_tab_right;
					time_of_last_tab = time;
				}
				break;

			case _gamepad_binary_button_start:
				if (event.data.button.value == 1)
				{
					virtual_keyboard_globals.row = 0;
					virtual_keyboard_globals.column = 0;
					action = _event_key_select;
				}
				break;

			case _gamepad_analog_button_x:
				if (event.data.button.value == 1)
				{
					if (virtual_keyboard_globals.first_key_replaces_buffer == TRUE)
					{
						match_assert(
							"c:\\halo\\SOURCE\\interface\\virtual_keyboard.c",
							635,
							virtual_keyboard_globals.buffer_size>0);
						csmemset(
							virtual_keyboard_globals.text_buffer,
							0,
							virtual_keyboard_globals.buffer_size);
						virtual_keyboard_globals.cursor = virtual_keyboard_globals.text_buffer;
						virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
						ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
					}
					else
					{
						virtual_keyboard_backspace();
					}
					handled = TRUE;
				}
				break;

			case _gamepad_analog_button_left_trigger:
				if (event.data.button.value == 1)
				{
					virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
					if (virtual_keyboard_globals.cursor > virtual_keyboard_globals.text_buffer)
						virtual_keyboard_globals.cursor--;
					ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
					handled = TRUE;
				}
				break;

			case _gamepad_analog_button_right_trigger:
				if (event.data.button.value == 1)
				{
					virtual_keyboard_globals.first_key_replaces_buffer = FALSE;
					if (*virtual_keyboard_globals.cursor)
						virtual_keyboard_globals.cursor++;
					ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);
					handled = TRUE;
				}
				break;
			}
			break;
		}
	}

	if (action != NONE)
	{
		virtual_keyboard_globals.last_key = virtual_keyboard_key_layout[
			virtual_keyboard_globals.row][virtual_keyboard_globals.column];
		switch (action)
		{
		case _event_tab_left:
			handled = virtual_keyboard_tab_left();
			break;
		case _event_tab_right:
			handled = virtual_keyboard_tab_right();
			break;
		case _event_tab_up:
			handled = virtual_keyboard_tab_up();
			break;
		case _event_tab_down:
			handled = virtual_keyboard_tab_down();
			break;
		case _event_key_select:
			handled = virtual_keyboard_select();
			break;
		case _event_cancel:
			handled = virtual_keyboard_cancel();
			break;
		}

		if (handled == TRUE)
		{
			virtual_keyboard_globals.time_of_last_event = time;
			virtual_keyboard_globals.last_event = action;
		}
	}

	return;
}
