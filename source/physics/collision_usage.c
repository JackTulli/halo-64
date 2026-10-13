/*
COLLISION_USAGE.C

symbols in this file:
0013C830 0030:
	_collision_log_compare_overall_usage (0000)
0013C860 0060:
	_collision_log_initialize (0000)
0013C8C0 0010:
	_collision_log_enable (0000)
0013C8D0 0080:
	_code_0013c8d0 (0000)
0013C950 0020:
	_collision_log_begin_period (0000)
0013C970 0020:
	_collision_log_continue_period (0000)
0013C990 0070:
	_collision_log_end_period (0000)
0013CA00 0060:
	_collision_log_format_usage (0000)
0013CA60 0630:
	_collision_log_render (0000)
0013D090 0100:
	_collision_log_get_current_user (0000)
0013D190 0010:
	_collision_log_start_time (0000)
0013D1A0 0080:
	_collision_log_end_time (0000)
0013D220 0050:
	_collision_log_usage (0000)
0013D270 0060:
	_collision_log_display (0000)
0028B750 0006:
	??_C@_05GFCDIDHO@debug?$AA@ (0000)
0028B758 0003:
	??_C@_02OGLLFMLI@ui?$AA@ (0000)
0028B75C 0005:
	??_C@_04EMABALKC@limp?$AA@ (0000)
0028B764 0004:
	??_C@_03HDNHDHMH@veh?$AA@ (0000)
0028B768 0005:
	??_C@_04NNKAIPAK@proj?$AA@ (0000)
0028B770 0008:
	??_C@_07MNOKBFBP@pt?9phys?$AA@ (0000)
0028B778 0006:
	??_C@_05DPHKKMC@obsrv?$AA@ (0000)
0028B780 0008:
	??_C@_07NPPHEEOC@areadmg?$AA@ (0000)
0028B788 0009:
	??_C@_08PBPOKHFB@ai?9melee?$AA@ (0000)
0028B794 0008:
	??_C@_07DMCDIGNF@ai?9fire?$AA@ (0000)
0028B79C 0008:
	??_C@_07OALDIDM@ai?9comm?$AA@ (0000)
0028B7A4 0007:
	??_C@_06GPJBDMGE@ai?9los?$AA@ (0000)
0028B7AC 0008:
	??_C@_07NJIFNMAF@ai?9look?$AA@ (0000)
0028B7B4 0005:
	??_C@_04MFANEHND@?$DP?$DP?$DP?$DP?$AA@ (0000)
0028B7BC 0015:
	??_C@_0BF@HGKOAJPN@structure?9bsp?9sphere?$AA@ (0000)
0028B7D4 0012:
	??_C@_0BC@CFFMELFA@object?9bsp?9sphere?$AA@ (0000)
0028B7E8 0015:
	??_C@_0BF@PJIFAACM@structure?9bsp?9vector?$AA@ (0000)
0028B800 0012:
	??_C@_0BC@KKHHECIB@object?9bsp?9vector?$AA@ (0000)
0028B814 000d:
	??_C@_0N@FPLJAJFK@model?9vector?$AA@ (0000)
0028B824 0013:
	??_C@_0BD@PLJPFLJA@features?9in?9sphere?$AA@ (0000)
0028B838 000f:
	??_C@_0P@EBIJMEDI@vector?9objects?$AA@ (0000)
0028B848 0011:
	??_C@_0BB@MLMKGLBG@vector?9structure?$AA@ (0000)
0028B85C 0029:
	??_C@_0CJ@GINJHFMK@c?3?2halo?2SOURCE?2physics?2collision@ (0000)
0028B888 0047:
	??_C@_0EH@MHCPNMCF@?$CItime_period?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CItime_peri@ (0000)
0028B8D0 0027:
	??_C@_0CH@NAJCGNL@collision_usage_current_period?5?$DN@ (0000)
0028B8F8 006d:
	??_C@_0GN@KBNMKOHA@?$CIcollision_usage_current_period?5@ (0000)
0028B968 0008:
	??_C@_07PPMMBGBH@?$CFd?1?$CF?42f?$AA@ (0000)
0028B970 0043:
	??_C@_0ED@GBKMDDBI@?$CIuser_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIuser_index@ (0000)
0028B9B4 0005:
	??_C@_04CDPLFOEF@?$CFc?$CFs?$AA@ (0000)
0028B9C0 0057:
	??_C@_0FH@FIPBMPPP@?$CIcollision_function?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIco@ (0000)
0028BA18 0037:
	??_C@_0DH@BEMKFPGN@?$CIuser?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIuser?5?$DM?5NUMBER_OF@ (0000)
0028BA50 0028:
	??_C@_0CI@PANLECBF@global_current_collision_user_de@ (0000)
0028BA78 003b:
	??_C@_0DL@IIDCDOP@sphere?5?$CF?53df?5?$CF?53db?0?5str?9vec?5?$CF?53d@ (0000)
0030CD58 008a:
	_global_collision_function_names (0000)
	_global_collision_user_names (0028)
	_global_collision_log_enable (0084)
	_collision_usage_current_period (0088)
0045DFF0 000a:
	_collision_log_render_enable (0000)
	_collision_log_detailed (0001)
	_collision_log_extended (0002)
	_collision_log_totals_only (0003)
	_collision_log_time (0004)
	_global_current_collision_user_depth (0008)
*/

