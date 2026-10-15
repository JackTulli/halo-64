/*
WEAPONS.C

symbols in this file:
000EA4D0 0010:
	_animation_convert_frame_to_pal (0000)
000EA4E0 0010:
	_animation_key_frame_index (0000)
000EA4F0 0020:
	_animation_update (0000)
000EA510 0020:
	_animation_choose_random_permutation (0000)
000EA530 0010:
	_weapons_initialize (0000)
000EA540 0010:
	_weapons_initialize_for_new_map (0000)
000EA550 0010:
	_weapons_dispose_from_old_map (0000)
000EA560 0010:
	_weapons_dispose (0000)
000EA570 00d0:
	_weapon_place (0000)
000EA640 0050:
	_weapon_preprocess_node_orientations (0000)
000EA690 0030:
	_weapon_get_label (0000)
000EA6C0 0020:
	_weapon_set_integrated_light_power (0000)
000EA6E0 0080:
	_weapon_estimate_time_to_target (0000)
000EA760 0090:
	_weapon_can_be_fired (0000)
000EA7F0 0030:
	_weapon_useful (0000)
000EA820 0070:
	_weapon_compute_movement_penalty (0000)
000EA890 0010:
	_weapon_melee_attack (0000)
000EA8A0 0030:
	_weapon_must_be_readied (0000)
000EA8D0 0030:
	_weapon_is_flag (0000)
000EA900 0050:
	_weapon_prevents_grenade_throwing (0000)
000EA950 01b0:
	_weapon_get_first_person_animation_time (0000)
000EAB00 0030:
	_weapon_overcharged (0000)
000EAB30 0050:
	_code_000eab30 (0000)
000EAB80 0050:
	_code_000eab80 (0000)
000EABD0 0050:
	_code_000eabd0 (0000)
000EAC20 0040:
	_code_000eac20 (0000)
000EAC60 0030:
	_code_000eac60 (0000)
000EAC90 0040:
	_code_000eac90 (0000)
000EACD0 0050:
	_code_000eacd0 (0000)
000EAD20 0090:
	_code_000ead20 (0000)
000EADB0 00f0:
	_code_000eadb0 (0000)
000EAEA0 0050:
	_code_000eaea0 (0000)
000EAEF0 00f0:
	_code_000eaef0 (0000)
000EAFE0 0070:
	_code_000eafe0 (0000)
000EB050 0040:
	_code_000eb050 (0000)
000EB090 0090:
	_code_000eb090 (0000)
000EB120 0080:
	_code_000eb120 (0000)
000EB1A0 0050:
	_code_000eb1a0 (0000)
000EB1F0 0020:
	_code_000eb1f0 (0000)
000EB210 0020:
	_code_000eb210 (0000)
000EB230 01b0:
	_code_000eb230 (0000)
000EB3E0 0120:
	_weapon_set_total_rounds (0000)
000EB500 0010:
	_power (0000)
000EB510 0010:
	_random (0000)
000EB520 0190:
	_weapon_new (0000)
000EB6B0 0060:
	_weapon_delete (0000)
000EB710 0390:
	_weapon_export_function_values (0000)
000EBAA0 0220:
	_weapon_handle_potential_inventory_item (0000)
000EBCC0 00a0:
	_weapon_owner_update (0000)
000EBD60 0140:
	_weapon_build_weapon_interface_state (0000)
000EBEA0 0080:
	_weapon_reloading (0000)
000EBF20 0070:
	_weapon_rotate_zoom_level (0000)
000EBF90 0160:
	_weapon_get_zoom_magnification (0000)
000EC0F0 0050:
	_weapon_get_field_of_view (0000)
000EC140 0060:
	_weapon_prevents_melee_attack (0000)
000EC1A0 0160:
	_code_000ec1a0 (0000)
000EC300 00e0:
	_code_000ec300 (0000)
000EC3E0 00c0:
	_code_000ec3e0 (0000)
000EC4A0 0080:
	_code_000ec4a0 (0000)
000EC520 00c0:
	_code_000ec520 (0000)
000EC5E0 0090:
	_code_000ec5e0 (0000)
000EC670 0060:
	_code_000ec670 (0000)
000EC6D0 0060:
	_code_000ec6d0 (0000)
000EC730 0190:
	_code_000ec730 (0000)
000EC8C0 00a0:
	_code_000ec8c0 (0000)
000EC960 0030:
	_code_000ec960 (0000)
000EC990 0160:
	_weapon_set_current_amount (0000)
000ECAF0 0080:
	_weapon_ready (0000)
000ECB70 00a0:
	_weapon_put_away (0000)
000ECC10 0110:
	_weapon_aim (0000)
000ECD20 0010:
	_weapon_stop_reload (0000)
000ECD30 0050:
	_code_000ecd30 (0000)
000ECD80 0720:
	_code_000ecd80 (0000)
000ED4A0 07c0:
	_code_000ed4a0 (0000)
000EDC60 0270:
	_code_000edc60 (0000)
000EDED0 00d0:
	_code_000eded0 (0000)
000EDFA0 0100:
	_code_000edfa0 (0000)
000EE0A0 0080:
	_code_000ee0a0 (0000)
000EE120 0af0:
	_weapon_update (0000)
00279248 000e:
	??_C@_0O@NGIMIMAN@weapon_update?$AA@ (0000)
00279258 0010:
	??_C@_0BA@HKDAKBBH@?$HOsecondary?9blur?$AA@ (0000)
00279268 000e:
	??_C@_0O@MICLHJCM@?$HOprimary?9blur?$AA@ (0000)
00279278 001f:
	??_C@_0BP@EKCOHDKN@c?3?2halo?2SOURCE?2items?2weapons?4c?$AA@ (0000)
00279298 004b:
	??_C@_0EL@OJBCMDA@trigger_index?$DO?$DN0?5?$CG?$CG?5trigger_inde@ (0000)
002792E8 004e:
	??_C@_0EO@GOIBLAPE@magazine_index?$DO?$DN0?5?$CG?$CG?5magazine_in@ (0000)
00279338 0033:
	??_C@_0DD@MMDDCEO@new_state?$DO?$DN0?5?$CG?$CG?5new_state?$DMNUMBER@ (0000)
00279370 0048:
	??_C@_0EI@IEKIMHHO@trigger_index?$DO?$DN0?5?$CG?$CG?5trigger_inde@ (0000)
002793B8 000d:
	??_C@_0N@PABHPCND@rounds_array?$AA@ (0000)
002793C8 001e:
	??_C@_0BO@MNJMFFGN@?$CBweapon_is_flag?$CIweapon_index?$CJ?$AA@ (0000)
002793E8 001f:
	??_C@_0BP@BIBKIDBE@weapon?9?$DOweapon?4primary_trigger?$AA@ (0000)
00279408 0013:
	??_C@_0BD@CJIOBLLL@magnification?$DO0?40f?$AA@ (0000)
0027941C 000e:
	??_C@_0O@NODOJABJ@magnification?$AA@ (0000)
0027942C 0004:
	__real@40470d23 (0000)
00279430 0004:
	__real@3d00adfd (0000)
00279434 0012:
	??_C@_0BC@IMIGIGHG@secondary?5trigger?$AA@ (0000)
00279448 0004:
	__real@3d2aaaab (0000)
00307140 0600:
	_data_00307140 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "weapons.h"

#include "equipment.h"
#include "weapon_datum_flags.h"
#include "weapon_definitions.h"
#include "weapon_export_function_mode.h"
#include "projectile_definitions.h"
#include "projectiles.h"

#include "ai/actors.h"
#include "ai/ai_runtime.h"
#include "cache/cache_files.h"
#include "cseries/profile.h"
#include "effects/effect_definitions.h"
#include "effects/effects.h"
#include "game/cheats.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "interface/first_person_weapons.h"
#include "models/model_animation_definitions.h"
#include "objects/damage.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum weapon_trigger_flags
{
	_weapon_trigger_released_since_last_shot_bit = 0,
	_weapon_trigger_was_down_bit,
	_weapon_trigger_toggled_bit,
	_weapon_trigger_useless_bit,
	_weapon_trigger_blurred_bit,
	_weapon_trigger_fired_before_charging_bit,
	NUMBER_OF_WEAPON_TRIGGER_DATUM_FLAGS,
};

enum
{
	MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON = 2,
};

/* TU-local copies: no shared header declares these tag/runtime enumerations yet.
   Names follow the HCEA database enumerations; ai.c and actors.c carry their own
   ai unit effect copies, and first_person_weapons.c an animation update result copy. */
enum trigger_distribution_function
{
	_trigger_distribution_point = 0,
	_trigger_distribution_horizontal_fan,
	NUMBER_OF_TRIGGER_DISTRIBUTION_FUNCTIONS,
};

enum trigger_firing_effect_type
{
	_trigger_firing_effect = 0,
	_trigger_overheated_effect,
	_trigger_empty_effect,
	NUMBER_OF_TRIGGER_FIRING_EFFECTS,
};

enum weapon_overcharged_action
{
	_trigger_overcharged_none = 0,
	_trigger_overcharged_explodes,
	_trigger_overcharged_fire,
	NUMBER_OF_TRIGGER_OVERCHARGED_ACTIONS,
};

enum weapon_secondary_trigger_mode
{
	_weapon_secondary_trigger_normal = 0,
	_weapon_secondary_trigger_slaved_to_primary,
	_weapon_secondary_trigger_inhibits_primary,
	_weapon_secondary_trigger_loads_alternate_ammunition,
	_weapon_secondary_trigger_loads_multiple_primary_ammunition,
	NUMBER_OF_WEAPON_SECONDARY_TRIGGER_MODES,
};

enum weapon_magazine_flags
{
	_weapon_magazine_wastes_rounds_when_reloaded_bit = 0,
	_weapon_magazine_must_be_chambered_every_shot_bit,
	NUMBER_OF_WEAPON_MAGAZINE_FLAGS,
};

enum animation_update_result
{
	_animation_running = 0,
	_animation_key_frame,
	_animation_will_restart_on_next_frame,
	_animation_restarted,
	_animation_looped,
	NUMBER_OF_ANIMATION_UPDATE_RESULTS,
};

enum
{
	_ai_unit_effect_bump = 0,
	_ai_unit_effect_shooting,
	_ai_unit_effect_death_scream,
	_ai_unit_effect_magic_sight,
	NUMBER_OF_AI_UNIT_EFFECTS,
};

/* ---------- macros */

/* ---------- structures */

struct weapon_interface_magazine_state
{
	boolean reloading;
	boolean can_fire;
	short rounds_loaded;
	short rounds_loaded_maximum;
	short rounds_remaining;
	short rounds_remaining_maximum;
};

struct weapon_interface_state
{
	real heat;
	real age;
	boolean overheated;
	byte _pad09;
	short magazine_count;
	struct weapon_interface_magazine_state magazines[2];
};

struct animation_graph_weapon_animations
{
	long unused1[4];
	struct tag_block animations;
};

struct animation_graph_first_person_weapon_animations
{
	long unused1[4];
	struct tag_block animations;
};

/* TU-local: weapon_trigger_definition.firing_effects element; no shared header declares it yet. */
struct trigger_firing_effect
{
	short shots_lower_bound;
	short shots_upper_bound;
	long unused[8];
	struct tag_reference effects[NUMBER_OF_TRIGGER_FIRING_EFFECTS];
	struct tag_reference damage_effects[NUMBER_OF_TRIGGER_FIRING_EFFECTS];
};

/* ---------- prototypes */

real transition_function_evaluate(
	short function_type,
	real value);
void unit_handle_weapon_state_change(
	long object_index,
	short new_state);

