/*
ENCOUNTERS.C

symbols in this file:
00047520 00f0:
	_encounters_initialize (0000)
00047610 0010:
	_encounters_dispose (0000)
00047620 0020:
	_encounters_dispose_from_old_map (0000)
00047640 04b0:
	_encounter_compute_activation_cluster_bit_vector (0000)
00047AF0 01b0:
	_encounter_detach_actor (0000)
00047CA0 0110:
	_encounter_attach_unit (0000)
00047DB0 00b0:
	_encounterless_attach_actor (0000)
00047E60 0140:
	_encounterless_detach_actor (0000)
00047FA0 0060:
	_encounter_get_by_name (0000)
00048000 0030:
	_encounter_iterator_new (0000)
00048030 0040:
	_encounter_iterator_next (0000)
00048070 0050:
	_encounter_actor_iterator_new (0000)
000480C0 0040:
	_encounter_actor_iterator_next (0000)
00048100 0080:
	_encounter_actor_iterator_prev (0000)
00048180 0040:
	_actor_iterator_new (0000)
000481C0 00a0:
	_actor_iterator_next (0000)
00048260 0050:
	_code_00048260 (0000)
000482B0 00f0:
	_code_000482b0 (0000)
000483A0 00a0:
	_encounter_modify_pursuit_desires (0000)
00048440 0280:
	_encounter_determine_pursuit_availability (0000)
000486C0 00d0:
	_code_000486c0 (0000)
00048790 0290:
	_code_00048790 (0000)
00048A20 0080:
	_code_00048a20 (0000)
00048AA0 0060:
	_encounterless_activate (0000)
00048B00 0050:
	_code_00048b00 (0000)
00048B50 00c0:
	_code_00048b50 (0000)
00048C10 00a0:
	_encounter_link_activation (0000)
00048CB0 00a0:
	_code_00048cb0 (0000)
00048D50 03d0:
	_code_00048d50 (0000)
00049120 01b0:
	_encounter_stand_down (0000)
000492D0 0090:
	_code_000492d0 (0000)
00049360 0070:
	_code_00049360 (0000)
000493D0 0030:
	_encounter_set_blind (0000)
00049400 0030:
	_encounter_set_deaf (0000)
00049430 00b0:
	_encounter_squad_timer_expire (0000)
000494E0 0100:
	_code_000494e0 (0000)
000495E0 0290:
	_code_000495e0 (0000)
00049870 00a0:
	_encounters_initialize_for_new_map (0000)
00049910 00d0:
	_encounters_unit_died (0000)
000499E0 0140:
	_encounter_verify_firing_position_owner_actor_indices (0000)
00049B20 0130:
	_encounter_build_firing_position_owner_actor_indices (0000)
00049C50 0100:
	_encounter_mark_examined_pursuit_position (0000)
00049D50 00b0:
	_encounter_pursuit_position_already_examined (0000)
00049E00 02e0:
	_encounter_get_actor_starting_location (0000)
0004A0E0 0030:
	_encounter_force_activate (0000)
0004A110 0030:
	_encounter_force_deactivate (0000)
0004A140 0110:
	_code_0004a140 (0000)
0004A250 07c0:
	_code_0004a250 (0000)
0004AA10 0170:
	_code_0004aa10 (0000)
0004AB80 0120:
	_encounter_spawn_actor (0000)
0004ACA0 0050:
	_encounter_set_respawn (0000)
0004ACF0 02c0:
	_code_0004acf0 (0000)
0004AFB0 0140:
	_code_0004afb0 (0000)
0004B0F0 0780:
	_code_0004b0f0 (0000)
0004B870 0220:
	_encounter_attach_actor (0000)
0004BA90 0470:
	_encounter_update_status (0000)
0004BF00 0080:
	_encounters_update_dirty_status (0000)
0004BF80 02f0:
	_encounter_create (0000)
0004C270 01c0:
	_code_0004c270 (0000)
0004C430 00c0:
	_encounters_create_for_new_map (0000)
0004C4F0 0100:
	_encounters_update (0000)
0024CDBC 0018:
	_global_empty_possibility (0000)
	_global_post_combat_translation_table (0010)
0024CDD4 000d:
	??_C@_0N@BNPPEOCF@pursuit_data?$AA@ (0000)
0024CDE4 000b:
	??_C@_0L@BAANOOMI@ai?5pursuit?$AA@ (0000)
0024CDF0 000e:
	??_C@_0O@CIMAAJHO@platoon_array?$AA@ (0000)
0024CE00 0008:
	??_C@_07GONBCAAG@platoon?$AA@ (0000)
0024CE08 000c:
	??_C@_0M@EHCGPGM@squad_array?$AA@ (0000)
0024CE14 0006:
	??_C@_05BDFMLHFG@squad?$AA@ (0000)
0024CE1C 000f:
	??_C@_0P@MECKMOOF@encounter_data?$AA@ (0000)
0024CE2C 001f:
	??_C@_0BP@EGKKNGMH@c?3?2halo?2SOURCE?2ai?2encounters?4c?$AA@ (0000)
0024CE50 0058:
	??_C@_0FI@BMEIILPH@?$CImove_position?9?$DOcluster_index?5?$DO?$DN@ (0000)
0024CEA8 005c:
	??_C@_0FM@DFHIGNCO@?$CIfiring_position?9?$DOcluster_index?5@ (0000)
0024CF04 003a:
	??_C@_0DK@EFAAMOOO@?$CIcluster_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIcluster@ (0000)
0024CF40 0031:
	??_C@_0DB@KHGAFALI@structure_bsp?9?$DOclusters?4count?5?$DM?$DN@ (0000)
0024CF74 001c:
	??_C@_0BM@ONCBDDBG@platoon?9?$DOoriginal_count?5?$DO?50?$AA@ (0000)
0024CF90 001a:
	??_C@_0BK@NALIBEDP@squad?9?$DOoriginal_count?5?$DO?50?$AA@ (0000)
0024CFAC 001e:
	??_C@_0BO@FMKIINLL@encounter?9?$DOoriginal_count?5?$DO?50?$AA@ (0000)
0024CFCC 001d:
	??_C@_0BN@DOEKPEEH@?$CKactor_index_reference?$CB?$DNNONE?$AA@ (0000)
0024CFEC 0025:
	??_C@_0CF@FBFKAFPA@actor?9?$DOmeta?4unit_index?5?$DN?$DN?5unit_i@ (0000)
0024D014 002f:
	??_C@_0CP@NCMOENFD@actor?9?$DOmeta?4encounter_index?5?$DN?$DN?5e@ (0000)
0024D044 001b:
	??_C@_0BL@OIOHEOEO@?$CBactor?9?$DOmeta?4encounterless?$AA@ (0000)
0024D060 0022:
	??_C@_0CC@PHCDCJEH@actor?9?$DOmeta?4encounter_index?$DN?$DNNON@ (0000)
0024D084 0022:
	??_C@_0CC@LJBHAGND@actor?9?$DOmeta?4platoon_index?5?$DN?$DN?5NON@ (0000)
0024D0A8 0020:
	??_C@_0CA@HNKMILAG@actor?9?$DOmeta?4squad_index?5?$DN?$DN?5NONE?$AA@ (0000)
0024D0C8 0024:
	??_C@_0CE@BMJAJPHM@actor?9?$DOmeta?4encounter_index?5?$DN?$DN?5N@ (0000)
0024D0F0 005d:
	??_C@_0FN@LOCOBBFO@WARNING?3?5too?5many?5actors?5searchi@ (0000)
0024D150 003d:
	??_C@_0DN@ECMJNADN@?$CFs?5?$CF04X?3?5coord?5?$CFd?5current?5?$CFd?1?$CFd?1@ (0000)
0024D190 0029:
	??_C@_0CJ@GDIDEEDO@overflowed?5MAXIMUM_PLATOONS_PER_@ (0000)
0024D1C0 0047:
	??_C@_0EH@JLNGBIIA@encounter_definition?9?$DOplatoons?4c@ (0000)
0024D208 0027:
	??_C@_0CH@CNGIJGEJ@overflowed?5MAXIMUM_SQUADS_PER_MA@ (0000)
0024D230 0043:
	??_C@_0ED@JFKFDEJF@encounter_definition?9?$DOsquads?4cou@ (0000)
0024D278 0063:
	??_C@_0GD@GOGHLEAJ@?$CIlink_encounter_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CI@ (0000)
0024D2DC 0036:
	??_C@_0DG@EPEOFOPA@parent_prop?9?$DOorphan_prop_index?5?$DN@ (0000)
0024D314 001a:
	??_C@_0BK@GDHJJECC@?$CBencounter?9?$DOenemy_visible?$AA@ (0000)
0024D330 001c:
	??_C@_0BM@IJGMLGCM@?$CFs?1?$CFs?3?5delay?5timer?5finished?$AA@ (0000)
0024D34C 0026:
	??_C@_0CG@HJILMOOD@?$CFs?1?$CFs?3?5delay?5timer?5started?5?$CI?$CF?41f@ (0000)
0024D374 0011:
	??_C@_0BB@HBMEBMNH@survivors?5?$CFd?5?$DN?50?$AA@ (0000)
0024D388 0012:
	??_C@_0BC@FNKOIBFL@survivors?5?$CFd?5?$DM?$DN?51?$AA@ (0000)
0024D39C 0021:
	??_C@_0CB@HJKPDMEB@survivors?5?$CFd?5?$DM?$DN?575?$CF?$CF?5of?5total?5?$CFd@ (0000)
0024D3C0 0021:
	??_C@_0CB@NHMKHBJA@survivors?5?$CFd?5?$DM?$DN?550?$CF?$CF?5of?5total?5?$CFd@ (0000)
0024D3E4 0021:
	??_C@_0CB@INABNAMN@survivors?5?$CFd?5?$DM?$DN?525?$CF?$CF?5of?5total?5?$CFd@ (0000)
0024D408 0018:
	??_C@_0BI@IGDFHOAI@survivors?5?$CFd?5?$DM?5total?5?$CFd?$AA@ (0000)
0024D420 0015:
	??_C@_0BF@FJJAHOMB@strength?5?$CF?42f?5?$DM?525?$CF?$CF?$AA@ (0000)
0024D438 0015:
	??_C@_0BF@NMGOFCOD@strength?5?$CF?42f?5?$DM?550?$CF?$CF?$AA@ (0000)
0024D450 0015:
	??_C@_0BF@JBHAPBLB@strength?5?$CF?42f?5?$DM?575?$CF?$CF?$AA@ (0000)
0024D468 004a:
	??_C@_0EK@PHDJOLGD@owner_actor_indices?$FLactor?9?$DOfirin@ (0000)
0024D4B8 0093:
	??_C@_0JD@GAKCCOHE@actor?9?$DOfiring_positions?4current_@ (0000)
0024D550 005a:
	??_C@_0FK@DNNFPKMF@firing_position_owner_actor_indi@ (0000)
0024D5B0 0084:
	??_C@_0IE@GJHEMMJI@?$CIpursuit?9?$DOnext_actor_index_index@ (0000)
0024D634 0038:
	??_C@_0DI@GONNFCHM@pursuit?9?$DOfiring_position_index?5?$DN@ (0000)
0024D66C 0014:
	??_C@_0BE@MIHACDDA@found_index?5?$CB?$DN?5NONE?$AA@ (0000)
0024D680 0035:
	??_C@_0DF@FCNHHGJA@BIT_VECTOR_TEST_FLAG?$CIsquad?9?$DOunus@ (0000)
0024D6B8 0063:
	??_C@_0GD@GIHDEKAB@?$CIselected_behavior_index?5?$DO?$DN?50?$CJ?5?$CG@ (0000)
0024D720 009d:
	??_C@_0JN@COMBHAJN@?$CIprimary_postcombat_behaviors?$FLpr@ (0000)
0024D7C0 0004:
	__real@3b888889 (0000)
0024D7C8 006c:
	??_C@_0GM@MFIMGOEE@WARNING?3?5cannot?5spawn?5actors?5in?5@ (0000)
0024D834 0022:
	??_C@_0CC@EIGEOIGF@?$CFs?1?$CFs?3?5randomly?5selected?5to?5spaw@ (0000)
0024D858 002b:
	??_C@_0CL@DJPLPKCC@?$CFs?1?$CFs?3?5current?5?$CFd?5?$DM?5max?5?$CFd?5?9?$DO?5de@ (0000)
0024D884 002f:
	??_C@_0CP@JPOKCDIH@?$CFs?1?$CFs?3?5unable?5to?5spawn?0?5out?5of?5s@ (0000)
0024D8B4 002e:
	??_C@_0CO@IIDKDHP@?$CFs?1?$CFs?3?5current?5?$CFd?5?$DM?5min?5?$CFd?5?9?$DO?5sp@ (0000)
0024D8E4 0018:
	??_C@_0BI@NJPCKEMP@?$CFs?1?$CFs?5triggered?5?$CFs?5rule?$AA@ (0000)
0024D8FC 000a:
	??_C@_09MEJNCKEO@attacking?$AA@ (0000)
0024D908 000a:
	??_C@_09GNBACBHC@defending?$AA@ (0000)
0024D914 0021:
	??_C@_0CB@JPLEGLHP@?$CFs?1?$CFs?5triggered?5maneuvering?5rule@ (0000)
0024D938 0030:
	??_C@_0DA@LKCBJOBO@?$CFs?3?5current?5?$CFs?1?$CF?41f?5best?5?$CFs?1?$CF?41f@ (0000)
0024D968 0004:
	__real@c47a0000 (0000)
0024D96C 0005:
	??_C@_04NEBKMGJO@stay?$AA@ (0000)
0024D974 0008:
	??_C@_07JBBGNPIP@migrate?$AA@ (0000)
0024D980 004d:
	??_C@_0EN@MMBDPONO@WARNING?3?5squad?5?$CFs?1?$CFs?5has?5an?5inva@ (0000)
0024D9D0 004b:
	??_C@_0EL@LEBBGDPA@WARNING?3?5actor?5changing?5to?5encou@ (0000)
0024DA1C 0035:
	??_C@_0DF@OEDIHECK@?$CBencounter?9?$DOenemy_visible?5?$CG?$CG?5?$CBen@ (0000)
0024DA54 002e:
	??_C@_0CO@DFDAGGAE@?$CFs?1?$CFs?3?5?$CFd?5current?5?$CFd?5leaders?0?5cr@ (0000)
0024DA84 000a:
	??_C@_09PONLBLAH@no?5leader?$AA@ (0000)
0024DA90 000b:
	??_C@_0L@JLIMGEDN@new?5leader?$AA@ (0000)
0024DA9C 000c:
	??_C@_0M@ELGJNJDG@ai_place?5?$CFs?$AA@ (0000)
0024DAA8 000f:
	??_C@_0P@IGLHCPJF@ai_place?5?$CFs?1?$CFs?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "ai/actor_activation.h"
#include "ai/actor_iterators.h"
#include "encounters.h"

#include "actions.h"
#include "actors.h"
#include "actor_definitions.h"
#include "actor_iterators.h"
#include "actor_placement.h"
#include "actor_types.h"
#include "ai.h"
#include "ai_debug.h"
#include "ai_communication.h"
#include "ai_profile.h"
#include "ai_script.h"
#include "ai_scenario_definitions.h"
#include "props.h"
#include "cseries/errors.h"
#include "editor/editor_stubs.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "main/console.h"
#include "math/integer_math.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "structures/structure_bsp_definitions.h"
#include "units/units.h"

#include <stddef.h>

/* ---------- constants */

