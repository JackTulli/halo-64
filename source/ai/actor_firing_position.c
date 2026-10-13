/*
ACTOR_FIRING_POSITION.C

symbols in this file:
00012610 0050:
	_firing_position_store_evaluation_debug (0000)
00012660 0010:
	_firing_position_reject (0000)
00012670 00d0:
	_pre_evaluator_guard (0000)
00012740 0240:
	_pre_evaluator_combatmove (0000)
00012980 00e0:
	_post_evaluator_global (0000)
00012A60 0180:
	_post_evaluator_pursuit (0000)
00012BE0 00e0:
	_post_evaluator_hide (0000)
00012CC0 00c0:
	_post_evaluator_uncover (0000)
00012D80 00e0:
	_post_evaluator_attack (0000)
00012E60 0040:
	_firing_position_pre_evaluate (0000)
00012EA0 0070:
	_firing_position_post_evaluate (0000)
00012F10 0050:
	_firing_positions_get_post_evaluation_bound (0000)
00012F60 0110:
	_firing_position_compare (0000)
00013070 0120:
	_actor_get_firing_position_group (0000)
00013190 0060:
	_actor_clear_discarded_firing_positions (0000)
000131F0 00c0:
	_actor_discard_firing_position (0000)
000132B0 0050:
	_actor_firing_position_discarded (0000)
00013300 0650:
	_pre_evaluator_global (0000)
00013950 01d0:
	_pre_evaluator_pursuit (0000)
00013B20 0290:
	_pre_evaluator_panic (0000)
00013DB0 01d0:
	_code_00013db0 (0000)
00013F80 0090:
	_code_00013f80 (0000)
00014010 0210:
	_actor_nearby_firing_positions (0000)
00014220 1480:
	_actor_select_firing_position (0000)
000156A0 0240:
	_actor_active_select_firing_position (0000)
000158E0 0140:
	_actor_change_firing_position (0000)
00015A20 0460:
	_pre_evaluator_attack (0000)
00244794 0068:
	_global_pre_evaluator_table (0000)
	_global_post_evaluator_table (0038)
002447FC 002e:
	??_C@_0CO@CPIODGNI@?$CIevaluation?5?$DO?$DN?50?40f?$CJ?5?$CG?$CG?5?$CIevaluat@ (0000)
0024482C 002a:
	??_C@_0CK@HINGLJIA@c?3?2halo?2SOURCE?2ai?2actor_firing_p@ (0000)
00244858 0004:
	__real@447a0000 (0000)
0024485C 0004:
	__real@41080000 (0000)
00244860 0004:
	__real@41700000 (0000)
00244864 0004:
	__real@40a00000 (0000)
00244868 0004:
	__real@41400000 (0000)
0024486C 0004:
	__real@40f00000 (0000)
00244870 0004:
	__real@41a00000 (0000)
00244874 0020:
	??_C@_0CA@KIJFLGCC@firing_position?9?$DOvalid?5?$DN?$DN?5valid?$AA@ (0000)
00244898 0048:
	??_C@_0EI@IDDMBEDB@?$CIindex2?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIindex2?5?$DM?5globa@ (0000)
002448E0 0048:
	??_C@_0EI@BDJPJEGE@?$CIindex1?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIindex1?5?$DM?5globa@ (0000)
00244928 002c:
	??_C@_0CM@BJJOKNGI@global_temporary_sort_firing_pos@ (0000)
00244954 003b:
	??_C@_0DL@JAGLMMAN@?$CIindex?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIindex?5?$DM?5NUMBER_@ (0000)
00244990 0004:
	__real@42dfec4b (0000)
00244994 0004:
	__real@c1700000 (0000)
00244998 0004:
	__real@42800000 (0000)
0024499C 0004:
	__real@3be38e39 (0000)
002449A0 0004:
	__real@44610000 (0000)
002449A4 0004:
	__real@40200000 (0000)
002449A8 0060:
	??_C@_0GA@KIPPIIL@?$CIactor?9?$DOdanger_zone?4danger_type?5@ (0000)
00244A08 0004:
	__real@3fb504f3 (0000)
00244A0C 0004:
	__real@40555555 (0000)
00244A10 0004:
	__real@42440000 (0000)
00244A14 0004:
	__real@41800000 (0000)
00244A18 001f:
	??_C@_0BP@KIOMHHMP@evaluation_context?9?$DOhas_target?$AA@ (0000)
00244A38 0052:
	??_C@_0FC@JPOFKNDC@?$CItest_surface_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIte@ (0000)
00244A90 005b:
	??_C@_0FL@LHFKPKFO@fp?9eval?5?$CFs?3?5encounter?5?$CF3d?5consid@ (0000)
00244AEC 0007:
	??_C@_06GFDMOFFM@pursue?$AA@ (0000)
00244AF4 0006:
	??_C@_05MGHOCOML@cover?$AA@ (0000)
00244AFC 0015:
	??_C@_0BF@JGHAJKFH@?$CBexpected_to_discard?$AA@ (0000)
00244B14 001a:
	??_C@_0BK@FJCPIFLD@firing_position_count?5?$DO?50?$AA@ (0000)
00244B30 0004:
	__real@43c80000 (0000)
00244B34 0035:
	??_C@_0DF@NLMOLOFE@encounter?5?$CFs?5has?5too?5many?5firing@ (0000)
00244B6C 0016:
	??_C@_0BG@HHMJFCBB@area_path_state_valid?$AA@ (0000)
00244B84 0004:
	__real@42a00000 (0000)
00244B88 0020:
	??_C@_0CA@EEMPDNKJ@?$CBactor?9?$DOinput?4vehicle_passenger?$AA@ (0000)
00244BA8 0046:
	??_C@_0EG@KOFLIKBB@?$CItotal_groups?5?$CG?5currently_allowe@ (0000)
00244BF0 001e:
	??_C@_0BO@EAEPOCDN@actor_index?5?$CB?$DN?5previous_owner?$AA@ (0000)
00244C10 0024:
	??_C@_0CE@DHIELLBI@actor?9?$DOmeta?4encounter_index?5?$CB?$DN?5N@ (0000)
00244C34 0004:
	__real@41440000 (0000)
00244C38 0039:
	??_C@_0DJ@OIJCIDAH@?$CGevaluation_context?9?$DOattack_vect@ (0000)
00319CA0 0008:
	_global_temporary_sort_firing_position_count (0000)
	_global_temporary_sort_firing_position_array (0004)
*/


/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"

#include "actor_definitions.h"
#include "actor_perception.h"
#include "actors.h"
#include "ai.h"
#include "ai_debug.h"
#include "ai_profile.h"
#include "ai_scenario_definitions.h"
#include "encounters.h"
#include "props.h"
#include "game/game.h"
#include "items/weapon_definitions.h"
#include "physics/collision_bsp_definitions.h"
#include "units/vehicles.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"

/* ---------- constants */

enum
{
	_firing_position_attack_vector_friend = 0,
	_firing_position_attack_vector_friend_player,
	_firing_position_attack_vector_dangerous_enemy,
	NUMBER_OF_FIRING_POSITION_ATTACK_VECTOR_TYPES,
};

enum
{
	_actor_definition_flags2_avoid_all_enemy_attack_vectors_bit = 0,
	_actor_definition_flags2_pathfinding_ignores_danger_bit = 4,
};

enum
{
	_actor_combat_status_definite = 3,
};

enum
{
	MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER = 512,
};

/* debug evaluation categories reported through firing_position_store_evaluation_debug;
the values passed by January's out-of-line calls are pinned, the inlined sites carry none */
enum
{
	_firing_position_evaluation_guard_range = 0,
	_firing_position_evaluation_combatmove_range,
	_firing_position_evaluation_pursuit_range,
	_firing_position_evaluation_pursuit_hint_alignment = 3,
	_firing_position_evaluation_pursuit_target_distance,
	_firing_position_evaluation_pursuit_recency = 5,
	_firing_position_evaluation_pursuit_crowding = 6,
	_firing_position_evaluation_pursuit_fresh = 7,
	_firing_position_evaluation_panic_range,
	_firing_position_evaluation_panic_target_distance,
	_firing_position_evaluation_panic_target_approach,
	_firing_position_evaluation_attack_range = 12,
	_firing_position_evaluation_combat_range = 13,
	_firing_position_evaluation_attack_line_of_sight = 14,
	_firing_position_evaluation_enemy_attack_line = 15,
	_firing_position_evaluation_hide_line_of_sight = 18,
	_firing_position_evaluation_uncover_line_of_sight = 20,
	_firing_position_evaluation_preferred_group = 21,
	_firing_position_evaluation_avoid_points = 22,
	_firing_position_evaluation_danger_clearance = 23,
	_firing_position_evaluation_vehicle_heading = 24,
	_firing_position_evaluation_flying_path = 25,
};


/* ---------- macros */

/* ---------- structures */

struct firing_position_pre_evaluator
{
	short evaluation_mode_mask;
	void (*evaluation_function)(
		long actor_index,
		struct firing_position_evaluation_context *evaluation_context,
		short firing_position_count,
		struct firing_position *firing_positions);
};

struct firing_position_post_evaluator
{
	short evaluation_mode_mask;
	long (*evaluation_function)(
		long actor_index,
		struct firing_position_evaluation_context *evaluation_context,
		struct firing_position *firing_position);
};

typedef char actor_firing_position_definition_size_assert[
	sizeof(struct firing_position_definition) == 0x18 ? 1 : -1];
typedef char actor_firing_position_evaluation_context_size_assert[
	sizeof(struct firing_position_evaluation_context) == 0x670 ? 1 : -1];
typedef char actor_firing_position_size_assert[
	sizeof(struct firing_position) == 0x3C ? 1 : -1];
typedef char actor_firing_position_context_rejected_offset_assert[
	offsetof(struct firing_position_evaluation_context, allow_rejected_positions) == 0x14 ? 1 : -1];
typedef char actor_firing_position_context_avoid_count_offset_assert[
	offsetof(struct firing_position_evaluation_context, avoid_point_count) == 0x50 ? 1 : -1];
typedef char actor_firing_position_context_attack_vectors_offset_assert[
	offsetof(struct firing_position_evaluation_context, attack_vectors) == 0x25C ? 1 : -1];
typedef char actor_firing_position_context_target_offset_assert[
	offsetof(struct firing_position_evaluation_context, has_target) == 0x5FC ? 1 : -1];
typedef char actor_firing_position_context_bound_offset_assert[
	offsetof(struct firing_position_evaluation_context, post_evaluation_bound) == 0x660 ? 1 : -1];
