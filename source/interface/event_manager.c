/*
EVENT_MANAGER.C

symbols in this file:
000CB700 0030:
	_event_manager_initialize (0000)
000CB730 0020:
	_event_manager_dispose (0000)
000CB750 0020:
	_event_manager_flush (0000)
000CB770 0010:
	_event_manager_suppress (0000)
000CB780 00c0:
	_get_next_event (0000)
000CB840 0010:
	_event_manager_time_of_last_event (0000)
000CB850 0300:
	_code_000cb850 (0000)
000CBB50 0110:
	_event_manager_update (0000)
002706D8 0066:
	??_C@_0GG@GKBBBOFI@event?5?$CG?$CG?5?$CI?$CIlocal_player_index?$DO?$DN0@ (0000)
00270740 0029:
	??_C@_0CJ@HBEODFJG@c?3?2halo?2SOURCE?2interface?2event_m@ (0000)
00453B60 0168:
	_event_manager_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "event_manager.h"
#include "input.h"

/* ---------- constants */

enum
{
	_event_type_null,
	_event_type_left_stick,
	_event_type_right_stick,
	_event_type_button,

	STICK_EVENT_THRESHOLD = 29490,
	STICK_EVENT_REPEAT_MILLISECONDS = 250
};

/* ---------- macros */

/* ---------- structures */

struct event_manager_state
{
	boolean initialized;
	boolean suppressed;
	short __unknown2;
	unsigned long time_of_last_event;
	struct event_record events[MAXIMUM_GAMEPADS][8];
};

struct event_manager_globals
{
	struct event_manager_state state;
	unsigned long stick_event_times[NUMBER_OF_GAMEPAD_STICKS][MAXIMUM_GAMEPADS];
	long previous_stick_axes[NUMBER_OF_GAMEPAD_STICKS][2][MAXIMUM_GAMEPADS];
};

typedef char verify_event_manager_state_size[
	sizeof(struct event_manager_state) == 0x108 ? 1 : -1];
typedef char verify_event_manager_globals_size[
	sizeof(struct event_manager_globals) == 0x168 ? 1 : -1];

/* ---------- prototypes */

static void queue_event(
	struct event_record *event,
	short controller_index);

/* ---------- globals */

struct event_manager_globals event_manager_globals = { 0 };

/* ---------- public code */

void event_manager_initialize(
	void)
{
	csmemset(&event_manager_globals.state, 0, sizeof(event_manager_globals.state));
	event_manager_globals.state.time_of_last_event = system_milliseconds();
	event_manager_globals.state.initialized = TRUE;
	return;
}

void event_manager_dispose(
	void)
{
	csmemset(&event_manager_globals.state, 0, sizeof(event_manager_globals.state));
	return;
}

void event_manager_flush(
	void)
{
	csmemset(event_manager_globals.state.events, 0, sizeof(event_manager_globals.state.events));
	return;
}

void event_manager_suppress(
	boolean suppress)
{
	event_manager_globals.state.suppressed = suppress;
	return;
}

boolean get_next_event(
	struct event_record *event,
	short local_player_index)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\event_manager.c",
		211,
		event && ((local_player_index>=0 && local_player_index<MAXIMUM_GAMEPADS) || local_player_index==NONE));

	if (event_manager_globals.state.initialized)
	{
		if (local_player_index == NONE)
		{
			local_player_index = 0;
			while (!result)
			{
				if (local_player_index >= MAXIMUM_GAMEPADS)
					break;
				result = get_next_event(event, local_player_index);
				local_player_index++;
			}
		}
		else
		{
			long event_index;

			for (event_index = 7; event_index >= 0; event_index--)
			{
				if (event_manager_globals.state.events[local_player_index][event_index].type != 0)
				{
					*event = event_manager_globals.state.events[local_player_index][event_index];
					event_manager_globals.state.events[local_player_index][event_index].type = 0;
					return TRUE;
				}
			}
		}
	}

	return result;
}

unsigned long event_manager_time_of_last_event(
	void)
{
	return event_manager_globals.state.time_of_last_event;
}

#ifdef HALO_LINUX
void event_manager_post_button(
	short controller_index,
	short button_index)
{
	struct event_record event = {0};

	if (!event_manager_globals.state.initialized ||
		controller_index < 0 ||
		controller_index >= MAXIMUM_GAMEPADS)
	{
		return;
	}
	event.type = _event_type_button;
	event.data.button.index = (byte)button_index;
	event.data.button.value = 1;
	queue_event(&event, controller_index);

	return;
}

#endif
/* ---------- private code */

