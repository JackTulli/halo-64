/*
ACTOR_PERCEPTION.C

symbols in this file:
0001D8C0 00d0:
	_actor_perception_acknowledge (0000)
0001D990 00f0:
	_actor_get_perception_knowledge (0000)
0001DA80 0140:
	_actor_get_vision_distances (0000)
0001DBC0 0040:
	_code_0001dbc0 (0000)
0001DC00 00f0:
	_code_0001dc00 (0000)
0001DCF0 0230:
	_actor_perception_desire_prop (0000)
0001DF20 00a0:
	_actor_perception_find_prop_pathfinding_location (0000)
0001DFC0 00c0:
	_actor_perception_find_killer_prop_index (0000)
0001E080 0010:
	_arctangent (0000)
0001E090 00f0:
	_actor_perception_find_recent_damaging_prop_index (0000)
0001E180 0050:
	_actor_perception_forget_recent_damage (0000)
0001E1D0 0060:
	_actor_perception_retreat_successful (0000)
0001E230 00f0:
	_actor_compute_prop_unopposable (0000)
0001E320 03a0:
	_actor_compute_prop_target_weight (0000)
0001E6C0 0200:
	_actor_situation_update_target_status (0000)
0001E8C0 0140:
	_actor_situation_combat_status_update (0000)
0001EA00 04f0:
	_actor_situation_update (0000)
0001EEF0 00f0:
	_actor_situation_try_new_target (0000)
0001EFE0 01b0:
	_actor_perception_friend_prop_is_attacking (0000)
0001F190 0190:
	_actor_perception_aiming_vector_test_blockage (0000)
0001F320 0150:
	_actor_emotion_flee_with_friends (0000)
0001F470 0080:
	_code_0001f470 (0000)
0001F4F0 0070:
	_code_0001f4f0 (0000)
0001F560 04f0:
	_code_0001f560 (0000)
0001FA50 00b0:
	_actor_berserk (0000)
0001FB00 0360:
	_actor_visibility_at_point (0000)
0001FE60 0240:
	_actor_audibility_at_point (0000)
000200A0 0170:
	_actor_perception_find_sense_position (0000)
00020210 01f0:
	_code_00020210 (0000)
00020400 0380:
	_prop_position_refresh (0000)
00020780 0210:
	_code_00020780 (0000)
00020990 05c0:
	_actor_perception_refresh_danger_zone (0000)
00020F50 0180:
	_actor_expected_acknowledgement (0000)
000210D0 0090:
	_actor_perception_unreachable (0000)
00021160 0060:
	_actor_perception_tried_to_uncover (0000)
000211C0 0060:
	_actor_perception_tried_to_search (0000)
00021220 00a0:
	_actor_perception_abandoned_search (0000)
000212C0 0680:
	_actor_emotion_update (0000)
00021940 0110:
	_actor_perception_become_acknowledged (0000)
00021A50 0e60:
	_prop_status_refresh (0000)
000228B0 06d0:
	_code_000228b0 (0000)
00022F80 0310:
	_actor_perception_create_orphan_from_friend (0000)
00023290 0970:
	_code_00023290 (0000)
00023C00 1270:
	_actor_perception_update (0000)
00245AB8 0038:
	_global_combat_status_table (0000)
	_global_acknowledgement_speeds (0018)
00245AF0 0020:
	??_C@_0CA@OIEKNKJL@prop?9?$DOorphan_prop_index?5?$DN?$DN?5NONE?$AA@ (0000)
00245B10 0018:
	??_C@_0BI@EKGDDPPJ@prop_acknowledged?$CIprop?$CJ?$AA@ (0000)
00245B28 0027:
	??_C@_0CH@HJCPLECH@prop?9?$DOowner_actor_index?5?$DN?$DN?5actor@ (0000)
00245B50 0025:
	??_C@_0CF@BOOBPIOF@c?3?2halo?2SOURCE?2ai?2actor_percepti@ (0000)
00245B78 0004:
	__real@42100000 (0000)
00245B7C 0004:
	__real@43610000 (0000)
00245B80 0004:
	__real@44c80000 (0000)
00245B84 0022:
	??_C@_0CC@ECCIDOIG@damaging_prop_index?5?$CB?$DN?50x0000000@ (0000)
00245BA8 0014:
	??_C@_0BE@JCINMMJG@prop_orphaned?$CIprop?$CJ?$AA@ (0000)
00245BBC 0013:
	??_C@_0BD@GACNHPJP@target_prop?9?$DOenemy?$AA@ (0000)
00245BD0 005f:
	??_C@_0FP@LFCAOKBD@?$CIactor?9?$DOtarget?4target_type?5?$DO?$DN?50?$CJ@ (0000)
00245C30 003a:
	??_C@_0DK@BNBOMGMC@?$CIactor_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIactor_type@ (0000)
00245C6C 0010:
	??_C@_0BA@NODDFKHB@new_prop?9?$DOenemy?$AA@ (0000)
00245C80 004c:
	??_C@_0EM@HGNPMJBO@prop_acknowledged?$CIfriend_prop?$CJ?5?$CG@ (0000)
00245CCC 0004:
	__real@bf4ccccd (0000)
00245CD0 0004:
	__real@3f9ae148 (0000)
00245CD4 0004:
	__real@3eb851ec (0000)
00245CD8 0004:
	__real@bf490fdb (0000)
00245CDC 0004:
	__real@3f060a92 (0000)
00245CE0 0004:
	__real@3e19999a (0000)
00245CE4 0004:
	__real@3fd55555 (0000)
00245CE8 0004:
	__real@40010204 (0000)
00245CEC 0004:
	__real@3ee66666 (0000)
00245CF0 0018:
	??_C@_0BI@GICPJHMO@best_unit_index?5?$CB?$DN?5NONE?$AA@ (0000)
00245D08 0025:
	??_C@_0CF@GHOKMPCH@actor?9?$DOmeta?4swarm_unit_index?5?$CB?$DN?5@ (0000)
00245D30 0021:
	??_C@_0CB@MPKGJDOK@actor?9?$DOmeta?4swarm_unit_count?5?$DO?50@ (0000)
00245D54 003b:
	??_C@_0DL@MCFLLIBG@?$CIexisting_unit_index?5?$DN?$DN?5NONE?$CJ?5?$HM?$HM@ (0000)
00245D90 0004:
	__real@40100000 (0000)
00245D94 0018:
	??_C@_0BI@LJGDFJPJ@swarm_actor?9?$DOmeta?4swarm?$AA@ (0000)
00245DAC 0004:
	__real@3a91a2b4 (0000)
00245DB0 002f:
	??_C@_0CP@MPBEBNC@object?9?$DOobject?4type?5?$DN?$DN?5_object_t@ (0000)
00245DE0 0004:
	__real@383a69dc (0000)
00245DE8 00c6:
	??_C@_0MG@IKFAABEC@?$CIactor?9?$DOdanger_zone?4danger_type?5@ (0000)
00245EB0 0008:
	__real@3ff8000000000000 (0000)
00245EB8 0015:
	??_C@_0BF@NEEODENG@?$CBprop_orphaned?$CIprop?$CJ?$AA@ (0000)
00245ED0 0008:
	__real@bfa7a8d000000000 (0000)
00245ED8 0004:
	__real@3f680347 (0000)
00245EDC 0004:
	__real@3f7e147b (0000)
00245EE0 0004:
	__real@bb5a740e (0000)
00245EE4 0004:
	__real@bc888889 (0000)
00245EE8 0004:
	__real@bd088889 (0000)
00245EEC 0004:
	__real@3c888889 (0000)
00245EF0 0004:
	__real@3b5a740e (0000)
00245EF4 0006:
	??_C@_05KKADFBMH@?$CBdead?$AA@ (0000)
00245EFC 0038:
	??_C@_0DI@GEDMEBOM@current_orphan?9?$DOparent_prop_inde@ (0000)
00245F34 0038:
	??_C@_0DI@CJNILOBC@current_prop?9?$DOorphan_prop_index?5@ (0000)
00245F6C 0031:
	??_C@_0DB@FLDFMNOK@current_orphan?9?$DOowner_actor_inde@ (0000)
00245FA0 002f:
	??_C@_0CP@JODIIBNI@current_prop?9?$DOowner_actor_index?5@ (0000)
00245FD0 001e:
	??_C@_0BO@PHJOMEBD@prop_orphaned?$CIcurrent_orphan?$CJ?$AA@ (0000)
00245FF0 0022:
	??_C@_0CC@OGFEAFJH@prop_unacknowledged?$CIcurrent_prop@ (0000)
00246014 003c:
	??_C@_0DM@LBOGCEIF@actor_perception_refresh?5overflo@ (0000)
00246050 0008:
	??_C@_07BJLCCHPO@friends?$AA@ (0000)
00246058 0008:
	??_C@_07OMOEAPJD@enemies?$AA@ (0000)
00246060 0004:
	__real@3f31c71c (0000)
00246064 000c:
	??_C@_0M@JMHOJBDN@?$CBprop?9?$DOdead?$AA@ (0000)
00246070 0020:
	??_C@_0CA@JNPJKEOJ@prop?9?$DOparent_prop_index?5?$CB?$DN?5NONE?$AA@ (0000)
00246090 0017:
	??_C@_0BH@IAHDHPJP@new_state?$CB?$DNprop?9?$DOstate?$AA@ (0000)
002460A8 0012:
	??_C@_0BC@COANDKJL@?$CFs?3?5become?5aware?$CB?$AA@ (0000)
002460C0 0041:
	??_C@_0EB@HOGHONEE@?5?5awareness?5delta?3?5?$CF?42f?5?$CIcurrent@ (0000)
00246104 002b:
	??_C@_0CL@DNKONGGM@?$CFs?3?5knowledge?5?$CFs?5percep?5?$CFs?5?9?$DO?5aw@ (0000)
00246130 000d:
	??_C@_0N@OCHNCJBI@unmistakable?$AA@ (0000)
00246140 0005:
	??_C@_04PLMLMMEO@full?$AA@ (0000)
00246148 0008:
	??_C@_07JHIHCBKH@partial?$AA@ (0000)
00246150 0009:
	??_C@_08GMLBJMKA@definite?$AA@ (0000)
0024615C 000a:
	??_C@_09HJLAOOGF@searching?$AA@ (0000)
00246168 0008:
	??_C@_07IFJEFPGA@instant?$AA@ (0000)
00246170 0006:
	??_C@_05DAFACFLE@never?$AA@ (0000)
00246178 0051:
	??_C@_0FB@GKMFLAFD@?$CIprop?9?$DOperception?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIprop@ (0000)
002461D0 004c:
	??_C@_0EM@LBLHCEME@?$CIknowledge_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIknowle@ (0000)
0024621C 0018:
	??_C@_0BI@PALENGAG@?$CFs?3?5stop?5becoming?5aware?$AA@ (0000)
00246234 001a:
	??_C@_0BK@OHGMLFML@?$CFs?3?5start?5to?5become?5aware?$AA@ (0000)
00246250 0024:
	??_C@_0CE@DMMCLDJG@?$CBrefresh_status?5?$HM?$HM?5refresh_posit@ (0000)
00246274 0031:
	??_C@_0DB@OOOKPDMG@parent_prop?9?$DOorphan_prop_index?5?$DN@ (0000)
002462A8 002b:
	??_C@_0CL@GMGBPAA@prop?9?$DOunopposable_casualties_inf@ (0000)
002B6AE0 0004:
	_data_002b6ae0 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"

#include "actions.h"
#include "actor_definitions.h"
#include "actor_types.h"
#include "actors.h"
#include "ai.h"
#include "ai_communication.h"
#include "ai_debug.h"
#include "ai_profile.h"
#include "encounters.h"
#include "game/game.h"
#include "items/projectile_definitions.h"
#include "items/projectiles.h"
#include "items/weapon_definitions.h"
#include "math/integer_math.h"
#include "props.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/unit_definitions.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"
#include "units/biped_definitions.h"

/* ---------- constants */

enum
{
	_actor_mode_asleep = 1,
	_actor_mode_combat = 3,
};

/*
 * TU-local copy: no shared header owns the actor knowledge domain. January's
 * actor_perception_update prints the names "noncombat", "guard", "searching"
 * and "definite" and asserts NUMBER_OF_ACTOR_KNOWLEDGE_TYPES.
 */
enum
{
	_actor_knowledge_noncombat = 0,
	_actor_knowledge_guard,
	_actor_knowledge_searching,
	_actor_knowledge_definite,
	NUMBER_OF_ACTOR_KNOWLEDGE_TYPES,
};

/* TU-local copy: complete copies also exist in actors.c, ai.c and ai_script.c. */
enum
{
	_ai_unit_effect_bump = 0,
	_ai_unit_effect_shooting,
	_ai_unit_effect_death_scream,
	_ai_unit_effect_magic_sight,
	NUMBER_OF_AI_UNIT_EFFECTS,
};

/* TU-local copy of actor_external_orders.desired_target_type; also in ai_script.c. */
enum
{
	_desired_target_none = 0,
	_desired_target_ai,
	_desired_target_player,
};

/*
 * TU-local copy of the ai reference scope stored in the top two bits of an ai
 * reference; also in ai_script.c (enum ai_reference_type) and actions.c.
 */
enum
{
	_ai_reference_type_encounter = 0,
	_ai_reference_type_platoon,
	_ai_reference_type_squad,
	NUMBER_OF_AI_REFERENCE_TYPES,
};

/*
 * January's acknowledgement speed classes, indexed from
 * global_acknowledgement_speeds; actor_perception_update prints their names.
 */
enum
{
	_awareness_speed_never = 0,
	_awareness_speed_noncombat,
	_awareness_speed_guard,
	_awareness_speed_combat,
	_awareness_speed_instant,
};

/* TU-local copy: also in actors.c, action_obey.c and actor_combat.c. */
enum
{
	_actor_fire_target_none = 0,
	_actor_fire_target_prop,
};

/*
 * TU-local copy of the ai_information_data union selector (ai.h owns the
 * union); ai_communication.c carries a partial copy.
 */
enum
{
	_ai_information_none = 0,
	_ai_information_allegiance,
	_ai_information_combat_stimulus,
	_ai_information_target_knowledge,
};

enum
{
	_actor_combat_status_none = 0,
	_actor_combat_status_investigate = 2,
	_actor_combat_status_definite = 3,
	_actor_combat_status_certain = 4,
	_actor_combat_status_visible = 7,
};

enum
{
	_defensive_crouch_none = 0,
	_defensive_crouch_danger,
	_defensive_crouch_shield_low,
	_defensive_crouch_hide_behind_shield,
	_defensive_crouch_any_target,
	_defensive_crouch_flood_shamble,
};

/* ---------- macros */

/*
 * January's Actor Perception names these shared HCEX actor slots
 * differently.  Keep the aliases typed through actor_datum rather than
 * overlaying the datum with an incompatible structure type.
 */
#define actor_perception_preferred_target_prop_index(actor) \
	((actor)->meta.interesting_orphan_index)

#define actor_perception_target_weight_combat_status(actor) \
	((actor)->state.combat_status)

#define actor_perception_audibility_combat_status(actor) \
	((actor)->state.mode)

/* INFERRED FROM JANUARY'S BYTES. This macro is not attested in any surviving
 * header or source; it is reconstructed because January's object requires the
 * canonical macro expansion ((a) * (a)) at the actor_perception_refresh call
 * site, and no simpler spelling of the square reaches it. Measured, with the
 * rest of the function held constant:
 *
 *     prop->distance * prop->distance          residual
 *     (prop->distance) * (prop->distance)      residual
 *     (prop->distance * prop->distance)        residual
 *     bind a local first, (distance * distance)  residual
 *     ((prop->distance) * (prop->distance))    EXACT
 *
 * A hand-written expression does not produce the doubly-parenthesised form;
 * a correctly written macro produces it inevitably, so the byte evidence is
 * itself the argument that a macro stood here. It is kept translation-unit
 * private and named to match its sibling below.
 *
 * actor_emotion_unopposable_retreat's friend-target square is a second use,
 * and its evidence is WEAKER than at the site above. Measured with the rest
 * of that function held constant, d being friend_target_prop->distance:
 *
 *     d * d                                    residual (size 1280)
 *     (d * d)                                  residual (size 1280)
 *     (d) * (d)                                EXACT
 *     ((d) * (d))   this macro's expansion     EXACT
 *
 * Parenthesised operands are what January requires. Unlike the site above,
 * the hand-written single-parenthesis form ALSO matches here, so this site on
 * its own does not prove that a macro stood there. The macro is used here by
 * owner ruling (2026-09-20) because it is the admitted spelling of this
 * square. Mechanism: written as d * d, VC7 computes the product ahead of the
 * target->count increment and folds the minimum-distance compare into an
 * indexed operand; with parenthesised operands it places the increment first
 * and compares through the bound target pointer, as January does.
 */
#define actor_perception_distance_squared(distance) \
	((distance) * (distance))

#define actor_perception_distance_squared2d(a, b, delta_x, delta_y) \
	((delta_x) = (b)->x - (a)->x, \
		(delta_y) = (b)->y - (a)->y, \
		(delta_x) * (delta_x) + (delta_y) * (delta_y))

/* ---------- structures */

struct actor_perception_ai_debug_state_view
{
	byte __unknown000[6];
	boolean perception_ignore_player_props;
	boolean perception_blind_player_props;
	byte __unknown008[3];
	boolean perception_blind_all_props;
	boolean perception_deaf_all_props;
	byte __unknown00D[0x94];
	boolean perception_trace_player_awareness;
};

struct actor_perception_debug_info_view
{
	byte __unknown0000[0x6578];
	short perception_awareness_speed;
	short evaluation_mode;
};

#define actor_perception_ai_debug_get() \
	((struct actor_perception_ai_debug_state_view *)&ai_debug)

#define actor_perception_debug_get(index) \
	((struct actor_perception_debug_info_view *)&actor_debug_array[AI_DEBUG_ACTOR_SLOT((index))])

#define actor_perception_object_get(index) \
	((struct object_datum *)object_get_and_verify_type( \
		(index), _object_mask_all))

/*
 * January actor/prop fields whose HCEX-derived shared structure positions do
 * not agree with this executable. Keep the executable-specific view local
 * until the complete January layouts are recovered.
 */
struct actor_danger_zone_view
{
	short danger_type;
	short hostility;
	short acknowledgement_timer;
	boolean currently_perceived;
	boolean noticed_danger;
	boolean allow_dive_evasion;
	boolean communicated;
	boolean attached_to_us;
	byte __unknown00B;
	long object_index;
	long owner_unit_index;
	real danger_radius;
	real_point3d initial_position;
	real_vector3d initial_velocity;
	real_point3d position;
	real_vector3d velocity;
	real_point3d predict_danger_position;
	real current_distance_from_actor;
	real bounding_sphere_radius;
	real_point3d bounding_sphere_center;
	short predicted_impact_ticks;
	byte __unknown06A[2];
};

struct actor_perception_actor_view
{
	byte __unknown000[4];
	short type;
	boolean swarm;
	boolean frozen;
	boolean active;
	byte __unknown009[0xA];
	boolean dormant;
	byte __unknown014[4];
	long unit_index;
	byte __unknown01C[8];
	long swarm_unit_index;
	long swarm_cache_index;
	byte __unknown02C[8];
	long encounter_index;
	byte __unknown038[2];
	short encounter_squad_index;
	short encounter_platoon_index;
	short team;
	byte __unknown040[0x14];
	long preferred_target_prop_index;
	long definition_index;
	long variant_definition_index;
	byte __unknown060[0xA];
	short combat_status;
	short friend_state;
	short artificial_combat_status;
	short suspicion_combat_status;
	short target_combat_status;
	short transient_combat_status;
	long transient_combat_status_time;
	long combat_status_timer;
	long certain_combat_status_timer;
	long combat_status_decay_timer;
	long uncertain_combat_status_timer;
	boolean combat_status_high;
	byte __unknown08D[0x1B];
	short friend_fighting_count;
	byte __unknown0AA[0x9A];
	long body_leaf_index;
	short body_cluster_index;
	byte __unknown14A[0xE];
	long vehicle_index;
	boolean in_midair;
	boolean underwater;
	byte __unknown15E[3];
	boolean flying;
	boolean long_orphan_inspection;
	byte __unknown163[0x69];
	boolean corpse_interest_inhibited;
	byte __unknown1CD[7];
	short danger_relationship_type;
	byte __unknown1D6[2];
	union
	{
		long danger_relationship_handle;

		struct
		{
			byte __unknown1D8[2];
			byte danger_relationship_variant;
			byte __unknown1DB;
		};
	};
	byte __unknown1DC[0x10];
	char nearby_fighting_friend_count;
	boolean searching;
	byte __unknown1EE[0x14];
	boolean vehicle_passenger;
	byte __unknown203[0x65];
	union
	{
		short target_type;
		struct
	{
		short target_type;
	} target;
	};
	byte __unknown26A[2];
	long target_last_visible_time;
	long target_prop_index;
	byte __unknown274[8];
	boolean target_outside_active_area;
	byte __unknown27D[3];
	struct actor_danger_zone_view danger_zone;
	byte __unknown2EC[0x1C];
	short active_threat_count;
	byte __unknown30A[0x40];
	short pending_combat_status;
	long pending_combat_status_time;
	byte __unknown350[0x27];
	boolean sighted_friendly_player;
	boolean berserk;
	byte __unknown379[0x27];
	long corpse_ignore_time;
	byte __unknown3A4[4];
	short unopposable_retreat_timer;
	byte __unknown3AA[2];
	long unopposable_retreat_prop_index;
};

struct actor_perception_prop_view
{
	byte __unknown000[4];
	long owner_actor_index;
	byte __unknown008[4];
	long orphan_prop_index;
	short type;
	short team_index;
	boolean swarm;
	byte __unknown015[3];
	long unit_index;
	long actor_index;
	real suicide_radius;
	short state;
	short timer;
	long swarm_unit_selected_time;
	real awareness;
	short perception_result;
	short visibility_result;
	short audibility_result;
	short ineffability_result;
	short line_of_sight_result;
	short orphan_lifespan_ticks;
	short orphan_inspection_ticks;
	byte __unknown03E[2];
	real_vector3d orphan_hint_vector;
	short ticks_until_orphan;
	boolean orphan_corpse_cheated;
	byte __unknown04F;
	real target_weight;
	real look_interest;
	real last_idle_look_interest;
	long last_idle_look_time;
	boolean enemy;
	boolean ally;
	boolean ally_status_changed;
	boolean in_use;
	boolean refresh_stimuli;
	byte __unknown065;
	short perception;
	short perception_decay_ticks;
	short required_ticks;
	short ticks_since_damage;
	byte __unknown06E[2];
	real damage_inflicted_on_me;
	boolean currently_damaging_me;
	byte __unknown075;
	short dead_ticks;
	short visible_ticks;
	byte __unknown07A[2];
	long last_perceived_time;
	real_point3d last_perceived_body_position;
	long last_visible_time;
	real_point3d last_visible_head_position;
	short unreachable_ticks;
	byte __unknown09E[2];
	long last_unreachable_time;
	boolean unopposable_enemy;
	byte __unknown0A5[1];
	short unopposable_casualties_inflicted;
	short unopposable_casualty_decay_timer;
	short unopposable_trigger_hysteresis;
	short unopposable_trigger_timer;
	short unopposable_trigger_threshold;
	short ticks_since_definitely_located;
	byte __unknown0B2[2];
	long definite_knowledge_source_actor;
	boolean definitely_located;
	boolean tried_to_uncover;
	boolean tried_to_search;
	boolean abandoned_search;
	real_point3d body_position;
	real_point3d center_of_mass;
	real_vector3d velocity;
	real_vector3d actor_to_prop;
	long pathfinding_surface_index;
	real_point3d pathfinding_point;
	struct location body_location;
	real_point3d head_position;
	long vehicle_index;
	long attached_to_unit_index;
	boolean underwater;
	byte __unknown119[3];
	real distance;
	char lighting;
	char quantized_distance;
	char quantized_facing;
	char quantized_speed;
	char quantized_closing_speed;
	char child_units_attached;
	boolean delay_requirement_decision;
	boolean dead;
	boolean really_dead;
	boolean just_killed;
	boolean just_became_visible;
	boolean fighting;
	boolean in_combat;
	boolean noncombat;
	boolean player;
	boolean shooting;
	boolean flying;
	boolean active_camouflage;
	boolean flashlight;
	boolean ignore;
	boolean preferred_target;
	boolean vehicle_gunner;
	boolean dangerous_vehicle_driver;
};

struct actor_perception_encounter_view
{
	byte __unknown000[0x40];
	boolean force_blind;
	boolean deaf;
	boolean blind;
	byte __unknown043[1];
	boolean stand_down;
	boolean enemy_target;
	byte __unknown046[0xA];
	long postcombat_timer;
	byte __unknown054[4];
	long corpse_ignore_time;
};

struct actor_perception_unit_view
{
	byte __unknown000[0x3D0];
	short parent_seat_index;
};

struct actor_perception_target_unit_view
{
	byte __unknown000[0xB6];
	byte active_region_flags;
};

struct actor_perception_status_unit_view
{
	byte __unknown000[0x1B4];
	unsigned long status_flags;
};

struct actor_target_weight_weapon_definition_view
{
	byte __unknown000[0x40C];
	real minimum_target_range;
};

struct actor_perception_projectile_datum_view
{
	byte __unknown000[0x1DC];
	byte attachment_flags;
};

