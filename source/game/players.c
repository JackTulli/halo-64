/*
PLAYERS.C

symbols in this file:
000A9470 0070:
	_players_initialize (0000)
000A94E0 00b0:
	_players_initialize_for_new_map (0000)
000A9590 0020:
	_players_dispose_from_old_map (0000)
000A95B0 0030:
	_players_dispose (0000)
000A95E0 0020:
	_machine_get_player_list (0000)
000A9600 0050:
	_machine_add_player (0000)
000A9650 0060:
	_local_player_exists (0000)
000A96B0 0050:
	_find_unused_local_player_index (0000)
000A9700 0020:
	_player_delete (0000)
000A9720 0010:
	_players_get_respawn_failure (0000)
000A9730 0050:
	_local_player_get_player_index (0000)
000A9780 00a0:
	_local_player_set_player_index (0000)
000A9820 0010:
	_local_player_count (0000)
000A9830 0040:
	_local_player_get_next (0000)
000A9870 0050:
	_player_index_from_unit_index (0000)
000A98C0 0090:
	_player_died (0000)
000A9950 0010:
	_players_are_all_dead (0000)
000A9960 00c0:
	_players_set_local_player_unit (0000)
000A9A20 0010:
	_players_get_combined_pvs_local (0000)
000A9A30 0010:
	_players_get_combined_pvs (0000)
000A9A40 0020:
	_player_input_enable (0000)
000A9A60 0020:
	_player_input_enabled (0000)
000A9A80 00f0:
	_any_player_is_in_the_air (0000)
000A9B70 0050:
	_any_player_is_dead (0000)
000A9BC0 0040:
	_is_player_in_trigger (0000)
000A9C00 00e0:
	_player_pseudo_kill (0000)
000A9CE0 0120:
	_player_control_fix_for_loaded_game_state (0000)
000A9E00 0050:
	_player_get_starting_location_count (0000)
000A9E50 00c0:
	_player_get_starting_location (0000)
000A9F10 0070:
	_placement_data_set_change_color (0000)
000A9F80 0070:
	_create_weapon (0000)
000A9FF0 0170:
	_players_compute_combined_pvs (0000)
000AA160 0020:
	_players_compute_local_player_count (0000)
000AA180 0010:
	_player_examine_nearby_unit (0000)
000AA190 0090:
	_unit_should_autopick_weapon (0000)
000AA220 0020:
	_player_reset_action_result (0000)
000AA240 00c0:
	_player_handle_weapon_swap (0000)
000AA300 00b0:
	_player_over_shield_screen_effect (0000)
000AA3B0 00b0:
	_player_active_camo_screen_effect (0000)
000AA460 0090:
	_player_health_pack_screen_effect (0000)
000AA4F0 0040:
	_player_powerup_on (0000)
000AA530 0030:
	_player_powerup_additional (0000)
000AA560 0030:
	_player_powerup_off (0000)
000AA590 0070:
	_players_handle_deleted_object (0000)
000AA600 0020:
	_random_direction3d (0000)
000AA620 0040:
	_valid_real_vector2d (0000)
000AA660 0120:
	_player_new (0000)
000AA780 0150:
	_player_add_equipment (0000)
000AA8D0 0110:
	_player_aiming_vector_from_facing (0000)
000AA9E0 0510:
	_player_teleport_internal (0000)
000AAEF0 0060:
	_player_teleport (0000)
000AAF50 00d0:
	_find_best_starting_location_index (0000)
000AB020 0330:
	_player_spawn (0000)
000AB350 00f0:
	_player_set_action_result (0000)
000AB440 0250:
	_player_handle_action (0000)
000AB690 00f0:
	_player_handle_powerup (0000)
000AB780 00a0:
	_player_handle_powerup_minor (0000)
000AB820 0070:
	_player_update_powerups (0000)
000AB890 01a0:
	_players_debug_render (0000)
000ABA30 0090:
	_debug_player_teleport (0000)
000ABAC0 01d0:
	_players_respawn_coop (0000)
000ABC90 0140:
	_player_teleport_on_bsp_switch (0000)
000ABDD0 02e0:
	_players_reconnect_to_structure_bsp (0000)
000AC0B0 01c0:
	_player_examine_nearby_vehicle (0000)
000AC270 00b0:
	_player_examine_nearby_device (0000)
000AC320 0150:
	_player_handle_powerup_equipment (0000)
000AC470 06e0:
	_players_update_before_game (0000)
000ACB50 0320:
	_player_examine_nearby_item (0000)
000ACE70 00f0:
	_player_examine_nearby_objects (0000)
000ACF60 0330:
	_players_update_after_game (0000)
0025CED8 006c:
	_adjustment_weights (0000)
0025CF44 001a:
	??_C@_0BK@OOAKICCI@players_update_after_game?$AA@ (0000)
0025CF60 001b:
	??_C@_0BL@FKGHDGFM@players_update_before_game?$AA@ (0000)
0025CF7C 0010:
	??_C@_0BA@CEEHCBBD@players?5globals?$AA@ (0000)
0025CF8C 0006:
	??_C@_05EINLALJG@teams?$AA@ (0000)
0025CF94 0008:
	??_C@_07BCGOFIOM@players?$AA@ (0000)
0025CF9C 001a:
	??_C@_0BK@CGPNFENM@failed?5to?5create?5a?5player?$AA@ (0000)
0025CFB8 001e:
	??_C@_0BO@IAEAGICD@c?3?2halo?2SOURCE?2game?2players?4c?$AA@ (0000)
0025CFD8 004f:
	??_C@_0EP@PCIBAKOM@local_player_index?$DO?$DNNONE?5?$CG?$CG?5loca@ (0000)
0025D028 009b:
	??_C@_0JL@EEPEKCFO@tried?5to?5fix?5broken?5player?5contr@ (0000)
0025D0C8 0069:
	??_C@_0GJ@LOLKJFPO@failed?5to?5correct?5player?5control@ (0000)
0025D134 0031:
	??_C@_0DB@FKMNJOMP@corrected?5player?5control?5for?5res@ (0000)
0025D168 0070:
	??_C@_0HA@DONIDEOA@?$CI?$CIlocal_player_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIloc@ (0000)
0025D1D8 002b:
	??_C@_0CL@OKOJEGMM@Could?5not?5attach?5starting?5weapon@ (0000)
0025D204 0021:
	??_C@_0CB@EBNMJKAL@player?9?$DOlocal_player_index?$CB?$DNNONE@ (0000)
0025D228 002f:
	??_C@_0CP@BNOOKFKL@couldn?8t?5teleport?5player?5into?5a?5@ (0000)
0025D258 0019:
	??_C@_0BJ@LHNHGGK@player?9?$DOunit_index?$CB?$DNNONE?$AA@ (0000)
0025D274 0006:
	??_C@_05MHMEBBGO@scale?$AA@ (0000)
0025D27C 0025:
	??_C@_0CF@PKKNMABJ@magnitude3d?$CI?$CGbest_adjustment_vec@ (0000)
0025D2A4 0032:
	??_C@_0DC@CLGKJEDK@source_unit_index?$DN?$DNNONE?5?$HM?$HM?5local@ (0000)
0025D2D8 0008:
	__real@3fe6a09e60000000 (0000)
0025D2E0 003a:
	??_C@_0DK@GFFNPIMP@powerup_type?$DO?$DN0?5?$CG?$CG?5powerup_type?$DM@ (0000)
0025D31C 0016:
	??_C@_0BG@HMDKOAOM@no?5players?5in?5the?5bsp?$AA@ (0000)
0025D334 0013:
	??_C@_0BD@ELAHGGCE@seat_index?5?$CB?$DN?5NONE?$AA@ (0000)
0025D348 0046:
	??_C@_0EG@POGHKPOA@?$CINONE?5?$DN?$DN?5control_data?4zoom_level@ (0000)
0025D390 008c:
	??_C@_0IM@DKBPHNBM@?$CINONE?5?$DN?$DN?5control_data?4grenade_in@ (0000)
0025D420 0085:
	??_C@_0IF@OCHKLACI@?$CINONE?5?$DN?$DN?5control_data?4weapon_ind@ (0000)
0025D4A8 004c:
	??_C@_0EM@ECBIAFIL@?$CINONE?5?$DN?$DN?5action?9?$DOdesired_zoom_le@ (0000)
0025D4F8 0095:
	??_C@_0JF@HCBNBPOG@?$CINONE?5?$DN?$DN?5action?9?$DOdesired_grenade@ (0000)
0025D590 008e:
	??_C@_0IO@DMKDCDID@?$CINONE?5?$DN?$DN?5action?9?$DOdesired_weapon_@ (0000)
0025D620 0018:
	??_C@_0BI@GKAKFFBF@action?9?$DOprimary_trigger?$AA@ (0000)
0025D638 0027:
	??_C@_0CH@OILDLPCJ@?$CFs?3?5assert_valid_real_vector2d?$CI?$CF@ (0000)
0025D660 0012:
	??_C@_0BC@MPDBBHIH@?$CGaction?9?$DOthrottle?$AA@ (0000)
0025D674 001b:
	??_C@_0BL@PMBMFGJI@action?9?$DOdesired_facing?4yaw?$AA@ (0000)
0025D690 001d:
	??_C@_0BN@NJOPCJPO@action?9?$DOdesired_facing?4pitch?$AA@ (0000)
0025D6B0 0042:
	??_C@_0EC@JMPLFNIK@action_index?$DO?$DN0?5?$CG?$CG?5action_index?$DM@ (0000)
0025D6F4 003b:
	??_C@_0DL@NOPAIALJ@?$CB?$CB?$CBWARNING?$CB?$CB?$CB?5teleported?5player?5@ (0000)
0025D730 0004:
	__real@3c360b61 (0000)
002DEE08 0c18:
	_players_static_data (0000)
00453408 005d:
	_machine_to_player_table (0000)
	_debug_render_player_teleport (005c)
*/

/* ---------- headers */

#include "cseries.h"
#ifdef HALO_N64
#include "../../port/n64/src/n64.h"
#endif
#include "cseries/profile.h"
#include "ai/ai.h"
#include "ai/ai_debug.h"
#include "ai/ai_scenario_definitions.h"
#include "camera/observer.h"
#include "data.h"
#include "devices/devices.h"
#include "editor/editor_stubs.h"
#include "effects/effects.h"
#include "effects/player_effects.h"
#include "game.h"
#include "game/game_globals.h"
#include "game_engine.h"
#include "input.h"
#include "interface/hud.h"
#include "interface/hud_messaging.h"
#include "interface/hud_unit.h"
#include "interface/hud_weapon.h"
#include "interface/player_ui.h"
#include "interface/ui_widget.h"
#include "items/equipment.h"
#include "items/equipment_definitions.h"
#include "items/projectiles.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "main/main.h"
#include "objects/damage.h"
#include "objects/damage_effect_definitions.h"
#include "physics/collisions.h"
#include "real_math.h"
#include "render/render_debug.h"
#include "players.h"
#include "player_queues_new.h"
#include "player_control.h"
#include "objects/objects.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "structures/structure_bsp_definitions.h"
#include "tag_files/tag_groups.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "units/biped_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

#ifdef HALO_LINUX
/* network_game_globals.c's */
boolean network_game_distributed_client(void);
/* port/linux/game/network_distributed.c's */
void network_distributed_player_picked_up(long player_index, short kind, long definition_index, short count);
/* game_sound.c's */
long unspatialized_impulse_sound_new(long sound_definition_index, real scale);

/* what a player picked up, for the distributed netcode's client whose
player it is: the host decides the pickup, the client shows it
(network_player_show_pickup) */
enum
{
	_network_pickup_weapon,
	_network_pickup_ammunition,
	_network_pickup_grenade,
	_network_pickup_equipment,
	_network_pickup_powerup,
};

#define player_network_picked_up(player, player_index, kind, definition_index, count) \
	if ((player)->local_player_index == NONE) \
		network_distributed_player_picked_up(player_index, kind, definition_index, count)

static void network_player_log_idle_action(long player_index, unsigned long control_flags);
boolean network_game_distributed(void);

/* whether this machine decides pickups: not a client of the distributed
netcode, whose players' weapons, grenades and power-ups are the host's
(port/linux/game/network_distributed.c) */
#define players_decide_pickups() (!network_game_distributed_client())
#else
#define player_network_picked_up(player, player_index, kind, definition_index, count)
#define players_decide_pickups() TRUE
#endif

/* ---------- constants */

enum
{
#ifdef HALO_LINUX
	/* the native builds' session limits (port/linux/include/halo_port_limits.h) */
	NETWORK_GAME_MAXIMUM_PLAYER_COUNT = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
	MAXIMUM_NETWORK_MACHINE_COUNT = HALO_PORT_MAXIMUM_NETWORK_MACHINES,
#else
	NETWORK_GAME_MAXIMUM_PLAYER_COUNT = 16,
	MAXIMUM_NETWORK_MACHINE_COUNT = 4,
#endif
	MULTIPLAYER_GAME_TEXT_YOU_WERE_TELEFRAGGED = 183,
	_collision_test_for_player_teleport_flags =
		FLAG(_collision_test_front_facing_surfaces_bit) |
		FLAG(_collision_test_ignore_invisible_surfaces_bit) |
		FLAG(_collision_test_structure_bit) |
		FLAG(_collision_test_objects_scenery_bit),
	_object_mask_player_interaction =
		_object_mask_unit |
		_object_mask_item |
		_object_mask_control
};

/* ---------- macros */

/* ---------- structures */

/* These scenario records are consumed only by Players.  The broader scenario
   header intentionally cannot own them yet: older consumers still carry their
   own incompatible partial definitions. */
struct scenario_bsp_switch_trigger_volume
{
	short trigger_volume_index;
	short source_structure_bsp_index;
	short destination_structure_bsp_index;
	short cutscene_flag_index;
};

