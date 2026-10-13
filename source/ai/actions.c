/*
ACTIONS.C

symbols in this file:
0000A880 0090:
	_encounter_get_squad (0000)
0000A910 0070:
	_actor_action_perform (0000)
0000A980 0070:
	_actor_action_update (0000)
0000A9F0 0070:
	_actor_action_control (0000)
0000AA60 0070:
	_actor_action_replace_prop (0000)
0000AAD0 0070:
	_actor_action_flush_position_indices (0000)
0000AB40 0070:
	_actor_action_flush_structure_indices (0000)
0000ABB0 00c0:
	_actor_action_handle_panic_from_surprise (0000)
0000AC70 00f0:
	_actor_action_handle_panic_from_damage (0000)
0000AD60 00a0:
	_actor_action_handle_panic_from_burning_to_death (0000)
0000AE00 0090:
	_actor_action_handle_panic_from_attached_projectiles (0000)
0000AE90 0050:
	_actor_action_handle_panic_from_attached_melee_attackers (0000)
0000AEE0 0070:
	_actor_action_handle_berserking_from_attacking_mode (0000)
0000AF50 00c0:
	_actor_action_handle_berserking_from_proximity (0000)
0000B010 0090:
	_actor_action_handle_berserking_from_damage (0000)
0000B0A0 00a0:
	_actor_action_deny_transition (0000)
0000B140 0040:
	_actor_action_allowed_to_enter_vehicle (0000)
0000B180 0150:
	_actor_action_handle_vehicle_exit (0000)
0000B2D0 00e0:
	_actor_action_allow_cover_seeking (0000)
0000B3B0 0170:
	_actor_action_determine_pursuit_options (0000)
0000B520 0090:
	_actor_action_can_stop_guarding (0000)
0000B5B0 0090:
	_actor_action_can_stop_conversing (0000)
0000B640 0150:
	_actor_action_change (0000)
0000B790 01d0:
	_actor_action_test_grenade (0000)
0000B960 0070:
	_actor_action_try_to_seek_cover (0000)
0000B9D0 0060:
	_actor_action_try_to_panic (0000)
0000BA30 00d0:
	_actor_action_try_to_enter_vehicle (0000)
0000BB00 0040:
	_actor_get_pursuit_location (0000)
0000BB40 0090:
	_actor_pursuit_consider_nearby_actor (0000)
0000BBD0 0030:
	_actor_action_name (0000)
0000BC00 0030:
	_actor_mode_name (0000)
0000BC30 00b0:
	_actor_action_debug_color (0000)
0000BCE0 0060:
	_actor_action_class (0000)
0000BD40 0030:
	_actor_action_get_default_state (0000)
0000BD70 0020:
	_set_real_vector2d (0000)
0000BD90 0020:
	_set_real_vector3d (0000)
0000BDB0 0020:
	_point_to_line_distance3d (0000)
0000BDD0 02f0:
	_actor_action_set_default_state (0000)
0000C0C0 0040:
	_actor_action_handle_initial_action (0000)
0000C100 00a0:
	_actor_action_handle_pending_command_list (0000)
0000C1A0 01b0:
	_actor_action_handle_surprise (0000)
0000C350 0160:
	_actor_action_handle_panic_transition (0000)
0000C4B0 0100:
	_actor_action_handle_berserking_from_attached_projectiles (0000)
0000C5B0 03c0:
	_actor_action_handle_vehicle_entry (0000)
0000C970 03a0:
	_actor_action_find_escape_from_danger (0000)
0000CD10 01a0:
	_actor_action_handle_active_cover_seeking (0000)
0000CEB0 06f0:
	_actor_action_handle_combat_selection (0000)
0000D5A0 0750:
	_actor_action_handle_lost_contact (0000)
0000DCF0 0090:
	_actor_action_handle_done_fleeing (0000)
0000DD80 01b0:
	_actor_action_handle_combat_status (0000)
0000DF30 0080:
	_actor_action_handle_combat_failure (0000)
0000DFB0 00c0:
	_actor_action_handle_exit_pursuit (0000)
0000E070 0120:
	_actor_action_try_to_throw_grenade (0000)
0000E190 0120:
	_actor_action_consider_grenade (0000)
0000E2B0 01d0:
	_actor_action_try_to_evade (0000)
0000E480 02d0:
	_actor_action_try_to_dive (0000)
0000E750 0140:
	_actors_searching_same_position (0000)
0000E890 01f0:
	_actor_pursuit_find_nearby_actors (0000)
0000EA80 0080:
	_actor_action_handle_berserk_transition (0000)
0000EB00 00b0:
	_actor_action_handle_combat_transition (0000)
0000EBB0 00d0:
	_actor_action_handle_grenade_throwing (0000)
0000EC80 0320:
	_actor_action_handle_evasion (0000)
0000EFA0 05f0:
	_actor_action_handle_danger_avoidance (0000)
00243B40 0378:
	_global_action_functions (0000)
	_global_dive_animation_table (0310)
	_global_state_move_position_orders (0348)
	_global_default_states (0360)
00243EB8 0006:
	??_C@_05NONFCOJF@avoid?$AA@ (0000)
00243EC0 0009:
	??_C@_08KNPIOEIC@converse?$AA@ (0000)
00243ECC 0005:
	??_C@_04DHBBMIC@obey?$AA@ (0000)
00243ED4 0007:
	??_C@_06MBMIPGD@charge?$AA@ (0000)
00243EDC 0008:
	??_C@_07CJDNLGID@vehicle?$AA@ (0000)
00243EE4 0005:
	??_C@_04PNOAOIAG@wait?$AA@ (0000)
00243EEC 0007:
	??_C@_06GFPLNGOC@search?$AA@ (0000)
00243EF4 0006:
	??_C@_05LPIAIMPK@guard?$AA@ (0000)
00243EFC 0008:
	??_C@_07CNKHPIEN@uncover?$AA@ (0000)
00243F04 0005:
	??_C@_04IMJFEJN@flee?$AA@ (0000)
00243F0C 0006:
	??_C@_05KPNEEOPH@fight?$AA@ (0000)
00243F14 0006:
	??_C@_05MBIJBPBC@alert?$AA@ (0000)
00243F1C 0006:
	??_C@_05PCJCMMHD@sleep?$AA@ (0000)
00243F24 0005:
	??_C@_04CGFJFPFD@none?$AA@ (0000)
00243F30 0047:
	??_C@_0EH@CGNGLHPB@squad_absolute_index?$DO?$DN0?5?$CG?$CG?5squad@ (0000)
00243F78 0061:
	??_C@_0GB@EMHNKODG@squad_index?$DO?$DN0?5?$CG?$CG?5squad_index?$DMMA@ (0000)
00243FDC 001f:
	??_C@_0BP@NJNCCKKA@c?3?2halo?2source?2ai?2encounters?4h?$AA@ (0000)
00244000 004e:
	??_C@_0EO@IKFFOKNJ@?$CIactor?9?$DOstate?4action?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIa@ (0000)
00244050 001c:
	??_C@_0BM@GPCMGMOB@c?3?2halo?2SOURCE?2ai?2actions?4c?$AA@ (0000)
00244070 0054:
	??_C@_0FE@KHFDCMPI@?$CIactor?9?$DOstimuli?4panic_type?5?$DN?$DN?50?$CJ@ (0000)
002440C4 0028:
	??_C@_0CI@KJFLMANP@actor?9?$DOtarget?4target_prop_index?5@ (0000)
002440EC 002b:
	??_C@_0CL@GADKCBCF@actor?9?$DOstate?4action?5?$DN?$DN?5_actor_ac@ (0000)
00244118 0043:
	??_C@_0ED@CKGDDGIN@global_action_functions?$FLnew_acti@ (0000)
00244160 0046:
	??_C@_0EG@OMPPNNLA@?$CInew_action_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CInew_a@ (0000)
002441A8 0008:
	??_C@_07CIFAGBMG@unknown?$AA@ (0000)
002441B0 002e:
	??_C@_0CO@OBMELGHJ@actor?9?$DOstimuli?4panic_prop_index?5@ (0000)
002441E0 0004:
	__real@40c00000 (0000)
002441E4 0004:
	__real@40400000 (0000)
002441E8 0056:
	??_C@_0FG@OAJOJGLC@escape_direction_reference?5?$CG?$CG?5es@ (0000)
00244240 0004:
	__real@be99999a (0000)
00244244 0004:
	__real@3d088889 (0000)
00244248 0039:
	??_C@_0DJ@MMBFFLAI@actor?9?$DOdanger_zone?4danger_type?5?$DO@ (0000)
00244284 002b:
	??_C@_0CL@HAGOODCI@actor?9?$DOstate?4action?5?$DN?$DN?5_actor_ac@ (0000)
002442B0 00a6:
	??_C@_0KG@MODGEEMN@?$CBactor?9?$DOstate?4action_data?4charge@ (0000)
00244358 0033:
	??_C@_0DD@BHCNCEDG@?$CBactor?9?$DOstate?4action_data?4charge@ (0000)
0024438C 002c:
	??_C@_0CM@BKKPPKIL@actor?9?$DOstate?4action?5?$DN?$DN?5_actor_ac@ (0000)
002443B8 0008:
	??_C@_07IPNMCAN@success?$AA@ (0000)
002443C0 0046:
	??_C@_0EG@EGFAACLG@handled?5?$HM?$HM?5?$CIactor_action_class?$CIa@ (0000)
00244408 000a:
	??_C@_09FGCFOJPF@encounter?$AA@ (0000)
00244414 0025:
	??_C@_0CF@BPNEOJLI@evade_direction?5?$DN?$DN?5_actor_evade_@ (0000)
00244440 0052:
	??_C@_0FC@DABHPCKC@?$CIpossibility?9?$DOanimation_directio@ (0000)
00244494 0004:
	__real@40e00000 (0000)
00244498 0004:
	__real@42340000 (0000)
0024449C 0004:
	__real@7f7fffff (0000)
002B6AD0 0010:
	_global_actor_mode_names (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"

#include "actors.h"
#include "actor_definitions.h"
#include "actor_types.h"
#include "ai.h"
#include "ai_communication.h"
#include "ai_debug.h"
#include "ai_profile.h"
#include "encounters.h"
#include "props.h"

#include "game/game.h"
#include "game/game_globals.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/units.h"
#include "units/unit_definitions.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

/* ---------- constants */

enum
{
	NUMBER_OF_ACTOR_MODES = 4,
	NUMBER_OF_ACTOR_DEFAULT_STATES = 12,
};

enum
{
	_actor_panic_none = 0,
	_actor_panic_damage,
	_actor_panic_unopposable_enemy = 5,
	_actor_panic_surprise = 7,
	_actor_panic_delayed_projectile_attached_to_us = 10,
	_actor_panic_melee_attached_to_us = 11,
	_actor_panic_burning_to_death,
};

enum
{
	_actor_berserk_attacking_mode = 1,
	_actor_berserk_proximity,
	_actor_berserk_damage = 3,
	_actor_berserk_attached_projectile,
};

enum
{
	_ai_conversation_stop_if_damaged_bit = 1,
	_ai_conversation_stop_if_visible_enemy_bit,
	_ai_conversation_stop_if_alerted_to_enemy_bit,
};

enum
{
	_actor_grenade_stimulus_visible_target = 1,
	_actor_grenade_stimulus_seek_cover,
};

enum
{
	_actor_animation_surprise_front = 4,
	_actor_animation_surprise_back,
	_actor_animation_evade_left,
	_actor_animation_evade_right,
};

/* TU-local copy: no shared owner header declares it; actor_moving.c holds the other copy. */
enum actor_evade_direction
{
	_actor_evade_left = 0,
	_actor_evade_right,
	_actor_evade_forward,
	_actor_evade_back,
	_actor_evade_random,
};

enum
{
	_ai_index_platoon = 1,
	_ai_index_squad,
};

#define ACTOR_GRENADE_THROW_MINIMUM_COSINE 0.86602539f
#define ACTOR_GRENADE_TICKS_PER_SECOND 30.0f
#define AI_INDEX_SCOPE(index) ((unsigned long)(index) >> 30)
#define AI_INDEX_SUB_INDEX(index) (((unsigned long)(index) >> 16) & 0xFF)

/* ---------- macros */

/* ---------- structures */

/* The fields used here from the 0x74-byte scenario conversation record. */
struct scenario_conversation_action_definition
{
	char name[TAG_STRING_LENGTH + 1];
	word flags;
	byte __unknown22[0x52];
};

typedef char scenario_conversation_action_definition_size_assert[
	sizeof(struct scenario_conversation_action_definition) == 0x74 ? 1 : -1];

/* Only the January-proven field consumed by this translation unit is named. */
struct ai_vehicle_enterable
{
	long vehicle_index;
	real radius;
	short team_bitmask;
	short actor_type_bitmask;
	short ai_indices_count;
	word __pad0E;
	long ai_indices[6];
};

struct ai_globals_action_data
{
	byte __unknown0[0x3B4];
	boolean grenades_enabled;
	byte __unknown3B5;
	short enterable_vehicle_count;
	struct ai_vehicle_enterable enterable_vehicles[32];
};

/* The January iterator is exactly three datum indices; callers consume the
 * current actor index after encounter_actor_iterator_next advances it. */
struct encounter_actor_iterator
{
	long encounter_index;
	long actor_index;
	long next_actor_index;
};

struct actor_dive_animation
{
	short animation;
	short animation_direction;
	real score_bias;
};

/* The shared vehicle tag layout is still opaque past the common unit
 * definition.  Actions reads the two January-authenticated AI-driving
 * values at 0x390/0x394 only, so keep that ownership local to this TU. */
struct vehicle_definition
{
	byte __unknown0[0x390];
	real ai_charge_repeat_time;
	real ai_strafing_stop_range;
};

typedef char ai_globals_action_vehicle_size_assert[
	sizeof(struct ai_vehicle_enterable) == 0x28 ? 1 : -1];
typedef char ai_globals_action_grenades_enabled_offset_assert[
	offsetof(struct ai_globals_action_data, grenades_enabled) == 0x3B4 ? 1 : -1];
typedef char ai_globals_action_enterable_vehicles_offset_assert[
	offsetof(struct ai_globals_action_data, enterable_vehicles) == 0x3B8 ? 1 : -1];
typedef char ai_globals_action_data_size_assert[
	sizeof(struct ai_globals_action_data) == 0x8B8 ? 1 : -1];
typedef char actions_prop_enemy_offset_assert[
	offsetof(struct prop_datum, enemy) == 0x60 ? 1 : -1];
typedef char actions_prop_dead_offset_assert[
	offsetof(struct prop_datum, dead) == 0x127 ? 1 : -1];
typedef char actions_actor_moving_offset_assert[
	offsetof(struct actor_datum, control.moving) == 0x504 ? 1 : -1];
typedef char actions_unit_dive_distance_offset_assert[
	(offsetof(struct unit_definition, unit) +
		offsetof(struct _unit_definition, dive_distance)) == 0x238 ? 1 : -1];
typedef char actions_vehicle_charge_repeat_offset_assert[
	offsetof(struct vehicle_definition, ai_charge_repeat_time) == 0x390 ? 1 : -1];
typedef char actions_vehicle_strafing_stop_offset_assert[
	offsetof(struct vehicle_definition, ai_strafing_stop_range) == 0x394 ? 1 : -1];
typedef char actions_actor_debug_cover_offset_assert[
	offsetof(struct actor_debug_info, field_B8) == 0xB8 ? 1 : -1];
typedef char actions_actor_debug_grenade_offset_assert[
	offsetof(struct actor_debug_info, grenade_eval_time) == 0x150 ? 1 : -1];
typedef char actions_actor_debug_danger_offset_assert[
	offsetof(struct actor_debug_info, danger_avoidance_time) == 0x168 ? 1 : -1];