enum
{
	MAXIMUM_ENCOUNTERS = 128,
	MAXIMUM_EXAMINED_PURSUIT_POSITIONS_PER_MAP = 256,
	NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION = 6,
	MAXIMUM_STARTING_LOCATIONS_PER_SQUAD = 64,
};

enum
{
	MAXIMUM_LINK_ENCOUNTERS_PER_ENCOUNTER = 3,
	MAXIMUM_FIRING_POSITIONS_PER_ENCOUNTER = 512,
	NUMBER_OF_POST_COMBAT_POSSIBILITIES = 2,
	NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES = 4,
	ENCOUNTER_REMAIN_ACTIVE_TIME = 150,
	ENCOUNTER_UPDATE_INTERVAL = TICKS_PER_SECOND/2,
	ENCOUNTER_ENEMY_RECENT_TICKS = TICKS_PER_SECOND*2,
	ENCOUNTER_ENEMY_MEMORY_TICKS = TICKS_PER_SECOND*15,
	SQUAD_DELAY_FOREVER_TICKS = 999,
	SQUAD_UNLIMITED_RESPAWN_ACTOR_COUNT = 999,
};

// encounter_definition.searching
enum
{
	_encounter_searching_normal = 0,
	_encounter_searching_never,
	_encounter_searching_forever,
	NUMBER_OF_ENCOUNTER_SEARCHING_TYPES,
};

// actor_type_definition.when_to_search_at_target/when_to_pursue/when_to_search_pursuit
enum
{
	_actor_pursuit_always = 0,
	_actor_pursuit_not_when_group,
	_actor_pursuit_never,
};

// encounter_determine_pursuit_availability
enum
{
	MAXIMUM_SEARCHING_ACTORS = 6,
	MAXIMUM_FLEEING_ACTORS = 4,
	MINIMUM_PURSUING_ACTORS = 3,
	UNLIMITED_PURSUING_ACTORS = 999,
	MINIMUM_GROUP_PURSUIT_ACTORS = 2,
};

enum group_pursuit_restriction
{
	_group_pursuit_normal = 0,
	_group_pursuit_nobody,
	_group_pursuit_everyone,
	NUMBER_OF_GROUP_PURSUIT_RESTRICTIONS,
};

// actor_state_data.mode/combat_status (TU-local until actors.h names them)
enum
{
	_actor_mode_alert = 2,
	_actor_mode_combat = 3,
};

enum
{
	_actor_combat_status_investigate = 2,
	_actor_combat_status_definite = 3,
	_actor_combat_status_visible = 7,
};

// encounter_definition.flags (TU-local until ai_scenario_definitions.h names them)
enum
{
	_encounter_not_initially_created_bit = 0,
	_encounter_respawn_enabled_bit,
	_encounter_blind_bit,
	_encounter_deaf_bit,
};

// squad_definition.unique_leader_type (TU-local until ai_scenario_definitions.h names them)
enum
{
	_unique_leader_type_normal = 0,
	_unique_leader_type_none,
	_unique_leader_type_random,
	_unique_leader_type_sergeant_johnson,
	_unique_leader_type_sergeant_lehto,
	NUMBER_OF_UNIQUE_LEADER_TYPES,
};

// encounter_update_follow: follow-target units and firing position letter groups (a..z)
#define MAXIMUM_FOLLOW_TARGET_UNITS 8
#define NUMBER_OF_FIRING_POSITION_GROUP_INDICES 26

enum
{
	_ai_reference_squad_bit = 15,
};

// actor_definition.flags2 (TU-local until actor_definitions.h names it)
enum
{
	_actor_definition_no_corpse_shooting_bit = 6,
};

/* ---------- macros */

#define pursuit_get(index) ((struct pursuit_datum *)datum_get(pursuit_data, (index)))
#define STRENGTH_FRACTION_EPSILON 0.001f

/* ---------- structures */

struct pursuit_datum
{
	short identifier;
	short firing_position_index;
	long last_examined_time;
	short actor_count;
	short next_actor_index_index;
	long actor_indices[NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION];
	long next_pursuit_index;
};

struct post_combat_possibility
{
	long actor_index;
	real weight;
	long prop_index;
	long unit_index;
};

struct encounter_iterator
{
	struct data_iterator data;
	long index;
	boolean active_only;
};

struct encounter_actor_iterator
{
	long encounter_index;
	long index;
	long next_index;
};

struct actor_iterator
{
	struct data_iterator encounter_iterator;
	boolean iterated_encounterless_list;
	boolean active_only;
	byte pad[2];
	long index;
	long next_index;
};

struct encounter_ai_globals_prefix
{
	boolean ai_active;
	boolean ai_initialized_for_map;
	boolean ai_has_control_data;
	boolean time_given_this_frame;
	short last_highest_service_timer;
	short current_highest_service_timer;
	long first_encounterless_actor_index;
};

typedef char encounter_iterator_size_assert[
	sizeof(struct encounter_iterator) == 0x18 ? 1 : -1];
typedef char encounter_iterator_index_offset_assert[
	offsetof(struct encounter_iterator, index) == 0x10 ? 1 : -1];
typedef char encounter_iterator_active_only_offset_assert[
	offsetof(struct encounter_iterator, active_only) == 0x14 ? 1 : -1];
typedef char encounter_actor_iterator_size_assert[
	sizeof(struct encounter_actor_iterator) == 0xC ? 1 : -1];
typedef char encounter_actor_iterator_index_offset_assert[
	offsetof(struct encounter_actor_iterator, index) == 0x4 ? 1 : -1];
typedef char encounter_actor_iterator_next_index_offset_assert[
	offsetof(struct encounter_actor_iterator, next_index) == 0x8 ? 1 : -1];
typedef char actor_iterator_size_assert[
	sizeof(struct actor_iterator) == 0x1C ? 1 : -1];
typedef char actor_iterator_iterated_encounterless_offset_assert[
	offsetof(struct actor_iterator, iterated_encounterless_list) == 0x10 ? 1 : -1];
typedef char actor_iterator_active_only_offset_assert[
	offsetof(struct actor_iterator, active_only) == 0x11 ? 1 : -1];
typedef char actor_iterator_index_offset_assert[
	offsetof(struct actor_iterator, index) == 0x14 ? 1 : -1];
typedef char actor_iterator_next_index_offset_assert[
	offsetof(struct actor_iterator, next_index) == 0x18 ? 1 : -1];
typedef char encounter_ai_globals_initialized_offset_assert[
	offsetof(struct encounter_ai_globals_prefix, ai_initialized_for_map) == 0x1 ? 1 : -1];
typedef char encounter_ai_globals_encounterless_actor_offset_assert[
	offsetof(struct encounter_ai_globals_prefix, first_encounterless_actor_index) == 0x8 ? 1 : -1];
typedef char encounter_datum_active_offset_assert[
	offsetof(struct encounter_datum, active) == 0xD ? 1 : -1];
typedef char encounter_datum_status_dirty_offset_assert[
	offsetof(struct encounter_datum, status_dirty) == 0x28 ? 1 : -1];
typedef char encounter_datum_first_actor_index_offset_assert[
	offsetof(struct encounter_datum, first_actor_index) == 0x14 ? 1 : -1];
typedef char encounter_datum_blind_offset_assert[
	offsetof(struct encounter_datum, blind) == 0x40 ? 1 : -1];
typedef char encounter_datum_deaf_offset_assert[
	offsetof(struct encounter_datum, deaf) == 0x41 ? 1 : -1];
typedef char actor_datum_next_actor_index_offset_assert[
	offsetof(struct actor_datum, meta.next_actor_index) == 0x2C ? 1 : -1];
typedef char pursuit_datum_size_assert[
	sizeof(struct pursuit_datum) == 0x28 ? 1 : -1];

/* ---------- prototypes */

static void encounter_update_squads(
	long encounter_index);
static void encounter_update_respawn(
	long encounter_index);
static void encounter_update_platoons(
	long encounter_index);
static boolean encounter_test_rule(
	long encounter_index,
	struct platoon_rule *rule);
static short encounter_post_combat_select_random_behavior(
	struct post_combat_possibility possibilities[NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES][NUMBER_OF_POST_COMBAT_POSSIBILITIES],
	struct post_combat_possibility *selected_possibility);
static void encounter_post_combat(
	long encounter_index);
static void encounter_update_follow(
	long encounter_index);
static void encounter_control_actors(
	long encounter_index);

static void encounter_clear_pursuit(
	long encounter_index);
static boolean encounter_place_actor(
	long encounter_index,
	short squad_index,
	short initial_variant,
	boolean spawning);
static void encounterless_deactivate(
	long actor_index);
static void encounters_test_activation(
	void);
static long encounter_find_pursuit(
	long encounter_index,
	short firing_position_index,
	long history_start_time,
	boolean force_create);
static void squad_reset_starting_locations(
	long encounter_index,
	short squad_index);
static void encounter_new(
	struct encounter_definition *encounter_definition,
	short *squad_base,
	short *platoon_base);
static boolean encounter_activate(
	long encounter_index);
static void encounter_deactivate(
	long encounter_index);
static void encounter_update_timers(
	long encounter_index);
static short squad_get_actor_type(
	struct squad_definition *squad_definition);

/* ---------- globals */

extern struct encounter_ai_globals_prefix *ai_globals;

struct data_array *encounter_data;
struct platoon_datum *platoon_array;
struct squad_datum *squad_array;
struct data_array *pursuit_data;

struct post_combat_possibility const global_empty_possibility =
{
	NONE,
	0.0f,
	NONE,
	NONE,
};

short const global_post_combat_translation_table[NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES] =
{
	_actor_postcombat_shoot_corpse,
	_actor_postcombat_check_enemy,
	_actor_postcombat_check_friend,
	_actor_postcombat_celebrate,
};

/* ---------- public code */

void encounters_initialize(
	void)
{
	encounter_data = game_state_data_new("encounter", MAXIMUM_ENCOUNTERS, sizeof(struct encounter_datum));
	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 110, encounter_data);

	squad_array = (struct squad_datum *)game_state_malloc("squad", "squad", MAXIMUM_SQUADS_PER_MAP * sizeof(struct squad_datum));
	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 113, squad_array);

	platoon_array = (struct platoon_datum *)game_state_malloc("platoon", "platoon", MAXIMUM_PLATOONS_PER_MAP * sizeof(struct platoon_datum));
	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 116, platoon_array);

	pursuit_data = game_state_data_new("ai pursuit", MAXIMUM_EXAMINED_PURSUIT_POSITIONS_PER_MAP, sizeof(struct pursuit_datum));
	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 119, pursuit_data);

	return;
}

void encounters_dispose(
	void)
{
	return;
}

void encounters_dispose_from_old_map(
	void)
{
	data_make_invalid(encounter_data);
	data_make_invalid(pursuit_data);
	return;
}

void encounter_compute_activation_cluster_bit_vector(
	long encounter_index,
	boolean update_actor_dormancy,
	long bit_vector_size,
	unsigned long const *active_area,
	unsigned long *bit_vector)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
		struct encounter_definition);
	unsigned long active_squad_mask = 0;
	unsigned long firing_position_group_mask = 0;
	long actor_index;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\encounters.c",
		355,
		structure_bsp->clusters.count <= bit_vector_size);
	csmemset(
		bit_vector,
		0,
		BIT_VECTOR_SIZE_IN_BYTES(structure_bsp->clusters.count));

	for (actor_index = encounter->first_actor_index;
		actor_index != NONE;)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct unit_datum *unit;
		boolean dormant_desire = TRUE;

		if (actor->meta.swarm)
		{
			long unit_index = actor->meta.swarm_unit_index;

			while (unit_index != NONE)
			{
				long ultimate_parent_index;
				struct object_datum *parent_object;
				short cluster_index;

				unit = unit_get(unit_index);
				ultimate_parent_index = object_get_ultimate_parent(unit_index);
				parent_object = object_get(ultimate_parent_index);
				cluster_index = parent_object->object.location.cluster_index;

				if (cluster_index != NONE)
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\encounters.c",
						382,
						(cluster_index >= 0) && (cluster_index < bit_vector_size));
					BIT_VECTOR_SET_FLAG(bit_vector, cluster_index, TRUE);
					if (active_area && BIT_VECTOR_TEST_FLAG(active_area, cluster_index))
						dormant_desire = FALSE;
				}

				unit_index = unit->unit.swarm_next_unit_index;
			}
		}
		else
		{
			long ultimate_parent_index;
			struct object_datum *parent_object;
			short cluster_index;

			unit = unit_get(actor->meta.unit_index);
			ultimate_parent_index = object_get_ultimate_parent(actor->meta.unit_index);
			parent_object = object_get(ultimate_parent_index);
			cluster_index = parent_object->object.location.cluster_index;
			if (cluster_index != NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\encounters.c",
					408,
					(cluster_index >= 0) && (cluster_index < bit_vector_size));
				BIT_VECTOR_SET_FLAG(bit_vector, cluster_index, TRUE);
				if (active_area && BIT_VECTOR_TEST_FLAG(active_area, cluster_index))
					dormant_desire = FALSE;
			}

			if (encounter->active)
			{
				if (actor->state.mode == _actor_mode_combat)
				{
					if (actor->state.combat_status >= _actor_combat_status_investigate)
					{
						firing_position_group_mask |= actor_get_firing_position_group(
							actor_index,
							_firing_point_evaluation_mode_pursue,
							_firing_position_group_when_searching);
					}

					if (actor->state.action == _actor_action_guard ||
						actor->state.action == _actor_action_flee)
					{
						firing_position_group_mask |= actor_get_firing_position_group(
							actor_index,
							_firing_point_evaluation_mode_guard,
							_firing_position_group_normal);
					}
					else if (actor->state.action == _actor_action_fight ||
						actor->state.action == _actor_action_uncover)
					{
						firing_position_group_mask |= actor_get_firing_position_group(
							actor_index,
							_firing_point_evaluation_mode_fight,
							_firing_position_group_normal);
					}
				}
				else if (actor->state.mode == _actor_mode_alert &&
					actor->state.action_data.alert.move_position_order)
				{
					SET_FLAG(active_squad_mask, actor->meta.squad_index, TRUE);
				}

				if (actor->target.target_prop_index != NONE)
				{
					struct prop_datum *prop = prop_get(actor->target.target_prop_index);
					short cluster_index = prop->body_location.cluster_index;

					if (cluster_index != NONE)
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\encounters.c",
							448,
							(cluster_index >= 0) && (cluster_index < bit_vector_size));
						BIT_VECTOR_SET_FLAG(bit_vector, cluster_index, TRUE);
					}
				}
			}
		}

		if (update_actor_dormancy)
		{
			struct squad_datum *squad = encounter_get_squad(
				encounter,
				actor->meta.squad_index);

			if (squad->disable_dormant)
				dormant_desire = FALSE;
			actor->meta.dormant_desire = dormant_desire;
		}

		actor_index = actor->meta.next_actor_index;
	}

	if (firing_position_group_mask)
	{
		short firing_position_index;

		for (firing_position_index = 0;
			firing_position_index < encounter_definition->firing_positions.count;
			firing_position_index++)
		{
			struct firing_position_definition *firing_position = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->firing_positions,
				firing_position_index,
				struct firing_position_definition);

			if (firing_position->cluster_index != NONE &&
				TEST_FLAG(firing_position_group_mask, firing_position->group_index))
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\encounters.c",
					487,
					(firing_position->cluster_index >= 0) && (firing_position->cluster_index < bit_vector_size));
				BIT_VECTOR_SET_FLAG(bit_vector, firing_position->cluster_index, TRUE);
			}
		}
	}

	if (active_squad_mask)
	{
		short squad_index;

		for (squad_index = 0;
			squad_index < encounter_definition->squads.count;
			squad_index++)
		{
			if (TEST_FLAG(active_squad_mask, squad_index))
			{
				struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					squad_index,
					struct squad_definition);
				short move_position_index;

				for (move_position_index = 0;
					move_position_index < squad_definition->move_positions.count;
					move_position_index++)
				{
					struct move_position_definition *move_position = TAG_BLOCK_GET_ELEMENT(
						&squad_definition->move_positions,
						move_position_index,
						struct move_position_definition);

					if (move_position->cluster_index != NONE)
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\encounters.c",
							511,
							(move_position->cluster_index >= 0) && (move_position->cluster_index < bit_vector_size));
						BIT_VECTOR_SET_FLAG(bit_vector, move_position->cluster_index, TRUE);
					}
				}
			}
		}
	}

	return;
}

