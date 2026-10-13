/*
GAME_TIME.C

symbols in this file:
000A4D00 0020:
	_game_time_initialized (0000)
000A4D20 0030:
	_game_time_initialize (0000)
000A4D50 0050:
	_game_time_initialize_for_new_map (0000)
000A4DA0 0020:
	_game_time_dispose_from_old_map (0000)
000A4DC0 0010:
	_game_time_dispose (0000)
000A4DD0 0040:
	_game_time_end (0000)
000A4E10 0040:
	_game_time_get (0000)
000A4E50 0040:
	_game_time_get_elapsed (0000)
000A4E90 0040:
	_local_time_get (0000)
000A4ED0 0040:
	_local_time_get_elapsed (0000)
000A4F10 0040:
	_game_predicting (0000)
000A4F50 0050:
	_game_in_progress (0000)
000A4FA0 0040:
	_game_time_get_paused (0000)
000A4FE0 0050:
	_game_time_set_paused (0000)
000A5030 0040:
	_game_time_get_speed (0000)
000A5070 0050:
	_game_time_set_speed (0000)
000A50C0 0010:
	_code_000a50c0 (0000)
000A50D0 01e0:
	_code_000a50d0 (0000)
000A52B0 00e0:
	_game_time_start (0000)
000A5390 0360:
	_game_time_update (0000)
0025C480 0012:
	??_C@_0BC@ENHAGKAA@game?5time?5globals?$AA@ (0000)
0025C494 0035:
	??_C@_0DF@KKMNPDPK@game_time_globals?5?$CG?$CG?5?$CBgame_time_@ (0000)
0025C4CC 0020:
	??_C@_0CA@INLHHDGN@c?3?2halo?2SOURCE?2game?2game_time?4c?$AA@ (0000)
0025C4EC 0012:
	??_C@_0BC@NOJBGBLH@game_time_globals?$AA@ (0000)
0025C500 0034:
	??_C@_0DE@PPGKJIJK@game_time_globals?5?$CG?$CG?5game_time_g@ (0000)
0025C534 001b:
	??_C@_0BL@DECMFGMD@?$CBgame_time_globals?9?$DOactive?$AA@ (0000)
0025C550 0021:
	??_C@_0CB@OIFBECDL@ticks_elapsed?5?$DM?$DN?5maximum_actions@ (0000)
0025C578 004c:
	??_C@_0EM@FOLKNDLG@game_time_globals?9?$DOleftover_dt?$DO?$DN@ (0000)
0025C5C8 004c:
	??_C@_0EM@FBBKPKLD@update?5server?5is?5too?5far?5ahead?5o@ (0000)
0043EE04 002c:
	_bss_0043ee04 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "real_math.h"
#include "game.h"
#include "player_queues_new.h"
#ifdef HALO_LINUX
/* network_game_globals.c's */
boolean network_game_distributed(void);
/* port/linux/game/network_distributed.c's */
void network_distributed_tick(void);
#endif

/* ---------- constants */

enum
{
	SOME_LARGE_NUMBER_OF_TICKS = 1000, // 0x03E8
};

/* ---------- macros */

/* ---------- structures */

struct game_time_statistics
{
	FILE *file; // 0x0
	boolean active; // 0x4
	boolean first_line; // 0x5
	unsigned long last_milliseconds; // 0x8
	short frame_count; // 0xC
	short total_milliseconds_elapsed; // 0xE
	short minimum_milliseconds_per_frame; // 0x10
	short maximum_milliseconds_per_frame; // 0x12
	short total_latency; // 0x14
	short minimum_latency; // 0x16
	short maximum_latency; // 0x18
	short server_updates; // 0x1A
	short minimum_server_updates; // 0x1C
	short maximum_server_updates; // 0x1E
	short predicted_updates; // 0x20
	short minimum_predicted_updates; // 0x22
	short maximum_predicted_updates; // 0x24
};

struct game_time_globals_struct
{
	boolean initialized;
	boolean active;
	boolean paused;
	short monitor_state;
	short monitor_counter;
	short monitor_latency;
	long local_time;
	short last_local_time_elapsed;
	long server_time;
	real speed;
	real leftover_dt;
};

/* ---------- prototypes */

extern void *game_state_malloc(char const *, char const *, long);
struct network_game_server;
extern struct network_game_server *global_network_game_server_get(void);
extern long network_game_server_get_oldest_client_update_received(struct network_game_server *server);
extern void network_game_server_stalled_on_client(struct network_game_server *server, boolean stalled);
extern void network_game_server_update_ticks(struct network_game_server *server, long ticks);
/* ---------- globals */

static struct game_time_statistics game_time_statistics;
static struct game_time_globals_struct *game_time_globals;

/* ---------- public code */

