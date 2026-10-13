/*
INTERFACE.C

symbols in this file:
000CE180 0020:
	_interface_initialize (0000)
000CE1A0 0010:
	_interface_dispose_from_old_map (0000)
000CE1B0 0020:
	_interface_dispose (0000)
000CE1D0 0080:
	_interface_get_tag_index (0000)
000CE250 0080:
	_interface_get_real_argb_color (0000)
000CE2D0 0050:
	_interface_set_bitmap_text_draw_mode (0000)
000CE320 01c0:
	_interface_get_weapon_hud_index (0000)
000CE4E0 03a0:
	_interface_draw_screen (0000)
000CE880 0080:
	_profile_graph_toggle (0000)
000CE900 0110:
	_render_debug_profile_stall_tick (0000)
000CEA10 08e0:
	_render_debug_profile (0000)
000CF2F0 00e0:
	_interface_splitscreen_render (0000)
000CF3D0 0060:
	_interface_initialize_for_new_map (0000)
000CF430 0070:
	_interface_get_rgb_color (0000)
000CF4A0 0020:
	_interface_draw_fullscreen_overlays (0000)
000CF4C0 0180:
	_interface_draw_bitmap (0000)
000CF640 01c0:
	_interface_draw_bitmap_modulated (0000)
000CF800 0160:
	_interface_draw_bitmap_modulated_p32 (0000)
00270988 0047:
	??_C@_0EH@NDHILNFF@interface_tag_index?$DO?$DN0?5?$CG?$CG?5interf@ (0000)
002709D0 0025:
	??_C@_0CF@HPKANGD@c?3?2halo?2SOURCE?2interface?2interfa@ (0000)
002709F8 001f:
	??_C@_0BP@HBELBMEI@drawingbuf_counts?$FLindex?$FN?5?$DM?5512?$AA@ (0000)
00270A18 000d:
	??_C@_0N@KBDODLDN@?$HMn?$HMn?$HMn?$HMn?$HMn?$HMn?$AA@ (0000)
00270A28 0011:
	??_C@_0BB@EMOPPCDP@?$CF?55d?5particles?$HMn?$AA@ (0000)
00270A3C 002e:
	??_C@_0CO@GJNLBLAB@?$CF?55d?5active?5of?5?$CF?55d?5effects?5?$CI?$CF5d@ (0000)
00270A6C 002e:
	??_C@_0CO@ENADKIPF@?$CF?55d?5active?5of?5?$CF?55d?5objects?5?$CI?$CF?53@ (0000)
00270A9C 0020:
	??_C@_0CA@DMKIAGNI@?$CF?56?41fk?5free?5of?5?$CF?56?41fk?5total?$HMn?$AA@ (0000)
00270ABC 0004:
	__real@3a800000 (0000)
00270AC0 0010:
	??_C@_0BA@DAFKNDHA@window_count?$DN?$DN4?$AA@ (0000)
002E4C88 18918:
	_profile_game_value_count (0000)
	_profile_game_values (0008)
	_profile_frame_value_count (8308)
	_profile_frame_values (8310)
	_profile_graph_value_count (10610)
	_profile_graph_values (10618)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "ai/ai_profile.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/color_table_group.h"
#include "camera/director.h"
#include "cseries/profile.h"
#include "cutscene/cinematics.h"
#include "effects/effects.h"
#include "effects/particles.h"
#include "game/game_globals.h"
#include "game/game_engine.h"
#include "game/player_control.h"
#include "game/players.h"
#include "interface/first_person_weapons.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/interface.h"
#include "interface/hud_messaging.h"
#include "interface/terminal.h"
#include "main/main.h"
#include "main/main_runtime.h"
#include "math/real_math.h"
#include "objects/objects.h"
#include "physics/collision_usage.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_cinematics.h"
#include "render/render.h"
#include "render/render_cameras_internal.h"
#include "scenario/scenario.h"
#include "text/draw_string.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

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
	NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS
};

enum
{
	MAXIMUM_PROFILE_VALUES = 64,

	NUMBER_OF_PROFILE_GAME_VALUES = 3,
	NUMBER_OF_PROFILE_FRAME_VALUES = 1,
	NUMBER_OF_PROFILE_GRAPH_VALUES = 14,
};

enum
{
	_hud_screen_effect_mask_only_when_zoomed_bit = 0,
	_hud_screen_effect_convolution_only_when_zoomed_bit = 0,
	_hud_screen_effect_light_enhancement_only_when_zoomed_bit = 0,
	_hud_screen_effect_light_enhancement_connect_to_flashlight_bit = 1,
	_hud_screen_effect_light_enhancement_uses_convolution_mask_bit = 2,
	_hud_screen_effect_desaturation_only_when_zoomed_bit = 0,
	_hud_screen_effect_desaturation_connect_to_flashlight_bit = 1,
	_hud_screen_effect_desaturation_is_additive_bit = 2,
	_hud_screen_effect_desaturation_uses_convolution_mask_bit = 3,
};

enum
{
	_rasterizer_screen_effect_convolution_type_none = 0,
	_rasterizer_screen_effect_convolution_type_blur,
	_rasterizer_screen_effect_convolution_type_warp,
};

/* ---------- macros */

#define interface_tag_references_get() \
	(scenario_get_game_globals()->interface_tag_references.count ? \
		TAG_BLOCK_GET_ELEMENT(&scenario_get_game_globals()->interface_tag_references, 0, \
			struct interface_tag_references_definition) : \
		NULL)