struct actor_perception_vehicle_definition_view
{
	byte __unknown000[4];
	real bounding_radius;
	byte __unknown008[0x2E8];
	byte danger_zone_flags;
};

struct actor_perception_projectile_view
{
	byte __unknown000[0x1F0];
	real time_remaining;
	real arming_rate;
};

struct actor_perception_communication_context
{
	short source_team;
	short destination_team;
	boolean enemy;
};

struct actor_perception_responsible_unit_view
{
	byte __unknown000[0x68];
	short team;
};

struct actor_orphan_prop_view
{
	byte __unknown000[4];
	long owner_actor_index;
	long next_prop_index;
	long related_prop_index;
	short type;
	short team_index;
	boolean swarm;
	byte __unknown015[3];
	long unit_index;
	long actor_index;
	byte __unknown020[4];
	short state;
	byte __unknown026[0x16];
	short orphan_inspection_ticks;
	byte __unknown03E[0x12];
	real target_weight;
	byte __unknown054[0x50];
	boolean unopposable_enemy;
	byte __unknown0A5[0xB];
	short ticks_since_definitely_located;
	byte __unknown0B2[2];
	long definite_knowledge_source_actor;
	boolean definitely_located;
};

struct actor_perception_refresh_entry
{
	long unit_index;
	long prop_index;
	real priority;
};

/*
 * code_00023290 maintains separate enemy and friend candidate lists on its
 * stack. Each list has two 16-bit counters followed by 128 12-byte entries.
 */
struct actor_perception_refresh_list
{
	short accepted_count;
	short entry_count;
	struct actor_perception_refresh_entry entries[128];
};

struct actor_perception_refresh_locals
{
	struct object_cluster_iterator cluster_iterator;
	short dead_ticks;
	byte __unknown00A[2];
	real suicide_radius;
	struct actor_perception_actor_view *actor;
	short visible_ticks;
	byte __unknown016[2];
	long unit_index;
	struct prop_iterator iterator;
	struct actor_perception_actor_view *current_actor;

	union
	{
		struct structure_bsp *structure_bsp;
		short target_count;
	} bsp_or_target;

	boolean in_use;
	byte __unknown02D[3];
	boolean dead;
	byte __unknown031[3];
	unsigned long *cluster_pvs;
};

/* These two make the out-of-bounds read in actor_emotion_update analysable
 * rather than merely undefined: specific_threats holds exactly
 * NUMBER_OF_ACTOR_THREAT_TYPES elements, and cumulative_threats begins
 * immediately after it, so specific_threats[NUMBER_OF_ACTOR_THREAT_TYPES] is
 * cumulative_threats[_actor_threat_none] and nothing else. If either ever stops
 * holding, that read stops being harmless and this file stops compiling. */
typedef char actor_perception_specific_threats_size_assert[
	sizeof(((struct actor_situation *)0)->specific_threats) ==
		NUMBER_OF_ACTOR_THREAT_TYPES ? 1 : -1];
typedef char actor_perception_threat_arrays_adjacent_assert[
	offsetof(struct actor_situation, cumulative_threats) ==
		offsetof(struct actor_situation, specific_threats) +
			NUMBER_OF_ACTOR_THREAT_TYPES ? 1 : -1];

typedef char actor_perception_actor_view_target_prop_index_offset_assert[
	offsetof(struct actor_perception_actor_view, target_prop_index) == 0x270 ? 1 : -1];
typedef char actor_perception_actor_view_team_offset_assert[
	offsetof(struct actor_perception_actor_view, team) == 0x3E ? 1 : -1];
typedef char actor_perception_actor_view_definition_index_offset_assert[
	offsetof(struct actor_perception_actor_view, definition_index) == 0x58 ? 1 : -1];
typedef char actor_perception_actor_view_preferred_prop_offset_assert[
	offsetof(struct actor_perception_actor_view, preferred_target_prop_index) == 0x54 ? 1 : -1];
typedef char actor_perception_actor_view_variant_definition_offset_assert[
	offsetof(struct actor_perception_actor_view, variant_definition_index) == 0x5C ? 1 : -1];
typedef char actor_perception_actor_view_underwater_offset_assert[
	offsetof(struct actor_perception_actor_view, underwater) == 0x15D ? 1 : -1];
typedef char actor_perception_actor_view_berserk_offset_assert[
	offsetof(struct actor_perception_actor_view, berserk) == 0x378 ? 1 : -1];
typedef char actor_perception_actor_view_orphan_inspection_offset_assert[
	offsetof(struct actor_perception_actor_view, long_orphan_inspection) == 0x162 ? 1 : -1];
typedef char actor_perception_actor_view_sighted_friend_offset_assert[
	offsetof(struct actor_perception_actor_view, sighted_friendly_player) == 0x377 ? 1 : -1];
typedef char actor_perception_actor_view_danger_zone_offset_assert[
	offsetof(struct actor_perception_actor_view, danger_zone) == 0x280 ? 1 : -1];
typedef char actor_perception_actor_view_active_offset_assert[
	offsetof(struct actor_perception_actor_view, active) == 0x8 ? 1 : -1];
typedef char actor_perception_actor_view_dormant_offset_assert[
	offsetof(struct actor_perception_actor_view, dormant) == 0x13 ? 1 : -1];
typedef char actor_perception_actor_view_encounter_index_offset_assert[
	offsetof(struct actor_perception_actor_view, encounter_index) == 0x34 ? 1 : -1];
typedef char actor_perception_actor_view_swarm_unit_index_offset_assert[
	offsetof(struct actor_perception_actor_view, swarm_unit_index) == 0x24 ? 1 : -1];
typedef char actor_perception_actor_view_swarm_cache_index_offset_assert[
	offsetof(struct actor_perception_actor_view, swarm_cache_index) == 0x28 ? 1 : -1];
typedef char actor_perception_actor_view_body_cluster_index_offset_assert[
	offsetof(struct actor_perception_actor_view, body_cluster_index) == 0x148 ? 1 : -1];
typedef char actor_perception_actor_view_vehicle_index_offset_assert[
	offsetof(struct actor_perception_actor_view, vehicle_index) == 0x158 ? 1 : -1];
typedef char actor_perception_actor_view_corpse_interest_inhibited_offset_assert[
	offsetof(struct actor_perception_actor_view, corpse_interest_inhibited) == 0x1CC ? 1 : -1];
typedef char actor_perception_actor_view_corpse_ignore_time_offset_assert[
	offsetof(struct actor_perception_actor_view, corpse_ignore_time) == 0x3A0 ? 1 : -1];
typedef char actor_perception_prop_view_unopposable_enemy_offset_assert[
	offsetof(struct actor_perception_prop_view, unopposable_enemy) == 0xA4 ? 1 : -1];
typedef char actor_perception_actor_view_active_threat_count_offset_assert[
	offsetof(struct actor_perception_actor_view, active_threat_count) == 0x308 ? 1 : -1];
typedef char actor_perception_actor_view_target_combat_status_offset_assert[
	offsetof(struct actor_perception_actor_view, target_combat_status) == 0x72 ? 1 : -1];
typedef char actor_perception_actor_view_pending_combat_status_offset_assert[
	offsetof(struct actor_perception_actor_view, pending_combat_status) == 0x34A ? 1 : -1];
typedef char actor_perception_actor_view_nearby_fighting_friend_count_offset_assert[
	offsetof(struct actor_perception_actor_view, nearby_fighting_friend_count) == 0x1EC ? 1 : -1];
typedef char actor_perception_prop_view_in_combat_offset_assert[
	offsetof(struct actor_perception_prop_view, in_combat) == 0x12C ? 1 : -1];
typedef char actor_perception_prop_swarm_offset_assert[
	offsetof(struct prop_datum, swarm) == 0x14 ? 1 : -1];
typedef char actor_perception_prop_unit_index_offset_assert[
	offsetof(struct prop_datum, unit_index) == 0x18 ? 1 : -1];
typedef char actor_perception_prop_actor_index_offset_assert[
	offsetof(struct prop_datum, actor_index) == 0x1C ? 1 : -1];
typedef char actor_perception_prop_enemy_offset_assert[
	offsetof(struct prop_datum, enemy) == 0x60 ? 1 : -1];
typedef char actor_perception_prop_body_position_offset_assert[
	offsetof(struct prop_datum, body_position) == 0xBC ? 1 : -1];
typedef char actor_perception_prop_dead_offset_assert[
	offsetof(struct prop_datum, dead) == 0x127 ? 1 : -1];
typedef char actor_perception_prop_view_vehicle_gunner_offset_assert[
	offsetof(struct actor_perception_prop_view, vehicle_gunner) == 0x135 ? 1 : -1];
typedef char actor_perception_prop_view_player_offset_assert[
	offsetof(struct actor_perception_prop_view, player) == 0x12E ? 1 : -1];
typedef char actor_perception_prop_view_perception_result_offset_assert[
	offsetof(struct actor_perception_prop_view, perception_result) == 0x30 ? 1 : -1];
typedef char actor_perception_prop_view_dangerous_vehicle_driver_offset_assert[
	offsetof(struct actor_perception_prop_view, dangerous_vehicle_driver) == 0x136 ? 1 : -1];
typedef char actor_perception_status_unit_flags_offset_assert[
	offsetof(struct actor_perception_status_unit_view, status_flags) == 0x1B4 ? 1 : -1];
typedef char actor_perception_target_unit_active_region_offset_assert[
	offsetof(struct actor_perception_target_unit_view, active_region_flags) == 0xB6 ? 1 : -1];
typedef char actor_target_weight_definition_melee_velocity_offset_assert[
	offsetof(struct actor_definition, berserk.melee_leap_velocity) == 0x38C ? 1 : -1];
typedef char actor_target_weight_variant_melee_range_offset_assert[
	offsetof(struct actor_variant_definition, ranged_combat.melee_range) == 0x160 ? 1 : -1];
typedef char actor_target_weight_variant_berserk_melee_range_offset_assert[
	offsetof(struct actor_variant_definition, ranged_combat.berserk_melee_range) == 0x170 ? 1 : -1];
typedef char actor_target_weight_firing_maximum_range_offset_assert[
	offsetof(struct actor_variant_definition, ranged_combat.maximum_firing_range) == 0x74 ? 1 : -1];
typedef char actor_target_weight_firing_combat_range_offset_assert[
	offsetof(struct actor_variant_definition, ranged_combat.combat_range_upper_bound) == 0xA0 ? 1 : -1];
typedef char actor_target_weight_weapon_minimum_range_offset_assert[
	offsetof(struct actor_target_weight_weapon_definition_view, minimum_target_range) == 0x40C ? 1 : -1];
typedef char actor_perception_preferred_target_prop_index_offset_assert[
	offsetof(struct actor_datum, meta.interesting_orphan_index) == 0x54 ? 1 : -1];
typedef char actor_perception_audibility_combat_status_offset_assert[
	offsetof(struct actor_datum, state.mode) == 0x6A ? 1 : -1];
typedef char actor_perception_target_weight_combat_status_offset_assert[
	offsetof(struct actor_datum, state.combat_status) == 0x6E ? 1 : -1];
typedef char actor_emotion_actor_unit_offset_assert[
	offsetof(struct actor_datum, meta.unit_index) == 0x18 ? 1 : -1];
typedef char actor_emotion_actor_definition_offset_assert[
	offsetof(struct actor_datum, meta.definition_index) == 0x58 ? 1 : -1];
typedef char actor_emotion_actor_combat_status_offset_assert[
	offsetof(struct actor_datum, state.combat_status) == 0x6E ? 1 : -1];
typedef char actor_emotion_actor_body_position_offset_assert[
	offsetof(struct actor_datum, input.position.body_position) == 0x12C ? 1 : -1];
typedef char actor_emotion_actor_body_vitality_offset_assert[
	offsetof(struct actor_datum, input.body_vitality) == 0x1B8 ? 1 : -1];
typedef char actor_emotion_actor_external_orders_offset_assert[
	offsetof(struct actor_datum, external_orders) == 0x1C8 ? 1 : -1];
typedef char actor_emotion_actor_target_offset_assert[
	offsetof(struct actor_datum, target.target_type) == 0x268 ? 1 : -1];
typedef char actor_emotion_actor_emotions_offset_assert[
	offsetof(struct actor_datum, emotions) == 0x350 ? 1 : -1];
typedef char actor_emotion_actor_firing_position_offset_assert[
	offsetof(struct actor_datum, firing_positions.current_position_index) == 0x3B8 ? 1 : -1];
typedef char actor_emotion_actor_control_moving_offset_assert[
	offsetof(struct actor_datum, control.moving) == 0x504 ? 1 : -1];
typedef char actor_emotion_actor_control_vector_offset_assert[
	offsetof(struct actor_datum, control.moving_towards_vector) == 0x518 ? 1 : -1];
typedef char actor_emotion_definition_crouch_type_offset_assert[
	offsetof(struct actor_definition, defensive.defensive_crouch_type) == 0x2F8 ? 1 : -1];
typedef char actor_emotion_definition_attacking_threshold_offset_assert[
	offsetof(struct actor_definition, defensive.defensive_threshold_attacking) == 0x2FC ? 1 : -1];
typedef char actor_emotion_definition_defending_threshold_offset_assert[
	offsetof(struct actor_definition, defensive.defensive_threshold_defending) == 0x300 ? 1 : -1];
typedef char actor_emotion_definition_minimum_stand_offset_assert[
	offsetof(struct actor_definition, defensive.defensive_crouch_min_stand_time) == 0x304 ? 1 : -1];
typedef char actor_emotion_definition_minimum_crouch_offset_assert[
	offsetof(struct actor_definition, defensive.defensive_crouch_min_crouch_time) == 0x308 ? 1 : -1];
typedef char actor_perception_ai_debug_ignore_players_offset_assert[
	offsetof(struct actor_perception_ai_debug_state_view, perception_ignore_player_props) == 6 ? 1 : -1];
typedef char actor_perception_ai_debug_blind_players_offset_assert[
	offsetof(struct actor_perception_ai_debug_state_view, perception_blind_player_props) == 7 ? 1 : -1];
typedef char actor_perception_ai_debug_blind_all_offset_assert[
	offsetof(struct actor_perception_ai_debug_state_view, perception_blind_all_props) == 11 ? 1 : -1];
typedef char actor_perception_ai_debug_deaf_all_offset_assert[
	offsetof(struct actor_perception_ai_debug_state_view, perception_deaf_all_props) == 12 ? 1 : -1];
typedef char actor_perception_ai_debug_trace_offset_assert[
	offsetof(struct actor_perception_ai_debug_state_view, perception_trace_player_awareness) == 0xA1 ? 1 : -1];
typedef char actor_perception_debug_awareness_speed_offset_assert[
	offsetof(struct actor_perception_debug_info_view, perception_awareness_speed) == 0x6578 ? 1 : -1];
typedef char actor_perception_projectile_attachment_flags_offset_assert[
	offsetof(struct actor_perception_projectile_datum_view, attachment_flags) == 0x1DC ? 1 : -1];
typedef char actor_perception_source_unit_sound_offset_assert[
	offsetof(struct unit_definition, unit.constant_sound) == 0x182 ? 1 : -1];
typedef char actor_perception_source_actor_target_offset_assert[
	offsetof(struct actor_datum, target.target_type) == 0x268 ? 1 : -1];
typedef char actor_perception_source_actor_shooting_offset_assert[
	offsetof(struct actor_datum, orders.combat.shoot_at_target) == 0x454 ? 1 : -1];
typedef char actor_perception_actor_weapon_range_offset_assert[
	offsetof(struct actor_datum, control.weapon_maximum_range) == 0x608 ? 1 : -1];
typedef char actor_perception_definition_melee_range_offset_assert[
	offsetof(struct actor_definition, berserk.melee_attack_range) == 0x37C ? 1 : -1];
typedef char actor_perception_vehicle_definition_radius_offset_assert[
	offsetof(struct actor_perception_vehicle_definition_view, bounding_radius) == 4 ? 1 : -1];
typedef char actor_perception_vehicle_definition_danger_zone_offset_assert[
	offsetof(struct actor_perception_vehicle_definition_view, danger_zone_flags) == 0x2F0 ? 1 : -1];
typedef char actor_perception_responsible_unit_team_offset_assert[
	offsetof(struct actor_perception_responsible_unit_view, team) == 0x68 ? 1 : -1];
typedef char actor_perception_encounter_view_blind_offset_assert[
	offsetof(struct actor_perception_encounter_view, blind) == 0x42 ? 1 : -1];
typedef char actor_perception_encounter_view_stand_down_offset_assert[
	offsetof(struct actor_perception_encounter_view, stand_down) == 0x44 ? 1 : -1];
typedef char actor_perception_encounter_view_enemy_target_offset_assert[
	offsetof(struct actor_perception_encounter_view, enemy_target) == 0x45 ? 1 : -1];
typedef char actor_perception_encounter_view_postcombat_timer_offset_assert[
	offsetof(struct actor_perception_encounter_view, postcombat_timer) == 0x50 ? 1 : -1];
typedef char actor_perception_encounter_view_corpse_ignore_time_offset_assert[
	offsetof(struct actor_perception_encounter_view, corpse_ignore_time) == 0x58 ? 1 : -1];
typedef char actor_visibility_variant_modified_vision_range_offset_assert[
	offsetof(struct actor_variant_definition, ranged_combat.modified_vision_range) == 0x150 ? 1 : -1];
typedef char actor_visibility_debug_info_size_assert[
	sizeof(struct actor_debug_info) == 0x657C ? 1 : -1];
typedef char actor_visibility_debug_info_last_time_offset_assert[
	offsetof(struct actor_debug_info, vision_last_time) == 0x656C ? 1 : -1];
typedef char actor_orphan_prop_view_related_prop_index_offset_assert[
	offsetof(struct actor_orphan_prop_view, related_prop_index) == 0xC ? 1 : -1];
typedef char actor_orphan_prop_view_orphan_inspection_ticks_offset_assert[
	offsetof(struct actor_orphan_prop_view, orphan_inspection_ticks) == 0x3C ? 1 : -1];
typedef char actor_orphan_prop_view_target_weight_offset_assert[
	offsetof(struct actor_orphan_prop_view, target_weight) == 0x50 ? 1 : -1];
typedef char actor_orphan_prop_view_unopposable_enemy_offset_assert[
	offsetof(struct actor_orphan_prop_view, unopposable_enemy) == 0xA4 ? 1 : -1];
typedef char actor_orphan_prop_view_definite_source_offset_assert[
	offsetof(struct actor_orphan_prop_view, definite_knowledge_source_actor) == 0xB4 ? 1 : -1];
typedef char actor_orphan_prop_view_definitely_located_offset_assert[
	offsetof(struct actor_orphan_prop_view, definitely_located) == 0xB8 ? 1 : -1];
typedef char actor_danger_zone_view_size_assert[
	sizeof(struct actor_danger_zone_view) == 0x6C ? 1 : -1];
typedef char actor_danger_zone_view_object_index_offset_assert[
	offsetof(struct actor_danger_zone_view, object_index) == 0xC ? 1 : -1];
typedef char actor_danger_zone_view_danger_radius_offset_assert[
	offsetof(struct actor_danger_zone_view, danger_radius) == 0x14 ? 1 : -1];
typedef char actor_danger_zone_view_initial_position_offset_assert[
	offsetof(struct actor_danger_zone_view, initial_position) == 0x18 ? 1 : -1];
typedef char actor_danger_zone_view_initial_velocity_offset_assert[
	offsetof(struct actor_danger_zone_view, initial_velocity) == 0x24 ? 1 : -1];
typedef char actor_danger_zone_view_position_offset_assert[
	offsetof(struct actor_danger_zone_view, position) == 0x30 ? 1 : -1];
typedef char actor_danger_zone_view_projected_position_offset_assert[
	offsetof(struct actor_danger_zone_view, predict_danger_position) == 0x48 ? 1 : -1];
typedef char actor_danger_zone_view_distance_offset_assert[
	offsetof(struct actor_danger_zone_view, current_distance_from_actor) == 0x54 ? 1 : -1];
typedef char actor_danger_zone_view_midpoint_offset_assert[
	offsetof(struct actor_danger_zone_view, bounding_sphere_center) == 0x5C ? 1 : -1];
typedef char actor_danger_zone_view_impact_ticks_offset_assert[
	offsetof(struct actor_danger_zone_view, predicted_impact_ticks) == 0x68 ? 1 : -1];
typedef char actor_perception_refresh_entry_size_assert[
	sizeof(struct actor_perception_refresh_entry) == 0xC ? 1 : -1];
typedef char actor_perception_refresh_list_entries_offset_assert[
	offsetof(struct actor_perception_refresh_list, entries) == 4 ? 1 : -1];
typedef char actor_perception_refresh_list_size_assert[
	sizeof(struct actor_perception_refresh_list) == 0x604 ? 1 : -1];

/*
 * Runtime perception values in the January actor definition. The shared
 * HCEX-derived definition still labels two of these slots as unused.
 */
struct actor_perception_definition_view
{
	unsigned long flags;
	byte __unknown004[0x18];
	real maximum_vision_angle;
	real central_vision_angle;
	byte __unknown024[4];
	real peripheral_distance;
	real maximum_peripheral_distance;
};

/* ---------- prototypes */

static long actor_perception_qsort_compare_optional_props(
	void const *a,
	void const *b);

void actor_situation_update_target_status(
	long actor_index);

void actor_situation_combat_status_update(
	long actor_index);

real actor_compute_prop_target_weight(
	long actor_index,
	long prop_index);

boolean actor_compute_prop_unopposable(
	long actor_index,
	long prop_index);

long ai_get_responsible_unit(
	long object_index,
	boolean include_self);

long vehicle_find_pathfinding_surface_index(
	long vehicle_index,
	real_point3d *position);

long biped_find_pathfinding_surface_index(
	long biped_index,
	real_point3d *position);

boolean actor_attacking_target(
	long actor_index,
	real_vector3d *attack_vector);

void actor_stimulus_prop_sighted(
	long actor_index,
	long prop_index,
	boolean initial_acknowledgement);

static long actor_perception_unit_from_swarm(
	long swarm_actor_index,
	long actor_index,
	long existing_unit_index,
	boolean mark,
	struct actor_position_data const *position);

static boolean actor_perception_assess_vehicle_danger(
	long actor_index,
	long vehicle_index,
	boolean mark,
	struct actor_position_data const *position);

static void actor_perception_refresh(
	long actor_index);

static void actor_perception_refresh_test_object(
	long actor_index,
	long object_index,
	struct actor_perception_refresh_list *friend_list,
	struct actor_perception_refresh_list *enemy_list);

void actor_input_sample_position(
	long actor_index,
	long unit_index,
	struct actor_position_data *position);

boolean vehicle_causes_collision_damage(
	long vehicle_index);

boolean game_team_is_enemy(
	short team_index0,
	short team_index1);

short actor_visibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	real_point3d const *target_position,
	char lighting,
	short line_of_sight,
	boolean use_maximum_distance,
	boolean target_is_player,
	short perception_knowledge);

short actor_audibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	real_point3d const *source_position,
	struct location const *source_location,
	short source_type,
	real scale,
	short line_of_sight);

static boolean actor_perception_assess_suicide_danger(
	long actor_index,
	long object_index,
	real suicide_radius,
	real distance,
	boolean enemy,
	boolean visible);

real actor_look_compute_prop_interest(
	long actor_index,
	long prop_index);

/* TU-local copy: the same macro exists in actor_stimulus.c. */
#define prop_acknowledged(prop) \
	((prop)->state >= _prop_state_becoming_unacknowledged && \
		(prop)->state <= _prop_state_acknowledged)

static __inline void actor_perception_midpoint3d(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d *result)
{
	result->x = (p0->x + p1->x) * 0.5f;
	result->y = (p0->y + p1->y) * 0.5f;
	result->z = (p0->z + p1->z) * 0.5f;

	return;
}

/* ---------- globals */

short const global_combat_status_table[NUMBER_OF_ACTOR_TARGET_TYPES] =
{
	0, 0, 0, 1, 2, 2, 3, 4, 5, 5, 7, 7
};

short const global_acknowledgement_speeds[4][4] =
{
	{ 0, 0, 1, 3 },
	{ 0, 1, 2, 3 },
	{ 0, 2, 3, 4 },
	{ 0, 3, 4, 4 }
};

static long last_refresh_overflow_warning_time = NONE;

/* ---------- public code */