/* ---------- headers */

#include "cseries.h"
#include "collision_debug.h"
#include "collision_usage.h"
#include "editor/editor_stubs.h"
#include "game/game.h"
#include "interface/interface.h"
#include "math/integer_math.h"
#include "rasterizer/rasterizer.h"
#include "text/draw_string.h"

/* ---------- constants */

enum
{
	NUMBER_OF_COLLISION_TIME_PERIODS = 3
};

/* ---------- macros */

/* ---------- structures */

struct collision_log
{
	long calls;
	__int64 elapsed_time;
};

struct collision_function
{
	struct collision_log total_all_users;
	struct collision_log usage_by_user[NUMBER_OF_COLLISION_USER_TYPES];
};

struct collision_period
{
	boolean reset_upon_next_use;
	boolean valid;
	long period_count;
	struct collision_function function[NUMBER_OF_COLLISION_FUNCTION_TYPES];
};

struct collision_overall_usage
{
	short user_index;
	struct collision_log total_all_periods;
	struct collision_log usage_by_period[NUMBER_OF_COLLISION_TIME_PERIODS];
};

/* ---------- prototypes */

/* ---------- globals */

char const *global_collision_function_names[] =
{
	"vector-structure",
	"vector-objects",
	"features-in-sphere",
	"model-vector",
	"object-bsp-vector",
	"structure-bsp-vector",
	"object-bsp-sphere",
	"structure-bsp-sphere",
	NULL
};

char const *global_collision_user_names[] =
{
	"????",
	"ai-look",
	"ai-los",
	"ai-comm",
	"ai-fire",
	"ai-melee",
	"aim",
	"biped",
	"melee",
	"decal",
	"areadmg",
	"item",
	"obsrv",
	"pt-phys",
	"proj",
	"light",
	"sound",
	"veh",
	"limp",
	"object",
	"ui",
	"debug",
	NULL
};

boolean global_collision_log_enable = TRUE;
short collision_usage_current_period = NONE;

boolean collision_log_render_enable = FALSE;
boolean collision_log_detailed = FALSE;
boolean collision_log_extended = FALSE;
boolean collision_log_totals_only = FALSE;
boolean collision_log_time = FALSE;

short global_current_collision_user_depth = 0;

short global_current_collision_users[MAXIMUM_COLLISION_USER_STACK_DEPTH];

struct collision_period collision_usage_buffer[NUMBER_OF_COLLISION_TIME_PERIODS];
struct collision_period collision_usage_current;

/* ---------- public code */

static int collision_log_compare_overall_usage(
	void const *left_pointer,
	void const *right_pointer)
{
	struct collision_overall_usage const *left = left_pointer;
	struct collision_overall_usage const *right = right_pointer;
	long left_calls = left->total_all_periods.calls;
	long right_calls = right->total_all_periods.calls;

	if (left_calls > right_calls)
	{
		return -1;
	}

	return left_calls < right_calls;
}

