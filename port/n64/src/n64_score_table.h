/*
N64_SCORE_TABLE.H

The scores' table at a game's end. source/game/game_engine.c builds it
from Halo's own rows, places and strings (game_engine_rasterize_in_game_score
and game_engine_post_rasterize_post_game's), and
port/n64/game/n64_hud_game.c draws it after the view's HUD, both styles:

- while the game ends (postgame state 1), each view's title, place, name
  and score, its own player's row marked;
- after it (states 2 and 3), the one view's whole table: place, name,
  score, kills, deaths, and assists where they fit; in a team game each
  team's row with its total, its players under it; PRESS A as Halo shows
  its prompt (hidden until state 3, then fading in as A begins to work).

The scores are in the unit the game counts to its score to win: King of
the Hill and Oddball in whole seconds, from 1000 s on as minutes:seconds
("16:40"); a Race player's laps (a Team Race's total, the team's laps);
kills (below 0 too) and flags as they are.

Not in Firefight (a game against the AI, with its own wave, lives and score
line, and its own results in the menu).

The record is on the heap only while a table shows (game_engine.c), built
again only when a score changes; each view's layout is measured once for
it (n64_hud_game.c).
*/

#ifndef N64_SCORE_TABLE_H
#define N64_SCORE_TABLE_H

#define N64_TABLE_PLAYERS 12
/* the views' layouts: the 4 players' views, and the one view after the game */
#define N64_TABLE_VIEWS 5

enum
{
	N64_TABLE_PLACE,
	N64_TABLE_NAME,
	N64_TABLE_SCORE,
	N64_TABLE_KILLS,
	N64_TABLE_ASSISTS,
	N64_TABLE_DEATHS,
	N64_TABLE_COLUMNS
};

struct n64_table_row
{
	long player_index;
	/* the cells' text: Halo's place ("1st" to "16th"; ties share one), the
	name, the score, kills, assists, deaths */
	char cell[N64_TABLE_COLUMNS][12];
	signed char team; /* 0 red, 1 blue; -1 without teams */
	unsigned char best; /* (after the game) bit c: the best of column c, as Halo marks it */
	unsigned char local; /* a player on this N64's controllers */
};

/* a view's layout (n64_hud_game.c), measured for the table and the view
as they are now (key) */
struct n64_table_layout
{
	long key[8];
	float x0, x1, squeeze, gap, pad, bar;
	short y0, height, pitch, toff, padv, rule;
	unsigned char title, headings, grouped, prompt_line, team_line;
	signed char columns[N64_TABLE_COLUMNS + 1];
	float x[N64_TABLE_COLUMNS], width[N64_TABLE_COLUMNS];
};

struct n64_table
{
	int count, teams, postgame;
	/* one more each build (the layouts' key); the scores and counts it was
	built from (built again when they change) */
	long generation, signature;
	char headings[N64_TABLE_COLUMNS][10];
	/* the teams, the winner first: name and total */
	char team_name[2][12], team_score[2][12];
	signed char team_index[2];
	struct n64_table_row rows[N64_TABLE_PLAYERS];
	struct n64_table_layout layouts[N64_TABLE_VIEWS];
};

/* the view's table, after its HUD (n64_render_game.c): the current view's
(render.local_player_index; NONE: the one view after the game) */
void n64_game_engine_score_table(void);
/* a table drawn in the current view (n64_hud_game.c): its title (none:
NULL), the view's own player (NONE: none), its text's opacity, PRESS A's
(after the game); the view's layout kept in the table */
void n64_hud_score_table(struct n64_table *table, const char *title, long view_player_index, float alpha,
	float prompt_alpha);

#endif
