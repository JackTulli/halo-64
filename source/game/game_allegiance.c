/*
GAME_ALLEGIANCE.C

symbols in this file:
00095FB0 0030:
	_game_allegiance_initialize (0000)
00095FE0 0010:
	_game_allegiance_dispose (0000)
00095FF0 00a0:
	_game_allegiance_initialize_for_new_map (0000)
00096090 0010:
	_game_allegiance_dispose_from_old_map (0000)
000960A0 0060:
	_game_team_is_enemy (0000)
00096100 0050:
	_game_team_is_ally (0000)
00096150 0060:
	_game_team_ally_status_changed (0000)
000961B0 0080:
	_game_allegiance_get_incidents (0000)
00096230 0070:
	_game_allegiance_provoke (0000)
000962A0 0060:
	_game_allegiance_notify_change (0000)
00096300 01a0:
	_game_allegiance_broken (0000)
000964A0 0080:
	_game_allegiance_update (0000)
00096520 00e0:
	_game_allegiance_create (0000)
00096600 00b0:
	_game_allegiance_remove (0000)
000966B0 00d0:
	_game_allegiance_incident (0000)
0025B0D0 0018:
	??_C@_0BI@FALCEHFN@game?5allegiance?5globals?$AA@ (0000)
0025B0E8 0018:
	??_C@_0BI@MMPFGAEA@game_allegiance_globals?$AA@ (0000)
0025B100 0026:
	??_C@_0CG@MBPCBMLP@c?3?2halo?2SOURCE?2game?2game_allegia@ (0000)
0025B128 0022:
	??_C@_0CC@EJEFDMDC@allegiance?9?$DOcurrent_incidents?5?$DO?5@ (0000)
0025B14C 003d:
	??_C@_0DN@FFLAEGAA@game_allegiance_create?3?5too?5many@ (0000)
002DE3B8 0028:
	_global_game_team_names (0000)
0043E490 0004:
	_game_allegiance_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "ai/ai_runtime.h"
#include "game/game_allegiance.h"
#include "saved games/game_state.h"

/* ---------- constants */

enum
{
	NUMBER_OF_GAME_TEAMS = 10
};

/* ---------- macros */

/* ---------- structures */

struct game_allegiance_record
{
	short team1_index;
	short team2_index;
	short incident_threshold;
	short incident_decay_time;
	boolean team1_suspicious;
	boolean team2_suspicious;
	boolean currently_broken;
	boolean status_changed;
	boolean requires_communication;
	byte reserved0D;
	short current_incidents;
	short current_incident_decay_time;
};

struct game_allegiance_globals
{
	short allegiance_count;
	struct game_allegiance_record allegiances[8];
	byte reserved92[2];
	unsigned long ally_bitvector[4];
	unsigned long friendly_bitvector[4];
};

typedef char game_allegiance_record_size_assert[
	sizeof(struct game_allegiance_record) == 0x12 ? 1 : -1];
typedef char game_allegiance_globals_size_assert[
	sizeof(struct game_allegiance_globals) == 0xB4 ? 1 : -1];
typedef struct game_allegiance_globals game_allegiance_globals_type;

/* ---------- globals */

char const *global_game_team_names[NUMBER_OF_GAME_TEAMS] =
{
	"default",
	"player",
	"human",
	"covenant",
	"flood",
	"sentinel",
	"unused6",
	"unused7",
	"unused8",
	"unused9"
};

static struct game_allegiance_globals *game_allegiance_globals = NULL;

/* ---------- public code */

void game_allegiance_initialize(
	void)
{
	game_allegiance_globals = game_state_malloc(
		"game allegiance globals",
		NULL,
		sizeof(*game_allegiance_globals));
	csmemset(
		game_allegiance_globals,
		0,
		sizeof(*game_allegiance_globals));

	return;
}

void game_allegiance_dispose(
	void)
{
	return;
}

