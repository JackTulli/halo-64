/*
ACTION_OBEY.C

symbols in this file:
00004F70 0040:
	_code_00004f70 (0000)
00004FB0 0230:
	_code_00004fb0 (0000)
000051E0 0070:
	_code_000051e0 (0000)
00005250 0040:
	_code_00005250 (0000)
00005290 0050:
	_code_00005290 (0000)
000052E0 0020:
	_code_000052e0 (0000)
00005300 0050:
	_code_00005300 (0000)
00005350 0130:
	_code_00005350 (0000)
00005480 0180:
	_action_obey_command_list_setup (0000)
00005600 0070:
	_action_obey_flush_command_indices (0000)
00005670 0030:
	_action_obey_flush_structure_indices (0000)
000056A0 0030:
	_action_obey_advance_command_list (0000)
000056D0 0030:
	_action_obey_begin (0000)
00005700 0030:
	_action_obey_end (0000)
00005730 0790:
	_action_obey_describe_command (0000)
00005EC0 0020:
	_vector_from_points2d (0000)
00005EE0 0040:
	_cross_product3d (0000)
00005F20 0030:
	_negate_vector3d (0000)
00005F50 0020:
	_random_range (0000)
00005F70 0150:
	_code_00005f70 (0000)
000060C0 10e0:
	_code_000060c0 (0000)
000071A0 0580:
	_code_000071a0 (0000)
00007720 0120:
	_code_00007720 (0000)
00007840 0050:
	_code_00007840 (0000)
00007890 0030:
	_action_obey_update (0000)
000078C0 00c0:
	_action_obey_perform (0000)
00007980 03e0:
	_action_obey_control (0000)
00243270 003c:
	??_C@_0DM@JNBLNJLG@?$CFs?3?5command?5list?5?$CFs?5is?5stuck?5loo@ (0000)
002432AC 0036:
	??_C@_0DG@FLEHDLBK@?$CFs?3?5command?5list?5?$CFs?5entry?5?$CD?$CFd?5tr@ (0000)
002432E4 0014:
	??_C@_0BE@EJBCJKCJ@targeting_reference?$AA@ (0000)
002432F8 0020:
	??_C@_0CA@HHCNJNKK@c?3?2halo?2SOURCE?2ai?2action_obey?4c?$AA@ (0000)
00243318 003e:
	??_C@_0DO@JCOPLIPB@?$CBactor?9?$DOmeta?4swarm?5?$HM?$HM?5?$CIactor?9?$DOme@ (0000)
00243358 0034:
	??_C@_0DE@LLDIJCBD@wrong?5structure?5bsp?0?5cannot?5exec@ (0000)
00243390 0044:
	??_C@_0EE@KEBHPAE@swarm?5actor?5?$CFs?5cannot?5execute?5co@ (0000)
002433D4 000d:
	??_C@_0N@CEGJDJND@?$DMunknown?5?$CFd?$DO?$AA@ (0000)
002433E4 0004:
	??_C@_03DFHDCPPK@die?$AA@ (0000)
002433E8 0014:
	??_C@_0BE@OLHBIEN@teleport?5to?5?$CIp?$CFd?$CJ?$CFs?$AA@ (0000)
002433FC 0013:
	??_C@_0BD@OIBANJLL@?5and?5face?5at?5?$CIp?$CFd?$CJ?$AA@ (0000)
0024340F 0001:
	??_C@_00CNPNBAHC@?$AA@ (0000)
00243410 0010:
	??_C@_0BA@MGBBCPDG@set?5radius?5?$CF?42f?$AA@ (0000)
00243420 000f:
	??_C@_0P@KENFEEEA@loop?5to?5?$CD?$CFd?5?$CFs?$AA@ (0000)
00243430 0012:
	??_C@_0BC@OEJHODGH@loop?5to?5?$DMnone?$DO?5?$CFs?$AA@ (0000)
00243444 001b:
	??_C@_0BL@DGGHLGE@only?5until?5told?5to?5advance?$AA@ (0000)
00243460 0007:
	??_C@_06KHDOHNKC@always?$AA@ (0000)
00243468 0008:
	??_C@_07KIIGOGND@wait?5?$CFs?$AA@ (0000)
00243470 0016:
	??_C@_0BG@LEMJPAIB@until?5told?5to?5advance?$AA@ (0000)
00243488 0014:
	??_C@_0BE@DKOCLLMJ@until?5visible?5enemy?$AA@ (0000)
0024349C 000e:
	??_C@_0O@NBMOMCME@until?5alerted?$AA@ (0000)
002434AC 000e:
	??_C@_0O@EFALGMLH@initiative?5?$CFs?$AA@ (0000)
002434BC 000a:
	??_C@_09DEIMKGFJ@action?5?$CFs?$AA@ (0000)
002434C8 000f:
	??_C@_0P@OKJMMNMD@vehicle?9scared?$AA@ (0000)
002434D8 000f:
	??_C@_0P@HOGHMCM@vehicle?9woohoo?$AA@ (0000)
002434E8 000b:
	??_C@_0L@JFPBJIOJ@dive?9right?$AA@ (0000)
002434F4 000a:
	??_C@_09GLAJHPNM@dive?9left?$AA@ (0000)
00243500 000a:
	??_C@_09JAMKJKJP@dive?9back?$AA@ (0000)
0024350C 0009:
	??_C@_08HHGPGIIN@dive?9fwd?$AA@ (0000)
00243518 000c:
	??_C@_0M@FCCJKDHL@evade?9right?$AA@ (0000)
00243524 000b:
	??_C@_0L@GEJKFJKH@evade?9left?$AA@ (0000)
00243530 000e:
	??_C@_0O@MHCNGAHE@surprise?9back?$AA@ (0000)
00243540 000f:
	??_C@_0P@OMDAPGKA@surprise?9front?$AA@ (0000)
00243550 0008:
	??_C@_07PEIBNLKE@berserk?$AA@ (0000)
00243558 000d:
	??_C@_0N@PMOONPD@targeting?5?$CFs?$AA@ (0000)
00243568 000c:
	??_C@_0M@FBODOLG@vocalize?5?$CFs?$AA@ (0000)
00243574 0012:
	??_C@_0BC@JABMHOI@play?5recording?5?$CFs?$AA@ (0000)
00243588 000b:
	??_C@_0L@HCEEAAG@animate?5?$CFs?$AA@ (0000)
00243594 000d:
	??_C@_0N@EHHELJPO@script?5?$CFs?5?$CFs?$AA@ (0000)
002435A4 0005:
	??_C@_04OKGDLNCL@NONE?$AA@ (0000)
002435AC 0012:
	??_C@_0BC@EGIAGBFJ@wake?9and?9continue?$AA@ (0000)
002435C0 0010:
	??_C@_0BA@GMMPCDLM@wait?9for?9finish?$AA@ (0000)
002435D0 001d:
	??_C@_0BN@CPMBOFMF@targeted?5jump?5?$CI?$CF?42fh?0?5?$CF?42fv?$CJ?$AA@ (0000)
002435F0 000d:
	??_C@_0N@MHAJODEH@running?5jump?$AA@ (0000)
00243600 0023:
	??_C@_0CD@CFLDIMMG@enter?5vehicle?5as?5?$CFs?5if?5within?5?$CF?4@ (0000)
00243624 0009:
	??_C@_08OFIBMPOE@any?9seat?$AA@ (0000)
00243630 0007:
	??_C@_06NCODBMHO@driver?$AA@ (0000)
00243638 000a:
	??_C@_09NFKEFBPD@passenger?$AA@ (0000)
00243644 0007:
	??_C@_06NJIDJFDL@gunner?$AA@ (0000)
0024364C 000f:
	??_C@_0P@ELLMOHEN@any?9non?9driver?$AA@ (0000)
0024365C 0017:
	??_C@_0BH@JKPBBGH@throw?5grenade?5at?5?$CIp?$CFd?$CJ?$AA@ (0000)
00243674 0018:
	??_C@_0BI@CFJADMHA@shoot?5at?5?$CIp?$CFd?$CJ?5for?5?$CF?41f?$AA@ (0000)
0024368C 000a:
	??_C@_09PCOPMOEK@crouch?5?$CFs?$AA@ (0000)
00243698 0007:
	??_C@_06OFNJOBDK@enable?$AA@ (0000)
002436A0 0008:
	??_C@_07DBNODEFN@disable?$AA@ (0000)
002436A8 0012:
	??_C@_0BC@MGCAJCHA@animation?5mode?5?$CFs?$AA@ (0000)
002436BC 0006:
	??_C@_05DDGCEFJL@panic?$AA@ (0000)
002436C4 0007:
	??_C@_06NDKEFKOH@combat?$AA@ (0000)
002436CC 0007:
	??_C@_06PDPNPFPP@asleep?$AA@ (0000)
002436D4 000a:
	??_C@_09CENHCFNA@noncombat?$AA@ (0000)
002436E0 0017:
	??_C@_0BH@IPOBIPFB@look?5?$CFs?5at?5?$CFs?5for?5?$CF?41f?$AA@ (0000)
002436F8 0008:
	??_C@_07CBDMFKJF@?$DMerror?$DO?$AA@ (0000)
00243700 001b:
	??_C@_0BL@FFKOOMBE@look?5?$CFs?5at?5player?5for?5?$CF?41f?$AA@ (0000)
0024371C 0031:
	??_C@_0DB@IOOLPGJB@look?5?$CFs?5at?5random?5one?5of?5?$CIp?$CFd?9p?$CF@ (0000)
00243750 001a:
	??_C@_0BK@IEJFKCKH@look?5?$CFs?5at?5?$CIp?$CFd?$CJ?5for?5?$CF?41f?$AA@ (0000)
0024376C 0015:
	??_C@_0BF@IFMPJNE@move?5?$CFs?5for?5?$CF?41f?5sec?$AA@ (0000)
00243784 0021:
	??_C@_0CB@IFAAGCMK@move?5?$CFs?5towards?5?$CIp?$CFd?$CJ?0?5dist?5?$CF?42f@ (0000)
002437A8 0024:
	??_C@_0CE@PMEAGMOK@move?5?$CFs?5along?5angle?5?$CF?41f?0?5dist?5?$CF@ (0000)
002437CC 000b:
	??_C@_0L@CPPIDKFP@any?9facing?$AA@ (0000)
002437D8 000a:
	??_C@_09GNCFCLDO@backwards?$AA@ (0000)
002437E4 0006:
	??_C@_05DHJDAOHK@right?$AA@ (0000)
002437EC 0005:
	??_C@_04GOOOJOPP@left?$AA@ (0000)
002437F4 0009:
	??_C@_08HMDMCCDA@forwards?$AA@ (0000)
00243800 001b:
	??_C@_0BL@OJAEMMNB@go?5to?5?$CIp?$CFd?$CJ?5and?5face?5?$CIp?$CFd?$CJ?$AA@ (0000)
0024381C 000f:
	??_C@_0P@POPHLLEH@go?5to?5?$CIp?$CFd?$CJ?5?$CFs?$AA@ (0000)
0024382C 000c:
	??_C@_0M@PMIIAAGB@keep_moving?$AA@ (0000)
00243838 000e:
	??_C@_0O@MHBHOGCE@stop_at_point?$AA@ (0000)
00243848 000b:
	??_C@_0L@ENHEDOMM@pause?5?$CF?41f?$AA@ (0000)
00243854 0012:
	??_C@_0BC@PPCIDFNG@forced?5aim?5weapon?$AA@ (0000)
00243868 0014:
	??_C@_0BE@MMENGNPN@forced?5exact?5facing?$AA@ (0000)
0024387C 0014:
	??_C@_0BE@HLOJAFEL@idle?5look?5with?5head?$AA@ (0000)
00243890 0011:
	??_C@_0BB@CNNKGAPI@idle?5turn?5around?$AA@ (0000)
002438A4 0010:
	??_C@_0BA@HMGFJNDM@idle?5aim?5weapon?$AA@ (0000)
002438B4 0011:
	??_C@_0BB@FOMPEOPI@?$CFs?3?5?$CFs?5?$CD?$CFd?$CFs?3?5?$CFs?$AA@ (0000)
002438C8 0008:
	??_C@_07OGBBMDNN@?5FAILED?$AA@ (0000)
002438D0 0006:
	??_C@_05GFOLEBJA@?$CFs?1?$CFs?$AA@ (0000)
002438D8 000f:
	??_C@_0P@MPLGIMAD@?$DMno?5encounter?$DO?$AA@ (0000)
002438E8 0004:
	__real@3d888889 (0000)
002438EC 0004:
	__real@3c8efa35 (0000)
002438F0 0004:
	__real@43b40000 (0000)
002438F4 0004:
	__real@3f7be76d (0000)
002438F8 0036:
	??_C@_0DG@HCPHDADC@current_command?9?$DOatom_type?5?$DN?$DN?5_a@ (0000)
00243930 0013:
	??_C@_0BD@NEGHAILN@finished_reference?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"

#include "actor_looking.h"
#include "actors.h"
#include "ai_communication.h"
#include "ai_debug.h"
#include "ai_scenario_definitions.h"
#include "cseries/errors.h"
#include "math/real_math.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/bipeds.h"
#include "units/dialogue_definitions.h"
#include "cutscene/recorded_animations.h"
#include "cutscene/recorded_animation_definitions.h"
#include "game/game.h"
#include "game/players.h"
#include "actor_definitions.h"
#include "props.h"
#include "hs/hs.h"
#include "objects/objects.h"
#include "units/unit_definitions.h"
#include "units/biped_definitions.h"
#include "memory/data.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	OBEY_MAXIMUM_LOOP_COUNT = 10,
	_action_obey_actor_mode_combat = 3,
	_action_obey_idle_look_none = 0,
	_action_obey_idle_look_noncombat = 1,
	_action_obey_idle_look_combat = 4,
	_action_obey_primary_priority_exact_facing = 4,
	_action_obey_primary_priority_locked_aiming = 7,
	_action_obey_combat_status_clear_los = 5,
};

/* ---------- macros */