static struct weapon_trigger *weapon_trigger_get(
	struct weapon_datum *weapon,
	short trigger_index);
static struct weapon_magazine *weapon_magazine_get(
	struct weapon_datum *weapon,
	short magazine_index);
static real weapon_trigger_get_charged_fraction(
	long weapon_index,
	short trigger_index);

static boolean weapon_busy(
	long weapon_index);
static boolean weapon_magazine_state_change_ok(
	long weapon_index);
static long weapon_get_effect_object_index(
	long weapon_index);
static long weapon_get_owner_object_index(
	long weapon_index);
static long weapon_get_projectile_owner_object_index(
	long weapon_index);
static boolean weapon_trigger_can_fire_again(
	long weapon_index,
	short trigger_index);
static void weapon_magazine_idle(
	long weapon_index,
	short magazine_index);
static long weapon_effect_looping_new(
	long weapon_index,
	long effect_index);
static void weapon_detonate(
	long weapon_index);
static void weapon_trigger_change_state(
	long weapon_index,
	short trigger_index,
	short new_state,
	short new_state_timer);
static void weapon_trigger_start_ejection_port(
	long weapon_index,
	short trigger_index,
	boolean chamber);
static void weapon_state_key_frame(
	long weapon_index);
static boolean weapon_magazine_state_interruptable(
	short old_state,
	short new_state);
static long weapon_effect_new(
	long weapon_index,
	long effect_index,
	real effect_scale,
	real effect_error);
static void weapon_magazine_start_chamber(
	long weapon_index,
	short magazine_index);
static void weapon_magazine_finish_chamber(
	long weapon_index,
	short magazine_index);
static void weapon_trigger_fully_charged(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_idle(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_locked(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_recover(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_finish_tracking(
	long weapon_index,
	short trigger_index);
static void weapon_reset(
	long weapon_index);

static boolean weapon_state_interruptable(
	short old_state,
	short new_state);
static boolean weapon_set_state(
	long weapon_index,
	short new_state,
	boolean immediate);

static void weapon_magazine_finish_reload(
	long weapon_index,
	short magazine_index);
static void weapon_magazine_start_reload(
	long weapon_index,
	short magazine_index,
	boolean unknown);
static void weapon_state_next(
	long weapon_index);

static void projectile_distribute(
	real_vector3d *forward,
	real_vector3d *up,
	short distribution_function,
	real distribution_angle,
	short projectile_index,
	short projectile_count);
static void trigger_create_projectiles(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_fire(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_begin_firing(
	long weapon_index,
	short trigger_index,
	boolean force);
static void weapon_trigger_overload(
	long weapon_index,
	long trigger_index);
static void weapon_trigger_release_charge(
	long weapon_index,
	short trigger_index);
static void weapon_trigger_overcharged(
	long weapon_index,
	short trigger_index);

/* ---------- globals */

struct weapons_globals
{
	char *blurred_permutation_names[2];
	struct profile_section update_profile;
};

struct weapons_globals data_00307140 =
{
	{"~primary-blur", "~secondary-blur"},
	{"weapon_update", NONE, TRUE}
};

/* ---------- public code */

void weapons_initialize(
	void)
{
	return;
}

void weapons_initialize_for_new_map(
	void)
{
	return;
}

void weapons_dispose_from_old_map(
	void)
{
	return;
}

void weapons_dispose(
	void)
{
	return;
}

void weapon_place(
	long weapon_index,
	struct scenario_weapon_datum *scenario_weapon)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	if (weapon_definition->weapon.magazines.count>0)
	{
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, 0, struct weapon_magazine_definition);

		weapon->weapon.magazines[0].rounds_total = MIN(scenario_weapon->rounds_total, magazine_definition->rounds_total_maximum);
		weapon->weapon.magazines[0].rounds_loaded = MIN(scenario_weapon->rounds_loaded, magazine_definition->rounds_loaded_maximum);
	}

	SET_FLAG(weapon->object.flags, _object_at_rest_bit, TEST_FLAG(scenario_weapon->flags, _weapon_created_at_rest_bit));
	SET_FLAG(weapon->object.flags, _object_cannot_be_garbage_bit, TRUE);
	SET_FLAG(weapon->item.flags, _item_does_not_accelerate_bit, !TEST_FLAG(scenario_weapon->flags, _weapon_does_accelerate_bit));

	if (!TEST_FLAG(scenario_weapon->flags, _weapon_created_at_rest_bit))
		weapon->object.position.z += 0.05f;

	return;
}

void weapon_ready(
	long weapon_index)
{
	struct weapon_datum* weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	weapon_reset(weapon_index);
	weapon_set_state(weapon_index, _weapon_state_ready, TRUE);
	first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_ready);
	weapon_effect_new(weapon_index, weapon_definition->weapon.ready_effect.index, 0.f, 0.f);
	weapon->weapon.state_timer = weapon_get_first_person_animation_time(weapon_index, 0, _first_person_weapon_animation_ready, NONE);

	return;
}

boolean weapon_put_away(
	long weapon_index,
	boolean immediate)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean put_away = FALSE;

	if ((immediate || !weapon_busy(weapon_index)) && weapon_set_state(weapon_index, _weapon_state_put_away, immediate))
	{
		weapon->weapon.control_flags = 0;
		weapon_reset(weapon_index);

		if (weapon->weapon.overheated_effect_index != NONE)
		{
			effect_delete(weapon->weapon.overheated_effect_index);
			weapon->weapon.overheated_effect_index = NONE;
		}
		
		first_person_weapon_message_from_weapon(weapon_index, 11);
		put_away = TRUE;
	}

	return put_away;
}

boolean weapon_aim(
	long weapon_index,
	short trigger_index,
	real_point3d const *origin,
	real_point3d const *target_point,
	boolean lob,
	real_vector3d *result_aim_vector,
	real *result_ticks,
	real *result_distance,
	boolean *result_linear)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean result = FALSE;

	if (trigger_index>=0 && trigger_index<weapon_definition->weapon.triggers.count)
	{
		struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
		struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

		projectile_aim(projectile_definition_get(trigger_definition->projectile.index), origin, target_point, NULL, NULL, NULL, NULL, lob, result_aim_vector, NULL, result_ticks, result_distance, result_linear);
		match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\items\\weapons.c", 1301, result_aim_vector);

		result = TRUE;
	}

	return result;
}

boolean weapon_is_flag(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	return (weapon_definition->weapon.flags>>3)&1;
}

boolean weapon_must_be_readied(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	return (weapon_definition->weapon.flags>>3)&1;
}

boolean weapon_overcharged(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	return weapon->weapon.triggers[0].state==_trigger_charging || weapon->weapon.triggers[0].state==_trigger_charged;
}

void weapon_stop_reload(
	long weapon_index)
{
	weapon_reset(weapon_index);

	return;
}

char const *weapon_get_label(
	long weapon_index)
{
	char const *label = "";

	if (weapon_index!=NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

		label = weapon_definition->weapon.label;
	}

	return label;
}

boolean weapon_useful(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	if (weapon->weapon.age>=1.0f)
		return FALSE;

	return TRUE;
}

void weapon_set_integrated_light_power(
	long weapon_index,
	real light_power)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	weapon->weapon.integrated_light_power = light_power;

	return;
}

boolean weapon_prevents_grenade_throwing(
	long weapon_index)
{
	boolean result = TRUE;

	if (weapon_index!=NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

		result = (weapon_definition->weapon.flags>>6)&1;
		if (weapon->weapon.state>=_weapon_state_primary_reload && weapon->weapon.state<=_weapon_state_put_away)
			result = TRUE;
	}

	return result;
}

boolean weapon_prevents_melee_attack(
	long weapon_index)
{
	boolean result = TRUE;

	if (weapon_index!=NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

		result = (weapon_definition->weapon.flags>>9)&1;
		if (weapon_overcharged(weapon_index))
			result = TRUE;
	}

	return result;
}

void weapon_melee_attack(
	long weapon_index)
{
	return;
}

boolean weapon_new(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	short magazine_index;
	short trigger_index;

	weapon->weapon.state = _weapon_state_idle;
	weapon->weapon.overheated_effect_index = NONE;

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.magazines.count; ++magazine_index)
	{
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

		magazine->rounds_loaded = MIN(magazine_definition->rounds_total_initial, magazine_definition->rounds_loaded_maximum);
		magazine->rounds_total = magazine_definition->rounds_total_initial-magazine->rounds_loaded;
	}

	for (trigger_index = 0; trigger_index<weapon_definition->weapon.triggers.count; ++trigger_index)
	{
		struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);

		(void)TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
		trigger->charging_effect_index = NONE;
		trigger->idle_ticks = 127;
	}

	return TRUE;
}

void weapon_set_total_rounds(
	long weapon_index,
	short *rounds_array)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	short magazine_index;

	match_assert("c:\\halo\\SOURCE\\items\\weapons.c", 3082, rounds_array);

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.magazines.count; ++magazine_index)
	{
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

		magazine->rounds_total = MIN(magazine_definition->rounds_total_maximum, rounds_array[magazine_index]);
		magazine->rounds_loaded = MIN(magazine->rounds_loaded, magazine->rounds_total);
	}

	return;
}

void weapon_delete(
	long weapon_index)
{
	if (game_engine_running())
	{
		match_assert("c:\\halo\\SOURCE\\items\\weapons.c", 234, !weapon_is_flag(weapon_index));
	}

	return;
}

void weapon_export_function_values(
	long weapon_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct object_datum *object= (struct object_datum *)weapon;
	real *function_values;
	short *function_modes;
	long function_index;

	while (TEST_FLAG(object->object.flags, _object_invisible_bit) && object->object.parent_object_index!=NONE)
	{
		object= object_get(object->object.parent_object_index);
	}

	function_values= object->object.incoming_function_values;
	function_modes= weapon_definition->weapon.function_modes;

	for (function_index= NUMBER_OF_INCOMING_OBJECT_FUNCTIONS; function_index; --function_index, ++function_modes, ++function_values)
	{
		if (*function_modes!=_weapon_function_none)
		{
			real function_value= 0.0f;

			switch (*function_modes)
			{
			case _weapon_function_ready:
				function_value= 1.0f;
				break;

			case _weapon_function_heat:
				function_value= weapon->weapon.heat;
				break;

			case _weapon_function_overheated:
				if (TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit) && weapon_definition->weapon.heat_recovery_threshold!=1.0f)
				{
					function_value= (weapon->weapon.heat-weapon_definition->weapon.heat_recovery_threshold)/(1.0f-weapon_definition->weapon.heat_recovery_threshold);
				}
				break;

			case _weapon_function_illumination:
				{
					short trigger_index;

					for (trigger_index= 0; trigger_index<weapon_definition->weapon.triggers.count; ++trigger_index)
					{
						struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
						struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);

						if (trigger_definition->charging_time>0.0f)
						{
							real charged_illumination= weapon_trigger_get_charged_fraction(weapon_index, trigger_index)*trigger_definition->charged_illumination;

							function_value= MAX(function_value, charged_illumination);
						}

						if (trigger->state==_trigger_charged)
						{
							function_value= MAX(function_value, (1.0f-trigger_definition->charged_illumination)*weapon->weapon.overcharged+trigger_definition->charged_illumination);
						}

						function_value= MAX(function_value, trigger->illumination);
						trigger->illumination= function_value;
					}

					function_value= MAX(function_value, weapon_definition->weapon.heat_illumination*weapon->weapon.heat);
				}
				break;

			case _weapon_function_primary_ammunition:
			case _weapon_function_secondary_ammunition:
				{
					short magazine_index= (short)(*function_modes-_weapon_function_primary_ammunition);

					if (magazine_index<weapon_definition->weapon.magazines.count)
					{
						struct weapon_magazine_definition *magazine_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

						if (magazine_definition->rounds_loaded_maximum)
						{
							function_value= (real)weapon->weapon.magazines[magazine_index].rounds_loaded/magazine_definition->rounds_loaded_maximum;
						}
					}
				}
				break;

			case _weapon_function_primary_ejection_port:
			case _weapon_function_secondary_ejection_port:
				{
					short trigger_index= (short)(*function_modes-_weapon_function_primary_ejection_port);

					if (trigger_index<weapon_definition->weapon.triggers.count)
					{
						function_value= weapon->weapon.triggers[trigger_index].ejection_port_position;
					}
				}
				break;

			case _weapon_function_primary_rate_of_fire:
			case _weapon_function_secondary_rate_of_fire:
				{
					short trigger_index= (short)(*function_modes-_weapon_function_primary_rate_of_fire);

					if (trigger_index<weapon_definition->weapon.triggers.count)
					{
						function_value= weapon->weapon.triggers[trigger_index].rate_of_fire;
					}
				}
				break;

			case _weapon_function_primary_firing:
			case _weapon_function_secondary_firing:
				{
					short trigger_index= (short)(*function_modes-_weapon_function_primary_firing);

					if (trigger_index<weapon_definition->weapon.triggers.count)
					{
						function_value= weapon->weapon.triggers[trigger_index].rate_of_fire;

						if (game_time_get()-weapon->weapon.game_time_last_fired>1)
						{
							function_value= 0.0f;
						}
					}
				}
				break;

			case _weapon_function_primary_charged:
			case _weapon_function_secondary_charged:
				{
					short trigger_index= (short)(*function_modes-_weapon_function_primary_charged);

					if (trigger_index<weapon_definition->weapon.triggers.count)
					{
						struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
						struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);

						function_value= weapon_trigger_get_charged_fraction(weapon_index, trigger_index);
					}
				}
				break;

			case _weapon_function_integrated_light:
				function_value= weapon->weapon.integrated_light_power;
				break;

			case _weapon_function_age:
				function_value= weapon->weapon.age;
				break;
			}

			*function_values= function_value;
		}
	}

	return;
}