void encounter_iterator_new(
	struct encounter_iterator *iterator,
	boolean active_only)
{
	if (ai_globals->ai_initialized_for_map)
	{
		data_iterator_new(&iterator->data, encounter_data);
		iterator->active_only = active_only;
	}

	return;
}

struct encounter_datum *encounter_iterator_next(
	struct encounter_iterator *iterator)
{
	struct encounter_datum *result = NULL;

	if (ai_globals->ai_initialized_for_map)
	{
		do
		{
			result = (struct encounter_datum *)data_iterator_next(&iterator->data);
		} while (result && iterator->active_only && !result->active);

		iterator->index = iterator->data.datum_index;
	}

	return result;
}

void encounter_actor_iterator_new(
	struct encounter_actor_iterator *iterator,
	long encounter_index)
{
	if (!ai_globals->ai_initialized_for_map)
		return;

	iterator->encounter_index = encounter_index;
	iterator->index = NONE;
	if (encounter_index == NONE)
		iterator->next_index = ai_globals->first_encounterless_actor_index;
	else
		iterator->next_index = encounter_get(encounter_index)->first_actor_index;

	return;
}

struct actor_datum *encounter_actor_iterator_next(
	struct encounter_actor_iterator *iterator)
{
	struct actor_datum *result = NULL;

	if (ai_globals->ai_initialized_for_map)
	{
		long next_index = iterator->next_index;

		iterator->index = next_index;
		if (next_index != NONE)
		{
			result = actor_get(next_index);
			iterator->next_index = result->meta.next_actor_index;
		}
	}

	return result;
}

void encounter_update_status(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;
	boolean real_enemy_targeted = FALSE;
	boolean post_combat_behavior_pending = FALSE;
	boolean had_visible_enemy = FALSE;
	boolean been_in_combat = FALSE;
	boolean stay_active;
	short i;

	encounter->enemy_alive = FALSE;
	encounter->enemy_visible = FALSE;
	encounter->current_fighting_count = 0;
	encounter->current_in_combat_count = 0;
	encounter->current_swarm_count = 0;
	encounter->current_count = 0;
	encounter->current_strength_fraction = 0.0f;
	for (i = 0; i < encounter->squad_count; ++i)
	{
		struct squad_datum *squad = encounter_get_squad(encounter, i);

		squad->current_swarm_count = 0;
		squad->current_count = 0;
		squad->current_strength_fraction = 0.0f;
	}
	for (i = 0; i < encounter->platoon_count; ++i)
	{
		struct platoon_datum *platoon = encounter_get_platoon(encounter, i);

		platoon->current_swarm_count = 0;
		platoon->current_count = 0;
		platoon->current_strength_fraction = 0.0f;
	}

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		struct squad_datum *squad = encounter_get_squad(encounter, actor->meta.squad_index);
		short body_count;
		real strength;

		if (actor->meta.unit_index != NONE)
		{
			strength = unit_get(actor->meta.unit_index)->object.body_vitality;
			body_count = 1;
		}
		else
		{
			strength = (real)actor->meta.swarm_unit_count/actor->meta.swarm_original_unit_count;
			body_count = actor->meta.swarm_unit_count;
		}

		if (actor->meta.platoon_index != NONE)
		{
			struct platoon_datum *platoon = encounter_get_platoon(encounter, actor->meta.platoon_index);

			platoon->current_count += body_count;
			platoon->current_swarm_count += actor->meta.swarm*body_count;
			platoon->current_strength_fraction += strength;
		}
		squad->current_count += body_count;
		squad->current_swarm_count += actor->meta.swarm*body_count;
		squad->current_strength_fraction += strength;
		encounter->current_count += body_count;
		encounter->current_swarm_count += actor->meta.swarm*body_count;
		encounter->current_in_combat_count += actor_in_combat(iterator.index)*body_count;
		encounter->current_fighting_count += actor_is_fighting(iterator.index)*body_count;
		encounter->current_strength_fraction += strength;

		if (actor->target.target_prop_index != NONE)
		{
			struct prop_datum *prop = prop_get(actor->target.target_prop_index);

			encounter->enemy_target = TRUE;
			if (!game_team_is_ally(actor->meta.team_index, prop->team_index))
				real_enemy_targeted = TRUE;
			actor_in_combat(iterator.index);
			if (actor->state.had_visible_enemy)
				had_visible_enemy = TRUE;
			if (actor->state.been_in_combat)
				been_in_combat = TRUE;

			if (actor->state.combat_status >= _actor_combat_status_visible)
			{
				encounter->enemy_visible = TRUE;
				encounter->enemy_alive = TRUE;
			}
			else if (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged)
			{
				if (!prop->dead)
					encounter->enemy_alive = TRUE;
			}
			else if (!TEST_FLAG(unit_get(prop->unit_index)->object.damage_flags, _object_dead_bit))
			{
				encounter->enemy_alive = TRUE;
			}
		}

		if (actor->external_orders.postcombat_type > _actor_postcombat_none)
			post_combat_behavior_pending = TRUE;
	}

	if (real_enemy_targeted)
		encounter->enemy_traitor = FALSE;

	stay_active = encounter->enemy_visible ||
		(encounter->enemy_visible_timer != NONE && encounter->enemy_visible_timer < ENCOUNTER_ENEMY_RECENT_TICKS) ||
		((encounter->enemy_alive || (encounter->enemy_alive_timer != NONE && encounter->enemy_alive_timer < ENCOUNTER_ENEMY_RECENT_TICKS)) &&
			encounter->enemy_visible_timer != NONE && encounter->enemy_visible_timer < ENCOUNTER_ENEMY_MEMORY_TICKS);
	if (stay_active)
	{
		encounter->stand_down = FALSE;
		encounter->post_combat = FALSE;
	}
	else if (encounter->stand_down)
	{
		encounter->post_combat = FALSE;
		encounter->prebattle_living_count = encounter->current_count;
		encounter->corpse_ignore_time = game_time_get();
		encounter->enemies_defeated = 0;
		if (!encounter->enemy_target)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\encounters.c",
				2156,
				!encounter->enemy_visible && !encounter->enemy_alive);
			encounter->enemy_visible_timer = NONE;
			encounter->enemy_alive_timer = NONE;
		}
	}
	else if (encounter->post_combat)
	{
		encounter->post_combat_delay = !post_combat_behavior_pending;
		if (!encounter->post_combat_delay_timer)
			encounter_stand_down(encounter_index);
	}
	else if (had_visible_enemy && been_in_combat)
	{
		encounter_post_combat(encounter_index);
	}
	else
	{
		encounter_stand_down(encounter_index);
	}

	if (encounter->original_count > 0)
	{
		encounter->current_strength_fraction = MAX(
			0.0f,
			encounter->current_strength_fraction/encounter->original_count - STRENGTH_FRACTION_EPSILON);
	}
	for (i = 0; i < encounter->squad_count; ++i)
	{
		struct squad_datum *squad = encounter_get_squad(encounter, i);

		squad->current_strength_fraction = MAX(
			0.0f,
			squad->current_strength_fraction/squad->original_count - STRENGTH_FRACTION_EPSILON);
	}
	for (i = 0; i < encounter->platoon_count; ++i)
	{
		struct platoon_datum *platoon = encounter_get_platoon(encounter, i);

		platoon->current_strength_fraction = MAX(
			0.0f,
			platoon->current_strength_fraction/platoon->original_count - STRENGTH_FRACTION_EPSILON);
	}
	encounter->status_dirty = FALSE;

	return;
}

void encounter_set_blind(
	long encounter_index,
	boolean blind)
{
	if (ai_globals->ai_initialized_for_map)
		encounter_get(encounter_index)->blind = blind;

	return;
}

void encounter_set_deaf(
	long encounter_index,
	boolean deaf)
{
	if (ai_globals->ai_initialized_for_map)
		encounter_get(encounter_index)->deaf = deaf;

	return;
}

void actor_iterator_new(
	struct actor_iterator *iterator,
	boolean active_only)
{
	if (!ai_globals->ai_initialized_for_map)
		return;

	data_iterator_new(&iterator->encounter_iterator, encounter_data);
	iterator->iterated_encounterless_list = FALSE;
	iterator->next_index = NONE;
	iterator->index = NONE;
	iterator->active_only = active_only;

	return;
}

void encounterless_activate(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\encounters.c",
		0x720,
		actor->meta.encounterless);
	actor->meta.encounterless_active_timer = 90;
	actor_set_active(actor_index, TRUE);

	return;
}

void encounters_update_dirty_status(
	void)
{
	struct encounter_iterator iterator;
	struct encounter_datum *encounter;

	encounter_iterator_new(&iterator, FALSE);
	while ((encounter = encounter_iterator_next(&iterator)) != NULL)
	{
		if (encounter->status_dirty)
			encounter_update_status(iterator.index);
	}

	return;
}

long encounter_get_by_name(
	char const *encounter_name)
{
	long encounter_index = NONE;
	struct scenario *scenario = global_scenario_get();

	if (scenario)
	{
		long i;

		for (i = 0; i < scenario->ai_encounters.count; ++i)
		{
			struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_encounters, i, struct encounter_definition);

			if (csstrncmp(encounter_definition->name, encounter_name, TAG_STRING_LENGTH+1) == 0)
			{
				encounter_index = i;
				break;
			}
		}
	}

	return encounter_index;
}

struct actor_datum *encounter_actor_iterator_prev(
	struct encounter_actor_iterator *iterator)
{
	struct actor_datum *result = NULL;

	if (ai_globals->ai_initialized_for_map)
	{
		long actor_index = encounter_get(iterator->encounter_index)->first_actor_index;
		long previous_index = NONE;

		while (actor_index != iterator->index && actor_index != NONE)
		{
			previous_index = actor_index;
			result = actor_get(actor_index);
			actor_index = result->meta.next_actor_index;
		}

		if (actor_index != iterator->index)
			return NULL;

		iterator->next_index = actor_index;
		iterator->index = previous_index;
	}

	return result;
}

struct actor_datum *actor_iterator_next(
	struct actor_iterator *iterator)
{
	struct actor_datum *result = NULL;

	if (ai_globals->ai_initialized_for_map)
	{
		if (iterator->next_index == NONE)
		{
			struct encounter_datum *encounter;

			do
			{
				encounter = (struct encounter_datum *)data_iterator_next(&iterator->encounter_iterator);
				if (!encounter)
					break;
				if (!iterator->active_only || encounter->active)
					iterator->next_index = encounter->first_actor_index;
			} while (iterator->next_index == NONE);

			if (!encounter && !iterator->iterated_encounterless_list)
			{
				iterator->next_index = ai_globals->first_encounterless_actor_index;
				iterator->iterated_encounterless_list = TRUE;
			}
		}

		do
		{
			long next_index = iterator->next_index;

			iterator->index = next_index;
			if (next_index == NONE)
				break;

			result = actor_get(next_index);
			iterator->next_index = result->meta.next_actor_index;
			if (iterator->active_only && !result->meta.active)
				result = NULL;
		} while (!result);
	}

	return result;
}

void encounter_modify_pursuit_desires(
	long encounter_index,
	short squad_index,
	boolean *pursue_tenacious,
	short *group_pursuit_restriction,
	boolean *group_pursuit_controller,
	short *desired_target_search,
	short *desired_pursuit,
	short *desired_pursuit_search)
{
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
		&encounter_definition->squads, squad_index, struct squad_definition);
	short searching = encounter_definition->searching;

	if (TEST_FLAG(squad_definition->flags, _squad_never_search_bit))
		searching = _encounter_searching_never;

	switch (searching)
	{
	case _encounter_searching_never:
		*group_pursuit_restriction = _group_pursuit_nobody;
		*desired_pursuit = _actor_pursuit_never;
		*desired_pursuit_search = _actor_pursuit_never;
		break;

	case _encounter_searching_forever:
		*pursue_tenacious = TRUE;
		*desired_target_search = _actor_pursuit_always;
		*desired_pursuit = _actor_pursuit_always;
		*desired_pursuit_search = _actor_pursuit_always;
		*group_pursuit_controller = FALSE;
		break;
	}

	return;
}