/* ---------- structures */

typedef void (*action_obey_individual_iterator_proc)(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);

struct vehicle_possibility
{
	real distance_squared;
	long vehicle_index;
};

/* actor fire-target types and combat-status levels (actors.h does not yet
   declare these; actors.c and actor_combat.c carry the none/prop fire-target
   values, and actors.c, actor_perception.c and ai_script.c carry the
   combat-status levels TU-locally; HCEX PDB enumerator names) */
enum
{
	_actor_fire_target_none = 0,
	_actor_fire_target_prop = 1,
	_actor_fire_target_manual_point = 2,
	number_of_actor_fire_targets = 3,
};

enum
{
	_actor_combat_status_none = 0,
	_actor_combat_status_wary = 1,
	_actor_combat_status_investigate = 2,
	_actor_combat_status_definite = 3,
	_actor_combat_status_certain = 4,
	_actor_combat_status_clear_los = 5,
	_actor_combat_status_dangerous = 6,
	_actor_combat_status_visible = 7,
	NUMBER_OF_ACTOR_COMBAT_STATUS_LEVELS = 8,
};

/* unit animation impulses (units.h does not declare these; units.c declares
   only NUMBER_OF_UNIT_ANIMATION_IMPULSES locally; HCEX PDB enumerator names;
   values match the impulses January's command_begin stores) */
enum
{
	_unit_animation_impulse_berserk = 0,
	_unit_animation_impulse_signal_move = 1,
	_unit_animation_impulse_signal_attack = 2,
	_unit_animation_impulse_signal_warn = 3,
	_unit_animation_impulse_surprise_front = 4,
	_unit_animation_impulse_surprise_back = 5,
	_unit_animation_impulse_evade_left = 6,
	_unit_animation_impulse_evade_right = 7,
	_unit_animation_impulse_dive_front = 8,
	_unit_animation_impulse_dive_back = 9,
	_unit_animation_impulse_dive_left = 10,
	_unit_animation_impulse_dive_right = 11,
	_unit_animation_impulse_vehicle_celebrate = 12,
	_unit_animation_impulse_vehicle_panic = 13,
	NUMBER_OF_UNIT_ANIMATION_IMPULSES = 14,
};

typedef char action_obey_simple_control_size_assert[
	sizeof(struct obey_individual_simple_control) == 0x24 ? 1 : -1];
typedef char action_obey_complex_control_size_assert[
	sizeof(struct obey_individual_complex_control) == 0x58 ? 1 : -1];
typedef char action_obey_state_size_assert[
	sizeof(struct obey_state_data) == 0x84 ? 1 : -1];
typedef char action_obey_state_offset_assert[
	offsetof(struct actor_datum, state.action_data.obey) == 0x9C ? 1 : -1];

/* ---------- prototypes */

static int vehicle_possibility_qsort(
	void const *a,
	void const *b);
