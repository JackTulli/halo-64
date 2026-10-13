/*
ERRORS.C

symbols in this file:
0007D860 0010:
	_errors_dispose (0000)
0007D870 0010:
	_errors_output_to_debug_file (0000)
0007D880 0010:
	_errors_overflow_suppression_enable (0000)
0007D890 0010:
	_error_get (0000)
0007D8A0 0130:
	_write_to_error_file (0000)
0007D9D0 0010:
	_code_0007d9d0 (0000)
0007D9E0 0020:
	_errors_initialize (0000)
0007DA00 0270:
	_error (0000)
0007DC70 0020:
	_errors_handle (0000)
0007DC90 0010:
	_errors_clear (0000)
00257C68 0015:
	??_C@_0BF@MNPBN@?$DMTIME?5UNAVAILABLE?$DO?5?5?$AA@ (0000)
00257C80 0020:
	??_C@_0CA@LIAFHJOP@?$CF02d?4?$CF02d?4?$CF02d?5?$CF02d?3?$CF02d?3?$CF02d?5?5?$AA@ (0000)
00257CA0 000d:
	??_C@_0N@MOPBLMMO@d?3?2debug?4txt?$AA@ (0000)
00257CB0 0018:
	??_C@_0BI@PFJPDCFK@reference?5address?3?5?$CFx?$AN?6?$AA@ (0000)
00257CC8 0019:
	??_C@_0BJ@KFNJAEPH@reference?5function?3?5?$CFs?$AN?6?$AA@ (0000)
00257CE4 0015:
	??_C@_0BF@OBFANCCF@_write_to_error_file?$AA@ (0000)
00257D00 0054:
	??_C@_0FE@EJMLAPFK@halobeta?5xbox?501?401?414?42342?$CICACH@ (0000)
00257D54 0005:
	??_C@_04FMMHFHOO@?$AN?6?$AN?6?$AA@ (0000)
00257D60 0047:
	??_C@_0EH@CIPINKCN@prefix_size?5?$CL?5copy_size?5?$CL?5new_si@ (0000)
00257DA8 0023:
	??_C@_0CD@BCANCLDH@?$FL?4?4?4too?5many?5errors?5to?5print?4?4?4?$FN@ (0000)
00257DCC 002c:
	??_C@_0CM@FGNANMFL@too?5many?5errors?0?5only?5printing?5t@ (0000)
00257DF8 003b:
	??_C@_0DL@DEAPBIIL@priority?$DO?$DN0?5?$CG?$CG?5priority?$DMNUMBER_O@ (0000)
00257E34 0020:
	??_C@_0CA@JEDAOIGL@c?3?2halo?2SOURCE?2cseries?2errors?4c?$AA@ (0000)
002DCD2C 0001:
	_data_002dcd2c (0000)
0031DF2C 0010:
	_bss_0031df2c (0000)
	_find_all_fucked_up_shit (0008)
	_fucked_up_shit_count (000c)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "errors.h"
#include "interface/terminal.h"

#include <stdarg.h>
#include <time.h>

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct error_suppression_globals
{
	long last_error_time;
	long error_count;
};

/* ---------- globals */

boolean data_002dcd2c = TRUE;
struct error_suppression_globals bss_0031df2c = { 0, 0 };
boolean find_all_fucked_up_shit = FALSE;
long fucked_up_shit_count = 0;

/* ---------- public code */

void errors_dispose(
	void)
{
	stack_walk_dispose();

	return;
}

void errors_output_to_debug_file(
	boolean output_to_debug_file)
{
	error_globals.output_to_debug_file = output_to_debug_file;

	return;
}

void errors_overflow_suppression_enable(
	boolean overflow_suppression)
{
	error_globals.overflow_suppression = overflow_suppression;

	return;
}

char *error_get(
	void)
{
	return error_globals.message_buffer;
}

#ifdef HALO_LINUX
/* The native builds keep the log open and flush each line: opening and
closing it per line takes milliseconds on Windows, and a host logs
thousands of lines when a hundred machines join, load or leave. The file is
opened once even if threads log their first lines at once, and each line is
one write, so lines from several threads do not interleave. */
static void write_to_debug_file(
	char const *string,
	boolean date)
{
	static FILE *volatile debug_file = NULL;
	FILE *file = debug_file;
	char prefix[32];

#ifdef HALO_N64
	/* the N64 has no disk: the log is the debug output (n64_main.c) */
	{
		extern void n64_log(const char *text);

		(void)date;
		n64_log(string);
		return;
	}
#endif
	if (!file)
	{
		FILE *opened = fopen("d:\\debug.txt", "a+b");

		if (!opened)
		{
			return;
		}
		file = __sync_val_compare_and_swap(&debug_file, (FILE *)NULL, opened);
		if (file)
		{
			fclose(opened);
		}
		else
		{
			file = opened;
		}
	}

	prefix[0] = 0;
	if (date)
	{
		long timeptr;
		struct tm *_time;

		time(&timeptr);
		_time = localtime(&timeptr);
		if (_time)
		{
			sprintf(
				prefix,
				"%02d.%02d.%02d %02d:%02d:%02d  ",
				_time->tm_mon + 1,
				_time->tm_mday,
				_time->tm_year % 100,
				_time->tm_hour,
				_time->tm_min,
				_time->tm_sec);
		}
		else
		{
			csstrcpy(prefix, "<TIME UNAVAILABLE>  ");
		}
	}
	fprintf(file, "%s%s", prefix, string);
#ifdef HALO_XBOX
	/* the console: a flush is a write to the disk on the game's thread;
	every line also goes out on the network log and into the crash ring
	at once, so the file is flushed every sixteen lines or two seconds */
	{
		static unsigned long lines, flushed_time;
		unsigned long now = GetTickCount();

		if (++lines >= 16 || now - flushed_time >= 2000)
		{
			fflush(file);
			lines = 0;
			flushed_time = now;
		}
	}
#else
	fflush(file);
#endif
}

