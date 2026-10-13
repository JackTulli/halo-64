/*
HUD_MESSAGING.C

symbols in this file:
000C3810 0080:
	_render_state_text (0000)
000C3890 0190:
	_render_state_bitmap (0000)
000C3A20 0020:
	_hud_messaging_initialize (0000)
000C3A40 0030:
	_hud_messaging_initialize_for_new_map (0000)
000C3A70 0010:
	_hud_messaging_dispose_from_old_map (0000)
000C3A80 0010:
	_hud_messaging_dispose (0000)
000C3A90 0050:
	_scripted_hud_set_state_message (0000)
000C3AE0 0040:
	_scripted_hud_set_flashing_state (0000)
000C3B20 0040:
	_scripted_hud_restart_flashing (0000)
000C3B60 00a0:
	_scripted_hud_set_objective (0000)
000C3C00 0080:
	_scripted_hud_set_timer_time (0000)
000C3C80 0020:
	_scripted_hud_set_timer_warning_cutoff (0000)
000C3CA0 0060:
	_scripted_hud_set_timer_position (0000)
000C3D00 0020:
	_scripted_hud_show_timer (0000)
000C3D20 0050:
	_scripted_hud_pause_timer (0000)
000C3D70 0050:
	_scripted_hud_get_timer_ticks (0000)
000C3DC0 0030:
	_scripted_hud_time_code_show (0000)
000C3DF0 0040:
	_scripted_hud_time_code_start (0000)
000C3E30 0020:
	_scripted_hud_time_code_reset (0000)
000C3E50 02c0:
	_hud_render_timer (0000)
000C4110 00a0:
	_hud_set_state_message (0000)
000C41B0 0060:
	_hud_set_state_message_icon (0000)
000C4210 0070:
	_hud_set_state_message_text (0000)
000C4280 0070:
	_hud_enable_custom_state_message (0000)
000C42F0 0040:
	_hud_set_state_text (0000)
000C4330 00c0:
	_hud_messaging_get_objective (0000)
000C43F0 0080:
	_find_free_hud_message (0000)
000C4470 0030:
	_compare_messages (0000)
000C44A0 0040:
	_scripted_hud_messages_clear (0000)
000C44E0 0020:
	_hud_get_font_index (0000)
000C4500 0030:
	_hud_get_text_color (0000)
000C4530 0010:
	_hud_messaging_globals_update (0000)
000C4540 0080:
	_hud_print_message (0000)
000C45C0 00a0:
	_hud_add_item_message (0000)
000C4660 0070:
	_hud_broadcast_team_message (0000)
000C46D0 0b70:
	_hud_messaging_update (0000)
0026FEE4 000e:
	??_C@_0O@JAAAMDJP@hud?5messaging?$AA@ (0000)
0026FEF4 003f:
	??_C@_0DP@DEHGFPPE@trying?5to?5restart?5help?5text?5flas@ (0000)
0026FF34 002b:
	??_C@_0CL@KDPBOCMB@objective?5text?5MUST?5only?5be?5text@ (0000)
0026FF60 0008:
	__real@4004000000000000 (0000)
0026FF68 0029:
	??_C@_0CJ@FOFBAHGJ@c?3?2halo?2SOURCE?2interface?2hud_mes@ (0000)
0026FF94 0026:
	??_C@_0CG@FLCOGOLF@element?9?$DOtype?$DN?$DN_hud_message_type@ (0000)
0026FFBC 001a:
	??_C@_0BK@PKALLFKO@message?9?$DOelement_count?$DN?$DN1?$AA@ (0000)
0026FFD8 0022:
	??_C@_1CC@FMBGLMAK@?$AA?$DM?$AAn?$AAo?$AA?5?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?5?$AAi?$AAc?$AAo?$AAn?$AA?$DO?$AA?$AA@ (0000)
0026FFFC 0022:
	??_C@_0CC@OCKONGBA@help?5text?5cannot?5use?5custom?5icon@ (0000)
00270020 0037:
	??_C@_0DH@PPCCMLMB@help?5message?5using?5old?5code?4?5get@ (0000)
00270058 0014:
	??_C@_1BE@ELICABCK@?$AA?$DM?$AAu?$AAn?$AAk?$AAn?$AAo?$AAw?$AAn?$AA?$DO?$AA?$AA@ (0000)
0027006C 0037:
	??_C@_0DH@GJFOKBEC@global_scenario_get?$CI?$CJ?9?$DOcustom_ob@ (0000)
002700A4 0028:
	??_C@_0CI@PMMKILCE@custom_index?$DMNUMBER_OF_HUD_CUSTO@ (0000)
002700CC 0013:
	??_C@_0BD@LPFKFODL@show_state_message?$AA@ (0000)
002700E0 0054:
	??_C@_0FE@EGLPDJAB@hud_messaging_globals?9?$DOobjective@ (0000)
002E4C4C 001c:
	_button_mappings (0000)
	_time_code_time (0010)
	_time_code_stop_time (0014)
00453AB8 0004:
	_hud_messaging_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"

#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "cache/texture_cache.h"
#include "cutscene/cinematics.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "input/input_abstraction.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_messaging.h"
#include "interface/interface.h"
#include "interface/ui_widget.h"
#include "items/item_definitions.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "render/render_debug.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"
#include "text/draw_string.h"
#include "text/font_group.h"
#include "text/text_group.h"
#include "text/unicode.h"

#include <stddef.h>

/* ---------- constants */

enum
{
	hud_message_text_group_tag = 'hmt ',
	hud_number_group_tag = 'hud#',
	NUMBER_OF_HUD_MESSAGING_DATUMS = 4,
	NUMBER_OF_HUD_MESSAGES_PER_DATUM = 4,
	MAXIMUM_HUD_STATE_MESSAGE_TEXT_LENGTH = 256
};

enum hud_corner
{
	_hud_corner_top_left,
	_hud_corner_top_right,
	_hud_corner_bottom_left,
	_hud_corner_bottom_right,
	_hud_corner_center,
	NUMBER_OF_HUD_CORNERS
};

enum hud_number_show_flags
{
	_hud_number_show_all_leading_zeros_bit,
	_hud_number_show_only_when_zoomed_bit,
	_hud_number_show_trailing_m_bit,
	NUMBER_OF_HUD_NUMBER_SHOW_FLAGS
};

enum hud_icon_flags
{
	_hud_icon_use_text_bit,
	_hud_icon_use_color_bit,
	_hud_icon_absolute_width_bit,
	NUMBER_OF_HUD_ICON_FLAGS
};

