/*
HUD_DRAW.C

symbols in this file:
000C08E0 0010:
	_get_return_eip (0000)
000C08F0 0030:
	_check_stack_buffer (0000)
000C0920 0110:
	_code_000c0920 (0000)
000C0A30 0010:
	_hud_globals_get_scale (0000)
000C0A40 01f0:
	_hud_retrieve_bitmap_and_bounding_rect (0000)
000C0C30 01e0:
	_code_000c0c30 (0000)
000C0E10 01e0:
	_code_000c0e10 (0000)
000C0FF0 0038:
	_fast_ftol_C (0000)
000C1030 0140:
	_real_argb_color_to_pixel32 (0000)
000C1170 00c0:
	_real_rgb_color_to_pixel32 (0000)
000C1230 00b0:
	_real_alpha_intensity_to_pixel32 (0000)
000C12E0 03c0:
	_hud_calculate_point (0000)
000C16A0 0020:
	_get_flash_duration (0000)
000C16C0 0260:
	_get_flash_color (0000)
000C1920 0220:
	_code_000c1920 (0000)
000C1B40 08e0:
	_code_000c1b40 (0000)
000C2420 0180:
	_code_000c2420 (0000)
000C25A0 0140:
	_hud_draw_bitmap_direct (0000)
000C26E0 0520:
	_hud_draw_meter (0000)
000C2C00 0740:
	_hud_draw_numbers (0000)
000C3340 0040:
	_hud_draw_bitmap (0000)
000C3380 0280:
	_hud_draw_static_element (0000)
000C3600 0210:
	_hud_draw_weapon_overlays (0000)
0026FD68 0015:
	??_C@_0BF@LPAPHBIL@corrupt?5stack?5at?5?$CFd?$CB?$AA@ (0000)
0026FD80 0018:
	??_C@_0BI@PIKMLAKB@corrupt?5return?5address?$CB?$AA@ (0000)
0026FD98 0024:
	??_C@_0CE@JPFFJICF@c?3?2halo?2SOURCE?2interface?2hud_dra@ (0000)
0026FDBC 0011:
	??_C@_0BB@HJJFJKIM@frame_index?5?$DO?$DN?50?$AA@ (0000)
0026FDD0 0005:
	??_C@_04POIGMEJN@clip?$AA@ (0000)
0026FDD8 0036:
	??_C@_0DG@HKMKFIHC@player?9?$DOlocal_player_index?$DN?$DNrend@ (0000)
0026FE10 0011:
	??_C@_0BB@NLHGFKHK@verify?5?$DN?$DN?5result?$AA@ (0000)
0026FE24 0023:
	??_C@_0CD@KHKIMBNF@intensity?$DO?$DN0?40f?5?$CG?$CG?5intensity?$DM?$DN1?4@ (0000)
0026FE48 000a:
	??_C@_09MKANOGAA@placement?$AA@ (0000)
0026FE54 0013:
	??_C@_0BD@GFIGJECD@absolute_placement?$AA@ (0000)
0026FE68 002e:
	??_C@_0CO@FKHKBGNC@render?4local_player_index?$DN?$DNlocal@ (0000)
0026FE98 0004:
	__real@40c90e56 (0000)
0026FE9C 0025:
	??_C@_0CF@FIKNBIIH@dest_value?$DO?$DN0?40f?5?$CG?$CG?5dest_value?$DM?$DN@ (0000)
0026FEC4 001d:
	??_C@_0BN@IFLKFMOJ@source_bitmap?$DN?$DNnumber_bitmap?$AA@ (0000)
00453AB4 0004:
	_bss_00453ab4 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps_inlines.h"
#include "bitmaps/bitmap_utilities.h"
#include "cache/texture_cache.h"
#include "effects/particles.h"
#include "game/game.h"
#include "game/player_control.h"
#include "game/players.h"
#include "interface/hud_definitions.h"
#include "interface/hud_draw.h"
#include "interface/interface.h"
#include "interface/unit_hud_interface_definition.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "objects/objects.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	_hud_dont_scale_offset_bit = 0,
};

/* hud number and meter definitions (no shared header declares these yet;
   number_hud_element_definition, hud_number_definition and
   rasterizer_meter_parameters have identical TU-local copies in hud_messaging.c,
   hud_nav_points.c, hud_weapon.c and rasterizer_xbox_dynavobgeom.c) */
enum
{
	hud_number_group_tag = 'hud#',
};

enum hud_number_show_flags
{
	_hud_number_show_all_leading_zeros_bit = 0,
	_hud_number_show_only_when_zoomed_bit,
	_hud_number_show_trailing_m_bit,

	NUMBER_OF_HUD_NUMBER_SHOW_FLAGS
};

enum hud_number
{
	_hud_number_decimal_index = 10,
	_hud_number_colon_index,
	_hud_number_negative_sign_index,
	_hud_number_meters_index,
	_hud_number_kilometers_index
};

enum hud_meter_flags
{
	_hud_meter_switch_color_on_state_change_bit = 0,
	_hud_meter_interpolates_between_min_max_bit,
	_hud_meter_interpolate_in_hsv_space_bit,
	_hud_meter_interpolate_along_farthest_hue_path_bit,
	_hud_meter_invert_interpolation_value_bit,

	NUMBER_OF_HUD_METER_FLAGS
};

enum hud_flash_flags
{
	_hud_flash_reverse_colors_bit = 0,
};

enum hud_weapon_overlay_flags
{
	_hud_overlay_flashes_bit = 0,
	_hud_overlay_runtime_invalid_bit,
};

enum hud_multitexture_overlay_effector_type
{
	_hud_multitexture_overlay_effector_type_tint = 0,
	_hud_multitexture_overlay_effector_type_horizontal_offset,
	_hud_multitexture_overlay_effector_type_vertical_offset,
	_hud_multitexture_overlay_effector_type_alpha,

	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPES
};

enum hud_multitexture_overlay_effector_destination
{
	_hud_multitexture_overlay_effector_destination_geometry_offset = 0,
	_hud_multitexture_overlay_effector_destination_primary_map,
	_hud_multitexture_overlay_effector_destination_secondary_map,
	_hud_multitexture_overlay_effector_destination_tertiary_map,

	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATIONS
};

enum hud_multitexture_overlay_effector_source
{
	_hud_multitexture_overlay_effector_source_player_pitch = 0,
	_hud_multitexture_overlay_effector_source_player_pitch_tangent,
	_hud_multitexture_overlay_effector_source_player_yaw,
	_hud_multitexture_overlay_effector_source_weapon_ammo_loaded,
	_hud_multitexture_overlay_effector_source_weapon_ammo_total,
	_hud_multitexture_overlay_effector_source_weapon_heat,
	_hud_multitexture_overlay_effector_source_explicit,
	_hud_multitexture_overlay_effector_source_zoom_level,

	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCES
};

enum hud_multitexture_overlay_blend_function
{
	_hud_multitexture_overlay_blend_function_add = 0,
	_hud_multitexture_overlay_blend_function_subtract,
	_hud_multitexture_overlay_blend_function_multiply,
	_hud_multitexture_overlay_blend_function_multiply2x,
	_hud_multitexture_overlay_blend_function_dot,

	NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_BLEND_FUNCTIONS
};

enum bitmap_group_type
{
	_bitmap_group_type_interface_bitmaps = 4,
};