void collision_log_initialize(
	void)
{
	memset(&collision_usage_buffer, 0, sizeof(collision_usage_buffer));

	match_assert("c:\\halo\\SOURCE\\physics\\collision_usage.c", 150, global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);

	global_current_collision_users[global_current_collision_user_depth] = 0;
	global_current_collision_user_depth = global_current_collision_user_depth + 1;

	return;
}

void collision_log_enable(
	boolean enable)
{
	global_collision_log_enable = enable;

	return;
}

static void collision_log_store_period(
	short time_period,
	boolean unused)
{
#ifdef HALO_N64
	/* the collision log feeds only a debug display: off on the N64 */
	return;
#endif
	match_assert("c:\\halo\\SOURCE\\physics\\collision_usage.c", 167, collision_usage_current_period == NONE);
	match_assert("c:\\halo\\SOURCE\\physics\\collision_usage.c", 168, (time_period >= 0) && (time_period < NUMBER_OF_COLLISION_TIME_PERIODS));

	memset(&collision_usage_current, 0, sizeof(collision_usage_current));
	collision_usage_current_period = time_period;

	return;
}

void collision_log_begin_period(
	short time_period)
{
	collision_log_store_period(time_period, TRUE);

	return;
}

void collision_log_continue_period(
	short time_period)
{
	collision_log_store_period(time_period, FALSE);

	return;
}

void collision_log_end_period(
	void)
{
#ifdef HALO_N64
	return;
#endif
	match_assert("c:\\halo\\SOURCE\\physics\\collision_usage.c", 198,
		(collision_usage_current_period >= 0) && (collision_usage_current_period < NUMBER_OF_COLLISION_TIME_PERIODS));

	collision_usage_current.reset_upon_next_use = TRUE;

	collision_usage_buffer[collision_usage_current_period] = collision_usage_current;
	collision_usage_current_period = NONE;

	return;
}

static void collision_log_format_usage(
	char *buffer,
	struct collision_log const *usage)
{
	if (collision_log_time)
	{
		LARGE_INTEGER frequency;

		QueryPerformanceFrequency(&frequency);
		sprintf(
			buffer,
			"%d/%.2f",
			usage->calls,
			usage->elapsed_time * 1000.0f / frequency.QuadPart);

		return;
	}

	sprintf(buffer, "%d", usage->calls);

	return;
}