enum hud_flash_flags
{
	_hud_flash_reverse_colors_bit,
	NUMBER_OF_HUD_FLASH_FLAGS
};

enum text_justification
{
	_text_justification_left,
	_text_justification_right,
	_text_justification_center,
	NUMBER_OF_TEXT_JUSTIFICATIONS
};

enum hud_icon_type
{
	_icon_a_button,
	_icon_b_button,
	_icon_x_button,
	_icon_y_button,
	_icon_black_button,
	_icon_white_button,
	_icon_left_trigger,
	_icon_right_trigger,
	_icon_dpad_up,
	_icon_dpad_down,
	_icon_dpad_left,
	_icon_dpad_right,
	_icon_start_button,
	_icon_back_button,
	_icon_left_thumb,
	_icon_right_thumb,
	_icon_left_stick,
	_icon_right_stick,
	_icon_action,
	_icon_throw_grenade,
	_icon_primary_trigger,
	_icon_integrated_light,
	_icon_jump,
	_icon_use_equipment,
	_icon_rotate_weapons,
	_icon_rotate_grenades,
	_icon_crouch,
	_icon_zoom,
	_icon_accept,
	_icon_back,
	_icon_move,
	_icon_look,
	_icon_custom_1,
	_icon_custom_2,
	_icon_custom_3,
	_icon_custom_4,
	_icon_custom_5,
	_icon_custom_6,
	_icon_custom_7,
	_icon_custom_8,
	NUMBER_OF_ICON_TYPES
};

enum
{
	NUMBER_OF_HUD_CUSTOM_ICONS = _icon_custom_8 - _icon_custom_1 + 1
};

/* ---------- macros */

#define HUD_MESSAGE_TEXT_DEFINITION_GET(index) \
	((struct hud_message_text_definition *)tag_get(hud_message_text_group_tag, (index)))
#define HUD_NUMBER_DEFINITION_GET(index) \
	((struct hud_number_definition *)tag_get(hud_number_group_tag, (index)))

/* ---------- structures */

struct hud_message_definition
{
	long time;
	wchar_t text[63];
	boolean valid;
	boolean magic_number;
	long item_definition_index;
	short quantity;
	char message_offset;
	byte pad8B;
};

struct hud_state_message_definition
{
	char name[32];
	word text_start_index;
	word element_start_index;
	byte element_count;
	byte pad25[3];
	long unused28[6];
};

enum
{
	_hud_message_type_text,
	_hud_message_type_icon,
	NUMBER_OF_HUD_MESSAGE_TYPES
};

struct hud_state_message_element
{
	byte type;
	byte data;
};

struct icon_hud_element_definition
{
	short sequence_index;
	short width_offset;
	point2d offset;
	pixel32 color;
	char frame_rate;
	byte flags;
	short text_index;
};

struct hud_state_message_text_info_definition
{
	short string_index;
	boolean uses_scenario_names;
	byte reserved3;
};

union hud_state_message_info_definition
{
	struct icon_hud_element_definition const *icon;
	struct hud_state_message_text_info_definition text;
};

struct hud_state_message_runtime_definition
{
	wchar_t message_buffer[MAXIMUM_HUD_STATE_MESSAGE_TEXT_LENGTH];
	byte reserved200[4];
	union hud_state_message_info_definition info[8];
	struct hud_state_message_definition *state_message;
	boolean valid;
	byte is_text_flags;
	byte reserved22A[2];
};

struct hud_messaging_datum_definition
{
	struct hud_message_definition messages[NUMBER_OF_HUD_MESSAGES_PER_DATUM];
	struct hud_state_message_runtime_definition state_message;
	short first_message;
	boolean leave_first_line_blank;
	boolean custom_message;
};

struct hud_message_text_definition
{
	struct tag_data text_data;
	struct tag_block elements;
	struct tag_block messages;
	long unused2C[21];
};

struct hud_scripted_globals_definition
{
	boolean show_hud;
	boolean show_hud_help_text;
	byte reserved2[2];
};

struct hud_timer_data_definition
{
	long reference_time;
	short ticks;
	short flash_cutoff;
	point2d position;
	short corner;
	boolean paused;
	boolean enabled;
};

struct hud_objective_runtime_definition
{
	struct hud_state_message_definition *message;
	short uptime;
	short pad6;
};

struct hud_messaging_globals_definition
{
	struct hud_messaging_datum_definition message_data[NUMBER_OF_HUD_MESSAGING_DATUMS];
	long flash_start_time;
	boolean use_flash;
	byte magic_number;
	byte reserved1186[6];
	struct hud_state_message_definition *help_message;
	struct hud_objective_runtime_definition objective;
	struct hud_timer_data_definition timer;
};

struct number_hud_element_definition
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	char digits;
	byte number_flags;
	char fractional_digits;
	byte pad;
	long unused[3];
};

struct hud_number_definition
{
	struct tag_reference number_bitmap;
	char character_width;
	char screen_width;
	char x_offset;
	char y_offset;
	char decimal_point_width;
	char colon_width;
	short pad;
	long unused[19];
};

typedef char hud_timer_data_size_assert[
	sizeof(struct hud_timer_data_definition) == 0x10 ? 1 : -1];
typedef char hud_message_valid_offset_assert[
	offsetof(struct hud_message_definition, valid) == 0x82 ? 1 : -1];
typedef char hud_message_size_assert[
	sizeof(struct hud_message_definition) == 0x8C ? 1 : -1];
typedef char hud_state_message_text_info_size_assert[
	sizeof(struct hud_state_message_text_info_definition) == 4 ? 1 : -1];
typedef char hud_state_message_info_size_assert[
	sizeof(union hud_state_message_info_definition) == 4 ? 1 : -1];
typedef char hud_state_message_runtime_size_assert[
	sizeof(struct hud_state_message_runtime_definition) == 0x22C ? 1 : -1];
typedef char hud_state_message_runtime_info_offset_assert[
	offsetof(struct hud_state_message_runtime_definition, info) == 0x204 ? 1 : -1];
typedef char hud_state_message_runtime_state_message_offset_assert[
	offsetof(struct hud_state_message_runtime_definition, state_message) == 0x224 ? 1 : -1];
typedef char hud_state_message_runtime_valid_offset_assert[
	offsetof(struct hud_state_message_runtime_definition, valid) == 0x228 ? 1 : -1];
typedef char hud_state_message_runtime_is_text_flags_offset_assert[
	offsetof(struct hud_state_message_runtime_definition, is_text_flags) == 0x229 ? 1 : -1];