void game_allegiance_initialize_for_new_map(
	void)
{
	game_allegiance_globals_type *globals;
	long bit_index;
	short team_index;

	globals = game_allegiance_globals;
	if (!globals)
	{
		display_assert(
			"game_allegiance_globals",
			"c:\\halo\\SOURCE\\game\\game_allegiance.c",
			87,
			TRUE);
		system_exit(-1);
		globals = game_allegiance_globals;
	}

	globals->allegiance_count = 0;
	csmemset(
		globals->ally_bitvector,
		0,
		sizeof(globals->ally_bitvector));
	csmemset(
		game_allegiance_globals->friendly_bitvector,
		0,
		sizeof(game_allegiance_globals->friendly_bitvector));

	globals = game_allegiance_globals;
	for (team_index = 0;
		team_index < NUMBER_OF_GAME_TEAMS;
		team_index++)
	{
		bit_index = NUMBER_OF_GAME_TEAMS * team_index + team_index;
		BIT_VECTOR_SET_FLAG(
			globals->friendly_bitvector,
			bit_index,
			TRUE);
	}

	return;
}

void game_allegiance_dispose_from_old_map(
	void)
{
	return;
}

boolean game_team_is_enemy(
	short our_team_index,
	short other_team_index)
{
	boolean result = TRUE;

	if (VALID_INDEX(our_team_index, NUMBER_OF_GAME_TEAMS) &&
		VALID_INDEX(other_team_index, NUMBER_OF_GAME_TEAMS))
	{
		result = !BIT_VECTOR_TEST_FLAG(
			game_allegiance_globals->friendly_bitvector,
			NUMBER_OF_GAME_TEAMS * our_team_index + other_team_index);
	}
#ifdef HALO_LINUX
	/* port: free for all gives every player a team, up to the player limit,
	past the table's 10. Treat such a team like teams 0..9, whose diagonal is
	friendly: it is not its own enemy, so a suicide does not score as a kill.
	(game_team_is_ally already matches them: the ally diagonal is clear.) */
	else if (our_team_index == other_team_index &&
		our_team_index >= NUMBER_OF_GAME_TEAMS)
	{
		result = FALSE;
	}
#endif

	return result;
}

boolean game_team_is_ally(
	short our_team_index,
	short other_team_index)
{
	boolean result = FALSE;

	if (VALID_INDEX(our_team_index, NUMBER_OF_GAME_TEAMS) &&
		VALID_INDEX(other_team_index, NUMBER_OF_GAME_TEAMS))
	{
		result = BIT_VECTOR_TEST_FLAG(
			game_allegiance_globals->ally_bitvector,
			NUMBER_OF_GAME_TEAMS * our_team_index + other_team_index);
	}

	return result;
}

boolean game_team_ally_status_changed(
	short our_team_index,
	short other_team_index)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;
	boolean result = FALSE;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		short team1_index = allegiance->team1_index;

		if ((team1_index == our_team_index &&
				allegiance->team2_index == other_team_index) ||
			(allegiance->team2_index == our_team_index &&
				team1_index == other_team_index))
		{
			result = allegiance->status_changed;
			break;
		}
	}

	return result;
}

short game_allegiance_get_incidents(
	short our_team_index,
	short other_team_index,
	short *incident_threshold)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;
	short current_incidents = 0;
	short threshold = NONE;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		if ((allegiance->team1_index == our_team_index &&
				allegiance->team2_index == other_team_index) ||
			(allegiance->team2_index == our_team_index &&
				allegiance->team1_index == other_team_index))
		{
			current_incidents = allegiance->current_incidents;
			threshold = allegiance->incident_threshold;
			break;
		}
	}

	if (incident_threshold)
		*incident_threshold = threshold;

	return current_incidents;
}