boolean actor_perception_desire_prop(
	long actor_index,
	short desired_target_state,
	long unit_index,
	long prop_actor_index,
	boolean in_use,
	boolean player,
	boolean enemy,
	boolean dead,
	short dead_ticks,
	real suicide_radius,
	real distance_squared,
	short required_ticks,
	boolean *too_far_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_datum *related_actor;
	boolean desire;
	boolean too_far;

	if (prop_actor_index == NONE)
		related_actor = NULL;
	else
		related_actor = actor_get(prop_actor_index);

	too_far = FALSE;

	if ((!enemy || dead) &&
		desired_target_state >= _prop_state_uninspected_orphan &&
		desired_target_state <= _prop_state_inspected_orphan)
	{
		desire = FALSE;
	}
	else if (player)
	{
		desire = TRUE;
	}
	else if (related_actor &&
		(!related_actor->meta.active || related_actor->meta.dormant))
	{
		desire = FALSE;
	}
	else if (desired_target_state == NONE && (in_use || required_ticks > 0))
	{
		desire = TRUE;
	}
	else if (distance_squared > 1600.0f)
	{
		desire = FALSE;
	}
	else if (dead)
	{
		desire = TRUE;

		if (actor->meta.encounter_index != NONE)
		{
			struct encounter_datum *encounter =
				encounter_get(actor->meta.encounter_index);
			struct unit_datum *unit = unit_get(unit_index);
			long ignore_time = encounter->corpse_ignore_time;
			boolean inactive_encounter;

			if (ignore_time <= actor->emotions.corpse_ignore_time)
				ignore_time = actor->emotions.corpse_ignore_time;

			if (ignore_time != NONE &&
				(unit->unit.time_of_death == NONE ||
					unit->unit.time_of_death < ignore_time))
			{
				desire = FALSE;
			}

			inactive_encounter =
				!encounter->enemy_visible &&
				!encounter->enemy_alive &&
				!encounter->stand_down;

			if (!desire)
				goto done;

			if (inactive_encounter)
			{
				if (distance_squared < 225.0f)
				{
					desire = TRUE;
					goto done;
				}

				desire = FALSE;
				goto done;
			}
		}

		if (suicide_radius > 0.0f)
		{
			desire = TRUE;
		}
		else if (enemy && dead_ticks > 150)
		{
			desire = FALSE;
		}
		else if (actor_action_class(actor_index) > _action_class_passive)
		{
			desire = FALSE;
		}
		else
		{
			real maximum_distance_squared = 16.0f;

			if (!enemy && actor->state.mode < 3)
				maximum_distance_squared = 64.0f;

			/* INFERRED FROM JANUARY'S BYTES: the explicit branch, not
			 * `desire = distance_squared < maximum_distance_squared;`, is what
			 * gives this else-if arm its own cross-jump resolution. It also
			 * matches the three sibling arms above, which assign TRUE/FALSE
			 * literals, and the branchy form used on this same variable in the
			 * inactive-encounter block. Required jointly with the squared-distance
			 * macro; neither reaches January alone. */
			if (distance_squared < maximum_distance_squared)
				desire = TRUE;
			else
				desire = FALSE;
		}
	}
	else
	{
		if (enemy)
		{
			desire = TRUE;
			too_far = distance_squared > 36.0f;
		}
		else
		{
			desire = distance_squared < 225.0f;

			if (actor->state.combat_status >= 4)
				too_far = TRUE;
			else if (actor->external_orders.pursuit_is_coordinator)
				too_far = FALSE;
			else
				too_far = distance_squared > 16.0f;
		}
	}

done:
	if (too_far_reference)
		*too_far_reference = too_far;

	return desire;
}

void actor_perception_acknowledge(
	long actor_index,
	long prop_index,
	boolean had_orphan,
	boolean expected_acknowledgement)
{
	struct prop_datum *prop = prop_get(prop_index);

#line 1037 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	assert(prop->owner_actor_index == actor_index);
	vassert(prop_acknowledged(prop), "prop_acknowledged(prop)");
#line 1039 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	vassert(prop->orphan_prop_index == NONE, "prop->orphan_prop_index == NONE");
#line 300 "source\\ai\\actor_perception.c"

	prop->tried_to_search = FALSE;
	prop->tried_to_uncover = FALSE;
	prop->abandoned_search = FALSE;
	prop->refresh_stimuli = TRUE;

	actor_stimulus_prop_acknowledged(
		actor_index,
		prop_index,
		had_orphan,
		expected_acknowledgement);

	return;
}

short actor_get_perception_knowledge(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	short knowledge_type = NONE;

	if (prop_index != NONE)
	{
		struct prop_datum *prop = prop_get(prop_index);

#line 1394 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		assert(prop->owner_actor_index == actor_index);
#line 390 "source\\ai\\actor_perception.c"

		if (prop_acknowledged(prop) ||
			prop->unit_effect == _ai_unit_effect_shooting ||
			prop->unit_effect == _ai_unit_effect_death_scream ||
			(!prop->enemy &&
				(!prop->dead || actor->state.mode >= _actor_mode_combat)))
		{
			knowledge_type = _actor_knowledge_definite;
		}

		if (knowledge_type == NONE &&
			prop->orphan_prop_index != NONE)
		{
			knowledge_type =
				_actor_knowledge_searching +
				(prop_get(prop->orphan_prop_index)->definitely_located != FALSE);
		}
	}

	if (knowledge_type == NONE)
	{
		if (actor->state.combat_status >= _actor_combat_status_investigate)
		{
			knowledge_type = _actor_knowledge_searching;
		}
		else
		{
			knowledge_type = actor->state.mode >= _actor_mode_combat ?
				_actor_knowledge_guard :
				_actor_knowledge_noncombat;
		}
	}

	return knowledge_type;
}

void actor_get_vision_distances(
	long actor_index,
	real maximum_vision_distance,
	real perception_factor,
	real horizontal_angle,
	real *full_distance_reference,
	real *partial_distance_reference)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_perception_definition_view const *definition =
		(struct actor_perception_definition_view const *)
			actor_definition_get(actor->meta.definition_index);
	real full_distance;
	real partial_distance;

	if (horizontal_angle > definition->peripheral_distance)
	{
		partial_distance = 0.0f;
		full_distance = 0.0f;
	}
	else
	{
		real maximum_distance =
			maximum_vision_distance * perception_factor;
		real peripheral_distance =
			perception_factor * definition->maximum_peripheral_distance;
		real minimum_full_distance = 0.7f * maximum_distance;
		real minimum_partial_distance = 0.7f * peripheral_distance;

		if (minimum_partial_distance > 3.5f)
			minimum_partial_distance = 3.5f;

		if (horizontal_angle > definition->central_vision_angle)
		{
			partial_distance = peripheral_distance;
			full_distance = minimum_partial_distance;
		}
		else
		{
			real maximum_vision_angle =
				definition->maximum_vision_angle;
			real full_vision_angle = 0.8f * maximum_vision_angle;

			if (horizontal_angle < maximum_vision_angle)
			{
				partial_distance = maximum_distance;
			}
			else
			{
				real interpolation =
					(horizontal_angle - maximum_vision_angle) /
					(definition->central_vision_angle -
						maximum_vision_angle);

				partial_distance =
					(1.0f - interpolation) * maximum_distance +
					peripheral_distance * interpolation;
			}

			if (horizontal_angle < full_vision_angle)
			{
				full_distance = minimum_full_distance;
			}
			else
			{
				real interpolation =
					(horizontal_angle - full_vision_angle) /
					(definition->central_vision_angle -
						full_vision_angle);

				full_distance =
					(1.0f - interpolation) *
						minimum_full_distance +
					minimum_partial_distance * interpolation;
			}
		}
	}

	*partial_distance_reference = partial_distance;
	*full_distance_reference = full_distance;

	return;
}

void actor_situation_update_target_status(
	long actor_index)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);

	if (actor->target_prop_index == NONE)
	{
		actor->target_type = 0;
		actor->target_last_visible_time = NONE;
		actor->target_outside_active_area = FALSE;
	}
	else
	{
		struct actor_perception_prop_view *target_prop =
			(struct actor_perception_prop_view *)prop_get(
				actor->target_prop_index);
		struct actor_perception_target_unit_view *target_unit =
			(struct actor_perception_target_unit_view *)
				unit_get(target_prop->unit_index);
		short target_type;

#line 4291 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		assert(target_prop->enemy);
#line 510 "source\\ai\\actor_perception.c"

		switch (target_prop->state)
		{
		case _prop_state_unacknowledged:
			target_type = 0;
			actor->target_prop_index = NONE;
			actor->target_last_visible_time = NONE;
			break;

		case _prop_state_becoming_acknowledged:
			target_type = 1;
			break;

		case _prop_state_becoming_unacknowledged:
		case _prop_state_acknowledged:
			if (target_prop->dead)
				target_type = 2;
			else if (target_prop->currently_damaging_me)
				target_type = 11;
			else if (target_prop->visibility_result >= 2)
				target_type = 10;
			else if (target_prop->line_of_sight_result != 0 &&
				target_prop->line_of_sight_result != 1)
			{
				target_type = 7;
			}
			else if (target_prop->quantized_facing <= 2 &&
				target_prop->distance < 6.f)
			{
				target_type = 9;
			}
			else
			{
				target_type = 8;
			}
			break;

		case _prop_state_inspected_orphan:
			if (target_prop->dead)
				target_type = 2;
			else
				target_type =
					target_prop->abandoned_search ? 3 : 4;
			break;

		case _prop_state_uninspected_orphan:
			target_type =
				(target_prop->definitely_located != FALSE) + 5;
			break;

		/* target_type is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			display_assert(
				NULL,
				"c:\\halo\\SOURCE\\ai\\actor_perception.c",
				4362,
				TRUE);
			system_exit(-1);
			break;
		}

		actor->target_type = target_type;

		if (target_prop->state >=
				_prop_state_becoming_unacknowledged &&
			target_prop->state <= _prop_state_acknowledged)
		{
			actor->target_outside_active_area =
				!target_prop->dead;

			if (target_prop->visibility_result > 0)
				actor->target_last_visible_time =
					target_prop->last_visible_time;
		}
		else
		{
			actor->target_outside_active_area =
				!TEST_FLAG(
					target_unit->active_region_flags,
					2);
		}
	}

	return;
}

void actor_situation_combat_status_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->stimuli.suspicion_combat_status > _actor_combat_status_none)
	{
		if (actor->state.suspicion_combat_status <
			actor->stimuli.suspicion_combat_status)
		{
			actor->state.suspicion_combat_status =
				actor->stimuli.suspicion_combat_status;
			actor->state.suspicion_timer = actor->stimuli.suspicion_timer;
		}
		else if (actor->state.suspicion_combat_status ==
			actor->stimuli.suspicion_combat_status)
		{
			actor->state.suspicion_timer = MAX(
				actor->state.suspicion_timer,
				actor->stimuli.suspicion_timer);
		}

		actor->stimuli.suspicion_combat_status =
			_actor_combat_status_none;
	}

#line 4408 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	assert((actor->target.target_type >= 0) && (actor->target.target_type < NUMBER_OF_ACTOR_TARGET_TYPES));
#line 540 "source\\ai\\actor_perception.c"

	actor->state.combat_status = MAX(
		actor->state.suspicion_combat_status,
		MAX(
			actor->state.artificial_combat_status,
			global_combat_status_table[actor->target.target_type]));

	if (actor->state.combat_status > actor->state.suspicion_combat_status)
	{
		actor->state.suspicion_combat_status = _actor_combat_status_none;
	}

	if (actor->state.mode < _actor_mode_combat)
	{
		actor->state.combat_mode_timer = 0;
	}
	else
	{
		actor->state.combat_mode_timer++;
	}

	if (actor->state.combat_status == _actor_combat_status_none)
	{
		actor->state.in_combat_timer = 0;
	}
	else
	{
		actor->state.in_combat_timer++;
	}

	if (actor->state.combat_status >= _actor_combat_status_certain)
	{
		actor->state.certain_combat_timer++;
		actor->state.uncertain_combat_timer = 0;
	}
	else
	{
		actor->state.certain_combat_timer = 0;
		if (actor->state.uncertain_combat_timer != NONE)
		{
			actor->state.uncertain_combat_timer++;
		}
	}

	if (actor->state.combat_status >= _actor_combat_status_visible)
	{
		actor->state.had_visible_enemy = TRUE;
	}

	return;
}

void actor_situation_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_iterator iterator;
	struct prop_datum *prop;
	long best_prop_index = NONE;
	real best_target_weight = 0.0f;
	boolean charging =
		actor->emotions.berserk ||
		actor->state.action == _actor_action_charge;

	memset(&actor->situation, 0, sizeof(actor->situation));
	prop_iterator_new(&iterator, actor_index);
	prop = prop_iterator_next(&iterator);
	while (prop != NULL)
	{
		if (prop_acknowledged(prop) &&
			!prop->dead)
		{
			if (prop->enemy)
			{
				boolean visible = prop->visibility >= _actor_perception_full;
				short priority = _actor_threat_none;

				actor->situation.known_enemies++;
				if (visible && prop->unreachable_ticks == 0)
				{
					actor->situation.visible_reachable_enemies++;
				}

				if (visible ||
					(prop->shooting && prop->line_of_sight == _ai_line_of_sight_clear))
				{
					if (visible)
					{
						actor->situation.cumulative_threats[_actor_threat_visible]++;
						priority = MAX(priority, _actor_threat_visible);
					}

					if (prop->currently_damaging_me)
					{
						actor->situation.cumulative_threats[_actor_threat_damaging_me]++;
						priority = MAX(priority, _actor_threat_damaging_me);
					}

					if (prop->shooting)
					{
						actor->situation.cumulative_threats[_actor_threat_shooting]++;
						priority = MAX(priority, _actor_threat_shooting);
					}

					if (prop->quantized_facing <= 2)
					{
						if (visible)
						{
							actor->situation.cumulative_threats[_actor_threat_visible_facing_me]++;
							priority = MAX(priority, _actor_threat_visible_facing_me);

							if (!charging &&
								prop->distance < 2.0f)
							{
								actor->situation.cumulative_threats[_actor_threat_extremely_close_to_me]++;
								priority = MAX(priority, _actor_threat_extremely_close_to_me);
							}
						}

						if (prop->quantized_facing <= 1)
						{
							if (prop->shooting)
							{
								actor->situation.cumulative_threats[_actor_threat_shooting_near_me]++;
								priority = MAX(priority, _actor_threat_shooting_near_me);
							}

							if (prop->quantized_facing <= 0)
							{
								if (visible)
								{
									actor->situation.cumulative_threats[_actor_threat_visible_aiming_at_me]++;
									priority = MAX(priority, _actor_threat_visible_aiming_at_me);
								}

								if (prop->shooting)
								{
									actor->situation.cumulative_threats[_actor_threat_shooting_at_me]++;
									priority = MAX(priority, _actor_threat_shooting_at_me);
								}
							}
						}
					}
				}

				actor->situation.specific_threats[priority]++;
			}
			else
			{
				struct unit_datum *unit = unit_get(prop->unit_index);
				struct actor_datum *friend_actor =
					unit->unit.actor_index == NONE ?
						NULL :
						actor_get(unit->unit.actor_index);
				short actor_type;
				boolean area;
				boolean visible;
				boolean close;

				if (unit->unit.player_index != NONE)
				{
					actor_type = _actor_player;
				}
				else if (friend_actor != NULL)
				{
					actor_type = friend_actor->meta.type;
				}
				else
				{
					actor_type = _actor_none;
				}

				area = FALSE;
				visible = FALSE;
				close = FALSE;

				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_perception.c",
					4572,
					(actor_type >= 0) &&
					(actor_type < NUMBER_OF_ACTOR_TYPES));

				if (prop->distance < 8.0f)
				{
					area = TRUE;
				}
				else if (prop->fighting &&
					friend_actor != NULL &&
					actor->target.target_prop_index != NONE &&
					friend_actor->target.target_prop_index != NONE)
				{
					struct prop_datum *target_prop =
						prop_get(actor->target.target_prop_index);
					struct prop_datum *friend_target_prop =
						prop_get(friend_actor->target.target_prop_index);

					if (target_prop->unit_index == friend_target_prop->unit_index)
					{
						area = TRUE;
					}
				}

				if (prop->line_of_sight == _ai_line_of_sight_clear ||
					prop->line_of_sight == _ai_line_of_sight_occluded)
				{
					visible = TRUE;
					close = prop->distance < 3.0f;
				}

				if (area)
				{
					actor->situation.area_friends++;
					if (prop->fighting)
					{
						actor->situation.area_fighting_friends++;
					}
					if (prop->fighting && prop->vehicle_gunner)
					{
						actor->situation.area_fire_support_friends++;
					}
					actor->situation.area_friends_by_type[actor_type]++;
					if (prop->fighting)
					{
						actor->situation.area_fighting_friends_by_type[actor_type]++;
					}
				}

				if (visible)
				{
					actor->situation.visible_friends++;
					if (prop->fighting)
					{
						actor->situation.visible_fighting_friends++;
					}
					actor->situation.visible_friends_by_type[actor_type]++;
					if (prop->fighting)
					{
						actor->situation.visible_fighting_friends_by_type[actor_type]++;
					}
				}

				if (close)
				{
					actor->situation.close_friends++;
					if (prop->fighting)
					{
						actor->situation.close_fighting_friends++;
					}
					actor->situation.close_friends_by_type[actor_type]++;
					if (prop->fighting)
					{
						actor->situation.close_fighting_friends_by_type[actor_type]++;
					}
				}
			}
		}

		if (prop->target_weight > best_target_weight)
		{
			best_prop_index = iterator.index;
			best_target_weight = prop->target_weight;
		}

		prop = prop_iterator_next(&iterator);
	}

	if (best_prop_index != actor->target.target_prop_index)
	{
		long old_target_prop_index = actor->target.target_prop_index;

		actor->target.target_type = _actor_target_none;
		actor->target.target_prop_index = best_prop_index;
		actor->target.target_last_visible_time = NONE;

		if (old_target_prop_index != NONE)
		{
			struct prop_datum *old_target_prop =
				prop_get(old_target_prop_index);

			old_target_prop->target_weight =
				actor_compute_prop_target_weight(
					actor_index,
					old_target_prop_index);
		}

		if (best_prop_index != NONE)
		{
			struct prop_datum *new_target_prop =
				prop_get(best_prop_index);

			new_target_prop->target_weight =
				actor_compute_prop_target_weight(
					actor_index,
					actor->target.target_prop_index);
		}
	}

	actor_situation_update_target_status(actor_index);
	actor_situation_combat_status_update(actor_index);

	return;
}

boolean actor_perception_friend_prop_is_attacking(
	long actor_index,
	long friend_prop_index,
	real_vector3d *attack_vector)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *friend_prop = prop_get(friend_prop_index);
	boolean attacking = FALSE;

#line 4710 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	vassert(
		prop_acknowledged(friend_prop) && !friend_prop->enemy && !friend_prop->dead,
		"prop_acknowledged(friend_prop) && !friend_prop->enemy && !friend_prop->dead");
#line 610 "source\\ai\\actor_perception.c"

	if (friend_prop->swarm)
	{
		attacking = FALSE;
	}
	else if (friend_prop->player)
	{
		attacking = friend_prop->shooting;
		unit_get_aiming_vector(friend_prop->unit_index, attack_vector);
		if (!attacking && actor->situation.known_enemies > 0)
		{
			struct prop_iterator iterator;
			struct prop_datum *prop;

			prop_iterator_new(&iterator, actor_index);
			while ((prop = prop_iterator_next(&iterator)) != NULL)
			{
				if (prop_acknowledged(prop) && prop->enemy)
				{
					real_vector3d friend_to_enemy;

					vector_from_points3d(
						&friend_prop->body_position,
						&prop->body_position,
						&friend_to_enemy);
					if (normalize3d(&friend_to_enemy) > 0.0f &&
						dot_product3d(attack_vector, &friend_to_enemy) > 0.5f)
					{
						attacking = TRUE;
						break;
					}
				}
			}
		}
	}
	else if (friend_prop->actor_index != NONE)
	{
		attacking = actor_attacking_target(
			friend_prop->actor_index,
			attack_vector);
	}

	return attacking;
}

short actor_perception_aiming_vector_test_blockage(
	real_point3d const *origin,
	real_vector3d const *aiming_vector,
	real_point3d const *point,
	real_vector3d *error)
{
	real_vector2d horizontal_aiming;
	real_vector3d friend_vector;
	real_vector3d blockage_vector;
	real horizontal_aiming_magnitude;
	real projection;
	real friend_distance;
	real horizontal_error_squared;
	short blockage = 0;

	horizontal_aiming.i = aiming_vector->i;
	horizontal_aiming.j = aiming_vector->j;
	horizontal_aiming_magnitude = magnitude2d(&horizontal_aiming);
	if (!(_real_epsilon > fabs(horizontal_aiming_magnitude)))
	{
		real inverse_magnitude = 1.0f / horizontal_aiming_magnitude;

		horizontal_aiming.i *= inverse_magnitude;
		horizontal_aiming.j *= inverse_magnitude;
	}
	else
	{
		horizontal_aiming_magnitude = 0.0f;
	}

	if (!(horizontal_aiming_magnitude > 0.0f))
		goto done;

	vector_from_points3d(origin, point, &friend_vector);
	projection =
		friend_vector.i * horizontal_aiming.i +
		friend_vector.j * horizontal_aiming.j;
	friend_distance = square_root(
		friend_vector.i * friend_vector.i +
		friend_vector.j * friend_vector.j);
	friend_distance *= 0.8660254f;

	if (!(projection > friend_distance))
		goto done;

	projection = -projection;
	blockage_vector.i =
		friend_vector.i + projection * aiming_vector->i;
	blockage_vector.j =
		friend_vector.j + projection * aiming_vector->j;
	blockage_vector.k =
		friend_vector.k + projection * aiming_vector->k;

	if (error != NULL)
	{
		error->i = -blockage_vector.i;
		error->j = -blockage_vector.j;
		error->k = -blockage_vector.k;
	}

	if (blockage_vector.k > -0.5f && blockage_vector.k < 0.9f)
	{
		blockage = 2;
	}
	else if (blockage_vector.k > -0.8f && blockage_vector.k < 1.2f)
	{
		blockage = 1;
	}
	else
	{
		blockage = 0;
		goto done;
	}

	horizontal_error_squared =
		blockage_vector.i * blockage_vector.i +
		blockage_vector.j * blockage_vector.j;
	if (!(horizontal_error_squared < 0.36f))
	{
		if (horizontal_error_squared < 1.21f)
			blockage = 1;
		else
			blockage = 0;
	}

done:
	return blockage;
}

real actor_compute_prop_target_weight(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);
	struct actor_definition *actor_definition;
	struct actor_variant_definition *variant_definition;
	struct actor_variant_definition *firing_variant;
	struct actor_target_weight_weapon_definition_view *weapon_definition;
	short range_weight;
	short knowledge_weight = 0;
	struct
	{
		long target_weight;
		long preferred_weight;
		real bonus_weight;
	} weights;

	if (prop->ignore ||
		!prop->enemy ||
		(prop->state >= 0 && prop->state <= 1) ||
		(prop->dead && prop->dead_ticks >= 150) ||
		prop->type == 15)
	{
		return 0.0f;
	}

	actor_definition =
		actor_definition_get(actor->meta.definition_index);
	variant_definition =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	weights.target_weight = 0;
	weights.preferred_weight = 0;
	weights.bonus_weight = 0.0f;

	if (actor->meta.swarm)
	{
		range_weight = 0;
		goto range_weight_done;
	}

	if (prop->unreachable_ticks > 0)
	{
		range_weight = 0;
		goto range_weight_done;
	}

	if (!actor_has_ranged_weapon(actor_index))
	{
		real maximum_range;

		if (actor->emotions.berserk)
			maximum_range =
				variant_definition->ranged_combat.melee_range;
		else
			maximum_range =
				variant_definition->ranged_combat.berserk_melee_range;

		if (prop->distance < 2.0f)
		{
			range_weight = 5;
			if (prop->state != _prop_state_inspected_orphan)
				goto range_weight_done;
		}

		if (prop->vehicle_index != NONE)
			range_weight = 0;
		else if (prop->flying &&
			actor_definition->berserk.melee_leap_velocity == 0.0f)
			range_weight = 0;
		else if (prop->underwater !=
			actor->input.underwater)
			range_weight = 1;
		else if (prop->distance < maximum_range)
			range_weight = 3;
		else
			goto ranged_weapon_range_two;
	}
	else
	{
		weapon_definition =
			(struct actor_target_weight_weapon_definition_view *)
				actor_get_weapon_definition(actor_index);
		firing_variant =
			actor_combat_get_firing_variant_definition(actor_index);

		if (weapon_definition != NULL &&
			prop->distance <
				weapon_definition->minimum_target_range)
		{
ranged_weapon_range_two:
			range_weight = 2;
			goto range_weight_done;
		}

		if (prop->underwater != actor->input.underwater)
		{
			range_weight = 2;
			goto range_weight_done;
		}

		if (prop->distance < 2.0f)
		{
			range_weight = 5;
			if (prop->state != _prop_state_inspected_orphan)
				goto range_weight_done;
		}

		if (prop->distance <
			firing_variant->ranged_combat.combat_range_upper_bound)
			range_weight = 3;
		else if (prop->distance <
			firing_variant->ranged_combat.maximum_firing_range)
			range_weight = 2;
		else
			range_weight = 1;
	}

range_weight_done:
	if (prop->dead)
		knowledge_weight = 1;
	else
	{
		if (!actor->meta.swarm &&
			prop->currently_damaging_me &&
			prop->unreachable_ticks == 0)
		{
			knowledge_weight = 6;
		}
		else if (prop->state >= 2 &&
			prop->state <= 3)
		{
			if (actor->meta.swarm)
				knowledge_weight = 4;
			else if (prop->unreachable_ticks > 0)
			{
				knowledge_weight = 3;
			}
			else if (prop->line_of_sight != 0 &&
				prop->line_of_sight != 1)
			{
				knowledge_weight = 3;
			}
			/* Preserve January's quantized-facing read at +0x122. */
			else if (prop->shooting &&
				prop->quantized_facing <= 1)
			{
				knowledge_weight = 5;
			}
			else
				knowledge_weight = 4;
		}
		else
		{
			if (prop->state < _prop_state_uninspected_orphan ||
				prop->state > _prop_state_inspected_orphan)
			{
				display_assert(
					"prop_orphaned(prop)",
					"c:\\halo\\SOURCE\\ai\\actor_perception.c",
					4230,
					TRUE);
				system_exit(-1);
			}

			if (prop->definitely_located)
				knowledge_weight = 3;
			else
				knowledge_weight =
					(prop->state ==
						_prop_state_uninspected_orphan) + 1;
		}
	}

	if (actor->target.target_prop_index == NONE)
	{
		if (prop->player ||
			prop_index ==
				actor_perception_preferred_target_prop_index(actor))
		{
			weights.bonus_weight = 3.0f;
		}
	}
	else if (prop_index == actor->target.target_prop_index &&
		actor_perception_target_weight_combat_status(actor) >= 3)
	{
		weights.target_weight = 1;
	}

	if (prop->preferred_target)
		weights.preferred_weight = 2;

	weights.target_weight =
		((short)weights.target_weight +
			(short)weights.preferred_weight) +
		(knowledge_weight + range_weight);

	/* Preserve January's swapped target/distance scale constants. */
	return
		weights.target_weight * 10.0f +
		(5.0f / (prop->distance * 0.1f + 1.0f) +
			weights.bonus_weight);
}