void collision_log_render(
	void)
{
	if (collision_log_render_enable)
	{
		char line_buffer[2048];
		short function_index;
		short debug_string_position = rasterizer_globals.reserved04.frame_bounds.y1 - 30;

		for (function_index = 0; function_index < NUMBER_OF_COLLISION_FUNCTION_TYPES; function_index++)
		{
			boolean recorded = FALSE;
			short time_period_index;

			if (function_index < 3 || collision_log_extended)
			{
				for (time_period_index = 0; time_period_index < NUMBER_OF_COLLISION_TIME_PERIODS; time_period_index++)
				{
					if (collision_usage_buffer[time_period_index].reset_upon_next_use &&
						collision_usage_buffer[time_period_index].function[function_index].total_all_users.calls > 0)
					{
						recorded = TRUE;
					}
				}

				if (recorded)
				{
					struct collision_overall_usage total_usage;
					struct collision_overall_usage overall_usage[NUMBER_OF_COLLISION_USER_TYPES];

					memset(&total_usage, 0, sizeof(total_usage));
					memset(overall_usage, 0, sizeof(overall_usage));

					{
						short user_index;

						for (user_index = 0; user_index < NUMBER_OF_COLLISION_USER_TYPES; user_index++)
						{
							overall_usage[user_index].user_index = user_index;
							if (user_index != _collision_user_debugging || collision_log_extended)
							{
								for (time_period_index = 0; time_period_index < NUMBER_OF_COLLISION_TIME_PERIODS; time_period_index++)
								{
									if (collision_usage_buffer[time_period_index].reset_upon_next_use)
									{
										overall_usage[user_index].usage_by_period[time_period_index] =
											collision_usage_buffer[time_period_index].function[function_index].usage_by_user[user_index];
										total_usage.usage_by_period[time_period_index].calls +=
											collision_usage_buffer[time_period_index].function[function_index].usage_by_user[user_index].calls;
										total_usage.usage_by_period[time_period_index].elapsed_time +=
											collision_usage_buffer[time_period_index].function[function_index].usage_by_user[user_index].elapsed_time;
									}

									overall_usage[user_index].total_all_periods.calls +=
										overall_usage[user_index].usage_by_period[time_period_index].calls;
									overall_usage[user_index].total_all_periods.elapsed_time +=
										overall_usage[user_index].usage_by_period[time_period_index].elapsed_time;
									total_usage.total_all_periods.calls +=
										overall_usage[user_index].usage_by_period[time_period_index].calls;
									total_usage.total_all_periods.elapsed_time +=
										overall_usage[user_index].usage_by_period[time_period_index].elapsed_time;
								}
							}
						}
					}

					qsort(
						overall_usage,
						NUMBEROF(overall_usage),
						sizeof(overall_usage[0]),
						collision_log_compare_overall_usage);
					sprintf(line_buffer, "%s:", global_collision_function_names[function_index]);

					if (collision_log_totals_only)
					{
						char temporary_string[512];

						csstrcpy(temporary_string, " ");
						if (collision_log_detailed)
						{
							short index;

							for (index = 0; index < NUMBER_OF_COLLISION_TIME_PERIODS; index++)
							{
								char temporary_buffer[256];
								short length = (short)csstrlen(temporary_string);

								collision_log_format_usage(temporary_buffer, &total_usage.usage_by_period[index]);
								_snprintf(
									temporary_string + length,
									sizeof(temporary_string) - length,
									"%c%s",
									index ? '/' : ' ',
									temporary_buffer);
							}
						}
						else
						{
							char temporary_buffer[256];
							short length = (short)csstrlen(temporary_string);

							collision_log_format_usage(temporary_buffer, &total_usage.total_all_periods);
							_snprintf(
								temporary_string + length,
								sizeof(temporary_string) - length,
								" %s",
								temporary_buffer);
						}

						csstrcat(line_buffer, temporary_string);
					}
					else
					{
						short index;

						for (index = 0; index < 6; index++)
						{
							short user_index = overall_usage[index].user_index;

							match_assert(
								"c:\\halo\\SOURCE\\physics\\collision_usage.c",
								342,
								(user_index >= 0) && (user_index < NUMBER_OF_COLLISION_USER_TYPES));

							if (overall_usage[index].total_all_periods.calls > 0)
							{
								char temporary_string[512];

								_snprintf(
									temporary_string,
									sizeof(temporary_string),
									" %s",
									global_collision_user_names[user_index]);

								if (collision_log_detailed)
								{
									short time_period_index;

									for (time_period_index = 0; time_period_index < NUMBER_OF_COLLISION_TIME_PERIODS; time_period_index++)
									{
										char temporary_buffer[256];
										short length = (short)csstrlen(temporary_string);

										collision_log_format_usage(
											temporary_buffer,
											&overall_usage[index].usage_by_period[time_period_index]);
										_snprintf(
											temporary_string + length,
											sizeof(temporary_string) - length,
											"%c%s",
											time_period_index ? '/' : ' ',
											temporary_buffer);
									}
								}
								else
								{
									char temporary_buffer[256];
									short length = (short)csstrlen(temporary_string);

									collision_log_format_usage(
										temporary_buffer,
										&overall_usage[index].total_all_periods);
									_snprintf(
										temporary_string + length,
										sizeof(temporary_string) - length,
										" %s",
										temporary_buffer);
								}

								csstrcat(line_buffer, temporary_string);
							}
						}
					}

					{
						rectangle2d bounds;
						point2d cursor;

						bounds.y0 = debug_string_position;
						bounds.x0 = rasterizer_globals.reserved04.frame_bounds.x0;
						bounds.x1 = SHORT_MAX;
						bounds.y1 = SHORT_MAX;

						interface_set_bitmap_text_draw_mode(
							_interface_font_terminal,
							NONE,
							0,
							0,
							_interface_color_table_dialog,
							0);
						draw_string_set_color(global_real_argb_white);
						draw_string_set_tab_stops(NULL, 0);
						rasterizer_draw_string(&bounds, NULL, &cursor, 0, line_buffer);

						debug_string_position += bounds.y0 - cursor.y;
					}
				}
			}
		}
	}

	return;
}