typedef char actor_firing_position_valid_offset_assert[
	offsetof(struct firing_position, valid) == 0x30 ? 1 : -1];
typedef char actor_firing_position_evaluation_offset_assert[
	offsetof(struct firing_position, evaluation) == 0x38 ? 1 : -1];
typedef char actor_firing_position_scenario_encounters_offset_assert[
	offsetof(struct scenario, ai_encounters) == 0x42C ? 1 : -1];
typedef char actor_firing_position_encounter_squads_offset_assert[
	offsetof(struct encounter_definition, squads) == 0x80 ? 1 : -1];
typedef char actor_firing_position_encounter_firing_positions_offset_assert[
	offsetof(struct encounter_definition, firing_positions) == 0x98 ? 1 : -1];
typedef char actor_firing_position_encounter_size_assert[
	sizeof(struct encounter_definition) == 0xB0 ? 1 : -1];
typedef char actor_firing_position_squad_groups_offset_assert[
	offsetof(struct squad_definition, firing_position_groups) == 0x54 ? 1 : -1];
typedef char actor_firing_position_squad_size_assert[
	sizeof(struct squad_definition) == 0xE8 ? 1 : -1];
typedef char actor_firing_position_actor_encounter_offset_assert[
	offsetof(struct actor_datum, meta.encounter_index) == 0x34 ? 1 : -1];
typedef char actor_firing_position_actor_squad_offset_assert[
	offsetof(struct actor_datum, meta.squad_index) == 0x3A ? 1 : -1];
typedef char actor_firing_position_actor_searching_offset_assert[
	offsetof(struct actor_datum, state.searching) == 0x98 ? 1 : -1];
typedef char actor_firing_position_actor_defending_offset_assert[
	offsetof(struct actor_datum, emotions.currently_defending) == 0x374 ? 1 : -1];
typedef char actor_discarded_firing_position_size_assert[
	sizeof(struct actor_discarded_firing_position) == 4 ? 1 : -1];
typedef char actor_firing_position_discard_cursor_offset_assert[
	offsetof(struct actor_datum, firing_positions.next_discarded_firing_positions_entry) == 0x3C6 ? 1 : -1];
typedef char actor_firing_position_discard_ring_offset_assert[
	offsetof(struct actor_datum, firing_positions.discarded_firing_positions) == 0x3C8 ? 1 : -1];
typedef char actor_firing_position_discard_index_offset_assert[
	offsetof(struct actor_datum, firing_positions.discarded_firing_positions) +
		offsetof(struct actor_discarded_firing_position, index) == 0x3CA ? 1 : -1];
typedef char actor_firing_position_last_discard_valid_offset_assert[
	offsetof(struct actor_datum, firing_positions.last_discarded_firing_position_valid) == 0x3D8 ? 1 : -1];
typedef char actor_firing_position_last_discard_temporary_offset_assert[
	offsetof(struct actor_datum, firing_positions.last_discarded_firing_position_temporary) == 0x3D9 ? 1 : -1];
typedef char actor_firing_position_last_discard_point_offset_assert[
	offsetof(struct actor_datum, firing_positions.last_discarded_firing_position) == 0x3DC ? 1 : -1];

/* ---------- prototypes */

static void firing_position_store_evaluation_debug(
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position,
	real evaluation,
	short evaluation_type);
static boolean firing_position_reject(
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);

static void pre_evaluator_global(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);
static void pre_evaluator_attack(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);

static void pre_evaluator_combatmove(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);
static void pre_evaluator_guard(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);
static void pre_evaluator_panic(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);
static void pre_evaluator_pursuit(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);

static long post_evaluator_global(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static long post_evaluator_attack(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static long post_evaluator_uncover(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static long post_evaluator_hide(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static long post_evaluator_pursuit(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);

static void firing_position_pre_evaluate(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions);
static boolean firing_position_post_evaluate(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static boolean firing_positions_get_post_evaluation_bound(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context);
static boolean firing_position_compare(
	long index1,
	long index2);
static void firing_position_compute_line_of_sight(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);
static boolean firing_position_forced_evaluation(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position);

/* ---------- globals */

struct firing_position_pre_evaluator const global_pre_evaluator_table[]=
{
	{-1, pre_evaluator_global},
	{FLAG(_firing_point_evaluation_mode_fight)|FLAG(_firing_point_evaluation_mode_uncover), pre_evaluator_attack},
	{FLAG(_firing_point_evaluation_mode_fight)|FLAG(_firing_point_evaluation_mode_cover)|
		FLAG(_firing_point_evaluation_mode_uncover)|FLAG(_firing_point_evaluation_mode_avoid), pre_evaluator_combatmove},
	{FLAG(_firing_point_evaluation_mode_guard), pre_evaluator_guard},
	{FLAG(_firing_point_evaluation_mode_panic), pre_evaluator_panic},
	{FLAG(_firing_point_evaluation_mode_pursue), pre_evaluator_pursuit},
	{0, NULL},
};

struct firing_position_post_evaluator const global_post_evaluator_table[]=
{
	{-1, post_evaluator_global},
	{FLAG(_firing_point_evaluation_mode_fight)|FLAG(_firing_point_evaluation_mode_guard)|
		FLAG(_firing_point_evaluation_mode_avoid), post_evaluator_attack},
	{FLAG(_firing_point_evaluation_mode_uncover), post_evaluator_uncover},
	{FLAG(_firing_point_evaluation_mode_panic)|FLAG(_firing_point_evaluation_mode_cover), post_evaluator_hide},
	{FLAG(_firing_point_evaluation_mode_pursue), post_evaluator_pursuit},
	{0, NULL},
};

short global_temporary_sort_firing_position_count= 0;
struct firing_position *global_temporary_sort_firing_position_array= NULL;

/* ---------- private code */

static void firing_position_store_evaluation_debug(
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position,
	real evaluation,
	short evaluation_type)
{
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		129,
		(evaluation >= 0.0f) && (evaluation < 1e+03f));
	firing_position->evaluation+= evaluation;

	return;
}

static boolean firing_position_reject(
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	firing_position->rejected= TRUE;

	return !evaluation_context->allow_rejected_positions;
}

static void pre_evaluator_guard(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct firing_position *firing_position= firing_positions;
	short index;

	for (index= 0; index<firing_position_count; index++, firing_position++)
	{

		if (firing_position->valid)
		{
			real half_range= evaluation_context->maximum_allowable_range*0.5f;
			real evaluation= 0.0f;

			if (firing_position->path_distance_from_actor<half_range)
			{
				evaluation= 8.0f;
			}
			else if (firing_position->path_distance_from_actor<evaluation_context->maximum_allowable_range)
			{
				evaluation= (1.0f/half_range)*
					(evaluation_context->maximum_allowable_range-firing_position->path_distance_from_actor)*8.0f;
			}

			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_guard_range);
		}
	}

	return;
}

static void pre_evaluator_combatmove(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct actor_variant_definition *variant_definition;
	real range_weight;
	boolean in_range= TRUE;
	struct firing_position *firing_position;
	short index;

	actor_get(actor_index);
	variant_definition= actor_combat_get_firing_variant_definition(actor_index);
	range_weight= 8.0f;

	if (evaluation_context->has_target)
	{
		if (evaluation_context->target_current_distance>variant_definition->ranged_combat.maximum_firing_range)
		{
			in_range= FALSE;
		}
		else
		{
			range_weight= (1.0f-evaluation_context->target_current_distance/
				variant_definition->ranged_combat.maximum_firing_range)*8.0f;
			in_range= range_weight>0.0f;
		}
	}

	if (in_range)
	{
		for (index= 0, firing_position= firing_positions; index<firing_position_count; index++, firing_position++)
		{
			if (firing_position->valid &&
				firing_position->path_distance_from_actor<evaluation_context->maximum_allowable_range)
			{
				firing_position_store_evaluation_debug(evaluation_context, firing_position,
					MAX(0.0f, 1.0f-firing_position->path_distance_from_actor/evaluation_context->maximum_allowable_range)*range_weight,
					_firing_position_evaluation_combatmove_range);
			}
		}
	}

	if (evaluation_context->friend_attack_vector_count>0)
	{
		for (index= 0, firing_position= firing_positions; index<firing_position_count; index++, firing_position++)
		{
			short worst_friend_blockage= 0;
			short worst_player_blockage= 0;
			short vector_index;
			real evaluation;

			if (firing_position->valid)
			{
				for (vector_index= 0; vector_index<evaluation_context->attack_vector_count; vector_index++)
				{
					if (evaluation_context->attack_vectors[vector_index].type==_firing_position_attack_vector_friend ||
						evaluation_context->attack_vectors[vector_index].type==_firing_position_attack_vector_friend_player)
					{
						short blockage= actor_perception_aiming_vector_test_blockage(
							&evaluation_context->attack_vectors[vector_index].point,
							&evaluation_context->attack_vectors[vector_index].vector,
							&firing_position->definition->position, NULL);

						if (evaluation_context->attack_vectors[vector_index].type==_firing_position_attack_vector_friend)
						{
							worst_friend_blockage= MAX(worst_friend_blockage, blockage);
						}
						else if (evaluation_context->attack_vectors[vector_index].type==_firing_position_attack_vector_friend_player)
						{
							worst_player_blockage= MAX(worst_player_blockage, blockage);
						}
					}
				}

				if (worst_player_blockage>=2)
				{
					evaluation= 0.0f;
				}
				else if (worst_player_blockage>=1)
				{
					evaluation= 1.5f;
				}
				else if (worst_friend_blockage>=2)
				{
					evaluation= 6.0f;
				}
				else if (worst_friend_blockage>=1)
				{
					evaluation= 8.5f;
				}
				else
				{
					evaluation= 10.0f;
				}

				firing_position->evaluation+= evaluation;
			}
		}
	}

	return;
}

static long post_evaluator_global(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	struct actor_datum *actor= actor_get(actor_index);

	actor_definition_get(actor->meta.definition_index);

	if (evaluation_context->flying)
	{
		if (!firing_position)
		{
			evaluation_context->post_evaluation_bound+= 15.0f;
		}
		else
		{
			real avoidance_distance= 0.0f;
			boolean path_available= actor_path_3d_available(actor_index, &firing_position->definition->position,
				&avoidance_distance);

			if (path_available)
			{
				path_available= path_3d_available(global_structure_bsp_get(), &actor->input.position.body_position,
					avoidance_distance, &firing_position->definition->position, NULL, NULL);
			}

			if (path_available)
			{
				firing_position_store_evaluation_debug(evaluation_context, firing_position, 15.0f,
					_firing_position_evaluation_flying_path);
			}
			else if (firing_position_reject(evaluation_context, firing_position))
			{
				firing_position->valid= FALSE;
			}
		}
	}

	if (!firing_position)
	{
		return TRUE;
	}

	return firing_position->valid;
}

static long post_evaluator_pursuit(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	struct actor_datum *actor= actor_get(actor_index);
	long current_time= game_time_get();
	long last_examined_time= NONE;
	short examining_actor_count= 0;
	long result= FALSE;

	if (firing_position)
	{
		boolean fresh= TRUE;

		if (firing_position->line_of_sight==_ai_line_of_sight_clear && firing_position->path_distance_from_actor<6.0f)
		{
			encounter_mark_examined_pursuit_position(actor->meta.encounter_index, actor_index,
				firing_position->original_index, evaluation_context->evaluation_data.pursue.last_perceived_time);
			last_examined_time= current_time;
			examining_actor_count= 7;
			fresh= FALSE;
		}
		else
		{
			if (encounter_pursuit_position_already_examined(actor->meta.encounter_index, actor_index,
				firing_position->original_index, evaluation_context->evaluation_data.pursue.last_perceived_time,
				&examining_actor_count, &last_examined_time))
			{
				fresh= FALSE;
			}
		}

		if (evaluation_context->evaluation_data.pursue.tenacious)
		{
			if (fresh)
			{
				firing_position_store_evaluation_debug(evaluation_context, firing_position, 15.0f,
					_firing_position_evaluation_pursuit_fresh);
			}
		}
		else if (!fresh)
		{
			if (firing_position_reject(evaluation_context, firing_position))
			{
				firing_position->valid= FALSE;
			}
		}

		if (firing_position->valid)
		{
			real evaluation= 0.0f;

			if (last_examined_time==NONE || last_examined_time+10*TICKS_PER_SECOND<current_time)
			{
				evaluation= 10.0f;
			}
			else if (last_examined_time<current_time)
			{
				evaluation= (current_time-last_examined_time)*(1.0f/TICKS_PER_SECOND);
			}
			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_pursuit_recency);

			evaluation= 0.0f;
			if (examining_actor_count<4)
			{
				evaluation= (4-examining_actor_count)*5.0f;
			}
			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_pursuit_crowding);
		}

		result= firing_position->valid;
	}

	return result;
}

static long post_evaluator_hide(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	if (evaluation_context->has_target)
	{
		if (!firing_position)
		{
			evaluation_context->post_evaluation_bound+= 12.0f;
		}
		else
		{
			real evaluation= 0.0f;

			switch (firing_position->line_of_sight)
			{
			case _ai_line_of_sight_from_cover:
				evaluation= 12.0f;
				break;
			case _ai_line_of_sight_obstructed:
				evaluation= 10.0f;
				break;
			case _ai_line_of_sight_occluded:
				if (evaluation_context->evaluation_data.panic.forced_to_flee)
				{
					evaluation= 6.0f;
				}
				else if (firing_position_reject(evaluation_context, firing_position))
				{
					firing_position->valid= FALSE;
				}
				break;
			case _ai_line_of_sight_to_cover:
				evaluation= 4.0f;
				break;
			case _ai_line_of_sight_clear:
				if (firing_position_reject(evaluation_context, firing_position))
				{
					firing_position->valid= FALSE;
				}
				break;
			default:
				match_vassert("c:\\halo\\SOURCE\\ai\\actor_firing_position.c", 1117, FALSE, NULL);
				break;
			}

			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_hide_line_of_sight);
		}
	}

	if (!firing_position)
	{
		return TRUE;
	}

	return firing_position->valid;
}

