/*
RASTERIZER_FRAME_STATISTICS.C

symbols in this file:
0016E3A0 0040:
	_rasterizer_frame_statistics_initialize (0000)
0016E3E0 0020:
	_rasterizer_frame_statistics_begin (0000)
0016E400 0180:
	_rasterizer_frame_statistics_get_fps (0000)
0016E580 0040:
	_rasterizer_fps_accumulate (0000)
0016E5C0 0020:
	_eat_my_shorts (0000)
0016E5E0 0040:
	_rasterizer_frame_statistics_count_static_vertices (0000)
0016E620 0130:
	_rasterizer_frame_statistics_count_dynamic_vertices (0000)
0016E750 1050:
	_rasterizer_frame_statistics_draw (0000)
0016F7A0 0010:
	_rasterizer_frame_statistics_end (0000)
0016F7B0 0020:
	_rasterizer_frame_statistics_dispose (0000)
0029DC30 000e:
	??_C@_0O@JFBHNPBL@d?3?2r?9prof?4txt?$AA@ (0000)
0029DC40 0018:
	??_C@_0BI@GCMBLDJP@?$CD?$CD?$CD?5ERROR?5out?5of?5memory?$AA@ (0000)
0029DC58 0038:
	??_C@_0DI@DAFJELOG@c?3?2halo?2SOURCE?2rasterizer?2raster@ (0000)
0029DC90 0028:
	??_C@_0CI@LGKHNHMC@rasterizer_frame_statistics_temp@ (0000)
0029DCB8 0040:
	??_C@_0EA@PGBNPAHI@triangle_count?$DMRASTERIZER_MAXIMU@ (0000)
0029DCF8 0024:
	??_C@_0CE@KDLIJAEE@average?5total?5pushbuffer?$DN?5?$CFd?5byt@ (0000)
0029DD1C 0026:
	??_C@_0CG@DFFGIJGJ@average?5total?5frame?5time?$DN?5?$CF?42f?5m@ (0000)
0029DD44 0007:
	??_C@_06NBDEAPCB@?5?5?9?9?9?9?$AA@ (0000)
0029DD4C 0006:
	??_C@_05GDJIKCCN@?$CF6?42f?$AA@ (0000)
0029DD54 0002:
	??_C@_01LFCBOECM@?4?$AA@ (0000)
0029DD58 0035:
	??_C@_0DF@GMJOLAAA@?$CD?$CD?$CD?5ERROR?5failed?5to?5open?5rasteri@ (0000)
0029DD90 0019:
	??_C@_0BJ@JJFENEFI@?$HMtsystem?5available?$HMt?$CFdKb?$AA@ (0000)
0029DDAC 0017:
	??_C@_0BH@EJIDKNJD@?$HMn?$HMtsystem?5total?$HMt?$CFdKb?$AA@ (0000)
0029DDC4 0013:
	??_C@_0BD@JJMMIDNN@?$HMn?$HMttotal?$HMt?$CFd?5?$CI?$CFd?$CJ?$AA@ (0000)
0029DDD8 0009:
	??_C@_08NACADNAD@?$HMt?$CFs?$HMt?$CFd?$AA@ (0000)
0029DDE4 0024:
	??_C@_0CE@GPLJMHJA@vertex?5shaders?$HMt?$HO35k?5last?5i?5chec@ (0000)
0029DE08 0010:
	??_C@_0BA@PIMAGLFH@debug?5geometry?$CK?$AA@ (0000)
0029DE18 0017:
	??_C@_0BH@OHBLOMMO@motion?5sensor?5buffers?$CK?$AA@ (0000)
0029DE30 000e:
	??_C@_0O@LPNPGMEL@water?5buffers?$AA@ (0000)
0029DE40 0012:
	??_C@_0BC@MBLMGAF@sun?5glow?5buffers?$CK?$AA@ (0000)
0029DE54 000f:
	??_C@_0P@JGCOFABI@shadow?5buffers?$AA@ (0000)
0029DE64 0024:
	??_C@_0CE@GGJOAJBB@mirror?5buffers?5?$CIincludes?5z?9buffe@ (0000)
0029DE88 0011:
	??_C@_0BB@IAJCMCIG@bump?5map?5palette?$AA@ (0000)
0029DE9C 001c:
	??_C@_0BM@KDCCBLNC@transparent?5geometry?5groups?$AA@ (0000)
0029DEB8 0012:
	??_C@_0BC@BJBCACME@dynamic?5triangles?$AA@ (0000)
0029DECC 0022:
	??_C@_0CC@LDJJCNJE@dynamic?5vertices?5?$CIdetail?5objects@ (0000)
0029DEF0 0019:
	??_C@_0BJ@MNAIOGBM@dynamic?5vertices?5?$CImodel?$CJ?$AA@ (0000)
0029DF0C 001a:
	??_C@_0BK@CDEJLHGG@dynamic?5vertices?5?$CIscreen?$CJ?$AA@ (0000)
0029DF28 0018:
	??_C@_0BI@HPJPBHLO@dynamic?5vertices?5?$CIlit?$CK?$CJ?$AA@ (0000)
0029DF40 0019:
	??_C@_0BJ@DKPGOBKB@dynamic?5vertices?5?$CIunlit?$CJ?$AA@ (0000)
0029DF5C 000c:
	??_C@_0M@DEOKAGLG@memory?5pool?$AA@ (0000)
0029DF68 0023:
	??_C@_0CD@ELLHHMII@?$HMtallocation?$HMtmemory?5usage?5?$CIbyte@ (0000)
0029DF8C 0014:
	??_C@_0BE@ELPADKKO@?$HMttotal?$HMt?$CF?42f?$HMt?$CFd?$HMn?$AA@ (0000)
0029DFA0 000e:
	??_C@_0O@NBNGOGEK@?$HMt?$CFs?$HMt?9?9?9?9?$HMt0?$AA@ (0000)
0029DFB0 000f:
	??_C@_0P@EMPKACCD@?$HMt?$CFs?$HMt?$CF?42f?$HMt?$CFd?$AA@ (0000)
0029DFC0 002a:
	??_C@_0CK@PDOKMEKL@?$HMtGPU?5profile?$HMttime?5?$CImsecs?$CJ?$HMtdat@ (0000)
0029DFEC 0028:
	??_C@_0CI@OJALLPNK@?$HMt?$CFd?5dynamic?5lights?$HMn?$HMt?$CFd?5lens?5f@ (0000)
0029E014 0023:
	??_C@_0CD@DPCNBBDK@?$HMtdynamic?5geometry?$HMt?$CFd?1?$CFd?$HMt?$CFd?1?$CFd@ (0000)
0029E038 0017:
	??_C@_0BH@JLJHFKOG@?$HMtdecals?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$HMn?$AA@ (0000)
0029E050 0034:
	??_C@_0DE@DDLABOMO@?$HMtsolid?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$HMn?$HMttranspare@ (0000)
0029E084 001a:
	??_C@_0BK@IOKCDMIA@?$HMtmodels?5?$CI?$CFd?$CJ?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$AA@ (0000)
0029E0A0 0021:
	??_C@_0CB@EOMGBMNP@?$HMtmodel?5shadows?5?$CI?$CFd?$CJ?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd@ (0000)
0029E0C8 0113:
	??_C@_0BBD@GCJAAPME@?$HMtlightmaps?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$HMn?$HMtshado@ (0000)
0029E1DC 001a:
	??_C@_0BK@KOANHEHC@?$HMtenvironment?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$AA@ (0000)
0029E1F8 0016:
	??_C@_0BG@GAEDMNLC@?$HMttotal?$HMt?$CFd?$HMt?$CFd?$HMt?$CFd?$HMn?$AA@ (0000)
0029E210 0024:
	??_C@_0CE@EIBEMOMJ@?$HMt?$HMtvertices?$HMttriangles?$HMtprimiti@ (0000)
0029E234 0038:
	??_C@_0DI@JADDMGFO@?$HMtlocal_player_count?$HMt?$CFd?$HMn?$HMtmain@ (0000)
0029E26C 0037:
	??_C@_0DH@JFMJJCIO@?$HMtskinning?$HMt?$CFd?$HMn?$HMtlighting?$HMt?$CFd?$HMn@ (0000)
0029E2A4 0038:
	??_C@_0DI@GOEPEHGF@?$HMtfogged?$HMt?$CFd?$HMn?$HMtnormal?$HMt?$CFd?$HMn?$HMtfa@ (0000)
0029E2DC 001b:
	??_C@_0BL@IBKJHKIF@?$HMt?$CF?40f?$HMt?$CF?40f?$HMt?$CF?40f?$HMt?$CF?40f?$HMn?$AA@ (0000)
0029E2F8 0020:
	??_C@_0CA@DPIIOMPF@?$HMt?$CF?40f?$HMt?$CF?40f?1?$CF?40f?$HMt?$CF?40f?$HMt?$CF?40f?$HMn?$AA@ (0000)
0029E318 0029:
	??_C@_0CJ@FFAAFLA@?$HMn?$HMtframerate?$HMtaverage?5?$CIof?5?$CFd?$CJ?$HMt@ (0000)
0030D4CC 0004:
	_profile_log_path (0000)
00466320 0860:
	_bss_00466320 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/sort.h"
#include "cseries/cseries_windows.h"
#include "errors.h"
#include "game/players.h"
#include "interface/interface.h"
#include "main/main_runtime.h"
#include "math/integer_math.h"
#include "rasterizer.h"
#include "rasterizer_debug_options.h"
#include "rasterizer_frame_statistics.h"
#include "rasterizer_geometry.h"
#include "rasterizer/xbox/rasterizer_xbox.h"
#include "text/draw_string.h"

/* ---------- constants */

