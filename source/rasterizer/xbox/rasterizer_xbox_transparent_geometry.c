/*
RASTERIZER_XBOX_TRANSPARENT_GEOMETRY.C

symbols in this file:
00163B40 01b0:
	_code_00163b40 (0000)
00163CF0 0050:
	_code_00163cf0 (0000)
00163D40 0020:
	_IDirect3DDevice8_CreateVertexBuffer@24 (0000)
00163D60 0180:
	_rasterizer_transparent_geometry_group_draw__internal (0000)
00163EE0 0220:
	_code_00163ee0 (0000)
00164100 0060:
	_code_00164100 (0000)
00164160 0010:
	_code_00164160 (0000)
00164170 0010:
	_code_00164170 (0000)
00164180 0010:
	_IDirect3DDevice8_BeginVisibilityTest@4 (0000)
00164190 0010:
	_IDirect3DDevice8_EndVisibilityTest@8 (0000)
001641A0 0010:
	_IDirect3DDevice8_GetVisibilityTestResult@16 (0000)
001641B0 0010:
	_D3DVertexBuffer_Unlock@4 (0000)
001641C0 0010:
	_IDirect3DVertexBuffer8_Release@4 (0000)
001641D0 0020:
	_IDirect3DVertexBuffer8_Lock@20 (0000)
001641F0 0010:
	_IDirect3DVertexBuffer8_Unlock@4 (0000)
00164200 01b0:
	_rasterizer_transparent_geometry_groups_end (0000)
001643B0 0030:
	_subtract_vectors4d (0000)
001643E0 0040:
	_offset_vector4d (0000)
00164420 00f0:
	_rasterizer_transparent_geometry_initialize_aux_buffer (0000)
00164510 0020:
	_rasterizer_transparent_geometry_dispose_aux_buffer (0000)
00164530 0030:
	_rasterizer_transparent_geometry_groups_begin (0000)
00164560 3b10:
	_rasterizer_transparent_geometry_group_draw (0000)
00292CF0 0030:
	??_C@_0DA@COBMAJBI@group?9?$DOtriangle_count?$DN?$DNvertices_@ (0000)
00292D20 000e:
	??_C@_0O@MKIGFILJ@?$CBhas_lightmap?$AA@ (0000)
00292D30 0046:
	??_C@_0EG@MGBFGMKK@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
00292D78 003e:
	??_C@_0DO@HNCJICHK@?$CD?$CD?$CD?5ERROR?5rasterizer_transparent@ (0000)
00292DB8 0006:
	??_C@_05IDMOFOMC@?$CF?402f?$AA@ (0000)
00292DC0 0003:
	??_C@_02FLHDKHAB@hr?$AA@ (0000)
00292DC4 003d:
	??_C@_0DN@KFMAMEG@IDirect3DDevice8_EndVisibilityTe@ (0000)
00292E04 002d:
	??_C@_0CN@MKPFLBON@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5tex@ (0000)
00292E38 00bb:
	??_C@_0LL@MPPGICGO@IDirect3DVertexBuffer8_Lock?$CIrast@ (0000)
00292EF8 00f7:
	??_C@_0PH@NENLLNM@IDirect3DDevice8_CreateVertexBuf@ (0000)
00292FF0 003c:
	??_C@_0DM@KCHMKPAC@?$CD?$CD?$CD?5ERROR?5rasterizer_transparent@ (0000)
0029302C 0007:
	??_C@_06GACNCAPN@?$CBdirty?$AA@ (0000)
00293034 0022:
	??_C@_0CC@BMOKPLKC@?$CD?$CD?$CD?5ERROR?5unsupported?5shader?5typ@ (0000)
00293058 005a:
	??_C@_0FK@JJJJNNKB@reflection_type?$DO?$DN0?5?$CG?$CG?5reflection@ (0000)
002930B4 003b:
	??_C@_0DL@EGPBHLCN@type?$DO?$DN0?5?$CG?$CG?5type?$DMNUMBER_OF_SHADER@ (0000)
002930F0 00c2:
	??_C@_0MC@HFAHGKIF@?$CBTEST_FLAG?$CIshader_transparent_ch@ (0000)
002931B4 003b:
	??_C@_0DL@BJELFOGH@constant_color0?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5c@ (0000)
002931F0 003b:
	??_C@_0DL@HNJBBLJI@constant_color0?4green?$DO?$DN0?40f?5?$CG?$CG?5c@ (0000)
0029322C 0039:
	??_C@_0DJ@EMPIAIDC@constant_color0?4red?5?$DO?$DN0?40f?5?$CG?$CG?5co@ (0000)
00293268 002e:
	??_C@_0CO@NNMDDBLI@stage?9?$DOconstant_color0_animation@ (0000)
00293298 002c:
	??_C@_0CM@NLPJEHHH@?$CD?$CD?$CD?5ERROR?5unsupported?5framebuffe@ (0000)
002932C8 0094:
	??_C@_0JE@HDBOEKDA@IDirect3DDevice8_SetVertexShader@ (0000)
00293360 0091:
	??_C@_0JB@BKNCAMGA@IDirect3DDevice8_SetVertexShader@ (0000)
002933F4 003b:
	??_C@_0DL@NHKIEDLC@type?$DO?$DN0?5?$CG?$CG?5type?$DMNUMBER_OF_SHADER@ (0000)
00293430 00c2:
	??_C@_0MC@PDFLODDB@?$CBTEST_FLAG?$CIshader_transparent_ge@ (0000)
002934F8 007d:
	??_C@_0HN@NGBJCHJM@IDirect3DDevice8_SetStreamSource@ (0000)
00293578 0091:
	??_C@_0JB@KCBKPHDD@IDirect3DDevice8_SetVertexShader@ (0000)
0029360C 0004:
	__real@4b7fffff (0000)
00293610 0004:
	__real@7149f2ca (0000)
00293618 0076:
	??_C@_0HG@HBDJNJMC@IDirect3DDevice8_SetVertexShader@ (0000)
00293690 0091:
	??_C@_0JB@JFCPBDMI@IDirect3DDevice8_SetVertexShader@ (0000)
00293724 003d:
	??_C@_0DN@FLJIIOKP@?$CD?$CD?$CD?5ERROR?5unsupported?5model?5effe@ (0000)
00293764 0006:
	??_C@_05ICAMDEPF@?$CF?403f?$AA@ (0000)
0029376C 0027:
	??_C@_0CH@EGNMKNCN@color?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5color?4blue?5@ (0000)
00293794 0027:
	??_C@_0CH@MEAOFMIG@color?4green?$DO?$DN0?40f?5?$CG?$CG?5color?4green@ (0000)
002937BC 0025:
	??_C@_0CF@PKCJOKKO@color?4red?5?$DO?$DN0?40f?5?$CG?$CG?5color?4red?5?$DM?$DN@ (0000)
002937E4 0004:
	__real@3e3851ec (0000)
002937E8 0027:
	??_C@_0CH@FDCPMIOG@vertex_shader_table?$FLvertex_type?$FN@ (0000)
00293810 0040:
	??_C@_0EA@EGBNPDLH@vertex_type?$DO?$DN0?5?$CG?$CG?5vertex_type?$DMNU@ (0000)
004662D8 0010:
	_rasterizer_xbox_transparent_geometry_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "main/main_internal.h"
#include "real_math.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_active_camouflage.h"
#include "rasterizer/rasterizer_geometry.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "rasterizer/xbox/rasterizer_xbox_internal.h"
#include "render/render_debug.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"
#include "rasterizer/xbox/rasterizer_xbox_draw_primitives.h"
#include "rasterizer/common/rasterizer_common.h"
#include "text/draw_string.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "bitmaps/bitmap_group.h"
#include "effects/decals.h"
#include "game/game_globals.h"
#include "math/periodic_functions.h"
#include "tag_files/tag_groups.h"

/* January retains the stock XDK D3DINLINE out-of-line wrappers emitted by
 * the real device calls below. Keep the stock definitions intact. */
#include <xtl.h>
#include "interface/progress_bar_internal.h"
#include "rasterizer/xbox/rasterizer_xbox.h"
#include "rasterizer/xbox/rasterizer_xbox_plasma_energy.h"
#include "rasterizer/xbox/rasterizer_xbox_water.h"
#include "rasterizer/xbox/shader_transparent_chicago_preprocessor.h"

/* ---------- constants */

enum
{
	RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE = 8192,
	RASTERIZER_STATIC_BUFFER_USAGE = D3DUSAGE_WRITEONLY,
	RASTERIZER_STATIC_BUFFER_POOL = D3DPOOL_MANAGED,
	RASTERIZER_TRANSPARENT_GEOMETRY_VISIBILITY_TEST_INDEX = 0xfff,
};

enum
{
	_rasterizer_geometry_no_sort_bit = 0,
	_rasterizer_geometry_no_queue_bit,
	_rasterizer_geometry_no_fog_bit,
	_rasterizer_geometry_no_zbuffer_bit,
	_rasterizer_geometry_sky_bit,
	_rasterizer_geometry_viewspace_bit,
	_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit,
	_rasterizer_geometry_first_person_bit,
	_rasterizer_geometry_parts_define_local_nodes_bit,
	NUMBER_OF_RASTERIZER_GEOMETRY_FLAGS
};

enum
{
	_shader_type_screen = 0,
	_shader_type_effect,
	_shader_type_decal,
	_shader_type_environment,
	_shader_type_model,
	_shader_type_transparent_generic,
	_shader_type_transparent_chicago,
	_shader_type_transparent_water,
	_shader_type_transparent_glass,
	_shader_type_transparent_meter,
	_shader_type_transparent_plasma,
	NUMBER_OF_SHADER_TYPES
};

enum
{
	_shader_effect_uses_nonlinear_tint_bit = 1,
	_shader_effect_flags_dont_overdraw_first_person_weapon_bit,
};

enum
{
	_shader_effect_map_point_sampled_bit = 0,
	_shader_effect_map_u_clamp_bit,
	_shader_effect_map_v_clamp_bit,
};

enum
{
	_shader_effect_secondary_map_anchor_zsprite = 2,
};

enum
{
	_render_model_effect_type_none = 0,
	_render_model_effect_type_active_camouflage,
	_render_model_effect_type_cortana,
	NUMBER_OF_RENDER_MODEL_EFFECT_TYPES
};

enum
{
	RASTERIZER_STENCIL_MODE_NONE = 0,
	RASTERIZER_STENCIL_MODE_REJECT = 2,
	RASTERIZER_STENCIL_MODE_ACCEPT = 3,
};

enum
{
	RASTERIZER_TRANSPARENT_GEOMETRY_MAXIMUM_PASSES = 2,
	RASTERIZER_TRANSPARENT_GEOMETRY_ALL_GROUPS_INDEX = 1000,
	RASTERIZER_TRANSPARENT_GEOMETRY_CAMERA_CONSTANT_INDEX = 88,
};

enum
{
	VSH_CONSTANTS__TEXSCALE_OFFSET = -0x54,
	VSH_CONSTANTS__TEXSCALE_COUNT = 3,
	VSH_CONSTANTS__TEXANIM_OFFSET = -0x51,
	VSH_CONSTANTS__TEXANIM_COUNT = 8,
	VSH_CONSTANTS__INVERSE_OFFSET = 0x58,
	VSH_CONSTANTS__INVERSE_COUNT = 3,
	VSH_CONSTANTS__ZSPRITE_OFFSET = -0x3F,
	VSH_CONSTANTS__ZSPRITE_COUNT = 2,
};

enum
{
	NUMBER_OF_SHADER_TRANSPARENT_MAPS = 4
};

enum
{
	_shader_radiosity_FILTHY_transparent_lit_bit = 2
};

enum
{
	_shader_transparent_generic_type_2d_map = 0,
	NUMBER_OF_SHADER_TRANSPARENT_GENERIC_TYPES = 4
};

enum
{
	_shader_transparent_chicago_type_2d_map = 0,
	NUMBER_OF_SHADER_TRANSPARENT_CHICAGO_TYPES = 4
};

enum
{
	_shader_transparent_map_flag_unfiltered_bit = 0,
	_shader_transparent_map_flag_u_clamped_bit,
	_shader_transparent_map_flag_v_clamped_bit,
};

enum
{
	_shader_transparent_chicago_map_flag_u_clamped_bit = 2,
	_shader_transparent_chicago_map_flag_v_clamped_bit = 3,
};

enum
{
	_shader_transparent_flag_alpha_tested_bit = 0,
	_shader_transparent_flag_decal_bit,
	_shader_transparent_flag_two_sided_bit,
	_shader_transparent_flag_first_map_is_in_screenspace_bit,
	_shader_transparent_flag_draw_before_water_bit,
	_shader_transparent_flag_ignore_effect_bit,
	_shader_transparent_flag_scale_first_map_with_distance_bit,
	_shader_transparent_flag_numeric_bit,
};

enum
{
	_shader_transparent_chicago_extra_flag_dont_fade_active_camouflage_bit = 0,
	_shader_transparent_chicago_extra_flag_numeric_countdown_timer_bit,
};

enum
{
	_shader_transparent_glass_flag_alpha_tested_bit = 0,
	_shader_transparent_glass_flag_decal_bit,
	_shader_transparent_glass_flag_two_sided_bit,
	_shader_transparent_glass_flag_bump_map_is_specular_mask_bit,
};

enum
{
	_shader_transparent_glass_reflection_type_bumped_cube_map = 0,
	_shader_transparent_glass_reflection_type_flat_cube_map,
	_shader_transparent_glass_reflection_type_dynamic_mirror,
	NUMBER_OF_SHADER_TRANSPARENT_GLASS_REFLECTION_TYPES
};

enum
{
	_shader_transparent_meter_flag_decal_bit = 0,
	_shader_transparent_meter_flag_two_sided_bit,
	_shader_transparent_meter_flag_flash_color_is_negative_bit,
	_shader_transparent_meter_flag_tint_mode_2_bit,
	_shader_transparent_meter_flag_unfiltered_bit,
};

enum
{
	_shader_transparent_generic_stage_flag_a_out_controls_color0_animation_bit = 2
};

enum
{
	_framebuffer_fade_mode_none = 0,
	_framebuffer_fade_mode_fade_when_perpendicular,
	_framebuffer_fade_mode_fade_when_parallel,
	NUMBER_OF_FRAMEBUFFER_FADE_MODES
};

enum
{
	_framebuffer_blend_function_alpha_blend = 0,
	_framebuffer_blend_function_multiply,
	_framebuffer_blend_function_double_multiply,
	_framebuffer_blend_function_add,
	_framebuffer_blend_function_subtract,
	_framebuffer_blend_function_component_min,
	_framebuffer_blend_function_component_max,
	_framebuffer_blend_function_alpha_multiply_add,
	NUMBER_OF_FRAMEBUFFER_BLEND_FUNCTIONS
};

enum
{
	NUMBER_OF_SHADER_ANIMATION_FUNCTIONS = 4,
	NUMBER_OF_SHADER_ANIMATION_SOURCES = 5
};

enum
{
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_EFFECT = 65,
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_GENERIC = 24,
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_TINT = 46,
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_REFLECTION = 43,
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_DIFFUSE = 25,
	TRANSPARENT_GEOMETRY_VERTEX_SHADER_METER = 22
};

/* ---------- macros */

/* ---------- structures */