typedef char actions_actor_debug_dive_offset_assert[
	offsetof(struct actor_debug_info, dive_decision_time) == 0x184 ? 1 : -1];
typedef char actions_actor_debug_info_size_assert[
	sizeof(struct actor_debug_info) == 0x657C ? 1 : -1];
typedef char firing_position_evaluation_context_size_assert[
	sizeof(struct firing_position_evaluation_context) == 0x670 ? 1 : -1];

/* ---------- prototypes */

boolean action_guard_setup_from_fleeing(
	long actor_index,
	struct flee_state_data *flee_state,
	struct guard_state_data *state_data);

boolean action_guard_setup_from_combat_transition(
	long actor_index,
	struct guard_state_data *state_data);

boolean action_guard_setup_current_position(
	long actor_index,
	struct guard_state_data *state_data);

boolean action_guard_setup_find_position(
	long actor_index,
	long wait_ticks,
	struct guard_state_data *state_data);

boolean action_guard_setup_postcombat(
	long actor_index,
	struct guard_state_data *state_data);

void actor_action_change(
	long actor_index,
	long new_action_type,
	struct action_state_data *new_action_data);

long actor_perception_find_recent_damaging_prop_index(
	long actor_index,
	boolean enemies_only);

long ai_get_responsible_unit(
	long object_index,
	boolean include_self);

boolean actor_action_handle_combat_selection(
	long actor_index);

boolean actor_action_handle_lost_contact(
	long actor_index);

boolean actor_action_allow_cover_seeking(
	long actor_index,
	boolean unopposable);

short vehicle_scripting_find_available_seats(
	long vehicle_index,
	char const *seat_substring_name,
	short seat_desire_type,
	short *seat_indices,
	short maximum_seat_count);

boolean action_vehicle_setup_specific(
	long actor_index,
	long vehicle_index,
	short seat_index,
	struct vehicle_state_data *state_data);

void actor_perception_tried_to_uncover(
	long actor_index,
	long prop_index);

void actor_perception_tried_to_search(
	long actor_index,
	long prop_index);

void actor_perception_abandoned_search(
	long actor_index,
	long prop_index);

boolean actor_action_consider_grenade(
	long actor_index);

boolean actor_action_test_grenade(
	long actor_index);

boolean actor_action_try_to_panic(
	long actor_index,
	short panic_type,
	long panic_prop_index,
	boolean force_flee_transition);

boolean actor_action_try_to_seek_cover(
	long actor_index,
	boolean cover_from_last_visible_location,
	boolean allow_occluded_points);

boolean actor_action_try_to_throw_grenade(
	long actor_index,
	boolean known_trajectory);

boolean action_vehicle_setup_impromptu(
	long actor_index,
	long vehicle_index,
	real attempt_distance,
	real continue_distance,
	struct vehicle_state_data *state_data);

boolean action_charge_setup(
	long actor_index,
	short goal,
	struct charge_state_data *state_data);

boolean action_fight_setup(
	long actor_index,
	struct fight_state_data *state_data);

boolean action_uncover_setup_target(
	long actor_index,
	boolean able_to_search,
	struct uncover_state_data *state_data);

boolean action_uncover_setup_pursuit(
	long actor_index,
	short firing_position_index,
	struct uncover_state_data *state_data);

boolean action_search_setup_target(
	long actor_index,
	boolean must_charge,
	struct search_state_data *state_data);

boolean action_search_setup_pursuit(
	long actor_index,
	short firing_position_index,
	boolean tenacious,
	struct search_state_data *state_data);

boolean action_search_setup_undirected(
	long actor_index,
	boolean tenacious,
	struct search_state_data *state_data);

boolean action_wait_setup(
	long actor_index,
	boolean was_actively_searching,
	struct wait_state_data *state_data);

boolean action_avoid_setup(
	long actor_index,
	struct avoid_state_data *state_data);

real vector_to_line_distance_squared3d(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *base,
	real_vector3d const *height);

boolean actor_move_try_evasion_direction(
	long actor_index,
	real_vector2d *alignment_vector,
	real evade_distance,
	short *evade_direction,
	real ledge_avoidance_distance,
	boolean *is_ledge,
	void *collision_result);

boolean actor_move_try_evasion_vector(
	long actor_index,
	real_vector2d const *evasion_vector,
	real evade_distance,
	real ledge_avoidance_distance,
	boolean *is_ledge,
	void *collision_result);

void actor_stimulus_was_surprised(
	long actor_index);

boolean actor_situation_try_new_target(
	long actor_index,
	long prop_index);

void action_sleep_control(
	long actor_index);

void action_fight_begin(
	long actor_index);
boolean action_fight_perform(
	long actor_index);
void action_fight_update(
	long actor_index);
void action_fight_control(
	long actor_index);
void action_fight_end(
	long actor_index);

void action_uncover_begin(
	long actor_index);
boolean action_uncover_perform(
	long actor_index);
void action_uncover_update(
	long actor_index);
void action_uncover_control(
	long actor_index);
void action_uncover_modify_color(
	long actor_index,
	real_argb_color *color);
void action_uncover_flush_position_indices(
	long actor_index);

void action_guard_begin(
	long actor_index);
boolean action_guard_perform(
	long actor_index);
void action_guard_update(
	long actor_index);
void action_guard_control(
	long actor_index);
void action_guard_end(
	long actor_index);
void action_guard_modify_color(
	long actor_index,
	real_argb_color *color);
void action_guard_replace_prop(
	long actor_index,
	long invalid_prop_index,
	long replacement_prop_index);
void action_guard_flush_position_indices(
	long actor_index);
void action_guard_flush_structure_indices(
	long actor_index);

void action_search_begin(
	long actor_index);
boolean action_search_perform(
	long actor_index);
void action_search_update(
	long actor_index);
void action_search_control(
	long actor_index);
void action_search_flush_position_indices(
	long actor_index);

void action_wait_begin(
	long actor_index);
boolean action_wait_perform(
	long actor_index);
void action_wait_update(
	long actor_index);
void action_wait_control(
	long actor_index);

boolean action_vehicle_perform(
	long actor_index);
void action_vehicle_update(
	long actor_index);

void action_charge_begin(
	long actor_index);
boolean action_charge_perform(
	long actor_index);
void action_charge_update(
	long actor_index);
void action_charge_control(
	long actor_index);

void action_converse_begin(
	long actor_index);
boolean action_converse_perform(
	long actor_index);
void action_converse_update(
	long actor_index);
void action_converse_control(
	long actor_index);
void action_converse_end(
	long actor_index);
void action_converse_replace_prop(
	long actor_index,
	long invalid_prop_index,
	long replacement_prop_index);

void action_avoid_begin(
	long actor_index);
boolean action_avoid_perform(
	long actor_index);
void action_avoid_update(
	long actor_index);
void action_avoid_control(
	long actor_index);
void action_avoid_end(
	long actor_index);

static boolean actor_action_allowed_to_enter_vehicle(
	long actor_index,
	long vehicle_index);

/* ---------- globals */

extern struct ai_globals_action_data *ai_globals;

struct action_specification const global_action_functions[NUMBER_OF_ACTOR_ACTIONS] =
{

	{
		0,
		"none",
		&global_real_argb_black,
		0u,
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		1,
		"sleep",
		&global_real_argb_lightblue,
		0u,
		0,
		NULL,
		NULL,
		NULL,
		&action_sleep_control,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		2,
		"alert",
		&global_real_argb_cyan,
		92u,
		0,
		&action_alert_begin,
		&action_alert_perform,
		&action_alert_update,
		&action_alert_control,
		NULL,
		NULL,
		NULL,
		&action_alert_flush_position_indices,
		&action_alert_flush_structure_indices
	},
	{
		3,
		"fight",
		&global_real_argb_white,
		4u,
		4,
		&action_fight_begin,
		&action_fight_perform,
		&action_fight_update,
		&action_fight_control,
		&action_fight_end,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		4,
		"flee",
		&global_real_argb_yellow,
		48u,
		2,
		&action_flee_begin,
		&action_flee_perform,
		&action_flee_update,
		&action_flee_control,
		&action_flee_end,
		&action_flee_modify_color,
		&action_flee_replace_prop,
		&action_flee_flush_position_indices,
		NULL
		},
		{
		5,
		"uncover",
		&global_real_argb_blue,
		52u,
		3,
		&action_uncover_begin,
		&action_uncover_perform,
		&action_uncover_update,
		&action_uncover_control,
		NULL,
		&action_uncover_modify_color,
		NULL,
		&action_uncover_flush_position_indices,
		NULL
	},
	{
		6,
		"guard",
		&global_real_argb_salmon,
		68u,
		1,
		&action_guard_begin,
		&action_guard_perform,
		&action_guard_update,
		&action_guard_control,
		&action_guard_end,
		&action_guard_modify_color,
		&action_guard_replace_prop,
		&action_guard_flush_position_indices,
		&action_guard_flush_structure_indices
	},
	{
		7,
		"search",
		&global_real_argb_aqua,
		44u,
		3,
		&action_search_begin,
		&action_search_perform,
		&action_search_update,
		&action_search_control,
		NULL,
		NULL,
		NULL,
		&action_search_flush_position_indices,
		NULL
	},
	{
		8,
		"wait",
		&global_real_argb_magenta,
		24u,
		3,
		&action_wait_begin,
		&action_wait_perform,
		&action_wait_update,
		&action_wait_control,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		9,
		"vehicle",
		&global_real_argb_darkgreen,
		76u,
		2,
		&action_vehicle_begin,
		&action_vehicle_perform,
		&action_vehicle_update,
		&action_vehicle_control,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		10,
		"charge",
		&global_real_argb_red,
		56u,
		4,
		&action_charge_begin,
		&action_charge_perform,
		&action_charge_update,
		&action_charge_control,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		11,
		"obey",
		&global_real_argb_purple,
		132u,
		2,
		&action_obey_begin,
		&action_obey_perform,
		&action_obey_update,
		&action_obey_control,
		&action_obey_end,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		12,
		"converse",
		&global_real_argb_orange,
		20u,
		2,
		&action_converse_begin,
		&action_converse_perform,
		&action_converse_update,
		&action_converse_control,
		&action_converse_end,
		NULL,
		&action_converse_replace_prop,
		NULL,
		NULL
	},
	{
		13,
		"avoid",
		&global_real_argb_grey,
		4u,
		2,
		&action_avoid_begin,
		&action_avoid_perform,
		&action_avoid_update,
		&action_avoid_control,
		&action_avoid_end,
		NULL,
		NULL,
		NULL,
		NULL
	}
};

struct actor_dive_animation const global_dive_animation_table[7] =
{
	{ 10, 0, 1.5f },
	{ 6, 0, 0.0f },
	{ 11, 1, 1.5f },
	{ 7, 1, 0.0f },
	{ 8, 2, 1.5f },
	{ 9, 3, 1.5f },
	{ NONE, NONE, 0.0f },
};

short const global_state_move_position_orders[NUMBER_OF_ACTOR_DEFAULT_STATES] =
{
	0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0,
};

short const global_default_states[NUMBER_OF_ACTOR_DEFAULT_STATES] =
{
	0, 2, 2, 3, 4, 5, 6, 7, 8, 9, 9, 8,
};

char const *global_actor_mode_names[NUMBER_OF_ACTOR_MODES] =
{
	"asleep",
	"alert",
	"combat",
	NULL
};

/* ---------- public code */

boolean actor_action_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		143,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].perform)
		result = global_action_functions[actor->state.action].perform(actor_index);

	return result;
}

void actor_action_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		158,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].update)
		global_action_functions[actor->state.action].update(actor_index);

	return;
}

void actor_action_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		173,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].control)
		global_action_functions[actor->state.action].control(actor_index);

	return;
}

void actor_action_replace_prop(
	long actor_index,
	long invalid_prop_index,
	long replacement_prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		190,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].replace_prop)
	{
		global_action_functions[actor->state.action].replace_prop(
			actor_index,
			invalid_prop_index,
			replacement_prop_index);
	}

	return;
}

void actor_action_flush_position_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		205,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].flush_position_indices)
	{
		global_action_functions[actor->state.action].flush_position_indices(
			actor_index);
	}

	return;
}

void actor_action_flush_structure_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		220,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	if (global_action_functions[actor->state.action].flush_structure_indices)
	{
		global_action_functions[actor->state.action].flush_structure_indices(
			actor_index);
	}

	return;
}

char const *actor_action_name(
	short actor_action_index)
{
	char const *result = "unknown";

	if (VALID_INDEX(actor_action_index, NUMBER_OF_ACTOR_ACTIONS))
		result = global_action_functions[actor_action_index].name;

	return result;
}

char const *actor_mode_name(
	short actor_mode_index)
{
	char const *result = "unknown";

	if (VALID_INDEX(actor_mode_index, NUMBER_OF_ACTOR_MODES))
		result = global_actor_mode_names[actor_mode_index];

	return result;
}

real_argb_color *actor_action_debug_color(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	global_temporary_render_color = *global_real_argb_black;

	if (VALID_INDEX(actor->state.action, NUMBER_OF_ACTOR_ACTIONS))
	{
		global_temporary_render_color = **global_action_functions[actor->state.action].color;

		if (global_action_functions[actor->state.action].modify_color)
		{
			global_action_functions[actor->state.action].modify_color(actor_index, &global_temporary_render_color);
		}
	}

	return &global_temporary_render_color;
}

short actor_action_class(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		3736,
		(actor->state.action >= 0) && (actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	return global_action_functions[actor->state.action].action_class;
}

short actor_action_get_default_state(
	short initial_state)
{
	short result = 0;

	if (VALID_INDEX(initial_state, NUMBER_OF_ACTOR_DEFAULT_STATES))
		result = global_default_states[initial_state];

	return result;
}

struct pursuit_location *actor_get_pursuit_location(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct pursuit_location *result = NULL;

	if (actor->state.action == _actor_action_search)
	{
		result = &actor->state.action_data.search.pursuit_location;
	}
	else if (actor->state.action == _actor_action_uncover)
	{
		result = &actor->state.action_data.uncover.pursuit_location;
	}

	return result;
}

boolean actor_action_set_default_state(
	long actor_index,
	short override_state)
{
	struct actor_datum *actor = actor_get(actor_index);
	long current_time = game_time_get();
	boolean changed = FALSE;
	struct action_state_data action_data;

	if (override_state != NONE ||
		actor->state.last_default_state_time == NONE ||
		actor->state.last_default_state_time + 45 < current_time)
	{
		short state;

		actor->state.last_default_state_time = current_time;

		if (override_state != NONE)
			state = override_state;
		else if (actor->state.initial_state != NONE)
		{
			state = actor->state.initial_state;
			actor->state.initial_state = NONE;
		}
		else
		{
			state = actor->state.default_state == NONE ? 0 :
				actor->state.default_state;
		}

		switch (state)
		{
		case 1:
			if (actor->state.mode != 1)
			{
				actor->state.mode = 1;
				actor_action_change(actor_index, _actor_action_sleep, NULL);
				changed = TRUE;
			}
			break;

		case 8:
			if ((actor->state.action != _actor_action_guard ||
				actor->state.action_data.guard.guard_location_type != 1) &&
				action_guard_setup_current_position(actor_index, &action_data.guard))
			{
				actor_action_change(actor_index, _actor_action_guard, &action_data);
				changed = TRUE;
			}
			break;

		case 9:
			if (actor->state.action == _actor_action_guard)
			{
				if (actor->state.action_data.guard.guard_location_type != 3)
					actor->state.action_data.guard.find_new_guard_position = TRUE;
			}
			else if (action_guard_setup_find_position(
				actor_index,
				0,
				&action_data.guard))
			{
				actor_action_change(actor_index, _actor_action_guard, &action_data);
				changed = TRUE;
			}
			break;

		case 10:
			if (actor_action_class(actor_index) != 3)
			{
				actor->state.mode = 3;
				actor->state.artificial_combat_status = 2;
				actor->state.combat_status = 2;

				if (!actor_action_handle_lost_contact(actor_index) &&
					action_guard_setup_current_position(actor_index, &action_data.guard))
				{
					actor_action_change(actor_index, _actor_action_guard, &action_data);
					changed = TRUE;
				}
			}
			break;

		case 11:
			if (actor->state.action != _actor_action_flee)
			{
				if (action_flee_setup(
					actor_index,
					13,
					NONE,
					TRUE,
					FALSE,
					FALSE,
					&action_data.flee))
				{
					actor_action_change(actor_index, _actor_action_flee, &action_data);
					changed = TRUE;
				}
				else if (actor->state.action != _actor_action_guard &&
					action_guard_setup_current_position(actor_index, &action_data.guard))
				{
					actor_action_change(actor_index, _actor_action_guard, &action_data);
					changed = TRUE;
				}
			}
			break;

		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			if ((actor->state.action != _actor_action_alert ||
				actor->state.action_data.alert.move_position_order !=
					global_state_move_position_orders[state]) &&
				action_alert_setup(
					actor_index,
					global_state_move_position_orders[state],
					NONE,
					&action_data.alert))
			{
				actor_action_change(actor_index, _actor_action_alert, &action_data);
				changed = TRUE;
			}
			break;
		}

		if (!changed && actor->state.action == _actor_action_none &&
			action_alert_setup(actor_index, 0, NONE, &action_data.alert))
		{
			actor_action_change(actor_index, _actor_action_alert, &action_data);
			changed = TRUE;
		}
	}

	return changed;
}

boolean actor_action_handle_initial_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->state.action == _actor_action_none && actor->state.mode != 0)
		result = actor_action_set_default_state(actor_index, NONE);

	return result;
}

