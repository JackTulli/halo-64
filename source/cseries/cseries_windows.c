/*
CSERIES_WINDOWS.C

symbols in this file:
0007C950 0010:
	_display_debug_string (0000)
0007C960 0010:
	_system_exit (0000)
0007C970 0020:
	_system_unique_identifier_get (0000)
0007C990 0050:
	_system_unique_identifiers_equal (0000)
0007C9E0 0010:
	_system_milliseconds (0000)
0007C9F0 0010:
	_system_seconds (0000)
0007CA00 0020:
	_system_get_user_name (0000)
0007CA20 0020:
	_system_calloc (0000)
0007CA40 0010:
	_system_malloc (0000)
0007CA50 0010:
	_system_free (0000)
0007CA60 0080:
	_system_realloc (0000)
0007CAE0 0010:
	_system_get_used_memory_size (0000)
0007CAF0 0050:
	_system_memory_information_get (0000)
0007CB40 0010:
	_system_show_wait_cursor (0000)
0007CB50 0010:
	_system_alert (0000)
0007CB60 0010:
	_system_kill_screen_saver (0000)
0007CB70 00f0:
	_code_0007cb70 (0000)
0007CC60 0060:
	_generic_exception_filter (0000)
002575EC 0029:
	??_C@_0CJ@NOCHNOLG@c?3?2halo?2SOURCE?2cseries?2cseries_w@ (0000)
00257618 0005:
	??_C@_04MFDNODIH@xbox?$AA@ (0000)
00257620 000e:
	??_C@_0O@LDEKIBEC@pointer?$HM?$HMsize?$AA@ (0000)
00257630 0008:
	??_C@_07LOEGKGOL@size?$DO?$DN0?$AA@ (0000)
00257638 001b:
	??_C@_0BL@HJPDFKEO@EXCEPTION_PRIV_INSTRUCTION?$AA@ (0000)
00257654 0017:
	??_C@_0BH@HNBKDLPO@EXCEPTION_INT_OVERFLOW?$AA@ (0000)
0025766C 001d:
	??_C@_0BN@LBLGDPIG@EXCEPTION_INT_DIVIDE_BY_ZERO?$AA@ (0000)
0025768C 0018:
	??_C@_0BI@CHKFJJC@EXCEPTION_FLT_UNDERFLOW?$AA@ (0000)
002576A4 001a:
	??_C@_0BK@COBCCPEI@EXCEPTION_FLT_STACK_CHECK?$AA@ (0000)
002576C0 0017:
	??_C@_0BH@LGFBHAFE@EXCEPTION_FLT_OVERFLOW?$AA@ (0000)
002576D8 0020:
	??_C@_0CA@JJDIIMDB@EXCEPTION_FLT_INVALID_OPERATION?$AA@ (0000)
002576F8 001d:
	??_C@_0BN@KLCBBCKL@EXCEPTION_FLT_INEXACT_RESULT?$AA@ (0000)
00257718 0020:
	??_C@_0CA@JNIHFDGO@EXCEPTION_ARRAY_BOUNDS_EXCEEDED?$AA@ (0000)
00257738 001f:
	??_C@_0BP@NMNHALMC@EXCEPTION_FLT_DENORMAL_OPERAND?$AA@ (0000)
00257758 001d:
	??_C@_0BN@PHPNMCML@EXCEPTION_FLT_DIVIDE_BY_ZERO?$AA@ (0000)
00257778 0023:
	??_C@_0CD@JGPJMHJA@EXCEPTION_NONCONTINUABLE_EXCEPTI@ (0000)
0025779C 001b:
	??_C@_0BL@CMOMFGDN@EXCEPTION_ACCESS_VIOLATION?$AA@ (0000)
002577B8 0016:
	??_C@_0BG@HPCFEJHG@EXCEPTION_SINGLE_STEP?$AA@ (0000)
002577D0 0020:
	??_C@_0CA@LEJEDDLG@EXCEPTION_DATATYPE_MISALIGNMENT?$AA@ (0000)
002577F0 0015:
	??_C@_0BF@FLLDOCJP@EXCEPTION_BREAKPOINT?$AA@ (0000)
00257808 0018:
	??_C@_0BI@GNJKFLBB@unknown?5exception?5?$CF08lX?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "errors.h"

#include <time.h>

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct system_memory_information
{
	long available_physical_memory;
	long total_physical_memory;
};

/* ---------- globals */

/* ---------- public code */

void display_debug_string(
	const char *string)
{
	OutputDebugStringA(string);
	return;
}

void system_exit(
	long code)
{
#ifdef HALO_RELEASE
	/* an assertion a release build skipped (display_assert) carries on; a
	fatal error (errors.c exits with -4998) still stops */
	if (code == -1 && display_assert_skipped)
	{
		display_assert_skipped = FALSE;
		return;
	}
#endif
#ifdef HALO_N64
	/* the crash screen, with the backtrace (port/n64/src/n64_main.c) */
	{
		extern void n64_fatal(const char *text);

		n64_fatal("the game stopped (see the log above)");
	}
#endif
	halt_and_catch_fire();
	return;
}