enum
{
	_shader_framebuffer_blend_function_alpha_blend = 0,
	_shader_framebuffer_blend_function_multiply,
	_shader_framebuffer_blend_function_double_multiply,
	_shader_framebuffer_blend_function_add,
	_shader_framebuffer_blend_function_subtract,
	_shader_framebuffer_blend_function_component_min,
	_shader_framebuffer_blend_function_component_max,
	_shader_framebuffer_blend_function_alpha_multiply_add,
};

/* ---------- macros */

#define hud_number_definition_get(index) \
	((struct hud_number_definition *)tag_get(hud_number_group_tag, (index)))

#define _hud_anchor_right_bit 0
#define _hud_anchor_bottom_bit 1

/* ---------- structures */

struct number_hud_element_definition
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	char digits;
	byte number_flags;
	char fractional_digits;
	byte pad;
	long unused[3];
};

struct hud_number_definition
{
	struct tag_reference number_bitmap;
	char character_width;
	char screen_width;
	char x_offset;
	char y_offset;
	char decimal_point_width;
	char colon_width;
	short pad;
	long unused[19];
};

struct rasterizer_meter_parameters
{
	pixel32 gradient_min_color;
	pixel32 gradient_max_color;
	pixel32 background_color;
	pixel32 flash_color;
	boolean flash_color_is_negative;
	boolean tint_mode_2;
	byte pad12[2];
	pixel32 tint_color;
	real gradient;
};

struct weapon_hud_overlay_item
{
	struct hud_placement_definition placement;
	struct hud_color_definition colors;
	short frame_rate;
	short pad;
	short sequence_index;
	short type;
	long flags;
	long unused[14];
};

struct weapon_hud_overlay_definition
{
	struct tag_reference bitmap;
	struct tag_block items;
};

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
	byte pad09;
	short magazine_count;
	struct weapon_interface_magazine_state magazines[2];
};

struct multitexture_overlay_hud_element_effector_definition
{
	long unused0[16];
	short destination_type;
	short destination;
	short source;
	word pad46;
	real in_bounds[2];
	real out_bounds[2];
	long unused58[16];
	real_rgb_color tint_color_lower_bounds;
	real_rgb_color tint_color_upper_bounds;
	short periodic_function;
	word padB2;
	real periodic_function_period;
	real periodic_function_phase;
	long unusedBC[8];
};

struct multitexture_overlay_hud_element_definition
{
	word flags;
	short type;
	short framebuffer_blend_function;
	word pad06;
	long unused08[8];
	word map_flags[3];
	short map_blending_function[2];
	short pad32;
	real_vector2d map_scale[3];
	real_vector2d map_offset[3];
	struct tag_reference map[3];
	short map_clamp[3];
	short pad9A;
	long unused9C[46];
	struct tag_block functions;
	long unused160[32];
};

typedef char number_hud_element_definition_size_assert[
	sizeof(struct number_hud_element_definition) == 0x54 ? 1 : -1];
typedef char hud_number_definition_size_assert[
	sizeof(struct hud_number_definition) == 0x64 ? 1 : -1];
typedef char rasterizer_meter_parameters_size_assert[
	sizeof(struct rasterizer_meter_parameters) == 0x1C ? 1 : -1];
typedef char weapon_interface_state_size_assert[
	sizeof(struct weapon_interface_state) == 0x20 ? 1 : -1];
typedef char multitexture_overlay_hud_element_effector_definition_size_assert[
	sizeof(struct multitexture_overlay_hud_element_effector_definition) == 0xDC ? 1 : -1];
typedef char multitexture_overlay_hud_element_definition_size_assert[
	sizeof(struct multitexture_overlay_hud_element_definition) == 0x1E0 ? 1 : -1];

/* ---------- prototypes */

static real_rectangle2d const *get_sprite_clip_rect(
	long bitmap_group_index,
	short sequence_index,
	short frame_index);
static void hud_calculate_bitmap_bounds(
	struct bitmap_data const *bitmap,
	short placement_type,
	real_rectangle2d const *clip,
	real_rectangle2d *bounds,
	boolean is_interface_bitmap);
static void hud_draw_bitmap_internal(
	void *meter_parameters,
	struct bitmap_data const *bitmap,
	point2d const *point,
	real_rectangle2d const *clip,
	real_rectangle2d const *bounds,
	real_vector2d const *xy_scale,
	real theta,
	pixel32 color);
static void hud_draw_bitmap_with_meter(
	void *meter_parameters,
	struct bitmap_data const *bitmap,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct hud_placement_definition const *placement,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	pixel32 color,
	boolean in_multiplayer,
	boolean is_interface_bitmap,
	boolean is_crosshair_bitmap);
static boolean hud_draw_multitexture_overlay_get_current_weapon_definition(
	struct player_datum const *player,
	struct weapon_interface_state *weapon_state);
static void hud_draw_multitexture_overlay(
	struct multitexture_overlay_hud_element_definition const *overlay,
	short local_player_index,
	point2d const *point,
	real_rectangle2d const *clip,
	real_rectangle2d const *bounds,
	real_vector2d const *xy_scale,
	real theta,
	pixel32 color);

/* ---------- globals */

/* ---------- public code */

/* Inspect the guarded caller's frame, not the return site of this helper.
 * A normal prologue would replace EBP and defeat the paired stack check. */
#ifdef HALO_LINUX
__attribute__((noinline)) long get_return_eip(
	void)
{
	/* the caller's return address, as [ebp+4] is in the naked original */
	return (long)__builtin_return_address(1);
}
#else
__declspec(naked) long get_return_eip(
	void)
{
	__asm
	{
		mov eax, [ebp+4]
		ret
	}
}
#endif

real hud_globals_get_scale(
	boolean in_multiplayer)
{
	return 1.0f;
}

void hud_retrieve_bitmap_and_bounding_rect(
	long bitmap_group_index,
	short sequence_index,
	short frame_index,
	struct bitmap_data const **bitmap,
	real_rectangle2d const **clip)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	match_assert("c:\\halo\\SOURCE\\interface\\hud_draw.c", 193, bitmap);
	match_assert("c:\\halo\\SOURCE\\interface\\hud_draw.c", 194, clip);

	if (bitmap_group_index!=NONE)
	{
		struct bitmap_group *group = bitmap_group_get(bitmap_group_index);

		if (sequence_index<group->sequences.count)
		{
			struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
				&group->sequences, sequence_index, struct bitmap_group_sequence);
			long sprite_count;

			frame_index &= 0x7FFF;
			match_assert("c:\\halo\\SOURCE\\interface\\hud_draw.c", 205, frame_index >= 0);

			sprite_count = sequence->sprites.count;
			if (sprite_count)
			{
				struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
					&sequence->sprites, frame_index%sprite_count, struct bitmap_group_sprite);

				*bitmap = TAG_BLOCK_GET_ELEMENT(
					&group->bitmaps, sprite->bitmap_index, struct bitmap_data);
			}
			else
			{
				*bitmap = bitmap_group_get_bitmap_from_sequence(
					bitmap_group_index, sequence_index, frame_index);
			}
		}
	}

	if (*bitmap)
		*clip = get_sprite_clip_rect(bitmap_group_index, sequence_index, frame_index);
	else
		*clip = NULL;

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 228);

	return;
}