struct scenario_cutscene_flag
{
	long runtime_unused;
	char name[TAG_STRING_LENGTH];
	real_point3d position;
	real_euler_angles2d facing;
	byte unused[0x24];
};

struct unit_control_data
{
	char animation_state;
	char aiming_speed;
	word control_flags;
	short weapon_index;
	short grenade_index;
	short zoom_level;
	short pad;
	real_vector3d throttle;
	real primary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
};

typedef char unit_control_data_size_assert[
	sizeof(struct unit_control_data) == 0x40 ? 1 : -1];

/* Players needs the vehicle control prefix only.  VEHICLES.C owns the full
   runtime datum, which has not yet been made a shared definition. */
struct _players_vehicle_datum
{
	word flags;
	short unknown426;
	byte unknown_state;
	char approach_type;
	byte unknown42A;
	byte unused42B;
};

struct players_vehicle_datum
{
	long definition_index;
	struct _object_datum object;
	struct _unit_datum unit;
	struct _players_vehicle_datum vehicle;
};

struct player_screen_flash_parameters
{
	short type;
	word pad;
	real maximum_intensity;
	real red;
	real green;
	real duration;
};

struct players_static_data
{
	struct profile_section update_before_game_profile;
	struct profile_section update_after_game_profile;
	struct player_screen_flash_parameters screen_flash_parameters[2];
};

typedef char players_static_data_size_assert[
	sizeof(struct players_static_data) == 0xC18 ? 1 : -1];
typedef char players_static_data_screen_flash_offset_assert[
	offsetof(struct players_static_data, screen_flash_parameters) == 0xBF0 ? 1 : -1];

/* ---------- prototypes */

static long is_player_in_trigger(
	short bsp_switch_trigger_volume_index,
	long object_index);
static void player_reset_action_result(
	long player_index);
static boolean player_handle_weapon_swap(
	long player_index);
static void player_over_shield_screen_effect(
	long player_index);
static void player_active_camo_screen_effect(
	long player_index);
static void player_health_pack_screen_effect(
	long player_index);
static void player_powerup_on(
	long player_index,
	short powerup_type);
static void player_powerup_additional(
	long player_index,
	short powerup_type);
static void player_powerup_off(
	long player_index,
	short powerup_type);
static void player_set_action_result(
	long player_index,
	short action_result,
	long object_index,
	short seat_index);
static boolean player_handle_action(
	long player_index);
static void player_update_powerups(
	long player_index);
static void player_examine_nearby_vehicle(
	long player_index,
	long vehicle_index);
static void player_examine_nearby_item(
	long player_index,
	long item_index);
static void player_examine_nearby_objects(
	long player_index);
static void player_teleport_on_bsp_switch(
	long player_index,
	long source_unit_index,
	real_point3d const *position);
static void player_handle_powerup_equipment(
	long player_index,
	long equipment_index);

/* ---------- globals */

struct players_globals *players_globals;
struct data_array *team_data;
struct data_array *player_data;
static long machine_to_player_table[MAXIMUM_NETWORK_MACHINE_COUNT][MAXIMUM_LOCAL_PLAYERS] = { 0 };
static boolean players_lost_map_started = FALSE;
static short over_shield_screen_flash_fade_function = 0;
static real over_shield_screen_flash_alpha = 0.f;
static real over_shield_screen_flash_green = 0.f;
static short active_camo_screen_flash_fade_function = 0;
static real active_camo_screen_flash_alpha = 0.f;
static real active_camo_screen_flash_blue = 0.f;
boolean debug_render_player_teleport = FALSE;
static struct players_static_data players_static_data =
{
	{ "players_update_before_game", NONE, TRUE },
	{ "players_update_after_game", NONE, TRUE },
	{
		{ 5, 0, 1.0f, 0.8f, 0.8f, 2.0f },
		{ 2, 0, 1.0f, 0.35f, 0.35f, 2.0f },
	},
};

/* January owns this nine-point co-op teleport search table in Players.obj
   .rdata (0x6C bytes, no relocations); HCEA independently authenticates the
   name, element type, and values. */
static real_point3d const adjustment_weights[9] =
{
	{ { 1.f, 0.f, 0.f } },
	{ { 0.f, 1.f, 0.f } },
	{ { 0.f, -1.f, 0.f } },
	{ { 0.70710677f, -0.70710677f, 0.f } },
	{ { 0.70710677f, 0.70710677f, 0.f } },
	{ { 0.57735026f, 0.57735026f, 0.57735026f } },
	{ { 0.57735026f, 0.57735026f, -0.57735026f } },
	{ { 0.57735026f, -0.57735026f, 0.57735026f } },
	{ { 0.57735026f, -0.57735026f, -0.57735026f } },
};

#define PLAYERS_UPDATE_BEFORE_GAME_PROFILE \
	(players_static_data.update_before_game_profile)
#define PLAYERS_UPDATE_AFTER_GAME_PROFILE \
	(players_static_data.update_after_game_profile)

/* ---------- public code */

void players_initialize(
	void)
{
	player_data = game_state_data_new(
		"players",
#ifdef HALO_LINUX
		/* a player's datum index is also its action slot in every update */
		NETWORK_GAME_MAXIMUM_PLAYER_COUNT,
#else
		16,
#endif
		sizeof(struct player_datum));
	team_data = game_state_data_new(
		"teams",
		16,
		0x40);
	players_globals = (struct players_globals *)game_state_malloc(
		"players globals",
		NULL,
		sizeof(struct players_globals));

	csmemset(
		players_globals->local_players,
		NONE,
		sizeof(players_globals->local_players));
	players_globals->unknown0 = NONE;
	players_globals->local_player_count = 0;

	player_control_initialize();

	return;
}

void players_initialize_for_new_map(
	void)
{
	player_control_dispose();
	csmemset(
		players_globals,
		0,
		sizeof(struct players_globals));
	csmemset(
		players_globals->local_players,
		NONE,
		sizeof(players_globals->local_players));
	csmemset(
		players_globals->dead_units,
		NONE,
		sizeof(players_globals->dead_units));
	players_globals->unknown0 = NONE;
	players_globals->input_disabled = FALSE;
	players_globals->double_speed_ticks = 0;
	players_globals->all_dead = FALSE;
	players_globals->pending_teleport_starting_location_index = NONE;
	players_globals->respawn_failure = 0;
	data_make_valid(player_data);
	data_make_valid(team_data);
	csmemset(
		machine_to_player_table,
		NONE,
#ifdef HALO_LINUX
		sizeof(machine_to_player_table));
#else
		0x40);
#endif

	return;
}

void players_dispose_from_old_map(
	void)
{
	data_make_invalid(player_data);
	data_make_invalid(team_data);

	return;
}

void players_dispose(
	void)
{
	if (player_data)
		player_data = NULL;
	if (team_data)
		team_data = NULL;
	if (players_globals)
		players_globals = NULL;

	return;
}

long *machine_get_player_list(
	long machine_index)
{
	return machine_to_player_table[DATUM_INDEX_TO_ABSOLUTE_INDEX(machine_index)];
}

boolean local_player_exists(
	long local_player_index)
{
	boolean result;
	struct player_datum *player;
	struct data_iterator iterator;

	result = FALSE;
	data_iterator_new(
		&iterator,
		player_data);
	while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		if (player->local_player_index == local_player_index)
		{
			result = TRUE;
			break;
		}
	}

	return result;
}

long find_unused_local_player_index(
	void)
{
	long index;
	long result;

	result = NONE;
	for (index = 0; index < MAXIMUM_LOCAL_PLAYERS; index++)
	{
		if (input_has_gamepad((short)index) && !local_player_exists(index))
		{
			result = index;
			break;
		}
	}

	if (result == NONE)
	{
		for (index = 0; index < MAXIMUM_LOCAL_PLAYERS; index++)
		{
			if (!local_player_exists(index))
			{
				result = index;
				break;
			}
		}
	}

	return result;
}

void player_delete(
	long player_index)
{
	datum_delete(
		player_data,
		player_index);

	return;
}

short players_get_respawn_failure(
	void)
{
	return players_globals->respawn_failure;
}

long local_player_get_player_index(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\game\\players.c",
		943,
		local_player_index>=NONE && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	if (local_player_index == NONE)
		return NONE;

	return players_globals->local_players[local_player_index];
}

void local_player_set_player_index(
	short local_player_index,
	long player_index)
{
	long previous_player_index;

	match_assert(
		"c:\\halo\\SOURCE\\game\\players.c",
		952,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	previous_player_index = players_globals->local_players[local_player_index];
	if (previous_player_index != NONE)
		player_get(previous_player_index)->local_player_index = NONE;

	players_globals->local_players[local_player_index] = player_index;

	if (player_index != NONE)
	{
		player_get(players_globals->local_players[local_player_index])->local_player_index =
			local_player_index;
	}

	return;
}

short local_player_count(
	void)
{
	return players_globals->local_player_count;
}

short local_player_get_next(
	short local_player_index)
{
	short result;
	short index;

	result = NONE;
	for (index = 0; index < MAXIMUM_LOCAL_PLAYERS; index++)
	{
		if (players_globals->local_players[index] != NONE &&
			index > local_player_index &&
			(index < result || result == NONE))
		{
			result = index;
		}
	}

	return result;
}

long player_index_from_unit_index(
	long unit_index)
{
	long player_index;
	struct player_datum *player;
	struct data_iterator iterator;

	player_index = NONE;
	data_iterator_new(
		&iterator,
		player_data);
	while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		if (player->unit_index == unit_index)
			player_index = iterator.datum_index;
	}

	return player_index;
}

void player_died(
	long player_index)
{
	struct player_datum *player;
	struct player_datum *other_player;
	struct data_iterator iterator;

	player = player_get(player_index);
	player->dead_unit_index = player->unit_index;
	player->unit_index = NONE;
	if (player->local_player_index != NONE)
	{
		player_control_new_unit(
			player->local_player_index,
			NONE);
	}
#ifdef HALO_N64_CAMPAIGN
	/* (a campaign keeps no statistics, game_engine_can_score: counted here,
	or players_update_before_game would spawn the dead player at the level's
	start at once -- the campaign cartridge brings it back itself,
	n64_player_respawn_at) */
	if (!game_engine_running())
		player->statistics.deaths++;
#endif

	players_globals->all_dead = TRUE;
	data_iterator_new(
		&iterator,
		player_data);
	while ((other_player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		if (other_player->unit_index != NONE)
			players_globals->all_dead = FALSE;
	}

	return;
}

void players_set_local_player_unit(
	short local_player_index,
	long unit_index)
{
	long old_unit_index;
	long player_index;
	struct player_datum *player;
	struct unit_datum *unit;

	old_unit_index = player_control_get_unit_index(local_player_index);
	match_assert(
		"c:\\halo\\SOURCE\\game\\players.c",
		1060,
		game_connection()==_game_connection_local);

	if (old_unit_index != NONE)
	{
		unit = unit_get(old_unit_index);
		unit->unit.player_index = NONE;
		unit_set_actively_controlled(old_unit_index, FALSE);
	}

	if (unit_index != NONE)
	{
		unit = unit_get(unit_index);
		unit_set_actively_controlled(unit_index, TRUE);
		unit->unit.player_index = local_player_get_player_index(local_player_index);
	}

	player_index = local_player_get_player_index(local_player_index);
	player = player_get(player_index);
	player->unit_index = unit_index;
	player->dead_unit_index = NONE;
	player_control_new_unit(local_player_index, unit_index);

	return;
}

boolean players_are_all_dead(
	void)
{
	return players_globals->all_dead;
}

unsigned long const *players_get_combined_pvs_local(
	void)
{
	return players_globals->combined_pvs_local;
}

unsigned long const *players_get_combined_pvs(
	void)
{
	return players_globals->combined_pvs;
}

void player_input_enable(
	boolean enable)
{
	players_globals->input_disabled = !enable;

	return;
}

boolean player_input_enabled(
	void)
{
	return !players_globals->input_disabled;
}

boolean any_player_is_in_the_air(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct unit_datum *unit;
	struct object_datum *root_object;
	struct players_vehicle_datum *vehicle;
	struct unit_definition *vehicle_definition;
	long root_object_index;

	data_iterator_new(&iterator, player_data);
	while (player = data_iterator_next(&iterator))
	{
		if (player->unit_index == NONE)
			continue;

		unit = unit_get(player->unit_index);
		root_object_index = object_get_ultimate_parent(player->unit_index);
		root_object = object_get(root_object_index);
		if (TEST_FLAG(root_object->object.flags, _object_outside_of_map_bit))
			return TRUE;

		if (unit->object.parent_object_index == NONE)
		{
			if (unit->object.type == _object_type_biped)
			{
				if (biped_flying_through_air(player->unit_index))
					return TRUE;
				continue;
			}
			if (unit->object.type != _object_type_vehicle)
				continue;
			vehicle = (struct players_vehicle_datum *)unit;
		}
		else
		{
			vehicle = (struct players_vehicle_datum *)object_try_and_get_and_verify_type(
				unit->object.parent_object_index,
				_object_mask_vehicle);
			if (!vehicle)
				continue;
			vehicle_definition = vehicle_definition_get(vehicle->definition_index);
			if (!TEST_FLAG(
					vehicle_definition->unit.flags,
					_unit_causes_passenger_dialogue_bit))
			{
				continue;
			}
		}

		if (vehicle->vehicle.unknown_state > 2)
			return TRUE;
	}

	return FALSE;
}

boolean any_player_is_dead(
	void)
{
	struct player_datum *player;
	struct data_iterator iterator;

	data_iterator_new(
		&iterator,
		player_data);
	while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		if (player->unit_index == NONE)
			return TRUE;
	}

	return FALSE;
}

