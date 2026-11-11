/*
N64_HUD64_MODULE.H

HUD STYLE 64's module (rom:/hud64.dso) and the program: the one symbol the
module exports, n64_hud64_entries (n64_hud64_game.c), its entry points. The
program's stub (n64_hud64_stub.c) passes each call on through it; the
module's link keeps only what they reach (tools/n64_build.py). After
cseries.h and math/real_math.h.
*/

#ifndef N64_HUD64_MODULE_H
#define N64_HUD64_MODULE_H

#define N64_HUD64_ENTRIES "n64_hud64_entries"

struct unit_hud_interface_definition;

struct n64_hud64_entries
{
	int (*prepare)(int views);
	void (*release)(void);
	void (*game_begin)(int active);
	void (*game_end)(int x0, int y0, int x1, int y1);
	void (*game_weapon)(long weapon_index, boolean heat_hot);
	void (*game_grenades)(long unit_index, unsigned long fill, unsigned long edge);
	void (*game_grenade_switch)(short local_player_index, long unit_index);
	void (*game_unit)(long unit_index, struct unit_hud_interface_definition const *definition, long player_index,
		long script_flags, struct n64_hud64_unit_state *state);
	void (*game_tracker_clip)(point2d const *center);
	int (*game_prompt)(char const *name, int swap);
	void (*game_collect)(int what, real alpha);
	int (*game_collect_string)(wchar_t const *string);
	int (*game_collect_button)(int kind);
};

#endif