boolean actor_action_handle_pending_command_list(
	long actor_index)
{
	struct action_state_data state;
	struct actor_datum *actor = actor_get(actor_index);
	boolean entered_obey = FALSE;

	if (actor->state.command_list_index != NONE)
	{
		boolean blocked = FALSE;

		if (!actor->state.command_list_immediate &&
			(actor->state.mode == 0 || actor_action_deny_transition(actor_index)))
		{
			blocked = TRUE;
		}

		if (!blocked)
		{
			if (action_obey_command_list_setup(
				actor_index,
				actor->state.command_list_index,
				&state.obey))
			{
				actor_action_change(actor_index, _actor_action_obey, &state);
				entered_obey = TRUE;
			}

			actor->state.command_list_immediate = FALSE;
			actor->state.command_list_index = NONE;
		}
	}

	return entered_obey;
}

boolean actor_action_handle_panic_from_attached_melee_attackers(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->input.melee_attacker_attached)
	{
		actor->stimuli.panic_type = MAX(
			actor->stimuli.panic_type,
			_actor_panic_melee_attached_to_us);
		actor->stimuli.panic_prop_index = NONE;
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_panic_from_surprise(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (actor->stimuli.was_surprised &&
		TEST_FLAG(definition->flags, _actor_definition_panic_on_surprise_bit))
	{
		if (actor->stimuli.panic_type == _actor_panic_none ||
			actor->stimuli.panic_prop_index == NONE)
		{
			actor->stimuli.panic_prop_index =
				actor->stimuli.surprise_prop_index;
		}

		actor->stimuli.panic_type = MAX(
			actor->stimuli.panic_type,
			_actor_panic_surprise);
		actor->stimuli.was_surprised = FALSE;
		result = TRUE;
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		528,
		(actor->stimuli.panic_type == 0) ||
			(actor->stimuli.panic_prop_index != 0x00000000));

	return result;
}

boolean actor_action_handle_panic_from_damage(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (actor->stimuli.was_damaged &&
		((game_connection() == _game_connection_local &&
			ai_debug.flee_always) ||
			actor->input.recent_body_damage >
				definition->panic.panic_damage_threshold))
	{
		if (actor->stimuli.panic_type == _actor_panic_none ||
			actor->stimuli.panic_prop_index == NONE)
		{
			actor->stimuli.panic_prop_index =
				actor_perception_find_recent_damaging_prop_index(
					actor_index,
					TRUE);
		}

		actor->stimuli.panic_type = MAX(
			actor->stimuli.panic_type,
			_actor_panic_damage);
		actor->stimuli.was_damaged = FALSE;
		result = TRUE;
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		552,
		(actor->stimuli.panic_type == 0) ||
			(actor->stimuli.panic_prop_index != 0x00000000));

	return result;
}

boolean actor_action_handle_panic_from_attached_projectiles(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->input.delayed_attached_projectile_index != NONE)
	{
		struct object_datum *attached_projectile =
			object_try_and_get(actor->input.delayed_attached_projectile_index);
		long panic_prop_index = NONE;

		if (attached_projectile)
		{
			long responsible_unit_index = ai_get_responsible_unit(
				attached_projectile->object.owner_object_index,
				TRUE);

			if (responsible_unit_index != NONE)
			{
				panic_prop_index = prop_get_active_by_unit_index(
					actor_index,
					responsible_unit_index);
			}
		}

		if (actor->stimuli.panic_type == _actor_panic_none ||
			actor->stimuli.panic_prop_index == NONE)
		{
			actor->stimuli.panic_prop_index = panic_prop_index;
		}

		actor->stimuli.panic_type = MAX(
			actor->stimuli.panic_type,
			_actor_panic_delayed_projectile_attached_to_us);
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_panic_from_burning_to_death(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->input.burning_to_death)
	{
		long panic_prop_index = NONE;

		if (actor->meta.unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(actor->meta.unit_index);
			long responsible_unit_index = ai_get_responsible_unit(
				unit->unit.flaming_death_attacker_object_index,
				TRUE);

			if (responsible_unit_index != NONE)
			{
				panic_prop_index = prop_get_active_by_unit_index(
					actor_index,
					responsible_unit_index);
			}
		}

		if (actor->stimuli.panic_type == _actor_panic_none ||
			actor->stimuli.panic_prop_index == NONE)
		{
			actor->stimuli.panic_prop_index = panic_prop_index;
		}

		actor->stimuli.panic_type = MAX(
			actor->stimuli.panic_type,
			_actor_panic_burning_to_death);
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_berserking_from_attacking_mode(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (TEST_FLAG(
			definition->flags,
			_actor_definition_always_berserk_in_attacking_mode_bit) &&
		!actor->external_orders.defending &&
		actor->state.combat_status >= 5)
	{
		actor->stimuli.berserk_type = MAX(
			actor->stimuli.berserk_type,
			_actor_berserk_attacking_mode);
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_berserking_from_damage(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (actor->stimuli.was_damaged &&
		actor->input.recent_body_damage >
			definition->berserk.damage_berserk_amount &&
		actor->input.body_vitality <
			definition->berserk.damage_berserk_threshold)
	{
		actor->stimuli.berserk_type = MAX(
			actor->stimuli.berserk_type,
			_actor_berserk_damage);
		actor->stimuli.was_damaged = FALSE;
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_berserking_from_proximity(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (actor->state.combat_status >= 5)
	{
		struct prop_datum *prop = prop_get(actor->target.target_prop_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actions.c",
			737,
			actor->target.target_prop_index != NONE);

		if (prop->distance < definition->berserk.proximity_berserk_distance)
		{
			actor->stimuli.berserk_type = MAX(
				actor->stimuli.berserk_type,
				_actor_berserk_proximity);
			result = TRUE;
		}
	}

	return result;
}

boolean actor_action_deny_transition(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean deny_transition = FALSE;

	if (actor->state.command_list_index != NONE &&
		actor->state.command_list_delay_timer > 0)
	{
		deny_transition = TRUE;
	}

	if (actor->meta.encounter_index != NONE)
	{
		struct encounter_datum *encounter =
			encounter_get(actor->meta.encounter_index);
		struct squad_datum *squad = encounter_get_squad(
			encounter,
			actor->meta.squad_index);

		if (squad->delay_timer > 0)
		{
			if (actor->state.combat_status >= 5)
			{
				encounter_squad_timer_expire(
					actor->meta.encounter_index,
					actor->meta.squad_index);
			}
			else
			{
				deny_transition = TRUE;
			}
		}
	}

	if (actor->state.action == _actor_action_obey &&
		!actor->state.action_data.obey.initiative &&
		!actor->state.action_data.obey.finished)
	{
		deny_transition = TRUE;
	}

	return deny_transition;
}

static void actor_action_determine_pursuit_options(
	long actor_index,
	short desire_target_search,
	short desire_pursuit,
	short desire_pursuit_search,
	boolean controlling_group,
	boolean controlled_by_group,
	boolean should_return_to_idle,
	boolean force_charge,
	boolean *allow_target_uncover,
	boolean *allow_indefinite_target_uncover,
	boolean *allow_target_search,
	boolean *allow_pursuit,
	boolean *allow_pursuit_search,
	boolean *wait_after_pursuit)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (desire_target_search == 2 ||
		(desire_target_search == 1 && controlling_group))
	{
		*allow_target_search = FALSE;
	}

	if (desire_pursuit == 2 ||
		(desire_pursuit == 1 && controlling_group))
	{
		*allow_pursuit = FALSE;
		*allow_pursuit_search = FALSE;
	}
	else if (desire_pursuit_search == 2 ||
		(desire_pursuit_search == 1 && controlling_group))
	{
		*allow_pursuit_search = FALSE;
	}

	if (should_return_to_idle)
	{
		*allow_pursuit = FALSE;
		*allow_pursuit_search = FALSE;
		*wait_after_pursuit = FALSE;
	}

	if (actor->target.target_prop_index == NONE)
	{
		*allow_target_uncover = FALSE;
		*allow_target_search = FALSE;
		return;
	}

	{
		struct prop_datum *prop =
			prop_get(actor->target.target_prop_index);

		*allow_target_uncover = !prop->tried_to_uncover;
		*allow_target_search = !prop->tried_to_search;

		if (force_charge)
		{
			*allow_target_uncover = FALSE;
		}
		else if (*allow_target_search)
		{
			if (prop->state >= 2 && prop->state <= 3)
			{
				actor_perception_find_prop_pathfinding_location(
					actor_index,
					actor->target.target_prop_index);
			}

			*allow_target_search = actor_nearby_firing_positions(
				actor_index,
				&prop->pathfinding_point,
				prop->pathfinding_surface_index,
				_firing_position_group_when_searching);
		}
	}

	if (!*allow_indefinite_target_uncover && actor->state.combat_status < 3)
		*allow_target_uncover = FALSE;

	if (actor->input.vehicle_driver_type > 0)
		*wait_after_pursuit = FALSE;

	if (actor->input.vehicle_driver_type == 4)
	{
		*allow_target_search = FALSE;
		*allow_pursuit_search = FALSE;
	}

	return;
}

boolean actor_action_can_stop_guarding(
	long actor_index,
	short guard_investigate_threshold,
	short cower_investigate_threshold)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		2697,
		actor->state.action == _actor_action_guard);

	if (actor->state.action_data.guard.cower)
	{
		return (boolean)(actor->state.combat_status >=
			cower_investigate_threshold);
	}

	if (actor->state.action_data.guard.wait_ticks <= 0)
		return TRUE;

	return (boolean)(actor->state.combat_status >= guard_investigate_threshold ||
		(actor->external_orders.postcombat_type > 0 &&
			!actor->state.action_data.guard.post_combat));
}

boolean actor_action_can_stop_conversing(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	long conversation_index = actor->external_orders.conversation_index;
	boolean result = TRUE;

	if (conversation_index != NONE)
	{
		word flags;
		struct ai_conversation_datum_header *conversation =
			ai_conversation_header_get(conversation_index);
		struct scenario_conversation_action_definition *definition =
			TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_conversations,
				conversation->scenario_conversation_index,
				struct scenario_conversation_action_definition);

		flags = definition->flags;

		if (TEST_FLAG(flags, _ai_conversation_stop_if_damaged_bit) &&
			actor->situation.specific_threats[8])
		{
			result = TRUE;
		}
		else if (TEST_FLAG(flags, _ai_conversation_stop_if_visible_enemy_bit) &&
			actor->target.target_type >= 9)
		{
			result = TRUE;
		}
		else if (TEST_FLAG(flags, _ai_conversation_stop_if_alerted_to_enemy_bit) &&
			actor->target.target_type >= 6)
		{
			result = TRUE;
		}
		else
		{
			result = FALSE;
		}
	}

	return result;
}

static boolean actor_pursuit_consider_nearby_actor(
	long actor_index,
	boolean pursuit_controller,
	long friend_actor_index)
{
	struct actor_datum *friend_actor;
	boolean result;

	/* The January build intentionally performs this lookup only for its datum
	 * validation side effect; the returned actor is not otherwise needed. */
	actor_get(actor_index);
	friend_actor = actor_try_and_get(friend_actor_index);
	result = FALSE;

	if (friend_actor &&
		friend_actor->state.combat_status >= 2 &&
		friend_actor->state.combat_status < 4 &&
		(friend_actor->state.action == _actor_action_search ||
			friend_actor->state.action == _actor_action_uncover ||
			(!pursuit_controller &&
				friend_actor->state.action == _actor_action_wait) ||
			(friend_actor->state.action == _actor_action_guard &&
				!friend_actor->state.action_data.guard.cower &&
				friend_actor->state.action_data.guard.wait_ticks > 0)))
	{
		if (actor_type_get_pursuit_controller(friend_actor->meta.type) !=
			pursuit_controller)
			result = TRUE;
	}

	return result;
}

boolean actor_action_handle_surprise(
	long actor_index,
	short surprise_level)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	boolean result = FALSE;

	if (!actor->input.vehicle_passenger &&
		actor->stimuli.surprise_level >= surprise_level)
	{
		real_vector2d direction;
		short animation;

		if (actor->stimuli.surprise_has_vector)
		{
			direction.i = actor->stimuli.surprise_vector.i;
			direction.j = actor->stimuli.surprise_vector.j;
			normalize2d(&direction);

			if (!(direction.j * actor->control.desired_facing_vector.j +
				direction.i * actor->control.desired_facing_vector.i >= 0.0f))
			{
				direction.i = -direction.i;
				direction.j = -direction.j;
				animation = _actor_animation_surprise_back;
			}
			else
			{
				animation = _actor_animation_surprise_front;
			}
		}
		else
		{
			direction.i = actor->input.facing_vector.i;
			direction.j = actor->input.facing_vector.j;
			normalize2d(&direction);
			animation = _actor_animation_surprise_front;
		}

		actor_move_animation_impulse(actor_index, animation, &direction);

		{
			long unit_index = NONE;
			long weapon_state = 0;

			if (actor->stimuli.surprise_prop_index != NONE)
			{
				struct prop_datum *prop =
					prop_get(actor->stimuli.surprise_prop_index);
				unit_index = prop->unit_index;
				weapon_state = (prop->enemy != FALSE) + 2;
			}

			ai_communication_event(
				_ai_communication_surprise,
				actor->meta.unit_index,
				unit_index,
				weapon_state,
				NONE,
				NONE,
				NULL);
		}

		if (variant_definition->ranged_combat.surprise_wildfire_time > 0.0f)
		{
			actor_combat_fire_wildly(
				actor_index,
				(long)(variant_definition->ranged_combat.surprise_wildfire_time *
					TICKS_PER_SECOND));
		}

		if (variant_definition->ranged_combat.surprise_delay_time > 0.0f)
		{
			actor_combat_disable_bursts(
				actor_index,
				(long)(variant_definition->ranged_combat.surprise_delay_time *
					TICKS_PER_SECOND));
		}

		actor_stimulus_was_surprised(actor_index);

		if (actor->stimuli.surprise_prop_index != NONE)
		{
			actor_situation_try_new_target(
				actor_index,
				actor->stimuli.surprise_prop_index);
		}

		result = TRUE;
	}

	actor->stimuli.surprise_level = 0;
	return result;
}

boolean actor_action_handle_berserking_from_attached_projectiles(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct actor_variant_definition *firing_variant =
		actor_combat_get_firing_variant_definition(actor_index);
	boolean result = FALSE;

	if (actor->input.delayed_attached_projectile_index != NONE &&
		actor->state.combat_status >= 5)
	{
		struct prop_datum *prop = prop_get(actor->target.target_prop_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actions.c",
			712,
			actor->target.target_prop_index != NONE);

		if (prop->distance <
				firing_variant->ranged_combat.berserk_firing_range_upper_bound &&
			real_seed_random(get_global_random_seed_address()) <
				definition->berserk.grenade_attached_chance)
		{
			actor->stimuli.berserk_type = MAX(
				actor->stimuli.berserk_type,
				_actor_berserk_attached_projectile);
			result = TRUE;
		}
	}

	return result;
}

boolean actor_action_handle_panic_transition(
	long actor_index,
	short minimum_panic_type,
	boolean communicate_failure,
	short force_panic_type)
{
	struct actor_datum *actor;
	short panic_type;
	short flee_panic_type;
	long current_time;
	boolean force;
	boolean result = FALSE;

	actor = actor_get(actor_index);
	panic_type = actor->stimuli.panic_type;
	result = FALSE;

	if (panic_type >= minimum_panic_type &&
		!actor->input.vehicle_passenger)
	{
		if (actor->state.action == _actor_action_flee &&
			(flee_panic_type = actor->state.action_data.flee.panic_type) >
				_actor_panic_none)
		{
			actor->state.action_data.flee.panic_type =
				MAX(flee_panic_type, panic_type);
			goto done;
		}

		if (actor->emotions.last_flee_failed_time != NONE)
		{
			current_time = game_time_get();
			if (actor->emotions.last_flee_failed_time + 7 >= current_time)
				goto done;
		}

		{
			force =
				actor->stimuli.panic_type >= force_panic_type;
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actions.c",
				661,
				actor->stimuli.panic_prop_index != 0x00000000);

			if (communicate_failure && !force)
			{
				ai_communication_event(
					_ai_communication_attempted_flee,
					actor->meta.unit_index,
					NONE,
					NONE,
					NONE,
					NONE,
					NULL);
				goto done;
			}

			result = actor_action_try_to_panic(
				actor_index,
				actor->stimuli.panic_type,
				actor->stimuli.panic_prop_index,
				force);
		}
	}

done:
	actor->stimuli.panic_type = _actor_panic_none;
	return result;
}

boolean actor_action_handle_active_cover_seeking(
	long actor_index,
	boolean allow_defensive_cover,
	boolean force_flee_transition)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_debug_info *debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean result = FALSE;

	if (actor->meta.timeslice)
	{
		long target_hidden_ticks;

		debug_info->field_B8 = TRUE;
		debug_info->field_BA = 4;

		if (actor->target.target_last_visible_time == NONE)
			target_hidden_ticks = 1000;
		else
			target_hidden_ticks =
				game_time_get() - actor->target.target_last_visible_time;

		debug_info->field_BC = (short)target_hidden_ticks;
		debug_info->field_C0 = actor->input.shield_vitality;

		if (actor->input.shield_vitality <=
			definition->defensive.shield_fraction_hide)
		{
			short action_class = actor_action_class(actor_index);

			debug_info->field_BA = 0;
			if (!actor->emotions.berserk &&
				(action_class == 4 || action_class == 3))
			{
				debug_info->field_BA = 1;
				if (actor->state.combat_status >= 2)
				{
					long current_time = game_time_get();

					debug_info->field_BA = 2;
					if (actor->emotions.last_active_cover_seeking_time == NONE ||
						current_time >=
							actor->emotions.last_active_cover_seeking_time + 30)
					{
						debug_info->field_BA = 3;
						actor->emotions.last_active_cover_seeking_time = current_time;

						if (actor_action_allow_cover_seeking(actor_index, FALSE))
						{
							debug_info->field_BA = 5;
							if (actor_action_try_to_seek_cover(
								actor_index,
								TRUE,
								FALSE))
							{
								debug_info->field_BA = 6;
								result = TRUE;
								goto done;
							}

							if (allow_defensive_cover &&
								actor_action_try_to_panic(
									actor_index,
									4,
									actor->target.target_prop_index,
									force_flee_transition))
							{
								debug_info->field_BA = 7;
								result = TRUE;
							}
						}
					}
				}
			}
		}
	}

done:
	return result;
}

