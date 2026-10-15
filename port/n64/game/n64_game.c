/*
N64_GAME.C

The game-side half of the N64 platform layer, compiled like the game's own
units (it sees the game's headers):

- the menu's games (n64_game_menu, from main.c's main_menu_load: the
  N64's own menu, port/n64/src/n64_menu.c) and, for testing, the start of
  a game without it (n64_game_setup, called from main.c),
- the memory map: the game state and the loaded map are ordinary
  allocations (physical_memory_map.c is left out),
- the cache files: maps are converted ahead of time (tools/n64/convert.py)
  and read from the cartridge whole; the Xbox's copy-to-hard-disk cache
  (cache_files_windows.c) is left out,
- the game state's buffer and its (absent) persistent storage,
- the hooks the Linux port added to the game (interpolation, screen width).
*/

#include "cseries.h"

#include "ai/ai_debug.h"
#include "cache/cache_files.h"
#include "cache/physical_memory_map.h"
#include "game/game.h"
#include "main/main.h"
#include "saved games/game_state.h"
#include "game/players.h"
#include "objects/objects.h"
#include "memory/data.h"
#include "units/units.h"
#include "items/weapons.h"
#include "game/game_engine.h"
#include "text/unicode.h"
#include "tag_files/tag_groups.h"

#include "../src/n64.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *memalign(size_t alignment, size_t size);

extern short player_spawn_count;

/* ---------- Firefight (n64_firefight.c) */

void n64_firefight_begin(int difficulty, int player_count);
void n64_bots_begin(int count, int skill, int teams);
void n64_bots_stop(void);
int n64_bot_number(long player_index);
void n64_firefight_stop(void);
int n64_firefight_active(void);
void n64_firefight_tick(void);
int n64_firefight_results(char lines[][64], int maximum);
int n64_firefight_status(char *text, int size);
long n64_game_engine_postgame_state(void);

#ifdef HALO_N64_CAMPAIGN
/* ---------- a campaign level (n64_campaign.c) */

void n64_campaign_begin(int difficulty);
void n64_campaign_stop(void);
int n64_campaign_active(void);
void n64_campaign_tick(void);
int n64_campaign_status(char *text, int size);
void main_set_difficulty(short difficulty);
#endif

/* ---------- a game straight from the start, for testing */

#define N64_TEST_GAME_VARIANT "slayer"
#define N64_TEST_PLAYER_COUNT 2

/* what the menu chose (or the test game) */
static struct n64_menu_choice n64_choice = { .bot_skill = 1, .enhanced_fx = 1 };
/* a Pro variant: everyone starts with a pistol and an assault rifle */
static int n64_pro_weapons;

/* the game's variant, players and map from the choice */
static void n64_game_start(
	void)
{
	static char map_name[96];
	struct game_variant variant, temporary_variant;
	char const *folder = n64_menu_map_folder(n64_choice.map);
	char line[96];

#ifdef HALO_N64_CAMPAIGN
	/* a campaign level: no game engine (Halo's campaign), the difficulty,
	one player; its squads and checkpoints are n64_campaign.c's */
	if (n64_choice.campaign)
	{
		extern int n64_gamepad_ports[4];
		int port;

		n64_pro_weapons = FALSE;
		game_set_game_variant(NULL);
		main_set_difficulty((short)PIN(n64_choice.difficulty, 0, 3));
		n64_firefight_stop();
		n64_bots_stop();
		n64_campaign_begin(n64_choice.difficulty);
		player_spawn_count = (short)PIN(n64_choice.player_count, 1, 2);
		snprintf(map_name, sizeof(map_name), "levels\\%s\\%s", folder, folder);
		main_set_map_name(map_name);
		main_disallow_persistent_storage();
		n64_gamepad_ports[0] = n64_choice.ports[0];
		for (port = 0; port < 4; port++)
		{
			if (port != n64_choice.ports[0])
			{
				n64_gamepad_ports[1] = port;
				break;
			}
		}
		snprintf(line, sizeof(line), "n64: campaign %s, difficulty %d", map_name, n64_choice.difficulty);
		n64_log(line);
		return;
	}
	n64_campaign_stop();
#endif

	/* Firefight: team slayer, every player on the red team, no score to
	reach; the waves are n64_firefight.c's */
	{
		/* (a Pro variant: its plain one, everyone starting with a pistol
		and an assault rifle -- n64_game_pro_weapons) */
		char variant_name[32];
		char const *name = n64_choice.firefight ? "team_slayer" : n64_menu_game_variant(n64_choice.game_type);
		int length = (int)strlen(name);

		n64_pro_weapons = length > 4 && !strcmp(name + length - 4, "_pro");
		snprintf(variant_name, sizeof(variant_name), "%.*s", n64_pro_weapons ? length - 4 : length, name);
		variant = *game_engine_get_variant_by_name(&temporary_variant, variant_name);
	}
	if (n64_choice.firefight)
		variant.universal_variant.score_to_win = 9999;
	else if (n64_choice.score > 0)
		variant.universal_variant.score_to_win = n64_choice.score;
	/* Native set zero admits every vehicle in the map palette. Defaults and
	the original Rocket Hogs mode retain the game type's existing filter. */
	if (!n64_choice.firefight &&
		(n64_choice.vehicle_set == N64_VEHICLES_ALL_MAP || n64_choice.vehicle_set == N64_VEHICLES_ALL_ROCKET))
		variant.universal_variant.vehicle_set = 0;
	game_set_game_variant(&variant);
	n64_particles_set_enhanced(n64_choice.enhanced_fx);
	n64_modifiers_set(!n64_choice.firefight && n64_choice.big_head, !n64_choice.firefight && n64_choice.paintball);
	n64_golden_reset();
	if (n64_choice.firefight)
	{
		n64_firefight_begin(n64_choice.difficulty, n64_choice.player_count);
		n64_bots_stop();
	}
	else
	{
		n64_firefight_stop();
		/* (up to 8, whatever the players: each biped ~2.7 ms of a tick) */
		n64_bots_begin(PIN(n64_choice.bots, n64_menu_game_bots(n64_choice.game_type) ? 1 : 0, 8), n64_choice.bot_skill,
			n64_menu_game_teams(n64_choice.game_type));
	}
	player_spawn_count = (short)n64_choice.player_count;
	snprintf(map_name, sizeof(map_name), "levels\\test\\%s\\%s", folder, folder);
	main_set_map_name(map_name);
	main_disallow_persistent_storage();
	{
		/* the game's controllers 0 to n-1 are the players' ports */
		extern int n64_gamepad_ports[4];
		int used = 0, i, port;

		for (i = 0; i < n64_choice.player_count; i++)
		{
			n64_gamepad_ports[i] = n64_choice.ports[i];
			used |= 1 << n64_choice.ports[i];
		}
		for (port = 0; port < 4 && i < 4; port++)
		{
			if (!(used & (1 << port)))
				n64_gamepad_ports[i++] = port;
		}
	}
	snprintf(line, sizeof(line), "n64: %s on %s, %d players, %d bots, score %d",
		n64_choice.firefight ? "firefight" : n64_menu_game_variant(n64_choice.game_type),
		map_name, n64_choice.player_count, n64_choice.firefight ? 0 : n64_choice.bots, n64_choice.score);
	n64_log(line);
}

