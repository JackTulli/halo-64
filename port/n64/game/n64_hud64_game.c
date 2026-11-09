/*
N64_HUD64_GAME.C

HUD STYLE 64 (port/n64/src/n64_hud64.c) fed by the game's own HUD code. With
the style on, interface_draw_screen still runs for each view: the parts the
design redraws (the weapon's ammunition and grenade panels, the unit's
shield and health, the messages and the pickup prompt) hand their values
here instead of drawing; everything else (the crosshairs and the motion
tracker, nav points, damage arrows, team markers, vehicles' panels, scopes'
masks) is drawn by the game as before. n64_render_game.c then draws the
view's 64 HUD from what was handed over (n64_hud64_game_end).
*/

#include "n64_hud64_imports.h"

#include "cseries.h"

#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "game/player_control.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/unit_hud_interface_definition.h"
#include "items/weapons.h"
#include "items/weapon_definitions.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "render/render.h"
#include "units/units.h"

#include "../src/n64.h"
#include "n64_hud64_module.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

extern struct game_variant global_variant;
extern int n64_firefight_active(void);

/* the view being drawn hands its HUD over (n64_hud64_game_begin) */
/* (the resident part's: port/n64/game/n64_hud64_stub.c) */
extern int n64_hud64_active;
/* (this frame shows Halo's own small range label: n64_hud64.c's prepare) */
extern int n64_hud64_label_halo;

static struct n64_hud64_state view;
/* messages taken from rasterizer_draw_unicode_string (n64_hud_game.c)
while hud_messaging_update draws them */
static int collecting, collect_alpha;
/* the state message names a button the N64 has none of for it (Halo's
BACK, "Hold %back for score"): not shown, as ORIGINAL shows none */
static int state_dropped;

int n64_hud64_game_collect_button(int kind);

enum { COLLECT_NONE, COLLECT_MESSAGE, COLLECT_STATE };

void n64_hud64_game_begin(
	int active)
{
	n64_hud64_active = active;
	csmemset(&view, 0, sizeof(view));
	collecting = COLLECT_NONE;
	return;
}

/* a weapon without magazines whose triggers spend its age (a plasma
weapon's battery); the flag and the ball spend none */
static boolean uses_battery(
	struct weapon_definition *definition)
{
	short i;

	for (i = 0; i < definition->weapon.triggers.count; i++)
	{
		struct weapon_trigger_definition *trigger = TAG_BLOCK_GET_ELEMENT(
			&definition->weapon.triggers, i, struct weapon_trigger_definition);

		if (trigger->age_generated_per_round > 0.f)
			return TRUE;
	}
	return FALSE;
}

/* the letter of the button that throws a grenade with a controller
port's controls, as xdk_input.c's pad_to_gamepad maps the grenade: 'R', 'A'
(bumper jumper), 'D' (the D-pad's left, Golden style) or 'Z' (fire and
grenades swapped) */
static char grenade_button_letter(
	int port)
{
	const struct n64_controls *controls = &n64_controls[port & 3];
	unsigned short grenade = controls->scheme == N64_SCHEME_GOLDEN_STYLE ? N64_BUTTON_D_LEFT :
		controls->scheme == N64_SCHEME_BUMPER_JUMPER ? N64_BUTTON_A : N64_BUTTON_R;

	if (controls->swap_fire)
		grenade = N64_BUTTON_Z;
	return grenade == N64_BUTTON_Z ? 'Z' : grenade == N64_BUTTON_A ? 'A' : grenade == N64_BUTTON_D_LEFT ? 'D' : 'R';
}

/* the seat's secondary function in the grenade panel's place: the
button's letter over a fill that shows its recharge. A weapon no player
holds (a seat's: Halo hides the grenade panel of a vehicle's driver and
gunner) whose grenade button fires its second trigger -- the weapon's flag "secondary
trigger overrides grenades", which weapons.c tests (weapon_owner_update),
so a Custom Edition vehicle's shows too. Its icon: a charge if that trigger
draws from a magazine (it must recharge: the Banshee's fuel rod), else a
round (it never stops: the Scorpion's machine gun, always ready). Its fill:
the part of the magazine's recharge gone, from the weapon's own state --
weapons.c reloads an empty magazine (state_timer from original_time, the
reload's length), then chambers it (chamber_time), and fires only from an
idle magazine */
static void secondary_seat(
	struct weapon_datum *weapon,
	struct weapon_definition *definition)
{
	struct weapon_trigger_definition *trigger;
	long left = 0, total = 0;

