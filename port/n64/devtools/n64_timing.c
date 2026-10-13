/*
N64_TIMING.C

Timing chosen game functions (a debugging aid, not built): copy it into
port/n64/game/ and add -Wl,--wrap=<name> to port.json's ldflags for every
function it wraps; each wrapper adds up the time spent in its function,
printed every 240 HUD draws ("timing:" lines). Only calls between
translation units are seen.
*/

#include "cseries.h"
#include "../src/n64.h"

#include <stdio.h>

#define TIMED(name, parameters, arguments) \
	void __real_##name parameters; \
	static unsigned long long name##_ticks; \
	void __wrap_##name parameters \
	{ \
		unsigned long long start = n64_ticks64(); \
		__real_##name arguments; \
		name##_ticks += n64_ticks64() - start; \
	}

#define REPORT(name) \
	do { \
		char line[96]; \
		snprintf(line, sizeof(line), "timing: " #name " %lu us\n", (unsigned long)(name##_ticks * 1000000ull / 46875000ull / 240)); \
		n64_log(line); \
		name##_ticks = 0; \
	} while (0)

struct player_datum;

TIMED(motion_sensor_tick, (void), ())
TIMED(hud_render_weapon_interface, (struct player_datum *player), (player))
TIMED(hud_render_unit_interface, (struct player_datum *player), (player))
TIMED(hud_render_nav_points, (short local_player_index), (local_player_index))
TIMED(hud_render_damage_indicators, (short local_player_index), (local_player_index))
TIMED(hud_messaging_update, (short local_player_index), (local_player_index))
TIMED(game_engine_post_rasterize, (void), ())

void __real_hud_draw_screen(void);
static unsigned long long hud_draw_screen_ticks;
static unsigned long hud_draw_screen_calls;

void __wrap_hud_draw_screen(void)
{
	unsigned long long start = n64_ticks64();

	__real_hud_draw_screen();
	hud_draw_screen_ticks += n64_ticks64() - start;
	if (!(++hud_draw_screen_calls % 240))
	{
		REPORT(hud_draw_screen);
		REPORT(motion_sensor_tick);
		REPORT(hud_render_weapon_interface);
		REPORT(hud_render_unit_interface);
		REPORT(hud_render_nav_points);
		REPORT(hud_render_damage_indicators);
		REPORT(hud_messaging_update);
		REPORT(game_engine_post_rasterize);
	}
}