boolean weapon_handle_potential_inventory_item(
	long weapon_index,
	long item_object_index,
	short local_player_index,
	short *rounds_picked_up)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct item_datum *item = item_get(item_object_index);
	long item_definition_index = item->definition_index;
	boolean handled = FALSE;
	short magazine_index;

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.magazines.count; ++magazine_index)
	{
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

		if (magazine->rounds_total<magazine_definition->rounds_total_maximum)
		{
			short rounds_needed = magazine_definition->rounds_total_maximum-magazine->rounds_total;
			short rounds_taken = 0;

			if (weapon->definition_index==item_definition_index)
			{
				struct weapon_datum *item_weapon = weapon_get(item_object_index);
				struct weapon_magazine *item_magazine = weapon_magazine_get(item_weapon, magazine_index);

				rounds_taken = MIN(item_magazine->rounds_total, rounds_needed);

				if (rounds_taken>0)
				{
					item_magazine->rounds_total -= rounds_taken;

					if (weapon_definition->weapon.pickup_sound.index!=NONE && local_player_index!=NONE)
						unspatialized_impulse_sound_new(weapon_definition->weapon.pickup_sound.index, 1.0f);

					if (item_magazine->rounds_total==0)
						object_delete(item_object_index);
				}

				handled = TRUE;
			}
			else
			{
				short ammunition_index;

				for (ammunition_index = 0; ammunition_index<magazine_definition->ammunition_objects.count; ++ammunition_index)
				{
					struct weapon_ammunition_object *ammunition_object = TAG_BLOCK_GET_ELEMENT(&magazine_definition->ammunition_objects, ammunition_index, struct weapon_ammunition_object);

					if (ammunition_object->object.index==item_definition_index)
					{
						rounds_taken = MIN(ammunition_object->rounds, rounds_needed);

						if (rounds_taken>0)
						{
							if (local_player_index!=NONE)
								equipment_definition_handle_pickup(ammunition_object->object.index);

							object_delete(item_object_index);
							handled = TRUE;
							break;
						}
					}
				}
			}

			magazine->rounds_total += rounds_taken;
			*rounds_picked_up = rounds_taken;
		}
	}

	return handled;
}

short animation_choose_random_permutation(
	long animation_graph_index,
	short animation_index)
{
	return animation_choose_random_permutation_internal(TRUE, animation_graph_index, animation_index);
}

short animation_update(
	long animation_graph_index,
	struct animation_state *state,
	long *sound_index)
{
	return animation_update_internal(TRUE, animation_graph_index, state, sound_index);
}

short animation_key_frame_index(
	struct animation const *animation)
{
	return animation->private_key_frame_index;
}

short animation_convert_frame_to_pal(
	struct animation const *animation,
	short frame_index)
{
	return frame_index;
}

boolean weapon_reloading(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean result = FALSE;

	if (weapon_definition->weapon.magazines.count>0)
	{
		if (weapon_magazine_get(weapon, 0)->state==_magazine_reloading)
			result = TRUE;
	}

	return result;
}

short weapon_rotate_zoom_level(
	long weapon_index,
	short zoom_level)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	if (!weapon_reloading(weapon_index))
	{
		if (zoom_level>=0 && zoom_level<weapon_definition->weapon.zoom_level_count-1)
			zoom_level++;
		else
			zoom_level = zoom_level==weapon_definition->weapon.zoom_level_count-1 ? NONE : 0;
	}

	return zoom_level;
}

real weapon_get_zoom_magnification(
	long weapon_index,
	short zoom_level)
{
	real magnification = 1.0f;
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	if (zoom_level>=0 && zoom_level<weapon_definition->weapon.zoom_level_count)
	{
		real zoom_fraction = weapon_definition->weapon.zoom_level_count>1 ? (real)zoom_level/(weapon_definition->weapon.zoom_level_count-1) : 0.0f;
		real minimum_magnification = weapon_definition->weapon.zoom_magnification_minimum>0.0f ? weapon_definition->weapon.zoom_magnification_minimum : 1.0f;
		real maximum_magnification = weapon_definition->weapon.zoom_magnification_maximum>0.0f ? weapon_definition->weapon.zoom_magnification_maximum : 1.0f;

		magnification = power(maximum_magnification/minimum_magnification, zoom_fraction)*minimum_magnification;

		match_assert_valid_real("c:\\halo\\SOURCE\\items\\weapons.c", 1442, magnification);
		match_assert("c:\\halo\\SOURCE\\items\\weapons.c", 1443, magnification>0.0f);
	}

	return magnification;
}

real weapon_get_field_of_view(
	long weapon_index,
	real field_of_view,
	short zoom_level)
{
	real result = field_of_view;
	real magnification = weapon_get_zoom_magnification(weapon_index, zoom_level);

	if (magnification!=1.0f)
	{
		real zoom_field_of_view = field_of_view/magnification;

		if (zoom_field_of_view>_pi/100.f && zoom_field_of_view<_pi-_pi/100.f)
			result = zoom_field_of_view;
	}

	return result;
}

void weapon_build_weapon_interface_state(
	long weapon_index,
	struct weapon_interface_state *state)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	short magazine_index;

	state->heat = weapon->weapon.heat;
	state->age = weapon->weapon.age;
	state->overheated = TEST_FLAG(weapon->weapon.flags, 0);
	state->magazine_count = (short)weapon_definition->weapon.magazines.count;

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.magazines.count; ++magazine_index)
	{
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);
		long reloading = magazine->state==_magazine_reloading || magazine->state==_magazine_chambering;

		state->magazines[magazine_index].reloading = reloading;
		state->magazines[magazine_index].can_fire = magazine->state==_magazine_idle;
		state->magazines[magazine_index].rounds_loaded = magazine->rounds_loaded;
		state->magazines[magazine_index].rounds_loaded_maximum = magazine_definition->rounds_loaded_maximum;
		state->magazines[magazine_index].rounds_remaining = magazine->rounds_total;
		state->magazines[magazine_index].rounds_remaining_maximum = magazine_definition->rounds_total_maximum;
	}

	return;
}

void weapon_set_current_amount(
	long weapon_index,
	real amount)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean uses_age = FALSE;

	if (weapon_definition->weapon.magazines.count==0)
	{
		uses_age = TRUE;
	}
	else
	{
		short trigger_index;

		for (trigger_index = 0; trigger_index<weapon_definition->weapon.triggers.count; ++trigger_index)
		{
			struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

			if (trigger_definition->age_generated_per_round>0.0f)
			{
				uses_age = TRUE;
				break;
			}
		}
	}

	if (amount<0.0f)
		amount = 0.0f;
	else if (amount>1.0f)
		amount = 1.0f;

	if (uses_age)
	{
		weapon->weapon.age = 1.0f-amount;
	}
	else if (weapon_definition->weapon.magazines.count>0)
	{
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, 0, struct weapon_magazine_definition);
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, 0);
		short rounds_loaded;

		amount = magazine_definition->rounds_loaded_maximum*amount;
		rounds_loaded = (short)fast_ftol(amount);

		magazine->rounds_total += rounds_loaded-magazine->rounds_loaded;
		magazine->rounds_loaded = rounds_loaded;
	}

	return;
}

short weapon_get_first_person_animation_time(
	long weapon_index,
	short mode,
	short animation_type,
	short shotgun_reload_type)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	short time = 0;

	if (weapon_definition->weapon.interface_definition.first_person_animations.index!=NONE)
	{
		struct animation_graph *animation_graph = animation_graph_definition_get(weapon_definition->weapon.interface_definition.first_person_animations.index);

		if (animation_graph->first_person_weapon_animations.count)
		{
			struct animation_graph_first_person_weapon_animations *weapon_animations = TAG_BLOCK_GET_ELEMENT(&animation_graph->first_person_weapon_animations, 0, struct animation_graph_first_person_weapon_animations);

			if (weapon_animations && animation_type>=0 && animation_type<weapon_animations->animations.count)
			{
				short animation_index = animation_graph_animation_index_get(&weapon_animations->animations)[animation_type].animation_index;

				if (animation_index!=NONE)
				{
					struct animation *animation = TAG_BLOCK_GET_ELEMENT(&animation_graph->animations, animation_index, struct animation);

					switch (mode)
					{
					case _weapon_first_person_animation_time_frame_count:
						time = animation->frame_count;
						break;

					case _weapon_first_person_animation_time_private_key_frame:
						time = animation->private_key_frame_index;
						break;

					default:
						match_vassert("c:\\halo\\SOURCE\\items\\weapons.c", 1588, FALSE, NULL);
						break;
					}

					if (mode==_weapon_first_person_animation_time_frame_count && weapon_definition->weapon.weapon_type==_weapon_type_shotgun)
					{
						struct animation *shotgun_enter = TAG_BLOCK_GET_ELEMENT(&animation_graph->animations, _first_person_weapon_animation_shotgun_enter<weapon_animations->animations.count ? animation_graph_animation_index_get(&weapon_animations->animations)[_first_person_weapon_animation_shotgun_enter].animation_index : NONE, struct animation);
						struct animation *shotgun_exit_empty = TAG_BLOCK_GET_ELEMENT(&animation_graph->animations, _first_person_weapon_animation_shotgun_exit_empty<weapon_animations->animations.count ? animation_graph_animation_index_get(&weapon_animations->animations)[_first_person_weapon_animation_shotgun_exit_empty].animation_index : NONE, struct animation);
						struct animation *shotgun_exit_full = TAG_BLOCK_GET_ELEMENT(&animation_graph->animations, _first_person_weapon_animation_shotgun_exit_full<weapon_animations->animations.count ? animation_graph_animation_index_get(&weapon_animations->animations)[_first_person_weapon_animation_shotgun_exit_full].animation_index : NONE, struct animation);

						/* January resolves all three reload variants, but both handled phases use the enter animation. */
						(void)shotgun_exit_empty;
						(void)shotgun_exit_full;
						switch (shotgun_reload_type)
						{
						case _shotgun_reload_type_first_round:
							time = shotgun_enter->frame_count;
							break;

						case _shotgun_reload_type_first_and_last_round:
							time = shotgun_enter->frame_count;
							break;
						}
					}
				}
			}
		}
	}

	return time;
}