	if (TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) ||
		!TEST_FLAG(definition->weapon.flags, _weapon_secondary_trigger_overrides_grenades_bit) ||
		definition->weapon.triggers.count < 2)
		return;
	trigger = TAG_BLOCK_GET_ELEMENT(&definition->weapon.triggers, 1, struct weapon_trigger_definition);
	view.show_secondary = TRUE;
	view.secondary_button = grenade_button_letter(render.local_player_index);
	view.secondary_icon = (unsigned char)(trigger->magazine_index == NONE ||
		trigger->magazine_index >= definition->weapon.magazines.count);
	if (!view.secondary_icon)
	{
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(
			&definition->weapon.magazines, trigger->magazine_index, struct weapon_magazine_definition);
		struct weapon_magazine *magazine = &weapon->weapon.magazines[trigger->magazine_index];
		long chamber = (long)(magazine_definition->chamber_time * TICKS_PER_SECOND + 0.5f);
		long reload = magazine->original_time > 0 ? magazine->original_time : 0;

		/* (weapons.c spends a tick finishing the reload and one starting the chamber: an emptied magazine fires
		again reload + 2 + chamber ticks after the shot -- the Banshee's fuel rod, original_time 0: 122 ticks) */
		total = reload + 2 + chamber;
		switch (magazine->state)
		{
		case _magazine_reloading: left = magazine->state_timer + 2 + chamber; break;
		case _magazine_unchambered: left = 1 + chamber; break;
		case _magazine_chambering: left = magazine->state_timer; break;
		default: left = magazine->rounds_loaded < trigger->rounds_per_shot ? total : 0; break;
		}
	}
	/* (255 only when it can fire: a recharge's last part rounds down) */
	view.secondary_fill = (unsigned char)(left <= 0 || total <= 0 ? 255 :
		PIN((total - PIN(left, 0, total)) * 255 / total, 0, 254));
}

/* an unlimited weapon's main recharge under its card, in Halo's blue,
filling to ready: its first trigger draws rounds from a magazine that
holds some (the Scorpion's cannon: 1 round, chambered after
each; the rocket Warthog's gun: 3; the Multiplayer Ghost's: 50). The part
of the magazine's reload and chamber gone, from its state as weapons.c
runs it -- as secondary_seat counts it: the reload (original_time), a tick
finishing it, a tick starting the chamber, the chamber (chamber_time);
255 when the magazine is idle with a shot loaded */
static void main_cooldown(
	struct weapon_datum *weapon,
	struct weapon_definition *definition)
{
	struct weapon_trigger_definition *trigger;
	struct weapon_magazine_definition *magazine_definition;
	struct weapon_magazine *magazine;
	long chamber, total, left;

	if (TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) || definition->weapon.triggers.count < 1)
		return;
	trigger = TAG_BLOCK_GET_ELEMENT(&definition->weapon.triggers, 0, struct weapon_trigger_definition);
	if (trigger->magazine_index == NONE || trigger->magazine_index >= definition->weapon.magazines.count ||
		trigger->rounds_per_shot <= 0)
		return;
	magazine_definition = TAG_BLOCK_GET_ELEMENT(&definition->weapon.magazines, trigger->magazine_index,
		struct weapon_magazine_definition);
	if (magazine_definition->rounds_loaded_maximum <= 0)
		return;
	magazine = &weapon->weapon.magazines[trigger->magazine_index];
	chamber = (long)(magazine_definition->chamber_time * TICKS_PER_SECOND + 0.5f);
	total = (magazine->original_time > 0 ? magazine->original_time : 0) + 2 + chamber;
	switch (magazine->state)
	{
	case _magazine_reloading: left = magazine->state_timer + 2 + chamber; break;
	case _magazine_unchambered: left = 1 + chamber; break;
	case _magazine_chambering: left = magazine->state_timer; break;
	default: left = magazine->rounds_loaded < trigger->rounds_per_shot ? total : 0; break;
	}
	view.show_cooldown = TRUE;
	view.cooldown_fill = (unsigned char)(left <= 0 ? 255 : PIN((total - PIN(left, 0, total)) * 255 / total, 0, 254));
}

