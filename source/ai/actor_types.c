/*
ACTOR_TYPES.C

symbols in this file:
00028C10 0140:
	_actor_type_definition_get (0000)
00028D50 0020:
	_actor_types_initialize (0000)
00028D70 0010:
	_actor_type_get_name (0000)
00028D80 0020:
	_actor_type_get_race (0000)
00028DA0 0020:
	_actor_type_get_when_to_search_at_target (0000)
00028DC0 0020:
	_actor_type_get_when_to_pursue (0000)
00028DE0 0020:
	_actor_type_get_when_to_search_pursuit (0000)
00028E00 0010:
	_actor_type_get_pursuit_controller (0000)
00028E10 0010:
	_actor_type_get_swarm (0000)
00028E20 0030:
	_actor_type_initialize (0000)
00028E50 0060:
	_actor_type_decide_action (0000)
00028EB0 0080:
	_actor_type_swarm_control (0000)
00028F30 0070:
	_actor_type_swarm_aim_jump (0000)
0024655C 0008:
	??_C@_07LAMOHCEE@monitor?$AA@ (0000)
00246564 000d:
	??_C@_0N@LIECLPHB@carrier_form?$AA@ (0000)
00246574 000f:
	??_C@_0P@FOBOHLGD@infection_form?$AA@ (0000)
00246584 000c:
	??_C@_0M@EFOJOBCH@combat_form?$AA@ (0000)
00246590 0007:
	??_C@_06BALNJMNP@player?$AA@ (0000)
00246598 0009:
	??_C@_08BFJJNNGH@assassin?$AA@ (0000)
002465A8 005e:
	??_C@_0FO@OPBGDNOE@actor_type_definitions?$FLactor_typ@ (0000)
00246608 0056:
	??_C@_0FG@KLDODOHK@actor_type_definitions?$FLactor_typ@ (0000)
00246660 0060:
	??_C@_0GA@NAINEHLM@actor_type_definitions?$FLactor_typ@ (0000)
002466C0 0032:
	??_C@_0DC@GKACNFKF@actor_type_definitions?$FLactor_typ@ (0000)
002466F4 0029:
	??_C@_0CJ@EFLHHJNK@actor_type_definitions?$FLactor_typ@ (0000)
00246720 0023:
	??_C@_0CD@DNHOLEJM@actor_type_definitions?$FLactor_typ@ (0000)
00246744 0032:
	??_C@_0DC@LEOJEHKO@actor_type?$DO?$DN0?5?$CG?$CG?5actor_type?$DMNUMB@ (0000)
00246778 002b:
	??_C@_0CL@LGMIAAJI@c?3?2halo?2source?2ai?2actor_type_def@ (0000)
002467A4 0025:
	??_C@_0CF@ECBFEJFF@actor_type_definition?9?$DOdecide_ac@ (0000)
002467CC 0020:
	??_C@_0CA@PFDDBNCA@c?3?2halo?2SOURCE?2ai?2actor_types?4c?$AA@ (0000)
002467EC 0025:
	??_C@_0CF@DIDKOMFD@actor_type_definition?9?$DOswarm_con@ (0000)
00246814 001d:
	??_C@_0BN@JJKIIINM@actor_type_definition?9?$DOswarm?$AA@ (0000)
002B6C68 0080:
	_actor_type_definitions (0000)
	_global_actor_type_names (0040)
*/

/* ---------- headers */

#include "cseries.h"

#include "actors.h"
#include "actor_types.h"

/* ---------- constants */

/* ---------- macros */


/* ---------- structures */

/* ---------- prototypes */

static struct actor_type_definition *actor_type_definition_get(
	short actor_type);

/* ---------- globals */

extern struct actor_type_definition actor_type_elite;
extern struct actor_type_definition actor_type_jackal;
extern struct actor_type_definition actor_type_grunt;
extern struct actor_type_definition actor_type_hunter;
extern struct actor_type_definition actor_type_engineer;
extern struct actor_type_definition actor_type_marine;
extern struct actor_type_definition actor_type_crew;
extern struct actor_type_definition actor_type_flood;
extern struct actor_type_definition actor_type_infection;
extern struct actor_type_definition actor_type_carrier;
extern struct actor_type_definition actor_type_sentinel;
extern struct actor_type_definition actor_type_mounted_weapon;