real weapon_estimate_time_to_target(
	long weapon_index,
	short trigger_index,
	real distance)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	real result = 0.0f;

	if (trigger_index>=0 && trigger_index<weapon_definition->weapon.triggers.count)
	{
		struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

		result = projectile_estimate_time_to_target(projectile_definition_get(trigger_definition->projectile.index), distance);
	}

	return result;
}

boolean weapon_can_be_fired(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean result;

	if (weapon->weapon.age>=1.0f)
	{
		result = FALSE;
	}
	else if (game_engine_running() &&
		weapon_definition->weapon.magazines.count>0 &&
		TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, 0, struct weapon_magazine_definition)->rounds_loaded_maximum>0 &&
		!weapon->weapon.magazines[0].rounds_loaded &&
		!weapon->weapon.magazines[0].rounds_total)
	{
		result = FALSE;
	}
	else
	{
		result = TRUE;
	}

	return result;
}

real weapon_compute_movement_penalty(
	long weapon_index,
	boolean forward,
	boolean airborne)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	real penalty;

	if (forward)
		penalty = weapon_definition->weapon.forward_movement_penalty;
	else
		penalty = weapon_definition->weapon.sideways_movement_penalty;

	switch (weapon_definition->weapon.movement_penalty_mode)
	{
	case 1:
		if (!airborne)
			penalty = 0.0f;
		break;

	case 2:
		if ((weapon->weapon.magazines[0].state==_magazine_reloading || weapon->weapon.magazines[1].state==_magazine_reloading) && !airborne)
			penalty = 0.0f;
		break;
	}

	return penalty;
}

void weapon_owner_update(
	long weapon_index,
	word control_flags,
	real primary_trigger)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	weapon->weapon.control_flags = control_flags;
	weapon->weapon.primary_trigger = transition_function_evaluate(4, primary_trigger);
	match_assert_valid_real("c:\\halo\\SOURCE\\items\\weapons.c", 1199, weapon->weapon.primary_trigger);

	return;
}

static void weapon_magazine_finish_reload(
	long weapon_index,
	short magazine_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);
	long rounds_to_load;
	short rounds_loaded;

	if (TEST_FLAG(magazine_definition->flags, 0))
		magazine->rounds_loaded = 0;

	rounds_to_load = magazine_definition->rounds_reloaded>magazine->rounds_total ? magazine->rounds_total : magazine_definition->rounds_reloaded;
	rounds_loaded = magazine->rounds_loaded+rounds_to_load;
	if (rounds_loaded>magazine_definition->rounds_loaded_maximum)
		rounds_loaded = magazine_definition->rounds_loaded_maximum;

	if (!cheat.infinite_ammo && TEST_FLAG(weapon->item.flags, 1))
		magazine->rounds_total = magazine->rounds_total-rounds_loaded+magazine->rounds_loaded;

	magazine->rounds_loaded = rounds_loaded;
	magazine->state = _magazine_unchambered;
	magazine->state_timer = 0;

	if (magazine->rounds_total>0 &&
		magazine->rounds_loaded<magazine_definition->rounds_loaded_maximum &&
		!TEST_FLAG(magazine_definition->flags, 0) &&
		!(weapon->weapon.control_flags & 0x26))
	{
		weapon_magazine_start_reload(weapon_index, magazine_index, FALSE);
	}

	return;
}

static void weapon_magazine_start_chamber(
	long weapon_index,
	short magazine_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);

	if (weapon_magazine_state_interruptable(magazine->state, _magazine_chambering) &&
		weapon_magazine_state_change_ok(weapon_index))
	{
		struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(
			&weapon_definition->weapon.magazines,
			magazine_index,
			struct weapon_magazine_definition);

		weapon_set_state(
			weapon_index,
			(short)(_weapon_state_primary_chamber + magazine_index),
			FALSE);
		weapon_effect_new(
			weapon_index,
			magazine_definition->chambering_effect.index,
			0.0f,
			0.0f);
		magazine->state = _magazine_chambering;
		magazine->state_timer = (short)(magazine_definition->chamber_time * TICKS_PER_SECOND);
	}

	return;
}

static void weapon_magazine_finish_chamber(
	long weapon_index,
	short magazine_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.magazines,
		magazine_index,
		struct weapon_magazine_definition);

	(void)magazine;
	(void)magazine_definition;
	weapon_magazine_idle(weapon_index, magazine_index);

	return;
}

static void weapon_trigger_fully_charged(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);

	(void)trigger;
	weapon_trigger_change_state(
		weapon_index,
		trigger_index,
		_trigger_charged,
		(short)(trigger_definition->charged_time * TICKS_PER_SECOND));
	weapon_set_state(
		weapon_index,
		(short)(_weapon_state_primary_charged + trigger_index),
		TRUE);
	first_person_weapon_message_from_weapon(
		weapon_index,
		_first_person_weapon_message_charged);

	return;
}

static void weapon_trigger_idle(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);

	(void)trigger;
	(void)trigger_definition;
	weapon_trigger_change_state(weapon_index, trigger_index, _trigger_idle, 0);

	return;
}

static void weapon_trigger_locked(
	long weapon_index,
	short trigger_index)
{
	weapon_trigger_change_state(weapon_index, trigger_index, _trigger_locked, NONE);
	return;
}

static void weapon_trigger_recover(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);

	(void)trigger_definition;
	trigger->idle_ticks = 0;
	weapon_trigger_idle(weapon_index, trigger_index);

	return;
}

static void weapon_trigger_finish_tracking(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);

	(void)trigger;
	(void)trigger_definition;
	weapon->weapon.tracked_object_index = NONE;
	weapon_trigger_recover(weapon_index, trigger_index);

	return;
}

static void weapon_magazine_start_reload(
	long weapon_index,
	short magazine_index,
	boolean unknown)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

	switch (magazine->state)
	{
	case _magazine_idle:
	case _magazine_unchambered:
		if (weapon_magazine_state_change_ok(weapon_index))
		{
			if (magazine->rounds_total>0 && magazine->rounds_loaded<magazine_definition->rounds_loaded_maximum)
			{
				short reload_type = NONE;

				weapon_set_state(weapon_index, (short)(_weapon_state_primary_reload+magazine_index), FALSE);
				weapon_effect_new(weapon_index, magazine_definition->reloading_effect.index, 0.0f, 0.0f);
				first_person_weapon_message_from_weapon(weapon_index, (short)(9+(magazine->rounds_loaded!=0)));

				if (weapon_definition->weapon.weapon_type==1)
				{
					if (unknown)
						reload_type = magazine_definition->rounds_loaded_maximum-magazine->rounds_loaded==1 ? 2 : 0;
					else
						reload_type = magazine_definition->rounds_loaded_maximum-magazine->rounds_loaded==1 ? 1 : NONE;
				}

				magazine->state = _magazine_reloading;
				magazine->original_time = magazine->state_timer = weapon_get_first_person_animation_time(weapon_index, 0, _first_person_weapon_animation_reload_while_empty, reload_type);
			}

			weapon->weapon.flags &= ~FLAG(3);
		}
		break;
	}

	return;
}

static void weapon_state_next(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	if (weapon->weapon.state<_weapon_state_primary_charged ||
		(weapon->weapon.state>_weapon_state_secondary_charged && weapon->weapon.state!=_weapon_state_put_away))
		weapon_set_state(weapon_index, _weapon_state_idle, TRUE);

	return;
}

/* ---------- private code */

static struct weapon_trigger *weapon_trigger_get(
	struct weapon_datum *weapon,
	short trigger_index)
{
	struct weapon_definition const *weapon_definition = weapon_definition_get(weapon->definition_index);

	match_assert("c:\\halo\\SOURCE\\items\\weapons.c", 1639, trigger_index>=0 && trigger_index<weapon_definition->weapon.triggers.count);

	return &weapon->weapon.triggers[trigger_index];
}

static struct weapon_magazine *weapon_magazine_get(
	struct weapon_datum *weapon,
	short magazine_index)
{
	struct weapon_definition const *weapon_definition = weapon_definition_get(weapon->definition_index);

	match_assert("c:\\halo\\SOURCE\\items\\weapons.c", 1650, magazine_index>=0 && magazine_index<weapon_definition->weapon.magazines.count);

	return &weapon->weapon.magazines[magazine_index];
}

static real weapon_trigger_get_charged_fraction(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

	switch (trigger->state)
	{
	case _trigger_charging:
		return 1.0f-(trigger->state_timer*(1.0f/TICKS_PER_SECOND))/trigger_definition->charging_time;

	case _trigger_charged:
		return 1.0f;
	}

	return 0.0f;
}

static boolean weapon_busy(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	return
		weapon->weapon.triggers[0].state != _trigger_idle ||
		weapon->weapon.triggers[1].state != _trigger_idle ||
		weapon->weapon.magazines[0].state != _magazine_idle ||
		weapon->weapon.magazines[1].state != _magazine_idle ||
		weapon->weapon.state != _weapon_state_idle;
}

static boolean weapon_magazine_state_change_ok(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	return 
		weapon->weapon.triggers[0].state==_trigger_idle &&
		weapon->weapon.triggers[1].state==_trigger_idle &&
		weapon->weapon.state == _weapon_state_idle;
}

static long weapon_get_effect_object_index(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	long result = weapon_index;

	if (TEST_FLAG(weapon->object.flags, _object_invisible_bit) && weapon->object.parent_object_index!=NONE)
	{
		result = weapon->object.parent_object_index;
	}

	return result;
}

static long weapon_get_owner_object_index(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	long result = NONE;

	if (weapon->object.parent_object_index!=NONE && unit_try_and_get(weapon->object.parent_object_index))
	{
		result = weapon->object.parent_object_index;
	}

	return result;
}

static long weapon_get_projectile_owner_object_index(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	long parent_object_index = weapon->object.parent_object_index;
	struct unit_datum *unit = NULL;
	long result = NONE;

	if (parent_object_index != NONE)
	{
		unit = unit_try_and_get(parent_object_index);
		if (unit)
		{
			result = weapon->object.parent_object_index;
			if (unit->unit.gunner_object_index != NONE)
				result = unit->unit.gunner_object_index;
		}
	}

	return result;
}

static boolean weapon_trigger_can_fire_again(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);
	boolean result = FALSE;
	real fraction = TEST_FLAG(trigger_definition->flags, _weapon_trigger_analog_rate_of_fire_bit)
		? weapon->weapon.primary_trigger
		: trigger->rate_of_fire;
	real rate_of_fire = (trigger_definition->final_rate_of_fire - trigger_definition->initial_rate_of_fire) *
		fraction + trigger_definition->initial_rate_of_fire;
	real required_ticks = rate_of_fire > 0.0001f ? TICKS_PER_SECOND / rate_of_fire : 0.0f;
	char ticks_since_fire;

	if (weapon_definition->weapon.age_rate_of_fire_penalty > 0.0f)
	{
		required_ticks = (weapon->weapon.age * weapon_definition->weapon.age_rate_of_fire_penalty + 1.0f) *
			required_ticks;
	}

	ticks_since_fire = trigger->idle_ticks;
	if ((real)ticks_since_fire + 1.0f >= required_ticks)
		result = TRUE;

	if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_latched_bit) &&
		TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) &&
		!TEST_FLAG(trigger->flags, _weapon_trigger_released_since_last_shot_bit))
	{
		return FALSE;
	}

	return result;
}

