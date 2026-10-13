/*
PROFILE.H
*/

#ifndef __PROFILE_H
#define __PROFILE_H
#pragma once

/* ---------- constants */

enum
{
	MAXIMUM_PROFILE_HISTORY = 120,
};

enum profile_sort_mode
{
	_profile_sort_mode_name,
	_profile_sort_mode_average_time,
	_profile_sort_mode_total_time,

	NUMBER_OF_PROFILE_SORT_MODES
};

enum profile_dump_format_mode
{
	_profile_dump_format_mode_file,
	_profile_dump_format_mode_screen,

	NUMBER_OF_PROFILE_DUMP_FORMAT_MODES
};

/* ---------- macros */

#define profile_enter(section)							\
if (profile_global_enable && section.active)	\
{														\
	profile_enter_private(&section);					\
}														\

#define profile_exit(section)							\
if (profile_global_enable && section.active)	\
{														\
	profile_exit_private(&section);						\
}														\

/* ---------- structures */

union point2d;
union real_argb_color;

struct profile_frame_iterator
{
	short current_buffer_index;
	short next_buffer_index;
};

struct profile_frame_info
{
	__int64 vertical_blank_index;
};

struct profile_section
{
	const char *name;
	long section_index;
	boolean active;
	short stack_depth;
	long field_C;
	unsigned __int64 entry_timebase;
	long recent_call_count;
	byte reserved1C[4];
	__int64 recent_elapsed_timebase;
	long frame_call_count_history[MAXIMUM_PROFILE_HISTORY];
	__int64 frame_elapsed_timebase_history[MAXIMUM_PROFILE_HISTORY];
	long sample_count;
	long frame_call_count;
	__int64 frame_elapsed_timebase;
	long total_call_count;
	byte reserved5DC[4];
	__int64 total_elapsed_timebase;
	long peak_call_count;
	byte reserved5EC[4];
	__int64 peak_elapsed_timebase;
};

typedef char profile_section_size_assert[
	sizeof(struct profile_section) == 0x5F8 ? 1 : -1];
typedef char profile_section_recent_call_count_offset_assert[
	offsetof(struct profile_section, recent_call_count) == 0x18 ? 1 : -1];
typedef char profile_section_reserved1C_offset_assert[
	offsetof(struct profile_section, reserved1C) == 0x1C ? 1 : -1];
typedef char profile_section_recent_elapsed_timebase_offset_assert[
	offsetof(struct profile_section, recent_elapsed_timebase) == 0x20 ? 1 : -1];
typedef char profile_section_frame_call_count_history_offset_assert[
	offsetof(struct profile_section, frame_call_count_history) == 0x28 ? 1 : -1];
typedef char profile_section_frame_elapsed_timebase_history_offset_assert[
	offsetof(struct profile_section, frame_elapsed_timebase_history) == 0x208 ? 1 : -1];
typedef char profile_section_sample_count_offset_assert[
	offsetof(struct profile_section, sample_count) == 0x5C8 ? 1 : -1];
typedef char profile_section_frame_call_count_offset_assert[
	offsetof(struct profile_section, frame_call_count) == 0x5CC ? 1 : -1];
typedef char profile_section_frame_elapsed_timebase_offset_assert[
	offsetof(struct profile_section, frame_elapsed_timebase) == 0x5D0 ? 1 : -1];
typedef char profile_section_total_call_count_offset_assert[
	offsetof(struct profile_section, total_call_count) == 0x5D8 ? 1 : -1];
typedef char profile_section_reserved5DC_offset_assert[
	offsetof(struct profile_section, reserved5DC) == 0x5DC ? 1 : -1];
typedef char profile_section_total_elapsed_timebase_offset_assert[
	offsetof(struct profile_section, total_elapsed_timebase) == 0x5E0 ? 1 : -1];
typedef char profile_section_peak_call_count_offset_assert[
	offsetof(struct profile_section, peak_call_count) == 0x5E8 ? 1 : -1];
typedef char profile_section_reserved5EC_offset_assert[
	offsetof(struct profile_section, reserved5EC) == 0x5EC ? 1 : -1];
typedef char profile_section_peak_elapsed_timebase_offset_assert[
	offsetof(struct profile_section, peak_elapsed_timebase) == 0x5F0 ? 1 : -1];

/* ---------- prototypes/PROFILE.C */

void profile_initialize(
	void);
void profile_tick_start(
	void);
void profile_tick_end(
	void);

void profile_enter_private(
	struct profile_section *section);
void profile_exit_private(
	struct profile_section *section);

void profile_render_window_start(
	boolean player_window);
void profile_render_window_end(void);
void profile_render_start(
	void);
void profile_render_end(
	void);
void profile_frame_start(
	void);
void profile_frame_end(
	void);
void profile_idle_start(
	void);
void profile_idle_end(
	void);
void profile_lapsed_frames(
	short frames,
	boolean lapsed,
	char const *reason);
void profile_lapsed_msec(
	long msec);
void profile_frame_iterator_new(
	struct profile_frame_iterator *iterator);
boolean profile_frame_iterator_next(
	struct profile_frame_iterator *iterator,
	struct profile_frame_info *info);
void profile_frame_get_messages(
	struct profile_frame_iterator *iterator,
	short *message_count,
	short maximum_message_count,
	char **messages,
	union point2d *locations,
	union real_argb_color const **colors);
long profile_frame_get_stalls(
	struct profile_frame_iterator *iterator,
	short *stall_index,
	real *stall_msec);
short profile_find_frame_value(
	const char *name,
	short *section_index_reference);
real profile_frame_get_value(
	struct profile_frame_iterator *iterator,
	short frame_value,
	short section_index);
void profile_seconds_elapsed(
	real seconds);
void profile_texture_start(
	void);
void profile_texture_end(
	void);
void profile_dump(
	const char *name,
	short sort_mode,
	short format_mode,
	short maximum_section_count,
	char *buffer);
void profile_dump_to_file(
	const char *name);

/* ---------- globals */

extern boolean profile_timebase_ticks;
extern boolean profile_global_enable;
extern boolean profile_dump_frames;
extern boolean profile_dump_lost_frames;
extern boolean profile_display;
extern boolean profile_graph;

/* ---------- public code */

#endif // __PROFILE_H