void encounter_determine_pursuit_availability(
	long encounter_index,
	long actor_index,
	short group_pursuit_restriction,
	boolean group_pursuit_controller,
	boolean *allow_target_uncover,
	boolean *allow_indefinite_target_uncover,
	boolean *allow_target_search,
	boolean *allow_pursuit,
	boolean *allow_pursuit_search,
	boolean *controlling_group_pursuit,
	boolean *controlled_by_group_pursuit,
	boolean *wait_after_pursuit)
{
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;
	short combat_count;
	short fleeing_count;
	short pursuing_count;
	short searching_count;
	short maximum_pursuing_count;

	encounter_actor_iterator_new(&iterator, encounter_index);
	combat_count = 0;
	fleeing_count = 0;
	pursuing_count = 0;
	searching_count = 0;
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		if (iterator.index != actor_index &&
			actor->external_orders.pursuit_is_coordinator == group_pursuit_controller)
		{
			if (actor->state.action == _actor_action_uncover)
			{
				if (actor->state.action_data.uncover.pursuit_location.type == _pursuit_location_target)
				{
					if (actor->state.combat_status < _actor_combat_status_definite)
						searching_count++;
				}
				else
				{
					pursuing_count++;
				}
			}
			else if (actor->state.action == _actor_action_search)
			{
				if (actor->state.action_data.search.pursuit_location.type == _pursuit_location_target)
					fleeing_count++;
				else
					pursuing_count++;
			}
		}

		if (actor->state.mode == _actor_mode_combat)
			combat_count++;
	}

	switch (group_pursuit_restriction)
	{
	case _group_pursuit_nobody:
		maximum_pursuing_count = 0;
		break;

	case _group_pursuit_everyone:
		maximum_pursuing_count = UNLIMITED_PURSUING_ACTORS;
		break;

	default:
		maximum_pursuing_count = MAX(MINIMUM_PURSUING_ACTORS, combat_count/3);
		break;
	}

	actor = actor_get(actor_index);
	if (group_pursuit_controller)
	{
		*controlling_group_pursuit = actor_pursuit_find_nearby_actors(actor_index, TRUE) >= MINIMUM_GROUP_PURSUIT_ACTORS;
		actor->external_orders.pursuit_is_coordinator = *controlling_group_pursuit;
	}
	else
	{
		actor_pursuit_find_nearby_actors(actor_index, FALSE);
		*controlled_by_group_pursuit = actor->external_orders.pursuit_group_prop_index != NONE;
		actor->external_orders.pursuit_is_coordinator = FALSE;
	}

	/* January and HCEA both test the two out-pointers, not their values. */
	*wait_after_pursuit = controlling_group_pursuit || controlled_by_group_pursuit;
	*allow_indefinite_target_uncover = searching_count < MAXIMUM_SEARCHING_ACTORS;
	*allow_target_search = fleeing_count < MAXIMUM_FLEEING_ACTORS;
	*allow_pursuit = *allow_pursuit_search = pursuing_count < maximum_pursuing_count;

	if (ai_debug.print_pursuit_checks && encounter_index == ai_debug.selected_squad_index &&
		(actor_index == ai_debug.selected_actor_index || ai_debug.selected_actor_index == NONE))
	{
		actor = actor_get(actor_index);
		csprintf(
			temporary,
			"%s %04X: coord %d current %d/%d/%d max %d/%d/%d allow %c%c%c",
			actor_type_get_name(actor->meta.type),
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index),
			group_pursuit_controller,
			searching_count,
			fleeing_count,
			pursuing_count,
			MAXIMUM_SEARCHING_ACTORS,
			MAXIMUM_FLEEING_ACTORS,
			maximum_pursuing_count,
			*allow_indefinite_target_uncover ? 'Y' : 'N',
			*allow_target_search ? 'Y' : 'N',
			*allow_pursuit ? 'Y' : 'N');
		error(_error_silent, temporary);
		console_printf(FALSE, temporary);
	}

	return;
}

boolean encounter_link_activation(
	long encounter_index,
	short link_encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	boolean result = FALSE;
	short i;

	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 1916, (link_encounter_index >= 0) && (link_encounter_index < global_scenario_get()->ai_encounters.count));

	for (i = 0; i < encounter->link_encounter_count; ++i)
	{
		if (encounter->link_encounter_indices[i] == link_encounter_index)
		{
			result = TRUE;
			break;
		}
	}

	if (!result && encounter->link_encounter_count < MAXIMUM_LINK_ENCOUNTERS_PER_ENCOUNTER)
	{
		encounter->link_encounter_indices[encounter->link_encounter_count] = link_encounter_index;
		encounter->link_encounter_count++;
		result = TRUE;
	}

	return result;
}

void encounterless_attach_actor(
	long actor_index)
{
	if (ai_globals->ai_initialized_for_map)
	{
		struct actor_datum *actor = actor_get(actor_index);

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 758, actor->meta.encounter_index==NONE);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 759, !actor->meta.encounterless);

		actor->meta.next_actor_index = ai_globals->first_encounterless_actor_index;
		ai_globals->first_encounterless_actor_index = actor_index;
		actor->meta.encounterless = TRUE;
		actor->meta.encounterless_active_timer = actor->meta.active ? 90 : 0;
		actor_flush_position_indices(actor_index);
	}

	return;
}

boolean encounter_mark_examined_pursuit_position(
	long encounter_index,
	long actor_index,
	short firing_position_index,
	long history_start_time)
{
	boolean marked = FALSE;
	long pursuit_index = encounter_find_pursuit(encounter_index, firing_position_index, history_start_time, TRUE);

	if (pursuit_index != NONE)
	{
		struct pursuit_datum *pursuit = pursuit_get(pursuit_index);
		short i;

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 1031, pursuit->firing_position_index == firing_position_index);

		for (i = 0; i < NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION; ++i)
		{
			if (pursuit->actor_indices[i] == actor_index)
				break;
		}

		if (i >= NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION)
		{
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 1044, (pursuit->next_actor_index_index >= 0) && (pursuit->next_actor_index_index < NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION));
			pursuit->actor_indices[pursuit->next_actor_index_index] = actor_index;
			pursuit->next_actor_index_index = (pursuit->next_actor_index_index+1) % NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION;
			pursuit->actor_count++;
			marked = TRUE;
		}

		pursuit->last_examined_time = game_time_get();
	}

	return marked;
}

boolean encounter_pursuit_position_already_examined(
	long encounter_index,
	long actor_index,
	short firing_position_index,
	long start_time,
	short *examined_count,
	long *last_examined_time_out)
{
	long pursuit_index = encounter_find_pursuit(encounter_index, firing_position_index, start_time, FALSE);
	long last_examined_time = NONE;
	boolean already_examined = FALSE;
	short actor_count = 0;

	if (pursuit_index != NONE)
	{
		struct pursuit_datum *pursuit = pursuit_get(pursuit_index);
		short i;

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 1076, pursuit->firing_position_index == firing_position_index);

		actor_count = pursuit->actor_count;
		last_examined_time = pursuit->last_examined_time;

		if (actor_count >= NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION+1)
		{
			already_examined = TRUE;
		}
		else
		{
			for (i = 0; i < NUMBER_OF_ACTOR_INDICES_PER_EXAMINED_PURSUIT_POSITION; i++)
			{
				if (pursuit->actor_indices[i] == actor_index)
				{
					already_examined = TRUE;
					break;
				}
			}
		}
	}

	if (examined_count)
		*examined_count = actor_count;
	if (last_examined_time_out)
		*last_examined_time_out = last_examined_time;

	return already_examined;
}

short encounter_get_actor_starting_location(
	long encounter_index,
	long squad_index,
	boolean spawning)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
	struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
		&encounter_definition->squads, squad_index, struct squad_definition);
	unsigned long assigned_locations[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_STARTING_LOCATIONS_PER_SQUAD)];
	short found_index = NONE;
	short index;

	csmemset(assigned_locations, 0, sizeof(assigned_locations));

	{
		short required_count = 0;

		for (index = 0; index < squad_definition->starting_locations.count; index++)
		{
			if (BIT_VECTOR_TEST_FLAG(squad->required_locations, index) &&
				!BIT_VECTOR_TEST_FLAG(assigned_locations, index))
			{
				required_count++;
			}
		}

		if (required_count > 0)
		{
			short random_index = seed_random_range(get_global_random_seed_address(), 0, required_count);

			for (index = 0; index < squad_definition->starting_locations.count; index++)
			{
				if (BIT_VECTOR_TEST_FLAG(squad->required_locations, index) &&
					!BIT_VECTOR_TEST_FLAG(assigned_locations, index))
				{
					if (!random_index)
					{
						BIT_VECTOR_SET_FLAG(squad->required_locations, index, FALSE);
						match_assert(
							"c:\\halo\\SOURCE\\ai\\encounters.c",
							1557,
							BIT_VECTOR_TEST_FLAG(squad->unused_locations, index));
						BIT_VECTOR_SET_FLAG(squad->unused_locations, index, FALSE);
						found_index = index;
						break;
					}
					random_index--;
				}
			}

			match_assert(
				"c:\\halo\\SOURCE\\ai\\encounters.c",
				1571,
				found_index != NONE);
		}
	}

	if (found_index == NONE)
	{
		short unused_count;
		boolean reset_unused;

		do
		{
			boolean unused_locations_available = FALSE;

			reset_unused = FALSE;
			unused_count = 0;
			for (index = 0; index < squad_definition->starting_locations.count; index++)
			{
				if (!BIT_VECTOR_TEST_FLAG(assigned_locations, index))
				{
					if (BIT_VECTOR_TEST_FLAG(squad->unused_locations, index))
						unused_count++;
					else
						unused_locations_available = TRUE;
				}
			}

			if (unused_locations_available && !unused_count)
			{
				csmemset(squad->unused_locations, NONE,
					BIT_VECTOR_SIZE_IN_BYTES(squad_definition->starting_locations.count));
				reset_unused = TRUE;
			}
		}
		while (reset_unused);

		if (unused_count > 0)
		{
			short random_index = seed_random_range(get_global_random_seed_address(), 0, unused_count);

			for (index = 0; index < squad_definition->starting_locations.count; index++)
			{
				if (BIT_VECTOR_TEST_FLAG(squad->unused_locations, index) &&
					!BIT_VECTOR_TEST_FLAG(assigned_locations, index))
				{
					if (!random_index)
					{
						BIT_VECTOR_SET_FLAG(squad->unused_locations, index, FALSE);
						found_index = index;
						break;
					}
					random_index--;
				}
			}

			match_assert(
				"c:\\halo\\SOURCE\\ai\\encounters.c",
				1637,
				found_index != NONE);
		}
	}

	return found_index;
}

void encounter_force_activate(
	long encounter_index)
{
	encounter_get(encounter_index)->remain_active_timer = ENCOUNTER_REMAIN_ACTIVE_TIME;
	encounter_activate(encounter_index);

	return;
}

void encounter_force_deactivate(
	long encounter_index)
{
	encounter_get(encounter_index)->remain_active_timer = 0;
	encounter_deactivate(encounter_index);

	return;
}

void encounter_set_respawn(
	long encounter_index,
	boolean respawn)
{
	if (ai_globals->ai_initialized_for_map)
	{
		encounter_get(encounter_index)->respawn_enabled = respawn;
		encounter_force_activate(encounter_index);
	}

	return;
}

void encounter_squad_timer_expire(
	long encounter_index,
	short squad_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
	struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
		&encounter_definition->squads, squad_index, struct squad_definition);

	squad->delay_timer = 0;

	if (TEST_FLAG(squad_definition->flags, _squad_magic_sight_after_timer_bit))
	{
		ai_scripting_magically_see_players(
			DATUM_INDEX_NEW(
				DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
				FLAG(_ai_reference_squad_bit) | (squad_index & UNSIGNED_CHAR_MAX)));
	}

	if (ai_debug.print_rules)
		console_printf(FALSE, "%s/%s: delay timer finished", encounter_definition->name, squad_definition->name);

	return;
}

void encounters_create_for_new_map(
	void)
{
	struct scenario *scenario = global_scenario_get();
	struct encounter_iterator iterator;
	struct encounter_datum *encounter;

	encounter_iterator_new(&iterator, FALSE);
	while ((encounter = encounter_iterator_next(&iterator)) != NULL)
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.index), struct encounter_definition);
		boolean create = !TEST_FLAG(encounter_definition->flags, _encounter_not_initially_created_bit);

		if (DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index) == DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.index))
			create = TRUE;

		if (create)
		{
			encounter_create(iterator.index, NONE, NONE);
		}
	}

	return;
}

void encounters_initialize_for_new_map(
	void)
{
	struct scenario *scenario = global_scenario_get();
	short squad_base = 0;
	short platoon_base = 0;
	short encounter_index;

	data_make_valid(encounter_data);
	data_make_valid(pursuit_data);
	csmemset(
		squad_array,
		0,
		MAXIMUM_SQUADS_PER_MAP * sizeof(struct squad_datum));
	csmemset(
		platoon_array,
		0,
		MAXIMUM_PLATOONS_PER_MAP * sizeof(struct platoon_datum));

	for (encounter_index = 0;
		encounter_index < scenario->ai_encounters.count;
		encounter_index++)
	{
		struct encounter_definition *encounter_definition =
			TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_encounters,
				encounter_index,
				struct encounter_definition);

		encounter_new(encounter_definition, &squad_base, &platoon_base);
	}

	return;
}

void encounters_unit_died(
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);

	if (unit->object.owner_team_index != NONE)
	{
		struct encounter_iterator iterator;
		struct encounter_datum *encounter;

		encounter_iterator_new(&iterator, TRUE);
		while ((encounter = encounter_iterator_next(&iterator)) != NULL)
		{
			if (game_team_is_enemy(encounter->team_index, unit->object.owner_team_index) &&
				encounter->enemy_target && !encounter->stand_down && !encounter->post_combat)
			{
				encounter->enemies_defeated++;
			}
		}
	}

	return;
}

void encounter_attach_unit(
	long encounter_index,
	long unit_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct unit_datum *unit = unit_get(unit_index);

	if (unit->unit.swarm_actor_index != NONE)
	{
		struct actor_datum *actor = actor_get(unit->unit.swarm_actor_index);

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 603, actor->meta.encounter_index == encounter_index);
	}
	else
	{
		struct actor_datum *actor = actor_get(unit->unit.actor_index);

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 609, actor->meta.encounter_index == encounter_index);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 610, actor->meta.unit_index == unit_index);
	}

	if (!game_engine_running() && encounter->team_index == 0)
	{
		encounter->team_index = unit->object.owner_team_index;
		if (encounter->first_actor_index != NONE)
			ai_update_team_status();
	}

	return;
}

boolean encounter_spawn_actor(
	long encounter_index,
	short squad_index)
{
	if (ai_globals->ai_initialized_for_map)
	{
		if (encounter_place_actor(encounter_index, squad_index, 0, TRUE))
		{
			struct encounter_datum *encounter = encounter_get(encounter_index);
			struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
			struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
			struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->squads, squad_index, struct squad_definition);

			encounter->current_count++;
			squad->current_count++;
			if (squad_definition->respawn_total_count > 0)
				squad->respawn_actors_left--;

			encounter->respawn_delay_ticks = real_random_range(encounter_definition->respawn_time_lower_bound, encounter_definition->respawn_time_upper_bound) * TICKS_PER_SECOND;
			squad->respawn_delay_ticks = real_random_range(squad_definition->respawn_time_lower_bound, squad_definition->respawn_time_upper_bound) * TICKS_PER_SECOND;
		}
	}

	/* Original January and HCEA behavior: the successful placement is not reported to the caller;
	 * this routine returns FALSE on every path. */
	return FALSE;
}

void encounter_verify_firing_position_owner_actor_indices(
	long encounter_index)
{
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	long owner_actor_indices[MAXIMUM_FIRING_POSITIONS_PER_ENCOUNTER];
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	csmemset(owner_actor_indices, NONE, encounter_definition->firing_positions.count*sizeof(long));

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		if (actor->firing_positions.current_position_index != NONE)
		{
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 302, actor->firing_positions.current_position_index>=0 && actor->firing_positions.current_position_index < encounter_definition->firing_positions.count);
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 303, owner_actor_indices[actor->firing_positions.current_position_index]==NONE);
			owner_actor_indices[actor->firing_positions.current_position_index] = iterator.index;
		}
	}

	return;
}

