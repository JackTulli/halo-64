/*
HUD_NAV_POINTS.C

symbols in this file:
000C5240 0080:
	_find_nav_point (0000)
000C52C0 0070:
	_get_nav_point_datum (0000)
000C5330 0040:
	_hud_initialize_nav_points (0000)
000C5370 0020:
	_hud_initialize_nav_points_for_new_map (0000)
000C5390 0010:
	_hud_dispose_nav_points_from_old_map (0000)
000C53A0 0010:
	_hud_dispose_nav_points (0000)
000C53B0 00f0:
	_code_000c53b0 (0000)
000C54A0 0020:
	_hud_activate_nav_point_with_game_engine_flag (0000)
000C54C0 0020:
	_hud_activate_nav_point_with_flag (0000)
000C54E0 0020:
	_hud_activate_nav_point_with_object (0000)
000C5500 0070:
	_code_000c5500 (0000)
000C5570 0030:
	_hud_activate_team_nav_point_with_game_engine_flag (0000)
000C55A0 0030:
	_hud_activate_team_nav_point_with_flag (0000)
000C55D0 0030:
	_hud_activate_team_nav_point_with_object (0000)
000C5600 0070:
	_code_000c5600 (0000)
000C5670 0030:
	_hud_activate_global_nav_point_with_game_engine_flag (0000)
000C56A0 0070:
	_code_000c56a0 (0000)
000C5710 0020:
	_hud_deactivate_nav_point_with_game_engine_flag (0000)
000C5730 0020:
	_hud_deactivate_nav_point_with_flag (0000)
000C5750 0020:
	_hud_deactivate_nav_point_with_object (0000)
000C5770 0060:
	_code_000c5770 (0000)
000C57D0 0020:
	_hud_deactivate_team_nav_point_with_flag (0000)
000C57F0 0020:
	_hud_deactivate_team_nav_point_with_object (0000)
000C5810 0030:
	_hud_unit_activate_nav_point_with_flag (0000)
000C5840 0030:
	_hud_unit_activate_nav_point_with_object (0000)
000C5870 0030:
	_hud_unit_deactivate_nav_point_with_flag (0000)
000C58A0 0030:
	_hud_unit_deactivate_nav_point_with_object (0000)
000C58D0 0110:
	_hud_get_nav_point_render_type (0000)
000C59E0 0660:
	_custom_render_nav_point (0000)
000C6040 0190:
	_hud_render_nav_points (0000)
000C61D0 0230:
	_code_000c61d0 (0000)
000C6400 0030:
	_hud_update_nav_points (0000)
00270134 0019:
	??_C@_0BJ@MJDIOPPI@could?5not?5find?5nav?5point?$AA@ (0000)
00270150 000f:
	??_C@_0P@MOLLEBG@nav_point_data?$AA@ (0000)
00270160 004a:
	??_C@_0EK@HLGJFFJP@local_player_index?$DO?$DN0?$CG?$CGlocal_pla@ (0000)
002701AC 002a:
	??_C@_0CK@OOCBMBAL@c?3?2halo?2SOURCE?2interface?2hud_nav@ (0000)
002701D8 000f:
	??_C@_0P@FGJPNLAM@hud?5nav?5points?$AA@ (0000)
002701E8 0020:
	??_C@_0CA@LFILMGD@Could?5not?5add?5another?5nav?5point?$AA@ (0000)
00270208 0008:
	__real@4010000000000000 (0000)
00270210 0008:
	__real@4024000000000000 (0000)
00270218 0004:
	__real@3f28f5c3 (0000)
0027021C 0004:
	__real@3ea8f5c3 (0000)
00270220 0004:
	__real@4043126f (0000)
00270224 0014:
	??_C@_0BE@OJIBABCH@waypoint_type?$CB?$DNNONE?$AA@ (0000)
00270238 0008:
	__real@3fe6666666666666 (0000)
00453ABC 0004:
	_nav_point_data (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "game_state.h"
#include "game/players.h"
#include "networking/network_connection.h"
#include "memory/data.h"
#include "physics/collisions.h"
#include "physics/collision_usage.h"
#include "math/integer_math.h"
#include "math/real_math.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmap_utilities.h"
#include "cache/texture_cache.h"
#include "tag_files/tag_groups.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/hud_draw.h"
#include "objects/object_types.h"
#include "render/render.h"
#include "render/render_cameras_internal.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/units.h"
#ifdef HALO_N64
#include "game/game.h"
#endif

/* ---------- constants */