/* the weapon's ammunition: rounds (the reserve as the number, the magazine
as the grid, one tick a round up to 60, else in proportion), a battery
(its percent, the grid in proportion; the plasma weapons' magazine holds no
rounds), or neither (the panel alone: the flag, the ball) */
void n64_hud64_game_weapon(
	long weapon_index,
	boolean heat_hot)
{
	struct weapon_datum *weapon;
	struct weapon_definition *definition;

	if (!n64_hud64_active || weapon_index == NONE)
		return;
	weapon = weapon_get(weapon_index);
	definition = weapon_definition_get(weapon->definition_index);
	view.show_ammo = TRUE;
	view.ammo_kind = N64_HUD64_NO_AMMO;
	secondary_seat(weapon, definition);
	main_cooldown(weapon, definition);
	/* (Halo's weapons.c spends a magazine's rounds and a battery's age only
	of a weapon a player holds: a vehicle's guns never run out) */
	view.ammo_infinite = (unsigned char)!TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit);
	if (definition->weapon.magazines.count > 0 &&
		TAG_BLOCK_GET_ELEMENT(&definition->weapon.magazines, 0, struct weapon_magazine_definition)->rounds_loaded_maximum > 0)
	{
		struct weapon_magazine_definition *magazine = TAG_BLOCK_GET_ELEMENT(
			&definition->weapon.magazines, 0, struct weapon_magazine_definition);
		long loaded = weapon->weapon.magazines[0].rounds_loaded;
		long maximum = magazine->rounds_loaded_maximum;

		view.ammo_kind = N64_HUD64_ROUNDS;
		view.total = (short)weapon->weapon.magazines[0].rounds_total;
		if (maximum > 60)
		{
			loaded = (loaded * 60 + maximum - 1) / maximum;
			maximum = 60;
		}
		view.loaded = (short)PIN(loaded, 0, 60);
		view.loaded_max = (short)PIN(maximum, 0, 60);
	}
	else if (uses_battery(definition))
	{
		/* (a battery, Halo's age: the plasma weapons have an empty magazine) */
		real left = PIN(1.f - weapon->weapon.age, 0.f, 1.f);

		view.ammo_kind = N64_HUD64_BATTERY;
		view.total = (short)(left * 100.f + 0.5f);
		/* (its heat, as ORIGINAL shows it under the battery) */
		view.show_heat = TRUE;
		view.heat = PIN(weapon->weapon.heat, 0.f, 1.f);
		view.heat_hot = (unsigned char)(heat_hot != FALSE);
		view.loaded = (short)(left * 60.f + 0.5f);
		view.loaded_max = 60;
	}
	return;
}

/* the slots a switch attempt flashes, for each local player's next
grenade panel (player_control.c, at the switch button: the type a switch
goes to has none) */
static unsigned char grenade_switch_slots[4];

void n64_hud64_game_grenade_switch(
	short local_player_index,
	long unit_index)
{
	struct unit_datum *unit;
	short current, next;

	if (local_player_index < 0 || local_player_index > 3 || unit_index == NONE || NUMBER_OF_UNIT_GRENADE_TYPES < 2)
		return;
	unit = unit_get(unit_index);
	current = unit->unit.current_grenade_index;
	if (current == NONE)
	{
		/* (none of either type) */
		grenade_switch_slots[local_player_index] |= 3;
		return;
	}
	next = (short)((current + 1) % 2);
	if (unit->unit.grenade_counts[next] == 0)
		grenade_switch_slots[local_player_index] |= (unsigned char)(1 << next);
	return;
}