void n64_game_setup(
	void)
{
	char name[32];
	char difficulty[8];
	int i;

	/* ai_debug_initialize's defaults (the AI is left out) */
	ai_debug.selected_actor_index = NONE;
	ai_debug.selected_squad_index = NONE;

	/* testing (port.json flag "debug_map=<map>", or "autotest"): straight
	into a two-player game; else main.c starts at the menu */
	name[0] = 0;
	if (!n64_debug_text("debug_map", name, sizeof(name)) && !n64_debug_flag("autotest"))
		return;
	n64_choice.map = n64_menu_map_from_folder(name[0] ? name : "wizard");
	if (n64_choice.map < 0)
		n64_choice.map = n64_menu_map_from_folder("wizard");
	n64_choice.game_type = 0;
	n64_choice.score = 0;
	/* (port.json flag "debug_firefight=<difficulty>": a Firefight game) */
	n64_choice.firefight = n64_debug_text("debug_firefight", difficulty, sizeof(difficulty)) != 0;
	n64_choice.difficulty = n64_choice.firefight ? atoi(difficulty) : 0;
	/* (port.json flag "debug_players=<1-4>": another count) */
	n64_choice.player_count = n64_debug_text("debug_players", name, sizeof(name)) ? PIN(atoi(name), 1, 4) :
		N64_TEST_PLAYER_COUNT;
	/* (port.json flags "debug_bots=<n>", "debug_bot_skill=<0-3>") */
	n64_choice.bots = n64_debug_text("debug_bots", name, sizeof(name)) ? PIN(atoi(name), 0, 12) : 0;
	n64_choice.bot_skill = n64_debug_text("debug_bot_skill", name, sizeof(name)) ? PIN(atoi(name), 0, 3) : 1;
	/* (port.json flag "debug_game_type=<n>": the menu's game type) */
	if (n64_debug_text("debug_game_type", name, sizeof(name)))
		n64_choice.game_type = atoi(name);
	if (n64_debug_text("debug_weapons", name, sizeof(name)))
#ifdef HALO_N64_PC_EXPANSION
		n64_choice.weapon_set = PIN(atoi(name), 0, N64_WEAPONS_PC_ADDITIVE);
#else
		n64_choice.weapon_set = PIN(atoi(name), 0, 3);
#endif
	if (n64_debug_text("debug_vehicles", name, sizeof(name)))
		n64_choice.vehicle_set = PIN(atoi(name), 0, N64_VEHICLES_COUNT - 1);
	n64_choice.big_head = n64_debug_flag("debug_big_head");
	n64_choice.paintball = n64_debug_flag("debug_paintball");
	if (n64_debug_flag("debug_golden_style"))
		for (i = 0; i < 4; i++) n64_controls[i].scheme = N64_SCHEME_GOLDEN_STYLE;
	/* (port.json flag "debug_high_detail": the 3D in high detail) */
	n64_render_set_high_detail(n64_debug_flag("debug_high_detail"));
	for (i = 0; i < 4; i++)
	{
		static short const colors[4] = {2, 3, 6, 5};

		n64_choice.ports[i] = i;
		n64_choice.teams[i] = i & 1;
		n64_choice.colors[i] = colors[i];
	}
	n64_game_start();
}

/* testing (port.json flag "debug_cycle=<seconds>"): after so many seconds
of a game, back to the menu, which starts the next map by itself
(n64_menu_run): every map in turn, through the menu as a player goes, the
maps' memory used and given back each time */
void n64_game_cycle_frame(
	void)
{
	static int seconds = -1;
	static unsigned long long start;
	char text[16];

	if (seconds < 0)
		seconds = n64_debug_text("debug_cycle", text, sizeof(text)) ? atoi(text) : 0;
	if (!seconds)
		return;
	if (!start)
		start = n64_microseconds();
	if (n64_microseconds() - start > (unsigned long long)seconds * 1000000ULL)
	{
		start = 0;
		main_goto_main_menu();
	}
}

/* ---------- the menu's games (main.c's main_menu_load, HALO_N64) */

/* the winner, then up to 12 players (4 on the controllers, 8 bots) */
#define N64_RESULT_LINES 13

static char n64_result_text[N64_RESULT_LINES][64];
static char const *n64_result_lines[N64_RESULT_LINES];
static int n64_result_count;

/* the pause menu's MAIN MENU chosen: no results, the menu's title */
static boolean n64_quit_to_title;

static void n64_ascii(
	char *out,
	int size,
	wchar_t const *text)
{
	int i;

	for (i = 0; i + 1 < size && text && text[i]; i++)
		out[i] = text[i] < 128 ? (char)(text[i] >= 'a' && text[i] <= 'z' ? text[i] - 32 : text[i]) : '?';
	out[i] = 0;
}

/* ---------- the scoreboard (the pause menu's, the results') */

/* a row ahead of another: in a team game the leading team's first (red
when tied), then by score, kills and fewest deaths */
static boolean n64_score_ahead(
	struct n64_score_row const *row,
	long score,
	struct n64_score_row const *other,
	long other_score,
	short first_team)
{
	if (row->team >= 0 && other->team >= 0 && row->team != other->team)
		return row->team == first_team;
	if (score != other_score)
		return score > other_score;
	if (row->kills != other->kills)
		return row->kills > other->kills;
	return row->deaths < other->deaths;
}

/* every player and bot of the game as the scoreboard's rows, sorted (and
their players, if asked) */
static int n64_score_rows(
	struct n64_score_row *rows,
	long *player_indices,
	int maximum)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long scores[N64_SCORE_ROWS];
	boolean teams;
	short first_team;
	int count = 0;

	if (!game_engine)
		return 0;
	teams = game_engine_has_teams();
	first_team = teams && game_engine->get_player_score &&
		game_engine_get_team_score(1) > game_engine_get_team_score(0) ? 1 : 0;
	if (maximum > N64_SCORE_ROWS)
		maximum = N64_SCORE_ROWS;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL && count < maximum)
	{
		long player_index = iterator.datum_index;
		struct n64_score_row row;
		wchar_t score_text[32];
		long score;
		int at;

		csmemset(&row, 0, sizeof(row));
		/* the players on the controllers by their port, as the menu
		names them; the bots by their names (n64_bots.c) */
		row.human = player->local_player_index >= 0 && player->local_player_index < 4;
		if (row.human)
			snprintf(row.name, sizeof(row.name), "PLAYER %d", n64_choice.ports[player->local_player_index] + 1);
		else
		{
			const char *full_name = n64_bots_display_name(player_index);
			if (full_name)
				snprintf(row.name, sizeof(row.name), "%s", full_name);
			else
				n64_ascii(row.name, sizeof(row.name), player->name);
		}
		csmemset(score_text, 0, sizeof(score_text));
		if (game_engine->format_player_score)
			game_engine->format_player_score(player_index, score_text);
		n64_ascii(row.score, sizeof(row.score), score_text);
		row.kills = player->statistics.kills[0];
		row.deaths = player->statistics.deaths;
		row.team = teams ? (signed char)(player->team_index & 1) : -1;
		score = game_engine->get_player_score ?
			game_engine->get_player_score(player_index, _get_score_individual) : row.kills;
		for (at = count; at > 0 && n64_score_ahead(&row, score, &rows[at - 1], scores[at - 1], first_team); at--)
		{
			rows[at] = rows[at - 1];
			scores[at] = scores[at - 1];
			if (player_indices)
				player_indices[at] = player_indices[at - 1];
		}
		rows[at] = row;
		scores[at] = score;
		if (player_indices)
			player_indices[at] = player_index;
		count++;
	}
	return count;
}