void player_control_fix_for_loaded_game_state(
	void)
{
	struct player_datum *player;
	long player_index;
	short controller_local_player_index;
	short local_player_index;

	local_player_index = 0;
	controller_local_player_index =
		player_ui_get_single_player_local_player_controller(0);
	if (controller_local_player_index == NONE)
		controller_local_player_index = 0;

	if (local_player_get_player_index(controller_local_player_index) == NONE)
	{
		if (player_spawn_count == 1)
		{
			for (local_player_index = 0;
				local_player_index < MAXIMUM_LOCAL_PLAYERS;
				local_player_index++)
			{
				match_assert(
					"c:\\halo\\SOURCE\\game\\players.c",
					943,
					local_player_index>=NONE &&
						local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
				player_index = local_player_index == NONE
					? NONE
					: players_globals->local_players[local_player_index];
				if (player_index != NONE)
				{
					player = player_get(player_index);
					local_player_set_player_index(local_player_index, NONE);
					player_control_new_unit(local_player_index, NONE);
					local_player_set_player_index(
						controller_local_player_index,
						player_index);
					player_control_new_unit(
						controller_local_player_index,
						player->unit_index);
					hud_fix_weapon_data(
						local_player_index,
						controller_local_player_index);
					hud_fix_unit_data(
						local_player_index,
						controller_local_player_index);
					error(2, "corrected player control for restored saved game");
					break;
				}
			}

			if (local_player_index == MAXIMUM_LOCAL_PLAYERS)
			{
				error(2,
					"failed to correct player control for restored saved game... probably won't be able to control the player");
			}
		}
		else
		{
			error(2,
				"tried to fix broken player control for a game w/ player_spawn_count= %d... but we don't allow restored games for anything other than player_spawn_count= 1",
				player_spawn_count);
		}
	}

	return;
}

short player_get_starting_location_count(
	void)
{
	struct scenario *scenario;
	struct encounter_definition *encounter;
	short starting_location_count;

	scenario = global_scenario_get();
	starting_location_count = scenario->players.count;
	if (ai_debug.selected_squad_index != NONE)
	{
		encounter = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index),
			struct encounter_definition);
		if (encounter->player_starting_locations.count > 0)
			starting_location_count = encounter->player_starting_locations.count;
	}

	return starting_location_count;
}

struct player_starting_location *player_get_starting_location(
	short starting_location_index)
{
	struct scenario *scenario;
	struct encounter_definition *encounter;
	struct player_starting_location *starting_location;
	short structure_bsp_reference_index;

	scenario = global_scenario_get();
	starting_location = NULL;
	if (starting_location_index >= 0 && starting_location_index < scenario->players.count)
	{
		starting_location = TAG_BLOCK_GET_ELEMENT(
			&scenario->players,
			starting_location_index,
			struct player_starting_location);
	}

	if (ai_debug.selected_squad_index != NONE)
	{
		encounter = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index),
			struct encounter_definition);

		if (starting_location_index >= 0 &&
			starting_location_index < encounter->player_starting_locations.count)
		{
			starting_location = TAG_BLOCK_GET_ELEMENT(
				&encounter->player_starting_locations,
				starting_location_index,
				struct player_starting_location);
			structure_bsp_reference_index = encounter->runtime_structure_bsp_reference_index;
			if (structure_bsp_reference_index >= 0 &&
				structure_bsp_reference_index < scenario->structure_bsp_references.count)
			{
				starting_location->structure_bsp_reference_index = structure_bsp_reference_index;
			}
		}
	}

	return starting_location;
}

static long is_player_in_trigger(
	short bsp_switch_trigger_volume_index,
	long object_index)
{
	struct scenario_bsp_switch_trigger_volume *bsp_switch_trigger_volume;

	if (bsp_switch_trigger_volume_index != NONE)
	{
		bsp_switch_trigger_volume = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->bsp_switch_trigger_volumes,
			bsp_switch_trigger_volume_index,
			struct scenario_bsp_switch_trigger_volume);
		if (scenario_trigger_volume_test_object(
			bsp_switch_trigger_volume->trigger_volume_index,
			object_index))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void placement_data_set_change_color(
	struct object_placement_data *placement_data,
	real_rgb_color const *change_color)
{
	placement_data->change_colors[_object_change_color_a] = *change_color;
	placement_data->change_colors[_object_change_color_b] = *change_color;
	placement_data->change_colors[_object_change_color_c] = *change_color;
	placement_data->change_colors[_object_change_color_d] = *change_color;

	return;
}

boolean valid_real_vector2d(
	real_vector2d const *vector)
{
	return valid_real(vector->i) && valid_real(vector->j);
}

static void machine_add_player(
	long machine_index,
	long player_index)
{
	long *machine_player_list;
	long machine_player_index;

	machine_player_list =
		machine_to_player_table[DATUM_INDEX_TO_ABSOLUTE_INDEX(machine_index)];
	for (machine_player_index = 0;
		machine_player_index < MAXIMUM_LOCAL_PLAYERS;
		machine_player_index++)
	{
		if (machine_player_list[machine_player_index] == NONE)
		{
			machine_player_list[machine_player_index] = player_index;
			break;
		}
	}

	if (machine_player_index == MAXIMUM_LOCAL_PLAYERS)
	{
		display_assert(
			"failed to create a player",
			"c:\\halo\\SOURCE\\game\\players.c",
			240,
			TRUE);
		system_exit(-1);
	}

	return;
}

long player_new(
	long machine_index,
	long player_index,
	short local_player_index,
	struct network_player const *network_player)
{
	struct player_datum *player;
	struct player_datum *player2;
	wchar_t const *player_name;

	if (player_index == NONE)
		player_index = datum_new(player_data);
	else
		player_index = datum_new_at_index(player_data, player_index);

	match_assert("c:\\halo\\SOURCE\\game\\players.c", 309,
		((local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)) || (local_player_index==NONE));

	if (player_index != NONE)
	{
		player = player_get(player_index);
		player_name = L"";
		if (network_player)
			player_name = network_player->name;
		ustrncpy(player->name, player_name, 11);
		player->name[11] = 0;
		player->local_player_index = local_player_index;
		player->unit_index = NONE;
		player->dead_unit_index = NONE;
		player->squad_index = NONE;
		player->cluster_index = NONE;
		player->aim_assist_unit_index = NONE;
		player->speed_multiplier = 1.0f;
		player->team_index = 1;

		player2 = player_get(player_index);
		player2->action_result = _player_action_result_reload;
		player2->action_object_index = NONE;
		player->quit_out_of_game_time = NONE;
		player->quit_out_of_game = FALSE;

		if (network_player)
			csmemcpy(&player->network_player_data, network_player, sizeof(struct network_player));
	}

	machine_add_player(machine_index, player_index);

	return player_index;
}

long find_best_starting_location_index(
	long player_index)
{
	struct scenario *scenario;
	struct encounter_definition *encounter;
	struct player_starting_location *starting_location;
	short starting_location_count;
	short starting_location_index;
	short best_starting_location_index;
	real starting_location_rating;
	real best_starting_location_rating;

	scenario = global_scenario_get();
	starting_location_count = scenario->players.count;
	if (ai_debug.selected_squad_index != NONE)
	{
		encounter = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index),
			struct encounter_definition);
		if (encounter->player_starting_locations.count > 0)
			starting_location_count = encounter->player_starting_locations.count;
	}

	best_starting_location_index = NONE;
	best_starting_location_rating = 0.0f;
	for (starting_location_index = 0;
		starting_location_index < starting_location_count;
		starting_location_index++)
	{
		starting_location = player_get_starting_location(starting_location_index);
		starting_location_rating = game_engine_get_starting_location_rating(
			player_index,
			starting_location);
		starting_location_rating *= (real)pow(
			(double)real_random_range(0.0f, 1.0f),
			0.5);
		if (starting_location_rating > best_starting_location_rating)
		{
			best_starting_location_rating = starting_location_rating;
			best_starting_location_index = starting_location_index;
		}
	}

	return best_starting_location_index;
}

static void player_spawn(
	long player_index)
{
	struct player_datum *player;
	struct game_globals *game_globals;
	struct player_datum *player2;
	struct unit_datum *unit;
	struct scenario *scenario;
	struct game_globals_player_information *player_information;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct player_starting_location *starting_location;
	struct object_placement_data placement_data;
	real_rgb_color change_color;
	real_rgb_color change_color_storage;
	long saved_unit_index;
	long weapon_index;
	long unit_definition_index;
	long unit_index;
	long starting_equipment_count;
	short starting_location_index;

	player = player_get(player_index);
	saved_unit_index = NONE;
	if (!game_engine_running() && player->local_player_index != NONE)
	{
		saved_unit_index =
			players_globals->dead_units[player->local_player_index];
		players_globals->dead_units[player->local_player_index] = NONE;
		if (saved_unit_index != NONE)
		{
			unit = unit_get(saved_unit_index);
			if (TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
			{
				object_delete(saved_unit_index);
				saved_unit_index = NONE;
			}
		}
	}

	if (!game_engine_running() && saved_unit_index != NONE)
	{
		unit = unit_get(saved_unit_index);
		weapon_index = unit_inventory_get_weapon(
			saved_unit_index,
			unit->unit.current_weapon_index);
		match_assert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x73A,
			player->local_player_index!=NONE);
		object_activate(saved_unit_index);
		object_set_visibility(saved_unit_index, TRUE);
		players_set_local_player_unit(
			player->local_player_index,
			saved_unit_index);
		if (weapon_index != NONE)
			object_set_visibility(weapon_index, TRUE);
	}
	else
	{
		scenario = global_scenario_get();
		if (ai_debug.selected_squad_index != NONE)
		{
			TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index),
				struct encounter_definition);
		}

		starting_location_index =
			(short)find_best_starting_location_index(player_index);
		if (starting_location_index != NONE)
		{
			game_globals = scenario_get_game_globals();
			player_information = TAG_BLOCK_GET_ELEMENT(
				&scenario_get_game_globals()->player_information,
				0,
				struct game_globals_player_information);
			if (player_information->player_unit.index != NONE)
			{
				starting_location =
					player_get_starting_location(starting_location_index);
				if (game_engine_running())
				{
					multiplayer_information = TAG_BLOCK_GET_ELEMENT(
						&game_globals->multiplayer_information,
						0,
						struct game_globals_multiplayer_information);
					unit_definition_index = multiplayer_information->unit.index;
				}
				else
				{
					unit_definition_index = player_information->player_unit.index;
				}

				object_placement_data_new(
					&placement_data,
					unit_definition_index,
					NONE);
				placement_data.position = starting_location->position;
				vector3d_from_angle(
					&placement_data.forward,
					starting_location->facing);
				placement_data.up = *global_up3d;
				change_color = *game_engine_player_get_change_color(
					&change_color_storage,
					player_index);
				placement_data_set_change_color(
					&placement_data,
					&change_color);
				unit_index = object_new(&placement_data);
				if (unit_index != NONE)
				{
					unit = unit_try_and_get(unit_index);
					if (unit != NULL)
					{
						player2 = player_get(player_index);
						unit->object.owner_player_index = player_index;
						unit->object.owner_team_index = (short)player2->team_index;
						unit->unit.player_index = player_index;
						player2->unit_index = unit_index;
						unit_set_actively_controlled(unit_index, TRUE);
						if (player2->local_player_index != NONE)
						{
							player_control_new_unit(
								player2->local_player_index,
								unit_index);
						}

						if (!game_engine_running())
						{
							scenario = global_scenario_get();
							starting_equipment_count =
								*(long *)((byte *)scenario + 0x348);
							if (starting_equipment_count > 1 &&
								*(short *)((byte *)player2 + 0xAA) > 0)
							{
								player_add_equipment(
									player2->unit_index,
									1,
									TRUE);
							}
							else if (starting_equipment_count != 0)
							{
								player_add_equipment(
									player2->unit_index,
									0,
									TRUE);
							}
						}
					}
				}
			}
		}
	}

	csmemset(
		player->powerup_durations,
		0,
		sizeof(player->powerup_durations));
	player2 = player_get(player_index);
	player2->action_result = _player_action_result_reload;
	player2->action_object_index = NONE;
	if (player->local_player_index != NONE)
		observer_obsolete_position(player->local_player_index);

	return;
}

#ifdef HALO_LINUX
/* the distributed netcode (port/linux/game/network_distributed.c): a
client's player takes the unit the host spawned it with (the host's object,
at the host's index, with the host's weapons), as player_spawn gives a
player the unit it makes */
void network_player_attach_unit(
	long player_index,
	long unit_index)
{
	struct player_datum *player = player_get(player_index);
	struct unit_datum *unit = unit_get(unit_index);

	unit->object.owner_player_index = player_index;
	unit->object.owner_team_index = (short)player->team_index;
	unit->unit.player_index = player_index;
	player->unit_index = unit_index;
	unit_set_actively_controlled(unit_index, TRUE);
	if (player->local_player_index != NONE)
		player_control_new_unit(player->local_player_index, unit_index);
	csmemset(player->powerup_durations, 0, sizeof(player->powerup_durations));
	player->action_result = _player_action_result_reload;
	player->action_object_index = NONE;
	if (player->local_player_index != NONE)
		observer_obsolete_position(player->local_player_index);
}

