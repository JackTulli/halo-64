/*
MOTION_SENSOR.C

symbols in this file:
000CA160 00b0:
	_tiny_point2d_set (0000)
000CA210 0050:
	_tiny_point2d_get (0000)
000CA260 0160:
	_blip_type_get (0000)
000CA3C0 0060:
	_motion_sensor_blip_set_type_and_size (0000)
000CA420 0010:
	_blip_size_get (0000)
000CA430 0040:
	_get_motion_sensor_data (0000)
000CA470 0050:
	_motion_sensor_initialize (0000)
000CA4C0 0010:
	_motion_sensor_dispose (0000)
000CA4D0 0060:
	_motion_sensor_initialize_for_new_map (0000)
000CA530 0010:
	_motion_sensor_dispose_from_old_map (0000)
000CA540 0020:
	_should_track_object (0000)
000CA560 0070:
	_blip_begin (0000)
000CA5D0 00e0:
	_should_draw_object (0000)
000CA6B0 0190:
	_render_blip (0000)
000CA840 0490:
	_motion_sensor_update (0000)
000CACD0 0360:
	_update_motion_sensor (0000)
000CB030 0300:
	_render_motion_sensor (0000)
000CB330 0050:
	_motion_sensor_draw_screen (0000)
000CB380 0070:
	_motion_sensor_tick (0000)
002703E0 0054:
	_blip_colors (0000)
00270434 003e:
	??_C@_0DO@GOKNGPMD@fabs?$CIposition?9?$DOy?$CJ?5?$DM?5hud_globals?9@ (0000)
00270474 003e:
	??_C@_0DO@HNEGEKG@fabs?$CIposition?9?$DOx?$CJ?5?$DM?5hud_globals?9@ (0000)
002704B4 0029:
	??_C@_0CJ@LFAICIPE@c?3?2halo?2SOURCE?2interface?2motion_@ (0000)
002704E0 0004:
	__real@3c010204 (0000)
002704E4 000b:
	??_C@_0L@KEKONIMN@c_dropship?$AA@ (0000)
002704F0 0016:
	??_C@_0BG@PHBPOBKD@motion_sensor_globals?$AA@ (0000)
00270508 0016:
	??_C@_0BG@KNHDKBCC@motion?5sensor?5?$CIradar?$CJ?$AA@ (0000)
00270520 000c:
	??_C@_0M@EFNLDGJP@sensor?5data?$AA@ (0000)
0027052C 000a:
	??_C@_09KJONPCOF@reference?$AA@ (0000)
00270538 0008:
	__real@3fd5555555555555 (0000)
00270540 0004:
	__real@3dd6774d (0000)
00270544 0004:
	__real@3c800000 (0000)
00270548 001d:
	??_C@_0BN@LEDPOOMI@_blip_type_none?5?$CB?$DN?5blip_type?$AA@ (0000)
00270568 001c:
	??_C@_0BM@FMFNPKOG@blip_index?$DMMAX_CUSTOM_BLIPS?$AA@ (0000)
00270588 0008:
	__real@401c000000000000 (0000)
00270590 0008:
	__real@400c000000000000 (0000)
00270598 0003:
	??_C@_02BPBMPCEP@pt?$AA@ (0000)
0027059C 0004:
	__real@40026666 (0000)
002705A0 0008:
	__real@4000ccccc0000000 (0000)
002E4C6C 0018:
	_scale (0000)
00453AC8 000d:
	_next_vertex_index (0000)
	_motion_sensor_globals (0004)
	_sweep_theta (0008)
	_debug_motion_sensor_draw_all_units (000c)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"
#include "motion_sensor.h"
#include "camera/director.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/game_engine.h"
#include "game/player_control.h"
#include "game/players.h"
#include "game_state.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_draw.h"
#include "objects/objects.h"
#include "rasterizer/rasterizer.h"
#include "units/unit_definitions.h"
#include "units/units.h"
#include "units/vehicles.h"

/* ---------- constants */

enum
{
	_blip_type_self,
	_blip_type_friend,
	_blip_type_enemy,
	_blip_type_vehicle_friend,
	_blip_type_vehicle_enemy,
	_blip_type_custom,
	_blip_type_none,
	NUMBER_OF_BLIP_TYPES
};