static struct actor_type_definition *actor_type_definitions[NUMBER_OF_ACTOR_TYPES] =
{
	&actor_type_elite,
	&actor_type_jackal,
	&actor_type_grunt,
	&actor_type_hunter,
	&actor_type_engineer,
	&actor_type_elite,
	&actor_type_marine,
	&actor_type_marine,
	&actor_type_crew,
	&actor_type_flood,
	&actor_type_infection,
	&actor_type_carrier,
	&actor_type_sentinel,
	&actor_type_sentinel,
	&actor_type_grunt,
	&actor_type_mounted_weapon
};

const char *global_actor_type_names[NUMBER_OF_ACTOR_TYPES] =
{
	"elite",
	"jackal",
	"grunt",
	"hunter",
	"engineer",
	"assassin",
	"player",
	"marine",
	"crew",
	"combat_form",
	"infection_form",
	"carrier_form",
	"monitor",
	"sentinel",
	"none",
	"mounted_weapon"
};

/* ---------- public code */

void actor_types_initialize(
	void)
{
	short actor_type;

	for (actor_type = 0; actor_type < NUMBER_OF_ACTOR_TYPES; actor_type++)
		actor_type_definition_get(actor_type);

	return;
}

const char *actor_type_get_name(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->name;
}

short actor_type_get_race(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->race;
}

short actor_type_get_when_to_search_at_target(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->when_to_search_at_target;
}

short actor_type_get_when_to_pursue(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->when_to_pursue;
}

short actor_type_get_when_to_search_pursuit(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->when_to_search_pursuit;
}

byte actor_type_get_pursuit_controller(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->pursuit_controller;
}

boolean actor_type_get_swarm(
	short actor_type)
{
	return actor_type_definition_get(actor_type)->swarm;
}

void actor_type_initialize(
	long actor_index)
{
	struct actor_type_definition *actor_type_definition = actor_type_definition_get(actor_get(actor_index)->meta.type);

	if (actor_type_definition->initialize)
		actor_type_definition->initialize(actor_index);

	return;
}

void actor_type_decide_action(
	long actor_index)
{
	struct actor_type_definition *actor_type_definition = actor_type_definition_get(actor_get(actor_index)->meta.type);

	match_assert("c:\\halo\\SOURCE\\ai\\actor_types.c", 129, actor_type_definition->decide_action);
	actor_type_definition->decide_action(actor_index);

	return;
}

void actor_type_swarm_control(
	long actor_index)
{
	struct actor_type_definition *actor_type_definition = actor_type_definition_get(actor_get(actor_index)->meta.type);

	match_assert("c:\\halo\\SOURCE\\ai\\actor_types.c", 141, actor_type_definition->swarm);
	match_assert("c:\\halo\\SOURCE\\ai\\actor_types.c", 142, actor_type_definition->swarm_control);
	actor_type_definition->swarm_control(actor_index);

	return;
}

void actor_type_swarm_aim_jump(
	long actor_index,
	long unit_index,
	real jump_magnitude,
	real_vector3d *jump_velocity)
{
	struct actor_type_definition *actor_type_definition = actor_type_definition_get(actor_get(actor_index)->meta.type);

	match_assert("c:\\halo\\SOURCE\\ai\\actor_types.c", 157, actor_type_definition->swarm);
	if (actor_type_definition->swarm_aim_jump)
	{
		actor_type_definition->swarm_aim_jump(
			actor_index,
			unit_index,
			jump_magnitude,
			jump_velocity);
	}

	return;
}

/* ---------- private code */

static struct actor_type_definition *actor_type_definition_get(
	short actor_type)
{
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 46, actor_type>=0 && actor_type<NUMBER_OF_ACTOR_TYPES);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 47, actor_type_definitions[actor_type]);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 50, actor_type_definitions[actor_type]->name);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 51, actor_type_definitions[actor_type]->decide_action);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 53, actor_type_definitions[actor_type]->when_to_search_at_target < NUMBER_OF_ACTOR_PURSUIT_SETTINGS);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 54, actor_type_definitions[actor_type]->when_to_pursue < NUMBER_OF_ACTOR_PURSUIT_SETTINGS);
	match_assert("c:\\halo\\source\\ai\\actor_type_definitions.h", 55, actor_type_definitions[actor_type]->when_to_search_pursuit < NUMBER_OF_ACTOR_PURSUIT_SETTINGS);

	return actor_type_definitions[actor_type];
}