#define weapon_hud_interface_definition_get(index) \
	((struct weapon_hud_interface_definition *)tag_get('wphi', (index)))

/* ---------- structures */

union argb_color
{
	word n[4];
};
typedef union argb_color argb_color;

typedef char argb_color_size_assert[
	sizeof(argb_color) == 0x8 ? 1 : -1];

struct interface_tag_references_definition
{
	struct tag_reference tags[NUMBER_OF_INTERFACE_TAGS];
	byte unused[48];
};

typedef char interface_tag_references_definition_size_assert[
	sizeof(struct interface_tag_references_definition) == 0x130 ? 1 : -1];

struct weapon_flash_state_definition
{
	short flags;
	short pad02;
	short total_ammo;
	short loaded_ammo;
	short heat;
	short age;
	long unused0C[8];
};

struct icon_hud_element_definition
{
	short sequence_index;
	short width_offset;
	point2d offset;
	pixel32 color;
	char frame_rate;
	byte flags;
	short text_index;
};

struct weapon_hud_interface_definition
{
	struct tag_reference parent_hud;
	struct weapon_flash_state_definition flash_cutoffs;
	struct hud_absolute_placement_definition absolute_placement;
	struct tag_block statics;
	struct tag_block meters;
	struct tag_block numbers;
	struct tag_block crosshairs;
	struct tag_block overlays;
	unsigned long valid_crosshair_types_flags;
	struct tag_block warning_sounds;
	struct tag_block screen_effects;
	long unusedB8[33];
	struct icon_hud_element_definition messaging_icon;
	long unused14C[12];
};

struct hud_screen_effect_definition
{
	long unused00;
	word mask_flags;
	word mask_pad;
	long mask_unused[4];
	struct tag_reference mask_fullscreen;
	struct tag_reference mask_splitscreen;
	long unused38[2];
	word convolution_flags;
	word convolution_pad;
	real convolution_radius_in_bounds[2];
	real convolution_radius_out_bounds[2];
	long unused54[6];
	word light_enhancement_flags;
	short light_enhancement_script_source;
	real light_enhancement_intensity;
	long unused74[6];
	word desaturation_flags;
	short desaturation_script_source;
	real desaturation_intensity;
	real_rgb_color desaturation_tint;
	long unusedA0[6];
};

typedef char weapon_hud_interface_definition_screen_effects_offset_assert[
	offsetof(struct weapon_hud_interface_definition, screen_effects) == 0xAC ? 1 : -1];
typedef char weapon_flash_state_definition_size_assert[
	sizeof(struct weapon_flash_state_definition) == 0x2C ? 1 : -1];
typedef char icon_hud_element_definition_size_assert[
	sizeof(struct icon_hud_element_definition) == 0x10 ? 1 : -1];
typedef char weapon_hud_interface_definition_size_assert[
	sizeof(struct weapon_hud_interface_definition) == 0x17C ? 1 : -1];
typedef char hud_screen_effect_definition_size_assert[
	sizeof(struct hud_screen_effect_definition) == 0xB8 ? 1 : -1];
typedef char hud_screen_effect_definition_light_flags_offset_assert[
	offsetof(struct hud_screen_effect_definition, light_enhancement_flags) == 0x6C ? 1 : -1];
typedef char hud_screen_effect_definition_desaturation_flags_offset_assert[
	offsetof(struct hud_screen_effect_definition, desaturation_flags) == 0x8C ? 1 : -1];
typedef char rasterizer_cinematic_screen_effect_parameters_tint_offset_assert[
	offsetof(struct rasterizer_cinematic_screen_effect_parameters, filter_desaturation_tint) == 0x14 ? 1 : -1];

struct system_memory_information
{
	long available_physical_memory;
	long total_physical_memory;
};

struct profile_value
{
	char name[256];
	char label[256];
	real_argb_color const **color;
	short frame_value;
	short section_index;
	boolean subtract_previous;
	boolean enabled;
};

typedef char profile_value_size_assert[
	sizeof(struct profile_value) == 0x20C ? 1 : -1];

struct interface_hud_scripted_globals
{
	boolean show_hud;
	boolean show_hud_help_text;
	byte unused[2];
};

typedef char interface_hud_globals_default_weapon_hud_index_offset_assert[
	offsetof(struct hud_globals_definition, defaults.default_weapon_hud.index) == 0x2CC ? 1 : -1];

/* ---------- prototypes */

void interface_splitscreen_render(
	void);
void render_debug_profile(
	void);
static void render_debug_profile_stall_tick(
	short stall_type,
	real_rectangle2d const *bounds,
	real *below_position,
	real x,
	real scale);
/* ---------- globals */

extern struct interface_hud_scripted_globals *hud_scripted_globals;

static short profile_game_value_count = NUMBER_OF_PROFILE_GAME_VALUES;
static struct profile_value profile_game_values[MAXIMUM_PROFILE_VALUES] =
{
	{ "game", "game", &global_real_argb_yellow, NONE, NONE, FALSE, TRUE },
	{ "objects_update", "objects", &global_real_argb_green, NONE, NONE, FALSE, TRUE },
	{ "ai_update", "ai", &global_real_argb_blue, NONE, NONE, FALSE, TRUE },
};

static short profile_frame_value_count = NUMBER_OF_PROFILE_FRAME_VALUES;
static struct profile_value profile_frame_values[MAXIMUM_PROFILE_VALUES] =
{
	{ "frame", "frame", &global_real_argb_white, NONE, NONE, FALSE, TRUE },
};