/* ... and shows what the host says its player picked up (the host decides
pickups): the HUD's message, the pickup's sound, a powerup's screen flash */
void network_player_show_pickup(
	long player_index,
	short kind,
	long definition_index,
	short count)
{
	struct player_datum *player = player_get(player_index);
	boolean tag_index_is_group(long tag_index, long group_tag);

	if (player->local_player_index == NONE)
		return;
	switch (kind)
	{
	case _network_pickup_weapon:
		if (tag_index_is_group(definition_index, WEAPON_DEFINITION_TAG))
		{
			hud_picked_up_weapon(player->local_player_index, definition_index);
			if (player->unit_index != NONE)
				player_control_unzoom(player->unit_index);
		}
		break;
	case _network_pickup_ammunition:
		if (tag_index_is_group(definition_index, WEAPON_DEFINITION_TAG))
		{
			struct weapon_definition *weapon_definition = weapon_definition_get(definition_index);

			hud_picked_up_ammunition(player->local_player_index, definition_index, count);
			if (weapon_definition->weapon.pickup_sound.index != NONE)
				unspatialized_impulse_sound_new(weapon_definition->weapon.pickup_sound.index, 1.0f);
		}
		break;
	case _network_pickup_grenade:
		if (tag_index_is_group(definition_index, EQUIPMENT_DEFINITION_TAG))
		{
			hud_picked_up_grenade(player->local_player_index, definition_index);
			equipment_definition_handle_pickup(definition_index);
		}
		break;
	case _network_pickup_equipment:
		if (tag_index_is_group(definition_index, EQUIPMENT_DEFINITION_TAG))
			hud_picked_up_powerup(player->local_player_index, definition_index);
		break;
	case _network_pickup_powerup:
		if (tag_index_is_group(definition_index, EQUIPMENT_DEFINITION_TAG))
		{
			/* (as player_handle_powerup_equipment shows its player) */
			switch (equipment_definition_get(definition_index)->equipment.powerup_type)
			{
			case _equipment_powerup_overshield: player_over_shield_screen_effect(player_index); break;
			case _equipment_powerup_health: player_health_pack_screen_effect(player_index); break;
			case _equipment_powerup_active_camouflage: player_active_camo_screen_effect(player_index); break;
			}
			hud_picked_up_powerup(player->local_player_index, definition_index);
			equipment_definition_handle_pickup(definition_index);
		}
		break;
	}
}

/* the host: a player on another machine presses the action button with
nothing here to do (to pick up, swap for, enter): what the host has around
them, to the log, at most every few seconds (a client sees what to pick up
where the host has nothing) */
static void network_player_log_idle_action(
	long player_index,
	unsigned long control_flags)
{
	static long logged_times[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
	struct player_datum *player = player_get(player_index);
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	struct object_iterator iterator;
	struct unit_datum *unit;
	long nearest_index = NONE;
	real nearest_distance = 0.0f;

	if (!network_game_distributed() || game_connection() != _game_connection_network_server ||
		player->local_player_index != NONE || player->action_result != _player_action_result_reload ||
		!(control_flags & (FLAG(_unit_control_action_bit) | FLAG(_unit_control_swap_weapons_bit))) ||
		absolute_index >= HALO_PORT_MAXIMUM_NETWORK_PLAYERS ||
		(logged_times[absolute_index] && game_time_get() - logged_times[absolute_index] < 3 * TICKS_PER_SECOND))
	{
		return;
	}
	logged_times[absolute_index] = game_time_get();
	unit = unit_get(player->unit_index);
	object_iterator_new(&iterator, _object_mask_weapon | _object_mask_equipment, 0);
	while (object_iterator_next(&iterator))
	{
		struct object_datum *object = object_get(iterator.index);
		real distance;

		if (object->object.parent_object_index != NONE || !TEST_FLAG(object->object.flags, _object_connected_to_map_bit))
			continue;
		distance = distance3d(&unit->object.position, &object->object.position);
		if (nearest_index == NONE || distance < nearest_distance)
		{
			nearest_index = iterator.index;
			nearest_distance = distance;
		}
	}
	{
		boolean tag_index_is_group(long tag_index, long group_tag);
		long definition_index = nearest_index != NONE ? object_get(nearest_index)->definition_index : NONE;
		/* named only when the index is a loaded object tag: a bad one would
		be read as a tag instance anyway (tag_get_name), and its garbage
		name pointer took the host down (2026-09-29) */
		const char *name = definition_index != NONE && tag_index_is_group(definition_index, OBJECT_DEFINITION_TAG) ?
			tag_get_name(definition_index) : "?";

		error(2, "distributed: player %ld pressed action with nothing to pick up here: unit %lx at %.2f %.2f %.2f, "
			"nearest item %lx (tag %lx %s) at %.2f",
			absolute_index, player->unit_index, unit->object.position.x, unit->object.position.y,
			unit->object.position.z, nearest_index, definition_index, name, nearest_distance);
	}
}

/* ... and gives up the one it has (the host's unit for it is another) */
void network_player_detach_unit(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	struct unit_datum *unit = player->unit_index != NONE ?
		(struct unit_datum *)object_try_and_get_and_verify_type(player->unit_index, _object_mask_unit) : NULL;

	if (unit)
	{
		unit->unit.player_index = NONE;
		unit_set_actively_controlled(player->unit_index, FALSE);
	}
	player->unit_index = NONE;
	if (player->local_player_index != NONE)
		player_control_new_unit(player->local_player_index, NONE);
}
#endif

/* Exact: January emits this private dead-unit replacement helper from the
   reconstructed player_teleport_internal caller below. */
static void player_pseudo_kill(
	long player_index,
	long replacement_dead_unit_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	long dead_unit_index;
	long weapon_index;

	player = player_get(player_index);
	if (player->unit_index != NONE)
	{
		if (game_engine_can_score())
		{
			game_statistics_record_kill(
				player->unit_index,
				NONE,
				NONE,
				NONE);
		}

		players_globals->dead_units[player->local_player_index] =
			player->unit_index;
		player_died(player_index);

		dead_unit_index =
			players_globals->dead_units[player->local_player_index];
		unit = unit_get(dead_unit_index);
		weapon_index = unit_inventory_get_weapon(
			dead_unit_index,
			unit_get(dead_unit_index)->unit.current_weapon_index);
		unit->unit.player_index = NONE;
		object_deactivate(dead_unit_index);
		object_set_visibility(dead_unit_index, FALSE);
		if (weapon_index != NONE)
			object_set_visibility(weapon_index, FALSE);

		if (replacement_dead_unit_index != NONE)
			player->dead_unit_index = replacement_dead_unit_index;
		players_globals->all_dead = FALSE;
	}

	return;
}

/* Matching status is tracked in the Players object log; this is the ordinary
   combined-PVS calculation used by the game loop. */
/* Residual instruction scheduling is recorded in the matching log. */
static void players_compute_combined_pvs(
	unsigned long *combined_pvs,
	boolean local_player_only)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct object_datum *root_object;
	struct structure_bsp *structure_bsp;
	struct structure_leaf *leaf;
	long leaf_index;
	short activating_cluster_index;
	long root_object_index;
	short cluster_index;

	structure_bsp = global_structure_bsp_get();
	csmemset(combined_pvs, 0, 0x40);
	if (!game_in_editor())
	{
		activating_cluster_index = objects_get_activating_cluster_index();
		data_iterator_new(&iterator, player_data);
		while (player = data_iterator_next(&iterator))
		{
			if (local_player_only && player->local_player_index == NONE)
				continue;

			if (player->unit_index != NONE)
			{
				root_object_index =
					object_get_ultimate_parent(player->unit_index);
				root_object = object_get(root_object_index);
				if (root_object->object.location.cluster_index != NONE)
				{
					player->cluster_index =
						root_object->object.location.cluster_index;
				}
			}

			cluster_index = player->cluster_index;
			if (cluster_index != NONE)
			{
				bit_vector_or(
					(short)structure_bsp->clusters.count,
					combined_pvs,
					structure_bsp_get_cluster_pvs(
						structure_bsp,
						cluster_index),
					combined_pvs);
			}
		}

		if (activating_cluster_index != NONE)
		{
			bit_vector_or(
				(short)structure_bsp->clusters.count,
				combined_pvs,
				structure_bsp_get_cluster_pvs(
					structure_bsp,
					activating_cluster_index),
				combined_pvs);
		}
	}
	else
	{
		leaf_index = scenario_leaf_index_from_point(
			&observer_get_camera(0)->position);
		if (leaf_index != NONE)
		{
			leaf = TAG_BLOCK_GET_ELEMENT(
				&global_structure_bsp_get()->leaves,
				scenario_leaf_index_from_point(
					&observer_get_camera(0)->position) & LONG_MAX,
				struct structure_leaf);
			cluster_index = leaf->cluster_index;
			if (cluster_index != NONE)
			{
				bit_vector_or(
					(short)structure_bsp->clusters.count,
					combined_pvs,
					structure_bsp_get_cluster_pvs(
						structure_bsp,
						cluster_index),
					combined_pvs);
			}
		}
	}

	return;
}

static boolean player_handle_action(
	long player_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct unit_datum *nearby_unit;
	struct item_datum *equipment;
	struct players_vehicle_datum *target_vehicle;
	real_matrix4x3 player_matrix;
	real_matrix4x3 target_matrix;
	real_point3d *player_position;
	real_point3d *target_position;
	real_vector3d delta;
	real dot;
	char approach_type;
	boolean result;

	player = player_get(player_index);
	unit_get(player->unit_index);
	result = FALSE;
	switch (player->action_result)
	{
	case _player_action_result_touch_device:
		device_touched(player->action_object_index, player->unit_index);
		result = TRUE;
		break;

	case _player_action_result_swap_for_powerup:
		unit_drop_current_equipment(player->unit_index);
		if (unit_add_equipment_to_inventory(
			player->unit_index,
			player->action_object_index,
			FALSE))
		{
			equipment = equipment_get(player->action_object_index);
			hud_picked_up_powerup(
				player->local_player_index,
				equipment->definition_index);
			player_network_picked_up(player, player_index, _network_pickup_equipment, equipment->definition_index, 0);
		}
		result = TRUE;
		break;

	case _player_action_result_enter_vehicle:
	case _player_action_result_evict_from_vehicle:
	{
		long nearby_unit_index;

		if (unit_can_enter_seat(
			player->unit_index,
			player->action_object_index,
			player->action_seat_index,
			(nearby_unit_index = NONE, &nearby_unit_index)))
		{
			unit_enter_seat(
				player->unit_index,
				player->action_object_index,
				player->action_seat_index);
		}
		else if (nearby_unit_index != NONE)
		{
			nearby_unit = unit_get(nearby_unit_index);
			if (nearby_unit->unit.actor_index != NONE)
			{
				ai_try_vehicle_eviction(
					nearby_unit->unit.actor_index,
					player->unit_index,
					TRUE);
			}
		}
		break;
	}

	case _player_action_result_flip_vehicle:
		unit = unit_get(player->unit_index);
		target_vehicle = (struct players_vehicle_datum *)vehicle_get(
			player->action_object_index);
		unit->unit.last_vehicle_index = player->action_object_index;
		unit->unit.game_time_at_last_vehicle_exit = game_time_get();

		if (fabs(target_vehicle->object.forward.k) > 0.70710677f)
		{
			approach_type = (target_vehicle->object.forward.k < 0.f) + 3;
		}
		else
		{
			target_position = &object_get_world_matrix(
				player->action_object_index,
				&target_matrix)->position;
			player_position = &object_get_world_matrix(
				player->unit_index,
				&player_matrix)->position;
			delta.i = target_position->x - player_position->x;
			delta.j = target_position->y - player_position->y;
			delta.k = target_position->z - player_position->z;
			cross_product3d(global_up3d, &delta, &delta);
			dot = dot_product3d(&delta, &target_vehicle->object.forward);
			approach_type = (dot > 0.f) + 1;
		}

		SET_FLAG(target_vehicle->vehicle.flags, _vehicle_upending_bit, TRUE);
		target_vehicle->vehicle.approach_type = approach_type;
		target_vehicle->vehicle.unknown42A = 0;
		result = TRUE;
		break;
	}

	return result;
}