void encounter_build_firing_position_owner_actor_indices(
	long encounter_index,
	long *firing_position_owner_actor_indices)
{
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	csmemset(firing_position_owner_actor_indices, NONE, encounter_definition->firing_positions.count*sizeof(long));

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		if (actor->firing_positions.current_position_index != NONE)
		{
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 332, actor->firing_positions.current_position_index>=0 && actor->firing_positions.current_position_index < encounter_definition->firing_positions.count);
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 333, firing_position_owner_actor_indices[actor->firing_positions.current_position_index]==NONE);
			firing_position_owner_actor_indices[actor->firing_positions.current_position_index] = iterator.index;
		}
	}

	return;
}

void encounterless_detach_actor(
	long actor_index)
{
	if (ai_globals->ai_initialized_for_map)
	{
		struct actor_datum *actor = actor_get(actor_index);
		long *actor_index_reference;

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 782, actor->meta.encounterless);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 783, actor->meta.encounter_index == NONE);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 784, actor->meta.squad_index == NONE);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 785, actor->meta.platoon_index == NONE);

		for (actor_index_reference = &ai_globals->first_encounterless_actor_index;
			*actor_index_reference != actor_index;
			actor_index_reference = &actor_get(*actor_index_reference)->meta.next_actor_index)
		{
			match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 796, *actor_index_reference!=NONE);
		}

		*actor_index_reference = actor->meta.next_actor_index;
		actor->meta.encounterless = FALSE;
		actor->meta.next_actor_index = NONE;
		actor->meta.force_active = FALSE;
	}

	return;
}

void encounter_detach_actor(
	long actor_index,
	boolean died)
{
	if (ai_globals->ai_initialized_for_map)
	{
		struct actor_datum *actor = actor_get(actor_index);

		if (actor->meta.encounter_index != NONE)
		{
			struct encounter_datum *encounter = encounter_get(actor->meta.encounter_index);
			long *actor_index_reference;

			for (actor_index_reference = &encounter->first_actor_index;
				*actor_index_reference != actor_index;
				actor_index_reference = &actor_get(*actor_index_reference)->meta.next_actor_index)
			{
				match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 544, *actor_index_reference!=NONE);
			}
			*actor_index_reference = actor->meta.next_actor_index;

			if (!died)
			{
				struct squad_datum *squad = encounter_get_squad(encounter, actor->meta.squad_index);

				match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 556, encounter->original_count > 0);
				encounter->original_count--;

				if (actor->meta.unique_leader)
				{
					match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 561, encounter->unique_leader_count > 0);
					encounter->unique_leader_count--;
				}

				match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 565, squad->original_count > 0);
				squad->original_count--;

				if (actor->meta.platoon_index != NONE)
				{
					struct platoon_datum *platoon = encounter_get_platoon(encounter, actor->meta.platoon_index);

					match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 573, platoon->original_count > 0);
					platoon->original_count--;
				}
			}

			actor->meta.next_actor_index = NONE;
			actor->meta.encounter_index = NONE;
			actor->meta.platoon_index = NONE;
			actor->meta.squad_index = NONE;
			encounter->status_dirty = TRUE;
		}
	}

	return;
}

void encounter_stand_down(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 2431, !encounter->enemy_visible);

	encounter->stand_down = TRUE;
	encounter->enemies_defeated = 0;
	encounter_clear_pursuit(encounter_index);

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		struct prop_iterator prop_iterator;
		struct prop_datum *prop;

		prop_iterator_new(&prop_iterator, iterator.index);
		while ((prop = prop_iterator_next(&prop_iterator)) != NULL)
		{
			if (prop->state >= _prop_state_uninspected_orphan && prop->state <= _prop_state_inspected_orphan &&
				prop->enemy && prop_iterator.index != actor->target.target_prop_index)
			{
				struct prop_datum *parent_prop;

				match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 2458, prop->parent_prop_index != NONE);
				parent_prop = prop_get(prop->parent_prop_index);
				match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 2463, parent_prop->orphan_prop_index == prop_iterator.index);
				parent_prop->orphan_prop_index = NONE;

				actor_switch_props(iterator.index, prop_iterator.index, NONE);
				prop_delete(iterator.index, prop_iterator.index);
			}
		}
	}

	return;
}

void encounter_attach_actor(
	long actor_index,
	long encounter_index,
	short squad_index,
	boolean has_previous_team)
{
	if (ai_globals->ai_initialized_for_map)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct actor_definition *actor_definition = actor_definition_get(actor->meta.definition_index); // January fetches the definition but never reads it
		struct encounter_datum *encounter = encounter_get(encounter_index);
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
		struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
		struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
			&encounter_definition->squads, squad_index, struct squad_definition);
		short platoon_index = squad_definition->platoon_index;
		boolean activated = FALSE;

		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 650, actor->meta.encounter_index==NONE);

		actor->meta.disconnected_encounter_index = NONE;
		actor->meta.disconnected_squad_index = NONE;
		actor->meta.next_actor_index = encounter->first_actor_index;
		encounter->first_actor_index = actor_index;

		if (platoon_index < 0 || platoon_index >= encounter_definition->platoons.count)
			platoon_index = NONE;

		actor->meta.encounter_index = encounter_index;
		actor->meta.squad_index = squad_index;
		actor->meta.platoon_index = platoon_index;

		if (actor->meta.active && !actor->meta.dormant)
		{
			encounter_get(encounter_index)->remain_active_timer = ENCOUNTER_REMAIN_ACTIVE_TIME;
			activated = encounter_activate(encounter_index);
		}

		if (!activated)
		{
			actor_set_active(actor_index, encounter->active);
			if (encounter->active)
				actor_set_dormant(actor_index, FALSE);
		}

		if (actor->meta.unit_index != NONE)
			encounter_attach_unit(encounter_index, actor->meta.unit_index);

		if (actor->meta.team_index != encounter->team_index)
		{
			if (has_previous_team)
			{
				if (encounter->current_count == 0)
				{
					encounter->team_index = actor->meta.team_index;
					ai_update_team_status();
				}
				else
				{
					error(_error_silent, "WARNING: actor changing to encounter %s/%s is being forced to change teams", encounter_definition->name, squad_definition->name);
					actor_set_team(actor_index, encounter->team_index);
				}
			}
			else
			{
				actor_set_team(actor_index, encounter->team_index);
			}
		}

		encounter->original_count++;
		squad->original_count++;
		if (actor->meta.unique_leader)
			encounter->unique_leader_count++;

		if (platoon_index != NONE)
		{
			struct platoon_datum *platoon = encounter_get_platoon(encounter, platoon_index);

			actor->external_orders.defending = platoon->defending;
			actor->emotions.currently_defending = platoon->defending;
			platoon->original_count++;
		}

		encounter->status_dirty = TRUE;
	}

	return;
}

void encounter_create(
	long encounter_index,
	short desired_platoon_index,
	short desired_squad_index)
{
	if (ai_globals->ai_initialized_for_map)
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
		boolean all_squads = desired_platoon_index == NONE && desired_squad_index == NONE;
		short squad_index;

		if (ai_debug.print_placement)
		{
			if (desired_platoon_index != NONE)
			{
				console_printf(FALSE, "ai_place %s/%s", encounter_definition->name,
					TAG_BLOCK_GET_ELEMENT(&encounter_definition->platoons, desired_platoon_index, struct platoon_definition)->name);
			}
			else if (desired_squad_index != NONE)
			{
				console_printf(FALSE, "ai_place %s/%s", encounter_definition->name,
					TAG_BLOCK_GET_ELEMENT(&encounter_definition->squads, desired_squad_index, struct squad_definition)->name);
			}
			else
			{
				console_printf(FALSE, "ai_place %s", encounter_definition->name);
			}
		}

		for (squad_index = 0; squad_index < encounter_definition->squads.count; ++squad_index)
		{
			struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->squads, squad_index, struct squad_definition);
			short initial_variant;
			short count;
			short actor_type;
			short i;

			if (!all_squads && squad_index != desired_squad_index)
			{
				if (squad_definition->platoon_index == NONE || squad_definition->platoon_index != desired_platoon_index)
					continue;
			}

			initial_variant = 0;

			switch (game_difficulty_level_get())
			{
			case _game_difficulty_level_easy:
			case _game_difficulty_level_normal:
				count = squad_definition->min_count;
				break;
			case _game_difficulty_level_hard:
				count = (squad_definition->max_count + squad_definition->min_count) / 2;
				break;
			case _game_difficulty_level_impossible:
				count = squad_definition->max_count;
				break;
			/* count is left unassigned only by this default arm. Not reached unassigned: the
			 * arm's assertion failure calls system_exit, which does not return in January
			 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
			 * Source-policy approval pending (2026-09-27 audit). */
			default:
				match_vassert("c:\\halo\\SOURCE\\ai\\encounters.c", 1730, FALSE, NULL);
			}

			actor_type = squad_get_actor_type(squad_definition);

			switch (squad_definition->unique_leader_type)
			{
			case _unique_leader_type_normal:
				{
					struct encounter_datum *encounter = encounter_get(encounter_index);
					boolean create_leader = FALSE;

					if (actor_type == _actor_marine)
					{
						if (encounter->unique_leader_count == 0)
							create_leader = encounter->original_count + count >= 4;
						else if (encounter->unique_leader_count == 1)
							create_leader = encounter->original_count + count >= 10;

						if (ai_debug.print_placement)
						{
							console_printf(FALSE, "%s/%s: %d current %d leaders, create %d -> %s",
								encounter_definition->name, squad_definition->name,
								encounter->original_count, encounter->unique_leader_count, count,
								create_leader ? "new leader" : "no leader");
						}
					}

					if (!create_leader)
						break;
				}
				// fall through
			case _unique_leader_type_random:
				switch (actor_type)
				{
				case _actor_marine:
					initial_variant = 100 + random_range(0, 2);
					break;
				}
				break;
			case _unique_leader_type_sergeant_johnson:
				if (actor_type == _actor_marine)
					initial_variant = 100;
				break;
			case _unique_leader_type_sergeant_lehto:
				if (actor_type == _actor_marine)
					initial_variant = 101;
				break;
			case _unique_leader_type_none:
				break;
			}

			for (i = 0; i < count; ++i)
			{
				encounter_place_actor(encounter_index, squad_index, initial_variant, FALSE);
				initial_variant = 0;
			}
		}

		encounter_update_status(encounter_index);
		encounters_test_activation();
	}

	return;
}

void encounters_update(
	void)
{
	long time = game_time_get();
	short phase;
	struct encounter_iterator iterator;
	struct encounter_datum *encounter;

	if (time % TICKS_PER_SECOND == 0)
	{
		encounters_update_dirty_status();
		encounters_test_activation();
	}
	phase = time % ENCOUNTER_UPDATE_INTERVAL;

	encounter_iterator_new(&iterator, TRUE);
	while ((encounter = encounter_iterator_next(&iterator)) != NULL)
	{
		short encounter_phase = DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.index) % ENCOUNTER_UPDATE_INTERVAL;

		ai_profile.meters[_ai_meter_encounters_updated].accumulator++;
		if (encounter_phase == phase)
		{
			encounter_update_status(iterator.index);
			encounter_update_timers(iterator.index);
			encounter_update_respawn(iterator.index);
			encounter_update_squads(iterator.index);
			encounter_update_platoons(iterator.index);
			encounter_update_follow(iterator.index);
			encounter_control_actors(iterator.index);
		}
	}

	return;
}

/* ---------- private code */

static void encounter_clear_pursuit(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	long pursuit_index;

	for (pursuit_index = encounter->first_pursuit_index; pursuit_index != NONE; pursuit_index = encounter->first_pursuit_index)
	{
		encounter->first_pursuit_index = pursuit_get(pursuit_index)->next_pursuit_index;
		datum_delete(pursuit_data, pursuit_index);
	}

	return;
}

static long encounter_find_pursuit(
	long encounter_index,
	short firing_position_index,
	long history_start_time,
	boolean force_create)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	long pursuit_index = encounter->first_pursuit_index;
	boolean reset = FALSE;

	while (pursuit_index != NONE)
	{
		struct pursuit_datum *pursuit = pursuit_get(pursuit_index);

		if (pursuit->firing_position_index == firing_position_index)
		{
			if (pursuit->last_examined_time < history_start_time)
				reset = TRUE;
			break;
		}

		pursuit_index = pursuit->next_pursuit_index;
	}

	if (pursuit_index == NONE)
	{
		if (force_create)
		{
			pursuit_index = datum_new(pursuit_data);
			if (pursuit_index != NONE)
			{
				struct pursuit_datum *pursuit = pursuit_get(pursuit_index);

				pursuit->firing_position_index = firing_position_index;
				pursuit->next_pursuit_index = encounter->first_pursuit_index;
				encounter->first_pursuit_index = pursuit_index;
				reset = TRUE;
			}
			else
			{
				error(_error_silent, "WARNING: too many actors searching, exceeded MAXIMUM_EXAMINED_PURSUIT_POSITIONS_PER_MAP (%d)", MAXIMUM_EXAMINED_PURSUIT_POSITIONS_PER_MAP);
			}
		}
	}

	if (reset)
	{
		struct pursuit_datum *pursuit = pursuit_get(pursuit_index);

		pursuit->last_examined_time = NONE;
		pursuit->actor_count = 0;
		pursuit->next_actor_index_index = 0;
		csmemset(pursuit->actor_indices, NONE, sizeof(pursuit->actor_indices));

		if (!force_create)
			return NONE;
	}

	return pursuit_index;
}

static void squad_reset_starting_locations(
	long encounter_index,
	short squad_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
		struct encounter_definition);
	struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
	struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
		&encounter_definition->squads,
		squad_index,
		struct squad_definition);
	short starting_location_index;

	csmemset(
		squad->unused_locations,
		NONE,
		BIT_VECTOR_SIZE_IN_BYTES(squad_definition->starting_locations.count));

	for (starting_location_index = 0;
		starting_location_index < squad_definition->starting_locations.count;
		starting_location_index++)
	{
		struct actor_starting_location *starting_location =
			TAG_BLOCK_GET_ELEMENT(
				&squad_definition->starting_locations,
				starting_location_index,
				struct actor_starting_location);

		if (TEST_FLAG(
			starting_location->flags,
			_actor_starting_location_required_bit))
		{
			BIT_VECTOR_SET_FLAG(
				squad->required_locations,
				starting_location_index,
				TRUE);
		}
	}

	return;
}