enum
{
	MAXIMUM_NUMBER_OF_NAV_POINTS = 4
};

enum
{
	_hud_nav_point_type_flag,
	_hud_nav_point_type_object,
	_hud_nav_point_type_game_engine_flag
};

enum waypoint_type
{
	_waypoint_on_screen,
	_waypoint_off_screen,
	_waypoint_occluded,
	NUMBER_OF_WAYPOINT_TYPES
};

enum hud_waypoint_arrow_flags
{
	_hud_waypoint_dont_rotate_offscreen_bit,
	NUMBER_OF_HUD_WAYPOINT_ARROW_FLAGS
};

enum hud_number_show_flags
{
	_hud_number_show_all_leading_zeros_bit,
	_hud_number_show_only_when_zoomed_bit,
	_hud_number_show_trailing_m_bit,
	NUMBER_OF_HUD_NUMBER_SHOW_FLAGS
};

/* ---------- structures */

struct number_hud_element_definition
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	byte digits;
	byte number_flags;
	byte fractional_digits;
	byte pad;
	long unused[3];
};

struct hud_waypoint_arrow
{
	char name[0x20];
	long unused0[2];
	unsigned long color;
	real opacity;
	real fade;
	short sequence_indices[3];
	short pad;
	long unused1[4];
	unsigned long flags;
	long unused2[6];
};

struct hud_nav_point_datum
{
	short nav_index;
	short type : 4;
	short screen_type : 4;
	float z_offset;
	long reference_index;
};

struct hud_nav_point_player_datum
{
	struct hud_nav_point_datum nav_points[MAXIMUM_NUMBER_OF_NAV_POINTS];
};

struct scenario_cutscene_flag
{
	long runtime_unused;
	char name[TAG_STRING_LENGTH];
	real_point3d position;
	real_euler_angles2d facing;
	byte unused[0x24];
};

struct hud_nav_object_datum
{
	byte unused[0xB6];
	word damage_flags;
};

#define hud_nav_object_try_and_get(object_index) \
	((struct hud_nav_object_datum *)object_try_and_get_and_verify_type((object_index), _object_mask_all))

/* ---------- prototypes */

static void hud_update_nav_point_local_player(
	short local_player_index);

void *object_try_and_get_and_verify_type(
	long object_index,
	unsigned long valid_type_flags);

void object_get_bounding_sphere(
	long object_index,
	real_point3d *center,
	real *radius);

real_point3d *game_engine_get_goal_position(
	real_point3d *position,
	short goal_index);

void custom_render_nav_point(
	short local_player_index,
	real_point3d const *position,
	short nav_index,
	short render_type);

void game_engine_render_nav_points(
	short local_player_index);

void unit_get_head_position(
	long unit_index,
	real_point3d *head_position);

/* ---------- globals */

static struct hud_nav_point_player_datum *nav_point_data;

/* ---------- private code */

short find_nav_point(
	char const *name)
{
	short nav_index = NONE;

	if (hud_globals)
	{
		short index;

		for (index = 0; index<hud_globals->waypoint.arrows.count; index++)
		{
			struct hud_waypoint_arrow *arrow = TAG_BLOCK_GET_ELEMENT(
				&hud_globals->waypoint.arrows, index, struct hud_waypoint_arrow);

			if (!_stricmp(name, arrow->name))
			{
				nav_index = index;
				break;
			}
		}
	}

	if (nav_index==NONE)
	{
		error(2, "could not find nav point");
	}

	return nav_index;
}