/* Matching status is tracked in the Players object log. */
static boolean player_teleport_internal(
	long player_index,
	long source_unit_index,
	real_point3d const *position)
{
	struct player_datum *player = player_get(player_index);
	long player_unit_index = player->unit_index;
	struct biped_datum *biped = biped_get(player_unit_index);
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\game\\players.c",
		0x4FB,
		source_unit_index==NONE || local_player_count()>1);
	if (source_unit_index != NONE &&
		object_get_ultimate_parent(source_unit_index) != source_unit_index)
	{
		long source_root_object_index = object_get_ultimate_parent(source_unit_index);
		struct object_datum *source_root_object;
		real_vector3d best_adjustment_vector;
		real collision_radius;
		real_matrix4x3 adjustment_matrix;
		real scale;
		short adjustment_index;
		real_point3d anchor_point;

		unit_get(source_unit_index);
		source_root_object = object_get(source_root_object_index);
		best_adjustment_vector = source_root_object->object.translational_velocity;
		best_adjustment_vector.k = 0.f;
		source_unit_index = source_root_object_index;
		if (!(magnitude_squared3d(&best_adjustment_vector) > 0.f))
		{
			if (source_root_object->object.forward.k < 0.70710677f)
			{
				best_adjustment_vector = source_root_object->object.forward;
			}
			else
			{
				best_adjustment_vector = source_root_object->object.up;
			}
		}
		best_adjustment_vector.k = 0.f;

		collision_radius = biped_definition_get(biped->definition_index)->biped.collision_radius;
		scale = collision_radius * 3.f + source_root_object->object.bounding_sphere_radius;
		match_assert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x525,
			magnitude3d(&best_adjustment_vector));
		match_assert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x526,
			scale);

		negate_vector3d(
			&best_adjustment_vector,
			&best_adjustment_vector);
		normalize3d(&best_adjustment_vector);
		matrix4x3_from_point_and_vectors(
			&adjustment_matrix,
			&source_root_object->object.bounding_sphere_center,
			&best_adjustment_vector,
			global_up3d);
		adjustment_matrix.scale = scale;

		for (adjustment_index = 0;
			adjustment_index < NUMBEROF(adjustment_weights) && !result;
			adjustment_index++)
		{
			matrix4x3_transform_point(
				&adjustment_matrix,
				&adjustment_weights[adjustment_index],
				&anchor_point);
			result = biped_fix_position(
				player_unit_index,
				source_unit_index,
				&anchor_point,
				NULL,
				2.f,
				FALSE,
				FALSE,
				TRUE);

			if (!result)
			{
				short random_adjustment_index;
				real_point3d new_position;
				real_vector3d random_offset;

				for (random_adjustment_index = 0;
					random_adjustment_index < 8 && !result;
					random_adjustment_index++)
				{
					random_offset = *global_zero_vector3d;
					random_direction3d(&random_offset);
					point_from_line3d(
						&anchor_point,
						&random_offset,
						collision_radius,
						&new_position);
					result = biped_fix_position(
						player_unit_index,
						source_unit_index,
						&new_position,
						NULL,
						2.f,
						FALSE,
						FALSE,
						TRUE);
				}
			}
		}
	}
	else
	{
		result = biped_fix_position(
			player_unit_index,
			source_unit_index,
			(real_point3d *)position,
			NULL,
			2.f,
			FALSE,
			FALSE,
			TRUE);
	}

	player->cluster_index = NONE;
	if (result)
	{
		struct scenario *scenario = global_scenario_get();
		short bsp_switch_trigger_volume_index;

		match_assert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x56A,
			player->unit_index!=NONE);
		for (bsp_switch_trigger_volume_index = 0;
			bsp_switch_trigger_volume_index < scenario->bsp_switch_trigger_volumes.count;
			bsp_switch_trigger_volume_index++)
		{
			struct scenario_bsp_switch_trigger_volume *bsp_switch_trigger_volume = TAG_BLOCK_GET_ELEMENT(
				&scenario->bsp_switch_trigger_volumes,
				bsp_switch_trigger_volume_index,
				struct scenario_bsp_switch_trigger_volume);

			if (bsp_switch_trigger_volume->source_structure_bsp_index ==
				global_structure_bsp_index &&
				scenario_trigger_volume_test_object(
					bsp_switch_trigger_volume->trigger_volume_index,
					player->unit_index))
			{
				result = FALSE;
				break;
			}
		}
	}

	if (result)
	{
		biped->object.translational_velocity = *global_zero_vector3d;
		if (source_unit_index != NONE)
		{
			real_vector3d forward = unit_get(source_unit_index)->object.forward;
			struct biped_datum *source_biped = biped_try_and_get(source_unit_index);

			if (source_biped && source_biped->biped.elevator_object_index != NONE)
			{
				biped->biped.elevator_object_index =
					source_biped->biped.elevator_object_index;
				biped->biped.elevator_ticks = source_biped->biped.elevator_ticks;
			}

			biped->unit.desired_facing_vector = forward;
			biped->unit.desired_aiming_vector = forward;
			biped->unit.desired_looking_vector = forward;
			if (player->local_player_index != NONE)
			{
				player_control_set_facing(
					player->local_player_index,
					&forward);
			}
		}

		if (source_unit_index != NONE)
		{
			long respawn_effect_index = TAG_BLOCK_GET_ELEMENT(
				&scenario_get_game_globals()->player_information,
				0,
				struct game_globals_player_information)->coop_respawn_effect.index;

			if (respawn_effect_index != NONE)
			{
				players_compute_combined_pvs(players_globals->combined_pvs, FALSE);
				effect_new_from_object(
					respawn_effect_index,
					player_unit_index,
					player_unit_index,
					NONE,
					0.f,
					0.f,
					NULL,
					NULL);
			}
		}
	}
	else
	{
		error(2, "couldn't teleport player into a valid location");
		match_assert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x5AB,
			player->local_player_index!=NONE);
		player_pseudo_kill(player_index, source_unit_index);
	}

	return result;
}

boolean player_teleport(
	long player_index,
	long source_unit_index,
	real_point3d const *position)
{
	struct player_datum *player;
	struct biped_datum *biped;
	long unit_index;
	boolean result;

	player = player_get(player_index);
	unit_index = player->unit_index;
	biped = biped_try_and_get(unit_index);
	result = FALSE;
	if (biped)
	{
		if (biped->object.parent_object_index != NONE)
			unit_exit_seat_end(unit_index);
		result = player_teleport_internal(player_index, source_unit_index, position);
	}

	return result;
}

#ifdef HALO_N64_CAMPAIGN
/* The campaign cartridge's checkpoints (port/n64/game/n64_campaign.c): the
N64 keeps no saved game to go back to when the players are all dead
(players_update_after_game), so a dead player comes back where the
campaign chose -- a new unit with the scenario's starting profile, moved
there. FALSE if it could not be made */
boolean n64_player_respawn_at(
	long player_index,
	real_point3d const *position,
	real facing)
{
	struct player_datum *player = player_get(player_index);
	real_vector3d forward;

	if (player->unit_index == NONE)
	{
		player_spawn(player_index);
		if (player->unit_index == NONE)
			return FALSE;
	}
	vector3d_from_angle(&forward, facing);
	object_set_position(player->unit_index, position, &forward, global_up3d);
	players_globals->all_dead = FALSE;
	return TRUE;
}
#endif

void players_debug_render(
	void)
{
	struct collision_result collision;
	real_point3d pill_position;
	real_vector3d pill_vector;
	real initial_radius;
	real_point3d fixed_position;
	short local_player_count;
	real pill_height;
	real pill_width;
	short local_player_index;
	long player_index;
	long biped_index;
	struct player_datum *player;
	struct unit_datum *unit;
	struct biped_definition *biped_definition;

	if (debug_render_player_teleport)
	{
		for (local_player_count = 0, local_player_index = local_player_get_next(NONE);
			local_player_count < 2 && local_player_index != NONE;
			local_player_count++, local_player_index = local_player_get_next(local_player_index))
		{
			match_assert("c:\\halo\\SOURCE\\game\\players.c", 0x3AF,
				local_player_index>=NONE && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

			if (players_globals->local_players[local_player_index] != NONE)
			{
				player_index = local_player_get_player_index(local_player_index);
				player = player_get(player_index);
				biped_index = player->unit_index;
				if (biped_index != NONE)
				{
					unit = unit_get(biped_index);
					biped_get_physics_pill(
						biped_index,
						&fixed_position,
						&initial_radius,
						&initial_radius);
					if (biped_fix_position(
						NONE,
						biped_index,
						&fixed_position,
						&fixed_position,
						2.0f,
						FALSE,
						TRUE,
						TRUE))
					{
						biped_definition = biped_definition_get(unit->definition_index);
						biped_get_physics_pill(
							biped_index,
							&pill_position,
							&pill_height,
							&pill_width);
						fixed_position.z +=
							biped_definition->biped.collision_radius;
						scale_vector3d(global_up3d, pill_height, &pill_vector);
						if (collision_test_pill(
							_collision_test_for_player_teleport_flags,
							&fixed_position,
							&pill_vector,
							pill_width,
							biped_index,
							&collision))
						{
							render_debug_pill(
								FALSE,
								&fixed_position,
								&pill_vector,
								pill_width,
								global_real_argb_red);
						}
						else
						{
							render_debug_pill(
								FALSE,
								&fixed_position,
								&pill_vector,
								pill_width,
								global_real_argb_green);
						}
					}
				}
			}
		}
	}

	return;
}

static void player_update_powerups(
	long player_index)
{
	struct player_datum *player;
	short powerup_index;

	player = player_get(player_index);
	for (powerup_index= 0; powerup_index<NUMBER_OF_PLAYER_POWERUPS; powerup_index++)
	{
		if (player->powerup_durations[powerup_index] > 0)
		{
			player->powerup_durations[powerup_index]--;
			if (player->powerup_durations[powerup_index] == 0)
				player_powerup_off(player_index, powerup_index);
		}
	}

	return;
}

void debug_player_teleport(
	short local_player_index,
	short target_local_player_index)
{
	long player_index;
	long unit_index;
	long target_player_index;
	long target_unit_index;
	struct player_datum *player;
	struct unit_datum *target_unit;

	player_index = local_player_get_player_index(local_player_index);
	if (player_index == NONE)
	{
		unit_index = NONE;
	}
	else
	{
		player = player_get(local_player_get_player_index(local_player_index));
		unit_index = player->unit_index;
	}

	target_player_index = local_player_get_player_index(target_local_player_index);
	if (target_player_index == NONE)
	{
		target_unit_index = NONE;
	}
	else
	{
		player = player_get(local_player_get_player_index(target_local_player_index));
		target_unit_index = player->unit_index;
	}

	if (unit_index != NONE && target_unit_index != NONE)
	{
		target_unit = unit_get(target_unit_index);
		player_teleport_internal(
			player_index_from_unit_index(unit_index),
			target_unit_index,
			&target_unit->object.bounding_sphere_center);
	}

	return;
}

boolean players_respawn_coop(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct biped_datum *biped;
	struct players_vehicle_datum *vehicle;
	long respawn_unit_index;
	long root_object_index;
	boolean result;
	boolean dangerous;

	players_globals->respawn_failure = 0;
	result = FALSE;
	if (!players_globals->respawn_failed)
	{
		if (dangerous_projectiles_near_player() ||
			any_unit_is_dangerous())
		{
			players_globals->respawn_failure = 1;
			goto result_complete;
		}
	}

	if (!players_globals->respawn_failed &&
		ai_enemies_attacking_player())
	{
		players_globals->respawn_failure = 2;
		goto result_complete;
	}

	respawn_unit_index = NONE;
	data_iterator_new(&iterator, player_data);
	while (player = data_iterator_next(&iterator))
	{
		if (player->unit_index != NONE)
		{
			root_object_index =
				object_get_ultimate_parent(player->unit_index);
			if (root_object_index == player->unit_index)
			{
				biped = biped_try_and_get(player->unit_index);
				if (biped)
				{
					dangerous = TEST_FLAG(
						biped->biped.flags,
						_biped_airborne_bit);
				}
				else
				{
					dangerous = FALSE;
				}
			}
			else
			{
				vehicle =
					(struct players_vehicle_datum *)
						object_try_and_get_and_verify_type(
							root_object_index,
							_object_mask_vehicle);
				if (vehicle)
					dangerous = vehicle->vehicle.unknown_state > 0;
				else
					dangerous = FALSE;
			}

			if (!dangerous)
				respawn_unit_index = player->unit_index;
			else
				players_globals->respawn_failure = 3;
		}
	}

	if (respawn_unit_index != NONE)
	{
		result = TRUE;
		data_iterator_new(&iterator, player_data);
		while (player = data_iterator_next(&iterator))
		{
			if (player->unit_index == NONE)
			{
				player_spawn(iterator.datum_index);
				if (player->unit_index != NONE)
				{
					result = player_teleport(
						iterator.datum_index,
						respawn_unit_index,
						&object_get(respawn_unit_index)->
							object.bounding_sphere_center);
				}
				else
				{
					result = FALSE;
				}
			}
		}
	}

	players_globals->respawn_failed =
		players_globals->respawn_failed && !result;
	if (result)
		players_globals->respawn_failure = 0;

result_complete:
	return result;
}

static void player_teleport_on_bsp_switch(
	long player_index,
	long source_unit_index,
	real_point3d const *position)
{
	struct player_datum *player;
	struct biped_datum *biped;
	struct biped_datum *source_biped;
	long unit_index;
	boolean outside_switch_trigger;
	boolean teleport_succeeded;

	player = player_get(player_index);
	unit_index = player->unit_index;
	biped = biped_try_and_get(unit_index);
	match_assert("c:\\halo\\SOURCE\\game\\players.c", 0x4CB,
		player_index!=NONE);
	match_assert("c:\\halo\\SOURCE\\game\\players.c", 0x4CC,
		position);
	if (biped)
	{
		if (players_globals->pending_teleport_starting_location_index != NONE &&
			!scenario_trigger_volume_test_object(
				TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->bsp_switch_trigger_volumes,
					players_globals->pending_teleport_starting_location_index,
					struct scenario_bsp_switch_trigger_volume)->trigger_volume_index,
				unit_index))
		{
			outside_switch_trigger = TRUE;
		}
		else
		{
			outside_switch_trigger = FALSE;
		}

		if (scenario_leaf_index_from_point(
			&biped->object.bounding_sphere_center) == NONE ||
			outside_switch_trigger)
		{
			if (biped->object.parent_object_index != NONE)
			{
				source_biped = biped_get(source_unit_index);
				if (biped->object.parent_object_index !=
					source_biped->object.parent_object_index)
				{
					unit_exit_seat_end(unit_index);
				}
			}

			if (biped->object.parent_object_index == NONE)
			{
				teleport_succeeded = player_teleport_internal(
					player_index,
					source_unit_index,
					position);
			}
			else
			{
				teleport_succeeded = TRUE;
			}
			players_globals->respawn_failed = !teleport_succeeded;
		}
	}

	return;
}