/* the game that just ended, for the menu: who won, then each player's
score, kills and deaths, the best first */
void n64_game_results(
	void)
{
	struct n64_score_row rows[N64_SCORE_ROWS];
	long player_indices[N64_SCORE_ROWS];
	char winner[48] = "";
	int count = 1, row_count, i;

	n64_result_count = 0;
	if (n64_quit_to_title)
	{
		/* (left from the pause menu: straight to the menu's title) */
		n64_quit_to_title = FALSE;
		n64_choice.to_title = TRUE;
		return;
	}
	if (n64_firefight_active())
	{
		n64_result_count = n64_firefight_results(n64_result_text, 6);
		for (count = 0; count < n64_result_count; count++)
			n64_result_lines[count] = n64_result_text[count];
		return;
	}
	if (!game_engine)
		return;
	row_count = n64_score_rows(rows, player_indices, N64_RESULT_LINES - 1);
	for (i = 0; i < row_count; i++)
	{
		snprintf(n64_result_text[count], sizeof(n64_result_text[count]), "%s   %s   (%d KILLS, %d DEATHS)",
			rows[i].name, rows[i].score, rows[i].kills, rows[i].deaths);
		n64_result_lines[count] = n64_result_text[count];
		count++;
		if (game_engine_did_player_win(player_indices[i]) == TRUE && !winner[0])
		{
			if (n64_menu_game_teams(n64_choice.game_type))
				snprintf(winner, sizeof(winner), "%s TEAM WINS", rows[i].team ? "BLUE" : "RED");
			else
				snprintf(winner, sizeof(winner), "%s WINS", rows[i].name);
		}
	}
	snprintf(n64_result_text[0], sizeof(n64_result_text[0]), "%s", winner[0] ? winner : "TIE GAME");
	n64_result_lines[0] = n64_result_text[0];
	n64_result_count = count;
}

/* ---------- the pause menu: START during a game stops its clock and shows
the scoreboard over it (n64_hud_pause), every player and bot; START (or B)
goes back to the game. Controls is selected first; left/right selects it
or Main Menu, A opens it. Every joined pad edits only its own Controls
panel; B returns to the scoreboard, START resumes. Meanwhile the
game's gamepads read as untouched, its sounds are silent and the Rumble
Paks still; the frames go on being drawn (main_loop draws a paused game) */

static boolean n64_paused;
static int n64_pause_controls, n64_pause_selected;
static int n64_pause_control_rows[4];
static signed char n64_pause_stick[4];
static unsigned long long n64_pause_repeat[4];

/* Identity is a physical controller port, never a combined button mask.
Only joined players can pause or edit their own saved controls. */
static int n64_pause_player(int port)
{
	int i;
	for (i = 0; i < n64_choice.player_count; i++)
		if (n64_choice.ports[i] == port) return i;
	return -1;
}

static unsigned short n64_pause_directions(int port)
{
	struct n64_pad pad;
	unsigned long long now = n64_microseconds();
	int direction;
	n64_input_get(port, &pad);
	direction = pad.stick_y > 50 ? 1 : pad.stick_y < -50 ? 2 :
		pad.stick_x < -50 ? 3 : pad.stick_x > 50 ? 4 : 0;
	if (!direction) { n64_pause_stick[port] = 0; return 0; }
	if (direction == n64_pause_stick[port] && now < n64_pause_repeat[port]) return 0;
	n64_pause_repeat[port] = now + (direction != n64_pause_stick[port] ? 330000 : 130000);
	n64_pause_stick[port] = direction;
	return direction == 1 ? N64_BUTTON_D_UP : direction == 2 ? N64_BUTTON_D_DOWN :
		direction == 3 ? N64_BUTTON_D_LEFT : N64_BUTTON_D_RIGHT;
}

static void n64_pause_controls_input(int port, unsigned short pressed)
{
	int *row, step;
	if (n64_pause_player(port) < 0) return;
	row = &n64_pause_control_rows[port];
	if (pressed & N64_BUTTON_D_UP) *row = (*row + CONTROL_COUNT - 1) % CONTROL_COUNT;
	else if (pressed & N64_BUTTON_D_DOWN) *row = (*row + 1) % CONTROL_COUNT;
	step = pressed & N64_BUTTON_D_LEFT ? -1 : pressed & (N64_BUTTON_D_RIGHT | N64_BUTTON_A) ? 1 : 0;
	if (step) n64_controls_step(port, *row, step,
		!!(pressed & N64_BUTTON_A) && !(pressed & (N64_BUTTON_D_LEFT | N64_BUTTON_D_RIGHT)));
}

static void n64_pause_end(
	void)
{
	n64_paused = FALSE;
	n64_pause_controls = 0;
	n64_controls_save();
	/* Aim is transient, but other players' crouch toggles survive a pause. */
	n64_golden_reset();
	{
		int p;
		for (p = 0; p < 4; p++)
		{
			struct n64_pad pad;
			n64_input_get(p, &pad);
			n64_controls[p].last_buttons = pad.buttons;
		}
	}
	game_time_set_paused(FALSE);
	n64_sound_pause(0);
}