boolean game_time_initialized(
	void)
{
	return (game_time_globals && game_time_globals->initialized);
}

void game_time_initialize(
	void)
{
	game_time_globals = (struct game_time_globals_struct *)game_state_malloc("game time globals", NULL, sizeof(*game_time_globals));
	memset(game_time_globals, 0, sizeof(*game_time_globals));

	return;
}

void game_time_initialize_for_new_map(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 131, game_time_globals && !game_time_globals->initialized);
	memset(game_time_globals, 0, sizeof(*game_time_globals));
	game_time_globals->initialized = TRUE;

	return;
}

void game_time_dispose_from_old_map(
	void)
{
	if (game_time_globals)
	{
		game_time_globals->initialized = FALSE;
		game_time_globals->active = FALSE;
	}

	return;
}

void game_time_dispose(
	void)
{
	return;
}

void game_time_end(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 196, game_time_globals);
	game_time_globals->active = FALSE;

	return;
}

long game_time_get(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 463, game_time_globals && game_time_globals->initialized);

	return game_time_globals->local_time;
}

short game_time_get_elapsed(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 471, game_time_globals && game_time_globals->initialized);

	return game_time_globals->last_local_time_elapsed;
}

long local_time_get(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 479, game_time_globals && game_time_globals->initialized);

	return game_time_globals->local_time;
}

short local_time_get_elapsed(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 487, game_time_globals && game_time_globals->initialized);

	return game_time_globals->last_local_time_elapsed;
}

boolean game_predicting(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 495, game_time_globals && game_time_globals->initialized);

	return FALSE;
}

boolean game_in_progress(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 505, game_time_globals);

	if (game_time_globals->initialized)
	{
		if (game_time_globals->active)
		{
			return TRUE;
		}

		/* bug? */
		if (game_time_globals->paused)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

boolean game_time_get_paused(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 533, game_time_globals);

	return game_time_globals->paused;
}

void game_time_set_paused(
	boolean paused)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 541, game_time_globals);

	if (game_time_globals->initialized)
	{
		game_time_globals->active = !paused;
	}

	game_time_globals->paused = paused;

	return;
}

#ifdef HALO_LINUX
/* how far the clock has run into the next tick, 0 to 1: the native ports
draw frames between ticks (port/linux/game/render_interpolation.c) */
real game_time_get_tick_fraction(
	void)
{
	real fraction;

	if (!game_time_globals || !game_time_globals->active || game_time_globals->paused)
		return 1.0f;
	fraction = game_time_globals->leftover_dt * game_time_globals->speed * TICKS_PER_SECOND;
	return PIN(fraction, 0.0f, 1.0f);
}

#endif
real game_time_get_speed(
	void)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 555, game_time_globals);

	return game_time_globals->speed;
}

void game_time_set_speed(
	real speed)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 562, game_time_globals);

	game_time_globals->speed = speed;

	return;
}

void code_000a50c0(
	void)
{
	game_time_statistics.first_line = TRUE;
	game_time_statistics.active = FALSE;

	return;
}