void game_allegiance_provoke(
	short aggressor_team_index,
	short victim_team_index)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		if ((allegiance->team1_index == aggressor_team_index &&
				allegiance->team2_index == victim_team_index &&
				allegiance->team2_suspicious) ||
			(allegiance->team2_index == aggressor_team_index &&
				allegiance->team1_index == victim_team_index &&
				allegiance->team1_suspicious))
		{
			if (allegiance->current_incidents > 0 &&
				allegiance->incident_decay_time != NONE)
			{
				allegiance->current_incident_decay_time =
					allegiance->incident_decay_time;
			}

			return;
		}
	}

	return;
}

void game_allegiance_notify_change(
	short team1_index,
	short team2_index)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		if ((allegiance->team1_index == team1_index &&
				allegiance->team2_index == team2_index) ||
			(allegiance->team2_index == team1_index &&
				allegiance->team1_index == team2_index))
		{
			allegiance->status_changed = FALSE;
			return;
		}
	}

	return;
}

/* ---------- private code */

static void game_allegiance_broken(
	struct game_allegiance_record *allegiance,
	boolean currently_broken,
	boolean permanently_broken)
{
	if (!permanently_broken &&
		allegiance->currently_broken == currently_broken)
	{
		return;
	}

	allegiance->currently_broken = currently_broken;
	if (allegiance->team1_index < NUMBER_OF_GAME_TEAMS &&
		allegiance->team2_index < NUMBER_OF_GAME_TEAMS)
	{
		game_allegiance_globals_type *globals = game_allegiance_globals;

		BIT_VECTOR_SET_FLAG(
			globals->ally_bitvector,
			NUMBER_OF_GAME_TEAMS * allegiance->team1_index +
				allegiance->team2_index,
			!permanently_broken);
		BIT_VECTOR_SET_FLAG(
			globals->ally_bitvector,
			NUMBER_OF_GAME_TEAMS * allegiance->team2_index +
				allegiance->team1_index,
			!permanently_broken);
		BIT_VECTOR_SET_FLAG(
			globals->friendly_bitvector,
			NUMBER_OF_GAME_TEAMS * allegiance->team1_index +
				allegiance->team2_index,
			!currently_broken);
		BIT_VECTOR_SET_FLAG(
			globals->friendly_bitvector,
			NUMBER_OF_GAME_TEAMS * allegiance->team2_index +
				allegiance->team1_index,
			!currently_broken);
	}

	allegiance->status_changed = TRUE;
	ai_handle_allegiance_status_changed(
		allegiance->team1_index,
		allegiance->team2_index,
		currently_broken,
		permanently_broken);

	return;
}

void game_allegiance_update(
	void)
{
	struct game_allegiance_record *allegiance;
	short allegiance_index;

	allegiance_index = 0;
	allegiance = game_allegiance_globals->allegiances;
	if (game_allegiance_globals->allegiance_count > 0)
	{
		do
		{
			if (allegiance->current_incident_decay_time > 0)
			{
				allegiance->current_incident_decay_time--;
				if (allegiance->current_incident_decay_time == 0)
				{
					match_assert(
						"c:\\halo\\SOURCE\\game\\game_allegiance.c",
						121,
						allegiance->current_incidents > 0);
					allegiance->current_incidents--;
					if (allegiance->current_incidents == 0)
					{
						game_allegiance_broken(allegiance, FALSE, FALSE);
					}
					else
					{
						allegiance->current_incident_decay_time =
							allegiance->incident_decay_time;
					}
				}
			}

			allegiance_index++;
			allegiance++;
		}
		while (allegiance_index < game_allegiance_globals->allegiance_count);
	}

	return;
}