/* render_frame_present's (n64_render_game.c), each frame before it is shown */
void n64_game_pause_frame(
	void)
{
	struct n64_score_row rows[N64_SCORE_ROWS];
	struct n64_pause_screen screen;
	char note[64];
	unsigned short pressed = 0, presses[4];
	int port;

	n64_input_poll();
	for (port = 0; port < 4; port++)
	{
		presses[port] = n64_input_take_pressed(port);
		if (n64_pause_player(port) >= 0) pressed |= presses[port];
	}
	if (!n64_paused)
	{
		/* (a game being played: not its end, the results coming) */
		boolean playing = game_engine_running() && n64_game_engine_postgame_state() == 0;

#ifdef HALO_N64_CAMPAIGN
		playing = playing || n64_campaign_active();
#endif
		if (!(pressed & N64_BUTTON_START) || n64_input_blocked || !game_in_progress() || !playing ||
			game_time_get() < 3)
		{
			return;
		}
		n64_paused = TRUE;
		game_time_set_paused(TRUE);
		n64_input_blocked = 1;
		n64_sound_pause(1);
		n64_pause_controls = 0;
		n64_pause_selected = 0;
		memset(n64_pause_stick, 0, sizeof(n64_pause_stick));
		pressed = 0;
		memset(presses, 0, sizeof(presses));
	}
	else if ((pressed & N64_BUTTON_START) || (!n64_pause_controls && (pressed & N64_BUTTON_B)))
	{
		n64_pause_end();
		n64_input_blocked = 0;
		return;
	}
	else if (n64_pause_controls)
	{
		if (pressed & N64_BUTTON_B)
		{
			n64_controls_save();
			n64_pause_controls = 0;
		}
		else
			for (port = 0; port < 4; port++)
				n64_pause_controls_input(port, presses[port] | n64_pause_directions(port));
	}
	else
	{
		for (port = 0; port < 4; port++)
		{
			unsigned short buttons;
			if (n64_pause_player(port) < 0) continue;
			buttons = presses[port] | n64_pause_directions(port);
			if (buttons & (N64_BUTTON_D_LEFT | N64_BUTTON_D_UP)) n64_pause_selected = 0;
			else if (buttons & (N64_BUTTON_D_RIGHT | N64_BUTTON_D_DOWN)) n64_pause_selected = 1;
			if (!(buttons & N64_BUTTON_A)) continue;
			if (!n64_pause_selected) n64_pause_controls = 1;
			else
			{
				n64_pause_end();
				n64_quit_to_title = TRUE;
				main_goto_main_menu();
				return;
			}
			break;
		}
	}
	for (port = 0; port < 4; port++)
		n64_input_rumble(port, 0.f, 0.f);
	if (n64_pause_controls)
	{
		n64_hud_pause_controls(n64_choice.ports, n64_choice.player_count, n64_pause_control_rows);
		return;
	}

	screen.title = "PAUSED";
	screen.note = NULL;
	if (n64_firefight_status(note, sizeof(note)))
		screen.note = note;
#ifdef HALO_N64_CAMPAIGN
	else if (n64_campaign_status(note, sizeof(note)))
		screen.note = note;
#endif
	else if (game_engine && game_engine_has_teams() && game_engine->get_player_score)
	{
		long red = game_engine_get_team_score(0), blue = game_engine_get_team_score(1);
		long engine = game_engine_get_variant()->game_engine_index;

		/* (kills and flags as numbers; times and laps only who leads) */
		if (engine == game_engine_slayer || engine == game_engine_ctf)
			snprintf(note, sizeof(note), "RED %ld   BLUE %ld", red, blue);
		else
			snprintf(note, sizeof(note), "%s", red == blue ? "TEAMS TIED" : red > blue ? "RED TEAM LEADS" : "BLUE TEAM LEADS");
		screen.note = note;
	}
	else
	{
		snprintf(note, sizeof(note), "%s", n64_menu_map_name(n64_choice.map));
		screen.note = note;
	}
	screen.rows = rows;
	screen.row_count = n64_score_rows(rows, NULL, N64_SCORE_ROWS);
	screen.selected = n64_pause_selected;
	n64_hud_pause(&screen);
}

/* the menu, then the game it chose */
void n64_game_menu(
	void)
{
	int count = n64_result_count;

	n64_result_count = 0;
	n64_menu_run(&n64_choice, n64_result_lines, count);
	n64_game_start();
	/* (nothing of the last game's pause carried into this one) */
	n64_paused = FALSE;
	n64_quit_to_title = FALSE;
	n64_input_blocked = 0;
	n64_input_clear_pressed();
	n64_sound_pause(0);
}

/* game_engine_postspawn_player_update's (game_engine.c, HALO_N64): in a
Pro variant a new unit gets a pistol (in hand) and an assault rifle instead
of the map's starting weapons; FALSE otherwise */
boolean n64_game_pro_weapons(
    long unit_index)
{
	static char const *const loadouts[3][2] = {
		{ "weapons\\pistol\\pistol", "weapons\\assault rifle\\assault rifle" },
		{ "weapons\\plasma_cannon\\plasma_cannon", "weapons\\pistol\\pistol" },
		{ "weapons\\flamethrower\\flamethrower", "weapons\\pistol\\pistol" }
	};
	/* Optional world pickups use MAP DEFAULT's exact loadout policy,
	including its Pistol + AR override for Pro game variants. */
	int weapon_set =
#ifdef HALO_N64_PC_EXPANSION
		n64_choice.weapon_set == N64_WEAPONS_PC_ADDITIVE ? 0 :
#endif
		n64_choice.weapon_set;
	char const *const *names = loadouts[weapon_set > 1 ? PIN(weapon_set - 1, 0, 2) : 0];
	boolean first = TRUE;
	short i;
	char diagnostic[160];

	if ((!n64_pro_weapons && !weapon_set) || unit_index == NONE || n64_choice.firefight)
		return FALSE;
	/* Refuse a partial loadout: keep the map's starting weapons instead. */
	if (tag_loaded('weap', names[0]) == NONE || tag_loaded('weap', names[1]) == NONE)
	{
		snprintf(diagnostic, sizeof(diagnostic), "n64: loadout %d missing tags %lx %lx", n64_choice.weapon_set, tag_loaded('weap', names[0]), tag_loaded('weap', names[1]));
		n64_log(diagnostic);
		return FALSE;
	}
	for (i = 0; i < 2; i++)
	{
		long definition_index = tag_loaded('weap', names[i]);
		struct object_placement_data placement_data;
		long weapon_index;

		if (definition_index == NONE)
			continue;
		object_placement_data_new(&placement_data, definition_index, NONE);
		weapon_index = object_new(&placement_data);
		if (weapon_index != NONE)
		{
			boolean added = unit_add_weapon_to_inventory(unit_index, weapon_index, first ? 2 : 0);
			if (n64_debug_flag("debug_weapon_showcase"))
			{
				snprintf(diagnostic, sizeof(diagnostic), "n64: loadout %d %s -> %s added %d", n64_choice.weapon_set, names[i], tag_get_name(object_get(weapon_index)->definition_index), added);
				n64_log(diagnostic);
			}
			if (added) first = FALSE;
			else object_delete(weapon_index);
		}
	}
	return !first;
}

long n64_game_vehicle_override(long definition_index)
{
	if (definition_index != NONE && !n64_choice.firefight &&
		(n64_choice.vehicle_set == N64_VEHICLES_ROCKET_HOGS || n64_choice.vehicle_set == N64_VEHICLES_ALL_ROCKET))
	{
		char const *name = tag_get_name(definition_index);
		if (name && !strncmp(name, "vehicles\\warthog\\", 17))
		{
			long rocket = tag_loaded('vehi', "vehicles\\rwarthog\\rwarthog");
			if (rocket != NONE) return rocket;
		}
	}
	return definition_index;
}

/* main.c's create_local_players: the index-th player's controller port,
Halo colour index and team */
void n64_game_local_player(
	long index,
	short *port,
	short *color,
	short *team)
{
	index = PIN(index, 0, 3);
	*port = (short)n64_choice.ports[index];
	*color = (short)n64_choice.colors[index];
	*team = (short)(n64_choice.firefight ? 0 :
		n64_menu_game_teams(n64_choice.game_type) ? n64_choice.teams[index] : index);
#ifdef HALO_N64_CAMPAIGN
	if (n64_choice.campaign)
		*team = 1; /* _game_team_player: n64_campaign.c's Marines */
#endif
}

/* ---------- memory map */

static struct
{
	void *game_state;
	void *tag_cache;
	/* the game state's buffer, its GPU part (n64_game_state_trim) */
	unsigned long game_state_size, game_state_gpu_size;
} n64_memory;

void physical_memory_allocate(
	void)
{
	/* game_state.c allocates its own buffer (game_state_allocate_buffer) */
	return;
}

void physical_memory_verify(
	void)
{
	return;
}

void physical_memory_free(
	void)
{
	return;
}

void *physical_memory_get_game_state_base_address(
	void)
{
	return n64_memory.game_state;
}

void *physical_memory_get_tag_cache_base_address(
	void)
{
	return n64_memory.tag_cache;
}

void *physical_memory_get_texture_cache_base_address(
	void)
{
	return NULL;
}

