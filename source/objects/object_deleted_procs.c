/*
OBJECT_DELETED_PROCS.C

symbols in this file:
00128700 0030:
	_object_deleted_procs_call (0000)
0030B378 000c:
	_object_deleted_procs (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "objects.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void ai_handle_deleted_object(long deleted_object_index);
void players_handle_deleted_object(long deleted_object_index);

/* ---------- globals */

object_deleted_proc object_deleted_procs[3] =
{
	objects_fix_for_deleted_object,
	ai_handle_deleted_object,
	players_handle_deleted_object
};

/* ---------- public code */

void object_deleted_procs_call(
	long deleted_object_index)
{
	object_deleted_proc *deleted_proc = object_deleted_procs;
	long deleted_proc_count = NUMBEROF(object_deleted_procs);

	do
	{
		(*deleted_proc)(deleted_object_index);
		deleted_proc++;
	}
	while (--deleted_proc_count);
}

/* ---------- private code */