#endif
void write_to_error_file(
	char *string,
	boolean date)
{
	char line[1024];
	long time_value;

	if (data_002dcd2c)
	{
		data_002dcd2c = FALSE;
		write_to_error_file("\r\n\r\n", FALSE);
		write_to_error_file("halobeta xbox 01.01.14.2342(CACHE) ----------------------------------------------\r\n", TRUE);
		sprintf(line, "reference function: %s\r\n", "_write_to_error_file");
		write_to_error_file(line, TRUE);
		sprintf(line, "reference address: %x\r\n", write_to_error_file);
		write_to_error_file(line, TRUE);
	}

	if (error_globals.output_to_debug_file)
	{
#ifdef HALO_LINUX
		write_to_debug_file(string, date);
	}
#else
		FILE *handle = fopen("d:\\debug.txt", "a+b");
		if (handle)
		{
			if (date)
			{
				long timeptr;
				struct tm *_time;

				time(&timeptr);
				_time = localtime(&timeptr);
				if (_time)
				{
					fprintf(
						handle,
						"%02d.%02d.%02d %02d:%02d:%02d  ",
						_time->tm_mon + 1,
						_time->tm_mday,
						_time->tm_year % 100,
						_time->tm_hour,
						_time->tm_min,
						_time->tm_sec);
				}
				else
				{
					fprintf(handle, "<TIME UNAVAILABLE>  ");
				}
			}
			fprintf(handle, "%s", string);
			fclose(handle);
		}
	}
#endif

	return;
}

void reset_error_state(
	void)
{
	error_globals.delayed = FALSE;
	error_globals.message_buffer_size = 0;

	return;
}

void errors_initialize(
	void)
{
	error_globals.output_to_debug_file = TRUE;
	error_globals.overflow_suppression = TRUE;
	error_globals.delayed = FALSE;
	error_globals.message_buffer_size = 0;
	stack_walk_initialize();

	return;
}

void error(
	short priority,
	const char *format,
	...)
{
	char string[1024];
	char *newline;
	long new_size;

	match_assert(
		"c:\\halo\\SOURCE\\cseries\\errors.c",
		0x61,
		priority>=0 && priority<NUMBER_OF_ERROR_MESSAGE_PRIORITIES);

	if (error_globals.overflow_suppression && priority == _error_silent)
	{
		long time = system_milliseconds();

		if (time > bss_0031df2c.last_error_time+900)
		{
			bss_0031df2c.error_count = 0;
		}
		bss_0031df2c.last_error_time = time;
		if (bss_0031df2c.error_count == 10)
		{
			terminal_printf(
				global_real_argb_white,
				"too many errors, only printing to debug.txt");
		}
		bss_0031df2c.error_count++;
		if (bss_0031df2c.error_count >= 10)
		{
			priority = _error_log;
		}
	}

	if (!error_globals.recursion_lock)
	{
		error_globals.recursion_lock = TRUE;
		if (priority == _error_delayed)
		{
			error_globals.delayed = TRUE;
		}

		if (format)
		{
			va_list argument_list;

			va_start(argument_list, format);
			vsprintf(string, format, argument_list);
			va_end(argument_list);
			csstrcat(string, "\r\n");

			if (priority != _error_log)
			{
				terminal_printf(global_real_argb_white, "%s", string);
			}
			write_to_error_file(string, TRUE);

			new_size = csstrlen(string);
			if (error_globals.message_buffer_size+new_size >= ERROR_MESSAGE_BUFFER_MAXIMUM_SIZE)
			{
				long old_size;
				long copy_size;
				long prefix_size = csstrlen("[...too many errors to print...]\r\n");
				long offset = prefix_size+1024+new_size;

				if (offset < 0)
				{
					offset = 0;
				}
				else if (offset > error_globals.message_buffer_size-1)
				{
					offset = error_globals.message_buffer_size-1;
				}

				newline = strchr(error_globals.message_buffer+offset, '\n');
				if (!newline)
				{
					old_size = error_globals.message_buffer_size;
				}
				else
				{
					old_size = newline-error_globals.message_buffer+1;
				}
				copy_size = error_globals.message_buffer_size-old_size;
				match_assert(
					"c:\\halo\\SOURCE\\cseries\\errors.c",
					0xBF,
					prefix_size + copy_size + new_size < ERROR_MESSAGE_BUFFER_MAXIMUM_SIZE);
				csstrncpy(
					error_globals.message_buffer,
					"[...too many errors to print...]\r\n",
					prefix_size);
				if (copy_size > 0)
				{
					csmemmove(
						error_globals.message_buffer+prefix_size,
						newline,
						copy_size);
				}

				error_globals.message_buffer[prefix_size+copy_size] = 0;
				error_globals.message_buffer_size = (short)(prefix_size+copy_size);
			}

			if (error_globals.message_buffer_size+new_size < ERROR_MESSAGE_BUFFER_MAXIMUM_SIZE)
			{
				csstrcpy(
					error_globals.message_buffer+error_globals.message_buffer_size,
					string);
				error_globals.message_buffer_size += (short)new_size;
			}
		}

		if (priority == _error_immediate)
		{
			system_exit(-4998);
		}
		error_globals.recursion_lock = FALSE;
	}

	return;
}

boolean errors_handle(
	void)
{
	boolean delayed = error_globals.delayed;

	error_globals.delayed = FALSE;
	error_globals.message_buffer_size = 0;

	return delayed;
}

void errors_clear(
	void)
{
	error_globals.delayed = FALSE;
	error_globals.message_buffer_size = 0;

	return;
}

/* ---------- private code */