void n64_hud64_game_grenades(
	long unit_index,
	unsigned long fill,
	unsigned long edge)
{
	struct unit_datum *unit;

	if (!n64_hud64_active || unit_index == NONE)
		return;
	unit = unit_get(unit_index);
	view.show_grenades = TRUE;
	view.frags = unit->unit.grenade_counts[0];
	view.plasmas = NUMBER_OF_UNIT_GRENADE_TYPES > 1 ? unit->unit.grenade_counts[1] : 0;
	/* (the type a throw uses: Halo's own grenade panel shows that type) */
	view.grenade_type = (signed char)unit->unit.current_grenade_index;
	/* (the panel in Halo's colours for its state: its normal ones are the
	design's blue and rim) */
	view.grenade_fill = fill | 0xFF000000;
	view.grenade_edge = edge | 0xFF000000;
	/* a brief double flash of a type's slot -- on, off, on, off -- when the
	type runs out, when the player tries to switch to it with none, and when
	the player tries to throw with none (Halo's "no grenades": its weapon
	HUD's threw-with-no-grenade state); at no other time, so that a flash
	always means one of these. A phase lasts 2 ticks and at least a
	frame of this view, so 3-4 players' frame rates show both blinks */
	{
		static long last_unit[4] = {NONE, NONE, NONE, NONE};
		static short last_count[4][2];
		static unsigned char last_no_grenade[4], slots[4];
		static signed char phase[4] = {4, 4, 4, 4};
		static long toggled[4];
		short p = render.local_player_index, t;
		long now = game_time_get();
		short counts[2];
		unsigned char start = 0, no_grenade;

		if (p < 0 || p > 3)
			return;
		counts[0] = view.frags;
		counts[1] = view.plasmas;
		/* (hud_weapon.c's threw-with-no-grenade condition) */
		no_grenade = (unsigned char)(counts[0] == 0 && counts[1] == 0 && !unit->unit.grenade_throw_state &&
			TEST_FLAG(unit->unit.control_flags, _unit_control_throw_grenade_bit));
		if (last_unit[p] == unit_index)
		{
			for (t = 0; t < 2; t++)
			{
				if (last_count[p][t] > 0 && counts[t] == 0)
					start |= (unsigned char)(1 << t);
			}
			/* (not over a flash that runs: the throw that spent the last
			grenade, its button still held) */
			if (no_grenade && !last_no_grenade[p] && phase[p] >= 4)
				start |= view.grenade_type == 1 ? 2 : view.grenade_type == 0 ? 1 : 3;
		}
		start |= grenade_switch_slots[p];
		grenade_switch_slots[p] = 0;
		if (start)
			slots[p] = start, phase[p] = 0, toggled[p] = now;
		else if (phase[p] < 4 && now - toggled[p] >= 2)
			phase[p]++, toggled[p] = now;
		last_unit[p] = unit_index;
		last_count[p][0] = counts[0];
		last_count[p][1] = counts[1];
		last_no_grenade[p] = no_grenade;
		view.grenade_flash = phase[p] < 4 && phase[p] % 2 == 0 ? slots[p] : 0;
	}
	return;
}

/* Halo's get_flash_color (hud_draw.c) as the factor it blends by: 0
the colour, 1 its flashing colour -- HUD STYLE 64 blends its own colours
toward the tag's flashing colour by it. A flash that began at game time 0
gives 1 throughout, as get_flash_color does. (Here, not in hud_unit.c: the
module's code, out of the program for HUD STYLE ORIGINAL's players) */
static real flash_factor(
	struct hud_color_definition const *c,
	long reference)
{
	boolean reverse = TEST_FLAG(c->flash_flags, 0); /* (hud_draw.c's _hud_flash_reverse_colors_bit) */
	real phase, p;

	if (!(c->flash_period > 0.f))
		return 0.f;
	phase = (real)fmod((game_time_get() - reference) * (1.0f / TICKS_PER_SECOND), c->flash_period);
	if (!(phase < c->number_of_flashes * (c->flash_delay + c->flash_length)))
		return 0.f;
	p = (real)fmod(phase, c->flash_delay + c->flash_length);
	if (!reference)
		return reverse ? 0.f : 1.f;
	if (p < c->flash_length)
	{
		real f = square_root(PIN((real)(1.0 - (cos(p / c->flash_length * 6.283f) + 1.0) * 0.5), 0.0f, 1.0f));

		return reverse ? 1.f - f : f;
	}
	return reverse ? 1.f : 0.f;
}