static boolean hud_draw_multitexture_overlay_get_current_weapon_definition(
	struct player_datum const *player,
	struct weapon_interface_state *weapon_state)
{
	boolean result = FALSE;
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	long weapon_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	weapon_index = unit_inventory_get_weapon(
		player->unit_index,
		unit_get(player->unit_index)->unit.current_weapon_index);
	if (weapon_index == NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);

		if (unit->object.parent_object_index != NONE &&
			unit->unit.parent_seat_index != NONE)
		{
			struct unit_datum *parent_unit = unit_get(unit->object.parent_object_index);
			struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
				&unit_definition_get(parent_unit->definition_index)->unit.seats,
				unit->unit.parent_seat_index,
				struct unit_seat);

			if (TEST_FLAG(seat->flags, _unit_seat_gunner_bit))
			{
				weapon_index = unit_inventory_get_weapon(
					unit->object.parent_object_index,
					unit_get(
						unit->object.parent_object_index)->unit.current_weapon_index);
			}
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_draw.c",
		1021,
		player->local_player_index==render.local_player_index);

	if (weapon_index != NONE)
	{
		struct weapon_definition *weapon_definition = weapon_definition_get(
			weapon_get(weapon_index)->definition_index);

		weapon_build_weapon_interface_state(weapon_index, weapon_state);
		result = TRUE;
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 1032);

	return result;
}

static void hud_draw_multitexture_overlay(
	struct multitexture_overlay_hud_element_definition const *overlay,
	short local_player_index,
	point2d const *point,
	real_rectangle2d const *clip,
	real_rectangle2d const *bounds,
	real_vector2d const *xy_scale,
	real theta,
	pixel32 color)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct player_datum *player;
	struct weapon_interface_state weapon_state;
	struct dynamic_screen_vertex vertices[4];
	struct rasterizer_dynamic_screen_geometry_parameters parameters;
	real_point2d texture_offset[3];
	real_rgb_color texture_tint[3];
	real texture_fade[3];
	real_vector2d geometry_offset;
	real sin_theta;
	real cos_theta;
	short vertex_index;
	short map_index;
	short function_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	sin_theta = (real)sin(theta);
	texture_offset[0].x = overlay->map_offset[0].i;
	texture_offset[0].y = overlay->map_offset[0].j;
	texture_offset[1].x = overlay->map_offset[1].i;
	texture_offset[1].y = overlay->map_offset[1].j;
	texture_offset[2].x = overlay->map_offset[2].i;
	texture_offset[2].y = overlay->map_offset[2].j;
	texture_tint[0].red = 0.0f;
	texture_tint[0].green = 0.0f;
	texture_tint[0].blue = 0.0f;
	texture_tint[1].red = 0.0f;
	texture_tint[1].green = 0.0f;
	texture_tint[1].blue = 0.0f;
	texture_tint[2].red = 0.0f;
	texture_tint[2].green = 0.0f;
	texture_tint[2].blue = 0.0f;
	texture_fade[0] = 1.0f;
	texture_fade[1] = 1.0f;
	texture_fade[2] = 1.0f;
	geometry_offset.i = 0.0f;
	geometry_offset.j = 0.0f;
	cos_theta = (real)cos(theta);

	player = player_get(local_player_get_player_index(local_player_index));
	hud_draw_multitexture_overlay_get_current_weapon_definition(
		player,
		&weapon_state);

	for (vertex_index = 0; vertex_index < 4; vertex_index++)
	{
		real texture_x = ((vertex_index + 1) & 2) ? clip->x1 : clip->x0;
		real texture_y = vertex_index > 1 ? clip->y1 : clip->y0;
		real bound_x = ((vertex_index + 1) & 2) ? bounds->x1 : bounds->x0;
		real bound_y = vertex_index > 1 ? bounds->y1 : bounds->y0;

		vertices[vertex_index].position.x = (real)(point->x + fast_ftol(
			(bound_x*cos_theta-bound_y*sin_theta)*xy_scale->i));
		vertices[vertex_index].position.y = (real)(point->y + fast_ftol(
			(bound_y*cos_theta+bound_x*sin_theta)*xy_scale->j));
		vertices[vertex_index].texture_coordinates.x = texture_x;
		vertices[vertex_index].texture_coordinates.y = texture_y;
		vertices[vertex_index].color = color;
	}

	csmemset(&parameters, 0, sizeof(parameters));
	parameters.map_texture_scale[0].j = 1.0f;
	parameters.map_texture_scale[0].i = 1.0f;
	parameters.map_scale[0].j = 1.0f;
	parameters.map_scale[0].i = 1.0f;
	parameters.meter_parameters = NULL;
	parameters.point_sampled = local_player_count() == 1;
	parameters.map[0] = bitmap_group_get_bitmap_from_sequence(
		overlay->map[0].index,
		0,
		0);
	parameters.map[1] = bitmap_group_get_bitmap_from_sequence(
		overlay->map[1].index,
		0,
		0);
	parameters.map[2] = bitmap_group_get_bitmap_from_sequence(
		overlay->map[2].index,
		0,
		0);

	for (map_index = 0; map_index < 3; map_index++)
	{
		if (parameters.map[map_index])
		{
			real scale_x = overlay->map_scale[map_index].i == 0.0f ?
				1.0f : 1.0f/overlay->map_scale[map_index].i;
			real scale_y = overlay->map_scale[map_index].j == 0.0f ?
				1.0f : 1.0f/overlay->map_scale[map_index].j;
			boolean non_power_of_two =
				((parameters.map[map_index]->width-1)&parameters.map[map_index]->width) != 0 ||
				((parameters.map[map_index]->height-1)&parameters.map[map_index]->height) != 0;

			if (non_power_of_two)
			{
				parameters.map_texture_scale[map_index].i =
					1.0f/(real)parameters.map[map_index]->width;
				parameters.map_texture_scale[map_index].j =
					1.0f/(real)parameters.map[map_index]->height;
			}
			else
			{
				parameters.map_texture_scale[map_index].i = 1.0f;
				parameters.map_texture_scale[map_index].j = 1.0f;
			}

			parameters.map_offset[map_index] = &texture_offset[map_index];
			parameters.map_scale[map_index].i = scale_x;
			parameters.map_scale[map_index].j = scale_y;
			parameters.map_wrapped[map_index] = (boolean)overlay->map_clamp[map_index];
		}

		if (map_index < 2)
		{
			short *out_modes[2] =
			{
				&parameters.map0_to_1_blend_function,
				&parameters.map1_to_2_blend_function
			};

			switch (overlay->map_blending_function[map_index])
			{
			case _hud_multitexture_overlay_blend_function_add:
				*out_modes[map_index] =
					_shader_framebuffer_blend_function_alpha_blend;
				break;

			case _hud_multitexture_overlay_blend_function_subtract:
				*out_modes[map_index] =
					_shader_framebuffer_blend_function_double_multiply;
				break;

			case _hud_multitexture_overlay_blend_function_multiply:
				*out_modes[map_index] =
					_shader_framebuffer_blend_function_multiply;
				break;

			case _hud_multitexture_overlay_blend_function_multiply2x:
				*out_modes[map_index] =
					_shader_framebuffer_blend_function_add;
				break;

			case _hud_multitexture_overlay_blend_function_dot:
				*out_modes[map_index] =
					_shader_framebuffer_blend_function_subtract;
				break;
			}
		}
		else
		{
			parameters.framebuffer_blend_function =
				overlay->framebuffer_blend_function;
		}
	}

	for (function_index = 0;
		function_index < overlay->functions.count;
		function_index++)
	{
		static real theta;
		struct multitexture_overlay_hud_element_effector_definition *effector;
		real source_value;
		real dest_value;
		real_rgb_color dest_color;

		theta += 0.05f;
		effector = TAG_BLOCK_GET_ELEMENT(
			&overlay->functions,
			function_index,
			struct multitexture_overlay_hud_element_effector_definition);

		switch (effector->source)
		{
		case _hud_multitexture_overlay_effector_source_player_pitch:
			{
				long unit_index = local_player_get_player_index(local_player_index) == NONE ?
					NONE :
					player_get(local_player_get_player_index(local_player_index))->unit_index;
				real_vector3d direction;
				real_euler_angles2d angles;

				unit_get_aiming_vector(unit_index, &direction);
				euler_angles2d_from_vector3d(&angles, &direction);
				source_value = angles.pitch;
			}
			break;

		case _hud_multitexture_overlay_effector_source_player_pitch_tangent:
		case _hud_multitexture_overlay_effector_source_player_yaw:
			source_value = 0.0f;
			break;

		case _hud_multitexture_overlay_effector_source_weapon_ammo_loaded:
			source_value = (real)weapon_state.magazines[0].rounds_loaded;
			break;

		case _hud_multitexture_overlay_effector_source_weapon_ammo_total:
			source_value = (real)weapon_state.magazines[0].rounds_remaining;
			break;

		case _hud_multitexture_overlay_effector_source_weapon_heat:
			source_value = weapon_state.heat;
			break;

		case _hud_multitexture_overlay_effector_source_explicit:
			source_value = effector->in_bounds[0];
			break;

		case _hud_multitexture_overlay_effector_source_zoom_level:
			source_value = (real)player_control_get_zoom_level(local_player_index);
			break;
		}

		/* The switch above has no default: a source value outside the eight enumerators
		 * leaves source_value unassigned for the interpolation below. Not shown reachable:
		 * the value comes from hud tag data, which was not scanned. Source-policy approval
		 * pending (2026-09-27 audit). */
		if (effector->in_bounds[1] == effector->in_bounds[0] ||
			effector->out_bounds[1] == effector->out_bounds[0])
		{
			dest_value = effector->out_bounds[0];
			dest_color = effector->tint_color_lower_bounds;
		}
		else
		{
			real fraction = PIN(
				(source_value-effector->in_bounds[0])/
					(effector->in_bounds[1]-effector->in_bounds[0]),
				0.0f,
				1.0f);

			scalars_interpolate(
				effector->out_bounds[0],
				effector->out_bounds[1],
				fraction,
				&dest_value);
			rgb_colors_interpolate(
				&dest_color,
				0,
				&effector->tint_color_lower_bounds,
				&effector->tint_color_upper_bounds,
				fraction);
		}

		switch (effector->destination)
		{
		case _hud_multitexture_overlay_effector_destination_geometry_offset:
			geometry_offset.i =
				effector->destination_type ==
					_hud_multitexture_overlay_effector_type_horizontal_offset ?
				dest_value : 0.0f;
			geometry_offset.j =
				effector->destination_type ==
					_hud_multitexture_overlay_effector_type_vertical_offset ?
				dest_value : 0.0f;
			parameters.offset = &geometry_offset;
			break;

		case _hud_multitexture_overlay_effector_destination_primary_map:
			if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_tint)
			{
				texture_tint[0] = dest_color;
				parameters.map_tint[0] = &texture_tint[0];
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_horizontal_offset)
			{
				parameters.map_offset[0]->x += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_vertical_offset)
			{
				parameters.map_offset[0]->y += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_alpha)
			{
				texture_fade[0] = dest_value;
				parameters.map_fade[0] = &texture_fade[0];
				match_assert(
					"c:\\halo\\SOURCE\\interface\\hud_draw.c",
					1287,
					dest_value>=0.0f && dest_value<=1.0f);
			}
			break;

		case _hud_multitexture_overlay_effector_destination_secondary_map:
			if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_tint)
			{
				texture_tint[1] = dest_color;
				parameters.map_tint[1] = &texture_tint[1];
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_horizontal_offset)
			{
				parameters.map_offset[1]->x += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_vertical_offset)
			{
				parameters.map_offset[1]->y += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_alpha)
			{
				texture_fade[1] = dest_value;
				parameters.map_fade[1] = &texture_fade[1];
				match_assert(
					"c:\\halo\\SOURCE\\interface\\hud_draw.c",
					1310,
					dest_value>=0.0f && dest_value<=1.0f);
			}
			break;

		case _hud_multitexture_overlay_effector_destination_tertiary_map:
			if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_tint)
			{
				texture_tint[2] = dest_color;
				parameters.map_tint[2] = &texture_tint[2];
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_horizontal_offset)
			{
				parameters.map_offset[2]->x += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_vertical_offset)
			{
				parameters.map_offset[2]->y += dest_value;
			}
			else if (effector->destination_type ==
				_hud_multitexture_overlay_effector_type_alpha)
			{
				texture_fade[2] = dest_value;
				parameters.map_fade[2] = &texture_fade[2];
				match_assert(
					"c:\\halo\\SOURCE\\interface\\hud_draw.c",
					1333,
					dest_value>=0.0f && dest_value<=1.0f);
			}
			break;
		}
	}

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 1372);

	return;
}