enum
{
	MAXIMUM_FPS_SAMPLE_COUNT = 60,
	MAXIMUM_PROFILE_ACCUMULATION_COUNT = 16,
	NUMBER_OF_STATISTICS_TAB_STOPS = 6,
	NUMBER_OF_MEMORY_USAGE_REPORTS = 16,
	STATISTICS_TEXT_BUFFER_SIZE = 12288
};

enum
{
	_rasterizer_statistics_mode_none = 0,
	_rasterizer_statistics_mode_objects,
	_rasterizer_statistics_mode_geometry,
	_rasterizer_statistics_mode_profile,
	_rasterizer_statistics_mode_memory
};

/* ---------- macros */

/* ---------- structures */

struct rasterizer_memory_usage_report
{
	char const *name;
	long allocation;
	long shared;
};

struct rasterizer_frame_statistics_private_globals_definition
{
	real profile_times[NUMBER_OF_RASTERIZER_PROFILES][MAXIMUM_PROFILE_ACCUMULATION_COUNT];
	word *temp_buffer;
	unsigned __int64 fps_accumulation_time;
	unsigned __int64 fps_accumulation_frame_index;
	FILE *profile_log_file;
	byte reserved75C[4];
	unsigned long fps_sample_times[MAXIMUM_FPS_SAMPLE_COUNT];
};