static void weapon_magazine_idle(
	long weapon_index,
	short magazine_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.magazines,
		magazine_index,
		struct weapon_magazine_definition);

	(void)magazine_definition;
	magazine->state = _magazine_idle;
	magazine->state_timer = 0;

	return;
}

static long weapon_effect_looping_new(
	long weapon_index,
	long effect_index)
{
	long result = NONE;

	if (effect_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		long effect_object_index = weapon_index;

		if (TEST_FLAG(weapon->object.flags, _object_invisible_bit) &&
			weapon->object.parent_object_index != NONE)
		{
			effect_object_index = weapon->object.parent_object_index;
		}

		weapon_get_owner_object_index(weapon_index);
		if (effect_object_index != NONE)
			result = effect_new_looping(effect_index, effect_object_index, NONE, NONE, NONE);
	}

	return result;
}

static void weapon_detonate(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	weapon_effect_new(
		weapon_index,
		weapon_definition->weapon.detonation_effect.index,
		0.0f,
		0.0f);
	object_delete(weapon_index);

	return;
}

static void weapon_trigger_change_state(
	long weapon_index,
	short trigger_index,
	short new_state,
	short new_state_timer)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	match_assert(
		"c:\\halo\\SOURCE\\items\\weapons.c",
		0xA11,
		trigger_index>=0 && trigger_index<MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON);
	match_assert(
		"c:\\halo\\SOURCE\\items\\weapons.c",
		0xA12,
		new_state>=0 && new_state<NUMBER_OF_TRIGGER_STATES);

	weapon->weapon.triggers[trigger_index].state = (char)new_state;
	weapon->weapon.triggers[trigger_index].state_timer = new_state_timer;

	return;
}

static void weapon_trigger_start_ejection_port(
	long weapon_index,
	short trigger_index,
	boolean chamber)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct weapon_trigger *trigger = weapon_trigger_get(weapon, trigger_index);
	struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(
		&weapon_definition->weapon.triggers,
		trigger_index,
		struct weapon_trigger_definition);

	if (trigger_definition->ejection_port_recovery_time > 0.0f)
	{
		if ((TEST_FLAG(
				trigger_definition->flags,
				_weapon_trigger_ejection_port_during_chamber_animation_bit) && chamber) ||
			(!TEST_FLAG(
				trigger_definition->flags,
				_weapon_trigger_ejection_port_during_chamber_animation_bit) && !chamber))
		{
			trigger->ejection_port_position = 1.0f;
		}
	}

	return;
}

static void weapon_state_key_frame(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);

	(void)weapon_definition_get(weapon->definition_index);
	switch (weapon->weapon.state)
	{
	case _weapon_state_primary_chamber:
		weapon_trigger_start_ejection_port(weapon_index, 0, TRUE);
		break;

	case _weapon_state_secondary_chamber:
		weapon_trigger_start_ejection_port(weapon_index, 1, TRUE);
		break;
	}

	return;
}

static boolean weapon_magazine_state_interruptable(
	short old_state,
	short new_state)
{
	boolean interruptable = FALSE;

	(void)new_state;
	switch (old_state)
	{
	case _magazine_idle:
	case _magazine_unchambered:
		interruptable = TRUE;
		break;
	}

	return interruptable;
}

static long weapon_effect_new(
	long weapon_index,
	long effect_index,
	real effect_scale,
	real effect_error)
{
	long result = NONE;

	if (effect_index!=NONE)
	{
		long effect_object_index = weapon_get_effect_object_index(weapon_index);
		long object_index = weapon_get_owner_object_index(weapon_index);
		long group_tag = tag_get_group_tag(effect_index);

		if (group_tag!=EFFECT_DEFINITION_TAG)
		{
			match_vassert("c:\\halo\\SOURCE\\items\\weapons.c", 2514, group_tag==SOUND_DEFINITION_TAG, NULL);
			
			if (group_tag==SOUND_DEFINITION_TAG)
			{
				object_impulse_sound_new(object_index, effect_index, NONE, global_origin3d, global_forward3d, effect_scale);
				result = NONE;
			}
		}
		else
		{
			result = effect_new_from_object(effect_index, object_index, effect_object_index, NONE, effect_scale, effect_error, NULL, NULL);
		}
	}

	return result;
}

// TODO: finish
static void weapon_reset(
	long weapon_index)
{
	short magazine_index;

	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.triggers.count; ++magazine_index)
	{
		struct weapon_trigger* trigger = weapon_trigger_get(weapon, magazine_index);
		struct weapon_trigger_definition *trigger_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, magazine_index, struct weapon_trigger_definition);

		trigger->state = _trigger_uninitialized;
		trigger->state_timer = 0;
	}

	for (magazine_index = 0; magazine_index<weapon_definition->weapon.magazines.count; ++magazine_index)
	{
		struct weapon_magazine *magazine = weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition = TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

		if (magazine->state==_magazine_reloading)
		{
			if (2*magazine->state_timer<weapon_get_first_person_animation_time(weapon_index, 0, _first_person_weapon_animation_reload_while_empty, NONE))
			{
				weapon_magazine_finish_reload(weapon_index, magazine_index);
			}
		}

		magazine->state = _magazine_idle;
		magazine->state_timer = 0;
	}

	return;
}

static boolean weapon_state_interruptable(
	short old_state,
	short new_state)
{
	boolean interruptable = FALSE;
	long state = old_state;

	if (state!=_weapon_state_idle)
	{
		if (state>_weapon_state_idle && state<=_weapon_state_secondary_recoil)
			interruptable = new_state >= old_state;
	}
	else
	{
		interruptable = TRUE;
	}

	return interruptable;
}

void weapon_preprocess_node_orientations(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	struct animation_graph *animation_graph = animation_graph_definition_get(weapon_definition->object.animation_graph.index);

	if (animation_graph->weapon_animations.count)
		TAG_BLOCK_GET_ELEMENT(&animation_graph->weapon_animations, 0, struct animation_graph_weapon_animations);

	return;
}

static boolean weapon_set_state(
	long weapon_index,
	short new_state,
	boolean immediate)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct weapon_definition *weapon_definition = weapon_definition_get(weapon->definition_index);
	boolean busy = FALSE;

	if (immediate || weapon_state_interruptable(weapon->weapon.state, new_state))
	{
		long owner_object_index;

		if (weapon_definition->object.animation_graph.index!=NONE)
		{
			struct animation_graph *animation_graph = animation_graph_definition_get(weapon_definition->object.animation_graph.index);

			if (animation_graph->weapon_animations.count)
			{
				struct animation_graph_weapon_animations *weapon_animation = TAG_BLOCK_GET_ELEMENT(&animation_graph->weapon_animations, 0, struct animation_graph_weapon_animations);

				if (weapon_animation)
				{
					short animation_index;

					switch (new_state)
					{
					case _weapon_state_idle:             animation_index = 0; break;
					case _weapon_state_primary_recoil:   animation_index = 9; break;
					case _weapon_state_secondary_recoil: animation_index = 10; break;
					case _weapon_state_primary_chamber:  animation_index = 5; break;
					case _weapon_state_secondary_chamber: animation_index = 6; break;
					case _weapon_state_primary_reload:
					case _weapon_state_secondary_reload: animation_index = 3; break;
					case _weapon_state_primary_charged:
					case _weapon_state_secondary_charged: animation_index = 8; break;
					case _weapon_state_ready:            animation_index = 1; break;
					case _weapon_state_put_away:         animation_index = 2; break;
					default: goto skip_animation;
					}

					if (animation_index<weapon_animation->animations.count)
						animation_index = animation_graph_animation_index_get(&weapon_animation->animations)[animation_index].animation_index;
					else
						animation_index = NONE;

					if (animation_index!=NONE || new_state==_weapon_state_idle)
					{
						long graph_index = weapon_definition->object.animation_graph.index;

						weapon->object.animation.state.index = animation_choose_random_permutation_internal(TRUE, graph_index, animation_index);
						weapon->object.animation.state.frame_index = 0;
						weapon->weapon.state = (char)new_state;
					}

skip_animation:;
				}
			}
		}

		owner_object_index = weapon_get_owner_object_index(weapon_index);
		if (unit_try_and_get(owner_object_index))
		{
			unit_handle_weapon_state_change(owner_object_index, new_state);
		}

		busy = TRUE;
	}

	return busy;
}

static void projectile_distribute(
	real_vector3d *forward,
	real_vector3d *up,
	short distribution_function,
	real distribution_angle,
	short projectile_index,
	short projectile_count)
{
	real offset;
	real angle;

	if (projectile_count&1)
	{
		if (projectile_index==0)
		{
			offset= 0.0f;
		}
		else
		{
			short step= projectile_index-1;

			if (step&1)
			{
				step>>= 1;
			}
			else
			{
				step= -(step>>1);
			}
			offset= step;
		}
	}
	else
	{
		offset= (projectile_index>>1)-0.5f;
		if (projectile_index&1)
		{
			offset= -offset;
		}
	}

	angle= offset*distribution_angle;
	switch (distribution_function)
	{
	case _trigger_distribution_horizontal_fan:
		rotate_vector_about_axis(forward, up, sine(angle), cosine(angle));
		break;
	}

	return;
}