static long post_evaluator_uncover(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	if (evaluation_context->has_target)
	{
		if (!firing_position)
		{
			evaluation_context->post_evaluation_bound+= 20.0f;
		}
		else
		{
			real evaluation= 0.0f;

			switch (firing_position->line_of_sight)
			{
			case _ai_line_of_sight_clear:
				evaluation= 20.0f;
				break;
			case _ai_line_of_sight_occluded:
				evaluation= 10.0f;
				break;
			default:
				{
					real closer_distance= evaluation_context->target_current_distance-7.5f;

					if (closer_distance<0.0f ||
						firing_position->linear_distance_squared_to_target>closer_distance*closer_distance)
					{
						if (firing_position_reject(evaluation_context, firing_position))
						{
							firing_position->valid= FALSE;
						}
					}
				}
				break;
			}

			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_uncover_line_of_sight);
		}
	}

	if (!firing_position)
	{
		return TRUE;
	}

	return firing_position->valid;
}

static long post_evaluator_attack(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	if (evaluation_context->has_target)
	{
		if (!firing_position)
		{
			evaluation_context->post_evaluation_bound+= evaluation_context->target_line_of_sight_optional ? 6.0f : 15.0f;
		}
		else
		{
			real evaluation= 0.0f;

			switch (firing_position->line_of_sight)
			{
			case _ai_line_of_sight_clear:
				if (evaluation_context->target_line_of_sight_optional)
				{
					evaluation= 6.0f;
				}
				else
				{
					evaluation= 15.0f;
				}
				break;
			case _ai_line_of_sight_occluded:
				evaluation= evaluation_context->target_line_of_sight_optional ? 2.5f : 5.0f;
				break;
			default:
				if (!evaluation_context->target_line_of_sight_optional)
				{
					if (firing_position_reject(evaluation_context, firing_position))
					{
						firing_position->valid= FALSE;
					}
				}
				break;
			}

			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_attack_line_of_sight);
		}
	}

	if (!firing_position)
	{
		return TRUE;
	}

	return firing_position->valid;
}

static void firing_position_pre_evaluate(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct firing_position_pre_evaluator const *evaluator;

	for (evaluator= global_pre_evaluator_table; evaluator->evaluation_function; evaluator++)
	{
		if (TEST_FLAG(evaluator->evaluation_mode_mask, evaluation_context->evaluation_mode))
		{
			evaluator->evaluation_function(actor_index, evaluation_context, firing_position_count, firing_positions);
		}
	}

	return;
}

static boolean firing_position_post_evaluate(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	struct firing_position_post_evaluator const *evaluator= global_post_evaluator_table;
	boolean valid= TRUE;

	do
	{
		if (!evaluator->evaluation_function)
		{
			break;
		}
		if (TEST_FLAG(evaluator->evaluation_mode_mask, evaluation_context->evaluation_mode))
		{
			valid= evaluator->evaluation_function(actor_index, evaluation_context, firing_position);
		}
		evaluator++;
	}
	while (valid);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		1262,
		firing_position->valid == valid);

	return valid;
}

static boolean firing_positions_get_post_evaluation_bound(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context)
{
	struct firing_position_post_evaluator const *evaluator= global_post_evaluator_table;
	boolean valid= TRUE;

	evaluation_context->post_evaluation_bound= 0.0f;

	do
	{
		if (!evaluator->evaluation_function)
		{
			break;
		}
		if (TEST_FLAG(evaluator->evaluation_mode_mask, evaluation_context->evaluation_mode))
		{
			valid= evaluator->evaluation_function(actor_index, evaluation_context, NULL);
		}
		evaluator++;
	}
	while (valid);

	return valid;
}

static boolean firing_position_compare(
	long index1,
	long index2)
{
	struct firing_position *firing_position1= &global_temporary_sort_firing_position_array[index1];
	struct firing_position *firing_position2= &global_temporary_sort_firing_position_array[index2];

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		1295,
		global_temporary_sort_firing_position_array);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		1296,
		(index1 >= 0) && (index1 < global_temporary_sort_firing_position_count));
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		1297,
		(index2 >= 0) && (index2 < global_temporary_sort_firing_position_count));

	if (firing_position1->valid!=firing_position2->valid)
	{
		return (firing_position1->valid ? -1 : 1)>0;
	}

	if (firing_position1->rejected!=firing_position2->rejected)
	{
		return (firing_position1->rejected ? 1 : -1)>0;
	}

	if (firing_position1->evaluation>firing_position2->evaluation)
	{
		return FALSE;
	}

	if (firing_position1->evaluation<firing_position2->evaluation)
	{
		return TRUE;
	}

	return FALSE;
}

/* ---------- public code */

long actor_get_firing_position_group(
	long actor_index,
	short evaluation_mode,
	short group_selection_mode)
{
	struct actor_datum *actor = actor_get(actor_index);
	long result = 0;

	if (actor->meta.encounter_index != NONE)
	{
		struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
			&encounter->squads,
			actor->meta.squad_index,
			struct squad_definition);
		boolean searching = actor->state.searching;
		short index;

		switch (group_selection_mode)
		{
		case _firing_position_group_when_searching:
			searching = TRUE;
			break;
		case _firing_position_group_when_not_searching:
			searching = FALSE;
			break;
		}

		if (evaluation_mode == _firing_point_evaluation_mode_panic)
			index = _firing_position_group_defending_guard;
		else if (evaluation_mode == _firing_point_evaluation_mode_guard)
			index = actor->emotions.currently_defending
				? _firing_position_group_defending_guard
				: _firing_position_group_attacking_guard;
		else if (evaluation_mode == _firing_point_evaluation_mode_pursue)
			index = _firing_position_group_pursuing;
		else if (actor->emotions.currently_defending)
			index = searching
				? _firing_position_group_defending_search
				: _firing_position_group_defending;
		else
			index = searching
				? _firing_position_group_attacking_search
				: _firing_position_group_attacking;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
			1412,
			(index >= 0) && (index < NUMBER_OF_FIRING_POSITION_GROUPS));
		result = squad->firing_position_groups[index];
	}

	return result;
}

void actor_clear_discarded_firing_positions(
	long actor_index,
	boolean clear_temporary_only)
{
	struct actor_datum *actor = actor_get(actor_index);
	long index;

	actor->firing_positions.next_discarded_firing_positions_entry = 0;
	for (index = 0; index < NUMBER_OF_DISCARDED_FIRING_POSITIONS_PER_ACTOR; index++)
		actor->firing_positions.discarded_firing_positions[index].index = NONE;

	if (actor->firing_positions.last_discarded_firing_position_valid &&
		(!clear_temporary_only || actor->firing_positions.last_discarded_firing_position_temporary))
	{
		actor->firing_positions.last_discarded_firing_position_valid = FALSE;
	}

	return;
}

