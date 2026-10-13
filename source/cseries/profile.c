/*
PROFILE.C

symbols in this file:
0007DCA0 0080:
	_profile_initialize (0000)
0007DD20 0160:
	_profile_internal_step (0000)
0007DE80 0020:
	_profile_rasterizer_stats (0000)
0007DEA0 0040:
	_profile_timesection_inherit (0000)
0007DEE0 0010:
	_profile_seconds_elapsed (0000)
0007DEF0 0040:
	_profile_lapsed_frames (0000)
0007DF30 0020:
	_profile_lapsed_msec (0000)
0007DF50 0160:
	_find_profile_section (0000)
0007E0B0 0080:
	_profile_enter_private (0000)
0007E130 00a0:
	_profile_exit_private (0000)
0007E1D0 0610:
	_profile_describe_frame (0000)
0007E7E0 0010:
	_profile_timesection_begin (0000)
0007E7F0 0050:
	_profile_timesection_end (0000)
0007E840 0120:
	_compare_profile_sections (0000)
0007E960 0360:
	_profile_dump (0000)
0007ECC0 0080:
	_profile_dump_to_file (0000)
0007ED40 0080:
	_profile_dump_frame (0000)
0007EDC0 0040:
	_profile_dump_frame_stop (0000)
0007EE00 0030:
	_string_has_prefix (0000)
0007EE30 00a0:
	_profile_sections_activation (0000)
0007EED0 0020:
	_profile_sections_activate (0000)
0007EEF0 0020:
	_profile_sections_deactivate (0000)
0007EF10 0430:
	_profile_find_frame_value (0000)
0007F340 0040:
	_profile_find_game_value (0000)
0007F380 03a0:
	_profile_frame_get_value (0000)
0007F720 0060:
	_profile_frame_iterator_new (0000)
0007F780 0080:
	_profile_frame_iterator_next (0000)
0007F800 0090:
	_profile_frame_get_messages (0000)
0007F890 00a0:
	_profile_frame_get_stalls (0000)
0007F930 0090:
	_profile_rasterizer_stalls (0000)
0007F9C0 0030:
	_profile_timesection_begin_now (0000)
0007F9F0 0060:
	_profile_timesection_end_now (0000)
0007FA50 0090:
	_profile_tick_start (0000)
0007FAE0 00a0:
	_profile_tick_end (0000)
0007FB80 0030:
	_profile_render_start (0000)
0007FBB0 0070:
	_profile_render_end (0000)
0007FC20 0090:
	_profile_render_window_start (0000)
0007FCB0 00a0:
	_profile_render_window_end (0000)
0007FD50 0030:
	_profile_texture_start (0000)
0007FD80 0070:
	_profile_texture_end (0000)
0007FDF0 0080:
	_profile_frame_start (0000)
0007FE70 0370:
	_profile_frame_end (0000)
000801E0 0030:
	_profile_idle_start (0000)
00080210 0070:
	_profile_idle_end (0000)
00257E54 0033:
	??_C@_0DD@GJHCEALK@?$HMl?$CFs?$HMt?$HMr?$CF?53?42f?1?$CF?54ld?$HMt?$CF?53?42f?1?$CF?54@ (0000)
00257E88 0044:
	??_C@_0EE@LGPEPOLG@?$CF?950s?$CF6ld?5?1?5?$CF7?43f?5?5?5?5?5?5?5?5?5?5?5?5?$CF5?4@ (0000)
00257ECC 0020:
	??_C@_0CA@IFDPKCJJ@?$HMt?$HMrthis?5frame?$HMtaverage?$HMtpeak?$HMn?$AA@ (0000)
00257EF0 0074:
	??_C@_0HE@OCELNIBL@section?7?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5?5@ (0000)
00257F68 0041:
	??_C@_0EB@DNCFGCLL@parent_timesection?9?$DOself_msec?5?$DO?$DN@ (0000)
00257FAC 0021:
	??_C@_0CB@JIHDKIHN@c?3?2halo?2SOURCE?2cseries?2profile?4c@ (0000)
00257FD0 0037:
	??_C@_0DH@IAAOFPKE@profile_globals?4section_count?$DMMA@ (0000)
00258008 0039:
	??_C@_0DJ@MGEBDNKL@don?8t?5call?5profile_enter_private@ (0000)
00258044 0010:
	??_C@_0BA@OFIJJPPF@section?9?$DOactive?$AA@ (0000)
00258054 0008:
	??_C@_07BNGFJMOB@section?$AA@ (0000)
0025805C 001b:
	??_C@_0BL@DCGIKKDM@section?9?$DOstack_depth?$DN?$DNNONE?$AA@ (0000)
00258078 0032:
	??_C@_0DC@EBAGECGL@section?9?$DOstack_depth?$DN?$DNprofile_gl@ (0000)
002580AC 000e:
	??_C@_0O@CAINCP@f?9misc?5?$CF6?42f?5?$AA@ (0000)
002580BC 000e:
	??_C@_0O@HPDODOFI@r?9misc?5?$CF6?42f?5?$AA@ (0000)
002580CC 000b:
	??_C@_0L@CCBFPMDD@?5?5?5?5?5?5?5?5?5?5?$AA@ (0000)
002580D8 000a:
	??_C@_09DIDNDMFB@tex?$CF6?42f?5?$AA@ (0000)
002580E4 000d:
	??_C@_0N@ONKJJLFA@?5?5?5?5?5?5?5?5?5?5?5?5?$AA@ (0000)
002580F4 000c:
	??_C@_0M@CIJICDBL@stall?$CF6?42f?5?$AA@ (0000)
00258100 000a:
	??_C@_09LLNJGGAC@?5?5?5?5?5?5?5?$CFs?$AA@ (0000)
0025810C 000a:
	??_C@_09MEJENFBG@?$CFs?$CF6?42f?$CFs?$AA@ (0000)
00258118 0002:
	??_C@_01EFFIKLCJ@n?$AA@ (0000)
0025811C 0002:
	??_C@_01JBBJJEPG@p?$AA@ (0000)
00258120 000d:
	??_C@_0N@KKPPKAFO@?5render?$CF6?42f?$AA@ (0000)
00258130 0002:
	??_C@_01PKGAHCOL@?$CJ?$AA@ (0000)
00258134 0009:
	??_C@_08PBAHBJAK@?5?5?5?5?5?5?$CFs?$AA@ (0000)
00258140 0008:
	??_C@_07CCGGFFIN@?$CF6?42f?$CFs?$AA@ (0000)
00258148 0003:
	??_C@_02GFKOMOKH@?5?$CI?$AA@ (0000)
0025814C 0009:
	??_C@_08GLEMDNF@game?$CF2d?5?$AA@ (0000)
00258158 000c:
	??_C@_0M@CKHGFAEI@?5?5?5?5?5?5?5?5?5?5?5?$AA@ (0000)
00258164 000b:
	??_C@_0L@PHBKFODP@idle?$CF6?42f?5?$AA@ (0000)
00258170 000a:
	??_C@_09CIKJCKOO@?$CIsynced?$CJ?5?$AA@ (0000)
0025817C 000a:
	??_C@_09CCMCFOJI@?$CIslowed?$CJ?5?$AA@ (0000)
00258188 000a:
	??_C@_09CLBOIIGO@?$CIf?4?$CF3dms?$CJ?$AA@ (0000)
00258194 000a:
	??_C@_09LODAACPA@?$CIl?4?$CF3dms?$CJ?$AA@ (0000)
002581A0 000a:
	??_C@_09OAGDEMOJ@?$CIfree?$CF3d?$CJ?$AA@ (0000)
002581AC 000a:
	??_C@_09FOHHJKEH@?$CIlost?$CF3d?$CJ?$AA@ (0000)
002581B8 001e:
	??_C@_0BO@KFBPKJAE@frame?5?$CF5d?5vbl?5?$CF5I64d?5tot?$CF6?42f?$AA@ (0000)
002581D8 0008:
	__real@408f400000000000 (0000)
002581E0 0018:
	??_C@_0BI@LMKLONDN@maximum_section_count?$DO0?$AA@ (0000)
002581F8 0042:
	??_C@_0EC@FFADJNIP@format_mode?$DO?$DN0?5?$CG?$CG?5format_mode?$DMNU@ (0000)
0025823C 0037:
	??_C@_0DH@ICGEGPLO@sort_mode?$DO?$DN0?5?$CG?$CG?5sort_mode?$DMNUMBER@ (0000)
00258274 0005:
	??_C@_04LLEBNMDN@?$CFs?$AN?6?$AA@ (0000)
0025827C 000f:
	??_C@_0P@NHLICBHE@d?3?2profile?4txt?$AA@ (0000)
0025828C 0011:
	??_C@_0BB@MGJFLJKK@d?3?2framedump?4txt?$AA@ (0000)
002582A0 0003:
	??_C@_02GMLFBBN@wb?$AA@ (0000)
002582A4 0002:
	??_C@_01NBENCBCI@?$CK?$AA@ (0000)
002582A8 000b:
	??_C@_0L@KMPGOPBB@pushbuffer?$AA@ (0000)
002582B4 0004:
	??_C@_03HOPDAKLK@gpu?$AA@ (0000)
002582B8 0003:
	??_C@_02EDDPJOD@dt?$AA@ (0000)
002582BC 0008:
	??_C@_07JDHEGGGP@texture?$AA@ (0000)
002582C4 0006:
	??_C@_05MEMGOBLF@stall?$AA@ (0000)
002582CC 000c:
	??_C@_0M@BFMMLLIC@game_render?$AA@ (0000)
002582D8 000c:
	??_C@_0M@HBFEAABD@render0_3np?$AA@ (0000)
002582E4 000a:
	??_C@_09PKCOAFOC@render0_3?$AA@ (0000)
002582F0 000a:
	??_C@_09ODDFDEKD@render0_2?$AA@ (0000)
002582FC 000a:
	??_C@_09MIBIGHGA@render0_1?$AA@ (0000)
00258308 0008:
	??_C@_07HHKBOHGC@render0?$AA@ (0000)
00258310 0007:
	??_C@_06IAAOMEKN@render?$AA@ (0000)
00258318 000a:
	??_C@_09OHMOODOI@nonplayer?$AA@ (0000)
00258324 0008:
	??_C@_07OCBHBHOJ@player3?$AA@ (0000)
0025832C 0008:
	??_C@_07PLAMCGKI@player2?$AA@ (0000)
00258334 0008:
	??_C@_07NACBHFGL@player1?$AA@ (0000)
0025833C 0008:
	??_C@_07MJDKEECK@player0?$AA@ (0000)
00258344 0006:
	??_C@_05OIMJLJGC@game7?$AA@ (0000)
0025834C 0006:
	??_C@_05PBNCIICD@game6?$AA@ (0000)
00258354 0006:
	??_C@_05NKPPNLOA@game5?$AA@ (0000)
0025835C 0006:
	??_C@_05MDOEOKKB@game4?$AA@ (0000)
00258364 0006:
	??_C@_05IMKFHMGG@game3?$AA@ (0000)
0025836C 0006:
	??_C@_05JFLOENCH@game2?$AA@ (0000)
00258374 0006:
	??_C@_05LOJDBOOE@game1?$AA@ (0000)
0025837C 0006:
	??_C@_05KHIICPKF@game0?$AA@ (0000)
00258384 0005:
	??_C@_04EONOHKEP@load?$AA@ (0000)
0025838C 0006:
	??_C@_05MIJNFGED@frame?$AA@ (0000)
00258394 0020:
	??_C@_0CA@LFOMCIIE@name?5?$CG?$CG?5section_index_reference?$AA@ (0000)
002583B4 0004:
	__real@42055555 (0000)
002583B8 0004:
	__real@35aaaaab (0000)
002583C0 004e:
	??_C@_0EO@JBFLBLLD@iterator?9?$DOcurrent_buffer_index?5?$CB@ (0000)
00258410 0078:
	??_C@_0HI@EANDMEO@?$CIiterator?9?$DOcurrent_buffer_index?5@ (0000)
00258488 0087:
	??_C@_0IH@HJNMEKLP@?$CIprofile_globals?4current_frame?4g@ (0000)
00258510 0074:
	??_C@_0HE@FKBCHJEP@?$CIprofile_globals?4current_frame?4w@ (0000)
00258588 0075:
	??_C@_0HF@JPMAIONJ@?$CIprofile_globals?4current_frame?4w@ (0000)
00258600 0088:
	??_C@_0II@PFDJGDHA@?$CIprofile_globals?4current_frame?4g@ (0000)
002DCD30 0010:
	_header_strings (0000)
	_format_strings (0008)
0031DF40 113d54:
	_bss_0031df40 (0000)
	_profile_timebase_ticks (113d50)
	_profile_global_enable (113d51)
	_profile_dump_frames (113d52)
	_profile_dump_lost_frames (113d53)
*/