static void trigger_create_projectiles(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
	long owner_object_index= weapon_get_owner_object_index(weapon_index);
	char const *trigger_marker_names[MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON];
	struct object_marker markers[MAXIMUM_MARKERS_PER_OBJECT];
	short marker_count;
	short marker_index;

	trigger_marker_names[0]= "primary trigger";
	trigger_marker_names[1]= "secondary trigger";

	marker_count= object_get_marker_by_name(weapon_get_effect_object_index(weapon_index), trigger_marker_names[trigger_index], markers, MAXIMUM_MARKERS_PER_OBJECT);
#ifdef HALO_N64
	/* The PC flame starts at the visible view-model nozzle, rather than the
	unused Xbox world-model marker below the local player's camera. Bots
	and other weapons retain the normal world-marker path. */
	if (!strcmp(tag_get_name(weapon->definition_index), "weapons\\flamethrower\\flamethrower"))
	{
		short first_person_markers = first_person_weapon_get_marker_by_name(weapon_index,
			trigger_marker_names[trigger_index], markers, MAXIMUM_MARKERS_PER_OBJECT);
		if (first_person_markers > 0) marker_count = first_person_markers;
	}
#endif
	if (!marker_count)
	{
		marker_count= 1;
	}
	if (!TEST_FLAG(trigger_definition->flags, _weapon_trigger_uses_weapon_origin_bit))
	{
		marker_count= 1;
	}

	for (marker_index= 0; marker_index<marker_count; marker_index++)
	{
		real_point3d origin= markers[marker_index].matrix.position;
		real_vector3d forward= markers[marker_index].matrix.forward;
		real velocity= 0.0f;
		real error= 0.0f;
		struct unit_datum *unit= unit_try_and_get(owner_object_index);
		long target_object_index= NONE;
		long projectile_definition_index;
		short projectile_count;

		if (!TEST_FLAG(trigger_definition->flags, _weapon_trigger_projectiles_cannot_be_aimed_bit) &&
			unit &&
			!TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
		{
			struct unit_definition *unit_definition= unit_definition_get(unit->definition_index);
			long player_index= unit->unit.player_index;
			long actor_index= unit->unit.actor_index;
			boolean adjust_origin;
			boolean use_aiming_vector;

			if (unit->unit.gunner_object_index!=NONE)
			{
				struct unit_datum *gunner= unit_get(unit->unit.gunner_object_index);

				player_index= gunner->unit.player_index;
				actor_index= gunner->unit.actor_index;
			}

			adjust_origin= TEST_FLAG(unit_definition->unit.flags, _unit_fires_from_camera_bit);
			use_aiming_vector= TRUE;
			if (actor_index!=NONE && actor_firing_blindly(actor_index))
			{
				use_aiming_vector= FALSE;
			}
			if (unit->unit.gunner_object_index!=NONE)
			{
				use_aiming_vector= FALSE;
			}

			unit_adjust_projectile_ray(owner_object_index, &origin, &forward, &velocity, adjust_origin, use_aiming_vector);

			if (player_index!=NONE)
			{
				real_vector3d right;
				real_vector3d up;

				cross_product3d(global_up3d, &forward, &right);
				if (normalize3d(&right)==0.0f)
				{
					right= *global_left3d;
				}
				cross_product3d(&forward, &right, &up);
				normalize3d(&up);

				point_from_line3d(
					&origin,
					&forward,
					trigger_definition->first_person_weapon_offset.x,
					&origin);
				point_from_line3d(
					&origin,
					&right,
					trigger_definition->first_person_weapon_offset.y,
					&origin);
				point_from_line3d(
					&origin,
					&up,
					trigger_definition->first_person_weapon_offset.z,
					&origin);

				target_object_index= player_aim_projectile(player_index, &origin, &forward);
			}
			else if (actor_index!=NONE)
			{
				target_object_index= actor_aim_projectile(actor_index, &origin, &forward, &error);
			}
		}

		if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_uses_weapon_origin_bit))
		{
			origin= markers[marker_index].matrix.position;
		}

		if (trigger_index==0 && weapon->weapon.alternate_shots_loaded>0)
		{
			struct weapon_trigger_definition *secondary_trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, 1, struct weapon_trigger_definition);
			short alternate_shots_loaded= weapon->weapon.alternate_shots_loaded;

			projectile_definition_index= secondary_trigger_definition->projectile.index;
			if (weapon_definition->weapon.secondary_trigger_mode==_weapon_secondary_trigger_loads_multiple_primary_ammunition)
			{
				alternate_shots_loaded++;
			}
			projectile_count= trigger_definition->projectiles_per_shot*alternate_shots_loaded;
			weapon->weapon.alternate_shots_loaded= 0;
		}
		else
		{
			projectile_definition_index= trigger_definition->projectile.index;
			projectile_count= trigger_definition->projectiles_per_shot;
		}

		if (projectile_definition_index!=NONE)
		{
			long projectile_owner_object_index= weapon_get_projectile_owner_object_index(weapon_index);
			real_vector3d first_projectile_forward;
			short projectile_index;

			for (projectile_index= 0; projectile_index<projectile_count; projectile_index++)
			{
				struct object_placement_data data;
				boolean tracer= FALSE;
				boolean inside_bsp;
				long projectile_object_index;

				object_placement_data_new(&data, trigger_definition->projectile.index, projectile_owner_object_index);
				data.position= origin;
				data.forward= forward;

				if (trigger->rate_of_fire==0.0f || trigger->sequential_non_tracer_rounds++>=trigger_definition->rounds_between_tracers)
				{
					tracer= TRUE;
					trigger->sequential_non_tracer_rounds= 0;
				}

				if (error==0.0f)
				{
					real fraction= TEST_FLAG(trigger_definition->flags, _weapon_trigger_analog_rate_of_fire_bit) ?
						weapon->weapon.primary_trigger :
						trigger->error;

					error= (1.0f-fraction)*trigger_definition->projectile_error_angle_lower_bound + fraction*trigger_definition->projectile_error_angle_upper_bound;
				}

				if (!TEST_FLAG(trigger_definition->flags, _weapon_trigger_use_error_when_unzoomed_bit) ||
					!TEST_FLAG(weapon->weapon.control_flags, _weapon_control_zoomed_bit))
				{
					random_vector_in_cone3d(&data.forward, trigger_definition->projectile_error_inner_cone_angle, error, &data.forward);
				}

				if (projectile_index==0)
				{
					first_projectile_forward= data.forward;
				}
				if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_projectiles_have_identical_error_bit))
				{
					data.forward= first_projectile_forward;
				}

				normalize3d(perpendicular3d(&data.forward, &data.up));
				projectile_distribute(&data.forward, &data.up, trigger_definition->projectile_distribution_function, trigger_definition->projectile_distribution_angle, projectile_index, projectile_count);
				scale_vector3d(&data.forward, velocity, &data.translational_velocity);

				inside_bsp= unit && unit->unit.player_index!=NONE;
				if (inside_bsp)
				{
					SET_FLAG(data.flags, _new_object_never_automatically_delete_bit, TRUE);
				}

				projectile_object_index= object_new(&data);
				if (projectile_object_index!=NONE)
				{
					if (inside_bsp)
					{
						real_point3d camera_position;

						unit_get_camera_position(owner_object_index, &camera_position);
#ifdef HALO_N64
						{
							/* (objects.c: the shooter left out, a shot's
							pellets one test) */
							extern void n64_force_projectile_inside_bsp(long object_index,
								real_point3d const *known_good_point, long shooter_index, boolean first_of_shot);

							n64_force_projectile_inside_bsp(projectile_object_index, &camera_position,
								owner_object_index, projectile_index == 0);
						}
#else
						object_force_inside_bsp(projectile_object_index, &camera_position);
#endif
					}
					if (target_object_index!=NONE)
					{
						projectile_set_target_object_index(projectile_object_index, target_object_index);
					}
					if (!tracer)
					{
						projectile_kill_tracer(projectile_object_index);
					}
				}
			}
		}
	}

	return;
}

static void weapon_trigger_fire(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
	long owner_object_index= weapon_get_owner_object_index(weapon_index);
	long damage_effect_index= NONE;
	long effect_index= NONE;
	real effect_error= 0.0f;
	real effect_scale= 0.0f;
	boolean fired= FALSE;
	boolean misfired= FALSE;
	boolean loads_alternate_ammunition= FALSE;

	if (trigger_index==1 &&
		(weapon_definition->weapon.secondary_trigger_mode==_weapon_secondary_trigger_loads_alternate_ammunition ||
		weapon_definition->weapon.secondary_trigger_mode==_weapon_secondary_trigger_loads_multiple_primary_ammunition))
	{
		loads_alternate_ammunition= TRUE;
	}

	if (trigger_definition->magazine_index!=NONE)
	{
		struct weapon_magazine_definition *magazine_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, trigger_definition->magazine_index, struct weapon_magazine_definition);
		struct weapon_magazine *magazine= weapon_magazine_get(weapon, trigger_definition->magazine_index);

		if (!loads_alternate_ammunition || weapon->weapon.alternate_shots_loaded<weapon_definition->weapon.maximum_alternate_shots_loaded)
		{
			if ((magazine->rounds_loaded>=trigger_definition->rounds_per_shot || TEST_FLAG(trigger_definition->flags, _weapon_trigger_can_fire_with_partial_ammunition_bit)) &&
				!(TEST_FLAG(weapon_definition->weapon.flags, _weapon_cannot_fire_at_maximum_age_bit) && weapon->weapon.age>=1.0f) &&
				(magazine->rounds_loaded>=trigger_definition->minimum_rounds_loaded_per_shot || !TEST_FLAG(trigger->flags, _weapon_trigger_released_since_last_shot_bit)))
			{
				if (!cheat.bottomless_clip && (magazine->rounds_loaded-= trigger_definition->rounds_per_shot)<=0)
				{
					magazine->rounds_loaded= 0;
				}
				else if (TEST_FLAG(magazine_definition->flags, _weapon_magazine_must_be_chambered_every_shot_bit))
				{
					magazine->state= _magazine_unchambered;
					magazine->state_timer= 0;
				}

				fired= TRUE;
			}
		}
	}
	else
	{
		fired= TRUE;
	}

	if (cheat.bottomless_clip)
	{
		fired= TRUE;
	}

	if (trigger_definition->firing_effects.count>0)
	{
		struct trigger_firing_effect *firing_effect;
		short effect_type;

		if (trigger->firing_effect_shots_remaining<=0)
		{
			short starting_firing_effect_index= trigger->firing_effect_index;
			short firing_effect_index;

			if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_random_firing_effects_bit))
			{
				firing_effect_index= random()%trigger_definition->firing_effects.count;
			}
			else
			{
				firing_effect_index= starting_firing_effect_index;
			}

			do
			{
				struct trigger_firing_effect *next_firing_effect;

				if (trigger->firing_effects_used_flags==FLAG(trigger_definition->firing_effects.count)-1)
				{
					trigger->firing_effects_used_flags= 0;
				}

				do
				{
					firing_effect_index++;
					if (firing_effect_index>=trigger_definition->firing_effects.count)
					{
						firing_effect_index= 0;
					}
				}
				while (TEST_FLAG(trigger->firing_effects_used_flags, firing_effect_index));

				next_firing_effect= TAG_BLOCK_GET_ELEMENT(&trigger_definition->firing_effects, firing_effect_index, struct trigger_firing_effect);
				trigger->firing_effect_index= firing_effect_index;
				SET_FLAG(trigger->firing_effects_used_flags, firing_effect_index, TRUE);
				trigger->firing_effect_shots_remaining= random_range(next_firing_effect->shots_lower_bound, next_firing_effect->shots_upper_bound);
			}
			while (trigger->firing_effect_shots_remaining<=0 && firing_effect_index!=starting_firing_effect_index);
		}

		trigger->firing_effect_shots_remaining--;
		firing_effect= TAG_BLOCK_GET_ELEMENT(&trigger_definition->firing_effects, trigger->firing_effect_index, struct trigger_firing_effect);

		if (weapon_definition->weapon.age_misfire_start>0.0f &&
			weapon_definition->weapon.age_misfire_start<1.0f &&
			weapon->weapon.age>weapon_definition->weapon.age_misfire_start)
		{
			real misfire_chance= ((weapon->weapon.age-weapon_definition->weapon.age_misfire_start)*weapon_definition->weapon.age_misfire_chance)/(1.0f-weapon_definition->weapon.age_misfire_start);

			if (trigger->state==_trigger_spewing)
			{
				misfire_chance*= 2.0f;
			}
			if (real_random()<misfire_chance)
			{
				misfired= TRUE;
			}
		}

		if (!fired)
		{
			effect_type= _trigger_empty_effect;
			effect_scale= 1.0f;
			effect_error= 0.0f;
		}
		else if (misfired)
		{
			effect_type= _trigger_overheated_effect;
			effect_scale= trigger->rate_of_fire;
			effect_error= 0.0f;
		}
		else
		{
			effect_type= _trigger_firing_effect;
			effect_scale= trigger->rate_of_fire;
			if (weapon_definition->weapon.heat_overheated_threshold==0.0f)
			{
				effect_error= 0.0f;
			}
			else
			{
				effect_error= weapon->weapon.heat/weapon_definition->weapon.heat_overheated_threshold;
			}
		}

		effect_index= firing_effect->effects[effect_type].index;
		damage_effect_index= firing_effect->damage_effects[effect_type].index;
	}

	if (fired)
	{
		if (TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) && game_engine_running())
		{
			long player_index= player_index_from_unit_index(owner_object_index);

			if (player_index!=NONE)
			{
				game_engine_weapon_fired(player_index);
			}
		}

		weapon->weapon.game_time_last_fired= game_time_get();

		first_person_weapon_message_from_weapon(weapon_index, misfired ?
			(trigger_index ? _first_person_weapon_message_secondary_misfire : _first_person_weapon_message_primary_misfire) :
			(trigger_index ? _first_person_weapon_message_secondary_fire : _first_person_weapon_message_primary_fire));
		weapon_trigger_start_ejection_port(weapon_index, trigger_index, FALSE);

		if (trigger_definition->illumination_recovery_time>0.0f)
		{
			trigger->illumination= 1.0f;
		}

		if (!cheat.bottomless_clip)
		{
			weapon->weapon.heat+= trigger_definition->heat_generated_per_round;
		}

		if (!TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) && weapon->weapon.heat>weapon_definition->weapon.heat_overheated_threshold)
		{
			weapon->weapon.heat= weapon_definition->weapon.heat_overheated_threshold;
		}
		else if (weapon->weapon.heat>1.0f)
		{
			weapon->weapon.heat= 1.0f;
		}

		if (TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) && !cheat.infinite_ammo)
		{
			weapon->weapon.age+= trigger_definition->age_generated_per_round;
			if (weapon->weapon.age>1.0f)
			{
				weapon->weapon.age= 1.0f;
			}
		}

		weapon_set_state(weapon_index, trigger_index ? _weapon_state_secondary_recoil : _weapon_state_primary_recoil, FALSE);

		if (!misfired)
		{
			if (loads_alternate_ammunition)
			{
				weapon->weapon.alternate_shots_loaded++;
			}
			else
			{
				trigger_create_projectiles(weapon_index, trigger_index);
				ai_handle_unit_effect(owner_object_index, _ai_unit_effect_shooting, trigger_definition->firing_noise);
			}
		}

		if (owner_object_index!=NONE && damage_effect_index!=NONE)
		{
			struct unit_datum *unit= unit_get(owner_object_index);
			struct damage_data damage;

			damage_data_new(&damage, damage_effect_index);
			SET_FLAG(damage.flags, _damage_from_weapon_bit, TRUE);
			negate_vector3d(&unit->unit.aiming_vector, &damage.direction);
			damage.epicenter= unit->object.bounding_sphere_center;
			damage.origin= damage.epicenter;
			object_cause_damage(&damage, owner_object_index, NONE, NONE, NONE, NULL);
		}

		if (weapon_definition->weapon.weapon_type==_weapon_type_plasma_pistol && trigger_index==1)
		{
			SET_FLAG(weapon->weapon.flags, _weapon_overheat_recoil_bit, TRUE);
		}
	}

	if (weapon->weapon.heat>weapon_definition->weapon.heat_detonation_threshold && real_random()<weapon_definition->weapon.overheated_explosion_fraction)
	{
		weapon_detonate(weapon_index);
	}

	if (!fired)
	{
		weapon_trigger_change_state(weapon_index, trigger_index, _trigger_locked, NONE);
	}
	else if (trigger->state!=_trigger_spewing || misfired)
	{
		if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_tracks_projectile_bit))
		{
			weapon_trigger_change_state(weapon_index, trigger_index, _trigger_tracking, NONE);
		}
		else
		{
			weapon_trigger_recover(weapon_index, trigger_index);
		}
	}

	SET_FLAG(trigger->flags, _weapon_trigger_released_since_last_shot_bit, FALSE);
	weapon_effect_new(weapon_index, effect_index, effect_scale, effect_error);

	return;
}

