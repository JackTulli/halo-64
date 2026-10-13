/*
HUD_MESSAGING.H

header included in hcex build.
*/

#ifndef __HUD_MESSAGING_H
#define __HUD_MESSAGING_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct icon_hud_element_definition;

/* ---------- prototypes/HUD_MESSAGING.C */

void hud_messaging_initialize(
	void);
void hud_messaging_initialize_for_new_map(
	void);
void hud_messaging_dispose_from_old_map(
	void);
void hud_messaging_dispose(
	void);
void hud_messaging_globals_update(
	void);
void scripted_hud_set_flashing_state(
	boolean flash);
void scripted_hud_set_objective(
	short message_index);
void scripted_hud_set_timer_position(
	short x,
	short y,
	short corner);
void scripted_hud_set_timer_time(
	short minutes,
	word seconds);
short scripted_hud_get_timer_ticks(
	void);
void scripted_hud_messages_clear(
	void);
void hud_render_timer(
	void);
void hud_print_message(
	short local_player_index,
	wchar_t const *message);
void hud_add_item_message(
	short local_player_index,
	long item_definition_index,
	short quantity,
	char message_offset);
void hud_broadcast_team_message(
	long victim_player_index,
	wchar_t const *message);
void hud_messaging_update(
	short local_player_index);
void hud_set_state_message(
	short local_player_index,
	short message_index);
void hud_set_state_message_text(
	short local_player_index,
	short custom_icon_index,
	short icon_string_index,
	boolean uses_scenario_names);
void hud_set_state_message_icon(
	short local_player_index,
	short custom_icon_index,
	struct icon_hud_element_definition const *icon);
void hud_enable_custom_state_message(
	short local_player_index,
	boolean enabled);
void hud_set_state_text(
	short local_player_index,
	wchar_t const *message);
wchar_t *hud_messaging_get_objective(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __HUD_MESSAGING_H