/* ---------- headers */

#include "cseries.h"
#include "profile.h"

#include "math/real_math.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "main/main_internal.h"
#include "game/players.h"

#ifdef HALO_LINUX
#include <xtl.h>
#endif

/* ---------- constants */

enum
{
	MAXIMUM_PROFILE_SECTIONS = 256,
#ifdef HALO_N64
	/* (a frame's ticks past these are counted in the last: the N64 runs at
	most seven a frame, and the 150 timers of the nine frames kept were 32
	KB of the 40 the profiler's globals took) */
	MAXIMUM_GAME_TICKS_PER_FRAME = 8,
#else
	MAXIMUM_GAME_TICKS_PER_FRAME = 150,
#endif
#if defined(HALO_XBOX) || defined(HALO_N64)
	/* the console's memory: a short history (every index is taken modulo
	this, and the dump looks three frames back) */
	MAXIMUM_PROFILE_FRAMES = 8,
#else
	MAXIMUM_PROFILE_FRAMES = 256,
#endif
	MAXIMUM_PROFILE_WINDOWS = 4,
};

enum profile_frame_value
{
	_profile_frame_value_frame = 1,
	_profile_frame_value_load,
	_profile_frame_value_game0,
	_profile_frame_value_game1,
	_profile_frame_value_game2,
	_profile_frame_value_game3,
	_profile_frame_value_game4,
	_profile_frame_value_game5,
	_profile_frame_value_game6,
	_profile_frame_value_game7,
	_profile_frame_value_player0,
	_profile_frame_value_player1,
	_profile_frame_value_player2,
	_profile_frame_value_player3,
	_profile_frame_value_nonplayer,
	_profile_frame_value_render0,
	_profile_frame_value_render0_1,
	_profile_frame_value_render0_2,
	_profile_frame_value_render0_3,
	_profile_frame_value_render0_3np,
	_profile_frame_value_render,
	_profile_frame_value_game_render,
	_profile_frame_value_stall,
	_profile_frame_value_texture,
	_profile_frame_value_idle,
	_profile_frame_value_dt,
	_profile_frame_value_gpu,
	_profile_frame_value_pushbuffer,

