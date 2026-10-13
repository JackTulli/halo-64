/*
UI_WIDGET_GAME_DATA_INPUT_FUNCTIONS.H

header included in hcex build.
*/

#ifndef __UI_WIDGET_GAME_DATA_INPUT_FUNCTIONS_H
#define __UI_WIDGET_GAME_DATA_INPUT_FUNCTIONS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct widget_instance;

typedef void (*ui_widget_game_data_function)(
	struct widget_instance *widget);

/* ---------- prototypes/UI_WIDGET_GAME_DATA_INPUT_FUNCTIONS.C */

void ui_widget_game_data_function_invoke(
	struct widget_instance *widget,
	word function);
void widget_function_null(
	struct widget_instance *widget);
void settings_menu_update_extended_description(
	struct widget_instance *widget);
void playlist_settings_menu_update_extended_description(
	struct widget_instance *widget);
void playlist_gametype_select_menu_update_extended_description(
	struct widget_instance *widget);
void multiplayer_type_menu_update_extended_description(
	struct widget_instance *widget);
void solo_level_select_list_update_displayed_items(
	struct widget_instance *widget);
void difficulty_select_menu_update_extended_description(
	struct widget_instance *widget);
void set_textbox_to_build_number(
	struct widget_instance *widget);
void server_list_menu_update(
	struct widget_instance *widget);
void network_pregame_status_screen_update(
	struct widget_instance *widget);
void splitscreen_pregame_status_screen_update(
	struct widget_instance *widget);
void netgame_prejoin_players(
	struct widget_instance *widget);
void mutliplayer_settings_select_list_update_displayed_items(
	struct widget_instance *widget);
void player_profile_3wide_list_update(
	struct widget_instance *widget);
void player_profile_edit_select_menu_update_extended_description(
	struct widget_instance *widget);
void player_profile_1wide_list_update(
	struct widget_instance *widget);
void solo_game_objective_text(
	struct widget_instance *widget);
void player_profile_color_picker_update(
	struct widget_instance *widget);
void main_menu_animation_fakery(
	struct widget_instance *widget);
void mp_level_select_list_update_displayed_items(
	struct widget_instance *widget);
void get_active_player_profile_display_name(
	struct widget_instance *widget);
void get_editable_player_profile_display_name(
	struct widget_instance *widget);
void get_editable_playlist_profile_display_name(
	struct widget_instance *widget);
void get_active_player_profile_color_index(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_map_name(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_game_ruleset(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_teams_noteams(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_score_limit(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_score_limit_type(
	struct widget_instance *widget);
void multiplayer_game_set_bitmap_for_map(
	struct widget_instance *widget);
void multiplayer_game_set_bitmap_for_ruleset(
	struct widget_instance *widget);
void multiplayer_game_set_text_box_for_number_of_players(
	struct widget_instance *widget);
void multiplayer_edit_profile_set_ruleset_textbox_string_index(
	struct widget_instance *widget);
void system_link_status_check(
	struct widget_instance *widget);
void multiplayer_game_directions(
	struct widget_instance *widget);
void teams_no_teams_mp_game_bitmap_update(
	struct widget_instance *widget);
void warn_if_difficulty_will_nuke_saved_game(
	struct widget_instance *widget);
void dim_if_no_system_link_cable(
	struct widget_instance *widget);

/* ---------- globals */

extern ui_widget_game_data_function game_data_input_function_list[41];

/* ---------- public code */

#endif // __UI_WIDGET_GAME_DATA_INPUT_FUNCTIONS_H