pixel32 real_alpha_intensity_to_pixel32(
	real alpha,
	real intensity)
{
	real_argb_color color;

	match_assert(
		"..\\bitmaps\\bitmaps_inlines.h",
		0x13F,
		alpha>=0.0f && alpha<=1.0f);
	match_assert(
		"..\\bitmaps\\bitmaps_inlines.h",
		0x140,
		intensity>=0.0f && intensity<=1.0f);

	color.alpha = alpha;
	color.red = intensity;
	color.green = intensity;
	color.blue = intensity;

	return real_argb_color_to_pixel32(&color);
}

void hud_calculate_point(
	short local_player_index,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct hud_placement_definition const *placement,
	struct bitmap_data const *bitmap_data,
	boolean in_multiplayer,
	real override_scale,
	point2d *result)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real_point2d point;
	real scale;
	short corner;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	if (in_multiplayer && override_scale != 0.0f)
	{
		scale = override_scale;
	}
	else
	{
		scale = hud_globals_get_scale(in_multiplayer);
	}

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_draw.c",
		125,
		render.local_player_index==local_player_index);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_draw.c",
		126,
		absolute_placement);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_draw.c",
		127,
		placement);

	corner = absolute_placement->corner;
	if (corner < _hud_anchor_center)
	{
		point.x = placement->offset.x *
			(TEST_FLAG(corner, _hud_anchor_right_bit) ? -1 : 1) * scale +
			render.camera.window_bounds.v[
				((corner & FLAG(_hud_anchor_right_bit)) << 1) |
					FLAG(_hud_anchor_right_bit)] -
			render.camera.viewport_bounds.x0;
		point.y = placement->offset.y *
			(TEST_FLAG(corner, _hud_anchor_bottom_bit) ? -1 : 1) * scale +
			render.camera.window_bounds.v[corner & FLAG(_hud_anchor_bottom_bit)] -
			render.camera.viewport_bounds.y0;
	}
	else
	{
		point2d window_center;

		window_center.x = (short)(
			(render.camera.window_bounds.x1 + render.camera.window_bounds.x0) / 2);
		window_center.y = (short)(
			(render.camera.window_bounds.y1 + render.camera.window_bounds.y0) / 2);

		point.x = window_center.x - render.camera.viewport_bounds.x0 +
			placement->offset.x * scale;
		point.y = window_center.y - render.camera.viewport_bounds.y0 +
			placement->offset.y * scale;
	}

	if (bitmap_data)
	{
		switch (corner)
		{
		case _hud_anchor_top_left:
			point.x += bitmap_data->registration_point.x * scale;
			point.y += bitmap_data->registration_point.y * scale;
			break;

		case _hud_anchor_top_right:
			point.x += (bitmap_data->registration_point.x - bitmap_data->width) * scale;
			point.y += bitmap_data->registration_point.y * scale;
			break;

		case _hud_anchor_bottom_left:
			point.x += bitmap_data->registration_point.x * scale;
			point.y += (bitmap_data->registration_point.y - bitmap_data->height) * scale;
			break;

		case _hud_anchor_bottom_right:
			point.x += (bitmap_data->registration_point.x - bitmap_data->width) * scale;
			point.y += (bitmap_data->registration_point.y - bitmap_data->height) * scale;
			break;

		case _hud_anchor_center:
			point.x += (bitmap_data->registration_point.x + bitmap_data->width / 2) * scale;
			point.y += (bitmap_data->registration_point.y + bitmap_data->width / 2) * scale;
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\interface\\hud_draw.c",
				174,
				!"unreachable");
			break;
		}
	}

	result->x = (short)fast_ftol(point.x);
	result->y = (short)fast_ftol(point.y);

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 181);

	return;
}

