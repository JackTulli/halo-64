/*
ACTOR_LOOKING.C

symbols in this file:
00015E80 0070:
	_actor_look_secondary_stop (0000)
00015EF0 0130:
	_actor_look_compute_prop_interest (0000)
00016020 0050:
	_actor_look_get_looking_definition (0000)
00016070 0370:
	_actor_look_secondary (0000)
000163E0 0080:
	_actor_look_valid_aim_vector (0000)
00016460 00f0:
	_actor_look_valid_look_vector (0000)
00016550 00b0:
	_actor_looking_test_validity (0000)
00016600 0260:
	_actor_look_idle_find_prop (0000)
00016860 0160:
	_actor_look_idle_timer (0000)
000169C0 0260:
	_actor_look_find_random_vector (0000)
00016C20 0050:
	_valid_real_normal2d (0000)
00016C70 0660:
	_actor_look_decode_direction (0000)
000172D0 0210:
	_actor_look_idle_new_major_direction (0000)
000174E0 0170:
	_actor_look_idle_new_minor_direction (0000)
00017650 1270:
	_actor_look_update (0000)
000188C0 0080:
	_actor_look_affect_movement (0000)
00244C74 0070:
	_global_secondary_look_priorities (0000)
	_global_secondary_look_times (0038)
00244CE4 000e:
	??_C@_0O@FNONJPCB@?$CFs?3?5look?9stop?$AA@ (0000)
00244CF4 0004:
	__real@3fe66666 (0000)
00244CF8 0012:
	??_C@_0BC@CBOPKOHG@?$CFs?3?5look?5?$CFs?5?$CFs?5?$CFd?$AA@ (0000)
00244D0C 0009:
	??_C@_08IILCFMOF@scripted?$AA@ (0000)
00244D18 0007:
	??_C@_06NCEIAKB@danger?$AA@ (0000)
00244D20 0007:
	??_C@_06GJOIPFFF@damage?$AA@ (0000)
00244D28 000c:
	??_C@_0M@KFECPMLC@combat?9prop?$AA@ (0000)
00244D34 000f:
	??_C@_0P@HANCNCFH@comm?9direction?$AA@ (0000)
00244D44 000a:
	??_C@_09OLJCJFBL@comm?9prop?$AA@ (0000)
00244D50 000e:
	??_C@_0O@BOCADBDG@shooting?9prop?$AA@ (0000)
00244D60 0009:
	??_C@_08NDPBJKMG@new?9prop?$AA@ (0000)
00244D6C 0007:
	??_C@_06LDFNIMBB@impact?$AA@ (0000)
00244D74 000c:
	??_C@_0M@JOHOFPJB@moving?9prop?$AA@ (0000)
00244D80 000c:
	??_C@_0M@PINLHHEI@environment?$AA@ (0000)
00244D8C 0010:
	??_C@_0BA@MGHLNDAM@override?9facing?$AA@ (0000)
00244D9C 0009:
	??_C@_08PNBPAFID@override?$AA@ (0000)
00244DA8 000d:
	??_C@_0N@OPBBNFGJ@stop?9and?9aim?$AA@ (0000)
00244DB8 000d:
	??_C@_0N@FFMCKHMJ@turn?9and?9aim?$AA@ (0000)
00244DC8 0004:
	??_C@_03MKHEFFMA@aim?$AA@ (0000)
00244DCC 0009:
	??_C@_08IECGFEAB@idle?9aim?$AA@ (0000)
00244DD8 000a:
	??_C@_09CPPFOLKN@idle?9look?$AA@ (0000)
00244DE4 0008:
	??_C@_07DLHCIBDH@default?$AA@ (0000)
00244DEC 0037:
	??_C@_0DH@BKMNBGNP@?$CItype?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CItype?5?$DM?5NUMBER_OF@ (0000)
00244E24 0022:
	??_C@_0CC@OBFKKCBI@c?3?2halo?2SOURCE?2ai?2actor_looking?4@ (0000)
00244E48 0004:
	__real@3ada740e (0000)
00244E50 0049:
	??_C@_0EJ@PDHFHBNF@?$CFs?3?5logic?5error?5in?5actor?5looking@ (0000)
00244E9C 0008:
	??_C@_07FJPMPBHO@looking?$AA@ (0000)
00244EA4 0007:
	??_C@_06CPNACJHN@aiming?$AA@ (0000)
00244EAC 0007:
	??_C@_06LJBECLPP@facing?$AA@ (0000)
00244EB4 000e:
	??_C@_0O@LDGGHEKC@result_vector?$AA@ (0000)
00244EC4 0023:
	??_C@_0CD@JLCDLMPD@denormalized?5?$CFf?3?5?$DMerror?$DO?5?$CItype?5?$CF@ (0000)
00244EE8 0021:
	??_C@_0CB@ILCNAOOH@denormalized?5?$CFf?3?5object?5?$CI0x?$CF08X?$CJ@ (0000)
00244F0C 001f:
	??_C@_0BP@JMCBPAFA@denormalized?5?$CFf?3?5danger?5?$CInone?$CJ?$AA@ (0000)
00244F2C 0023:
	??_C@_0CD@BJDFLJJM@denormalized?5?$CFf?3?5danger?5?$CI?$CFf?5?$CFf?5?$CF@ (0000)
00244F50 0023:
	??_C@_0CD@MPCIMEDM@denormalized?5?$CFf?3?5vector?5?$CI?$CFf?5?$CFf?5?$CF@ (0000)
00244F74 0022:
	??_C@_0CC@KKBEICMF@denormalized?5?$CFf?3?5point?5?$CI?$CFf?5?$CFf?5?$CFf@ (0000)
00244F98 003a:
	??_C@_0DK@KHHBJFIN@denormalized?5?$CFf?3?5prop?50x?$CF08X?$CFs?5?$CI@ (0000)
00244FD4 000b:
	??_C@_0L@CGKLOAKK@?5?$CIinvalid?$CJ?$AA@ (0000)
00244FE0 001f:
	??_C@_0BP@IAJLBFML@denormalized?5?$CFf?3?5target?5?$CInone?$CJ?$AA@ (0000)
00245000 0036:
	??_C@_0DG@LJDBLOA@denormalized?5?$CFf?3?5target?5?$CIprop?50x@ (0000)
00245038 0042:
	??_C@_0EC@ONEINDFA@denormalized?5?$CFf?3?5target?5type?5?$CFd?5@ (0000)
0024507C 003c:
	??_C@_0DM@KFLCBPJA@denormalized?5?$CFf?3?5target?5type?5?$CFd?5@ (0000)
002450B8 0032:
	??_C@_0DC@MBKOGPLI@denormalized?5?$CFf?3?5?$CFsmoving?5?$CIp?$CFf?5?$CF@ (0000)
002450EC 0005:
	??_C@_04LKOGMCLF@not?5?$AA@ (0000)
002450F4 000a:
	??_C@_09HPKLLIJF@direction?$AA@ (0000)
00245100 000e:
	??_C@_0O@DNCJFFDC@specification?$AA@ (0000)
00245110 001e:
	??_C@_0BO@ICLEICLJ@?$CGactor?9?$DOoutput?4looking_vector?$AA@ (0000)
00245130 001d:
	??_C@_0BN@MLJELDAF@?$CGactor?9?$DOoutput?4aiming_vector?$AA@ (0000)
00245150 001d:
	??_C@_0BN@BNGLBJLG@?$CGactor?9?$DOoutput?4facing_vector?$AA@ (0000)
00245170 0004:
	__real@3f666666 (0000)
00245174 0027:
	??_C@_0CH@MHPDGGDP@?$CFs?3?5assert_valid_real_normal2d?$CI?$CF@ (0000)
0024519C 002f:
	??_C@_0CP@OAIIPIIE@?$CIreal_vector2d?5?$CK?$CJ?5?$CGactor?9?$DOoutput@ (0000)
002451CC 0013:
	??_C@_0BD@CADKODCD@?$CGidle_minor_vector?$AA@ (0000)
002451E0 0024:
	??_C@_0CE@LCIPMIDM@actor?9?$DOcontrol?4idle_minor_timer?5@ (0000)
00245204 0013:
	??_C@_0BD@JGOPOGFP@?$CGidle_major_vector?$AA@ (0000)
00245218 004b:
	??_C@_0EL@IKADNPKK@?$CBactor?9?$DOcontrol?4idle_major_activ@ (0000)
00245264 0016:
	??_C@_0BG@GOAPBADG@?$CFs?3?5look?5timer?5expire?$AA@ (0000)
0024527C 0012:
	??_C@_0BC@OJIPDKMD@?$CGsecondary_vector?$AA@ (0000)
00245290 0010:
	??_C@_0BA@GFAAOIFJ@?$CGprimary_vector?$AA@ (0000)
002452A0 0027:
	??_C@_0CH@JLLNHPPI@?$CGactor?9?$DOcontrol?4desired_looking_@ (0000)
002452C8 0026:
	??_C@_0CG@KOFPIDJD@?$CGactor?9?$DOcontrol?4desired_aiming_v@ (0000)
002452F0 0026:
	??_C@_0CG@HIKACJCA@?$CGactor?9?$DOcontrol?4desired_facing_v@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "ai/actor_looking.h"

#include "actors.h"
#include "actor_definitions.h"
#include "ai_debug.h"
#include "ai_profile.h"
#include "cseries/errors.h"
#include "items/weapon_definitions.h"
#include "main/console.h"
#include "physics/collisions.h"
#include "physics/collision_usage.h"
#include "props.h"
#include "units/units.h"


/* ---------- constants */

