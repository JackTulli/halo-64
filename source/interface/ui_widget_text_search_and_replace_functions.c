/*
UI_WIDGET_TEXT_SEARCH_AND_REPLACE_FUNCTIONS.C

symbols in this file:
000E4A40 0050:
	_ui_widget_search_and_replace_invoke (0000)
000E4A90 0010:
	_code_000e4a90 (0000)
000E4AA0 00b0:
	_code_000e4aa0 (0000)
00278C60 0014:
	??_C@_1BE@PJJJEMAC@?$AA?$DM?$AAi?$AAn?$AAv?$AAa?$AAl?$AAi?$AAd?$AA?$DO?$AA?$AA@ (0000)
00278C78 0047:
	??_C@_0EH@LDHNIENI@c?3?2halo?2SOURCE?2interface?2ui_widg@ (0000)
0030632C 0008:
	_data_0030632c (0000)
00454D08 0004:
	_bss_00454d08 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "ui_widget_text_search_and_replace_functions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* Fields shared by every widget instance and evidenced by ui_widget.c. */
struct widget_instance_prefix
{
	long definition_tag_index;
	char const *name;
	short local_player_index;
};

typedef char widget_instance_local_player_index_offset[
	offsetof(struct widget_instance_prefix, local_player_index) == 8 ? 1 : -1];

/* ---------- prototypes */

typedef wchar_t *(*ui_widget_text_replacement_function)(void *widget);

wchar_t *widget_replace_function_null(void *widget);
wchar_t *widget_controller(void *widget);

/* ---------- globals */

ui_widget_text_replacement_function replace_function_list[2] =
{
	widget_replace_function_null,
	widget_controller
};

wchar_t result[2] = { 0 };

/* ---------- public code */

wchar_t *ui_widget_search_and_replace_invoke(void *widget, unsigned short function_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\ui_widget_text_search_and_replace_functions.c", 45, widget);

	if ((short)function_index >= 0 && function_index < 2)
		return replace_function_list[(short)function_index](widget);

	return L"<invalid>";
}

/* ---------- private code */

wchar_t *widget_replace_function_null(void *widget)
{
	(void)widget;
	return L"";
}

wchar_t *widget_controller(void *widget)
{
	struct widget_instance_prefix *instance = widget;

	switch (instance->local_player_index)
	{
	case NONE:
	case 0:
		result[0] = L'1';
		result[1] = L'\0';
		return result;

	case 1:
		result[0] = L'2';
		result[1] = L'\0';
		return result;

	case 2:
		result[0] = L'3';
		result[1] = L'\0';
		return result;

	case 3:
		result[0] = L'4';
		result[1] = L'\0';
		return result;

	default:
		result[0] = L'?';
		result[1] = L'\0';
		return result;
	}
}