long get_flash_duration(
	struct hud_color_definition const *hud_color)
{
	return fast_ftol(hud_color->flash_period * 30.0f);
}

pixel32 get_flash_color(
	struct hud_color_definition const *hud_color,
	long reference_value)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real_argb_color result;
	real_argb_color base_color;
	real_argb_color flash_color;
	real flash_phase;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	flash_phase = (real)fmod(
		(game_time_get() - reference_value) * (1.0f / TICKS_PER_SECOND),
		hud_color->flash_period);
	pixel32_to_real_argb_color(hud_color->color, &base_color);
	pixel32_to_real_argb_color(hud_color->flash_color, &flash_color);

	if (flash_phase < hud_color->number_of_flashes *
		(hud_color->flash_delay + hud_color->flash_length))
	{
		flash_phase = (real)fmod(
			flash_phase,
			hud_color->flash_delay + hud_color->flash_length);

		if (!reference_value)
		{
			if (TEST_FLAG(
				hud_color->flash_flags,
				_hud_flash_reverse_colors_bit))
			{
				result = base_color;
			}
			else
			{
				result = flash_color;
			}
		}
		else if (flash_phase < hud_color->flash_length)
		{
			/* the color is blended as a vector of its first three components
			 * followed by a scalar for the fourth (n[3]) */
			real fraction = square_root(
				PIN(
					(real)(1.0 -
						(cos(flash_phase / hud_color->flash_length * 6.283f) + 1.0) * 0.5),
					0.0f,
					1.0f));

			if (TEST_FLAG(
				hud_color->flash_flags,
				_hud_flash_reverse_colors_bit))
			{
				vectors_interpolate(
					(real_vector3d const *)&flash_color,
					(real_vector3d const *)&base_color,
					fraction,
					(real_vector3d *)&result);
				scalars_interpolate(
					flash_color.n[3],
					base_color.n[3],
					fraction,
					&result.n[3]);
			}
			else
			{
				vectors_interpolate(
					(real_vector3d const *)&base_color,
					(real_vector3d const *)&flash_color,
					fraction,
					(real_vector3d *)&result);
				scalars_interpolate(
					base_color.n[3],
					flash_color.n[3],
					fraction,
					&result.n[3]);
			}
		}
		else
		{
			if (TEST_FLAG(
				hud_color->flash_flags,
				_hud_flash_reverse_colors_bit))
			{
				result = flash_color;
			}
			else
			{
				result = base_color;
			}
		}
	}
	else
	{
		result = base_color;
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 311);

	return real_argb_color_to_pixel32(&result);
}