/* (hud_unit.c's panel flags in its globals' script_flags: the script hides a
panel or makes it blink) */
enum
{
	PANEL_HEALTH_DONT_SHOW_BIT = 0,
	PANEL_HEALTH_BLINK_BIT,
	PANEL_SHIELD_DONT_SHOW_BIT,
	PANEL_SHIELD_BLINK_BIT,
	PANEL_MOTION_SENSOR_DONT_SHOW_BIT
};

/* The player's own unit panel (hud_unit.c hud_render_unit_interface, which
draws only the motion sensor then): the shield and health, the health's
colour, and Halo's flashes and damage ghost, each as Halo's own drawing of
the panel works it out (hud_unit.c, hud_draw.c), from the unit, its HUD's
tag, the script's panel flags and the view's unit HUD state, whose flash
start ticks are set here as Halo sets them (here, not in hud_unit.c: the
module's code, out of the program for HUD STYLE ORIGINAL's players) */
void n64_hud64_game_unit(
	long unit_index,
	struct unit_hud_interface_definition const *definition,
	long player_index,
	long script_flags,
	struct n64_hud64_unit_state *state)
{
	struct unit_datum *unit;
	boolean shield_shown, health_flashing, shield_flashing;
	real vitality;

	if (!n64_hud64_active || unit_index == NONE)
		return;
	unit = unit_get(unit_index);
	shield_shown = game_engine_has_shield(player_index) && !TEST_FLAG(script_flags, PANEL_SHIELD_DONT_SHOW_BIT);
	view.show_shield = shield_shown;
	/* (above 1 the overshield: Halo's hud_unit.c draws a meter a level, to 4
	levels over the shield) */
	view.shield = PIN(unit->object.shield_vitality, 0.f, 5.f);
	view.health = PIN(unit->object.body_vitality, 0.f, 1.f);
	view.show_sensor = !TEST_FLAG(script_flags, PANEL_MOTION_SENSOR_DONT_SHOW_BIT) &&
		game_engine_hud_draw_motion_sensor(player_index);
	/* (the health's colour as Halo picks it: the tag's middle colour between
	its cutoffs, its low colour under the lower one; 0 above: the design's) */
	vitality = unit->object.body_vitality;
	view.health_colour = vitality >= definition->health_meter.health_extras.max_cutoff ? 0 :
		vitality <= definition->health_meter.health_extras.min_cutoff ?
			(definition->health_meter.meter.min_color & 0xFFFFFF) | 0xFF000000 :
			(definition->health_meter.health_extras.mid_color & 0xFFFFFF) | 0xFF000000;
	/* (Halo's flashes) the health panel's background flashes while the
	unit's shield is depleted -- only where the tag gives that panel a
	background (1 player's 'cyborg'; not 'cyborg_mp') */
	health_flashing = TEST_FLAG(unit->object.damage_flags, _object_shield_depleted_bit) ||
		TEST_FLAG(script_flags, PANEL_HEALTH_BLINK_BIT);
	if (!health_flashing)
		state->last_health_flash_time = NONE;
	else if (state->last_health_flash_time == NONE)
		state->last_health_flash_time = game_time_get();
	if (health_flashing && definition->health_meter.background.interface_bitmap.index != NONE &&
		!TEST_FLAG(script_flags, PANEL_HEALTH_DONT_SHOW_BIT))
	{
		struct hud_color_definition const *c = &definition->health_meter.background.colors;

		view.health_flash = (unsigned char)(255.f * flash_factor(c, state->last_health_flash_time) + 0.5f);
		view.health_flash_colour = c->flash_color & 0xFFFFFF;
	}
	/* (the low-shield blink: the shield panel's background flashes while the
	shield is under 25% or the script asks) */
	shield_flashing = shield_shown && (unit->object.shield_vitality < 0.25f ||
		TEST_FLAG(script_flags, PANEL_SHIELD_BLINK_BIT));
	if (!shield_flashing)
		state->last_shield_flash_time = NONE;
	else if (state->last_shield_flash_time == NONE)
		state->last_shield_flash_time = game_time_get();
	if (shield_flashing && definition->shield_meter.background.interface_bitmap.index != NONE)
	{
		struct hud_color_definition const *c = &definition->shield_meter.background.colors;

		view.shield_flash = (unsigned char)(255.f * flash_factor(c, state->last_shield_flash_time) + 0.5f);
		view.shield_flash_colour = c->flash_color & 0xFFFFFF;
	}
	/* (the damage ghost: the value Halo holds after a drop --
	hud_update_unit_local_player keeps it -- and the factor its meter's flash
	colour adds by, 1 through the 15-tick hold, as hud_draw_meter's fade) */
	if (state->last_shield_vitality > unit->object.shield_vitality)
	{
		real fade = state->fade_time < 0.0f ? 0.0f : PIN(1.0f - state->fade_time, 0.0f, 1.0f);

		view.shield_held = state->last_shield_vitality;
		view.shield_ghost = (unsigned char)(255.f * fade + 0.5f);
		view.shield_ghost_colour = definition->shield_meter.meter.flash_color & 0xFFFFFF;
	}
	return;
}