	NUMBER_OF_PROFILE_FRAME_VALUES
};

/* ---------- macros */

#ifdef HALO_XBOX
/* the console: the time stamp counter itself, which its performance
counter is (at the 733 MHz QueryPerformanceFrequency gives) */
#define QUERY_TIMEBASE(timebase) \
{ \
	*(__int64 *)&(timebase) = (__int64)__builtin_ia32_rdtsc(); \
}
#elif defined(HALO_LINUX)
/* the native ports: the platform's performance counter stands in for the
time stamp counter, at its own rate (profile_initialize) */
#define QUERY_TIMEBASE(timebase) \
{ \
	LARGE_INTEGER halo_counter; \
	QueryPerformanceCounter(&halo_counter); \
	*(__int64 *)&(timebase) = halo_counter.QuadPart; \
}
#else
#define QUERY_TIMEBASE(timebase) \
{ \
	__asm push eax \
	__asm push edx \
	__asm rdtsc \
	__asm mov dword ptr timebase, eax \
	__asm mov dword ptr timebase+4, edx \
	__asm pop edx \
	__asm pop eax \
}
#endif

/* ---------- structures */

struct profile_timer
{
	__int64 start;
	__int64 end;
	real total;
	real frame_total;
};

struct profile_frame
{
	boolean dumped;
	long frame_index;
	__int64 vertical_blank_index;
	short game_tick_count;
	short window_count;
	byte window_ids[MAXIMUM_PROFILE_WINDOWS];
	struct profile_timer frame;
	struct profile_timer game_ticks[MAXIMUM_GAME_TICKS_PER_FRAME];
	struct profile_timer windows[MAXIMUM_PROFILE_WINDOWS];
	struct profile_timer render;
	struct profile_timer stall;
	struct profile_timer texture;
	struct profile_timer idle;
	real seconds_elapsed;
	short lapsed_frames;
	byte __unknown0F06[2];
	long lapsed_msec;
	boolean lapsed_msec_valid;
	char lapsed_reason[0x203];
	real rasterizer_gpu_time;
	unsigned long rasterizer_pushbuffer_size;
	long stall_count;
	short stall_index;
	byte __unknown111E[2];
	real stall_msec;
	byte __unknown1124[4];
};

struct profile_globals
{
	__int64 timebase_frequency;
	short stack_depth;
	boolean initialization_pending;
	long history_index;
	short section_count;
	struct profile_section* sections[MAXIMUM_PROFILE_SECTIONS];
	FILE* framedump_file;
	short compare_type;
	long lost_frame_count;
	boolean framedump_flush_pending;
	short current_frame_history_count;
	short current_frame_history_index;
	struct profile_frame frames[MAXIMUM_PROFILE_FRAMES];
	struct profile_frame current_frame;
};

/* ---------- prototypes */

static void profile_timesection_inherit(
	struct profile_timer *parent_timesection,
	struct profile_timer *child_timesection);
static void profile_describe_frame(
	struct profile_frame *frame,
	char *string,
	short maximum_length);
static void profile_dump_frame(
	struct profile_frame *frame);
static void profile_internal_step(
	void);
void find_profile_section(
	struct profile_section *section);
static void profile_dump_frame_stop(
	void);
static void profile_sections_activation(
	const char *name,
	boolean active);
int compare_profile_sections(
	void const *section0,
	void const *section1);
static boolean string_has_prefix(
	char const *string,
	char const *prefix);
static void profile_timesection_begin_now(
	struct profile_timer *timer);
static void profile_timesection_begin(
	struct profile_timer *timer,
	__int64 start);
static void profile_timesection_end_now(
	struct profile_timer *timer);
static void profile_timesection_end(
	struct profile_timer *timer,
	__int64 end);

/* ---------- globals */

const char *header_strings[NUMBER_OF_PROFILE_DUMP_FORMAT_MODES] =
{
	"section\t                                     total calls / time           av. calls / time      peak calls / time\r\n",
	"|t|rthis frame|taverage|tpeak|n"
};

const char *format_strings[NUMBER_OF_PROFILE_DUMP_FORMAT_MODES] =
{
	"%-50s%6ld / %7.3f            %5.2f / %7.3f            %ld / %7.3f\r\n",
	"|l%s|t|r% 3.2f/% 4ld|t% 3.2f/% 4ld|t% 3.2f/% 4ld|n"
};

static struct profile_globals profile_globals = {0};
boolean profile_timebase_ticks = FALSE;
boolean profile_global_enable = FALSE;
boolean profile_dump_frames = FALSE;
boolean profile_dump_lost_frames = FALSE;

/* ---------- public code */

void profile_dump_to_file(
	const char *name)
{
	char buffer[0x2000];
	long use_name;
	FILE *file;

	use_name = name && csstrlen(name);

	file = fopen("d:\\profile.txt", "a+b");
	if (file)
	{
		profile_dump(name, use_name, 0, 256, buffer);
		fprintf(file, "%s\r\n", buffer);
	}
	fclose(file);

	return;
}

