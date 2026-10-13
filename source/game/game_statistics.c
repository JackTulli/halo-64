/*
GAME_STATISTICS.C

symbols in this file:
000A49B0 0010:
	_game_statistics_start (0000)
000A49C0 0090:
	_game_statistics_stop (0000)
000A4A50 0010:
	_game_statistics_record_damage (0000)
000A4A60 02a0:
	_game_statistics_record_kill (0000)
0043EE00 0001:
	_game_statistics_active (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"
#include "game/game_allegiance.h"
#include "game/game_engine.h"
#include "game/game_statistics_internal.h"
#include "game/players.h"
#include "memory/data.h"
#include "units/units.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

boolean game_statistics_active = FALSE;

/* ---------- public code */

void game_statistics_start(
	void)
{
	game_statistics_active = TRUE;

	return;
}

void
game_statistics_stop(
	short winning_team_index)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);

	while (player)
	{
		player->statistics.seconds_online = game_time_get() / 30;
		player->statistics.games_played = 1;

		if (player->team_index == winning_team_index)
			player->statistics.games_won++;

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	game_statistics_active = FALSE;

	return;
}

void game_statistics_record_damage(
	long object_index,
	real damage,
	long owner_player_index,
	long owner_object_index,
	short owner_team_index)
{
	return;
}

void game_statistics_record_kill(
	long dead_unit_index,
	long killing_player_index,
	long killing_object_index,
	short killing_team_index)
{
	if (game_statistics_active)
	{
		long dead_player_index = player_index_from_unit_index(dead_unit_index);
		long statistic_index;

		/* only player victims are tallied (NONE otherwise); the later HCEA build
		   still indexes kills[] and assists[] through this NONE-or-zero value */
		if (dead_player_index == NONE)
			statistic_index = NONE;
		else
			statistic_index = 0;

		if (statistic_index != NONE)
		{
			struct player_datum *dead_player = player_get(dead_player_index);
			long dead_team_index = dead_player->team_index;
			boolean friendly_fire;
			long assist_time;
			long recent_kill_time;
			struct unit_datum *dead_unit;
			struct unit_attacker *attackers;
			struct unit_attacker *attacker;
			short killer_attacker_index;
			short best_attacker_index;
			real best_damage;
			short attacker_index;
			long credited_player_index;
			real assist_damage;
			boolean credited_friendly_fire;

			dead_player = player_get(dead_player_index);
			if (killing_player_index == dead_player_index)
				dead_player->statistics.suicides++;

			dead_player->statistics.deaths++;
			dead_player->statistics.kills_in_a_row = 0;
			dead_player->statistics.last_kill_time = NONE;
			dead_player->statistics.multiple_kills = 0;

			friendly_fire = killing_player_index == NONE ||
				!game_team_is_enemy(dead_team_index, killing_team_index);
			assist_time = game_time_get() - 120;
			recent_kill_time = game_time_get() - 180;
			dead_unit = unit_get(dead_unit_index);
			best_attacker_index = NONE;
			best_damage = REAL_MIN;
			killer_attacker_index = NONE;
			attacker_index = 0;
			attacker = dead_unit->unit.attackers;
			attackers = attacker;
			for (;
				attacker_index < MAXIMUM_ATTACKERS_PER_UNIT;
				attacker_index++, attacker++)
			{
				if (attacker->player_index == killing_player_index)
					killer_attacker_index = (short)attacker_index;

				if ((killer_attacker_index == attacker_index ||
					(friendly_fire && game_team_is_enemy(dead_team_index, (short)attacker->player_index))) &&
					attacker->game_time_stamp > (unsigned long)recent_kill_time &&
					attacker->damage_inflicted > best_damage)
				{
					best_attacker_index = attacker_index;
					best_damage = attacker->damage_inflicted;
				}
			}

			credited_friendly_fire = FALSE;

			if (best_attacker_index == NONE)
				best_attacker_index = killer_attacker_index;

			if (best_attacker_index != NONE)
			{
				credited_player_index =
					dead_unit->unit.attackers[best_attacker_index].player_index;
				assist_damage =
					dead_unit->unit.attackers[best_attacker_index].damage_inflicted * 0.4f;
			}
			else
			{
				credited_player_index = killing_player_index;
				assist_damage = 0.f;
			}

			if (credited_player_index != NONE)
			{
				struct player_datum *credited_player = player_get(credited_player_index);

				if (game_team_is_enemy(dead_team_index, (short)credited_player->team_index))
				{
					credited_player->statistics.kills[statistic_index]++;
					credited_player->statistics.kills_in_a_row++;
					if (credited_player->statistics.last_kill_time >= assist_time)
						credited_player->statistics.multiple_kills++;
					else
						credited_player->statistics.multiple_kills = 1;
					credited_player->statistics.last_kill_time = (short)game_time_get();
				}
				else
				{
					credited_player->statistics.friendly_fire_kills++;
					credited_friendly_fire = TRUE;
				}
			}

			if (assist_damage > 0.f)
			{
				attacker = attackers;
				for (attacker_index = 0;
					attacker_index < MAXIMUM_ATTACKERS_PER_UNIT;
					attacker_index++, attacker++)
				{
					if ((attacker_index == killer_attacker_index || attacker->damage_inflicted > assist_damage) &&
						attacker->player_index != NONE &&
						attacker->player_index != credited_player_index)
					{
						struct player_datum *assisting_player = player_get(attacker->player_index);

						if (game_team_is_enemy(dead_team_index, (short)assisting_player->team_index))
							assisting_player->statistics.assists[statistic_index]++;
					}
				}
			}

			game_engine_player_killed(
				credited_player_index,
				killing_object_index,
				dead_player_index,
				credited_friendly_fire);
		}
	}

	return;
}

/* ---------- private code */