boolean actors_searching_same_position(
	long actor_index,
	long other_actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_datum *other_actor = actor_get(other_actor_index);
	struct pursuit_location *location = actor_get_pursuit_location(actor_index);
	struct pursuit_location *other_location = actor_get_pursuit_location(other_actor_index);
	boolean result = FALSE;

	if (location && other_location)
	{
		if (location->type == _pursuit_location_target &&
			other_location->type == _pursuit_location_target)
		{
			struct prop_datum *prop =
				prop_try_and_get(actor->target.target_prop_index);
			struct prop_datum *other_prop =
				prop_try_and_get(other_actor->target.target_prop_index);

			if (prop && other_prop)
			{
				result = distance_squared3d(
					&prop->body_position,
					&other_prop->body_position) < 0.7f * 0.7f;
			}
		}
		else if (location->type == _pursuit_location_position &&
			other_location->type == _pursuit_location_position)
		{
			result = location->firing_position_index ==
				other_location->firing_position_index;
		}
		else if (location->type == _pursuit_location_undirected &&
			other_location->type == _pursuit_location_undirected)
		{
			result = TRUE;
		}
	}

	return result;
}

long actor_pursuit_find_nearby_actors(
	long actor_index,
	boolean pursuit_controller)
{
	struct actor_datum *actor = actor_get(actor_index);
	long actor_count = 0;
	long best_prop_index = NONE;
	short desired_actor_count = (pursuit_controller != FALSE) + 1;
	real best_distance = REAL_MAX;

	{
		struct prop_iterator prop_iterator;
		struct prop_datum *prop;

		prop_iterator_new(&prop_iterator, actor_index);
		for (prop = prop_iterator_next(&prop_iterator);
			prop;
			prop = prop_iterator_next(&prop_iterator))
		{
			if (!prop->enemy &&
				!prop->dead &&
				prop->actor_index != NONE &&
				(!pursuit_controller ||
					(prop->state >= 2 && prop->state <= 3)) &&
				actor_pursuit_consider_nearby_actor(
					actor_index,
					pursuit_controller,
					prop->actor_index))
			{
				actor_count++;
				if (prop->distance < best_distance)
				{
					best_prop_index = prop_iterator.index;
					best_distance = prop->distance;
				}
			}
		}
	}

	if (actor_count < desired_actor_count && actor->meta.encounter_index != NONE)
	{
		struct encounter_actor_iterator actor_iterator;
		struct actor_datum *friend_actor;

		encounter_actor_iterator_new(
			&actor_iterator,
			actor->meta.encounter_index);
		for (friend_actor = encounter_actor_iterator_next(&actor_iterator);
			friend_actor;
			friend_actor = encounter_actor_iterator_next(&actor_iterator))
		{
			if (friend_actor->meta.unit_index != NONE &&
				actor_pursuit_consider_nearby_actor(
					actor_index,
					pursuit_controller,
					actor_iterator.actor_index))
			{
				long prop_index = prop_get_active_by_unit_index(
					actor_index,
					friend_actor->meta.unit_index);

				if (prop_index == NONE)
				{
					prop_index = prop_get_base_by_unit_index(
						actor_index,
						friend_actor->meta.unit_index,
						TRUE,
						FALSE);
				}

				if (prop_index != NONE)
				{
					real distance;
					real_vector3d displacement;

					actor_count++;
					vector_from_points3d(
						&actor->input.position.body_position,
						&friend_actor->input.position.body_position,
						&displacement);
					distance = square_root(
						displacement.k * displacement.k +
						displacement.j * displacement.j +
						displacement.i * displacement.i);
					if (distance < best_distance)
					{
						best_prop_index = prop_index;
						best_distance = distance;
					}

					if (actor_count >= desired_actor_count)
						break;
				}
			}
		}
	}

	actor->external_orders.pursuit_group_prop_index = best_prop_index;
	return actor_count;
}

void actor_action_change(
	long actor_index,
	long new_action_type,
	struct action_state_data *new_action_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	void (*handler)(
		long actor_index);

	ai_profile.meters[_ai_meter_action_change].accumulator++;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		2947,
		(new_action_type >= 0) && (new_action_type < NUMBER_OF_ACTOR_ACTIONS));

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		2948,
		global_action_functions[new_action_type].action == new_action_type);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		2951,
		(actor->state.action >= 0) &&
			(actor->state.action < NUMBER_OF_ACTOR_ACTIONS));

	handler = global_action_functions[actor->state.action].end;
	if (handler)
		handler(actor_index);

	if (global_action_functions[new_action_type].action_class == 0)
	{
		if (actor->state.mode >= 3)
			actor->state.mode = 2;
	}
	else if (actor->state.mode < 3)
	{
		actor->state.mode = 3;
	}

	actor_clear_discarded_firing_positions(actor_index, FALSE);

	if (global_action_functions[new_action_type].data_size > 0 &&
		new_action_data)
	{
		csmemcpy(
			&actor->state.action_data,
			new_action_data,
			global_action_functions[new_action_type].data_size);
	}

	actor->state.action = new_action_type;
	actor->state.action_changed = TRUE;

	handler = global_action_functions[actor->state.action].begin;
	if (handler)
		handler(actor_index);

	return;
}

boolean actor_action_test_grenade(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	long current_time = game_time_get();
	struct actor_debug_info *debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

	debug_info->grenade_eval_time = current_time;

	if (actor->input.vehicle_index != NONE)
	{
		debug_info->grenade_decision = 0;
		return FALSE;
	}

	if (actor->meta.encounter_index != NONE)
	{
		struct encounter_datum *encounter =
			encounter_get(actor->meta.encounter_index);
		real grenade_timeout =
			variant_definition->grenade_combat.encounter_grenade_timeout;
		real throw_delay = game_difficulty_get_team_value(
			_game_difficulty_value_grenade_timer_scale,
			encounter->team_index) * grenade_timeout;
		long throw_delay_ticks;

		if (actor->external_orders.playfighting)
			throw_delay *= 2.0f;

		throw_delay_ticks = (long)(throw_delay * TICKS_PER_SECOND);
		if (encounter->last_grenade_throw_time != NONE &&
			(short)throw_delay_ticks + encounter->last_grenade_throw_time >
				current_time)
		{
			debug_info->grenade_decision = 5;
			debug_info->grenade_encounter_timeout_ticks =
				(short)((short)(throw_delay_ticks - current_time) +
					(short)encounter->last_grenade_throw_time);
			return FALSE;
		}
	}

	{
	{
		real_point3d grenade_target;
		long grenade_target_prop_index;
		long grenade_ignore_object_index;
		short enemy_count;

		if (actor_combat_find_grenade_target(
			actor_index,
			&grenade_target,
			&grenade_target_prop_index,
			&grenade_ignore_object_index))
		{
			if (actor_combat_check_collateral_damage(
				actor_index,
				variant_definition->grenade_combat.enemy_radius,
				variant_definition->grenade_combat.collateral_damage_radius,
				&grenade_target,
				&enemy_count))
			{
				if (enemy_count >=
					variant_definition->grenade_combat.minimum_enemy_count)
				{
					if (actor_combat_plan_grenade_trajectory(
						actor_index,
						variant_definition->grenade_combat.trajectory_type,
						&grenade_target,
						grenade_target_prop_index,
						grenade_ignore_object_index))
					{
						debug_info->grenade_decision = 10;
						return TRUE;
					}

					debug_info->grenade_decision = 9;
				}
				else
				{
					debug_info->grenade_enemy_count = enemy_count;
					debug_info->grenade_decision = 7;
					debug_info->grenade_required_enemy_count =
						variant_definition->grenade_combat.minimum_enemy_count;
				}
			}
			else
			{
				debug_info->grenade_decision = 8;
			}
		}
		else
		{
			debug_info->grenade_decision = 6;
		}
	}
	}

	return FALSE;
}

boolean actor_action_allow_cover_seeking(
	long actor_index,
	boolean unopposable)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	boolean allow = TRUE;

	if (!unopposable && !actor->external_orders.playfighting)
	{
		if (definition->defensive.hide_target_not_visible_time > 0.0f)
		{
			if (actor->state.combat_status >= 7)
			{
				allow = FALSE;
			}
			else
			{
				short target_not_visible_ticks =
					(short)(definition->defensive.hide_target_not_visible_time *
						TICKS_PER_SECOND);

				if (actor->target.target_last_visible_time != NONE &&
					game_time_get() <
						target_not_visible_ticks +
						actor->target.target_last_visible_time)
				{
					allow = FALSE;
				}
			}
		}

		if (definition->defensive.cover_damage_threshold > 0.0f &&
			actor->input.recent_body_damage <
				definition->defensive.cover_damage_threshold)
		{
			allow = FALSE;
		}
	}

	if (actor->emotions.berserk)
		allow = FALSE;

	if (actor->input.vehicle_passenger)
		allow = FALSE;

	return allow;
}

