/*
HUD.H

header included in hcex build.
*/

#ifndef __HUD_H
#define __HUD_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct hud_globals_definition;
struct tag_block;
union real_argb_color;

/* ---------- prototypes/HUD.C */

void hud_initialize(
	void);
void hud_dispose(
	void);
void hud_dispose_from_old_map(
	void);
void hud_update(
	void);
void hud_initialize_for_new_map(
	void);
void hud_initialize_nav_points(
	void);
void hud_initialize_nav_points_for_new_map(
	void);
void hud_dispose_nav_points_from_old_map(
	void);
void hud_dispose_nav_points(
	void);
void hud_update_nav_points(
	void);

wchar_t const *hud_get_item_string(
	long string_index);

void hud_load(
	boolean load);
void hud_autosave(
	boolean active);
void hud_draw_players(
	void);

void hud_picked_up_powerup(
	short local_player_index,
	long powerup_definition_index);
void hud_picked_up_grenade(
	short local_player_index,
	long grenade_definition_index);
void hud_picked_up_ammunition(
	short local_player_index,
	long weapon_definition_index,
	short ammunition_count);
void hud_picked_up_weapon(
	short local_player_index,
	long weapon_definition_index);
void hud_render_nav_points(
	short local_player_index);
void temporary_hud_draw_reticle(
	real angle,
	union real_argb_color const *color);
void temporary_hud_draw(
	void);
void hud_draw_screen(
	void);

/* ---------- prototypes/HUD_SOUNDS.C */

void hud_play_sound(
	short local_player_index,
	unsigned long state_flags,
	struct tag_block const *sounds,
	long *sound_indices,
	word *played_flags);

/* ---------- globals */

extern struct hud_globals_definition *hud_globals;

/* ---------- public code */

#endif // __HUD_H