void hud_draw_weapon_overlays(
	short local_player_index,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct weapon_hud_overlay_definition const *overlays,
	long type_flags,
	long reference_time,
	short draw_flags,
	boolean in_multiplayer)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	long item_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	for (item_index = 0; item_index < overlays->items.count; item_index++)
	{
		struct weapon_hud_overlay_item const *item = TAG_BLOCK_GET_ELEMENT(
			&overlays->items,
			item_index,
			struct weapon_hud_overlay_item);

		if (!TEST_FLAG(item->flags, _hud_overlay_runtime_invalid_bit) &&
			(item->type & type_flags))
		{
			struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
				&bitmap_group_get(overlays->bitmap.index)->sequences,
				item->sequence_index,
				struct bitmap_group_sequence);
			pixel32 color;
			short frame_index;

			if (TEST_FLAG(item->flags, _hud_overlay_flashes_bit) &&
				TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
			{
				color = get_flash_color(&item->colors, reference_time);
			}
			else
			{
				color = item->colors.color;
			}

			if (TEST_FLAG(item->flags, _hud_overlay_flashes_bit) &&
				TEST_FLAG(draw_flags, _hud_draw_flashing_bit) &&
				item->frame_rate > 0)
			{
				frame_index = (short)(((game_time_get() - reference_time) /
					item->frame_rate / TICKS_PER_SECOND) % sequence->sprites.count);
			}
			else
			{
				frame_index = 0;
			}

			{
				struct bitmap_data const *bitmap = NULL;
				real_rectangle2d const *clip = NULL;

				hud_retrieve_bitmap_and_bounding_rect(
					overlays->bitmap.index,
					item->sequence_index,
					frame_index,
					&bitmap,
					&clip);

				if (bitmap && _texture_cache_bitmap_get_hardware_format(
					(struct bitmap_data *)bitmap,
					FALSE,
					TRUE))
				{
					hud_draw_bitmap_with_meter(
						NULL,
						bitmap,
						absolute_placement,
						&item->placement,
						clip,
						1.0f,
						0.0f,
						color,
						in_multiplayer,
						FALSE,
						FALSE);
				}
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 748);

	return;
}

void hud_draw_bitmap(
	struct bitmap_data const *bitmap,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct hud_placement_definition const *placement,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	pixel32 color,
	boolean in_multiplayer,
	boolean is_interface_bitmap,
	boolean is_crosshair_bitmap)
{
	hud_draw_bitmap_with_meter(
		NULL,
		bitmap,
		absolute_placement,
		placement,
		clip,
		scale,
		theta,
		color,
		in_multiplayer,
		is_interface_bitmap,
		is_crosshair_bitmap);

	return;
}

void hud_draw_static_element(
	short local_player_index,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct static_hud_element_definition const *static_element,
	short draw_flags,
	long flash_reference_time)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct bitmap_group *bitmap_group;
	struct bitmap_data *bitmap;
	short overlay_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	bitmap_group = bitmap_group_get(
		verify_tag_reference(&static_element->interface_bitmap));
	bitmap = bitmap_group_get_bitmap_from_sequence(
		static_element->interface_bitmap.index,
		static_element->sequence_index,
		0);
	if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
	{
		real_rectangle2d const *clip = get_sprite_clip_rect(
			verify_tag_reference(&static_element->interface_bitmap),
			static_element->sequence_index,
			0);
		pixel32 color;
		boolean is_interface_bitmap;

		if (TEST_FLAG(draw_flags, _hud_draw_disabled_bit))
			color = static_element->colors.disabled_color;
		else if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
			color = get_flash_color(
				&static_element->colors,
				flash_reference_time);
		else
			color = static_element->colors.color;

		is_interface_bitmap =
			bitmap_group->type == _bitmap_group_type_interface_bitmaps;

		hud_draw_bitmap_with_meter(
			NULL,
			bitmap,
			absolute_placement,
			&static_element->placement,
			clip,
			1.0f,
			0.0f,
			color,
			TEST_FLAG(draw_flags, _hud_draw_in_multiplayer_bit),
			is_interface_bitmap,
			FALSE);

		for (overlay_index = 0;
			overlay_index < static_element->multitexture_overlays.count;
			overlay_index++)
		{
			struct multitexture_overlay_hud_element_definition const *overlay =
				TAG_BLOCK_GET_ELEMENT(
					&static_element->multitexture_overlays,
					overlay_index,
					struct multitexture_overlay_hud_element_definition);
			real_rectangle2d default_clip;
			real_vector2d xy_scale;
			point2d point;
			real_rectangle2d bounds;

			default_clip.x0 = 0.0f;
			default_clip.x1 = 1.0f;
			default_clip.y0 = 0.0f;
			default_clip.y1 = 1.0f;

			if (is_interface_bitmap)
			{
				default_clip.x1 = (real)bitmap->width;
				default_clip.y1 = (real)bitmap->height;
			}

			if (!clip)
				clip = &default_clip;

			xy_scale = static_element->placement.scale;

			hud_calculate_point(
				render.local_player_index,
				absolute_placement,
				&static_element->placement,
				NULL,
				TEST_FLAG(draw_flags, _hud_draw_in_multiplayer_bit) &&
					!TEST_FLAG(
						static_element->placement.multiplayer_scaling_flags,
						_hud_dont_scale_offset_bit),
				0.0f,
				&point);
			hud_calculate_bitmap_bounds(
				bitmap,
				absolute_placement->corner,
				clip,
				&bounds,
				is_interface_bitmap);
			hud_draw_multitexture_overlay(
				overlay,
				local_player_index,
				&point,
				clip,
				&bounds,
				&xy_scale,
				0.0f,
				color);
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 685);

	return;
}

static void hud_draw_bitmap_internal(
	void *meter_parameters,
	struct bitmap_data const *bitmap,
	point2d const *point,
	real_rectangle2d const *clip,
	real_rectangle2d const *bounds,
	real_vector2d const *xy_scale,
	real theta,
	pixel32 color)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real sin_theta;
	real cos_theta;
	struct dynamic_screen_vertex vertices[4];
	struct rasterizer_dynamic_screen_geometry_parameters parameters;
	short vertex_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	sin_theta = (real)sin(theta);
	cos_theta = (real)cos(theta);

	for (vertex_index = 0; vertex_index < 4; vertex_index++)
	{
		real texture_x = ((vertex_index + 1) & 2) ? clip->x1 : clip->x0;
		real texture_y = vertex_index > 1 ? clip->y1 : clip->y0;
		real bound_x = ((vertex_index + 1) & 2) ? bounds->x1 : bounds->x0;
		real bound_y = vertex_index > 1 ? bounds->y1 : bounds->y0;

		vertices[vertex_index].position.x = (real)(point->x + fast_ftol(
			(bound_x*cos_theta-bound_y*sin_theta)*xy_scale->i));
		vertices[vertex_index].position.y = (real)(point->y + fast_ftol(
			(bound_y*cos_theta+bound_x*sin_theta)*xy_scale->j));
		vertices[vertex_index].texture_coordinates.x = texture_x;
		vertices[vertex_index].texture_coordinates.y = texture_y;
		vertices[vertex_index].color = color;
	}

	csmemset(&parameters, 0, sizeof(parameters));
	parameters.map_texture_scale[0].j = 1.0f;
	parameters.map_texture_scale[0].i = 1.0f;
	parameters.map_scale[0].j = 1.0f;
	parameters.map_scale[0].i = 1.0f;
	parameters.meter_parameters = meter_parameters;
	parameters.point_sampled = meter_parameters && local_player_count()==1;
	parameters.framebuffer_blend_function =
		_shader_framebuffer_blend_function_alpha_multiply_add;
	parameters.map[0] = (struct bitmap_data *)bitmap;

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 985);

	return;
}

static void hud_draw_bitmap_with_meter(
	void *meter_parameters,
	struct bitmap_data const *bitmap,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct hud_placement_definition const *placement,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	pixel32 color,
	boolean in_multiplayer,
	boolean is_interface_bitmap,
	boolean is_crosshair_bitmap)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real_rectangle2d default_clip;
	real_vector2d xy_scale;
	point2d point;
	real_rectangle2d bounds;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	default_clip.x0 = 0.0f;
	default_clip.x1 = 1.0f;
	default_clip.y0 = 0.0f;
	default_clip.y1 = 1.0f;

	if (is_interface_bitmap)
	{
		default_clip.x1 = (real)bitmap->width;
		default_clip.y1 = (real)bitmap->height;
	}

	if (!clip)
	{
		clip = &default_clip;
	}

	xy_scale.i = placement->scale.i*scale;
	xy_scale.j = placement->scale.j*scale;

	hud_calculate_point(
		render.local_player_index,
		absolute_placement,
		placement,
		NULL,
		in_multiplayer && !TEST_FLAG(
			placement->multiplayer_scaling_flags,
			_hud_dont_scale_offset_bit),
		0.0f,
		&point);
	hud_calculate_bitmap_bounds(
		bitmap,
		absolute_placement->corner,
		clip,
		&bounds,
		is_interface_bitmap);
	hud_draw_bitmap_internal(
		meter_parameters,
		bitmap,
		&point,
		clip,
		&bounds,
		&xy_scale,
		theta,
		color);

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 814);

	return;
}

/* ---------- private code */

static real_rectangle2d const *get_sprite_clip_rect(
	long bitmap_group_index,
	short sequence_index,
	short frame_index)
{
	real_rectangle2d const *result = NULL;
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	if (bitmap_group_index!=NONE && sequence_index!=NONE && frame_index!=NONE)
	{
		struct bitmap_group *group = bitmap_group_get(bitmap_group_index);

		if (sequence_index<group->sequences.count)
		{
			struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
				&group->sequences, sequence_index, struct bitmap_group_sequence);
			long sprite_count = sequence->sprites.count;

			if (sprite_count)
			{
				struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
					&sequence->sprites, frame_index%sprite_count, struct bitmap_group_sprite);

				result = &sprite->bounds;
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 100);

	return result;
}