static short profile_graph_value_count = NUMBER_OF_PROFILE_GRAPH_VALUES;
static struct profile_value profile_graph_values[MAXIMUM_PROFILE_VALUES] =
{
	{ "stall", "stall", &global_real_argb_red, NONE, NONE, FALSE, TRUE },
	{ "texture", "texture", &global_real_argb_orange, NONE, NONE, FALSE, TRUE },
	{ "render0", "window0", &global_real_argb_blue, NONE, NONE, FALSE, TRUE },
	{ "render0_1", "window1", &global_real_argb_lightblue, NONE, NONE, TRUE, TRUE },
	{ "render0_2", "window2", &global_real_argb_cyan, NONE, NONE, TRUE, TRUE },
	{ "render0_3", "window3", &global_real_argb_purple, NONE, NONE, TRUE, TRUE },
	{ "render0_3np", "overlay", &global_real_argb_salmon, NONE, NONE, TRUE, TRUE },
	{ "render", "render", &global_real_argb_violet, NONE, NONE, TRUE, TRUE },
	{ "game_render", "game", &global_real_argb_yellow, NONE, NONE, TRUE, TRUE },
	{ "load", "load", &global_real_argb_magenta, NONE, NONE, TRUE, TRUE },
	{ "frame", "time", &global_real_argb_white, NONE, NONE, TRUE, TRUE },
	{ "gpu", "gpu", &global_real_argb_green, NONE, NONE, FALSE, TRUE },
	{ "pushbuffer", "pushbuffer", &global_real_argb_darkgreen, NONE, NONE, FALSE, TRUE },
	{ "dt", "dt", &global_real_argb_grey, NONE, NONE, FALSE, TRUE },
};

/* ---------- public code */

void interface_initialize(
	void)
{
	terminal_initialize();
	hud_initialize();
	draw_string_initialize();
	first_person_weapons_initialize();

	return;
}

void interface_initialize_for_new_map(
	void)
{
	hud_initialize_for_new_map();
	draw_string_initialize_for_new_map();
	first_person_weapons_initialize_for_new_map();

	draw_string_set_draw_mode(
		interface_tag_references_get()->tags[_interface_font_terminal].index,
		NONE,
		0,
		0,
		global_real_argb_white);

	return;
}

void interface_dispose_from_old_map(
	void)
{
	draw_string_dispose_from_old_map();
	hud_dispose_from_old_map();
	first_person_weapons_dispose_from_old_map();

	return;
}

void interface_dispose(
	void)
{
	draw_string_dispose();
	terminal_dispose();
	hud_dispose();
	first_person_weapons_dispose();

	return;
}

long interface_get_tag_index(
	short interface_tag_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\interface.c",
		109,
		interface_tag_index>=0 && interface_tag_index<NUMBER_OF_INTERFACE_TAGS);

	return interface_tag_references_get()->tags[interface_tag_index].index;
}

real_argb_color *interface_get_real_argb_color(
	short interface_color_table_index,
	short color_index,
	real_argb_color *color)
{
	long color_table_tag_index = interface_get_tag_index(interface_color_table_index);

	color->alpha = color->red = color->green = color->blue = 1.0f;

	if (color_table_tag_index != NONE)
	{
		struct color_table_definition *color_table =
			color_table_definition_get(color_table_tag_index);

		if (color_table->colors.count)
		{
			color_index %= color_table->colors.count;
			*color = TAG_BLOCK_GET_ELEMENT(
				&color_table->colors,
				color_index,
				struct color_table_color)->real_color;
		}
	}

	return color;
}

void interface_set_bitmap_text_draw_mode(
	short interface_font_index,
	short style,
	short justification,
	unsigned long flags,
	short color_table_index,
	short color_index)
{
	long font_tag_index;
	real_argb_color color;

	font_tag_index = interface_get_tag_index(interface_font_index);
	interface_get_real_argb_color(color_table_index, color_index, &color);
	draw_string_set_draw_mode(font_tag_index, style, justification, flags, &color);

	return;
}

argb_color *interface_get_rgb_color(
	short interface_color_table_index,
	short color_index,
	argb_color *color)
{
	real_argb_color real_color;

	interface_get_real_argb_color(
		interface_color_table_index,
		color_index,
		&real_color);
	color->n[0] = (word)(real_color.n[0] * 65535.0f);
	color->n[1] = (word)(real_color.n[1] * 65535.0f);
	color->n[2] = (word)(real_color.n[2] * 65535.0f);
	color->n[3] = (word)(real_color.n[3] * 65535.0f);

	return color;
}

#ifdef HALO_LINUX
boolean console_is_active(
	void);
#endif

void interface_draw_fullscreen_overlays(
	void)
{
	cinematic_render();
	interface_splitscreen_render();
	hud_render_timer();
#ifdef HALO_LINUX
	/* performance mode: the terminal's lines cost a draw per glyph every
	frame; only the console itself still shows */
	if (!halo_performance_mode() || console_is_active())
#endif
	terminal_draw();
	main_framerate_render();
	render_debug_profile();

	return;
}