void actor_discard_firing_position(
	long actor_index,
	short firing_position_index,
	boolean temporary)
{
	if (firing_position_index != NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct encounter_definition *encounter;
		struct firing_position_definition *firing_position;

		actor->firing_positions.discarded_firing_positions[
			actor->firing_positions.next_discarded_firing_positions_entry].temporary = temporary;
		actor->firing_positions.discarded_firing_positions[
			actor->firing_positions.next_discarded_firing_positions_entry].index = firing_position_index;
		actor->firing_positions.next_discarded_firing_positions_entry =
			(actor->firing_positions.next_discarded_firing_positions_entry + 1) %
			NUMBER_OF_DISCARDED_FIRING_POSITIONS_PER_ACTOR;

		encounter = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		firing_position = TAG_BLOCK_GET_ELEMENT(
			&encounter->firing_positions,
			firing_position_index,
			struct firing_position_definition);

		actor->firing_positions.last_discarded_firing_position_temporary = temporary;
		actor->firing_positions.last_discarded_firing_position_valid = TRUE;
		actor->firing_positions.last_discarded_firing_position = firing_position->position;
	}

	return;
}

boolean actor_firing_position_discarded(
	long actor_index,
	short firing_position_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (firing_position_index != NONE)
	{
		short index;

		for (index = 0; index < NUMBER_OF_DISCARDED_FIRING_POSITIONS_PER_ACTOR; index++)
		{
			if (firing_position_index == actor->firing_positions.discarded_firing_positions[index].index)
			{
				result = TRUE;
				break;
			}
		}
	}

	return result;
}

/* ---------- private code (evaluators defined after the public interface, as in January) */

/* January's two always-owned pre-evaluator table entries. HCEA corroborates
 * the algorithm; January supplies diagnostics, typed object mask and scores. */
static void pre_evaluator_global(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct firing_position *firing_position = firing_positions;
	short index;

	for (index = 0; index < firing_position_count; index++, firing_position++)
	{
		if (!firing_position->valid)
		{
			continue;
		}

		if (actor_firing_position_discarded(actor_index, firing_position->original_index) &&
			firing_position_reject(evaluation_context, firing_position))
		{
			firing_position->valid = FALSE;
			continue;
		}

		if (evaluation_context->find_path_direction_from_actor)
		{
			real_vector3d danger_vector;
			real_vector3d path_vector;

			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
				186,
				(actor->danger_zone.danger_type > _actor_danger_zone_none) && actor->danger_zone.noticed_danger);

			vector_from_points3d(
				&actor->danger_zone.position,
				&actor->danger_zone.predict_danger_position,
				&danger_vector);
			if (distance_squared3d(
					&actor->danger_zone.bounding_sphere_center,
					&firing_position->definition->position) <
				(actor->danger_zone.bounding_sphere_radius + 2.5f) * (actor->danger_zone.bounding_sphere_radius + 2.5f))
			{
				real distance_squared = point_to_line_distance_squared3d(
					&firing_position->definition->position,
					&actor->danger_zone.position,
					&danger_vector);
				real evaluation = 0.0f;

				if (distance_squared < actor->danger_zone.danger_radius * actor->danger_zone.danger_radius)
				{
					if (firing_position_reject(evaluation_context, firing_position))
					{
						firing_position->valid = FALSE;
						continue;
					}
				}
				else
				{
					real clearance_radius = actor->danger_zone.danger_radius + 2.5f;

					if (distance_squared < clearance_radius * clearance_radius)
					{
						evaluation = (square_root(distance_squared) - actor->danger_zone.danger_radius) * 8.0f;
					}
					else
					{
						evaluation = 20.0f;
					}
				}

				firing_position_store_evaluation_debug(
					evaluation_context,
					firing_position,
					evaluation,
					_firing_position_evaluation_danger_clearance);
			}

			if (distance_squared3d(
					&actor->danger_zone.bounding_sphere_center,
					&actor->input.position.body_position) <
				(actor->danger_zone.bounding_sphere_radius + 3.0f) * (actor->danger_zone.bounding_sphere_radius + 3.0f) &&
				actor->danger_zone.current_distance_from_actor > actor->danger_zone.danger_radius)
			{
				real distance_squared = point_to_line_distance_squared3d(
					&actor->input.position.body_position,
					&actor->danger_zone.position,
					&danger_vector);

				if (distance_squared > actor->danger_zone.danger_radius * actor->danger_zone.danger_radius)
				{
					scale_vector3d(&firing_position->path_direction_from_actor, 3.0f, &path_vector);
					if (magnitude_squared3d(&path_vector) > _real_epsilon &&
						vector_to_line_distance_squared3d(
							&actor->input.position.body_position,
							&path_vector,
							&actor->danger_zone.position,
							&danger_vector) < actor->danger_zone.danger_radius * actor->danger_zone.danger_radius &&
						firing_position_reject(evaluation_context, firing_position))
					{
						firing_position->valid = FALSE;
						continue;
					}
				}
			}
		}

		if (TEST_FLAG(evaluation_context->preferred_groups, firing_position->definition->group_index))
		{
			firing_position_store_evaluation_debug(
				evaluation_context,
				firing_position,
				evaluation_context->preferred_weight,
				_firing_position_evaluation_preferred_group);
		}

		if (evaluation_context->avoid_point_count > 0)
		{
			real closest_ratio = 1.0f;
			real evaluation;
			short avoid_index;

			for (avoid_index = 0; avoid_index < evaluation_context->avoid_point_count; avoid_index++)
			{
				real ratio = distance_squared3d(
					&firing_position->definition->position,
					&evaluation_context->avoid_points[avoid_index].point) /
					(evaluation_context->avoid_points[avoid_index].radius * evaluation_context->avoid_points[avoid_index].radius);

				if (ratio < closest_ratio)
				{
					closest_ratio = ratio;
				}
			}
			evaluation = 10.0f;
			if (closest_ratio < 1.0f)
			{
				evaluation = square_root(closest_ratio) * 10.0f;
			}
			firing_position_store_evaluation_debug(
				evaluation_context,
				firing_position,
				evaluation,
				_firing_position_evaluation_avoid_points);
		}
	}

	if (evaluation_context->directional_driving && actor->input.vehicle_index != NONE)
	{
		struct unit_datum *vehicle = vehicle_get(actor->input.vehicle_index);
		real_point3d vehicle_origin;

		object_get_origin(actor->input.vehicle_index, &vehicle_origin);
		for (index = 0, firing_position = firing_positions; index < firing_position_count; index++, firing_position++)
		{
			if (firing_position->valid)
			{
				real_vector3d direction;
				real distance_squared;

				vector_from_points3d(&vehicle_origin, &firing_position->definition->position, &direction);
				distance_squared = magnitude_squared3d(&direction);
				if (!(fabs(distance_squared) < _real_epsilon) && distance_squared < 30.0f * 30.0f)
				{
					real alignment = dot_product3d(&direction, &vehicle->object.forward) / square_root(distance_squared);
					real evaluation;

					if ((evaluation_context->directional_driving_cannot_stop ||
						magnitude_squared3d(&vehicle->object.translational_velocity) > 1.0f / 144.0f) &&
						distance_squared < 8.0f * 8.0f &&
						alignment < 0.70710677f &&
						firing_position_reject(evaluation_context, firing_position))
					{
						firing_position->valid = FALSE;
						continue;
					}

					evaluation = 15.0f;
					if (alignment < 0.0f)
					{
						evaluation = 15.0f - alignment * -15.0f;
					}
					else if (alignment > 0.86602539f)
					{
						evaluation = (alignment - 0.86602539f) * 111.96151f + 15.0f;
					}
					firing_position_store_evaluation_debug(
						evaluation_context,
						firing_position,
						evaluation,
						_firing_position_evaluation_vehicle_heading);
				}
			}
		}
	}

	return;
}


static void pre_evaluator_pursuit(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct firing_position *firing_position= firing_positions;
	short index;

	for (index= 0; index<firing_position_count; index++, firing_position++)
	{

		if (firing_position->valid)
		{
			real half_range= evaluation_context->maximum_allowable_range*0.5f;
			real evaluation= 0.0f;

			if (firing_position->path_distance_from_actor<half_range)
			{
				evaluation= 5.0f;
			}
			else if (firing_position->path_distance_from_actor<evaluation_context->maximum_allowable_range)
			{
				evaluation= (1.0f/half_range)*
					(evaluation_context->maximum_allowable_range-firing_position->path_distance_from_actor)*5.0f;
			}
			firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
				_firing_position_evaluation_pursuit_range);

			if (evaluation_context->has_target)
			{
				if (firing_position->path_distance_to_target<20.0f)
				{
					firing_position_store_evaluation_debug(evaluation_context, firing_position,
						(20.0f-firing_position->path_distance_to_target)*0.5f,
						_firing_position_evaluation_pursuit_target_distance);
				}

				if (evaluation_context->find_path_direction_from_target && evaluation_context->target_has_hint_vector)
				{
					real alignment= dot_product3d(&firing_position->path_direction_from_target,
						&evaluation_context->target_hint_vector);
					real evaluation;

					if (alignment>0.70710677f)
					{
						evaluation= 10.0f;
					}
					else
					{
						evaluation= MAX(0.0f, alignment*1.4142135f)*10.0f;
					}
					firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
						_firing_position_evaluation_pursuit_hint_alignment);
				}
			}
		}
	}

	return;
}

static void pre_evaluator_panic(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct firing_position *firing_position= firing_positions;
	short index;

	for (index= 0; index<firing_position_count; index++, firing_position++)
	{
		if (firing_position->valid)
		{
			if (firing_position->path_distance_from_actor<4.0f)
			{
				if (firing_position_reject(evaluation_context, firing_position))
				{
					firing_position->valid= FALSE;
					continue;
				}
			}
			else
			{
				real evaluation= 0.0f;

				if (firing_position->path_distance_from_actor<8.0f)
				{
					evaluation= (firing_position->path_distance_from_actor-4.0f)*2.0f;
				}
				else if (firing_position->path_distance_from_actor<evaluation_context->maximum_allowable_range)
				{
					evaluation= (evaluation_context->maximum_allowable_range-firing_position->path_distance_from_actor)*8.0f/
						(evaluation_context->maximum_allowable_range-8.0f);
				}
				firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
					_firing_position_evaluation_panic_range);
			}

			if (evaluation_context->has_target)
			{
				real evaluation;

				if (firing_position->linear_distance_squared_to_target<16.0f)
				{
					if (firing_position_reject(evaluation_context, firing_position))
					{
						firing_position->valid= FALSE;
						continue;
					}
				}

				if (firing_position->linear_distance_squared_to_target<16.0f)
				{
					evaluation= 0.0f;
				}
				else if (firing_position->linear_distance_squared_to_target<49.0f)
				{
					evaluation= (square_root(firing_position->linear_distance_squared_to_target)-4.0f)*(10.0f/3.0f);
				}
				else
				{
					evaluation= 10.0f;
				}
				firing_position_store_evaluation_debug(evaluation_context, firing_position, evaluation,
					_firing_position_evaluation_panic_target_distance);

				if (firing_position->path_closest_approach_to_target<REAL_MAX)
				{
					if (evaluation_context->target_current_distance>0.0f && evaluation_context->target_current_distance<REAL_MAX)
					{
						real proximity= 1.0f-firing_position->path_closest_approach_to_target/
							(evaluation_context->target_current_distance*0.8f);

						if (proximity>0.5f)
						{
							if (firing_position_reject(evaluation_context, firing_position))
							{
								firing_position->valid= FALSE;
								continue;
							}
						}

						firing_position_store_evaluation_debug(evaluation_context, firing_position,
							(1.0f-PIN(proximity, 0.0f, 1.0f))*8.0f, _firing_position_evaluation_panic_target_approach);
					}
				}
			}
		}
	}

	return;
}