static long actor_emotion_assess_unopposable_danger(
	long prop_index)
{
	struct prop_datum *prop;
	long priority;

	prop = prop_get(prop_index);
	priority = 0;

	if (prop->state >= _prop_state_becoming_unacknowledged &&
		prop->state <= _prop_state_acknowledged &&
		prop->unopposable_enemy)
	{
		if (prop->currently_damaging_me)
		{
			priority = 4;
		}
		else if (prop->shooting)
		{
			if (prop->quantized_facing <= 1)
				priority = 3;
			else
				priority = 2;
		}
		else if (prop->visibility >= _actor_perception_full)
		{
			priority = 1;
		}
	}

	return priority;
}

struct actor_emotion_prop_view;

struct actor_emotion_target
{
	short priority;
	short pad;
	long prop_index;
	long unit_index;
	struct actor_emotion_prop_view *prop;
	short count;
	short pad2;
	real minimum_distance_squared;
	long closest_unit_index;
};

struct actor_emotion_actor_view
{
	byte __unknown000[0x58];
	long definition_index;
	byte __unknown05C[0x348];
	long last_emotion_target_time;
	short emotion_target_ticks;
	byte __unknown3AA[2];
	long emotion_target_prop_index;
	long emotion_target_time;
};

struct actor_emotion_definition_view
{
	byte __unknown000[0x268];
	short normal_threshold;
	short vehicle_threshold;
	short player_threshold;
	byte __unknown26E[2];
	real trigger_delay_lower;
	real trigger_delay_upper;
	short casualty_threshold;
	short friend_threshold;
	byte __unknown27C[0xC];
	real target_duration_lower;
	real target_duration_upper;
};

struct actor_emotion_prop_view
{
	byte __unknown000[0x18];
	long unit_index;
	long actor_index;
	byte __unknown020[4];
	short state;
	byte __unknown026[0xC];
	short combat_status;
	byte __unknown034[0x2C];
	boolean enemy;
	byte __unknown061[0x13];
	boolean recent_damage;
	byte __unknown075[3];
	short dead_ticks;
	byte __unknown07A[0x2A];
	boolean unopposable;
	byte __unknown0A5;
	short unopposable_casualties;
	short unopposable_casualty_decay_timer;
	short emotion_trigger_ticks;
	short emotion_trigger_age;
	short emotion_trigger_threshold;
	byte __unknown0B0[0x6C];
	real distance;
	byte __unknown120[2];
	char visibility;
	byte __unknown123[0xB];
	boolean player;
	boolean friend_attacking;
	byte __unknown130[5];
	boolean vehicle_gunner;
	boolean dangerous_vehicle_driver;
};

typedef char actor_emotion_target_size_assert[
	sizeof(struct actor_emotion_target) == 0x1C ? 1 : -1];
typedef char actor_emotion_actor_last_time_offset_assert[
	offsetof(struct actor_emotion_actor_view, last_emotion_target_time) == 0x3A4 ? 1 : -1];
typedef char actor_emotion_actor_ticks_offset_assert[
	offsetof(struct actor_emotion_actor_view, emotion_target_ticks) == 0x3A8 ? 1 : -1];
typedef char actor_emotion_actor_prop_offset_assert[
	offsetof(struct actor_emotion_actor_view, emotion_target_prop_index) == 0x3AC ? 1 : -1];
typedef char actor_emotion_prop_distance_offset_assert[
	offsetof(struct actor_emotion_prop_view, distance) == 0x11C ? 1 : -1];
typedef char actor_emotion_prop_driver_offset_assert[
	offsetof(struct actor_emotion_prop_view, dangerous_vehicle_driver) == 0x136 ? 1 : -1];

static short actor_emotion_get_unopposable_enemy(
	struct actor_emotion_target *targets,
	long unit_index,
	long actor_index,
	short *target_count,
	short maximum_target_count)
{
	short count;
	short target_index = NONE;
	short index;

	count = *target_count;
	for (index = 0; index < count; index++)
	{
		if (targets[index].unit_index == unit_index)
		{
			target_index = index;
			break;
		}
	}

	if (target_index == NONE && count < maximum_target_count)
	{
		*target_count = count + 1;
		targets[count].priority = 0;
		targets[count].prop_index = NONE;
		targets[count].unit_index = NONE;
		targets[count].prop = NULL;
		targets[count].count = 0;
		targets[count].minimum_distance_squared = FLT_MAX;
		targets[count].closest_unit_index = NONE;
		target_index = count;
	}

	return target_index;
}



short actor_visibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	real_point3d const *target_position,
	char lighting,
	short line_of_sight,
	boolean use_maximum_distance,
	boolean target_is_player,
	short perception_knowledge)
{
	long result = 0;

	if (line_of_sight == 0 || line_of_sight == 1)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct actor_definition *definition =
			actor_definition_get(actor->meta.definition_index);
		struct actor_variant_definition *firing_variant =
			actor_combat_get_firing_variant_definition(actor_index);
		real maximum_distance =
			definition->perception.maximum_vision_distance;
		real_vector3d direction;
		real distance_squared;
		real full_distance;
		real partial_distance;

		if (firing_variant->ranged_combat.modified_vision_range > 0.0f)
		{
			maximum_distance =
				firing_variant->ranged_combat.modified_vision_range;
		}

		{
			real knowledge_factor = 1.0f;

			switch (perception_knowledge)
			{
			case 0:
				knowledge_factor = 0.4f;
				break;
			case 1:
				knowledge_factor = 0.6f;
				break;
			case 2:
				knowledge_factor = 0.8f;
				break;
			case 3:
				knowledge_factor = 1.0f;
				break;
			default:
				display_assert(
					"!\"unreachable\"",
					"c:\\halo\\SOURCE\\ai\\actor_perception.c",
					1268,
					TRUE);
				system_exit(-1);
				break;
			}
			maximum_distance *= knowledge_factor;
		}

		{
			vector_from_points3d(
				&position->head_position,
				target_position,
				&direction);
			distance_squared = magnitude_squared3d(&direction);

			if (distance_squared < maximum_distance * maximum_distance)
			{
				real perception_factor = 1.0f;

				if (!TEST_FLAG(definition->flags, 0))
				{
					switch (lighting)
					{
					case 0:
						perception_factor = 0.3f;
						break;
					case 1:
						perception_factor = 0.7f;
						break;
					}
				}

				{
					real fog =
						scenario_fog_at_point(
							&position->body_location,
							&position->head_position,
							target_position);

					if (fog > 0.8f)
					{
						perception_factor = 0.15f;
					}
					else
					{
						if (fog > 0.2f)
						{
							perception_factor =
								(0.8f - fog) *
								perception_factor *
								1.6666666f;
						}

						if (perception_factor > 0.15f)
							goto perception_factor_ready;

						perception_factor = 0.15f;
					}
				}

perception_factor_ready:
				{
					struct actor_debug_info *debug =
						&actor_debug_array[AI_DEBUG_ACTOR_SLOT(
							DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

					if (target_is_player)
					{
						debug->vision_last_time = game_time_get();
						debug->vision_last_maximum_distance =
							maximum_distance;
						debug->vision_last_perception_factor =
							perception_factor;
					}
				}

				{
					real visible_distance =
						perception_factor * maximum_distance;
					real visible_distance_i = visible_distance;
					real visible_distance_j = visible_distance;

					if (distance_squared <
						visible_distance_i * visible_distance_j)
					{
						if (!actor->meta.swarm && use_maximum_distance)
						{
							real_vector3d facing;
							real pitch;

							facing.i =
								dot_product3d(
									&direction,
									&actor->input.looking_vector);
							facing.j =
								dot_product3d(
									&direction,
									&actor->input.looking_left_vector);
							facing.k =
								dot_product3d(
									&direction,
									&actor->input.looking_up_vector);
							pitch =
								arctangent(
									facing.k,
									square_root(
										facing.i * facing.i +
											facing.j * facing.j));

							if (pitch > 0.5235988f ||
								pitch < -0.7853982f)
							{
								full_distance = 0.0f;
								visible_distance = 0.0f;
							}
							else
							{
								actor_get_vision_distances(
									actor_index,
									maximum_distance,
									perception_factor,
									fabs(arctangent(
										facing.j,
										facing.i)),
									&full_distance,
									&partial_distance);
								visible_distance = partial_distance;
							}
						}
						else
						{
							full_distance =
								0.7f * visible_distance;
						}

						if (line_of_sight == 0 &&
							distance_squared <
								full_distance * full_distance)
						{
							result =
								distance_squared < 36.0f ? 3 : 2;
						}
						else if (distance_squared <
							visible_distance * visible_distance)
						{
							result = 1;
						}
					}
				}
			}
		}
	}

	return result;
}

short actor_audibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	real_point3d const *source_position,
	struct location const *source_location,
	short source_type,
	real scale,
	short line_of_sight)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	short result = 0;

	if (source_type != 0 &&
		position->body_location.cluster_index != NONE &&
		source_location->cluster_index != NONE)
	{
		real maximum_distance =
			definition->perception.hearing_distance;
		real encoded_distance = -1.0f;
		real audible_distance = -1.0f;
		real_vector3d direction;
		real distance_squared;

		vector_from_points3d(
			&position->head_position,
			source_position,
			&direction);
		distance_squared = magnitude_squared3d(&direction);

		if (dot_product3d(
				&direction,
				&position->body_facing) < 0.0f)
		{
			maximum_distance *= 0.8f;
		}

		if (actor_perception_audibility_combat_status(actor) == 2)
		{
			maximum_distance *= 0.7f;
		}
		else if (actor_perception_audibility_combat_status(actor) == 1)
		{
			maximum_distance *= 0.4f;
		}

		if (source_type == 4)
			maximum_distance *= 0.2f;
		else if (source_type == 1)
			maximum_distance *= 0.45f;
		else if (source_type == 3)
			maximum_distance *= 0.7f;

		if (scenario_location_deafening(&position->body_location) ||
			scenario_location_deafening(source_location))
		{
			maximum_distance *= 0.25f;
		}

		if (line_of_sight != 0 && line_of_sight != 1)
			maximum_distance *= 0.7f;

		if (distance_squared < maximum_distance * maximum_distance)
		{
			char encoded_sound_distance =
				(char)structure_bsp_get_cluster_encoded_sound_distance(
					global_structure_bsp_get(),
					source_location->cluster_index,
					position->body_location.cluster_index);

			if (!(encoded_sound_distance & FLAG(7)))
			{
				encoded_distance =
					((byte)encoded_sound_distance & ~FLAG(7)) *
						2.015748f;
				audible_distance = encoded_distance * 2.0f;
				audible_distance =
					MAX(
						audible_distance,
						square_root(distance_squared));

				if (audible_distance < maximum_distance)
					result = (source_type >= 3) + 2;
			}
		}

		{
			struct actor_debug_info *debug =
				&actor_debug_array[AI_DEBUG_ACTOR_SLOT(
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

			debug->field_A8 = maximum_distance;
			debug->field_A4 = TRUE;
			debug->field_A6 = result;
			debug->field_B0 = encoded_distance;
			debug->field_AC = square_root(distance_squared);
			debug->field_B4 = audible_distance;
		}
	}

	return result;
}

static void actor_emotion_unopposable_retreat(
	long actor_index)
{
	struct actor_emotion_target targets[16];
	struct actor_emotion_actor_view *actor;
	short target_count;
	short target_index;
	long target_prop_index;
	struct actor_emotion_definition_view *definition;
	struct actor_emotion_prop_view *prop;

	actor =
		(struct actor_emotion_actor_view *)actor_get(actor_index);
	definition =
		(struct actor_emotion_definition_view *)
			actor_definition_get(actor->definition_index);
	target_count = 0;

	{
		struct prop_iterator iterator;

		prop_iterator_new(&iterator, actor_index);
		while ((prop =
			(struct actor_emotion_prop_view *)
				prop_iterator_next(&iterator)) != NULL)
		{
			short priority =
				(short)actor_emotion_assess_unopposable_danger(iterator.index);

			if (priority > 0)
			{
				target_index = (short)actor_emotion_get_unopposable_enemy(
					targets,
					prop->unit_index,
					actor_index,
					&target_count,
					NUMBEROF(targets));

				if (target_index != NONE)
				{
					struct actor_emotion_target *target =
						&targets[target_index];

					if (target->priority < priority)
					{
						target->prop_index = iterator.index;
						target->unit_index = prop->unit_index;
						target->prop = prop;
						target->priority = priority;
					}
				}
			}
			else if (prop->state >= _prop_state_becoming_unacknowledged &&
				prop->state <= _prop_state_acknowledged &&
				!prop->enemy &&
				prop->actor_index != NONE &&
				prop->distance < 8.0f)
			{
				struct actor_emotion_actor_view *friend_actor =
					(struct actor_emotion_actor_view *)actor_get(prop->actor_index);

				if (friend_actor->emotion_target_ticks != 0 &&
					friend_actor->emotion_target_prop_index != NONE &&
					(actor->last_emotion_target_time == NONE ||
						friend_actor->emotion_target_time >=
							actor->last_emotion_target_time))
				{
					struct actor_emotion_prop_view *friend_target_prop =
						(struct actor_emotion_prop_view *)prop_get(
							friend_actor->emotion_target_prop_index);

					target_prop_index =
						prop_get_active_by_unit_index(
							actor_index,
							friend_target_prop->unit_index);

					if (target_prop_index != NONE)
					{
						struct actor_emotion_prop_view *target_prop =
							(struct actor_emotion_prop_view *)
								prop_get(target_prop_index);

						if (target_prop->state >=
								_prop_state_becoming_unacknowledged &&
							target_prop->state <= _prop_state_acknowledged &&
							target_prop->unopposable)
						{
							target_index = (short)actor_emotion_get_unopposable_enemy(
								targets,
								friend_target_prop->unit_index,
								actor_index,
								&target_count,
								NUMBEROF(targets));

							if (target_index != NONE)
							{
								struct actor_emotion_target *target =
									&targets[target_index];
								real distance_squared =
									actor_perception_distance_squared(
										friend_target_prop->distance);

								target->count++;
								if (distance_squared <
									target->minimum_distance_squared)
								{
									target->minimum_distance_squared =
										distance_squared;
									target->closest_unit_index =
										prop->actor_index;
								}

								if (target->prop_index == NONE)
								{
									target->prop_index =
										target_prop_index;
									target->unit_index =
										target_prop->unit_index;
									target->prop = target_prop;
								}
							}
						}
					}
				}
			}
		}
	}

	for (target_index = 0;
		target_index < target_count;
		target_index++)
	{
		struct actor_emotion_target *target = &targets[target_index];
		struct actor_emotion_prop_view *target_prop = target->prop;
		short threshold = definition->normal_threshold;
		boolean player_triggered = FALSE;

		if (target_prop->vehicle_gunner ||
			target_prop->dangerous_vehicle_driver)
		{
			threshold = definition->vehicle_threshold;
		}

		if (target_prop->player &&
			definition->player_threshold > 0 &&
			threshold > definition->player_threshold)
		{
			threshold = definition->player_threshold;
		}

		if (threshold > 0 &&
			target->priority >= threshold)
		{
			if (target_prop->player)
				player_triggered = TRUE;
			else
				target_prop->emotion_trigger_ticks = 22;
		}
		else if (target_prop->player)
		{
			target_prop->emotion_trigger_ticks = 22;
		}

		if (target_prop->emotion_trigger_ticks > 0)
		{
			if (target_prop->emotion_trigger_age == 0)
			{
				real upper_bound = definition->trigger_delay_upper;
				real lower_bound = definition->trigger_delay_lower;

				target_prop->emotion_trigger_threshold =
					(short)(real_seed_random_range(
						get_global_random_seed_address(),
						lower_bound,
						upper_bound) *
						30.0f);
			}

			target_prop->emotion_trigger_ticks--;
			target_prop->emotion_trigger_age++;
		}

		if (target_prop->dead_ticks >= 45 ||
			target->priority >= 4)
		{
			if (target_prop->emotion_trigger_threshold > 0 &&
				target_prop->emotion_trigger_age >=
					target_prop->emotion_trigger_threshold)
			{
				target->priority =
					MAX(target->priority, 7);
			}

			if (player_triggered)
				target->priority =
					MAX(target->priority, 8);

			if (definition->casualty_threshold > 0 &&
				target_prop->unopposable_casualties >=
					definition->casualty_threshold)
			{
				target->priority =
					MAX(target->priority, 9);
			}

			if (definition->friend_threshold > 0 &&
				target->count >=
					definition->friend_threshold)
			{
				target->priority =
					MAX(target->priority, 6);
			}
		}
	}

	if (actor->emotion_target_ticks > 0)
	{
		actor->emotion_target_ticks--;
		if (actor->emotion_target_ticks == 0)
		{
			actor->last_emotion_target_time = game_time_get();
			return;
		}
	}
	else
	{
		long best_prop_index = NONE;
		short best_priority = 5;

		for (target_index = 0;
			target_index < target_count;
			target_index++)
		{
			struct actor_emotion_target *target = &targets[target_index];

			if (target->priority > best_priority &&
				target->prop_index != NONE)
			{
				best_priority = target->priority;
				best_prop_index = target->prop_index;
			}
		}

		if (best_prop_index != NONE)
		{
			real upper_bound = definition->target_duration_upper;
			real lower_bound = definition->target_duration_lower;

			actor->emotion_target_ticks =
				(short)(real_seed_random_range(
					get_global_random_seed_address(),
					lower_bound,
					upper_bound) *
					30.0f);
			actor->emotion_target_prop_index = best_prop_index;
			actor->emotion_target_time = game_time_get();
		}
	}

	return;
}

void actor_berserk(
	long actor_index,
	boolean berserk)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (berserk != actor->emotions.berserk)
	{
		actor->emotions.berserk = berserk;
		actor->emotions.played_berserk_sound = FALSE;

		if (actor->meta.swarm)
		{
			long object_index;

			for (object_index = actor->meta.swarm_unit_index;
				object_index != NONE;)
			{
				struct unit_datum *unit = unit_get(object_index);

				SET_FLAG(
					unit->object.damage_flags,
					_object_melee_attack_inhibited_bit,
					TRUE);
				object_index = unit->unit.swarm_next_unit_index;
			}
		}
		else
		{
			struct unit_datum *unit = unit_get(actor->meta.unit_index);

			SET_FLAG(
				unit->unit.flags,
				7,
				berserk);
		}

		if (berserk)
			actor->emotions.forced_to_charge = TRUE;
	}

	return;
}

long actor_perception_find_killer_prop_index(
	long actor_index,
	long prop_index,
	boolean enemies_only)
{
	struct prop_datum *prop = prop_get(prop_index);
	struct unit_datum *unit = unit_get(prop->unit_index);
	long killer_prop_index = NONE;
	long most_recent_damage_time = 0;
	short attacker_index;

	for (attacker_index = 0;
		attacker_index < MAXIMUM_ATTACKERS_PER_UNIT;
		attacker_index++)
	{
		long *attacker_object_index =
			&unit->unit.attackers[attacker_index].object_index;
		long damage_time = attacker_object_index[-2];
		long unit_index =
			ai_get_responsible_unit(
				*attacker_object_index,
				TRUE);

		if (unit_index != NONE)
		{
			long current_prop_index =
				prop_get_active_by_unit_index(
					actor_index,
					unit_index);

			if (current_prop_index != NONE)
			{
				struct prop_datum *current_prop =
					prop_get(current_prop_index);

				if (current_prop->state >=
						_prop_state_becoming_unacknowledged &&
					current_prop->state <=
						_prop_state_acknowledged &&
					(current_prop->enemy || !enemies_only) &&
					damage_time > most_recent_damage_time)
				{
					killer_prop_index = current_prop_index;
					most_recent_damage_time = damage_time;
				}
			}
		}
	}

	return killer_prop_index;
}

long actor_perception_find_recent_damaging_prop_index(
	long actor_index,
	boolean enemies_only)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	long damaging_prop_index = NONE;

	if (actor->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(actor->unit_index);
		unsigned long most_recent_damage_time = 0;
		short attacker_index;

		for (attacker_index = 0;
			attacker_index < MAXIMUM_ATTACKERS_PER_UNIT;
			attacker_index++)
		{
			struct unit_attacker *attacker =
				&unit->unit.attackers[attacker_index];
			long unit_index =
				ai_get_responsible_unit(
					attacker->object_index,
					TRUE);

			if (unit_index != NONE)
			{
				long current_prop_index =
					prop_get_active_by_unit_index(
						actor_index,
						unit_index);

				if (current_prop_index != NONE)
				{
					struct prop_datum *current_prop =
						prop_get(current_prop_index);

					if (current_prop->state >=
							_prop_state_becoming_unacknowledged &&
						current_prop->state <=
							_prop_state_acknowledged &&
						(current_prop->enemy || !enemies_only) &&
						attacker->game_time_stamp >
							most_recent_damage_time)
					{
						damaging_prop_index =
							current_prop_index;
						most_recent_damage_time =
							attacker->game_time_stamp;
					}
				}
			}

		}

#line 3726 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		assert(damaging_prop_index != 0x00000000);
#line 500 "source\\ai\\actor_perception.c"
	}

	return damaging_prop_index;
}

void actor_perception_forget_recent_damage(
	long actor_index)
{
	struct prop_iterator iterator;
	struct prop_datum *prop;

	prop_iterator_new(&iterator, actor_index);
	prop = prop_iterator_next(&iterator);
	while (prop != NULL)
	{
		prop->currently_damaging_me = FALSE;
		prop->ticks_since_damage = NONE;
		prop = prop_iterator_next(&iterator);
	}

	return;
}

void actor_perception_retreat_successful(
	long actor_index)
{
	struct prop_iterator iterator;
	struct prop_datum *prop;

	actor_get(actor_index);
	prop_iterator_new(&iterator, actor_index);
	prop = prop_iterator_next(&iterator);
	while (prop != NULL)
	{
		prop->unopposable_trigger_hysteresis = 0;
		prop->unopposable_trigger_threshold = 0;
		prop->unopposable_trigger_timer = 0;
		prop = prop_iterator_next(&iterator);
	}

	return;
}

boolean actor_compute_prop_unopposable(
	long actor_index,
	long prop_index)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	struct actor_perception_prop_view *prop =
		(struct actor_perception_prop_view *)prop_get(prop_index);
	short type = prop->state;
	boolean result = FALSE;

	if (type >= 2 && type <= 3 &&
		prop->enemy &&
		!prop->dead)
	{
		if (prop->unreachable_ticks != 0 &&
			(actor->target_prop_index == prop_index ||
				!actor->searching))
		{
			result = TRUE;
		}
		else if ((prop->vehicle_gunner ||
			prop->dangerous_vehicle_driver) &&
			!actor->flying &&
			!actor->vehicle_passenger)
		{
			result = TRUE;
		}
		else if (prop->type == 15)
		{
			result = TRUE;
		}
	}

	if (prop->unopposable_enemy && !result)
	{
		prop->unopposable_trigger_hysteresis = 0;
		prop->unopposable_trigger_threshold = 0;
		prop->unopposable_trigger_timer = 0;
	}

	if (type >= 2 && type <= 3 &&
		!result &&
		actor->unopposable_retreat_timer > 0 &&
		actor->unopposable_retreat_prop_index == prop_index)
	{
		actor->unopposable_retreat_timer = 0;
		actor->unopposable_retreat_prop_index = NONE;
	}

	prop->unopposable_enemy = result;

	return result;
}

void actor_perception_find_prop_pathfinding_location(
	long actor_index,
	long prop_index)
{
	struct actor_perception_prop_view *prop =
		(struct actor_perception_prop_view *)prop_get(prop_index);

#line 3585 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	assert(prop->owner_actor_index == actor_index);
#line 510 "source\\ai\\actor_perception.c"

	if (prop->pathfinding_surface_index == NONE)
	{
		if (prop->vehicle_index != NONE)
		{
			prop->pathfinding_surface_index =
				vehicle_find_pathfinding_surface_index(
					prop->vehicle_index,
					&prop->pathfinding_point);
			return;
		}

		if (biped_try_and_get(prop->unit_index))
		{
			prop->pathfinding_surface_index =
				biped_find_pathfinding_surface_index(
					prop->unit_index,
					&prop->pathfinding_point);
		}
	}

	return;
}

void actor_perception_tried_to_uncover(
	long actor_index,
	long prop_index)
{
	if (prop_index != NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct prop_datum *prop = prop_get(prop_index);

		prop->tried_to_uncover = TRUE;
		if (prop_index == actor->target.target_prop_index)
		{
			actor_situation_update_target_status(actor_index);
			actor_situation_combat_status_update(actor_index);
		}
	}

	return;
}