typedef char hud_messaging_datum_state_message_offset_assert[
	offsetof(struct hud_messaging_datum_definition, state_message) == 0x230 ? 1 : -1];
typedef char hud_messaging_datum_leave_first_line_blank_offset_assert[
	offsetof(struct hud_messaging_datum_definition, leave_first_line_blank) == 0x45E ? 1 : -1];
typedef char hud_messaging_datum_custom_message_offset_assert[
	offsetof(struct hud_messaging_datum_definition, custom_message) == 0x45F ? 1 : -1];
typedef char hud_messaging_datum_size_assert[
	sizeof(struct hud_messaging_datum_definition) == 0x460 ? 1 : -1];
typedef char hud_messaging_globals_size_assert[
	sizeof(struct hud_messaging_globals_definition) == 0x11A8 ? 1 : -1];
typedef char hud_messaging_flash_start_time_offset_assert[
	offsetof(struct hud_messaging_globals_definition, flash_start_time) == 0x1180 ? 1 : -1];
typedef char hud_messaging_use_flash_offset_assert[
	offsetof(struct hud_messaging_globals_definition, use_flash) == 0x1184 ? 1 : -1];
typedef char hud_messaging_magic_number_offset_assert[
	offsetof(struct hud_messaging_globals_definition, magic_number) == 0x1185 ? 1 : -1];
typedef char hud_messaging_help_message_offset_assert[
	offsetof(struct hud_messaging_globals_definition, help_message) == 0x118C ? 1 : -1];
typedef char hud_messaging_timer_offset_assert[
	offsetof(struct hud_messaging_globals_definition, timer) == 0x1198 ? 1 : -1];
typedef char hud_messaging_timer_flash_cutoff_offset_assert[
	offsetof(struct hud_messaging_globals_definition, timer.flash_cutoff) == 0x119E ? 1 : -1];
typedef char hud_messaging_timer_enabled_offset_assert[
	offsetof(struct hud_messaging_globals_definition, timer.enabled) == 0x11A7 ? 1 : -1];
typedef char hud_messaging_parameters_size_assert[
	sizeof(struct hud_messaging_parameters_definition) == 0x120 ? 1 : -1];
typedef char hud_messaging_single_player_font_index_offset_assert[
	offsetof(struct hud_messaging_parameters_definition, single_player_font.index) == 0x54 ? 1 : -1];
typedef char hud_messaging_multi_player_font_index_offset_assert[
	offsetof(struct hud_messaging_parameters_definition, multi_player_font.index) == 0x64 ? 1 : -1];
typedef char hud_messaging_state_color_offset_assert[
	offsetof(struct hud_messaging_parameters_definition, state_color) == 0x70 ? 1 : -1];
typedef char hud_globals_messaging_offset_assert[
	offsetof(struct hud_globals_definition, messaging) == 0 ? 1 : -1];
typedef char hud_absolute_placement_size_assert[
	sizeof(struct hud_absolute_placement_definition) == 0x24 ? 1 : -1];
typedef char hud_placement_size_assert[
	sizeof(struct hud_placement_definition) == 0x24 ? 1 : -1];
typedef char number_hud_element_size_assert[
	sizeof(struct number_hud_element_definition) == 0x54 ? 1 : -1];
typedef char hud_number_size_assert[
	sizeof(struct hud_number_definition) == 0x64 ? 1 : -1];
typedef char hud_globals_timer_definition_offset_assert[
	offsetof(struct hud_globals_definition, timer_definition) == 0x360 ? 1 : -1];

/* ---------- prototypes */

static struct hud_message_definition *find_free_hud_message(
	struct hud_messaging_datum_definition *datum,
	long item_definition_index,
	char message_offset);
static void render_state_text(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	wchar_t const *text,
	boolean custom);
static void render_state_bitmap(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	pixel32 color,
	struct icon_hud_element_definition const *icon);

/* ---------- globals */

static struct hud_messaging_globals_definition *hud_messaging_globals;
extern struct hud_messaging_parameters_definition *hud_msg_def;
extern struct hud_scripted_globals_definition *hud_scripted_globals;
static char button_mappings[_icon_custom_1 - _icon_action] =
{
	2,
	6,
	7,
	5,
	0,
	4,
	3,
	1,
	11,
	10,
	_icon_a_button,
	_icon_b_button,
	_icon_left_stick,
	_icon_right_stick
};
long time_code_time = NONE;
long time_code_stop_time = NONE;
static long split_screen_hud_message_offset = 17;

/* ---------- public code */

static void render_state_text(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	wchar_t const *text,
	boolean custom)
{
	rectangle2d text_bounds;

	draw_string_set_indents(cursor_bounds->x0 - bounds->x0, 0);
	draw_unicode_string_compute_bounds(bounds, text, &text_bounds, cursor_bounds);
	cursor_bounds->x0 -= 3;
	text_bounds.x0 = bounds->x0;
	if (custom && game_engine_running())
		draw_string_and_hack_in_icons(&text_bounds, NULL, NULL, 0, text, TRUE);
	else
		rasterizer_draw_unicode_string(&text_bounds, NULL, NULL, 0, text);
	bounds->y0 = cursor_bounds->y0;

	return;
}

static void render_state_bitmap(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	pixel32 color,
	struct icon_hud_element_definition const *icon)
{
	long bitmap_group_index = hud_globals->messaging.messaging_icons.index;
	long frame_index = 0;
	struct bitmap_data const *bitmap = NULL;
	real_rectangle2d const *clip = NULL;
	real scale;
	point2d point;

	if (icon->frame_rate)
		frame_index = game_time_get() / icon->frame_rate;
	hud_retrieve_bitmap_and_bounding_rect(
		bitmap_group_index,
		icon->sequence_index,
		frame_index,
		&bitmap,
		&clip);
	if (bitmap && _texture_cache_bitmap_get_hardware_format(
		(struct bitmap_data *)bitmap, FALSE, TRUE))
	{
		scale = local_player_count() > 1 ? 0.75f : 1.0f;
		point.x = (short)(icon->offset.x * scale + cursor_bounds->x0);
		point.y = (short)(cursor_bounds->y1 - icon->offset.y * scale);
		hud_draw_bitmap_direct(
			bitmap,
			_hud_corner_bottom_left,
			&point,
			clip,
			scale,
			0.0f,
			TEST_FLAG(icon->flags, _hud_icon_use_color_bit) ? icon->color : color,
			FALSE);
		if (TEST_FLAG(icon->flags, _hud_icon_absolute_width_bit))
			cursor_bounds->x0 = (short)(icon->width_offset * scale + point.x);
		else if (clip)
		{
			cursor_bounds->x0 = (short)(
				((clip->x1 - clip->x0) * bitmap->width + icon->width_offset) * scale + point.x);
		}
		else
		{
			cursor_bounds->x0 = (short)(
				(bitmap->width + icon->width_offset) * scale + point.x);
		}
	}