void interface_draw_bitmap(
	struct bitmap_data const *bitmap,
	point2d const *point,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	real fade)
{
	real_rectangle2d entire_bitmap = { 0.0f, 1.0f, 0.0f, 1.0f };
	real sine_theta = sine(theta);
	real cosine_theta = cosine(theta);
	pixel32 color;
	struct dynamic_screen_vertex vertices[NUMBER_OF_POINTS_PER_RECTANGLE];
	struct rasterizer_dynamic_screen_geometry_parameters parameters;
	short vertex_index;

	if (!clip)
		clip = &entire_bitmap;

	color = (((pixel32)(long)(fade*255.0f))<<24) | 0x00FFFFFF;

	for (vertex_index = 0; vertex_index < NUMBER_OF_POINTS_PER_RECTANGLE; vertex_index++)
	{
		real u = ((vertex_index+1)&2) ? clip->x1 : clip->x0;
		real v = (vertex_index>1) ? clip->y1 : clip->y0;
		real local_x = (bitmap->width*u - bitmap->registration_point.x)*scale;
		real local_y = (bitmap->height*v - bitmap->registration_point.y)*scale;

		vertices[vertex_index].position.x = point->x + local_x*cosine_theta - local_y*sine_theta;
		vertices[vertex_index].position.y = point->y + local_x*sine_theta + local_y*cosine_theta;
		vertices[vertex_index].texture_coordinates.u = u;
		vertices[vertex_index].texture_coordinates.v = v;
		vertices[vertex_index].color = color;
	}

	csmemset(&parameters, 0, sizeof(parameters));
	parameters.map_scale[0].i = parameters.map_scale[0].j =
		parameters.map_texture_scale[0].i = parameters.map_texture_scale[0].j = 1.0f;
	parameters.meter_parameters = NULL;
	parameters.point_sampled = FALSE;
	parameters.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_multiply_add;
	parameters.map[0] = (struct bitmap_data *)bitmap;

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

	return;
}

static long interface_get_weapon_hud_index(
	real *flashlight_power)
{
	long player_index = local_player_get_player_index(render.local_player_index);
	long weapon_hud_index = NONE;
	real flashlight = 0.0f;

	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);
		director_perspective perspective = director_get_perspective(render.local_player_index);

		if (hud_scripted_globals &&
			hud_scripted_globals->show_hud &&
			perspective != _director_perspective_neutral &&
			perspective != _director_perspective_scripted &&
			player->unit_index != NONE)
		{
			long weapon_index = unit_inventory_get_weapon(
				player->unit_index,
				unit_get(player->unit_index)->unit.current_weapon_index);

			if (weapon_index == NONE)
			{
				struct unit_datum *unit = unit_get(player->unit_index);

				if (unit->object.parent_object_index != NONE &&
					unit->unit.parent_seat_index != NONE)
				{
					struct unit_datum *parent = unit_get(unit->object.parent_object_index);
					struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
						&unit_definition_get(parent->definition_index)->unit.seats,
						unit->unit.parent_seat_index,
						struct unit_seat);

					if (TEST_FLAG(seat->flags, _unit_seat_gunner_bit))
					{
						weapon_index = unit_inventory_get_weapon(
							unit->object.parent_object_index,
							unit_get(unit->object.parent_object_index)->unit.current_weapon_index);
					}
				}
			}
			else
			{
				flashlight = unit_get(player->unit_index)->unit.integrated_night_vision_power;
			}

			if (weapon_index != NONE)
			{
				long hud_index = weapon_definition_get(
					weapon_get(weapon_index)->definition_index)->
					weapon.interface_definition.hud_interface.index;

				if (hud_index != NONE)
				{
					weapon_hud_index = hud_index;
				}
				else if (!unit_get_weapon_count(player->unit_index))
				{
					weapon_hud_index = hud_globals->defaults.default_weapon_hud.index;
				}
			}
		}
	}

	*flashlight_power = flashlight;

	return weapon_hud_index;
}

void interface_draw_bitmap_modulated(
	struct bitmap_data const *bitmap,
	point2d const *point,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	real_argb_color const *modulated_color,
	short shader_type)
{
	real_rectangle2d entire_bitmap =
		{ 0.0f, (real)bitmap->width, 0.0f, (real)bitmap->height };
	real sine_theta = sine(theta);
	real cosine_theta = cosine(theta);
	pixel32 color;
	struct dynamic_screen_vertex vertices[NUMBER_OF_POINTS_PER_RECTANGLE];
	struct rasterizer_dynamic_screen_geometry_parameters parameters;
	short vertex_index;

	if (!clip)
		clip = &entire_bitmap;

	color =
		(((pixel32)(long)(modulated_color->alpha*255.0f))<<24) |
		(((pixel32)(long)(modulated_color->red*255.0f))<<16) |
		(((pixel32)(long)(modulated_color->green*255.0f))<<8) |
		((pixel32)(long)(modulated_color->blue*255.0f));

	for (vertex_index = 0; vertex_index < NUMBER_OF_POINTS_PER_RECTANGLE; vertex_index++)
	{
		real u = ((vertex_index+1)&2) ? clip->x1 : clip->x0;
		real v = (vertex_index>1) ? clip->y1 : clip->y0;
		real local_x = (u - bitmap->registration_point.x)*scale;
		real local_y = (v - bitmap->registration_point.y)*scale;

		vertices[vertex_index].position.x = point->x + local_x*cosine_theta - local_y*sine_theta;
		vertices[vertex_index].position.y = point->y + local_x*sine_theta + local_y*cosine_theta;
		vertices[vertex_index].texture_coordinates.u = u;
		vertices[vertex_index].texture_coordinates.v = v;
		vertices[vertex_index].color = color;
	}

	csmemset(&parameters, 0, sizeof(parameters));
	parameters.map_scale[0].i = parameters.map_scale[0].j =
		parameters.map_texture_scale[0].i = parameters.map_texture_scale[0].j = 1.0f;
	parameters.meter_parameters = NULL;
	parameters.point_sampled = FALSE;
	parameters.framebuffer_blend_function = shader_type;
	parameters.map[0] = (struct bitmap_data *)bitmap;

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

	return;
}