boolean actor_action_handle_combat_selection(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct actor_variant_definition *firing_variant =
		actor_combat_get_firing_variant_definition(actor_index);
	struct charge_state_data *charge = &actor->state.action_data.charge;
	boolean action_changed = FALSE;
	struct prop_datum *prop = NULL;
	real prop_distance = REAL_MAX;
	real charge_delay;
	struct action_state_data new_action_data;

	if (actor->target.target_prop_index != NONE)
	{
		prop = prop_get(actor->target.target_prop_index);
		prop_distance = prop->distance;

		if (actor->state.action == _actor_action_charge &&
			charge->goal == _charge_goal_stalking)
		{
			boolean discovered = prop->currently_damaging_me ||
				(prop->shooting && prop->quantized_distance <= 1) ||
				(definition->defensive.stalking_discovery_time > 0.0f &&
					charge->stalking_discovery_timer >=
						(short)(definition->defensive.stalking_discovery_time *
							TICKS_PER_SECOND));

			if (discovered)
			{
				if (prop_distance >
					firing_variant->ranged_combat.combat_range_upper_bound)
				{
					action_changed = actor_action_try_to_seek_cover(
						actor_index,
						FALSE,
						FALSE);
				}

				if (!action_changed)
					actor_berserk(actor_index, TRUE);
			}
		}

		if (!actor_has_ranged_weapon(actor_index) ||
			(prop->vehicle_index == NONE && !prop->swarm))
		{
			if (actor->state.action != _actor_action_charge ||
				(charge->goal != _charge_goal_melee &&
					charge->goal != _charge_goal_melee_leaping))
			{
				if (!action_changed && !actor->meta.swarm &&
					actor->input.vehicle_index == NONE &&
					actor->control.fire_state != _actor_fire_state_bursting)
				{
					/* The original validates the controlled unit here even though
					 * this policy decision does not otherwise consume it. */
					struct unit_datum *unit = unit_get(actor->meta.unit_index);
					long current_time = game_time_get();
					boolean use_berserk_range = actor->emotions.berserk;
					real charge_range;
					boolean begin_charge = FALSE;

					(void)unit;

					if (!actor_has_ranged_weapon(actor_index) &&
						!TEST_FLAG(
							definition->flags,
							_actor_definition_stalking_behavior_bit))
					{
						use_berserk_range = TRUE;
					}

					charge_delay = actor->emotions.berserk ?
						0.0f : definition->berserk.melee_attack_delay_timer;
					charge_delay = game_difficulty_get_value(
						_game_difficulty_value_melee_delay_scale) * charge_delay +
						game_difficulty_get_value(
							_game_difficulty_value_melee_delay_base);
					charge_range = use_berserk_range ?
						variant_definition->ranged_combat.berserk_melee_range :
						variant_definition->ranged_combat.melee_range;

					if (actor->emotions.last_melee_check_time == NONE ||
						actor->emotions.last_melee_check_time + 10 <
							current_time)
					{
						if (!(prop_distance > charge_range))
						{
							boolean in_range = TRUE;

							if (actor->external_orders.disable_charging)
							{
								real point_blank_range = MAX(
									0.0f,
									definition->berserk.melee_attack_range);

								if (prop_distance > point_blank_range + 0.8f)
									in_range = FALSE;
							}

							if (in_range)
							{
								long last_charge_time =
									actor->emotions.last_melee_attack_time;

								if (last_charge_time == NONE ||
									!(charge_delay * TICKS_PER_SECOND +
										(real)last_charge_time >=
											(real)current_time))
								{
									actor_has_ranged_weapon(actor_index);
									begin_charge = TRUE;
								}
							}
						}
					}

					if (begin_charge)
					{
						actor->emotions.last_melee_check_time = current_time;

						if (action_charge_setup(
							actor_index,
							_charge_goal_melee,
							&new_action_data.charge))
						{
							actor_action_change(
								actor_index,
								_actor_action_charge,
								&new_action_data);
							action_changed = TRUE;
						}
					}
				}
			}
		}

		if (actor->state.action != _actor_action_charge &&
			!actor->external_orders.disable_charging &&
			!action_changed &&
			actor->input.vehicle_driver_type > _actor_vehicle_driver_none)
		{
			struct unit_datum *vehicle =
				vehicle_get(actor->input.vehicle_index);
			struct vehicle_definition *vehicle_definition =
				vehicle_specific_definition_get(vehicle->definition_index);
			long last_charge_time = actor->emotions.last_vehicle_charge_time;
			boolean repeat_elapsed = FALSE;

			if (last_charge_time == NONE)
			{
				repeat_elapsed = TRUE;
			}
			else if (vehicle_definition->ai_charge_repeat_time *
					TICKS_PER_SECOND + (real)last_charge_time <
				(real)game_time_get())
			{
				repeat_elapsed = TRUE;
			}

			if (repeat_elapsed)
			{
				boolean begin_charge = FALSE;

				if (actor->input.vehicle_driver_type ==
						_actor_vehicle_driver_directional_flying &&
					prop_distance > firing_variant->ranged_combat.melee_range &&
					!prop->line_of_sight)
				{
					begin_charge = TRUE;
				}

				if (begin_charge && action_charge_setup(
					actor_index,
					_charge_goal_vehicle_strafing,
					&new_action_data.charge))
				{
					actor_action_change(
						actor_index,
						_actor_action_charge,
						&new_action_data);
					action_changed = TRUE;
				}
			}
		}
	}

	if (!action_changed)
	{
		boolean desires_charge = actor->emotions.forced_to_charge &&
			!actor->external_orders.disable_charging;
		boolean restart_charge = FALSE;

		if (!actor->external_orders.disable_charging &&
			!actor_has_ranged_weapon(actor_index) &&
			TEST_FLAG(
				definition->flags,
				_actor_definition_charge_in_attacking_mode_bit))
		{
			desires_charge = TRUE;
		}

		if (actor->state.action == _actor_action_charge)
		{
			short goal = charge->goal;

			if (goal == _charge_goal_melee ||
				goal == _charge_goal_melee_leaping)
			{
				if (!charge->finished_melee_attack &&
					!charge->aborted_melee_attack &&
					!charge->unable_to_advance)
				{
					desires_charge = TRUE;
				}
				else
				{
					restart_charge = TRUE;
				}
			}
			else if (actor->external_orders.disable_charging)
			{
				desires_charge = FALSE;
				restart_charge = TRUE;
			}
			else if (goal == _charge_goal_vehicle_strafing ||
				goal == _charge_goal_vehicle_ramming)
			{
				if (charge->unable_to_advance ||
					actor->input.vehicle_driver_type <=
						_actor_vehicle_driver_unknown)
				{
					restart_charge = TRUE;
				}
				else
				{
					desires_charge = TRUE;

					if (goal == _charge_goal_vehicle_strafing)
					{
						struct unit_datum *vehicle =
							vehicle_get(actor->input.vehicle_index);
						struct vehicle_definition *vehicle_definition =
							vehicle_specific_definition_get(vehicle->definition_index);

						if (actor->control.path.at_destination &&
							actor->control.path.destination_orders.destination_type ==
								_destination_prop &&
							actor->control.path.destination_orders.prop.prop_index ==
								actor->target.target_prop_index)
						{
							desires_charge = FALSE;
						}
						else if (prop_distance <
							vehicle_definition->ai_strafing_stop_range)
						{
							desires_charge = FALSE;
						}
						else if (prop_distance <
								vehicle_definition->
									ai_strafing_stop_range * 2.0f &&
							dot_product3d(
								&actor->input.facing_vector,
								&prop->actor_to_prop) < 0.5f)
						{
							desires_charge = FALSE;
						}
					}
				}
			}
		}

		if (desires_charge &&
			(restart_charge ||
				actor->state.action != _actor_action_charge))
		{
			if (action_charge_setup(
				actor_index,
				_charge_goal_close_range,
				&new_action_data.charge))
			{
				actor_action_change(
					actor_index,
					_actor_action_charge,
					&new_action_data);
				action_changed = TRUE;
			}
			else
			{
				desires_charge = FALSE;
			}
		}

		if (!desires_charge &&
			!action_changed &&
			actor->state.action != _actor_action_fight)
		{
			boolean success = action_fight_setup(
				actor_index,
				&new_action_data.fight);

			match_assert(
				"c:\\halo\\SOURCE\\ai\\actions.c",
				2171,
				success);

			actor_action_change(
				actor_index,
				_actor_action_fight,
				&new_action_data);
			action_changed = TRUE;
		}

		else if (!action_changed)
		{
			if (desires_charge)
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actions.c",
					2182,
					actor->state.action == _actor_action_charge);

				if (actor->state.action_data.charge.goal == _charge_goal_melee ||
					actor->state.action_data.charge.goal == _charge_goal_melee_leaping)
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\actions.c",
						2185,
						!actor->state.action_data.charge.finished_melee_attack &&
							!actor->state.action_data.charge.aborted_melee_attack &&
							!actor->state.action_data.charge.unable_to_advance);
				}
				else if (actor->state.action_data.charge.goal ==
						_charge_goal_vehicle_strafing ||
					actor->state.action_data.charge.goal ==
						_charge_goal_vehicle_ramming)
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\actions.c",
						2189,
						!actor->state.action_data.charge.unable_to_advance);
				}
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actions.c",
					2194,
					actor->state.action == _actor_action_fight);
			}
		}
	}

	return action_changed;
}

boolean actor_action_handle_lost_contact(
	long actor_index)
{
	struct path_state search_workspace;
	struct action_state_data action_data;
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct encounter_datum *encounter = actor->meta.encounter_index == NONE ?
		NULL : encounter_get(actor->meta.encounter_index);
	boolean handled = FALSE;
	boolean should_return_to_idle = FALSE;
	boolean wants_postcombat_guard = FALSE;

	if (encounter &&
		encounter->stand_down &&
		actor->state.combat_status <= 2 &&
		!actor->state.artificial_combat_status &&
		!actor->state.suspicion_combat_status)
	{
		should_return_to_idle = TRUE;
	}

	if (actor->external_orders.postcombat_type > 0 &&
		actor->state.combat_status <= 2 &&
		!actor->state.suspicion_combat_status)
	{
		wants_postcombat_guard = TRUE;
	}

	if (actor->state.mode < 3 && !actor_action_class(actor_index))
		handled = TRUE;

	if (!handled &&
		!actor->input.vehicle_passenger &&
		!wants_postcombat_guard &&
		!should_return_to_idle &&
		actor->state.combat_status >= 2)
	{
		struct prop_datum *orphan = actor->target.target_prop_index == NONE ?
			NULL : prop_get(actor->target.target_prop_index);
		boolean pursue_tenacious = FALSE;
		boolean allow_target_uncover = FALSE;
		boolean allow_indefinite_target_uncover = FALSE;
		boolean allow_target_search = FALSE;
		boolean allow_pursuit = FALSE;
		boolean allow_pursuit_search = FALSE;
		boolean wait_after_pursuit = FALSE;

		if (!orphan || !orphan->abandoned_search)
		{
			boolean controlling_group_pursuit;
			boolean group_pursuit_controller;
			boolean controlled_by_group_pursuit;
			short desire_pursuit;
			short desire_pursuit_search;
			short group_pursuit_restriction;
			short desire_target_search;

			desire_target_search =
				actor_type_get_when_to_search_at_target(actor->meta.type);
			desire_pursuit = actor_type_get_when_to_pursue(actor->meta.type);
			desire_pursuit_search =
				actor_type_get_when_to_search_pursuit(actor->meta.type);
			group_pursuit_controller =
				actor_type_get_pursuit_controller(actor->meta.type);
			group_pursuit_restriction = 0;
			controlling_group_pursuit = FALSE;
			controlled_by_group_pursuit = FALSE;

			if (orphan)
			{
				allow_indefinite_target_uncover = TRUE;
				allow_target_uncover = TRUE;
			}

			allow_pursuit = TRUE;
			allow_target_search = TRUE;
			allow_pursuit_search = TRUE;

			if (actor->meta.encounter_index != NONE)
			{
				encounter_modify_pursuit_desires(
					actor->meta.encounter_index,
					actor->meta.squad_index,
					&pursue_tenacious,
					&group_pursuit_restriction,
					&group_pursuit_controller,
					&desire_target_search,
					&desire_pursuit,
					&desire_pursuit_search);

				if (actor->meta.swarm)
				{
					allow_target_search = TRUE;
					allow_indefinite_target_uncover = TRUE;
					allow_target_uncover = TRUE;
					allow_pursuit_search = TRUE;
					allow_pursuit = TRUE;
				}
				else
				{
					encounter_determine_pursuit_availability(
						actor->meta.encounter_index,
						actor_index,
						group_pursuit_restriction,
						group_pursuit_controller,
						&allow_target_uncover,
						&allow_indefinite_target_uncover,
						&allow_target_search,
						&allow_pursuit,
						&allow_pursuit_search,
						&controlling_group_pursuit,
						&controlled_by_group_pursuit,
						&wait_after_pursuit);
				}
			}

			actor_action_determine_pursuit_options(
				actor_index,
				desire_target_search,
				desire_pursuit,
				desire_pursuit_search,
				controlling_group_pursuit,
				controlled_by_group_pursuit,
				should_return_to_idle,
				actor->emotions.forced_to_charge,
				&allow_target_uncover,
				&allow_indefinite_target_uncover,
				&allow_target_search,
				&allow_pursuit,
				&allow_pursuit_search,
				&wait_after_pursuit);
		}

		if (actor->firing_positions.pursuit_prop_index !=
			actor->target.target_prop_index)
		{
			actor->firing_positions.pursuit_positions_count = 0;
			actor->firing_positions.pursuit_prop_index =
				actor->target.target_prop_index;
			actor->firing_positions.pursuit_fired_at_orphan = FALSE;
			actor->firing_positions.pursuit_communicated_lost_contact = FALSE;
		}

		if (allow_target_uncover &&
			action_uncover_setup_target(
				actor_index,
				allow_target_search,
				&action_data.uncover))
		{
			actor_action_change(
				actor_index,
				_actor_action_uncover,
				&action_data);
			handled = TRUE;
		}

		if (!handled)
		{
			actor_perception_tried_to_uncover(
				actor_index,
				actor->target.target_prop_index);

			if (allow_target_search &&
				action_search_setup_target(
					actor_index,
					actor->emotions.forced_to_charge,
					&action_data.search))
			{
				actor_action_change(
					actor_index,
					_actor_action_search,
					&action_data);
				handled = TRUE;
			}

			if (!handled)
			{
				actor_perception_tried_to_search(
					actor_index,
					actor->target.target_prop_index);

				if (actor->firing_positions.pursuit_fired_at_orphan &&
					!actor->firing_positions.pursuit_communicated_lost_contact)
				{
					ai_communication_event(
						13,
						actor->meta.unit_index,
						actor_target_unit_index(actor_index),
						NONE,
						NONE,
						NONE,
						NULL);
					actor->firing_positions.pursuit_communicated_lost_contact = TRUE;
				}

				if (allow_pursuit)
				{
					short firing_position_index = NONE;
					boolean pursuit_from_uncover = FALSE;

					actor->state.searching = TRUE;

					if (actor->meta.swarm)
					{
						if (allow_pursuit_search &&
							action_search_setup_undirected(
								actor_index,
								pursue_tenacious,
								&action_data.search))
						{
							actor_action_change(
								actor_index,
								_actor_action_search,
								&action_data);
							handled = TRUE;
						}
					}
					else
					{
						if (actor->state.action == _actor_action_uncover &&
							allow_pursuit_search &&
							actor->state.action_data.uncover.pursuit_location.type ==
								_pursuit_location_position)
						{
							firing_position_index = actor->state.action_data.uncover.
								pursuit_location.firing_position_index;
							pursuit_from_uncover = TRUE;
						}

						if (firing_position_index == NONE)
						{
							short examined_threshold =
								actor->external_orders.pursuit_group_prop_index == NONE ?
									definition->pursuit.uncoordinated_pursuit_positions :
									definition->pursuit.coordinated_pursuit_positions;

							if (pursue_tenacious ||
								actor->firing_positions.pursuit_prop_index !=
									actor->target.target_prop_index ||
								actor->firing_positions.pursuit_positions_count <
									examined_threshold)
							{
								struct firing_position_evaluation_context search;
								struct firing_position candidate;
								long previous_owner_actor_index;
								boolean position_flags;

								csmemset(&search, 0, sizeof(search));
								search.evaluation_mode = _firing_point_evaluation_mode_pursue;
								search.evaluation_data.pursue.orphan_prop_index =
									actor->target.target_prop_index;
								search.evaluation_data.pursue.last_perceived_time = orphan == NULL ?
									NONE : orphan->last_perceived_time;
								search.evaluation_data.pursue.tenacious = pursue_tenacious;
								search.find_path_direction_from_target =
									actor->target.target_prop_index != NONE;
								search.allowed_position_mask =
									actor_get_firing_position_group(
										actor_index,
										_firing_point_evaluation_mode_pursue,
										_firing_position_group_normal);
								search.maximum_search_range = 20.0f;
								firing_position_index = actor_select_firing_position(
									actor_index,
									&search,
									&candidate,
									&previous_owner_actor_index,
									&search_workspace,
									&position_flags);
							}
						}

						if (firing_position_index != NONE)
						{
							if (!pursuit_from_uncover &&
								action_uncover_setup_pursuit(
									actor_index,
									firing_position_index,
									&action_data.uncover))
							{
								actor_action_change(
									actor_index,
									_actor_action_uncover,
									&action_data);
								handled = TRUE;
							}
							else if (allow_pursuit_search &&
								action_search_setup_pursuit(
									actor_index,
									firing_position_index,
									pursue_tenacious,
									&action_data.search))
							{
								actor_action_change(
									actor_index,
									_actor_action_search,
									&action_data);
								handled = TRUE;
							}
						}
					}

					if (handled)
					{
						long last_perceived_time = orphan == NULL ?
							NONE : orphan->last_perceived_time;

						if (actor->meta.encounter_index != NONE &&
							encounter_mark_examined_pursuit_position(
								actor->meta.encounter_index,
								actor_index,
								firing_position_index,
								last_perceived_time))
						{
							if (actor->firing_positions.pursuit_positions_count == 0)
							{
								ai_communication_event(
									16,
									actor->meta.unit_index,
									NONE,
									NONE,
									NONE,
									NONE,
									NULL);
							}

							actor->firing_positions.pursuit_positions_count++;
						}
					}
				}

				if (!handled)
				{
					if (actor->firing_positions.pursuit_positions_count > 0 &&
						actor->meta.unit_index != NONE)
					{
						ai_communication_event(
							19,
							actor->meta.unit_index,
							NONE,
							NONE,
							NONE,
							NONE,
							NULL);
					}

					if (!actor->meta.swarm &&
						wait_after_pursuit &&
						action_wait_setup(
							actor_index,
							allow_pursuit,
							&action_data.wait))
					{
						actor_action_change(
							actor_index,
							_actor_action_wait,
							&action_data);
						handled = TRUE;
					}
				}
			}
		}
	}

	if (!handled && wants_postcombat_guard)
	{
		if (actor->state.action == _actor_action_guard &&
			actor->state.action_data.guard.post_combat)
		{
			handled = TRUE;
		}
		else if (action_guard_setup_postcombat(actor_index, &action_data.guard))
		{
			actor_action_change(
				actor_index,
				_actor_action_guard,
				&action_data);
			handled = TRUE;
		}
	}

	if (!handled && should_return_to_idle)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actions.c",
			2629,
			encounter);

		handled = actor_action_set_default_state(actor_index, NONE);
	}

	if (!handled && actor_action_class(actor_index) != _action_class_passive)
	{
		long find_position_delay = 0;

		if ((actor->state.action != _actor_action_search ||
			actor->state.action_data.search.search_failed) &&
			actor->state.action != _actor_action_wait)
		{
			find_position_delay = 90;
		}

		actor_perception_abandoned_search(
			actor_index,
			actor->target.target_prop_index);
		{
			boolean success = action_guard_setup_find_position(
				actor_index,
				find_position_delay,
				&action_data.guard);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actions.c",
				2658,
				success);
		}
		actor_action_change(
			actor_index,
			_actor_action_guard,
			&action_data);
		handled = TRUE;
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		2663,
		handled || (actor_action_class(actor_index) == _action_class_passive));

	return handled;
}