typedef char verify_rasterizer_frame_statistics_temp_buffer_offset[
	offsetof(
		struct rasterizer_frame_statistics_private_globals_definition,
		temp_buffer) == 0x740 ? 1 : -1];
typedef char verify_rasterizer_frame_statistics_accumulation_time_offset[
	offsetof(
		struct rasterizer_frame_statistics_private_globals_definition,
		fps_accumulation_time) == 0x748 ? 1 : -1];
typedef char verify_rasterizer_frame_statistics_accumulation_frame_index_offset[
	offsetof(
		struct rasterizer_frame_statistics_private_globals_definition,
		fps_accumulation_frame_index) == 0x750 ? 1 : -1];
typedef char verify_rasterizer_frame_statistics_profile_log_file_offset[
	offsetof(
		struct rasterizer_frame_statistics_private_globals_definition,
		profile_log_file) == 0x758 ? 1 : -1];
typedef char verify_rasterizer_frame_statistics_fps_sample_times_offset[
	offsetof(
		struct rasterizer_frame_statistics_private_globals_definition,
		fps_sample_times) == 0x760 ? 1 : -1];
typedef char verify_rasterizer_frame_statistics_private_globals_size[
	sizeof(struct rasterizer_frame_statistics_private_globals_definition) == 0x850 ? 1 : -1];

/* ---------- prototypes */

static boolean eat_my_shorts(
	word first,
	word second);

/* ---------- globals */

static struct rasterizer_frame_statistics_private_globals_definition rasterizer_frame_statistics_private_globals = {0};

/* the FPS sample count and the profile-log accumulators are separate file-scope statics, not
   members of the private record: get_fps reloads fps_sample_count from memory after the
   sample-shift loop, which VC7 only does when the loop's array stores cannot alias it */
static short fps_sample_count = 0;
static short profile_accumulation_index = 0;
static real profile_accumulated_time = 0.0f;
static long profile_accumulated_pushbuffer_size = 0;

char const *profile_log_path = "d:\\r-prof.txt";

#define rasterizer_frame_statistics_temp_buffer rasterizer_frame_statistics_private_globals.temp_buffer
#define rasterizer_fps_sample_times rasterizer_frame_statistics_private_globals.fps_sample_times

/* ---------- public code */

boolean rasterizer_frame_statistics_initialize(
	void)
{
	boolean success = TRUE;

	rasterizer_frame_statistics_temp_buffer = match_malloc(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_frame_statistics.c",
		41,
		sizeof(word) *
			RASTERIZER_MAXIMUM_TRIANGLES_PER_TRIANGLE_BUFFER *
			NUMBER_OF_VERTICES_PER_TRIANGLE);

	if (!rasterizer_frame_statistics_temp_buffer)
	{
		error(_error_silent, "### ERROR out of memory");
		success = FALSE;
	}

	return success;
}

void rasterizer_frame_statistics_begin(
	void)
{
	memset(&rasterizer_frame_statistics, 0, sizeof(rasterizer_frame_statistics));

	return;
}

void rasterizer_frame_statistics_get_fps(
	struct rasterizer_frame_statistics_globals *frame_statistics)
{
	if (rasterizer_debug_options.stats && frame_statistics)
	{
		unsigned long current_time = system_milliseconds();
		short sample_count = fps_sample_count;

		if (sample_count)
		{
			unsigned long minimum_frame_time = current_time - rasterizer_fps_sample_times[0];
			unsigned long maximum_frame_time = current_time - rasterizer_fps_sample_times[0];
			unsigned long window_time;
			short index;

			for (index = fps_sample_count - 1; index > 0; index--)
			{
				if (index > 1)
				{
					unsigned long frame_time =
						rasterizer_fps_sample_times[index - 1] - rasterizer_fps_sample_times[index];

					minimum_frame_time = MIN(frame_time, minimum_frame_time);
					maximum_frame_time = MAX(frame_time, maximum_frame_time);
				}
				rasterizer_fps_sample_times[index] = rasterizer_fps_sample_times[index - 1];
			}

			window_time = current_time - rasterizer_fps_sample_times[0];
			frame_statistics->frames_per_second = 1000.0f / (real)MAX(window_time, 1);
			frame_statistics->fps_sample_count = sample_count;

			window_time = current_time - rasterizer_fps_sample_times[sample_count - 1];
			frame_statistics->average_frames_per_second =
				(real)sample_count * 1000.0f / (real)MAX(window_time, 1);

			frame_statistics->maximum_frames_per_second =
				1000.0f / (real)MAX(minimum_frame_time, 1);
			frame_statistics->minimum_frames_per_second =
				1000.0f / (real)MAX(maximum_frame_time, 1);
		}

		rasterizer_fps_sample_times[0] = current_time;
		fps_sample_count =
			(short)MIN(sample_count + 1, MAXIMUM_FPS_SAMPLE_COUNT);
	}
	else
	{
		fps_sample_count = 0;
	}