void interface_draw_screen(
	void)
{
	real flashlight_power;
	long weapon_hud_index;

	if (render.local_player_index == NONE)
		return;

	weapon_hud_index = interface_get_weapon_hud_index(&flashlight_power);
	if (weapon_hud_index != NONE)
	{
		struct weapon_hud_interface_definition *hud_definition =
			weapon_hud_interface_definition_get(weapon_hud_index);

		if (hud_definition->screen_effects.count > 0)
		{
			struct hud_screen_effect_definition *screen_effect = TAG_BLOCK_GET_ELEMENT(
				&hud_definition->screen_effects,
				0,
				struct hud_screen_effect_definition);
			boolean zoomed = player_control_get_zoom_level(render.local_player_index) != NONE;
			struct rasterizer_cinematic_screen_effect_parameters parameters;

			csmemset(&parameters, 0, sizeof(parameters));

			if (zoomed ||
				!TEST_FLAG(screen_effect->mask_flags,
					_hud_screen_effect_mask_only_when_zoomed_bit))
			{
				long mask_tag_index = main_get_window_count() <= 1 ?
					screen_effect->mask_fullscreen.index :
					screen_effect->mask_splitscreen.index;

				if (mask_tag_index != NONE)
				{
					parameters.convolution_mask = TAG_BLOCK_GET_ELEMENT(
						&bitmap_group_get(mask_tag_index)->bitmaps,
						0,
						struct bitmap_data);
					parameters.filter_light_enhancement_uses_convolution_mask =
						TEST_FLAG(
							screen_effect->light_enhancement_flags,
							_hud_screen_effect_light_enhancement_uses_convolution_mask_bit);
					parameters.filter_desaturation_uses_convolution_mask =
						TEST_FLAG(
							screen_effect->desaturation_flags,
							_hud_screen_effect_desaturation_uses_convolution_mask_bit);
				}
			}

			if (main_get_window_count() <= 1 &&
				(zoomed ||
					!TEST_FLAG(
						screen_effect->convolution_flags,
						_hud_screen_effect_convolution_only_when_zoomed_bit)))
			{
				real convolution_radius = 0.0f;

				if (screen_effect->convolution_radius_in_bounds[0] !=
					screen_effect->convolution_radius_in_bounds[1])
				{
					real interpolation = PIN(
						(render.camera.vertical_field_of_view -
							screen_effect->convolution_radius_in_bounds[0]) /
						(screen_effect->convolution_radius_in_bounds[1] -
							screen_effect->convolution_radius_in_bounds[0]),
						0.0f,
						1.0f);

					scalars_interpolate(
						screen_effect->convolution_radius_out_bounds[0],
						screen_effect->convolution_radius_out_bounds[1],
						interpolation,
						&convolution_radius);
				}
				else
				{
					convolution_radius = screen_effect->convolution_radius_out_bounds[1];
				}

				if (convolution_radius > 0.0f)
				{
					parameters.convolution_radius = convolution_radius;
					parameters.convolution_type =
						_rasterizer_screen_effect_convolution_type_warp;
				}
			}

			if (zoomed ||
				!TEST_FLAG(
					screen_effect->light_enhancement_flags,
					_hud_screen_effect_light_enhancement_only_when_zoomed_bit))
			{
				real intensity = screen_effect->light_enhancement_intensity;

				if (TEST_FLAG(
					screen_effect->light_enhancement_flags,
					_hud_screen_effect_light_enhancement_connect_to_flashlight_bit))
				{
					intensity *= PIN(flashlight_power, 0.0f, 1.0f);
				}

				intensity *= PIN(
					rasterizer_script_screen_effect_get_value(
						screen_effect->light_enhancement_script_source),
					0.0f,
					1.0f);
				if (intensity > 0.0f)
					parameters.filter_light_enhancement_intensity = intensity;
			}

			if (zoomed ||
				!TEST_FLAG(
					screen_effect->desaturation_flags,
					_hud_screen_effect_desaturation_only_when_zoomed_bit))
			{
				real intensity = screen_effect->desaturation_intensity;

				if (TEST_FLAG(
					screen_effect->desaturation_flags,
					_hud_screen_effect_desaturation_connect_to_flashlight_bit))
				{
					intensity *= PIN(flashlight_power, 0.0f, 1.0f);
				}

				intensity *= PIN(
					rasterizer_script_screen_effect_get_value(
						screen_effect->desaturation_script_source),
					0.0f,
					1.0f);
				if (intensity > 0.0f)
				{
					parameters.filter_desaturation_intensity = intensity;
					parameters.filter_desaturation_is_additive = TEST_FLAG(
						screen_effect->desaturation_flags,
						_hud_screen_effect_desaturation_is_additive_bit);
					parameters.filter_desaturation_tint = screen_effect->desaturation_tint;
				}
			}

			rasterizer_screen_effect(&parameters);
		}
		else
		{
			rasterizer_screen_effect(NULL);
		}
	}
	else
	{
		rasterizer_screen_effect(NULL);
	}

	hud_draw_screen();
	game_engine_post_rasterize();

	return;
}