enum
{
	_hud_blip_type_medium,
	_hud_blip_type_small,
	_hud_blip_type_large,
	NUMBER_OF_HUD_BLIP_TYPES
};

enum
{
	MAXIMUM_MOTION_SENSOR_BLIPS = 16,
	MAX_CUSTOM_BLIPS = 16,
	MOTION_SENSOR_HISTORY_COUNT = 10,
	MOTION_SENSOR_UPDATE_PERIOD = 15
};

/* ---------- macros */

/* ---------- structures */

struct tiny_point2d
{
	char x;
	char y;
};

struct motion_sensor_blip
{
	struct tiny_point2d position;
	char type;
	char size;
};

struct motion_sensor_datum
{
	struct motion_sensor_blip blips[MAXIMUM_MOTION_SENSOR_BLIPS];
	struct tiny_point2d custom_blips[MAXIMUM_MOTION_SENSOR_BLIPS];
	char custom_blip_goal_indices[MAXIMUM_MOTION_SENSOR_BLIPS];
	real_point2d reference_point;
	long blip_count;
	real yaw;
	byte custom_blip_count;
	byte pad[3];
};

struct motion_sensor_player
{
	struct motion_sensor_datum sensor_data[MOTION_SENSOR_HISTORY_COUNT];
	long unit_indices[MAXIMUM_MOTION_SENSOR_BLIPS];
};

struct motion_sensor_globals_definition
{
	struct motion_sensor_player players[4];
	long last_update_time;
	short active_sensor_index;
	boolean update;
	byte pad;
};

typedef char motion_sensor_blip_size_assert[
	sizeof(struct motion_sensor_blip) == 4 ? 1 : -1];
typedef char motion_sensor_datum_size_assert[
	sizeof(struct motion_sensor_datum) == 0x84 ? 1 : -1];
typedef char motion_sensor_player_size_assert[
	sizeof(struct motion_sensor_player) == 0x568 ? 1 : -1];
typedef char motion_sensor_globals_size_assert[
	sizeof(struct motion_sensor_globals_definition) == 0x15A8 ? 1 : -1];

/* ---------- prototypes */

static void tiny_point2d_set(
	struct tiny_point2d *tiny_position,
	real_point2d const *position);
static void tiny_point2d_get(
	struct tiny_point2d const *tiny_position,
	real_point2d *position);
static char blip_type_get(
	long object_index,
	long local_player_index);
static void motion_sensor_blip_set_type_and_size(
	struct motion_sensor_blip *blip,
	long unit_index,
	long local_player_index);
static real blip_size_get(
	char size_type);
static struct motion_sensor_player *get_motion_sensor_data(
	short local_player_index);
static boolean should_track_object(
	long object_index);
static void blip_begin(
	real_point2d const *reference,
	long unused,
	boolean in_multiplayer,
	short local_player_index);
static boolean should_draw_object(
	long object_index);
static void render_blip(
	struct motion_sensor_datum *sensor,
	char blip_type,
	char blip_size_type,
	real_point2d const *point,
	real relative_scale,
	real fade,
	real radius);
static void motion_sensor_update(
	void);
static void update_motion_sensor(
	short local_player_index);
static void render_motion_sensor(
	short local_player_index,
	boolean in_multiplayer,
	point2d const *reference);

/* ---------- globals */

const real_rgb_color blip_colors[] =
{
	{ 1.f, .5f, 0.f },
	{ 1.f, 1.f, 0.f },
	{ 1.f, 0.f, 0.f },
	{ 1.f, 1.f, 0.f },
	{ 1.f, 0.f, 0.f },
	{ .5f, .5f, 1.f },
	{ 0.f, 0.f, 0.f }
};

real scale[] =
{
	1.0f,
	0.0f,
	-0.75f,
	1.0f,
	1.0f,
	1.1f
};

long next_vertex_index= {0};
static struct motion_sensor_globals_definition *motion_sensor_globals = NULL;
static real sweep_theta = 0.f;
boolean debug_motion_sensor_draw_all_units= {0};