void players_reconnect_to_structure_bsp(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct scenario *scenario;
	struct scenario_bsp_switch_trigger_volume *bsp_switch;
	struct scenario_cutscene_flag *cutscene_flag;
	real_point3d teleport_position;
	long source_unit_index;
	boolean teleport_position_valid;
	boolean found_player;
	short local_player_index;
	short cutscene_flag_index;
	long player_index;
	long player_unit_index;

	if (players_globals->pending_teleport_starting_location_index != NONE &&
		players_globals->local_player_count > 1)
	{
		scenario = global_scenario_get();
		bsp_switch = TAG_BLOCK_GET_ELEMENT(
			&scenario->bsp_switch_trigger_volumes,
			players_globals->pending_teleport_starting_location_index,
			struct scenario_bsp_switch_trigger_volume);
		source_unit_index = NONE;
		found_player = FALSE;
		teleport_position_valid = FALSE;
		cutscene_flag_index = bsp_switch->cutscene_flag_index;
		if (cutscene_flag_index != NONE)
		{
			real adjustment = 0.f;

			cutscene_flag = TAG_BLOCK_GET_ELEMENT(
				&scenario->cutscene_flags,
				cutscene_flag_index,
				struct scenario_cutscene_flag);
			teleport_position = cutscene_flag->position;
			while (adjustment < 0.3f &&
				collision_test_point(
					FLAG(_collision_test_front_facing_surfaces_bit) |
						FLAG(_collision_test_ignore_invisible_surfaces_bit) |
						FLAG(_collision_test_structure_bit) |
						FLAG(_collision_test_objects_scenery_bit),
					&teleport_position,
					NONE))
			{
				teleport_position.z += 0.05f;
				adjustment += 0.05f;
			}
			teleport_position_valid = adjustment < 0.3f;
		}

		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) && !found_player)
		{
			player_unit_index = player->unit_index;
			if (player_unit_index != NONE &&
				players_globals->pending_teleport_starting_location_index != NONE)
			{
				if (is_player_in_trigger(
					players_globals->pending_teleport_starting_location_index,
					player_unit_index))
				{
					real_point3d biped_base;
					real biped_height;
					real biped_width;
					long cluster_index;

					biped_get_physics_pill(
						player->unit_index,
						&biped_base,
						&biped_height,
						&biped_width);
					cluster_index = scenario_leaf_index_from_point(&biped_base) == NONE ?
						NONE :
						TAG_BLOCK_GET_ELEMENT(
							&global_structure_bsp_get()->leaves,
							scenario_leaf_index_from_point(&biped_base) & LONG_MAX,
							struct structure_leaf)->cluster_index;
					if (cluster_index != NONE)
					{
						if (!teleport_position_valid)
						{
							teleport_position = biped_base;
						}
						else
						{
							teleport_position.z += biped_width;
						}

						source_unit_index = player->unit_index;
						found_player = TRUE;
					}
				}
			}
		}

		match_vassert(
			"c:\\halo\\SOURCE\\game\\players.c",
			0x63E,
			found_player,
			"no players in the bsp");
		if (found_player)
		{
			local_player_index = local_player_get_next(NONE);
			while (local_player_index != NONE)
			{
				player_index = local_player_get_player_index(local_player_index);
				player = player_get(player_index);
				if (player->unit_index != NONE &&
					player->unit_index != source_unit_index)
				{
					player_teleport_on_bsp_switch(
						player_index,
						source_unit_index,
						&teleport_position);
					player_get(player_index)->cluster_index = NONE;
				}
				local_player_index = local_player_get_next(local_player_index);
			}
		}
		players_globals->pending_teleport_starting_location_index = NONE;
	}

	data_iterator_new(&iterator, player_data);
	while (player = data_iterator_next(&iterator))
		player->cluster_index = NONE;

	return;
}

boolean unit_should_autopick_weapon(
	long unit_index,
	long weapon_index)
{
	struct weapon_datum *weapon;
	struct weapon_definition *weapon_definition;
	long weapon_count;
	boolean result;

	weapon = weapon_try_and_get(weapon_index);
	weapon_definition = weapon_definition_get(weapon->definition_index);
	weapon_count = unit_get_weapon_count(unit_index);

	if ((unit_approve_weapon_pickup(unit_index, weapon_index) &&
		TEST_FLAG(weapon_definition->weapon.flags, _weapon_doesnt_count_toward_maximum_bit)) ||
		weapon_count == 0 ||
		(!game_engine_running() &&
			unit_approve_weapon_pickup(unit_index, weapon_index) &&
			weapon_count < 2) ||
		game_engine_force_autopickup(unit_index, weapon_index))
	{
		result = TRUE;
	}
	else
	{
		result = FALSE;
	}

	return result;
}

static long players_compute_local_player_count(
	void)
{
	long local_player_count = 0;
	short local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_LOCAL_PLAYERS;
		local_player_index++)
	{
		if (players_globals->local_players[local_player_index] != NONE)
			local_player_count++;
	}

	return local_player_count;
}

/* Reset the per-tick interaction proposal before scanning nearby objects. */
static void player_reset_action_result(
	long player_index)
{
	struct player_datum *player;

	player = player_get(player_index);
	player->action_result = _player_action_result_reload;
	player->action_object_index = NONE;

	return;
}

static boolean player_handle_weapon_swap(
	long player_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct weapon_datum *weapon;
	boolean result;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	result = FALSE;
	if (!players_decide_pickups())
		return result;
	switch (player->action_result)
	{
	case _player_action_result_swap_for_weapon:
		if (unit_drop_current_weapon(player->unit_index, TRUE) &&
			unit_add_weapon_to_inventory(
				player->unit_index,
				player->action_object_index,
				TRUE))
		{
			weapon = weapon_get(player->action_object_index);
			hud_picked_up_weapon(
				player->local_player_index,
				weapon->definition_index);
			player_network_picked_up(player, player_index, _network_pickup_weapon, weapon->definition_index, 0);
			player_control_unzoom(player->unit_index);
		}
		result = TRUE;
		break;

	case _player_action_result_add_weapon_to_inventory:
		if (unit_add_weapon_to_inventory(
			player->unit_index,
			player->action_object_index,
			TRUE))
		{
			weapon = weapon_get(player->action_object_index);
			hud_picked_up_weapon(
				player->local_player_index,
				weapon->definition_index);
			player_network_picked_up(player, player_index, _network_pickup_weapon, weapon->definition_index, 0);
		}
		break;
	}

	return result;
}

boolean player_handle_powerup(
	long player_index,
	short powerup_type,
	short duration)
{
	struct player_datum *player;
	struct unit_datum *unit;

	player = player_get(player_index);
	match_assert("c:\\halo\\SOURCE\\game\\players.c", 0xAEE,
		powerup_type>=0 && powerup_type<NUMBER_OF_PLAYER_POWERUPS);

	if (powerup_type == _player_powerup_active_camouflage)
	{
		long unit_index = player->unit_index;

		unit = unit_get(unit_index);
		if (TEST_FLAG(unit->unit.flags, _unit_active_camouflaged_bit))
			return FALSE;
	}

	if (player->powerup_durations[powerup_type] == 0)
		player_powerup_on(player_index, powerup_type);
	else if (!game_engine_running())
		player_powerup_additional(player_index, powerup_type);

	player->powerup_durations[powerup_type] += duration;

	return TRUE;
}

void player_handle_powerup_minor(
	long player_index,
	short powerup_type,
	short duration)
{
	struct player_datum *player;

	player = player_get(player_index);
	match_assert("c:\\halo\\SOURCE\\game\\players.c", 0xB19,
		powerup_type>=0 && powerup_type<NUMBER_OF_PLAYER_POWERUPS);

	if (player->powerup_durations[powerup_type] == 0)
		player_powerup_on(player_index, powerup_type);

	player->powerup_durations[powerup_type] =
		MAX(player->powerup_durations[powerup_type], duration);

	return;
}

static void player_over_shield_screen_effect(
	long player_index)
{
	struct player_datum *player;

	if (player_index != NONE)
	{
		player = player_get(player_index);
		if (player->local_player_index != NONE)
		{
			struct screen_flash_definition screen_flash = { 0 };

			screen_flash.fade_function =
				over_shield_screen_flash_fade_function;
			screen_flash.type =
				players_static_data.screen_flash_parameters[0].type;
			screen_flash.priority = 2;
			screen_flash.duration =
				players_static_data.screen_flash_parameters[0].duration;
			screen_flash.max_intensity =
				players_static_data.screen_flash_parameters[0].maximum_intensity;
			screen_flash.zero_scale_factor = 0.f;
			screen_flash.screen_flash_color.alpha =
				over_shield_screen_flash_alpha;
			screen_flash.screen_flash_color.red =
				players_static_data.screen_flash_parameters[0].red;
			screen_flash.screen_flash_color.green =
				over_shield_screen_flash_green;
			screen_flash.screen_flash_color.blue =
				players_static_data.screen_flash_parameters[0].green;
			player_effect_screen_flash(player_index, &screen_flash, 1.f);
		}
	}

	return;
}

static void player_active_camo_screen_effect(
	long player_index)
{
	struct player_datum *player;

	if (player_index != NONE)
	{
		player = player_get(player_index);
		if (player->local_player_index != NONE)
		{
			struct screen_flash_definition screen_flash = { 0 };

			screen_flash.fade_function =
				active_camo_screen_flash_fade_function;
			screen_flash.type =
				players_static_data.screen_flash_parameters[1].type;
			screen_flash.priority = 2;
			screen_flash.duration =
				players_static_data.screen_flash_parameters[1].duration;
			screen_flash.max_intensity =
				players_static_data.screen_flash_parameters[1].maximum_intensity;
			screen_flash.zero_scale_factor = 0.f;
			screen_flash.screen_flash_color.alpha =
				active_camo_screen_flash_alpha;
			screen_flash.screen_flash_color.red =
				players_static_data.screen_flash_parameters[1].red;
			screen_flash.screen_flash_color.green =
				players_static_data.screen_flash_parameters[1].green;
			screen_flash.screen_flash_color.blue =
				active_camo_screen_flash_blue;
			player_effect_screen_flash(player_index, &screen_flash, 1.f);
		}
	}

	return;
}

static void player_health_pack_screen_effect(
	long player_index)
{
	struct player_datum *player;

	if (player_index != NONE)
	{
		player = player_get(player_index);
		if (player->local_player_index != NONE)
		{
			struct screen_flash_definition screen_flash = { 0 };

			screen_flash.fade_function = 1;
			screen_flash.type = 6;
			screen_flash.priority = 2;
			screen_flash.duration = 2.f;
			screen_flash.max_intensity = 0.5f;
			screen_flash.zero_scale_factor = 0.f;
			screen_flash.screen_flash_color.alpha = 1.f;
			screen_flash.screen_flash_color.red = 0.917647f;
			screen_flash.screen_flash_color.green = 0.917647f;
			screen_flash.screen_flash_color.blue = 0.917647f;
			player_effect_screen_flash(player_index, &screen_flash, 1.f);
		}
	}

	return;
}

/* HCEA authenticates this helper and both callers. Keep the ordinary call
   topology even when VC7 chooses to inline one call site. */
static void player_powerup_on(
	long player_index,
	short powerup_type)
{
	struct player_datum *player;
	struct unit_datum *unit;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	switch (powerup_type)
	{
	case _player_powerup_active_camouflage:
		SET_FLAG(unit->unit.flags, _unit_active_camouflaged_bit, TRUE);
		unit->unit.cause_for_camo_regrowth = cause_for_camo_regrowth_default;
		break;
	}

	return;
}

/* HCEA authenticates the additional/off helpers and their call topology. */
static void player_powerup_additional(
	long player_index,
	short powerup_type)
{
	struct player_datum *player;
	struct unit_datum *unit;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	switch (powerup_type)
	{
	case _player_powerup_active_camouflage:
		SET_FLAG(unit->unit.flags, _unit_super_camouflaged_bit, TRUE);
		break;
	}

	return;
}

static void player_powerup_off(
	long player_index,
	short powerup_type)
{
	struct player_datum *player;
	struct unit_datum *unit;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	switch (powerup_type)
	{
	case _player_powerup_active_camouflage:
		SET_FLAG(unit->unit.flags, _unit_active_camouflaged_bit, FALSE);
		break;
	}

	return;
}

static void player_set_action_result(
	long player_index,
	short action_result,
	long object_index,
	short seat_index)
{
	struct player_datum *player;
	real_point3d const *unit_position;
	real_point3d const *current_position;
	real_point3d const *new_position;
	boolean set_action = FALSE;

	player = player_get(player_index);
	if (action_result == _player_action_result_flip_vehicle)
	{
		set_action = TRUE;
	}
	else if (action_result == player->action_result)
	{
		unit_position = &object_get(player->unit_index)->object.position;
		current_position =
			&object_get(player->action_object_index)->object.position;
		new_position = &object_get(object_index)->object.position;
		set_action = distance3d(unit_position, current_position) >
			distance3d(unit_position, new_position);
	}
	else if (action_result > player->action_result)
	{
		set_action = TRUE;
	}

	if (set_action)
	{
		player->action_result = action_result;
		player->action_object_index = object_index;
		player->action_seat_index = seat_index;
	}

	return;
}

static void player_examine_nearby_unit(
	long player_index,
	long unit_index)
{
	return;
}

static void player_examine_nearby_vehicle(
	long player_index,
	long vehicle_index)
{
	struct player_datum *player;
	struct players_vehicle_datum *vehicle;
	struct unit_datum *unit;
	struct game_globals_player_control *player_control;
	real flipping_angle;
	short seat_state;

	player = player_get(player_index);
	vehicle = (struct players_vehicle_datum *)vehicle_get(vehicle_index);
	if (!TEST_FLAG(vehicle->object.damage_flags, _object_dead_bit))
	{
		player_control = TAG_BLOCK_GET_ELEMENT(
			&scenario_get_game_globals()->player_control,
			0,
			struct game_globals_player_control);
		flipping_angle =
			(_pi * 0.5f) - player_control->minimum_vehicle_flipping_angle;
		if (vehicle_get(vehicle_index)->object.up.k > (real)cos(flipping_angle))
		{
			if (!unit_overcharged(player->unit_index))
			{
				unit = unit_get(player->unit_index);
				if (magnitude_squared3d(&unit->object.translational_velocity) < 0.01f)
				{
					if (magnitude_squared3d(
						&((struct players_vehicle_datum *)vehicle_get(
							vehicle_index))->object.angular_velocity) <
						0.01f)
					{
						short seat_index;

						seat_index = NONE;
						seat_state = unit_find_nearby_seat(
							player->unit_index,
							vehicle_index,
							&seat_index);
						switch (seat_state)
						{
						case _unit_nearby_seat_available:
							if (seat_index == NONE)
							{
								display_assert(
									"seat_index != NONE",
									"c:\\halo\\SOURCE\\game\\players.c",
									0x83C,
									TRUE);
								system_exit(-1);
							}
							player_set_action_result(
								player_index,
								_player_action_result_enter_vehicle,
								vehicle_index,
								seat_index);
							break;

						case _unit_nearby_seat_occupied:
							if (seat_index == NONE)
							{
								display_assert(
									"seat_index != NONE",
									"c:\\halo\\SOURCE\\game\\players.c",
									0x841,
									TRUE);
								system_exit(-1);
							}
							player_set_action_result(
								player_index,
								_player_action_result_evict_from_vehicle,
								vehicle_index,
								seat_index);
							break;
						}
					}
				}
			}
		}
		else if (!TEST_FLAG(vehicle->vehicle.flags, _vehicle_upending_bit) &&
			vehicle->unit.driver_object_index == NONE)
		{
			player_set_action_result(
				player_index,
				_player_action_result_flip_vehicle,
				vehicle_index,
				NONE);
		}
	}

	return;
}