	return;
}

void hud_messaging_initialize(
	void)
{
	hud_messaging_globals = game_state_malloc(
		"hud messaging",
		NULL,
		sizeof(*hud_messaging_globals));
	return;
}

void hud_messaging_initialize_for_new_map(
	void)
{
	hud_msg_def = &hud_globals->messaging;
	csmemset(hud_messaging_globals, 0, sizeof(*hud_messaging_globals));
	return;
}

void hud_messaging_dispose_from_old_map(
	void)
{
	return;
}

void hud_messaging_dispose(
	void)
{
	return;
}

void scripted_hud_set_state_message(
	short message_index)
{
	struct scenario *scenario = global_scenario_get();

	if (hud_scripted_globals->show_hud_help_text &&
		scenario->hud_messages.index != NONE)
	{
		struct hud_message_text_definition *hud_messages = HUD_MESSAGE_TEXT_DEFINITION_GET(
			scenario->hud_messages.index);

		hud_messaging_globals->help_message = TAG_BLOCK_GET_ELEMENT(
			&hud_messages->messages,
			message_index,
			struct hud_state_message_definition);
	}

	return;
}

void scripted_hud_set_flashing_state(
	boolean flash)
{
	long time;

	if (flash && !hud_messaging_globals->use_flash)
	{
		time = game_time_get();
		hud_messaging_globals->flash_start_time = time;
		hud_messaging_globals->use_flash = flash;
		return;
	}
	hud_messaging_globals->use_flash = flash;

	return;
}

void scripted_hud_restart_flashing(
	void)
{
	if (hud_messaging_globals->use_flash)
		hud_messaging_globals->flash_start_time = game_time_get();
	else
		error(
			_error_silent,
			"trying to restart help text flashing when flashing is disabled");

	return;
}

void scripted_hud_set_objective(
	short message_index)
{
	struct scenario *scenario = global_scenario_get();

	if (scenario->hud_messages.index != NONE)
	{
		struct hud_message_text_definition *hud_messages =
			HUD_MESSAGE_TEXT_DEFINITION_GET(scenario->hud_messages.index);
		struct hud_state_message_definition *message = TAG_BLOCK_GET_ELEMENT(
			&hud_messages->messages,
			message_index,
			struct hud_state_message_definition);
		struct hud_state_message_element *element = TAG_BLOCK_GET_ELEMENT(
			&hud_messages->elements,
			message->element_start_index,
			struct hud_state_message_element);

		if (message->element_count == 1 && element->type == _hud_message_type_text)
		{
			struct hud_color_definition *objective_color = &hud_globals->messaging.objective_color;

			hud_messaging_globals->objective.message = message;
			hud_messaging_globals->objective.uptime =
				objective_color->custom.objective.up_ticks +
				objective_color->custom.objective.fade_ticks;
		}
		else
		{
			error(
				_error_silent,
				"objective text MUST only be text, no icons");
		}
	}

	return;
}

void scripted_hud_set_timer_time(
	short minutes,
	word seconds)
{
	struct hud_timer_data_definition *timer;
	long time;

	hud_messaging_globals->timer.ticks = 30 * (60 * minutes + seconds);
	timer = &hud_messaging_globals->timer;
	timer->paused = FALSE;
	timer->enabled = TRUE;
	time = game_time_get();
	timer = &hud_messaging_globals->timer;
	timer->reference_time = time;
	timer->corner = PIN(timer->corner, 0, 4);

	return;
}

void scripted_hud_set_timer_warning_cutoff(
	short minutes,
	word seconds)
{
	hud_messaging_globals->timer.flash_cutoff = 30 * (60 * minutes + seconds);
	return;
}

void scripted_hud_set_timer_position(
	short x,
	short y,
	short corner)
{
	struct hud_messaging_globals_definition *globals = hud_messaging_globals;

	globals->timer.position.n[0] = x;
	globals->timer.position.n[1] = y;
	globals->timer.corner = PIN(corner, 0, 4);

	return;
}

void scripted_hud_show_timer(
	boolean show)
{
	hud_messaging_globals->timer.enabled = show;
	return;
}

void scripted_hud_pause_timer(
	boolean paused)
{
	struct hud_timer_data_definition *timer = &hud_messaging_globals->timer;
	short now;

	timer->paused = paused;
	if (timer->ticks > 0)
	{
		if (paused)
		{
			now = (short)game_time_get();
			timer->ticks += timer->reference_time - now;
			return;
		}

		now = (short)game_time_get();
		timer->ticks += now - timer->reference_time;
	}

	return;
}

short scripted_hud_get_timer_ticks(
	void)
{
	struct hud_timer_data_definition *timer = &hud_messaging_globals->timer;
	short ticks = 0;

	if (timer->enabled)
	{
		if (timer->ticks == NONE)
		{
			ticks = NONE;
		}
		else if (timer->paused)
		{
			ticks = timer->ticks;
		}
		else
		{
			ticks = (short)(timer->ticks + timer->reference_time - game_time_get());
		}
	}

	return ticks;
}

void scripted_hud_time_code_show(
	boolean show)
{
	if (show)
	{
		time_code_time = game_time_get();
		time_code_stop_time = time_code_time;
		return;
	}
	time_code_time = NONE;

	return;
}

void scripted_hud_time_code_start(
	boolean start)
{
	if (start)
	{
		time_code_time += game_time_get() - time_code_stop_time;
		time_code_stop_time = NONE;
		return;
	}
	time_code_stop_time = game_time_get();

	return;
}

void scripted_hud_time_code_reset(
	void)
{
	long time = game_time_get();

	time_code_time = time;
	if (time_code_stop_time != NONE)
		time_code_stop_time = time;

	return;
}