/* Halo's tracker foreground (its marker and its small range label at the
plate's bottom left) drawn above 0.78 of the blip disc's radius under its
centre -- the label left out, the marker kept (HUD STYLE 64 draws its own
label, clear of the tracker's circle); not in a frame that shows Halo's own
label (n64_hud64_label_halo: a view with no clear place for the style's).
center: the
blip disc's (hud_unit.c's corner); NULL: the view's whole rectangle again,
after it. (n64_hud_game.c's blip disc: a radius of 32, 0.75 of it in split
screen; its rows scaled as the HUD's) */
void n64_hud64_game_tracker_clip(
	point2d const *center)
{
	int rect[4], bottom;

	if (!n64_hud64_active || n64_hud64_label_halo)
		return;
	n64_hud_view_rect(rect);
	bottom = rect[3];
	if (center)
	{
		bottom = (int)(((real)center->y + render.camera.viewport_bounds.y0 +
			0.78f * 32.f * (local_player_count() > 1 ? 0.75f : 1.f)) * n64_render_layout_scale_y());
		bottom = bottom < rect[3] ? (bottom > rect[1] ? bottom : rect[1]) : rect[3];
	}
	n64_hud64_scissor(rect[0], rect[1], rect[2], bottom);
	return;
}

/* the weapon pickup prompt (hud.c, n64_hud_weapon_action_response) */
int n64_hud64_game_prompt(
	char const *name,
	int swap)
{
	if (!n64_hud64_active)
		return FALSE;
	snprintf(view.prompt, sizeof(view.prompt), "%s", name);
	view.prompt_swap = (unsigned char)(swap != 0);
	return TRUE;
}

/* hud_messaging_update's text taken instead of drawn: the message queue's
lines one by one (COLLECT_MESSAGE, each with its fade), or the pieces of
the state message (COLLECT_STATE) */
void n64_hud64_game_collect(
	int what,
	real alpha)
{
	collecting = n64_hud64_active ? what : COLLECT_NONE;
	collect_alpha = (int)PIN(alpha * 255.f, 0.f, 255.f);
	if (what == COLLECT_STATE)
		state_dropped = FALSE;
	return;
}

static void append_ascii(
	char *out,
	int size,
	wchar_t const *text)
{
	int n = (int)strlen(out);
	int i;

	for (i = 0; text && text[i] && n + 1 < size; i++)
	{
		wchar_t c = text[i];

		if (c == '|' && text[i + 1] == 'n')
		{
			/* (Halo's line break) */
			out[n++] = ' ';
			i++;
			continue;
		}
		if (c >= 'a' && c <= 'z')
			c -= 'a' - 'A';
		out[n++] = c >= 32 && c < 127 ? (char)c : c == '\n' || c == '\t' ? ' ' : '?';
	}
	out[n] = 0;
	return;
}