static void action_obey_command_end(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	byte *next_command_index);
static void action_obey_individual_setup(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individual_flush_command_indices(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individual_begin(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individual_advance(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individual_end(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individuals_iterate(
	long actor_index,
	boolean initialize_structures,
	struct obey_state_data *state_data,
	action_obey_individual_iterator_proc iterator,
	void *user_data);
static void action_obey_directmovement_update_facing(
	long actor_index,
	long unit_index,
	struct obey_individual_simple_control *simple_control);
static boolean action_obey_command_begin(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control);
static boolean action_obey_command_perform(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control);
static void action_obey_individual_perform(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);
static void action_obey_individual_update(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data);

/* ---------- globals */

/* ---------- public code */

boolean action_obey_command_list_setup(
	long actor_index,
	short command_list_index,
	struct obey_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct scenario *scenario = global_scenario_get();
	struct ai_command_list_definition *command_list = NULL;
	boolean success = FALSE;

	match_assert("c:\\halo\\SOURCE\\ai\\action_obey.c", 1597, state_data);
	csmemset(state_data, 0, sizeof(*state_data));

	if (command_list_index >= 0 &&
		command_list_index < scenario->ai_command_lists.count)
	{
		command_list = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_command_lists,
			command_list_index,
			struct ai_command_list_definition);

		if (actor->meta.swarm && actor->meta.swarm_cache_index == NONE)
		{
			if (actor->meta.active)
			{
				char buffer[256];

				ai_debug_describe_actor(actor_index, NONE, TRUE, buffer, sizeof(buffer));
				error(
					_error_silent,
					"swarm actor %s cannot execute command list, ran out of swarm caches",
					buffer);
			}
			else
			{
				actor->state.command_list_index = command_list_index;
			}
		}
		else if (command_list->runtime_structure_bsp_reference_index != NONE &&
			command_list->runtime_structure_bsp_reference_index != global_structure_bsp_index)
		{
			error(
				_error_silent,
				"wrong structure bsp, cannot execute command list %s",
				command_list->name);
		}
		else
		{
			state_data->command_list_index = command_list_index;
			success = TRUE;
		}
	}

	if (success)
	{
		boolean initiative = TEST_FLAG(command_list->flags, _ai_command_list_allow_initiative_bit);
		boolean targeting = TEST_FLAG(command_list->flags, _ai_command_list_allow_targeting_bit);
		boolean allow_looking = !TEST_FLAG(command_list->flags, _ai_command_list_disable_looking_bit);
		boolean allow_communication = !TEST_FLAG(command_list->flags, _ai_command_list_disable_communication_bit);

		if (!allow_looking)
		{
			actor_look_secondary_stop(actor_index);
		}
		state_data->initiative = initiative;
		state_data->allow_looking = allow_looking;
		state_data->allow_communication = allow_communication;
		action_obey_individuals_iterate(
			actor_index,
			TRUE,
			state_data,
			action_obey_individual_setup,
			&targeting);
	}

	return success;
}

void action_obey_flush_command_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;
	struct scenario *scenario = global_scenario_get();

	if (state_data->command_list_index < 0 ||
		state_data->command_list_index >= scenario->ai_command_lists.count)
	{
		state_data->command_list_index = NONE;
		state_data->finished = TRUE;
		actor_action_change(actor_index, _actor_action_none, NULL);
	}
	else
	{
		action_obey_individuals_iterate(
			actor_index,
			FALSE,
			state_data,
			action_obey_individual_flush_command_indices,
			NULL);
	}

	return;
}

void action_obey_flush_structure_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	if (state_data->complex_control.destination_valid)
	{
		state_data->complex_control.destination_surface_index = NONE;
	}

	return;
}

void action_obey_advance_command_list(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	action_obey_individuals_iterate(
		actor_index,
		FALSE,
		state_data,
		action_obey_individual_advance,
		NULL);

	return;
}

void action_obey_begin(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	action_obey_individuals_iterate(
		actor_index,
		FALSE,
		state_data,
		action_obey_individual_begin,
		NULL);

	return;
}

void action_obey_end(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	action_obey_individuals_iterate(
		actor_index,
		FALSE,
		state_data,
		action_obey_individual_end,
		NULL);

	return;
}

void action_obey_describe_command(
	struct scenario *scenario,
	struct ai_command_definition *command,
	char *string,
	long string_size)
{
	char *look_modifier_names[] =
	{
		"idle aim weapon",
		"idle turn around",
		"idle look with head",
		"forced exact facing",
		"forced aim weapon"
	};

	switch (command->atom_type)
	{
	case _ai_atom_pause:
		_snprintf(string, string_size, "pause %.1f", command->parameter1);
		break;

	case _ai_atom_go_to:
		{
			char *go_to_modifier_names[] =
			{
				"stop_at_point",
				"keep_moving"
			};

			_snprintf(
				string,
				string_size,
				"go to (p%d) %s",
				command->point1_index,
				go_to_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_go_to_and_face:
		_snprintf(
			string,
			string_size,
			"go to (p%d) and face (p%d)",
			command->point1_index,
			command->point2_index);
		break;

	case _ai_atom_move_direction:
		{
			char *move_facing_names[] =
			{
				"forwards",
				"left",
				"right",
				"backwards",
				"any-facing"
			};

			if (command->point1_index == NONE)
			{
				_snprintf(
					string,
					string_size,
					"move %s along angle %.1f, dist %.2f",
					move_facing_names[command->atom_modifier],
					command->parameter2,
					command->parameter1);
			}
			else
			{
				_snprintf(
					string,
					string_size,
					"move %s towards (p%d), dist %.2f",
					move_facing_names[command->atom_modifier],
					command->point1_index,
					command->parameter1);
			}
		}
		break;

	case _ai_atom_move_immediate:
		{
			char *move_facing_names[] =
			{
				"forwards",
				"left",
				"right",
				"backwards"
			};

			_snprintf(
				string,
				string_size,
				"move %s for %.1f sec",
				move_facing_names[command->atom_modifier],
				command->parameter1);
		}
		break;

	case _ai_atom_look:
		_snprintf(
			string,
			string_size,
			"look %s at (p%d) for %.1f",
			look_modifier_names[command->atom_modifier],
			command->point1_index,
			command->parameter1);
		break;

	case _ai_atom_look_random:
		_snprintf(
			string,
			string_size,
			"look %s at random one of (p%d-p%d) for %.1f-%.1f",
			look_modifier_names[command->atom_modifier],
			command->point1_index,
			command->point2_index,
			command->parameter1,
			command->parameter2);
		break;

	case _ai_atom_look_player:
		_snprintf(
			string,
			string_size,
			"look %s at player for %.1f",
			look_modifier_names[command->atom_modifier],
			command->parameter1);
		break;

	case _ai_atom_look_object:
		{
			char *object_name = "<error>";

			if (command->object_name_index >= 0 &&
				command->object_name_index < scenario->object_names.count)
			{
				object_name = TAG_BLOCK_GET_ELEMENT(
					&scenario->object_names,
					command->object_name_index,
					struct scenario_object_name)->name;
			}

			_snprintf(
				string,
				string_size,
				"look %s at %s for %.1f",
				look_modifier_names[command->atom_modifier],
				object_name,
				command->parameter1);
		}
		break;

	case _ai_atom_animation_mode:
		{
			char *animation_mode_names[] =
			{
				"noncombat",
				"asleep",
				"combat",
				"panic"
			};

			_snprintf(
				string,
				string_size,
				"animation mode %s",
				animation_mode_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_crouch:
		{
			char *crouch_modifier_names[] =
			{
				"disable",
				"enable"
			};

			_snprintf(
				string,
				string_size,
				"crouch %s",
				crouch_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_shoot:
		_snprintf(
			string,
			string_size,
			"shoot at (p%d) for %.1f",
			command->point1_index,
			command->parameter1);
		break;

	case _ai_atom_grenade:
		_snprintf(
			string,
			string_size,
			"throw grenade at (p%d)",
			command->point1_index);
		break;

	case _ai_atom_vehicle:
		{
			char *vehicle_modifier_names[] =
			{
				"any-non-driver",
				"gunner",
				"passenger",
				"driver",
				"any-seat"
			};

			_snprintf(
				string,
				string_size,
				"enter vehicle as %s if within %.1f",
				vehicle_modifier_names[command->atom_modifier],
				command->parameter1);
		}
		break;

	case _ai_atom_running_jump:
		_snprintf(string, string_size, "running jump");
		break;

	case _ai_atom_targeted_jump:
		_snprintf(
			string,
			string_size,
			"targeted jump (%.2fh, %.2fv)",
			command->parameter1,
			command->parameter2);
		break;

	case _ai_atom_script:
		{
			char *script_modifier_names[] =
			{
				"wait-for-finish",
				"wake-and-continue"
			};
			char *script_name = "<error>";

			if (command->script_reference_index == NONE)
			{
				script_name = "NONE";
			}
			else if (command->script_reference_index >= 0 &&
				command->script_reference_index < scenario->ai_script_references.count)
			{
				script_name = TAG_BLOCK_GET_ELEMENT(
					&scenario->ai_script_references,
					command->script_reference_index,
					struct ai_script_reference_definition)->script_name;
			}

			_snprintf(
				string,
				string_size,
				"script %s %s",
				script_name,
				script_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_animate:
		{
			char *animation_name = "<error>";

			if (command->animation_reference_index == NONE)
			{
				animation_name = "NONE";
			}
			else if (command->animation_reference_index >= 0 &&
				command->animation_reference_index < scenario->ai_animation_references.count)
			{
				animation_name = TAG_BLOCK_GET_ELEMENT(
					&scenario->ai_animation_references,
					command->animation_reference_index,
					struct ai_animation_reference_definition)->animation_name;
			}

			_snprintf(string, string_size, "animate %s", animation_name);
		}
		break;

	case _ai_atom_recording:
		{
			char *recording_name = "<error>";

			if (command->recording_reference_index == NONE)
			{
				recording_name = "NONE";
			}
			else if (command->recording_reference_index >= 0 &&
				command->recording_reference_index < scenario->ai_recording_references.count)
			{
				recording_name = TAG_BLOCK_GET_ELEMENT(
					&scenario->ai_recording_references,
					command->recording_reference_index,
					struct ai_recording_reference_definition)->recording_name;
			}

			_snprintf(string, string_size, "play recording %s", recording_name);
		}
		break;

	case _ai_atom_vocalize:
		_snprintf(
			string,
			string_size,
			"vocalize %s",
			dialogue_get_vocalization_name(command->atom_modifier, FALSE));
		break;

	case _ai_atom_targeting:
		{
			char *targeting_modifier_names[] =
			{
				"enable",
				"disable"
			};

			_snprintf(
				string,
				string_size,
				"targeting %s",
				targeting_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_action:
		{
			char *action_modifier_names[] =
			{
				"berserk",
				"surprise-front",
				"surprise-back",
				"evade-left",
				"evade-right",
				"dive-fwd",
				"dive-back",
				"dive-left",
				"dive-right",
				"vehicle-woohoo",
				"vehicle-scared"
			};

			_snprintf(
				string,
				string_size,
				"action %s",
				action_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_initiative:
		{
			char *initiative_modifier_names[] =
			{
				"enable",
				"disable"
			};

			_snprintf(
				string,
				string_size,
				"initiative %s",
				initiative_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_wait:
		{
			char *wait_modifier_names[] =
			{
				"until alerted",
				"until visible enemy",
				"until told to advance"
			};

			_snprintf(
				string,
				string_size,
				"wait %s",
				wait_modifier_names[command->atom_modifier]);
		}
		break;

	case _ai_atom_loop:
		{
			char *loop_modifier_names[] =
			{
				"always",
				"only until told to advance"
			};

			if (command->command_index == NONE)
			{
				_snprintf(
					string,
					string_size,
					"loop to <none> %s",
					loop_modifier_names[command->atom_modifier]);
			}
			else
			{
				_snprintf(
					string,
					string_size,
					"loop to #%d %s",
					command->command_index,
					loop_modifier_names[command->atom_modifier]);
			}
		}
		break;

	case _ai_atom_set_radius:
		_snprintf(string, string_size, "set radius %.2f", command->parameter1);
		break;

	case _ai_atom_teleport:
		{
			char facing_string[256];

			csstrcpy(facing_string, "");
			if (command->point2_index != NONE)
			{
				sprintf(facing_string, " and face at (p%d)", command->point2_index);
			}

			_snprintf(
				string,
				string_size,
				"teleport to (p%d)%s",
				command->point1_index,
				facing_string);
		}
		break;

	case _ai_atom_die:
		_snprintf(string, string_size, "die");
		break;

	default:
		_snprintf(string, string_size, "<unknown %d>", command->atom_type);
		break;
	}

	return;
}

void action_obey_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	action_obey_individuals_iterate(
		actor_index,
		FALSE,
		state_data,
		action_obey_individual_update,
		NULL);

	return;
}

boolean action_obey_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;
	boolean finished = TRUE;

	action_obey_individuals_iterate(
		actor_index,
		FALSE,
		state_data,
		action_obey_individual_perform,
		&finished);

	if (finished && !state_data->finished)
	{
		struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_command_lists,
			state_data->command_list_index,
			struct ai_command_list_definition);

		if (!TEST_FLAG(command_list->flags, _ai_command_list_disable_falling_damage_bit) ||
			!actor->input.in_midair ||
			TEST_FLAG(actor_definition_get(actor->meta.definition_index)->flags, _actor_definition_flying_bit))
		{
			actor->state.last_command_list_time = game_time_get();
			state_data->finished = TRUE;
		}
	}

	return actor->state.action == _actor_action_obey && state_data->finished;
}

void action_obey_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct obey_state_data *state_data = &actor->state.action_data.obey;

	match_assert("c:\\halo\\SOURCE\\ai\\action_obey.c", 1777, !actor->meta.swarm);

	if (state_data->complex_control.shoot_at_target)
	{
		actor->orders.look.primary_priority = _action_obey_primary_priority_locked_aiming;
		actor->orders.look.primary_direction.type = _direction_specification_target;
		actor->orders.look.idle_look_type = _action_obey_idle_look_combat;
		actor->orders.combat.shoot_at_target = TRUE;
		actor->orders.combat.override_firing_restrictions = TRUE;
		actor->orders.combat.use_manual_target_point = TRUE;
		actor->orders.combat.target_point = state_data->complex_control.shoot_target;
		actor->orders.combat.override_burst_duration = state_data->complex_control.shoot_burst_length;
		goto flush_order_effects;
	}

	if (state_data->complex_control.destination_facing &&
		actor_path_at_destination(actor_index))
	{
		boolean flying = actor->state.flying;

		actor->orders.look.primary_priority = _action_obey_primary_priority_exact_facing;
		actor->orders.look.primary_direction.type = _direction_specification_point;
		actor->orders.look.primary_direction.point = state_data->complex_control.destination_facing_point;

		if (!flying)
		{
			actor->orders.look.primary_direction.point.z = actor->input.position.head_position.z;
		}

		actor->orders.look.idle_look_type = actor->state.mode >= _action_obey_actor_mode_combat ?
			_action_obey_idle_look_combat : _action_obey_idle_look_noncombat;
		goto flush_order_effects;
	}

	if (state_data->complex_control.override_movement_type == _actor_movement_type_panic ||
		state_data->complex_control.override_movement_type == _actor_movement_type_asleep)
	{
		actor->orders.look.primary_priority = _action_obey_primary_priority_locked_aiming;
		actor->orders.look.primary_direction.type = _direction_specification_movement;
		actor->orders.look.idle_look_type = _action_obey_idle_look_none;
		goto flush_order_effects;
	}

	if (actor->state.combat_status >= _action_obey_combat_status_clear_los &&
		TEST_FLAG(state_data->simple_control.metadata_flags, _obey_metadata_targeting_bit))
	{
		actor->orders.look.primary_priority = _action_obey_primary_priority_locked_aiming;
		actor->orders.look.primary_direction.type = _direction_specification_target;
		actor->orders.look.idle_look_type = _action_obey_idle_look_combat;
		actor->orders.combat.shoot_at_target = TRUE;
		goto flush_order_effects;
	}

	actor->orders.look.primary_priority = _primary_priority_none;
	if (state_data->allow_looking)
	{
		actor->orders.look.idle_look_type = actor->state.mode >= _action_obey_actor_mode_combat ?
			_action_obey_idle_look_combat : _action_obey_idle_look_noncombat;
	}
	else
	{
		actor->orders.look.idle_look_type = _action_obey_idle_look_none;
	}

flush_order_effects:
	if (state_data->complex_control.grenade_throw_depress_trigger)
	{
		actor->orders.combat.throw_grenade = TRUE;
		state_data->complex_control.grenade_throw_depress_trigger = FALSE;
	}

	actor->orders.move.stationary_crouch = state_data->complex_control.override_crouch;
	actor->orders.move.moving_crouch = state_data->complex_control.override_crouch;
	actor->orders.move.override_movement_type = state_data->complex_control.override_movement_type;

	if (state_data->complex_control.play_action && !actor_move_animation_busy(actor_index))
	{
		short animation_impulse = state_data->complex_control.action_animation_impulse;

		if (animation_impulse != NONE)
		{
			real_vector2d alignment_vector;

			alignment_vector.i = actor->control.desired_facing_vector.i;
			alignment_vector.j = actor->control.desired_facing_vector.j;
			normalize2d(&alignment_vector);
			actor_move_animation_impulse(actor_index, animation_impulse, &alignment_vector);
		}

		if (state_data->complex_control.action_communication_type != NONE)
		{
			ai_communication_event(
				state_data->complex_control.action_communication_type,
				actor->meta.unit_index,
				NONE,
				NONE,
				NONE,
				NONE,
				NULL);
		}

		state_data->complex_control.play_action = FALSE;
	}

	if (TEST_FLAG(state_data->simple_control.simple_control_flags, _obey_simple_directmovement_bit))
	{
		actor->orders.move.override_movement_direction = TRUE;
		actor->orders.move.override_movement_direction_vector = state_data->simple_control.directmovement.vector;
		actor->orders.move.override_movement_facing = state_data->simple_control.directmovement.facing;
	}

	if (TEST_FLAG(state_data->simple_control.simple_control_flags, _obey_simple_jump_bit))
	{
		boolean aim_set = FALSE;

		if (TEST_FLAG(state_data->simple_control.simple_control_flags, _obey_simple_jump_jumped_bit))
		{
			aim_set = state_data->simple_control.jump.delay_ticks > 0;
		}
		else if (state_data->simple_control.jump.delay_ticks ||
			actor->input.in_midair ||
			unit_is_busy(actor->meta.unit_index))
		{
			aim_set = TRUE;
		}
		else
		{
			real_vector2d alignment_vector;

			alignment_vector = *(real_vector2d const *)&actor->input.facing_vector;
			if (normalize2d(&alignment_vector) == 0.f)
			{
				alignment_vector = *global_forward2d;
			}

			actor->orders.move.jump = TRUE;
			actor->orders.move.jump_leap =
				state_data->simple_control.jump.target_vertical_vel <
				state_data->simple_control.jump.target_horizontal_vel * 0.7f;
			actor->orders.move.jump_targeted = TEST_FLAG(
				state_data->simple_control.simple_control_flags,
				_obey_simple_jump_targeted_bit);
			actor->orders.move.jump_alignment_vector = alignment_vector;
			actor->orders.move.jump_target_horizontal_vel = state_data->simple_control.jump.target_horizontal_vel;
			actor->orders.move.jump_target_vertical_vel = state_data->simple_control.jump.target_vertical_vel;
			SET_FLAG(
				state_data->simple_control.simple_control_flags,
				_obey_simple_jump_jumped_bit,
				TRUE);

			if (!TEST_FLAG(
				state_data->simple_control.simple_control_flags,
				_obey_simple_jump_targeted_bit))
			{
				state_data->simple_control.jump.delay_ticks = 15;
			}
		}

		if (aim_set)
		{
			actor->orders.move.override_movement_direction = TRUE;
			actor->orders.move.override_movement_direction_vector = actor->input.facing_vector;
			actor->orders.move.override_movement_facing = 0;
		}
	}

	return;
}

/* ---------- private code */

static int vehicle_possibility_qsort(
	void const *a,
	void const *b)
{
	struct vehicle_possibility const *possibility_a = a;
	struct vehicle_possibility const *possibility_b = b;

	if (possibility_a->distance_squared < possibility_b->distance_squared)
	{
		return -1;
	}
	else if (possibility_a->distance_squared > possibility_b->distance_squared)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static void action_obey_command_end(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	byte *next_command_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);

	if (simple_control->current_command_index < command_list->commands.count)
	{
		struct ai_command_definition *command = TAG_BLOCK_GET_ELEMENT(
			&command_list->commands,
			simple_control->current_command_index,
			struct ai_command_definition);

		switch (command->atom_type)
		{
		case _ai_atom_go_to:
		case _ai_atom_go_to_and_face:
			if (unit_index == actor->meta.unit_index)
			{
				actor_move_halt(actor_index);
			}
			if (complex_control)
			{
				complex_control->destination_valid = FALSE;
				complex_control->destination_facing = FALSE;
			}
			break;

		case _ai_atom_move_direction:
		case _ai_atom_move_immediate:
			SET_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_bit, FALSE);
			simple_control->directmovement.facing = NONE;
			break;

		case _ai_atom_shoot:
			if (complex_control)
			{
				complex_control->shoot_at_target = FALSE;
			}
			break;

		case _ai_atom_running_jump:
		case _ai_atom_targeted_jump:
			SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_bit, FALSE);
			simple_control->jump.delay_ticks = 0;
			break;

		case _ai_atom_loop:
			{
				boolean loop = TRUE;

				if (command->atom_modifier == _ai_atom_loop_modifier_until_told_to_advance)
				{
					loop = !TEST_FLAG(simple_control->metadata_flags, _obey_metadata_told_to_advance_bit);
					SET_FLAG(simple_control->metadata_flags, _obey_metadata_told_to_advance_bit, FALSE);
					SET_FLAG(simple_control->metadata_flags, _obey_metadata_waiting_for_advance_notification_bit, loop);
				}

				if (loop)
				{
					if (command->command_index == simple_control->current_command_index)
					{
						char buffer[512];

						ai_debug_describe_actor(actor_index, NONE, TRUE, buffer, sizeof(buffer));
						error(
							_error_silent,
							"%s: command list %s entry #%d tried to loop to itself",
							buffer,
							command_list->name,
							simple_control->current_command_index);
					}
					else if (simple_control->loop_counter >= OBEY_MAXIMUM_LOOP_COUNT)
					{
						char buffer[512];

						ai_debug_describe_actor(actor_index, NONE, TRUE, buffer, sizeof(buffer));
						error(
							_error_silent,
							"%s: command list %s is stuck looping (aborting on loop #%d)",
							buffer,
							command_list->name,
							simple_control->current_command_index);
					}
					else
					{
						*next_command_index = command->command_index;
						simple_control->loop_counter += 1;
					}
				}
			}
			break;

		case _ai_atom_animate:
			{
				struct biped_datum *biped = biped_try_and_get(unit_index);

				if (biped)
				{
					SET_FLAG(biped->biped.flags, _biped_absolute_movement_bit, FALSE);
					SET_FLAG(biped->biped.flags, _biped_no_collision_bit, FALSE);
				}
			}
			break;

		case _ai_atom_look:
		case _ai_atom_look_random:
		case _ai_atom_look_player:
		case _ai_atom_look_object:
			if (unit_index == actor->meta.unit_index)
			{
				actor_look_secondary_stop(actor_index);
			}
			break;
		}
	}

	return;
}

static void action_obey_individual_setup(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	boolean *targeting_reference = user_data;

	csmemset(simple_control, 0, sizeof(*simple_control));
	simple_control->current_command_index = NONE;
	match_assert("c:\\halo\\SOURCE\\ai\\action_obey.c", 1360, targeting_reference);
	SET_FLAG(simple_control->metadata_flags, _obey_metadata_targeting_bit, *targeting_reference);

	if (complex_control)
	{
		csmemset(complex_control, 0, sizeof(*complex_control));
		complex_control->override_movement_type = NONE;
	}

	return;
}

static void action_obey_individual_flush_command_indices(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);

	if (simple_control->current_command_index >= command_list->commands.count)
	{
		simple_control->current_command_index = NONE;
	}

	return;
}

static void action_obey_individual_begin(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);

	if (TEST_FLAG(command_list->flags, _ai_command_list_disable_falling_damage_bit))
	{
		struct unit_datum *unit = unit_get(unit_index);

		SET_FLAG(unit->unit.flags, _unit_no_falling_damage_bit, TRUE);
	}

	return;
}

static void action_obey_individual_advance(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	SET_FLAG(simple_control->metadata_flags, _obey_metadata_waiting_for_advance_notification_bit, FALSE);
	SET_FLAG(simple_control->metadata_flags, _obey_metadata_told_to_advance_bit, TRUE);

	return;
}

static void action_obey_individual_end(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	struct unit_datum *unit = unit_get(unit_index);

	if (!TEST_FLAG(simple_control->metadata_flags, _obey_metadata_commands_finished_bit))
	{
		byte next_command_index;

		action_obey_command_end(
			actor_index,
			unit_index,
			command_list_index,
			simple_control,
			complex_control,
			&next_command_index);
	}
	SET_FLAG(unit->unit.flags, _unit_no_falling_damage_bit, FALSE);

	return;
}

static void action_obey_individuals_iterate(
	long actor_index,
	boolean initialize_structures,
	struct obey_state_data *state_data,
	action_obey_individual_iterator_proc iterator,
	void *user_data)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\action_obey.c",
		1553,
		!actor->meta.swarm || (actor->meta.swarm_cache_index != NONE));

	if (actor->meta.swarm)
	{
		struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);
		short component_index;

		for (component_index = 0; component_index < swarm->unit_count; component_index++)
		{
			struct swarm_component_datum *component =
				swarm_component_get(swarm->component_indices[component_index]);

			if (initialize_structures)
			{
				csmemset(&component->obey, 0, sizeof(component->obey));
				SET_FLAG(component->flags, _swarm_component_wander_bit, FALSE);
				SET_FLAG(component->flags, _swarm_component_obey_bit, TRUE);
			}

			if (TEST_FLAG(component->flags, _swarm_component_obey_bit))
			{
				iterator(
					actor_index,
					swarm->unit_indices[component_index],
					state_data->command_list_index,
					&component->obey,
					NULL,
					user_data);
			}
		}
	}
	else
	{
		iterator(
			actor_index,
			actor->meta.unit_index,
			state_data->command_list_index,
			&state_data->simple_control,
			&state_data->complex_control,
			user_data);
	}

	return;
}

static void action_obey_directmovement_update_facing(
	long actor_index,
	long unit_index,
	struct obey_individual_simple_control *simple_control)
{
	struct actor_datum *actor = actor_get(actor_index);
	real_vector3d facing;
	real_vector3d perpendicular;

	if (unit_index == actor->meta.unit_index)
	{
		facing = actor->input.facing_vector;
	}
	else
	{
		unit_get_facing_vector(unit_index, &facing);
	}

	switch (simple_control->directmovement.facing)
	{
	case _ai_atom_move_facing_forwards:
		simple_control->directmovement.vector = facing;
		break;

	case _ai_atom_move_facing_backwards:
		negate_vector3d(&facing, &simple_control->directmovement.vector);
		break;

	case _ai_atom_move_facing_left:
	case _ai_atom_move_facing_right:
		cross_product3d(global_up3d, &facing, &perpendicular);
		if (normalize3d(&perpendicular) == 0.0f)
		{
			struct unit_datum *unit = unit_get(unit_index);

			cross_product3d(&unit->object.up, &facing, &perpendicular);
			if (normalize3d(&perpendicular) == 0.0f)
			{
				perpendicular = *global_forward3d;
			}
		}
		if (simple_control->directmovement.facing == _ai_atom_move_facing_left)
		{
			simple_control->directmovement.vector = perpendicular;
		}
		else
		{
			negate_vector3d(&perpendicular, &simple_control->directmovement.vector);
		}
		break;
	}

	return;
}

static boolean action_obey_command_begin(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *actor_definition = actor_definition_get(actor->meta.definition_index);
	struct actor_variant_definition *actor_variant_definition = actor_variant_definition_get(actor->meta.variant_definition_index);
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);
	boolean result = FALSE;

	if (simple_control->current_command_index < command_list->commands.count)
	{
		struct ai_command_definition *command = TAG_BLOCK_GET_ELEMENT(
			&command_list->commands,
			simple_control->current_command_index,
			struct ai_command_definition);
		short command_number = simple_control->current_command_index + 1;

		switch (command->atom_type)
		{
		case _ai_atom_pause:
			simple_control->pause_timer = (short)(command->parameter1 * TICKS_PER_SECOND);
			result = TRUE;
			break;

		case _ai_atom_go_to:
		case _ai_atom_go_to_and_face:
			if (complex_control &&
				command->point1_index >= 0 &&
				command->point1_index < command_list->points.count)
			{
				struct ai_command_point_definition *destination_point = TAG_BLOCK_GET_ELEMENT(
					&command_list->points,
					command->point1_index,
					struct ai_command_point_definition);

				simple_control->pause_timer = 0;
				complex_control->destination_valid = TRUE;
				complex_control->destination_keep_moving = command->atom_modifier == _ai_atom_go_to_modifier_keep_moving;
				complex_control->destination_point = destination_point->position;
				complex_control->destination_surface_index = destination_point->surface_index;
				result = actor_move_to_point(
					actor_index,
					&complex_control->destination_point,
					complex_control->destination_surface_index,
					NONE);

				if (result)
				{
					if (complex_control->destination_keep_moving)
					{
						actor_move_keep_moving_past_destination(actor_index);
					}

					if (command->atom_type == _ai_atom_go_to_and_face &&
						command->point2_index >= 0 &&
						command->point2_index < command_list->points.count)
					{
						struct ai_command_point_definition *facing_point = TAG_BLOCK_GET_ELEMENT(
							&command_list->points,
							command->point2_index,
							struct ai_command_point_definition);

						complex_control->destination_facing = TRUE;
						complex_control->destination_facing_point = facing_point->position;
					}
				}
			}
			break;

		case _ai_atom_look:
		case _ai_atom_look_random:
		case _ai_atom_look_player:
		case _ai_atom_look_object:
			if (complex_control)
			{
				short look_point_index = NONE;
				long look_prop_index = NONE;
				long look_unit_index = NONE;
				real look_duration = command->parameter1;

				if (command->atom_type == _ai_atom_look)
				{
					if (command->point1_index >= 0 &&
						command->point1_index < command_list->points.count)
					{
						look_point_index = command->point1_index;
						look_duration = command->parameter1;
					}
				}
				else if (command->atom_type == _ai_atom_look_random)
				{
					if (command->point1_index >= 0 &&
						command->point1_index < command_list->points.count &&
						command->point2_index >= 0 &&
						command->point2_index < command_list->points.count)
					{
						look_point_index = random_range(command->point1_index, command->point2_index + 1);

						if (command->parameter1 == 0.0f && command->parameter2 == 0.0f)
						{
							look_duration = real_random_range(
								actor_definition->looking.idle_look_noncombat.idle_look_time_lower_bound,
								actor_definition->looking.idle_look_noncombat.idle_look_time_upper_bound);
						}
						else
						{
							look_duration = real_random_range(command->parameter1, command->parameter2);
						}
					}
				}
				else if (command->atom_type == _ai_atom_look_player)
				{
					{
						struct prop_iterator prop_iterator;
						struct prop_datum *prop;
						real closest_prop_distance = REAL_MAX;

						prop_iterator_new(&prop_iterator, actor_index);
						while (prop = prop_iterator_next(&prop_iterator))
						{
							if (prop->state >= _prop_state_becoming_unacknowledged &&
								prop->state <= _prop_state_acknowledged &&
								prop->player &&
								prop->distance < closest_prop_distance)
							{
								look_prop_index = prop_iterator.index;
								closest_prop_distance = prop->distance;
							}
						}
					}

					if (look_prop_index == NONE)
					{
						struct data_iterator player_iterator;
						struct player_datum *player;
						real closest_player_distance_squared = REAL_MAX;

						data_iterator_new(&player_iterator, player_data);
						while (player = data_iterator_next(&player_iterator))
						{
							if (player->unit_index != NONE)
							{
								real_point3d head_position;
								real distance_squared;

								unit_get_head_position(player->unit_index, &head_position);
								distance_squared = distance_squared3d(&head_position, &actor->input.position.head_position);
								if (distance_squared < closest_player_distance_squared)
								{
									look_unit_index = player->unit_index;
									closest_player_distance_squared = distance_squared;
								}
							}
						}
					}
				}
				else if (command->atom_type == _ai_atom_look_object)
				{
					struct scenario *scenario = global_scenario_get();

					if (command->object_name_index >= 0 &&
						command->object_name_index < scenario->object_names.count)
					{
						long object_index = object_index_from_name_index(command->object_name_index);

						if (unit_try_and_get(object_index))
						{
							look_prop_index = prop_get_active_by_unit_index(actor_index, object_index);
							look_unit_index = object_index;
						}
					}
				}

				if (look_duration > 0.0f &&
					(look_prop_index != NONE ||
					look_unit_index != NONE ||
					(look_point_index >= 0 && look_point_index < command_list->points.count)))
				{
					struct direction_specification direction;
					short priority = _secondary_look_priority_default;

					if (command->atom_modifier == _ai_atom_look_modifier_idle_turn_around)
					{
						priority = _secondary_look_priority_turn_and_aim;
					}
					else if (command->atom_modifier == _ai_atom_look_modifier_idle_look)
					{
						priority = _secondary_look_priority_idle_look;
					}
					else if (command->atom_modifier == _ai_atom_look_modifier_force_aim_weapon)
					{
						priority = _secondary_look_priority_override;
					}
					else if (command->atom_modifier == _ai_atom_look_modifier_force_facing)
					{
						priority = _secondary_look_priority_override_exact_facing;
					}

					if (look_prop_index != NONE)
					{
						direction.type = _direction_specification_prop;
						direction.prop_index = look_prop_index;
					}
					else if (look_unit_index != NONE)
					{
						direction.type = _direction_specification_point;
						unit_get_head_position(look_unit_index, &direction.point);
					}
					else
					{
						struct ai_command_point_definition *look_point = TAG_BLOCK_GET_ELEMENT(
							&command_list->points,
							look_point_index,
							struct ai_command_point_definition);

						direction.type = _direction_specification_point;
						direction.point = look_point->position;
					}

					actor_look_secondary(actor_index, _secondary_look_scripted, priority, &direction);
					simple_control->pause_timer = (short)(look_duration * TICKS_PER_SECOND);
					result = TRUE;
				}
			}
			break;

		case _ai_atom_move_direction:
			if (unit_index == actor->meta.unit_index)
			{
				simple_control->directmovement.start_position = actor->input.position.body_position;
			}
			else
			{
				object_get_origin(unit_index, &simple_control->directmovement.start_position);
			}

			if (command->point1_index >= 0 &&
				command->point1_index < command_list->points.count)
			{
				struct ai_command_point_definition *direction_point = TAG_BLOCK_GET_ELEMENT(
					&command_list->points,
					command->point1_index,
					struct ai_command_point_definition);

				vector_from_points3d(
					&simple_control->directmovement.start_position,
					&direction_point->position,
					&simple_control->directmovement.vector);
				result = normalize3d(&simple_control->directmovement.vector) > 0.0f;
			}
			else if (command->parameter2 >= 0.0f && command->parameter2 < 360.0f)
			{
				vector3d_from_angle(&simple_control->directmovement.vector, DEGREES_TO_RADIANS(command->parameter2));
				result = TRUE;
			}

			if (result)
			{
				if (command->atom_modifier >= _ai_atom_move_facing_forwards &&
					command->atom_modifier <= _ai_atom_move_facing_right)
				{
					simple_control->directmovement.facing = command->atom_modifier;
				}
				else
				{
					simple_control->directmovement.facing = NONE;
				}

				if (unit_index == actor->meta.unit_index)
				{
					actor_move_halt(actor_index);
				}

				SET_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_update_continuously_bit, FALSE);
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_bit, TRUE);
			}
			break;

		case _ai_atom_move_immediate:
			if (command->atom_modifier >= _ai_atom_move_facing_forwards &&
				command->atom_modifier <= _ai_atom_move_facing_right)
			{
				simple_control->directmovement.facing = command->atom_modifier;
			}
			else
			{
				simple_control->directmovement.facing = _ai_atom_move_facing_forwards;
			}

			action_obey_directmovement_update_facing(actor_index, unit_index, simple_control);
			simple_control->pause_timer = (short)(command->parameter1 * TICKS_PER_SECOND);
			SET_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_bit, TRUE);
			SET_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_update_continuously_bit, TRUE);
			result = TRUE;
			break;

		case _ai_atom_running_jump:
			if (unit_index != actor->meta.unit_index || actor->input.vehicle_index == NONE)
			{
				boolean moving_forward;

				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_bit, TRUE);
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_jumped_bit, FALSE);
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_targeted_bit, FALSE);

				if (unit_index == actor->meta.unit_index)
				{
					moving_forward = actor->control.moving && actor->control.moving_facing_direction == _ai_atom_move_facing_forwards;
				}
				else
				{
					struct unit_datum *unit = unit_get(unit_index);

					moving_forward = unit->object.parent_object_index == NONE &&
						dot_product3d(&unit->object.translational_velocity, &unit->object.forward) > 0.06666667f;
				}

				simple_control->jump.delay_ticks = moving_forward ? 0 : 10;
				simple_control->pause_timer = 2 * TICKS_PER_SECOND;
				result = TRUE;
			}
			break;

		case _ai_atom_targeted_jump:
			if (unit_index != actor->meta.unit_index || actor->input.vehicle_index == NONE)
			{
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_bit, TRUE);
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_jumped_bit, FALSE);
				SET_FLAG(simple_control->simple_control_flags, _obey_simple_jump_targeted_bit, TRUE);
				simple_control->jump.delay_ticks = 0;
				simple_control->jump.target_horizontal_vel = command->parameter1;
				simple_control->jump.target_vertical_vel = command->parameter2;
				simple_control->pause_timer = 2 * TICKS_PER_SECOND;
				result = TRUE;
			}
			break;

		case _ai_atom_animation_mode:
			if (complex_control &&
				command->atom_modifier >= _ai_atom_animation_mode_modifier_noncombat &&
				command->atom_modifier < NUMBER_OF_AI_ATOM_ANIMATION_MODE_MODIFIERS)
			{
				complex_control->override_movement_type = command->atom_modifier;
				result = TRUE;
			}
			break;

		case _ai_atom_crouch:
			if (complex_control)
			{
				complex_control->override_crouch = command->atom_modifier == _ai_atom_crouch_modifier_enable;
				result = TRUE;
			}
			break;

		case _ai_atom_targeting:
			SET_FLAG(
				simple_control->metadata_flags,
				_obey_metadata_targeting_bit,
				command->atom_modifier == _ai_atom_targeting_modifier_enable);
			result = TRUE;
			break;

		case _ai_atom_initiative:
			if (unit_index == actor->meta.unit_index)
			{
				actor->state.action_data.obey.initiative = command->atom_modifier == _ai_atom_initiative_modifier_enable;
				result = TRUE;
			}
			break;

		case _ai_atom_set_radius:
			if (complex_control && command->parameter1 > 0.0f)
			{
				complex_control->destination_radius_valid = TRUE;
				complex_control->destination_radius = command->parameter1;
				result = TRUE;
			}
			break;

		case _ai_atom_action:
			if (complex_control)
			{
				complex_control->play_action = FALSE;

				switch (command->atom_modifier)
				{
				case _ai_atom_action_modifier_berserk:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_berserk;
					complex_control->action_communication_type = _ai_communication_berserk;
					break;

				case _ai_atom_action_modifier_surprise_front:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_surprise_front;
					complex_control->action_communication_type = _ai_communication_surprise;
					break;

				case _ai_atom_action_modifier_surprise_back:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_surprise_back;
					complex_control->action_communication_type = _ai_communication_surprise;
					break;

				case _ai_atom_action_modifier_evade_left:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_evade_left;
					complex_control->action_communication_type = NONE;
					break;

				case _ai_atom_action_modifier_evade_right:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_evade_right;
					complex_control->action_communication_type = NONE;
					break;

				case _ai_atom_action_modifier_dive_forward:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_dive_front;
					complex_control->action_communication_type = _ai_communication_dive;
					break;

				case _ai_atom_action_modifier_dive_back:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_dive_back;
					complex_control->action_communication_type = _ai_communication_dive;
					break;

				case _ai_atom_action_modifier_dive_left:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_dive_left;
					complex_control->action_communication_type = _ai_communication_dive;
					break;

				case _ai_atom_action_modifier_dive_right:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = _unit_animation_impulse_dive_right;
					complex_control->action_communication_type = _ai_communication_dive;
					break;

				case _ai_atom_action_modifier_vehicle_woohoo:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = NONE;
					complex_control->action_communication_type = _ai_communication_vehicle_woohoo;
					break;

				case _ai_atom_action_modifier_vehicle_scared:
					complex_control->play_action = TRUE;
					complex_control->action_animation_impulse = NONE;
					complex_control->action_communication_type = _ai_communication_vehicle_scared;
					break;
				}

				result = complex_control->play_action;
			}
			break;

		case _ai_atom_shoot:
			if (complex_control &&
				command->point1_index >= 0 &&
				command->point1_index < command_list->points.count)
			{
				struct ai_command_point_definition *shoot_point = TAG_BLOCK_GET_ELEMENT(
					&command_list->points,
					command->point1_index,
					struct ai_command_point_definition);

				complex_control->shoot_at_target = TRUE;
				complex_control->shoot_target = shoot_point->position;
				complex_control->shoot_burst_length = command->parameter1;
				result = TRUE;
			}
			break;

		case _ai_atom_grenade:
			if (complex_control &&
				actor_variant_definition->grenade_combat.grenade_type != NONE &&
				command->point1_index >= 0 &&
				command->point1_index < command_list->points.count)
			{
				struct ai_command_point_definition *grenade_point = TAG_BLOCK_GET_ELEMENT(
					&command_list->points,
					command->point1_index,
					struct ai_command_point_definition);

				unit_add_grenade_type_to_inventory(
					actor->meta.unit_index,
					actor_variant_definition->grenade_combat.grenade_type,
					1);
				complex_control->grenade_throw_started = FALSE;
				complex_control->grenade_throw_depress_trigger = FALSE;
				complex_control->grenade_target = grenade_point->position;
				complex_control->grenade_throw_trajectory_type = _actor_atom_grenade_modifier_toss;
				if (command->atom_modifier >= _actor_atom_grenade_modifier_toss &&
					command->atom_modifier < NUMBER_OF_AI_ATOM_GRENADE_MODIFIERS)
				{
					complex_control->grenade_throw_trajectory_type = command->atom_modifier;
				}
				simple_control->pause_timer = 2 * TICKS_PER_SECOND;
				result = TRUE;
			}
			break;

		case _ai_atom_vehicle:
			if (unit_index == actor->meta.unit_index)
			{
				struct vehicle_possibility possibilities[16];
				struct object_iterator iterator;
				short possibility_count = 0;
				short seat_desire_type = NONE;
				short possibility_index;

				object_iterator_new(&iterator, _object_mask_vehicle, 0);
				while (object_iterator_next(&iterator))
				{
					real_point3d vehicle_origin;
					real distance_squared;

					object_get_origin(iterator.index, &vehicle_origin);
					distance_squared = distance_squared3d(&actor->input.position.body_position, &vehicle_origin);
					if (command->parameter1 == 0.0f ||
						distance_squared < command->parameter1 * command->parameter1)
					{
						possibilities[possibility_count].distance_squared = distance_squared;
						possibilities[possibility_count].vehicle_index = iterator.index;
						possibility_count++;
						if (possibility_count >= NUMBEROF(possibilities))
						{
							break;
						}
					}
				}

				if (possibility_count > 1)
				{
					qsort(possibilities, possibility_count, sizeof(struct vehicle_possibility), vehicle_possibility_qsort);
				}

				if (command->atom_modifier >= _ai_atom_vehicle_modifier_any_non_driver &&
					command->atom_modifier < NUMBER_OF_AI_ATOM_VEHICLE_MODIFIERS)
				{
					seat_desire_type = command->atom_modifier;
				}

				for (possibility_index = 0; possibility_index < possibility_count; possibility_index++)
				{
					if (actor_action_try_to_enter_vehicle(
						actor_index,
						possibilities[possibility_index].vehicle_index,
						"",
						seat_desire_type,
						0,
						NULL))
					{
						SET_FLAG(simple_control->metadata_flags, _obey_metadata_action_changed_bit, TRUE);
						result = TRUE;
						break;
					}
				}
			}
			break;

		case _ai_atom_animate:
			if (command->animation_reference_index != NONE)
			{
				struct ai_animation_reference_definition *animation_reference = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_animation_references,
					command->animation_reference_index,
					struct ai_animation_reference_definition);
				long animation_graph_index = animation_reference->animation_graph.index;
				boolean absolute_movement = FALSE;
				boolean no_collision = FALSE;
				boolean interpolate = TRUE;

				if (animation_graph_index == NONE)
				{
					animation_graph_index = unit_definition_get(unit_get(unit_index)->definition_index)->object.animation_graph.index;
				}

				switch (command->atom_modifier)
				{
				case _ai_atom_animate_modifier_absolute_movement:
					absolute_movement = TRUE;
					break;

				case _ai_atom_animate_modifier_absolute_movement_no_collision:
					no_collision = TRUE;
					absolute_movement = TRUE;
					break;

				case _ai_atom_animate_modifier_no_interpolation_relative_movement:
					interpolate = FALSE;
					break;

				case _ai_atom_animate_modifier_no_interpolation_absolute_movement:
					interpolate = FALSE;
					absolute_movement = TRUE;
					break;

				case _ai_atom_animate_modifier_no_interpolation_absolute_movement_no_collision:
					interpolate = FALSE;
					no_collision = TRUE;
					absolute_movement = TRUE;
					break;
				}

				if (unit_start_user_animation(
					unit_index,
					animation_graph_index,
					animation_reference->animation_name,
					interpolate))
				{
					struct biped_datum *biped = biped_try_and_get(unit_index);

					if (biped)
					{
						SET_FLAG(biped->biped.flags, _biped_absolute_movement_bit, absolute_movement);
						SET_FLAG(biped->biped.flags, _biped_no_collision_bit, no_collision);
					}
					result = TRUE;
				}
			}
			break;

		case _ai_atom_recording:
			if (command->recording_reference_index >= 0 &&
				command->recording_reference_index < global_scenario_get()->ai_recording_references.count)
			{
				struct ai_recording_reference_definition *recording_reference = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_recording_references,
					command->recording_reference_index,
					struct ai_recording_reference_definition);
				short recorded_animation_index = scenario_get_animation_by_name(
					global_scenario_get(),
					recording_reference->recording_name);

				if (recorded_animation_index != NONE)
				{
					result = recorded_animation_play(unit_index, recorded_animation_index);
				}
			}
			break;

		case _ai_atom_script:
			if (command->script_reference_index >= 0 &&
				command->script_reference_index < global_scenario_get()->ai_script_references.count)
			{
				result = hs_wake_by_name(TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_script_references,
					command->script_reference_index,
					struct ai_script_reference_definition)->script_name);
			}
			break;

		case _ai_atom_loop:
			if (command->command_index >= 0 &&
				command->command_index < command_list->commands.count &&
				command->command_index != simple_control->current_command_index)
			{
				result = TRUE;
			}
			break;

		case _ai_atom_die:
			{
				struct unit_datum *unit = unit_get(unit_index);

				if (command->atom_modifier == _ai_atom_die_modifier_silent)
				{
					SET_FLAG(unit->object.damage_flags, _object_die_act_of_god_silent_bit, TRUE);
				}
				else
				{
					SET_FLAG(unit->object.damage_flags, _object_die_act_of_god_bit, TRUE);
				}
				result = TRUE;
			}
			break;

		case _ai_atom_vocalize:
			{
				short vocalization_type = command->atom_modifier;
				long sound_definition_index = NONE;
				short speech_type = unit_test_speech(
					unit_index,
					_unit_speech_scripted,
					TRUE,
					TRUE,
					NULL,
					&vocalization_type,
					&sound_definition_index);

				if (speech_type > 0)
				{
					struct unit_speech_item speech_item;

					csmemset(&speech_item, 0, sizeof(speech_item));
					speech_item.vocalization_type = vocalization_type;
					speech_item.sound_definition_index = sound_definition_index;
					speech_item.priority = _unit_speech_scripted;
					ai_communication_packet_new(&speech_item.ai);
					unit_speak(unit_index, speech_type, &speech_item);
					result = TRUE;
				}
			}
			break;

		case _ai_atom_teleport:
			if (command->point1_index >= 0 &&
				command->point1_index < command_list->points.count)
			{
				struct ai_command_point_definition *position_point = TAG_BLOCK_GET_ELEMENT(
					&command_list->points,
					command->point1_index,
					struct ai_command_point_definition);
				real_vector3d facing;

				unit_get_facing_vector(unit_index, &facing);
				if (command->point2_index >= 0 &&
					command->point2_index < command_list->points.count)
				{
					struct ai_command_point_definition *facing_point = TAG_BLOCK_GET_ELEMENT(
						&command_list->points,
						command->point2_index,
						struct ai_command_point_definition);
					struct biped_datum *biped = biped_try_and_get(unit_index);
					struct biped_definition *biped_definition = !biped ? NULL : biped_definition_get(biped->definition_index);

					vector_from_points3d(&position_point->position, &facing_point->position, &facing);
					if (biped_definition &&
						(TEST_FLAG(biped_definition->biped.flags, _biped_flying_bit) ||
						TEST_FLAG(biped_definition->biped.flags, _biped_climbs_anything_bit)))
					{
						if (normalize3d(&facing) == 0.0f)
						{
							unit_get_facing_vector(unit_index, &facing);
						}
					}
					else
					{
						facing.k = 0.0f;
						if (normalize2d((real_vector2d *)&facing) == 0.0f)
						{
							unit_get_facing_vector(unit_index, &facing);
						}
					}
				}

				object_set_position(unit_index, &position_point->position, &facing, NULL);
				object_reset(unit_index);
				object_compute_node_matrices_recursive(unit_index);
				if (unit_index == actor->meta.unit_index)
				{
					actor_input_sample_position(actor_index, actor->meta.unit_index, &actor->input.position);
					actor_move_halt(actor_index);
				}
				result = TRUE;
			}
			break;

		case _ai_atom_wait:
			result = TRUE;
			break;
		}

		if (ai_debug.print_command_lists)
		{
			char actor_description[256];

			if (actor->meta.encounter_index == NONE)
			{
				csstrcpy(actor_description, "<no encounter>");
			}
			else
			{
				struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
					struct encounter_definition);
				struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
					&encounter->squads,
					actor->meta.squad_index,
					struct squad_definition);

				sprintf(actor_description, "%s/%s", encounter->name, squad->name);
			}

			action_obey_describe_command(global_scenario_get(), command, temporary, NUMBEROF(temporary));
			error(
				_error_silent,
				"%s: %s #%d%s: %s",
				actor_description,
				command_list->name,
				command_number,
				result ? "" : " FAILED",
				temporary);
		}
	}

	return result;
}