enum
{
	_idle_look_none = 0,
	_idle_look_noncombat,
	_idle_look_guarding,
	_idle_look_searching,
	_idle_look_combat,
	NUMBER_OF_IDLE_LOOK_TYPES,
};

/* TU-local copy: no shared header owns the actor mode domain yet; partial or
 * complete copies also exist in actors.c, actor_moving.c, actor_perception.c,
 * actor_stimulus.c, actor_type_flood.c, ai_communication.c, ai_script.c,
 * encounters.c and actions.c. */
enum
{
	_actor_mode_braindead = 0,
	_actor_mode_asleep,
	_actor_mode_alert,
	_actor_mode_combat,
	NUMBER_OF_ACTOR_MODES,
};

/* TU-local copy: no shared header owns the actor combat status domain yet; a
 * complete copy also exists in action_obey.c (partial ones in action_charge.c). */
enum
{
	_actor_combat_status_none = 0,
	_actor_combat_status_wary,
	_actor_combat_status_investigate,
	_actor_combat_status_definite,
	_actor_combat_status_certain,
	_actor_combat_status_clear_los,
	_actor_combat_status_dangerous,
	_actor_combat_status_visible,
	NUMBER_OF_ACTOR_COMBAT_STATUS_LEVELS,
};

enum
{
	_idle_timer_facing = 0,
	_idle_timer_aiming,
	_idle_timer_looking,
};

/* ---------- macros */

#define ACTOR_LOOKING_DEBUG_PRINTING_ENABLED() (ai_debug.print_secondary_looking)




/* ---------- structures */

typedef char actor_looking_ai_debug_printing_offset_must_be_0x9D[
	offsetof(struct ai_debug_state, print_secondary_looking) == 0x9D ? 1 : -1];
typedef char actor_looking_actor_vehicle_index_offset_must_be_0x158[
	offsetof(struct actor_datum, input.vehicle_index) == 0x158 ? 1 : -1];
typedef char actor_looking_secondary_look_type_offset_must_be_0x544[
	offsetof(struct actor_datum, control.secondary_look_type) == 0x544 ? 1 : -1];
typedef char actor_looking_secondary_look_priority_offset_must_be_0x546[
	offsetof(struct actor_datum, control.secondary_look_priority) == 0x546 ? 1 : -1];
typedef char actor_looking_secondary_look_timer_offset_must_be_0x548[
	offsetof(struct actor_datum, control.secondary_look_timer) == 0x548 ? 1 : -1];
typedef char actor_looking_prop_unit_index_offset_must_be_0x18[
	offsetof(struct prop_datum, unit_index) == 0x18 ? 1 : -1];
typedef char actor_looking_prop_state_offset_must_be_0x24[
	offsetof(struct prop_datum, state) == 0x24 ? 1 : -1];
typedef char actor_looking_prop_enemy_offset_must_be_0x60[
	offsetof(struct prop_datum, enemy) == 0x60 ? 1 : -1];
typedef char actor_looking_prop_dead_ticks_offset_must_be_0x76[
	offsetof(struct prop_datum, dead_ticks) == 0x76 ? 1 : -1];
typedef char actor_looking_prop_vehicle_index_offset_must_be_0x110[
	offsetof(struct prop_datum, vehicle_index) == 0x110 ? 1 : -1];
typedef char actor_looking_prop_quantized_distance_offset_must_be_0x121[
	offsetof(struct prop_datum, quantized_distance) == 0x121 ? 1 : -1];
typedef char actor_looking_prop_quantized_speed_offset_must_be_0x123[
	offsetof(struct prop_datum, quantized_speed) == 0x123 ? 1 : -1];
typedef char actor_looking_prop_dead_offset_must_be_0x127[
	offsetof(struct prop_datum, dead) == 0x127 ? 1 : -1];
typedef char actor_looking_prop_shooting_offset_must_be_0x12F[
	offsetof(struct prop_datum, shooting) == 0x12F ? 1 : -1];

/* ---------- prototypes */

static struct actor_idle_looking *actor_look_get_looking_definition(
	long actor_index);
static boolean actor_look_valid_aim_vector(
	real yaw_deviation,
	real_vector3d const *facing_vector,
	real_vector3d const *attempted_aiming_vector);
static boolean actor_look_valid_look_vector(
	real yaw_deviation,
	real *look_delta_deviations,
	real_vector3d const *aiming_vector,
	real_vector3d const *attempted_looking_vector,
	real_vector3d const *look_vector);
static boolean actor_look_idle_find_prop(
	long actor_index,
	boolean free_facing,
	boolean aim_at_prop,
	struct direction_specification *direction,
	boolean *interesting_prop);
static long actor_look_idle_timer(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	short timer_type,
	boolean interesting_direction);
static boolean actor_look_find_random_vector(
	real_point3d const *center_point,
	real_vector3d const *base_vector,
	boolean avoid_obstructions,
	real yaw_minimum,
	real yaw_maximum,
	real pitch_minimum,
	real pitch_maximum,
	real_vector3d *result_vector);
static boolean actor_look_decode_direction(
	long actor_index,
	struct direction_specification *specification,
	real_vector3d *direction);
static boolean actor_look_idle_new_major_direction(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	real_vector3d const *base_vector,
	boolean free_facing,
	boolean major_is_aiming,
	boolean minor_variation);
static void actor_look_idle_new_minor_direction(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	real_vector3d const *base_vector);

/* ---------- globals */

short const global_secondary_look_priorities[NUMBER_OF_SECONDARY_LOOK_TYPES][2] =
{
	{ _secondary_look_priority_default, _secondary_look_priority_default },
	{ _secondary_look_priority_idle_look, _secondary_look_priority_idle_look },
	{ _secondary_look_priority_idle_look, _secondary_look_priority_idle_look },
	{ _secondary_look_priority_idle_aim, _secondary_look_priority_idle_aim },
	{ _secondary_look_priority_turn_and_aim, _secondary_look_priority_idle_aim },
	{ _secondary_look_priority_aim, _secondary_look_priority_aim },
	{ _secondary_look_priority_turn_and_aim, _secondary_look_priority_aim },
	{ _secondary_look_priority_turn_and_aim, _secondary_look_priority_aim },
	{ _secondary_look_priority_aim, _secondary_look_priority_idle_aim },
	{ _secondary_look_priority_turn_and_aim, _secondary_look_priority_turn_and_aim },
	{ _secondary_look_priority_stop_and_aim, _secondary_look_priority_idle_aim },
	{ _secondary_look_priority_stop_and_aim, _secondary_look_priority_idle_aim },
	{ _secondary_look_priority_override, _secondary_look_priority_turn_and_aim },
	{ _secondary_look_priority_override, _secondary_look_priority_idle_look },
};

real const global_secondary_look_times[NUMBER_OF_SECONDARY_LOOK_TYPES] =
{
	0.0f,
	1.3f,
	0.9f,
	0.9f,
	0.9f,
	0.7f,
	0.9f,
	0.9f,
	1.2f,
	2.0f,
	2.0f,
	2.5f,
	1.5f,
	1000.0f,
};