static void firing_position_compute_line_of_sight(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	struct actor_datum *actor= actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
		828,
		evaluation_context->has_target);

	if (evaluation_context->evaluation_mode==_firing_point_evaluation_mode_pursue)
	{
		if (firing_position->path_distance_from_actor<6.0f)
		{
			real_point3d estimated_position;

			unit_estimate_position(
				actor->meta.unit_index,
				_unit_estimate_head_standing,
				&firing_position->definition->position,
				NULL,
				NULL,
				&estimated_position);
			firing_position->line_of_sight= ai_test_line_of_sight(
				&actor->input.position.head_position,
				actor->input.position.body_location.cluster_index,
				&estimated_position,
				firing_position->definition->cluster_index,
				_ai_line_of_sight_normal,
				FALSE,
				NONE,
				actor->input.vehicle_index!=NONE);
		}
		else
		{
			firing_position->line_of_sight= _ai_line_of_sight_obstructed;
		}
	}
	else
	{
		long line_of_sight_mode= _ai_line_of_sight_normal;
		long estimate_mode;
		real_vector3d *desired_facing;
		real_vector3d *desired_gun_offset;
		real_vector3d facing;
		real_point3d estimated_position;

		if (evaluation_context->evaluation_mode==_firing_point_evaluation_mode_panic ||
			evaluation_context->evaluation_mode==_firing_point_evaluation_mode_cover)
		{
			estimate_mode= _unit_estimate_head_crouching;
			desired_facing= NULL;
			desired_gun_offset= NULL;
		}
		else if (evaluation_context->has_gun_offset_stand)
		{
			estimate_mode= _unit_estimate_gun_position;
			desired_gun_offset= &evaluation_context->gun_offset_stand;
			vector_from_points3d(
				&firing_position->definition->position,
				&evaluation_context->target_head_position,
				&facing);
			if (normalize2d((real_vector2d *)&facing)>0.0f)
			{
				facing.k= 0.0f;
				desired_facing= &facing;
			}
			else
			{
				desired_facing= &actor->input.facing_vector;
			}
		}
		else
		{
			estimate_mode= _unit_estimate_head_standing;
			desired_facing= NULL;
			desired_gun_offset= NULL;
		}

		unit_estimate_position(
			actor->meta.unit_index,
			estimate_mode,
			&firing_position->definition->position,
			desired_facing,
			desired_gun_offset,
			&estimated_position);
		{
			long evaluation_mode= evaluation_context->evaluation_mode;

			if (evaluation_mode>_firing_point_evaluation_mode_fight &&
				evaluation_mode<=_firing_point_evaluation_mode_uncover)
			{
				line_of_sight_mode= _ai_line_of_sight_expand_source;
			}
		}
		firing_position->line_of_sight= ai_test_line_of_sight(
			&estimated_position,
			firing_position->definition->cluster_index,
			&evaluation_context->target_line_of_sight_position,
			evaluation_context->target_cluster_index,
			line_of_sight_mode,
			TRUE,
			evaluation_context->target_vehicle_index,
			actor->input.vehicle_index!=NONE);
	}

	return;
}

static boolean firing_position_forced_evaluation(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *firing_position)
{
	firing_position->valid= TRUE;
	firing_position->evaluation= 0.0f;
	firing_position->pre_evaluation= 0.0f;
	firing_position->rejected= FALSE;
	firing_position_pre_evaluate(actor_index, evaluation_context, 1, firing_position);

	if (firing_position->valid)
	{
		evaluation_context->debug_valid_count++;
	}
	if (!firing_position->rejected)
	{
		evaluation_context->debug_nonrejected_count++;
	}
	if (firing_position->valid)
	{
		if (evaluation_context->has_target)
		{
			firing_position_compute_line_of_sight(
				actor_index,
				evaluation_context,
				firing_position);
		}

		firing_position->pre_evaluation= firing_position->evaluation;
		firing_position->valid= firing_position_post_evaluate(
			actor_index,
			evaluation_context,
			firing_position);
		evaluation_context->debug_post_evaluated_count++;
	}

	return firing_position->valid;
}