static void queue_event(
	struct event_record *event,
	short controller_index)
{
	unsigned long time;
	boolean post = TRUE;

	if (event_manager_globals.state.suppressed)
		return;

	time = system_milliseconds();

	if (event->type == _event_type_left_stick)
	{
		long x = event->data.stick.x;
		long y = event->data.stick.y;

		if (ABS(x) < STICK_EVENT_THRESHOLD && ABS(y) < STICK_EVENT_THRESHOLD)
		{
			post = FALSE;
		}
		else if (!((ABS(x) >= STICK_EVENT_THRESHOLD &&
				ABS(event_manager_globals.previous_stick_axes[_gamepad_stick_left][0][controller_index]) < STICK_EVENT_THRESHOLD) ||
			(ABS(y) >= STICK_EVENT_THRESHOLD &&
				ABS(event_manager_globals.previous_stick_axes[_gamepad_stick_left][1][controller_index]) < STICK_EVENT_THRESHOLD) ||
			time - event_manager_globals.stick_event_times[_gamepad_stick_left][controller_index] >= STICK_EVENT_REPEAT_MILLISECONDS))
		{
			post = FALSE;
		}
		else
		{
			event_manager_globals.stick_event_times[_gamepad_stick_left][controller_index] = time;
			post = TRUE;

			if (ABS(x) >= STICK_EVENT_THRESHOLD)
			{
				switch (x >= 0 ? 1 : -1)
				{
				case 1:
					x = event->data.stick.x = 32767;
					break;
				case -1:
					x = event->data.stick.x = -32768;
					break;
				}
			}
			if (ABS(y) >= STICK_EVENT_THRESHOLD)
			{
				switch (y >= 0 ? 1 : -1)
				{
				case 1:
					y = event->data.stick.y = 32767;
					break;
				case -1:
					y = event->data.stick.y = -32768;
					break;
				}
			}
		}

		event_manager_globals.previous_stick_axes[_gamepad_stick_left][0][controller_index] = x;
		event_manager_globals.previous_stick_axes[_gamepad_stick_left][1][controller_index] = y;
	}
	else if (event->type == _event_type_right_stick)
	{
		long x = event->data.stick.x;
		long y = event->data.stick.y;

		if (ABS(x) < STICK_EVENT_THRESHOLD && ABS(y) < STICK_EVENT_THRESHOLD)
		{
			post = FALSE;
		}
		else if (!((ABS(x) >= STICK_EVENT_THRESHOLD &&
				ABS(event_manager_globals.previous_stick_axes[_gamepad_stick_right][0][controller_index]) < STICK_EVENT_THRESHOLD) ||
			(ABS(y) >= STICK_EVENT_THRESHOLD &&
				ABS(event_manager_globals.previous_stick_axes[_gamepad_stick_right][1][controller_index]) < STICK_EVENT_THRESHOLD) ||
			time - event_manager_globals.stick_event_times[_gamepad_stick_right][controller_index] >= STICK_EVENT_REPEAT_MILLISECONDS))
		{
			post = FALSE;
		}
		else
		{
			event_manager_globals.stick_event_times[_gamepad_stick_right][controller_index] = time;
			post = TRUE;

			if (ABS(x) >= STICK_EVENT_THRESHOLD)
			{
				switch (x >= 0 ? 1 : -1)
				{
				case 1:
					x = event->data.stick.x = 32767;
					break;
				case -1:
					x = event->data.stick.x = -32768;
					break;
				}
			}
			if (ABS(y) >= STICK_EVENT_THRESHOLD)
			{
				switch (y >= 0 ? 1 : -1)
				{
				case 1:
					y = event->data.stick.y = 32767;
					break;
				case -1:
					y = event->data.stick.y = -32768;
					break;
				}
			}
		}

		event_manager_globals.previous_stick_axes[_gamepad_stick_right][0][controller_index] = x;
		event_manager_globals.previous_stick_axes[_gamepad_stick_right][1][controller_index] = y;
	}

	if (post)
	{
		event->controller_index = controller_index;
		csmemmove(
			event_manager_globals.state.events[controller_index],
			&event_manager_globals.state.events[controller_index][1],
			sizeof(event_manager_globals.state.events[controller_index]) - sizeof(struct event_record));
		event_manager_globals.state.events[controller_index][0] = *event;
		if (event->type != _event_type_null)
		{
			event_manager_globals.state.time_of_last_event = time;
		}
	}

	return;
}

void event_manager_update(
	void)
{
	short gamepad_index;

	if (!event_manager_globals.state.initialized)
		return;

	for (gamepad_index = 0; gamepad_index < MAXIMUM_GAMEPADS; gamepad_index++)
	{
		boolean posted = FALSE;

		if (input_has_gamepad(gamepad_index))
		{
			struct gamepad_state const *state = input_get_gamepad_state(gamepad_index);

			if (state)
			{
				struct event_record event;
				short button_index;

				if (state->sticks[_gamepad_stick_left].x != 0 ||
					state->sticks[_gamepad_stick_left].y != 0)
				{
					event.type = _event_type_left_stick;
					event.data.stick = state->sticks[_gamepad_stick_left];
					queue_event(&event, gamepad_index);
					posted = TRUE;
				}

				if (state->sticks[_gamepad_stick_right].x != 0 ||
					state->sticks[_gamepad_stick_right].y != 0)
				{
					event.type = _event_type_right_stick;
					event.data.stick = state->sticks[_gamepad_stick_right];
					queue_event(&event, gamepad_index);
					posted = TRUE;
				}

				for (button_index = 0; button_index < NUMBER_OF_GAMEPAD_BUTTONS; button_index++)
				{
					byte value = state->buttons[button_index];

					if (value)
					{
						event.type = _event_type_button;
						event.data.button.index = (byte)button_index;
						event.data.button.value = value;
						queue_event(&event, gamepad_index);
						posted = TRUE;
					}
				}
			}
		}

		if (!posted)
		{
			struct event_record event = {0};

			queue_event(&event, gamepad_index);
		}
	}

	return;
}