/* ---------- code */

void actor_look_secondary_stop(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->control.secondary_look_type > 0 &&
		ACTOR_LOOKING_DEBUG_PRINTING_ENABLED())
	{
		console_printf(
			FALSE,
			"%s: look-stop",
			ai_debug_describe_actor(
				actor_index,
				NONE,
				FALSE,
				temporary,
				NUMBEROF(temporary)));
	}

	actor->control.secondary_look_priority = _secondary_look_priority_none;
	actor->control.secondary_look_type = _secondary_look_none;
	actor->control.secondary_look_timer = 0;

	return;
}

real actor_look_compute_prop_interest(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);
	real interest = 0.0f;
	real weight;
	short state = prop->state;

	if (state >= _prop_state_becoming_unacknowledged &&
		state <= _prop_state_acknowledged)
	{
		if (prop->dead)
		{
			interest = prop->dead_ticks < 210 ? 1.8f : 0.4f;
		}
		else if (prop->enemy)
		{
			interest = 2.0f;
		}
		else
		{
			interest = 1.0f;
		}
	}
	else if (prop->enemy &&
		state >= _prop_state_uninspected_orphan &&
		state <= _prop_state_inspected_orphan)
	{
		interest = 1.5f;
	}

	if (prop->unit_index == actor->input.vehicle_index ||
		prop->vehicle_index == actor->input.vehicle_index)
	{
		interest = 0.0f;
	}

	weight = prop->vehicle_index != NONE ? 1.5f : 1.0f;

	switch (prop->quantized_speed)
	{
	case 1:
		interest += weight * 0.5f;
		break;
	case 2:
		interest += weight;
		break;
	case 3:
		interest += weight * 2.0f;
		break;
	}

	if (prop->shooting)
	{
		interest += weight * 2.0f;
	}

	switch (prop->quantized_distance)
	{
	case 1:
		interest *= 0.6f;
		break;
	case 3:
		interest *= 0.4f;
		break;
	case 4:
		interest *= 0.2f;
		break;
	}

	return interest;
}

static struct actor_idle_looking *actor_look_get_looking_definition(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	long idle_look_type = actor->orders.look.idle_look_type;

	if (idle_look_type != _idle_look_guarding)
	{
		if (idle_look_type <= _idle_look_guarding || idle_look_type > _idle_look_combat)
			return &definition->looking.idle_look_noncombat;

		return &definition->looking.idle_look_search_combat;
	}

	return &definition->looking.idle_look_guard;
}

boolean actor_look_secondary(
	long actor_index,
	short type,
	short priority,
	struct direction_specification const *direction)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	boolean locked_aiming;
	real duration;
	long ticks;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\actor_looking.c",
		135,
		(type >= 0) && (type < NUMBER_OF_SECONDARY_LOOK_TYPES));

	if (actor->state.mode <= _actor_mode_asleep && type < _secondary_look_scripted)
		return FALSE;
	if (actor->control.secondary_look_type > type)
		return FALSE;

	locked_aiming = actor->orders.look.primary_priority >= _primary_priority_locked_aiming;
	if (type < _secondary_look_scripted &&
		actor->state.action == _actor_action_obey &&
		!actor->state.action_data.obey.allow_looking)
	{
		return FALSE;
	}
	if (locked_aiming && type < _secondary_look_newly_acknowledged_prop)
		return FALSE;

	if (direction->type == _direction_specification_prop)
	{
		struct prop_datum *prop = prop_try_and_get(direction->prop_index);

		if (!prop)
			return FALSE;

		if (type < _secondary_look_communicating_prop &&
			((!prop->enemy && !prop->dead) || (prop->dead && actor->state.mode >= _actor_mode_combat)))
		{
			long game_time = game_time_get();

			if (locked_aiming)
				return FALSE;
			if ((!prop->player || type < _secondary_look_newly_acknowledged_prop) &&
				prop->last_idle_look_time != NONE &&
				prop->last_idle_look_time + 20 * TICKS_PER_SECOND > game_time)
			{
				return FALSE;
			}

			prop->last_idle_look_time = game_time;
			prop->last_idle_look_interest = MAX(prop->last_idle_look_interest, prop->look_interest);
		}
	}

	duration = global_secondary_look_times[type];
	if (actor->state.mode < _actor_mode_combat || !actor->state.combat_status)
		duration *= 2.0f;

	if (definition->looking.event_look_modifier_lower_bound != 0.0f ||
		definition->looking.event_look_modifier_upper_bound != 0.0f)
	{
		real lower_bound = MAX(definition->looking.event_look_modifier_lower_bound, 0.5f);
		real upper_bound = MIN(definition->looking.event_look_modifier_upper_bound, 2.0f);

		duration *= real_seed_random_range(
			get_global_random_seed_address(),
			lower_bound,
			upper_bound);
	}

	ticks = fast_ftol(duration * TICKS_PER_SECOND);
	ticks = MIN(ticks, SHORT_MAX);

	if (priority == _secondary_look_priority_default)
	{
		boolean certain = actor->state.combat_status >= _actor_combat_status_certain;

		priority = global_secondary_look_priorities[type][certain];
	}

	if (ACTOR_LOOKING_DEBUG_PRINTING_ENABLED())
	{
		char const *type_names[] =
		{
			"none",
			"environment",
			"moving-prop",
			"impact",
			"new-prop",
			"shooting-prop",
			"comm-prop",
			"comm-direction",
			"combat-prop",
			"damage",
			"danger",
			"scripted",
		};
		char const *priority_names[NUMBER_OF_SECONDARY_LOOK_PRIORITIES] =
		{
			"none",
			"default",
			"idle-look",
			"idle-aim",
			"aim",
			"turn-and-aim",
			"stop-and-aim",
			"override",
			"override-facing",
		};

		console_printf(
			FALSE,
			"%s: look %s %s %d",
			ai_debug_describe_actor(
				actor_index,
				NONE,
				FALSE,
				temporary,
				NUMBEROF(temporary)),
			type_names[type],
			priority_names[priority],
			(short)ticks);
	}

	actor->control.secondary_look_type = type;
	actor->control.secondary_look_priority = priority;
	actor->control.secondary_look_timer = (short)ticks;
	actor->control.secondary_look_direction = *direction;

	return TRUE;
}

static boolean actor_look_valid_aim_vector(
	real yaw_deviation,
	real_vector3d const *facing_vector,
	real_vector3d const *attempted_aiming_vector)
{
	real_vector2d aiming_vector2d;
	boolean result;

	aiming_vector2d = *(real_vector2d const *)attempted_aiming_vector;
	result = FALSE;
	if (normalize2d(&aiming_vector2d) > 0.0f &&
		dot_product2d(&aiming_vector2d, (real_vector2d const *)facing_vector) > yaw_deviation)
	{
		result = TRUE;
	}

	return result;
}

static boolean actor_look_valid_look_vector(
	real yaw_deviation,
	real *look_delta_deviations,
	real_vector3d const *aiming_vector,
	real_vector3d const *attempted_looking_vector,
	real_vector3d const *look_vector)
{
	real_vector2d attempted_looking_vector2d;
	real_vector2d looking_vector2d;
	boolean result = FALSE;

	looking_vector2d = *(real_vector2d const *)look_vector;
	attempted_looking_vector2d = *(real_vector2d const *)attempted_looking_vector;

	if (normalize2d(&looking_vector2d) > 0.0f &&
		dot_product2d(&looking_vector2d, (real_vector2d const *)aiming_vector) > yaw_deviation &&
		normalize2d(&attempted_looking_vector2d) > 0.0f)
	{
		boolean side = cross_product2d(&looking_vector2d, &attempted_looking_vector2d) > 0.0f;

		if (dot_product2d(&attempted_looking_vector2d, &looking_vector2d) > look_delta_deviations[side])
			result = TRUE;
	}

	return result;
}