void interface_draw_bitmap_modulated_p32(
	struct bitmap_data const *bitmap,
	point2d const *point,
	real_rectangle2d const *clip,
	real scale,
	real theta,
	pixel32 modulated_color,
	short shader_type)
{
	real_rectangle2d entire_bitmap =
		{ 0.0f, (real)bitmap->width, 0.0f, (real)bitmap->height };
	real sine_theta = sine(theta);
	real cosine_theta = cosine(theta);
	struct dynamic_screen_vertex vertices[NUMBER_OF_POINTS_PER_RECTANGLE];
	struct rasterizer_dynamic_screen_geometry_parameters parameters;
	short vertex_index;

	if (!clip)
		clip = &entire_bitmap;

	for (vertex_index = 0; vertex_index < NUMBER_OF_POINTS_PER_RECTANGLE; vertex_index++)
	{
		real u = ((vertex_index+1)&2) ? clip->x1 : clip->x0;
		real v = (vertex_index>1) ? clip->y1 : clip->y0;
		real local_x = (u - bitmap->registration_point.x)*scale;
		real local_y = (v - bitmap->registration_point.y)*scale;

		vertices[vertex_index].position.x = point->x + local_x*cosine_theta - local_y*sine_theta;
		vertices[vertex_index].position.y = point->y + local_x*sine_theta + local_y*cosine_theta;
		vertices[vertex_index].texture_coordinates.u = u;
		vertices[vertex_index].texture_coordinates.v = v;
		vertices[vertex_index].color = modulated_color;
	}

	csmemset(&parameters, 0, sizeof(parameters));
	parameters.map_scale[0].i = parameters.map_scale[0].j =
		parameters.map_texture_scale[0].i = parameters.map_texture_scale[0].j = 1.0f;
	parameters.meter_parameters = NULL;
	parameters.point_sampled = FALSE;
	parameters.framebuffer_blend_function = shader_type;
	parameters.map[0] = (struct bitmap_data *)bitmap;

	rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

	return;
}

void profile_graph_toggle(
	char const *graph_name)
{
	short graph_value_index;

	for (graph_value_index = 0;
		graph_value_index < profile_graph_value_count;
		graph_value_index++)
	{
		struct profile_value *graph_value = &profile_graph_values[graph_value_index];

		if (!_stricmp(graph_value->name, graph_name) ||
			!_stricmp(graph_value->label, graph_name))
		{
			graph_value->enabled = !graph_value->enabled;
		}
	}

	return;
}

/* ---------- private code */

static void render_debug_profile_stall_tick(
	short stall_type,
	real_rectangle2d const *bounds,
	real *below_position,
	real x,
	real scale)
{
	real_argb_color const *color;
	real_point3d point0;
	real_point3d point1;

	/* January's stall labels are preserved numerically: the original enum names
	 * are not recoverable from the available profile producer or symbols. */
	switch (stall_type)
	{
		case 1:
			color = global_real_argb_blue;
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
			color = global_real_argb_yellow;
			break;

		case 21:
			color = global_real_argb_green;
			break;

		case 26:
			color = global_real_argb_pink;
			break;

		default:
			color = global_real_argb_white;
			break;
	}

	point0.x = x*scale;
	point0.y = (bounds->y0 - (bounds->y1 - bounds->y0)*(*below_position))*scale;
	point0.z = -scale;
	*below_position += 0.03f;
	point1.x = x*scale;
	point1.y = (bounds->y0 - (bounds->y1 - bounds->y0)*(*below_position))*scale;
	point1.z = -scale;
	*below_position += 0.01f;

	matrix4x3_transform_point(&render.frustum.view_to_world, &point0, &point0);
	matrix4x3_transform_point(&render.frustum.view_to_world, &point1, &point1);
	rasterizer_debug_immediate_line(&point0, &point1, &color->rgb, NULL);

	return;
}