void *physical_memory_get_sound_cache_base_address(
	void)
{
	return NULL;
}

/* ---------- the game state's buffer */

void *game_state_allocate_buffer(
	unsigned long address,
	unsigned long cpu_size,
	unsigned long gpu_size)
{
	(void)address;
	n64_memory.game_state = memalign(16, cpu_size + gpu_size);
	if (!n64_memory.game_state)
		n64_fatal("no memory for the game state");
	memset(n64_memory.game_state, 0, cpu_size + gpu_size);
	n64_memory.game_state_size = cpu_size + gpu_size;
	n64_memory.game_state_gpu_size = gpu_size;
	return n64_memory.game_state;
}

/* The buffer is HALO_PORT_GAME_STATE_CPU_SIZE, a ceiling; the game's
allocations are all made before the first map locks it (game_state.c,
HALO_N64), and the rest goes back to the heap here, once: the allocations
are made from the buffer's start (the GPU's part, from its end, is none on
the N64), and shrinking a block keeps it where it is. */
#define N64_GAME_STATE_SLACK 256

void n64_game_state_trim(
	unsigned long used)
{
	static boolean trimmed = FALSE;
	unsigned long size, before;
	void *block;
	char line[128];

	if (trimmed || !n64_memory.game_state || n64_memory.game_state_gpu_size)
		return;
	trimmed = TRUE;
	size = (used + N64_GAME_STATE_SLACK + 15) & ~15UL;
	if (size >= n64_memory.game_state_size)
		return;
	before = n64_heap_free();
	/* (libc's: the buffer is memalign's, and in the game's units realloc is
	cseries.h's debug_realloc) */
	block = (realloc)(n64_memory.game_state, size);
	if (block != n64_memory.game_state)
		n64_fatal("the game state moved when it was trimmed");
	snprintf(line, sizeof(line), "n64: game state %lu KB of %lu KB used, %lu KB given back to the heap",
		used / 1024, n64_memory.game_state_size / 1024, (n64_heap_free() - before) / 1024);
	n64_log(line);
	n64_memory.game_state_size = size;
	return;
}

void game_state_free_buffer(
	void)
{
	return;
}

void game_state_create_or_open_file(void) { return; }
void game_state_close_file(void) { return; }
boolean game_state_write_core(char const *name, void *buffer, long size) { (void)name; (void)buffer; (void)size; return FALSE; }
boolean game_state_read_core_header(char const *name, void *header, long size) { (void)name; (void)header; (void)size; return FALSE; }
boolean game_state_write_to_file(void) { return FALSE; }
void game_state_read_core(char const *name, void *buffer, long size) { (void)name; (void)buffer; (void)size; return; }

/* ---------- maps

An N64 map (tools/n64/convert.py): a 64-byte header, the image, then the
image offsets of its pointers. The image is copied into one allocation and
every pointer moved to it. */

struct n64_map_header
{
	char magic[4];
	unsigned long version;
	char name[32];
	unsigned long image_size;
	unsigned long relocation_count;
	unsigned long tag_header_offset;
	unsigned long structure_bsp_header_offset;
	unsigned long reserved[2];
};

#define N64_MAP_VERSION 2
#ifdef HALO_N64_CAMPAIGN
#define N64_MAP_FORMAT_KIND 0x43503136UL /* CP16: campaign compact collision */
#else
#define N64_MAP_FORMAT_KIND 0x4D503332UL /* MP32: multiplayer collision */
#endif

typedef char n64_map_header_size_assert[sizeof(struct n64_map_header) == 64 ? 1 : -1];
typedef char n64_map_header_kind_offset_assert[offsetof(struct n64_map_header, reserved) == 56 ? 1 : -1];

static boolean n64_map_header_compatible(struct n64_map_header const *header)
{
	return !memcmp(header->magic, "H64M", 4) && header->version == N64_MAP_VERSION &&
		header->reserved[0] == N64_MAP_FORMAT_KIND && header->reserved[1] == 0;
}

static struct
{
	char name[32];
	byte *image;
	unsigned long image_size;
	struct n64_map_header header;
	/* the images' home (n64_map_home_reserve) */
	byte *home;
	unsigned long home_size;
} n64_map;

/* The maps' images have a home of their own, made at boot (n64_main.c) from
the heap's bottom, the size of the largest map the menu offers: each image
goes in it, and nothing else does but display buffers lent while it has
room (n64_map_home_free_part, n64_render.c), so that no block the menu or a
game leaves behind -- whatever made it -- can split the memory an image
wants in one piece. After the menu, Sidewinder and Blood Gulch found none:
the list of libdragon's display buffers (40 bytes, made when the game's
display came back), and 44 small blocks at the top of what the menu had
used, stood in the middle of 3.9 MB free. */
void n64_map_home_reserve(
	void)
{
	unsigned long largest = 0;
	int count = n64_menu_map_count(), i;
	char line[64];

	for (i = 0; i < count; i++)
	{
		struct n64_map_header header;
		char path[64];
		long file;

		snprintf(path, sizeof(path), "rom:/maps/%s.map", n64_menu_map_folder(i));
		file = n64_file_open(path);
		if (file < 0)
			continue;
		if (n64_file_read(file, &header, sizeof(header)) == (long)sizeof(header) &&
			n64_map_header_compatible(&header) && header.image_size > largest)
			largest = header.image_size;
		n64_file_close(file);
	}
	if (!largest)
		return;
	largest = (largest + 63) & ~63UL;
	/* (libc's: in the game's units malloc is cseries.h's) */
	n64_map.home = memalign(64, largest);
	if (!n64_map.home)
		return;
	n64_map.home_size = largest;
	n64_display_set_home(n64_map.home, largest);
	snprintf(line, sizeof(line), "n64: the maps' home, %lu KB", largest / 1024);
	n64_log(line);
}

/* what of the home no map's image is in (all of it between games), for the
display to borrow (n64_render.c); NULL if there is no home */
void *n64_map_home_free_part(
	unsigned long *size)
{
	unsigned long used;

	*size = 0;
	if (!n64_map.home)
		return NULL;
	used = n64_map.image == n64_map.home ? (n64_map.image_size + 63) & ~63UL : 0;
	if (used >= n64_map.home_size)
		return NULL;
	*size = n64_map.home_size - used;
	return n64_map.home + used;
}

void n64_map_boot_complete(void)
{
	/* Persistent pools were allocated above the largest arena. Release its
	unused tail for the title; all transient users leave before it grows. */
	unsigned long size = 3 * 1024 * 1024;
	if (n64_map.home && n64_map.home_size > size)
	{
		void *home = (realloc)(n64_map.home, size);
		if (home != n64_map.home) n64_fatal("map arena moved while shrinking");
		n64_map.home_size = size;
		n64_display_set_home(home, size);
	}
}