struct rasterizer_transparent_geometry_debug_options_prefix
{
	byte reserved00[0xA];
	boolean debug_transparent_geometry;
	boolean debug_meter_shader;
	byte reserved0C[0x10];
	boolean draw_environment_fog;
	byte reserved1D[0x15];
	short transparent_geometry_index;
	byte reserved34[0xE];
	boolean active_camouflage_multipass;
	byte reserved43[0x11];
	unsigned long zbias;
	byte reserved58[0x8];
	boolean zsprites;
	byte reserved61[0xB];
	real transparent_geometry_intensity;
	real debug_shader_values[6];
	boolean transparent_pixel_counter_active;
	boolean transparent_pixel_counter;
	byte reserved8A[2];
};

typedef char rasterizer_transparent_geometry_debug_options_index_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		transparent_geometry_index) == 0x32 ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_zbias_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		zbias) == 0x54 ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_intensity_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		transparent_geometry_intensity) == 0x6C ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_counter_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		transparent_pixel_counter_active) == 0x88 ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_fog_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		draw_environment_fog) == 0x1C ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_zsprites_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		zsprites) == 0x60 ? 1 : -1];
typedef char rasterizer_transparent_geometry_debug_options_values_offset_assert[
	offsetof(struct rasterizer_transparent_geometry_debug_options_prefix,
		debug_shader_values) == 0x70 ? 1 : -1];

struct pixel_shader_definition
{
	unsigned long alpha_inputs[8];
	unsigned long final_combiner_inputs_abcd;
	unsigned long final_combiner_inputs_efg;
	unsigned long constant_0[8];
	unsigned long constant_1[8];
	unsigned long alpha_outputs[8];
	unsigned long rgb_inputs[8];
	unsigned long compare_mode;
	unsigned long final_combiner_constant_0;
	unsigned long final_combiner_constant_1;
	unsigned long rgb_outputs[8];
	unsigned long combiner_count;
	unsigned long texture_modes;
	unsigned long dot_mapping;
	unsigned long input_texture;
	unsigned long c0_mapping;
	unsigned long c1_mapping;
	unsigned long final_combiner_constants;
};

/* the transparent shader tag layouts January reads here; the same file-local
 * form SHADER_TRANSPARENT_GENERIC_PREPROCESSOR.C and
 * SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR.C use, extended with the fields this
 * file touches */

struct shader_transparent_generic_map
{
	word flags;
	word pad02;
	real map_u_scale;
	real map_v_scale;
	real map_u_offset;
	real map_v_offset;
	real map_rotation;
	real mipmap_bias;
	struct tag_reference map;
	struct shader_texture_animation map_animation;
};

typedef char shader_transparent_generic_map_size_assert[
	sizeof(struct shader_transparent_generic_map) == 0x64 ? 1 : -1];
typedef char shader_transparent_generic_map_animation_offset_assert[
	offsetof(struct shader_transparent_generic_map, map_animation) == 0x2C ? 1 : -1];

struct shader_transparent_generic_stage
{
	word flags;
	short pad02;
	short constant_color0_animation_source;
	short constant_color0_animation_function;
	real constant_color0_animation_period;
	real_argb_color constant_color0;
	real_argb_color constant_color1;
	byte reserved_after_constant_color1[68];
};

typedef char shader_transparent_generic_stage_size_assert[
	sizeof(struct shader_transparent_generic_stage) == 0x70 ? 1 : -1];

struct shader_transparent_generic
{
	byte numeric_counter_limit;
	byte flags;
	short type;
	short framebuffer_blend_function;
	short framebuffer_fade_mode;
	short framebuffer_fade_source;
	short pad32;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
	struct tag_block extra_layers;
	struct tag_block maps;
	struct tag_block stages;
};

struct shader_transparent_generic_definition
{
	struct shader_base shader;
	struct shader_transparent_generic generic;
};

typedef char shader_transparent_generic_maps_offset_assert[
	offsetof(struct shader_transparent_generic_definition, generic.maps) == 0x54 ? 1 : -1];

struct shader_transparent_chicago_map
{
	word flags;
	byte reserved_before_functions[42];
	short color_function;
	short alpha_function;
	byte reserved_before_map_u_scale[36];
	real map_u_scale;
	real map_v_scale;
	real map_u_offset;
	real map_v_offset;
	real map_rotation;
	real mipmap_bias;
	struct tag_reference map;
	byte reserved_after_map[40];
	struct shader_texture_animation map_animation;
};

typedef char shader_transparent_chicago_map_size_assert[
	sizeof(struct shader_transparent_chicago_map) == 0xDC ? 1 : -1];
typedef char shader_transparent_chicago_map_u_scale_offset_assert[
	offsetof(struct shader_transparent_chicago_map, map_u_scale) == 0x54 ? 1 : -1];

struct shader_transparent_chicago
{
	byte numeric_counter_limit;
	byte flags;
	short type;
	short framebuffer_blend_function;
	short framebuffer_fade_mode;
	short framebuffer_fade_source;
	short pad32;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
	struct tag_block extra_layers;
	struct tag_block maps;
	word extra_flags;
	word pad62;
};

struct shader_transparent_chicago_definition
{
	struct shader_base shader;
	struct shader_transparent_chicago chicago;
};

typedef char shader_transparent_chicago_extra_flags_offset_assert[
	offsetof(struct shader_transparent_chicago_definition, chicago.extra_flags) == 0x60 ? 1 : -1];

struct shader_transparent_glass_definition
{
	struct shader shader;
	word flags;
	short pad2A;
	byte reserved_before_tint_color[40];
	real_rgb_color tint_color;
	real tint_map_scale;
	struct tag_reference tint_map;
	byte reserved_before_reflection_flags[20];
	word reflection_flags;
	short reflection_type;
	real_argb_color reflection_view_perpendicular_color;
	real_argb_color reflection_view_parallel_color;
	struct tag_reference reflection_map;
	real reflection_bump_map_scale;
	struct tag_reference reflection_bump_map;
	byte reserved_before_diffuse_flags[128];
	word diffuse_flags;
	word pad152;
	real diffuse_map_scale;
	struct tag_reference diffuse_map;
	real diffuse_detail_map_scale;
	struct tag_reference diffuse_detail_map;
	byte reserved_after_diffuse_detail_map[100];
};

typedef char shader_transparent_glass_reflection_type_offset_assert[
	offsetof(struct shader_transparent_glass_definition, reflection_type) == 0x8A ? 1 : -1];
typedef char shader_transparent_glass_diffuse_detail_map_offset_assert[
	offsetof(struct shader_transparent_glass_definition, diffuse_detail_map) == 0x16C ? 1 : -1];

struct shader_transparent_meter_definition
{
	struct shader shader;
	word flags;
	short pad2A;
	byte reserved_before_map[32];
	struct tag_reference map;
	byte reserved_before_gradient_min_color[32];
	real_rgb_color gradient_min_color;
	real_rgb_color gradient_max_color;
	real_rgb_color background_color;
	real_rgb_color flash_color;
	real_rgb_color tint_color;
	real meter_transparency;
	real background_transparency;
	byte reserved_before_meter_brightness_source[24];
	short meter_brightness_source;
	short flash_brightness_source;
	short value_source;
	short gradient_source;
	short flash_extension_source;
	word padE2;
	byte reserved_after_flash_extension_source[32];
};

typedef char shader_transparent_meter_gradient_min_color_offset_assert[
	offsetof(struct shader_transparent_meter_definition, gradient_min_color) == 0x7C ? 1 : -1];
typedef char shader_transparent_meter_brightness_source_offset_assert[
	offsetof(struct shader_transparent_meter_definition, meter_brightness_source) == 0xD8 ? 1 : -1];

struct rasterizer_model_skinning_parameters
{
	void const *node_matrices;
	short node_matrix_count;
	word pad06;
};

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader *shader;
	short shader_permutation_index;
	short pad12;
	short effect_type;
	short pad16;
	real effect_intensity;
	byte reserved1C[0x20];
	real_vector2d model_base_map_scale;
	long dynamic_triangle_buffer_index;
	/* a NULL shader marks a widget group: rasterizer_xbox_widgets.c stores
	 * render_proc here and its two arguments in the next two fields */
	union
	{
		struct triangle_buffer const *triangle_buffer;
		void (*render_proc)(
			long object_index,
			long widget_index);
	};
	long first_triangle_index;
	long triangle_count;
	long dynamic_vertex_buffer_index;
	struct vertex_buffer const *vertex_buffer;
	struct bitmap_data const *lightmap;
	void const *node_matrices;
	short node_matrix_count;
	word pad66;
	struct render_lighting const *lighting;
	struct render_animation const *animation;
	real z_sort;
	real_point3d centroid;
	real_plane3d plane;
	long sorted_index;
	short previous_group_presorted_index;
	short next_group_presorted_index;
	long active_camouflage_transparent_source_object_index;
	byte reserved9C;
	boolean cortana_hack;
	byte reserved9E[2];
};

typedef char transparent_geometry_group_size_assert[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];
typedef char transparent_geometry_group_triangle_buffer_offset_assert[
	offsetof(struct transparent_geometry_group, triangle_buffer) == 0x48 ? 1 : -1];
typedef char transparent_geometry_group_vertex_buffer_offset_assert[
	offsetof(struct transparent_geometry_group, vertex_buffer) == 0x58 ? 1 : -1];
typedef char transparent_geometry_group_effect_type_offset_assert[
	offsetof(struct transparent_geometry_group, effect_type) == 0x14 ? 1 : -1];
typedef char transparent_geometry_group_effect_intensity_offset_assert[
	offsetof(struct transparent_geometry_group, effect_intensity) == 0x18 ? 1 : -1];
typedef char transparent_geometry_group_node_matrices_offset_assert[
	offsetof(struct transparent_geometry_group, node_matrices) == 0x60 ? 1 : -1];
typedef char transparent_geometry_group_lighting_offset_assert[
	offsetof(struct transparent_geometry_group, lighting) == 0x68 ? 1 : -1];
typedef char transparent_geometry_group_centroid_offset_assert[
	offsetof(struct transparent_geometry_group, centroid) == 0x74 ? 1 : -1];
typedef char transparent_geometry_group_sorted_index_offset_assert[
	offsetof(struct transparent_geometry_group, sorted_index) == 0x90 ? 1 : -1];
typedef char transparent_geometry_group_active_camouflage_offset_assert[
	offsetof(struct transparent_geometry_group,
		active_camouflage_transparent_source_object_index) == 0x98 ? 1 : -1];

struct rasterizer_xbox_transparent_geometry_globals
{
	long last_source_object_index;
	D3DVertexBuffer *texcoord_stream;
	boolean test_no_more_active_camo;
	byte reserved09[3];
	unsigned long transparent_pixel_count;
};

typedef char rasterizer_xbox_transparent_geometry_globals_size_assert[
	sizeof(struct rasterizer_xbox_transparent_geometry_globals) == 16 ? 1 : -1];

/* ---------- globals */

static struct rasterizer_xbox_transparent_geometry_globals
	rasterizer_xbox_transparent_geometry_globals = { 0 };

extern struct rasterizer_transparent_geometry_debug_options_prefix
	rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

boolean rasterizer_transparent_geometry_initialize_aux_buffer(
	void)
{
	byte *vertices = NULL;
	boolean success;
	long result = IDirect3DDevice8_CreateVertexBuffer(
		global_d3d_device,
		RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE*(2*sizeof(byte)),
		RASTERIZER_STATIC_BUFFER_USAGE,
		0,
		RASTERIZER_STATIC_BUFFER_POOL,
		&rasterizer_xbox_transparent_geometry_globals.texcoord_stream);

	if (result >= 0)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			result,
			"IDirect3DDevice8_CreateVertexBuffer(global_d3d_device, RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE*(2*sizeof(byte)), RASTERIZER_STATIC_BUFFER_USAGE, 0, RASTERIZER_STATIC_BUFFER_POOL, &rasterizer_xbox_transparent_geometry_texcoord_stream)");
	}

	rasterizer_globals.current_lock_operation = _rasterizer_lock_vertexbuffer_new;
	if (IDirect3DVertexBuffer8_Lock(
		rasterizer_xbox_transparent_geometry_globals.texcoord_stream,
		0,
		RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE*(2*sizeof(byte)),
		&vertices,
		0) >= 0 && success)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			0,
			"IDirect3DVertexBuffer8_Lock(rasterizer_xbox_transparent_geometry_texcoord_stream, 0, RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE*(2*sizeof(byte)), (unsigned char**)&vertices, 0)");
	}
	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	if (success && vertices)
	{
		char quad_texcoords[8] = { 0, 0, 0, -1, -1, -1, -1, 0 };
		long quad_index;

		for (quad_index = 0;
			quad_index < RASTERIZER_TRANSPARENT_GEOMETRY_TEXCOORD_STREAM_SIZE/8;
			quad_index++)
		{
			csmemcpy(vertices, quad_texcoords, sizeof(quad_texcoords));
			vertices += sizeof(quad_texcoords);
		}
		IDirect3DVertexBuffer8_Unlock(
			rasterizer_xbox_transparent_geometry_globals.texcoord_stream);

		success = TRUE;
	}
	else
	{
		success = FALSE;
		error(_error_silent, "### ERROR failed to allocate texcoord stream");
	}

	return success;
}

void rasterizer_transparent_geometry_groups_begin(
	void)
{
	rasterizer_xbox_transparent_geometry_globals.last_source_object_index = 0;
	rasterizer_xbox_transparent_geometry_globals.test_no_more_active_camo = FALSE;
	if (rasterizer_debug_options.transparent_pixel_counter_active &&
		rasterizer_debug_options.transparent_pixel_counter &&
		global_window_parameters.window_index != NONE)
	{
		IDirect3DDevice8_BeginVisibilityTest(global_d3d_device);
	}

	return;
}

void rasterizer_transparent_geometry_groups_end(
	void)
{
	if (rasterizer_debug_options.transparent_pixel_counter_active &&
		rasterizer_debug_options.transparent_pixel_counter)
	{
		long index = RASTERIZER_TRANSPARENT_GEOMETRY_VISIBILITY_TEST_INDEX;
		unsigned long pixel_count = NONE;
		ULONGLONG timestamp;
		boolean success;
		HRESULT result;

		if (global_window_parameters.window_index == 0)
			rasterizer_xbox_transparent_geometry_globals.transparent_pixel_count = 0;

		result = IDirect3DDevice8_EndVisibilityTest(
			global_d3d_device,
			index);
		if (result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"IDirect3DDevice8_EndVisibilityTest(global_d3d_device, index)");
		}

		do
		{
			result = IDirect3DDevice8_GetVisibilityTestResult(
				global_d3d_device,
				index,
				&pixel_count,
				&timestamp);
		}
		while (result == D3DERR_TESTINCOMPLETE);

		if (success && result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"hr");
		}

		rasterizer_xbox_transparent_geometry_globals.transparent_pixel_count += pixel_count;

		if (global_window_parameters.window_index == main_get_window_count()-1)
		{
			long font_index = hud_globals->messaging.single_player_font.index;

			if (font_index != NONE)
			{
				char string[256];
				rectangle2d bounds = global_window_parameters.camera.window_bounds;
				short window_width = global_window_parameters.camera.viewport_bounds.x1 -
					global_window_parameters.camera.viewport_bounds.x0;
				short window_height = global_window_parameters.camera.viewport_bounds.y1 -
					global_window_parameters.camera.viewport_bounds.y0;
				long window_area = window_width*window_height;

				sprintf(
					string,
					"%.02f",
					(real)rasterizer_xbox_transparent_geometry_globals.transparent_pixel_count/window_area);
				bounds.x0 = bounds.x1 - 160;
				bounds.y0 = bounds.y1 - 50;
				bounds.x1 -= 50;
				draw_string_set_format(
					NONE,
					1,
					0);
				draw_string_set_color(global_real_argb_yellow);
				draw_string_set_font(font_index);
				rasterizer_set_stencil_mode(0);
				rasterizer_draw_string(
					&bounds,
					NULL,
					NULL,
					0,
					string);
			}
		}

		if (!success)
			error(_error_silent, "### ERROR rasterizer_transparent_geometry_groups_begin failed");
	}

	return;
}