extern short blip_player_index;
extern real_point2d center_point;

/* ---------- private code */

static void tiny_point2d_set(
	struct tiny_point2d *tiny_position,
	real_point2d const *position)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		105,
		fabs(position->x) < hud_globals->defaults.motion_sensor_range);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		106,
		fabs(position->y) < hud_globals->defaults.motion_sensor_range);

	tiny_position->x = (char)(position->x / hud_globals->defaults.motion_sensor_range * 127.0f);
	tiny_position->y = (char)(position->y / hud_globals->defaults.motion_sensor_range * 127.0f);

	return;
}

static void motion_sensor_update(
	void)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	long current_time;
	short active_sensor_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));
	game_engine_running();
	current_time = game_time_get();
	motion_sensor_globals->update = TRUE;
	motion_sensor_globals->last_update_time = current_time;
	active_sensor_index =
		(short)((motion_sensor_globals->active_sensor_index + 1) %
			MOTION_SENSOR_HISTORY_COUNT);
	motion_sensor_globals->active_sensor_index = active_sensor_index;

	if ((current_time % MOTION_SENSOR_UPDATE_PERIOD) && current_time)
	{
		short previous_sensor_index =
			(short)((active_sensor_index + MOTION_SENSOR_HISTORY_COUNT - 1) %
				MOTION_SENSOR_HISTORY_COUNT);
		short player_count = local_player_count();
		short local_player_index = local_player_get_next(NONE);

		while (player_count-- > 0)
		{
			struct motion_sensor_player *player =
				&motion_sensor_globals->players[local_player_index];

			csmemcpy(
				&player->sensor_data[active_sensor_index],
				&player->sensor_data[previous_sensor_index],
				sizeof(struct motion_sensor_datum));
			local_player_index = local_player_get_next(local_player_index);
		}
	}
	else
	{
#ifdef HALO_N64
		/* each local player's blip count, then the players in turn: the
		decompiled array (and the cameras') held two players and overran with
		four, corrupting the stack */
		short local_player_slots[2 * MAXIMUM_LOCAL_PLAYERS] = { 0 };
		real_point3d camera_positions[MAXIMUM_LOCAL_PLAYERS];
#define MOTION_SENSOR_PLAYER_LIST MAXIMUM_LOCAL_PLAYERS
#else
		short local_player_slots[4] = { 0 };
		real_point3d camera_positions[2];
#define MOTION_SENSOR_PLAYER_LIST 2
#endif
		short player_count = local_player_count();
		short local_player_index = local_player_get_next(NONE);
		struct object_iterator iterator;
		boolean done = FALSE;
		short player_scan_index;

		for (player_scan_index = 0;
			player_scan_index < player_count;
			player_scan_index++)
		{
			struct motion_sensor_datum *sensor =
				&motion_sensor_globals->players[local_player_index]
					.sensor_data[active_sensor_index];
			long player_index =
				local_player_get_player_index(local_player_index);
			long unit_index = player_index == NONE
				? NONE
				: player_get(
					local_player_get_player_index(local_player_index))->unit_index;
			short blip_index;

			local_player_slots[player_scan_index + MOTION_SENSOR_PLAYER_LIST] = local_player_index;
			camera_positions[local_player_index].x = 0.0f;
			camera_positions[local_player_index].y = 0.0f;
			camera_positions[local_player_index].z = 0.0f;
			if (unit_index != NONE)
			{
				unit_get_camera_position(
					unit_index,
					&camera_positions[local_player_index]);
			}

			sensor->blip_count = 0;
			for (blip_index = 0;
				blip_index < MAXIMUM_MOTION_SENSOR_BLIPS;
				blip_index++)
			{
				sensor->blips[blip_index].type = _blip_type_none;
			}

			local_player_index = local_player_get_next(local_player_index);
		}

		object_iterator_new(&iterator, _object_mask_unit, TRUE);
		while (object_iterator_next(&iterator) && !done)
		{
			if (should_track_object(iterator.index) &&
				should_draw_object(iterator.index))
			{
				real_point3d object_position;
				real object_radius;
				short players_full = 0;

				object_get_bounding_sphere(
					iterator.index,
					&object_position,
					&object_radius);

				for (player_scan_index = 0;
					player_scan_index < player_count;
					player_scan_index++)
				{
					short scan_player_index =
						local_player_slots[player_scan_index + MOTION_SENSOR_PLAYER_LIST];
					long player_index =
						local_player_get_player_index(scan_player_index);

					if (player_index != NONE &&
						player_get(
							local_player_get_player_index(scan_player_index))
							->unit_index != NONE)
					{
						short blip_index = local_player_slots[scan_player_index];

						if (blip_index >= MAXIMUM_MOTION_SENSOR_BLIPS)
						{
							players_full++;
						}
						else
						{
							real_vector3d displacement;

							displacement.i =
								object_position.x -
								camera_positions[scan_player_index].x;
							displacement.j =
								object_position.y -
								camera_positions[scan_player_index].y;
							displacement.k =
								object_position.z -
								camera_positions[scan_player_index].z;

#ifdef HALO_LINUX
							/* test the range before taking a blip slot in multiplayer
							too: with the native builds' larger sessions, units out of
							range would otherwise fill the slots (update_motion_sensor
							only drops them later) and crowd nearby ones out. Like that
							test, the multiplayer range ignores height. */
							if (game_engine_running())
								displacement.k = 0.0f;

							if (magnitude_squared3d(&displacement) <=
								hud_globals->defaults.motion_sensor_range *
									hud_globals->defaults.motion_sensor_range)
#else
							if (game_engine_running() ||
								magnitude_squared3d(&displacement) <=
									hud_globals->defaults.motion_sensor_range *
										hud_globals->defaults.motion_sensor_range)
#endif
							{
								struct motion_sensor_player *player =
									&motion_sensor_globals->players[scan_player_index];
								struct motion_sensor_datum *sensor =
									&player->sensor_data[active_sensor_index];
								struct motion_sensor_blip *blip =
									&sensor->blips[blip_index];

								motion_sensor_blip_set_type_and_size(
									blip,
									iterator.index,
									scan_player_index);

								player->unit_indices[blip_index] = iterator.index;
								sensor->blip_count++;
								local_player_slots[scan_player_index] =
									(short)(blip_index + 1);
							}
						}
					}
				}

				if (players_full == player_count)
					done = TRUE;
			}
		}
	}

	{
		short corrupt_index;
		short buffer_index;

		for (buffer_index = STACK_BUFFER_LENGTH - 1; buffer_index >= 0; buffer_index--)
		{
			if (stack_buffer[buffer_index] != 0x62626262)
				goto corrupt_stack_found_motion_sensor_update;
		}

		corrupt_index = NONE;
		goto stack_buffer_checked_motion_sensor_update;

corrupt_stack_found_motion_sensor_update:
		corrupt_index = buffer_index;

stack_buffer_checked_motion_sensor_update:
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
			642,
			return_eip == get_return_eip(),
			"corrupt return address!");
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
			642,
			corrupt_index == NONE,
			csprintf(temporary, "corrupt stack at %d!", corrupt_index));
	}

	return;
}