boolean actor_action_handle_done_fleeing(
	long actor_index)
{
	struct action_state_data state;
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->state.action == _actor_action_flee &&
		actor->state.action_data.flee.done_fleeing)
	{
		boolean success = action_guard_setup_from_fleeing(
			actor_index,
			&actor->state.action_data.flee,
			&state.guard);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\actions.c",
			2681,
			success);

		actor_action_change(actor_index, _actor_action_guard, &state);
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_combat_status(
	long actor_index,
	boolean allow_initiative,
	boolean force_decision)
{
	struct actor_datum *actor = actor_get(actor_index);
	short action_class = actor_action_class(actor_index);
	boolean result = FALSE;

	if (force_decision)
		allow_initiative = TRUE;

	switch (action_class)
	{
	case 0:
		break;

	case 1:
	case 2:
		if (allow_initiative)
		{
			if (actor->state.combat_status >= 5)
			{
				result = actor_action_handle_combat_selection(actor_index);
			}
			else if (actor->state.combat_status >= 2 ||
				actor->state.action == _actor_action_alert ||
				(actor->state.combat_status == 0 &&
					(actor->external_orders.stand_down ||
						actor->external_orders.postcombat_type > 0)))
			{
				result = actor_action_handle_lost_contact(actor_index);
			}
		}
		break;

	case 4:
		result = actor->state.combat_status >= 4 ?
			actor_action_handle_combat_selection(actor_index) :
			actor_action_handle_lost_contact(actor_index);
		break;

	case 3:
		if (allow_initiative && actor->state.combat_status >= 4)
		{
			result = actor_action_handle_combat_selection(actor_index);
		}
		else if (actor->state.combat_status < 2)
		{
			result = actor_action_handle_lost_contact(actor_index);
		}
		else if (actor->target.target_prop_index != NONE)
		{
			struct prop_datum *prop = prop_get(actor->target.target_prop_index);

			if (actor->target.target_prop_index !=
					actor->firing_positions.pursuit_prop_index ||
				(!prop->tried_to_uncover &&
					(actor->state.action != _actor_action_uncover ||
						actor->state.action_data.search.pursuit_location.type)) ||
				(!prop->tried_to_search &&
					(actor->state.action != _actor_action_uncover ||
						actor->state.action_data.search.pursuit_location.type) &&
					(actor->state.action != _actor_action_search ||
						actor->state.action_data.search.pursuit_location.type)))
			{
				result = actor_action_handle_lost_contact(actor_index);
			}
		}
		break;

	default:
		display_assert(
			NULL,
			"c:\\halo\\SOURCE\\ai\\actions.c",
			2841,
			TRUE);
		system_exit(-1);
		break;
	}

	if (!result && force_decision)
		result = actor_action_handle_lost_contact(actor_index);

	return result;
}

boolean actor_action_handle_combat_failure(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->state.action == _actor_action_charge)
	{
		boolean failed = FALSE;
		short goal = actor->state.action_data.charge.goal;

		if (goal == _charge_goal_melee ||
			goal == _charge_goal_melee_leaping)
		{
			failed = actor->state.action_data.charge.finished_melee_attack ||
				actor->state.action_data.charge.aborted_melee_attack ||
				actor->state.action_data.charge.unable_to_advance;
		}
		else if (goal == _charge_goal_vehicle_strafing ||
			goal == _charge_goal_vehicle_ramming)
		{
			failed = actor->state.action_data.charge.unable_to_advance;
		}

		if (failed)
			result = actor_action_handle_combat_selection(actor_index);
	}

	return result;
}

boolean actor_action_handle_berserk_transition(
	long actor_index,
	short berserk_level)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->stimuli.berserk_type >= berserk_level &&
		!actor->emotions.berserk)
	{
		actor_berserk(actor_index, TRUE);

		if (actor->state.combat_status >= 4)
			result = actor_action_handle_combat_selection(actor_index);
	}

	actor->stimuli.berserk_type = 0;
	return result;
}

boolean actor_action_handle_combat_transition(
	long actor_index)
{
	struct action_state_data state;
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->state.mode < 3 && actor->stimuli.combat_transition)
	{
		actor->state.mode = 3;

		if (action_guard_setup_from_combat_transition(actor_index, &state.guard))
			actor_action_change(actor_index, _actor_action_guard, &state);
		else
			actor_action_handle_combat_selection(actor_index);

		actor->stimuli.combat_transition = 0;
		result = TRUE;
	}
	else if (actor->state.mode == 3 && actor->state.combat_status == 0)
	{
		/* The original call intentionally preserves actor_action_class's
		 * range assertion; its returned class is not otherwise needed here. */
		actor_action_class(actor_index);
	}

	return result;
}

boolean actor_action_handle_exit_pursuit(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean reported = FALSE;

	switch (actor->state.action)
	{
	case _actor_action_uncover:
		if (actor->state.action_data.uncover.uncover_done)
		{
			if (!actor->state.action_data.uncover.pursuit_location.type)
			{
				actor_perception_tried_to_uncover(
					actor_index,
					actor->target.target_prop_index);
			}
			reported = TRUE;
		}
		break;

	case _actor_action_search:
		if (actor->state.action_data.search.search_done)
		{
			if (!actor->state.action_data.search.pursuit_location.type)
			{
				actor_perception_tried_to_search(
					actor_index,
					actor->target.target_prop_index);
			}
			reported = TRUE;
		}
		break;

	case _actor_action_wait:
		if (actor->state.action_data.wait.wait_done)
		{
			actor_perception_abandoned_search(
				actor_index,
				actor->target.target_prop_index);
			reported = TRUE;
		}
		break;
	}

	if (reported)
		reported = actor_action_handle_lost_contact(actor_index);

	return reported;
}

boolean actor_action_handle_grenade_throwing(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	boolean result = FALSE;

	if (actor->target.target_type >= 5 &&
		!(actor->state.action == _actor_action_flee &&
			actor->state.action_data.flee.panic_type > 0))
	{
		struct prop_datum *prop = prop_get(actor->target.target_prop_index);
		long grenade_stimulus =
			variant_definition->grenade_combat.stimulus_type;

		switch (grenade_stimulus)
		{
		case _actor_grenade_stimulus_visible_target:
			if (actor->state.combat_status >= 5)
				result = actor_action_consider_grenade(actor_index);
			break;

		case _actor_grenade_stimulus_seek_cover:
			if (prop->swarm ||
				(actor->state.action == _actor_action_flee &&
					actor->state.action_data.flee.panic_type == _actor_panic_none))
			{
				result = actor_action_consider_grenade(actor_index);
			}
			break;
		}

		if (actor->control.grenade_trying_to_throw)
			actor_action_try_to_throw_grenade(actor_index, FALSE);
	}
	else
	{
		actor->control.grenade_trying_to_throw = FALSE;
	}

	return result;
}

boolean actor_action_try_to_throw_grenade(
	long actor_index,
	boolean known_trajectory)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct unit_datum *unit = unit_get(actor->meta.unit_index);
	boolean result = FALSE;

	if (!unit_is_busy(actor->meta.unit_index) &&
		!(unit->object.current_body_damage > 0.0f))
	{
		if (!known_trajectory && !actor_action_test_grenade(actor_index))
			actor->control.grenade_trying_to_throw = FALSE;

		if (actor->control.grenade_trying_to_throw)
		{
			real_vector2d to_target;

			to_target.i =
				actor->control.grenade_current_target.x -
				actor->input.position.body_position.x;
			to_target.j =
				actor->control.grenade_current_target.y -
				actor->input.position.body_position.y;

			if (normalize2d(&to_target) > 0.0f &&
				!(actor->input.facing_vector.j * to_target.j +
					to_target.i * actor->input.facing_vector.i <
					ACTOR_GRENADE_THROW_MINIMUM_COSINE))
			{
				long encounter_index;

				actor->orders.combat.throw_grenade = TRUE;
				actor->control.grenade_trying_to_throw = FALSE;
				encounter_index = actor->meta.encounter_index;

				if (encounter_index != NONE)
				{
					struct encounter_datum *encounter = encounter_get(encounter_index);
					encounter->last_grenade_throw_time = game_time_get();
				}

				result = TRUE;
			}
		}
	}

	return result;
}

boolean actor_action_consider_grenade(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition const *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	boolean result = FALSE;

	if (actor->control.grenade_trying_to_throw)
	{
		result = TRUE;
	}
	else if (ai_globals->grenades_enabled &&
		variant_definition->grenade_combat.grenade_type != NONE &&
		variant_definition->grenade_combat.trajectory_type != NONE)
	{
		real chance;
		long current_time = game_time_get();
		{
			if (actor->control.grenade_last_check_time == NONE ||
				(real)current_time >=
					(real)actor->control.grenade_last_check_time +
					variant_definition->grenade_combat.throw_grenade_delay *
						ACTOR_GRENADE_TICKS_PER_SECOND)
			{
				chance =
					variant_definition->grenade_combat.throw_grenade_chance;
				chance *= game_difficulty_get_team_value(
					_game_difficulty_value_grenade_chance_scale,
					(word)actor->meta.team_index);

				actor->control.grenade_last_check_time = current_time;

				if (real_seed_random(get_global_random_seed_address()) < chance &&
					actor_action_test_grenade(actor_index))
				{
					actor->control.grenade_trying_to_throw = TRUE;
					actor_action_try_to_throw_grenade(actor_index, TRUE);
					result = TRUE;
				}
			}
		}
	}

	return result;
}

boolean actor_action_try_to_evade(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (actor->input.vehicle_index == NONE &&
		!actor_move_animation_busy(actor_index) &&
		!actor->control.moving &&
		actor->target.target_prop_index != NONE)
	{
		struct actor_definition *definition =
			actor_definition_get(actor->meta.definition_index);
		struct unit_datum *unit = unit_get(actor->meta.unit_index);
		struct unit_definition *unit_definition =
			unit_definition_get(unit->definition_index);
		struct prop_datum *prop = prop_get(actor->target.target_prop_index);

		if (unit_definition->unit.evade_distance > 0.0f)
		{
			real_vector2d const *attractor_vector;
			real dot;

			if (TEST_FLAG(definition->flags, _actor_definition_flying_bit))
			{
				attractor_vector = (real_vector2d const *)&prop->actor_to_prop;
				dot = dot_product3d(
					&prop->actor_to_prop,
					&actor->input.facing_vector);
			}
			else
			{
				attractor_vector = (real_vector2d const *)&prop->actor_to_prop;
				{
				real_vector2d direction = *attractor_vector;

				if (!(normalize2d(&direction) > 0.0f))
					goto try_evade;

				dot = dot_product2d(
					&direction,
					(real_vector2d const *)&actor->input.facing_vector);
				}
			}

			if (!(dot > 0.4f))
				goto result_exit;

		try_evade:
			{
				short evade_direction = _actor_evade_random;
				real_vector2d alignment_vector = *attractor_vector;
				boolean is_ledge;
				byte collision_result[0x1C];

				normalize2d(&alignment_vector);
				if (actor_move_try_evasion_direction(
					actor_index,
					&alignment_vector,
					unit_definition->unit.evade_distance,
					&evade_direction,
					0.0f,
					&is_ledge,
					collision_result))
				{
					long animation;

					if (evade_direction == _actor_evade_right)
						animation = _actor_animation_evade_right;
					else
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\actions.c",
							3299,
							evade_direction == _actor_evade_left);
						animation = _actor_animation_evade_left;
					}

					if (unit_test_animation_impulse(
						actor->meta.unit_index,
						animation))
					{
						result = actor_move_animation_impulse(
							actor_index,
							animation,
							&alignment_vector);
					}
				}
		}
	}
	}

	result_exit:
	return result;
}

boolean actor_action_try_to_dive(
	long actor_index,
	short escape_direction,
	real dive_distance,
	real_vector2d *alignment_vector,
	real maximum_ledge_height)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_debug_info *debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
	boolean dive_off_ledge = FALSE;
	byte collision_result[0x1C];
	real_vector2d evade_vector;
	real_vector2d left_vector;
	real animation_desire[4];
	short best_animation;
	short best_direction;
	real best_score;
	struct actor_dive_animation const *possibility;
	boolean result;

	debug_info->dive_decision_time = game_time_get();

	if (actor->input.vehicle_index == NONE &&
		actor_move_try_evasion_direction(
			actor_index,
			alignment_vector,
			dive_distance,
			&escape_direction,
			maximum_ledge_height,
			&dive_off_ledge,
			collision_result))
	{
		switch (escape_direction)
		{
		case _actor_evade_left:
			set_real_vector2d(
				&evade_vector,
				-alignment_vector->j,
				alignment_vector->i);
			break;

		case _actor_evade_right:
			set_real_vector2d(
				&evade_vector,
				alignment_vector->j,
				-alignment_vector->i);
			break;

		case _actor_evade_forward:
			set_real_vector2d(
				&evade_vector,
				alignment_vector->i,
				alignment_vector->j);
			break;

		case _actor_evade_back:
			set_real_vector2d(
				&evade_vector,
				alignment_vector->i,
				alignment_vector->j);
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\ai\\actions.c",
				3364,
				FALSE,
				NULL);
		}

		best_animation = NONE;
		best_direction = NONE;
		best_score = -0.5f;
		set_real_vector2d(
			&left_vector,
			-actor->input.facing_vector.j,
			actor->input.facing_vector.i);
		animation_desire[2] = dot_product2d(
			&evade_vector,
			(real_vector2d const *)&actor->input.facing_vector);
		animation_desire[0] = dot_product2d(
			&evade_vector,
			&left_vector);
		animation_desire[3] = -animation_desire[2];
		animation_desire[1] = -animation_desire[0];

		possibility = global_dive_animation_table;
		do
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actions.c",
				3390,
				(possibility->animation_direction >= 0) &&
					(possibility->animation_direction < 4));

			if (best_score <
					animation_desire[possibility->animation_direction] +
						possibility->score_bias &&
				unit_test_animation_impulse(
					actor->meta.unit_index,
					possibility->animation))
			{
				best_animation = possibility->animation;
				best_direction = possibility->animation_direction;
				best_score = animation_desire[possibility->animation_direction] +
					possibility->score_bias;
			}

			possibility++;
		}
		while (possibility->animation != NONE);

		if (best_animation == NONE)
		{
			debug_info->dive_decision = _dive_no_animation;
			result = FALSE;
		}
		else
		{
			real_vector2d animation_alignment_vector;

			switch (best_direction)
			{
			case 0:
				set_real_vector2d(
					&animation_alignment_vector,
					evade_vector.j,
					-evade_vector.i);
				break;

			case 1:
				set_real_vector2d(
					&animation_alignment_vector,
					-evade_vector.j,
					evade_vector.i);
				break;

			case 2:
				set_real_vector2d(
					&animation_alignment_vector,
					evade_vector.i,
					evade_vector.j);
				break;

			case 3:
				set_real_vector2d(
					&animation_alignment_vector,
					evade_vector.i,
					evade_vector.j);
				break;

			default:
				match_vassert(
					"c:\\halo\\SOURCE\\ai\\actions.c",
					3433,
					FALSE,
					NULL);
			}

			result = actor_move_animation_impulse(
				actor_index,
				best_animation,
				&animation_alignment_vector);

			if (result)
			{
				ai_communication_event(
					_ai_communication_dive,
					actor->meta.unit_index,
					NONE,
					NONE,
					NONE,
					NONE,
					NULL);
				debug_info->dive_decision = _dive_success;
			}
			else
			{
				debug_info->dive_decision = _dive_animation_failure;
			}
		}

	}
	else
	{
		debug_info->dive_decision = _dive_cannot_move;
		result = FALSE;
	}

	return result;
}