void profile_dump(
	const char *name,
	short sort_mode,
	short format_mode,
	short maximum_section_count,
	char *buffer)
{
	struct profile_section *sections[MAXIMUM_PROFILE_SECTIONS];
	short displayed_count = 0;
	short section_index;

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 879,
		sort_mode>=0 && sort_mode<NUMBER_OF_PROFILE_SORT_MODES);
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 880,
		format_mode>=0 && format_mode<NUMBER_OF_PROFILE_DUMP_FORMAT_MODES);
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 881,
		maximum_section_count>0);
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 882,
		buffer);

	profile_globals.compare_type = sort_mode;

	csmemcpy(
		sections,
		profile_globals.sections,
		profile_globals.section_count*sizeof(struct profile_section *));
	qsort(
		sections,
		profile_globals.section_count,
		sizeof(struct profile_section *),
		compare_profile_sections);

	if (profile_globals.section_count && sections[0]->active)
	{
		sprintf(buffer, header_strings[format_mode]);

		for (section_index = 0; section_index<profile_globals.section_count; section_index++)
		{
			struct profile_section *section;
			long total_calls;
			real total_time;
			real average_calls;
			real average_msec;
			long peak_calls;
			real peak_msec;
			long frame_calls;
			real frame_msec;

			if (displayed_count>=maximum_section_count)
				break;

			section = sections[section_index];
			total_calls = 0;
			total_time = 0.0f;
			average_calls = 0.0f;
			average_msec = 0.0f;
			peak_calls = 0;
			peak_msec = 0.0f;
			frame_calls = 0;
			frame_msec = 0.0f;

			if (section->active && (!name || strstr(section->name, name)))
			{
				do
				{
					total_calls += sections[section_index]->total_call_count;
					total_time += sections[section_index]->total_elapsed_timebase/
						(double)profile_globals.timebase_frequency;
					average_calls += (real)sections[section_index]->total_call_count/
						sections[section_index]->sample_count;
					average_msec += (sections[section_index]->sample_count==0 ?
						0.0 :
						(double)sections[section_index]->total_elapsed_timebase/
							sections[section_index]->sample_count)*1000.0/
						profile_globals.timebase_frequency;
					peak_calls += sections[section_index]->peak_call_count;
					peak_msec += sections[section_index]->peak_elapsed_timebase*1000.0/
						profile_globals.timebase_frequency;
					frame_calls += sections[section_index]->frame_call_count;
					frame_msec += sections[section_index]->frame_elapsed_timebase*1000.0/
						profile_globals.timebase_frequency;

					section_index++;
				}
				while (section_index<profile_globals.section_count &&
					csstrcmp(sections[section_index]->name, section->name)==0);

				switch (format_mode)
				{
					case _profile_dump_format_mode_file:
						sprintf(
							buffer+csstrlen(buffer),
							format_strings[format_mode],
							section->name,
							total_calls,
							total_time,
							average_calls,
							average_msec,
							peak_calls,
							peak_msec);
						break;

					case _profile_dump_format_mode_screen:
						sprintf(
							buffer+csstrlen(buffer),
							format_strings[format_mode],
							section->name,
							frame_msec,
							frame_calls,
							average_msec,
							(long)(average_calls+0.5f),
							peak_msec,
							peak_calls,
							total_time,
							total_calls);
						break;
				}

				section_index--;
				displayed_count++;
			}
		}
	}

	return;
}

void profile_rasterizer_stalls(
	long stall_count,
	short stall_index,
	unsigned long stall_ticks,
	long unused,
	__int64 stall_timebase)
{
	profile_timesection_begin(&profile_globals.current_frame.stall, 0);
	profile_timesection_end(&profile_globals.current_frame.stall, stall_timebase);
	profile_globals.current_frame.stall_count = stall_count;
	profile_globals.current_frame.stall_index = stall_index;
	profile_globals.current_frame.stall_msec = (real)(stall_ticks*1000.0f/profile_globals.timebase_frequency);

	return;
}

void profile_frame_get_messages(
	struct profile_frame_iterator *iterator,
	short *message_count,
	short maximum_message_count,
	char **messages,
	union point2d *locations,
	union real_argb_color const **colors)
{
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1463, iterator);
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1464, (iterator->current_buffer_index >= 0) && (iterator->current_buffer_index < profile_globals.current_frame_history_count));
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1465, iterator->current_buffer_index != profile_globals.current_frame_history_index);

	return;
}

long profile_frame_get_stalls(
	struct profile_frame_iterator *iterator,
	short *stall_index,
	real *stall_msec)
{
	struct profile_frame *frame = &profile_globals.frames[iterator->current_buffer_index];

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1480, (iterator->current_buffer_index >= 0) && (iterator->current_buffer_index < profile_globals.current_frame_history_count));
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1481, iterator->current_buffer_index != profile_globals.current_frame_history_index);

	*stall_index = frame->stall_index;
	*stall_msec = frame->stall_msec;

	return frame->stall_count;
}

void profile_lapsed_frames(
	short frames,
	boolean lapsed,
	const char *reason)
{
	profile_globals.current_frame.lapsed_frames = frames;
	profile_globals.current_frame.lapsed_msec_valid = frames>0 || !lapsed;

	if (reason)
	{
		csstrcpy(profile_globals.current_frame.lapsed_reason, reason);
	}

	return;
}