void system_unique_identifier_get(
	void *identifier)
{
	display_assert(NULL, "c:\\halo\\SOURCE\\cseries\\cseries_windows.c", 65, TRUE);
	halt_and_catch_fire();
	return;
}

long system_unique_identifiers_equal(
	const void *identifier1,
	const void *identifier2)
{
	byte empty_identifier[16];

	csmemset(empty_identifier, 0, sizeof(empty_identifier));
	if (csmemcmp(identifier1, empty_identifier, sizeof(empty_identifier)) != 0)
	{
		if (csmemcmp(identifier1, identifier2, sizeof(empty_identifier)) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

unsigned long system_milliseconds(
	void)
{
	return GetTickCount();
}

unsigned long system_seconds(
	void)
{
	return time(NULL);
}

void system_get_user_name(
	char *user_name,
	short maximum_length)
{
	csstrncpy(user_name, "xbox", maximum_length);
	return;
}

void *system_calloc(
	long count,
	long size)
{
	return GlobalAlloc(GMEM_ZEROINIT, count * size);
}

void *system_malloc(
	long size)
{
	return GlobalAlloc(0, size);
}

void system_free(
	void *pointer)
{
	LocalFree(pointer);
	return;
}

void *system_realloc(
	void *pointer,
	long size)
{
	if (size < 0)
	{
		display_assert("size>=0", "c:\\halo\\SOURCE\\cseries\\cseries_windows.c", 156, TRUE);
		halt_and_catch_fire();
	}

	if (pointer == NULL)
	{
		if (size == 0)
		{
			display_assert("pointer||size", "c:\\halo\\SOURCE\\cseries\\cseries_windows.c", 157, TRUE);
			halt_and_catch_fire();
		}
		return GlobalAlloc(0, size);
	}

	if (size != 0)
	{
		return GlobalReAlloc(pointer, size, GMEM_MOVEABLE);
	}

	LocalFree(pointer);
	return NULL;
}

unsigned long system_get_used_memory_size(
	void *pointer)
{
	return LocalSize(pointer);
}

void system_memory_information_get(
	struct system_memory_information *information)
{
	MEMORYSTATUS status;

	csmemset(&status, 0, sizeof(status));
	status.dwLength = sizeof(status);
	GlobalMemoryStatus(&status);
	csmemset(information, 0, sizeof(*information));
	information->available_physical_memory = status.dwAvailPhys;
	information->total_physical_memory = status.dwTotalPhys;
	return;
}

void system_show_wait_cursor(
	const char *file,
	long line)
{
	return;
}

void system_alert(
	void)
{
	return;
}

void system_kill_screen_saver(
	void)
{
	return;
}

static const char *code_0007cb70(
	unsigned long exception_code)
{
	const char *exception_name = NULL;

	switch (exception_code)
	{
	case EXCEPTION_FLT_INVALID_OPERATION:
		exception_name = "EXCEPTION_FLT_INVALID_OPERATION";
		break;
	case EXCEPTION_FLT_OVERFLOW:
		exception_name = "EXCEPTION_FLT_OVERFLOW";
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		exception_name = "EXCEPTION_FLT_STACK_CHECK";
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		exception_name = "EXCEPTION_FLT_UNDERFLOW";
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		exception_name = "EXCEPTION_INT_DIVIDE_BY_ZERO";
		break;
	case EXCEPTION_INT_OVERFLOW:
		exception_name = "EXCEPTION_INT_OVERFLOW";
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		exception_name = "EXCEPTION_PRIV_INSTRUCTION";
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		exception_name = "EXCEPTION_FLT_INEXACT_RESULT";
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		exception_name = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		exception_name = "EXCEPTION_FLT_DENORMAL_OPERAND";
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		exception_name = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		exception_name = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		break;
	case EXCEPTION_ACCESS_VIOLATION:
		exception_name = "EXCEPTION_ACCESS_VIOLATION";
		break;
	case EXCEPTION_SINGLE_STEP:
		exception_name = "EXCEPTION_SINGLE_STEP";
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		exception_name = "EXCEPTION_DATATYPE_MISALIGNMENT";
		break;
	case EXCEPTION_BREAKPOINT:
		exception_name = "EXCEPTION_BREAKPOINT";
		break;
	}

	return exception_name;
}

long generic_exception_filter(
	unsigned long exception_code,
	PEXCEPTION_POINTERS exception_information)
{
	const char *exception_name = code_0007cb70(exception_code);

	stack_walk_with_context(NULL, 0, exception_information->ContextRecord);
	if (exception_name)
	{
		error(_error_silent, "%s", exception_name);
	}
	else
	{
		error(_error_silent, "unknown exception %08lX", exception_code);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

/* ---------- private code */