boolean actor_action_try_to_panic(
	long actor_index,
	short panic_type,
	long panic_prop_index,
	boolean force_flee_transition)
{
	struct action_state_data state;
	boolean result = FALSE;

	if (action_flee_setup(
		actor_index,
		panic_type,
		panic_prop_index,
		force_flee_transition,
		FALSE,
		FALSE,
		&state.flee))
	{
		actor_action_change(actor_index, _actor_action_flee, &state);
		result = TRUE;
	}

	return result;
}

boolean actor_action_try_to_enter_vehicle(
	long actor_index,
	long vehicle_index,
	char const *seat_substring_name,
	short seat_desire_type,
	short precomputed_seat_count,
	short *precomputed_seat_indices)
{
	struct actor_datum *actor = actor_get(actor_index);
	short local_seat_indices[16];
	short seat_count;

	if (!precomputed_seat_indices)
	{
		precomputed_seat_indices = local_seat_indices;
		seat_count = vehicle_scripting_find_available_seats(
			vehicle_index,
			seat_substring_name,
			seat_desire_type,
			local_seat_indices,
			NUMBEROF(local_seat_indices));
	}
	else
	{
		seat_count = precomputed_seat_count;
	}

	{
		struct action_state_data state;
		short seat_index_index = 0;

		while (seat_index_index < seat_count)
		{
			short seat_index = (word)precomputed_seat_indices[seat_index_index];

			if (seat_index != NONE &&
				unit_has_animation_to_enter_seat(
					actor->meta.unit_index,
					vehicle_index,
					precomputed_seat_indices[seat_index_index]) &&
				action_vehicle_setup_specific(
					actor_index,
					vehicle_index,
					seat_index,
					&state.vehicle))
			{
				actor_action_change(actor_index, _actor_action_vehicle, &state);
				precomputed_seat_indices[seat_index_index] = NONE;
				return TRUE;
			}

			seat_index_index++;
		}
	}

	return FALSE;
}

static boolean actor_action_allowed_to_enter_vehicle(
	long actor_index,
	long vehicle_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = TRUE;

	if (vehicle_index == actor->emotions.vehicle_ignore_index &&
		game_time_get() < actor->emotions.vehicle_ignore_time)
	{
		result = FALSE;
	}

	return result;
}

boolean actor_action_handle_vehicle_entry(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	long current_time = game_time_get();
	boolean result = FALSE;
	real best_distance_squared;
	long best_vehicle_index;
	real attempt_distance;
	real continue_distance;
	struct action_state_data state;

	if (actor->state.action == _actor_action_flee &&
		actor->state.action_data.flee.panic_type > _actor_panic_none)
	{
		goto result_exit;
	}

	if (actor->state.action == _actor_action_obey)
	{
		goto result_exit;
	}

	if (actor->emotions.last_vehicle_check_time != NONE &&
		actor->emotions.last_vehicle_check_time + 45 >= current_time)
	{
		goto result_exit;
	}

	actor->emotions.last_vehicle_check_time = current_time;
	best_distance_squared = REAL_MAX;
	attempt_distance = REAL_MAX;
	continue_distance = REAL_MAX;
	best_vehicle_index = NONE;

	if (TEST_FLAG(
		definition->flags,
		_actor_definition_helps_players_in_vehicles_bit))
	{
		struct prop_iterator iterator;
		struct prop_datum *prop;

		prop_iterator_new(&iterator, actor_index);
		prop = prop_iterator_next(&iterator);

		while (prop)
		{
			if (prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged &&
				prop->player &&
				!prop->enemy &&
				prop->vehicle_index != NONE &&
				actor_action_allowed_to_enter_vehicle(actor_index, prop->vehicle_index))
			{
				struct unit_datum *vehicle =
					vehicle_try_and_get(prop->vehicle_index);

				if (vehicle &&
					vehicle->unit.driver_object_index == prop->unit_index)
				{
					real_point3d origin;
					real distance_squared;

					object_get_origin(prop->vehicle_index, &origin);
					distance_squared = distance_squared3d(
						&actor->input.position.body_position,
						&origin);

					if (distance_squared < 100.0f &&
						distance_squared < best_distance_squared)
					{
						best_distance_squared = prop->distance * prop->distance;
						best_vehicle_index = prop->vehicle_index;
						attempt_distance = 8.0f;
						continue_distance = 10.0f;
					}
				}
			}

			prop = prop_iterator_next(&iterator);
		}

		if (best_vehicle_index != NONE)
			goto setup_entry;
	}

	if (actor->state.certain_combat_timer < 60)
		return FALSE;

	{
		long slot = 0;

		if (0 >= ai_globals->enterable_vehicle_count)
			return FALSE;

		for (;
			(short)slot < ai_globals->enterable_vehicle_count;
			slot++)
		{
			struct ai_vehicle_enterable *enterable =
				&ai_globals->enterable_vehicles[(short)slot];
			real_point3d origin;
			real distance_squared;
			short team_bitmask;
			short actor_type_bitmask;

			if (!vehicle_try_and_get(enterable->vehicle_index) ||
				!actor_action_allowed_to_enter_vehicle(actor_index, enterable->vehicle_index))
			{
				continue;
			}

			object_get_origin(enterable->vehicle_index, &origin);
			distance_squared = distance_squared3d(
				&origin,
				&actor->input.position.body_position);

			if (!(distance_squared < best_distance_squared) ||
				(enterable->radius != REAL_MAX &&
					distance_squared > enterable->radius * enterable->radius))
			{
				continue;
			}

			team_bitmask = enterable->team_bitmask;
			if (team_bitmask > 0 &&
				(actor->meta.team_index == NONE ||
					!TEST_FLAG(team_bitmask, actor->meta.team_index)))
			{
				continue;
			}

			actor_type_bitmask = enterable->actor_type_bitmask;
			if (actor_type_bitmask > 0 &&
				!TEST_FLAG(actor_type_bitmask, actor->meta.type))
			{
				continue;
			}

			if (enterable->ai_indices_count > 0)
			{
				boolean matched = FALSE;
				long index;

				for (index = 0;
					(short)index < enterable->ai_indices_count;
					index++)
				{
					long ai_index = enterable->ai_indices[(short)index];

					if (ai_index == NONE)
						continue;

					matched =
						((actor->meta.encounter_index ^ ai_index) & 0xFFFF) == 0;

					if (matched)
					{
						switch (AI_INDEX_SCOPE(ai_index))
						{
						case _ai_index_platoon:
							matched = actor->meta.platoon_index ==
								AI_INDEX_SUB_INDEX(ai_index);
							break;

						case _ai_index_squad:
							matched = actor->meta.squad_index ==
								AI_INDEX_SUB_INDEX(ai_index);
							break;
						}
					}

					if (matched)
						break;
				}

				if (!matched)
					continue;
			}

			best_vehicle_index = enterable->vehicle_index;
			best_distance_squared = distance_squared;
			attempt_distance = enterable->radius + 3.0f;
			continue_distance = enterable->radius + 6.0f;
		}
	}

	if (best_vehicle_index == NONE)
		return FALSE;

setup_entry:
	if (action_vehicle_setup_impromptu(
		actor_index,
		best_vehicle_index,
		attempt_distance,
		continue_distance,
		&state.vehicle))
	{
		actor_action_change(actor_index, _actor_action_vehicle, &state);
		result = TRUE;
	}
	else
	{
		return FALSE;
	}

result_exit:
	return result;
}

static boolean actor_action_find_escape_from_danger(
	long actor_index,
	short *escape_direction_reference,
	real *escape_distance_reference,
	real_vector2d *alignment_vector_reference,
	boolean *escape_is_ledge_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct unit_datum *unit = unit_get(actor->meta.unit_index);
	struct unit_definition *unit_definition =
		unit_definition_get(unit->definition_index);
	short escape_direction = NONE;
	boolean result = FALSE;
	boolean selected_is_ledge = FALSE;
	real evade_distance = unit_definition->unit.dive_distance;
	real_vector2d alignment_vector;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actions.c",
		1393,
		actor->danger_zone.danger_type > _actor_danger_zone_none);

	if (evade_distance > 0.0f)
	{
		real_point3d const *body_position;
		real_point3d const *danger_position;
		real ledge_avoidance_distance;
		real_vector3d danger_motion;
		real_vector3d left_vector;
		real_vector3d right_vector;
		real_point3d left_position;
		real_point3d right_position;
		byte collision_result[0x1C];
		boolean left_is_ledge;
		boolean right_is_ledge;
		boolean left_found;
		boolean left_clear;
		boolean right_found;
		boolean right_clear;
		real left_distance;
		real right_distance;

		alignment_vector.i = -actor->danger_zone.velocity.i;
		alignment_vector.j = -actor->danger_zone.velocity.j;
		if (normalize2d(&alignment_vector) < 0.033333335f)
		{
			alignment_vector.i = actor->danger_zone.position.x -
				actor->input.position.body_position.x;
			alignment_vector.j = actor->danger_zone.position.y -
				actor->input.position.body_position.y;

			if (normalize2d(&alignment_vector) == 0.0f)
			{
				alignment_vector.i = actor->input.facing_vector.i;
				alignment_vector.j = actor->input.facing_vector.j;

				if (normalize2d(&alignment_vector) == 0.0f)
					alignment_vector = *global_forward2d;
			}
		}

		ledge_avoidance_distance =
			TEST_FLAG(
				actor_definition_get(actor->meta.definition_index)->flags,
				_actor_definition_dive_off_ledges_bit) ? 8.0f : 0.0f;

		danger_position = &actor->danger_zone.position;
		body_position = &actor->input.position.body_position;
		danger_motion.i = actor->danger_zone.predict_danger_position.x -
			danger_position->x;
		danger_motion.j = actor->danger_zone.predict_danger_position.y -
			danger_position->y;
		danger_motion.k = actor->danger_zone.predict_danger_position.z -
			danger_position->z;
		set_real_vector3d(
			&left_vector,
			-alignment_vector.j,
			alignment_vector.i,
			0.0f);
		right_vector.i = alignment_vector.j;
		right_vector.j = -alignment_vector.i;
		right_vector.k = 0.0f;

		left_position.x = left_vector.i * evade_distance + body_position->x;
		left_position.y = left_vector.j * evade_distance + body_position->y;
		left_position.z = left_vector.k * evade_distance + body_position->z;
		right_position.x = right_vector.i * evade_distance +
			actor->input.position.body_position.x;
		right_position.y = right_vector.j * evade_distance +
			actor->input.position.body_position.y;
		right_position.z = right_vector.k * evade_distance +
			actor->input.position.body_position.z;

		left_found = actor_move_try_evasion_vector(
			actor_index,
			(real_vector2d const *)&left_vector,
			evade_distance,
			ledge_avoidance_distance,
			&left_is_ledge,
			collision_result);
		left_distance = point_to_line_distance3d(
			&left_position,
			danger_position,
			&danger_motion);
		left_clear = left_found &&
			left_distance > actor->danger_zone.danger_radius;

		right_found = actor_move_try_evasion_vector(
			actor_index,
			(real_vector2d const *)&right_vector,
			evade_distance,
			ledge_avoidance_distance,
			&right_is_ledge,
			collision_result);
		right_distance = point_to_line_distance3d(
			&right_position,
			danger_position,
			&danger_motion);
		right_clear = right_found &&
			right_distance > actor->danger_zone.danger_radius;

		if (left_found)
		{
			if (right_found)
			{
				real distance_difference = left_distance - right_distance;

				if (left_is_ledge > right_is_ledge ||
					left_clear > right_clear ||
					distance_difference > 0.3f)
				{
					escape_direction = _actor_evade_left;
					selected_is_ledge = left_is_ledge;
					result = TRUE;
				}
				else if (right_is_ledge > left_is_ledge ||
					right_clear > left_clear ||
					distance_difference < -0.3f)
				{
					escape_direction = _actor_evade_right;
					selected_is_ledge = right_is_ledge;
					result = TRUE;
				}
				else
				{
					escape_direction = _actor_evade_random;
					selected_is_ledge = left_is_ledge;
					result = left_clear;
				}
			}
			else
			{
				escape_direction = _actor_evade_left;
				selected_is_ledge = left_is_ledge;
				result = left_clear;
			}
		}
		else if (right_found)
		{
			escape_direction = _actor_evade_right;
			selected_is_ledge = right_is_ledge;
			result = right_clear;
		}
	}

	/* Original January bug: the diagnostic names all four outputs, but the
	 * condition forgets to validate escape_is_ledge_reference.  Preserve that
	 * behavior; a corrected build would test the fourth pointer too. */
	if (!(escape_direction_reference && escape_distance_reference &&
		alignment_vector_reference))
	{
		display_assert(
			"escape_direction_reference && escape_distance_reference && alignment_vector_reference",
			"c:\\halo\\SOURCE\\ai\\actions.c",
			1484,
			TRUE);
		system_exit(-1);
	}

	/* BUG (preserved for exact matching): alignment_vector is assigned only when the
	 * unit's dive_distance is positive; otherwise it is stored below unassigned (January
	 * 0x40c970 +0x386..+0x38f). Reachable for any unit whose dive distance is not
	 * positive; escape_direction is then NONE, and the only caller,
	 * actor_action_handle_danger_avoidance, leaves that block without reading the vector.
	 * A corrected build should store a defined vector on that path. Source-policy
	 * approval pending (2026-09-27 audit). */
	*escape_direction_reference = escape_direction;
	*escape_distance_reference = evade_distance;
	*escape_is_ledge_reference = selected_is_ledge;
	*alignment_vector_reference = alignment_vector;
	return result;
}