static void *n64_map_load(
	char const *name)
{
	char path[64];
	long file;
	unsigned long remaining, offset;
	unsigned long *relocations;
	unsigned long base;

	if (n64_map.image && !strcmp(n64_map.name, name))
		return n64_map.image + n64_map.header.tag_header_offset;
	if (n64_map.image)
	{
		/* (libc's: the image is memalign's, and in the game's units free is
		cseries.h's debug_free; the home stays) */
		if (n64_map.image != n64_map.home)
			(free)(n64_map.image);
		n64_map.image = NULL;
	}

	snprintf(path, sizeof(path), "rom:/maps/%s.map", name);
	file = n64_file_open(path);
	if (file < 0)
	{
		error(_error_silent, "n64: no map %s", path);
		return NULL;
	}
	if (n64_file_read(file, &n64_map.header, sizeof(n64_map.header)) != sizeof(n64_map.header))
		n64_fatal("could not read map header");
	if (!n64_map_header_compatible(&n64_map.header))
		n64_fatal("the map was converted for another build (tools/n64/convert.py)");
	/* Free the decoder and every transient render allocation before growing
	the arena back into its original reserved span. */
	n64_file_close(file);
	n64_render_prepare_map_image();
	/* The reclaimable RSP reserve may sit in the arena's released tail. */
	n64_uncached_reserve_release();
	unsigned long arena_size = MAX(n64_map.header.image_size, 3 * 1024 * 1024);
	arena_size = (arena_size + 63) & ~63UL;
	if (n64_map.home_size != arena_size)
	{
		void *home = (realloc)(n64_map.home, arena_size);
		if (!home)
		{
			char pieces[200], message[320];
			unsigned long largest = n64_heap_describe(pieces, sizeof(pieces));
			n64_heap_dump("map resize", n64_map.home + n64_map.home_size);
			snprintf(message, sizeof(message), "no memory to resize map arena: %lu -> %lu KB, %lu KB free, largest %lu KB; %s",
				n64_map.home_size / 1024, arena_size / 1024, n64_heap_free() / 1024, largest / 1024, pieces);
			n64_fatal(message);
		}
		n64_map.home = home; n64_map.home_size = arena_size;
		n64_display_set_home(home, arena_size);
	}
	n64_uncached_reserve_take();
	file = n64_file_open(path);
	struct n64_map_header reopened_header;
	if (file < 0 || n64_file_read(file, &reopened_header, sizeof(reopened_header)) != sizeof(reopened_header))
		n64_fatal("could not reopen map stream");
	if (!n64_map_header_compatible(&reopened_header) ||
		memcmp(&reopened_header, &n64_map.header, sizeof(reopened_header)))
		n64_fatal("map header changed while reopening stream");

	/* in the home (if it is not lent to the display), else from the heap */
	if (n64_map.home && n64_map.header.image_size <= n64_map.home_size && !n64_render_home_in_use())
		n64_map.image = n64_map.home;
	else
		n64_map.image = memalign(16, n64_map.header.image_size);
	if (!n64_map.image)
	{
		/* how much there was, the largest piece, and what split the rest
		(the heap's map: n64_heap_describe). A probe by halving said "in
		one piece" of nearly all of it: the compiler drops a malloc that is
		freed unused, and took every probe for a success */
		static char text[320];
		char pieces[200];
		unsigned long largest = n64_heap_describe(pieces, sizeof(pieces));

		snprintf(text, sizeof(text), "no memory for the map: %lu KB needed, %lu KB free, %lu KB in one piece. "
			"Free pieces: %s", n64_map.header.image_size / 1024, n64_heap_free() / 1024, largest / 1024, pieces);
		n64_fatal(text);
	}
	n64_map.image_size = n64_map.header.image_size;
	if (n64_file_read(file, n64_map.image, n64_map.header.image_size) != n64_map.header.image_size)
		n64_fatal("could not read map image");

	/* the relocations follow the image, 8-byte aligned */
	offset = (sizeof(n64_map.header) + n64_map.header.image_size + 7) & ~7UL;
	unsigned long padding = offset - sizeof(n64_map.header) - n64_map.header.image_size;
	byte padding_bytes[8];
	if (padding && n64_file_read(file, padding_bytes, padding) != padding)
		n64_fatal("could not read map padding");
	base = (unsigned long)n64_map.image;
	relocations = malloc(4096);
	if (!relocations)
		n64_fatal("no memory for map relocations");
	remaining = n64_map.header.relocation_count;
	while (remaining)
	{
		unsigned long count = remaining < 1024 ? remaining : 1024;
		unsigned long i;

		if (n64_file_read(file, relocations, count * 4) != count * 4)
			n64_fatal("could not read map relocations");
		for (i = 0; i < count; i++)
			*(unsigned long *)(n64_map.image + relocations[i]) += base;
		remaining -= count;
	}
	free(relocations);
	n64_file_close(file);
	n64_render_finish_map_image();

	csstrncpy(n64_map.name, name, sizeof(n64_map.name) - 1);
	n64_animation_open(name);
	n64_memory.tag_cache = n64_map.image;
	{
		char line[128];

		snprintf(line, sizeof(line), "n64: map %s loaded, %lu KB, %lu relocations, %lu KB of heap left",
			name, n64_map.header.image_size / 1024, n64_map.header.relocation_count, n64_heap_free() / 1024);
		n64_log(line);
	}
	return n64_map.image + n64_map.header.tag_header_offset;
}

/* main.c's main_menu_load: the map of the game that ended, once unloaded
(game_unload), gives its memory to the menu */
void n64_game_free_map(
	void)
{
	/* what the HUD and the effects made for the map, with it (else it stays
	until the next map's is made, above where this one was, and splits the
	memory the next image wants in one piece) */
	n64_hud_release();
	n64_particles_release();
#ifdef HALO_N64_CAMPAIGN
	n64_campaign_stop();
#endif
	/* and the level and its models: with the image in the maps' home,
	freeing it gives the heap nothing, and the menu (its fonts, its logo:
	menu_load) found the old level still there -- out of memory after
	Sidewinder */
	n64_render_load_scene(NULL);
	if (n64_map.image)
	{
		/* (libc's free: see n64_map_load; the home stays) */
		if (n64_map.image != n64_map.home)
			(free)(n64_map.image);
		n64_map.image = NULL;
	}
	n64_map.name[0] = 0;
	n64_memory.tag_cache = NULL;
	n64_uncached_reserve_settle();
	{
		char line[64];

		snprintf(line, sizeof(line), "n64: map freed, %lu KB of heap free", n64_heap_free() / 1024);
		n64_log(line);
	}
}

/* the loaded map's name ("wizard"), for the renderer's level geometry */
char const *n64_loaded_map_name(
	void)
{
	return n64_map.name;
}

/* cache_files.c (HALO_N64): the loaded map's tag header */
void *n64_cache_file_tag_header(
	char const *scenario_name)
{
	return n64_map_load(tag_name_strip_path(scenario_name));
}

/* ---------- the cache files' platform half (cache_files_windows.c) */

void cache_files_initialize(void) { return; }
void cache_files_dispose(void) { return; }
void cache_files_precache_set_priority(boolean blocking) { (void)blocking; return; }
boolean cache_files_precache_in_progress(void) { return FALSE; }
boolean cache_files_precache_is_copying_map(const char *map_name) { (void)map_name; return FALSE; }
boolean cache_files_precache_map_loaded(const char *map_name) { (void)map_name; return TRUE; }
boolean cache_files_precache_map_begin(const char *map_name, boolean copy_map) { (void)map_name; (void)copy_map; return TRUE; }
void cache_files_precache_map_end(void) { return; }
void cache_files_precache_map_queue_end(void) { return; }
short cache_files_precache_map_status(real *progress) { if (progress) *progress = 1.f; return 1; }
void cache_file_promote_read(short request) { (void)request; return; }
void cache_file_block_until_not_busy(void) { return; }
void cache_file_close(void) { return; }