	return;
}

void rasterizer_fps_accumulate(
	void)
{
	rasterizer_debug_options.fps_accumulation = TRUE;
	rasterizer_frame_statistics_private_globals.fps_accumulation_time = system_milliseconds();
	rasterizer_frame_statistics_private_globals.fps_accumulation_frame_index =
		rasterizer_globals.fps_accumulation_frame_index;

	return;
}

static boolean eat_my_shorts(
	word first,
	word second)
{
	if (first > second)
	{
		return TRUE;
	}
	else if (first < second)
	{
		return FALSE;
	}

	return FALSE;
}

long rasterizer_frame_statistics_count_static_vertices(
	struct triangle_buffer const *triangle_buffer,
	struct vertex_buffer const *vertex_buffer)
{
	long vertex_count = 0;

	if (triangle_buffer && vertex_buffer)
	{
		if (triangle_buffer->type == _triangle_buffer_type_precompiled_strip)
		{
			vertex_count = triangle_buffer->count + 2;
		}
		else if (triangle_buffer->type == _triangle_buffer_type_triangles)
		{
			if (vertex_buffer)
			{
				vertex_count = vertex_buffer->count;
			}
			else
			{
				long index_count = triangle_buffer->count * NUMBER_OF_VERTICES_PER_TRIANGLE;
				long index;
				word last_vertex_index = (word)NONE;

				memcpy(
					rasterizer_frame_statistics_temp_buffer,
					triangle_buffer->base_address,
					sizeof(word) * triangle_buffer->count * NUMBER_OF_VERTICES_PER_TRIANGLE);
				qsort_2byte(rasterizer_frame_statistics_temp_buffer, index_count, eat_my_shorts);
				for (index = 0; index < index_count; index++)
				{
					word vertex_index = rasterizer_frame_statistics_temp_buffer[index];

					if (last_vertex_index != vertex_index)
					{
						last_vertex_index = vertex_index;
						vertex_count++;
					}
				}
			}
		}
	}

	return vertex_count;
}

long rasterizer_frame_statistics_count_dynamic_vertices(
	long dynamic_triangle_buffer_index,
	long first_triangle_index,
	long triangle_count)
{
	long vertex_count = 0;

	if (dynamic_triangle_buffer_index >= 0)
	{
		short *triangles = rasterizer_dynamic_triangles_lock(dynamic_triangle_buffer_index);

		if (triangles)
		{
			long index_count = triangle_count * NUMBER_OF_VERTICES_PER_TRIANGLE;
			word last_vertex_index = (word)NONE;
			long index;

			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\rasterizer_frame_statistics.c",
				217,
				triangle_count<RASTERIZER_MAXIMUM_TRIANGLES_PER_TRIANGLE_BUFFER);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\rasterizer_frame_statistics.c",
				218,
				rasterizer_frame_statistics_temp_buffer);
			memcpy(
				rasterizer_frame_statistics_temp_buffer,
				triangles + first_triangle_index * NUMBER_OF_VERTICES_PER_TRIANGLE,
				sizeof(word) * triangle_count * NUMBER_OF_VERTICES_PER_TRIANGLE);
			qsort_2byte(rasterizer_frame_statistics_temp_buffer, index_count, eat_my_shorts);
			for (index = 0; index < index_count; index++)
			{
				word vertex_index = rasterizer_frame_statistics_temp_buffer[index];

				if (last_vertex_index != vertex_index)
				{
					last_vertex_index = vertex_index;
					vertex_count++;
				}
			}
			rasterizer_dynamic_triangles_unlock(dynamic_triangle_buffer_index);
		}
	}
	else
	{
		short vertices_per_primitive = (short)-dynamic_triangle_buffer_index;

		if (vertices_per_primitive == NUMBER_OF_VERTICES_PER_TRIANGLE ||
			vertices_per_primitive == NUMBER_OF_VERTICES_PER_QUADRILATERAL)
		{
			vertex_count = triangle_count / (vertices_per_primitive - 2);
		}
		else
		{
			vertex_count = vertices_per_primitive;
		}
	}

	return vertex_count;
}