short profile_find_frame_value(
	const char *name,
	short *section_index_reference)
{
	short frame_value = NONE;

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1094,
		name && section_index_reference);

	if (!_stricmp(name, "frame"))
		frame_value = _profile_frame_value_frame;
	else if (!_stricmp(name, "load"))
		frame_value = _profile_frame_value_load;
	else if (!_stricmp(name, "game0"))
		frame_value = _profile_frame_value_game0;
	else if (!_stricmp(name, "game1"))
		frame_value = _profile_frame_value_game1;
	else if (!_stricmp(name, "game2"))
		frame_value = _profile_frame_value_game2;
	else if (!_stricmp(name, "game3"))
		frame_value = _profile_frame_value_game3;
	else if (!_stricmp(name, "game4"))
		frame_value = _profile_frame_value_game4;
	else if (!_stricmp(name, "game5"))
		frame_value = _profile_frame_value_game5;
	else if (!_stricmp(name, "game6"))
		frame_value = _profile_frame_value_game6;
	else if (!_stricmp(name, "game7"))
		frame_value = _profile_frame_value_game7;
	else if (!_stricmp(name, "player0"))
		frame_value = _profile_frame_value_player0;
	else if (!_stricmp(name, "player1"))
		frame_value = _profile_frame_value_player1;
	else if (!_stricmp(name, "player2"))
		frame_value = _profile_frame_value_player2;
	else if (!_stricmp(name, "player3"))
		frame_value = _profile_frame_value_player3;
	else if (!_stricmp(name, "nonplayer"))
		frame_value = _profile_frame_value_nonplayer;
	else if (!_stricmp(name, "render"))
		frame_value = _profile_frame_value_render;
	else if (!_stricmp(name, "render0"))
		frame_value = _profile_frame_value_render0;
	else if (!_stricmp(name, "render0_1"))
		frame_value = _profile_frame_value_render0_1;
	else if (!_stricmp(name, "render0_2"))
		frame_value = _profile_frame_value_render0_2;
	else if (!_stricmp(name, "render0_3"))
		frame_value = _profile_frame_value_render0_3;
	else if (!_stricmp(name, "render0_3np"))
		frame_value = _profile_frame_value_render0_3np;
	else if (!_stricmp(name, "game_render"))
		frame_value = _profile_frame_value_game_render;
	else if (!_stricmp(name, "stall"))
		frame_value = _profile_frame_value_stall;
	else if (!_stricmp(name, "texture"))
		frame_value = _profile_frame_value_texture;
	else if (!_stricmp(name, "idle"))
		frame_value = _profile_frame_value_idle;
	else if (!_stricmp(name, "dt"))
		frame_value = _profile_frame_value_dt;
	else if (!_stricmp(name, "gpu"))
		frame_value = _profile_frame_value_gpu;
	else if (!_stricmp(name, "pushbuffer"))
		frame_value = _profile_frame_value_pushbuffer;

	*section_index_reference = NONE;

	return frame_value;
}

short profile_find_game_value(
	const char *name,
	short *section_index_reference)
{
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1224, name && section_index_reference);

	*section_index_reference = NONE;

	return NONE;
}

real profile_frame_get_value(
	struct profile_frame_iterator *iterator,
	short frame_value,
	short section_index)
{
	struct profile_frame *frame =
		&profile_globals.frames[iterator->current_buffer_index];
	real value = 0.0f;

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1239,
		(iterator->current_buffer_index >= 0) &&
		(iterator->current_buffer_index < profile_globals.current_frame_history_count));
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1240,
		iterator->current_buffer_index != profile_globals.current_frame_history_index);

	switch (frame_value)
	{
		case _profile_frame_value_frame:
			value = frame->frame.total;
			break;

		case _profile_frame_value_load:
			value = frame->frame.total-frame->idle.total;
			break;

		case _profile_frame_value_game0:
			if (frame->game_tick_count>0)
			{
				value = frame->game_ticks[0].total;
			}
			break;

		case _profile_frame_value_player0:
			if (frame->window_count>0)
			{
				value = frame->windows[0].total;
			}
			break;

		case _profile_frame_value_nonplayer:
		{
			short window_index;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (!frame->window_ids[window_index])
				{
					value = frame->windows[window_index].total;
					break;
				}
			}
			break;
		}

		case _profile_frame_value_render:
			value = frame->render.total;
			break;

		case _profile_frame_value_render0:
		{
			short window_index;
			short player_window_count = 0;

			value = 0.0f;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (player_window_count>=1)
					break;

				if (frame->window_ids[window_index])
				{
					value += frame->windows[window_index].total;
					player_window_count++;
				}
			}
			break;
		}

		case _profile_frame_value_render0_1:
		{
			short window_index;
			short player_window_count = 0;

			value = 0.0f;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (player_window_count>=2)
					break;

				if (frame->window_ids[window_index])
				{
					value += frame->windows[window_index].total;
					player_window_count++;
				}
			}
			break;
		}

		case _profile_frame_value_render0_2:
		{
			short window_index;
			short player_window_count = 0;

			value = 0.0f;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (player_window_count>=3)
					break;

				if (frame->window_ids[window_index])
				{
					value += frame->windows[window_index].total;
					player_window_count++;
				}
			}
			break;
		}

		case _profile_frame_value_render0_3:
		{
			short window_index;
			short player_window_count = 0;

			value = 0.0f;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (player_window_count>=4)
					break;

				if (frame->window_ids[window_index])
				{
					value += frame->windows[window_index].total;
					player_window_count++;
				}
			}
			break;
		}

		case _profile_frame_value_render0_3np:
		{
			short window_index;
			short player_window_count = 0;

			value = 0.0f;

			for (window_index = 0;
				window_index<frame->window_count;
				window_index++)
			{
				if (player_window_count>=4)
					break;

				if (frame->window_ids[window_index])
				{
					value += frame->windows[window_index].total;
					player_window_count++;
				}
				else
				{
					value += frame->windows[window_index].total;
				}
			}
			break;
		}

		case _profile_frame_value_game_render:
		{
			short game_tick_index;

			value = frame->render.total;

			for (game_tick_index = 0;
				game_tick_index<frame->game_tick_count;
				game_tick_index++)
			{
				value += frame->game_ticks[game_tick_index].total;
			}
			break;
		}

		case _profile_frame_value_stall:
			value = frame->stall.total;
			break;

		case _profile_frame_value_texture:
			value = frame->texture.total;
			break;

		case _profile_frame_value_idle:
			value = frame->idle.total;
			break;

		case _profile_frame_value_dt:
			value = frame->seconds_elapsed*1000.0f;
			break;

		case _profile_frame_value_gpu:
			value = frame->rasterizer_gpu_time;
			break;

		case _profile_frame_value_pushbuffer:
			value = (real)(frame->rasterizer_pushbuffer_size*
				(1.0f/(1024.0f*768.0f)))*(100.0f/3.0f);
			break;
	}

	return value;
}

void profile_sections_activate(
	const char *name)
{
	profile_sections_activation(name, TRUE);

	return;
}

void profile_sections_deactivate(
	const char *name)
{
	profile_sections_activation(name, FALSE);

	return;
}

void profile_frame_iterator_new(
	struct profile_frame_iterator *iterator)
{
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 1419, iterator);

	iterator->current_buffer_index = NONE;
	iterator->next_buffer_index = (profile_globals.current_frame_history_index+MAXIMUM_PROFILE_FRAMES-1)%MAXIMUM_PROFILE_FRAMES;

	return;
}

boolean profile_frame_iterator_next(
	struct profile_frame_iterator *iterator,
	struct profile_frame_info *info)
{
	short buffer_index = iterator->next_buffer_index;
	boolean result = FALSE;

	iterator->current_buffer_index = buffer_index;

	if (buffer_index!=NONE &&
		buffer_index<profile_globals.current_frame_history_count)
	{
		result = TRUE;

		if (info)
		{
			info->vertical_blank_index = profile_globals.frames[buffer_index].vertical_blank_index;
		}

		iterator->next_buffer_index = (iterator->current_buffer_index+MAXIMUM_PROFILE_FRAMES-1)%MAXIMUM_PROFILE_FRAMES;
		if (iterator->next_buffer_index==profile_globals.current_frame_history_index)
		{
			iterator->next_buffer_index = NONE;
		}
	}