void game_allegiance_create(
	short team1_index,
	boolean team1_suspicious,
	short team2_index,
	boolean team2_suspicious,
	short incident_threshold,
	short incident_decay_time,
	boolean requires_communication)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;
	short allegiance_team1_index;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	allegiance_index = 0;
	if (allegiance_count > 0)
	{
		do
		{
			allegiance_team1_index = allegiance->team1_index;

			if (allegiance_team1_index == team1_index &&
				allegiance->team2_index == team2_index)
			{
				break;
			}
			if (allegiance->team2_index == team1_index &&
				allegiance_team1_index == team2_index)
			{
				break;
			}

			allegiance_index++;
			allegiance++;
		}
		while (allegiance_index < game_allegiance_globals->allegiance_count);
	}

	if (allegiance_index >= game_allegiance_globals->allegiance_count)
	{
		if (allegiance_count < 8)
		{
			allegiance_index = allegiance_count;
			game_allegiance_globals->allegiance_count = allegiance_count + 1;
		}
		else
		{
			error(
				_error_silent,
				"game_allegiance_create: too many allegiances (maximum is %d)",
				8);
		}
	}

	if (allegiance_index < game_allegiance_globals->allegiance_count)
	{
		struct game_allegiance_record *target =
			&game_allegiance_globals->allegiances[allegiance_index];

		target->team1_index = team1_index;
		target->team1_suspicious = team1_suspicious;
		target->team2_index = team2_index;
		target->team2_suspicious = team2_suspicious;
		target->incident_threshold = incident_threshold;
		target->incident_decay_time = incident_decay_time;
		target->current_incidents = 0;
		target->current_incident_decay_time = 0;
		target->requires_communication = requires_communication;
		target->currently_broken = TRUE;
		game_allegiance_broken(target, FALSE, FALSE);
		target->status_changed = FALSE;
	}

	return;
}

boolean game_allegiance_remove(
	short team1_index,
	short team2_index)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;
	boolean result = FALSE;
	game_allegiance_globals_type *globals;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		if ((allegiance->team1_index == team1_index &&
				allegiance->team2_index == team2_index) ||
			(allegiance->team2_index == team1_index &&
				allegiance->team1_index == team2_index))
		{
			game_allegiance_broken(allegiance, TRUE, TRUE);
			globals = game_allegiance_globals;
			globals->allegiance_count--;
			if (globals->allegiance_count > allegiance_index)
			{
				globals->allegiances[allegiance_index] =
					globals->allegiances[globals->allegiance_count];
			}

			result = TRUE;
			break;
		}
	}

	return result;
}

boolean game_allegiance_incident(
	short aggressor_team_index,
	short victim_team_index,
	short incident_type,
	boolean *notify_immediately)
{
	struct game_allegiance_record *allegiance;
	short allegiance_count;
	short allegiance_index;
	short increment;
	boolean result = FALSE;

	allegiance = game_allegiance_globals->allegiances;
	allegiance_count = game_allegiance_globals->allegiance_count;
	for (allegiance_index = 0;
		allegiance_index < allegiance_count;
		allegiance_index++, allegiance++)
	{
		if ((allegiance->team1_index == aggressor_team_index &&
				allegiance->team2_index == victim_team_index &&
				allegiance->team2_suspicious) ||
			(allegiance->team2_index == aggressor_team_index &&
				allegiance->team1_index == victim_team_index &&
				allegiance->team1_suspicious))
		{
			increment = 0;
			switch (incident_type)
			{
			case _allegiance_incident_accident:
				increment = 1;
				break;
			case _allegiance_incident_betrayal:
				increment = 3;
				break;
			case _allegiance_incident_forgive:
				increment = -1;
				break;
			}

			allegiance->current_incidents += increment;
			if (allegiance->incident_decay_time != NONE)
			{
				allegiance->current_incident_decay_time =
					allegiance->incident_decay_time;
			}

			if (allegiance->incident_threshold != NONE &&
				allegiance->current_incidents >= allegiance->incident_threshold)
			{
				game_allegiance_broken(allegiance, TRUE, FALSE);
				result = TRUE;
				if (notify_immediately)
				{
					*notify_immediately = !allegiance->requires_communication;
				}
			}

			break;
		}
	}

	return result;
}