void actor_looking_test_validity(
	long actor_index,
	real_vector3d const *vector,
	boolean *aiming_valid,
	boolean *looking_valid)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	real look_delta_deviations[2];

	*aiming_valid = actor_look_valid_aim_vector(
		definition->looking.runtime_maximum_aiming_deviation_cosine.yaw,
		&actor->input.facing_vector,
		vector);

	if (actor->state.mode == _actor_mode_combat)
	{
		look_delta_deviations[0] = cosine(definition->looking.combat_looking_delta_angles[0]);
		look_delta_deviations[1] = cosine(definition->looking.combat_looking_delta_angles[1]);
	}
	else
	{
		look_delta_deviations[0] = cosine(definition->looking.noncombat_looking_delta_angles[0]);
		look_delta_deviations[1] = cosine(definition->looking.noncombat_looking_delta_angles[1]);
	}

	*looking_valid = actor_look_valid_look_vector(
		definition->looking.runtime_maximum_looking_deviation_cosine.yaw,
		look_delta_deviations,
		&actor->input.facing_vector,
		&actor->input.aiming_vector,
		vector);

	return;
}

static boolean actor_look_idle_find_prop(
	long actor_index,
	boolean free_facing,
	boolean aim_at_prop,
	struct direction_specification *direction,
	boolean *interesting_prop)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	long best_prop_index = NONE;
	real best_weight = 0.0f;
	struct prop_datum *best_prop = NULL;
	boolean best_interesting = FALSE;
	long game_time = game_time_get();
	real aiming_cosine = definition->looking.runtime_maximum_aiming_deviation_cosine.yaw;
	real looking_cosine = definition->looking.runtime_maximum_looking_deviation_cosine.yaw;
	real look_delta_deviations[2];
	struct prop_iterator iterator;
	struct prop_datum *prop;

	if (actor->state.mode == _actor_mode_combat)
	{
		look_delta_deviations[0] = cosine(definition->looking.combat_looking_delta_angles[0]);
		look_delta_deviations[1] = cosine(definition->looking.combat_looking_delta_angles[1]);
	}
	else
	{
		look_delta_deviations[0] = cosine(definition->looking.noncombat_looking_delta_angles[0]);
		look_delta_deviations[1] = cosine(definition->looking.noncombat_looking_delta_angles[1]);
	}

	if (actor_index == ai_debug.selected_actor_index)
		ai_debug_idle_look_clear(actor_index);

	prop_iterator_new(&iterator, actor_index);
	while (prop = prop_iterator_next(&iterator))
	{
		real weight = 0.0f;
		boolean interesting = FALSE;

		if (prop->state >= _prop_state_becoming_unacknowledged &&
			prop->state <= _prop_state_acknowledged &&
			prop->visibility)
		{
			if (prop->look_interest > 0.0f)
			{
				real recency;
				real novelty;

				if (prop->last_idle_look_time == NONE)
				{
					recency = 1.0f;
				}
				else
				{
					recency = ((real)game_time - prop->last_idle_look_time) / (20.0f * TICKS_PER_SECOND) - 1.0f;
				}

				novelty = (prop->look_interest - prop->last_idle_look_interest) / prop->look_interest + recency;
				if (novelty > 1.0f)
					novelty = 1.0f;
				weight = novelty * prop->look_interest;
				interesting = prop->look_interest > prop->last_idle_look_interest;
			}
		}
		else
		{
			prop->last_idle_look_interest = 0.0f;
		}

		if (actor_index == ai_debug.selected_actor_index)
			ai_debug_idle_look_addprop(iterator.index, weight);

		if (weight > 0.0f)
		{
			boolean valid;

			if (aim_at_prop)
			{
				if (free_facing && interesting)
				{
					valid = TRUE;
				}
				else
				{
					valid = actor_look_valid_aim_vector(
						aiming_cosine,
						&actor->control.desired_facing_vector,
						&prop->actor_to_prop);
				}
			}
			else
			{
				valid = actor_look_valid_look_vector(
					looking_cosine,
					look_delta_deviations,
					&actor->control.desired_facing_vector,
					&actor->control.desired_aiming_vector,
					&prop->actor_to_prop);
			}

			if (valid && weight > best_weight)
			{
				best_prop = prop;
				best_prop_index = iterator.index;
				best_weight = weight;
				best_interesting = interesting;
			}
		}
	}

	if (best_prop_index != NONE)
	{
		best_prop->last_idle_look_time = game_time;
		best_prop->last_idle_look_interest = best_prop->look_interest;
		direction->type = _direction_specification_prop;
		direction->prop_index = best_prop_index;
		*interesting_prop = best_interesting;

		return TRUE;
	}

	return FALSE;
}

static long actor_look_idle_timer(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	short timer_type,
	boolean interesting_direction)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	struct weapon_definition *weapon_definition = actor_get_weapon_definition(actor_index);
	real time_lower_bound;
	real time_upper_bound;
	real time;
	long ticks;

	switch (timer_type)
	{
	case _idle_timer_facing:
		time_lower_bound = looking_definition->idle_facing_time_lower_bound;
		time_upper_bound = looking_definition->idle_facing_time_upper_bound;
		break;
	case _idle_timer_aiming:
		time_lower_bound = looking_definition->idle_aim_time_lower_bound;
		time_upper_bound = looking_definition->idle_aim_time_upper_bound;
		break;
	case _idle_timer_looking:
		time_lower_bound = looking_definition->idle_look_time_lower_bound;
		time_upper_bound = looking_definition->idle_look_time_upper_bound;
		break;
	/* time_lower_bound and time_upper_bound are left unassigned only by this default arm. Not reached unassigned: the
	 * arm's assertion failure calls system_exit, which does not return in January
	 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
	 * Source-policy approval pending (2026-09-27 audit). */
	default:
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_looking.c",
			986,
			FALSE,
			NULL);
		break;
	}

	if (time_lower_bound <= 0.0f && time_upper_bound <= 0.0f)
	{
		char const *timer_names[] =
		{
			"facing",
			"aiming",
			"looking",
		};

		error(
			_error_silent,
			"%s: logic error in actor looking: %s mode should be disabled (%.1f %.1f)",
			ai_debug_describe_actor(
				actor_index,
				NONE,
				TRUE,
				temporary,
				NUMBEROF(temporary)),
			timer_names[timer_type],
			time_lower_bound,
			time_upper_bound);
		time = 0.5f;
	}
	else
	{
		time = real_seed_random_range(
			get_global_random_seed_address(),
			time_lower_bound,
			time_upper_bound);
	}

	if (weapon_definition && weapon_definition->weapon.ai_idle_look_time_modifier > 0.0f)
		time *= weapon_definition->weapon.ai_idle_look_time_modifier;
	if (interesting_direction)
		time *= 1.5f;

	ticks = fast_ftol(time * TICKS_PER_SECOND);
	if (ticks <= 1)
		ticks = 1;

	return ticks;
}

static boolean actor_look_find_random_vector(
	real_point3d const *center_point,
	real_vector3d const *base_vector,
	boolean avoid_obstructions,
	real yaw_minimum,
	real yaw_maximum,
	real pitch_minimum,
	real pitch_maximum,
	real_vector3d *result_vector)
{
	real_vector3d pitch_axis;
	real_vector3d direction;
	short attempt_count;
	boolean result = FALSE;

	pitch_axis.i = -base_vector->j;
	pitch_axis.j = base_vector->i;
	pitch_axis.k = 0.0f;
	if (normalize3d(&pitch_axis) == 0.0f)
		pitch_axis = *global_left3d;

	for (attempt_count = 0; attempt_count < 10; attempt_count++)
	{
		real yaw = real_seed_random_range(
			get_global_random_seed_address(),
			yaw_minimum,
			yaw_maximum);
		real pitch = real_seed_random_range(
			get_global_random_seed_address(),
			pitch_minimum,
			pitch_maximum);
		boolean unobstructed = TRUE;

		direction = *base_vector;
		rotate_vector_about_axis(
			&direction,
			&pitch_axis,
			sine(pitch),
			cosine(pitch));
		rotate_vector_about_axis(
			&direction,
			global_up3d,
			sine(yaw),
			cosine(yaw));

		if (avoid_obstructions)
		{
			real_vector3d collision_vector;
			struct collision_result collision;

			ai_profile.meters[_ai_meter_collisions].accumulator++;
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_looking.c",
				1066,
				global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
			global_current_collision_users[global_current_collision_user_depth++] =
				_collision_user_ai_look;
			scale_vector3d(&direction, 3.0f, &collision_vector);
			unobstructed = !collision_test_vector(
				FLAG(_collision_test_front_facing_surfaces_bit) |
					FLAG(_collision_test_structure_bit),
				center_point,
				&collision_vector,
				NONE,
				&collision);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_looking.c",
				1070,
				global_current_collision_user_depth > 1);
			--global_current_collision_user_depth;
		}

		if (unobstructed)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\actor_looking.c",
				1077,
				result_vector);
			normalize3d(&direction);
			*result_vector = direction;
			result = TRUE;
			break;
		}
	}

	return result;
}