void render_debug_profile(
	void)
{
	if (profile_display)
	{
		char buffer[8192];

		interface_set_bitmap_text_draw_mode(
			_interface_font_terminal,
			NONE,
			0,
			0,
			_interface_color_table_dialog,
			0);

		{
			struct objects_information objects_information;
			struct effects_information effects_information;
			struct system_memory_information memory_information;

			objects_information_get(&objects_information);
			system_memory_information_get(&memory_information);
			effects_information_get(&effects_information);

			csstrcpy(buffer, "");
			sprintf(buffer+csstrlen(buffer), "% 6.1fk free of % 6.1fk total|n",
				memory_information.available_physical_memory/1024.0f,
				memory_information.total_physical_memory/1024.0f);
			sprintf(buffer+csstrlen(buffer), "% 5d active of % 5d objects (% 3.1f%% used)|n",
				objects_information.active_object_count,
				objects_information.object_count,
				objects_information.used_memory*100.0f);
			sprintf(buffer+csstrlen(buffer), "% 5d active of % 5d effects (%5d locations)|n",
				effects_information.active_effect_count,
				effects_information.effect_count,
				effects_information.location_count);
			sprintf(buffer+csstrlen(buffer), "% 5d particles|n",
				particle_data->actual_count);

			ai_profile_display(buffer);
			collision_log_display(buffer);
			rasterizer_draw_string(NULL, NULL, NULL, 0, buffer);
		}

		{
			short tab_stops[4];

			tab_stops[0] = 300;
			tab_stops[1] = 380;
			tab_stops[2] = 460;
			tab_stops[3] = 540;
			sprintf(buffer, "|n|n|n|n|n|n");
			profile_dump(
				NULL,
				_profile_sort_mode_total_time,
				_profile_dump_format_mode_screen,
				10,
				buffer+csstrlen(buffer));
			draw_string_set_tab_stops(tab_stops, 4);
			rasterizer_draw_string(NULL, NULL, NULL, 0, buffer);
			draw_string_set_tab_stops(tab_stops, 0);
		}
	}

	if (profile_graph)
	{
		real_rectangle2d screen_clip_bounds;
		real_rectangle2d graph_bounds;
		real_rectangle2d graph_screen_bounds;

		render_frustum_get_projection_bounds(&render.frustum, &screen_clip_bounds);

		graph_bounds.x0 = screen_clip_bounds.x0 - 0.1f;
		graph_bounds.x1 = screen_clip_bounds.x1 + 0.1f;
		graph_bounds.y0 = screen_clip_bounds.y1 + 0.1f;
		graph_bounds.y1 = screen_clip_bounds.y0 - 0.1f;

		graph_screen_bounds.x0 = rasterizer_globals.reserved04.screen_bounds.x1 -
			(rasterizer_globals.reserved04.screen_bounds.x1 -
				rasterizer_globals.reserved04.screen_bounds.x0)*0.1f;
		graph_screen_bounds.x1 = rasterizer_globals.reserved04.screen_bounds.x0 +
			(rasterizer_globals.reserved04.screen_bounds.x1 -
				rasterizer_globals.reserved04.screen_bounds.x0)*0.1f;
		graph_screen_bounds.y0 = rasterizer_globals.reserved04.screen_bounds.y1 -
			(rasterizer_globals.reserved04.screen_bounds.y1 -
				rasterizer_globals.reserved04.screen_bounds.y0)*0.1f;
		graph_screen_bounds.y1 = rasterizer_globals.reserved04.screen_bounds.y0 +
			(rasterizer_globals.reserved04.screen_bounds.y1 -
				rasterizer_globals.reserved04.screen_bounds.y0)*0.1f;

		if (profile_graph)
		{
			short graph_value_index;

			for (graph_value_index = 0;
				graph_value_index < profile_graph_value_count;
				graph_value_index++)
			{
				struct profile_value *graph_value =
					&profile_graph_values[graph_value_index];

				graph_value->frame_value = profile_find_frame_value(
					graph_value->name,
					&graph_value->section_index);
			}
		}

		if (profile_graph)
		{
			struct profile_frame_iterator iterator;
			struct profile_frame_info frame_info;
			__int64 base_vertical_blank_index =
				rasterizer_globals.vertical_blank_index;

			{
				point2d drawingbuf[MAXIMUM_PROFILE_VALUES][512];
				short drawingbuf_counts[MAXIMUM_PROFILE_VALUES];
				real current_values[MAXIMUM_PROFILE_VALUES];
				real last_values[MAXIMUM_PROFILE_VALUES];
				point2d current_screen_points[MAXIMUM_PROFILE_VALUES];
				point2d last_screen_points[MAXIMUM_PROFILE_VALUES];
				/* The /Od RTC descriptors attest both January locals. Their values
				 * are retained and copied, although no later graph path reads them. */
				real_point3d current_world_points[MAXIMUM_PROFILE_VALUES];
				real_point3d last_world_points[MAXIMUM_PROFILE_VALUES];
				boolean first_frame = TRUE;

				{
					real_point3d point0;
					real_point3d point1;

					point0.x = graph_bounds.x0*0.1f;
					point0.y = graph_bounds.y0*0.1f;
					point0.z = -0.1f;
					point1.x = graph_bounds.x1*0.1f;
					point1.y = graph_bounds.y0*0.1f;
					point1.z = -0.1f;
					rasterizer_debug_immediate_line(
						&point0,
						&point1,
						&global_real_argb_white->rgb,
						NULL);
				}

				csmemset(drawingbuf_counts, 0, sizeof(drawingbuf_counts));

				rasterizer_debug_immediate_begin_screenspace();
				profile_frame_iterator_new(&iterator);
				while (profile_frame_iterator_next(&iterator, &frame_info))
				{
					real frame_seconds = (real)(base_vertical_blank_index -
						frame_info.vertical_blank_index)*(1.0f/60.0f);

					if (frame_seconds < 10.0f)
					{
						real graph_screen_x = graph_screen_bounds.x0 +
							(graph_screen_bounds.x1 - graph_screen_bounds.x0)*
								frame_seconds*0.1f;
						real graph_x = graph_bounds.x0 +
							(graph_bounds.x1 - graph_bounds.x0)*frame_seconds*0.1f;
						short index;

						for (index = 0; index < profile_graph_value_count; index++)
						{
							struct profile_value *graph_value =
								&profile_graph_values[index];
							boolean overlapping;
							real value;
							point2d screenspace_point;
							real_point3d view_point;
							real_point3d world_point;

							if (graph_value->frame_value == NONE)
								continue;

							value = profile_frame_get_value(
								&iterator,
								graph_value->frame_value,
								graph_value->section_index);
							overlapping = FALSE;

							if (value < 0.0f)
								value = 0.0f;
							else if (value > 100.0f)
								value = 100.0f;

							if (graph_value->subtract_previous &&
								index > 0 &&
								profile_graph_values[index-1].enabled &&
								profile_graph_values[index-1].frame_value != NONE &&
								!first_frame)
							{
								if (fabs(value - current_values[index-1]) < 0.1f &&
									fabs(last_values[index] - last_values[index-1]) < 0.1f)
								{
									overlapping = TRUE;
								}
							}

							screenspace_point.x = (short)graph_screen_x;
							screenspace_point.y = (short)(graph_screen_bounds.y0 +
								(graph_screen_bounds.y1 - graph_screen_bounds.y0)*
									(value*0.01f));

							view_point.x = graph_x*0.1f;
							view_point.y = (graph_bounds.y0 +
								(graph_bounds.y1 - graph_bounds.y0)*(value*0.01f))*0.1f;
							view_point.z = -0.1f;
							matrix4x3_transform_point(
								&render.frustum.view_to_world,
								&view_point,
								&world_point);

							if (graph_value->enabled && !first_frame && !overlapping)
							{
								if (!drawingbuf_counts[index])
								{
									drawingbuf[index][0] = last_screen_points[index];
									drawingbuf_counts[index] = 1;
								}

								match_assert(
									"c:\\halo\\SOURCE\\interface\\interface.c",
									704,
									drawingbuf_counts[index] < 512);

								drawingbuf[index][drawingbuf_counts[index]++] =
									screenspace_point;
							}
							else if (drawingbuf_counts[index] > 0)
							{
								rasterizer_debug_immediate_linestrip_screenspace(
									drawingbuf[index],
									drawingbuf_counts[index],
									&(*graph_value->color)->rgb);
								drawingbuf_counts[index] = 0;
							}

							current_values[index] = value;
							current_screen_points[index] = screenspace_point;
							current_world_points[index] = world_point;
						}
					}

					csmemcpy(last_values, current_values, sizeof(last_values));
					csmemcpy(last_screen_points, current_screen_points,
						sizeof(last_screen_points));
					csmemcpy(last_world_points, current_world_points,
						sizeof(last_world_points));
					first_frame = FALSE;
				}

				{
					short index;

					for (index = 0; index < profile_graph_value_count; index++)
					{
						if (drawingbuf_counts[index] > 0)
						{
							rasterizer_debug_immediate_linestrip_screenspace(
								drawingbuf[index],
								drawingbuf_counts[index],
								&(*profile_graph_values[index].color)->rgb);
							drawingbuf_counts[index] = 0;
						}
					}
				}

				rasterizer_debug_immediate_end_screenspace();
			}

			rasterizer_debug_immediate_begin();
			profile_frame_iterator_new(&iterator);
			while (profile_frame_iterator_next(&iterator, &frame_info))
			{
				real frame_seconds = (real)(base_vertical_blank_index -
					frame_info.vertical_blank_index)*(1.0f/60.0f);

				if (frame_seconds < 10.0f)
				{
					char message_strings[48][256];
					char *message_stringptrs[48];
					point2d message_locations[48];
					real_argb_color const *message_colors[48];
					short message_count;
					short largest_stall_type;
					real largest_stall_msec;
					long stall_count;
					short message_index;
					real graph_x = graph_bounds.x0 +
						(graph_bounds.x1 - graph_bounds.x0)*frame_seconds*0.1f;

					for (message_index = 0; message_index < 48; message_index++)
					{
						message_stringptrs[message_index] =
							message_strings[message_index];
					}

					message_count = 0;
					profile_frame_get_messages(
						&iterator,
						&message_count,
						48,
						message_stringptrs,
						message_locations,
						message_colors);

					largest_stall_type = 0;
					largest_stall_msec = 0.0f;
					stall_count = profile_frame_get_stalls(
						&iterator,
						&largest_stall_type,
						&largest_stall_msec);

					if (stall_count)
					{
						real below_position = 0.01f;
						short stall_type;

						if (TEST_FLAG(stall_count, largest_stall_type))
						{
							render_debug_profile_stall_tick(
								largest_stall_type,
								&graph_bounds,
								&below_position,
								graph_x,
								0.1f);
							stall_count &= ~FLAG(largest_stall_type);
						}

						for (stall_type = 0; stall_type < 27; stall_type++)
						{
							if (TEST_FLAG(stall_count, stall_type))
							{
								render_debug_profile_stall_tick(
									stall_type,
									&graph_bounds,
									&below_position,
									graph_x,
									0.1f);
							}
						}
					}
				}
			}

			rasterizer_debug_immediate_end();
		}
	}

	return;
}

void interface_splitscreen_render(
	void)
{
	rectangle2d bounds;
	short window_count;

	if (game_engine_force_single_screen() || cinematic_in_progress())
		return;

	window_count = local_player_count();

	if (window_count <= 1)
		return;

	bounds.y0 = 239;
	bounds.x0 = 0;
	bounds.y1 = 241;
	bounds.x1 = 640;
	draw_quad(&bounds, 0xFF000000);

	if (window_count <= 2)
		return;

	if (window_count == 3)
	{
		bounds.y0 = 240;
		bounds.x0 = 319;
		bounds.y1 = 480;
		bounds.x1 = 321;
		draw_quad(&bounds, 0xFF000000);

		return;
	}

	bounds.y0 = 0;
	bounds.x0 = 319;
	bounds.y1 = 480;
	bounds.x1 = 321;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\interface.c",
		884,
		window_count==4);

	draw_quad(&bounds, 0xFF000000);

	return;
}