	return result;
}

void profile_seconds_elapsed(
	real seconds)
{
	profile_globals.current_frame.seconds_elapsed = seconds;

	return;
}

void profile_lapsed_msec(
	long msec)
{
	profile_globals.current_frame.lapsed_msec = msec;
	profile_globals.current_frame.lapsed_msec_valid = msec>0;

	return;
}

void profile_rasterizer_stats(
	real gpu_time,
	__int64 pushbuffer_size)
{
	profile_globals.current_frame.rasterizer_gpu_time = gpu_time;
	profile_globals.current_frame.rasterizer_pushbuffer_size = (unsigned long)pushbuffer_size;

	return;
}

void profile_initialize(
	void)
{
	short section_index = 0;

#ifdef HALO_LINUX
	{
		LARGE_INTEGER frequency;

		QueryPerformanceFrequency(&frequency);
		profile_globals.timebase_frequency = frequency.QuadPart;
	}
#else
	profile_globals.timebase_frequency = 733333333;
#endif

	while (section_index<profile_globals.section_count)
	{
		profile_globals.sections[section_index++]->section_index = NONE;
	}

	profile_global_enable = TRUE;
	profile_globals.section_count = 0;
	profile_globals.stack_depth = 0;
	profile_globals.initialization_pending = TRUE;
	profile_globals.history_index = 0;
	profile_globals.current_frame_history_count = 0;
	profile_globals.current_frame_history_index = 0;
	profile_globals.lost_frame_count = 999;
	profile_globals.framedump_file = NULL;

	return;
}

void profile_frame_start(
	void)
{
	if (!profile_timebase_ticks)
	{
		profile_internal_step();
	}

	csmemset(&profile_globals.current_frame, 0, sizeof(profile_globals.current_frame));

	profile_globals.current_frame.frame_index = render.frame_index;
	profile_globals.current_frame.vertical_blank_index = rasterizer_globals.vertical_blank_index;
	profile_globals.current_frame.game_tick_count = 0;
	profile_timesection_begin_now(&profile_globals.current_frame.frame);

	return;
}

void profile_tick_start(
	void)
{
	struct profile_timer *timer;

	if (profile_timebase_ticks)
	{
		profile_internal_step();
	}

	if (profile_globals.current_frame.game_tick_count<MAXIMUM_GAME_TICKS_PER_FRAME)
	{
		profile_globals.current_frame.game_tick_count++;
	}

	match_assert(
		"c:\\halo\\SOURCE\\cseries\\profile.c",
		311,
		(profile_globals.current_frame.game_tick_count > 0) && (profile_globals.current_frame.game_tick_count <= MAXIMUM_GAME_TICKS_PER_FRAME));

	timer = &profile_globals.current_frame.game_ticks[profile_globals.current_frame.game_tick_count-1];
	profile_timesection_begin_now(timer);

	return;
}

void profile_tick_end(
	void)
{
	struct profile_timer *timer;

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 320,
		(profile_globals.current_frame.game_tick_count > 0) && (profile_globals.current_frame.game_tick_count <= MAXIMUM_GAME_TICKS_PER_FRAME));

	timer = &profile_globals.current_frame.game_ticks[profile_globals.current_frame.game_tick_count-1];
	profile_timesection_end_now(timer);

	return;
}

void profile_render_window_start(
	boolean player_window)
{
	struct profile_timer *timer;

	if (profile_globals.current_frame.window_count<MAXIMUM_WINDOWS)
	{
		profile_globals.current_frame.window_count++;
		profile_globals.current_frame.window_ids[profile_globals.current_frame.window_count-1] = player_window;
	}

	match_assert(
		"c:\\halo\\SOURCE\\cseries\\profile.c",
		353,
		(profile_globals.current_frame.window_count > 0) && (profile_globals.current_frame.window_count <= MAXIMUM_WINDOWS));

	timer = &profile_globals.current_frame.windows[profile_globals.current_frame.window_count-1];
	profile_timesection_begin_now(timer);

	return;
}

void profile_render_window_end(
	void)
{
	struct profile_timer *timer;

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 362,
		(profile_globals.current_frame.window_count > 0) && (profile_globals.current_frame.window_count <= MAXIMUM_WINDOWS));

	timer = &profile_globals.current_frame.windows[profile_globals.current_frame.window_count-1];
	profile_timesection_end_now(timer);

	return;
}

void profile_render_start(
	void)
{
	profile_globals.current_frame.window_count = 0;
	profile_timesection_begin_now(&profile_globals.current_frame.render);

	return;
}

void profile_render_end(
	void)
{
	profile_timesection_end_now(&profile_globals.current_frame.render);

	return;
}

void profile_texture_start(
	void)
{
	profile_timesection_begin_now(&profile_globals.current_frame.texture);

	return;
}

void profile_texture_end(
	void)
{
	profile_timesection_end_now(&profile_globals.current_frame.texture);

	return;
}

void profile_idle_start(
	void)
{
	profile_timesection_begin_now(&profile_globals.current_frame.idle);

	return;
}

void profile_idle_end(
	void)
{
	profile_timesection_end_now(&profile_globals.current_frame.idle);

	return;
}

void profile_enter_private(
	struct profile_section *section)
{
	__int64 timebase;

	find_profile_section(section);

	match_assert(
		"c:\\halo\\SOURCE\\cseries\\profile.c",
		597,
		section->stack_depth==NONE);

	profile_globals.stack_depth++;
	section->stack_depth = profile_globals.stack_depth;
	QUERY_TIMEBASE(timebase);
	section->entry_timebase = timebase;
	section->frame_call_count++;

	return;
}

void profile_exit_private(
	struct profile_section *section)
{
	__int64 timebase;

	if (!profile_globals.initialization_pending)
	{
		find_profile_section(section);

		match_assert(
			"c:\\halo\\SOURCE\\cseries\\profile.c",
			615,
			section->stack_depth==profile_globals.stack_depth);

		profile_globals.stack_depth--;
		QUERY_TIMEBASE(timebase);
		section->frame_elapsed_timebase += timebase-section->entry_timebase;
		section->stack_depth = NONE;
	}
	else
	{
		section->stack_depth = NONE;
	}

	return;
}

static void profile_timesection_inherit(
	struct profile_timer *parent_timesection,
	struct profile_timer *child_timesection)
{
	match_vassert("c:\\halo\\SOURCE\\cseries\\profile.c", 434,
		parent_timesection->frame_total>=child_timesection->total,
		"parent_timesection->self_msec >= child_timesection->elapsed_msec");

