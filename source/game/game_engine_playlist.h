/*
GAME_ENGINE_PLAYLIST.H
*/

#ifndef __GAME_ENGINE_PLAYLIST_H
#define __GAME_ENGINE_PLAYLIST_H
#pragma once

/* ---------- headers */

#include "game/game_engine.h"

/* ---------- prototypes/GAME_ENGINE.C */

void game_engine_playlist_next(
	long parameter0,
	long parameter1,
	long playlist_type);

struct game_variant *build_game_variant_slayer(
	struct game_variant *variant);
struct game_variant *build_game_variant_slayer_pro(
	struct game_variant *variant);
struct game_variant *build_game_variant_elimination(
	struct game_variant *variant);
struct game_variant *build_game_variant_phantoms(
	struct game_variant *variant);
struct game_variant *build_game_variant_endurance(
	struct game_variant *variant);
struct game_variant *build_game_variant_rockets(
	struct game_variant *variant);
struct game_variant *build_game_variant_snipers(
	struct game_variant *variant);
struct game_variant *build_game_variant_oddball(
	struct game_variant *variant);
struct game_variant *build_game_variant_reverse_tag(
	struct game_variant *variant);
struct game_variant *build_game_variant_accumulation(
	struct game_variant *variant);
struct game_variant *build_game_variant_juggernaut(
	struct game_variant *variant);
struct game_variant *build_game_variant_stalker(
	struct game_variant *variant);
struct game_variant *build_game_variant_king(
	struct game_variant *variant);
struct game_variant *build_game_variant_king_pro(
	struct game_variant *variant);
struct game_variant *build_game_variant_crazy_king(
	struct game_variant *variant);
struct game_variant *build_game_variant_race(
	struct game_variant *variant);
struct game_variant *build_game_variant_rally(
	struct game_variant *variant);
struct game_variant *build_game_variant_ctf(
	struct game_variant *variant);
struct game_variant *build_game_variant_invasion(
	struct game_variant *variant);
struct game_variant *build_game_variant_iron_ctf(
	struct game_variant *variant);
struct game_variant *build_game_variant_ctf_pro(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_race(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_rally(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_oddball(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_king(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_slayer(
	struct game_variant *variant);

#endif // __GAME_ENGINE_PLAYLIST_H