struct hud_nav_point_player_datum *get_nav_point_datum(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 95, local_player_index>=0&&local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 96, nav_point_data);

	return &nav_point_data[local_player_index];
}

static void hud_activate_nav_point(
	short nav_index,
	long player_index,
	short type,
	long reference_index,
	float vertical_offset)
{
	if (player_index!=NONE)
	{
		short local_player_index = player_get(player_index)->local_player_index;

		if (local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS &&
			reference_index!=NONE && nav_index!=NONE)
		{
			struct hud_nav_point_player_datum *datum = get_nav_point_datum(local_player_index);
			short empty_index = NONE;
			short index;

			for (index = 0; index<MAXIMUM_NUMBER_OF_NAV_POINTS; index++)
			{
				struct hud_nav_point_datum *nav_point = &datum->nav_points[index];

				if (nav_point->type==type && nav_point->reference_index==reference_index)
				{
					nav_point->nav_index = nav_index;
					nav_point->z_offset = vertical_offset;

					return;
				}

				if (nav_point->type==NONE)
					empty_index = index;
			}

			if (empty_index!=NONE)
			{
				struct hud_nav_point_datum *nav_point = &datum->nav_points[empty_index];

				nav_point->type = type;
				nav_point->reference_index = reference_index;
				nav_point->nav_index = nav_index;
				nav_point->z_offset = vertical_offset;
			}
			else
			{
				error(2, "Could not add another nav point");
			}
		}
	}

	return;
}

static void hud_activate_team_nav_point(
	short nav_index,
	short team_index,
	short type,
	long reference_index,
	float vertical_offset)
{
	struct data_iterator player_iterator;
	struct player_datum *player;

	data_iterator_new(&player_iterator, player_data);

	while ((player = data_iterator_next(&player_iterator))!=NULL)
	{
		if (player->local_player_index!=NONE && team_index==player->team_index)
		{
			hud_activate_nav_point(nav_index, player_iterator.datum_index, type,
				reference_index, vertical_offset);
		}
	}

	return;
}

static void hud_activate_global_nav_point(
	short nav_index,
	short type,
	short reference_index,
	float vertical_offset)
{
	struct data_iterator player_iterator;
	struct player_datum *player;

	data_iterator_new(&player_iterator, player_data);

	while ((player = data_iterator_next(&player_iterator))!=NULL)
	{
		if (player->local_player_index!=NONE)
		{
			hud_activate_nav_point(nav_index, player_iterator.datum_index, type,
				reference_index, vertical_offset);
		}
	}

	return;
}

static void hud_deactivate_nav_point(
	long player_index,
	short type,
	long reference_index)
{
	if (player_index!=NONE)
	{
		short local_player_index = player_get(player_index)->local_player_index;

		if (local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS &&
			reference_index!=NONE)
		{
			struct hud_nav_point_player_datum *datum = get_nav_point_datum(local_player_index);
			short index;

			for (index = 0; index<MAXIMUM_NUMBER_OF_NAV_POINTS; index++)
			{
				struct hud_nav_point_datum *nav_point = &datum->nav_points[index];

				if (nav_point->type==type && nav_point->reference_index==reference_index)
				{
					nav_point->type = NONE;
					nav_point->reference_index = NONE;
					nav_point->nav_index = NONE;

					break;
				}
			}
		}
	}

	return;
}

static void hud_deactivate_team_nav_point(
	short team_index,
	short type,
	long reference_index)
{
	struct data_iterator player_iterator;
	struct player_datum *player;

	data_iterator_new(&player_iterator, player_data);

	while ((player = data_iterator_next(&player_iterator))!=NULL)
	{
		if (player->local_player_index!=NONE && team_index==player->team_index)
		{
			hud_deactivate_nav_point(player_iterator.datum_index, type, reference_index);
		}
	}

	return;
}

/* ---------- public code */