static void code_000a50d0(
	short latency,
	short server_updates,
	short predicted_updates,
	boolean first_line)
{
	short milliseconds_elapsed;
	unsigned long milliseconds;

	if (!game_time_statistics.active)
	{
		game_time_statistics.frame_count = 0;
		game_time_statistics.total_milliseconds_elapsed = 0;
		game_time_statistics.minimum_milliseconds_per_frame = SHORT_MAX;
		game_time_statistics.maximum_milliseconds_per_frame = SHORT_MIN;
		game_time_statistics.total_latency = 0;
		game_time_statistics.minimum_latency = SHORT_MAX;
		game_time_statistics.maximum_latency = SHORT_MIN;
		game_time_statistics.server_updates = 0;
		game_time_statistics.minimum_server_updates = SHORT_MAX;
		game_time_statistics.maximum_server_updates = SHORT_MIN;
		game_time_statistics.predicted_updates = 0;
		game_time_statistics.minimum_predicted_updates = SHORT_MAX;
		game_time_statistics.maximum_predicted_updates = SHORT_MIN;
		game_time_statistics.last_milliseconds = system_milliseconds();
		game_time_statistics.active = TRUE;
		return;
	}

	milliseconds = system_milliseconds();
	milliseconds_elapsed = (short)(milliseconds - game_time_statistics.last_milliseconds);
	game_time_statistics.frame_count++;
	game_time_statistics.total_milliseconds_elapsed += milliseconds_elapsed;
	game_time_statistics.last_milliseconds = milliseconds;
	if (milliseconds_elapsed > game_time_statistics.maximum_milliseconds_per_frame)
		game_time_statistics.maximum_milliseconds_per_frame = milliseconds_elapsed;
	if (milliseconds_elapsed < game_time_statistics.minimum_milliseconds_per_frame)
		game_time_statistics.minimum_milliseconds_per_frame = milliseconds_elapsed;

	game_time_statistics.total_latency += latency;
	if (latency > game_time_statistics.maximum_latency)
		game_time_statistics.maximum_latency = latency;
	if (latency < game_time_statistics.minimum_latency)
		game_time_statistics.minimum_latency = latency;

	game_time_statistics.server_updates += server_updates;
	if (server_updates > game_time_statistics.maximum_server_updates)
		game_time_statistics.maximum_server_updates = server_updates;
	if (server_updates < game_time_statistics.minimum_server_updates)
		game_time_statistics.minimum_server_updates = server_updates;

	game_time_statistics.predicted_updates += predicted_updates;
	if (predicted_updates > game_time_statistics.maximum_predicted_updates)
		game_time_statistics.maximum_predicted_updates = predicted_updates;
	if (predicted_updates < game_time_statistics.minimum_predicted_updates)
		game_time_statistics.minimum_predicted_updates = predicted_updates;

	if (game_time_statistics.total_milliseconds_elapsed >= SOME_LARGE_NUMBER_OF_TICKS &&
		game_time_statistics.frame_count > 0)
	{
		if (game_time_statistics.minimum_latency)
		{
			short monitor_state = game_time_statistics.minimum_latency >= 0;
			if (monitor_state != game_time_globals->monitor_state)
			{
				game_time_globals->monitor_state = monitor_state;
				game_time_globals->monitor_counter = 0;
				game_time_globals->monitor_latency = monitor_state ? SHORT_MAX : SHORT_MIN;
			}

			switch (game_time_globals->monitor_state)
			{
			case 0:
				if (game_time_statistics.minimum_latency > game_time_globals->monitor_latency)
					game_time_globals->monitor_latency = game_time_statistics.minimum_latency;
				break;
			case 1:
				if (game_time_statistics.minimum_latency < game_time_globals->monitor_latency)
					game_time_globals->monitor_latency = game_time_statistics.minimum_latency;
				break;
			}

			game_time_globals->monitor_counter++;
			if (game_time_globals->monitor_counter == 5)
			{
				game_time_globals->monitor_state = NONE;
				game_time_statistics.active = FALSE;
				return;
			}
		}
		else
		{
			game_time_globals->monitor_state = NONE;
		}

		game_time_statistics.active = FALSE;
	}

	return;
}

void game_time_start(
	void)
{
	short connection;

	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 162, game_time_globals && game_time_globals->initialized);
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 163, !game_time_globals->active);

	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 562, game_time_globals);

	game_time_globals->speed = 1.f;
	game_time_globals->leftover_dt = 0;
	game_time_globals->active = TRUE;
	
	code_000a50c0();

	connection = game_connection();

	switch (connection)
	{
	case _game_connection_local:
	case _game_connection_network_server:
	{
		update_server_start();
		break;
	}
	case _game_connection_network_client:
	case _game_connection_film_playback:
	{
		update_client_start();
		break;
	}
	}

	return;
}