/* INFERRED: the __inline specifier is not attested in surviving source. Its
 * sibling valid_real_normal3d is __inline in real_math.h, and four January
 * objects reference this function, the shape of a shared inline whose single
 * folded copy the splitter gave to this object. The call still stays out of
 * line. Admitted by owner ruling (2026-09-20); on its own it is byte-inert in
 * actor_look_update, and it changes this definition's COMDAT selection from
 * no-duplicates to select-any, which csplit does not record for any function
 * (it writes no-duplicates for all 8,223), so ownership is unchanged. */
__inline boolean valid_real_normal2d(
	real_vector2d const *normal)
{
	return valid_realcmp(magnitude_squared2d(normal), 1.0f);
}

static boolean actor_look_decode_direction(
	long actor_index,
	struct direction_specification *specification,
	real_vector3d *direction)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean result = FALSE;

	match_assert("c:\\halo\\SOURCE\\ai\\actor_looking.c", 349, specification);
	match_assert("c:\\halo\\SOURCE\\ai\\actor_looking.c", 350, direction);

	switch (specification->type)
	{
	case _direction_specification_movement:
		if (actor->control.moving)
		{
			*direction = actor->control.moving_towards_vector;
			result = normalize3d(direction) > 0.0f;
		}
		break;

	case _direction_specification_target:
		if (actor->control.fire_state == _actor_fire_state_bursting)
		{
			*direction = actor->control.burst_aim_vector;
			result = TRUE;
		}
		else if (actor->control.aiming_at_fire_target)
		{
			*direction = actor->control.current_fire_target_aim_vector;
			result = TRUE;
		}
		else if (actor->target.target_prop_index != NONE)
		{
			struct prop_datum *prop = prop_get(actor->target.target_prop_index);

			vector_from_points3d(&actor->input.position.head_position, &prop->center_of_mass, direction);
			result = normalize3d(direction) > 0.0f;
		}
		break;

	case _direction_specification_prop:
		{
			struct prop_datum *prop = prop_try_and_get(specification->prop_index);

			if (prop)
			{
				vector_from_points3d(&actor->input.position.head_position, &prop->head_position, direction);
				result = normalize3d(direction) > 0.0f;
			}
		}
		break;

	case _direction_specification_point:
		vector_from_points3d(&actor->input.position.head_position, &specification->point, direction);
		result = normalize3d(direction) > 0.0f;
		break;

	case _direction_specification_vector:
		*direction = specification->vector;
		result = TRUE;
		break;

	case _direction_specification_danger:
		if (actor->danger_zone.danger_type > _actor_danger_zone_none)
		{
			vector_from_points3d(&actor->input.position.head_position, &actor->danger_zone.position, direction);
			result = normalize3d(direction) > 0.0f;
		}
		break;

	case _direction_specification_object:
		{
			struct object_datum *object = object_try_and_get(specification->object_index);

			if (object)
			{
				real_point3d look_point;

				if (TEST_FLAG(_object_mask_unit, object->object.type))
				{
					unit_get_head_position(specification->object_index, &look_point);
				}
				else
				{
					object_get_origin(specification->object_index, &look_point);
				}

				vector_from_points3d(&actor->input.position.head_position, &look_point, direction);
				result = normalize3d(direction) > 0.0f;
			}
		}
		break;

	default:
		match_vassert("c:\\halo\\SOURCE\\ai\\actor_looking.c", 449, FALSE, NULL);
		break;
	}

	if (result)
	{
		real magnitude_squared = magnitude_squared3d(direction);

		if (!valid_realcmp(magnitude_squared, 1.0f))
		{
			real magnitude = square_root(magnitude_squared);

			switch (specification->type)
			{
			case _direction_specification_movement:
				/* BUG (preserved for exact matching): January loads the point's
				 * z field twice (actor + 0x514). A corrected diagnostic should
				 * print the y field as its second point component.
				 */
				sprintf(temporary, "denormalized %f: %smoving (p%f %f %f) (v%f %f %f)",
					magnitude, actor->control.moving ? "" : "not ",
					actor->control.moving_towards_point.x,
					actor->control.moving_towards_point.z,
					actor->control.moving_towards_point.z,
					actor->control.moving_towards_vector.i,
					actor->control.moving_towards_vector.j,
					actor->control.moving_towards_vector.k);
				break;

			case _direction_specification_target:
				if (actor->control.fire_state == _actor_fire_state_bursting)
				{
					sprintf(temporary, "denormalized %f: target type %d (burst aim vector %f %f %f)",
						magnitude, actor->control.current_fire_target_type,
						actor->control.burst_aim_vector.i,
						actor->control.burst_aim_vector.j,
						actor->control.burst_aim_vector.k);
				}
				else if (actor->control.aiming_at_fire_target)
				{
					sprintf(temporary, "denormalized %f: target type %d (fire target aim vector %f %f %f)",
						magnitude, actor->control.current_fire_target_type,
						actor->control.current_fire_target_aim_vector.i,
						actor->control.current_fire_target_aim_vector.j,
						actor->control.current_fire_target_aim_vector.k);
				}
				else if (actor->target.target_prop_index != NONE)
				{
					struct prop_datum *prop = prop_get(actor->target.target_prop_index);

					sprintf(temporary, "denormalized %f: target (prop 0x%08X center %f %f %f)",
						magnitude, actor->target.target_prop_index,
						prop->center_of_mass.x, prop->center_of_mass.y, prop->center_of_mass.z);
				}
				else
				{
					sprintf(temporary, "denormalized %f: target (none)", magnitude);
				}
				break;

			case _direction_specification_prop:
				{
					struct prop_datum *prop = prop_try_and_get(specification->prop_index);

					sprintf(temporary, "denormalized %f: prop 0x%08X%s (actor 0x%08X / us 0x%08X)",
						magnitude, specification->prop_index, !prop ? " (invalid)" : "",
						!prop ? 0 : prop->owner_actor_index, actor_index);
				}
				break;

			case _direction_specification_point:
				sprintf(temporary, "denormalized %f: point (%f %f %f)", magnitude,
					specification->point.x, specification->point.y, specification->point.z);
				break;

			case _direction_specification_vector:
				sprintf(temporary, "denormalized %f: vector (%f %f %f)", magnitude,
					specification->vector.i, specification->vector.j, specification->vector.k);
				break;

			case _direction_specification_danger:
				if (actor->danger_zone.danger_type > _actor_danger_zone_none)
				{
					sprintf(temporary, "denormalized %f: danger (%f %f %f)", magnitude,
						actor->danger_zone.position.x, actor->danger_zone.position.y, actor->danger_zone.position.z);
				}
				else
				{
					sprintf(temporary, "denormalized %f: danger (none)", magnitude);
				}
				break;

			case _direction_specification_object:
				sprintf(temporary, "denormalized %f: object (0x%08X)", magnitude, specification->object_index);
				break;

			default:
				sprintf(temporary, "denormalized %f: <error> (type %d)", magnitude, specification->type);
				break;
			}

			display_assert(temporary, "c:\\halo\\SOURCE\\ai\\actor_looking.c", 526, FALSE);
		}

		match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 529, direction);
	}

	return result;
}

