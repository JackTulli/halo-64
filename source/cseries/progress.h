/*
PROGRESS.H

header included in hcex build.
*/

#ifndef __PROGRESS_H
#define __PROGRESS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

typedef void (*progress_update_proc)(void *context, const char *description, long status, long percent_complete);

struct progress_callback
{
	progress_update_proc update;
	void *context;
};

struct progress_data
{
	progress_update_proc update;
	void *context;
	char description[256];
	long total;
	unsigned long last_update_milliseconds;
};

/* ---------- prototypes/PROGRESS.C */

void progress_new(struct progress_data *data, const struct progress_callback *callback, const char *description, long total);
void progress_update(struct progress_data *data, long status, long completed, boolean force_update);

/* ---------- globals */

/* ---------- public code */

#endif // __PROGRESS_H