static void encounter_new(
	struct encounter_definition *encounter_definition,
	short *squad_base,
	short *platoon_base)
{
	long encounter_index = datum_new(encounter_data);

	if (encounter_index != NONE)
	{
		struct encounter_datum *encounter = encounter_get(encounter_index);
		short squad_index;
		short platoon_index;

		encounter->team_index = encounter_definition->team_index;
		encounter->first_actor_index = NONE;
		encounter->first_pursuit_index = NONE;
		encounter->blind = TEST_FLAG(
			encounter_definition->flags,
			_encounter_blind_bit);
		encounter->deaf = TEST_FLAG(
			encounter_definition->flags,
			_encounter_deaf_bit);
		encounter->respawn_enabled = TEST_FLAG(
			encounter_definition->flags,
			_encounter_respawn_enabled_bit);
		encounter->respawn_delay_ticks = 0;
		encounter->enemy_traitor = FALSE;
		encounter->enemy_visible = FALSE;
		encounter->enemy_visible_timer = NONE;
		encounter->enemy_alive = FALSE;
		encounter->enemy_alive_timer = NONE;
		encounter->corpse_ignore_time = NONE;
		encounter->stand_down = TRUE;
		encounter->last_grenade_throw_time = NONE;
		encounter->link_encounter_count = 0;
		encounter->last_active_time = NONE;

		match_assert(
			"c:\\halo\\SOURCE\\ai\\encounters.c",
			1444,
			encounter_definition->squads.count <=
				MAXIMUM_SQUADS_PER_ENCOUNTER);

		encounter->squad_count = encounter_definition->squads.count;
		encounter->squad_base = *squad_base;
		*squad_base += encounter->squad_count;

		match_vassert(
			"c:\\halo\\SOURCE\\ai\\encounters.c",
			1448,
			*squad_base <= MAXIMUM_SQUADS_PER_MAP,
			csprintf(
				temporary,
				"overflowed MAXIMUM_SQUADS_PER_MAP (%d)",
				MAXIMUM_SQUADS_PER_MAP));

		for (squad_index = 0;
			squad_index < encounter->squad_count;
			squad_index++)
		{
			struct squad_datum *squad =
				encounter_get_squad(encounter, squad_index);
			struct squad_definition *squad_definition =
				TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					squad_index,
					struct squad_definition);

			squad->delay_timer_started = FALSE;
			if (TEST_FLAG(
				squad_definition->flags,
				_squad_delay_forever_bit))
			{
				squad->delay_timer = SQUAD_DELAY_FOREVER_TICKS;
			}
			else
			{
				squad->delay_timer = (short)(
					squad_definition->squad_delay_timer * TICKS_PER_SECOND);
			}

			squad->automatic_migration_target = TEST_FLAG(
				squad_definition->flags,
				_squad_automatic_migration_bit);
			squad_reset_starting_locations(encounter_index, squad_index);

			if (squad_definition->respawn_max_actors > 0 ||
				squad_definition->respawn_min_actors > 0)
			{
				squad->respawn_actors_left = squad_definition->respawn_total_count == 0 ?
					SQUAD_UNLIMITED_RESPAWN_ACTOR_COUNT :
					squad_definition->respawn_total_count;
			}
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\encounters.c",
			1483,
			encounter_definition->platoons.count <=
				MAXIMUM_PLATOONS_PER_ENCOUNTER);

		encounter->platoon_count = encounter_definition->platoons.count;
		encounter->platoon_base = *platoon_base;
		*platoon_base += encounter->platoon_count;

		match_vassert(
			"c:\\halo\\SOURCE\\ai\\encounters.c",
			1487,
			*platoon_base <= MAXIMUM_PLATOONS_PER_MAP,
			csprintf(
				temporary,
				"overflowed MAXIMUM_PLATOONS_PER_MAP (%d)",
				MAXIMUM_PLATOONS_PER_MAP));

		for (platoon_index = 0;
			platoon_index < encounter->platoon_count;
			platoon_index++)
		{
			struct platoon_datum *platoon =
				encounter_get_platoon(encounter, platoon_index);
			struct platoon_definition *platoon_definition =
				TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->platoons,
					platoon_index,
					struct platoon_definition);

			platoon->defending = TEST_FLAG(
				platoon_definition->flags,
				_platoon_initially_defending_bit);
		}
	}

	return;
}

static boolean encounter_activate(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);

	if (encounter_definition->runtime_structure_bsp_reference_index == NONE ||
		encounter_definition->runtime_structure_bsp_reference_index == global_structure_bsp_index)
	{
		if (!encounter->active)
		{
			struct encounter_actor_iterator iterator;

			encounter_actor_iterator_new(&iterator, encounter_index);
			while (encounter_actor_iterator_next(&iterator))
				actor_set_active(iterator.index, TRUE);
		}

		encounter->last_active_time = game_time_get();
		encounter->active = TRUE;
	}

	return encounter->active;
}

static void encounter_deactivate(
	long encounter_index)
{
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	encounter_get(encounter_index)->active = FALSE;
	encounter_clear_pursuit(encounter_index);

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		if (actor->meta.active)
			actor_set_active(iterator.index, FALSE);
		actor_verify_activation(iterator.index);
	}

	return;
}

static short squad_get_actor_type(
	struct squad_definition *squad_definition)
{
	struct scenario *scenario = global_scenario_get();
	short actor_palette_index = squad_definition->actor_palette_index;
	short actor_type = _actor_none;

	if (actor_palette_index >= 0 && actor_palette_index < scenario->ai_actor_palette.count)
	{
		long actor_variant_definition_index = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_actor_palette, actor_palette_index, struct tag_reference)->index;

		if (actor_variant_definition_index != NONE)
		{
			long actor_definition_index = actor_variant_definition_get(actor_variant_definition_index)->actor_reference.index;

			if (actor_definition_index != NONE)
				actor_type = actor_definition_get(actor_definition_index)->type;
		}
	}

	return actor_type;
}

static boolean encounter_post_combat_add_possibility(
	struct post_combat_possibility *possibility_array,
	long actor_index,
	real weight,
	long prop_index,
	long unit_index)
{
	boolean added = FALSE;
	short i, j;

	/* BUG (original): January and HCEA continue after inserting in slot zero,
	 * so one candidate can fill both slots. A corrected build should break
	 * after setting added below. */
	for (i = 0; i < NUMBER_OF_POST_COMBAT_POSSIBILITIES; ++i)
	{
		if (weight > possibility_array[i].weight)
		{
			for (j = NUMBER_OF_POST_COMBAT_POSSIBILITIES-1; j > i; --j)
				possibility_array[j] = possibility_array[j-1];

			possibility_array[i].actor_index = actor_index;
			possibility_array[i].weight = weight;
			possibility_array[i].prop_index = prop_index;
			possibility_array[i].unit_index = unit_index;
			added = TRUE;
		}
	}

	return added;
}

static short encounter_post_combat_select_random_behavior(
	struct post_combat_possibility possibilities[NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES][NUMBER_OF_POST_COMBAT_POSSIBILITIES],
	struct post_combat_possibility *selected_possibility)
{
	real total_weight = 0.0f;
	short selected_behavior_index = NONE;
	short possibility_count = 0;
	short behavior_index;

	for (behavior_index = 0;
		behavior_index < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES;
		behavior_index++)
	{
		if (possibilities[behavior_index][0].weight > 0.0f && possibilities[behavior_index][0].actor_index != NONE)
		{
			total_weight += possibilities[behavior_index][0].weight;
			selected_behavior_index = behavior_index;
			possibility_count++;
		}
	}

	if (possibility_count > 1)
	{
		real current_weight = 0.0f;
		real selected_weight = real_seed_random(get_global_random_seed_address()) * total_weight;

		for (behavior_index = 0;
			behavior_index < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES;
			behavior_index++)
		{
			if (possibilities[behavior_index][0].weight > 0.0f && possibilities[behavior_index][0].actor_index != NONE)
			{
				current_weight += possibilities[behavior_index][0].weight;
				if (current_weight > selected_weight)
				{
					selected_behavior_index = behavior_index;
					break;
				}
			}
		}
	}

	if (selected_behavior_index != NONE)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\encounters.c",
			2562,
			(selected_behavior_index >= 0) && (selected_behavior_index < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES));
		*selected_possibility = possibilities[selected_behavior_index][0];
	}

	return selected_behavior_index;
}

static void encounter_post_combat(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	short primary_postcombat_behaviors[NUMBER_OF_POST_COMBAT_POSSIBILITIES];
	struct post_combat_possibility primary_possibilities[NUMBER_OF_POST_COMBAT_POSSIBILITIES];
	struct post_combat_possibility possibilities[NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES][NUMBER_OF_POST_COMBAT_POSSIBILITIES];
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;
	short postcombat_behavior;
	long behavior_actor_index;
	long secondary_reply_actor_index;
	long secondary_reply_target_prop_index;
	boolean found_possibility;
	short behavior_index;
	short possibility_index;
	short primary_behavior_index;

	csmemset(primary_postcombat_behaviors, NONE, sizeof(primary_postcombat_behaviors));
	postcombat_behavior = NONE;
	behavior_actor_index = NONE;
	secondary_reply_actor_index = NONE;
	for (behavior_index = 0; behavior_index < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES; behavior_index++)
	{
		for (possibility_index = 0; possibility_index < NUMBER_OF_POST_COMBAT_POSSIBILITIES; possibility_index++)
			possibilities[behavior_index][possibility_index] = global_empty_possibility;
	}
	found_possibility = FALSE;

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		struct actor_definition *actor_definition = actor_definition_get(actor->meta.definition_index);
		boolean found_enemy = FALSE;

		if (actor->meta.unit_index != NONE)
		{
			real player_rating = ai_communication_get_player_rating(actor->meta.unit_index, TRUE, NULL, NULL);
			struct prop_iterator prop_iterator;
			struct prop_datum *prop;

			prop_iterator_new(&prop_iterator, iterator.index);
			while ((prop = prop_iterator_next(&prop_iterator)) != NULL)
			{
				if (prop->dead)
				{
					real maximum_distance;
					real extra_weight;

					if (prop->enemy)
					{
						if (TEST_FLAG(actor_definition->flags2, _actor_definition_no_corpse_shooting_bit) ||
							prop->dead_ticks >= 210)
						{
							behavior_index = 1;
							maximum_distance = 5.0f;
							extra_weight = 0.0f;
						}
						else
						{
							behavior_index = 0;
							maximum_distance = 10.0f;
							extra_weight = 0.7f;
						}
					}
					else
					{
						behavior_index = 2;
						maximum_distance = 9.0f;
						extra_weight = 0.4f;
					}

					if (prop->distance < maximum_distance)
					{
						real distance_factor = MIN(2.0f, maximum_distance / prop->distance);
						real rating = MAX(player_rating, 1.5f);
						real age = MIN(1.0f, (real)prop->dead_ticks * 0.0041666669f);
						real weight = (2.0f - age) * rating * distance_factor + extra_weight;

						if (prop->player)
							weight += 2.0f;
						if (prop->enemy)
							found_enemy = TRUE;

						if (encounter_post_combat_add_possibility(
							&possibilities[behavior_index][0],
							iterator.index,
							weight,
							prop_iterator.index,
							prop->unit_index))
						{
							found_possibility = TRUE;
						}
					}
				}
			}

			if (found_enemy)
			{
				struct unit_datum *unit = unit_get(actor->meta.unit_index);

				if (encounter_post_combat_add_possibility(
					&possibilities[3][0],
					iterator.index,
					(real)unit->unit.killing_spree_count * 0.7f + player_rating,
					NONE,
					NONE))
				{
					found_possibility = TRUE;
				}
			}
		}
	}

	if (found_possibility)
	{
		primary_postcombat_behaviors[0] = encounter_post_combat_select_random_behavior(
			possibilities,
			&primary_possibilities[0]);

		if (encounter->team_index != _game_team_human || encounter->enemies_defeated >= 8)
		{
			found_possibility = FALSE;
			for (behavior_index = 0; behavior_index < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES; behavior_index++)
			{
				if (behavior_index == primary_postcombat_behaviors[0])
				{
					for (possibility_index = 0; possibility_index < NUMBER_OF_POST_COMBAT_POSSIBILITIES; possibility_index++)
						possibilities[behavior_index][possibility_index] = global_empty_possibility;
				}
				else if (possibilities[behavior_index][0].actor_index == primary_possibilities[0].actor_index ||
					possibilities[behavior_index][0].unit_index == primary_possibilities[0].unit_index)
				{
					for (possibility_index = 0; possibility_index < NUMBER_OF_POST_COMBAT_POSSIBILITIES - 1; possibility_index++)
						possibilities[behavior_index][possibility_index] = possibilities[behavior_index][possibility_index + 1];
					possibilities[behavior_index][NUMBER_OF_POST_COMBAT_POSSIBILITIES - 1] = global_empty_possibility;
				}

				if (possibilities[behavior_index][0].actor_index != NONE)
					found_possibility = TRUE;
			}

			if (found_possibility)
			{
				primary_postcombat_behaviors[1] = encounter_post_combat_select_random_behavior(
					possibilities,
					&primary_possibilities[1]);
			}
		}
	}

	if (encounter->team_index != _game_team_human || encounter->enemies_defeated >= 4)
	{
		struct encounter_actor_iterator reply_iterator;
		long best_actor_index = NONE;
		real best_actor_rating = 0.0f;

		encounter_actor_iterator_new(&reply_iterator, encounter_index);
		while ((actor = encounter_actor_iterator_next(&reply_iterator)) != NULL)
		{
			if (actor->meta.unit_index != NONE)
			{
				real rating = ai_communication_get_player_rating(actor->meta.unit_index, TRUE, NULL, NULL);

				if (!actor_communication_team(reply_iterator.index) &&
					rating > 2.0f &&
					rating > best_actor_rating)
				{
					best_actor_index = reply_iterator.index;
					best_actor_rating = rating;
				}
			}
		}

		behavior_actor_index = best_actor_index;
		if (best_actor_index != NONE)
		{
			struct actor_datum *behavior_actor = actor_get(best_actor_index);
			boolean find_secondary_reply = FALSE;

			if (behavior_actor->input.body_vitality < 0.5f &&
				behavior_actor->emotions.original_body_vitality - behavior_actor->input.body_vitality > 0.3f)
			{
				postcombat_behavior = _actor_postcombat_speak_wounded;
				find_secondary_reply = TRUE;
			}
			else
			{
				if (encounter->current_count == 1 && encounter->prebattle_living_count > 1)
				{
					postcombat_behavior = _actor_postcombat_speak_alone;
				}
				else if (encounter->current_count >= 2 &&
					encounter->prebattle_living_count >= encounter->current_count + MIN(encounter->current_count, 2))
				{
					postcombat_behavior = _actor_postcombat_speak_massacre;
				}
				else if (encounter->current_count >= 2 &&
					encounter->current_count >= encounter->prebattle_living_count - 1)
				{
					postcombat_behavior = _actor_postcombat_speak_triumph;
				}
				else if (behavior_actor->input.body_vitality > 0.8f)
				{
					postcombat_behavior = _actor_postcombat_speak_unscathed;
				}
			}

			if (find_secondary_reply)
			{
				struct encounter_actor_iterator search_iterator;
				long closest_actor_index = NONE;
				long closest_prop_index = NONE;
				real closest_distance_squared = REAL_MAX;

				encounter_actor_iterator_new(&search_iterator, encounter_index);
				while ((actor = encounter_actor_iterator_next(&search_iterator)) != NULL)
				{
					if (actor->meta.unit_index != NONE && search_iterator.index != best_actor_index)
					{
						real distance_squared = distance_squared3d(
							&behavior_actor->input.position.head_position,
							&actor->input.position.head_position);

						if (closest_distance_squared == REAL_MAX ||
							distance_squared < closest_distance_squared * closest_distance_squared)
						{
							long active_prop_index = prop_get_active_by_unit_index(
								search_iterator.index,
								behavior_actor->meta.unit_index);

							if (active_prop_index != NONE)
							{
								closest_distance_squared = distance_squared;
								closest_actor_index = search_iterator.index;
								closest_prop_index = active_prop_index;
							}
						}
					}
				}

				if (closest_actor_index != NONE)
				{
					secondary_reply_actor_index = closest_actor_index;
					secondary_reply_target_prop_index = closest_prop_index;
				}
			}
		}
	}

	for (primary_behavior_index = 0; primary_behavior_index < NUMBER_OF_POST_COMBAT_POSSIBILITIES; primary_behavior_index++)
	{
		if (primary_postcombat_behaviors[primary_behavior_index] != NONE &&
			primary_possibilities[primary_behavior_index].actor_index != NONE)
		{
			struct actor_datum *primary_actor = actor_get(primary_possibilities[primary_behavior_index].actor_index);

			match_assert(
				"c:\\halo\\SOURCE\\ai\\encounters.c",
				2927,
				(primary_postcombat_behaviors[primary_behavior_index] >= 0) && (primary_postcombat_behaviors[primary_behavior_index] < NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES));
			primary_actor->external_orders.postcombat_type =
				global_post_combat_translation_table[primary_postcombat_behaviors[primary_behavior_index]];
			primary_actor->external_orders.postcombat_prop_index = primary_possibilities[primary_behavior_index].prop_index;
		}
	}

	if (postcombat_behavior != NONE && behavior_actor_index != NONE)
	{
		struct actor_datum *behavior_actor = actor_get(behavior_actor_index);

		behavior_actor->external_orders.postcombat_type = postcombat_behavior;
		behavior_actor->external_orders.postcombat_prop_index = NONE;
		if (secondary_reply_actor_index != NONE)
		{
			struct actor_datum *secondary_reply_actor = actor_get(secondary_reply_actor_index);

			secondary_reply_actor->external_orders.postcombat_type = _actor_postcombat_run_to;
			secondary_reply_actor->external_orders.postcombat_prop_index = secondary_reply_target_prop_index;
		}
	}

	encounter->post_combat = TRUE;
	encounter->post_combat_delay = FALSE;
	encounter->post_combat_delay_timer = 120;
	encounter->enemies_defeated = 0;

	return;
}