static boolean actor_look_idle_new_major_direction(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	real_vector3d const *base_vector,
	boolean free_facing,
	boolean major_is_aiming,
	boolean minor_variation)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	boolean interesting_direction = FALSE;
	boolean found = FALSE;

	actor->control.idle_major_active = FALSE;

	if (!minor_variation)
	{
		found = actor_look_idle_find_prop(
			actor_index,
			free_facing,
			major_is_aiming,
			&actor->control.idle_major_direction,
			&interesting_direction);
	}

	if (!found)
	{
		real_vector3d vector = *base_vector;
		real yaw_bound;
		real pitch_bound;
		real pitch_minimum;

		if (major_is_aiming)
		{
			yaw_bound = free_facing ?
				_pi :
				MIN(definition->looking.maximum_aiming_deviation.yaw, definition->looking.idle_aiming_deviation.yaw);
			pitch_bound = MIN(definition->looking.maximum_aiming_deviation.pitch, definition->looking.idle_aiming_deviation.pitch);

			vector.k = 0.0f;
			if (normalize3d(&vector) == 0.0f)
				vector = *global_forward3d;
		}
		else
		{
			yaw_bound = MIN(definition->looking.maximum_looking_deviation.yaw, definition->looking.idle_looking_deviation.yaw);
			pitch_bound = MIN(definition->looking.maximum_looking_deviation.pitch, definition->looking.idle_looking_deviation.pitch);
		}

		pitch_minimum = -pitch_bound;
		if (actor->input.vehicle_gunner)
			pitch_minimum *= 0.5f;

		actor->control.idle_major_direction.type = _direction_specification_vector;
		found = actor_look_find_random_vector(
			&actor->input.position.head_position,
			&vector,
			TRUE,
			-yaw_bound,
			yaw_bound,
			pitch_minimum,
			pitch_bound,
			&actor->control.idle_major_direction.vector);
	}

	if (found)
	{
		actor->control.idle_major_timer = actor_look_idle_timer(
			actor_index,
			looking_definition,
			major_is_aiming ? _idle_timer_aiming : _idle_timer_looking,
			interesting_direction);
		if (actor->control.idle_major_timer)
		{
			actor->control.idle_major_active = TRUE;
			actor->control.idle_major_is_aiming = major_is_aiming;
		}
	}

	return interesting_direction;
}

static void actor_look_idle_new_minor_direction(
	long actor_index,
	struct actor_idle_looking *looking_definition,
	real_vector3d const *base_vector)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	boolean interesting_direction = FALSE;
	boolean found;

	actor->control.idle_minor_active = FALSE;

	found = actor_look_idle_find_prop(
		actor_index,
		FALSE,
		FALSE,
		&actor->control.idle_minor_direction,
		&interesting_direction);
	if (!found)
	{
		real yaw_bound = MIN(definition->looking.maximum_looking_deviation.yaw, definition->looking.idle_looking_deviation.yaw);
		real pitch_bound = MIN(definition->looking.maximum_looking_deviation.pitch, definition->looking.idle_looking_deviation.pitch);
		real const *delta_angles = actor->state.mode == _actor_mode_combat ?
			definition->looking.combat_looking_delta_angles :
			definition->looking.noncombat_looking_delta_angles;
		real yaw_minimum = MAX(-yaw_bound, -delta_angles[0]);
		real yaw_maximum = MIN(yaw_bound, delta_angles[1]);
		real_vector3d vector;

		found = actor_look_find_random_vector(
			&actor->input.position.head_position,
			base_vector,
			FALSE,
			yaw_minimum,
			yaw_maximum,
			-pitch_bound,
			pitch_bound,
			&vector);
		if (found)
		{
			actor->control.idle_minor_direction.type = _direction_specification_vector;
			actor->control.idle_minor_direction.vector = vector;
			interesting_direction = FALSE;
		}
	}

	if (found)
	{
		actor->control.idle_minor_timer = actor_look_idle_timer(
			actor_index,
			looking_definition,
			_idle_timer_looking,
			interesting_direction);
		if (actor->control.idle_minor_timer)
			actor->control.idle_minor_active = TRUE;
	}

	return;
}