static void weapon_trigger_begin_firing(
	long weapon_index,
	short trigger_index,
	boolean force)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);
	boolean can_fire= TRUE;

	if (trigger_definition->magazine_index!=NONE)
	{
		struct weapon_magazine_definition *magazine_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, trigger_definition->magazine_index, struct weapon_magazine_definition);
		struct weapon_magazine *magazine= weapon_magazine_get(weapon, trigger_definition->magazine_index);

		if (magazine->state!=_magazine_idle)
		{
			can_fire= FALSE;
		}
	}

	if (TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit))
	{
		can_fire= FALSE;
	}

	if (scenario_location_underwater(&weapon->object.location, &weapon->object.position, NULL))
	{
		can_fire= FALSE;
	}

	if (can_fire)
	{
		if (!force && trigger_definition->charging_time>0.0f)
		{
			if (TEST_FLAG(weapon_definition->weapon.flags, _weapon_cannot_fire_at_maximum_age_bit) && weapon->weapon.age>=1.0f)
			{
				weapon_trigger_fire(weapon_index, trigger_index);
			}
			else
			{
				if (weapon_definition->weapon.triggers.count>1)
				{
					trigger->charging_effect_index= weapon_effect_new(weapon_index, trigger_definition->charging_effect.index, 0.0f, 0.0f);
				}
				else if (trigger->rate_of_fire>0.0f)
				{
					SET_FLAG(trigger->flags, _weapon_trigger_fired_before_charging_bit, TRUE);
					weapon_trigger_fire(weapon_index, trigger_index);
				}
				else
				{
					SET_FLAG(trigger->flags, _weapon_trigger_fired_before_charging_bit, FALSE);
				}

				weapon_trigger_change_state(weapon_index, trigger_index, _trigger_charging, (short)(trigger_definition->charging_time*TICKS_PER_SECOND));
			}
		}
		else if (!force && trigger_definition->overloading_time>0.0f)
		{
			weapon_trigger_change_state(weapon_index, trigger_index, _trigger_overloading, (short)(trigger_definition->overloading_time*TICKS_PER_SECOND));
		}
		else
		{
			weapon_trigger_fire(weapon_index, trigger_index);
		}
	}

	return;
}

static void weapon_trigger_overload(
	long weapon_index,
	long trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

	if (trigger_index+1<weapon_definition->weapon.triggers.count)
	{
		weapon_trigger_fire(weapon_index, (short)(trigger_index+1));
	}

	weapon_trigger_change_state(weapon_index, (short)trigger_index, _trigger_overloading, (short)(trigger_definition->overloading_time*TICKS_PER_SECOND));

	return;
}

static void weapon_trigger_release_charge(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

	if (trigger_definition->spew_time>0.0f)
	{
		weapon_trigger_change_state(weapon_index, trigger_index, _trigger_spewing, (short)(trigger_definition->spew_time*TICKS_PER_SECOND));
	}
	else
	{
		if (weapon_definition->weapon.triggers.count>1)
		{
			weapon_trigger_fire(weapon_index, 1);
		}
		weapon_trigger_recover(weapon_index, trigger_index);
	}

	trigger->rate_of_fire= 0.0f;

	return;
}

static void weapon_trigger_overcharged(
	long weapon_index,
	short trigger_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

	switch (trigger_definition->overcharged_action)
	{
	case _trigger_overcharged_explodes:
		weapon_detonate(weapon_index);
		break;

	case _trigger_overcharged_fire:
		weapon_trigger_release_charge(weapon_index, trigger_index);
		break;
	}

	return;
}