static boolean action_obey_command_perform(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);
	boolean done = TRUE;

	if (simple_control->current_command_index < command_list->commands.count)
	{
		struct ai_command_definition *current_command = TAG_BLOCK_GET_ELEMENT(
			&command_list->commands,
			simple_control->current_command_index,
			struct ai_command_definition);

		switch (current_command->atom_type)
		{
		case _ai_atom_pause:
		case _ai_atom_look:
		case _ai_atom_move_immediate:
		case _ai_atom_look_random:
		case _ai_atom_look_player:
		case _ai_atom_look_object:
			done = simple_control->pause_timer == 0;
			break;

		case _ai_atom_go_to:
		case _ai_atom_go_to_and_face:
			if (unit_index == actor->meta.unit_index && complex_control)
			{
				done = actor_path_at_destination(actor_index);

				if (!done &&
					complex_control->destination_keep_moving &&
					complex_control->destination_valid)
				{
					real tolerance = actor_destination_tolerance(actor_index);
					real_vector3d destination_vector;
					real distance_squared;

					vector_from_points3d(
						&actor->input.position.body_position,
						&complex_control->destination_point,
						&destination_vector);
					distance_squared = magnitude_squared3d(&destination_vector);

					if (distance_squared < tolerance * tolerance)
					{
						done = TRUE;
					}
					else if (distance_squared < (tolerance + 0.5f) * (tolerance + 0.5f))
					{
						struct unit_datum *unit = unit_get(actor->meta.unit_index);

						if (dot_product3d(&destination_vector, &unit->object.translational_velocity) < 0.0f)
						{
							done = TRUE;
						}
					}
				}

				if (current_command->atom_type == _ai_atom_go_to_and_face ||
					current_command->atom_modifier == _ai_atom_go_to_modifier_stop_at_point)
				{
					if (actor->control.moving)
					{
						simple_control->pause_timer = 10;
					}
					done = done && simple_control->pause_timer == 0;
				}

				if (done && complex_control->destination_facing)
				{
					real facing_dot;

					match_assert(
						"c:\\halo\\SOURCE\\ai\\action_obey.c",
						983,
						current_command->atom_type == _ai_atom_go_to_and_face);

					if (actor->state.flying)
					{
						real_vector3d facing;

						vector_from_points3d(
							&actor->input.position.body_position,
							&complex_control->destination_facing_point,
							&facing);
						if (!(normalize3d(&facing) > 0.0f))
						{
							goto facing_done;
						}
						facing_dot = dot_product3d(&facing, &actor->input.facing_vector);
					}
					else
					{
						real_vector2d facing;

						vector_from_points2d(
							(real_point2d const *)&actor->input.position.body_position,
							(real_point2d const *)&complex_control->destination_facing_point,
							&facing);
						if (!(normalize2d(&facing) > 0.0f))
						{
							goto facing_done;
						}
						facing_dot = dot_product2d(&facing, (real_vector2d const *)&actor->input.facing_vector);
					}

					if (facing_dot < 0.984f)
					{
						done = FALSE;
					}
				}

			facing_done:
				if (done)
				{
					actor_move_halt(actor_index);
				}
			}
			else
			{
				done = TRUE;
			}
			break;

		case _ai_atom_move_direction:
			{
				real_point3d origin;
				real_vector3d movement;

				if (unit_index == actor->meta.unit_index)
				{
					origin = actor->input.position.body_position;
				}
				else
				{
					object_get_origin(unit_index, &origin);
				}

				vector_from_points3d(
					&simple_control->directmovement.start_position,
					&origin,
					&movement);
				done = dot_product3d(&movement, &simple_control->directmovement.vector) > current_command->parameter1;
			}
			break;

		case _ai_atom_shoot:
			if (unit_index == actor->meta.unit_index && complex_control)
			{
				if (actor->control.current_fire_target_type != _actor_fire_target_manual_point ||
					!(distance_squared3d(
						&complex_control->shoot_target,
						&actor->control.current_fire_target_manual_point) < 0.25f))
				{
					struct actor_variant_definition *variant_definition =
						actor_variant_definition_get(actor->meta.variant_definition_index);
					short pause_ticks = (short)(variant_definition->ranged_combat.first_burst_delay_upper_bound * 30.0f);

					simple_control->pause_timer = MAX(pause_ticks, 60);
				}
				done = simple_control->pause_timer == 0;
			}
			else
			{
				done = TRUE;
			}
			break;

		case _ai_atom_running_jump:
		case _ai_atom_targeted_jump:
			if (TEST_FLAG(simple_control->simple_control_flags, _obey_simple_jump_bit))
			{
				boolean airborne;

				if (unit_index == actor->meta.unit_index)
				{
					airborne = actor->input.in_midair;
				}
				else
				{
					airborne = unit_flying_through_air(unit_index);
				}

				if (TEST_FLAG(simple_control->simple_control_flags, _obey_simple_jump_jumped_bit) && airborne)
				{
					simple_control->pause_timer = 0;
				}
				done = simple_control->pause_timer == 0;
			}
			else
			{
				done = TRUE;
			}
			break;

		case _ai_atom_grenade:
			if (unit_index == actor->meta.unit_index && complex_control)
			{
				if (complex_control->grenade_throw_started)
				{
					struct unit_datum *unit = unit_get(unit_index);

					simple_control->pause_timer = unit->unit.grenade_throw_state != _unit_grenade_throw_idle ? 30 : 0;
				}
				else if (!unit_is_busy(unit_index))
				{
					real_point3d grenade_target = complex_control->grenade_target;

					if (actor_combat_plan_grenade_trajectory(
						actor_index,
						complex_control->grenade_throw_trajectory_type,
						&grenade_target,
						NONE,
						NONE))
					{
						complex_control->grenade_throw_depress_trigger = TRUE;
					}
				}
				done = simple_control->pause_timer == 0;
			}
			else
			{
				done = TRUE;
			}
			break;

		case _ai_atom_animate:
			done = unit_get(unit_index)->unit.animation.state != _unit_state_user_animation;
			break;

		case _ai_atom_recording:
			done = !recorded_animation_controlling_unit(unit_index);
			break;

		case _ai_atom_action:
			done = !complex_control || !complex_control->play_action;
			break;

		case _ai_atom_wait:
			switch (current_command->atom_modifier)
			{
			case _ai_atom_wait_modifier_alerted:
				done = actor->state.combat_status > _actor_combat_status_none;
				break;

			case _ai_atom_wait_modifier_visible_enemy:
				done = actor->state.combat_status >= _actor_combat_status_visible;
				break;

			case _ai_atom_wait_modifier_told_to_advance:
				if (TEST_FLAG(simple_control->metadata_flags, _obey_metadata_told_to_advance_bit))
				{
					SET_FLAG(simple_control->metadata_flags, _obey_metadata_told_to_advance_bit, FALSE);
					SET_FLAG(simple_control->metadata_flags, _obey_metadata_waiting_for_advance_notification_bit, FALSE);
					done = TRUE;
				}
				else
				{
					SET_FLAG(simple_control->metadata_flags, _obey_metadata_waiting_for_advance_notification_bit, TRUE);
					done = FALSE;
				}
				break;

			default:
				done = TRUE;
				break;
			}
			break;

		case _ai_atom_vocalize:
			done = unit_get(unit_index)->unit.speech.current.priority != _unit_speech_scripted;
			break;

		case _ai_atom_animation_mode:
		case _ai_atom_crouch:
		case _ai_atom_vehicle:
		case _ai_atom_script:
		case _ai_atom_targeting:
		case _ai_atom_initiative:
		case _ai_atom_loop:
		case _ai_atom_die:
		case _ai_atom_set_radius:
		case _ai_atom_teleport:
			done = TRUE;
			break;
		}
	}

	return done;
}