void hud_initialize_nav_points(
	void)
{
	nav_point_data = game_state_malloc("hud nav points", NULL, 0xC0);
	match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 106, nav_point_data);

	return;
}

void hud_initialize_nav_points_for_new_map(
	void)
{
	csmemset(nav_point_data, NONE, 0xC0);

	return;
}

void hud_dispose_nav_points_from_old_map(
	void)
{
	return;
}

void hud_dispose_nav_points(
	void)
{
	return;
}

void hud_activate_nav_point_with_game_engine_flag(
	short nav_index,
	long player_index,
	short flag_index,
	float vertical_offset)
{
	hud_activate_nav_point(nav_index, player_index, _hud_nav_point_type_game_engine_flag, flag_index, vertical_offset);

	return;
}

void hud_activate_nav_point_with_flag(
	short nav_index,
	long player_index,
	short flag_index,
	float vertical_offset)
{
	hud_activate_nav_point(nav_index, player_index, _hud_nav_point_type_flag, flag_index, vertical_offset);

	return;
}

void hud_activate_nav_point_with_object(
	short nav_index,
	long player_index,
	long object_index,
	float vertical_offset)
{
	hud_activate_nav_point(nav_index, player_index, _hud_nav_point_type_object, object_index, vertical_offset);

	return;
}

void hud_activate_team_nav_point_with_game_engine_flag(
	short nav_index,
	short team_index,
	short flag_index,
	float vertical_offset)
{
	hud_activate_team_nav_point(nav_index, team_index, _hud_nav_point_type_game_engine_flag, flag_index, vertical_offset);

	return;
}

void hud_activate_team_nav_point_with_flag(
	short nav_index,
	short team_index,
	short flag_index,
	float vertical_offset)
{
	hud_activate_team_nav_point(nav_index, team_index, _hud_nav_point_type_flag, flag_index, vertical_offset);

	return;
}

void hud_activate_team_nav_point_with_object(
	short nav_index,
	short team_index,
	long object_index,
	float vertical_offset)
{
	hud_activate_team_nav_point(nav_index, team_index, _hud_nav_point_type_object, object_index, vertical_offset);

	return;
}

void hud_activate_global_nav_point_with_game_engine_flag(
	short nav_index,
	short flag_index,
	float vertical_offset)
{
	hud_activate_global_nav_point(nav_index, _hud_nav_point_type_game_engine_flag, flag_index, vertical_offset);

	return;
}

void hud_deactivate_nav_point_with_game_engine_flag(
	long player_unit_index,
	short flag_index)
{
	hud_deactivate_nav_point(player_unit_index, _hud_nav_point_type_game_engine_flag, flag_index);

	return;
}

void hud_deactivate_nav_point_with_flag(
	long player_unit_index,
	short flag_index)
{
	hud_deactivate_nav_point(player_unit_index, _hud_nav_point_type_flag, flag_index);

	return;
}

void hud_deactivate_nav_point_with_object(
	long player_unit_index,
	long object_index)
{
	hud_deactivate_nav_point(player_unit_index, _hud_nav_point_type_object, object_index);

	return;
}

void hud_deactivate_team_nav_point_with_flag(
	short team_index,
	short flag_index)
{
	hud_deactivate_team_nav_point(team_index, _hud_nav_point_type_flag, flag_index);

	return;
}

void hud_deactivate_team_nav_point_with_object(
	short team_index,
	long object_index)
{
	hud_deactivate_team_nav_point(team_index, _hud_nav_point_type_object, object_index);

	return;
}

void hud_unit_activate_nav_point_with_flag(
	short nav_index,
	long unit_index,
	short flag_index,
	float vertical_offset)
{
	long player_index = player_index_from_unit_index(unit_index);

	if (player_index!=NONE)
	{
		hud_activate_nav_point(nav_index, player_index, _hud_nav_point_type_flag, flag_index, vertical_offset);
	}

	return;
}