void rasterizer_transparent_geometry_dispose_aux_buffer(
	void)
{
	if (rasterizer_xbox_transparent_geometry_globals.texcoord_stream)
	{
		IDirect3DVertexBuffer8_Release(
			rasterizer_xbox_transparent_geometry_globals.texcoord_stream);
		rasterizer_xbox_transparent_geometry_globals.texcoord_stream = NULL;
	}

	return;
}

/* ---------- private code */

void rasterizer_transparent_geometry_group_draw__internal(
	struct transparent_geometry_group const *group,
	boolean has_lightmap)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
		109,
		group);

	if (group->triangle_buffer)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
			113,
			!has_lightmap);

		if (group->vertex_buffer)
		{
			rasterizer_draw_static_triangles_static_vertices(
				group->triangle_buffer,
				group->first_triangle_index,
				group->triangle_count,
				group->vertex_buffer);
		}
		else
		{
			rasterizer_draw_static_triangles_dynamic_vertices(
				group->triangle_buffer,
				group->first_triangle_index,
				group->triangle_count,
				group->dynamic_vertex_buffer_index);
		}
	}
	else if (group->vertex_buffer)
	{
		if (has_lightmap)
		{
			rasterizer_draw_dynamic_triangles_static_vertices2(
				group->dynamic_triangle_buffer_index,
				group->first_triangle_index,
				group->triangle_count,
				group->vertex_buffer,
				group->vertex_buffer+1);
		}
		else
		{
			rasterizer_draw_dynamic_triangles_static_vertices(
				group->dynamic_triangle_buffer_index,
				group->first_triangle_index,
				group->triangle_count,
				group->vertex_buffer);
		}
	}
	else
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
			156,
			!has_lightmap);

		if (group->dynamic_triangle_buffer_index >= 0)
		{
			rasterizer_draw_dynamic_triangles_dynamic_vertices(
				group->dynamic_triangle_buffer_index,
				group->first_triangle_index,
				group->triangle_count,
				group->dynamic_vertex_buffer_index);
		}
		else
		{
			short vertices_per_primitive = (short)-group->dynamic_triangle_buffer_index;
			short primitive_count;

			if (vertices_per_primitive == 3 || vertices_per_primitive == 4)
			{
				primitive_count = (short)(group->triangle_count/(vertices_per_primitive-2));
			}
			else
			{
				primitive_count = 1;
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					180,
					group->triangle_count==vertices_per_primitive-2);
			}

			rasterizer_draw_dynamic_vertices(
				0,
				primitive_count,
				group->dynamic_vertex_buffer_index,
				vertices_per_primitive);
		}
	}

	return;
}

real_vector4d *subtract_vectors4d(
	real_vector4d const *a,
	real_vector4d const *b,
	real_vector4d *result)
{
	result->i = a->i-b->i;
	result->j = a->j-b->j;
	result->k = a->k-b->k;
	result->l = a->l-b->l;

	return result;
}

real_vector4d *offset_vector4d(
	real_vector4d const *base,
	real_vector4d const *vector,
	real scale,
	real_vector4d *result)
{
	result->i = vector->i*scale + base->i;
	result->j = vector->j*scale + base->j;
	result->k = vector->k*scale + base->k;
	result->l = vector->l*scale + base->l;

	return result;
}

