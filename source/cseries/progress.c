/*
PROGRESS.C

symbols in this file:
00080280 0060:
	_progress_new (0000)
000802E0 0080:
	_progress_update (0000)
00258688 0005:
	??_C@_04PJOLNDGD@data?$AA@ (0000)
00258690 0022:
	??_C@_0CC@EAAFHEOB@c?3?2halo?2SOURCE?2cseries?2progress?4@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "progress.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void progress_new(
	struct progress_data *data,
	const struct progress_callback *callback,
	const char *description,
	long total)
{
	csmemset(data, 0, sizeof(*data));

	if (callback && callback->update)
	{
		data->update = callback->update;
		data->context = callback->context;

		if (description)
			csstrncpy(data->description, description, sizeof(data->description) - 1);

		data->total = total;
	}
}

void progress_update(
	struct progress_data *data,
	long status,
	long completed,
	boolean force_update)
{
	unsigned long milliseconds;

	match_assert("c:\\halo\\SOURCE\\cseries\\progress.c", 35, data);

	if (data->update && data->total)
	{
		milliseconds = system_milliseconds();
		if (milliseconds - data->last_update_milliseconds > 125 || force_update)
		{
			data->update(data->context, data->description, status, completed * 100 / data->total);
			data->last_update_milliseconds = milliseconds;
		}
	}
}

/* ---------- private code */