/* rasterizer_draw_unicode_string: 1 if the 64 HUD took the text */
int n64_hud64_game_collect_string(
	wchar_t const *string)
{
	if (!n64_hud64_active || collecting == COLLECT_NONE)
		return FALSE;
	if (collecting == COLLECT_STATE)
	{
		/* (a text with Halo's %icon names -- the game engine's own state
		texts -- as draw_string_and_hack_in_icons reads them: %action the
		action button; another button the message dropped) */
		wchar_t piece[96];
		int n = 0, i, j;

		if (view.state_text[0])
			append_ascii(view.state_text, sizeof(view.state_text), L" ");
		for (i = 0; string[i]; i++)
		{
			if (string[i] == '%' && string[i + 1] >= 'a' && string[i + 1] <= 'z')
			{
				j = i + 1;

				while ((string[j] >= 'a' && string[j] <= 'z') || string[j] == '-' || (string[j] >= '0' && string[j] <= '9'))
					j++;
				piece[n] = 0;
				append_ascii(view.state_text, sizeof(view.state_text), piece);
				n = 0;
				if (j - i - 1 == 6 && !memcmp(string + i + 1, L"action", 6 * sizeof(wchar_t)))
					n64_hud64_game_collect_button(1);
				else
					state_dropped = TRUE;
				i = j - 1;
				continue;
			}
			if (n < (int)(sizeof(piece) / sizeof(piece[0])) - 1)
				piece[n++] = string[i];
		}
		piece[n] = 0;
		append_ascii(view.state_text, sizeof(view.state_text), piece);
		/* (the game engine's "Hold BACK for score" in its first 15 seconds
		names the Xbox's BACK button in its words: the N64 has none, its
		score is START's pause screen, and ORIGINAL shows nothing then) */
		for (i = 0; string[i]; i++)
		{
			if (string[i] == 'B' && string[i + 1] == 'A' && string[i + 2] == 'C' && string[i + 3] == 'K' &&
				(i == 0 || string[i - 1] == ' ') && (!string[i + 4] || string[i + 4] == ' '))
				state_dropped = TRUE;
		}
		return TRUE;
	}
	if (view.message_count < N64_HUD64_MESSAGES)
	{
		view.messages[view.message_count].text[0] = 0;
		append_ascii(view.messages[view.message_count].text, sizeof(view.messages[0].text), string);
		view.messages[view.message_count].alpha = (unsigned char)collect_alpha;
		view.message_count++;
	}
	return TRUE;
}

/* a button icon in the state message: the action button as the design's
B (every scheme's action is the N64's B); 1 if taken */
int n64_hud64_game_collect_button(
	int kind)
{
	int n;

	if (!n64_hud64_active || collecting != COLLECT_STATE)
		return FALSE;
	/* (kind 0: a custom icon -- the seat's or vehicle's name, which Halo
	writes as text through render_state_text, collected there; 1: the action
	button; 2: another button, which the N64 has none of here: the message
	dropped -- a button must not become a bare word) */
	if (kind == 0)
		return FALSE;
	if (kind != 1)
	{
		state_dropped = TRUE;
		return TRUE;
	}
	n = (int)strlen(view.state_text);
	if (n + 3 < (int)sizeof(view.state_text))
	{
		if (n)
			view.state_text[n++] = ' ';
		view.state_text[n++] = '\2';
		view.state_text[n] = 0;
	}
	return TRUE;
}

/* the players' names in a text marked ('\1' around each) for the
numbers' colour, as the design colours "YOU KILLED <name>" */
static void mark_names(
	char *text,
	int size)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		char name[24] = "";
		char *at;
		int length;

		append_ascii(name, sizeof(name), player->name);
		length = (int)strlen(name);
		if (length < 2)
			continue;
		at = strstr(text, name);
		if (at && (at == text || at[-1] == ' ') && (at[length] == 0 || at[length] == ' ' || at[length] == '.' ||
			at[length] == '!') && (int)strlen(text) + 3 < size)
		{
			memmove(at + length + 2, at + length, strlen(at + length) + 1);
			memmove(at + 1, at, (size_t)length);
			at[0] = '\1';
			at[length + 1] = '\1';
		}
	}
	return;
}