void actor_look_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	boolean aiming_at_target = FALSE;
	boolean looking_enabled = TRUE;
	boolean can_look;

	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1103, &actor->control.desired_facing_vector);
	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1104, &actor->control.desired_aiming_vector);
	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1105, &actor->control.desired_looking_vector);

	if (actor->output.movement_type == _actor_movement_type_asleep)
	{
		can_look = FALSE;
		looking_enabled = FALSE;
	}
	else if (actor->input.vehicle_gunner)
	{
		can_look = TRUE;
	}
	else if (actor->output.movement_type == _actor_movement_type_alert ||
		actor->output.movement_type == _actor_movement_type_combat)
	{
		can_look = actor_get_weapon(actor_index) != NONE;
	}
	else
	{
		can_look = FALSE;
	}

	if (!looking_enabled)
	{
		actor->control.desired_aiming_vector = actor->control.desired_facing_vector;
		actor->control.desired_looking_vector = actor->control.desired_facing_vector;
		goto update_facing;
	}

	{
		boolean looking_free;
		boolean facing_free;
		boolean facing_optional;
		boolean aiming_free;
		boolean facing_locked;
		boolean looking_locked;
		real aiming_cosine;
		real looking_cosine;
		real look_delta_deviations[2];
		short primary_priority;
		short secondary_priority;
		real_vector3d primary_vector;
		real_vector3d secondary_vector;
		boolean burst_direction = FALSE;

		facing_free = actor->control.free_facing_vector;
		aiming_free = TRUE;
		looking_free = can_look;
		facing_optional = actor->control.optional_facing_vector;
		facing_locked = FALSE;
		looking_locked = FALSE;
		aiming_cosine = definition->looking.runtime_maximum_aiming_deviation_cosine.yaw;
		looking_cosine = definition->looking.runtime_maximum_looking_deviation_cosine.yaw;

		if (actor->state.mode == _actor_mode_combat)
		{
			look_delta_deviations[0] = cosine(definition->looking.combat_looking_delta_angles[0]);
			look_delta_deviations[1] = cosine(definition->looking.combat_looking_delta_angles[1]);
		}
		else
		{
			look_delta_deviations[0] = cosine(definition->looking.noncombat_looking_delta_angles[0]);
			look_delta_deviations[1] = cosine(definition->looking.noncombat_looking_delta_angles[1]);
		}

		if (actor_combat_currently_firing_burst(actor_index) &&
			!actor->orders.combat.abort_burst)
		{
			struct direction_specification burst_specification;

			burst_specification.type = _direction_specification_target;
			if (actor_look_decode_direction(actor_index, &burst_specification, &primary_vector))
			{
				primary_priority = _primary_priority_locked_aiming;
				aiming_at_target = TRUE;
				burst_direction = TRUE;
			}
		}

		if (!burst_direction)
		{
			primary_priority = actor->orders.look.primary_priority;
			if (primary_priority != _primary_priority_none &&
				primary_priority != _primary_priority_face_360)
			{
				if (actor_look_decode_direction(actor_index, &actor->orders.look.primary_direction, &primary_vector))
				{
					aiming_at_target = actor->orders.look.primary_direction.type == _direction_specification_target;
				}
				else
				{
					primary_priority = _primary_priority_none;
				}
			}
		}

		secondary_priority = _secondary_look_priority_none;
		if (actor->control.secondary_look_type >= _secondary_look_none &&
			actor->control.secondary_look_timer > 0 &&
			actor_look_decode_direction(actor_index, &actor->control.secondary_look_direction, &secondary_vector))
		{
			secondary_priority = actor->control.secondary_look_priority;
		}

		if (actor->control.moving &&
			actor_action_class(actor_index) == _action_class_transitory)
		{
			secondary_priority = MIN(secondary_priority, _secondary_look_priority_turn_and_aim);
		}

		if (primary_priority != _primary_priority_none &&
			primary_priority != _primary_priority_face_360)
		{
			match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1226, &primary_vector);
		}

		if (secondary_priority != _secondary_look_priority_none)
		{
			match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1230, &secondary_vector);
		}

		if (actor->control.secondary_look_timer > 0 &&
			--actor->control.secondary_look_timer == 0)
		{
			if (ACTOR_LOOKING_DEBUG_PRINTING_ENABLED())
			{
				console_printf(
					FALSE,
					"%s: look timer expire",
					ai_debug_describe_actor(
						actor_index,
						NONE,
						FALSE,
						temporary,
						NUMBEROF(temporary)));
			}

			actor->control.secondary_look_type = _secondary_look_none;
			actor->control.secondary_look_priority = _secondary_look_priority_none;
		}

		actor->control.aiming_away_from_primary = FALSE;

		if (primary_priority >= _primary_priority_opportunity_aiming)
		{
			boolean primary_accepted = FALSE;

			if (primary_priority >= _primary_priority_aiming &&
				(facing_free || actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &primary_vector)))
			{
				primary_accepted = TRUE;
			}
			else if (primary_priority >= _primary_priority_facing && facing_optional)
			{
				facing_optional = FALSE;
				facing_free = TRUE;
				primary_accepted = TRUE;
			}

			if (primary_accepted)
			{
				actor->control.desired_aiming_vector = primary_vector;
				aiming_free = FALSE;
				looking_free = can_look;

				if (primary_priority >= _primary_priority_locked_aiming)
				{
					looking_locked = TRUE;
					if (can_look)
						actor->control.desired_looking_vector = primary_vector;
				}
			}

			if (primary_priority == _primary_priority_opportunity_aiming)
				primary_priority = facing_free ? _primary_priority_aiming : _primary_priority_none;

			if (facing_free)
			{
				facing_free = FALSE;
				facing_optional = FALSE;
				actor->control.desired_facing_vector = primary_vector;
				actor->control.face_exactly |= (primary_priority == _primary_priority_exact_facing);
			}

			facing_locked = (!actor->control.free_facing_vector && !actor->control.optional_facing_vector) ||
				primary_priority >= _primary_priority_locked_facing;
		}

		switch (secondary_priority)
		{
		case _secondary_look_priority_idle_look:
		case _secondary_look_priority_idle_aim:
		case _secondary_look_priority_aim:
		case _secondary_look_priority_turn_and_aim:
		case _secondary_look_priority_stop_and_aim:
			{
				boolean secondary_aim_valid = actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &secondary_vector);

				if (!facing_locked && !looking_locked &&
					((secondary_priority >= _secondary_look_priority_stop_and_aim && actor_move_force_stop(actor_index)) ||
					(secondary_priority >= _secondary_look_priority_turn_and_aim && (facing_optional || actor->control.free_facing_vector)) ||
					(secondary_priority >= _secondary_look_priority_aim && !secondary_aim_valid && facing_free && aiming_free)))
				{
					if (!actor->control.face_exactly || !secondary_aim_valid)
					{
						actor->control.desired_facing_vector = secondary_vector;
						actor->control.face_exactly = FALSE;
					}

					actor->control.desired_aiming_vector = secondary_vector;
					actor->control.desired_looking_vector = secondary_vector;
					looking_free = can_look;
					facing_free = FALSE;
					actor->control.aiming_away_from_primary = TRUE;
					looking_locked = FALSE;
					aiming_free = FALSE;
				}
				else if (!looking_locked && secondary_aim_valid &&
					(secondary_priority >= _secondary_look_priority_turn_and_aim ||
					(secondary_priority >= _secondary_look_priority_idle_aim && aiming_free)))
				{
					actor->control.desired_aiming_vector = secondary_vector;
					actor->control.desired_looking_vector = secondary_vector;
					looking_free = can_look;
					actor->control.aiming_away_from_primary = TRUE;
					looking_locked = FALSE;
					aiming_free = FALSE;
				}
				else if (can_look &&
					actor_look_valid_look_vector(looking_cosine, look_delta_deviations, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector, &secondary_vector))
				{
					actor->control.desired_looking_vector = secondary_vector;
					looking_free = FALSE;
				}
				else if (aiming_free && secondary_aim_valid)
				{
					actor->control.desired_aiming_vector = secondary_vector;
					actor->control.desired_looking_vector = secondary_vector;
					looking_free = TRUE;
					aiming_free = FALSE;
				}
			}
			break;

		case _secondary_look_priority_override:
		case _secondary_look_priority_override_exact_facing:
			{
				boolean succeeded = FALSE;
				boolean change_facing = secondary_priority == _secondary_look_priority_override_exact_facing;
				boolean face_exactly = secondary_priority == _secondary_look_priority_override_exact_facing;

				if (actor->control.free_facing_vector)
				{
					change_facing = TRUE;
					succeeded = TRUE;
				}
				else if (actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &secondary_vector))
				{
					succeeded = TRUE;
				}
				else if (actor_move_force_stop(actor_index))
				{
					face_exactly = TRUE;
					change_facing = TRUE;
					succeeded = TRUE;
				}

				if (succeeded)
				{
					if (change_facing)
					{
						actor->control.desired_facing_vector = secondary_vector;
						actor->control.face_exactly = face_exactly;
					}

					actor->control.desired_aiming_vector = secondary_vector;
					actor->control.desired_looking_vector = secondary_vector;
					looking_free = can_look;
					facing_free = FALSE;
					actor->control.aiming_away_from_primary = TRUE;
					looking_locked = FALSE;
					aiming_free = FALSE;
				}
			}
			break;
		}

		if (primary_priority == _primary_priority_opportunity_aiming &&
			aiming_free &&
			actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &primary_vector))
		{
			actor->control.desired_aiming_vector = primary_vector;
			if (looking_free)
				actor->control.desired_looking_vector = primary_vector;
			actor->control.aiming_away_from_primary = FALSE;
			aiming_free = FALSE;
		}

		{
			struct actor_idle_looking *looking_definition = actor_look_get_looking_definition(actor_index);
			boolean idle_facing = looking_definition->idle_facing_time_upper_bound > 0.0f;
			boolean idle_aiming = looking_definition->idle_aim_time_upper_bound > 0.0f;
			boolean idle_looking = looking_definition->idle_look_time_upper_bound > 0.0f;

			if (actor->orders.look.idle_look_type > _idle_look_none &&
				!looking_locked &&
				(aiming_free || looking_free) &&
				(idle_facing || idle_aiming || idle_looking))
			{
				boolean new_major = FALSE;
				boolean aiming_variation = FALSE;
				boolean major_valid = FALSE;
				boolean free_facing = idle_facing && facing_free &&
					primary_priority == _primary_priority_face_360 &&
					actor->control.idle_facing_timer == 0;
				real_vector3d idle_major_vector;

				if (actor->control.idle_facing_timer > 0)
					actor->control.idle_facing_timer--;

				if (actor->control.idle_major_active &&
					actor->control.idle_major_is_aiming &&
					!aiming_free)
				{
					actor->control.idle_major_active = TRUE;
					actor->control.idle_major_timer = actor_look_idle_timer(actor_index, looking_definition, _idle_timer_looking, TRUE);
					actor->control.idle_major_direction.type = _direction_specification_vector;
					actor->control.idle_major_direction.vector = actor->control.desired_aiming_vector;
				}

				if (!actor->control.idle_major_active || !actor->control.idle_major_timer)
				{
					boolean major_is_aiming = TRUE;
					boolean major_possible = aiming_free && idle_aiming;

					if (!major_possible)
					{
						major_is_aiming = FALSE;
						major_possible = looking_free && idle_looking;
					}

					if (major_possible)
					{
						actor->control.idle_major_direction_is_interesting = actor_look_idle_new_major_direction(
							actor_index,
							looking_definition,
							major_is_aiming ? &actor->control.desired_facing_vector : &actor->control.desired_aiming_vector,
							free_facing,
							major_is_aiming,
							major_is_aiming && looking_free && idle_looking);
						match_assert(
							"c:\\halo\\SOURCE\\ai\\actor_looking.c",
							1501,
							!actor->control.idle_major_active || (actor->control.idle_major_timer > 0));
						new_major = TRUE;
					}
				}

				if (actor->control.idle_major_active)
				{
					actor->control.idle_major_timer--;
					if (actor_look_decode_direction(actor_index, &actor->control.idle_major_direction, &idle_major_vector))
					{
						match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1512, &idle_major_vector);

						if (aiming_free)
						{
							if (facing_free && idle_facing && actor->state.flying)
							{
								free_facing = TRUE;
								aiming_variation = TRUE;
							}

							if (free_facing)
							{
								actor->control.desired_facing_vector = idle_major_vector;
								actor->control.desired_aiming_vector = idle_major_vector;
								actor->control.aiming_away_from_primary = TRUE;
								major_valid = TRUE;
							}
							else if (actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &idle_major_vector))
							{
								actor->control.desired_aiming_vector = idle_major_vector;
								actor->control.aiming_away_from_primary = TRUE;
								major_valid = TRUE;
							}
						}
						else if (actor_look_valid_look_vector(looking_cosine, look_delta_deviations, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector, &idle_major_vector))
						{
							actor->control.desired_looking_vector = idle_major_vector;
							major_valid = TRUE;
						}
					}
				}

				if (major_valid)
				{
					if (new_major && idle_looking)
					{
						actor->control.idle_minor_active = TRUE;
						actor->control.idle_minor_timer = actor_look_idle_timer(
							actor_index,
							looking_definition,
							_idle_timer_looking,
							actor->control.idle_major_direction_is_interesting);
						actor->control.idle_minor_direction = actor->control.idle_major_direction;
						if (free_facing)
						{
							actor->control.idle_facing_timer = actor_look_idle_timer(
								actor_index,
								looking_definition,
								_idle_timer_facing,
								actor->control.idle_major_direction_is_interesting);
						}
					}
				}
				else
				{
					actor->control.idle_major_active = FALSE;
					idle_major_vector = actor->control.desired_aiming_vector;
				}

				if (aiming_free &&
					((looking_free && idle_looking) || (aiming_variation && idle_aiming)))
				{
					if (!actor->control.idle_minor_timer)
					{
						actor_look_idle_new_minor_direction(actor_index, looking_definition, &idle_major_vector);
						match_assert(
							"c:\\halo\\SOURCE\\ai\\actor_looking.c",
							1581,
							actor->control.idle_minor_timer > 0);
					}

					actor->control.idle_minor_timer--;
					if (actor->control.idle_minor_active)
					{
						real_vector3d idle_minor_vector;

						if (actor_look_decode_direction(actor_index, &actor->control.idle_minor_direction, &idle_minor_vector) &&
							(aiming_variation ?
								actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &idle_minor_vector) :
								actor_look_valid_look_vector(looking_cosine, look_delta_deviations, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector, &idle_minor_vector)))
						{
							match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1605, &idle_minor_vector);

							if (aiming_variation)
								actor->control.desired_aiming_vector = idle_minor_vector;
							actor->control.desired_looking_vector = idle_minor_vector;
						}
						else
						{
							actor->control.idle_minor_active = FALSE;
						}
					}
				}
				else
				{
					actor->control.idle_minor_active = FALSE;
				}
			}
			else
			{
				actor->control.idle_major_active = FALSE;
				actor->control.idle_major_direction_is_interesting = FALSE;
				actor->control.idle_minor_active = FALSE;
			}
		}

		if (!actor->control.moving &&
			!actor->control.moving_forced_by_aiming &&
			!unit_is_busy(actor->meta.unit_index) &&
			actor->input.vehicle_index == NONE)
		{
			if ((actor_look_valid_aim_vector(aiming_cosine, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector) &&
				!actor_look_valid_aim_vector(aiming_cosine, &actor->input.facing_vector, &actor->control.desired_aiming_vector)) ||
				(can_look &&
				actor_look_valid_look_vector(looking_cosine, look_delta_deviations, &actor->control.desired_facing_vector, &actor->control.desired_aiming_vector, &actor->control.desired_looking_vector) &&
				!actor_look_valid_look_vector(looking_cosine, look_delta_deviations, &actor->input.facing_vector, &actor->control.desired_aiming_vector, &actor->control.desired_looking_vector)))
			{
				actor->control.face_exactly = TRUE;
			}
		}

		if (!can_look)
			actor->control.desired_looking_vector = actor->control.desired_aiming_vector;
	}