static boolean encounter_place_actor(
	long encounter_index,
	short squad_index,
	short initial_variant,
	boolean spawning)
{
	boolean placed = FALSE;
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
		&encounter_definition->squads, squad_index, struct squad_definition);
	short starting_location_index = encounter_get_actor_starting_location(encounter_index, squad_index, spawning);

	if (starting_location_index != NONE)
	{
		struct actor_starting_location *starting_location = TAG_BLOCK_GET_ELEMENT(
			&squad_definition->starting_locations, starting_location_index, struct actor_starting_location);
		short actor_palette_index = squad_definition->actor_palette_index;
		struct scenario *scenario = global_scenario_get();

		if (starting_location->actor_variant_index != NONE)
			actor_palette_index = starting_location->actor_variant_index;

		if (VALID_INDEX(actor_palette_index, scenario->ai_actor_palette.count))
		{
			struct tag_reference *actor_palette_entry = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_actor_palette, actor_palette_index, struct tag_reference);

			if (actor_palette_entry->index != NONE)
			{
				struct actor_variant_definition *actor_variant_definition = actor_variant_definition_get(actor_palette_entry->index);
				boolean upgrade_major = FALSE;

				if (actor_variant_definition->major_upgrade_reference.index != NONE)
				{
					boolean random = FALSE;
					real chance = 0.f;

					ai_get_major_upgrade_chance(squad_definition->major_upgrade, &upgrade_major, &random, &chance);
					if (random)
						upgrade_major = ai_consider_major_upgrade(encounter_index, squad_index, chance);
				}

				placed = actor_place(actor_palette_entry->index, encounter_index, squad_index, starting_location, upgrade_major, initial_variant) != NONE;
			}
		}
		else
		{
			error(_error_silent, "WARNING: cannot spawn actors in %s/%s because the actor variant specified for this squad is NONE or invalid", encounter_definition->name, squad_definition->name);
		}
	}

	return placed;
}

static void encounter_update_timers(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);

	if (encounter->enemy_visible)
		encounter->enemy_visible_timer = 0;
	else if (encounter->enemy_visible_timer != NONE)
		encounter->enemy_visible_timer += ENCOUNTER_UPDATE_INTERVAL;

	if (encounter->enemy_alive)
		encounter->enemy_alive_timer = 0;
	else if (encounter->enemy_alive_timer != NONE)
		encounter->enemy_alive_timer += ENCOUNTER_UPDATE_INTERVAL;

	if (encounter->post_combat && encounter->post_combat_delay)
	{
		if (encounter->post_combat_delay_timer > ENCOUNTER_UPDATE_INTERVAL)
			encounter->post_combat_delay_timer -= ENCOUNTER_UPDATE_INTERVAL;
		else
			encounter->post_combat_delay_timer = 0;
	}

	return;
}

static boolean encounter_test_rule(
	long encounter_index,
	struct platoon_rule *rule)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	short platoon_index = rule->platoon_index;
	short original_count;
	real current_strength_fraction;
	short current_count;
	boolean result = FALSE;

	if (platoon_index >= 0 && platoon_index < encounter->platoon_count)
	{
		struct platoon_datum *platoon =
			encounter_get_platoon(encounter, platoon_index);

		original_count = platoon->original_count;
		current_strength_fraction = platoon->current_strength_fraction;
		current_count = platoon->current_count;
	}
	else
	{
		original_count = encounter->original_count;
		current_count = encounter->current_count;
		current_strength_fraction = encounter->current_strength_fraction;
	}

	if (original_count > 0)
	{
		switch (rule->rule_type)
		{
			case _platoon_rule_75_strength:
				result = current_strength_fraction < 0.75f;
				break;
			case _platoon_rule_50_strength:
				result = current_strength_fraction < 0.5f;
				break;
			case _platoon_rule_25_strength:
				result = current_strength_fraction < 0.25f;
				break;
			case _platoon_rule_anybody_dead:
				result = current_count < original_count;
				break;
			case _platoon_rule_25_dead:
				result = current_count * 4 / 3 <= original_count;
				break;
			case _platoon_rule_50_dead:
				result = current_count * 2 <= original_count;
				break;
			case _platoon_rule_75_dead:
				result = current_count * 4 <= original_count;
				break;
			case _platoon_rule_all_but_one_dead:
				result = current_count <= 1;
				break;
			case _platoon_rule_all_dead:
				result = current_count == 0;
				break;
			case _platoon_rule_never:
				result = FALSE;
				break;
			default:
				result = FALSE;
				break;
		}
	}

	if (ai_debug.print_rule_values && result)
	{
		switch (rule->rule_type)
		{
			case _platoon_rule_75_strength:
				console_printf(FALSE, "strength %.2f < 75%%", current_strength_fraction);
				break;
			case _platoon_rule_50_strength:
				console_printf(FALSE, "strength %.2f < 50%%", current_strength_fraction);
				break;
			case _platoon_rule_25_strength:
				console_printf(FALSE, "strength %.2f < 25%%", current_strength_fraction);
				break;
			case _platoon_rule_anybody_dead:
				console_printf(FALSE, "survivors %d < total %d", current_count, original_count);
				break;
			case _platoon_rule_25_dead:
				console_printf(FALSE, "survivors %d <= 25%% of total %d", current_count, original_count);
				break;
			case _platoon_rule_50_dead:
				console_printf(FALSE, "survivors %d <= 50%% of total %d", current_count, original_count);
				break;
			case _platoon_rule_75_dead:
				console_printf(FALSE, "survivors %d <= 75%% of total %d", current_count, original_count);
				break;
			case _platoon_rule_all_but_one_dead:
				console_printf(FALSE, "survivors %d <= 1", current_count);
				break;
			case _platoon_rule_all_dead:
				console_printf(FALSE, "survivors %d = 0", current_count);
				break;
			default:
				break;
		}
	}

	return result;
}

static void encounter_update_respawn(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition;
	unsigned long respawn_squads[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_SQUADS_PER_ENCOUNTER)];
	short respawn_squad_count;
	short squad_index;

	if (!encounter->respawn_enabled)
		return;

	if (encounter->respawn_delay_ticks > ENCOUNTER_UPDATE_INTERVAL)
	{
		encounter->respawn_delay_ticks -= ENCOUNTER_UPDATE_INTERVAL;
		return;
	}

	encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
		struct encounter_definition);
	encounter->respawn_delay_ticks = 0;
	respawn_squad_count = 0;
	csmemset(respawn_squads, 0, sizeof(respawn_squads));

	for (squad_index = 0; squad_index < encounter_definition->squads.count; ++squad_index)
	{
		struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
		struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
			&encounter_definition->squads,
			squad_index,
			struct squad_definition);

		while (squad->respawn_actors_left > 0 && squad->current_count < squad_definition->respawn_min_actors)
		{
			if (ai_debug.print_respawn)
			{
				console_printf(
					FALSE,
					"%s/%s: current %d < min %d -> spawn (%d left)",
					encounter_definition->name,
					squad_definition->name,
					squad->current_count,
					squad_definition->respawn_min_actors,
					squad->respawn_actors_left);
			}

			if (!encounter_spawn_actor(encounter_index, squad_index))
			{
				if (ai_debug.print_respawn)
				{
					console_printf(
						FALSE,
						"%s/%s: unable to spawn, out of starting points",
						encounter_definition->name,
						squad_definition->name);
				}
				break;
			}
		}

		if (squad->respawn_actors_left > 0 && squad->current_count < squad_definition->respawn_max_actors)
		{
			if (squad->respawn_delay_ticks > ENCOUNTER_UPDATE_INTERVAL)
			{
				squad->respawn_delay_ticks -= ENCOUNTER_UPDATE_INTERVAL;
			}
			else
			{
				respawn_squad_count++;
				squad->respawn_delay_ticks = 0;
				BIT_VECTOR_SET_FLAG(respawn_squads, squad_index, TRUE);
			}

			if (ai_debug.print_respawn)
			{
				console_printf(
					FALSE,
					"%s/%s: current %d < max %d -> desire spawn",
					encounter_definition->name,
					squad_definition->name,
					squad->current_count,
					squad_definition->respawn_max_actors);
			}
		}
	}

	if (respawn_squad_count > 0 && !encounter->respawn_delay_ticks)
	{
		short respawn_squad_index = seed_random_range(
			get_global_random_seed_address(),
			0,
			respawn_squad_count);

		for (squad_index = 0; squad_index < encounter->squad_count; ++squad_index)
		{
			if (BIT_VECTOR_TEST_FLAG(respawn_squads, squad_index))
			{
				if (respawn_squad_index > 0)
				{
					respawn_squad_index--;
				}
				else if (encounter_spawn_actor(encounter_index, squad_index))
				{
					if (ai_debug.print_respawn)
					{
						struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
							&encounter_definition->squads,
							squad_index,
							struct squad_definition);

						console_printf(
							FALSE,
							"%s/%s: randomly selected to spawn",
							encounter_definition->name,
							squad_definition->name);
					}
					return;
				}
				else if (ai_debug.print_respawn)
				{
					struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
						&encounter_definition->squads,
						squad_index,
						struct squad_definition);

					console_printf(
						FALSE,
						"%s/%s: unable to spawn, out of starting points",
						encounter_definition->name,
						squad_definition->name);
				}
			}
		}
	}

	return;
}

static void encounter_update_platoons(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
		struct encounter_definition);
	short platoon_index;

	for (platoon_index = 0;
		platoon_index < encounter->platoon_count;
		platoon_index++)
	{
		struct platoon_datum *platoon =
			encounter_get_platoon(encounter, platoon_index);

		if (platoon->current_count > 0)
		{
			struct platoon_definition *platoon_definition =
				TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->platoons,
					platoon_index,
					struct platoon_definition);

			if (!platoon->maneuvering)
			{
				platoon->maneuvering = encounter_test_rule(
					encounter_index,
					&platoon_definition->maneuvering_rule);

				if (platoon->maneuvering && ai_debug.print_rules)
				{
					console_printf(
						FALSE,
						"%s/%s triggered maneuvering rule",
						encounter_definition->name,
						platoon_definition->name);
				}
			}

			if (platoon->maneuver_disable || !platoon->maneuvering)
			{
				boolean defending = !TEST_FLAG(
					platoon_definition->flags,
					_platoon_initially_defending_bit);

				if (platoon->defending != defending &&
					encounter_test_rule(
						encounter_index,
						&platoon_definition->attacking_defending_rule))
				{
					platoon->defending = defending;
					if (ai_debug.print_rules)
					{
						console_printf(
							FALSE,
							"%s/%s triggered %s rule",
							encounter_definition->name,
							platoon_definition->name,
							defending ? "defending" : "attacking");
					}
				}
			}
		}
	}

	return;
}

// encounter_datum.follow_target_type (TU-local until encounters.h names them)
enum
{
	_follow_target_none = 0,
	_follow_target_players,
	_follow_target_unit,
	_follow_target_ai,
	NUMBER_OF_FOLLOW_TARGET_TYPES,
};