void hud_unit_activate_nav_point_with_object(
	short nav_index,
	long unit_index,
	long object_index,
	float vertical_offset)
{
	long player_index = player_index_from_unit_index(unit_index);

	if (player_index!=NONE)
	{
		hud_activate_nav_point(nav_index, player_index, _hud_nav_point_type_object, object_index, vertical_offset);
	}

	return;
}

void hud_unit_deactivate_nav_point_with_flag(
	long unit_index,
	short flag_index)
{
	long player_index = player_index_from_unit_index(unit_index);

	if (player_index!=NONE)
	{
		hud_deactivate_nav_point(player_index, _hud_nav_point_type_flag, flag_index);
	}

	return;
}

void hud_unit_deactivate_nav_point_with_object(
	long unit_index,
	long object_index)
{
	long player_index = player_index_from_unit_index(unit_index);

	if (player_index!=NONE)
	{
		hud_deactivate_nav_point(player_index, _hud_nav_point_type_object, object_index);
	}

	return;
}

short hud_get_nav_point_render_type(
	short local_player_index,
	real_point3d const *head,
	real_point3d const *position,
	long reference_object_index)
{
	struct collision_result result;
	real_vector3d vector;
	long player_unit_index;
	short render_type;
#ifdef HALO_N64
	/* the line of sight to a nav point (a teammate's), tested again only
	every few ticks: a ray through the BSP and the objects each HUD
	recording was a tenth of the HUD's time */
	static struct { long reference_object_index; long time; short render_type; } n64_seen[MAXIMUM_LOCAL_PLAYERS][8];
	short n64_slot = (short)(reference_object_index == NONE ? 7 :
		(DATUM_INDEX_TO_ABSOLUTE_INDEX(reference_object_index) % 7));

	if (local_player_index >= 0 && local_player_index < MAXIMUM_LOCAL_PLAYERS &&
		n64_seen[local_player_index][n64_slot].reference_object_index == reference_object_index &&
		game_time_get() - n64_seen[local_player_index][n64_slot].time < 6 &&
		game_time_get() >= n64_seen[local_player_index][n64_slot].time)
	{
		return n64_seen[local_player_index][n64_slot].render_type;
	}
#endif

	match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 510, global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = 20;

	player_unit_index = local_player_get_player_index(local_player_index)==NONE ?
		NONE :
		player_get(local_player_get_player_index(local_player_index))->unit_index;

	vector.i = position->x-head->x;
	vector.j = position->y-head->y;
	vector.k = position->z-head->z;

	if (collision_test_vector(_collision_test_for_line_of_sight_flags, head, &vector,
			player_unit_index, &result) &&
		(result.type!=3 || result.object_index!=reference_object_index))
	{
		render_type = 2;
	}
	else
	{
		render_type = 0;
	}

	match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 528, global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

#ifdef HALO_N64
	if (local_player_index >= 0 && local_player_index < MAXIMUM_LOCAL_PLAYERS)
	{
		n64_seen[local_player_index][n64_slot].reference_object_index = reference_object_index;
		n64_seen[local_player_index][n64_slot].time = game_time_get();
		n64_seen[local_player_index][n64_slot].render_type = render_type;
	}
#endif
	return render_type;
}