/* the score: yours and the best other's (team games: the teams'), of the
game's score to win; not in Firefight (its own status line) */
static void score(
	void)
{
	long player_index = local_player_get_player_index(render.local_player_index);
	struct data_iterator iterator;
	struct player_datum *player;
	boolean teams;
	long mine, other = 0;

	if (!game_engine || !game_engine->get_player_score || n64_firefight_active() || player_index == NONE)
		return;
	teams = game_engine_has_teams();
	mine = game_engine->get_player_score(player_index, teams ? _get_score_team : _get_score_individual);
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		long score;

		if (iterator.datum_index == player_index ||
			(teams && player->team_index == player_get(player_index)->team_index))
			continue;
		score = game_engine->get_player_score(iterator.datum_index, teams ? _get_score_team : _get_score_individual);
		other = score > other ? score : other;
	}
	view.show_score = TRUE;
	view.score = (short)PIN(mine, 0, 999);
	view.other = (short)PIN(other, 0, 999);
	view.other_leads = other > mine;
	view.limit = (short)PIN(global_variant.universal_variant.score_to_win, 1, 9999);
	return;
}

/* the view's 64 HUD, from what its HUD code handed over */
void n64_hud64_game_end(
	int x0,
	int y0,
	int x1,
	int y1)
{
	short i;

	if (!n64_hud64_active)
		return;
	collecting = COLLECT_NONE;
	if (state_dropped)
		view.state_text[0] = 0;
	score();
	for (i = 0; i < view.message_count; i++)
		mark_names(view.messages[i].text, sizeof(view.messages[i].text));
	if (view.state_text[0])
		mark_names(view.state_text, sizeof(view.state_text));
	n64_hud64_draw(&view, x0, y0, x1, y1, local_player_count());
	n64_hud64_active = FALSE;
	return;
}

/* the module's one export (n64_hud64_module.h): its entry points, which the
program's stub finds by name and calls through */
/* the stub's prepare (n64_hud64_stub.c): n64_hud64_prepare; -1, the module
given back and the game's own HUD for the rest of the map, when the view's
textures are not in memory -- at any frame: the game's HUD must not draw
while the module holds the HUD's meter block (n64_hud_lend_slots), and a
context that failed is not tried again -- and, once, without the margin
still free after the module's first 64 frame (cached models dropped for it
if short). That check is at the next frame's first prepare, not at this
frame's: render_frame asks for the HUD's critical sprites after each
view's prepare, and while the module holds the meter block they load on
the heap (n64_hud.c: no arena then). So it counts all the first frame
made: the module's textures, their upload blocks, the outline sheets and
every view's critical sprites. Without the margin the map has drawn one 64
frame, and the game's own HUD from then on */
static int prepare(
	int views)
{
	/* (the prepares of the module's first 64 frame, one a view: the check
	at the next; then done) */
	static int calls, checked;
	int ready = n64_hud64_prepare(views);
	char line[128];

	if (!n64_hud_style())
		return ready;
	if (!ready)
	{
		n64_log("hud64: the module given back (this view's textures not loaded): the game's own HUD for the rest "
			"of the map");
		return -1;
	}
	if (checked || ++calls <= views)
		return ready;
	if (!n64_render_room_leaving(0, n64_hud64_heap_margin()))
	{
		snprintf(line, sizeof(line), "hud64: the module given back (%lu KB free after its first frame, a %lu KB "
			"margin kept): the game's own HUD on this map", n64_heap_free() / 1024, n64_hud64_heap_margin() / 1024);
		n64_log(line);
		return -1;
	}
	checked = TRUE;
	return ready;
}

const struct n64_hud64_entries n64_hud64_entries =
{
	prepare,
	n64_hud64_release,
	n64_hud64_game_begin,
	n64_hud64_game_end,
	n64_hud64_game_weapon,
	n64_hud64_game_grenades,
	n64_hud64_game_grenade_switch,
	n64_hud64_game_unit,
	n64_hud64_game_tracker_clip,
	n64_hud64_game_prompt,
	n64_hud64_game_collect,
	n64_hud64_game_collect_string,
	n64_hud64_game_collect_button
};