boolean cache_file_open(
	char const *map_name,
	struct cache_file_header *header)
{
	(void)map_name;
	(void)header;
	return TRUE;
}

short cache_file_read(
	long tag_index,
	long offset,
	long size,
	void *buffer,
	boolean *completion_flag_reference,
	boolean blocking)
{
	(void)tag_index;
	(void)offset;
	(void)size;
	(void)buffer;
	(void)blocking;
	if (completion_flag_reference)
		*completion_flag_reference = TRUE;
	return NONE;
}

void tags_header_register_vertex_and_index_buffers(struct cache_file_tag_header *header) { (void)header; return; }
void tags_header_deregister_vertex_and_index_buffers(struct cache_file_tag_header *header) { (void)header; return; }
void structure_bsp_header_register_vertex_buffers(struct cache_file_structure_bsp_header *header) { (void)header; return; }
void structure_bsp_header_deregister_vertex_buffers(struct cache_file_structure_bsp_header *header) { (void)header; return; }

/* ---------- the Linux port's hooks into the game */

int halo_interpolation_enabled(void) { return 0; }
long halo_screen_width(void) { return 640; }
int halo_linux_mouse_look(short gamepad_index, float *yaw, float *pitch) { (void)gamepad_index; (void)yaw; (void)pitch; return 0; }
void network_lobby_update(void) { return; }
void render_interpolation_tick(void) { return; }
void render_interpolation_frame_begin(void) { return; }
void render_interpolation_frame_end(void) { return; }
void render_interpolation_dispose_from_old_map(void) { return; }
float render_interpolation_fraction(void) { return 1.f; }
struct real_matrix4x3 *render_interpolation_object_node_matrices(long object_index) { (void)object_index; return NULL; }
struct observer_result const *render_interpolation_camera(short local_player_index, struct observer_result const *observer) { (void)local_player_index; return observer; }
void render_interpolation_first_person(short local_player_index, struct real_matrix4x3 *node_matrices,
	short node_count, struct render_camera const *camera) { (void)local_player_index; (void)node_matrices; (void)node_count; (void)camera; return; }
float render_interpolation_game_time_sec(long ticks) { return (float)ticks / 30.f; }
float halo_pixel_aspect(void) { return 1.f; }
float halo_screen_texture_scale_x(void) { return 1.f; }
int halo_performance_mode(void) { return 1; }
long halo_screen_commit(void) { return 640; }
void halo_screen_ui_offset(unsigned char centered) { (void)centered; return; }
void p2p_lobby_report(const void *data, long size) { (void)data; (void)size; return; }
long p2p_peer_round_trip(unsigned long address) { (void)address; return -1; }
void p2p_accept_remote_connections(int accept) { (void)accept; return; }

/* ---------- the stack walk (cseries/stack_walk_windows.c walks x86 frames) */

void stack_walk(short levels_to_ignore) { (void)levels_to_ignore; n64_backtrace(); }
void stack_walk_initialize(void) { return; }
void stack_walk_dispose(void) { return; }
void stack_walk_disregard_symbol_names(boolean disregard) { (void)disregard; return; }

/* ---------- the tick's cost (game_time.c calls n64_game_tick, HALO_N64)

Every game tick is timed; every 150 ticks (5 s of game time) the log gets
the average and the worst, and how far game time keeps up with real time. */

void game_tick(void);

/* game.c's phases of a tick (HALO_N64), in CPU ticks, summed over the log
window */
unsigned long n64_tick_phases[8];
/* objects.c's objects_update, by object type (HALO_N64) */
unsigned long n64_object_type_time[16], n64_object_type_count[16];
/* objects.c's object_update, by stage */
unsigned long n64_object_stage_time[8], n64_equipment_stage_time[8];
/* the object type whose stages are logged apart (port.json flag
"debug_stage_type=<type>": 0 biped, 1 vehicle, 2 weapon, 3 equipment,
6 scenery, 7 machine) */
int n64_stage_type = 7;
/* (collisions.c, "debug_ray_callers": each caller of collision_test_vector
and how many rays it cast, those while the HUD drew in the top 16 bits) */
unsigned long n64_ray_callers[16][2];
/* (object_types.c, bipeds.c: the logged type's update by part -- object,
unit, biped... -- and bipeds' moving) */
unsigned long n64_part_time[8];
static char const *const n64_object_stage_names[5] =
{
	"type update", "damage/export", "node matrices", "functions/colours", "lights",
};
static char const *const n64_object_type_names[12] =
{
	"biped", "vehicle", "weapon", "equipment", "garbage", "projectile", "scenery", "machine", "control",
	"light fixture", "placeholder", "sound scenery",
};
static char const *const n64_tick_phase_names[8] =
{
	"quit/allegiance", "units", "players before", "effects", "first person", "engine", "objects", "players after/hud",
};

/* one line per player: where, whether alive, kills, deaths, shots */
static void n64_log_players(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		char line[160];

		if (player->unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);
			long weapon_index = unit->unit.current_weapon_index >= 0 ?
				unit->unit.weapon_object_indices[unit->unit.current_weapon_index] : NONE;
			char const *weapon_name = "none";
			int rounds = -1;

			if (weapon_index != NONE)
			{
				struct weapon_datum *weapon = weapon_get(weapon_index);

				weapon_name = tag_name_strip_path(tag_get_name(object_get(weapon_index)->definition_index));
				rounds = weapon->weapon.magazines[0].rounds_loaded;
			}
			snprintf(line, sizeof(line), "player %d: alive at %.1f %.1f %.1f, %s (%d rounds), kills %d, deaths %d, colour %.2f %.2f %.2f / base %.2f %.2f %.2f",
				(int)iterator.absolute_index,
				unit->object.position.x, unit->object.position.y, unit->object.position.z,
				weapon_name, rounds, player->statistics.kills[0], player->statistics.deaths,
				unit->object.outgoing_change_colors[0].red, unit->object.outgoing_change_colors[0].green,
				unit->object.outgoing_change_colors[0].blue, unit->object.base_change_colors[0].red,
				unit->object.base_change_colors[0].green, unit->object.base_change_colors[0].blue);
		}
		else
		{
			snprintf(line, sizeof(line), "player %d: dead (respawn in %ld), kills %d, deaths %d",
				(int)iterator.absolute_index, player->respawn_timer,
				player->statistics.kills[0], player->statistics.deaths);
		}
		n64_log(line);
	}
}

static struct
{
	unsigned long count;
	unsigned long long total;
	unsigned long worst;
	unsigned long long window_start;
} n64_tick_profile;

/* testing (port.json flag "debug_game_ticks=<n>"): each game ends after n
ticks (its scores, then the menu) */
static long n64_game_end_tick = -1;

