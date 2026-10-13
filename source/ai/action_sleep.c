/*
ACTION_SLEEP.C

symbols in this file:
00008660 0030:
	_action_sleep_control (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "ai/actors.h"

/* ---------- constants */

enum
{
	_actor_persistent_control_ticks_offset = 0x3FC,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void action_sleep_control(
	long actor_index)
{
	*(short *)((byte *)actor_get(actor_index) + _actor_persistent_control_ticks_offset) = 0;
}

/* ---------- private code */