void actor_perception_tried_to_search(
	long actor_index,
	long prop_index)
{
	if (prop_index != NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct prop_datum *prop = prop_get(prop_index);

		prop->tried_to_search = TRUE;
		if (prop_index == actor->target.target_prop_index)
		{
			actor_situation_update_target_status(actor_index);
			actor_situation_combat_status_update(actor_index);
		}
	}

	return;
}

void actor_perception_abandoned_search(
	long actor_index,
	long prop_index)
{
	if (prop_index == NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);

		actor->firing_positions.pursuit_positions_count = 0;
		actor->firing_positions.pursuit_fired_at_orphan = FALSE;
		actor->firing_positions.pursuit_communicated_lost_contact = FALSE;
		actor->state.artificial_combat_status = 0;
		actor->state.suspicion_combat_status = 0;
		actor_situation_combat_status_update(actor_index);
	}
	else
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct prop_datum *prop = prop_get(prop_index);

		if (prop->state == _prop_state_uninspected_orphan)
			prop->state = _prop_state_inspected_orphan;

		prop->abandoned_search = TRUE;
		if (prop_index == actor->target.target_prop_index)
		{
			actor_situation_update_target_status(actor_index);
			actor_situation_combat_status_update(actor_index);
		}
	}

	return;
}

void actor_emotion_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	short priority;

	if (actor->emotions.berserk &&
		(actor->state.combat_status == _actor_combat_status_none ||
			actor->state.mode < _actor_mode_combat ||
			(actor->input.shield_vitality == 1.0f &&
				actor->state.combat_status <
					_actor_combat_status_definite)))
	{
		actor_berserk(actor_index, FALSE);
	}

	if (actor->emotions.currently_defending !=
		actor->external_orders.defending)
	{
		boolean defending = actor->external_orders.defending;

		actor->emotions.currently_defending = defending;
		if (actor->meta.unit_index != NONE)
		{
			ai_communication_event(
				(defending != FALSE) + _ai_communication_advance,
				actor->meta.unit_index,
				NONE,
				NONE,
				NONE,
				NONE,
				NULL);
		}
	}

	actor->emotions.forced_to_charge =
		actor->emotions.berserk ||
		TEST_FLAG(
			definition->flags,
			_actor_definition_always_charge_bit);
	if (actor->input.vehicle_index != NONE)
	{
		actor->emotions.forced_to_charge = FALSE;
	}
	else if (TEST_FLAG(
			definition->flags,
			_actor_definition_charge_in_attacking_mode_bit) &&
		!actor->emotions.currently_defending)
	{
		actor->emotions.forced_to_charge = TRUE;
	}

	/* BUG (preserved for exact matching): the scan starts at
	 * NUMBER_OF_ACTOR_THREAT_TYPES rather than NUMBER_OF_ACTOR_THREAT_TYPES-1,
	 * so its first iteration reads specific_threats[9] - one element past the
	 * end of the nine-element array, which is cumulative_threats[0].
	 * Evidence: January emits `mov eax,9` at +0xf5, then the six-byte alignment
	 * nop 8d 9b 00 00 00 00 (`lea ebx,[ebx]`, which touches neither EAX nor
	 * memory), and falls into the loop head at +0x100, `movsx edx,ax` followed by
	 * `cmp byte ptr [edx+esi+0x1ee],0`; the `dec eax` is at +0x10d, AFTER that
	 * load. In the same object actor_situation_update increments
	 * cumulative_threats for threat types 1..8 at esi+0x1f8..esi+0x1ff and
	 * area_friends at esi+0x200, which fixes cumulative_threats at esi+0x1f7 and
	 * so specific_threats at esi+0x1ee with exactly nine elements.
	 * Layout is asserted, not assumed: see
	 * actor_perception_specific_threats_size_assert and
	 * actor_perception_threat_arrays_adjacent_assert above, which fail to compile
	 * if the two arrays ever stop being adjacent or change length.
	 * Consequence: none observable. cumulative_threats[_actor_threat_none] is the
	 * one element of that array nothing ever writes - every write in the tree
	 * uses _actor_threat_visible..._actor_threat_damaging_me, i.e. 1..8, and the
	 * single variable-index use is a read-only csprintf argument in the
	 * actor_debug_print_threat macro - so the stray byte is always zero, the
	 * early-out is never taken on the stray iteration, and the scan falls through
	 * to the intended starting index. */
	for (priority = NUMBER_OF_ACTOR_THREAT_TYPES;
		priority > 0 &&
			actor->situation.specific_threats[priority] <= 0;
		priority--)
	{
	}

	if (priority >= _actor_threat_damaging_me)
		actor->emotions.instantaneous_danger = 2.0f;
	else if (priority >= _actor_threat_extremely_close_to_me)
		actor->emotions.instantaneous_danger = 1.8f;
	else if (priority >= _actor_threat_shooting_at_me)
		actor->emotions.instantaneous_danger = 1.6f;
	else if (priority >= _actor_threat_shooting_near_me)
		actor->emotions.instantaneous_danger = 1.2f;
	else if (priority >= _actor_threat_visible_aiming_at_me)
		actor->emotions.instantaneous_danger = 0.7f;
	else
		actor->emotions.instantaneous_danger = 0.0f;

	{
		real interpolation =
			1.0f -
			(real)exp(-0.046209812164306640625);

		actor->emotions.perceived_danger =
			(actor->emotions.instantaneous_danger -
				actor->emotions.perceived_danger) *
				interpolation +
			actor->emotions.perceived_danger;
	}

	if (actor->external_orders.stand_down)
	{
		actor->emotions.original_body_vitality =
			actor->input.body_vitality;
	}

	if (definition->flags &
		(FLAG(_actor_definition_crouch_in_line_of_fire_bit) |
			FLAG(_actor_definition_avoid_friend_line_of_fire_bit)))
	{
		if (actor->input.vehicle_index == NONE &&
			actor->state.combat_status >=
				_actor_combat_status_definite)
		{
			boolean has_target_vector;
			real_vector3d attack_vector;
			struct prop_iterator iterator;
			struct prop_datum *prop;

			actor->emotions.crouch_blocking_line_of_fire = FALSE;
			actor->emotions.crouch_blocking_player_line_of_fire =
				FALSE;
			actor->emotions.crouch_friends_in_line_of_fire = FALSE;
			actor->emotions.moving_into_player_line_of_fire = FALSE;
			has_target_vector =
				actor->target.target_type > 8;

			if (has_target_vector)
			{
				struct prop_datum *target_prop =
					prop_get(actor->target.target_prop_index);

				attack_vector = target_prop->actor_to_prop;
			}

			prop_iterator_new(&iterator, actor_index);
			prop = prop_iterator_next(&iterator);
			while (prop != NULL)
			{
				if (prop->state >=
						_prop_state_becoming_unacknowledged &&
					prop->state <= _prop_state_acknowledged &&
					!prop->enemy &&
					!prop->dead &&
					!prop->swarm &&
					(prop->player ||
						prop->vehicle_index == NONE))
				{
					real_vector3d friend_attack_vector;

					if (actor_perception_friend_prop_is_attacking(
						actor_index,
						iterator.index,
						&friend_attack_vector))
					{
						real_vector3d vector_to_line_of_fire;
						short blockage =
							actor_perception_aiming_vector_test_blockage(
								&prop->body_position,
								&friend_attack_vector,
								&actor->input.position.body_position,
								&vector_to_line_of_fire);

						if (blockage >= 1)
						{
							actor->emotions.
								crouch_blocking_line_of_fire = TRUE;
							if (prop->player)
							{
								actor->emotions.
									crouch_blocking_player_line_of_fire =
										TRUE;
							}
						}

						if (TEST_FLAG(
								definition->flags,
								_actor_definition_avoid_friend_line_of_fire_bit) &&
							prop->player &&
							prop->shooting &&
							magnitude_squared3d(&vector_to_line_of_fire) < 1.0f &&
							(actor->control.moving ||
								actor->emotions.
									moving_into_fire_timer > 0))
						{
							real_vector3d movement_direction =
								actor->control.moving_towards_vector;

							if (normalize3d(&movement_direction) > 0.0f)
							{
								real_point3d future_point;
								short secondary_blockage;
								real dot;
								real threshold;

								point_from_line3d(
									&actor->input.position.body_position,
									&movement_direction,
									0.4f,
									&future_point);
								secondary_blockage =
									actor_perception_aiming_vector_test_blockage(
									&prop->body_position,
									&friend_attack_vector,
									&future_point,
										NULL);
								if (secondary_blockage <= blockage)
									secondary_blockage = blockage;
								if (secondary_blockage >= 1)
								{
									dot =
									dot_product3d(
										&movement_direction,
										&vector_to_line_of_fire);
								threshold =
									magnitude_squared3d(&vector_to_line_of_fire) < 0.25f ?
											0.0f :
											0.8660253882408142f;
									if (dot > threshold)
									{
										actor->emotions.
											moving_into_player_line_of_fire =
												TRUE;
									}
								}
							}
						}
					}

					if (has_target_vector &&
						actor_perception_aiming_vector_test_blockage(
							&actor->input.position.body_position,
							&attack_vector,
							&prop->body_position,
							NULL) >= 2)
					{
						actor->emotions.
							crouch_friends_in_line_of_fire = TRUE;
					}
				}

				prop = prop_iterator_next(&iterator);
			}
		}
		else
		{
			actor->emotions.crouch_blocking_line_of_fire = FALSE;
			actor->emotions.crouch_blocking_player_line_of_fire =
				FALSE;
			actor->emotions.crouch_friends_in_line_of_fire = FALSE;
			actor->emotions.moving_into_player_line_of_fire = FALSE;
		}
	}

	if (actor->emotions.moving_into_player_line_of_fire)
	{
		actor_discard_firing_position(
			actor_index,
			actor->firing_positions.current_position_index,
			TRUE);
		actor->emotions.moving_into_fire_timer = 22;
	}
	else if (actor->emotions.moving_into_fire_timer > 0)
	{
		actor->emotions.moving_into_fire_timer--;
	}

	if (actor->emotions.defensive_crouch_timer > 0)
	{
		actor->emotions.defensive_crouch_timer--;
	}
	else
	{
		real threshold;
		boolean crouch;

		if (!actor->emotions.currently_defending ||
			actor->emotions.berserk)
		{
			threshold =
				definition->defensive.
					defensive_threshold_attacking;
		}
		else
		{
			threshold =
				definition->defensive.
					defensive_threshold_defending;
		}

		switch (definition->defensive.defensive_crouch_type)
		{
		case _defensive_crouch_danger:
			crouch =
				actor->emotions.perceived_danger > threshold;
			break;

		case _defensive_crouch_shield_low:
			crouch =
				actor->input.shield_vitality < threshold;
			break;

		case _defensive_crouch_hide_behind_shield:
			crouch =
				actor->input.shield_vitality > threshold &&
				actor->situation.cumulative_threats[
					_actor_threat_visible_facing_me] > 0;
			break;

		case _defensive_crouch_any_target:
			crouch = actor->state.combat_status > 0;
			break;

		case _defensive_crouch_flood_shamble:
			crouch = actor_type_flood_desire_shamble(actor_index);
			break;

		default:
			crouch = FALSE;
			break;
		}

		if (TEST_FLAG(
			definition->flags,
			_actor_definition_crouch_in_line_of_fire_bit))
		{
			if (actor->emotions.crouch_blocking_player_line_of_fire)
				crouch = TRUE;
			else if (actor->emotions.crouch_friends_in_line_of_fire)
				crouch = FALSE;
			else if (actor->emotions.crouch_blocking_line_of_fire)
				crouch = TRUE;
		}

		if (actor->emotions.defensive_crouch && !crouch)
		{
			actor->emotions.defensive_crouch = FALSE;
			if (definition->defensive.
					defensive_crouch_min_stand_time > 0.0f)
			{
				actor->emotions.defensive_crouch_timer =
					(short)(definition->defensive.
						defensive_crouch_min_stand_time *
						30.0f);
			}
			else
			{
				actor->emotions.defensive_crouch_timer = 45;
			}
		}
		else if (!actor->emotions.defensive_crouch && crouch)
		{
			actor->emotions.defensive_crouch = TRUE;
			if (definition->defensive.
					defensive_crouch_min_crouch_time > 0.0f)
			{
				actor->emotions.defensive_crouch_timer =
					(short)(definition->defensive.
						defensive_crouch_min_crouch_time *
						30.0f);
			}
			else
			{
				actor->emotions.defensive_crouch_timer = 45;
			}
		}
	}

	if (actor->emotions.evasion_delay_timer > 0)
		actor->emotions.evasion_delay_timer--;

	actor_emotion_unopposable_retreat(actor_index);

	return;
}


void actor_perception_unreachable(
	long actor_index,
	long prop_index,
	boolean unreachable)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);

	if (unreachable)
	{
		if (prop->unreachable_ticks == 0)
			prop->unreachable_ticks = 1;

		prop->last_unreachable_time = game_time_get();
	}
	else
	{
		prop->unreachable_ticks = 0;
		prop->last_unreachable_time = NONE;
	}

	prop->unopposable_enemy =
		actor_compute_prop_unopposable(actor_index, prop_index);
	prop->target_weight =
		actor_compute_prop_target_weight(actor_index, prop_index);

	return;
}

boolean actor_perception_become_acknowledged(
	long actor_index,
	long prop_index,
	boolean *expected_acknowledgement_out)
{
	struct prop_datum *prop = prop_get(prop_index);
	boolean result = FALSE;
	boolean expected_acknowledgement = FALSE;

	if (prop->state < _prop_state_becoming_unacknowledged ||
		prop->state > _prop_state_acknowledged)
	{
		boolean had_orphan = prop->orphan_prop_index != NONE;

		expected_acknowledgement =
			actor_expected_acknowledgement(actor_index, prop_index);

		if (had_orphan)
		{
			struct prop_datum *orphan = prop_get(prop->orphan_prop_index);

			prop->target_weight = orphan->target_weight;
			prop->look_interest = orphan->look_interest;
			prop->last_idle_look_interest =
				orphan->last_idle_look_interest;
			prop->last_idle_look_time = orphan->last_idle_look_time;
			prop->unreachable_ticks = orphan->unreachable_ticks;
			prop->last_unreachable_time = orphan->last_unreachable_time;
			prop->unopposable_enemy = orphan->unopposable_enemy;
			prop->unopposable_casualties_inflicted =
				orphan->unopposable_casualties_inflicted;
			prop->unopposable_casualty_decay_timer =
				orphan->unopposable_casualty_decay_timer;

			actor_switch_props(
				actor_index,
				prop->orphan_prop_index,
				prop_index);
			prop_delete(actor_index, prop->orphan_prop_index);
			prop->orphan_prop_index = NONE;
		}

		prop->state = _prop_state_acknowledged;
		actor_perception_acknowledge(
			actor_index,
			prop_index,
			had_orphan,
			expected_acknowledgement);
		result = TRUE;
	}

	if (expected_acknowledgement_out)
		*expected_acknowledgement_out = expected_acknowledgement;

	return result;
}

void prop_status_refresh(
	long actor_index,
	long prop_index,
	struct actor_position_data *position)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->meta.active)
	{
		struct actor_definition *definition =
			actor_definition_get(actor->meta.definition_index);
		struct encounter_datum *encounter =
			actor->meta.encounter_index == NONE ?
				NULL :
				encounter_get(actor->meta.encounter_index);
		struct prop_datum *prop = prop_get(prop_index);
		struct unit_datum *unit = unit_get(prop->unit_index);
		long game_time = game_time_get();
		boolean blind =
			(encounter != NULL && encounter->blind) ||
			actor->state.mode == _actor_mode_asleep;
		short previous_quantized_speed;
		real_vector3d velocity;
		real_vector3d relative_velocity;
		real speed;
		real closing_speed;
		real facing;
		real facing_distance;

		prop->ignore = TEST_FLAG(unit->unit.flags, _unit_ignored_by_actors_bit);
		if (prop->player &&
			game_connection() == _game_connection_local &&
			ai_debug.ignore_player)
		{
			prop->ignore = TRUE;
		}

		if (prop->enemy)
		{
			prop->preferred_target = TEST_FLAG(unit->unit.flags, _unit_preferred_target_bit);

			switch (actor->external_orders.desired_target_type)
			{
			case _desired_target_ai:
				if (prop->actor_index != NONE &&
					actor->external_orders.desired_target_ai_index != NONE)
				{
					struct actor_datum *prop_actor = actor_get(prop->actor_index);

					if (DATUM_INDEX_TO_ABSOLUTE_INDEX(prop_actor->meta.encounter_index) == DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->external_orders.desired_target_ai_index))
					{
						switch ((unsigned long)actor->external_orders.desired_target_ai_index >> 30)
						{
						case _ai_reference_type_encounter:
							prop->preferred_target = TRUE;
							break;

						/* BUG (preserved for exact matching): January compares a platoon
						 * reference with the actor's squad index (actor+0x3a) and a squad
						 * reference with its platoon index (actor+0x3c), the reverse of
						 * actor_action_handle_vehicle_entry. A corrected build should compare
						 * platoon references with meta.platoon_index and squad references with
						 * meta.squad_index. */
						case _ai_reference_type_squad:
							if ((short)(((unsigned long)actor->external_orders.desired_target_ai_index >> 16) & UNSIGNED_CHAR_MAX) ==
								prop_actor->meta.platoon_index)
							{
								prop->preferred_target = TRUE;
							}
							break;

						case _ai_reference_type_platoon:
							if ((short)(((unsigned long)actor->external_orders.desired_target_ai_index >> 16) & UNSIGNED_CHAR_MAX) ==
								prop_actor->meta.squad_index)
							{
								prop->preferred_target = TRUE;
							}
							break;
						}
					}
				}
				break;

			case _desired_target_player:
				if (prop->player)
				{
					prop->preferred_target = TRUE;
				}
				break;
			}
		}

		previous_quantized_speed = prop->quantized_speed;
		object_get_velocities(prop->unit_index, &velocity, NULL);

		speed = magnitude3d(&velocity);
		if (speed < 0.1f / TICKS_PER_SECOND)
		{
			prop->quantized_speed = 0;
		}
		else if (speed < 0.5f / TICKS_PER_SECOND)
		{
			prop->quantized_speed = 1;
		}
		else if (speed < 1.0f / TICKS_PER_SECOND)
		{
			prop->quantized_speed = 2;
		}
		else
		{
			prop->quantized_speed = 3;
		}

		closing_speed =
			-dot_product3d(
				subtract_vectors3d(&velocity, &position->velocity, &relative_velocity),
				&prop->actor_to_prop);
		if (closing_speed < -1.0f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 0;
		}
		else if (closing_speed < -0.5f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 1;
		}
		else if (closing_speed < -0.1f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 2;
		}
		else if (closing_speed < 0.1f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 3;
		}
		else if (closing_speed < 0.5f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 4;
		}
		else if (closing_speed < 1.0f / TICKS_PER_SECOND)
		{
			prop->quantized_closing_speed = 5;
		}
		else
		{
			prop->quantized_closing_speed = 6;
		}

		if (prop_acknowledged(prop) &&
			previous_quantized_speed <= 1 &&
			prop->quantized_speed > 1)
		{
			struct direction_specification direction;

			direction.type = _direction_specification_prop;
			direction.prop_index = prop_index;
			actor_look_secondary(
				actor_index,
				_secondary_look_started_moving_prop,
				_secondary_look_priority_default,
				&direction);
		}

		if (prop->distance < 1.0f)
		{
			prop->quantized_distance = 0;
		}
		else if (prop->distance < 6.0f)
		{
			prop->quantized_distance = 1;
		}
		else if (prop->distance < 10.0f)
		{
			prop->quantized_distance = 2;
		}
		else if (prop->distance < 30.0f)
		{
			prop->quantized_distance = 3;
		}
		else
		{
			prop->quantized_distance = 4;
		}

		unit_get_aiming_vector(prop->unit_index, &velocity);
		facing = -dot_product3d(&velocity, &prop->actor_to_prop);
		if (facing <= 0.0f)
		{
			facing_distance = REAL_MAX;
		}
		else if (facing >= 1.0f)
		{
			facing_distance = 0.0f;
		}
		else
		{
			facing_distance = square_root(1.0f - facing * facing) * prop->distance;
		}

		if (facing > 0.99250001f || facing_distance < 0.5f)
		{
			prop->quantized_facing = 0;
		}
		else if (facing > 0.90630001f || facing_distance < 1.5f)
		{
			prop->quantized_facing = 1;
		}
		else if (facing > 0.5f)
		{
			prop->quantized_facing = 2;
		}
		else if (facing > 0.0f)
		{
			prop->quantized_facing = 3;
		}
		else
		{
			prop->quantized_facing = 4;
		}

		prop->shooting = prop->unit_effect == _ai_unit_effect_shooting;

		if (prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan)
		{
			prop->line_of_sight =
				ai_test_line_of_sight(
					&position->head_position,
					position->body_location.cluster_index,
					&prop->head_position,
					prop->body_location.cluster_index,
					prop->player && prop->enemy ? _ai_line_of_sight_expand_target : _ai_line_of_sight_normal,
					FALSE,
					prop->vehicle_index,
					actor->input.vehicle_index != NONE);

			if (prop->ignore || blind)
			{
				prop->perception = _actor_perception_none;
				prop->ineffability = _actor_perception_none;
				prop->audibility = _actor_perception_none;
				prop->visibility = _actor_perception_none;
			}
			else
			{
				prop->visibility =
					actor_visibility_at_point(
						actor_index,
						position,
						&prop->head_position,
						prop->lighting,
						prop->line_of_sight,
						TRUE,
						FALSE,
						_actor_knowledge_searching);
				prop->audibility = _actor_perception_none;
				prop->ineffability = _actor_perception_none;
				prop->perception = prop->visibility;
			}
		}
		else
		{
			boolean noticed = FALSE;
			boolean dead;
			boolean really_dead;
			long unit_actor_index;
			boolean swarm;
			boolean noncombat;
			boolean in_combat;
			boolean fighting;

			prop->line_of_sight =
				ai_test_line_of_sight(
					&position->head_position,
					position->body_location.cluster_index,
					&prop->head_position,
					prop->body_location.cluster_index,
					prop->player && prop->enemy ? _ai_line_of_sight_expand_target : _ai_line_of_sight_normal,
					FALSE,
					prop->vehicle_index,
					actor->input.vehicle_index != NONE);
			prop->lighting = _prop_lighting_bright;

			if (unit->object.type == _object_type_biped)
			{
				struct biped_definition *biped_definition =
					biped_definition_get(unit->definition_index);

				prop->flying = TEST_FLAG(biped_definition->biped.flags, _biped_flying_bit);
			}
			else
			{
				prop->flying = FALSE;
			}

			prop->active_camouflage = unit->unit.active_camouflage > 0.5f;
			prop->flashlight = TEST_FLAG(unit->unit.flags, _unit_integrated_light_on_bit);

			dead = TEST_FLAG(unit->object.damage_flags, _object_dead_bit);
			really_dead = dead && unit->unit.feign_death_timer == 0;
			prop->just_killed = dead && !prop->dead;
			prop->dead = dead;
			prop->really_dead = really_dead;

			if (prop->just_killed &&
				!prop->enemy &&
				actor->state.mode < _actor_mode_combat)
			{
				noticed = TRUE;
			}

			if (dead)
			{
				prop->required_ticks = 0;
			}

			unit_actor_index = unit->unit.swarm_actor_index;
			if (unit_actor_index != NONE)
			{
				swarm = TRUE;
			}
			else
			{
				unit_actor_index = unit->unit.actor_index;
				swarm = FALSE;
			}

			if (unit_actor_index != prop->actor_index)
			{
				prop->swarm = swarm;
				prop->actor_index = unit_actor_index;
				if (prop->orphan_prop_index != NONE)
				{
					struct prop_datum *orphan = prop_get(prop->orphan_prop_index);

					orphan->actor_index = prop->actor_index;
					orphan->swarm = prop->swarm;
				}
			}

			if (unit_actor_index == NONE)
			{
				noncombat = FALSE;
				in_combat = FALSE;
				fighting = !prop->dead;
			}
			else
			{
				noncombat = actor_is_noncombat(prop->actor_index);
				in_combat = actor_in_combat(prop->actor_index);
				fighting = actor_is_fighting(prop->actor_index);

				if (in_combat &&
					!prop->in_combat &&
					!prop->enemy &&
					actor->state.mode < _actor_mode_combat)
				{
					noticed = TRUE;
				}
			}

			prop->noncombat = noncombat;
			prop->in_combat = in_combat;
			prop->fighting = fighting;

			if (noticed)
			{
				short visibility = _actor_perception_none;

				if (!blind)
				{
					char lighting = prop->flashlight ? _prop_lighting_bright : prop->lighting;

					visibility =
						actor_visibility_at_point(
							actor_index,
							position,
							&prop->head_position,
							lighting,
							prop->line_of_sight,
							TRUE,
							FALSE,
							actor_get_perception_knowledge(actor_index, prop_index));
				}

				if (visibility < _actor_perception_full)
				{
					prop->perception = visibility;
					prop->visibility = visibility;
					prop->state = _prop_state_unacknowledged;
				}
			}

			if (prop->ignore)
			{
				prop->perception = _actor_perception_none;
				prop->ineffability = _actor_perception_none;
				prop->audibility = _actor_perception_none;
				prop->visibility = _actor_perception_none;
			}
			else
			{
				boolean invisible = blind;

				if ((game_connection() == _game_connection_local && ai_debug.blind) ||
					(game_connection() == _game_connection_local && ai_debug.invisible_player && prop->player))
				{
					invisible = TRUE;
				}

				if (prop->active_camouflage)
				{
					invisible = prop->enemy || (prop->player && prop->distance > 4.0f);
				}

				if (invisible)
				{
					prop->visibility = _actor_perception_none;
					prop->just_became_visible = FALSE;
				}
				else
				{
					boolean use_maximum_distance = TRUE;
					char lighting;
					short visibility;

					if (actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying ||
						actor->meta.type == _actor_mounted_weapon)
					{
						use_maximum_distance = FALSE;
					}
					else if (!prop->enemy)
					{
						use_maximum_distance = FALSE;
						if (actor->state.mode < _actor_mode_combat &&
							(prop->dead || prop->in_combat))
						{
							use_maximum_distance = TRUE;
						}
					}
					else if (prop_acknowledged(prop))
					{
						use_maximum_distance = FALSE;
					}

					lighting = prop->flashlight ? _prop_lighting_bright : prop->lighting;
					visibility =
						actor_visibility_at_point(
							actor_index,
							position,
							&prop->head_position,
							lighting,
							prop->line_of_sight,
							use_maximum_distance,
							prop->player,
							actor_get_perception_knowledge(actor_index, prop_index));
					prop->just_became_visible =
						prop->visibility == _actor_perception_none &&
						visibility > _actor_perception_none;
					prop->visibility = visibility;
					if (visibility != _actor_perception_none)
					{
						prop->last_visible_head_position = prop->head_position;
						prop->last_visible_time = game_time;
					}
				}

				if ((game_connection() == _game_connection_local && ai_debug.deaf) ||
					(encounter != NULL && encounter->deaf))
				{
					prop->audibility = _actor_perception_none;
				}
				else if (prop->unit_effect == _ai_unit_effect_shooting ||
					prop->unit_effect == _ai_unit_effect_death_scream)
				{
					prop->audibility = _actor_perception_unmistakable;
				}
				else
				{
					long sound_unit_index =
						prop->vehicle_index != NONE ?
							prop->vehicle_index :
							prop->unit_index;
					struct unit_datum *sound_unit = unit_get(sound_unit_index);
					struct unit_definition *sound_unit_definition =
						unit_definition_get(sound_unit->definition_index);

					prop->audibility =
						actor_audibility_at_point(
							actor_index,
							position,
							&prop->body_position,
							&prop->body_location,
							sound_unit_definition->unit.constant_sound,
							1.0f,
							prop->line_of_sight);
				}

				prop->ineffability = _actor_perception_none;
				if (prop->unit_effect == _ai_unit_effect_bump)
				{
					prop->ineffability = _actor_perception_unmistakable;
				}

				if (prop->flashlight &&
					prop->quantized_facing <= 2 &&
					prop->quantized_distance <= 2 &&
					(prop->line_of_sight == _ai_line_of_sight_clear ||
						prop->line_of_sight == _ai_line_of_sight_occluded))
				{
					prop->ineffability = MAX(prop->ineffability, _actor_perception_partial);
				}

				prop->perception =
					MAX(prop->visibility, MAX(prop->audibility, prop->ineffability));
				if (prop->perception == _actor_perception_partial &&
					prop_acknowledged(prop))
				{
					prop->perception = _actor_perception_full;
				}
			}

			if (prop->perception != _actor_perception_none)
			{
				prop->last_perceived_body_position = prop->body_position;
				prop->last_perceived_time = game_time;
			}

			if (prop_acknowledged(prop))
			{
				struct actor_datum *source_actor;

				if (prop->visibility >= _actor_perception_full ||
					(prop->definitely_located &&
						prop->definite_knowledge_source_actor != NONE &&
						(source_actor = actor_try_and_get(prop->definite_knowledge_source_actor)) != NULL &&
						source_actor->target.target_type >= _actor_target_visible_enemy &&
						source_actor->target.target_prop_index != NONE &&
						source_actor->orders.combat.shoot_at_target &&
						prop_get(source_actor->target.target_prop_index)->unit_index == prop->unit_index))
				{
					prop->definitely_located = TRUE;
					prop->ticks_since_definitely_located = 0;
				}
			}
		}

		if (prop->dangerous_vehicle_driver)
		{
			actor_perception_assess_vehicle_danger(
				actor_index,
				prop->vehicle_index,
				prop->perception >= _actor_perception_full,
				position);
		}

		if (prop->suicide_radius > 0.0f &&
			(prop->dead || unit->unit.animation.state == _unit_state_melee_attack))
		{
			actor_perception_assess_suicide_danger(
				actor_index,
				prop->unit_index,
				prop->suicide_radius,
				prop->distance,
				prop->enemy,
				prop->perception >= _actor_perception_full);
		}

		if (prop->enemy &&
			prop_acknowledged(prop))
		{
			if ((actor_has_ranged_weapon(actor_index) &&
					prop->distance < actor->control.weapon_maximum_range) ||
				(TEST_FLAG(definition->flags, _actor_definition_suicidal_melee_attack_bit) &&
					prop->distance < definition->berserk.melee_attack_range))
			{
				actor_perception_unreachable(actor_index, prop_index, FALSE);
			}
		}

		if (prop->last_unreachable_time != NONE &&
			prop->last_unreachable_time + 150 < game_time)
		{
			actor_perception_unreachable(actor_index, prop_index, FALSE);
		}

		if (prop->delay_requirement_decision)
		{
			if (!actor_perception_desire_prop(
					actor_index,
					NONE,
					prop->unit_index,
					prop->actor_index,
					prop->in_use,
					prop->player,
					prop->enemy,
					prop->dead,
					prop->dead_ticks,
					prop->suicide_radius,
					actor_perception_distance_squared(prop->distance),
					0,
					NULL))
			{
				prop->required_ticks = 0;
			}

			prop->delay_requirement_decision = FALSE;
		}

		prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, prop_index);
		prop->target_weight = actor_compute_prop_target_weight(actor_index, prop_index);
		prop->look_interest = actor_look_compute_prop_interest(actor_index, prop_index);
		prop->refresh_stimuli = TRUE;
	}

	return;
}