	parent_timesection->frame_total -= child_timesection->total;

	return;
}

static void profile_describe_frame(
	struct profile_frame *frame,
	char *string,
	short maximum_length)
{
	short game_tick_display_count;
	short window_display_count;
	short game_tick_index;
	short window_index;

	/* BUG (preserved for exact matching): January repeatedly appends with
	 * maximum_length-strlen without handling CRT truncation or exhaustion.
	 * A 512-byte dump buffer can be exhausted by the legal 150-tick record.
	 * A corrected build should bound appends and guarantee NUL termination.
	 */
	csstrcpy(string, "");

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string),
		"frame %5d vbl %5I64d tot%6.2f",
		frame->frame_index,
		frame->vertical_blank_index,
		frame->frame.total);

	if (frame->lapsed_frames>0)
	{
		if (global_frame_rate_throttle)
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(lost%3d)", frame->lapsed_frames);
		}
		else
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(free%3d)", frame->lapsed_frames);
		}
	}
	else if (frame->lapsed_msec>0)
	{
		if (global_frame_rate_throttle)
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(l.%3dms)", frame->lapsed_msec);
		}
		else
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(f.%3dms)", frame->lapsed_msec);
		}
	}
	else
	{
		if (frame->lapsed_msec_valid)
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(slowed) ");
		}
		else
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "(synced) ");
		}
	}

	if (frame->idle.frame_total>0.0f)
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "idle%6.2f ", frame->idle.frame_total);
	}
	else
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "           ", frame->idle.frame_total);
	}

	game_tick_display_count = MAX(frame->game_tick_count, 8);

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "game%2d ", frame->game_tick_count);

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), " (");

	for (game_tick_index = 0; game_tick_index<game_tick_display_count; game_tick_index++)
	{
		if (game_tick_index<frame->game_tick_count)
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "%6.2f%s",
				frame->game_ticks[game_tick_index].frame_total,
				game_tick_index<game_tick_display_count-1 ? " " : "");
		}
		else
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "      %s",
				game_tick_index<game_tick_display_count-1 ? " " : "");
		}
	}

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), ")");

	window_display_count = MAX(frame->window_count, local_player_count()+1);

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), " render%6.2f", frame->render.total);

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), " (");

	for (window_index = 0; window_index<window_display_count; window_index++)
	{
		if (window_index<frame->window_count)
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "%s%6.2f%s",
				frame->window_ids[window_index] ? "p" : "n",
				frame->windows[window_index].frame_total,
				window_index<window_display_count-1 ? " " : "");
		}
		else
		{
			_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "       %s",
				window_index<window_display_count-1 ? " " : "");
		}
	}

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), ")");

	if (frame->stall.frame_total>0.0f)
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "stall%6.2f ", frame->stall.frame_total);
	}
	else
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "            ", frame->stall.frame_total);
	}

	if (frame->texture.frame_total>0.0f)
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "tex%6.2f ", frame->texture.frame_total);
	}
	else
	{
		_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "          ", frame->texture.frame_total);
	}

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "r-misc %6.2f ", frame->render.frame_total);

	_snprintf(string+csstrlen(string), maximum_length-csstrlen(string), "f-misc %6.2f ", frame->frame.frame_total);

	csstrncat(string+csstrlen(string), frame->lapsed_reason, maximum_length-csstrlen(string));

	return;
}

static void profile_dump_frame(
	struct profile_frame *frame)
{
	char buffer[0x200];

	if (!profile_globals.framedump_file)
	{
		profile_globals.framedump_file = fopen("d:\\framedump.txt", "wb");
	}

	if (profile_globals.framedump_file && !frame->dumped)
	{
		frame->dumped = TRUE;

		profile_describe_frame(frame, buffer, sizeof(buffer));

		fprintf(profile_globals.framedump_file, "%s\r\n", buffer);

		frame->dumped = TRUE;
	}

	profile_globals.framedump_flush_pending = TRUE;

	return;
}

void profile_frame_end(
	void)
{
	short game_tick_index;
	short window_index;
	short frame_index;

	profile_timesection_end_now(&profile_globals.current_frame.frame);

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 448,
		(profile_globals.current_frame.game_tick_count >= 0) && (profile_globals.current_frame.game_tick_count <= MAXIMUM_GAME_TICKS_PER_FRAME));

	for (game_tick_index = 0; game_tick_index<profile_globals.current_frame.game_tick_count; game_tick_index++)
	{
		profile_timesection_inherit(&profile_globals.current_frame.frame, &profile_globals.current_frame.game_ticks[game_tick_index]);
	}

	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 454,
		(profile_globals.current_frame.window_count >= 0) && (profile_globals.current_frame.window_count <= MAXIMUM_WINDOWS));

	profile_timesection_inherit(&profile_globals.current_frame.frame, &profile_globals.current_frame.render);

	for (window_index = 0; window_index<profile_globals.current_frame.window_count; window_index++)
	{
		profile_timesection_inherit(&profile_globals.current_frame.render, &profile_globals.current_frame.windows[window_index]);
	}

	profile_timesection_inherit(&profile_globals.current_frame.frame, &profile_globals.current_frame.idle);

#ifdef HALO_XBOX
	{
		real ticks = 0.0f;

		for (game_tick_index = 0; game_tick_index<profile_globals.current_frame.game_tick_count; game_tick_index++)
		{
			ticks += profile_globals.current_frame.game_ticks[game_tick_index].frame_total;
		}
		halo_frame_profile(profile_globals.current_frame.frame.frame_total, ticks,
			profile_globals.current_frame.game_tick_count, profile_globals.current_frame.render.frame_total,
			profile_globals.current_frame.idle.frame_total, profile_globals.current_frame.texture.frame_total);
	}
#endif

	profile_globals.frames[profile_globals.current_frame_history_index] = profile_globals.current_frame;

	profile_globals.current_frame_history_count = MAX(profile_globals.current_frame_history_count, profile_globals.current_frame_history_index+1);
	profile_globals.current_frame_history_index = (profile_globals.current_frame_history_index+1)%MAXIMUM_PROFILE_FRAMES;

	if (profile_globals.current_frame.lapsed_msec_valid)
	{
		profile_globals.lost_frame_count = 0;
	}
	else
	{
		profile_globals.lost_frame_count++;

		if (profile_dump_lost_frames && !profile_dump_frames && profile_globals.lost_frame_count>3)
		{
			profile_dump_frame_stop();
		}
	}

	if (profile_dump_frames || (profile_dump_lost_frames && profile_globals.lost_frame_count<=3))
	{
		frame_index = (profile_globals.current_frame_history_index+MAXIMUM_PROFILE_FRAMES-3)%MAXIMUM_PROFILE_FRAMES;

		do
		{
			if (frame_index<profile_globals.current_frame_history_count)
			{
				profile_dump_frame(&profile_globals.frames[frame_index]);
			}

			frame_index = (frame_index+1)%MAXIMUM_PROFILE_FRAMES;
		}
		while (frame_index!=profile_globals.current_frame_history_index);
	}

	return;
}