static void update_motion_sensor(
	short local_player_index)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct motion_sensor_player *player;
	short active_sensor_index;
	long unit_index;
	struct motion_sensor_datum *sensor;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));
	player = get_motion_sensor_data(local_player_index);
	game_time_get();
	if (motion_sensor_globals->update)
	{
		active_sensor_index = motion_sensor_globals->active_sensor_index;
		unit_index = local_player_get_player_index(local_player_index) == NONE
			? NONE
			: player_get(local_player_get_player_index(local_player_index))->unit_index;
		sensor = &player->sensor_data[active_sensor_index];
		if (unit_index != NONE)
		{
			{
				real_point3d camera_position;

				unit_get_camera_position(unit_index, &camera_position);
				sensor->reference_point.x = camera_position.x;
				sensor->reference_point.y = camera_position.y;
			}

			{
				short blip_index;
				struct motion_sensor_blip *blips = sensor->blips;

				for (blip_index = 0;
					blip_index < MAXIMUM_MOTION_SENSOR_BLIPS;
					blip_index++)
				{
					long object_index = player->unit_indices[blip_index];

					if (unit_try_and_get(object_index))
					{
						real_point3d object_position;
						real_point3d player_center;
						real_point2d displacement;
						real object_radius;
						boolean draw = should_draw_object(object_index);

						object_get_bounding_sphere(
							object_index,
							&object_position,
							&object_radius);
						displacement.x = object_position.x - sensor->reference_point.x;
						displacement.y = object_position.y - sensor->reference_point.y;
						player_center.x = sensor->reference_point.x;
						player_center.y = sensor->reference_point.y;
						player_center.z = object_position.z;

						if (draw &&
							distance_squared3d(&player_center, &object_position) <=
								hud_globals->defaults.motion_sensor_range *
									hud_globals->defaults.motion_sensor_range)
						{
							tiny_point2d_set(
								&blips[blip_index].position,
								&displacement);
						}
						else
						{
							blips[blip_index].type = _blip_type_none;
							player->unit_indices[blip_index] = NONE;
						}
					}
				}
			}

			sensor->yaw =
				player_control_get_facing_angles(local_player_index)->yaw +
				1.5707964f;
			{
				real_point3d camera_position;
				real_point2d custom_positions[MAXIMUM_MOTION_SENSOR_BLIPS];
				long blip_index;
				long out_of_range_count = 0;

				sensor->custom_blip_count =
					(byte)game_engine_player_get_custom_motion_sensor_positions(
						local_player_get_player_index(local_player_index),
						custom_positions,
						(byte *)sensor->custom_blip_goal_indices,
						MAXIMUM_MOTION_SENSOR_BLIPS);
				unit_get_camera_position(unit_index, &camera_position);

				for (blip_index = 0;
					blip_index < sensor->custom_blip_count;
					blip_index++)
				{
					real_point3d custom_temp;

					custom_temp.x = custom_positions[blip_index].x;
					custom_temp.y = custom_positions[blip_index].y;
					custom_temp.z = camera_position.z;
					custom_positions[blip_index].x -= camera_position.x;
					custom_positions[blip_index].y -= camera_position.y;
					if (distance_squared3d(&camera_position, &custom_temp) <=
							hud_globals->defaults.motion_sensor_range *
								hud_globals->defaults.motion_sensor_range)
					{
						tiny_point2d_set(
							&sensor->custom_blips[blip_index - out_of_range_count],
							&custom_positions[blip_index]);
					}
					else
					{
						out_of_range_count++;
					}
				}

				sensor->custom_blip_count -= (byte)out_of_range_count;
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\motion_sensor.c", 756);

	return;
}

static void render_motion_sensor(
	short local_player_index,
	boolean in_multiplayer,
	point2d const *reference)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	director_perspective perspective;
	struct motion_sensor_player *player;
	real relative_scale;
	real_point2d center;
	short history_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));
	perspective = director_get_perspective(local_player_index);
	if (perspective == _director_perspective_neutral ||
		perspective == _director_perspective_scripted)
	{
		goto render_motion_sensor_done;
	}

	player = get_motion_sensor_data(local_player_index);
	relative_scale =
		hud_globals->defaults.motion_sensor_scale /
		hud_globals->defaults.motion_sensor_range;
	center.x = (real)reference->x;
	center.y = (real)reference->y;
	blip_begin(&center, 0, in_multiplayer, local_player_index);

	for (history_index = 0;
		history_index < MOTION_SENSOR_HISTORY_COUNT;
		history_index++)
	{
		short sensor_index = (short)(
			(motion_sensor_globals->active_sensor_index - history_index +
				MOTION_SENSOR_HISTORY_COUNT) %
			MOTION_SENSOR_HISTORY_COUNT);
		struct motion_sensor_datum *sensor =
			&player->sensor_data[sensor_index];
		real weight =
			(real)(MOTION_SENSOR_HISTORY_COUNT - history_index) * 0.1f;
		real fade = weight * weight;
		real radius = (real)(
			power(1.0f - weight, 3.5f) * 7.0 + 1.0);
		short blip_index;

		for (blip_index = 0;
			blip_index < MAXIMUM_MOTION_SENSOR_BLIPS;
			blip_index++)
		{
			struct motion_sensor_blip *blip = &sensor->blips[blip_index];

			if (blip->type != _blip_type_none)
			{
				real_point2d point;

				tiny_point2d_get(&blip->position, &point);
				render_blip(
					sensor,
					blip->type,
					blip->size,
					&point,
					relative_scale,
					fade,
					radius);
			}
		}

		for (blip_index = 0;
			blip_index < sensor->custom_blip_count;
			blip_index++)
		{
			match_assert(
				"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
				815,
				blip_index<MAX_CUSTOM_BLIPS);

			if (game_engine_get_goal_in_use(
				(short)sensor->custom_blip_goal_indices[blip_index]))
			{
				real_point2d point;

				tiny_point2d_get(&sensor->custom_blips[blip_index], &point);
				render_blip(
					sensor,
					_blip_type_custom,
					_hud_blip_type_medium,
					&point,
					relative_scale,
					fade,
					radius);
			}
		}
	}

	rasterizer_hud_motion_sensor_blip_end(&center_point, sweep_theta);