boolean actor_nearby_firing_positions(
	long actor_index,
	real_point3d const *test_point,
	long test_surface_index,
	short group_selection_mode)
{
	struct actor_datum *actor= actor_get(actor_index);
	struct actor_definition *definition= actor_definition_get(actor->meta.definition_index);

	if (!actor->state.flying && test_surface_index!=NONE)
	{
		struct collision_bsp *collision_bsp= global_collision_bsp_get();

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
			1434,
			(test_surface_index >= 0) && (test_surface_index < collision_bsp->surfaces.count));
	}

	if ((actor->state.flying || test_surface_index!=NONE) && actor->meta.encounter_index!=NONE)
	{
		struct encounter_definition *encounter= TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		long firing_position_groups= actor_get_firing_position_group(
			actor_index,
			_firing_point_evaluation_mode_fight,
			group_selection_mode);
		struct path_state state;
		short firing_position_index;

		if (!actor->state.flying)
		{
			struct path_input input;

			path_input_new(
				&input,
				definition->moving.pathfinding_radius,
				TRUE,
				NONE);
			path_input_set_start(&input, test_point, test_surface_index);
			path_input_set_search_bounds(&input, 4.0f);
			path_state_new(&input, &state, NULL);
			path_state_find(&state);
		}

		for (firing_position_index= 0;
			firing_position_index<encounter->firing_positions.count;
			firing_position_index++)
		{
			struct firing_position_definition *firing_position= TAG_BLOCK_GET_ELEMENT(
				&encounter->firing_positions,
				firing_position_index,
				struct firing_position_definition);

			if (TEST_FLAG(firing_position_groups, firing_position->group_index) &&
				distance_squared3d(test_point, &firing_position->position)<16.0f)
			{
				if (actor->state.flying)
				{
					if (path_3d_available(
						global_structure_bsp_get(),
						test_point,
						0.0f,
						&firing_position->position,
						NULL,
						NULL))
					{
						return TRUE;
					}
				}
				else
				{
					real distance;

					path_state_estimated_distance(
						&state,
						&firing_position->position,
						firing_position->surface_index,
						&distance,
						NULL,
						NULL);
					if (distance<4.0f)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

short actor_select_firing_position(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *best_firing_position,
	long *current_owner,
	struct path_state *area_path_state,
	boolean *area_path_state_valid)
{
	struct actor_datum *actor= actor_get(actor_index);
	short best_index= NONE;
	real best_evaluation= 0.0f;
	boolean debug_evaluation= FALSE;

	if (actor->meta.encounter_index==ai_debug.selected_squad_index &&
		(ai_debug.selected_actor_index==NONE || ai_debug.selected_actor_index==actor_index))
	{
		if (ai_debug.render_evaluations &&
			evaluation_context->evaluation_mode!=_firing_point_evaluation_mode_pursue)
		{
			debug_evaluation= TRUE;
		}
		if (ai_debug.render_pursuit &&
			evaluation_context->evaluation_mode==_firing_point_evaluation_mode_pursue)
		{
			debug_evaluation= TRUE;
		}
	}

	if (actor->meta.encounter_index!=NONE)
	{
		struct actor_definition *definition= actor_definition_get(actor->meta.definition_index);
		struct actor_variant_definition *variant_definition= actor_variant_definition_get(actor->meta.variant_definition_index);
		struct encounter_definition *encounter= TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		struct firing_position firing_positions[MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER];
		long sorted_indices[MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER];
		long owner_actor_indices[MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER];
		short firing_position_count= 0;
		boolean any_in_range= FALSE;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
			1552,
			!actor->input.vehicle_passenger);
		ai_profile.meters[_ai_meter_firing_point].accumulator++;

		encounter_build_firing_position_owner_actor_indices(actor->meta.encounter_index, owner_actor_indices);
		if (actor->firing_positions.current_position_index!=NONE)
		{
			owner_actor_indices[actor->firing_positions.current_position_index]= NONE;
		}

		evaluation_context->maximum_allowable_range=
			actor->input.vehicle_driver_type==_actor_vehicle_driver_none ? 15.0f : 80.0f;
		if (evaluation_context->maximum_search_range==0.0f)
		{
			evaluation_context->maximum_search_range= evaluation_context->maximum_allowable_range;
		}
		evaluation_context->find_path_distance_to_target=
			evaluation_context->evaluation_mode==_firing_point_evaluation_mode_pursue;
		evaluation_context->has_target= FALSE;

		if (evaluation_context->specific_target_enable)
		{
			evaluation_context->target_point= evaluation_context->specific_target_point;
			evaluation_context->has_target= TRUE;
			evaluation_context->target_pathfinding_point= evaluation_context->specific_target_point;
			evaluation_context->target_pathfinding_surface_index= evaluation_context->specific_target_surface_index;
			evaluation_context->target_cluster_index= evaluation_context->specific_target_cluster_index;
			evaluation_context->target_current_distance= distance3d(
				&actor->input.position.body_position,
				&evaluation_context->target_point);
			evaluation_context->target_prop_index= NONE;
			evaluation_context->target_vehicle_index= NONE;
			evaluation_context->target_danger_radius= 0.0f;
			unit_estimate_position(
				actor->meta.unit_index,
				_unit_estimate_head_standing,
				&evaluation_context->target_point,
				NULL,
				NULL,
				&evaluation_context->target_head_position);
			evaluation_context->target_line_of_sight_position= evaluation_context->target_head_position;
		}
		else
		{
			long target_prop_index= NONE;

			if (actor->state.action==_actor_action_flee &&
				actor->state.action_data.flee.flee_prop_index!=NONE)
			{
				target_prop_index= actor->state.action_data.flee.flee_prop_index;
			}
			else if (actor->target.target_prop_index!=NONE)
			{
				target_prop_index= actor->target.target_prop_index;
			}

			if (target_prop_index!=NONE)
			{
				struct prop_datum *prop= prop_get(target_prop_index);

				if (evaluation_context->find_path_distance_to_target &&
					prop->state>=_prop_state_becoming_unacknowledged &&
					prop->state<=_prop_state_acknowledged)
				{
					actor_perception_find_prop_pathfinding_location(actor_index, target_prop_index);
				}

				evaluation_context->has_target= TRUE;
				evaluation_context->target_point= prop->body_position;
				evaluation_context->target_pathfinding_point= prop->pathfinding_point;
				evaluation_context->target_pathfinding_surface_index= prop->pathfinding_surface_index;
				evaluation_context->target_cluster_index= prop->body_location.cluster_index;
				evaluation_context->target_current_distance= prop->distance;
				evaluation_context->target_prop_index= target_prop_index;
				evaluation_context->target_head_position= prop->head_position;
				evaluation_context->target_vehicle_index= prop->vehicle_index;
				evaluation_context->target_danger_radius= prop->suicide_radius;
				if (evaluation_context->use_last_visible_target_position &&
					prop->last_visible_time!=NONE)
				{
					evaluation_context->target_line_of_sight_position= prop->last_visible_head_position;
				}
				else
				{
					evaluation_context->target_line_of_sight_position= prop->head_position;
				}
				if (prop->state>=_prop_state_uninspected_orphan &&
					prop->state<=_prop_state_inspected_orphan)
				{
					evaluation_context->target_has_hint_vector= TRUE;
					evaluation_context->target_hint_vector= prop->orphan_hint_vector;
				}
				evaluation_context->target_line_of_sight_optional=
					evaluation_context->evaluation_mode==_firing_point_evaluation_mode_guard ||
					evaluation_context->evaluation_mode==_firing_point_evaluation_mode_avoid;
			}
		}

		if (magnitude_squared3d(&variant_definition->ranged_combat.gun_offset_stand)>0.0001f)
		{
			evaluation_context->has_gun_offset_stand= TRUE;
			evaluation_context->gun_offset_stand= variant_definition->ranged_combat.gun_offset_stand;
		}
		else if (magnitude_squared3d(&definition->perception.gun_offset_stand)>0.0001f)
		{
			evaluation_context->has_gun_offset_stand= TRUE;
			evaluation_context->gun_offset_stand= definition->perception.gun_offset_stand;
		}
		else
		{
			evaluation_context->has_gun_offset_stand= FALSE;
		}
		if (magnitude_squared3d(&variant_definition->ranged_combat.gun_offset_crouch)>0.0001f)
		{
			evaluation_context->has_gun_offset_crouch= TRUE;
			evaluation_context->gun_offset_crouch= variant_definition->ranged_combat.gun_offset_crouch;
		}
		else if (magnitude_squared3d(&definition->perception.gun_offset_crouch)>0.0001f)
		{
			evaluation_context->has_gun_offset_crouch= TRUE;
			evaluation_context->gun_offset_crouch= definition->perception.gun_offset_crouch;
		}
		else
		{
			evaluation_context->has_gun_offset_crouch= FALSE;
		}

		if (actor->danger_zone.danger_type>_actor_danger_zone_none &&
			actor->danger_zone.noticed_danger &&
			actor->danger_zone.current_distance_from_actor<actor->danger_zone.bounding_sphere_radius+3.0f)
		{
			evaluation_context->find_path_direction_from_actor= TRUE;
		}
		evaluation_context->flying= actor->state.flying;
		if (actor->input.vehicle_driver_type==_actor_vehicle_driver_directional_flying)
		{
			evaluation_context->directional_driving= TRUE;
			evaluation_context->directional_driving_cannot_stop= TRUE;
		}

		evaluation_context->avoid_point_count= 0;
		if (actor->firing_positions.last_discarded_firing_position_valid)
		{
			evaluation_context->avoid_points[evaluation_context->avoid_point_count].point=
				actor->firing_positions.last_discarded_firing_position;
			evaluation_context->avoid_points[evaluation_context->avoid_point_count].radius=
				definition->firing_position.old_avoidance_radius;
			evaluation_context->avoid_point_count++;
		}
		if (definition->firing_position.friend_avoidance_radius>0.0f)
		{
			switch (evaluation_context->evaluation_mode)
			{
			case _firing_point_evaluation_mode_fight:
			case _firing_point_evaluation_mode_uncover:
			case _firing_point_evaluation_mode_avoid:
				{
					struct prop_iterator iterator;
					struct prop_datum *prop;

					prop_iterator_new(&iterator, actor_index);
					while (evaluation_context->avoid_point_count<MAXIMUM_NUMBER_OF_FIRING_POSITION_AVOID_POINTS &&
						(prop= prop_iterator_next(&iterator))!=NULL)
					{
						if (prop->state>=_prop_state_becoming_unacknowledged &&
							prop->state<=_prop_state_acknowledged &&
							!prop->enemy && !prop->dead && !prop->player)
						{
							evaluation_context->avoid_points[evaluation_context->avoid_point_count].point= prop->body_position;
							evaluation_context->avoid_points[evaluation_context->avoid_point_count].radius=
								definition->firing_position.friend_avoidance_radius;
							evaluation_context->avoid_point_count++;
						}
					}
				}
				break;
			}
		}

		evaluation_context->attack_vector_count= 0;
		evaluation_context->friend_attack_vector_count= 0;
		evaluation_context->dangerous_enemy_attack_vector_count= 0;
		{
			boolean build_attack_vectors= FALSE;

			if (TEST_FLAG(definition->flags, _actor_definition_avoid_friend_line_of_fire_bit) &&
				actor->state.combat_status>=_actor_combat_status_definite &&
				actor->situation.area_friends>0)
			{
				build_attack_vectors= TRUE;
			}
			if (TEST_FLAG(definition->flags2, _actor_definition_flags2_avoid_all_enemy_attack_vectors_bit) &&
				actor->state.combat_status>=_actor_combat_status_definite)
			{
				build_attack_vectors= TRUE;
			}

			switch (evaluation_context->evaluation_mode)
			{
			case _firing_point_evaluation_mode_fight:
			case _firing_point_evaluation_mode_cover:
			case _firing_point_evaluation_mode_uncover:
			case _firing_point_evaluation_mode_avoid:
				if (build_attack_vectors)
				{
					struct prop_iterator iterator;
					struct prop_datum *prop;

					prop_iterator_new(&iterator, actor_index);
					while (evaluation_context->attack_vector_count<MAXIMUM_NUMBER_OF_FIRING_POSITION_ATTACK_VECTORS &&
						(prop= prop_iterator_next(&iterator))!=NULL)
					{
						if (prop->state>=_prop_state_becoming_unacknowledged &&
							prop->state<=_prop_state_acknowledged &&
							!prop->dead)
						{
							if (!prop->enemy && (prop->player || prop->vehicle_index==NONE))
							{
								real_vector3d attack_vector;

								if (actor_perception_friend_prop_is_attacking(actor_index, iterator.index, &attack_vector))
								{
									evaluation_context->attack_vectors[evaluation_context->attack_vector_count].type= prop->player ?
										_firing_position_attack_vector_friend_player :
										_firing_position_attack_vector_friend;
									evaluation_context->attack_vectors[evaluation_context->attack_vector_count].point= prop->body_position;
									evaluation_context->attack_vectors[evaluation_context->attack_vector_count].vector= attack_vector;
									evaluation_context->attack_vector_count++;
									evaluation_context->friend_attack_vector_count++;
								}
							}
							if (prop->enemy)
							{
								boolean dangerous= TEST_FLAG(definition->flags2, _actor_definition_flags2_avoid_all_enemy_attack_vectors_bit);

								if (prop->unopposable_enemy && (prop->player || prop->shooting))
								{
									dangerous= TRUE;
								}
								if (dangerous)
								{
									long unit_index= prop->vehicle_index!=NONE ? prop->vehicle_index : prop->unit_index;

									evaluation_context->attack_vectors[evaluation_context->attack_vector_count].type=
										_firing_position_attack_vector_dangerous_enemy;
									evaluation_context->attack_vectors[evaluation_context->attack_vector_count].point= prop->body_position;
									unit_get_aiming_vector(
										unit_index,
										&evaluation_context->attack_vectors[evaluation_context->attack_vector_count].vector);
									evaluation_context->attack_vector_count++;
									evaluation_context->dangerous_enemy_attack_vector_count++;
								}
							}
						}
					}
				}
				break;
			}
		}

		evaluation_context->debug_encounter_count=
			evaluation_context->debug_considered_count=
			evaluation_context->debug_valid_count=
			evaluation_context->debug_nonrejected_count=
			evaluation_context->debug_post_evaluated_count=
			evaluation_context->debug_skipped_count= 0;
		if (debug_evaluation)
		{
			ai_debug.evaluation_context_valid= TRUE;
			ai_debug.evaluation_context= *evaluation_context;
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
			1830,
			area_path_state_valid);
		*area_path_state_valid= FALSE;
		{
			boolean too_many_firing_positions= FALSE;
			short index;

			for (index= 0; index<encounter->firing_positions.count; index++)
			{
				struct firing_position_definition *firing_position_definition= TAG_BLOCK_GET_ELEMENT(
					&encounter->firing_positions,
					index,
					struct firing_position_definition);

				if (debug_evaluation)
				{
					ai_debug.actor_record[index].valid= FALSE;
				}
				if (TEST_FLAG(evaluation_context->allowed_position_mask, firing_position_definition->group_index))
				{
					if (actor->meta.encounter_index==ai_debug.selected_squad_index)
					{
						ai_debug.actor_record[index].pursuit=
							evaluation_context->evaluation_mode==_firing_point_evaluation_mode_pursue;
					}
					if ((evaluation_context->flying || firing_position_definition->surface_index!=NONE) &&
						(evaluation_context->evaluation_mode!=_firing_point_evaluation_mode_pursue ||
							actor_nearby_firing_positions(
								actor_index,
								&firing_position_definition->position,
								firing_position_definition->surface_index,
								_firing_position_group_when_searching)))
					{
						boolean acceptable= TRUE;

						if (owner_actor_indices[index]!=NONE)
						{
							struct actor_datum *owner_actor;
							real owner_distance;
							real actor_distance;

							if (evaluation_context->evaluation_mode==_firing_point_evaluation_mode_guard &&
								evaluation_context->avoid_point_count<MAXIMUM_NUMBER_OF_FIRING_POSITION_AVOID_POINTS)
							{
								evaluation_context->avoid_points[evaluation_context->avoid_point_count].point=
									firing_position_definition->position;
								evaluation_context->avoid_points[evaluation_context->avoid_point_count].radius= 4.0f;
								evaluation_context->avoid_point_count++;
							}
							owner_actor= actor_get(owner_actor_indices[index]);
							owner_distance= distance3d(
								&owner_actor->input.position.body_position,
								&firing_position_definition->position);
							actor_distance= distance3d(
								&actor->input.position.body_position,
								&firing_position_definition->position);
							if (owner_distance<1.0f || owner_distance<2.0f*actor_distance)
							{
								acceptable= FALSE;
							}
						}

						if (acceptable)
						{
							if (firing_position_count<MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER)
							{
								firing_positions[firing_position_count].definition= firing_position_definition;
								firing_positions[firing_position_count].original_index= index;
								firing_positions[firing_position_count].line_of_sight= _ai_line_of_sight_clear;
								firing_positions[firing_position_count].path_distance_from_actor= REAL_MAX;
								firing_positions[firing_position_count].path_direction_from_actor= *global_zero_vector3d;
								firing_positions[firing_position_count].path_distance_to_target= REAL_MAX;
								firing_positions[firing_position_count].path_closest_approach_to_target= REAL_MAX;
								firing_positions[firing_position_count].path_direction_from_target= *global_zero_vector3d;
								firing_positions[firing_position_count].linear_distance_squared_to_target= 0.0f;
								firing_positions[firing_position_count].pre_evaluation= 0.0f;
								firing_positions[firing_position_count].evaluation= 0.0f;
								firing_positions[firing_position_count].valid= TRUE;
								firing_positions[firing_position_count].rejected= FALSE;
								firing_position_count++;
							}
							else if (!too_many_firing_positions)
							{
								error(
									_error_silent,
									"encounter %s has too many firing positions (%d > %d)",
									encounter->name,
									encounter->firing_positions.count,
									MAXIMUM_NUMBER_OF_FIRING_POSITIONS_PER_ENCOUNTER);
								too_many_firing_positions= TRUE;
							}
						}
					}
				}
			}
		}
		evaluation_context->debug_encounter_count= (short)encounter->firing_positions.count;
		evaluation_context->debug_considered_count= firing_position_count;

		if (firing_position_count==0)
		{
			actor_clear_discarded_firing_positions(actor_index, FALSE);
		}
		else
		{
			if (evaluation_context->has_target && evaluation_context->find_path_distance_to_target)
			{
				if (evaluation_context->flying)
				{
					struct structure_bsp *structure_bsp= global_structure_bsp_get();
					short index;

					for (index= 0; index<firing_position_count; index++)
					{
						struct firing_position *firing_position= &firing_positions[index];
						real_vector3d target_vector;

						vector_from_points3d(
							&evaluation_context->target_point,
							&firing_position->definition->position,
							&target_vector);
						if (magnitude_squared3d(&target_vector)<400.0f &&
							path_3d_available(
								structure_bsp,
								&evaluation_context->target_point,
								0.0f,
								&firing_position->definition->position,
								NULL,
								NULL))
						{
							firing_position->path_distance_to_target= normalize3d(&target_vector);
							if (evaluation_context->find_path_direction_from_target)
							{
								firing_position->path_direction_from_target= target_vector;
							}
						}
					}
				}
				else if (evaluation_context->target_pathfinding_surface_index!=NONE)
				{
					struct path_input input;
					struct path_state state;
					short index;

					path_input_new(
						&input,
						definition->moving.pathfinding_radius,
						actor->emotions.ignorant_of_broken_surfaces,
						NONE);
					path_input_set_start(
						&input,
						&evaluation_context->target_pathfinding_point,
						evaluation_context->target_pathfinding_surface_index);
					path_input_set_search_bounds(&input, 20.0f);
					path_state_new(&input, &state, NULL);
					path_state_find(&state);
					for (index= 0; index<firing_position_count; index++)
					{
						struct firing_position *firing_position= &firing_positions[index];

						path_state_estimated_distance(
							&state,
							&firing_position->definition->position,
							firing_position->definition->surface_index,
							&firing_position->path_distance_to_target,
							NULL,
							evaluation_context->find_path_direction_from_target ? &firing_position->path_direction_from_target : NULL);
					}
				}
			}

			if (!evaluation_context->flying)
			{
				struct path_input input;

				actor_path_input_new(actor_index, &input);
				path_input_set_search_bounds(&input, evaluation_context->maximum_search_range);
				if (evaluation_context->attractor_enable && evaluation_context->has_target)
				{
					long object_index= NONE;

					if (evaluation_context->target_prop_index!=NONE)
					{
						struct prop_datum *target_prop= prop_get(evaluation_context->target_prop_index);

						object_index= target_prop->unit_index;
					}
					path_input_set_attractor(
						&input,
						&evaluation_context->target_point,
						evaluation_context->attractor_radius,
						object_index,
						evaluation_context->attractor_weight);
				}
				else if (actor->danger_zone.danger_type>_actor_danger_zone_none &&
					!TEST_FLAG(definition->flags2, _actor_definition_flags2_pathfinding_ignores_danger_bit))
				{
					path_input_set_attractor(
						&input,
						&actor->danger_zone.position,
						actor->danger_zone.danger_radius,
						actor->danger_zone.object_index,
						10.0f);
				}
				path_state_new(&input, area_path_state, ai_debug_get_path_storage(actor_index));
				if (path_state_find(area_path_state))
				{
					*area_path_state_valid= TRUE;
				}
			}

			{
				short index;

				for (index= 0; index<firing_position_count; index++)
				{
					struct firing_position *firing_position= &firing_positions[index];
					real_vector3d from_actor;

					if (evaluation_context->has_target)
					{
						firing_position->linear_distance_squared_to_target= distance_squared3d(
							&evaluation_context->target_point,
							&firing_position->definition->position);
					}
					vector_from_points3d(
						&actor->input.position.body_position,
						&firing_position->definition->position,
						&from_actor);
					if (magnitude_squared3d(&from_actor)<
						evaluation_context->maximum_search_range*evaluation_context->maximum_search_range)
					{
						if (evaluation_context->flying)
						{
							firing_position->path_closest_approach_to_target= square_root(
								point_to_line_distance_squared3d(
									&evaluation_context->target_point,
									&actor->input.position.body_position,
									&from_actor));
							firing_position->path_distance_from_actor= normalize3d(&from_actor);
							if (evaluation_context->find_path_direction_from_actor)
							{
								firing_position->path_direction_from_actor= from_actor;
							}
						}
						else
						{
							path_state_estimated_distance(
								area_path_state,
								&firing_position->definition->position,
								firing_position->definition->surface_index,
								&firing_position->path_distance_from_actor,
								&firing_position->path_closest_approach_to_target,
								evaluation_context->find_path_direction_from_actor ? &firing_position->path_direction_from_actor : NULL);
						}
					}
					if (firing_position->path_distance_from_actor<evaluation_context->maximum_search_range)
					{
						any_in_range= TRUE;
					}
					else
					{
						firing_position->valid= FALSE;
					}
				}
			}

			if (evaluation_context->allow_outside_range && !any_in_range)
			{
				struct firing_position *firing_position;

				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
					2108,
					firing_position_count > 0);
				best_index= seed_random_range(get_global_random_seed_address(), 0, firing_position_count);
				*area_path_state_valid= FALSE;
				actor_clear_discarded_firing_positions(actor_index, FALSE);
				firing_position= &firing_positions[best_index];
				if (!firing_position_forced_evaluation(actor_index, evaluation_context, firing_position))
				{
					best_index= NONE;
				}
				if (debug_evaluation)
				{
					ai_debug.actor_record[firing_position->original_index].valid= TRUE;
					ai_debug.actor_record[firing_position->original_index].firing_position= *firing_position;
				}
			}
			else
			{
				boolean expected_to_discard= FALSE;
				short index;

				firing_position_pre_evaluate(actor_index, evaluation_context, firing_position_count, firing_positions);
				for (index= 0; index<firing_position_count; index++)
				{
					sorted_indices[index]= index;
				}
				global_temporary_sort_firing_position_count= firing_position_count;
				global_temporary_sort_firing_position_array= firing_positions;
				qsort_4byte(sorted_indices, firing_position_count, firing_position_compare);
				evaluation_context->post_evaluation_bounded= firing_positions_get_post_evaluation_bound(actor_index, evaluation_context);

				for (index= 0; index<firing_position_count; index++)
				{
					short sorted_index= (short)sorted_indices[index];
					struct firing_position *firing_position= &firing_positions[sorted_index];

					if (firing_position->valid)
					{
						evaluation_context->debug_valid_count++;
					}
					if (!firing_position->rejected)
					{
						evaluation_context->debug_nonrejected_count++;
					}
					if (!firing_position->valid ||
						(evaluation_context->post_evaluation_bounded &&
							firing_position->evaluation+evaluation_context->post_evaluation_bound<=best_evaluation))
					{
						if (game_connection()!=_game_connection_local || !ai_debug.evaluate_all_positions)
						{
							short skipped_index;

							for (skipped_index= index; skipped_index<firing_position_count; skipped_index++)
							{
								if (firing_positions[sorted_indices[skipped_index]].valid)
								{
									evaluation_context->debug_valid_count++;
									evaluation_context->debug_skipped_count++;
								}
							}
							break;
						}
						expected_to_discard= TRUE;
					}
					if (firing_position->valid)
					{
						if (evaluation_context->has_target)
						{
							firing_position_compute_line_of_sight(actor_index, evaluation_context, firing_position);
						}
						evaluation_context->debug_post_evaluated_count++;
						firing_position->pre_evaluation= firing_position->evaluation;
						if (firing_position_post_evaluate(actor_index, evaluation_context, firing_position) &&
							firing_position->evaluation>best_evaluation)
						{
							match_assert(
								"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
								2243,
								!expected_to_discard);
							best_index= sorted_index;
							best_evaluation= firing_position->evaluation;
						}
					}
				}

				if (debug_evaluation)
				{
					for (index= 0; index<firing_position_count; index++)
					{
						ai_debug.actor_record[firing_positions[index].original_index].valid= TRUE;
						ai_debug.actor_record[firing_positions[index].original_index].firing_position= firing_positions[index];
					}
				}
				if (ai_debug.print_evaluation_statistics)
				{
					char *evaluation_mode_names[]=
					{
						"fight",
						"panic",
						"cover",
						"uncover",
						"guard",
						"pursue"
					};

					error(
						_error_silent,
						"fp-eval %s: encounter %3d consider %3d valid %3d nonrejected %3d post-eval %3d skipped %3d",
						evaluation_mode_names[evaluation_context->evaluation_mode],
						evaluation_context->debug_encounter_count,
						evaluation_context->debug_considered_count,
						evaluation_context->debug_valid_count,
						evaluation_context->debug_nonrejected_count,
						evaluation_context->debug_post_evaluated_count,
						evaluation_context->debug_skipped_count);
				}
			}

			if (best_index!=NONE)
			{
				if (best_firing_position)
				{
					*best_firing_position= firing_positions[best_index];
				}
				best_index= firing_positions[best_index].original_index;
				if (current_owner)
				{
					*current_owner= owner_actor_indices[best_index];
				}
			}
		}
	}

	return best_index;
}

short actor_active_select_firing_position(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	struct firing_position *best_firing_position,
	long *current_owner,
	struct path_state *area_path_state,
	boolean *area_path_state_valid)
{
	struct actor_datum *actor= actor_get(actor_index);
	short result= NONE;

	if (actor->meta.encounter_index!=NONE)
	{
		unsigned long currently_allowed_groups;
		unsigned long groups_when_not_searching;
		unsigned long total_groups;

		currently_allowed_groups= actor_get_firing_position_group(
			actor_index,
			evaluation_context->evaluation_mode,
			_firing_position_group_normal);
		groups_when_not_searching= actor_get_firing_position_group(
			actor_index,
			evaluation_context->evaluation_mode,
			_firing_position_group_when_not_searching);
		total_groups= actor_get_firing_position_group(
			actor_index,
			evaluation_context->evaluation_mode,
			_firing_position_group_when_searching);
		total_groups|= groups_when_not_searching;

		if (total_groups>currently_allowed_groups)
		{
			evaluation_context->preferred_groups= currently_allowed_groups;
			evaluation_context->preferred_weight= 8.0f;
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
				2311,
				(total_groups & currently_allowed_groups) == currently_allowed_groups);
			evaluation_context->allowed_position_mask= total_groups;
		}
		else
		{
			evaluation_context->allowed_position_mask= currently_allowed_groups;
		}

		if (actor->firing_positions.current_position_index!=NONE &&
			actor->firing_positions.current_position_found_outside_range)
		{
			evaluation_context->allow_outside_range= FALSE;
		}
		else
		{
			evaluation_context->allow_outside_range= TRUE;
		}
		evaluation_context->allow_rejected_positions= TRUE;

		result= actor_select_firing_position(
			actor_index,
			evaluation_context,
			best_firing_position,
			current_owner,
			area_path_state,
			area_path_state_valid);
		if (result!=NONE)
		{
			if (!TEST_FLAG(
				currently_allowed_groups,
				best_firing_position->definition->group_index))
			{
				actor= actor_get(actor_index);
				actor->state.searching= !actor->state.searching;
			}
		}
		else if (actor->firing_positions.current_position_index!=NONE &&
			actor->firing_positions.current_position_found_outside_range)
		{
			short firing_position_index= actor->firing_positions.current_position_index;
			struct encounter_definition *encounter= TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
				struct encounter_definition);
			struct firing_position_definition *definition= TAG_BLOCK_GET_ELEMENT(
				&encounter->firing_positions,
				firing_position_index,
				struct firing_position_definition);

			best_firing_position->definition= definition;
			best_firing_position->original_index= firing_position_index;
			best_firing_position->line_of_sight= _ai_line_of_sight_clear;
			best_firing_position->path_distance_from_actor= REAL_MAX;
			best_firing_position->path_distance_to_target= REAL_MAX;
			best_firing_position->path_closest_approach_to_target= REAL_MAX;
			best_firing_position->path_direction_from_target= *global_zero_vector3d;
			best_firing_position->path_direction_from_actor= *global_zero_vector3d;
			if (evaluation_context->has_target)
			{
				best_firing_position->linear_distance_squared_to_target= distance_squared3d(
					&evaluation_context->target_point,
					&definition->position);
			}
			else
			{
				best_firing_position->linear_distance_squared_to_target= 0.0f;
			}

			if (!firing_position_forced_evaluation(
				actor_index,
				evaluation_context,
				best_firing_position))
			{
				firing_position_index= NONE;
				actor->firing_positions.current_position_index= NONE;
			}
			*current_owner= NONE;
			*area_path_state_valid= FALSE;
			result= firing_position_index;
		}
	}

	return result;
}