boolean weapon_update(
	long weapon_index)
{
	struct weapon_datum *weapon= weapon_get(weapon_index);
	struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);
	boolean triggers_down[MAXIMUM_NUMBER_OF_TRIGGERS_PER_WEAPON];
	short magazine_index;
	short trigger_index;

	profile_enter(data_00307140.update_profile);

	if (weapon->weapon.tracked_object_index!=NONE && !object_try_and_get_and_verify_type(weapon->weapon.tracked_object_index, _object_mask_all))
	{
		weapon->weapon.tracked_object_index= NONE;
	}

	if (weapon_definition->object.animation_graph.index!=NONE && weapon->object.animation.state.index!=NONE)
	{
		switch (animation_update_internal(TRUE, weapon_definition->object.animation_graph.index, &weapon->object.animation.state, NULL))
		{
		case _animation_key_frame:
			weapon_state_key_frame(weapon_index);
			break;

		case _animation_will_restart_on_next_frame:
			weapon_state_next(weapon_index);
			break;
		}
	}

	if (TEST_FLAG(weapon_definition->weapon.flags, _weapon_detonates_when_dropped_bit) && weapon->object.parent_object_index==NONE)
	{
		item_detonate(weapon_index);
	}

	if (weapon->weapon.integrated_light_power>0.0f)
	{
		struct unit_datum *unit;

		if (!(weapon->object.parent_object_index!=NONE &&
			(unit= unit_try_and_get(weapon->object.parent_object_index))!=NULL &&
			unit->definition_index!=NONE &&
			TEST_FLAG(unit_definition_get(unit->definition_index)->unit.flags, _unit_integrated_light_controls_weapon_directly_bit)))
		{
			weapon->weapon.integrated_light_power-= (1.0f/24.0f);
			if (weapon->weapon.integrated_light_power<0.0f)
			{
				weapon->weapon.integrated_light_power= 0.0f;
			}
		}
	}

	if (weapon->weapon.heat>0.0f)
	{
		if (weapon->weapon.heat>=weapon_definition->weapon.heat_overheated_threshold && !TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit))
		{
			SET_FLAG(weapon->weapon.flags, _weapon_overheated_bit, TRUE);
			if (weapon_definition->weapon.weapon_type==_weapon_type_plasma_pistol && TEST_FLAG(weapon->weapon.flags, _weapon_overheat_recoil_bit))
			{
				SET_FLAG(weapon->weapon.flags, _weapon_overheat_recoil_bit, FALSE);
				first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_overheating_super_recoil);
			}
			else
			{
				first_person_weapon_message_from_weapon(weapon_index, _first_person_weapon_message_overheating);
			}
			weapon->weapon.overheated_effect_index= weapon_effect_looping_new(weapon_index, weapon_definition->weapon.overheated_effect.index);
		}

		if (weapon->weapon.overcharged==0.0f)
		{
			real heat_loss= weapon_definition->weapon.heat_loss_per_second*(1.0f/TICKS_PER_SECOND);

			if (weapon_definition->weapon.age_heat_recovery_penalty>0.0f)
			{
				heat_loss*= 1.0f-weapon->weapon.age*weapon_definition->weapon.age_heat_recovery_penalty;
			}

			weapon->weapon.heat-= heat_loss;
			if (weapon->weapon.heat<0.0f)
			{
				weapon->weapon.heat= 0.0f;
			}

			if (TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit) &&
				!TEST_FLAG(weapon->weapon.flags, _weapon_overheated_exit_bit) &&
				(weapon->weapon.heat-weapon_definition->weapon.heat_recovery_threshold)/heat_loss<=1.0f)
			{
				SET_FLAG(weapon->weapon.flags, _weapon_overheated_exit_bit, TRUE);
			}
		}

		if (TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit) && weapon->weapon.heat<weapon_definition->weapon.heat_recovery_threshold)
		{
			weapon->weapon.flags&= ~(FLAG(_weapon_overheated_bit)|FLAG(_weapon_overheated_exit_bit));
			if (weapon->weapon.overheated_effect_index!=NONE)
			{
				effect_stop(weapon->weapon.overheated_effect_index, TRUE);
			}
		}
	}

	weapon->weapon.overcharged= 0.0f;
	if (weapon->weapon.state_timer>0)
	{
		weapon->weapon.state_timer--;
	}

	if (!TEST_FLAG(weapon->weapon.control_flags, _weapon_control_user_busy_bit) && weapon->weapon.state_timer<=0)
	{
		triggers_down[0]= TEST_FLAG(weapon->weapon.control_flags, _weapon_control_primary_trigger_bit);
		triggers_down[1]= TEST_FLAG(weapon_definition->weapon.flags, _weapon_secondary_trigger_overrides_grenades_bit) && TEST_FLAG(weapon->weapon.control_flags, _weapon_control_secondary_trigger_bit);
	}
	else
	{
		triggers_down[0]= FALSE;
		triggers_down[1]= FALSE;
	}

	switch (weapon_definition->weapon.secondary_trigger_mode)
	{
	case _weapon_secondary_trigger_slaved_to_primary:
		if (triggers_down[1] && weapon_definition->weapon.triggers.count>0 && weapon->weapon.triggers[0].rate_of_fire!=1.0f)
		{
			triggers_down[1]= FALSE;
		}
		break;

	case _weapon_secondary_trigger_inhibits_primary:
		if (triggers_down[1])
		{
			triggers_down[0]= FALSE;
		}
		break;
	}

	if (TEST_FLAG(weapon->weapon.control_flags, _weapon_control_reload_bit) && weapon_definition->weapon.magazines.count>0)
	{
		SET_FLAG(weapon->weapon.flags, _weapon_needs_to_reload_bit, TRUE);
	}
	if (TEST_FLAG(weapon->weapon.flags, _weapon_needs_to_reload_bit))
	{
		weapon_magazine_start_reload(weapon_index, 0, TRUE);
	}

	for (magazine_index= 0; magazine_index<weapon_definition->weapon.magazines.count; magazine_index++)
	{
		struct weapon_magazine *magazine= weapon_magazine_get(weapon, magazine_index);
		struct weapon_magazine_definition *magazine_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.magazines, magazine_index, struct weapon_magazine_definition);

		if (magazine_definition->rounds_recharged_per_second>0 && magazine->rounds_loaded<magazine_definition->rounds_loaded_maximum)
		{
			short rounds_recharged= magazine_definition->rounds_recharged_per_second/TICKS_PER_SECOND;
			short fractional_rounds_recharged= magazine_definition->rounds_recharged_per_second%TICKS_PER_SECOND;

			magazine->rounds_loaded+= rounds_recharged;
			magazine->rounds_fractional_recharged+= fractional_rounds_recharged;
			if (magazine->rounds_fractional_recharged>=TICKS_PER_SECOND)
			{
				magazine->rounds_loaded++;
				magazine->rounds_fractional_recharged-= TICKS_PER_SECOND;
			}
			if (magazine->rounds_loaded>magazine_definition->rounds_loaded_maximum)
			{
				magazine->rounds_loaded= magazine_definition->rounds_loaded_maximum;
			}
		}

		if (magazine->state_timer)
		{
			magazine->state_timer--;
		}

		switch (magazine->state)
		{
		case _magazine_reloading:
			if (magazine->state_timer-1<=0)
			{
				weapon_magazine_finish_reload(weapon_index, magazine_index);
			}
			break;

		case _magazine_unchambered:
			weapon_magazine_start_chamber(weapon_index, magazine_index);
			break;

		case _magazine_chambering:
			if (!magazine->state_timer)
			{
				weapon_magazine_finish_chamber(weapon_index, magazine_index);
			}
			break;
		}
	}

	for (trigger_index= 0; trigger_index<weapon_definition->weapon.triggers.count; trigger_index++)
	{
		struct weapon_trigger *trigger= weapon_trigger_get(weapon, trigger_index);
		struct weapon_trigger_definition *trigger_definition= TAG_BLOCK_GET_ELEMENT(&weapon_definition->weapon.triggers, trigger_index, struct weapon_trigger_definition);

		if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_analog_rate_of_fire_bit) && TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit))
		{
			triggers_down[trigger_index]= weapon->weapon.primary_trigger>0.05f;
		}
		if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_sticks_when_dropped_bit) && weapon->object.parent_object_index==NONE)
		{
			triggers_down[trigger_index]= TRUE;
		}

		if (trigger->state_timer)
		{
			trigger->state_timer--;
		}

		if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_toggles_bit))
		{
			if (!TEST_FLAG(trigger->flags, _weapon_trigger_was_down_bit) && triggers_down[trigger_index])
			{
				trigger->flags^= FLAG(_weapon_trigger_toggled_bit);
			}
			SET_FLAG(trigger->flags, _weapon_trigger_was_down_bit, triggers_down[trigger_index]);
			triggers_down[trigger_index]= TEST_FLAG(trigger->flags, _weapon_trigger_toggled_bit);
		}

		if (!triggers_down[trigger_index])
		{
			SET_FLAG(trigger->flags, _weapon_trigger_released_since_last_shot_bit, TRUE);
		}

		if (trigger->ejection_port_position>0.0f)
		{
			trigger->ejection_port_position-= trigger_definition->runtime_ejection_port_recovery_time;
			if (trigger->ejection_port_position<=0.0f)
			{
				trigger->ejection_port_position= 0.0f;
			}
		}

		if (trigger->illumination>0.0f)
		{
			trigger->illumination-= trigger_definition->runtime_illumination_recovery_time;
			if (trigger->illumination<=0.0f)
			{
				trigger->illumination= 0.0f;
			}
		}

		switch (trigger->state)
		{
		case _trigger_idle:
			if (!TEST_FLAG(weapon->weapon.control_flags, _weapon_control_user_busy_bit) &&
				weapon->object.parent_object_index!=NONE &&
				trigger_definition->magazine_index!=NONE)
			{
				struct weapon_magazine *magazine= weapon_magazine_get(weapon, trigger_definition->magazine_index);

				if ((magazine->rounds_loaded<trigger_definition->rounds_per_shot && !TEST_FLAG(trigger_definition->flags, _weapon_trigger_can_fire_with_partial_ammunition_bit)) ||
					magazine->rounds_loaded<trigger_definition->minimum_rounds_loaded_per_shot ||
					magazine->rounds_loaded==0)
				{
					weapon_magazine_start_reload(weapon_index, trigger_definition->magazine_index, TRUE);
				}
			}

			if (triggers_down[trigger_index] && weapon_trigger_can_fire_again(weapon_index, trigger_index))
			{
				weapon_trigger_begin_firing(weapon_index, trigger_index, FALSE);
			}
			else if (trigger->idle_ticks<127)
			{
				trigger->idle_ticks++;
			}
			break;

		case _trigger_spewing:
			if (trigger->state_timer)
			{
				weapon_trigger_begin_firing(weapon_index, trigger_index, TRUE);
			}
			else
			{
				weapon_trigger_recover(weapon_index, trigger_index);
			}
			break;

		case _trigger_overloading:
			if (!triggers_down[trigger_index])
			{
				weapon_trigger_begin_firing(weapon_index, trigger_index, TRUE);
			}
			else if (!trigger->state_timer && weapon->weapon.alternate_shots_loaded<weapon_definition->weapon.maximum_alternate_shots_loaded)
			{
				weapon_trigger_overload(weapon_index, trigger_index);
			}
			break;

		case _trigger_charging:
			if (trigger->state_timer)
			{
				if (!triggers_down[trigger_index])
				{
					if (trigger_index==0 && weapon_definition->weapon.triggers.count>1 && !TEST_FLAG(trigger->flags, _weapon_trigger_fired_before_charging_bit))
					{
						weapon_trigger_begin_firing(weapon_index, trigger_index, TRUE);
					}
					else
					{
						weapon_trigger_idle(weapon_index, trigger_index);
					}

					if (trigger->charging_effect_index!=NONE)
					{
						effect_stop(trigger->charging_effect_index, TRUE);
						trigger->charging_effect_index= NONE;
					}
				}
			}
			else
			{
				weapon_trigger_fully_charged(weapon_index, trigger_index);
			}
			break;

		case _trigger_charged:
			if (triggers_down[trigger_index])
			{
				weapon->weapon.overcharged= 1.0f-(trigger->state_timer*(1.0f/TICKS_PER_SECOND))/trigger_definition->charged_time;
				if (trigger->state_timer)
				{
					struct weapon_magazine *magazine= weapon_magazine_get(weapon, trigger_definition->magazine_index);

					if (magazine->rounds_loaded<trigger_definition->rounds_per_shot && !TEST_FLAG(trigger_definition->flags, _weapon_trigger_can_fire_with_partial_ammunition_bit))
					{
						weapon_trigger_release_charge(weapon_index, trigger_index);
					}
				}
				else
				{
					weapon_trigger_overcharged(weapon_index, trigger_index);
				}
			}
			else
			{
				weapon_trigger_release_charge(weapon_index, trigger_index);
			}
			break;

		case _trigger_recovering:
			if (!trigger->state_timer)
			{
				if (TEST_FLAG(trigger_definition->flags, _weapon_trigger_latched_bit) &&
					TEST_FLAG(weapon->item.flags, _item_belongs_to_player_bit) &&
					!TEST_FLAG(trigger->flags, _weapon_trigger_released_since_last_shot_bit))
				{
					weapon_trigger_locked(weapon_index, trigger_index);
				}
				else
				{
					weapon_trigger_idle(weapon_index, trigger_index);
				}
			}
			break;

		case _trigger_tracking:
			if (!triggers_down[trigger_index] || weapon->weapon.tracked_object_index==NONE)
			{
				weapon_trigger_finish_tracking(weapon_index, trigger_index);
			}
			break;

		case _trigger_locked:
			if (!triggers_down[trigger_index])
			{
				weapon_trigger_idle(weapon_index, trigger_index);
			}
			break;

		case _trigger_uninitialized:
			if (!trigger->state_timer)
			{
				weapon_trigger_idle(weapon_index, trigger_index);
			}
			break;

		default:
			match_vassert("c:\\halo\\SOURCE\\items\\weapons.c", 778, FALSE, NULL);
			break;
		}

		if (triggers_down[trigger_index])
		{
			trigger->rate_of_fire+= trigger_definition->runtime_rate_of_fire_acceleration_time;
			if (trigger->rate_of_fire>1.0f)
			{
				trigger->rate_of_fire= 1.0f;
			}

			if (trigger_definition->blurred_rate_of_fire!=0.0f &&
				!TEST_FLAG(trigger->flags, _weapon_trigger_blurred_bit) &&
				trigger->rate_of_fire>trigger_definition->blurred_rate_of_fire)
			{
				object_permute_region(weapon_get_effect_object_index(weapon_index), data_00307140.blurred_permutation_names[trigger_index], NONE, TRUE);
				SET_FLAG(trigger->flags, _weapon_trigger_blurred_bit, TRUE);
			}
		}
		else
		{
			trigger->rate_of_fire-= trigger_definition->runtime_rate_of_fire_deceleration_time;
			if (trigger->rate_of_fire<0.0f)
			{
				trigger->rate_of_fire= 0.0f;
			}

			if (TEST_FLAG(trigger->flags, _weapon_trigger_blurred_bit) && trigger->rate_of_fire<trigger_definition->blurred_rate_of_fire)
			{
				object_permute_region(weapon_get_effect_object_index(weapon_index), data_00307140.blurred_permutation_names[trigger_index], NONE, FALSE);
				SET_FLAG(trigger->flags, _weapon_trigger_blurred_bit, FALSE);
			}
		}

		if (trigger->state==_trigger_spewing || trigger->state==_trigger_recovering || triggers_down[trigger_index])
		{
			trigger->error+= trigger_definition->runtime_error_acceleration_time;
			if (trigger->error>1.0f)
			{
				trigger->error= 1.0f;
			}
		}
		else
		{
			trigger->error-= trigger_definition->runtime_error_deceleration_time;
			if (trigger->error<0.0f)
			{
				trigger->error= 0.0f;
			}
		}
	}

	profile_exit(data_00307140.update_profile);

	return TRUE;
}