render_motion_sensor_done:
	{
		short corrupt_index;
		short buffer_index;

		for (buffer_index = STACK_BUFFER_LENGTH - 1; buffer_index >= 0; buffer_index--)
		{
			if (stack_buffer[buffer_index] != 0x62626262)
				goto corrupt_stack_found_render_motion_sensor;
		}

		corrupt_index = NONE;
		goto stack_buffer_checked_render_motion_sensor;

corrupt_stack_found_render_motion_sensor:
		corrupt_index = buffer_index;

stack_buffer_checked_render_motion_sensor:
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
			830,
			return_eip == get_return_eip(),
			"corrupt return address!");
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
			830,
			corrupt_index == NONE,
			csprintf(temporary, "corrupt stack at %d!", corrupt_index));
	}

	return;
}

void motion_sensor_draw_screen(
	short local_player_index,
	boolean in_multiplayer,
	point2d const *pt)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		476,
		pt);

	if (local_player_index != NONE)
	{
		update_motion_sensor(local_player_index);
		render_motion_sensor(local_player_index, in_multiplayer, pt);
	}

	return;
}

void motion_sensor_tick(
	void)
{
	real sweep_time = (real)fmod(
		(real)game_time_get() * (1.0f / TICKS_PER_SECOND),
		2.1f);

	if (sweep_time < 2.0375f)
	{
		sweep_theta =
			1.0f /
			((sweep_time + 0.0625f) * scale[5]);
	}
	else
	{
		sweep_theta = 0.4f;
	}

#if defined(HALO_LINUX) || defined(HALO_N64)
	/* The HUD is drawn once a frame, several frames per tick
	(port/linux/game/render_interpolation.c), and each update moves the
	blip history on a step: update once a tick, as on the Xbox. (The N64
	draws a view's HUD only every 2nd-6th frame, so hud_update calls this
	each tick as well: with four views the history stepped only at view
	0's recordings, and its scan, every 15th tick, seldom fell on one.) */
	if (motion_sensor_globals->last_update_time != game_time_get())
#endif
	motion_sensor_update();

	return;
}