void hud_render_timer(
	void)
{
	struct hud_timer_data_definition *timer = &hud_messaging_globals->timer;

	if (timer->enabled)
	{
		struct hud_absolute_placement_definition placement = { timer->corner };
		struct number_hud_element_definition numbers;
		long current_time = game_time_get();
		short flash = FALSE;
		short digit_advance = 0;
		short timer_ticks = scripted_hud_get_timer_ticks();
		long hud_number_index;

		numbers.digits = 2;
		numbers.fractional_digits = 4;
		numbers.number_flags = FLAG(_hud_number_show_all_leading_zeros_bit);
		numbers.placement.offset = timer->position;
		numbers.placement.scale.i = 1.0f;
		numbers.placement.scale.j = 1.0f;

		hud_number_index = interface_get_tag_index(_interface_hud_digits);
		if (hud_number_index != NONE)
		{
			struct hud_number_definition *hud_number =
				HUD_NUMBER_DEFINITION_GET(hud_number_index);

			digit_advance = (short)(long)((real)hud_number->screen_width * 2.0f);
		}

		switch (timer->corner)
		{
		case _hud_corner_top_left:
		case _hud_corner_bottom_left:
			break;

		case _hud_corner_top_right:
		case _hud_corner_bottom_right:
			numbers.placement.offset.x += 5 * digit_advance;
			digit_advance = -digit_advance;
			break;

		case _hud_corner_center:
			numbers.placement.offset.x -= 3 * digit_advance;
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
				0x1F8,
				!"unreachable");
			break;
		}

		if (timer_ticks > 0)
		{
			numbers.colors = hud_globals->timer_definition.color;
			{
				short flash_cutoff = timer->flash_cutoff;

				if (timer_ticks <= flash_cutoff)
				{
					flash = TRUE;
					if (timer->ticks > flash_cutoff)
					{
						timer->ticks = flash_cutoff;
						timer->reference_time =
							flash_cutoff - timer_ticks + current_time;
					}
				}
			}
		}
		else
		{
			long reference_time = timer->reference_time;

			timer->ticks = NONE;
			numbers.colors = hud_globals->timer_definition.time_up_color;
			flash = TRUE;
			if (reference_time == NONE)
				timer->reference_time = game_time_get();
		}

		{
			short clamped_ticks = MAX(timer_ticks, 0);
			short total_seconds = clamped_ticks / TICKS_PER_SECOND;
			double offset_advance;

			hud_draw_numbers(
				render.local_player_index,
				&placement,
				&numbers,
				total_seconds / SECONDS_PER_MINUTE,
				NONE,
				flash,
				timer->reference_time,
				2.0f);

			offset_advance = (double)digit_advance * 2.5;
			numbers.placement.offset.x = (short)(long)(
				(double)numbers.placement.offset.x + offset_advance);
			hud_draw_numbers(
				render.local_player_index,
				&placement,
				&numbers,
				total_seconds % SECONDS_PER_MINUTE,
				NONE,
				flash,
				timer->reference_time,
				2.0f);

			numbers.placement.offset.x = (short)(long)(
				(double)numbers.placement.offset.x + offset_advance);
			hud_draw_numbers(
				render.local_player_index,
				&placement,
				&numbers,
				100 * (clamped_ticks % TICKS_PER_SECOND) / TICKS_PER_SECOND,
				NONE,
				flash,
				timer->reference_time,
				2.0f);
		}
	}

	if (time_code_time != NONE)
	{
		long stop_time = time_code_stop_time;

		if (stop_time == NONE)
			stop_time = game_time_get();
		sprintf(temporary, "%d", stop_time - time_code_time);
		render_debug_string(TRUE, temporary);
	}

	return;
}

void hud_set_state_message(
	short local_player_index,
	short message_index)
{
	if (!hud_scripted_globals->show_hud_help_text)
	{
		long hud_messages_index = hud_globals->messaging.hud_messages.index;

		if (hud_messages_index != NONE)
		{
			struct hud_messaging_globals_definition *globals = hud_messaging_globals;
			struct hud_messaging_datum_definition *datum =
				&globals->message_data[local_player_index];

			if (message_index != NONE)
			{
				struct hud_message_text_definition *hud_messages =
					HUD_MESSAGE_TEXT_DEFINITION_GET(hud_messages_index);

				if (message_index < hud_messages->messages.count)
				{
					datum->state_message.state_message = TAG_BLOCK_GET_ELEMENT(
						&hud_messages->messages,
						message_index,
						struct hud_state_message_definition);
					datum->state_message.is_text_flags = 0;
				}
				else
				{
					message_index = NONE;
				}
			}

			datum->state_message.valid = message_index != NONE;
		}
	}

	return;
}

void
hud_set_state_message_icon(
	short local_player_index,
	short custom_icon_index,
	struct icon_hud_element_definition const *icon)
{
	struct hud_messaging_datum_definition *datum =
		&hud_messaging_globals->message_data[local_player_index];

	if (datum->state_message.valid &&
		!hud_scripted_globals->show_hud_help_text &&
		datum->state_message.state_message)
	{
		datum->state_message.info[custom_icon_index].icon = icon;
		datum->state_message.is_text_flags &= ~(1 << custom_icon_index);
	}

	return;
}

void
hud_set_state_message_text(
	short local_player_index,
	short custom_icon_index,
	short icon_string_index,
	boolean uses_scenario_names)
{
	struct hud_messaging_datum_definition *datum =
		&hud_messaging_globals->message_data[local_player_index];

	if (datum->state_message.valid &&
		!hud_scripted_globals->show_hud_help_text &&
		datum->state_message.state_message)
	{
		datum->state_message.info[custom_icon_index].text.string_index = icon_string_index;
		datum->state_message.info[custom_icon_index].text.uses_scenario_names = uses_scenario_names;
		datum->state_message.is_text_flags |= 1 << custom_icon_index;
	}

	return;
}

void
hud_enable_custom_state_message(
	short local_player_index,
	boolean enabled)
{
	struct hud_messaging_datum_definition *datum =
		&hud_messaging_globals->message_data[local_player_index];

	datum->leave_first_line_blank |= datum->state_message.valid != enabled;
	datum->state_message.valid = enabled;
	datum->state_message.state_message = NULL;
	if (enabled)
	{
		datum->state_message.state_message = NULL;
		ustrncpy(
			datum->state_message.message_buffer,
			L"",
			MAXIMUM_HUD_STATE_MESSAGE_TEXT_LENGTH - 1);
	}
	datum->custom_message = enabled;

	return;
}

void hud_set_state_text(
	short local_player_index,
	wchar_t const *message)
{
	struct hud_messaging_datum_definition *datum =
		&hud_messaging_globals->message_data[local_player_index];

	ustrncpy(
		datum->state_message.message_buffer,
		message,
		MAXIMUM_HUD_STATE_MESSAGE_TEXT_LENGTH - 1);
	datum->state_message.message_buffer[MAXIMUM_HUD_STATE_MESSAGE_TEXT_LENGTH - 1] = L'\0';

	return;
}