static void player_examine_nearby_item(
	long player_index,
	long item_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct item_datum *item;
	struct item_datum *equipment;
	struct equipment_definition *equipment_definition;
	struct equipment_definition *current_equipment_definition;
	long current_equipment_index;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	item = item_get(item_index);
	if (item->object.parent_object_index != NONE ||
		item->item.ignore_object_index == player->unit_index)
	{
		return;
	}

	{
		short inventory_index;
		short ammunition_count;
		long inventory_item_index;

		for (inventory_index = 0;
			inventory_index < MAXIMUM_WEAPONS_PER_UNIT;
			inventory_index++)
		{
			inventory_item_index =
				unit->unit.weapon_object_indices[inventory_index];
			if (inventory_item_index != NONE && players_decide_pickups() &&
				weapon_handle_potential_inventory_item(
					inventory_item_index,
					item_index,
					player->local_player_index,
					&ammunition_count))
			{
				if (ammunition_count > 0)
				{
					hud_picked_up_ammunition(
						player->local_player_index,
						weapon_get(inventory_item_index)->definition_index,
						ammunition_count);
					player_network_picked_up(player, player_index, _network_pickup_ammunition,
						weapon_get(inventory_item_index)->definition_index, ammunition_count);
				}
				break;
			}
		}
	}

	equipment = equipment_try_and_get(item_index);
	if (equipment)
	{
		equipment_definition = equipment_definition_get(equipment->definition_index);
		if (equipment_definition->equipment.powerup_type == _equipment_powerup_grenade)
		{
			if (players_decide_pickups() && unit_add_grenade_to_inventory(player->unit_index, item_index))
			{
				hud_picked_up_grenade(
					player->local_player_index,
					equipment->definition_index);
				player_network_picked_up(player, player_index, _network_pickup_grenade, equipment->definition_index, 0);
			}
		}
		else if (equipment_definition->equipment.powerup_type != _equipment_powerup_none)
		{
			current_equipment_index = unit_get_current_equipment(player->unit_index);
			if (current_equipment_index == NONE)
			{
				if (players_decide_pickups())
					player_handle_powerup_equipment(player_index, item_index);
			}
			else
			{
				/* BUG (original): January code and the HCEA disassembly both
				 * re-fetch the current equipment but then use the nearby
				 * equipment's definition again. A corrected build should pass
				 * equipment_get(current_equipment_index)->definition_index to
				 * equipment_definition_get below. */
				equipment_get(current_equipment_index);
				current_equipment_definition =
					equipment_definition_get(equipment->definition_index);
				if (equipment_definition->equipment.powerup_type !=
					current_equipment_definition->equipment.powerup_type)
				{
					player_set_action_result(
						player_index,
						_player_action_result_swap_for_powerup,
						item_index,
						NONE);
				}
			}
		}
	}

	{
		struct weapon_datum *weapon;
		struct weapon_datum *current_weapon;
		struct weapon_definition *weapon_definition;
		struct weapon_definition *current_weapon_definition;
		long current_weapon_index;
		long weapon_count;
		long weapon_item_index;
		boolean weapon_trigger_active;
		boolean current_weapon_does_not_count;

		weapon_item_index = item_index;
		weapon = weapon_try_and_get(weapon_item_index);
		if (!weapon || !unit_can_use_weapon(player->unit_index, weapon_item_index))
		{
			return;
		}

		weapon_definition = weapon_definition_get(weapon->definition_index);
		weapon_trigger_active =
			TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit) ||
			TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_secondary_trigger_bit);
		current_weapon_index = unit_inventory_get_weapon(
			player->unit_index,
			unit_get(player->unit_index)->unit.current_weapon_index);
		weapon_count = unit_get_weapon_count(player->unit_index);
		current_weapon_does_not_count = FALSE;
		if (weapon_count >= 2 &&
			current_weapon_index != NONE &&
			!TEST_FLAG(
				weapon_definition->weapon.flags,
				_weapon_doesnt_count_toward_maximum_bit))
		{
			current_weapon = weapon_get(current_weapon_index);
			current_weapon_definition =
				weapon_definition_get(current_weapon->definition_index);
			if (TEST_FLAG(
				current_weapon_definition->weapon.flags,
				_weapon_doesnt_count_toward_maximum_bit))
			{
				current_weapon_does_not_count = TRUE;
			}
		}

		if (weapon_trigger_active &&
			TEST_FLAG(weapon_definition->weapon.flags, _weapon_must_be_readied_bit))
		{
			return;
		}

		if (unit_should_autopick_weapon(player->unit_index, weapon_item_index))
		{
			if (players_decide_pickups() &&
				unit_add_weapon_to_inventory(
				player->unit_index,
				weapon_item_index,
				TRUE))
			{
				hud_picked_up_weapon(
					player->local_player_index,
					weapon_get(weapon_item_index)->definition_index);
				player_network_picked_up(player, player_index, _network_pickup_weapon,
					weapon_get(weapon_item_index)->definition_index, 0);
				player_control_unzoom(player->unit_index);
			}
		}
		else if (!current_weapon_does_not_count &&
			unit_approve_weapon_swap(player->unit_index, weapon_item_index))
		{
			current_weapon = weapon_try_and_get(current_weapon_index);
			if (weapon_count == 1 &&
				current_weapon &&
				current_weapon->definition_index != weapon->definition_index)
			{
				player_set_action_result(
					player_index,
					_player_action_result_add_weapon_to_inventory,
					weapon_item_index,
					NONE);
			}
			else
			{
				player_set_action_result(
					player_index,
					_player_action_result_swap_for_weapon,
					weapon_item_index,
					NONE);
			}
		}
	}

	return;
}

static void player_examine_nearby_device(
	long player_index,
	long device_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct device_datum *device;
	real_point3d camera_position;

	player = player_get(player_index);
	unit = unit_get(player->unit_index);
	device = device_get(device_index);
	unit_get_camera_position(player->unit_index, &camera_position);
	if (fast_vector_intersects_sphere(
		&camera_position,
		&unit->unit.aiming_vector,
		&device->object.bounding_sphere_center,
		device->object.bounding_sphere_radius) &&
		device_frontfacing(device_index, &camera_position, &unit->unit.aiming_vector) &&
		device_can_change_position(device_index))
	{
		player_set_action_result(
			player_index,
			_player_action_result_touch_device,
			device_index,
			NONE);
	}

	return;
}