static void tiny_point2d_get(
	struct tiny_point2d const *tiny_position,
	real_point2d *position)
{
	position->x =
		(real)tiny_position->x *
		hud_globals->defaults.motion_sensor_range *
		0.0078740157f;
	position->y =
		(real)tiny_position->y *
		hud_globals->defaults.motion_sensor_range *
		0.0078740157f;

	return;
}

static char blip_type_get(
	long object_index,
	long local_player_index)
{
	long local_player_team =
		player_get(local_player_get_player_index(local_player_index))->team_index;
	long owner_player_index;
	long controlling_local_player_index;
	struct unit_datum *unit;
	long team_index;
	char blip_type;

	if (object_index == NONE)
		return _blip_type_custom;

	owner_player_index = player_index_from_unit_index(object_index);
	controlling_local_player_index = owner_player_index == NONE
		? NONE
		: player_get(player_index_from_unit_index(object_index))->local_player_index;
	if (controlling_local_player_index == local_player_index)
		return _blip_type_self;

	if (!unit_try_and_get(object_index))
		return _blip_type_enemy;

	unit = unit_get(object_index);
	if (vehicle_try_and_get(object_index))
	{
		struct unit_datum *vehicle = vehicle_get(object_index);

		if (vehicle->unit.gunner_object_index != NONE)
		{
			struct unit_datum *seat_unit = unit_get(vehicle->unit.gunner_object_index);

			blip_type = (game_team_is_enemy(
				seat_unit->object.owner_team_index,
				local_player_team) != FALSE) + _blip_type_vehicle_friend;

			return blip_type;
		}

		if (vehicle->unit.driver_object_index != NONE)
		{
			struct unit_datum *seat_unit = unit_get(vehicle->unit.driver_object_index);

			blip_type = (game_team_is_enemy(
				seat_unit->object.owner_team_index,
				local_player_team) != FALSE) + _blip_type_vehicle_friend;

			return blip_type;
		}

		{
			struct unit_definition *definition =
				unit_definition_get(vehicle->definition_index);

			if (definition->unit.seats.count > 1)
			{
				struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
					&definition->unit.seats,
					0,
					struct unit_seat);

				if (!strncmp(seat->label, "c_dropship", 10))
					return _blip_type_vehicle_enemy;
			}
		}

		return _blip_type_vehicle_friend;
	}

	team_index = player_get(local_player_get_player_index(local_player_index))->team_index;
	blip_type = (game_team_is_enemy(
		unit->object.owner_team_index,
		team_index) != FALSE) + _blip_type_friend;

	return blip_type;
}