static void hud_calculate_bitmap_bounds(
	struct bitmap_data const *bitmap,
	short placement_type,
	real_rectangle2d const *clip,
	real_rectangle2d *bounds,
	boolean is_interface_bitmap)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real width;
	real height;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	width = (clip->x1-clip->x0)*(is_interface_bitmap ? 1 : bitmap->width);
	height = (clip->y1-clip->y0)*(is_interface_bitmap ? 1 : bitmap->height);

	switch (placement_type)
	{
	case _hud_anchor_top_left:
		bounds->x0 = 0.0f;
		bounds->x1 = width;
		bounds->y0 = 0.0f;
		bounds->y1 = height;
		break;

	case _hud_anchor_top_right:
		bounds->x0 = -width;
		bounds->x1 = 0.0f;
		bounds->y0 = 0.0f;
		bounds->y1 = height;
		break;

	case _hud_anchor_bottom_left:
		bounds->x0 = 0.0f;
		bounds->x1 = width;
		bounds->y0 = -height;
		bounds->y1 = 0.0f;
		break;

	case _hud_anchor_bottom_right:
		bounds->x0 = -width;
		bounds->x1 = 0.0f;
		bounds->y0 = -height;
		bounds->y1 = 0.0f;
		break;

	case _hud_anchor_center:
		bounds->x0 = width*-0.5f;
		bounds->x1 = width*0.5f;
		bounds->y0 = height*-0.5f;
		bounds->y1 = height*0.5f;
		break;

	default:
		match_assert("c:\\halo\\SOURCE\\interface\\hud_draw.c", 904, !"unreachable");
		break;
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 907);

	return;
}

void hud_draw_bitmap_direct(
	struct bitmap_data const *bitmap,
	short placement,
	point2d const *point,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	pixel32 color,
	boolean is_interface_bitmap)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	real_rectangle2d default_clip;
	real_vector2d xy_scale;
	real_rectangle2d bounds;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	default_clip.x0 = 0.0f;
	default_clip.x1 = 1.0f;
	default_clip.y0 = 0.0f;
	default_clip.y1 = 1.0f;

	if (is_interface_bitmap)
	{
		default_clip.x1 = (real)bitmap->width;
		default_clip.y1 = (real)bitmap->height;
	}

	if (!clip)
	{
		clip = &default_clip;
	}

	xy_scale.i = scale;
	xy_scale.j = scale;

	hud_calculate_bitmap_bounds(
		bitmap,
		placement,
		clip,
		&bounds,
		is_interface_bitmap);
	hud_draw_bitmap_internal(
		NULL,
		bitmap,
		point,
		clip,
		&bounds,
		&xy_scale,
		theta,
		color);

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 856);

	return;
}

void hud_draw_meter(
	short local_player_index,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct meter_hud_element_definition const *meter,
	byte min_value,
	byte max_value,
	short draw_flags,
	real reference_time,
	real reference_value)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	struct bitmap_group *bitmap_group;
	struct bitmap_data *bitmap;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	bitmap_group = bitmap_group_get(
		verify_tag_reference(&meter->meter_bitmap));
	bitmap = bitmap_group_get_bitmap_from_sequence(
		meter->meter_bitmap.index,
		meter->sequence_index,
		0);
	if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
	{
		real_rectangle2d const *clip = get_sprite_clip_rect(
			verify_tag_reference(&meter->meter_bitmap),
			meter->sequence_index,
			0);
		boolean is_interface_bitmap =
			bitmap_group->type == _bitmap_group_type_interface_bitmaps;
		short min_alpha = MAX(meter->minimum_value,
			PIN(fast_ftol_C(meter->alpha_multiplier*min_value + meter->alpha_bias), 0, UNSIGNED_CHAR_MAX));
		short max_alpha = MAX(meter->minimum_value,
			PIN(fast_ftol_C(meter->alpha_multiplier*max_value + meter->alpha_bias), 0, UNSIGNED_CHAR_MAX));
		struct rasterizer_meter_parameters meter_parameters;

		if (TEST_FLAG(draw_flags, _hud_draw_disabled_bit))
		{
			meter_parameters.gradient_min_color = 0;
			meter_parameters.flash_color = 0;
			meter_parameters.gradient_max_color = 0;
		}
		else if (!TEST_FLAG(meter->meter_flags, _hud_meter_switch_color_on_state_change_bit))
		{
			real fade = reference_time<0.0f ? 0.0f : PIN(1.0f-reference_time, 0.0f, 1.0f);
			real_rgb_color flash_color;

			pixel32_to_real_rgb_color(meter->flash_color, &flash_color);
			flash_color.red *= fade;
			flash_color.green *= fade;
			flash_color.blue *= fade;

			meter_parameters.gradient_min_color = (meter->min_color&0xFFFFFF) | (min_alpha<<24);
			meter_parameters.gradient_max_color = meter->max_color&0xFFFFFF;
			meter_parameters.flash_color = (real_rgb_color_to_pixel32(&flash_color)&0xFFFFFF) | (max_alpha<<24);
		}
		else if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
		{
			if (TEST_FLAG(meter->meter_flags, _hud_meter_interpolates_between_min_max_bit))
			{
				real_rgb_color min_color;
				real_rgb_color max_color;
				real_rgb_color color;
				real interpolation_value;

				pixel32_to_real_rgb_color(meter->min_color, &min_color);
				pixel32_to_real_rgb_color(meter->max_color, &max_color);
				if (TEST_FLAG(meter->meter_flags, _hud_meter_invert_interpolation_value_bit))
					interpolation_value = 1.0f-reference_value;
				else
					interpolation_value = reference_value;
				rgb_colors_interpolate(&color, 0, &min_color, &max_color, interpolation_value);

				meter_parameters.gradient_min_color = real_rgb_color_to_pixel32(&color) | (min_alpha<<24);
				meter_parameters.gradient_max_color = real_rgb_color_to_pixel32(&color);
				meter_parameters.flash_color = min_alpha<<24;
			}
			else
			{
				meter_parameters.gradient_min_color = (meter->max_color&0xFFFFFF) | (min_alpha<<24);
				meter_parameters.flash_color = min_alpha<<24;
				meter_parameters.gradient_max_color = meter->max_color&0xFFFFFF;
			}
		}
		else
		{
			meter_parameters.gradient_min_color = (min_alpha<<24) | (meter->min_color&0xFFFFFF);
			meter_parameters.flash_color = min_alpha<<24;
			meter_parameters.gradient_max_color = meter->min_color&0xFFFFFF;
		}

		meter_parameters.background_color =
			((UNSIGNED_CHAR_MAX - (meter->empty_color>>24))<<24) | (meter->empty_color&0xFFFFFF);
		meter_parameters.tint_color = real_alpha_intensity_to_pixel32(
			meter->fade,
			1.0f-meter->opacity);
		meter_parameters.gradient = 1.0f;
		meter_parameters.flash_color_is_negative = FALSE;
		meter_parameters.tint_mode_2 = TRUE;

		hud_draw_bitmap_with_meter(
			&meter_parameters,
			bitmap,
			absolute_placement,
			&meter->placement,
			clip,
			1.0f,
			0.0f,
			0xFFFFFFFF,
			TEST_FLAG(draw_flags, _hud_draw_in_multiplayer_bit),
			is_interface_bitmap,
			FALSE);
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 428);

	return;
}