void game_time_update(
	real time_delta_sec)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 205, game_time_globals);

	if (game_time_globals->active)
	{
		long connection;
		long ticks_elapsed;
		real ticks_per_second = game_time_globals->speed*TICKS_PER_SECOND;

#ifdef HALO_LINUX
		/* The native builds draw several frames per tick
		(port/linux/game/render_interpolation.c). A frame that runs no tick has
		elapsed no game time: without this, the ticks of the last frame that
		ran some would count again on every frame after it, and whatever
		advances by game_time_get_elapsed() once a frame (chapter titles, HUD
		messages and flashes, screen flashes, camera shake and impulses) would
		run as many times too fast as there are frames per tick. On the Xbox
		every frame ran at least one tick. */
		game_time_globals->last_local_time_elapsed = 0;
#endif

		if (ticks_per_second > 0.f)
		{
			boolean discard_leftover_time;
			real game_time;
			real ticks_elapsed_real;

			connection = game_connection();
			switch (connection)
			{
			case _game_connection_film_playback:
				connection = TICKS_PER_SECOND;
				discard_leftover_time = FALSE;
				goto calculate_elapsed_ticks;
			case _game_connection_network_client:
				connection = TICKS_PER_SECOND;
				break;
			case _game_connection_network_server:
#ifdef HALO_LINUX
				/* distributed netcode: the host waits for nobody */
				if (network_game_distributed())
				{
					connection = TICKS_PER_SECOND;
					break;
				}
#endif
				{
					struct network_game_server *server = global_network_game_server_get();
					long oldest_client_update = network_game_server_get_oldest_client_update_received(server);
					long game_time = game_time_get();

					match_vassert("c:\\halo\\SOURCE\\game\\game_time.c", 243,
						(unsigned long)(game_time - oldest_client_update) <= 128,
						"update server is too far ahead of a client for the client to ever catch up!");
					if ((unsigned long)game_time > 0)
					{
						game_time = oldest_client_update - game_time + 128;
						if (game_time < TICKS_PER_SECOND)
						{
							connection = game_time;
							if (game_time <= 0)
							{
								network_game_server_stalled_on_client(server, TRUE);
								break;
							}
						}
						else
						{
							connection = TICKS_PER_SECOND;
						}

						network_game_server_stalled_on_client(server, FALSE);
					}
					else
					{
						connection = 1;
					}
				}
				break;
			case _game_connection_local:
				connection = 7;
				break;
			default:
				connection = 7;
				break;
			}
			discard_leftover_time = TRUE;

		calculate_elapsed_ticks:
			game_time = time_delta_sec + game_time_globals->leftover_dt;
			ticks_elapsed_real = (real)floor(game_time*ticks_per_second);
			ticks_elapsed = (long)(ticks_elapsed_real <= (real)SOME_LARGE_NUMBER_OF_TICKS ?
				ticks_elapsed_real : (real)SOME_LARGE_NUMBER_OF_TICKS);
			if (ticks_elapsed > connection)
			{
				ticks_elapsed = connection;
				if (discard_leftover_time)
					game_time = ticks_elapsed_real/ticks_per_second;
			}

			game_time_globals->leftover_dt = game_time - ticks_elapsed_real/ticks_per_second;
			if (game_time_globals->leftover_dt < 0.f)
				game_time_globals->leftover_dt = 0.f;
			match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 306,
				game_time_globals->leftover_dt>=0.f && game_time_globals->leftover_dt<100.f);

			/* (distributed netcode: a client ticks on its own clock, with its
			own input and the latest the host relayed) */
			if (game_connection() == _game_connection_network_client
#ifdef HALO_LINUX
				&& !network_game_distributed()
#endif
				)
			{
				long maximum_actions = update_client_get_maximum_actions();
				if (ticks_elapsed > maximum_actions)
					ticks_elapsed = FLOOR(maximum_actions - 1, 0);
				else if (ticks_elapsed + 7 < maximum_actions)
					ticks_elapsed = FLOOR(maximum_actions - 1, 0);
				else if (ticks_elapsed + 1 < maximum_actions)
					ticks_elapsed++;

				match_assert("c:\\halo\\SOURCE\\game\\game_time.c", 339, ticks_elapsed <= maximum_actions);
				if (ticks_elapsed > maximum_actions)
					ticks_elapsed = maximum_actions;
			}

			if (ticks_elapsed > 0)
			{
				long final_local_time;
				long maximum_possible_server_time;
				long server_updates;

				while (ticks_elapsed > 0 &&
					game_time_globals->local_time < game_time_globals->server_time)
				{
					game_time_globals->local_time++;
					ticks_elapsed--;
				}

				final_local_time = game_time_globals->local_time + ticks_elapsed;
				switch (game_connection())
				{
				case _game_connection_local:
					update_client_local_ticks(ticks_elapsed);
					break;
				case _game_connection_network_server:
					network_game_server_update_ticks(global_network_game_server_get(), ticks_elapsed);
					break;
				}

				maximum_possible_server_time = update_client_get_maximum_possible_server_time();
#ifdef HALO_LINUX
				if (game_connection() == _game_connection_network_client && network_game_distributed())
					maximum_possible_server_time = final_local_time;
#endif
				if (maximum_possible_server_time > game_time_globals->server_time)
				{
					long final_server_time = MIN(maximum_possible_server_time, final_local_time);
					long update_index;
					server_updates = final_server_time - game_time_globals->server_time;
					for (update_index = 0; update_index < server_updates; update_index++)
					{
#ifdef HALO_N64
						/* (the N64's around each tick: Firefight, the audio, its
						cost; port/n64/game/n64_game.c) */
						void n64_game_tick(void);

						n64_game_tick();
#else
						game_tick();
#endif
#ifdef HALO_LINUX
						render_interpolation_tick();
#endif
						game_time_globals->server_time++;
						game_time_globals->local_time++;
#ifdef HALO_LINUX
						/* the distributed netcode's per-tick state */
						network_distributed_tick();
#endif
					}
				}
				else
				{
					server_updates = 0;
				}

				code_000a50d0((short)(maximum_possible_server_time - game_time_globals->local_time),
					(short)server_updates, 0, FALSE);

				game_time_globals->last_local_time_elapsed = (short)ticks_elapsed;
			}
		}

		game_frame(game_time_get_speed()*time_delta_sec);
	}
	else
	{
		game_time_globals->last_local_time_elapsed = 0;
	}

	return;
}

/* ---------- private code */