static void motion_sensor_blip_set_type_and_size(
	struct motion_sensor_blip *blip,
	long unit_index,
	long local_player_index)
{
	char blip_size;

	blip->type = blip_type_get(unit_index, local_player_index);
	if (unit_index != NONE && unit_try_and_get(unit_index))
	{
		struct unit_definition *definition =
			unit_definition_get(unit_get(unit_index)->definition_index);
		short size_type = definition->unit.blip_type;

		if (!VALID_INDEX(size_type, NUMBER_OF_HUD_BLIP_TYPES))
			size_type = _hud_blip_type_medium;
		blip_size = (char)size_type;
	}
	else
	{
		blip_size = _hud_blip_type_medium;
	}
	blip->size = blip_size;

	return;
}

static real blip_size_get(
	char size_type)
{
	return scale[size_type + 1];
}

static struct motion_sensor_player *get_motion_sensor_data(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		287,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	return &motion_sensor_globals->players[local_player_index];
}

/* ---------- public code */

void motion_sensor_initialize(
	void)
{
	motion_sensor_globals = (struct motion_sensor_globals_definition *)game_state_malloc("motion sensor (radar)", "sensor data", 5544);
	match_assert("c:\\halo\\SOURCE\\interface\\motion_sensor.c", 298, motion_sensor_globals);

	return;
}

void motion_sensor_dispose(
	void)
{
	return;
}

void motion_sensor_dispose_from_old_map(
	void)
{
	return;
}

static boolean should_track_object(
	long object_index)
{
	struct unit_datum *unit = unit_try_and_get(object_index);
	boolean result = FALSE;

	if (unit && !TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
		result = TRUE;

	return result;
}

/* January's only caller (render_motion_sensor) pushes a zero dword for the
 * second parameter and this body never reads it; its original name and
 * type are not recoverable (a long 0 and a real 0.0f emit identical bytes).
 * The parameter name is provisional and describes that evidence. */
static void blip_begin(
	real_point2d const *reference,
	long unused,
	boolean in_multiplayer,
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		841,
		reference);

	blip_player_index = local_player_index;
	scale[0] = in_multiplayer ? 0.75f : 1.0f;
	center_point = *reference;
	rasterizer_hud_motion_sensor_blip_begin();

	return;
}