/* ---------- private code */

static void profile_timesection_begin(
	struct profile_timer *timer,
	__int64 start)
{
	timer->start = start;

	return;
}

static void profile_timesection_end(
	struct profile_timer *timer,
	__int64 end)
{
	real msec;

	timer->end = end;
	msec = (real)((timer->end-timer->start)*1000.0f/profile_globals.timebase_frequency);
	timer->total += msec;
	timer->frame_total += msec;

	return;
}

static boolean string_has_prefix(
	char const *string,
	char const *prefix)
{
	boolean result = TRUE;

	while (*prefix)
	{
		if (*prefix != *string)
		{
			result = FALSE;
			break;
		}

		prefix++;
		string++;
	}

	return result;
}

static void profile_timesection_begin_now(
	struct profile_timer *timer)
{
	__int64 timebase;

	QUERY_TIMEBASE(timebase);
	timer->start = timebase;

	return;
}

static void profile_timesection_end_now(
	struct profile_timer *timer)
{
	__int64 timebase;
	real msec;

	QUERY_TIMEBASE(timebase);
	timer->end = timebase;
	msec = (real)((timer->end-timer->start)*1000.0f/profile_globals.timebase_frequency);
	timer->total += msec;
	timer->frame_total += msec;

	return;
}

int compare_profile_sections(
	void const *section0,
	void const *section1)
{
	struct profile_section *const *first = (struct profile_section *const *)section0;
	struct profile_section *const *second = (struct profile_section *const *)section1;
	int result = 0;

	if ((*first)->active && !(*second)->active)
	{
		result = -1;
	}
	else if ((*second)->active && !(*first)->active)
	{
		result = 1;
	}
	else
	{
		switch (profile_globals.compare_type)
		{
			case _profile_sort_mode_name:
				result = csstrcmp((*first)->name, (*second)->name);
				break;

			case _profile_sort_mode_average_time:
			{
				double first_average = (*first)->sample_count==0 ?
					0.0 :
					(double)(*first)->total_elapsed_timebase/(*first)->sample_count;
				double second_average = (*second)->sample_count==0 ?
					0.0 :
					(double)(*second)->total_elapsed_timebase/(*second)->sample_count;

				if (first_average>second_average)
					result = -1;
				else if (first_average<second_average)
					result = 1;
				break;
			}

			case _profile_sort_mode_total_time:
				if ((*first)->recent_elapsed_timebase>(*second)->recent_elapsed_timebase)
					result = -1;
				else if ((*first)->recent_elapsed_timebase<(*second)->recent_elapsed_timebase)
					result = 1;
				break;

			default:
				match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 844, !"unreachable");
				break;
		}
	}

	return result;
}

static void profile_internal_step(
	void)
{
	if (profile_global_enable)
	{
		short index;

		for (index = 0; index<profile_globals.section_count; index++)
		{
			struct profile_section *section = profile_globals.sections[index];

			if (section->active)
			{
				section->recent_elapsed_timebase -= section->frame_elapsed_timebase_history[profile_globals.history_index];
				section->recent_call_count -= section->frame_call_count_history[profile_globals.history_index];
				section->frame_elapsed_timebase_history[profile_globals.history_index] = section->frame_elapsed_timebase;
				section->frame_call_count_history[profile_globals.history_index] = section->frame_call_count;
				section->recent_elapsed_timebase += section->frame_elapsed_timebase;
				section->recent_call_count += section->frame_call_count;

				if (section->frame_elapsed_timebase>section->peak_elapsed_timebase)
					section->peak_elapsed_timebase = section->frame_elapsed_timebase;
				if (section->frame_call_count>section->peak_call_count)
					section->peak_call_count = section->frame_call_count;

				section->total_elapsed_timebase += section->frame_elapsed_timebase;
				section->total_call_count += section->frame_call_count;
				section->frame_elapsed_timebase = 0;
				section->frame_call_count = 0;
				section->sample_count++;
			}
		}
	}

	profile_globals.history_index = (profile_globals.history_index+1)%MAXIMUM_PROFILE_HISTORY;
	profile_globals.initialization_pending = FALSE;

	return;
}

void find_profile_section(
	struct profile_section *section)
{
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 559, section);
	match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 560, section->active);

	if (section->section_index!=NONE)
	{
		match_vassert("c:\\halo\\SOURCE\\cseries\\profile.c", 566,
			section->section_index >= 0 &&
			section->section_index < profile_globals.section_count &&
			profile_globals.sections[section->section_index] == section,
			"don't call profile_enter_private(), call profile_enter()");
	}
	else
	{
		match_assert("c:\\halo\\SOURCE\\cseries\\profile.c", 570, profile_globals.section_count<MAXIMUM_PROFILE_SECTIONS);

		section->section_index = profile_globals.section_count;
		profile_globals.section_count++;
		profile_globals.sections[section->section_index] = section;

		csmemset(section->frame_elapsed_timebase_history, 0, sizeof(section->frame_elapsed_timebase_history));
		csmemset(section->frame_call_count_history, 0, sizeof(section->frame_call_count_history));

		section->recent_call_count = 0;
		section->recent_elapsed_timebase = 0;
		section->stack_depth = NONE;
		section->sample_count = 0;
		section->frame_elapsed_timebase = 0;
		section->frame_call_count = 0;
		section->total_elapsed_timebase = 0;
		section->total_call_count = 0;
		section->peak_elapsed_timebase = 0;
		section->peak_call_count = 0;
	}

	return;
}

static void profile_dump_frame_stop(
	void)
{
	if (profile_globals.framedump_flush_pending)
	{
		if (profile_globals.framedump_file)
		{
			fprintf(profile_globals.framedump_file, "\r\n");
			fflush(profile_globals.framedump_file);
		}

		profile_globals.framedump_flush_pending = FALSE;
	}

	return;
}

static void profile_sections_activation(
	const char *name,
	boolean active)
{
	boolean all = csstrcmp(name, "*")==0;
	boolean prefix = name[0]=='_';
	short index;

	for (index = 0; index<profile_globals.section_count; index++)
	{
		struct profile_section *section = profile_globals.sections[index];

		if (!all)
		{
			if (prefix)
			{
				if (!string_has_prefix(section->name, name+1))
					goto next_section;
			}
			else if (!strstr(section->name, name))
			{
				goto next_section;
			}
		}

		section->active = active;

next_section:
		;
	}

	return;
}