void hud_draw_numbers(
	short local_player_index,
	struct hud_absolute_placement_definition const *absolute_placement,
	struct number_hud_element_definition const *numbers,
	short value,
	short decimal_value,
	short draw_flags,
	long flash_reference_time,
	real override_scale)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];
	long hud_number_index;

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	hud_number_index = interface_get_tag_index(_interface_hud_digits);
	if (hud_number_index != NONE)
	{
		struct hud_number_definition const *hud_number =
			hud_number_definition_get(hud_number_index);
		struct bitmap_group const *bitmap_group =
			bitmap_group_get(hud_number->number_bitmap.index);
		struct bitmap_data *source_bitmap = bitmap_group_get_bitmap_from_sequence(
			hud_number->number_bitmap.index,
			0,
			0);
		boolean kilometers = value > 999;

		if (_texture_cache_bitmap_get_hardware_format(source_bitmap, FALSE, TRUE))
		{
			boolean negative = value < 0;
			real digit_count = (real)(numbers->digits +
				(numbers->fractional_digits && decimal_value != NONE ?
					MIN(numbers->fractional_digits, 4) + 1 : 0));
			real decimal_point_width = (real)(numbers->fractional_digits ?
				hud_number->decimal_point_width : 0);
			real scale;
			point2d origin;
			point2d cursor;
			short digit_index;

			if (override_scale > 0.0f)
			{
				scale = override_scale;
			}
			else
			{
				scale = hud_globals_get_scale(
					TEST_FLAG(draw_flags, _hud_draw_in_multiplayer_bit));
			}

			if (TEST_FLAG(numbers->number_flags, _hud_number_show_trailing_m_bit))
			{
				digit_count += 1.0f;
				if (kilometers)
				{
					decimal_value = value*10;
					value /= 1000;
				}
			}
			value = abs(value);

			hud_calculate_point(
				local_player_index,
				absolute_placement,
				&numbers->placement,
				NULL,
				TEST_FLAG(draw_flags, _hud_draw_in_multiplayer_bit),
				0.0f,
				&origin);

			switch (absolute_placement->corner)
			{
			case _hud_anchor_top_left:
			case _hud_anchor_bottom_left:
				cursor.x = (short)(((digit_count-2.0f)*hud_number->screen_width +
					decimal_point_width)*scale + origin.x);
				break;

			case _hud_anchor_center:
				cursor.x = (short)(((digit_count-1.0f)*hud_number->screen_width +
					decimal_point_width)*scale*0.5f + origin.x);
				break;

			default:
				match_assert(
					"c:\\halo\\SOURCE\\interface\\hud_draw.c",
					493,
					!"unreachable");
			case _hud_anchor_top_right:
			case _hud_anchor_bottom_right:
				cursor.x = origin.x;
				break;
			}

			if (source_bitmap)
			{
				pixel32 color;

				if (TEST_FLAG(draw_flags, _hud_draw_disabled_bit))
					color = numbers->colors.disabled_color;
				else if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
					color = get_flash_color(&numbers->colors, flash_reference_time);
				else
					color = numbers->colors.color;

				if (TEST_FLAG(numbers->number_flags, _hud_number_show_trailing_m_bit))
				{
					struct bitmap_data const *number_bitmap;
					real_rectangle2d const *clip;
					point2d point;

					point.x = cursor.x;
					point.y = origin.y;
					number_bitmap = NULL;
					clip = NULL;
					hud_retrieve_bitmap_and_bounding_rect(
						hud_number->number_bitmap.index,
						0,
						kilometers ? _hud_number_kilometers_index : _hud_number_meters_index,
						&number_bitmap,
						&clip);
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_draw.c",
						515,
						source_bitmap==number_bitmap);
					hud_draw_bitmap_direct(
						number_bitmap,
						absolute_placement->corner,
						&point,
						clip,
						scale,
						0.0f,
						color,
						bitmap_group->type == _bitmap_group_type_interface_bitmaps);
					cursor.x = (short)(cursor.x - hud_number->screen_width*scale);
				}

				if (numbers->fractional_digits && decimal_value >= 0)
				{
					short fractional_digits = MIN(numbers->fractional_digits, 4);

					for (digit_index = fractional_digits; digit_index < 4; digit_index++)
					{
						decimal_value /= 10;
					}

					for (digit_index = 0; digit_index < fractional_digits; digit_index++)
					{
						struct bitmap_data const *number_bitmap;
						real_rectangle2d const *clip;
						point2d point;

						point.x = cursor.x;
						point.y = origin.y;
						number_bitmap = NULL;
						clip = NULL;
						hud_retrieve_bitmap_and_bounding_rect(
							hud_number->number_bitmap.index,
							0,
							decimal_value % 10,
							&number_bitmap,
							&clip);
						match_assert(
							"c:\\halo\\SOURCE\\interface\\hud_draw.c",
							539,
							source_bitmap==number_bitmap);
						hud_draw_bitmap_direct(
							number_bitmap,
							absolute_placement->corner,
							&point,
							clip,
							scale,
							0.0f,
							color,
							bitmap_group->type == _bitmap_group_type_interface_bitmaps);
						cursor.x = (short)(cursor.x - hud_number->screen_width*scale);
						decimal_value /= 10;
					}

					cursor.x = (short)(cursor.x + hud_number->screen_width*scale);
					cursor.x = (short)(cursor.x - hud_number->decimal_point_width*scale);

					{
						struct bitmap_data const *number_bitmap;
						real_rectangle2d const *clip;
						point2d point;

						point.x = cursor.x;
						point.y = origin.y;
						number_bitmap = NULL;
						clip = NULL;
						hud_retrieve_bitmap_and_bounding_rect(
							hud_number->number_bitmap.index,
							0,
							_hud_number_decimal_index,
							&number_bitmap,
							&clip);
						match_assert(
							"c:\\halo\\SOURCE\\interface\\hud_draw.c",
							556,
							source_bitmap==number_bitmap);
						hud_draw_bitmap_direct(
							number_bitmap,
							absolute_placement->corner,
							&point,
							clip,
							scale,
							0.0f,
							color,
							bitmap_group->type == _bitmap_group_type_interface_bitmaps);
						cursor.x = (short)(cursor.x - hud_number->screen_width*scale);
					}
				}

				for (digit_index = 0; digit_index < numbers->digits; digit_index++)
				{
					short digit = value % 10;
					struct bitmap_data const *number_bitmap;
					real_rectangle2d const *clip;
					point2d point;

					if (!value && !TEST_FLAG(numbers->number_flags, _hud_number_show_all_leading_zeros_bit))
						break;

					point.x = cursor.x;
					point.y = origin.y;
					number_bitmap = NULL;
					clip = NULL;
					hud_retrieve_bitmap_and_bounding_rect(
						hud_number->number_bitmap.index,
						0,
						digit,
						&number_bitmap,
						&clip);
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_draw.c",
						575,
						source_bitmap==number_bitmap);
					hud_draw_bitmap_direct(
						number_bitmap,
						absolute_placement->corner,
						&point,
						clip,
						scale,
						0.0f,
						color,
						bitmap_group->type == _bitmap_group_type_interface_bitmaps);
					cursor.x = (short)(cursor.x - hud_number->screen_width*scale);
					value /= 10;
				}

				if (negative)
				{
					struct bitmap_data const *number_bitmap;
					real_rectangle2d const *clip;
					point2d point;

					point.x = cursor.x;
					point.y = origin.y;
					number_bitmap = NULL;
					clip = NULL;
					hud_retrieve_bitmap_and_bounding_rect(
						hud_number->number_bitmap.index,
						0,
						_hud_number_negative_sign_index,
						&number_bitmap,
						&clip);
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_draw.c",
						595,
						source_bitmap==number_bitmap);
					hud_draw_bitmap_direct(
						number_bitmap,
						absolute_placement->corner,
						&point,
						clip,
						scale,
						0.0f,
						color,
						bitmap_group->type == _bitmap_group_type_interface_bitmaps);
				}
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_draw.c", 602);

	return;
}