static boolean should_draw_object(
	long object_index)
{
	boolean result = FALSE;

	if (unit_try_and_get(object_index) &&
		game_engine_draw_object_in_motion_sensor(object_index))
	{
		struct unit_datum *unit = unit_get(object_index);

		if (TEST_FLAG(
			unit->unit.control_flags,
			_unit_control_weapon_primary_trigger_bit) ||
			(unit->unit.grenade_throw_state != _unit_grenade_throw_idle &&
				unit->unit.grenade_throw_state != _unit_grenade_throw_ending))
		{
			result = TRUE;
		}
		else
		{
			real_vector3d velocity;
			boolean visible;
			boolean moving;

			object_get_velocities(object_index, &velocity, NULL);
			visible = (boolean)(
				game_engine_running() ||
				!TEST_FLAG(unit->unit.flags, _unit_active_camouflaged_bit));
			moving = (boolean)(
				magnitude_squared3d(&velocity) >=
					hud_globals->defaults.motion_sensor_velocity_sensitivity ||
				debug_motion_sensor_draw_all_units);
			result = (boolean)(visible && moving);
		}
	}

	return result;
}

static void render_blip(
	struct motion_sensor_datum *sensor,
	char blip_type,
	char blip_size_type,
	real_point2d const *point,
	real relative_scale,
	real fade,
	real radius)
{
	real_point2d blip_position = *point;
	real_rgb_color const *blip_color;
	real distance_squared;
	real distance;
	real mapped_distance;
	real distance_scale;
	real pulse_scale;
	real blip_draw_size;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\motion_sensor.c",
		410,
		_blip_type_none != blip_type);

	rotate_vector2d(
		(real_vector2d const *)&blip_position,
		sine(-sensor->yaw),
		cosine(-sensor->yaw),
		(real_vector2d *)&blip_position);

	distance_squared = magnitude_squared2d((real_vector2d const *)&blip_position);
	if (distance_squared <
		hud_globals->defaults.motion_sensor_range *
			hud_globals->defaults.motion_sensor_range)
	{
		distance = square_root(distance_squared);
		if (distance < 0.015625f)
			distance = 0.015625f;

		scale_vector2d(
			(real_vector2d const *)&blip_position,
			1.0f / distance,
			(real_vector2d *)&blip_position);
		distance_scale = (real)pow(
			distance / hud_globals->defaults.motion_sensor_range,
			0.7);
		mapped_distance =
			hud_globals->defaults.motion_sensor_range * distance_scale;
		scale_vector2d(
			(real_vector2d const *)&blip_position,
			mapped_distance,
			(real_vector2d *)&blip_position);
		scale_vector2d(
			(real_vector2d const *)&blip_position,
			relative_scale,
			(real_vector2d *)&blip_position);

		blip_color = &blip_colors[blip_type];
		pulse_scale = 1.0f;
		blip_draw_size = blip_size_get(blip_size_type);
		if (blip_type == _blip_type_custom)
		{
			pulse_scale = (real)(
				(sin((real)game_time_get() * 0.104719733f) + 1.0) *
					(1.0 / 3.0) +
				1.0);
		}

		blip_draw_size += pulse_scale * radius;
		rasterizer_hud_motion_sensor_blip_draw(
			&blip_position,
			fade,
			blip_draw_size,
			blip_color,
			(boolean)(blip_type == _blip_type_custom));
	}

	return;
}

void
motion_sensor_initialize_for_new_map(
	void)
{
	struct motion_sensor_player *player;
	long player_count;

	csmemset(motion_sensor_globals, 0, sizeof(*motion_sensor_globals));
#if defined(HALO_LINUX) || defined(HALO_N64)
	motion_sensor_globals->last_update_time = NONE;
#endif
	player = motion_sensor_globals->players;
	player_count = NUMBEROF(motion_sensor_globals->players);

	do
	{
		struct motion_sensor_datum *sensor = player->sensor_data;
		long sensor_count = NUMBEROF(player->sensor_data);

		do
		{
			struct motion_sensor_blip *blip = sensor->blips;
			long blip_count = NUMBEROF(sensor->blips);

			do
			{
				blip->type = _blip_type_none;
				blip++;
			}
			while (--blip_count);

			sensor++;
		}
		while (--sensor_count);

		player++;
	}
	while (--player_count);

	return;
}