short actor_change_firing_position(
	long actor_index,
	short firing_position_index,
	struct firing_position *firing_position,
	long previous_owner,
	struct path_state *cached_path_state,
	boolean cached_path_available)
{
	struct actor_datum *actor= actor_get(actor_index);

	if (firing_position_index==NONE)
	{
		actor_move_halt(actor_index);
		actor->firing_positions.current_position_index= NONE;
	}
	else
	{
		short current_position_index;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
			2427,
			actor->meta.encounter_index != NONE);

		current_position_index= actor->firing_positions.current_position_index;
		if (current_position_index!=NONE && current_position_index!=firing_position_index)
		{
			actor_discard_firing_position(actor_index, current_position_index, TRUE);
		}

		if (previous_owner!=NONE)
		{
			struct actor_datum *previous_actor= actor_get(previous_owner);

			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
				2440,
				actor_index != previous_owner);
			actor_move_halt(previous_owner);
			previous_actor->firing_positions.current_position_index= NONE;
		}

		if (actor->firing_positions.current_position_index!=firing_position_index)
		{
			actor->firing_positions.current_position_index= firing_position_index;
			actor->firing_positions.current_position_found_outside_range= !cached_path_available;
			actor->firing_positions.moved_away_from_firing_position= FALSE;

			if (!actor_move_to_firing_position(actor_index, firing_position_index,
				cached_path_available ? cached_path_state : NULL))
			{
				actor->firing_positions.current_position_index= NONE;
			}
		}
	}

	if (actor->meta.encounter_index!=NONE)
	{
		encounter_verify_firing_position_owner_actor_indices(actor->meta.encounter_index);
	}

	return actor->firing_positions.current_position_index;
}

