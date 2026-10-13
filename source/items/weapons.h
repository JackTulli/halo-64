/*
WEAPONS.H

header included in hcex build.
*/

#ifndef __WEAPONS_H
#define __WEAPONS_H
#pragma once

/* ---------- headers */

#include "items.h"

/* ---------- constants */

enum
{
	_weapon_state_idle = 0,
	_weapon_state_primary_recoil,
	_weapon_state_secondary_recoil,
	_weapon_state_primary_chamber,
	_weapon_state_secondary_chamber,
	_weapon_state_primary_reload,
	_weapon_state_secondary_reload,
	_weapon_state_primary_charged,
	_weapon_state_secondary_charged,
	_weapon_state_ready,
	_weapon_state_put_away,
	NUMBER_OF_WEAPON_STATES,
};

enum
{
	_trigger_idle = 0,
	_trigger_overloading,
	_trigger_charging,
	_trigger_charged,
	_trigger_recovering,
	_trigger_tracking,
	_trigger_spewing,
	_trigger_locked,
	_trigger_uninitialized,
	NUMBER_OF_TRIGGER_STATES,
};

enum
{
	_magazine_idle = 0,
	_magazine_reloading,
	_magazine_unchambered,
	_magazine_chambering,
	NUMBER_OF_WEAPON_MAGAZINE_STATES,
};

enum
{
	_weapon_runtime_game_engine_active_bit = 5,
};

enum
{
	_weapon_control_integrated_light_bit = 0,
	_weapon_control_primary_trigger_bit,
	_weapon_control_secondary_trigger_bit,
	_weapon_control_reload_bit,
	_weapon_control_user_busy_bit,
	_weapon_control_user_switching_weapons_bit,
	_weapon_control_zoomed_bit,
	NUMBER_OF_WEAPON_CONTROL_FLAGS,
};

enum
{
	_first_person_weapon_message_primary_fire = 0,
	_first_person_weapon_message_secondary_fire,
	_first_person_weapon_message_primary_misfire,
	_first_person_weapon_message_secondary_misfire,
	_first_person_weapon_message_melee,
	_first_person_weapon_message_light_on,
	_first_person_weapon_message_light_off,
	_first_person_weapon_message_shotgun_enter_reload,
	_first_person_weapon_message_shotgun_exit_reload,
	_first_person_weapon_message_reload_while_empty,
	_first_person_weapon_message_reload_while_full,
	_first_person_weapon_message_put_away,
	_first_person_weapon_message_ready,
	_first_person_weapon_message_drop,
	_first_person_weapon_message_charged,
	_first_person_weapon_message_overheating,
	_first_person_weapon_message_overheating_super_recoil,
	_first_person_weapon_message_throw_grenade,
	NUMBER_OF_FIRST_PERSON_WEAPON_MESSAGES,
};

enum weapon_type
{
	_weapon_type_undefined = 0,
	_weapon_type_shotgun,
	_weapon_type_needler,
	_weapon_type_plasma_pistol,
	_weapon_type_plasma_rifle,
	NUMBER_OF_WEAPON_TYPES,
};

enum shotgun_reload_type
{
	_shotgun_reload_type_first_round = 0,
	_shotgun_reload_type_last_round,
	_shotgun_reload_type_first_and_last_round,
	NUMBER_OF_SHOTGUN_RELOAD_TYPES,
};

enum weapon_first_person_animation_time_mode
{
	_weapon_first_person_animation_time_frame_count = 0,
	_weapon_first_person_animation_time_private_key_frame,
	NUMBER_OF_WEAPON_FIRST_PERSON_ANIMATION_TIME_MODES,
};

enum
{
	_first_person_weapon_animation_idle = 0,
	_first_person_weapon_animation_posing,
	_first_person_weapon_animation_primary_fire,
	_first_person_weapon_animation_moving,
	_first_person_weapon_animation_overlays,
	_first_person_weapon_animation_light_on,
	_first_person_weapon_animation_light_off,
	_first_person_weapon_animation_reload_while_empty,
	_first_person_weapon_animation_reload_while_full,
	_first_person_weapon_animation_overheated,
	_first_person_weapon_animation_ready,
	_first_person_weapon_animation_put_away,
	_first_person_weapon_animation_overcharged,
	_first_person_weapon_animation_melee,
	_first_person_weapon_animation_secondary_fire,
	_first_person_weapon_animation_overcharged_jitter,
	_first_person_weapon_animation_throw_grenade,
	_first_person_weapon_animation_ammunition,
	_first_person_weapon_animation_primary_misfire,
	_first_person_weapon_animation_secondary_misfire,
	_first_person_weapon_animation_throw_grenade_overheated,
	_first_person_weapon_animation_overheating,
	_first_person_weapon_animation_overheating_again,
	_first_person_weapon_animation_shotgun_enter,
	_first_person_weapon_animation_shotgun_exit_empty,
	_first_person_weapon_animation_shotgun_exit_full,
	_first_person_weapon_animation_overheated_exit,
	_first_person_weapon_animation_overheated_supercharge_enter,
	NUMBER_OF_FIRST_PERSON_WEAPON_ANIMATIONS,
};