void rasterizer_transparent_geometry_group_draw(
	struct transparent_geometry_group *group,
	boolean dirty)
{
	boolean success = TRUE;
	boolean draw_active_camouflage_groups2 = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
		232,
		group);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
		233,
		global_d3d_device);

	if ((!group->active_camouflage_transparent_source_object_index || dirty) &&
		rasterizer_transparent_geometry_get_group_pending_status(group))
	{
		rasterizer_transparent_geometry_set_group_pending_status(group, FALSE);

		if (group->previous_group_presorted_index != NONE)
			rasterizer_transparent_geometry_group_draw(
				rasterizer_transparent_geometry_get_group_from_presorted_index(
					group->previous_group_presorted_index),
				dirty);

		if (rasterizer_debug_options.debug_transparent_geometry)
		{
			struct rasterizer_model_skinning_parameters skinning;

			if (!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit) &&
				group->shader &&
				group->sorted_index != NONE)
			{
				short vertex_shader_table[NUMBER_OF_RASTERIZER_VERTEX_TYPES] =
				{
					6, 6, 6, 6, 13, 13, 65, 65, NONE, NONE, NONE, NONE
				};
				short vertex_type =
					rasterizer_transparent_geometry_get_primary_vertex_type(group);
				boolean accumulate =
					rasterizer_debug_options.transparent_geometry_index >=
						RASTERIZER_TRANSPARENT_GEOMETRY_ALL_GROUPS_INDEX ||
					rasterizer_debug_options.transparent_geometry_index < 0;
				unsigned long seed;
				real_argb_color color;
				real minimum;
				real maximum;
				real scale;
				long component_index;

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					280,
					vertex_type>=0 && vertex_type<NUMBER_OF_RASTERIZER_VERTEX_TYPES);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					281,
					vertex_shader_table[vertex_type]!=NONE);

				rasterizer_set_vertex_shader_permutation(
					vertex_shader_table[vertex_type],
					vertex_type,
					0);

				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_CULLMODE,
					D3DCULL_NONE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHABLENDENABLE,
					accumulate);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_SRCBLEND,
					D3DBLEND_ONE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_DESTBLEND,
					D3DBLEND_ONE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_BLENDOP,
					D3DBLENDOP_ADD);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHATESTENABLE,
					FALSE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZENABLE,
					TRUE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZWRITEENABLE,
					FALSE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZFUNC,
					D3DCMP_LESSEQUAL);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZBIAS,
					0);

				csmemset(&pixel_shader, 0, sizeof(pixel_shader));

				seed = rasterizer_debug_options.transparent_geometry_index +
					group->sorted_index;
				color.alpha = 1.0f;
				color.red = real_seed_random(&seed);
				color.green = real_seed_random(&seed);
				color.blue = real_seed_random(&seed);

				minimum = MIN(color.red, MIN(color.green, color.blue));
				maximum = MAX(color.red, MAX(color.green, color.blue));
				scale = 0.18f/(maximum-minimum);

				for (component_index = 0;
					component_index < NUMBEROF(color.rgb.n);
					component_index++)
					color.rgb.n[component_index] = (color.rgb.n[component_index]-minimum)*scale + 0.15f;

				if (accumulate)
				{
					real intensity = PIN(
						rasterizer_debug_options.transparent_geometry_intensity, 0.0f, 1.0f);

					if (intensity == 0.0f)
						intensity = 0.03125f;

					if (rasterizer_debug_options.transparent_geometry_index >=
						RASTERIZER_TRANSPARENT_GEOMETRY_ALL_GROUPS_INDEX)
					{
						color.red *= intensity;
						color.green *= intensity;
						color.blue *= intensity;
					}
					else
					{
						color.red= color.green= color.blue= intensity;
					}
				}

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					342,
					color.red >=0.0f && color.red <=1.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					343,
					color.green>=0.0f && color.green<=1.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
					344,
					color.blue >=0.0f && color.blue <=1.0f);

				pixel_shader.final_combiner_constant_0 =
					real_rgb_color_to_pixel32(&color.rgb);
				pixel_shader.combiner_count = 1;
				pixel_shader.final_combiner_inputs_abcd = 1;
				rasterizer_set_pixel_shader(&pixel_shader);

				if (group->node_matrices && group->node_matrix_count)
				{
					skinning.node_matrix_count = group->node_matrix_count;
					skinning.node_matrices = group->node_matrices;
				}
				else
				{
					skinning.node_matrix_count = 1;
					skinning.node_matrices = global_identity4x3;
				}
				rasterizer_set_model_skinning(&skinning);

				{
					real camera_transform[3][4] =
					{
						{ 1.0f, 0.0f, 0.0f, 0.0f },
						{ 0.0f, 1.0f, 0.0f, 0.0f },
						{ 0.0f, 0.0f, 1.0f, 0.0f }
					};

					if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_viewspace_bit))
					{
						camera_transform[0][0] =
							global_window_parameters.frustum.view_to_world.forward.i;
						camera_transform[0][1] =
							global_window_parameters.frustum.view_to_world.left.i;
						camera_transform[0][2] =
							global_window_parameters.frustum.view_to_world.up.i;
						camera_transform[0][3] =
							global_window_parameters.camera.position.x;
						camera_transform[1][0] =
							global_window_parameters.frustum.view_to_world.forward.j;
						camera_transform[1][1] =
							global_window_parameters.frustum.view_to_world.left.j;
						camera_transform[1][2] =
							global_window_parameters.frustum.view_to_world.up.j;
						camera_transform[1][3] =
							global_window_parameters.camera.position.y;
						camera_transform[2][0] =
							global_window_parameters.frustum.view_to_world.forward.k;
						camera_transform[2][1] =
							global_window_parameters.frustum.view_to_world.left.k;
						camera_transform[2][2] =
							global_window_parameters.frustum.view_to_world.up.k;
						camera_transform[2][3] =
							global_window_parameters.camera.position.z;
				}

				IDirect3DDevice8_SetVertexShaderConstant(
					global_d3d_device,
					RASTERIZER_TRANSPARENT_GEOMETRY_CAMERA_CONSTANT_INDEX,
					camera_transform,
					3);
				}

				success = TRUE;
				rasterizer_transparent_geometry_group_draw__internal(group, FALSE);

				if (!accumulate)
				{
					char string[256];

					sprintf(string, "%.03f", group->z_sort);
					color.red = PIN(color.red*3.0f, 0.0f, 1.0f);
					color.green = PIN(color.red*3.0f, 0.0f, 1.0f);
					color.blue = PIN(color.red*3.0f, 0.0f, 1.0f);
					render_debug_string_at_point(
						FALSE,
						&group->centroid,
						string,
						&color);
				}
			}
		}
		else
		{
			short pass;

			if (group->effect_type == _render_model_effect_type_cortana)
			{
				long source_object_index = group->source_object_index;

				if (source_object_index != rasterizer_xbox_transparent_geometry_globals.last_source_object_index &&
					!dirty)
				{
					struct transparent_geometry_group *source_group = group;

					rasterizer_set_vertex_shader_permutation(
						13,
						rasterizer_transparent_geometry_get_primary_vertex_type(group),
						0);

					SetRenderStateSmart(D3DRS_CULLMODE, D3DCULL_NONE);
					SetRenderStateSmart(D3DRS_COLORWRITEENABLE, 0);
					SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, FALSE);
					SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
					SetRenderStateSmart(D3DRS_ZENABLE, TRUE);
					SetRenderStateSmart(D3DRS_ZWRITEENABLE, TRUE);
					SetRenderStateSmart(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZBIAS,
						0);

					csmemset(&pixel_shader, 0, sizeof(pixel_shader));
					pixel_shader.combiner_count = 1;
					rasterizer_set_pixel_shader(&pixel_shader);

					do
					{
						if (source_group->source_object_index != source_object_index ||
							source_group->effect_type != _render_model_effect_type_cortana)
							break;

						if (!shader_ignores_effect(source_group->shader))
						{
							struct rasterizer_model_skinning_parameters skinning;

							if (source_group->node_matrices && source_group->node_matrix_count)
							{
								skinning.node_matrix_count = source_group->node_matrix_count;
								skinning.node_matrices = source_group->node_matrices;
							}
							else
							{
								skinning.node_matrix_count = 1;
								skinning.node_matrices = global_identity4x3;
							}
							rasterizer_set_model_skinning(&skinning);

							if (group->lighting)
								rasterizer_set_model_lighting(source_group->lighting);

							rasterizer_transparent_geometry_group_draw__internal(
								source_group,
								FALSE);
						}
					}
					while (source_group = rasterizer_transparent_geometry_next_group(source_group));
				}
			}

			if (!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit) &&
				global_window_parameters.rasterizer_target == 0 &&
				!dirty)
			{
				if (rasterizer_debug_options.active_camouflage_multipass ?
					(group->shader &&
						group->shader->base.type == _shader_type_model &&
						group->effect_type == _render_model_effect_type_active_camouflage &&
						group->source_object_index !=
							rasterizer_xbox_transparent_geometry_globals.last_source_object_index) :
					(!group->shader ||
						(group->shader->base.type != _shader_type_transparent_water &&
							!shader_is_water_decal(group->shader))))
				{
					rasterizer_active_camouflage_cache_primary_render_target();
				}
			}

			if (!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit) &&
				global_window_parameters.rasterizer_target == 0 &&
				group->effect_type == _render_model_effect_type_active_camouflage &&
				group->shader &&
				group->shader->base.type == _shader_type_model &&
				!dirty)
			{
				struct transparent_geometry_group const *next_group =
					rasterizer_transparent_geometry_next_group(group);

				if (!next_group ||
					next_group->effect_type != _render_model_effect_type_active_camouflage ||
					next_group->source_object_index != group->source_object_index ||
					!next_group->shader ||
					next_group->shader->base.type != _shader_type_model)
					draw_active_camouflage_groups2 = TRUE;
			}

			if (!group->shader)
			{
				group->render_proc(
					group->first_triangle_index,
					group->triangle_count);
			}
			else
			{
				short shader_permutation_index =
					shader_get_vertex_shader_permutation(group->shader);
				short vertex_type =
					rasterizer_transparent_geometry_get_primary_vertex_type(group);

				if (!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit))
				{
					struct rasterizer_model_skinning_parameters skinning;

					if (group->node_matrices && group->node_matrix_count)
					{
						skinning.node_matrix_count = group->node_matrix_count;
						skinning.node_matrices = group->node_matrices;
					}
					else
					{
						skinning.node_matrix_count = 1;
						skinning.node_matrices = global_identity4x3;
					}
					rasterizer_set_model_skinning(&skinning);

					if (group->lighting)
						rasterizer_set_model_lighting(group->lighting);
				}

				if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_zbuffer_bit))
				{
					if (global_window_parameters.rasterizer_target == 0)
						rasterizer_set_frustum_z(0.00390625f, 1024.0f);

					SetRenderStateSmart(D3DRS_ZENABLE, FALSE);
					SetRenderStateSmart(D3DRS_ZBIAS, 0);
				}
				else
				{
					SetRenderStateSmart(D3DRS_ZENABLE, TRUE);
					SetRenderStateSmart(D3DRS_ZWRITEENABLE, FALSE);
					SetRenderStateSmart(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
					SetRenderStateSmart(
						D3DRS_ZBIAS,
						shader_is_decal(group->shader) ?
							rasterizer_debug_options.zbias :
							0);
				}

				for (pass = 0;
					pass < RASTERIZER_TRANSPARENT_GEOMETRY_MAXIMUM_PASSES;
					pass++)
				{
					if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
					{
						if (group->effect_type == _render_model_effect_type_active_camouflage)
						{
							if (pass > 0)
								break;

							rasterizer_set_frustum_z(
								rasterizer_globals.first_person_weapon_near_clip_distance,
								rasterizer_globals.first_person_weapon_far_clip_distance);
						}
						else if (!pass)
						{
							if (group->shader &&
								group->shader->base.type == _shader_type_effect &&
								TEST_FLAG(
									((struct shader_effect_definition *)
										shader_get_and_verify_type(
											group->shader,
											_shader_type_effect))->flags,
									_shader_effect_flags_dont_overdraw_first_person_weapon_bit))
								continue;

							rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_ACCEPT);
						}
						else
						{
							rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_REJECT);
							SetRenderStateSmart(D3DRS_ZENABLE, FALSE);
						}
					}
					else if (pass > 0)
					{
						break;
					}

					switch (group->shader->base.type)
					{
						case _shader_type_model:
							switch (group->effect_type)
							{
								case _render_model_effect_type_active_camouflage:
									if (rasterizer_xbox_transparent_geometry_globals.test_no_more_active_camo)
										return;

									rasterizer_active_camouflage_draw(group);
									break;

								default:
									match_vassert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										683,
										FALSE,
										"### ERROR unsupported model effect type in transparent group");
									break;
							}
							continue;

						case _shader_type_effect:
							{
								struct shader_effect_definition const *shader_effect =
									(struct shader_effect_definition *)
										shader_get_and_verify_type(
											group->shader,
											_shader_type_effect);
								boolean secondary_map_active =
									shader_effect->secondary_map.index != NONE &&
									shader_effect->secondary_map_anchor !=
										_shader_effect_secondary_map_anchor_zsprite;
								boolean uses_nonlinear_tint =
									TEST_FLAG(shader_effect->flags, _shader_effect_uses_nonlinear_tint_bit);
								real vsh_constants__inverse[3][4];
								real vsh_constants__texanim[4][4];
								short combiner_index;
								long result;

								/* stage 0 comes from the geometry group, not from the shader */
								rasterizer_set_texture_bitmap_data(0, group->lightmap);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSU,
									TEST_FLAG(shader_effect->primary_map_flags,
										_shader_effect_map_u_clamp_bit) ?
										D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSV,
									TEST_FLAG(shader_effect->primary_map_flags,
										_shader_effect_map_v_clamp_bit) ?
										D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_MAGFILTER,
									TEST_FLAG(shader_effect->primary_map_flags,
										_shader_effect_map_point_sampled_bit) ?
										D3DTEXF_POINT : D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MINFILTER,
									TEST_FLAG(shader_effect->primary_map_flags,
										_shader_effect_map_point_sampled_bit) ?
										D3DTEXF_POINT : D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MIPFILTER,
									TEST_FLAG(shader_effect->primary_map_flags,
										_shader_effect_map_point_sampled_bit) ?
										D3DTEXF_POINT : D3DTEXF_LINEAR);

								if (shader_effect->secondary_map.index != NONE)
								{
									rasterizer_set_texture(
										1,
										0,
										1,
										shader_effect->secondary_map.index,
										group->shader_permutation_index);
									SetTextureStageStateSmart(1, D3DTSS_ADDRESSU,
										TEST_FLAG(shader_effect->secondary_map_flags,
											_shader_effect_map_u_clamp_bit) ?
											D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
									SetTextureStageStateSmart(1, D3DTSS_ADDRESSV,
										TEST_FLAG(shader_effect->secondary_map_flags,
											_shader_effect_map_v_clamp_bit) ?
											D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP);
									SetTextureStageStateSmart(1, D3DTSS_MAGFILTER,
										TEST_FLAG(shader_effect->secondary_map_flags,
											_shader_effect_map_point_sampled_bit) ?
											D3DTEXF_POINT : D3DTEXF_LINEAR);
									SetTextureStageStateSmart(1, D3DTSS_MINFILTER,
										TEST_FLAG(shader_effect->secondary_map_flags,
											_shader_effect_map_point_sampled_bit) ?
											D3DTEXF_POINT : D3DTEXF_LINEAR);
									SetTextureStageStateSmart(1, D3DTSS_MIPFILTER,
										TEST_FLAG(shader_effect->secondary_map_flags,
											_shader_effect_map_point_sampled_bit) ?
											D3DTEXF_POINT : D3DTEXF_LINEAR);
								}

								SetRenderStateSmart(D3DRS_CULLMODE, D3DCULL_NONE);
								SetRenderStateSmart(
									D3DRS_COLORWRITEENABLE,
									D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
								SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
								SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
								rasterizer_set_framebuffer_blend_function(
									shader_effect->framebuffer_blend_function);
								rasterizer_set_vertex_shader_permutation(
									TRANSPARENT_GEOMETRY_VERTEX_SHADER_EFFECT,
									vertex_type,
									shader_permutation_index);

								vsh_constants__inverse[0][0] = 1.0f;
								vsh_constants__inverse[0][1] = 0.0f;
								vsh_constants__inverse[0][2] = 0.0f;
								vsh_constants__inverse[0][3] = 0.0f;
								vsh_constants__inverse[1][0] = 0.0f;
								vsh_constants__inverse[1][1] = 1.0f;
								vsh_constants__inverse[1][2] = 0.0f;
								vsh_constants__inverse[1][3] = 0.0f;
								vsh_constants__inverse[2][0] = 0.0f;
								vsh_constants__inverse[2][1] = 0.0f;
								vsh_constants__inverse[2][2] = 1.0f;
								vsh_constants__inverse[2][3] = 0.0f;
								vsh_constants__texanim[0][0] = 1.0f;
								vsh_constants__texanim[0][1] = 0.0f;
								vsh_constants__texanim[0][2] = 0.0f;
								vsh_constants__texanim[0][3] = 0.0f;
								vsh_constants__texanim[1][0] = 0.0f;
								vsh_constants__texanim[1][1] = 1.0f;
								vsh_constants__texanim[1][2] = 0.0f;
								vsh_constants__texanim[1][3] = 0.0f;
								vsh_constants__texanim[2][0] = 0.0f;
								vsh_constants__texanim[2][1] = 0.0f;
								vsh_constants__texanim[2][2] = 0.0f;
								vsh_constants__texanim[2][3] = 0.0f;
								vsh_constants__texanim[3][0] = 0.0f;
								vsh_constants__texanim[3][1] = 0.0f;
								vsh_constants__texanim[3][2] = 0.0f;
								vsh_constants__texanim[3][3] = 0.0f;

								if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_viewspace_bit))
								{
									vsh_constants__inverse[0][0] =
										global_window_parameters.frustum.view_to_world.forward.i;
									vsh_constants__inverse[0][1] =
										global_window_parameters.frustum.view_to_world.left.i;
									vsh_constants__inverse[0][2] =
										global_window_parameters.frustum.view_to_world.up.i;
									vsh_constants__inverse[0][3] =
										global_window_parameters.camera.position.x;
									vsh_constants__inverse[1][0] =
										global_window_parameters.frustum.view_to_world.forward.j;
									vsh_constants__inverse[1][1] =
										global_window_parameters.frustum.view_to_world.left.j;
									vsh_constants__inverse[1][2] =
										global_window_parameters.frustum.view_to_world.up.j;
									vsh_constants__inverse[1][3] =
										global_window_parameters.camera.position.y;
									vsh_constants__inverse[2][0] =
										global_window_parameters.frustum.view_to_world.forward.k;
									vsh_constants__inverse[2][1] =
										global_window_parameters.frustum.view_to_world.left.k;
									vsh_constants__inverse[2][2] =
										global_window_parameters.frustum.view_to_world.up.k;
									vsh_constants__inverse[2][3] =
										global_window_parameters.camera.position.z;
								}

								if (secondary_map_active)
								{
									shader_texture_animation_evaluate(
										&shader_effect->secondary_map_animation,
										group->animation,
										group->model_base_map_scale.i,
										group->model_base_map_scale.j,
										0.0f,
										0.0f,
										0.0f,
										global_frame_parameters.game_time_sec,
										(real_vector4d *)vsh_constants__texanim[2],
										(real_vector4d *)vsh_constants__texanim[3]);
								}

								result = IDirect3DDevice8_SetVertexShaderConstant(
									global_d3d_device,
									VSH_CONSTANTS__INVERSE_OFFSET,
									vsh_constants__inverse,
									VSH_CONSTANTS__INVERSE_COUNT);
								if (success && result >= 0)
								{
									success = TRUE;
								}
								else
								{
									success = FALSE;
									rasterizer_error(
										result,
										"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__INVERSE_OFFSET, vsh_constants__inverse, VSH_CONSTANTS__INVERSE_COUNT)");
								}

								result = IDirect3DDevice8_SetVertexShaderConstant(
									global_d3d_device,
									VSH_CONSTANTS__TEXANIM_OFFSET,
									vsh_constants__texanim,
									4);
								if (success && result >= 0)
								{
									success = TRUE;
								}
								else
								{
									success = FALSE;
									rasterizer_error(
										result,
										"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXANIM_OFFSET, vsh_constants__texanim, 4)");
								}

								if (rasterizer_debug_options.zsprites &&
									shader_effect->secondary_map_anchor ==
										_shader_effect_secondary_map_anchor_zsprite &&
									shader_effect->secondary_map.index != NONE &&
									!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
								{
									struct render_camera const *camera = &global_window_parameters.camera;
									real vsh_constants__zsprite[2][4];
									real radius_scale =
										shader_effect->secondary_map_zsprite_radius_scale != 0.0f ?
											shader_effect->secondary_map_zsprite_radius_scale : 1.0f;
									real depth_range =
										RASTERIZER_GLOBALS_FLOATING_POINT_ZBUFFER(rasterizer_globals) ?
										1.0e30f : 16777215.0f;
									real z_near = camera->z_near;
									real z_far = camera->z_far;

									vsh_constants__zsprite[0][0] = z_far*depth_range/(z_far - z_near);
									vsh_constants__zsprite[0][1] = -(vsh_constants__zsprite[0][0]*z_near);
									vsh_constants__zsprite[0][2] =
										radius_scale*shader_effect->secondary_map_radius;
									vsh_constants__zsprite[0][3] =
										camera->z_near + 0.01f;
									vsh_constants__zsprite[1][0] =
										camera->forward.i;
									vsh_constants__zsprite[1][1] =
										camera->forward.j;
									vsh_constants__zsprite[1][2] =
										camera->forward.k;
									vsh_constants__zsprite[1][3] = -dot_product3d(
										&camera->forward,
										(real_vector3d *)&camera->position);

									result = IDirect3DDevice8_SetVertexShaderConstant(
										global_d3d_device,
										VSH_CONSTANTS__ZSPRITE_OFFSET,
										vsh_constants__zsprite,
										VSH_CONSTANTS__ZSPRITE_COUNT);
									if (success && result >= 0)
									{
										success = TRUE;
									}
									else
									{
										success = FALSE;
										rasterizer_error(
											result,
											"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__ZSPRITE_OFFSET, vsh_constants__zsprite, VSH_CONSTANTS__ZSPRITE_COUNT)");
									}
								}

								csmemset(&pixel_shader, 0, sizeof(pixel_shader));
								pixel_shader.alpha_inputs[0] = 0x18201415;
								pixel_shader.alpha_outputs[0] = 0x000000C4;
								pixel_shader.final_combiner_inputs_abcd = 0x0000000C;
								pixel_shader.final_combiner_inputs_efg = 0x00001C00;
								if (uses_nonlinear_tint)
								{
									pixel_shader.rgb_inputs[0] = 0x08080000;
									pixel_shader.rgb_outputs[0] = 0x000000C0;
									pixel_shader.rgb_inputs[1] = 0x0C0C0000;
									pixel_shader.rgb_outputs[1] = 0x000000C0;
									pixel_shader.rgb_inputs[2] = 0x250C0508;
									pixel_shader.rgb_outputs[2] = 0x00000C00;
									combiner_index = 3;
								}
								else
								{
									pixel_shader.rgb_inputs[0] = 0x08050000;
									pixel_shader.rgb_outputs[0] = 0x000000C0;
									combiner_index = 1;
								}

								if (secondary_map_active)
								{
									pixel_shader.alpha_inputs[combiner_index] = 0x1C190000;
									pixel_shader.alpha_outputs[combiner_index] = 0x000000C0;
									pixel_shader.rgb_inputs[combiner_index] = 0x0C090000;
									pixel_shader.rgb_outputs[combiner_index] = 0x000000C0;
									combiner_index++;
								}

								if (rasterizer_debug_options.zsprites &&
									shader_effect->secondary_map_anchor ==
										_shader_effect_secondary_map_anchor_zsprite &&
									shader_effect->secondary_map.index != NONE &&
									!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
								{
									pixel_shader.texture_modes = 0x00054421;
									pixel_shader.input_texture = 0x00110000;
									pixel_shader.dot_mapping = 0;
									SetTextureStageStateSmart(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
									SetTextureStageStateSmart(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
									SetTextureStageStateSmart(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
									result = IDirect3DDevice8_SetStreamSource(
										global_d3d_device,
										1,
										rasterizer_xbox_transparent_geometry_globals.texcoord_stream,
										2*sizeof(byte));
									if (success && result >= 0)
									{
										success = TRUE;
									}
									else
									{
										success = FALSE;
										rasterizer_error(
											result,
											"IDirect3DDevice8_SetStreamSource(global_d3d_device, 1, rasterizer_xbox_transparent_geometry_texcoord_stream, 2*sizeof(byte))");
									}
									rasterizer_set_texture(
										1,
										0,
										0,
										shader_effect->secondary_map.index,
										group->shader_permutation_index);
								}
								else
								{
									pixel_shader.texture_modes = PS_TEXTUREMODES(
										PS_TEXTUREMODES_PROJECT2D,
										secondary_map_active ? PS_TEXTUREMODES_PROJECT2D : PS_TEXTUREMODES_NONE,
										PS_TEXTUREMODES_NONE,
										PS_TEXTUREMODES_NONE);
								}

								if (rasterizer_debug_options.draw_environment_fog &&
									!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_fog_bit))
								{
									switch (shader_effect->framebuffer_blend_function)
									{
										case _framebuffer_blend_function_alpha_blend:
											pixel_shader.alpha_inputs[combiner_index] = 0x1C140000;
											pixel_shader.alpha_outputs[combiner_index] = 0x00000C00;
											break;

										case _framebuffer_blend_function_multiply:
										case _framebuffer_blend_function_component_min:
											pixel_shader.rgb_inputs[combiner_index] = 0x0C142034;
											pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
											break;

										case _framebuffer_blend_function_double_multiply:
											pixel_shader.rgb_inputs[combiner_index] = 0x0C14A034;
											pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
											break;

										case _framebuffer_blend_function_add:
										case _framebuffer_blend_function_subtract:
										case _framebuffer_blend_function_component_max:
											pixel_shader.rgb_inputs[combiner_index] = 0x0C140000;
											pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
											break;

										case _framebuffer_blend_function_alpha_multiply_add:
											pixel_shader.alpha_inputs[combiner_index] = 0x1C140000;
											pixel_shader.alpha_outputs[combiner_index] = 0x00000C00;
											pixel_shader.rgb_inputs[combiner_index] = 0x0C140000;
											pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
											break;

										default:
											match_vassert(
												"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
												930,
												FALSE,
												"### ERROR unsupported framebuffer blend function");
											break;
									}
									combiner_index++;
								}
								pixel_shader.combiner_count = combiner_index;

							}
							break;

						case _shader_type_transparent_generic:
						{
							struct shader_transparent_generic_definition const *shader_transparent_generic =
								(struct shader_transparent_generic_definition *)
									shader_get_and_verify_type(
										group->shader,
										_shader_type_transparent_generic);
							short bitmap_sequence_index = group->shader_permutation_index;
							real vsh_constants__texanim[8][4];
							short layer_index;
							short map_index;
							short stage_index;
							long result;

							for (layer_index = 0;
								layer_index < shader_transparent_generic->generic.extra_layers.count;
								layer_index++)
							{
								struct transparent_geometry_group layer_group;

								csmemcpy(&layer_group, group, sizeof(layer_group));
								layer_group.sorted_index = NONE;
								layer_group.shader = shader_definition_get(
									TAG_BLOCK_GET_ELEMENT(
										&shader_transparent_generic->generic.extra_layers,
										layer_index,
										struct tag_reference)->index);
								rasterizer_transparent_geometry_group_draw(&layer_group, dirty);
							}

							rasterizer_set_vertex_shader_permutation(
								TRANSPARENT_GEOMETRY_VERTEX_SHADER_GENERIC,
								vertex_type,
								shader_permutation_index);

							SetRenderStateSmart(
								D3DRS_CULLMODE,
								TEST_FLAG(shader_transparent_generic->generic.flags, _shader_transparent_flag_two_sided_bit) ?
									D3DCULL_NONE : D3DCULL_CCW);
							SetRenderStateSmart(
								D3DRS_COLORWRITEENABLE,
								D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
							SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
							SetRenderStateSmart(
								D3DRS_ALPHATESTENABLE,
								TEST_FLAG(shader_transparent_generic->generic.flags, _shader_transparent_flag_alpha_tested_bit));
							SetRenderStateSmart(D3DRS_ALPHAREF, 0x7F);
							rasterizer_set_framebuffer_blend_function(
								shader_transparent_generic->generic.framebuffer_blend_function);

							if (TEST_FLAG(shader_transparent_generic->generic.flags, _shader_transparent_flag_numeric_bit) &&
								group->animation &&
								shader_transparent_generic->generic.maps.count > 0)
							{
								struct bitmap_group const *bitmap_group =
									bitmap_group_get(TAG_BLOCK_GET_ELEMENT(
										&shader_transparent_generic->generic.maps,
										0,
										struct shader_transparent_generic_map)->map.index);
								short frame_count = (short)bitmap_group->bitmaps.count;
								short function_index = frame_count == 8 ? 3 : 0;
								short counter_limit = shader_transparent_generic->generic.numeric_counter_limit;
								short counter_value = PIN(fast_ftol(floor(counter_limit*
									group->animation->values[function_index] + 0.5f)), 0, counter_limit);
								short digit_index;

								for (digit_index = 0;
									digit_index < group->shader_permutation_index;
									digit_index++)
									counter_value /= frame_count;

								bitmap_sequence_index = counter_value%frame_count;
							}

							if (shader_transparent_generic->generic.maps.count > 0)
							{
								for (map_index = 0;
									map_index < NUMBER_OF_SHADER_TRANSPARENT_MAPS;
									map_index++)
								{
									if (map_index < shader_transparent_generic->generic.maps.count)
									{
										struct shader_transparent_generic_map const *map =
											TAG_BLOCK_GET_ELEMENT(
												&shader_transparent_generic->generic.maps,
												map_index,
												struct shader_transparent_generic_map);
										short type = shader_transparent_generic->generic.type;
										short map_type_bitmap_type[NUMBER_OF_SHADER_TRANSPARENT_MAPS] =
										{
											0, 2, 2, 2
										};
										unsigned long map_type_address_mode[NUMBER_OF_SHADER_TRANSPARENT_MAPS] =
										{
											D3DTADDRESS_WRAP,
											D3DTADDRESS_CLAMP,
											D3DTADDRESS_CLAMP,
											D3DTADDRESS_CLAMP
										};
										short bitmap_type = map_index ? 0 :
											map_type_bitmap_type[type];
										unsigned long address_u;
										unsigned long address_v;

										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
											1030,
											!TEST_FLAG(shader_transparent_generic->shader.radiosity.flags,
												_shader_radiosity_FILTHY_transparent_lit_bit) ||
												shader_transparent_generic->generic.type==_shader_transparent_generic_type_2d_map);
										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
											1031,
											type>=0 && type<NUMBER_OF_SHADER_TRANSPARENT_GENERIC_TYPES);

										rasterizer_set_texture(
											map_index,
											bitmap_type,
											0,
											map->map.index,
											bitmap_sequence_index);

										if (!bitmap_type &&
											TEST_FLAG(map->flags, _shader_transparent_map_flag_u_clamped_bit))
											address_u = D3DTADDRESS_CLAMP;
										else if (map_index)
											address_u = D3DTADDRESS_WRAP;
										else
											address_u = map_type_address_mode[type];

										if (!bitmap_type &&
											TEST_FLAG(map->flags, _shader_transparent_map_flag_v_clamped_bit))
											address_v = D3DTADDRESS_CLAMP;
										else if (map_index)
											address_v = D3DTADDRESS_WRAP;
										else
											address_v = map_type_address_mode[type];

										SetTextureStageStateSmart(map_index, D3DTSS_ADDRESSU, address_u);
										SetTextureStageStateSmart(map_index, D3DTSS_ADDRESSV, address_v);
										SetTextureStageStateSmart(
											map_index,
											D3DTSS_ADDRESSW,
											map_index ? D3DTADDRESS_WRAP : map_type_address_mode[type]);
										IDirect3DDevice8_SetTextureStageState(
											global_d3d_device,
											map_index,
											D3DTSS_MAGFILTER,
											D3DTEXF_LINEAR);
										SetTextureStageStateSmart(
											map_index,
											D3DTSS_MINFILTER,
											TEST_FLAG(map->flags, _shader_transparent_map_flag_unfiltered_bit) ?
												D3DTEXF_POINT : D3DTEXF_LINEAR);
										SetTextureStageStateSmart(
											map_index,
											D3DTSS_MIPFILTER,
											TEST_FLAG(map->flags, _shader_transparent_map_flag_unfiltered_bit) ?
												D3DTEXF_POINT : D3DTEXF_LINEAR);
									}

									if (map_index < shader_transparent_generic->generic.maps.count &&
										(map_index>0 || shader_transparent_generic->generic.type==_shader_transparent_generic_type_2d_map))
									{
										struct shader_transparent_generic_map const *map =
											TAG_BLOCK_GET_ELEMENT(
												&shader_transparent_generic->generic.maps,
												map_index,
												struct shader_transparent_generic_map);
										real_vector2d map_scale;

										map_scale.i = map->map_u_scale;
										map_scale.j = map->map_v_scale;

										if (!map_index &&
											TEST_FLAG(shader_transparent_generic->generic.flags,
												_shader_transparent_flag_scale_first_map_with_distance_bit))
										{
											map_scale.i = -(map_scale.i*group->z_sort);
											map_scale.j = -(map_scale.j*group->z_sort);
										}

										if (map_index>0 ||
											!TEST_FLAG(shader_transparent_generic->generic.flags,
												_shader_transparent_flag_first_map_is_in_screenspace_bit))
										{
											map_scale.i *= group->model_base_map_scale.i;
											map_scale.j *= group->model_base_map_scale.j;
										}

										shader_texture_animation_evaluate(
											&map->map_animation,
											group->animation,
											map_scale.i,
											map_scale.j,
											map->map_u_offset,
											map->map_v_offset,
											map->map_rotation,
											global_frame_parameters.game_time_sec,
											(real_vector4d *)vsh_constants__texanim[map_index*2],
											(real_vector4d *)vsh_constants__texanim[map_index*2+1]);
									}
									else if (map_index < shader_transparent_generic->generic.maps.count &&
										TEST_FLAG(shader_transparent_generic->generic.flags,
											_shader_transparent_flag_first_map_is_in_screenspace_bit))
									{
										vsh_constants__texanim[map_index*2][0] =
											global_window_parameters.frustum.view_to_world.forward.i;
										vsh_constants__texanim[map_index*2][1] =
											global_window_parameters.frustum.view_to_world.forward.j;
										vsh_constants__texanim[map_index*2][2] =
											global_window_parameters.frustum.view_to_world.forward.k;
										vsh_constants__texanim[map_index*2+1][0] =
											global_window_parameters.frustum.view_to_world.left.i;
										vsh_constants__texanim[map_index*2+1][1] =
											global_window_parameters.frustum.view_to_world.left.j;
										vsh_constants__texanim[map_index*2+1][2] =
											global_window_parameters.frustum.view_to_world.left.k;
										vsh_constants__texanim[map_index*2][3] = 0.0f;
										vsh_constants__texanim[map_index*2+1][3] = 0.0f;
									}
									else
									{
										vsh_constants__texanim[map_index*2][0] = 1.0f;
										vsh_constants__texanim[map_index*2][1] = 0.0f;
										vsh_constants__texanim[map_index*2][2] = 0.0f;
										vsh_constants__texanim[map_index*2+1][0] = 0.0f;
										vsh_constants__texanim[map_index*2+1][1] = 1.0f;
										vsh_constants__texanim[map_index*2+1][2] = 0.0f;
										vsh_constants__texanim[map_index*2][3] = 0.0f;
										vsh_constants__texanim[map_index*2+1][3] = 0.0f;
									}
							}

							result = IDirect3DDevice8_SetVertexShaderConstant(
								global_d3d_device,
								VSH_CONSTANTS__TEXANIM_OFFSET,
								vsh_constants__texanim,
								VSH_CONSTANTS__TEXANIM_COUNT);
							if (success && result >= 0)
							{
								success = TRUE;
							}
							else
							{
								success = FALSE;
								rasterizer_error(
									result,
									"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXANIM_OFFSET, vsh_constants__texanim, VSH_CONSTANTS__TEXANIM_COUNT)");
							}
							}

							success = success && shader_transparent_generic_create(
								group->shader,
								&pixel_shader);

							if (rasterizer_debug_options.draw_environment_fog)
							{
								short stage_count = FLOOR(shader_transparent_generic->generic.stages.count, 1);

								if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_sky_bit) &&
									shader_transparent_generic->generic.framebuffer_blend_function ==
										_framebuffer_blend_function_alpha_blend)
								{
									struct render_fog const *fog = &global_window_parameters.fog;
									struct render_camera const *camera = &global_window_parameters.camera;
									real plane_distance = plane3d_distance_to_point(
										&fog->plane,
										&camera->position);
									real planar_fog_density = PIN(
										-(plane_distance/fog->planar_maximum_depth), 0.0f, 1.0f)*
										fog->planar_maximum_density;

									pixel_shader.constant_0[stage_count] = real_a_rgb_color_to_pixel32(
										planar_fog_density,
										&fog->planar_color);
									pixel_shader.rgb_inputs[stage_count] = 0x310C1101;
									pixel_shader.rgb_outputs[stage_count] = 0xC00;
								}
								else
								{
									real vsh_constants__texscale[3][4];
									short fade_source = shader_transparent_generic->generic.framebuffer_fade_source;
									unsigned long combiner_constant;

									vsh_constants__texscale[0][0] = 0.0f;
									vsh_constants__texscale[0][1] = 0.0f;
									vsh_constants__texscale[0][2] = 0.0f;
									vsh_constants__texscale[0][3] = 0.0f;
									vsh_constants__texscale[1][0] = 0.0f;
									vsh_constants__texscale[1][1] = 0.0f;
									vsh_constants__texscale[1][2] = 0.0f;
									vsh_constants__texscale[1][3] = 0.0f;
									vsh_constants__texscale[2][0] = 0.0f;
									vsh_constants__texscale[2][1] = 0.0f;
									vsh_constants__texscale[2][2] = 1.0f;
									vsh_constants__texscale[2][3] = 0.0f;

									if (group->effect_type ==
										_render_model_effect_type_active_camouflage)
										vsh_constants__texscale[2][2] *=
											PIN(1.0f-group->effect_intensity, 0.0f, 1.0f);

									if (fade_source > 0 &&
										group->animation &&
										group->animation->values)
										vsh_constants__texscale[2][2] *=
											group->animation->values[fade_source-1];

									result = IDirect3DDevice8_SetVertexShaderConstant(
										global_d3d_device,
										VSH_CONSTANTS__TEXSCALE_OFFSET,
										vsh_constants__texscale,
										VSH_CONSTANTS__TEXSCALE_COUNT);
									if (success && result >= 0)
									{
										success = TRUE;
									}
									else
									{
										success = FALSE;
										rasterizer_error(
											result,
											"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
									}

									switch (shader_transparent_generic->generic.framebuffer_fade_mode)
									{
										case _framebuffer_fade_mode_none:
											combiner_constant = 0x14;
											break;

										case _framebuffer_fade_mode_fade_when_perpendicular:
											combiner_constant = 0x15;
											break;

										case _framebuffer_fade_mode_fade_when_parallel:
											combiner_constant = 0x05;
											break;

										/* combiner_constant is left unassigned only by this default arm. Not reached unassigned: the
										 * arm's assertion failure calls system_exit, which does not return in January
										 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
										 * Source-policy approval pending (2026-09-27 audit). */
										default:
											match_vassert(
												"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
												1185,
												FALSE,
												"### ERROR unsupported framebuffer fade mode");
											break;
									}

									switch (shader_transparent_generic->generic.framebuffer_blend_function)
									{
										case _framebuffer_blend_function_alpha_blend:
											pixel_shader.alpha_inputs[stage_count] =
												(combiner_constant|0x1C00)<<16;
											pixel_shader.alpha_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_multiply:
										case _framebuffer_blend_function_component_min:
											pixel_shader.rgb_inputs[stage_count] =
												((combiner_constant^0x20)|(combiner_constant<<16))|0x0C002000;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_double_multiply:
											pixel_shader.rgb_inputs[stage_count] =
												((combiner_constant^0x20)|(combiner_constant<<16))|0x0C00A000;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_add:
										case _framebuffer_blend_function_subtract:
										case _framebuffer_blend_function_component_max:
											pixel_shader.rgb_inputs[stage_count] =
												(combiner_constant|0x0C00)<<16;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_alpha_multiply_add:
											pixel_shader.alpha_inputs[stage_count] =
												(combiner_constant|0x1C00)<<16;
											pixel_shader.alpha_outputs[stage_count] = 0xC00;
											pixel_shader.rgb_inputs[stage_count] =
												(combiner_constant|0x0C00)<<16;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										default:
											match_vassert(
												"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
												1216,
												FALSE,
												"### ERROR unsupported framebuffer blend function");
											break;
									}
								}
							}

							for (stage_index = 0;
								stage_index < shader_transparent_generic->generic.stages.count;
								stage_index++)
							{
								struct shader_transparent_generic_stage const *stage =
									TAG_BLOCK_GET_ELEMENT(
										&shader_transparent_generic->generic.stages,
										stage_index,
										struct shader_transparent_generic_stage);
								real_argb_color constant_color0;
								real_argb_color color_delta;
								real function_value;
								long component_index;

								match_assert(
									"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
									1235,
									stage->constant_color0_animation_period!=0.0f);

								if (group->animation &&
									TEST_FLAG(stage->flags,
										_shader_transparent_generic_stage_flag_a_out_controls_color0_animation_bit))
									function_value = group->animation->values[0];
								else
									function_value = periodic_function_evaluate(
										stage->constant_color0_animation_function,
										global_frame_parameters.game_time_sec/
											stage->constant_color0_animation_period);

								for (component_index = 0;
									component_index < NUMBEROF(color_delta.n);
									component_index++)
									color_delta.n[component_index] = stage->constant_color1.n[component_index]-stage->constant_color0.n[component_index];
								for (component_index = 0;
									component_index < NUMBEROF(constant_color0.n);
									component_index++)
									constant_color0.n[component_index] = function_value*color_delta.n[component_index] + stage->constant_color0.n[component_index];

								match_assert(
									"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
									1256,
									constant_color0.red >=0.0f && constant_color0.red <=1.0f);
								match_assert(
									"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
									1257,
									constant_color0.green>=0.0f && constant_color0.green<=1.0f);
								match_assert(
									"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
									1258,
									constant_color0.blue >=0.0f && constant_color0.blue <=1.0f);

								if (stage->constant_color0_animation_source > 0 &&
									stage->constant_color0_animation_source <
										NUMBER_OF_SHADER_ANIMATION_SOURCES &&
									group->animation &&
									group->animation->colors)
								{
									real_rgb_color const *external_color =
										&group->animation->colors[
											stage->constant_color0_animation_source-1];

									match_assert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										1269,
										external_color->red >=0.0f && external_color->red <=1.0f);
									match_assert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										1270,
										external_color->green>=0.0f && external_color->green<=1.0f);
									match_assert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										1271,
										external_color->blue >=0.0f && external_color->blue <=1.0f);

									for (component_index = 0;
										component_index < NUMBEROF(external_color->n);
										component_index++)
										constant_color0.rgb.n[component_index] *= external_color->n[component_index];
								}

								pixel_shader.constant_0[stage_index] =
									real_argb_color_to_pixel32(&constant_color0);
							}
							break;
						}

						case _shader_type_transparent_chicago:
						{
							struct shader_transparent_chicago_definition const *shader_transparent_chicago =
								(struct shader_transparent_chicago_definition *)
									shader_get_and_verify_type(
										group->shader,
										_shader_type_transparent_chicago);
							short bitmap_sequence_index = group->shader_permutation_index;
							real vsh_constants__texanim[8][4];
							short layer_index;
							short map_index;
							long result;

							/* BUG (preserved for exact matching): January never advances layer_index, so the
							 * loop redraws extra layer 0 for as long as the block is non-empty (the bytes push
							 * index 0 and re-test the count). A corrected build should increment layer_index.
							 */
							for (layer_index = 0;
								layer_index < shader_transparent_chicago->chicago.extra_layers.count;
								)
							{
								struct transparent_geometry_group layer_group;

								csmemcpy(&layer_group, group, sizeof(layer_group));
								layer_group.sorted_index = NONE;
								layer_group.shader = shader_definition_get(
									TAG_BLOCK_GET_ELEMENT(
										&shader_transparent_chicago->chicago.extra_layers,
										layer_index,
										struct tag_reference)->index);
								rasterizer_transparent_geometry_group_draw(&layer_group, dirty);
							}

							rasterizer_set_vertex_shader_permutation(
								TRANSPARENT_GEOMETRY_VERTEX_SHADER_GENERIC,
								vertex_type,
								shader_permutation_index);

							SetRenderStateSmart(
								D3DRS_CULLMODE,
								TEST_FLAG(shader_transparent_chicago->chicago.flags, _shader_transparent_flag_two_sided_bit) ?
									D3DCULL_NONE : D3DCULL_CCW);
							SetRenderStateSmart(
								D3DRS_COLORWRITEENABLE,
								D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
							SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
							SetRenderStateSmart(
								D3DRS_ALPHATESTENABLE,
								TEST_FLAG(shader_transparent_chicago->chicago.flags, _shader_transparent_flag_alpha_tested_bit));
							SetRenderStateSmart(D3DRS_ALPHAREF, 0x7F);
							rasterizer_set_framebuffer_blend_function(
								shader_transparent_chicago->chicago.framebuffer_blend_function);

							if (TEST_FLAG(shader_transparent_chicago->chicago.flags, _shader_transparent_flag_numeric_bit) &&
								group->animation &&
								shader_transparent_chicago->chicago.maps.count > 0)
							{
								struct shader_transparent_chicago_map const *map =
									TAG_BLOCK_GET_ELEMENT(
										&shader_transparent_chicago->chicago.maps,
										0,
										struct shader_transparent_chicago_map);
								struct bitmap_group const *bitmap_group =
									bitmap_group_get(map->map.index);
								short frame_count = (short)bitmap_group->bitmaps.count;

								if (TEST_FLAG(shader_transparent_chicago->chicago.extra_flags,
									_shader_transparent_chicago_extra_flag_numeric_countdown_timer_bit))
								{
									bitmap_sequence_index = numeric_countdown_timer_get(
										group->shader_permutation_index);
								}
								else
								{
									short function_index = frame_count == 8 ? 3 : 0;
									short counter_limit = shader_transparent_chicago->chicago.numeric_counter_limit;
									short counter_value = PIN(fast_ftol(floor(counter_limit*
										group->animation->values[function_index] + 0.5f)), 0, counter_limit);
									short digit_index;

									for (digit_index = 0;
										digit_index < group->shader_permutation_index;
										digit_index++)
										counter_value /= frame_count;

									bitmap_sequence_index = counter_value%frame_count;
								}
							}

							for (map_index = 0;
								map_index < NUMBER_OF_SHADER_TRANSPARENT_MAPS;
								map_index++)
							{
								if (map_index < shader_transparent_chicago->chicago.maps.count)
								{
									struct shader_transparent_chicago_map const *map =
										TAG_BLOCK_GET_ELEMENT(
											&shader_transparent_chicago->chicago.maps,
											map_index,
											struct shader_transparent_chicago_map);
									short type = shader_transparent_chicago->chicago.type;
									short map_type_bitmap_type[NUMBER_OF_SHADER_TRANSPARENT_MAPS] =
									{
										0, 2, 2, 2
									};
									unsigned long map_type_address_mode[NUMBER_OF_SHADER_TRANSPARENT_MAPS] =
									{
										D3DTADDRESS_WRAP,
										D3DTADDRESS_CLAMP,
										D3DTADDRESS_CLAMP,
										D3DTADDRESS_CLAMP
									};
									short bitmap_type = map_index ? 0 :
										map_type_bitmap_type[type];
									unsigned long address_u;
									unsigned long address_v;

									match_assert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										1383,
										!TEST_FLAG(shader_transparent_chicago->shader.radiosity.flags,
											_shader_radiosity_FILTHY_transparent_lit_bit) ||
											shader_transparent_chicago->chicago.type==_shader_transparent_chicago_type_2d_map);
									match_assert(
										"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
										1384,
										type>=0 && type<NUMBER_OF_SHADER_TRANSPARENT_CHICAGO_TYPES);

									rasterizer_set_texture(
										map_index,
										bitmap_type,
										0,
										map->map.index,
										bitmap_sequence_index);

									if (!bitmap_type &&
										TEST_FLAG(map->flags,
											_shader_transparent_chicago_map_flag_u_clamped_bit))
										address_u = D3DTADDRESS_CLAMP;
									else if (map_index)
										address_u = D3DTADDRESS_WRAP;
									else
										address_u = map_type_address_mode[type];

									if (!bitmap_type &&
										TEST_FLAG(map->flags,
											_shader_transparent_chicago_map_flag_v_clamped_bit))
										address_v = D3DTADDRESS_CLAMP;
									else if (map_index)
										address_v = D3DTADDRESS_WRAP;
									else
										address_v = map_type_address_mode[type];

									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_ADDRESSU,
										address_u);
									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_ADDRESSV,
										address_v);
									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_ADDRESSW,
										map_index ? D3DTADDRESS_WRAP : map_type_address_mode[type]);
									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_MAGFILTER,
										D3DTEXF_LINEAR);
									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_MINFILTER,
										TEST_FLAG(map->flags, _shader_transparent_map_flag_unfiltered_bit) ?
											D3DTEXF_POINT : D3DTEXF_LINEAR);
									IDirect3DDevice8_SetTextureStageState(
										global_d3d_device,
										map_index,
										D3DTSS_MIPFILTER,
										TEST_FLAG(map->flags, _shader_transparent_map_flag_unfiltered_bit) ?
											D3DTEXF_POINT : D3DTEXF_LINEAR);
								}

								if (map_index < shader_transparent_chicago->chicago.maps.count &&
									(map_index>0 || shader_transparent_chicago->chicago.type==_shader_transparent_chicago_type_2d_map))
								{
									struct shader_transparent_chicago_map const *map =
										TAG_BLOCK_GET_ELEMENT(
											&shader_transparent_chicago->chicago.maps,
											map_index,
											struct shader_transparent_chicago_map);
									real_vector2d map_scale;

									map_scale.i = map->map_u_scale;
									map_scale.j = map->map_v_scale;

									if (!map_index &&
										TEST_FLAG(shader_transparent_chicago->chicago.flags,
											_shader_transparent_flag_scale_first_map_with_distance_bit))
									{
										map_scale.i = -(map_scale.i*group->z_sort);
										map_scale.j = -(map_scale.j*group->z_sort);
									}

									if (map_index>0 ||
										!TEST_FLAG(shader_transparent_chicago->chicago.flags,
											_shader_transparent_flag_first_map_is_in_screenspace_bit))
									{
										map_scale.i *= group->model_base_map_scale.i;
										map_scale.j *= group->model_base_map_scale.j;
									}

									shader_texture_animation_evaluate(
										&map->map_animation,
										group->animation,
										map_scale.i,
										map_scale.j,
										map->map_u_offset,
										map->map_v_offset,
										map->map_rotation,
										global_frame_parameters.game_time_sec,
										(real_vector4d *)vsh_constants__texanim[map_index*2],
										(real_vector4d *)vsh_constants__texanim[map_index*2+1]);
								}
								else if (map_index < shader_transparent_chicago->chicago.maps.count &&
									TEST_FLAG(shader_transparent_chicago->chicago.flags,
										_shader_transparent_flag_first_map_is_in_screenspace_bit))
								{
									vsh_constants__texanim[map_index*2][0] =
										global_window_parameters.frustum.view_to_world.forward.i;
									vsh_constants__texanim[map_index*2][1] =
										global_window_parameters.frustum.view_to_world.forward.j;
									vsh_constants__texanim[map_index*2][2] =
										global_window_parameters.frustum.view_to_world.forward.k;
									vsh_constants__texanim[map_index*2+1][0] =
										global_window_parameters.frustum.view_to_world.left.i;
									vsh_constants__texanim[map_index*2+1][1] =
										global_window_parameters.frustum.view_to_world.left.j;
									vsh_constants__texanim[map_index*2+1][2] =
										global_window_parameters.frustum.view_to_world.left.k;
									vsh_constants__texanim[map_index*2][3] = 0.0f;
									vsh_constants__texanim[map_index*2+1][3] = 0.0f;
								}
								else
								{
									vsh_constants__texanim[map_index*2][0] = 1.0f;
									vsh_constants__texanim[map_index*2][1] = 0.0f;
									vsh_constants__texanim[map_index*2][2] = 0.0f;
									vsh_constants__texanim[map_index*2+1][0] = 0.0f;
									vsh_constants__texanim[map_index*2+1][1] = 1.0f;
									vsh_constants__texanim[map_index*2+1][2] = 0.0f;
									vsh_constants__texanim[map_index*2][3] = 0.0f;
									vsh_constants__texanim[map_index*2+1][3] = 0.0f;
								}
							}

							result = IDirect3DDevice8_SetVertexShaderConstant(
								global_d3d_device,
								VSH_CONSTANTS__TEXANIM_OFFSET,
								vsh_constants__texanim,
								VSH_CONSTANTS__TEXANIM_COUNT);
							if (success && result >= 0)
							{
								success = TRUE;
							}
							else
							{
								success = FALSE;
								rasterizer_error(
									result,
									"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXANIM_OFFSET, vsh_constants__texanim, VSH_CONSTANTS__TEXANIM_COUNT)");
							}

							success = success && shader_transparent_chicago_create(
								group->shader,
								&pixel_shader);

							if (rasterizer_debug_options.draw_environment_fog)
							{
								short stage_count = shader_transparent_chicago->chicago.maps.count;

								if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_sky_bit) &&
									shader_transparent_chicago->chicago.framebuffer_blend_function ==
										_framebuffer_blend_function_alpha_blend)
								{
									struct render_fog const *fog = &global_window_parameters.fog;
									struct render_camera const *camera = &global_window_parameters.camera;
									real plane_distance = plane3d_distance_to_point(
										&fog->plane,
										&camera->position);
									real planar_fog_density = PIN(
										-(plane_distance/fog->planar_maximum_depth), 0.0f, 1.0f)*
										fog->planar_maximum_density;

									pixel_shader.constant_0[stage_count] = real_a_rgb_color_to_pixel32(
										planar_fog_density,
										&fog->planar_color);
									pixel_shader.rgb_inputs[stage_count] = 0x310C1101;
									pixel_shader.rgb_outputs[stage_count] = 0xC00;
								}
								else
								{
									real vsh_constants__texscale[3][4];
									short fade_source = shader_transparent_chicago->chicago.framebuffer_fade_source;
									unsigned long combiner_constant;

									vsh_constants__texscale[0][0] = 0.0f;
									vsh_constants__texscale[0][1] = 0.0f;
									vsh_constants__texscale[0][2] = 0.0f;
									vsh_constants__texscale[0][3] = 0.0f;
									vsh_constants__texscale[1][0] = 0.0f;
									vsh_constants__texscale[1][1] = 0.0f;
									vsh_constants__texscale[1][2] = 0.0f;
									vsh_constants__texscale[1][3] = 0.0f;
									vsh_constants__texscale[2][0] = 0.0f;
									vsh_constants__texscale[2][1] = 0.0f;
									vsh_constants__texscale[2][2] = 1.0f;
									vsh_constants__texscale[2][3] = 0.0f;

									if (group->effect_type ==
											_render_model_effect_type_active_camouflage &&
										!TEST_FLAG(shader_transparent_chicago->chicago.extra_flags,
											_shader_transparent_chicago_extra_flag_dont_fade_active_camouflage_bit))
										vsh_constants__texscale[2][2] *=
											PIN(1.0f-group->effect_intensity, 0.0f, 1.0f);

									if (fade_source > 0 &&
										group->animation &&
										group->animation->values)
										vsh_constants__texscale[2][2] *=
											group->animation->values[fade_source-1];

									result = IDirect3DDevice8_SetVertexShaderConstant(
										global_d3d_device,
										VSH_CONSTANTS__TEXSCALE_OFFSET,
										vsh_constants__texscale,
										VSH_CONSTANTS__TEXSCALE_COUNT);
									if (success && result >= 0)
									{
										success = TRUE;
									}
									else
									{
										success = FALSE;
										rasterizer_error(
											result,
											"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
									}

									switch (shader_transparent_chicago->chicago.framebuffer_fade_mode)
									{
										case _framebuffer_fade_mode_none:
											combiner_constant = 0x14;
											break;

										case _framebuffer_fade_mode_fade_when_perpendicular:
											combiner_constant = 0x15;
											break;

										case _framebuffer_fade_mode_fade_when_parallel:
											combiner_constant = 0x05;
											break;

										/* combiner_constant is left unassigned only by this default arm. Not reached unassigned: the
										 * arm's assertion failure calls system_exit, which does not return in January
										 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
										 * Source-policy approval pending (2026-09-27 audit). */
										default:
											match_vassert(
												"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
												1539,
												FALSE,
												"### ERROR unsupported framebuffer fade mode");
											break;
									}

									switch (shader_transparent_chicago->chicago.framebuffer_blend_function)
									{
										case _framebuffer_blend_function_alpha_blend:
											pixel_shader.alpha_inputs[stage_count] =
												(combiner_constant|0x1C00)<<16;
											pixel_shader.alpha_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_multiply:
										case _framebuffer_blend_function_component_min:
											pixel_shader.rgb_inputs[stage_count] =
												((combiner_constant^0x20)|(combiner_constant<<16))|0x0C002000;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_double_multiply:
											pixel_shader.rgb_inputs[stage_count] =
												((combiner_constant^0x20)|(combiner_constant<<16))|0x0C00A000;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_add:
										case _framebuffer_blend_function_subtract:
										case _framebuffer_blend_function_component_max:
											pixel_shader.rgb_inputs[stage_count] =
												(combiner_constant|0x0C00)<<16;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										case _framebuffer_blend_function_alpha_multiply_add:
											pixel_shader.alpha_inputs[stage_count] =
												(combiner_constant|0x1C00)<<16;
											pixel_shader.alpha_outputs[stage_count] = 0xC00;
											pixel_shader.rgb_inputs[stage_count] =
												(combiner_constant|0x0C00)<<16;
											pixel_shader.rgb_outputs[stage_count] = 0xC00;
											break;

										default:
											match_vassert(
												"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
												1570,
												FALSE,
												"### ERROR unsupported framebuffer blend function");
											break;
									}
								}
							}
							break;
						}

						case _shader_type_transparent_water:
							rasterizer_water_draw(group);
							continue;

						case _shader_type_transparent_glass:
						{
							struct shader_transparent_glass_definition const *glass =
								(struct shader_transparent_glass_definition *)
									shader_get_and_verify_type(
										group->shader,
										_shader_type_transparent_glass);
							short reflection_type = glass->reflection_type;
							long result;

							if (reflection_type ==
								_shader_transparent_glass_reflection_type_dynamic_mirror)
							{
								if (!global_window_parameters.has_mirror ||
									global_window_parameters.rasterizer_target != 0)
									continue;
							}
							else if (reflection_type ==
									_shader_transparent_glass_reflection_type_bumped_cube_map &&
								(TEST_FLAG(glass->flags,
										_shader_transparent_glass_flag_bump_map_is_specular_mask_bit) ||
									glass->reflection_bump_map.index == NONE))
							{
								reflection_type =
									_shader_transparent_glass_reflection_type_flat_cube_map;
							}

							if (glass->tint_map.index != NONE ||
								glass->tint_color.red != 0.0f ||
								glass->tint_color.green != 0.0f ||
								glass->tint_color.blue != 0.0f)
							{
								real vsh_constants__texscale[3][4];

								rasterizer_set_texture(
									0,
									0,
									1,
									glass->tint_map.index,
									group->shader_permutation_index);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								SetRenderStateSmart(
									D3DRS_CULLMODE,
									TEST_FLAG(glass->flags,
										_shader_transparent_glass_flag_two_sided_bit) ?
										D3DCULL_NONE : D3DCULL_CCW);
								SetRenderStateSmart(
									D3DRS_COLORWRITEENABLE,
									D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
								SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
								SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_ZERO);
								SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
								SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
								SetRenderStateSmart(D3DRS_ALPHATESTENABLE, TRUE);
								SetRenderStateSmart(D3DRS_ALPHAREF, 0);
								rasterizer_set_vertex_shader_permutation(
									TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_TINT,
									vertex_type,
									shader_permutation_index);

								vsh_constants__texscale[0][0] =
									group->model_base_map_scale.i*glass->tint_map_scale;
								vsh_constants__texscale[0][1] =
									group->model_base_map_scale.j*glass->tint_map_scale;
								vsh_constants__texscale[0][2] = 1.0f;
								vsh_constants__texscale[0][3] = 1.0f;
								vsh_constants__texscale[1][0] = 0.0f;
								vsh_constants__texscale[1][1] = 0.0f;
								vsh_constants__texscale[1][2] = 0.0f;
								vsh_constants__texscale[1][3] = 0.0f;
								vsh_constants__texscale[2][0] = 0.0f;
								vsh_constants__texscale[2][1] = 0.0f;
								vsh_constants__texscale[2][2] = 0.0f;
								vsh_constants__texscale[2][3] = 0.0f;

								result = IDirect3DDevice8_SetVertexShaderConstant(
									global_d3d_device,
									VSH_CONSTANTS__TEXSCALE_OFFSET,
									vsh_constants__texscale,
									VSH_CONSTANTS__TEXSCALE_COUNT);
								if (success && result >= 0)
								{
									success = TRUE;
								}
								else
								{
									success = FALSE;
									rasterizer_error(
										result,
										"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
								}

								csmemset(&pixel_shader, 0, sizeof(pixel_shader));
								pixel_shader.texture_modes = 1;
								pixel_shader.combiner_count = 1;
								pixel_shader.constant_0[0] =
									real_rgb_color_to_pixel32(&glass->tint_color);
								pixel_shader.rgb_inputs[0] = 0x08010000;
								pixel_shader.rgb_outputs[0] = 0xC0;
								if (group->effect_type ==
									_render_model_effect_type_active_camouflage)
								{
									pixel_shader.constant_1[0] =
										real_alpha_to_pixel32(group->effect_intensity);
									pixel_shader.alpha_inputs[0] = 0x14320000;
									pixel_shader.alpha_outputs[0] = 0x40;
								}
								pixel_shader.final_combiner_inputs_abcd = 0x140C2000;
								pixel_shader.final_combiner_inputs_efg = 0x1400;
								rasterizer_set_pixel_shader(&pixel_shader);
								rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
							}

							if ((glass->reflection_view_perpendicular_color.alpha > 0.0f ||
									glass->reflection_view_parallel_color.alpha > 0.0f) &&
								(glass->reflection_map.index != NONE ||
									reflection_type ==
										_shader_transparent_glass_reflection_type_dynamic_mirror))
							{
								real vsh_constants__texscale[3][4];

								match_assert(
									"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
									1694,
									reflection_type>=0 &&
										reflection_type<NUMBER_OF_SHADER_TRANSPARENT_GLASS_REFLECTION_TYPES);

								rasterizer_set_texture(
									0,
									0,
									3,
									glass->reflection_bump_map.index,
									group->shader_permutation_index);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								rasterizer_set_texture_direct(
									1,
									global_rasterizer_data->vector_normalization.index,
									0);
								SetTextureStageStateSmart(1, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(1, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(1, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(1, D3DTSS_MINFILTER, D3DTEXF_POINT);
								SetTextureStageStateSmart(1, D3DTSS_MIPFILTER, D3DTEXF_POINT);
								rasterizer_set_texture_direct(
									2,
									global_rasterizer_data->vector_normalization.index,
									0);
								SetTextureStageStateSmart(2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(2, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
								SetTextureStageStateSmart(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(2, D3DTSS_MINFILTER, D3DTEXF_POINT);
								SetTextureStageStateSmart(2, D3DTSS_MIPFILTER, D3DTEXF_POINT);

								if (reflection_type ==
									_shader_transparent_glass_reflection_type_dynamic_mirror)
								{
									rasterizer_set_target_as_texture(3, 1, FALSE);
									SetTextureStageStateSmart(3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(3, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(3, D3DTSS_MIPFILTER, D3DTEXF_POINT);
								}
								else
								{
									rasterizer_set_texture(
										3,
										2,
										0,
										glass->reflection_map.index,
										group->shader_permutation_index);
									SetTextureStageStateSmart(3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(3, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(3, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(3, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								}

								SetRenderStateSmart(
									D3DRS_CULLMODE,
									TEST_FLAG(glass->flags,
										_shader_transparent_glass_flag_two_sided_bit) ?
										D3DCULL_NONE : D3DCULL_CCW);
								SetRenderStateSmart(
									D3DRS_COLORWRITEENABLE,
									D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
								SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
								SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
								SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_ONE);
								SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
								SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
								rasterizer_set_vertex_shader_permutation(
									TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_REFLECTION,
									vertex_type,
									reflection_type);

								vsh_constants__texscale[0][0] = group->model_base_map_scale.i*
									glass->reflection_bump_map_scale;
								vsh_constants__texscale[0][1] = group->model_base_map_scale.j*
									glass->reflection_bump_map_scale;
								vsh_constants__texscale[0][2] = 320.0f;
								vsh_constants__texscale[0][3] = 240.0f;
								vsh_constants__texscale[1][0] = 0.0f;
								vsh_constants__texscale[1][1] = 0.0f;
								vsh_constants__texscale[1][2] = 0.0f;
								vsh_constants__texscale[1][3] = 0.0f;
								vsh_constants__texscale[2][0] = 0.0f;
								vsh_constants__texscale[2][1] = 0.0f;
								vsh_constants__texscale[2][2] = 0.0f;
								vsh_constants__texscale[2][3] = 0.0f;

								result = IDirect3DDevice8_SetVertexShaderConstant(
									global_d3d_device,
									VSH_CONSTANTS__TEXSCALE_OFFSET,
									vsh_constants__texscale,
									VSH_CONSTANTS__TEXSCALE_COUNT);
								if (success && result >= 0)
								{
									success = TRUE;
								}
								else
								{
									success = FALSE;
									rasterizer_error(
										result,
										"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
								}

								csmemset(&pixel_shader, 0, sizeof(pixel_shader));
								switch (reflection_type)
								{
									case _shader_transparent_glass_reflection_type_bumped_cube_map:
										pixel_shader.texture_modes = 0x62E21;
										pixel_shader.input_texture = 0;
										pixel_shader.dot_mapping = 0x111;
										break;

									case _shader_transparent_glass_reflection_type_flat_cube_map:
										pixel_shader.texture_modes = 0x18C61;
										break;

									case _shader_transparent_glass_reflection_type_dynamic_mirror:
										pixel_shader.texture_modes = 0x8C61;
										break;

									default:
										match_vassert(
											"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
											1799,
											FALSE,
											"### ERROR unsupported reflection type");
										break;
								}

								pixel_shader.combiner_count = 0x11005;
								if (reflection_type !=
										_shader_transparent_glass_reflection_type_bumped_cube_map &&
									glass->reflection_bump_map.index != NONE)
								{
									pixel_shader.rgb_inputs[0] = 0x49480B0B;
								}
								else
								{
									real_rgb_color view_direction_color;

									view_direction_color.red = PIN(0.5f -
										global_window_parameters.camera.forward.i*0.5f, 0.0f, 1.0f);
									view_direction_color.green = PIN(0.5f -
										global_window_parameters.camera.forward.j*0.5f, 0.0f, 1.0f);
									view_direction_color.blue = PIN(0.5f -
										global_window_parameters.camera.forward.k*0.5f, 0.0f, 1.0f);

									pixel_shader.constant_0[0] =
										real_rgb_color_to_pixel32(&view_direction_color);
									pixel_shader.rgb_inputs[0] = 0x4A410B0B;
								}

								pixel_shader.rgb_outputs[0] = 0x20CD;
								pixel_shader.rgb_inputs[1] = 0x0C0C0D0D;
								pixel_shader.rgb_outputs[1] = 0xCD;
								if (group->effect_type ==
									_render_model_effect_type_active_camouflage)
								{
									pixel_shader.constant_1[1] =
										real_alpha_to_pixel32(group->effect_intensity);
									pixel_shader.alpha_inputs[1] = 0x14320000;
									pixel_shader.alpha_outputs[1] = 0x40;
								}
								pixel_shader.rgb_inputs[2] = 0x0C0C0D0D;
								pixel_shader.rgb_outputs[2] = 0xD;
								pixel_shader.constant_0[3] = real_argb_color_to_pixel32(
									&glass->reflection_view_perpendicular_color);
								pixel_shader.constant_1[3] = real_argb_color_to_pixel32(
									&glass->reflection_view_parallel_color);
								pixel_shader.alpha_outputs[3] = 0xC00;
								pixel_shader.rgb_outputs[3] = 0xC00;
								pixel_shader.rgb_outputs[4] = 0xC00;
								pixel_shader.alpha_inputs[3] = 0x2C120C11;
								pixel_shader.rgb_inputs[3] = 0x2C020C01;
								pixel_shader.rgb_inputs[4] = 0x2C0D0C0B;
								pixel_shader.final_combiner_inputs_abcd = 0x0C0F0000;
								pixel_shader.final_combiner_inputs_efg =
									((TEST_FLAG(glass->flags,
										_shader_transparent_glass_flag_bump_map_is_specular_mask_bit) ?
										0x08 : 0x14)<<16) | 0x1C002000;
								rasterizer_set_pixel_shader(&pixel_shader);
								rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
							}

							if (glass->diffuse_map.index != NONE ||
								glass->diffuse_detail_map.index != NONE)
							{
								real vsh_constants__texscale[3][4];

								rasterizer_set_texture(
									0,
									0,
									1,
									glass->diffuse_map.index,
									group->shader_permutation_index);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								rasterizer_set_texture(
									1,
									0,
									2,
									glass->diffuse_detail_map.index,
									group->shader_permutation_index);
								SetTextureStageStateSmart(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
								SetTextureStageStateSmart(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
								SetTextureStageStateSmart(1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								SetRenderStateSmart(
									D3DRS_CULLMODE,
									TEST_FLAG(glass->flags,
										_shader_transparent_glass_flag_two_sided_bit) ?
										D3DCULL_NONE : D3DCULL_CCW);
								SetRenderStateSmart(
									D3DRS_COLORWRITEENABLE,
									D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
								SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
								SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
								SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
								SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
								SetRenderStateSmart(D3DRS_ALPHATESTENABLE, TRUE);
								SetRenderStateSmart(D3DRS_ALPHAREF, 0);

								if (group->lightmap)
								{
									rasterizer_set_texture_bitmap_data(2, group->lightmap);
									SetTextureStageStateSmart(2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								}
								else
								{
									rasterizer_set_texture(2, 0, 0, NONE, 0);
									SetTextureStageStateSmart(2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
									SetTextureStageStateSmart(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
									SetTextureStageStateSmart(2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
								}

								rasterizer_set_vertex_shader_permutation(
									TRANSPARENT_GEOMETRY_VERTEX_SHADER_GLASS_DIFFUSE,
									vertex_type,
									shader_permutation_index);

								vsh_constants__texscale[0][0] = glass->diffuse_map_scale*
									group->model_base_map_scale.i;
								vsh_constants__texscale[0][1] = glass->diffuse_map_scale*
									group->model_base_map_scale.j;
								vsh_constants__texscale[0][2] = glass->diffuse_detail_map_scale*
									group->model_base_map_scale.i;
								vsh_constants__texscale[0][3] = glass->diffuse_detail_map_scale*
									group->model_base_map_scale.j;
								vsh_constants__texscale[1][0] = 0.0f;
								vsh_constants__texscale[1][1] = 0.0f;
								vsh_constants__texscale[1][2] = 0.0f;
								vsh_constants__texscale[1][3] = 0.0f;
								vsh_constants__texscale[2][0] = 0.0f;
								vsh_constants__texscale[2][1] = 0.0f;
								vsh_constants__texscale[2][2] = 0.0f;
								vsh_constants__texscale[2][3] = 0.0f;

								result = IDirect3DDevice8_SetVertexShaderConstant(
									global_d3d_device,
									VSH_CONSTANTS__TEXSCALE_OFFSET,
									vsh_constants__texscale,
									VSH_CONSTANTS__TEXSCALE_COUNT);
								if (success && result >= 0)
								{
									success = TRUE;
								}
								else
								{
									success = FALSE;
									rasterizer_error(
										result,
										"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
								}

								csmemset(&pixel_shader, 0, sizeof(pixel_shader));
								pixel_shader.texture_modes = 0x421;
								pixel_shader.combiner_count = 3;
								pixel_shader.alpha_inputs[0] = 0x18190000;
								pixel_shader.alpha_outputs[0] = 0xC0;
								pixel_shader.rgb_inputs[0] = 0x08090000;
								pixel_shader.rgb_outputs[0] = 0x100C0;
								pixel_shader.alpha_inputs[1] = 0x1C140000;
								pixel_shader.alpha_outputs[1] = 0xC0;
								pixel_shader.rgb_inputs[1] = 0x0A200420;
								pixel_shader.rgb_outputs[1] = 0xD00;
								pixel_shader.rgb_inputs[2] = 0x0C0D0000;
								pixel_shader.rgb_outputs[2] = 0xC0;
								pixel_shader.final_combiner_inputs_abcd = 0xC;
								pixel_shader.final_combiner_inputs_efg = 0x1C00;
								rasterizer_set_pixel_shader(&pixel_shader);
								rasterizer_transparent_geometry_group_draw__internal(
									group,
									group->lightmap != NULL);
							}
							continue;
						}

						case _shader_type_transparent_meter:
						{
							struct shader_transparent_meter_definition const *meter =
								(struct shader_transparent_meter_definition *)
									shader_get_and_verify_type(
										group->shader,
										_shader_type_transparent_meter);
							real meter_brightness = 1.0f;
							real flash_brightness = 1.0f;
							real meter_value = 1.0f;
							real gradient_value = 1.0f;
							real flash_extension = 1.0f;
							real_rgb_color flash_color;
							real flash_alpha;
							pixel32 gradient_min_pixel;
							pixel32 gradient_max_pixel;
							pixel32 background_pixel;
							pixel32 flash_pixel;
							pixel32 tint_pixel;
							real vsh_constants__texscale[3][4];
							long result;

							if (group->animation && group->animation->values)
							{
								if (meter->meter_brightness_source>=1 && meter->meter_brightness_source<=NUMBER_OF_SHADER_ANIMATION_FUNCTIONS)
									meter_brightness = group->animation->values[
										meter->meter_brightness_source-1];
								if (meter->flash_brightness_source>=1 && meter->flash_brightness_source<=NUMBER_OF_SHADER_ANIMATION_FUNCTIONS)
									flash_brightness = group->animation->values[
										meter->flash_brightness_source-1];
								if (meter->value_source>=1 && meter->value_source<=NUMBER_OF_SHADER_ANIMATION_FUNCTIONS)
									meter_value = group->animation->values[
										meter->value_source-1];
								if (meter->gradient_source>=1 && meter->gradient_source<=NUMBER_OF_SHADER_ANIMATION_FUNCTIONS)
									gradient_value = group->animation->values[
										meter->gradient_source-1];
								if (meter->flash_extension_source>=1 && meter->flash_extension_source<=NUMBER_OF_SHADER_ANIMATION_FUNCTIONS)
									flash_extension = group->animation->values[
										meter->flash_extension_source-1];
							}

							if (rasterizer_debug_options.debug_meter_shader)
							{
								real debug_value = periodic_function_evaluate(
									2,
									global_frame_parameters.game_time_sec/
										rasterizer_debug_options.transparent_geometry_intensity);

								meter_brightness =
									rasterizer_debug_options.debug_shader_values[0] >= 0.0f ?
										rasterizer_debug_options.debug_shader_values[0] : debug_value;
								flash_brightness =
									rasterizer_debug_options.debug_shader_values[1] >= 0.0f ?
										rasterizer_debug_options.debug_shader_values[1] : debug_value;
								meter_value =
									rasterizer_debug_options.debug_shader_values[2] >= 0.0f ?
										rasterizer_debug_options.debug_shader_values[2] : debug_value;
								gradient_value =
									rasterizer_debug_options.debug_shader_values[3] >= 0.0f ?
										rasterizer_debug_options.debug_shader_values[3] : debug_value;
								flash_extension =
									rasterizer_debug_options.debug_shader_values[4] >= 0.0f ?
										rasterizer_debug_options.debug_shader_values[4] : debug_value;
							}

							flash_color.red = flash_brightness*meter->flash_color.red;
							flash_color.green = flash_brightness*meter->flash_color.green;
							flash_color.blue = flash_brightness*meter->flash_color.blue;
							flash_alpha = 1.0f/MAX(gradient_value*8.0f, 1.0f);

							if (TEST_FLAG(meter->flags, _shader_transparent_meter_flag_tint_mode_2_bit))
							{
								real background_alpha = meter->background_transparency;
								real tint_alpha = meter->meter_transparency;
								real_rgb_color tint_color;

								tint_color.red = meter_brightness*meter->tint_color.red;
								tint_color.green = meter_brightness*meter->tint_color.green;
								tint_color.blue = meter_brightness*meter->tint_color.blue;
								gradient_min_pixel = real_a_rgb_color_to_pixel32(
									meter_value,
									&meter->gradient_min_color);
								gradient_max_pixel = real_a_rgb_color_to_pixel32(
									flash_alpha,
									&meter->gradient_max_color);
								background_pixel = real_a_rgb_color_to_pixel32(
									background_alpha,
									&meter->background_color);
								flash_pixel = real_a_rgb_color_to_pixel32(
									flash_extension,
									&flash_color);
								tint_pixel = real_a_rgb_color_to_pixel32(tint_alpha, &tint_color);
							}
							else
							{
								gradient_min_pixel = real_a_rgb_color_to_pixel32(
									meter_value,
									&meter->gradient_min_color);
								gradient_max_pixel = real_a_rgb_color_to_pixel32(
									flash_alpha,
									&meter->gradient_max_color);
								background_pixel = real_a_rgb_color_to_pixel32(
									0.0f,
									&meter->background_color);
								flash_pixel = real_a_rgb_color_to_pixel32(
									flash_extension,
									&flash_color);
								tint_pixel = real_a_rgb_color_to_pixel32(meter_brightness, &meter->tint_color);
							}

							rasterizer_set_texture(
								0,
								0,
								1,
								meter->map.index,
								group->shader_permutation_index);
							SetTextureStageStateSmart(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
							SetTextureStageStateSmart(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
							SetTextureStageStateSmart(
								0,
								D3DTSS_MAGFILTER,
								TEST_FLAG(meter->flags, _shader_transparent_meter_flag_unfiltered_bit) ?
									D3DTEXF_POINT : D3DTEXF_LINEAR);
							SetTextureStageStateSmart(
								0,
								D3DTSS_MINFILTER,
								TEST_FLAG(meter->flags, _shader_transparent_meter_flag_unfiltered_bit) ?
									D3DTEXF_POINT : D3DTEXF_LINEAR);
							SetTextureStageStateSmart(
								0,
								D3DTSS_MIPFILTER,
								TEST_FLAG(meter->flags, _shader_transparent_meter_flag_unfiltered_bit) ?
									D3DTEXF_POINT : D3DTEXF_LINEAR);
							SetRenderStateSmart(
								D3DRS_CULLMODE,
								TEST_FLAG(meter->flags, _shader_transparent_meter_flag_two_sided_bit) ?
									D3DCULL_NONE : D3DCULL_CCW);
							SetRenderStateSmart(
								D3DRS_COLORWRITEENABLE,
								D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
							SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
							SetRenderStateSmart(
								D3DRS_SRCBLEND,
								TEST_FLAG(meter->flags,
									_shader_transparent_meter_flag_tint_mode_2_bit) ?
									D3DBLEND_CONSTANTCOLOR : D3DBLEND_CONSTANTALPHA);
							SetRenderStateSmart(
								D3DRS_DESTBLEND,
								TEST_FLAG(meter->flags,
									_shader_transparent_meter_flag_tint_mode_2_bit) ?
									D3DBLEND_SRCALPHA : D3DBLEND_CONSTANTCOLOR);
							SetRenderStateSmart(D3DRS_BLENDCOLOR, tint_pixel);
							SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
							SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
							rasterizer_set_vertex_shader_permutation(
								TRANSPARENT_GEOMETRY_VERTEX_SHADER_METER,
								vertex_type,
								shader_permutation_index);

							vsh_constants__texscale[0][0] = 1.0f;
							vsh_constants__texscale[0][1] = 1.0f;
							vsh_constants__texscale[0][2] = 1.0f;
							vsh_constants__texscale[0][3] = 1.0f;
							vsh_constants__texscale[1][0] = group->model_base_map_scale.i;
							vsh_constants__texscale[1][1] = 0.0f;
							vsh_constants__texscale[1][2] = 0.0f;
							vsh_constants__texscale[1][3] = 0.0f;
							vsh_constants__texscale[2][0] = 0.0f;
							vsh_constants__texscale[2][1] = group->model_base_map_scale.j;
							vsh_constants__texscale[2][2] = 0.0f;
							vsh_constants__texscale[2][3] = 0.0f;

							result = IDirect3DDevice8_SetVertexShaderConstant(
								global_d3d_device,
								VSH_CONSTANTS__TEXSCALE_OFFSET,
								vsh_constants__texscale,
								VSH_CONSTANTS__TEXSCALE_COUNT);
							if (success && result >= 0)
							{
								success = TRUE;
							}
							else
							{
								success = FALSE;
								rasterizer_error(
									result,
									"IDirect3DDevice8_SetVertexShaderConstant(global_d3d_device, VSH_CONSTANTS__TEXSCALE_OFFSET, vsh_constants__texscale, VSH_CONSTANTS__TEXSCALE_COUNT)");
							}

							csmemset(&pixel_shader, 0, sizeof(pixel_shader));
							SetTextureStageStateSmart(0, D3DTSS_ALPHAKILL, D3DTALPHAKILL_ENABLE);
							pixel_shader.texture_modes = 1;
							pixel_shader.combiner_count = 0x11104;
							pixel_shader.constant_0[0] = flash_pixel;
							pixel_shader.constant_1[0] = gradient_max_pixel;
							pixel_shader.alpha_inputs[0] = 0x12081208;
							pixel_shader.alpha_outputs[0] = 0x20C00;
							pixel_shader.rgb_inputs[0] = 0x1120E820;
							pixel_shader.rgb_outputs[0] = 0x20C00;
							pixel_shader.constant_0[1] = gradient_min_pixel;
							pixel_shader.constant_1[1] = gradient_max_pixel;
							pixel_shader.alpha_inputs[1] = 0x6C200000;
							pixel_shader.alpha_outputs[1] = 0xC0;
							pixel_shader.rgb_inputs[1] = 0x3C011C02;
							pixel_shader.rgb_outputs[1] = 0xC00;
							pixel_shader.constant_0[2] = gradient_min_pixel;
							pixel_shader.constant_1[2] = flash_pixel;
							pixel_shader.alpha_inputs[2] = 0x0820B120;
							pixel_shader.alpha_outputs[2] = 0xC00;
							pixel_shader.rgb_inputs[2] = ((TEST_FLAG(meter->flags,
								_shader_transparent_meter_flag_flash_color_is_negative_bit) ?
								0xE0 : 0) + 2) | 0x0C201C00;
							pixel_shader.rgb_outputs[2] = 0xC00;
							pixel_shader.constant_0[3] = background_pixel;
							pixel_shader.constant_1[3] = tint_pixel;
							pixel_shader.alpha_inputs[3] = 0x12201120;
							pixel_shader.alpha_outputs[3] = 0x4C00;
							pixel_shader.rgb_inputs[3] = 0x0C200120;
							pixel_shader.rgb_outputs[3] = 0x4C00;
							pixel_shader.final_combiner_inputs_abcd = 0x0C180000;
							pixel_shader.final_combiner_inputs_efg = 0x1C00;

							if (rasterizer_debug_options.debug_meter_shader &&
								rasterizer_debug_options.transparent_geometry_index)
							{
								csmemset(&pixel_shader, 0, sizeof(pixel_shader));
								SetTextureStageStateSmart(0, D3DTSS_ALPHAKILL, D3DTALPHAKILL_DISABLE);
								SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, FALSE);
								pixel_shader.texture_modes = 1;
								pixel_shader.combiner_count = 1;
								pixel_shader.final_combiner_inputs_abcd =
									(rasterizer_debug_options.transparent_geometry_index<=1 ?
										0 : 0x10) + 8;
							}

							rasterizer_set_pixel_shader(&pixel_shader);
							rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
							SetTextureStageStateSmart(0, D3DTSS_ALPHAKILL, D3DTALPHAKILL_DISABLE);
							continue;
						}

						case _shader_type_transparent_plasma:
							rasterizer_plasma_energy_draw(
								(struct rasterizer_transparent_geometry_group_plasma const *)group);
							continue;

						default:
							error(_error_silent, "### ERROR unsupported shader type");
							success = FALSE;
							continue;
					}

					rasterizer_set_pixel_shader(&pixel_shader);
					rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
				}

				if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_zbuffer_bit) &&
					global_window_parameters.rasterizer_target == 0)
					rasterizer_set_frustum_z(0.0f, 0.0f);

				if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit) &&
					group->effect_type == _render_model_effect_type_active_camouflage)
					rasterizer_set_frustum_z(0.0f, 0.0f);
			}
		}

		if (!dirty)
			rasterizer_xbox_transparent_geometry_globals.last_source_object_index =
				group->source_object_index;

		if (group->next_group_presorted_index != NONE)
			rasterizer_transparent_geometry_group_draw(
				rasterizer_transparent_geometry_get_group_from_presorted_index(
					group->next_group_presorted_index),
				dirty);

		if (draw_active_camouflage_groups2)
		{
			short group_count;
			struct transparent_geometry_group *groups2 =
				rasterizer_transparent_geometry_get_groups2(&group_count);
			short group_index;

			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_transparent_geometry.c",
				2272,
				!dirty);

			for (group_index = 0; group_index < group_count; group_index++)
			{
				if (groups2[group_index].active_camouflage_transparent_source_object_index ==
						group->source_object_index &&
					groups2[group_index].effect_type ==
						_render_model_effect_type_active_camouflage)
				{
					rasterizer_transparent_geometry_group_draw(&groups2[group_index], TRUE);

					if (rasterizer_debug_options.transparent_geometry_index)
						rasterizer_xbox_transparent_geometry_globals.test_no_more_active_camo =
							TRUE;
				}
			}
		}

		if (!success)
			error(
				_error_silent,
				"### ERROR rasterizer_transparent_geometry_group_draw failed");
	}

	return;
}