static void player_handle_powerup_equipment(
	long player_index,
	long equipment_index)
{
	struct player_datum *player;
	struct item_datum *equipment;
	struct equipment_definition *equipment_definition;
	short powerup_type;
	short duration;
	long powerup_index;

	player = player_get(player_index);
	equipment = equipment_get(equipment_index);
	equipment_definition = equipment_definition_get(equipment->definition_index);
	duration = (short)(equipment_definition->equipment.powerup_time * 30.f);
	if (duration <= 0)
		return;

	powerup_type = equipment_definition->equipment.powerup_type;
	if (powerup_type == _equipment_powerup_double_speed)
	{
		players_globals->double_speed_ticks += duration;
		game_set_players_are_double_speed(TRUE);
	}
	else if (powerup_type == _equipment_powerup_overshield)
	{
		if (!object_double_charge_shield(player->unit_index))
			return;
		player_over_shield_screen_effect(player_index);
	}
	else if (powerup_type == _equipment_powerup_health)
	{
		if (!object_restore_body(player->unit_index))
			return;
		player_health_pack_screen_effect(player_index);
	}
	else
	{
		switch (powerup_type)
		{
		case _equipment_powerup_active_camouflage:
			powerup_index = 0;
			break;

		case _equipment_powerup_full_spectrum_vision:
			powerup_index = 1;
			break;

		/* powerup_index is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			display_assert(
				NULL,
				"c:\\halo\\SOURCE\\game\\players.c",
				0xACB,
				TRUE);
			system_exit(-1);
		}

		if (!player_handle_powerup(player_index, (short)powerup_index, duration))
			return;
		if ((short)powerup_index == 0)
			player_active_camo_screen_effect(player_index);
	}

	equipment = equipment_get(equipment_index);
	hud_picked_up_powerup(
		(unsigned short)player->local_player_index,
		equipment->definition_index);
	player_network_picked_up(player, player_index, _network_pickup_powerup, equipment->definition_index, 0);
	if (player->local_player_index != NONE)
		equipment_handle_pickup(equipment_index);
	object_delete(equipment_index);

	return;
}

void players_handle_deleted_object(
	long deleted_object_index)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct object_datum *object;

	object = object_get(deleted_object_index);
	if (TEST_FLAG(_object_mask_unit, object->object.type))
	{
		data_iterator_new(&iterator, player_data);
		while (player = data_iterator_next(&iterator))
		{
			if (player->unit_index == deleted_object_index)
				player_died(iterator.datum_index);
		}
	}

	return;
}

static void player_examine_nearby_objects(
	long player_index)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct object_datum *object;
	long object_indices[16];
	short object_count;
	short object_number;

	player = player_get(player_index);
	if (player->unit_index != NONE)
	{
		unit = unit_get(player->unit_index);
		if (unit->object.parent_object_index == NONE)
		{
			object_count = objects_in_sphere(
				0,
				_object_mask_player_interaction,
				&unit->object.location,
				&unit->object.bounding_sphere_center,
				unit->object.bounding_sphere_radius,
				object_indices,
				NUMBEROF(object_indices));
			for (object_number = 0; object_number < object_count; object_number++)
			{
				object = object_get(object_indices[object_number]);
				switch (object->object.type)
				{
				case _object_type_biped:
					player_examine_nearby_unit(player_index, object_indices[object_number]);
					break;

				case _object_type_vehicle:
					player_examine_nearby_vehicle(player_index, object_indices[object_number]);
					break;

				case _object_type_weapon:
				case _object_type_equipment:
					player_examine_nearby_item(player_index, object_indices[object_number]);
					break;

				case _object_type_control:
					player_examine_nearby_device(player_index, object_indices[object_number]);
					break;
				}
			}
		}
	}

	return;
}

static long create_weapon(
	struct scenario_starting_profile_weapon const *starting_weapon,
	long unit_index)
{
	struct object_placement_data placement_data;
	struct weapon_datum *weapon;
	long weapon_index;

	weapon_index = NONE;
	if (starting_weapon->weapon.index != NONE)
	{
		object_placement_data_new(
			&placement_data,
			starting_weapon->weapon.index,
			unit_index);
		weapon_index = object_new(&placement_data);
		if (weapon_index != NONE)
		{
			weapon = weapon_get(weapon_index);
			weapon->weapon.magazines[0].rounds_total =
				starting_weapon->rounds_total;
			weapon->weapon.magazines[0].rounds_loaded =
				starting_weapon->rounds_loaded;
		}
	}

	return weapon_index;
}

void player_add_equipment(
	long unit_index,
	short starting_profile_index,
	boolean reset_equipment)
{
	struct unit_datum *unit;
	struct scenario_starting_profile *starting_profile;
	struct scenario *scenario;
	long weapon_index;
	short grenade_index;

	if (unit_index != NONE && starting_profile_index != NONE)
	{
		unit = unit_try_and_get(unit_index);
		if (unit->unit.player_index != NONE)
		{
			scenario = global_scenario_get();
			starting_profile = TAG_BLOCK_GET_ELEMENT(
				&scenario->starting_profiles,
				starting_profile_index,
				struct scenario_starting_profile);

			if (reset_equipment)
			{
				unit_delete_all_weapons(unit_index);
				unit->object.shield_vitality = 0.f;
				unit->object.body_vitality = 0.f;
				*(short *)unit->unit.grenade_counts = 0;
			}

			if (starting_profile->primary_weapon.weapon.index != NONE)
			{
				weapon_index = create_weapon(
					&starting_profile->primary_weapon,
					unit_index);
				if (weapon_index != NONE &&
					!unit_add_weapon_to_inventory(
						unit_index,
						weapon_index,
						reset_equipment != FALSE))
				{
					error(2, "Could not attach starting weapon to player");
					object_delete(weapon_index);
				}
			}

			if (starting_profile->secondary_weapon.weapon.index != NONE)
			{
				weapon_index = create_weapon(
					&starting_profile->secondary_weapon,
					unit_index);
				if (weapon_index != NONE &&
					!unit_add_weapon_to_inventory(
						unit_index,
						weapon_index,
						FALSE))
				{
					error(2, "Could not attach starting weapon to player");
					object_delete(weapon_index);
				}
			}

			unit->object.shield_vitality +=
				starting_profile->starting_shield_modifier;
			unit->object.body_vitality +=
				starting_profile->starting_health_modifier;
			unit = (struct unit_datum *)unit->unit.grenade_counts;
			starting_profile =
				(struct scenario_starting_profile *)starting_profile->grenade_counts;
			grenade_index = NUMBER_OF_UNIT_GRENADE_TYPES;
			do
			{
				*(byte *)unit += *(byte *)starting_profile;
				starting_profile = (struct scenario_starting_profile *)(
					(byte *)starting_profile + 1);
				unit = (struct unit_datum *)((byte *)unit + 1);
			}
			while (--grenade_index);
		}
	}

	return;
}

void player_aiming_vector_from_facing(
	long player_index,
	real_vector3d *aiming_vector,
	real_euler_angles2d const *facing)
{
	struct player_datum *player;
	struct unit_datum *unit;
	struct players_vehicle_datum *vehicle;
	struct unit_definition *vehicle_definition;
	struct unit_seat *seat;
	real_matrix4x3 rotation;

	player = player_get(player_index);
	vector3d_from_euler_angles2d(aiming_vector, facing);
	if (player->unit_index != NONE)
	{
		unit = unit_get(player->unit_index);
		if (unit->object.parent_object_index != NONE)
		{
			vehicle = (struct players_vehicle_datum *)object_try_and_get_and_verify_type(
				unit->object.parent_object_index,
				_object_mask_vehicle);
			if (vehicle)
			{
				vehicle_definition = vehicle_definition_get(
					vehicle->definition_index);
				seat = TAG_BLOCK_GET_ELEMENT(
					&vehicle_definition->unit.seats,
					unit->unit.parent_seat_index,
					struct unit_seat);
				if (!TEST_FLAG(seat->flags, _unit_seat_third_person_camera_bit))
				{
					cross_product3d(
						&vehicle->object.up,
						global_right3d,
						&rotation.forward);
					if (normalize3d(&rotation.forward) == 0.f)
					{
						cross_product3d(
							&vehicle->object.up,
							global_down3d,
							&rotation.forward);
						normalize3d(&rotation.forward);
					}
					matrix4x3_rotation_from_vectors(
						&rotation,
						&rotation.forward,
						&vehicle->object.up);
					matrix4x3_transform_normal(
						&rotation,
						aiming_vector,
						aiming_vector);
				}
			}
		}
	}

	return;
}

void players_update_before_game(
	void)
{
	struct player_action actions[NETWORK_GAME_MAXIMUM_PLAYER_COUNT];
	struct data_iterator iterator;
	struct player_datum *player;
	struct unit_datum *unit;
	struct player_action *action;
	struct unit_control_data control_data;
	long weapon_index;
	short action_index;

	profile_enter(PLAYERS_UPDATE_BEFORE_GAME_PROFILE);
	if (update_client_dequeue(actions))
	{
		data_iterator_new(&iterator, player_data);
		while (player = data_iterator_next(&iterator))
		{
			action_index = (short)iterator.datum_index;
			action = &actions[action_index];
			match_assert(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x256,
				action_index>=0 && action_index<NETWORK_GAME_MAXIMUM_PLAYER_COUNT);
			match_assert_valid_real(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x25B,
				action->desired_facing.pitch);
			match_assert_valid_real(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x25C,
				action->desired_facing.yaw);
			match_vassert(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x25D,
				valid_real_vector2d(&action->throttle),
				csprintf(
					temporary,
					"%s: assert_valid_real_vector2d(%f, %f)",
					"&action->throttle",
					action->throttle.i,
					action->throttle.j));
			match_assert_valid_real(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x25E,
				action->primary_trigger);
			match_assert(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x260,
				(NONE == action->desired_weapon_index) ||
				((action->desired_weapon_index >= 0) &&
				(action->desired_weapon_index <= MAXIMUM_WEAPONS_PER_UNIT)));
			match_assert(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x261,
				(NONE == action->desired_grenade_index) ||
				((action->desired_grenade_index >= 0) &&
				(action->desired_grenade_index <= NUMBER_OF_UNIT_GRENADE_TYPES)));
			match_assert(
				"c:\\halo\\SOURCE\\game\\players.c",
				0x262,
				(NONE == action->desired_zoom_level) || ((action->desired_zoom_level >= 0)));

			if (player->unit_index == NONE && !game_in_editor())
			{
				if (game_engine_running())
				{
					/* (a client of the distributed netcode's players take the units
					the host spawns them with, network_player_attach_unit) */
					if (
#ifdef HALO_LINUX
						!network_game_distributed_client() &&
#endif
						game_engine_should_spawn_player(iterator.datum_index))
					{
						game_engine_prespawn_player_update(iterator.datum_index);
						player_spawn(iterator.datum_index);
						if (player->unit_index != NONE)
							game_engine_postspawn_player_update(iterator.datum_index);
						else
							player->respawn_timer = 1;
					}
				}
				else if (!main_menu_is_active())
				{
					if (player->statistics.deaths == 0)
						player_spawn(iterator.datum_index);
					else if (!players_globals->all_dead)
						main_respawn(players_globals->respawn_failed);
				}
			}

			if (player->unit_index != NONE && unit_controllable(player->unit_index))
			{
				unit = unit_get(player->unit_index);
				if (!players_globals->input_disabled)
				{
#ifdef HALO_LINUX
					network_player_log_idle_action(iterator.datum_index, action->control_flags);
#endif
					if (TEST_FLAG(action->control_flags, _unit_control_action_bit) &&
						unit->object.parent_object_index == NONE &&
						!player_handle_action(iterator.datum_index))
					{
						SET_FLAG(action->control_flags, _unit_control_weapon_reload_bit, TRUE);
					}

					if (TEST_FLAG(action->control_flags, _unit_control_swap_weapons_bit) &&
						unit->object.parent_object_index == NONE)
					{
						if (!player->swapped_weapons)
							player->swapped_weapons = player_handle_weapon_swap(iterator.datum_index);
					}
					else
					{
						player->swapped_weapons = FALSE;
					}

					if (TEST_FLAG(action->control_flags, _unit_control_use_equipment_bit) &&
						unit->unit.equipment_object_index != NONE)
					{
						player_handle_powerup_equipment(
							iterator.datum_index,
							unit->unit.equipment_object_index);
						unit_delete_current_equipment(player->unit_index);
					}

					weapon_index = unit_inventory_get_weapon(
						player->unit_index,
						unit_get(player->unit_index)->unit.current_weapon_index);
					if (weapon_index != NONE && weapon_must_be_readied(weapon_index))
					{
						if (TEST_FLAG(action->control_flags, _unit_control_weapon_primary_trigger_bit) ||
							TEST_FLAG(action->control_flags, _unit_control_weapon_secondary_trigger_bit))
						{
							unit_drop_current_weapon(player->unit_index, TRUE);
						}
						action->desired_weapon_index = unit->unit.current_weapon_index;
					}

					control_data.control_flags = (word)action->control_flags;
					player_aiming_vector_from_facing(
						iterator.datum_index,
						&control_data.aiming_vector,
						&action->desired_facing);
					control_data.facing_vector =
						control_data.looking_vector = control_data.aiming_vector;
#ifdef HALO_N64
					if (player->local_player_index != NONE)
						n64_golden_ray(player->local_player_index, (float *)&control_data.aiming_vector);
#endif
					control_data.throttle.i = action->throttle.i;
					control_data.throttle.j = action->throttle.j;
					control_data.throttle.k = 0.f;
					control_data.primary_trigger = action->primary_trigger;
					control_data.animation_state = _unit_animation_state_in_combat;
					control_data.weapon_index = action->desired_weapon_index;
					control_data.grenade_index = action->desired_grenade_index;
					control_data.zoom_level = action->desired_zoom_level;
					control_data.aiming_speed = 0;

					match_assert(
						"c:\\halo\\SOURCE\\game\\players.c",
						0x2E3,
						(NONE == control_data.weapon_index) ||
						((control_data.weapon_index >= 0) &&
						(control_data.weapon_index <= MAXIMUM_WEAPONS_PER_UNIT)));
					match_assert(
						"c:\\halo\\SOURCE\\game\\players.c",
						0x2E4,
						(NONE == control_data.grenade_index) ||
						((control_data.grenade_index >= 0) &&
						(control_data.grenade_index <= NUMBER_OF_UNIT_GRENADE_TYPES)));
					match_assert(
						"c:\\halo\\SOURCE\\game\\players.c",
						0x2E5,
						(NONE == control_data.zoom_level) || ((control_data.zoom_level >= 0)));
					unit_control(player->unit_index, &control_data);
				}
				else if (unit->unit.swarm_actor_index == NONE &&
					unit->unit.actor_index == NONE)
				{
					struct unit_control_data inhibited_control_data;

					inhibited_control_data.animation_state = _unit_animation_state_in_combat;
					inhibited_control_data.aiming_speed = 0;
					inhibited_control_data.control_flags = 0;
					inhibited_control_data.weapon_index = NONE;
					inhibited_control_data.grenade_index = NONE;
					inhibited_control_data.zoom_level = NONE;
					inhibited_control_data.throttle = *global_zero_vector3d;
					inhibited_control_data.facing_vector = unit->unit.desired_facing_vector;
					inhibited_control_data.aiming_vector = unit->unit.desired_aiming_vector;
					inhibited_control_data.primary_trigger = 0.f;
					inhibited_control_data.looking_vector = unit->unit.desired_looking_vector;
					unit_control(player->unit_index, &inhibited_control_data);
				}
			}
		}

		players_compute_combined_pvs(players_globals->combined_pvs_local, TRUE);
		players_compute_combined_pvs(players_globals->combined_pvs, FALSE);
		players_globals->local_player_count = (short)players_compute_local_player_count();
	}
	else
	{
		display_assert(NULL, "c:\\halo\\SOURCE\\game\\players.c", 0x30B, TRUE);
		system_exit(-1);
	}

	profile_exit(PLAYERS_UPDATE_BEFORE_GAME_PROFILE);

	return;
}

void players_update_after_game(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	struct unit_datum *unit;
	struct object_datum *root_object;
	struct scenario *scenario;
	struct scenario_bsp_switch_trigger_volume *bsp_switch_trigger_volume;
	long telefrag_ticks;
	long root_object_index;
	long message_list_index;
	short bsp_switch_trigger_volume_index;

	profile_enter(PLAYERS_UPDATE_AFTER_GAME_PROFILE);

	if (players_globals->double_speed_ticks > 0)
	{
		players_globals->double_speed_ticks--;
		if (players_globals->double_speed_ticks == 0)
			game_set_players_are_double_speed(FALSE);
	}

	data_iterator_new(&iterator, player_data);
	while (player = data_iterator_next(&iterator))
	{
		if (!player->is_blocking_teleporter)
		{
			if (player->telefrag_timeout > 0)
				player->telefrag_timeout--;
		}
		else
		{
			telefrag_ticks = player->telefrag_timeout;
			if (telefrag_ticks >= 90)
			{
				if (player->unit_index != NONE)
				{
					unit = unit_get(player->unit_index);
					if (!TEST_FLAG(unit->object.damage_flags, _object_die_act_of_god_bit))
					{
						if (player->local_player_index != NONE)
						{
							message_list_index = tag_loaded(
								UNICODE_STRING_LIST_TAG,
								"ui\\multiplayer_game_text");
							hud_print_message(
								player->local_player_index,
								message_list_index != NONE
									? unicode_string_list_get_string(
										message_list_index,
										MULTIPLAYER_GAME_TEXT_YOU_WERE_TELEFRAGGED)
									: L"");
						}

						player_telefrag_effect_stop(iterator.datum_index);
						unit_kill(player->unit_index);
					}
				}
			}
			else
			{
				player_telefrag_effect_start(
					iterator.datum_index,
					(real)telefrag_ticks * (1.f / 90.f));
			}
		}

		player->is_blocking_teleporter = FALSE;
		if (player->unit_index != NONE)
			player_update_powerups(iterator.datum_index);

		if (player->unit_index != NONE)
		{
			root_object_index = object_get_ultimate_parent(player->unit_index);
			root_object = object_get(root_object_index);
			if (!TEST_FLAG(root_object->object.flags, _object_outside_of_map_bit))
			{
				scenario = global_scenario_get();
				for (bsp_switch_trigger_volume_index = 0;
					bsp_switch_trigger_volume_index < scenario->bsp_switch_trigger_volumes.count;
					bsp_switch_trigger_volume_index++)
				{
					bsp_switch_trigger_volume = TAG_BLOCK_GET_ELEMENT(
						&scenario->bsp_switch_trigger_volumes,
						bsp_switch_trigger_volume_index,
						struct scenario_bsp_switch_trigger_volume);
					if (bsp_switch_trigger_volume->source_structure_bsp_index ==
						global_structure_bsp_index &&
						scenario_trigger_volume_test_object(
							bsp_switch_trigger_volume->trigger_volume_index,
							player->unit_index))
					{
						if (players_globals->local_player_triggered_switch !=
							_local_player_triggered_switch_none &&
							players_globals->local_player_triggered_switch !=
							player->local_player_index)
						{
							error(
								2,
								"!!!WARNING!!! teleported player triggering a bsp switch!!!");
						}

						players_globals->bsp_check_recursive_switch_ticks = 0;
						players_globals->local_player_triggered_switch =
							player->local_player_index;
						players_globals->pending_teleport_starting_location_index =
							bsp_switch_trigger_volume_index;
						main_switch_structure_bsp(
							bsp_switch_trigger_volume->destination_structure_bsp_index);
					}
				}
			}
		}

#ifdef HALO_N64
		{
			/* a bot's (port/n64/game/n64_bots.c) only before a tick it
			thinks: an object search around it each tick, for 8 bots */
			extern boolean n64_bots_examine_turn(long player_index);

			if (!n64_bots_examine_turn(iterator.datum_index))
				continue;
		}
#endif
		player_reset_action_result(iterator.datum_index);
		player_examine_nearby_objects(iterator.datum_index);
	}

	if (players_globals->local_player_triggered_switch !=
		_local_player_triggered_switch_none)
	{
		players_globals->bsp_check_recursive_switch_ticks++;
		if (players_globals->bsp_check_recursive_switch_ticks > 0xC)
		{
			players_globals->local_player_triggered_switch =
				_local_player_triggered_switch_none;
			players_globals->bsp_check_recursive_switch_ticks = 0;
		}
	}

	if (players_globals->all_dead)
	{
		if (!game_engine_running() && !players_lost_map_started)
		{
#ifndef HALO_N64_CAMPAIGN
			/* (the campaign cartridge brings them back itself:
			n64_player_respawn_at) */
			main_lost_map();
#endif
			players_lost_map_started = TRUE;
		}
	}
	else if (players_lost_map_started)
	{
		players_lost_map_started = FALSE;
	}

	profile_exit(PLAYERS_UPDATE_AFTER_GAME_PROFILE);

	return;
}

/* ---------- private code */