wchar_t *hud_messaging_get_objective(
	void)
{
	wchar_t *result = NULL;

	if (hud_messaging_globals->objective.message)
	{
		struct scenario *scenario = global_scenario_get();
		struct hud_message_text_definition *hud_messages =
			HUD_MESSAGE_TEXT_DEFINITION_GET(scenario->hud_messages.index);
		struct hud_state_message_definition *message =
			hud_messaging_globals->objective.message;
		struct hud_state_message_element *element = TAG_BLOCK_GET_ELEMENT(
			&hud_messages->elements,
			message->element_start_index,
			struct hud_state_message_element);

		match_vassert(
			"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
			0x2A2,
			message->element_count == 1,
			"message->element_count==1");
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
			0x2A3,
			element->type == _hud_message_type_text,
			"element->type==_hud_message_type_text");

		result = tag_data_get_pointer(
			&hud_messages->text_data,
			message->text_start_index * sizeof(wchar_t),
			element->data * sizeof(wchar_t));
	}

	return result;
}

void scripted_hud_messages_clear(
	void)
{
	struct hud_messaging_datum_definition *datum = hud_messaging_globals->message_data;
	long datum_count = NUMBER_OF_HUD_MESSAGING_DATUMS;

	do
	{
		struct hud_message_definition *message = datum->messages;
		long message_count = NUMBER_OF_HUD_MESSAGES_PER_DATUM;

		do
		{
			message->valid = FALSE;
			message++;
		}
		while (--message_count);

		datum++;
	}
	while (--datum_count);

	return;
}

long hud_get_font_index(
	void)
{
	if (local_player_count() > 1 &&
		hud_msg_def->multi_player_font.index != NONE)
	{
		return hud_msg_def->multi_player_font.index;
	}

	return hud_msg_def->single_player_font.index;
}

real_argb_color *hud_get_text_color(
	real_argb_color *result)
{
	*result = hud_msg_def->state_color;

	return result;
}

void hud_messaging_globals_update(
	void)
{
	hud_messaging_globals->magic_number = 0;
	return;
}

void hud_print_message(
	short local_player_index,
	wchar_t const *message_text)
{
	if (local_player_index != NONE)
	{
		struct hud_messaging_datum_definition *datum =
			&hud_messaging_globals->message_data[local_player_index];
		struct hud_message_definition *message = find_free_hud_message(
			datum,
			NONE,
			0);

		ustrncpy(message->text, message_text, NUMBEROF(message->text));
		message->item_definition_index = NONE;
		message->time = game_time_get();
		message->valid = TRUE;
		message->magic_number = hud_messaging_globals->magic_number++;
		datum->leave_first_line_blank = FALSE;
	}

	return;
}

void hud_add_item_message(
	short local_player_index,
	long item_definition_index,
	short quantity,
	char message_offset)
{
	if (local_player_index != NONE)
	{
		struct hud_messaging_datum_definition *datum =
			&hud_messaging_globals->message_data[local_player_index];
		struct hud_message_definition *message = find_free_hud_message(
			datum,
			item_definition_index,
			message_offset);

		if (!message->valid)
			message->quantity = 0;
		message->quantity += quantity;
		message->item_definition_index = item_definition_index;
		message->message_offset = message_offset;
		message->time = game_time_get();
		message->valid = TRUE;
		message->magic_number = hud_messaging_globals->magic_number++;
		datum->leave_first_line_blank = FALSE;
	}

	return;
}

void hud_broadcast_team_message(
	long victim_player_index,
	wchar_t const *message)
{
	if (game_engine_running())
	{
		short local_player_index;

		for (local_player_index = 0;
			local_player_index < MAXIMUM_LOCAL_PLAYERS;
			local_player_index++)
		{
			long player_index = local_player_get_player_index(local_player_index);

			if (player_index != NONE &&
				player_get(player_index)->team_index == player_get(victim_player_index)->team_index)
			{
				hud_print_message(local_player_index, message);
			}
		}
	}

	return;
}

/* ---------- private code */

static struct hud_message_definition *find_free_hud_message(
	struct hud_messaging_datum_definition *datum,
	long item_definition_index,
	char message_offset)
{
	struct hud_message_definition *result = NULL;
	long oldest_time = LONG_MAX;
	short oldest_message_index = 0;
	short message_index = 0;

	do
	{
		struct hud_message_definition *message = &datum->messages[message_index];

		if ((item_definition_index != NONE &&
			item_definition_index == message->item_definition_index &&
			message_offset == message->message_offset) || !message->valid)
		{
			result = message;
			if (item_definition_index == NONE ||
				item_definition_index == message->item_definition_index)
			{
				break;
			}
		}
		else if (oldest_time > message->time)
		{
			oldest_time = message->time;
			oldest_message_index = message_index;
		}

		message_index++;
	}
	while ((word)message_index < NUMBER_OF_HUD_MESSAGES_PER_DATUM);

	if (!result)
		result = &datum->messages[oldest_message_index];

	return result;
}

static long compare_messages(
	struct hud_message_definition const *message_a,
	struct hud_message_definition const *message_b)
{
	long difference = message_b->time - message_a->time;

	if (!difference)
	{
		difference = message_b->item_definition_index - message_a->item_definition_index;
		if (!difference)
			difference = message_b->magic_number - message_a->magic_number;
	}

	return difference;
}