void n64_game_tick(
	void)
{
	unsigned long start = n64_ticks();
	unsigned long elapsed;

	if (!n64_tick_profile.window_start)
	{
		char setting[8];

		n64_tick_profile.window_start = n64_ticks64();
		if (n64_debug_text("debug_stage_type", setting, sizeof(setting)))
			n64_stage_type = atoi(setting);
		if (n64_debug_text("debug_game_ticks", setting, sizeof(setting)))
			n64_game_end_tick = atol(setting);
	}
	if (n64_game_end_tick > 0 && game_time_get() == n64_game_end_tick)
		game_engine_end_game();
	n64_profile_zone = n64_debug_flag("profile_tick");
	n64_firefight_tick();
#ifdef HALO_N64_CAMPAIGN
	n64_campaign_tick();
#endif
	game_tick();
	n64_profile_zone = 0;
	/* (the audio fed between ticks too: a slow frame runs several) */
	n64_sound_poll();
	elapsed = n64_ticks() - start;
	{
		extern unsigned long n64_last_tick_us;

		n64_last_tick_us = (unsigned long)((unsigned long long)elapsed * 1000000 / N64_TICKS_PER_SECOND);
	}
	n64_tick_profile.total += elapsed;
	if (elapsed > n64_tick_profile.worst)
		n64_tick_profile.worst = elapsed;
	if (++n64_tick_profile.count == 150)
	{
		char line[160];
		unsigned long long now = n64_ticks64();
		unsigned long average_us = (unsigned long)(n64_tick_profile.total * 1000000 / N64_TICKS_PER_SECOND / 150);
		unsigned long worst_us = (unsigned long)((unsigned long long)n64_tick_profile.worst * 1000000 / N64_TICKS_PER_SECOND);
		unsigned long window_ms = (unsigned long)((now - n64_tick_profile.window_start) * 1000 / N64_TICKS_PER_SECOND);

		snprintf(line, sizeof(line), "tick: %lu.%02lu ms average, %lu.%02lu ms worst; 150 ticks (5 s of game) took %lu ms",
			average_us / 1000, (average_us % 1000) / 10, worst_us / 1000, (worst_us % 1000) / 10, window_ms);
		n64_log(line);
		{
			char phases[320];
			int length = 0;
			int i;

			for (i = 0; i < 8; i++)
			{
				length += snprintf(phases + length, sizeof(phases) - length, "%s %lu.%lu ", n64_tick_phase_names[i],
					(unsigned long)((unsigned long long)n64_tick_phases[i] * 1000 / N64_TICKS_PER_SECOND / 150),
					(unsigned long)((unsigned long long)n64_tick_phases[i] * 10000 / N64_TICKS_PER_SECOND / 150 % 10));
				n64_tick_phases[i] = 0;
			}
			n64_log(phases);
			if (n64_debug_flag("debug_ray_callers"))
			{
				for (i = 0; i < 16 && n64_ray_callers[i][0]; i++)
				{
					snprintf(phases, sizeof(phases), "rays from %08lx: %lu (%lu in the HUD)", n64_ray_callers[i][0],
						n64_ray_callers[i][1] & 0xFFFF, n64_ray_callers[i][1] >> 16);
					n64_log(phases);
					n64_ray_callers[i][1] = 0;
				}
			}
			length = 0;
			for (i = 0; i < 12; i++)
			{
				if (!n64_object_type_count[i])
					continue;
				length += snprintf(phases + length, sizeof(phases) - length, "%s %lu.%02lu ms x%lu ", n64_object_type_names[i],
					(unsigned long)((unsigned long long)n64_object_type_time[i] * 1000 / N64_TICKS_PER_SECOND / 150),
					(unsigned long)((unsigned long long)n64_object_type_time[i] * 100000 / N64_TICKS_PER_SECOND / 150 % 100),
					n64_object_type_count[i] / 150);
			}
			for (i = 0; i < 16; i++)
				n64_object_type_time[i] = n64_object_type_count[i] = 0;
			n64_log(phases);
			length = 0;
			for (i = 0; i < 5; i++)
			{
				length += snprintf(phases + length, sizeof(phases) - length, "%s %lu.%02lu ", n64_object_stage_names[i],
					(unsigned long)((unsigned long long)n64_object_stage_time[i] * 1000 / N64_TICKS_PER_SECOND / 150),
					(unsigned long)((unsigned long long)n64_object_stage_time[i] * 100000 / N64_TICKS_PER_SECOND / 150 % 100));
				n64_object_stage_time[i] = 0;
			}
			n64_log(phases);
			{
				extern unsigned long n64_unmoved_reasons[4];

				length = snprintf(phases, sizeof(phases), "type %d (not still %lu, overlays %lu, moved %lu, skipped %lu): ",
					n64_stage_type, n64_unmoved_reasons[0], n64_unmoved_reasons[1], n64_unmoved_reasons[2],
					n64_unmoved_reasons[3]);
				n64_unmoved_reasons[0] = n64_unmoved_reasons[1] = n64_unmoved_reasons[2] = n64_unmoved_reasons[3] = 0;
			}
			for (i = 0; i < 5; i++)
			{
				length += snprintf(phases + length, sizeof(phases) - length, "part%d %lu.%02lu ", i,
					(unsigned long)((unsigned long long)n64_part_time[i] * 1000 / N64_TICKS_PER_SECOND / 150),
					(unsigned long)((unsigned long long)n64_part_time[i] * 100000 / N64_TICKS_PER_SECOND / 150 % 100));
				n64_part_time[i] = 0;
			}
			for (i = 0; i < 5; i++)
			{
				length += snprintf(phases + length, sizeof(phases) - length, "%s %lu.%02lu ", n64_object_stage_names[i],
					(unsigned long)((unsigned long long)n64_equipment_stage_time[i] * 1000 / N64_TICKS_PER_SECOND / 150),
					(unsigned long)((unsigned long long)n64_equipment_stage_time[i] * 100000 / N64_TICKS_PER_SECOND / 150 % 100));
				n64_equipment_stage_time[i] = 0;
			}
			snprintf(phases + length, sizeof(phases) - length, "\n");
			n64_log(phases);
		}
		n64_log_players();
		memset(&n64_tick_profile, 0, sizeof(n64_tick_profile));
		n64_tick_profile.window_start = now;
	}
}

/* ---------- player colours (saved games/player_profile.c is left out) */

static long const n64_profile_color_table[18] =
{
	0x00FFFFFF, 0x00000000, 0x00FE0000, 0x000201E3, 0x00707E71, 0x00FFFF01,
	0x0000FF01, 0x00FF56B9, 0x00AB10F4, 0x0001FFFF, 0x006493ED, 0x00FF7F00,
	0x001ECC91, 0x00006401, 0x00603814, 0x00C69C6C, 0x009D0B0E, 0x00F5999E,
};

real_rgb_color player_profile_get_rgb_color(
	long color_index)
{
	real_rgb_color rgb_color;
	long color;

	color_index = PIN(color_index, 0, NUMBEROF(n64_profile_color_table) - 1);
	color = n64_profile_color_table[color_index];
	rgb_color.red = ((color >> 16) & 0xFF) / 255.f;
	rgb_color.green = ((color >> 8) & 0xFF) / 255.f;
	rgb_color.blue = (color & 0xFF) / 255.f;
	return rgb_color;
}

word player_profile_number_of_available_primary_colors(void) { return NUMBEROF(n64_profile_color_table); }

#ifdef HALO_N64_PC_EXPANSION
int n64_game_pc_equipment_enabled(void)
{
    return n64_pc_equipment_enabled(n64_choice.weapon_set,
        n64_choice.firefight || n64_choice.campaign);
}
int n64_game_pc_vehicle_enabled(void)
{
    return n64_pc_vehicle_enabled(n64_choice.vehicle_set,
        n64_choice.firefight || n64_choice.campaign);
}
#endif