boolean actor_expected_acknowledgement(
	long actor_index,
	long prop_index)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	struct actor_perception_prop_view *prop =
		(struct actor_perception_prop_view *)prop_get(prop_index);
	struct prop_iterator iterator;
	struct actor_perception_prop_view *current_prop;
	boolean result = FALSE;
	real delta_x;
	real delta_y;

#line 3613 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	match_vassert(
		__FILE__,
		__LINE__,
		!(prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan),
		"!prop_orphaned(prop)");
#line 790 "source\\ai\\actor_perception.c"

	prop_iterator_new(&iterator, actor_index);
	current_prop =
		(struct actor_perception_prop_view *)prop_iterator_next(&iterator);
	while (current_prop != NULL)
	{
		if (iterator.index != prop_index &&
			(current_prop->unit_index == prop->unit_index ||
				current_prop->actor_index == prop->actor_index ||
				(prop->enemy &&
					current_prop->enemy &&
					((current_prop->state >=
							_prop_state_uninspected_orphan &&
							current_prop->state <=
							_prop_state_inspected_orphan) ||
						(current_prop->state >=
							_prop_state_becoming_unacknowledged &&
							current_prop->state <=
							_prop_state_acknowledged)))) &&
			actor_perception_distance_squared2d(
				&current_prop->body_position,
				&prop->body_position,
				delta_x,
				delta_y) < 6.25f &&
			fabs(current_prop->body_position.z - prop->body_position.z) <
				1.5f &&
			dot_product3d(
				&current_prop->actor_to_prop,
				&prop->actor_to_prop) > 0.5f)
		{
			result = TRUE;
		}

		current_prop =
			(struct actor_perception_prop_view *)prop_iterator_next(
				&iterator);
	}

	return result;
}

void actor_perception_find_sense_position(
	long actor_index,
	real_point3d const *position,
	long unused,
	struct actor_position_data *sense_position)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->meta.swarm)
	{
		struct swarm_datum *swarm =
			swarm_get(actor->meta.swarm_cache_index);
		real best_distance_squared = FLT_MAX;
		long best_unit_index = NONE;
		short unit_index;

#line 1637 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		assert(actor->meta.swarm_unit_count > 0);
#line 1634 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			actor->meta.swarm_unit_index != NONE,
			"actor->meta.swarm_unit_index != NONE");
#line 970 "source\\ai\\actor_perception.c"

		for (unit_index = 0; unit_index < swarm->unit_count; unit_index++)
		{
			struct swarm_component_datum *component =
				swarm_component_get(swarm->component_indices[unit_index]);
			real distance_squared =
				distance_squared3d(&component->position, position);

			if (distance_squared < best_distance_squared)
			{
				best_distance_squared = distance_squared;
				best_unit_index = swarm->unit_indices[unit_index];
			}
		}

#line 1651 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			best_unit_index != NONE,
			"best_unit_index != NONE");
#line 990 "source\\ai\\actor_perception.c"

		actor_input_sample_position(
			actor_index,
			best_unit_index,
			sense_position);
	}
	else
	{
		*sense_position = actor->input.position;
	}

	return;
}

static long actor_perception_unit_from_swarm(
	long swarm_actor_index,
	long actor_index,
	long existing_unit_index,
	boolean mark,
	struct actor_position_data const *position)
{
	struct actor_datum *swarm_actor = actor_get(swarm_actor_index);
	long best_unit_index = NONE;

#line 1677 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	assert(swarm_actor->meta.swarm);
#line 1001 "source\\ai\\actor_perception.c"

	if (swarm_actor->meta.swarm_cache_index != NONE)
	{
		struct swarm_datum *swarm =
			swarm_get(swarm_actor->meta.swarm_cache_index);
		real best_distance_squared = FLT_MAX;
		short unit_index;

		for (unit_index = 0; unit_index < swarm->unit_count; unit_index++)
		{
			struct swarm_component_datum *component =
				swarm_component_get(swarm->component_indices[unit_index]);
			real distance_squared =
				distance_squared3d(
					&component->position,
					&position->body_position);

			if (TEST_FLAG(component->flags, _swarm_component_attached_to_unit_bit))
			{
				distance_squared *= 2.25f;
			}
			else if (swarm->unit_indices[unit_index] == existing_unit_index)
			{
				distance_squared *= 0.36f;
			}

			if (distance_squared < best_distance_squared)
			{
				best_distance_squared = distance_squared;
				best_unit_index = swarm->unit_indices[unit_index];
			}

			if (mark)
			{
				object_mark_function(swarm->unit_indices[unit_index]);
			}
		}
	}
	else
	{
		long unit_index = swarm_actor->meta.swarm_unit_index;
		real best_distance_squared = FLT_MAX;

		while (unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(unit_index);
			real_point3d origin;
			real distance_squared;

			object_get_origin(unit_index, &origin);
			distance_squared =
				distance_squared3d(&origin, &position->body_position);
			if (unit_index == existing_unit_index)
			{
				distance_squared *= 0.36f;
			}

			if (distance_squared < best_distance_squared)
			{
				best_distance_squared = distance_squared;
				best_unit_index = unit_index;
			}

			if (mark)
			{
				object_mark_function(unit_index);
			}

			unit_index = unit->unit.swarm_next_unit_index;
		}
	}

#line 1749 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	match_vassert(
		__FILE__,
		__LINE__,
		existing_unit_index == NONE || best_unit_index != NONE,
		"(existing_unit_index == NONE) || (best_unit_index != NONE)");
#line 1090 "source\\ai\\actor_perception.c"

	return best_unit_index;
}

void prop_position_refresh(
	long actor_index,
	long prop_index,
	struct actor_position_data *position,
	boolean refresh_position,
	boolean refresh_vehicle)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->meta.active)
	{
		long current_prop_index = prop_index;
		struct prop_datum *prop = prop_get(current_prop_index);
		struct unit_datum *unit = unit_get(prop->unit_index);

		if (!refresh_position &&
			prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan)
		{
			boolean corpse_stopped;

			if (prop->orphan_corpse_cheated)
			{
				goto update_actor_to_prop;
			}

			if (TEST_FLAG(unit->object.damage_flags, 2) &&
				((struct actor_perception_unit_view *)unit)
						->parent_seat_index == 0 &&
				prop->perception == 0 &&
				magnitude_squared3d(&unit->object.translational_velocity) <
					0.010000000707805157f)
			{
				corpse_stopped = TRUE;
			}
			else
			{
				corpse_stopped = FALSE;
			}

			if (actor->target.target_prop_index == current_prop_index &&
				(!prop->tried_to_uncover || !prop->tried_to_search))
			{
				goto update_actor_to_prop;
			}

			if (!corpse_stopped)
			{
				goto update_actor_to_prop;
			}

			prop->orphan_corpse_cheated = TRUE;
			prop->dead = TRUE;
		}

		if (prop->swarm &&
			prop->actor_index != NONE &&
			refresh_vehicle)
		{
			long current_time = game_time_get();

			if (prop->swarm_unit_selected_time + 90 <= current_time)
			{
				long new_unit_index;

				prop->swarm_unit_selected_time = current_time;
				new_unit_index =
					actor_perception_unit_from_swarm(
						prop->actor_index,
						actor_index,
						prop->unit_index,
						FALSE,
						&actor->input.position);
				if (new_unit_index != prop->unit_index)
				{
					prop->unit_index = new_unit_index;
					unit = unit_get(new_unit_index);
					if (prop->state >= _prop_state_uninspected_orphan &&
						prop->state <= _prop_state_inspected_orphan)
					{
						struct prop_datum *parent_prop =
							prop_get(prop->orphan_prop_index);
						parent_prop->unit_index = prop->unit_index;
					}
					else if (prop->orphan_prop_index != NONE)
					{
						struct prop_datum *parent_prop =
							prop_get(prop->orphan_prop_index);
						parent_prop->unit_index = prop->unit_index;
					}
				}
			}
		}

		unit_get_head_position(prop->unit_index, &prop->head_position);
		object_get_origin(prop->unit_index, &prop->body_position);
		unit_get_center_of_mass(prop->unit_index, &prop->center_of_mass);
		prop->velocity = unit->object.translational_velocity;
		prop->pathfinding_surface_index = NONE;
		prop->body_location =
			actor_perception_object_get(
				object_get_ultimate_parent(prop->unit_index))
				->object.location;
		prop->underwater =
			scenario_location_underwater(
				&prop->body_location,
				&prop->center_of_mass,
				NULL);
		prop->vehicle_index = NONE;
		prop->vehicle_gunner = FALSE;
		prop->dangerous_vehicle_driver = FALSE;
		prop->attached_to_unit_index = NONE;

		if (unit->object.parent_object_index != NONE)
		{
			struct object_datum *parent =
				actor_perception_object_get(
					unit->object.parent_object_index);

			if (parent->object.type == _object_type_vehicle)
			{
				struct unit_datum *vehicle = (struct unit_datum *)parent;

				prop->vehicle_index = unit->object.parent_object_index;
				prop->vehicle_gunner =
					vehicle->unit.gunner_object_index == prop->unit_index ||
					prop->type == 15;
				prop->dangerous_vehicle_driver =
					vehicle->unit.driver_object_index == prop->unit_index &&
					vehicle_causes_collision_damage(prop->vehicle_index);
			}
			else if (((1 << parent->object.type) & 3) != 0)
			{
				prop->attached_to_unit_index =
					unit->object.parent_object_index;
			}
		}

		prop->child_units_attached = 0;
		{
			long child_object_index = unit->object.first_child_object_index;

			while (child_object_index != NONE)
			{
				struct object_datum *child =
					actor_perception_object_get(child_object_index);

				if (((1 << child->object.type) & _object_mask_unit) != 0)
				{
					prop->child_units_attached++;
				}

				child_object_index = child->object.next_object_index;
			}
		}

update_actor_to_prop:
		actor_perception_find_sense_position(
			actor_index,
			&prop->body_position,
			current_prop_index,
			position);
		vector_from_points3d(
			&position->body_position,
			&prop->body_position,
			&prop->actor_to_prop);
		prop->distance = normalize3d(&prop->actor_to_prop);
		if (prop->distance == 0.0f)
		{
			prop->actor_to_prop = *global_forward3d;
		}
	}

	return;
}

static boolean actor_perception_assess_vehicle_danger(
	long actor_index,
	long vehicle_index,
	boolean mark,
	struct actor_position_data const *position)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	boolean result = FALSE;

	if (actor->vehicle_index == NONE)
	{
		struct unit_datum *vehicle = vehicle_get(vehicle_index);
		struct actor_perception_vehicle_definition_view *vehicle_definition =
			(struct actor_perception_vehicle_definition_view *)
				vehicle_definition_get(vehicle->definition_index);

		if (TEST_FLAG(vehicle_definition->danger_zone_flags, 7) &&
			magnitude_squared3d(&vehicle->object.translational_velocity) >
				0.0011111111380159855f)
		{
			real_point3d vehicle_origin;
			struct actor_position_data sampled_position;
			real distance;

			object_get_origin(vehicle_index, &vehicle_origin);
			if (position == NULL)
			{
				actor_perception_find_sense_position(
					actor_index,
					&vehicle_origin,
					NONE,
					&sampled_position);
				position = &sampled_position;
			}

			distance =
				distance3d(&position->body_position, &vehicle_origin);
			if (distance <
				vehicle_definition->bounding_radius + 10.0f)
			{
				if (actor->danger_zone.danger_type <
						_actor_danger_zone_vehicle ||
					(actor->danger_zone.danger_type ==
							_actor_danger_zone_vehicle &&
						actor->danger_zone.object_index !=
							vehicle_index &&
						distance <
							actor->danger_zone
								.current_distance_from_actor))
				{
					struct actor_perception_responsible_unit_view
						*responsible_unit;

					csmemset(
						&actor->danger_zone,
						0,
						sizeof(actor->danger_zone));
					actor->danger_zone.object_index = vehicle_index;
					actor->danger_zone.danger_type =
						_actor_danger_zone_vehicle;
					actor->danger_zone.owner_unit_index =
						vehicle->unit.driver_object_index;
					actor->danger_zone.danger_radius =
						vehicle_definition->bounding_radius;
					actor->danger_zone.initial_position = vehicle_origin;
					actor->danger_zone.initial_velocity =
						vehicle->object.translational_velocity;
					actor->danger_zone.acknowledgement_timer = 20;
					actor->danger_zone.currently_perceived = mark;
					actor->danger_zone.hostility = 0;

					if (actor->danger_zone.owner_unit_index != NONE)
					{
						responsible_unit =
							(struct actor_perception_responsible_unit_view *)
								unit_get(
									vehicle->unit
										.driver_object_index);
						if (!game_team_is_enemy(
								actor->team,
								responsible_unit->team))
						{
							actor->danger_zone.hostility = 1;
						}
					}

					result = TRUE;
				}
			}
		}
	}

	return result;
}

static void actor_perception_refresh_danger_zone(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct object_datum *object;
	struct actor_position_data position;
	boolean perceived;
	boolean attached_to_us;

	if (actor->danger_zone.danger_type <= _actor_danger_zone_none)
	{
		return;
	}

	object = object_try_and_get(actor->danger_zone.object_index);
	if (object == NULL)
	{
		actor->danger_zone.danger_type = _actor_danger_zone_none;
		return;
	}

#line 3227 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
	assert((actor->danger_zone.danger_type == _actor_danger_zone_projectile) || (actor->danger_zone.danger_type == _actor_danger_zone_vehicle) || (actor->danger_zone.danger_type == _actor_danger_zone_suicide));
#line 4986 "source\\ai\\actor_perception.c"

	object_get_origin(
		actor->danger_zone.object_index,
		&actor->danger_zone.position);
	actor_perception_find_sense_position(
		actor_index,
		&actor->danger_zone.position,
		NONE,
		&position);

	actor->danger_zone.velocity = object->object.translational_velocity;
	actor->danger_zone.current_distance_from_actor =
		distance3d(
			&position.body_position,
			&actor->danger_zone.position);
	midpoint3d(
		point_from_line3d(
			&actor->danger_zone.position,
			&actor->danger_zone.velocity,
			45.0f,
			&actor->danger_zone.predict_danger_position),
		&actor->danger_zone.position,
		&actor->danger_zone.bounding_sphere_center);
	actor->danger_zone.bounding_sphere_radius =
		distance3d(
			&actor->danger_zone.bounding_sphere_center,
			&actor->danger_zone.position) +
		actor->danger_zone.danger_radius;

	perceived = FALSE;
	attached_to_us = FALSE;

	switch (actor->danger_zone.danger_type)
	{
	case _actor_danger_zone_vehicle:
		{
			struct unit_definition *vehicle_definition =
				vehicle_definition_get(object->definition_index);

			if (magnitude_squared3d(&object->object.translational_velocity) <
					0.000044444444065040908f ||
				actor->danger_zone.current_distance_from_actor >
					vehicle_definition->object.bounding_radius + 10.0f)
			{
				actor->danger_zone.danger_type = _actor_danger_zone_none;
				break;
			}

			perceived = actor->danger_zone.currently_perceived;
			if (!perceived)
			{
				struct unit_datum *vehicle = (struct unit_datum *)object;
				long prop_index;

				if (vehicle->unit.driver_object_index != NONE &&
					(prop_index = prop_get_active_by_unit_index(
						actor_index,
						vehicle->unit.driver_object_index)) != NONE)
				{
					perceived =
						prop_get(prop_index)->perception >= _actor_perception_full;
				}
				else
				{
					struct encounter_datum *encounter =
						actor->meta.encounter_index == NONE ?
							NULL :
							encounter_get(actor->meta.encounter_index);
					boolean blind = FALSE;
					struct location const *location;
					short line_of_sight;

					if (actor->state.mode == _actor_mode_asleep ||
						(encounter != NULL && encounter->blind))
					{
						blind = TRUE;
					}

					if (object->object.parent_object_index == NONE)
					{
						location = &object->object.location;
					}
					else
					{
						long ultimate_parent_index =
							object_get_ultimate_parent(
								actor->danger_zone.object_index);

						location = &object_get(ultimate_parent_index)->object.location;
					}

					line_of_sight =
						ai_test_line_of_sight(
							&position.head_position,
							position.body_location.cluster_index,
							&actor->danger_zone.position,
							location->cluster_index,
							0,
							FALSE,
							actor->danger_zone.object_index,
							actor->input.vehicle_index != NONE);

					if (!blind &&
						actor_visibility_at_point(
							actor_index,
							&position,
							&actor->danger_zone.position,
							0,
							line_of_sight,
							TRUE,
							FALSE,
							actor_get_perception_knowledge(
								actor_index,
								NONE)) >= _actor_perception_full)
					{
						perceived = TRUE;
					}
					else if (actor_audibility_at_point(
							actor_index,
							&position,
							&actor->danger_zone.position,
							location,
							vehicle_definition->unit.constant_sound,
							1.0f,
							line_of_sight) >= _actor_perception_full)
					{
						perceived = TRUE;
					}
				}
			}
			break;
		}

	case _actor_danger_zone_projectile:
		{
			struct projectile_datum *projectile =
				(struct projectile_datum *)object;

			if (actor->meta.unit_index != NONE &&
				object->object.parent_object_index == actor->meta.unit_index)
			{
				attached_to_us = TRUE;
			}

#line 3257 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
			assert(object->object.type == _object_type_projectile);
#line 5133 "source\\ai\\actor_perception.c"

			if (projectile->projectile.detonation_timer > 0.0f &&
				projectile->projectile.detonation_timer_delta > 0.0f)
			{
				actor->danger_zone.projectile.time_until_explosion =
					(short)fast_ftol(
						(1.0f - projectile->projectile.detonation_timer) /
						projectile->projectile.detonation_timer_delta);
			}
			else
			{
				actor->danger_zone.projectile.time_until_explosion = NONE;
			}

			perceived =
				actor->danger_zone.currently_perceived ||
				attached_to_us;
			if (!perceived)
			{
				struct encounter_datum *encounter =
					actor->meta.encounter_index == NONE ?
						NULL :
						encounter_get(actor->meta.encounter_index);

				if (actor->state.mode != _actor_mode_asleep &&
					(encounter == NULL || !encounter->blind) &&
					actor->danger_zone.current_distance_from_actor <
						projectile_definition_get(object->definition_index)
							->projectile.ai_perception_radius)
				{
					short cluster_index = object->object.location.cluster_index;
					short line_of_sight;

					if (object->object.parent_object_index != NONE)
					{
						cluster_index =
							object_get(
								object_get_ultimate_parent(
									actor->danger_zone.object_index))
								->object.location.cluster_index;
					}

					line_of_sight =
						ai_test_line_of_sight(
							&position.head_position,
							position.body_location.cluster_index,
							&actor->danger_zone.position,
							cluster_index,
							0,
							FALSE,
							actor->danger_zone.object_index,
							actor->input.vehicle_index != NONE);

					if (actor_visibility_at_point(
							actor_index,
							&position,
							&actor->danger_zone.position,
							0,
							line_of_sight,
							TRUE,
							FALSE,
							actor_get_perception_knowledge(
								actor_index,
								NONE)) >= _actor_perception_full)
					{
						perceived = TRUE;
					}
				}
			}
			break;
		}

	case _actor_danger_zone_suicide:
		{
			short animation_state;
			short frames_remaining;

			perceived = actor->danger_zone.currently_perceived;
			if (!perceived)
			{
				long prop_index =
					prop_get_active_by_unit_index(
						actor_index,
						actor->danger_zone.object_index);

				if (prop_index != NONE)
				{
					perceived =
						prop_get(prop_index)->perception >= _actor_perception_full;
				}
			}

			frames_remaining =
				unit_get_animation_frames_remaining(
					actor->danger_zone.object_index,
					&animation_state);
			actor->danger_zone.suicide.time_until_death =
				animation_state == _unit_state_dying ?
					frames_remaining :
					NONE;
			break;
		}
	}

	if (perceived && !actor->danger_zone.currently_perceived)
	{
		struct direction_specification direction;

		direction.type = _direction_specification_danger;
		actor_look_secondary(
			actor_index,
			_secondary_look_dangerous_object,
			_secondary_look_priority_default,
			&direction);
	}

	actor->danger_zone.currently_perceived = perceived;
	actor->danger_zone.attached_to_us = attached_to_us;

	return;
}