void rasterizer_frame_statistics_draw(
	void)
{
	if (rasterizer_debug_options.stats)
	{
		char string[STATISTICS_TEXT_BUFFER_SIZE];
		point2d cursor = { 0, 0 };
		short height_adjust = -4;
		short line_advance = -1;
		short left = rasterizer_globals.reserved04.frame_bounds.x0;
		short tab_stops[NUMBER_OF_STATISTICS_TAB_STOPS] = { 100, 200, 300, 400, 500, 600 };
		real_argb_color data_color = { 1.0f, 0.66f, 1.0f, 0.66f };
		real_argb_color header_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		real_argb_color default_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		long environment_vertices =
			rasterizer_frame_statistics.lightmap_dynamic_vertex_count +
			rasterizer_frame_statistics.shadow_vertex_count +
			rasterizer_frame_statistics.environment_dynamic_vertex_count +
			rasterizer_frame_statistics.diffuse_texture_dynamic_vertex_count +
			rasterizer_frame_statistics.specular_light_dynamic_vertex_count +
			rasterizer_frame_statistics.specular_lightmap_dynamic_vertex_count +
			rasterizer_frame_statistics.reflection_mask_dynamic_vertex_count +
			rasterizer_frame_statistics.reflection_dynamic_vertex_count +
			rasterizer_frame_statistics.transparent_geometry_dynamic_vertex_count +
			rasterizer_frame_statistics.environment_fog_dynamic_vertex_count;
		long environment_triangles =
			rasterizer_frame_statistics.lightmap_dynamic_triangle_count +
			rasterizer_frame_statistics.shadow_triangle_count +
			rasterizer_frame_statistics.environment_dynamic_triangle_count +
			rasterizer_frame_statistics.diffuse_texture_dynamic_triangle_count +
			rasterizer_frame_statistics.specular_light_dynamic_triangle_count +
			rasterizer_frame_statistics.specular_lightmap_dynamic_triangle_count +
			rasterizer_frame_statistics.reflection_mask_dynamic_triangle_count +
			rasterizer_frame_statistics.reflection_dynamic_triangle_count +
			rasterizer_frame_statistics.transparent_geometry_dynamic_triangle_count +
			rasterizer_frame_statistics.environment_fog_dynamic_triangle_count;
		long environment_primitives =
			rasterizer_frame_statistics.lightmap_dynamic_draw_count +
			rasterizer_frame_statistics.shadow_draw_count +
			rasterizer_frame_statistics.environment_dynamic_draw_count +
			rasterizer_frame_statistics.diffuse_texture_dynamic_draw_count +
			rasterizer_frame_statistics.specular_light_dynamic_draw_count +
			rasterizer_frame_statistics.specular_lightmap_dynamic_draw_count +
			rasterizer_frame_statistics.reflection_mask_dynamic_draw_count +
			rasterizer_frame_statistics.reflection_dynamic_draw_count +
			rasterizer_frame_statistics.transparent_geometry_dynamic_draw_count +
			rasterizer_frame_statistics.environment_fog_dynamic_draw_count;
		long model_vertices =
			rasterizer_frame_statistics.model_vertex_count +
			rasterizer_frame_statistics.transparent_model_vertex_count;
		long model_triangles =
			rasterizer_frame_statistics.model_triangle_count +
			rasterizer_frame_statistics.transparent_model_triangle_count;
		long model_primitives =
			rasterizer_frame_statistics.model_draw_count +
			rasterizer_frame_statistics.transparent_model_submit_count;
		long total_vertices =
			environment_vertices + model_vertices +
			rasterizer_frame_statistics.model_shadow_vertex_count;
		long total_triangles =
			environment_triangles + model_triangles +
			rasterizer_frame_statistics.model_shadow_triangle_count;
		long total_primitives =
			environment_primitives + model_primitives +
			rasterizer_frame_statistics.model_shadow_draw_count;
		rectangle2d bounds;
		short index;

		for (index = 0; index < NUMBER_OF_STATISTICS_TAB_STOPS; index++)
		{
			tab_stops[index] += left;
		}

		bounds = rasterizer_globals.reserved04.frame_bounds;
		offset_rectangle2d(&bounds, 0, 32);
		interface_set_bitmap_text_draw_mode(_interface_font_terminal, NONE, 0, 0, 5, 0);

		sprintf(string, "|n|tframerate|taverage (of %d)|tmin|tmax",
			rasterizer_frame_statistics.fps_sample_count);
		tab_stops[0] = left;
		draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
		draw_string_set_color(&header_color);
		rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
		bounds.y0 = cursor.y + line_advance;

		if (rasterizer_debug_options.fps_accumulation)
		{
			__int64 accumulated_frames =
				rasterizer_globals.fps_accumulation_frame_index -
				rasterizer_frame_statistics_private_globals.fps_accumulation_frame_index;
			__int64 accumulated_time =
				system_milliseconds() -
				rasterizer_frame_statistics_private_globals.fps_accumulation_time;

			sprintf(string, "|t%.0f|t%.0f/%.0f|t%.0f|t%.0f|n",
				rasterizer_frame_statistics.frames_per_second,
				rasterizer_frame_statistics.average_frames_per_second,
				accumulated_frames * 1000.0f / accumulated_time,
				rasterizer_frame_statistics.minimum_frames_per_second,
				rasterizer_frame_statistics.maximum_frames_per_second);
		}
		else
		{
			sprintf(string, "|t%.0f|t%.0f|t%.0f|t%.0f|n",
				rasterizer_frame_statistics.frames_per_second,
				rasterizer_frame_statistics.average_frames_per_second,
				rasterizer_frame_statistics.minimum_frames_per_second,
				rasterizer_frame_statistics.maximum_frames_per_second);
		}

		tab_stops[0] = left;
		draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
		draw_string_set_color(&data_color);
		rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
		bounds.y0 = cursor.y + line_advance;

		if (rasterizer_debug_options.stats == _rasterizer_statistics_mode_objects)
		{
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			draw_string_set_color(&data_color);

			sprintf(string, "|tfogged|t%d|n|tnormal|t%d|n|tfast|t%d|n|tscenery|t%d|n",
				rasterizer_frame_statistics.fogged_object_count,
				rasterizer_frame_statistics.normal_object_count,
				rasterizer_frame_statistics.fast_object_count,
				rasterizer_frame_statistics.scenery_object_count);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tskinning|t%d|n|tlighting|t%d|n|tvertex shaders|t%d|n",
				rasterizer_frame_statistics.model_skinning_constant_bytes,
				rasterizer_frame_statistics.model_lighting_constant_bytes,
				rasterizer_frame_statistics.model_vertex_shader_work_accumulated);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tlocal_player_count|t%d|n|tmain_get_window_count|t%d|n",
				local_player_count(),
				main_get_window_count());
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;
		}
		else if (rasterizer_debug_options.stats == _rasterizer_statistics_mode_geometry)
		{
			sprintf(string, "|t|tvertices|ttriangles|tprimitives");
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			draw_string_set_color(&header_color);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|ttotal|t%d|t%d|t%d|n",
				total_vertices,
				total_triangles,
				total_primitives);
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			draw_string_set_color(&data_color);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tenvironment|t%d|t%d|t%d",
				environment_vertices,
				environment_triangles,
				environment_primitives);
			tab_stops[0] = left + 25;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tlightmaps|t%d|t%d|t%d|n|tshadows (%d)|t%d|t%d|t%d|n|tlights|t%d|t%d|t%d|n|ttextures|t%d|t%d|t%d|n|tlights specular|t%d|t%d|t%d|n|tlightmaps specular|t%d|t%d|t%d|n|tlightmaps ref.mask|t%d|t%d|t%d|n|treflections|t%d|t%d|t%d|n|ttransparent|t%d|t%d/%d|t%d|n|tfog|t%d|t%d|t%d|n",
				rasterizer_frame_statistics.lightmap_dynamic_vertex_count,
				rasterizer_frame_statistics.lightmap_dynamic_triangle_count,
				rasterizer_frame_statistics.lightmap_dynamic_draw_count,
				rasterizer_frame_statistics.shadow_count,
				rasterizer_frame_statistics.shadow_vertex_count,
				rasterizer_frame_statistics.shadow_triangle_count,
				rasterizer_frame_statistics.shadow_draw_count,
				rasterizer_frame_statistics.environment_dynamic_vertex_count,
				rasterizer_frame_statistics.environment_dynamic_triangle_count,
				rasterizer_frame_statistics.environment_dynamic_draw_count,
				rasterizer_frame_statistics.diffuse_texture_dynamic_vertex_count,
				rasterizer_frame_statistics.diffuse_texture_dynamic_triangle_count,
				rasterizer_frame_statistics.diffuse_texture_dynamic_draw_count,
				rasterizer_frame_statistics.specular_light_dynamic_vertex_count,
				rasterizer_frame_statistics.specular_light_dynamic_triangle_count,
				rasterizer_frame_statistics.specular_light_dynamic_draw_count,
				rasterizer_frame_statistics.specular_lightmap_dynamic_vertex_count,
				rasterizer_frame_statistics.specular_lightmap_dynamic_triangle_count,
				rasterizer_frame_statistics.specular_lightmap_dynamic_draw_count,
				rasterizer_frame_statistics.reflection_mask_dynamic_vertex_count,
				rasterizer_frame_statistics.reflection_mask_dynamic_triangle_count,
				rasterizer_frame_statistics.reflection_mask_dynamic_draw_count,
				rasterizer_frame_statistics.reflection_dynamic_vertex_count,
				rasterizer_frame_statistics.reflection_dynamic_triangle_count,
				rasterizer_frame_statistics.reflection_dynamic_draw_count,
				rasterizer_frame_statistics.transparent_geometry_dynamic_vertex_count,
				rasterizer_frame_statistics.transparent_geometry_dynamic_triangle_count,
				rasterizer_frame_statistics.transparent_geometry_largest_dynamic_triangle_count,
				rasterizer_frame_statistics.transparent_geometry_dynamic_draw_count,
				rasterizer_frame_statistics.environment_fog_dynamic_vertex_count,
				rasterizer_frame_statistics.environment_fog_dynamic_triangle_count,
				rasterizer_frame_statistics.environment_fog_dynamic_draw_count);
			tab_stops[0] = left + 50;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tmodel shadows (%d)|t%d|t%d|t%d",
				rasterizer_frame_statistics.model_shadow_count,
				rasterizer_frame_statistics.model_shadow_vertex_count,
				rasterizer_frame_statistics.model_shadow_triangle_count,
				rasterizer_frame_statistics.model_shadow_draw_count);
			tab_stops[0] = left + 25;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tmodels (%d)|t%d|t%d|t%d",
				rasterizer_frame_statistics.model_count,
				model_vertices,
				model_triangles,
				model_primitives);
			tab_stops[0] = left + 25;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tsolid|t%d|t%d|t%d|n|ttransparent|t%d|t%d/%d|t%d|n",
				rasterizer_frame_statistics.model_vertex_count,
				rasterizer_frame_statistics.model_triangle_count,
				rasterizer_frame_statistics.model_draw_count,
				rasterizer_frame_statistics.transparent_model_vertex_count,
				rasterizer_frame_statistics.transparent_model_triangle_count,
				rasterizer_frame_statistics.transparent_model_maximum_triangle_count,
				rasterizer_frame_statistics.transparent_model_submit_count);
			tab_stops[0] = left + 50;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tdecals|t%d|t%d|t%d|n",
				rasterizer_frame_statistics.decal_vertex_count,
				rasterizer_frame_statistics.decal_triangle_count,
				rasterizer_frame_statistics.decal_draw_count);
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tdynamic geometry|t%d/%d|t%d/%d|n",
				rasterizer_frame_statistics.dynamic_vertex_count,
				rasterizer_frame_statistics.dynamic_vertex_buffer_count,
				rasterizer_frame_statistics.dynamic_triangle_count,
				rasterizer_frame_statistics.dynamic_triangle_buffer_count);
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|t%d dynamic lights|n|t%d lens flares|n",
				rasterizer_frame_statistics.dynamic_light_count,
				rasterizer_frame_statistics.lens_flare_count);
			tab_stops[0] = left;
			draw_string_set_tab_stops(tab_stops, NUMBER_OF_STATISTICS_TAB_STOPS);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;
		}
		else if (rasterizer_debug_options.stats == _rasterizer_statistics_mode_profile)
		{
			short profile;

			tab_stops[0] = left;
			tab_stops[1] = left + 200;
			tab_stops[2] = left + 300;
			tab_stops[3] = 600;

			sprintf(string, "|tGPU profile|ttime (msecs)|tdata (bytes)");
			draw_string_set_tab_stops(tab_stops, 4);
			draw_string_set_color(&header_color);
			cursor.y -= 30;
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;
			draw_string_set_color(&data_color);

			for (profile = 0; profile < NUMBER_OF_RASTERIZER_PROFILES; profile++)
			{
				real profile_time = rasterizer_profile_query(profile);

				if (profile_time >= 0.0f)
				{
					sprintf(string, "|t%s|t%.2f|t%d",
						rasterizer_profile_get_string(profile),
						profile_time * 1000.0f,
						rasterizer_profile_query_pushbuffer(profile));
				}
				else
				{
					sprintf(string, "|t%s|t----|t0",
						rasterizer_profile_get_string(profile));
				}

				rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
				bounds.y0 = cursor.y + line_advance;
			}

			sprintf(string, "|ttotal|t%.2f|t%d|n",
				rasterizer_profile_query(NUMBER_OF_RASTERIZER_PROFILES) * 1000.0f,
				rasterizer_profile_query_pushbuffer(NUMBER_OF_RASTERIZER_PROFILES));
			draw_string_set_color(global_real_argb_yellow);
			bounds.y0 += 4;
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;
		}
		else if (rasterizer_debug_options.stats == _rasterizer_statistics_mode_memory)
		{
			long total_allocation = 0;
			long total_unique_allocation = 0;
			MEMORYSTATUS memory_status;
			struct rasterizer_memory_usage_report memory_usage[NUMBER_OF_MEMORY_USAGE_REPORTS] =
			{
				{ "memory pool", 0x18000, 0 },
				{ "dynamic vertices (unlit)", 0x30000, 0 },
				/* BUG (original): January stores 0x48 here; a plausible corrected
				 * allocation would be 0x48000, but the target value is preserved. */
				{ "dynamic vertices (lit*)", 0x48, 0x4b000 },
				{ "dynamic vertices (screen)", 0x50000, 0 },
				{ "dynamic vertices (model)", 0x10000, 0 },
				{ "dynamic vertices (detail objects)", 0x20000, 0 },
				{ "dynamic triangles", 0x30000, 0 },
				{ "transparent geometry groups", 0xf000, 0 },
				{ "bump map palette", 0x400, 0 },
				{ "mirror buffers (includes z-buffer*)", 0x96000, 0x4b000 },
				{ "shadow buffers", 0x10000, 0 },
				{ "sun glow buffers*", 0x8000, 0x8000 },
				{ "water buffers", 0x10000, 0 },
				{ "motion sensor buffers*", 0x14000, 0x14000 },
				{ "debug geometry*", 0x1b0000, 0x1b0000 },
				{ "vertex shaders|t~35k last i checked", 0x8c00, 0x3000 }
			};
			short report_index;

			tab_stops[0] = left;
			tab_stops[1] = left + 300;
			tab_stops[2] = 600;
			sprintf(string, "|tallocation|tmemory usage (bytes)");
			draw_string_set_tab_stops(tab_stops, 3);
			draw_string_set_color(&header_color);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;
			draw_string_set_color(&data_color);

			for (report_index = 0; report_index < NUMBER_OF_MEMORY_USAGE_REPORTS; report_index++)
			{
				sprintf(string, "|t%s|t%d",
					memory_usage[report_index].name,
					memory_usage[report_index].allocation);
				rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
				bounds.y0 = cursor.y + line_advance;
				total_allocation += memory_usage[report_index].allocation;
				total_unique_allocation +=
					memory_usage[report_index].allocation - memory_usage[report_index].shared;
			}

			sprintf(string, "|n|ttotal|t%d (%d)", total_allocation, total_unique_allocation);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			GlobalMemoryStatus(&memory_status);
			draw_string_set_color(&header_color);

			sprintf(string, "|n|tsystem total|t%dKb", memory_status.dwTotalPhys >> 10);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			sprintf(string, "|tsystem available|t%dKb", memory_status.dwAvailPhys >> 10);
			rasterizer_draw_string(&bounds, NULL, &cursor, height_adjust, string);
			bounds.y0 = cursor.y + line_advance;

			draw_string_set_color(&data_color);
		}

		draw_string_set_tab_stops(NULL, 0);
		draw_string_set_color(&default_color);
	}

	if (rasterizer_debug_options.profile_log)
	{
		if (!rasterizer_frame_statistics_private_globals.profile_log_file)
		{
			rasterizer_frame_statistics_private_globals.profile_log_file =
				fopen(profile_log_path, "w");

			if (!rasterizer_frame_statistics_private_globals.profile_log_file)
			{
				error(
					_error_silent,
					"### ERROR failed to open rasterizer profile log (%s)",
					profile_log_path);
				rasterizer_debug_options.profile_log = FALSE;
			}

			profile_accumulated_time = 0.0f;
			profile_accumulated_pushbuffer_size = 0;
		}

		if (rasterizer_frame_statistics_private_globals.profile_log_file)
		{
			short profile;

			for (profile = 0; profile < NUMBER_OF_RASTERIZER_PROFILES; profile++)
			{
				rasterizer_frame_statistics_private_globals.profile_times[profile][
					profile_accumulation_index] =
					rasterizer_profile_query(profile) * 1000.0f;
			}

			profile_accumulated_time +=
				rasterizer_profile_query(NUMBER_OF_RASTERIZER_PROFILES) * 1000.0f;
			profile_accumulated_pushbuffer_size +=
				rasterizer_profile_query_pushbuffer(NUMBER_OF_RASTERIZER_PROFILES);

			if (++profile_accumulation_index ==
				MAXIMUM_PROFILE_ACCUMULATION_COUNT)
			{
				short accumulation;

				fprintf(rasterizer_frame_statistics_private_globals.profile_log_file, "\n");

				for (profile = 0; profile < NUMBER_OF_RASTERIZER_PROFILES; profile++)
				{
					short length;

					fprintf(
						rasterizer_frame_statistics_private_globals.profile_log_file,
						"%s",
						rasterizer_profile_get_string(profile));

					for (length = (short)csstrlen(rasterizer_profile_get_string(profile));
						length < 32;
						length++)
					{
						fprintf(rasterizer_frame_statistics_private_globals.profile_log_file, ".");
					}

					for (accumulation = 0;
						accumulation < MAXIMUM_PROFILE_ACCUMULATION_COUNT;
						accumulation++)
					{
						if (rasterizer_frame_statistics_private_globals.profile_times[profile][accumulation] >= 0.0f)
						{
							fprintf(
								rasterizer_frame_statistics_private_globals.profile_log_file,
								"%6.2f",
								rasterizer_frame_statistics_private_globals.profile_times[profile][accumulation]);
						}
						else
						{
							fprintf(
								rasterizer_frame_statistics_private_globals.profile_log_file,
								"  ----");
						}
					}

					fprintf(rasterizer_frame_statistics_private_globals.profile_log_file, "\n");
				}

				fprintf(
					rasterizer_frame_statistics_private_globals.profile_log_file,
					"average total frame time= %.2f msecs\n",
					profile_accumulated_time /
						MAXIMUM_PROFILE_ACCUMULATION_COUNT);
				fprintf(
					rasterizer_frame_statistics_private_globals.profile_log_file,
					"average total pushbuffer= %d bytes\n",
					(profile_accumulated_pushbuffer_size +
						MAXIMUM_PROFILE_ACCUMULATION_COUNT / 2) / MAXIMUM_PROFILE_ACCUMULATION_COUNT);
				fflush(rasterizer_frame_statistics_private_globals.profile_log_file);

				profile_accumulation_index = 0;
				profile_accumulated_time = 0.0f;
				profile_accumulated_pushbuffer_size = 0;
			}
		}
	}
	else if (rasterizer_frame_statistics_private_globals.profile_log_file)
	{
		fclose(rasterizer_frame_statistics_private_globals.profile_log_file);
		rasterizer_frame_statistics_private_globals.profile_log_file = NULL;
	}

	return;
}

void rasterizer_frame_statistics_end(
	void)
{
	return;
}

void rasterizer_frame_statistics_dispose(
	void)
{
	word *temp_buffer = rasterizer_frame_statistics_temp_buffer;

	if (temp_buffer)
	{
		match_free(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_frame_statistics.c",
			837,
			temp_buffer);
	}

	return;
}

/* ---------- private code */