boolean actor_action_handle_vehicle_exit(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean exited = FALSE;

	if (actor->input.vehicle_index != NONE)
	{
		boolean want_exit = FALSE;
		boolean forced_exit = FALSE;
		struct prop_iterator iterator;
		struct prop_datum *prop;

		prop_iterator_new(&iterator, actor_index);

		while (prop = prop_iterator_next(&iterator))
		{
			if (prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged &&
				prop->player &&
				prop->enemy &&
				prop->vehicle_index == actor->input.vehicle_index)
			{
				want_exit = TRUE;
				forced_exit = TRUE;
				break;
			}
		}

		if (actor->stimuli.vehicle_eviction)
			want_exit = TRUE;

		if (actor->input.vehicle_passenger &&
			(actor->input.delayed_attached_projectile_index != NONE ||
				(actor->danger_zone.danger_type == _actor_danger_zone_projectile &&
					actor->danger_zone.attached_to_us)))
		{
			want_exit = TRUE;
			forced_exit = TRUE;
		}

		if (want_exit)
		{
			actor->emotions.last_vehicle_exit_forced = forced_exit;

			if (unit_try_and_exit_seat(actor->meta.unit_index))
			{
				actor->emotions.vehicle_ignore_index = actor->input.vehicle_index;
				actor->emotions.vehicle_ignore_time = game_time_get() + 180;
				exited = TRUE;
			}

			actor->emotions.last_vehicle_exit_forced = FALSE;
		}
	}

	actor->stimuli.vehicle_eviction = FALSE;

	return exited;
}

boolean actor_action_handle_evasion(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition const *variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct actor_definition const *definition =
		actor_definition_get(actor->meta.definition_index);
	long current_time = game_time_get();
	boolean result;
	boolean prop_dangerous;
	boolean not_fleeing;
	real dive_threshold;

	result = FALSE;

	if (actor->emotions.unopposable_retreat_timer > 0 &&
		actor->input.vehicle_index == NONE)
	{
		struct prop_datum *prop =
			prop_get(actor->emotions.unopposable_retreat_prop_index);

		if (prop->unopposable_enemy &&
			(prop->line_of_sight == _ai_line_of_sight_clear ||
				prop->line_of_sight == _ai_line_of_sight_occluded) &&
			(actor->emotions.last_defensive_cover_seeking_time == NONE ||
				actor->emotions.last_defensive_cover_seeking_time + 30 <=
					current_time))
		{
			actor->emotions.last_defensive_cover_seeking_time = current_time;

			if (actor_action_allow_cover_seeking(actor_index, TRUE))
			{
				if (actor_action_try_to_seek_cover(
					actor_index,
					FALSE,
					TRUE))
				{
					result = TRUE;
				}
				else if (TEST_FLAG(
						definition->flags,
						_actor_definition_panicked_by_unopposable_enemy_bit) &&
					actor_action_try_to_panic(
						actor_index,
						_actor_panic_unopposable_enemy,
						actor->emotions.unopposable_retreat_prop_index,
						FALSE))
				{
					result = TRUE;
				}
			}
		}
	}

	if (!result)
	{
		if (!actor->emotions.currently_defending || actor->emotions.berserk)
		{
			dive_threshold =
				definition->defensive.evasion_danger_threshold_attacking;
		}
		else
		{
			dive_threshold =
				definition->defensive.evasion_danger_threshold_defending;
		}

		if (actor->external_orders.playfighting &&
			definition->defensive.evasion_seek_cover_chance > 0.0f &&
			dive_threshold > 1.1f)
		{
			dive_threshold = 1.1f;
		}

		if (actor->emotions.perceived_danger > dive_threshold)
		{
			if (!actor->control.moving)
				real_seed_random(get_global_random_seed_address());

			if (variant_definition->grenade_combat.stimulus_type ==
					_actor_grenade_stimulus_seek_cover &&
				actor_action_consider_grenade(actor_index))
			{
				actor->emotions.perceived_danger = 0.0f;
				result = TRUE;
			}

			prop_dangerous = TRUE;
			not_fleeing = TRUE;

			if (actor->emotions.defensive_crouch &&
				TEST_FLAG(
					definition->flags,
					_actor_definition_crouch_try_not_to_move_bit))
			{
				prop_dangerous = FALSE;

				if (actor->target.target_prop_index != NONE)
				{
					struct prop_datum *prop =
						prop_get(actor->target.target_prop_index);

					if (prop->quantized_facing <= 2 &&
						prop->quantized_distance <= 1)
					{
						prop_dangerous = TRUE;
					}
				}
			}

			if (actor->state.action == _actor_action_charge &&
				(actor->state.action_data.charge.goal == _charge_goal_melee ||
					actor->state.action_data.charge.goal ==
						_charge_goal_melee_leaping))
			{
				not_fleeing = FALSE;
			}

			if (!result &&
				prop_dangerous &&
				(actor->emotions.last_defensive_cover_seeking_time == NONE ||
					actor->emotions.last_defensive_cover_seeking_time + 30 <=
						current_time))
			{
				actor->emotions.last_defensive_cover_seeking_time = current_time;

				if (actor_action_allow_cover_seeking(actor_index, FALSE) &&
					real_seed_random(get_global_random_seed_address()) <
						definition->defensive.evasion_seek_cover_chance &&
					actor_action_try_to_seek_cover(actor_index, FALSE, TRUE))
				{
					ai_communication_event(
						_ai_communication_cover,
						actor->meta.unit_index,
						actor_target_unit_index(actor_index),
						NONE,
						NONE,
						NONE,
						NULL);
					actor->emotions.perceived_danger = 0.0f;
					result = TRUE;
				}
			}

			if (!result &&
				not_fleeing &&
				!actor->emotions.evasion_delay_timer &&
				actor_action_try_to_evade(actor_index))
			{
				actor->emotions.perceived_danger = 0.0f;
				actor->emotions.evasion_delay_timer =
					(short)(definition->defensive.evasion_delay_timer * 30.0f);
				actor->firing_positions.moved_away_from_firing_position = TRUE;
				result = TRUE;
			}
		}
	}

	return result;
}

boolean actor_action_try_to_seek_cover(
	long actor_index,
	boolean cover_from_last_visible_location,
	boolean allow_occluded_points)
{
	struct action_state_data state;
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	if (action_flee_setup(
		actor_index,
		_actor_panic_none,
		actor->target.target_prop_index,
		FALSE,
		cover_from_last_visible_location,
		allow_occluded_points,
		&state.flee))
	{
		actor_action_change(actor_index, _actor_action_flee, &state);
		result = TRUE;
	}

	return result;
}

boolean actor_action_handle_danger_avoidance(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_debug_info *debug_info =
		&actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];
	boolean result = FALSE;
	boolean within_escape_radius = FALSE;
	boolean path_re_enters_danger = FALSE;
	boolean body_within_danger;
	short escape_direction;
	real escape_distance;
	real_vector3d danger_line_delta;

	debug_info->danger_avoidance_time = game_time_get();
	debug_info->danger_decision = _danger_avoidance_none;
	debug_info->dive_decision = _dive_not_attempted;
	debug_info->danger_abandoned_path = FALSE;

	if (!actor->danger_zone.danger_type)
	{
		debug_info->danger_decision = _danger_avoidance_none;
		goto try_avoid_action;
	}

	if (!actor->danger_zone.noticed_danger)
	{
		debug_info->danger_decision = _danger_avoidance_unnoticed;
		goto try_avoid_action;
	}

	if (actor->danger_zone.attached_to_us)
	{
		debug_info->danger_decision = _danger_avoidance_attached_to_us;
		goto try_avoid_action;
	}

	{
		real_point3d const *danger_reference =
			&actor->danger_zone.bounding_sphere_center;
		real_point3d const *body_position =
			&actor->input.position.body_position;
		real reference_x =
			danger_reference->x -
			body_position->x;
		real reference_y =
			danger_reference->y -
			body_position->y;
		real reference_z =
			danger_reference->z -
			body_position->z;
		real reference_radius = actor->danger_zone.bounding_sphere_radius + 3.0f;

		if (reference_y * reference_y +
			(reference_z * reference_z + reference_x * reference_x) >
			reference_radius * reference_radius)
		{
			debug_info->danger_decision = _danger_avoidance_far_away;
			debug_info->danger_far_dist = distance3d(
				body_position,
				danger_reference);
			debug_info->danger_far_radius =
				actor->danger_zone.bounding_sphere_radius + 3.0f;
			goto try_avoid_action;
		}
	}

	if (actor_move_animation_busy(actor_index))
	{
		debug_info->danger_decision = _danger_avoidance_animation_busy;
		goto try_avoid_action;
	}

	if (actor->input.vehicle_passenger)
	{
		debug_info->danger_decision = _danger_avoidance_vehicle;
		goto try_avoid_action;
	}

	vector_from_points3d(
		&actor->danger_zone.position,
		&actor->danger_zone.predict_danger_position,
		&danger_line_delta);

	{
		real body_line_distance_squared = point_to_line_distance_squared3d(
			&actor->input.position.body_position,
			&actor->danger_zone.position,
			&danger_line_delta);
		boolean destination_within_danger;

		{
			real danger_radius = actor->danger_zone.danger_radius;

			body_within_danger =
				body_line_distance_squared < danger_radius * danger_radius;
		}
		within_escape_radius = body_line_distance_squared <
			(actor->danger_zone.danger_radius + 3.5f) *
			(actor->danger_zone.danger_radius + 3.5f);

		if (actor_path_has_path(actor_index))
		{
			real destination_line_distance_squared =
				point_to_line_distance_squared3d(
				&actor->control.path.path.endpoint.point,
				&actor->danger_zone.position,
				&danger_line_delta);
			real danger_radius = actor->danger_zone.danger_radius;

			destination_within_danger =
				destination_line_distance_squared <
					danger_radius * danger_radius;

			if (actor->control.moving &&
				!body_within_danger &&
				!destination_within_danger)
			{
				real_vector3d path_direction_scaled;
				real path_line_distance_squared;
				real danger_radius;

				path_direction_scaled.i =
					actor->control.moving_towards_vector.i * 3.0f;
				path_direction_scaled.j =
					actor->control.moving_towards_vector.j * 3.0f;
				path_direction_scaled.k =
					actor->control.moving_towards_vector.k * 3.0f;
				path_line_distance_squared = vector_to_line_distance_squared3d(
					&actor->input.position.body_position,
					&path_direction_scaled,
					&actor->danger_zone.position,
					&danger_line_delta);
				danger_radius = actor->danger_zone.danger_radius;
				path_re_enters_danger =
					path_line_distance_squared < danger_radius * danger_radius;
			}
		}
		else
		{
			destination_within_danger = body_within_danger;
			path_re_enters_danger = FALSE;
		}

		if (destination_within_danger ||
			(path_re_enters_danger && !body_within_danger))
		{
			short firing_position_index =
				actor->firing_positions.current_position_index;

			if (firing_position_index != NONE)
			{
				actor_discard_firing_position(
					actor_index,
					firing_position_index,
					TRUE);
				debug_info->danger_abandoned_path = TRUE;
			}
		}

		if (body_within_danger)
		{
			boolean reaction_is_urgent = FALSE;
			real urgency = fast_vector_intersection_with_sphere(
				&actor->danger_zone.position,
				&danger_line_delta,
				&actor->input.position.body_position,
				actor->danger_zone.danger_radius);

			if (urgency < REAL_MAX)
				urgency *= 45.0f;

			switch (actor->danger_zone.danger_type)
			{
			case _actor_danger_zone_suicide:
				if (urgency == 0.0f &&
					actor->danger_zone.suicide.time_until_death != NONE &&
					actor->danger_zone.suicide.time_until_death < 30)
				{
					reaction_is_urgent = TRUE;
				}
				break;

			case _actor_danger_zone_projectile:
				if (urgency == 0.0f &&
					actor->danger_zone.projectile.time_until_explosion != NONE &&
					actor->danger_zone.projectile.time_until_explosion < 20)
				{
					reaction_is_urgent = TRUE;
				}
				break;

			case _actor_danger_zone_vehicle:
				if (urgency < 30.0f)
					reaction_is_urgent = TRUE;
				break;
			}

			if ((!actor->danger_zone.hostility || reaction_is_urgent) &&
				!actor->danger_zone.communicated)
			{
				long hostility = NONE;

				switch (actor->danger_zone.hostility)
				{
				case _actor_danger_hostility_enemy:
					hostility = _comm_hostility_enemy;
					break;
				case _actor_danger_hostility_friend:
					hostility = _comm_hostility_friend;
					break;
				case _actor_danger_hostility_self:
					hostility = _comm_hostility_self;
					break;
				}

				if (actor->danger_zone.danger_type ==
					_actor_danger_zone_projectile)
				{
					ai_communication_event(
						_ai_communication_grenade_danger,
						actor->meta.unit_index,
						NONE,
						hostility,
						NONE,
						NONE,
						NULL);
				}

				actor->danger_zone.communicated = TRUE;
			}

			{
				real_vector2d alignment_vector;
				boolean escape_found;
				boolean escape_is_ledge;

				escape_found = actor_action_find_escape_from_danger(
					actor_index,
					&escape_direction,
					&escape_distance,
					&alignment_vector,
					&escape_is_ledge);

				if (escape_direction == NONE)
				{
					debug_info->danger_decision =
						_danger_avoidance_no_safe_direction;
					goto try_avoid_action;
				}

				if (actor->danger_zone.allow_dive_evasion &&
					actor->input.vehicle_index == NONE)
				{
					boolean should_dive = FALSE;

					switch (actor->danger_zone.danger_type)
					{
					case _actor_danger_zone_suicide:
						if ((escape_found || escape_is_ledge) && urgency == 0.0f)
						{
							debug_info->danger_decision =
								_danger_avoidance_can_avoid;
							should_dive = TRUE;
						}
						else if (reaction_is_urgent)
						{
							debug_info->danger_decision =
								_danger_avoidance_imminent_explosion;
							should_dive = TRUE;
						}
						break;

					case _actor_danger_zone_projectile:
						if ((escape_found || escape_is_ledge) && urgency < 7.0f)
						{
							debug_info->danger_decision =
								_danger_avoidance_can_avoid;
							should_dive = TRUE;
						}
						else if (reaction_is_urgent)
						{
							debug_info->danger_decision =
								_danger_avoidance_imminent_explosion;
							should_dive = TRUE;
						}
						break;

					case _actor_danger_zone_vehicle:
						if (reaction_is_urgent)
						{
							debug_info->danger_decision =
								_danger_avoidance_imminent_impact;
							should_dive = TRUE;
						}
						break;
					}

					if (should_dive)
					{
						struct actor_definition *definition =
							actor_definition_get(actor->meta.definition_index);
						real ledge_avoidance_distance =
							TEST_FLAG(
								definition->flags,
								_actor_definition_dive_off_ledges_bit) ?
								8.0f : 0.0f;

						result = actor_action_try_to_dive(
							actor_index,
							escape_direction,
							escape_distance,
							&alignment_vector,
							ledge_avoidance_distance);
					}
					else
					{
						debug_info->danger_decision = _danger_avoidance_no_desire;
						debug_info->danger_intersect_time = urgency;
					}
				}
				else
				{
					debug_info->danger_decision = _danger_avoidance_evasion_disallowed;
				}
			}
		}
		else
		{
			debug_info->danger_decision = _danger_avoidance_outside_zone;
			debug_info->danger_zone_dist =
				square_root(body_line_distance_squared);
			debug_info->danger_zone_radius = actor->danger_zone.danger_radius;
		}
	}

try_avoid_action:
	if (!result && within_escape_radius &&
		(actor_path_at_destination(actor_index) ||
			!actor->control.moving || path_re_enters_danger) &&
		!actor->emotions.forced_to_charge)
	{
		struct action_state_data action_data;
		short action_class = actor_action_class(actor_index);

		if ((action_class == _action_class_passive ||
				action_class == _action_class_pursuit) &&
			action_avoid_setup(actor_index, &action_data.avoid))
		{
			actor_action_change(actor_index, _actor_action_avoid, &action_data);
			result = TRUE;
		}
	}

	return result;
}

/* ---------- private code */