/*
 * January caller skeleton used while reconstructing the full status refresh.
 * Keep the real call expression active so VC7 can derive code_00020780's
 * private EAX argument from its actual translation-unit context.
 */



static void actor_perception_refresh_test_object(
	long actor_index,
	long object_index,
	struct actor_perception_refresh_list *enemy_list,
	struct actor_perception_refresh_list *friend_list)
{
	struct
	{
		struct actor_datum *actor;
		struct object_datum *current_object;
	} pointers;
#define actor pointers.actor
#define current_object pointers.current_object

	actor = actor_get(actor_index);

	while (object_index != NONE)
	{
		current_object = actor_perception_object_get(object_index);

		if (object_mark_function(object_index))
		{
			if (current_object->object.type == _object_type_biped)
			{
				struct unit_datum *unit =
					(struct unit_datum *)current_object;
				struct actor_position_data position;
				struct actor_perception_refresh_list *list;
				struct unit_definition *unit_definition;
				real_point3d origin;
				real distance_squared;
				real suicide_radius;
				long unit_index = object_index;
				long unit_actor_index;
				long prop_index;
				short dead_ticks;
				boolean player;
				boolean enemy;
				boolean dead;
				boolean optional;

				object_get_origin(object_index, &origin);
				actor_perception_find_sense_position(
					actor_index,
					&origin,
					NONE,
					&position);

				if (unit->unit.swarm_actor_index != NONE)
				{
					unit_actor_index = unit->unit.swarm_actor_index;
					unit_index =
						actor_perception_unit_from_swarm(
							unit_actor_index,
							actor_index,
							NONE,
							TRUE,
							&position);
					if (unit_index != NONE)
					{
						unit = unit_get(unit_index);
						object_get_origin(unit_index, &origin);
					}
				}
				else
				{
					unit_actor_index = unit->unit.actor_index;
				}

				if (unit_index == NONE ||
					unit_actor_index == actor_index)
					goto object_done;

				unit_definition = unit_definition_get(unit->definition_index);
				player = unit->unit.player_index != NONE;
				enemy =
					game_team_is_enemy(
						actor->meta.team_index,
						unit->object.owner_team_index);

				dead =
					TEST_FLAG(unit->object.damage_flags, _object_dead_bit) &&
					unit->unit.feign_death_timer == 0;

				if (!dead)
					dead_ticks = 0;
				else if (unit->unit.time_of_death == NONE)
					dead_ticks = 0x7FFF;
				else
					dead_ticks =
						(short)game_time_get() -
						(short)unit->unit.time_of_death;

				suicide_radius = unit_definition->unit.ai_danger_radius;
				distance_squared =
					distance_squared3d(
						&position.body_position,
						&origin);

				if (suicide_radius > 0.0f &&
					(dead ||
						unit->unit.animation.state ==
							0x1E))
				{
					actor_perception_assess_suicide_danger(
						actor_index,
						unit_index,
						suicide_radius,
						square_root(distance_squared),
						enemy,
						FALSE);
				}

				if (actor_perception_desire_prop(
						actor_index,
						_prop_state_unacknowledged,
						unit_index,
						unit_actor_index,
						FALSE,
						player,
						enemy,
						dead,
						dead_ticks,
						suicide_radius,
						distance_squared,
						0,
						&optional))
				{
					list = enemy ? enemy_list : friend_list;

					if (optional)
					{
#line 2966 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
						assert(!dead);
#line 3258 "source\\ai\\actor_perception.c"

						if (list->entry_count < 128)
						{
							list->entries[list->entry_count].prop_index = NONE;
							list->entries[list->entry_count].unit_index =
								unit_index;
							list->entries[list->entry_count].priority =
								distance_squared;
							list->entry_count++;
						}
					}
					else
					{
						prop_index =
							prop_new_unacknowledged(
								actor_index,
								unit_index,
								enemy);
						if (prop_index != NONE)
						{
							prop_position_refresh(
								actor_index,
								prop_index,
								&position,
								FALSE,
								FALSE);
							if (!dead)
								list->accepted_count++;
						}
					}
				}
			}
			else if (current_object->object.type == _object_type_vehicle)
			{
				struct unit_datum *vehicle =
					(struct unit_datum *)current_object;

				if (vehicle->unit.driver_object_index == NONE)
				{
					actor_perception_assess_vehicle_danger(
						actor_index,
						object_index,
						FALSE,
						NULL);
				}
			}
			else if (current_object->object.type == _object_type_projectile)
			{
				struct actor_perception_projectile_datum_view *projectile =
					(struct actor_perception_projectile_datum_view *)
						current_object;
				struct projectile_definition *projectile_definition =
					projectile_definition_get(
						current_object->definition_index);

				if (projectile_definition->projectile.danger_radius > 0.0f &&
					(current_object->object.parent_object_index == NONE ||
						TEST_FLAG(projectile->attachment_flags, 5)))
				{
					struct actor_position_data position;
					real_point3d origin;
					real distance;

					object_get_origin(object_index, &origin);
					actor_perception_find_sense_position(
						actor_index,
						&origin,
						NONE,
						&position);
					distance =
						distance3d(
							&position.body_position,
							&origin);

					if (distance <
							projectile_definition->projectile.danger_radius +
								10.0f &&
						(actor->danger_zone.danger_type < 2 ||
							(actor->danger_zone.danger_type == 2 &&
								actor->danger_zone.object_index != object_index &&
								distance <
									actor->danger_zone
										.current_distance_from_actor)))
					{
						long owner_unit_index;
						struct object_datum *owner_object = NULL;

						csmemset(
							&actor->danger_zone,
							0,
							sizeof(actor->danger_zone));
						actor->danger_zone.danger_type = 2;
						actor->danger_zone.object_index = object_index;
						actor->danger_zone.danger_radius =
							projectile_definition->projectile.danger_radius;
						actor->danger_zone.initial_position = origin;
						actor->danger_zone.initial_velocity =
							current_object->object.translational_velocity;
						actor->danger_zone.acknowledgement_timer = 30;
						actor->danger_zone.currently_perceived = FALSE;
						actor->danger_zone.hostility = 0;

						owner_unit_index = NONE;
						if (current_object->object.owner_object_index != NONE)
						{
							owner_object =
								object_try_and_get(
									current_object->object.owner_object_index);
						}

						if (owner_object != NULL &&
							TEST_FLAG(
								_object_mask_unit,
								owner_object->object.type))
						{
							owner_unit_index =
								current_object->object.owner_object_index;
							if (actor->meta.unit_index == NONE ||
								owner_unit_index != actor->meta.unit_index)
							{
								if (!game_team_is_enemy(
									actor->meta.team_index,
									current_object->object.owner_team_index))
								{
									actor->danger_zone.hostility = 1;
								}
							}
							else
							{
								actor->danger_zone.hostility = 2;
							}
						}

						actor->danger_zone.owner_unit_index =
							owner_unit_index;
					}
				}
			}
		}

object_done:
		if (current_object->object.first_child_object_index != NONE)
		{
			actor_perception_refresh_test_object(
				actor_index,
				current_object->object.first_child_object_index,
				enemy_list,
				friend_list);
		}
		object_index = current_object->object.next_object_index;
	}

	return;

#undef actor
#undef current_object
}

boolean actor_perception_create_orphan_from_friend(
	long actor_index,
	long unit_index,
	long source_actor_index,
	long friend_prop_index)
{
	struct actor_orphan_prop_view *current_prop;
	struct actor_position_data position;
	long current_prop_index;
	long current_orphan_index;
	boolean result;

	result = TRUE;
	current_prop_index =
		prop_get_base_by_unit_index(
			actor_index,
			unit_index,
			TRUE,
			FALSE);

	if (current_prop_index == NONE)
		goto done;

	current_prop =
		(struct actor_orphan_prop_view *)prop_get(current_prop_index);
	if (current_prop->state >= _prop_state_becoming_unacknowledged &&
		current_prop->state <= _prop_state_acknowledged)
	{
		result = FALSE;
	}
	else if (current_prop->related_prop_index != NONE)
	{
		struct actor_orphan_prop_view *current_orphan;
		boolean refresh_position;

		current_orphan_index = current_prop->related_prop_index;
		current_orphan =
			(struct actor_orphan_prop_view *)
				prop_get(current_orphan_index);
		refresh_position = FALSE;

#line 3759 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_prop->state >= _prop_state_unacknowledged &&
				current_prop->state <= _prop_state_becoming_acknowledged,
			"prop_unacknowledged(current_prop)");
#line 3760 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_orphan->state >= _prop_state_uninspected_orphan &&
				current_orphan->state <= _prop_state_inspected_orphan,
			"prop_orphaned(current_orphan)");
#line 3762 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_prop->owner_actor_index == actor_index,
			"current_prop->owner_actor_index == actor_index");
#line 3763 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_orphan->owner_actor_index == actor_index,
			"current_orphan->owner_actor_index == actor_index");
#line 3764 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_prop->related_prop_index == current_orphan_index,
			"current_prop->orphan_prop_index == current_orphan_index");
#line 3765 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_orphan->related_prop_index == current_prop_index,
			"current_orphan->parent_prop_index == current_prop_index");
#line 545 "source\\ai\\actor_perception.c"

		if (friend_prop_index != NONE)
		{
			prop_orphan_update_information(
				actor_index,
				current_orphan_index,
				friend_prop_index);
			current_prop->unit_index = current_orphan->unit_index;
		}
		else
		{
			current_orphan->state = _prop_state_uninspected_orphan;
			current_orphan->orphan_inspection_ticks = 0;
			refresh_position = TRUE;
		}

		prop_position_refresh(
			actor_index,
			current_orphan_index,
			&position,
			refresh_position,
			TRUE);
		prop_status_refresh(
			actor_index,
			current_orphan_index,
			&position);
		current_prop_index = current_orphan_index;
		current_prop =
			(struct actor_orphan_prop_view *)
				prop_get(current_orphan_index);
	}
	else
	{
#line 3802 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		match_vassert(
			__FILE__,
			__LINE__,
			current_prop->state >= _prop_state_unacknowledged &&
				current_prop->state <= _prop_state_becoming_acknowledged,
			"prop_unacknowledged(current_prop)");
#line 586 "source\\ai\\actor_perception.c"

		if (friend_prop_index != NONE)
		{
			current_prop_index =
				prop_orphan_from_friend(
					actor_index,
					current_prop_index,
					friend_prop_index);
			if (current_prop_index != NONE)
			{
				struct actor_orphan_prop_view *new_prop =
					(struct actor_orphan_prop_view *)
						prop_get(current_prop_index);
				new_prop->unit_index = current_prop->unit_index;
				new_prop->actor_index = current_prop->actor_index;
				new_prop->swarm = current_prop->swarm;
			}
		}
		else
		{
			prop_position_refresh(
				actor_index,
				current_prop_index,
				&position,
				FALSE,
				FALSE);
			current_prop_index =
				prop_orphan_transition(
					actor_index,
					current_prop_index);
		}

		if (current_prop_index == NONE)
		{
			result = FALSE;
			goto done;
		}

		current_prop =
			(struct actor_orphan_prop_view *)
				prop_get(current_prop_index);
	}

	if (current_prop != NULL)
	{
		if (source_actor_index == NONE ||
			(friend_prop_index != NONE &&
				prop_get(friend_prop_index)->visibility >=
						_actor_perception_full))
		{
			current_prop->definitely_located = TRUE;
			current_prop->ticks_since_definitely_located = 0;
			current_prop->definite_knowledge_source_actor =
				source_actor_index;
		}

		current_prop->unopposable_enemy =
			actor_compute_prop_unopposable(
				actor_index,
				current_prop_index);
		current_prop->target_weight =
			actor_compute_prop_target_weight(
				actor_index,
				current_prop_index);
	}

done:
	return result;
}


static void actor_perception_refresh(
	long actor_index)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	struct actor_datum *actor = actor_get(actor_index);
	unsigned long *pvs = NULL;
	unsigned long swarm_pvs[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_CLUSTERS_PER_STRUCTURE)];
	struct actor_perception_refresh_list enemies;
	struct actor_perception_refresh_list friends;
	struct prop_iterator iterator;
	struct prop_datum *prop;
	struct actor_position_data position;

	enemies.entry_count = 0;
	enemies.accepted_count = 0;
	friends.entry_count = 0;
	friends.accepted_count = 0;

	if (actor->meta.swarm)
	{
		struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);
		boolean swarm_pvs_valid = FALSE;
		short unit_index;

		csmemset(swarm_pvs, 0, sizeof(swarm_pvs));
		for (unit_index = 0; unit_index < swarm->unit_count; unit_index++)
		{
			struct unit_datum *unit = unit_get(swarm->unit_indices[unit_index]);
			short cluster_index = unit->object.location.cluster_index;

			if (cluster_index != NONE)
			{
				unsigned long *cluster_pvs =
					structure_bsp_get_cluster_pvs(structure_bsp, cluster_index);

				bit_vector_or(
					(short)structure_bsp->clusters.count,
					cluster_pvs,
					swarm_pvs,
					swarm_pvs);
				swarm_pvs_valid = TRUE;
			}
		}

		if (swarm_pvs_valid)
		{
			pvs = swarm_pvs;
		}
	}
	else
	{
		short cluster_index = actor->input.position.body_location.cluster_index;

		if (cluster_index != NONE)
		{
			pvs = structure_bsp_get_cluster_pvs(structure_bsp, cluster_index);
		}
	}

	object_marker_begin();

	prop_iterator_new(&iterator, actor_index);
	while ((prop = prop_iterator_next(&iterator)) != NULL)
	{
		if (prop->state < _prop_state_uninspected_orphan ||
			prop->state > _prop_state_inspected_orphan)
		{
			real distance_squared = actor_perception_distance_squared(prop->distance);
			boolean optional;
			boolean desired =
				actor_perception_desire_prop(
					actor_index,
					NONE,
					prop->unit_index,
					prop->actor_index,
					prop->in_use,
					prop->player,
					prop->enemy,
					prop->dead,
					prop->dead_ticks,
					prop->suicide_radius,
					distance_squared,
					prop->required_ticks,
					&optional);

			if (desired && pvs != NULL)
			{
				struct object_cluster_iterator cluster_iterator;
				short cluster_index;

				desired = FALSE;
				for (cluster_index = object_get_first_cluster(&cluster_iterator, prop->unit_index);
					cluster_index != NONE;
					cluster_index = object_get_next_cluster(&cluster_iterator, prop->unit_index))
				{
					if (BIT_VECTOR_TEST_FLAG(pvs, cluster_index))
					{
						desired = TRUE;
						break;
					}
				}
			}

			if (prop->swarm && prop->actor_index != NONE)
			{
				struct actor_datum *swarm_actor = actor_get(prop->actor_index);

				if (swarm_actor->meta.swarm_cache_index != NONE)
				{
					struct swarm_datum *swarm = swarm_get(swarm_actor->meta.swarm_cache_index);
					short unit_index;

					for (unit_index = 0; unit_index < swarm->unit_count; unit_index++)
					{
						object_mark_function(swarm->unit_indices[unit_index]);
					}
				}
				else
				{
					/* BUG (preserved for exact matching): January walks the perceiving actor's
					 * swarm unit list (actor+0x24 through the actor_get(actor_index) pointer),
					 * not the uncached swarm actor's list. A corrected build should start from
					 * swarm_actor->meta.swarm_unit_index. */
					long unit_index = actor->meta.swarm_unit_index;

					while (unit_index != NONE)
					{
						struct unit_datum *unit = unit_get(unit_index);

						object_mark_function(unit_index);
						unit_index = unit->unit.swarm_next_unit_index;
					}
				}
			}

			object_mark_function(prop->unit_index);

			if (desired)
			{
				struct actor_perception_refresh_list *list =
					prop->enemy ? &enemies : &friends;

				if (optional)
				{
#line 2669 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
					assert(!prop->dead);
#line 5988 "source\\ai\\actor_perception.c"

					if (list->entry_count < 128)
					{
						list->entries[list->entry_count].unit_index = prop->unit_index;
						list->entries[list->entry_count].prop_index = iterator.index;
						list->entries[list->entry_count].priority = distance_squared * 0.69444442f;
						list->entry_count++;
					}
					else if (last_refresh_overflow_warning_time == NONE ||
						game_time_get() > last_refresh_overflow_warning_time + 150)
					{
						error(
							_error_silent,
							"actor_perception_refresh overflowed max %s (%d), discarding",
							prop->enemy ? "enemies" : "friends",
							128);
						last_refresh_overflow_warning_time = game_time_get();
					}
				}
				else if (!prop->dead)
				{
					list->accepted_count++;
				}
			}
			else
			{
				if ((prop->state < _prop_state_uninspected_orphan ||
						prop->state > _prop_state_inspected_orphan) &&
					prop->orphan_prop_index != NONE)
				{
					actor_switch_props(actor_index, prop->orphan_prop_index, NONE);
					prop_delete(actor_index, prop->orphan_prop_index);
				}

				actor_switch_props(actor_index, iterator.index, NONE);
				prop_delete(actor_index, iterator.index);
			}
		}
	}

	if (pvs != NULL)
	{
		short cluster_index;

		for (cluster_index = 0; cluster_index < structure_bsp->clusters.count; cluster_index++)
		{
			if (BIT_VECTOR_TEST_FLAG(pvs, cluster_index))
			{
				long reference_index;
				long object_index;

				for (object_index = cluster_get_first_collideable_object(&reference_index, cluster_index);
					object_index != NONE;
					object_index = cluster_get_next_collideable_object(&reference_index))
				{
					actor_perception_refresh_test_object(actor_index, object_index, &enemies, &friends);
				}

				for (object_index = cluster_get_first_noncollideable_object(&reference_index, cluster_index);
					object_index != NONE;
					object_index = cluster_get_next_noncollideable_object(&reference_index))
				{
					actor_perception_refresh_test_object(actor_index, object_index, &enemies, &friends);
				}
			}
		}
	}

	if (enemies.entry_count > 0)
	{
		short entry_index = 0;

		if (enemies.accepted_count < 4)
		{
			qsort(
				enemies.entries,
				enemies.entry_count,
				sizeof(struct actor_perception_refresh_entry),
				actor_perception_qsort_compare_optional_props);
			for (; entry_index < enemies.entry_count; entry_index++)
			{
				if (enemies.entries[entry_index].prop_index == NONE)
				{
					long prop_index =
						prop_new_unacknowledged(
							actor_index,
							enemies.entries[entry_index].unit_index,
							TRUE);

					if (prop_index == NONE)
					{
						continue;
					}

					prop_position_refresh(actor_index, prop_index, &position, FALSE, FALSE);
				}

				/* BUG (preserved for exact matching): January leaves the loop before
				 * advancing past the entry that reached the limit, so the discard loop
				 * below also deletes that entry when it was an existing prop. */
				if (++enemies.accepted_count >= 4)
				{
					break;
				}
			}
		}

		for (; entry_index < enemies.entry_count; entry_index++)
		{
			if (enemies.entries[entry_index].prop_index != NONE)
			{
				struct prop_datum *discarded_prop = prop_get(enemies.entries[entry_index].prop_index);

				if ((discarded_prop->state < _prop_state_uninspected_orphan ||
						discarded_prop->state > _prop_state_inspected_orphan) &&
					discarded_prop->orphan_prop_index != NONE)
				{
					actor_switch_props(actor_index, discarded_prop->orphan_prop_index, NONE);
					prop_delete(actor_index, discarded_prop->orphan_prop_index);
				}

				actor_switch_props(actor_index, enemies.entries[entry_index].prop_index, NONE);
				prop_delete(actor_index, enemies.entries[entry_index].prop_index);
			}
		}
	}

	if (friends.entry_count > 0)
	{
		short entry_index = 0;
		short prop_count = enemies.accepted_count + friends.accepted_count;
		short maximum_prop_count = MAX(enemies.accepted_count + 2, 4);

		if (prop_count < maximum_prop_count)
		{
			qsort(
				friends.entries,
				friends.entry_count,
				sizeof(struct actor_perception_refresh_entry),
				actor_perception_qsort_compare_optional_props);
			for (; entry_index < friends.entry_count; entry_index++)
			{
				if (friends.entries[entry_index].prop_index == NONE)
				{
					long prop_index =
						prop_new_unacknowledged(
							actor_index,
							friends.entries[entry_index].unit_index,
							FALSE);

					if (prop_index == NONE)
					{
						continue;
					}

					prop_position_refresh(actor_index, prop_index, &position, FALSE, FALSE);
				}

				friends.accepted_count++;
				if (++prop_count >= maximum_prop_count)
				{
					break;
				}
			}
		}

		for (; entry_index < friends.entry_count; entry_index++)
		{
			if (friends.entries[entry_index].prop_index != NONE)
			{
				struct prop_datum *discarded_prop = prop_get(friends.entries[entry_index].prop_index);

				if ((discarded_prop->state < _prop_state_uninspected_orphan ||
						discarded_prop->state > _prop_state_inspected_orphan) &&
					discarded_prop->orphan_prop_index != NONE)
				{
					actor_switch_props(actor_index, discarded_prop->orphan_prop_index, NONE);
					prop_delete(actor_index, discarded_prop->orphan_prop_index);
				}

				actor_switch_props(actor_index, friends.entries[entry_index].prop_index, NONE);
				prop_delete(actor_index, friends.entries[entry_index].prop_index);
			}
		}
	}

	object_marker_end();

	return;
}
boolean actor_emotion_flee_with_friends(
	long actor_index,
	real *desire_to_flee)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	struct prop_iterator iterator;
	struct actor_perception_prop_view *prop;
	short fighting_friend_count = 0;
	short fleeing_friend_count = 0;
	real modifier;

	prop_iterator_new(&iterator, actor_index);
	prop =
		(struct actor_perception_prop_view *)prop_iterator_next(&iterator);
	while (prop != NULL)
	{
		if (prop->state >= _prop_state_becoming_unacknowledged &&
			prop->state <= _prop_state_acknowledged &&
			!prop->enemy &&
			prop->type == actor->type &&
			prop->actor_index != NONE)
		{
			struct actor_perception_actor_view *friend_actor =
				(struct actor_perception_actor_view *)actor_get(
					prop->actor_index);

			if (friend_actor->active_threat_count > 0 ||
				(friend_actor->friend_state == 4 &&
					friend_actor->friend_fighting_count > 0))
			{
				fighting_friend_count++;
			}
			else if (prop->in_combat)
			{
				fleeing_friend_count++;
			}
		}

		prop =
			(struct actor_perception_prop_view *)prop_iterator_next(
				&iterator);
	}

	if (fighting_friend_count > 1)
		return TRUE;

	if (fleeing_friend_count > 1)
		modifier = 1.0f - (fleeing_friend_count - 1) * 0.25f;
	else
		modifier = 1.0f + (1 - fleeing_friend_count) * 0.5f;

	*desire_to_flee *= PIN(modifier, 0.0f, 2.0f);

	return FALSE;
}

boolean actor_situation_try_new_target(
	long actor_index,
	long new_prop_index)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	struct actor_perception_prop_view *new_prop =
		(struct actor_perception_prop_view *)prop_get(new_prop_index);
	struct actor_perception_prop_view *target_prop;
	boolean result = FALSE;

	if (actor->target_prop_index == NONE)
		target_prop = NULL;
	else
		target_prop =
			(struct actor_perception_prop_view *)prop_get(
				actor->target_prop_index);

	new_prop->target_weight =
		actor_compute_prop_target_weight(actor_index, new_prop_index);

	if (new_prop->target_weight > 0.0f)
	{
#line 4685 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
		assert(new_prop->enemy);
#line 700 "source\\ai\\actor_perception.c"

		if (!target_prop ||
			new_prop->target_weight >= target_prop->target_weight)
		{
			actor->target_type = 0;
			actor->target_prop_index = new_prop_index;
			actor->target_last_visible_time = NONE;
			actor_situation_update_target_status(actor_index);
			actor_situation_combat_status_update(actor_index);
			result = TRUE;
		}
	}

	return result;
}


/* ---------- private code */

static long actor_perception_qsort_compare_optional_props(
	void const *a,
	void const *b)
{
	real a_value = ((real const *)a)[2];
	real b_value = ((real const *)b)[2];

	if (a_value < b_value)
		return -1;

	if (a_value > b_value)
		return 1;

	return 0;
}