void custom_render_nav_point(
	short local_player_index,
	real_point3d const *position,
	short nav_index,
	short waypoint_type)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct hud_waypoint_arrow *arrow;
	real_point3d view_point;
	real distance;
	real arrow_scale;
	real_point2d screen_point;
	real horizontal_radius;
	real vertical_radius;
	real vertical_component;
	real horizontal_component;
	real radius_product;
	real theta;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	arrow = TAG_BLOCK_GET_ELEMENT(
		&hud_globals->waypoint.arrows,
		nav_index,
		struct hud_waypoint_arrow);
	view_point = *position;

	{
		long unit_index = local_player_get_player_index(local_player_index)==NONE ?
			NONE :
			player_get(local_player_get_player_index(local_player_index))->unit_index;
		real_point3d camera_position;
		real delta_x;
		real delta_y;
		real delta_z;

		unit_get_camera_position(unit_index, &camera_position);

		delta_x = position->x-camera_position.x;
		delta_y = position->y-camera_position.y;
		delta_z = position->z-camera_position.z;
		distance = square_root(
			delta_x*delta_x + (delta_y*delta_y + delta_z*delta_z));
	}

	if (distance>15.0f)
	{
		arrow_scale = 0.5f;
	}
	else
	{
		arrow_scale = (real)pow(
			1.0f-distance*(1.0f/15.0f),
			0.7) + 0.5f;
	}

	matrix4x3_transform_point(
		&render.frustum.world_to_view,
		&view_point,
		&view_point);

	if (waypoint_type==_waypoint_off_screen ||
		!render_camera_view_to_screen(
			&render.camera,
			&render.frustum,
			&view_point,
			&screen_point))
	{
		screen_point.x = view_point.x;
		screen_point.y = -view_point.y;
		waypoint_type = _waypoint_off_screen;
	}
	else
	{
		screen_point.x -= (real)(
			((render.camera.viewport_bounds.x1-render.camera.viewport_bounds.x0)/2) +
			render.camera.viewport_bounds.x0);
		screen_point.y -= (real)(
			((render.camera.viewport_bounds.y1-render.camera.viewport_bounds.y0)/2) +
			render.camera.viewport_bounds.y0);
	}

	horizontal_radius =
		((real)(render.camera.window_bounds.x1-render.camera.window_bounds.x0) -
		(hud_globals->waypoint.right_offset+hud_globals->waypoint.left_offset))*0.5f;
	vertical_radius =
		((real)(render.camera.window_bounds.y1-render.camera.window_bounds.y0) -
		(hud_globals->waypoint.bottom_offset+hud_globals->waypoint.top_offset))*0.5f;
	radius_product = vertical_radius*horizontal_radius;
	vertical_component = vertical_radius*screen_point.x;
	horizontal_component = horizontal_radius*screen_point.y;
	theta = 0.0f;

	if (waypoint_type==_waypoint_off_screen ||
		radius_product*radius_product <=
		vertical_component*vertical_component + horizontal_component*horizontal_component)
	{
		real scale = square_root(
			(radius_product*radius_product) /
			(vertical_component*vertical_component + horizontal_component*horizontal_component));

		waypoint_type = _waypoint_off_screen;
		screen_point.x *= scale;
		screen_point.y *= scale;

		if (!TEST_FLAG(arrow->flags, _hud_waypoint_dont_rotate_offscreen_bit))
		{
			theta = -arctangent(screen_point.x, screen_point.y);
		}
	}

	screen_point.x += (real)(
		(render.camera.viewport_bounds.x1-render.camera.viewport_bounds.x0)/2);
	screen_point.y += (real)(
		(render.camera.viewport_bounds.y1-render.camera.viewport_bounds.y0)/2);

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_nav_points.c",
		615,
		waypoint_type!=NONE);

	{
		long bitmap_group_index = hud_globals->waypoint.arrow_bitmap.index;
		struct bitmap_data const *bitmap = NULL;
		real_rectangle2d const *clip = NULL;

		hud_retrieve_bitmap_and_bounding_rect(
			bitmap_group_index,
			arrow->sequence_indices[waypoint_type],
			0,
			&bitmap,
			&clip);

		if (bitmap && _texture_cache_bitmap_get_hardware_format(
			(struct bitmap_data *)bitmap, FALSE, TRUE))
		{
			point2d point;
			real_rgb_color color;
			byte alpha;
			real fade;

			point.x = (short)(long)screen_point.x;
			point.y = (short)(long)screen_point.y;
			alpha = (byte)PIN(fast_ftol_C(arrow->opacity)*255, 0, 255);
			pixel32_to_real_rgb_color(arrow->color, &color);
			fade = 1.0f-arrow->fade;
			color.red *= PIN(fade, 0.0f, 1.0f);
			color.green *= PIN(fade, 0.0f, 1.0f);
			color.blue *= PIN(fade, 0.0f, 1.0f);

			hud_draw_bitmap_direct(
				bitmap,
				_hud_anchor_center,
				&point,
				clip,
				arrow_scale,
				theta,
				((pixel32)alpha<<24) | real_rgb_color_to_pixel32(&color),
				FALSE);

			if (waypoint_type!=_waypoint_off_screen)
			{
				struct number_hud_element_definition numbers;
				struct hud_absolute_placement_definition placement;
				real decimal_modulus;
				real bitmap_extent;

				distance *= 3.0480001f;
				csmemset(&placement, 0, sizeof(placement));
				csmemset(&numbers, 0, sizeof(numbers));

				placement.corner = _hud_anchor_top_left;
				numbers.colors.color =
					((pixel32)alpha<<24) | real_rgb_color_to_pixel32(&color);
				numbers.colors.flash_color =
					((pixel32)alpha<<24) | real_rgb_color_to_pixel32(&color);
				numbers.digits = 3;
				numbers.fractional_digits = 1;
				numbers.number_flags =
					FLAG(_hud_number_show_all_leading_zeros_bit) |
					FLAG(_hud_number_show_trailing_m_bit);

				bitmap_extent =
					((clip->x1-clip->x0)*(real)bitmap->width)*0.5f;
				numbers.placement.offset.x = (short)(long)(
					bitmap_extent*arrow_scale*0.33000001f + (real)point.x);
				bitmap_extent =
					((clip->y1-clip->y0)*(real)bitmap->height)*0.5f;
				numbers.placement.offset.y = (short)(long)(
					bitmap_extent*arrow_scale*0.66000003f + (real)point.y);
				numbers.placement.offset.x -= render.camera.window_bounds.x0;
				numbers.placement.offset.x += render.camera.viewport_bounds.x0;
				numbers.placement.offset.y -= render.camera.window_bounds.y0;
				numbers.placement.offset.y += render.camera.viewport_bounds.y0;

				decimal_modulus = (real)pow(10.0, 4.0);
				{
					short decimal_value = (short)fast_ftol(
						(real)fmod(
							fabs(decimal_modulus*distance),
							decimal_modulus));

					hud_draw_numbers(
						local_player_index,
						&placement,
						&numbers,
						(short)fast_ftol_C(distance),
						decimal_value,
						0,
						0,
						0.0f);
				}
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 675);

	return;
}

void hud_render_nav_points(
	short local_player_index)
{
	if (local_player_index!=NONE &&
		(local_player_get_player_index(local_player_index)==NONE ?
			NONE :
			player_get(local_player_get_player_index(local_player_index))->unit_index)!=NONE &&
		hud_globals->waypoint.arrow_bitmap.index!=NONE)
	{
		struct hud_nav_point_player_datum *datum = get_nav_point_datum(local_player_index);
		short nav_point_index;

		for (nav_point_index = 0; nav_point_index<MAXIMUM_NUMBER_OF_NAV_POINTS; nav_point_index++)
		{
			struct hud_nav_point_datum *nav_point = &datum->nav_points[nav_point_index];
			real_point3d position;

			if (nav_point->nav_index==NONE ||
				nav_point->reference_index==NONE ||
				nav_point->type==NONE)
			{
				nav_point->type = NONE;
				continue;
			}

			switch (nav_point->type)
			{
			case _hud_nav_point_type_flag:
				position = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->cutscene_flags,
					nav_point->reference_index,
					struct scenario_cutscene_flag)->position;
				break;

			case _hud_nav_point_type_object:
				if (!hud_nav_object_try_and_get(nav_point->reference_index))
					continue;

				{
					real radius;
					object_get_bounding_sphere(
						nav_point->reference_index,
						&position,
						&radius);
				}
				break;

			case _hud_nav_point_type_game_engine_flag:
				{
					real_point3d goal_position;
					position = *game_engine_get_goal_position(
						&goal_position,
						(short)nav_point->reference_index);
				}
				break;

			/* position is left unassigned only by this default arm. Not reached unassigned: the
			 * arm's assertion failure calls system_exit, which does not return in January
			 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
			 * Source-policy approval pending (2026-09-27 audit). */
			default:
				match_assert("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 725, !"unreachable");
				break;
			}

			position.z += nav_point->z_offset;
			custom_render_nav_point(
				local_player_index,
				&position,
				nav_point->nav_index,
				nav_point->screen_type);
		}
	}

	game_engine_render_nav_points(local_player_index);

	return;
}

#ifdef HALO_N64
static unsigned short n64_nav_point_updates[4];
#endif

static void hud_update_nav_point_local_player(
	short local_player_index)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct hud_nav_point_player_datum *datum;
	long unit_index;
	short nav_point_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	datum = get_nav_point_datum(local_player_index);
#ifdef HALO_N64
	n64_nav_point_updates[local_player_index & 3]++;
#endif
	unit_index = local_player_get_player_index(local_player_index)==NONE ?
		NONE :
		player_get(local_player_get_player_index(local_player_index))->unit_index;

	for (nav_point_index = 0; nav_point_index<MAXIMUM_NUMBER_OF_NAV_POINTS; nav_point_index++)
	{
		struct hud_nav_point_datum *nav_point = &datum->nav_points[nav_point_index];

		if (nav_point->nav_index==NONE ||
			nav_point->reference_index==NONE ||
			nav_point->type==NONE)
		{
			nav_point->type = NONE;
			continue;
		}

		if (unit_index!=NONE)
		{
			long reference_object_index = NONE;
			real_point3d head_position;
			real_point3d position;

			unit_get_head_position(unit_index, &head_position);

			switch (nav_point->type)
			{
			case _hud_nav_point_type_flag:
				position = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->cutscene_flags,
					nav_point->reference_index,
					struct scenario_cutscene_flag)->position;
				break;

			case _hud_nav_point_type_object:
				{
					struct hud_nav_object_datum *object =
						hud_nav_object_try_and_get(nav_point->reference_index);

					reference_object_index = nav_point->reference_index;
					if (!object || TEST_FLAG(object->damage_flags, _object_dead_bit))
					{
						nav_point->type = NONE;
						nav_point->reference_index = NONE;
						nav_point->nav_index = NONE;
						continue;
					}

					{
						real radius;
						object_get_bounding_sphere(
							reference_object_index,
							&position,
							&radius);
					}
				}
				break;

			case _hud_nav_point_type_game_engine_flag:
				{
					real_point3d goal_position;
					position = *game_engine_get_goal_position(
						&goal_position,
						(short)nav_point->reference_index);
				}
				break;

			default:
				break;
			}

			/* The default arm leaves position unassigned. Not reached: every store to
			 * nav_point->type in this file writes one of the three types handled above or NONE
			 * (the map-start memset and the deactivations), the nav point array is private to
			 * this file, and NONE entries are skipped above. Source-policy approval pending
			 * (2026-09-27 audit). */
			position.z += nav_point->z_offset;
#ifdef HALO_N64
			/* (a ray across the map for each marker every update was a fifth
			of the HUD's time: each one tests again every fourth update,
			staggered, and keeps its last answer in between) */
			if (((n64_nav_point_updates[local_player_index & 3] + nav_point_index) & 3) == 0)
#endif
			nav_point->screen_type = hud_get_nav_point_render_type(
				local_player_index,
				&head_position,
				&position,
				reference_object_index);
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_nav_points.c", 496);

	return;
}

void hud_update_nav_points(
	void)
{
	short local_player_index = local_player_get_next(NONE);

	while (local_player_index!=NONE)
	{
		hud_update_nav_point_local_player(local_player_index);
		local_player_index = local_player_get_next(local_player_index);
	}

	return;
}