static short collision_log_get_current_user(
	short collision_function)
{
	short user;

	match_assert(
		"c:\\halo\\SOURCE\\physics\\collision_usage.c",
		403,
		global_current_collision_user_depth > 0);

	user = global_current_collision_users[global_current_collision_user_depth - 1];
	match_assert(
		"c:\\halo\\SOURCE\\physics\\collision_usage.c",
		406,
		(user >= 0) && (user < NUMBER_OF_COLLISION_USER_TYPES));
	match_assert(
		"c:\\halo\\SOURCE\\physics\\collision_usage.c",
		407,
		(collision_function >= 0) &&
			(collision_function < NUMBER_OF_COLLISION_FUNCTION_TYPES));

	if (!game_in_progress() || game_in_editor() || !global_collision_log_enable)
	{
		user = NONE;
	}
	else if (collision_usage_current_period == NONE)
	{
		user = NONE;
	}
	else
	{
		match_assert(
			"c:\\halo\\SOURCE\\physics\\collision_usage.c",
			424,
			(collision_usage_current_period >= 0) &&
				(collision_usage_current_period < NUMBER_OF_COLLISION_TIME_PERIODS));
	}

	return user;
}

void collision_log_start_time(
	LARGE_INTEGER *start_time)
{
#if defined(HALO_XBOX) || defined(HALO_N64)
	/* the console: the counts and timings feed only the collision display
	(a debug key), and cost two clock reads around every collision query */
	if (!collision_log_render_enable)
	{
		start_time->QuadPart = 0;
		return;
	}
#endif
	QueryPerformanceCounter(start_time);

	return;
}

void collision_log_end_time(
	short collision_function,
	__int64 start_time)
{
	LARGE_INTEGER end_time;
	short user;

#if defined(HALO_XBOX) || defined(HALO_N64)
	if (!collision_log_render_enable)
		return;
#endif
	QueryPerformanceCounter(&end_time);
	user = collision_log_get_current_user(collision_function);
	if (user != NONE)
	{
		__int64 elapsed_time = end_time.QuadPart - start_time;

		collision_usage_current.function[collision_function]
			.total_all_users.elapsed_time += elapsed_time;
		collision_usage_current.function[collision_function]
			.usage_by_user[user].elapsed_time += elapsed_time;
	}

	return;
}

void collision_log_usage(
	short collision_function)
{
#ifdef HALO_N64
	return;
#endif
	short user;

#ifdef HALO_XBOX
	if (!collision_log_render_enable)
		return;
#endif
	user = collision_log_get_current_user(collision_function);
	if (user != NONE)
	{
		collision_usage_current.function[collision_function]
			.total_all_users.calls++;
		collision_usage_current.function[collision_function]
			.usage_by_user[user].calls++;
	}

	return;
}

void collision_log_display(
	char *buffer)
{
	if (collision_usage_buffer[0].reset_upon_next_use)
	{
		sprintf(
			buffer + strlen(buffer),
			"sphere % 3df % 3db, str-vec % 3d/% 3d, obj-vec % 3d/% 3d|n",
			collision_usage_buffer[0]
				.function[_collision_function_vector_bounds_object]
				.total_all_users.calls,
			collision_usage_buffer[0]
				.function[_collision_function_sphere_intersect_bsp_structure]
				.total_all_users.calls +
				collision_usage_buffer[0]
					.function[_collision_function_sphere_intersect_bsp_object]
					.total_all_users.calls,
			collision_usage_buffer[0]
				.function[_collision_function_vector_structure]
				.total_all_users.calls,
			collision_usage_buffer[0]
				.function[_collision_function_vector_intersect_bsp_structure]
				.total_all_users.calls,
			collision_usage_buffer[0]
				.function[_collision_function_vector_objects]
				.total_all_users.calls,
			collision_usage_buffer[0]
				.function[_collision_function_vector_intersect_bsp_object]
				.total_all_users.calls);
	}

	return;
}

/* ---------- private code */