static boolean actor_perception_assess_suicide_danger(
	long actor_index,
	long object_index,
	real suicide_radius,
	real distance,
	boolean enemy,
	boolean visible)
{
	struct actor_perception_actor_view *actor =
		(struct actor_perception_actor_view *)actor_get(actor_index);
	struct actor_danger_zone_view *danger_zone;
	boolean result = FALSE;

	if (suicide_radius + 10.0f > distance)
	{
		danger_zone = &actor->danger_zone;

		if (actor->danger_zone.danger_type < 1 ||
			(actor->danger_zone.danger_type == 1 &&
				actor->danger_zone.object_index != object_index &&
				distance < actor->danger_zone.current_distance_from_actor))
		{
			struct unit_datum *unit = unit_get(object_index);

			memset(
				danger_zone,
				0,
				sizeof(*danger_zone));
			actor->danger_zone.danger_type = 1;
			actor->danger_zone.object_index = object_index;
			actor->danger_zone.danger_radius = suicide_radius;
			object_get_origin(
				object_index,
				&actor->danger_zone.initial_position);
			actor->danger_zone.initial_velocity =
				unit->object.translational_velocity;
			actor->danger_zone.acknowledgement_timer = 6;
			actor->danger_zone.currently_perceived = visible;
			actor->danger_zone.hostility = !enemy;
			result = TRUE;
		}
	}

	return result;
}

void actor_perception_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	short highest_prop_timer = 1;
	boolean prop_serviced = FALSE;
	long interesting_orphan_index = NONE;
	real nearest_orphan_distance = REAL_MAX;
	struct prop_iterator iterator;
	struct prop_datum *prop;
	struct actor_position_data position;

	if (!actor->meta.dormant)
	{
		if (actor->meta.timeslice)
		{
			actor_perception_refresh(actor_index);
		}

		actor_perception_refresh_danger_zone(actor_index);

		if (actor->danger_zone.danger_type > _actor_danger_zone_none)
		{
			boolean acknowledge_danger = FALSE;

			if (actor->danger_zone.attached_to_us ||
				actor->danger_zone.hostility != 0)
			{
				actor->danger_zone.noticed_danger = TRUE;
				acknowledge_danger = actor->danger_zone.acknowledgement_timer > 0;
				actor->danger_zone.acknowledgement_timer = 0;
			}
			else if (actor->danger_zone.acknowledgement_timer > 0 &&
				actor->danger_zone.currently_perceived)
			{
				if (actor->danger_zone.hostility == 0 &&
					!actor->danger_zone.attached_to_us &&
					(actor->state.uncertain_combat_timer == NONE ||
						actor->state.uncertain_combat_timer >= 60))
				{
					acknowledge_danger = --actor->danger_zone.acknowledgement_timer == 0;
				}
				else
				{
					actor->danger_zone.acknowledgement_timer = 0;
					acknowledge_danger = TRUE;
				}
			}

			if (acknowledge_danger)
			{
				real notice_chance;

				switch (actor->danger_zone.danger_type)
				{
				case _actor_danger_zone_vehicle:
					notice_chance = definition->perception.notice_vehicle_chance;
					goto test_notice_chance;

				case _actor_danger_zone_projectile:
					notice_chance = definition->perception.notice_projectile_chance;
				test_notice_chance:
					if (notice_chance > 0.0f &&
						real_seed_random(get_global_random_seed_address()) < notice_chance)
					{
						actor->danger_zone.noticed_danger = TRUE;
					}
					break;

				case _actor_danger_zone_suicide:
					actor->danger_zone.noticed_danger = TRUE;
					break;
				}

				if (actor->danger_zone.noticed_danger)
				{
					if (actor->danger_zone.attached_to_us)
					{
						actor->danger_zone.allow_dive_evasion = FALSE;
					}
					else if (actor->danger_zone.hostility != 0 ||
						actor->danger_zone.danger_type == _actor_danger_zone_vehicle ||
						actor->danger_zone.danger_type == _actor_danger_zone_suicide)
					{
						actor->danger_zone.allow_dive_evasion = TRUE;
					}
					else
					{
						actor->danger_zone.allow_dive_evasion =
							real_seed_random(get_global_random_seed_address()) <
								definition->moving.grenade_dive_chance;
					}

					actor_stimulus_noticed_danger_zone(
						actor_index,
						actor->danger_zone.danger_type,
						actor->danger_zone.hostility,
						actor->danger_zone.object_index,
						&actor->danger_zone.position);
				}
			}

			if (actor->danger_zone.acknowledgement_timer == 0)
			{
				if (actor->control.secondary_look_type == _secondary_look_dangerous_object)
				{
					actor->control.secondary_look_priority =
						MIN(actor->control.secondary_look_priority, _secondary_look_priority_turn_and_aim);
				}

				if (actor->danger_zone.attached_to_us)
				{
					actor->danger_zone.noticed_danger = TRUE;
					actor->danger_zone.allow_dive_evasion = FALSE;
				}
			}
		}
	}

	prop_iterator_new(&iterator, actor_index);
	while ((prop = prop_iterator_next(&iterator)) != NULL)
	{
		short new_state = NONE;
		boolean orphan_expired = FALSE;
		boolean refresh_position = FALSE;
		boolean refresh_status = FALSE;
		boolean became_acknowledged = FALSE;
		boolean expected_acknowledgement = FALSE;

		if (prop->unit_effect_decay_ticks > 0 &&
			--prop->unit_effect_decay_ticks == 0)
		{
			prop->unit_effect = NONE;
		}

		if (prop->ticks_since_damage != NONE &&
			++prop->ticks_since_damage >= 45)
		{
			prop->currently_damaging_me = FALSE;
		}

		if (prop->ticks_since_definitely_located != NONE &&
			++prop->ticks_since_definitely_located >= 60)
		{
			prop->definitely_located = FALSE;
			prop->definite_knowledge_source_actor = NONE;
		}

		if (prop->dead)
		{
			prop->dead_ticks++;
		}
		else
		{
			prop->dead_ticks = 0;
		}

		if (prop->ticks_until_orphan > 0)
		{
			prop->ticks_until_orphan--;
		}

		if (prop->required_ticks > 0 &&
			!prop->delay_requirement_decision)
		{
			prop->required_ticks--;
		}

		if (prop->unreachable_ticks > 0 &&
			prop->unreachable_ticks < SHORT_MAX)
		{
			prop->unreachable_ticks++;
		}

		if (prop->unopposable_casualty_decay_timer > 0 &&
			--prop->unopposable_casualty_decay_timer == 0)
		{
#line 316 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
			assert(prop->unopposable_casualties_inflicted > 0);
#line 6522 "source\\ai\\actor_perception.c"

			if (--prop->unopposable_casualties_inflicted > 0)
			{
				prop->unopposable_casualty_decay_timer = 750;
			}
		}

		if (prop->visibility >= _actor_perception_full)
		{
			if (prop->visible_ticks < SHORT_MAX)
			{
				prop->visible_ticks++;
			}
		}
		else
		{
			prop->visible_ticks = 0;
		}

		if (actor->meta.dormant)
		{
			prop->in_use = FALSE;
			prop->timer = 0;
		}
		else
		{
			short prop_timer = ++prop->timer;

			if (!prop->enemy)
			{
				prop_timer >>= 3;
			}

			if (prop->quantized_distance >= 3)
			{
				prop_timer >>= 1;
			}

			if (!prop_serviced &&
				prop_timer >= actor->meta.highest_prop_timer)
			{
				refresh_status = TRUE;
				refresh_position = TRUE;
				prop_timer = 0;
				prop->timer = 0;
				prop_serviced = TRUE;
			}

			if (prop_timer > highest_prop_timer)
			{
				highest_prop_timer = prop_timer;
			}

			if (prop->state < _prop_state_unacknowledged ||
				prop->state > _prop_state_becoming_acknowledged ||
				prop->orphan_prop_index == NONE)
			{
				if (actor->meta.swarm)
				{
					prop->in_use = FALSE;
				}
				else
				{
					prop->in_use =
						actor->target.target_prop_index == iterator.index ||
						actor->meta.interesting_orphan_index == iterator.index ||
						actor->emotions.unopposable_retreat_prop_index == iterator.index ||
						actor->external_orders.pursuit_group_prop_index == iterator.index ||
						(actor->control.secondary_look_type != _secondary_look_none &&
							actor->control.secondary_look_direction.type == _direction_specification_prop &&
							actor->control.secondary_look_direction.prop_index == iterator.index) ||
						(actor->control.idle_major_active &&
							actor->control.idle_major_direction.type == _direction_specification_prop &&
							actor->control.idle_major_direction.prop_index == iterator.index) ||
						(actor->control.idle_minor_active &&
							actor->control.idle_minor_direction.type == _direction_specification_prop &&
							actor->control.idle_minor_direction.prop_index == iterator.index);

					if (prop->state >= _prop_state_uninspected_orphan &&
						prop->state <= _prop_state_inspected_orphan)
					{
						struct prop_datum *parent_prop = prop_get(prop->orphan_prop_index);

#line 402 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
						assert(parent_prop->orphan_prop_index == iterator.index);
#line 6608 "source\\ai\\actor_perception.c"

						parent_prop->in_use = prop->in_use;
					}
				}
			}

			if (prop->in_use &&
				(prop->state < _prop_state_unacknowledged ||
					prop->state > _prop_state_becoming_acknowledged))
			{
				refresh_position = TRUE;
			}

#line 416 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
			assert(!refresh_status || refresh_position);
#line 6624 "source\\ai\\actor_perception.c"

			if (refresh_position)
			{
				prop_position_refresh(
					actor_index,
					iterator.index,
					&position,
					FALSE,
					refresh_status);
			}

			if (refresh_status)
			{
				prop_status_refresh(actor_index, iterator.index, &position);
			}
		}

		switch (prop->state)
		{
		case _prop_state_unacknowledged:
			if (prop->perception <= _actor_perception_none)
			{
				break;
			}

			new_state = _prop_state_becoming_acknowledged;
			prop->awareness = 0.0f;
			if (prop->player &&
				ai_debug.print_acknowledgement)
			{
				char buffer[256];

				ai_debug_describe_actor(actor_index, actor->meta.unit_index, NONE, buffer, sizeof(buffer));
				error(_error_silent, "%s: start to become aware", buffer);
			}

		case _prop_state_becoming_acknowledged:
			{
				struct actor_perception_debug_info_view *debug = actor_perception_debug_get(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index));

				if (prop->perception == _actor_perception_none)
				{
					prop->awareness = 0.0f;
					new_state = _prop_state_unacknowledged;
					if (prop->player)
					{
						debug->perception_awareness_speed = NONE;
						if (ai_debug.print_acknowledgement)
						{
							char buffer[256];

							ai_debug_describe_actor(actor_index, actor->meta.unit_index, NONE, buffer, sizeof(buffer));
							error(_error_silent, "%s: stop becoming aware", buffer);
						}
					}
				}
				else
				{
					short knowledge_type = actor_get_perception_knowledge(actor_index, iterator.index);
					short awareness_speed;
					real awareness_delta;

#line 489 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
					assert((knowledge_type >= 0) && (knowledge_type < NUMBER_OF_ACTOR_KNOWLEDGE_TYPES));
					assert((prop->perception >= 0) && (prop->perception < NUMBER_OF_ACTOR_PERCEPTION_TYPES));
#line 6690 "source\\ai\\actor_perception.c"

					awareness_speed = global_acknowledgement_speeds[knowledge_type][prop->perception];
					switch (awareness_speed)
					{
					case _awareness_speed_never:
						awareness_delta = 0.0f;
						break;

					case _awareness_speed_noncombat:
						awareness_delta = definition->perception.runtime_awareness_delta_non_combat;
						break;

					case _awareness_speed_guard:
						awareness_delta = definition->perception.runtime_awareness_delta_guard;
						break;

					case _awareness_speed_combat:
						awareness_delta = definition->perception.runtime_awareness_delta_combat;
						break;

					case _awareness_speed_instant:
						awareness_delta = 1.0f;
						break;

					/* awareness_delta is left unassigned only by this default arm. Not reached unassigned: the
					 * arm's assertion failure calls system_exit, which does not return in January
					 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
					 * Source-policy approval pending (2026-09-27 audit). */
					default:
#line 516 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
						assert(!"unreachable");
#line 6718 "source\\ai\\actor_perception.c"
						break;
					}

					if (prop->player &&
						debug->perception_awareness_speed != awareness_speed)
					{
						debug->perception_awareness_speed = awareness_speed;
						if (ai_debug.print_acknowledgement)
						{
							char const *awareness_speed_names[] =
							{
								"never",
								"noncombat",
								"guard",
								"combat",
								"instant"
							};
							char const *knowledge_names[] =
							{
								"noncombat",
								"guard",
								"searching",
								"definite"
							};
							char const *perception_names[] =
							{
								"none",
								"partial",
								"full",
								"unmistakable"
							};
							char buffer[256];

							ai_debug_describe_actor(actor_index, actor->meta.unit_index, NONE, buffer, sizeof(buffer));
							error(
								_error_silent,
								"%s: knowledge %s percep %s -> awareness %s",
								buffer,
								knowledge_names[knowledge_type],
								perception_names[prop->perception],
								awareness_speed_names[awareness_speed]);

							if (awareness_delta > 0.0f &&
								awareness_delta < 1.0f)
							{
								error(
									_error_silent,
									"  awareness delta: %.2f (current awareness %.2f -> time %.2fsec)",
									awareness_delta,
									prop->awareness,
									(1.0f - prop->awareness) / (awareness_delta * TICKS_PER_SECOND));
							}
						}
					}

					prop->awareness += awareness_delta;
					if (prop->awareness >= 1.0f)
					{
						new_state = _prop_state_acknowledged;
						if (prop->player)
						{
							debug->perception_awareness_speed = NONE;
							if (ai_debug.print_acknowledgement)
							{
								char buffer[256];

								ai_debug_describe_actor(actor_index, actor->meta.unit_index, NONE, buffer, sizeof(buffer));
								error(_error_silent, "%s: become aware!", buffer);
							}
						}
					}
				}
			}
			break;

		case _prop_state_becoming_unacknowledged:
			if (prop->perception > _actor_perception_none)
			{
				new_state = _prop_state_acknowledged;
			}
			else
			{
				if (prop->ticks_until_orphan == 0 ||
					distance_squared2d(
						(real_point2d const *)&prop->last_perceived_body_position,
						(real_point2d const *)&prop->body_position) > 1.0f)
				{
					long orphan_prop_index = NONE;

					if (actor_perception_desire_prop(
							actor_index,
							_prop_state_uninspected_orphan,
							prop->unit_index,
							prop->actor_index,
							prop->in_use,
							prop->player,
							prop->enemy,
							prop->dead,
							prop->dead_ticks,
							prop->suicide_radius,
							actor_perception_distance_squared(prop->distance),
							prop->required_ticks,
							NULL))
					{
						struct actor_position_data orphan_position;

						prop_position_refresh(
							actor_index,
							iterator.index,
							&orphan_position,
							FALSE,
							FALSE);
						actor_perception_find_prop_pathfinding_location(actor_index, iterator.index);
						orphan_prop_index = prop_orphan_transition(actor_index, iterator.index);
					}

					actor_switch_props(actor_index, iterator.index, orphan_prop_index);
					new_state = _prop_state_unacknowledged;
				}
			}
			break;

		case _prop_state_acknowledged:
			if (prop->perception == _actor_perception_none)
			{
				if (actor_perception_desire_prop(
						actor_index,
						_prop_state_uninspected_orphan,
						prop->unit_index,
						prop->actor_index,
						prop->in_use,
						prop->player,
						prop->enemy,
						prop->dead,
						prop->dead_ticks,
						prop->suicide_radius,
						actor_perception_distance_squared(prop->distance),
						prop->required_ticks,
						NULL))
				{
					new_state = _prop_state_becoming_unacknowledged;
				}
				else
				{
					actor_switch_props(actor_index, iterator.index, NONE);
					new_state = _prop_state_unacknowledged;
				}
			}
			break;

		case _prop_state_uninspected_orphan:
		case _prop_state_inspected_orphan:
			{
				short lifespan_decay;

				if (prop->state == _prop_state_uninspected_orphan)
				{
					short inspection_ticks =
						actor->input.vehicle_gunner_bombardment ? 300 : 45;

					if (prop->visibility >= _actor_perception_full ||
						(actor->control.current_fire_target_type == _actor_fire_target_prop &&
							actor->control.current_fire_target_prop_index == iterator.index &&
							game_time_get() % 3 == 0))
					{
						if (++prop->orphan_inspection_ticks >= inspection_ticks)
						{
							new_state = _prop_state_inspected_orphan;
						}
					}
				}

				if (iterator.index == actor->emotions.unopposable_retreat_prop_index ||
					(actor->state.action == _actor_action_flee &&
						actor->state.action_data.flee.flee_prop_index == iterator.index))
				{
					lifespan_decay = 0;
				}
				else if (iterator.index == actor->target.target_prop_index)
				{
					lifespan_decay = prop->abandoned_search != FALSE;
				}
				else if (iterator.index == actor->meta.interesting_orphan_index)
				{
					lifespan_decay = actor->state.combat_status >= _actor_combat_status_certain ? 6 : 1;
				}
				else
				{
					lifespan_decay = 10;
				}

				prop->orphan_lifespan_ticks -= lifespan_decay;
				if (prop->orphan_lifespan_ticks < 0)
				{
					orphan_expired = TRUE;
				}
			}
			break;
		}

		if (new_state != NONE)
		{
#line 694 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
			assert(new_state!=prop->state);
#line 6928 "source\\ai\\actor_perception.c"

			switch (new_state)
			{
			case _prop_state_becoming_unacknowledged:
				prop->ticks_until_orphan = prop->visibility >= _actor_perception_full ? 60 : 10;
				break;

			case _prop_state_acknowledged:
				became_acknowledged =
					actor_perception_become_acknowledged(
						actor_index,
						iterator.index,
						&expected_acknowledgement);
				iterator.next_index = prop->next_prop_index;
				break;

			case _prop_state_uninspected_orphan:
				match_vassert("c:\\halo\\SOURCE\\ai\\actor_perception.c", 721, FALSE, NULL);

			case _prop_state_unacknowledged:
			case _prop_state_inspected_orphan:
				prop->definitely_located = FALSE;
				prop->definite_knowledge_source_actor = NONE;
				break;

			case _prop_state_becoming_acknowledged:
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\ai\\actor_perception.c", 730, FALSE, NULL);
				break;
			}

			prop->state = new_state;
			prop->unopposable_enemy = actor_compute_prop_unopposable(actor_index, iterator.index);
			prop->target_weight = actor_compute_prop_target_weight(actor_index, iterator.index);
		}

		if (orphan_expired)
		{
			struct prop_datum *parent_prop;

			match_vassert(
				"c:\\halo\\SOURCE\\ai\\actor_perception.c",
				746,
				prop->parent_prop_index != NONE,
				"prop->parent_prop_index != NONE");

			parent_prop = prop_get(prop->parent_prop_index);

#line 751 "c:\\halo\\SOURCE\\ai\\actor_perception.c"
			assert(parent_prop->orphan_prop_index == iterator.index);
#line 6981 "source\\ai\\actor_perception.c"

			parent_prop->orphan_prop_index = NONE;
			actor_switch_props(actor_index, iterator.index, NONE);
			prop_delete(actor_index, iterator.index);
		}
		else if (prop->refresh_stimuli &&
			prop->state >= _prop_state_becoming_unacknowledged &&
			prop->state <= _prop_state_acknowledged)
		{
			if (prop->just_killed)
			{
				actor_stimulus_prop_just_killed(actor_index, iterator.index);
				prop->just_killed = FALSE;
			}

			if (prop->just_became_visible ||
				(became_acknowledged && prop->visibility > _actor_perception_none))
			{
				boolean initial_sighting = became_acknowledged && !expected_acknowledgement;

				actor_stimulus_prop_sighted(actor_index, iterator.index, initial_sighting);
				prop->just_became_visible = FALSE;
			}

			if (!actor->emotions.sighted_friendly_player &&
				!prop->enemy &&
				prop->player &&
				prop->visibility >= _actor_perception_full &&
				prop->quantized_facing <= 2 &&
				prop->distance < 7.0f)
			{
				actor->emotions.sighted_friendly_player = TRUE;
				ai_communication_event(
					_ai_communication_sighted_friend_player,
					actor->meta.unit_index,
					prop->unit_index,
					_comm_hostility_friend,
					NONE,
					NONE,
					NULL);
				actor_stimulus_prop_sighted(actor_index, iterator.index, FALSE);
			}

			if (actor->meta.unit_index != NONE &&
				!prop->dead &&
				prop->ally &&
				prop->ally_status_changed)
			{
				boolean enemy = game_team_is_enemy(actor->meta.team_index, prop->team_index);
				boolean close =
					prop->distance <
						(enemy ? 15.0f : prop->quantized_facing <= 2 ? 10.0f : 3.0f);

				if ((enemy && prop->currently_damaging_me) || close)
				{
					struct ai_information_data information;

					information.allegiance.team1_index = actor->meta.team_index;
					information.allegiance.team2_index = prop->team_index;
					information.allegiance.broken = enemy;
					ai_communication_event(
						_ai_communication_allegiance_changed,
						actor->meta.unit_index,
						prop->unit_index,
						enemy ? _comm_hostility_traitor : _comm_hostility_friend,
						NONE,
						_ai_information_allegiance,
						&information);
				}
			}

			if (actor->state.mode < _actor_mode_combat)
			{
				if (prop->dead && !prop->enemy)
				{
					actor_stimulus_enter_combat_found_body(actor_index, iterator.index);
				}
				else if (prop->enemy)
				{
					actor_stimulus_enter_combat_perceived_enemy(actor_index, iterator.index);
				}
			}

			if (!prop->enemy &&
				!prop->dead &&
				!prop->player)
			{
				struct encounter_datum *encounter;

				if ((actor->target.target_prop_index != NONE &&
						(actor->target.since_any_target_visible_timer == NONE ||
							actor->target.since_any_target_visible_timer >= 180)) ||
					(actor->meta.encounter_index != NONE &&
						((encounter = encounter_get(actor->meta.encounter_index))->enemy_visible_timer == NONE ||
							(encounter->enemy_visible_timer >= 180 && encounter->enemy_alive))))
				{
					if (actor->meta.unit_index != NONE)
					{
						if (actor->state.mode < _actor_mode_combat)
						{
							if (prop->in_combat)
							{
								ai_communication_event(
									_ai_communication_alert_noncombat,
									prop->unit_index,
									actor->meta.unit_index,
									_comm_hostility_friend,
									NONE,
									_ai_information_combat_stimulus,
									NULL);
							}
						}
						else if (actor_in_combat(actor_index) &&
							!actor_is_fighting(actor_index) &&
							prop->noncombat &&
							prop->visibility >= _actor_perception_full)
						{
							ai_communication_event(
								_ai_communication_alert_noncombat,
								actor->meta.unit_index,
								prop->unit_index,
								_comm_hostility_friend,
								NONE,
								_ai_information_combat_stimulus,
								NULL);
						}
					}
				}
			}
		}
		else if (prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan &&
			prop->distance < nearest_orphan_distance)
		{
			interesting_orphan_index = iterator.index;
			nearest_orphan_distance = prop->distance;
		}

		if (prop->dead)
		{
			if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged)
			{
				ai_profile.meters[_ai_meter_dead_props_acknowledged].accumulator++;
			}
			else if (prop->state >= _prop_state_uninspected_orphan && prop->state <= _prop_state_inspected_orphan)
			{
				ai_profile.meters[_ai_meter_dead_props_orphaned].accumulator++;
			}
			else if (prop->state >= _prop_state_unacknowledged && prop->state <= _prop_state_becoming_acknowledged)
			{
				ai_profile.meters[_ai_meter_dead_props_unacknowledged].accumulator++;
			}
		}
		else if (prop->enemy)
		{
			if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged)
			{
				ai_profile.meters[_ai_meter_enemy_props_acknowledged].accumulator++;
			}
			else if (prop->state >= _prop_state_uninspected_orphan && prop->state <= _prop_state_inspected_orphan)
			{
				ai_profile.meters[_ai_meter_enemy_props_orphaned].accumulator++;
			}
			else if (prop->state >= _prop_state_unacknowledged && prop->state <= _prop_state_becoming_acknowledged)
			{
				ai_profile.meters[_ai_meter_enemy_props_unacknowledged].accumulator++;
			}
		}
		else
		{
			if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged)
			{
				ai_profile.meters[_ai_meter_friendly_props_acknowledged].accumulator++;
			}
			else if (prop->state >= _prop_state_uninspected_orphan && prop->state <= _prop_state_inspected_orphan)
			{
				ai_profile.meters[_ai_meter_friendly_props_orphaned].accumulator++;
			}
			else if (prop->state >= _prop_state_unacknowledged && prop->state <= _prop_state_becoming_acknowledged)
			{
				ai_profile.meters[_ai_meter_friendly_props_unacknowledged].accumulator++;
			}
		}
	}

	if (actor->target.target_prop_index != NONE)
	{
		struct prop_datum *target_prop = prop_get(actor->target.target_prop_index);

		if (target_prop->state >= _prop_state_uninspected_orphan &&
			target_prop->state <= _prop_state_inspected_orphan)
		{
			interesting_orphan_index = NONE;
		}
	}

	if (actor->target.target_type >= _actor_target_definite_orphan)
	{
		actor->target.any_target_ever = TRUE;
	}

	if (actor->target.target_type >= _actor_target_visible_enemy)
	{
		actor->target.since_any_target_visible_timer = 0;
	}
	else if (actor->external_orders.stand_down)
	{
		actor->target.since_any_target_visible_timer = NONE;
	}
	else if (actor->target.since_any_target_visible_timer != NONE)
	{
		actor->target.since_any_target_visible_timer++;
	}

	actor->meta.highest_prop_timer = highest_prop_timer;
	actor->meta.interesting_orphan_index = interesting_orphan_index;

	return;
}