/* ---------- macros */

#define weapon_get(index)			((struct weapon_datum*)object_get_and_verify_type(index, _object_mask_weapon))
#define weapon_try_and_get(index)	((struct weapon_datum*)object_try_and_get_and_verify_type(index, _object_mask_weapon))

/* ---------- structures */

struct weapon_trigger
{
	char idle_ticks;
	char state;
	short state_timer;
	unsigned long flags;
	word firing_effects_used_flags;
	short firing_effect_index;
	short firing_effect_shots_remaining;
	short sequential_non_tracer_rounds;
	real rate_of_fire;
	real ejection_port_position;
	real illumination;
	real error;
	long charging_effect_index;
};

struct weapon_magazine
{
	short state;
	short state_timer;
	short original_time;
	short rounds_total;
	short rounds_loaded;
	short rounds_fractional_recharged;
};

struct _weapon_datum
{
	unsigned long flags;
	word control_flags;
	real primary_trigger;
	char state;
	char last_reported_state;
	short state_timer;
	real heat;
	real age;
	real overcharged;
	real integrated_light_power;
	char integrated_light_delay_ticks;
	long tracked_object_index;
	real recoil_angular_velocity;
	short recoil_recovery_time;
	short shots_until_demotion;
	short alternate_shots_loaded;
	struct weapon_trigger triggers[2];
	struct weapon_magazine magazines[2];
	struct animation_state animation;
	long overheated_effect_index;
	long game_time_last_fired;
};

struct weapon_datum
{
	long definition_index;
	struct _object_datum object;
	struct _item_datum item;
	struct _weapon_datum weapon;
};

/* ---------- prototypes/WEAPONS.C */

short weapon_rotate_zoom_level(
	long weapon_index,
	short zoom_level);

void weapons_initialize(
	void);
void weapons_initialize_for_new_map(
	void);
void weapons_dispose_from_old_map(
	void);
void weapons_dispose(
	void);

boolean weapon_must_be_readied(long weapon_index);
boolean weapon_overcharged(
	long weapon_index);
real weapon_get_zoom_magnification(
	long weapon_index,
	short zoom_level);
real weapon_get_field_of_view(
	long weapon_index,
	real field_of_view,
	short zoom_level);
boolean weapon_is_flag(long weapon_index);

char const *weapon_get_label(long weapon_index);

void weapon_ready(long weapon_index);
boolean weapon_handle_potential_inventory_item(
	long weapon_index,
	long item_object_index,
	short local_player_index,
	short *rounds_picked_up);
void weapon_unready(long weapon_index);
boolean weapon_put_away(long weapon_index, boolean immediate);

void weapon_set_integrated_light_power(long weapon_index, real light_power);

void weapon_owner_update(long weapon_index, word control_flags, real primary_trigger);

struct weapon_interface_state;

boolean weapon_new(long weapon_index);
void weapon_export_function_values(
	long weapon_index);

void weapon_set_total_rounds(long weapon_index, short *rounds_array);
void weapon_set_current_amount(
	long weapon_index,
	real current_amount);

void weapon_build_weapon_interface_state(long weapon_index, struct weapon_interface_state *state);

boolean weapon_can_be_fired(long weapon_index);

short weapon_get_first_person_animation_time(long weapon_index, short mode, short animation_type, short shotgun_reload_type);
void weapon_stop_reload(
	long weapon_index);
boolean weapon_prevents_grenade_throwing(
	long weapon_index);
boolean weapon_prevents_melee_attack(
	long weapon_index);

boolean weapon_aim(
	long weapon_index,
	short trigger_index,
	real_point3d const *origin,
	real_point3d const *target_point,
	boolean lob,
	real_vector3d *result_aim_vector,
	real *result_ticks,
	real *result_distance,
	boolean *result_linear);
real weapon_estimate_time_to_target(
	long weapon_index,
	short trigger_index,
	real distance);

/* ---------- globals */

/* ---------- public code */

#endif // __WEAPONS_H