static void encounter_update_follow(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
		struct encounter_definition);
	long target_unit_indices[MAXIMUM_FOLLOW_TARGET_UNITS];
	long follow_unit_index = NONE;
	short target_count = 0;
	real_point3d follow_position;

	switch (encounter->follow_target_type)
	{
	case _follow_target_players:
		{
			struct data_iterator iterator;
			struct player_datum *player;

			data_iterator_new(&iterator, player_data);
			while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
			{
				if (player->unit_index != NONE && target_count < MAXIMUM_FOLLOW_TARGET_UNITS)
					target_unit_indices[target_count++] = player->unit_index;
			}
		}
		break;

	case _follow_target_unit:
		if (object_try_and_get_and_verify_type(encounter->follow_target_unit_index, _object_mask_unit))
		{
			target_unit_indices[0] = encounter->follow_target_unit_index;
			target_count = 1;
		}
		else
		{
			encounter->follow_target_unit_index = NONE;
		}
		break;

	case _follow_target_ai:
		if (encounter->follow_target_ai_index != NONE)
		{
			struct ai_script_actor_reference_iterator iterator;
			struct actor_datum *actor;

			ai_index_actor_iterator_new(encounter->follow_target_ai_index, &iterator);
			while ((actor = ai_index_actor_iterator_next(&iterator)) != NULL &&
				target_count < MAXIMUM_FOLLOW_TARGET_UNITS)
			{
				target_unit_indices[target_count++] = actor->meta.unit_index;
			}
		}
		break;
	}

	if (target_count > 0)
	{
		unsigned long present_squads[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_SQUADS_PER_ENCOUNTER)];
		unsigned long migrating_squads[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_SQUADS_PER_ENCOUNTER)];
		unsigned long living_squads[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_SQUADS_PER_ENCOUNTER)];
		unsigned long squad_firing_position_groups[MAXIMUM_SQUADS_PER_ENCOUNTER];
		unsigned long firing_position_groups = 0;
		short living_count = 0;
		short squad_index;

		csmemset(present_squads, 0, sizeof(present_squads));
		csmemset(migrating_squads, 0, sizeof(migrating_squads));
		csmemset(living_squads, 0, sizeof(living_squads));

		for (squad_index = 0; squad_index < encounter->squad_count; squad_index++)
		{
			struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->squads,
				squad_index,
				struct squad_definition);

			if (TEST_FLAG(squad_definition->flags, _squad_automatic_migration_bit))
			{
				struct squad_datum *squad = encounter_get_squad(encounter, squad_index);

				BIT_VECTOR_SET_FLAG(present_squads, squad_index, TRUE);
				living_count += squad->current_count;
				if (squad->automatic_migration_target)
				{
					short platoon_index;
					unsigned long groups = 0;
					boolean defending = FALSE;
					short group_index;

					BIT_VECTOR_SET_FLAG(migrating_squads, squad_index, TRUE);
					platoon_index = squad_definition->platoon_index;
					if (VALID_INDEX(platoon_index, encounter_definition->platoons.count))
					{
						defending = encounter_get_platoon(encounter, platoon_index)->defending;
					}
					else if (platoon_index != NONE)
					{
						error(
							_error_silent,
							"WARNING: squad %s/%s has an invalid platoon - %d is outside range of [0, %d)",
							encounter_definition->name,
							squad_definition->name,
							platoon_index,
							encounter_definition->platoons.count);
					}

					if (defending)
					{
						for (group_index = _firing_position_group_defending;
							group_index <= _firing_position_group_defending_guard;
							group_index++)
						{
							groups |= squad_definition->firing_position_groups[group_index];
						}
					}
					else
					{
						for (group_index = _firing_position_group_attacking;
							group_index <= _firing_position_group_attacking_guard;
							group_index++)
						{
							groups |= squad_definition->firing_position_groups[group_index];
						}
					}

					firing_position_groups |= groups;
					squad_firing_position_groups[squad_index] = groups;
					if (squad->current_count > 0)
						BIT_VECTOR_SET_FLAG(living_squads, squad_index, TRUE);
				}
			}
		}

		if (living_count > 0 && firing_position_groups > 0)
		{
			if (target_count == 1)
			{
				follow_unit_index = target_unit_indices[0];
				object_get_origin(follow_unit_index, &follow_position);
			}
			else
			{
				real target_distances_squared[MAXIMUM_FOLLOW_TARGET_UNITS];
				real_point3d target_positions[MAXIMUM_FOLLOW_TARGET_UNITS];
				struct encounter_actor_iterator iterator;
				struct actor_datum *actor;
				short target_index;

				for (target_index = 0; target_index < target_count; target_index++)
				{
					target_distances_squared[target_index] = REAL_MAX;
					object_get_origin(target_unit_indices[target_index], &target_positions[target_index]);
				}

				encounter_actor_iterator_new(&iterator, encounter_index);
				while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
				{
					if (BIT_VECTOR_TEST_FLAG(present_squads, actor->meta.squad_index))
					{
						for (target_index = 0; target_index < target_count; target_index++)
						{
							real distance_squared = distance_squared3d(
								&actor->input.position.body_position,
								&target_positions[target_index]);

							target_distances_squared[target_index] = MIN(target_distances_squared[target_index], distance_squared);
						}
					}
				}

				for (target_index = 0; target_index < target_count; target_index++)
				{
					if (target_distances_squared[target_index] < REAL_MAX)
					{
						follow_unit_index = target_unit_indices[target_index];
						follow_position = target_positions[target_index];
					}
				}
			}

			if (follow_unit_index != NONE)
			{
				real group_distances_squared[NUMBER_OF_FIRING_POSITION_GROUP_INDICES];
				real best_distance_squared;
				real current_distance_squared;
				short best_squad_index = NONE;
				short current_squad_index;
				short candidate_squad_index;
				short firing_position_index;
				short group_index;

				for (group_index = 0; group_index < NUMBER_OF_FIRING_POSITION_GROUP_INDICES; group_index++)
					group_distances_squared[group_index] = REAL_MAX;

				for (firing_position_index = 0;
					firing_position_index < encounter_definition->firing_positions.count;
					firing_position_index++)
				{
					struct firing_position_definition *firing_position = TAG_BLOCK_GET_ELEMENT(
						&encounter_definition->firing_positions,
						firing_position_index,
						struct firing_position_definition);

					if (TEST_FLAG(firing_position_groups, firing_position->group_index))
					{
						real distance_squared = distance_squared3d(&firing_position->position, &follow_position);

						group_distances_squared[firing_position->group_index] = MIN(
							group_distances_squared[firing_position->group_index],
							distance_squared);
					}
				}

				best_distance_squared = REAL_MAX;
				current_distance_squared = REAL_MIN;
				current_squad_index = NONE;
				for (candidate_squad_index = 0; candidate_squad_index < encounter->squad_count; candidate_squad_index++)
				{
					if (BIT_VECTOR_TEST_FLAG(migrating_squads, candidate_squad_index))
					{
						real squad_distance_squared = REAL_MAX;

						for (group_index = 0; group_index < NUMBER_OF_FIRING_POSITION_GROUP_INDICES; group_index++)
						{
							if (TEST_FLAG(squad_firing_position_groups[candidate_squad_index], group_index))
								squad_distance_squared = MIN(squad_distance_squared, group_distances_squared[group_index]);
						}

						if (squad_distance_squared < best_distance_squared)
						{
							best_distance_squared = squad_distance_squared;
							best_squad_index = candidate_squad_index;
						}
						if (BIT_VECTOR_TEST_FLAG(living_squads, candidate_squad_index) &&
							squad_distance_squared > current_distance_squared)
						{
							current_distance_squared = squad_distance_squared;
							current_squad_index = candidate_squad_index;
						}
					}
				}

				if (best_squad_index != NONE)
				{
					boolean migrate = TRUE;

					if (current_squad_index != NONE)
					{
						real best_distance = square_root(best_distance_squared);
						real current_distance = square_root(current_distance_squared);
						real tolerance = encounter->follow_target_distance > 0.0f ? encounter->follow_target_distance : 2.0f;

						migrate = best_distance < current_distance - tolerance;
						if (ai_debug.print_automatic_migration)
						{
							struct squad_definition *current_squad_definition = TAG_BLOCK_GET_ELEMENT(
								&encounter_definition->squads,
								current_squad_index,
								struct squad_definition);
							struct squad_definition *best_squad_definition = TAG_BLOCK_GET_ELEMENT(
								&encounter_definition->squads,
								best_squad_index,
								struct squad_definition);

							console_printf(
								FALSE,
								"%s: current %s/%.1f best %s/%.1f tol %.1f -> %s",
								encounter_definition->name,
								!current_squad_definition ? "<none>" : current_squad_definition->name,
								current_distance_squared == REAL_MIN ? -1000.0f : current_distance,
								!best_squad_definition ? "<none>" : best_squad_definition->name,
								best_distance_squared == REAL_MAX ? 1000.0f : best_distance,
								tolerance,
								migrate ? "migrate" : "stay");
						}
					}

					if (migrate)
					{
						struct encounter_actor_iterator iterator;
						struct actor_datum *actor;

						encounter_actor_iterator_new(&iterator, encounter_index);
						while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
						{
							if (BIT_VECTOR_TEST_FLAG(present_squads, actor->meta.squad_index) &&
								actor->meta.squad_index != best_squad_index)
							{
								actor_change_encounter(iterator.index, encounter_index, best_squad_index);
							}
						}
					}
				}
			}
		}
	}

	return;
}

static void encounter_control_actors(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	encounter_actor_iterator_new(&iterator, encounter_index);
	while ((actor = encounter_actor_iterator_next(&iterator)) != NULL)
	{
		boolean defending = FALSE;
		boolean maneuvering = FALSE;

		actor->external_orders.stand_down = encounter->stand_down;
		actor->external_orders.playfighting = encounter->playfighting;
		if (!encounter->post_combat)
		{
			actor->external_orders.postcombat_type = _actor_postcombat_none;
			actor->external_orders.postcombat_prop_index = NONE;
		}

		if (actor->meta.platoon_index != NONE)
		{
			struct platoon_datum *platoon = encounter_get_platoon(encounter, actor->meta.platoon_index);

			defending = platoon->defending;
			maneuvering = platoon->maneuvering && !platoon->maneuver_disable;
		}
		actor->external_orders.defending = defending;

		if (maneuvering)
		{
			struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->squads, actor->meta.squad_index, struct squad_definition);
			struct platoon_definition *platoon_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->platoons, actor->meta.platoon_index, struct platoon_definition);
			short squad_index = squad_definition->maneuver_squad_index;

			if (squad_index >= 0 && squad_index < encounter_definition->squads.count)
			{
				actor_change_encounter(iterator.index, encounter_index, squad_index);
				actor_stimulus_maneuvering(
					iterator.index,
					TEST_FLAG(platoon_definition->flags, _platoon_advancing_maneuver_bit),
					TEST_FLAG(platoon_definition->flags, _platoon_flee_upon_maneuver_bit));
			}
		}
	}

	encounters_update_dirty_status();

	return;
}

static void encounter_update_squads(
	long encounter_index)
{
	struct encounter_datum *encounter = encounter_get(encounter_index);
	struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);
	short squad_index;

	for (squad_index = 0; squad_index < encounter->squad_count; ++squad_index)
	{
		struct squad_datum *squad = encounter_get_squad(encounter, squad_index);
		struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
			&encounter_definition->squads, squad_index, struct squad_definition);

		if (squad->delay_timer > 0 && !TEST_FLAG(squad_definition->flags, _squad_delay_forever_bit))
		{
			if (squad->delay_timer_started)
			{
				if (squad->delay_timer > ENCOUNTER_UPDATE_INTERVAL)
					squad->delay_timer -= ENCOUNTER_UPDATE_INTERVAL;
				else
					encounter_squad_timer_expire(encounter_index, squad_index);
			}
			else
			{
				squad->delay_timer_started = TEST_FLAG(squad_definition->flags, _squad_timer_starts_immediately_bit) ||
					encounter->current_in_combat_count > 0;
				if (squad->delay_timer_started && ai_debug.print_rules)
				{
					console_printf(FALSE, "%s/%s: delay timer started (%.1f sec)",
						encounter_definition->name, squad_definition->name, squad_definition->squad_delay_timer);
				}
			}
		}
	}

	return;
}

static void encounterless_deactivate(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 1838, actor->meta.encounterless);
	actor->meta.encounterless_active_timer = 0;
	actor_set_active(actor_index, FALSE);

	return;
}

static void encounters_test_activation(
	void)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	unsigned long const *combined_pvs = players_get_combined_pvs();
	unsigned long activation_cluster_bit_vector[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_CLUSTERS_PER_STRUCTURE)];
	struct data_iterator iterator;
	struct encounter_datum *encounter;
	struct actor_datum *actor;
	long actor_index;
	boolean active;

	for (actor_index = ai_globals->first_encounterless_actor_index; actor_index != NONE; actor_index = actor->meta.next_actor_index)
	{
		actor = actor_get(actor_index);
		match_assert("c:\\halo\\SOURCE\\ai\\encounters.c", 2234, actor->meta.encounterless);

		if (actor->meta.swarm)
		{
			actor->meta.dormant_desire = TRUE;

			if (actor->meta.swarm_cache_index == NONE)
			{
				long unit_index = actor->meta.swarm_unit_index;

				while (unit_index != NONE)
				{
					struct unit_datum *unit = unit_get(unit_index);
					long ultimate_parent_index = object_get_ultimate_parent(unit_index);
					struct object_datum *parent_object = object_get(ultimate_parent_index);
					short cluster_index = parent_object->object.location.cluster_index;

					if (cluster_index != NONE && BIT_VECTOR_TEST_FLAG(combined_pvs, cluster_index))
					{
						actor->meta.dormant_desire = FALSE;
						break;
					}

					unit_index = unit->unit.swarm_next_unit_index;
				}
			}
			else
			{
				struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);
				short i;

				for (i = 0; i < swarm->unit_count; ++i)
				{
					long ultimate_parent_index = object_get_ultimate_parent(swarm->unit_indices[i]);
					struct object_datum *parent_object = object_get(ultimate_parent_index);
					short cluster_index = parent_object->object.location.cluster_index;

					if (cluster_index != NONE && BIT_VECTOR_TEST_FLAG(combined_pvs, cluster_index))
					{
						actor->meta.dormant_desire = FALSE;
						break;
					}
				}
			}
		}
		else
		{
			long ultimate_parent_index = object_get_ultimate_parent(actor->meta.unit_index);
			struct object_datum *parent_object = object_get(ultimate_parent_index);
			short cluster_index = parent_object->object.location.cluster_index;

			if (cluster_index == NONE)
				actor->meta.dormant_desire = TRUE;
			else
				actor->meta.dormant_desire = !BIT_VECTOR_TEST_FLAG(combined_pvs, cluster_index);
		}

		active = actor->meta.force_active;
		active |= game_in_editor();
		active |= !actor->meta.dormant_desire;
		active |= ai_debug.force_all_active;
		if (active)
		{
			encounterless_activate(actor_index);
		}
		else if (actor->meta.encounterless_active_timer > TICKS_PER_SECOND)
		{
			actor->meta.encounterless_active_timer -= TICKS_PER_SECOND;
		}
		else
		{
			actor->meta.encounterless_active_timer = 0;
			encounterless_deactivate(actor_index);
		}

		actor_verify_activation(actor_index);
	}

	data_iterator_new(&iterator, encounter_data);
	while ((encounter = (struct encounter_datum *)data_iterator_next(&iterator)) != NULL)
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters, DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.datum_index), struct encounter_definition);
		active = encounter->force_active;
		active |= game_in_editor();
		active |= encounter->respawn_delay_ticks > 0;
		active |= ai_debug.force_all_active;

		if (encounter_definition->runtime_structure_bsp_reference_index == NONE ||
			encounter_definition->runtime_structure_bsp_reference_index == global_structure_bsp_index)
		{
			encounter_compute_activation_cluster_bit_vector(iterator.datum_index, TRUE, MAXIMUM_CLUSTERS_PER_STRUCTURE, combined_pvs, activation_cluster_bit_vector);
			active |= bit_vector_and(structure_bsp->clusters.count, combined_pvs, activation_cluster_bit_vector, NULL);
		}
		else
		{
			active = FALSE;
		}

		if (active)
		{
			encounter->remain_active_timer = ENCOUNTER_REMAIN_ACTIVE_TIME;
			encounter_activate(iterator.datum_index);
		}
		else if (encounter->active && encounter->remain_active_timer > TICKS_PER_SECOND)
		{
			encounter->remain_active_timer -= TICKS_PER_SECOND;
		}
		else
		{
			boolean link_active = FALSE;
			short i;

			for (i = 0; i < encounter->link_encounter_count; ++i)
			{
				struct encounter_datum *link_encounter = encounter_get(encounter->link_encounter_indices[i]);

				if (link_encounter->remain_active_timer > 0)
					link_active = TRUE;
			}

			encounter->remain_active_timer = 0;
			if (link_active)
				encounter_activate(iterator.datum_index);
			else
				encounter_deactivate(iterator.datum_index);
		}
	}

	return;
}