void hud_messaging_update(
	short local_player_index)
{
	if (!cinematic_in_progress() &&
		local_player_index != NONE &&
		game_engine_hud_draw_messages(local_player_get_player_index(local_player_index)))
	{
		long font_index = hud_get_font_index();
		boolean split_screen = local_player_count() > 1;
		point2d screen_point;
		struct font_header *font;
		short line_top;
		short line_height;
		short first_line_height;
		struct hud_messaging_datum_definition *datum;
		short maximum_message_count;
		boolean objective_active;
		boolean help_active;
		boolean state_active;
		short message_index;

		hud_calculate_point(
			local_player_index,
			&hud_msg_def->absolute_placement,
			&hud_msg_def->placement,
			NULL,
			local_player_count() > 1,
			0.0f,
			&screen_point);
		line_top = screen_point.y;
		font = font_definition_get(font_index);
		if (split_screen)
		{
			line_height = font->leading_height + font->ascending_height;
			line_top -= split_screen_hud_message_offset;
		}
		else
		{
			line_height = font->leading_height +
				font->descending_height +
				font->ascending_height;
		}
		first_line_height = line_height;
		datum = &hud_messaging_globals->message_data[render.local_player_index];
		maximum_message_count = 4 - (local_player_count() > 1);
		objective_active = hud_messaging_globals->objective.message &&
			hud_messaging_globals->objective.uptime;
		help_active = hud_scripted_globals->show_hud_help_text &&
			hud_messaging_globals->help_message;
		state_active = datum->state_message.valid &&
			(datum->state_message.state_message || datum->state_message.message_buffer[0]);

		if (objective_active || help_active || state_active)
		{
			struct hud_state_message_runtime_definition *state_message = &datum->state_message;
			struct game_input_preferences preferences;
			real_argb_color state_color;
			pixel32 pixel_color;
			rectangle2d line_bounds;
			rectangle2d line_cursor;

			input_abstraction_get_local_player_preferences(
				local_player_index,
				&preferences);
			if (objective_active)
			{
				struct hud_color_definition *objective_color =
					&hud_globals->messaging.objective_color;
				long objective_start_time = game_time_get() +
					hud_messaging_globals->objective.uptime -
					objective_color->custom.objective.up_ticks -
					objective_color->custom.objective.fade_ticks;

				pixel32_to_real_argb_color(
					get_flash_color(objective_color, objective_start_time),
					&state_color);
				state_color.alpha *= MIN(
					(real)hud_messaging_globals->objective.uptime /
						objective_color->custom.objective.fade_ticks,
					1.0f);
				pixel_color = real_argb_color_to_pixel32(&state_color);
			}
			else if (help_active)
			{
				if (hud_messaging_globals->use_flash)
				{
					pixel_color = get_flash_color(
						&hud_globals->messaging.color,
						hud_messaging_globals->flash_start_time);
				}
				else if (!TEST_FLAG(
					hud_globals->messaging.color.flash_flags,
					_hud_flash_reverse_colors_bit))
				{
					pixel_color = hud_globals->messaging.color.color;
				}
				else
				{
					pixel_color = hud_globals->messaging.color.flash_color;
				}
				pixel32_to_real_argb_color(pixel_color, &state_color);
			}
			else
			{
				state_color = hud_msg_def->state_color;
				pixel_color = real_argb_color_to_pixel32(&state_color);
			}

			line_bounds.x0 = screen_point.x;
			line_bounds.x1 = render.camera.window_bounds.x1 -
				render.camera.viewport_bounds.x0;
			line_bounds.y0 = line_top;
			line_bounds.y1 = line_top + 5 * line_height;
			line_cursor = line_bounds;
			draw_string_set_draw_mode(
				font_index,
				NONE,
				_text_justification_left,
				0,
				&state_color);

			if (!objective_active && !help_active &&
				!datum->state_message.state_message)
			{
				if (datum->state_message.message_buffer[0])
				{
					render_state_text(
						&line_bounds,
						&line_cursor,
						datum->state_message.message_buffer,
						TRUE);
				}
			}
			else
			{
				struct hud_message_text_definition *hud_messages;
				struct hud_state_message_definition *message;
				word text_position;
				short element_index;

				if (objective_active)
				{
					struct scenario *scenario;

					match_vassert(
						"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
						0x420,
						hud_messaging_globals->objective.message &&
							hud_messaging_globals->objective.uptime,
						"hud_messaging_globals->objective.message && hud_messaging_globals->objective.uptime");
					hud_messaging_globals->objective.uptime = MAX(
						hud_messaging_globals->objective.uptime - game_time_get_elapsed(),
						0);
					scenario = global_scenario_get();
					hud_messages = HUD_MESSAGE_TEXT_DEFINITION_GET(
						scenario->hud_messages.index);
					message = hud_messaging_globals->objective.message;
				}
				else if (help_active)
				{
					struct scenario *scenario = global_scenario_get();

					hud_messages = HUD_MESSAGE_TEXT_DEFINITION_GET(
						scenario->hud_messages.index);
					message = hud_messaging_globals->help_message;
				}
				else
				{
					match_vassert(
						"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
						0x42C,
						state_active,
						"show_state_message");
					hud_messages = HUD_MESSAGE_TEXT_DEFINITION_GET(
						hud_globals->messaging.hud_messages.index);
					message = datum->state_message.state_message;
				}

				text_position = message->text_start_index;
				for (element_index = 0;
					element_index < message->element_count;
					element_index++)
				{
					struct hud_state_message_element *element = TAG_BLOCK_GET_ELEMENT(
						&hud_messages->elements,
						message->element_start_index + element_index,
						struct hud_state_message_element);

					switch (element->type)
					{
					case _hud_message_type_text:
						{
							wchar_t const *text = tag_data_get_pointer(
								&hud_messages->text_data,
								text_position * sizeof(wchar_t),
								element->data * sizeof(wchar_t));
							rectangle2d text_bounds;

							draw_string_set_indents(line_cursor.x0 - line_bounds.x0, 0);
							draw_unicode_string_compute_bounds(
								&line_bounds,
								text,
								&text_bounds,
								&line_cursor);
							line_cursor.x0 -= 3;
							text_bounds.x0 = line_bounds.x0;
							rasterizer_draw_unicode_string(
								&text_bounds,
								NULL,
								NULL,
								0,
								text);
							line_bounds.y0 = line_cursor.y0;
							text_position += element->data;
						}
						break;

					case _hud_message_type_icon:
						{
							short icon_index = NONE;

							if (element->data > _icon_right_stick)
							{
								if (element->data <= _icon_look)
								{
									if (element->data <= _icon_accept)
									{
										icon_index = preferences.game_control_to_xbox_buttons[
											button_mappings[element->data - _icon_action]];
									}
									else
									{
										icon_index = button_mappings[element->data - _icon_action];
									}
								}
								else if (!hud_scripted_globals->show_hud_help_text)
								{
									short custom_index = element->data - _icon_custom_1;

									match_vassert(
										"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
										0x457,
										custom_index < NUMBER_OF_HUD_CUSTOM_ICONS,
										"custom_index<NUMBER_OF_HUD_CUSTOM_ICONS");
									if (TEST_FLAG(state_message->is_text_flags, custom_index))
									{
										short string_index = state_message->info[custom_index].text.string_index;

										if (string_index != NONE)
										{
											if (state_message->info[custom_index].text.uses_scenario_names)
											{
												match_vassert(
													"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
													0x460,
													global_scenario_get()->custom_object_names.index != NONE,
													"global_scenario_get()->custom_object_names.index!=NONE");
												render_state_text(
													&line_bounds,
													&line_cursor,
													unicode_string_list_get_string(
														global_scenario_get()->custom_object_names.index,
														string_index),
													FALSE);
											}
											else
											{
												render_state_text(
													&line_bounds,
													&line_cursor,
													unicode_string_list_get_string(
														hud_globals->messaging.alternate_icon_text.index,
														string_index),
													FALSE);
											}
										}
										else
										{
											render_state_text(
												&line_bounds,
												&line_cursor,
												L"<unknown>",
												FALSE);
										}
									}
									else
									{
										struct icon_hud_element_definition const *icon =
											state_message->info[custom_index].icon;

										if (icon)
										{
											render_state_bitmap(
												&line_bounds,
												&line_cursor,
												pixel_color,
												icon);
										}
										else
										{
											error(
												_error_silent,
												"help message using old code. get latest code and tags.");
										}
									}
									break;
								}
								else
								{
									error(_error_silent, "help text cannot use custom icons");
								}
							}
							else
							{
								icon_index = element->data;
							}

							if (icon_index < hud_globals->messaging.button_icons.count)
							{
								struct icon_hud_element_definition const *icon;

								icon_index = remap_sticks_for_local_player(
									icon_index,
									local_player_index);
								icon = TAG_BLOCK_GET_ELEMENT(
									&hud_globals->messaging.button_icons,
									icon_index,
									struct icon_hud_element_definition);
								if (TEST_FLAG(icon->flags, _hud_icon_use_text_bit))
								{
									real_argb_color text_color;
									wchar_t const *icon_text;
									rectangle2d text_bounds;

									if (TEST_FLAG(icon->flags, _hud_icon_use_color_bit))
									{
										pixel32_to_real_argb_color(pixel_color, &text_color);
										draw_string_set_draw_mode(
											font_index,
											NONE,
											_text_justification_left,
											0,
											&text_color);
									}
									icon_text = unicode_string_list_get_string(
										hud_globals->messaging.alternate_icon_text.index,
										icon->text_index);
									draw_string_set_indents(line_cursor.x0 - line_bounds.x0, 0);
									draw_unicode_string_compute_bounds(
										&line_bounds,
										icon_text,
										&text_bounds,
										&line_cursor);
									line_cursor.x0 -= 3;
									text_bounds.x0 = line_bounds.x0;
									rasterizer_draw_unicode_string(
										&text_bounds,
										NULL,
										NULL,
										0,
										icon_text);
									line_bounds.y0 = line_cursor.y0;
									draw_string_set_draw_mode(
										font_index,
										NONE,
										_text_justification_left,
										0,
										&state_color);
								}
								else
								{
									render_state_bitmap(
										&line_bounds,
										&line_cursor,
										pixel_color,
										icon);
								}
							}
							else
							{
								rectangle2d text_bounds;

								draw_string_set_indents(line_cursor.x0 - line_bounds.x0, 0);
								draw_unicode_string_compute_bounds(
									&line_bounds,
									L"<no button icon>",
									&text_bounds,
									&line_cursor);
								line_cursor.x0 -= 3;
								text_bounds.x0 = line_bounds.x0;
								rasterizer_draw_unicode_string(
									&text_bounds,
									NULL,
									NULL,
									0,
									L"<no button icon>");
								line_bounds.y0 = line_cursor.y0;
							}
						}
						break;

					default:
						match_assert(
							"c:\\halo\\SOURCE\\interface\\hud_messaging.c",
							0x4A7,
							!"unreachable");
						break;
					}
				}
			}

			draw_string_set_indents(0, 0);
			line_height = first_line_height;
			line_top = line_cursor.y1;
		}

		if (!objective_active && !help_active &&
			(datum->state_message.valid || datum->leave_first_line_blank))
		{
			if (split_screen)
			{
				line_top = (short)(
					screen_point.y - split_screen_hud_message_offset +
					hud_msg_def->spacing * line_height);
			}
			else
			{
				line_top = (short)(
					screen_point.y + hud_msg_def->spacing * line_height);
			}
			maximum_message_count--;
		}

		qsort(
			datum->messages,
			NUMBEROF(datum->messages),
			sizeof(datum->messages[0]),
			(int (__cdecl *)(void const *, void const *))compare_messages);
		for (message_index = 0;
			message_index < maximum_message_count;
			message_index++)
		{
			struct hud_message_definition *message = &datum->messages[message_index];
			long now;
			long elapsed;
			real_argb_color text_color;
			rectangle2d message_bounds;

			if (!message->valid)
				break;
			now = game_time_get();
			text_color = hud_msg_def->text_color;
			elapsed = now - message->time;
			if ((real)elapsed > hud_msg_def->up_time * TICKS_PER_SECOND)
			{
				real fade = PIN(
					1.0f -
						((real)elapsed - hud_msg_def->up_time * TICKS_PER_SECOND) /
						(hud_msg_def->fade_time * TICKS_PER_SECOND),
					0.0f,
					1.0f);

				text_color.alpha *= (real)pow((double)fade, 1.9f);
			}

			message_bounds = render.camera.window_bounds;
			message_bounds.x0 = screen_point.x;
			message_bounds.x1 = render.camera.window_bounds.x1 -
				render.camera.viewport_bounds.x0;
			message_bounds.y0 = line_top;
			message_bounds.y1 = line_top + line_height;
			line_top = (short)(hud_msg_def->spacing * line_height + line_top);
			draw_string_set_draw_mode(
				font_index,
				NONE,
				_text_justification_left,
				0,
				&text_color);

			if (message->item_definition_index != NONE)
			{
				char message_offset;
				struct item_definition *item;
				short string_index;
				wchar_t const *item_text;

				if (message->message_offset == NONE)
					message_offset = message->quantity > 1;
				else
					message_offset = message->message_offset;
				item = item_definition_get(message->item_definition_index);
				string_index = item->item.hud_message_index + message_offset;
				item_text = hud_get_item_string(string_index);
				if ((message->message_offset == NONE && message_offset) ||
					message->quantity)
				{
					wchar_t formatted[256];
					short value_scale = MAX(item->item.hud_message_value_scale, 1);

					usprintf(
						formatted,
						item_text,
						message->quantity / value_scale);
					rasterizer_draw_unicode_string(
						&message_bounds,
						NULL,
						NULL,
						0,
						formatted);
				}
				else
				{
					rasterizer_draw_unicode_string(
						&message_bounds,
						NULL,
						NULL,
						0,
						item_text);
				}
			}
			else
			{
				rasterizer_draw_unicode_string(
					&message_bounds,
					NULL,
					NULL,
					0,
					message->text);
			}
			message->valid = (real)(now - message->time) <
				(hud_msg_def->fade_time + hud_msg_def->up_time) * TICKS_PER_SECOND;
			if (!message->valid)
				message->time = NONE;
		}
	}

	return;
}