update_facing:
	if (!actor->state.flying)
	{
		if (!realcmp(actor->control.desired_facing_vector.k, 0.0f))
		{
			actor->control.desired_facing_vector.k = 0.0f;
			if (normalize2d((real_vector2d *)&actor->control.desired_facing_vector) == 0.0f)
				actor->control.desired_facing_vector = actor->input.facing_vector;
		}

		match_vassert(
			"c:\\halo\\SOURCE\\ai\\actor_looking.c",
			1673,
			valid_real_normal2d((real_vector2d *)&actor->output.facing_vector),
			csprintf(
				temporary,
				"%s: assert_valid_real_normal2d(%f, %f)",
				"(real_vector2d *) &actor->output.facing_vector",
				actor->output.facing_vector.i,
				actor->output.facing_vector.j));
	}

	if (actor->control.desire_stationary_facing)
	{
		if (!actor->control.fixed_stationary_facing)
		{
			if (!actor->control.moving &&
				dot_product3d(&actor->input.aiming_vector, &actor->control.desired_aiming_vector) > 0.9f)
			{
				actor->control.fixed_stationary_facing = TRUE;
				actor->control.fixed_stationary_facing_vector = actor->control.desired_facing_vector;
			}
		}
		else if (definition->defensive.stationary_facing_angle > 0.0f)
		{
			real stationary_cosine = cosine(definition->defensive.stationary_facing_angle);
			boolean valid;

			if (actor->state.flying)
			{
				valid = dot_product3d(&actor->control.desired_facing_vector, &actor->control.fixed_stationary_facing_vector) > stationary_cosine &&
					dot_product3d(&actor->control.desired_aiming_vector, &actor->control.fixed_stationary_facing_vector) > stationary_cosine;
			}
			else
			{
				real_vector2d facing2d;
				real_vector2d aiming2d;
				real_vector2d fixed2d;

				facing2d.i = actor->control.desired_facing_vector.i;
				facing2d.j = actor->control.desired_facing_vector.j;
				aiming2d.i = actor->control.desired_aiming_vector.i;
				aiming2d.j = actor->control.desired_aiming_vector.j;
				fixed2d.i = actor->control.fixed_stationary_facing_vector.i;
				fixed2d.j = actor->control.fixed_stationary_facing_vector.j;

				valid = normalize2d(&facing2d) != 0.0f &&
					normalize2d(&aiming2d) != 0.0f &&
					normalize2d(&fixed2d) != 0.0f &&
					dot_product2d(&fixed2d, &facing2d) > stationary_cosine &&
					dot_product2d(&aiming2d, &fixed2d) > stationary_cosine;
			}

			if (!valid)
			{
				actor->control.fixed_stationary_facing = FALSE;
				actor_stimulus_abandon_stationary_facing(actor_index);
			}
		}
	}
	else
	{
		actor->control.fixed_stationary_facing = FALSE;
	}

	actor->output.facing_vector = actor->control.desired_facing_vector;
	actor->output.aiming_vector = actor->control.desired_aiming_vector;
	actor->output.looking_vector = actor->control.desired_looking_vector;
	actor_unit_control_exact_facing(actor_index, actor->control.face_exactly);

	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1736, &actor->output.facing_vector);
	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1737, &actor->output.aiming_vector);
	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\ai\\actor_looking.c", 1738, &actor->output.looking_vector);

	/* Two separate tests rather than one disjunction: HCEX's line table puts
	 * them on separate source lines, 1740 and 1745. Admitted by owner ruling
	 * (2026-09-20). */
	if (aiming_at_target)
	{
		actor->output.aiming_speed = _unit_aiming_speed_alert;
	}
	else if (actor->orders.look.idle_look_type == _idle_look_combat)
	{
		actor->output.aiming_speed = _unit_aiming_speed_alert;
	}
	else
	{
		switch (actor->control.secondary_look_type)
		{
		case _secondary_look_weapon_impact:
		case _secondary_look_detonation:
		case _secondary_look_combat_stimulus_prop:
		case _secondary_look_damage:
		case _secondary_look_dangerous_object:
			actor->output.aiming_speed = _unit_aiming_speed_alert;
			break;
		default:
			actor->output.aiming_speed = _unit_aiming_speed_casual;
			break;
		}
	}

	return;
}

void actor_look_affect_movement(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct direction_specification *movement_direction = &actor->orders.look.primary_direction;

	if (movement_direction->type == _direction_specification_movement &&
		!actor_path_has_path(actor_index))
	{
		actor->orders.look.primary_priority = _primary_priority_none;
	}

	if (actor->orders.look.primary_priority >= _primary_priority_facing &&
		movement_direction->type != _direction_specification_movement &&
		actor_look_decode_direction(
			actor_index,
			movement_direction,
			&actor->control.moving_forced_aim_direction))
	{
		actor->control.moving_forced_by_aiming = TRUE;
	}
	else
	{
		actor->control.moving_forced_by_aiming = FALSE;
	}

	return;
}