static void action_obey_individual_perform(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);
	boolean *finished_reference = user_data;

	if (!TEST_FLAG(simple_control->metadata_flags, _obey_metadata_commands_finished_bit))
	{
		boolean command_valid = FALSE;

		if (simple_control->current_command_index < command_list->commands.count)
		{
			command_valid = TRUE;
		}

		simple_control->loop_counter = 0;
		do
		{
			byte next_command_index;

			if (command_valid &&
				!action_obey_command_perform(
					actor_index,
					unit_index,
					command_list_index,
					simple_control,
					complex_control))
			{
				break;
			}

			next_command_index = simple_control->current_command_index == NONE ? 0 : simple_control->current_command_index + 1;

			if (command_valid)
			{
				action_obey_command_end(
					actor_index,
					unit_index,
					command_list_index,
					simple_control,
					complex_control,
					&next_command_index);
			}

			if (next_command_index >= command_list->commands.count)
			{
				SET_FLAG(simple_control->metadata_flags, _obey_metadata_commands_finished_bit, TRUE);
				break;
			}

			simple_control->current_command_index = next_command_index;
			command_valid = action_obey_command_begin(
				actor_index,
				unit_index,
				command_list_index,
				simple_control,
				complex_control);
		}
		while (!TEST_FLAG(simple_control->metadata_flags, _obey_metadata_action_changed_bit));
	}

	if (!TEST_FLAG(simple_control->metadata_flags, _obey_metadata_commands_finished_bit))
	{
		match_assert("c:\\halo\\SOURCE\\ai\\action_obey.c", 1429, finished_reference);
		*finished_reference = FALSE;
	}

	return;
}

static void action_obey_individual_update(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control,
	void *user_data)
{
	if (simple_control->pause_timer > 0)
	{
		simple_control->pause_timer -= 1;
	}

	if (TEST_FLAG(simple_control->simple_control_flags, _obey_simple_jump_bit))
	{
		if (simple_control->jump.delay_ticks > 0)
		{
			simple_control->jump.delay_ticks -= 1;
		}
	}

	if (TEST_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_bit) &&
		TEST_FLAG(simple_control->simple_control_flags, _obey_simple_directmovement_update_continuously_bit))
	{
		action_obey_directmovement_update_facing(actor_index, unit_index, simple_control);
	}

	return;
}