static void pre_evaluator_attack(
	long actor_index,
	struct firing_position_evaluation_context *evaluation_context,
	short firing_position_count,
	struct firing_position *firing_positions)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition = actor_combat_get_firing_variant_definition(actor_index);
	struct firing_position *firing_position = firing_positions;
	short index;

	for (index = 0; index < firing_position_count; index++, firing_position++)
	{
		if (!firing_position->valid)
		{
			continue;
		}

		if (evaluation_context->has_target)
		{
			real distance_to_target = square_root(firing_position->linear_distance_squared_to_target);

			if (variant_definition->ranged_combat.maximum_firing_range > 0.0f)
			{
				real preferred_range = variant_definition->ranged_combat.maximum_firing_range * 0.8f;
				real evaluation = distance_to_target < preferred_range ? 10.0f : preferred_range / distance_to_target * 10.0f;

				firing_position_store_evaluation_debug(
					evaluation_context,
					firing_position,
					evaluation,
					_firing_position_evaluation_attack_range);
			}
			if (variant_definition->ranged_combat.combat_range_upper_bound > 0.0f &&
				distance_to_target < variant_definition->ranged_combat.combat_range_upper_bound)
			{
				real lower_bound;
				real upper_bound;
				real distance_from_boundary;
				real evaluation;
				struct weapon_definition *weapon;

				if (actor->emotions.berserk)
				{
					lower_bound = variant_definition->ranged_combat.berserk_firing_range_lower_bound;
					upper_bound = variant_definition->ranged_combat.berserk_firing_range_upper_bound;
				}
				else
				{
					lower_bound = variant_definition->ranged_combat.combat_range_lower_bound;
					upper_bound = variant_definition->ranged_combat.combat_range_upper_bound;
				}
				lower_bound = MAX(lower_bound, evaluation_context->target_danger_radius);
				weapon = actor_get_weapon_definition(actor_index);
				if (weapon && weapon->weapon.ai_minimum_target_range > 0.0f)
				{
					lower_bound = MAX(lower_bound, weapon->weapon.ai_minimum_target_range);
				}

				distance_from_boundary = upper_bound - distance_to_target;
				if (lower_bound > 0.0f)
				{
					distance_from_boundary = MIN(distance_from_boundary, distance_to_target - lower_bound);
				}
				evaluation = 0.0f;
				if (distance_from_boundary > 2.0f)
				{
					evaluation = 20.0f;
				}
				else if (distance_from_boundary > 0.0f)
				{
					evaluation = 20.0f * (distance_from_boundary / 2.0f);
				}
				firing_position_store_evaluation_debug(
					evaluation_context,
					firing_position,
					evaluation,
					_firing_position_evaluation_combat_range);
			}
		}

		if (evaluation_context->dangerous_enemy_attack_vector_count > 0)
		{
			real nearest_distance_squared = REAL_MAX;
			real evaluation;
			short vector_index;

			for (vector_index = 0; vector_index < evaluation_context->attack_vector_count; vector_index++)
			{
				struct firing_position_attack_vector *attack_vector = &evaluation_context->attack_vectors[vector_index];

				if (attack_vector->type == _firing_position_attack_vector_dangerous_enemy)
				{
					real_vector3d direction;
					real along;

					match_assert_valid_real_normal3d(
						"c:\\halo\\SOURCE\\ai\\actor_firing_position.c",
						657,
						&evaluation_context->attack_vectors[vector_index].vector);
					vector_from_points3d(&attack_vector->point, &firing_position->definition->position, &direction);
					along = dot_product3d(&direction, &attack_vector->vector);
					if (along > 0.0f)
					{
						real distance_squared;

						point_from_line3d((real_point3d *)&direction, &attack_vector->vector, -along, (real_point3d *)&direction);
						distance_squared = magnitude_squared3d(&direction);
						if (distance_squared < nearest_distance_squared)
						{
							nearest_distance_squared = distance_squared;
						}
					}
				}
			}

			evaluation = 6.0f;
			if (nearest_distance_squared < 3.5f * 3.5f)
			{
				evaluation = square_root(nearest_distance_squared) * 0.25f;
			}
			firing_position_store_evaluation_debug(
				evaluation_context,
				firing_position,
				evaluation,
				_firing_position_evaluation_enemy_attack_line);
		}
	}

	return;
}
