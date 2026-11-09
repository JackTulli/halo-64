/*
N64_HUD64_STUB.C

HUD STYLE 64's resident part. The style's code and tables are a module,
rom:/hud64.dso (port/n64/src/n64_hud64.c and port/n64/game/n64_hud64_game.c,
built by tools/n64_build.py): loaded by the first view's HUD of a map with
the style at 64 (n64_hud64_prepare: after the map, its level and the HUD's
display are in memory, so that they have the heap they have without it),
given back with the map (n64_game.c: n64_hud64_module_end). With HUD STYLE
ORIGINAL the program keeps none of it.

Here: the setting, the flag Halo's HUD code reads (n64_hud64_active), and
each entry point the game calls, passed on to the module while it is
loaded. Without it each does nothing, and the game draws its own HUD.
*/

#include "cseries.h"

#include "math/real_math.h"

#include "objects/objects.h"
#include "tag_files/tag_groups.h"

#include "../src/n64.h"
#include "n64_hud64_module.h"

#include <stdio.h>
#include <stdlib.h>

/* the module's file: its code, tables and state (54,000 bytes while it is
loaded on the PC Expansion cartridge, 53,968 on the Multiplayer cartridge;
n64_module.c finds it room) */
#define HUD64_MODULE "rom:/hud64.dso"

/* The heap the style leaves free once its module and textures are in (the
style where memory allows, else the game's own HUD on that map, never a
crash), so that a late allocation in a long game still finds room: HUD
STYLE ORIGINAL's lowest free heap during play, the Multiplayer and PC
Expansion maps at 1 and 4 players from the menu, HUD RESOLUTION HIGH
DETAIL: 58 KB, PC Infinity at 1 player (64 KB at 4; Death Island 91 and
84; no Multiplayer map under 568). The module's prepare checks it once, at
the map's first 64 frame, with everything the module made for that frame
counted (n64_hud64_game.c prepare). The model cache loads a model only with
96 KB left over (80 KB for the local player's view model on the PC
Expansion cartridge), so it never takes the margin */
#define HUD64_HEAP_MARGIN (58 * 1024)

/* (libdragon's) */
extern void rspq_wait(void);

/* (Halo's HUD code: the view's HUD handed over to the 64 style) */
int n64_hud64_active = 0;
/* (this frame shows Halo's own small range label -- a 1-2p view
with no clear place for the 64 style's; set by the module's prepare, read by
hud_unit.c before it cuts the label out of Halo's tracker) */
int n64_hud64_label_halo = 0;

static int hud_style;
/* the module's memory, its entry points; a load tried in this map */
static void *module;
static const struct n64_hud64_entries *entry;
static int module_tried;

/* ---------- the setting (Settings: HUD STYLE; n64_menu.c) */

void n64_hud_set_style(
	int style)
{
	hud_style = style != 0;
	return;
}

int n64_hud_style(
	void)
{
	static int flag = -2;

	/* (testing, port.json flag "debug_hud_style=64" or "=0": that style
	whatever the setting) */
	if (flag == -2)
	{
		char text[8];

		flag = n64_debug_text("debug_hud_style", text, sizeof(text)) ? atoi(text) == 64 : -1;
	}
	return flag >= 0 ? flag : hud_style;
}

/* ---------- the game's functions the module calls (n64_hud64_imports.h) */

void *n64_hud64_import_object(long object_index, unsigned long valid_type_flags)
{
	return object_get_and_verify_type(object_index, valid_type_flags);
}

void *n64_hud64_import_tag(long group_tag, long tag_index)
{
	return tag_get(group_tag, tag_index);
}

/* ---------- the module */

/* the module in, where there is room; else the game's own HUD for the
rest of the map */
static void module_start(
	void)
{
	void *address;

	module_tried = TRUE;
	module = n64_module_load(HUD64_MODULE, N64_HUD64_ENTRIES, n64_hud64_heap_margin(), &address);
	if (!module)
	{
		/* (without room, n64_module.c's own line says how much there was) */
		n64_log("hud64: module not loaded: ORIGINAL's HUD");
		return;
	}
	entry = (const struct n64_hud64_entries *)address;
	n64_log("hud64: module loaded");
	return;
}

/* the map's HUD given back (n64_hud.c n64_hud_release; also as a map's
HUD is set up): the module's textures, once the RDP is done with them */
void n64_hud64_release(
	void)
{
	if (entry)
	{
		rspq_wait();
		entry->release();
	}
	return;
}

unsigned long n64_hud64_heap_margin(
	void)
{
	return HUD64_HEAP_MARGIN;
}

/* the module given back: its textures, the RDP done with them; its code no
longer called */
static void module_unload(
	void)
{
	n64_hud64_active = FALSE;
	if (!module)
		return;
	n64_hud64_release();
	entry = NULL;
	n64_module_free(module);
	module = NULL;
	return;
}

/* the map given back (n64_game.c: n64_game_free_map, after
n64_hud_release; and before a map's image, in case): the module too, its
memory the menu's and the next map's */
void n64_hud64_module_end(
	void)
{
	module_tried = FALSE;
	module_unload();
	return;
}

/* ---------- the entry points, passed on */

/* (the first in a map with the style at 64 loads the module. Its
textures without room that leaves the margin: the module given back too,
the game's own HUD for the rest of the map) */
int n64_hud64_prepare(
	int views)
{
	int ready;

	n64_hud64_label_halo = 0;
	if (!entry && !module_tried && n64_hud_style())
		module_start();
	if (!entry)
		return 0;
	/* (-1: given back, the module's own line on the log says why) */
	ready = entry->prepare(views);
	if (ready < 0)
	{
		module_unload();
		return 0;
	}
	return ready;
}

void n64_hud64_game_begin(
	int active)
{
	if (entry)
		entry->game_begin(active);
	else
		n64_hud64_active = FALSE;
	return;
}

void n64_hud64_game_end(
	int x0,
	int y0,
	int x1,
	int y1)
{
	if (entry)
		entry->game_end(x0, y0, x1, y1);
	return;
}

void n64_hud64_game_weapon(
	long weapon_index,
	boolean heat_hot)
{
	if (entry)
		entry->game_weapon(weapon_index, heat_hot);
	return;
}

void n64_hud64_game_grenades(
	long unit_index,
	unsigned long fill,
	unsigned long edge)
{
	if (entry)
		entry->game_grenades(unit_index, fill, edge);
	return;
}

void n64_hud64_game_grenade_switch(
	short local_player_index,
	long unit_index)
{
	if (entry)
		entry->game_grenade_switch(local_player_index, unit_index);
	return;
}

void n64_hud64_game_unit(
	long unit_index,
	struct unit_hud_interface_definition const *definition,
	long player_index,
	long script_flags,
	struct n64_hud64_unit_state *state)
{
	if (entry)
		entry->game_unit(unit_index, definition, player_index, script_flags, state);
	return;
}

void n64_hud64_game_tracker_clip(
	point2d const *center)
{
	if (entry)
		entry->game_tracker_clip(center);
	return;
}

int n64_hud64_game_prompt(
	char const *name,
	int swap)
{
	return entry ? entry->game_prompt(name, swap) : FALSE;
}

void n64_hud64_game_collect(
	int what,
	real alpha)
{
	if (entry)
		entry->game_collect(what, alpha);
	return;
}

int n64_hud64_game_collect_string(
	wchar_t const *string)
{
	return entry ? entry->game_collect_string(string) : FALSE;
}

int n64_hud64_game_collect_button(
	int kind)
{
	return entry ? entry->game_collect_button(kind) : FALSE;
}
