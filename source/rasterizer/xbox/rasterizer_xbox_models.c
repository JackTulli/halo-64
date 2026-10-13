/*
RASTERIZER_XBOX_MODELS.C

symbols in this file:
0015A150 01b0:
	_D3DDevice_SetRenderState (0000)
0015A300 0050:
	_D3DDevice_SetTextureStageState (0000)
0015A350 03f0:
	_set_environment_shader_pixel_shader (0000)
0015A740 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
0015A960 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
0015A9C0 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
0015A9D0 0040:
	__rasterizer_models_begin (0000)
0015AA10 0080:
	__rasterizer_model_end (0000)
0015AA90 0030:
	__rasterizer_models_end (0000)
0015AAC0 0040:
	_rasterizer_model_ambient_reflection_tint (0000)
0015AB00 0c20:
	_rasterizer_model_draw_environment_shader (0000)
0015B720 01c0:
	__rasterizer_model_begin (0000)
0015B8E0 0510:
	__rasterizer_model_transparent_geometry_submit (0000)
0015BDF0 1430:
	__rasterizer_model_draw (0000)
00291818 0042:
	??_C@_0EC@KBJMJLJB@detail_mask?$DO?$DN0?5?$CG?$CG?5detail_mask?$DMNU@ (0000)
00291860 004e:
	??_C@_0EO@GAMPAHAK@detail_function?$DO?$DN0?5?$CG?$CG?5detail_fun@ (0000)
002918B0 0038:
	??_C@_0DI@FGKEFDJL@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
002918E8 0011:
	??_C@_0BB@BMPHCOIP@local_parameters?$AA@ (0000)
002918FC 002f:
	??_C@_0CP@IHIFHMCH@cc0_error?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5cc0_err@ (0000)
0029192C 002f:
	??_C@_0CP@KGDHFBB@cc0_error?4green?$DO?$DN0?40f?5?$CG?$CG?5cc0_err@ (0000)
0029195C 002d:
	??_C@_0CN@LKMEOKHH@cc0_error?4red?5?$DO?$DN0?40f?5?$CG?$CG?5cc0_erro@ (0000)
0029198C 0023:
	??_C@_0CD@OECGPNGC@cc1?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5cc1?4blue?5?$DM?$DN1?4@ (0000)
002919B0 0023:
	??_C@_0CD@DFKMLKHD@cc1?4green?$DO?$DN0?40f?5?$CG?$CG?5cc1?4green?$DM?$DN1?4@ (0000)
002919D4 0021:
	??_C@_0CB@CPJFGMCD@cc1?4red?5?$DO?$DN0?40f?5?$CG?$CG?5cc1?4red?5?$DM?$DN1?40f@ (0000)
002919F8 0023:
	??_C@_0CD@EPNLIMOH@cc0?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5cc0?4blue?5?$DM?$DN1?4@ (0000)
00291A1C 0023:
	??_C@_0CD@JOFBMLPG@cc0?4green?$DO?$DN0?40f?5?$CG?$CG?5cc0?4green?$DM?$DN1?4@ (0000)
00291A40 0021:
	??_C@_0CB@PMOEFNHH@cc0?4red?5?$DO?$DN0?40f?5?$CG?$CG?5cc0?4red?5?$DM?$DN1?40f@ (0000)
00291A68 0074:
	??_C@_0HE@IBEOFMMD@global_window_parameters?4fog?4atm@ (0000)
00291ADC 0039:
	??_C@_0DJ@MAOJHDPF@group?9?$DOactive_camouflage_transpa@ (0000)
00291B18 0030:
	??_C@_0DA@GOHJDLCN@local_parameters?9?$DOeffect?4source_@ (0000)
00291B48 0032:
	??_C@_0DC@CEHNCDIC@shader_type_is_valid_for_model?$CIs@ (0000)
00291B7C 003b:
	??_C@_0DL@NALNBEEB@?$CD?$CD?$CD?5ERROR?5model?5effect?5type?5?$CD?$CFd?5@ (0000)
00291BB8 0045:
	??_C@_0EF@FGOGCHIG@diffuse_change_color?4blue?5?$DO?$DN0?40f@ (0000)
00291C00 0045:
	??_C@_0EF@PPFPLJEH@diffuse_change_color?4green?$DO?$DN0?40f@ (0000)
00291C48 0043:
	??_C@_0ED@PEIKOPIA@diffuse_change_color?4red?5?$DO?$DN0?40f?5@ (0000)
00291C8C 003b:
	??_C@_0DL@BIOGNNCB@external_color?9?$DOblue?5?$DO?$DN0?40f?5?$CG?$CG?5e@ (0000)
00291CC8 003b:
	??_C@_0DL@HMDMJINO@external_color?9?$DOgreen?$DO?$DN0?40f?5?$CG?$CG?5e@ (0000)
00291D04 0039:
	??_C@_0DJ@CNNDHOMN@external_color?9?$DOred?5?$DO?$DN0?40f?5?$CG?$CG?5ex@ (0000)
00291D40 004b:
	??_C@_0EL@GNHHGHCF@self_illumination_color?4blue?5?$DO?$DN0@ (0000)
00291D90 004b:
	??_C@_0EL@HBGILNPJ@self_illumination_color?4green?$DO?$DN0@ (0000)
00291DE0 0049:
	??_C@_0EJ@FOGGPAPE@self_illumination_color?4red?5?$DO?$DN0?4@ (0000)
00291E2C 003d:
	??_C@_0DN@HPMCIMLL@shader_model?9?$DOmodel?4self_illumin@ (0000)
00291E6C 0038:
	??_C@_0DI@PCENFHCM@local_model_effect_type?$DN?$DN_render@ (0000)
00291EA4 0026:
	??_C@_0CG@IGIAPBFN@shader?9?$DObase?4type?$DN?$DN_shader_type_@ (0000)
0030CEFB 0001:
	_local_pixel_shader_dirty_flag (0000)
00465D68 00bf:
	_rasterizer_models_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#define REAL_MATH_EXTERNAL_POINT_FROM_LINE3D
#include "bitmaps/bitmap_color_conversion.h"
#include "math/periodic_functions.h"
#include "math/real_math.h"
#undef REAL_MATH_EXTERNAL_POINT_FROM_LINE3D
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_geometry.h"
#include "rasterizer/rasterizer_models.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "render/render.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"

#include <xtl.h>

#include "interface/progress_bar_internal.h"
#include "rasterizer/xbox/rasterizer_xbox.h"
#include "rasterizer/xbox/rasterizer_xbox_draw_primitives.h"
#include "rasterizer/xbox/rasterizer_xbox_internal.h"
#include "rasterizer/xbox/rasterizer_xbox_models.h"

/* ---------- constants */

enum
{
	_rasterizer_geometry_no_sort_bit = 0,
	_rasterizer_geometry_no_queue_bit = 1,
	_rasterizer_geometry_no_fog_bit = 2,
	_rasterizer_geometry_no_zbuffer_bit = 3,
	_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit = 6,
	_rasterizer_geometry_first_person_bit = 7,
	RASTERIZER_STENCIL_MODE_WRITE = 1,
	RASTERIZER_STENCIL_MODE_REJECT = 2,
	_render_model_effect_type_none = 0,
	_render_model_effect_type_active_camouflage,
	_render_model_effect_type_modifier,
	_rasterizer_statistics_mode_summary = 1,
	_rasterizer_statistics_mode_enabled = 2,
	_rasterizer_vertex_shader_model = 10,
	NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS = 3,
	NUMBER_OF_SHADER_MODEL_DETAIL_MASKS = 9,
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
};

enum
{
	NUMBER_OF_SHADER_ANIMATION_FUNCTIONS = 4,
	NUMBER_OF_SHADER_ANIMATION_SOURCES = 5,
};

enum
{
	_shader_model_self_illumination_no_random_phase_bit = 0,
};

enum
{
	_shader_detail_mask_none = 0,
};

enum
{
	_shader_environment_alpha_tested_bit = 0,
	_shader_environment_bump_map_is_specular_mask_bit,
	_shader_environment_true_atmospheric_fog_bit,
};

enum
{
	_shader_model_detail_after_reflection_bit = 0,
	_shader_model_two_sided_bit,
	_shader_model_not_alpha_tested_bit,
	_shader_model_alpha_blended_decal_bit,
	_shader_model_true_atmospheric_fog_bit,
	_shader_model_nocull_two_sided_bit,
};

enum
{
	_fog_definition_atmosphere_dominant_bit = 1,
};

enum
{
	_model_vertex_shader_permutation_planar_fog = 0,
	_model_vertex_shader_permutation_lights,
	_model_vertex_shader_permutation_reflection,
	_model_vertex_shader_permutation_reflection_one_node,
	NUMBER_OF_MODEL_VERTEX_SHADER_PERMUTATIONS
};

/* ---------- structures */

struct rasterizer_debug_options
{
	byte reserved00[2];
	short statistics_mode;
	byte reserved04[8];
	boolean draw_models;
	boolean draw_transparent_models;
	byte reserved0E[0xE];
	boolean fog;
	byte reserved1D[0x24];
	boolean active_camouflage;
	byte reserved42[0x12];
	unsigned long zbias;
};

struct rasterizer_model_skinning_parameters
{
	void const *node_matrices;
	short node_matrix_count;
	word pad06;
};

struct rasterizer_model_effect_parameters
{
	short type;
	word pad02;
	real intensity;
	byte reserved08[4];
	long source_object_index;
	real_point3d centroid;
	struct shader *shader;
	struct render_animation animation;
};

struct rasterizer_model_begin_parameters
{
	unsigned long geometry_flags;
	long unique_identifier;
	struct rasterizer_model_skinning_parameters skinning;
	struct render_lighting lighting;
	struct render_animation animation;
	struct rasterizer_model_effect_parameters effect;
	real_point3d centroid;
	real radius;
	real_vector2d base_map_scale;
};

struct shader_model_properties
{
	word flags;
	short type;
	byte reserved04[0xC];
	real translucency;
	byte reserved14[0x10];
	short change_color_source;
	byte reserved26[0x1E];
	word self_illumination_flags;
	short pad46;
	short self_illumination_color_source;
	short self_illumination_animation_function;
	real self_illumination_animation_period;
	real_rgb_color self_illumination_animation_color_lower_bound;
	real_rgb_color self_illumination_animation_color_upper_bound;
	byte reserved68[0xC];
	real map_u_scale;
	real map_v_scale;
	struct tag_reference base_map;
	byte reserved8C[8];
	struct tag_reference multipurpose_map;
	byte reservedA4[8];
	short detail_function;
	short detail_mask;
	real detail_map_scale;
	struct tag_reference detail_map;
	real detail_map_v_scale;
	byte reservedC8[0xC];
	struct shader_texture_animation texture_animation;
	byte reserved10C[8];
	real reflection_falloff_distance;
	real reflection_cutoff_distance;
	real perpendicular_brightness;
	real_rgb_color perpendicular_tint_color;
	real parallel_brightness;
	real_rgb_color parallel_tint_color;
	struct tag_reference reflection_cube_map;
};

struct shader_model_definition
{
	struct shader shader;
	struct shader_model_properties model;
};

struct shader_transparent_plasma_definition
{
	struct shader shader;
	byte reserved28[4];
	short intensity_exponent_source;
};

struct render_sort_filth
{
	short *previous_group_presorted_index_reference;
	short *next_group_presorted_index_reference;
	short group_index;
	short next_part_index;
	short part_index;
	word pad0A;
};

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader *shader;
	short shader_permutation_index;
	word pad12;
	struct rasterizer_model_effect_parameters effect;
	real_vector2d model_base_map_scale;
	long dynamic_triangle_buffer_index;
	struct triangle_buffer const *triangle_buffer;
	long first_triangle_index;
	long triangle_count;
	long dynamic_vertex_buffer_index;
	struct vertex_buffer const *vertex_buffer;
	struct bitmap_data const *lightmap;
	real_matrix4x3 const *node_matrices;
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
	boolean sort_last;
	boolean cortana_hack;
	byte pad9E[2];
};

struct rasterizer_models_frame_statistics
{
	byte reserved000[0x14];
	unsigned long vertices_by_permutation[
		NUMBER_OF_MODEL_VERTEX_SHADER_PERMUTATIONS];
	byte reserved024[0xB0];
	unsigned long model_count;
	unsigned long model_vertex_count;
	unsigned long model_triangle_count;
	unsigned long model_draw_count;
	long transparent_model_vertex_count;
	long transparent_model_triangle_count;
	long transparent_model_maximum_triangle_count;
	long transparent_model_submit_count;
	byte reserved0F4[0x5C];
	unsigned long skinning_work;
	unsigned long lighting_work;
	unsigned long vertex_shader_work;
	unsigned long pushbuffer_words;
	unsigned long skinning_work_accumulated;
	unsigned long lighting_work_accumulated;
	unsigned long vertex_shader_work_accumulated;
};

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

struct shader_environment_diffuse_properties
{
	byte reserved00[0x1C];
	struct tag_reference base_map;
	byte reserved2C[0x18];
	short detail_map_function;
	short pad46;
	real primary_detail_map_scale;
	struct tag_reference primary_detail_map;
	byte reserved5C[0x44];
	real_rgb_color material_color;
	byte reservedAC[0xC];
	real bump_map_scale;
	struct tag_reference bump_map;
	real_vector2d runtime_bump_map_scale;
	byte reservedD4[0x40];
};

struct shader_environment_specular_properties
{
	word flags;
	short type;
	long unused04[4];
	real brightness;
	long unused18[5];
	real_rgb_color view_perpendicular_color;
	real_rgb_color view_parallel_color;
	long unused44[4];
};

struct shader_environment_reflection_properties
{
	word flags;
	short type;
	real lightmap_brightness_scale;
	byte reserved08[0x1C];
	real view_perpendicular_brightness;
	real view_parallel_brightness;
	byte reserved2C[0x28];
	struct tag_reference cube_map;
};

struct shader_environment_properties
{
	word flags;
	short type;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
	long unused[11];
	struct shader_environment_diffuse_properties diffuse;
	byte reserved158[0xFC];
	struct shader_environment_specular_properties specular;
	struct shader_environment_reflection_properties reflection;
};

struct shader_environment_definition
{
	struct shader shader;
	struct shader_environment_properties environment;
};

typedef char verify_rasterizer_models_draw_models_offset[
	offsetof(struct rasterizer_debug_options, draw_models) == 0x0C
		? 1 : -1];
typedef char verify_rasterizer_models_active_camouflage_offset[
	offsetof(
		struct rasterizer_debug_options,
		active_camouflage) == 0x41 ? 1 : -1];
typedef char verify_rasterizer_model_parameters_skinning_offset[
	offsetof(struct rasterizer_model_begin_parameters, skinning) == 0x08
		? 1 : -1];
typedef char verify_rasterizer_model_parameters_lighting_offset[
	offsetof(struct rasterizer_model_begin_parameters, lighting) == 0x10
		? 1 : -1];
typedef char verify_rasterizer_model_parameters_effect_offset[
	offsetof(struct rasterizer_model_begin_parameters, effect) == 0x8C
		? 1 : -1];
typedef char verify_rasterizer_models_window_fog_offset[
	offsetof(struct rasterizer_window_begin_parameters, fog) == 0x1E8
		? 1 : -1];
typedef char verify_rasterizer_models_statistics_skinning_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		skinning_work) == 0x150 ? 1 : -1];
typedef char verify_rasterizer_models_statistics_pushbuffer_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		pushbuffer_words) == 0x15C ? 1 : -1];
typedef char verify_rasterizer_models_statistics_vertex_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		model_vertex_count) == 0xD8 ? 1 : -1];
typedef char verify_rasterizer_model_parameters_position_offset[
	offsetof(struct rasterizer_model_begin_parameters, centroid) == 0xB4
		? 1 : -1];
typedef char verify_rasterizer_model_parameters_texture_scale_offset[
	offsetof(struct rasterizer_model_begin_parameters, base_map_scale) == 0xC4
		? 1 : -1];
typedef char verify_rasterizer_model_lighting_reflection_tint_offset[
	offsetof(struct rasterizer_model_begin_parameters, lighting.reflection_tint_color)
		== 0x5C ? 1 : -1];
typedef char verify_rasterizer_model_parameters_effect_source_offset[
	offsetof(
		struct rasterizer_model_begin_parameters,
		effect.source_object_index) == 0x98 ? 1 : -1];
typedef char verify_transparent_geometry_group_size[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];
typedef char verify_transparent_geometry_group_z_sort_offset[
	offsetof(struct transparent_geometry_group, z_sort) == 0x70 ? 1 : -1];
typedef char verify_transparent_geometry_group_plane_offset[
	offsetof(struct transparent_geometry_group, plane) == 0x80 ? 1 : -1];
typedef char verify_transparent_geometry_group_cortana_hack_offset[
	offsetof(struct transparent_geometry_group, cortana_hack) == 0x9D
		? 1 : -1];
typedef char verify_rasterizer_models_statistics_transparent_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		transparent_model_vertex_count) == 0xE4 ? 1 : -1];
typedef char verify_rasterizer_model_pixel_shader_size[
	sizeof(struct pixel_shader_definition) == 0xF0 ? 1 : -1];
typedef char verify_shader_environment_base_map_offset[
	offsetof(struct shader_environment_definition, environment.diffuse.base_map)
		== 0x88 ? 1 : -1];
typedef char verify_shader_environment_detail_function_offset[
	offsetof(
		struct shader_environment_definition,
		environment.diffuse.detail_map_function) == 0xB0 ? 1 : -1];
typedef char verify_shader_environment_bump_map_offset[
	offsetof(struct shader_environment_definition, environment.diffuse.bump_map)
		== 0x128 ? 1 : -1];
typedef char verify_shader_environment_specular_offset[
	offsetof(
		struct shader_environment_definition,
		environment.specular.view_perpendicular_color) == 0x2A8 ? 1 : -1];
typedef char verify_shader_environment_reflection_offset[
	offsetof(
		struct shader_environment_definition,
		environment.reflection.view_perpendicular_brightness) == 0x2F4 ? 1 : -1];
typedef char verify_shader_environment_cube_map_offset[
	offsetof(
		struct shader_environment_definition,
		environment.reflection.cube_map) == 0x324 ? 1 : -1];

typedef char verify_rasterizer_models_debug_options_zbias_offset[
	offsetof(struct rasterizer_debug_options, zbias) == 0x54
		? 1 : -1];
typedef char verify_rasterizer_model_parameters_effect_shader_offset[
	offsetof(struct rasterizer_model_begin_parameters, effect.shader) == 0xA8
		? 1 : -1];
typedef char verify_rasterizer_model_parameters_effect_animation_offset[
	offsetof(struct rasterizer_model_begin_parameters, effect.animation) == 0xAC
		? 1 : -1];
typedef char verify_rasterizer_models_statistics_vertex_shader_work_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		vertex_shader_work) == 0x158 ? 1 : -1];
typedef char verify_rasterizer_models_statistics_vertex_shader_accum_offset[
	offsetof(
		struct rasterizer_models_frame_statistics,
		vertex_shader_work_accumulated) == 0x168 ? 1 : -1];
typedef char verify_shader_plasma_intensity_exponent_source_offset[
	offsetof(
		struct shader_transparent_plasma_definition,
		intensity_exponent_source) == 0x2C ? 1 : -1];
typedef char verify_shader_model_translucency_offset[
	offsetof(struct shader_model_definition, model.translucency) == 0x38
		? 1 : -1];
typedef char verify_shader_model_change_color_source_offset[
	offsetof(struct shader_model_definition, model.change_color_source) == 0x4C
		? 1 : -1];
typedef char verify_shader_model_self_illumination_flags_offset[
	offsetof(
		struct shader_model_definition,
		model.self_illumination_flags) == 0x6C ? 1 : -1];
typedef char verify_shader_model_self_illumination_period_offset[
	offsetof(
		struct shader_model_definition,
		model.self_illumination_animation_period) == 0x74 ? 1 : -1];
typedef char verify_shader_model_self_illumination_upper_bound_offset[
	offsetof(
		struct shader_model_definition,
		model.self_illumination_animation_color_upper_bound) == 0x84 ? 1 : -1];
typedef char verify_shader_model_map_u_scale_offset[
	offsetof(struct shader_model_definition, model.map_u_scale) == 0x9C
		? 1 : -1];
typedef char verify_shader_model_base_map_offset[
	offsetof(struct shader_model_definition, model.base_map.index) == 0xB0
		? 1 : -1];
typedef char verify_shader_model_multipurpose_map_offset[
	offsetof(struct shader_model_definition, model.multipurpose_map.index) ==
		0xC8 ? 1 : -1];
typedef char verify_shader_model_detail_function_offset[
	offsetof(struct shader_model_definition, model.detail_function) == 0xD4
		? 1 : -1];
typedef char verify_shader_model_detail_map_scale_offset[
	offsetof(struct shader_model_definition, model.detail_map_scale) == 0xD8
		? 1 : -1];
typedef char verify_shader_model_detail_map_offset[
	offsetof(struct shader_model_definition, model.detail_map.index) == 0xE8
		? 1 : -1];
typedef char verify_shader_model_detail_map_v_scale_offset[
	offsetof(struct shader_model_definition, model.detail_map_v_scale) == 0xEC
		? 1 : -1];
typedef char verify_shader_model_texture_animation_offset[
	offsetof(struct shader_model_definition, model.texture_animation) == 0xFC
		? 1 : -1];
typedef char verify_shader_model_reflection_falloff_offset[
	offsetof(
		struct shader_model_definition,
		model.reflection_falloff_distance) == 0x13C ? 1 : -1];
typedef char verify_shader_model_perpendicular_brightness_offset[
	offsetof(
		struct shader_model_definition,
		model.perpendicular_brightness) == 0x144 ? 1 : -1];
typedef char verify_shader_model_parallel_brightness_offset[
	offsetof(struct shader_model_definition, model.parallel_brightness) ==
		0x154 ? 1 : -1];
typedef char verify_shader_model_reflection_cube_map_offset[
	offsetof(
		struct shader_model_definition,
		model.reflection_cube_map.index) == 0x170 ? 1 : -1];

/* ---------- globals */

extern struct rasterizer_debug_options rasterizer_debug_options;
static struct render_animation const *transparent_geometry_cached_animation = NULL;
static struct render_lighting const *transparent_geometry_cached_lighting = NULL;
static short transparent_geometry_cached_node_matrix_count = 0;
static real_matrix4x3 const *transparent_geometry_cached_node_matrices = NULL;
static struct transparent_geometry_group immediate_transparent_geometry_group = {0};
static struct rasterizer_model_begin_parameters const *local_parameters = NULL;
static boolean local_parameters_queued_flag = FALSE;
static short local_model_effect_type = 0;
static boolean local_sky_flag = FALSE;
static boolean local_planar_fog_flag = FALSE;
static boolean local_environment_fog_screen_flag = FALSE;
static boolean local_do_not_change_z_stencil_states = FALSE;
static boolean local_reported_too_many_transparent_geometry_groups = FALSE;
static boolean local_pixel_shader_dirty_flag = TRUE;
extern boolean rasterizer_model_cortana_hack;
extern struct rasterizer_window_begin_parameters global_window_parameters;
extern struct rasterizer_models_frame_statistics rasterizer_frame_statistics;

/* ---------- public code */

void rasterizer_model_ambient_reflection_tint(
	real alpha,
	real red,
	real green,
	real blue)
{
	if (global_rasterizer_model_ambient_reflection_tint)
	{
		global_rasterizer_model_ambient_reflection_tint->alpha = alpha;
		global_rasterizer_model_ambient_reflection_tint->red = red;
		global_rasterizer_model_ambient_reflection_tint->green = green;
		global_rasterizer_model_ambient_reflection_tint->blue = blue;
	}

	return;
}

/* ---------- private code */

void _rasterizer_models_begin(
	boolean sky)
{
	if (rasterizer_debug_options.draw_models)
	{
		local_pixel_shader_dirty_flag = TRUE;
		local_sky_flag = sky;
		if (sky)
		{
			rasterizer_profile_begin(_rasterizer_profile_model_sky);
		}
		else
		{
			rasterizer_profile_begin(_rasterizer_profile_models);
		}
	}

	return;
}

void _rasterizer_models_end(
	void)
{
	if (rasterizer_debug_options.draw_models)
	{
		if (local_sky_flag)
		{
			rasterizer_profile_end(_rasterizer_profile_model_sky);
		}
		else
		{
			rasterizer_profile_end(_rasterizer_profile_models);
		}
	}

	return;
}

void _rasterizer_model_end(
	void)
{
	if (rasterizer_debug_options.draw_models)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			1491,
			local_parameters);
		if (local_environment_fog_screen_flag)
		{
			rasterizer_environment_fog_screen_model_end();
		}
		if (TEST_FLAG(
				local_parameters->geometry_flags,
				_rasterizer_geometry_first_person_bit) &&
			!local_do_not_change_z_stencil_states)
		{
			rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_REJECT);
			rasterizer_set_frustum_z(0.0f, 0.0f);
		}
		local_parameters = NULL;
	}

	return;
}

void _rasterizer_model_begin(
	struct rasterizer_model_begin_parameters const *parameters,
	boolean do_not_change_z_stencil_states)
{
	unsigned long skinning_work;
	unsigned long lighting_work;
	real camera_plane_distance;

	if (rasterizer_debug_options.draw_models)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			587,
			parameters);
		if (TEST_FLAG(
				parameters->geometry_flags,
				_rasterizer_geometry_first_person_bit) &&
			!do_not_change_z_stencil_states)
		{
			rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_WRITE);
			rasterizer_set_frustum_z(
				rasterizer_globals.first_person_weapon_near_clip_distance,
				rasterizer_globals.first_person_weapon_far_clip_distance);
		}

		local_parameters = parameters;
		local_parameters_queued_flag = FALSE;
		local_do_not_change_z_stencil_states =
			do_not_change_z_stencil_states;

		if (rasterizer_debug_options.active_camouflage &&
			global_window_parameters.rasterizer_target == 0 &&
			parameters->effect.type == _render_model_effect_type_active_camouflage &&
			parameters->effect.intensity > 0.0f)
		{
			local_model_effect_type =
				_render_model_effect_type_active_camouflage;
		}
		else if (parameters->effect.type ==
			_render_model_effect_type_modifier)
		{
			local_model_effect_type =
				_render_model_effect_type_modifier;
		}
		else
		{
			skinning_work = rasterizer_frame_statistics.skinning_work;
			rasterizer_set_model_skinning(&parameters->skinning);
			skinning_work =
				rasterizer_frame_statistics.skinning_work - skinning_work;
			lighting_work = rasterizer_frame_statistics.lighting_work;
			rasterizer_set_model_lighting(&parameters->lighting);
			rasterizer_frame_statistics.skinning_work_accumulated +=
				skinning_work;
			rasterizer_frame_statistics.lighting_work_accumulated +=
				rasterizer_frame_statistics.lighting_work - lighting_work;
			local_model_effect_type = _render_model_effect_type_none;
		}

		camera_plane_distance =
			global_window_parameters.camera.position.x *
				global_window_parameters.fog.plane.n.i +
			global_window_parameters.camera.position.y *
				global_window_parameters.fog.plane.n.j +
			global_window_parameters.camera.position.z *
				global_window_parameters.fog.plane.n.k -
			global_window_parameters.fog.plane.d;
		local_planar_fog_flag =
			global_window_parameters.fog.planar_mode &&
			!TEST_FLAG(
				parameters->geometry_flags,
				_rasterizer_geometry_no_fog_bit) &&
			(!TEST_FLAG(
				parameters->geometry_flags,
				_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit) ||
			camera_plane_distance < 0.0f);

		if (!local_sky_flag &&
			rasterizer_environment_fog_screen_model_begin(parameters))
		{
			local_environment_fog_screen_flag = TRUE;
		}
		else
		{
			local_environment_fog_screen_flag = FALSE;
		}

		if (rasterizer_debug_options.statistics_mode ==
			_rasterizer_statistics_mode_enabled)
		{
			rasterizer_frame_statistics.model_count++;
		}
	}

	return;
}

/* ---------- private code */

static void set_environment_shader_pixel_shader(
	pixel32 detail_color,
	short detail_function,
	short detail_mask,
	pixel32 base_color,
	pixel32 fog_color,
	pixel32 fog_error_color,
	pixel32 atmospheric_fog_color,
	boolean double_diffuse_intensity,
	boolean true_atmospheric_fog)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
		106,
		detail_function>=0 && detail_function<NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
		107,
		detail_mask>=0 && detail_mask<NUMBER_OF_SHADER_MODEL_DETAIL_MASKS);

	{
		unsigned long detail_mask_alpha_inputs[
			NUMBER_OF_SHADER_MODEL_DETAIL_MASKS] =
			{
				PS_REGISTER_ONE,
				PS_REGISTER_R1 | PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_R1,
				PS_REGISTER_R0 | PS_CHANNEL_ALPHA |
					PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_R0 | PS_CHANNEL_ALPHA,
				PS_REGISTER_T1 | PS_CHANNEL_ALPHA |
					PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_T1 | PS_CHANNEL_ALPHA,
				PS_REGISTER_T2 | PS_CHANNEL_ALPHA |
					PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_T2 | PS_CHANNEL_ALPHA,
			};
		unsigned long detail_function_alpha_inputs[
			NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS] =
			{
				PS_REGISTER_ONE_HALF,
				PS_REGISTER_ONE,
				PS_REGISTER_ONE_HALF,
			};

		pixel_shader.constant_0[2] = base_color;
		pixel_shader.constant_0[1] = detail_color;
		pixel_shader.rgb_inputs[3] = PS_COMBINERINPUTS(
			detail_mask_alpha_inputs[detail_mask] ^
				PS_INPUTMAPPING_UNSIGNED_INVERT,
			detail_function_alpha_inputs[detail_function],
			detail_mask_alpha_inputs[detail_mask],
			PS_REGISTER_T1);
	}

	if (double_diffuse_intensity)
	{
		unsigned long detail_function_rgb_inputs[
			NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS] =
			{
				PS_COMBINERINPUTS(
					PS_REGISTER_R0,
					PS_REGISTER_T1,
					PS_REGISTER_R0,
					PS_REGISTER_T1),
				PS_COMBINERINPUTS(
					PS_REGISTER_R0,
					PS_REGISTER_T1,
					PS_REGISTER_ZERO,
					PS_REGISTER_ZERO),
				PS_COMBINERINPUTS(
					PS_REGISTER_R0,
					PS_REGISTER_ONE,
					PS_REGISTER_T1 | PS_INPUTMAPPING_EXPAND_NORMAL,
					PS_REGISTER_ONE),
			};

		pixel_shader.rgb_inputs[6] = PS_COMBINERINPUTS(
			PS_REGISTER_T0,
			PS_REGISTER_V0,
			PS_REGISTER_T3,
			PS_REGISTER_R1 | PS_CHANNEL_ALPHA);
		pixel_shader.rgb_outputs[6] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[7] =
			detail_function_rgb_inputs[detail_function];
		pixel_shader.rgb_outputs[7] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			PS_COMBINEROUTPUT_IDENTITY);
	}
	else
	{
		unsigned long detail_function_rgb_inputs[
			NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS] =
			{
				PS_COMBINERINPUTS(
					PS_REGISTER_T0,
					PS_REGISTER_T1,
					PS_REGISTER_T0,
					PS_REGISTER_T1),
				PS_COMBINERINPUTS(
					PS_REGISTER_T0,
					PS_REGISTER_T1,
					PS_REGISTER_ZERO,
					PS_REGISTER_ZERO),
				PS_COMBINERINPUTS(
					PS_REGISTER_T0,
					PS_REGISTER_ONE,
					PS_REGISTER_T1 | PS_INPUTMAPPING_EXPAND_NORMAL,
					PS_REGISTER_ONE),
			};

		pixel_shader.rgb_inputs[6] =
			detail_function_rgb_inputs[detail_function];
		pixel_shader.rgb_outputs[6] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_T0,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[7] = PS_COMBINERINPUTS(
			PS_REGISTER_T0,
			PS_REGISTER_V0,
			PS_REGISTER_T3,
			PS_REGISTER_R1 | PS_CHANNEL_ALPHA);
		pixel_shader.rgb_outputs[7] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			PS_COMBINEROUTPUT_IDENTITY);
	}

	if (true_atmospheric_fog)
	{
		pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
			PS_REGISTER_FOG | PS_CHANNEL_ALPHA |
				PS_INPUTMAPPING_UNSIGNED_INVERT,
			PS_REGISTER_R0,
			PS_REGISTER_FOG,
			PS_REGISTER_ZERO);
		pixel_shader.final_combiner_inputs_efg = PS_COMBINERINPUTS(
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO,
			PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
			0);
	}
	else
	{
		pixel_shader.constant_0[5] = fog_error_color;
		pixel_shader.constant_1[5] = atmospheric_fog_color;
		pixel_shader.final_combiner_constant_0 = fog_color;
		pixel_shader.final_combiner_constant_1 = atmospheric_fog_color;
		pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
			PS_REGISTER_V0 | PS_CHANNEL_ALPHA |
				PS_INPUTMAPPING_UNSIGNED_INVERT,
			PS_REGISTER_EF_PROD,
			PS_REGISTER_C0,
			PS_REGISTER_R1);
		pixel_shader.final_combiner_inputs_efg = PS_COMBINERINPUTS(
			PS_REGISTER_R0,
			PS_REGISTER_C0 | PS_CHANNEL_ALPHA,
			PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
			0);
	}

	if (local_pixel_shader_dirty_flag)
	{
		pixel_shader.texture_modes = PS_TEXTUREMODES(
			PS_TEXTUREMODES_PROJECT2D,
			PS_TEXTUREMODES_PROJECT2D,
			PS_TEXTUREMODES_PROJECT2D,
			PS_TEXTUREMODES_CUBEMAP);
		pixel_shader.combiner_count = PS_COMBINERCOUNT(
			8,
			PS_COMBINERCOUNT_UNIQUE_C0 | PS_COMBINERCOUNT_UNIQUE_C1);
		pixel_shader.constant_0[0] = 0x00FF0000;
		pixel_shader.constant_1[0] = 0x0000FF00;
		pixel_shader.alpha_inputs[0] = PS_COMBINERINPUTS(
			PS_REGISTER_T2,
			PS_REGISTER_ONE,
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO);
		pixel_shader.alpha_outputs[0] = PS_COMBINEROUTPUTS(
			PS_REGISTER_R0,
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[0] = PS_COMBINERINPUTS(
			PS_REGISTER_T2,
			PS_REGISTER_C1,
			PS_REGISTER_T2,
			PS_REGISTER_C0);
		pixel_shader.rgb_outputs[0] = PS_COMBINEROUTPUTS(
			PS_REGISTER_R0,
			PS_REGISTER_R1,
			PS_REGISTER_DISCARD,
			PS_COMBINEROUTPUT_AB_DOT_PRODUCT |
				PS_COMBINEROUTPUT_CD_DOT_PRODUCT);
		pixel_shader.alpha_inputs[1] = PS_COMBINERINPUTS(
			PS_REGISTER_R0 | PS_CHANNEL_ALPHA,
			PS_REGISTER_ONE,
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO);
		pixel_shader.alpha_outputs[1] = PS_COMBINEROUTPUTS(
			PS_REGISTER_T1,
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[1] = PS_COMBINERINPUTS(
			PS_REGISTER_V0,
			PS_REGISTER_ONE,
			PS_REGISTER_R0,
			PS_REGISTER_C0);
		pixel_shader.rgb_outputs[1] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_V0,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.alpha_inputs[2] = PS_COMBINERINPUTS(
			PS_REGISTER_R0,
			PS_REGISTER_ONE,
			PS_REGISTER_R1,
			PS_REGISTER_V1 | PS_CHANNEL_ALPHA);
		pixel_shader.alpha_outputs[2] = PS_COMBINEROUTPUTS(
			PS_REGISTER_R0,
			PS_REGISTER_R1,
			PS_REGISTER_DISCARD,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[2] = PS_COMBINERINPUTS(
			PS_REGISTER_R0 | PS_CHANNEL_ALPHA |
				PS_INPUTMAPPING_UNSIGNED_INVERT,
			PS_REGISTER_ONE,
			PS_REGISTER_R0 | PS_CHANNEL_ALPHA,
			PS_REGISTER_C0);
		pixel_shader.rgb_outputs[2] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_outputs[3] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_T1,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[4] = PS_COMBINERINPUTS(
			PS_REGISTER_T3,
			PS_REGISTER_V1,
			PS_REGISTER_V0,
			PS_REGISTER_R0);
		pixel_shader.rgb_outputs[4] = PS_COMBINEROUTPUTS(
			PS_REGISTER_T3,
			PS_REGISTER_V0,
			PS_REGISTER_DISCARD,
			PS_COMBINEROUTPUT_IDENTITY);
		pixel_shader.rgb_inputs[5] = PS_COMBINERINPUTS(
			PS_REGISTER_C1,
			PS_REGISTER_ONE,
			PS_REGISTER_V0 | PS_CHANNEL_ALPHA,
			PS_REGISTER_C0 | PS_INPUTMAPPING_SIGNED_NEGATE);
		pixel_shader.rgb_outputs[5] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R1,
			PS_COMBINEROUTPUT_IDENTITY);
		rasterizer_set_pixel_shader(&pixel_shader);
		local_pixel_shader_dirty_flag = FALSE;
	}
	else
	{
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSCONSTANT0_1,
			pixel_shader.constant_0[1]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSCONSTANT0_2,
			pixel_shader.constant_0[2]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSCONSTANT0_5,
			pixel_shader.constant_0[5]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSCONSTANT1_5,
			pixel_shader.constant_1[5]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBINPUTS3,
			pixel_shader.rgb_inputs[3]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBINPUTS6,
			pixel_shader.rgb_inputs[6]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBOUTPUTS6,
			pixel_shader.rgb_outputs[6]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBINPUTS7,
			pixel_shader.rgb_inputs[7]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBOUTPUTS7,
			pixel_shader.rgb_outputs[7]);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERCONSTANT0,
			pixel_shader.final_combiner_constant_0);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERCONSTANT1,
			pixel_shader.final_combiner_constant_1);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERINPUTSABCD,
			pixel_shader.final_combiner_inputs_abcd);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERINPUTSEFG,
			pixel_shader.final_combiner_inputs_efg);

		if (rasterizer_debug_options.statistics_mode ==
			_rasterizer_statistics_mode_enabled)
		{
			rasterizer_frame_statistics.pushbuffer_words += 44;
		}
	}

	return;
}

void rasterizer_model_draw_environment_shader(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index)
{
	struct shader_environment_definition *shader_environment;
	real_vector3d camera_to_model;
	real vertex_constants[3][4];
	real specular_constants[2][4];
	real perpendicular_brightness;
	real_rgb_color perpendicular_color;
	real parallel_brightness;
	real_rgb_color parallel_color;
	real detail_map_scale;
	pixel32 cc0_pixel;
	pixel32 cc1_pixel;
	pixel32 cc0_error_pixel;
	short vertex_shader_permutation;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
		245,
		global_d3d_device);

	if (rasterizer_debug_options.draw_models)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			251,
			local_parameters);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			252,
			shader);

		shader_environment = (struct shader_environment_definition *)
			shader_get_and_verify_type(shader, _shader_type_environment);

		camera_to_model.i = local_parameters->centroid.x -
			global_window_parameters.camera.position.x;
		camera_to_model.j = local_parameters->centroid.y -
			global_window_parameters.camera.position.y;
		camera_to_model.k = local_parameters->centroid.z -
			global_window_parameters.camera.position.z;

		if (TEST_FLAG(
				local_parameters->geometry_flags,
				_rasterizer_geometry_no_zbuffer_bit))
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZENABLE,
				FALSE);
		}
		else
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZFUNC,
				D3DCMP_LESSEQUAL);
		}

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZBIAS,
			0);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			D3DCULL_CCW);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHABLENDENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_SRCBLEND,
			D3DBLEND_SRCALPHA);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_DESTBLEND,
			D3DBLEND_INVSRCALPHA);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_BLENDOP,
			D3DBLENDOP_ADD);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHATESTENABLE,
			TEST_FLAG(
				shader_environment->environment.flags,
				_shader_environment_alpha_tested_bit));
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHAREF,
			0x7F);

		rasterizer_set_texture(
			0,
			0,
			1,
			shader_environment->environment.diffuse.base_map.index,
			shader_permutation_index);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

		rasterizer_set_texture(
			1,
			0,
			2,
			shader_environment->environment.diffuse.primary_detail_map.index,
			shader_permutation_index);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

		rasterizer_set_texture(
			2,
			0,
			1,
			TEST_FLAG(
				shader_environment->environment.flags,
				_shader_environment_alpha_tested_bit) ?
				shader_environment->environment.diffuse.bump_map.index :
				NONE,
			shader_permutation_index);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

		rasterizer_set_texture(
			3,
			2,
			0,
			shader_environment->environment.reflection.cube_map.index,
			shader_permutation_index);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device, 3, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

		if (!TEST_FLAG(
				shader_environment->environment.flags,
				_shader_environment_true_atmospheric_fog_bit))
		{
			if (local_planar_fog_flag)
			{
				vertex_shader_permutation =
					_model_vertex_shader_permutation_planar_fog;
			}
			else if (local_parameters->lighting.point_light_count > 0)
			{
				vertex_shader_permutation =
					_model_vertex_shader_permutation_lights;
			}
			else if (shader_environment->environment.reflection.cube_map.index ==
					NONE &&
				local_parameters->skinning.node_matrix_count <= 1)
			{
				vertex_shader_permutation =
					_model_vertex_shader_permutation_reflection_one_node;
			}
			else
			{
				vertex_shader_permutation =
					_model_vertex_shader_permutation_reflection;
			}
		}
		else
		{
			vertex_shader_permutation =
				_model_vertex_shader_permutation_reflection;
		}

		rasterizer_set_vertex_shader_permutation(
			_rasterizer_vertex_shader_model,
			vertex_buffer ?
				vertex_buffer->type :
				rasterizer_dynamic_vertices_get_type(
					dynamic_vertex_buffer_index),
			vertex_shader_permutation);

		if (rasterizer_debug_options.statistics_mode >=
			_rasterizer_statistics_mode_summary)
		{
			rasterizer_frame_statistics.vertices_by_permutation[
				vertex_shader_permutation] += vertex_buffer->count;
		}

		perpendicular_brightness =
			shader_environment->environment.reflection.view_perpendicular_brightness *
			local_parameters->lighting.reflection_tint_color.alpha;
		perpendicular_color.red =
			shader_environment->environment.specular.view_perpendicular_color.red *
			local_parameters->lighting.reflection_tint_color.red;
		perpendicular_color.green =
			shader_environment->environment.specular.view_perpendicular_color.green *
			local_parameters->lighting.reflection_tint_color.green;
		perpendicular_color.blue =
			shader_environment->environment.specular.view_perpendicular_color.blue *
			local_parameters->lighting.reflection_tint_color.blue;
		parallel_brightness =
			shader_environment->environment.reflection.view_parallel_brightness *
			local_parameters->lighting.reflection_tint_color.alpha;
		parallel_color.red =
			shader_environment->environment.specular.view_parallel_color.red *
			local_parameters->lighting.reflection_tint_color.red;
		parallel_color.green =
			shader_environment->environment.specular.view_parallel_color.green *
			local_parameters->lighting.reflection_tint_color.green;
		parallel_color.blue =
			shader_environment->environment.specular.view_parallel_color.blue *
			local_parameters->lighting.reflection_tint_color.blue;

		detail_map_scale =
			shader_environment->environment.diffuse.primary_detail_map_scale;
		vertex_constants[0][0] = detail_map_scale;
		vertex_constants[0][1] = detail_map_scale;
		vertex_constants[0][2] = 1.0f;
		vertex_constants[0][3] = 1.0f;
		vertex_constants[1][0] = local_parameters->base_map_scale.i;
		vertex_constants[1][1] = 0.0f;
		vertex_constants[1][2] = 0.0f;
		vertex_constants[1][3] = 0.0f;
		vertex_constants[2][0] = 0.0f;
		vertex_constants[2][1] = local_parameters->base_map_scale.j;
		vertex_constants[2][2] = 0.0f;
		vertex_constants[2][3] = 0.0f;
		specular_constants[0][0] =
			perpendicular_color.red - parallel_color.red;
		specular_constants[0][1] =
			perpendicular_color.green - parallel_color.green;
		specular_constants[0][2] =
			perpendicular_color.blue - parallel_color.blue;
		specular_constants[0][3] =
			perpendicular_brightness - parallel_brightness;
		specular_constants[1][0] = parallel_color.red;
		specular_constants[1][1] = parallel_color.green;
		specular_constants[1][2] = parallel_color.blue;
		specular_constants[1][3] = parallel_brightness;
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-84,
			vertex_constants,
			3);
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-81,
			specular_constants,
			2);

		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			436,
			global_window_parameters.fog.atmospheric_maximum_distance>global_window_parameters.fog.atmospheric_minimum_distance);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			437,
			global_window_parameters.fog.atmospheric_maximum_distance>0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			438,
			global_window_parameters.fog.planar_maximum_distance>0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			439,
			global_window_parameters.fog.planar_maximum_depth>0.0f);

		if (rasterizer_debug_options.fog &&
			!TEST_FLAG(
				local_parameters->geometry_flags,
				_rasterizer_geometry_no_fog_bit))
		{
			real planar_fog_fraction;
			real fog_density;
			real_argb_color cc0;
			real_rgb_color cc1;
			real_argb_color cc0_error;

			planar_fog_fraction = PIN(
				(global_window_parameters.camera.position.x *
					global_window_parameters.fog.plane.n.i +
				global_window_parameters.camera.position.y *
					global_window_parameters.fog.plane.n.j +
				global_window_parameters.camera.position.z *
					global_window_parameters.fog.plane.n.k -
				global_window_parameters.fog.plane.d) /
				global_window_parameters.fog.atmospheric_maximum_distance,
				0.0f,
				1.0f);
			fog_density = PIN(
				(camera_to_model.i *
					global_window_parameters.camera.forward.i +
				camera_to_model.j *
					global_window_parameters.camera.forward.j +
				camera_to_model.k *
					global_window_parameters.camera.forward.k -
				global_window_parameters.fog.atmospheric_minimum_distance) /
				(global_window_parameters.fog.atmospheric_maximum_distance -
				global_window_parameters.fog.atmospheric_minimum_distance),
				0.0f,
				1.0f) *
				global_window_parameters.fog.atmospheric_maximum_density;

			if (TEST_FLAG(
					global_window_parameters.fog.fog_definition_flags,
					_fog_definition_atmosphere_dominant_bit))
			{
				planar_fog_fraction = 1.0f;
			}

			cc0.alpha = 1.0f - fog_density;
			cc0.red = global_window_parameters.fog.planar_color.red -
				fog_density *
				(global_window_parameters.fog.planar_color.red *
					planar_fog_fraction +
				global_window_parameters.fog.atmospheric_color.red *
					(1.0f - planar_fog_fraction));
			cc0.green = global_window_parameters.fog.planar_color.green -
				fog_density *
				(global_window_parameters.fog.planar_color.green *
					planar_fog_fraction +
				global_window_parameters.fog.atmospheric_color.green *
					(1.0f - planar_fog_fraction));
			cc0.blue = global_window_parameters.fog.planar_color.blue -
				fog_density *
				(global_window_parameters.fog.planar_color.blue *
					planar_fog_fraction +
				global_window_parameters.fog.atmospheric_color.blue *
					(1.0f - planar_fog_fraction));

			cc0_error.red = PIN(-cc0.red, 0.0f, 1.0f);
			cc0_error.green = PIN(-cc0.green, 0.0f, 1.0f);
			cc0_error.blue = PIN(-cc0.blue, 0.0f, 1.0f);

			cc0.red = PIN(cc0.red, 0.0f, 1.0f);
			cc0.green = PIN(cc0.green, 0.0f, 1.0f);
			cc0.blue = PIN(cc0.blue, 0.0f, 1.0f);

			cc1.red = global_window_parameters.fog.atmospheric_color.red *
				fog_density;
			cc1.green = global_window_parameters.fog.atmospheric_color.green *
				fog_density;
			cc1.blue = global_window_parameters.fog.atmospheric_color.blue *
				fog_density;

			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				478,
				cc0.red >=0.0f && cc0.red <=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				479,
				cc0.green>=0.0f && cc0.green<=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				480,
				cc0.blue >=0.0f && cc0.blue <=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				481,
				cc1.red >=0.0f && cc1.red <=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				482,
				cc1.green>=0.0f && cc1.green<=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				483,
				cc1.blue >=0.0f && cc1.blue <=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				484,
				cc0_error.red >=0.0f && cc0_error.red <=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				485,
				cc0_error.green>=0.0f && cc0_error.green<=1.0f);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				486,
				cc0_error.blue >=0.0f && cc0_error.blue <=1.0f);

			cc0_pixel = real_argb_color_to_pixel32(&cc0);
			cc0_error_pixel = real_rgb_color_to_pixel32(&cc0_error.rgb);
			cc1_pixel = real_rgb_color_to_pixel32(&cc1);
		}
		else
		{
			cc0_pixel = 0xFF000000;
			cc0_error_pixel = 0xFF000000;
			cc1_pixel = 0xFF000000;
		}

		set_environment_shader_pixel_shader(
			0,
			shader_environment->environment.diffuse.detail_map_function,
			_shader_detail_mask_none,
			0xFFFFFFFF,
			cc0_pixel,
			cc0_error_pixel,
			cc1_pixel,
			FALSE,
			(boolean)TEST_FLAG(
				shader_environment->environment.flags,
				_shader_environment_true_atmospheric_fog_bit));

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			D3DCULL_CCW);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBINPUTS0,
			PS_COMBINERINPUTS(
				PS_REGISTER_T2,
				PS_REGISTER_C1,
				PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
				PS_REGISTER_T1 | PS_CHANNEL_ALPHA));
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBOUTPUTS0,
			PS_COMBINEROUTPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_R1,
				PS_REGISTER_DISCARD,
				PS_COMBINEROUTPUT_AB_DOT_PRODUCT));
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERINPUTSEFG,
			PS_COMBINERINPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_C0 | PS_CHANNEL_ALPHA,
				PS_REGISTER_T2 | PS_CHANNEL_ALPHA,
				0));

		rasterizer_draw(
			triangle_buffer,
			dynamic_triangle_buffer_index,
			0,
			triangle_count,
			vertex_buffer,
			dynamic_vertex_buffer_index);

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBINPUTS0,
			PS_COMBINERINPUTS(
				PS_REGISTER_T2,
				PS_REGISTER_C1,
				PS_REGISTER_T2,
				PS_REGISTER_C0));
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSRGBOUTPUTS0,
			PS_COMBINEROUTPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_R1,
				PS_REGISTER_DISCARD,
				PS_COMBINEROUTPUT_AB_DOT_PRODUCT |
					PS_COMBINEROUTPUT_CD_DOT_PRODUCT));
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_PSFINALCOMBINERINPUTSEFG,
			PS_COMBINERINPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_C0 | PS_CHANNEL_ALPHA,
				PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
				0));

		if (rasterizer_debug_options.statistics_mode ==
			_rasterizer_statistics_mode_enabled)
		{
			rasterizer_frame_statistics.pushbuffer_words += 24;
			rasterizer_frame_statistics.model_draw_count++;
			rasterizer_frame_statistics.model_triangle_count += triangle_count;
			rasterizer_frame_statistics.model_vertex_count +=
				rasterizer_frame_statistics_count_static_vertices(
					triangle_buffer,
					vertex_buffer);
		}
	}

	return;
}

void _rasterizer_model_draw(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index)
{
	struct shader_transparent_plasma_definition const *plasma;
	short intensity_exponent_source;
	struct transparent_geometry_group *group;
	struct shader_model_definition *shader_model;
	boolean alpha_blended_decal;
	real_vector3d camera_to_model;
	real camera_distance;
	real reflection_fraction;
	real_rgb_color self_illumination_color;
	real_rgb_color const *external_color;
	real_rgb_color diffuse_change_color;
	unsigned long vertex_shader_work;
	short vertex_shader_permutation;
	real_argb_color perpendicular;
	real_argb_color parallel;
	real_vector4d vertex_constants[3];
	real specular_constants[2][4];
	pixel32 cc0_pixel;
	pixel32 cc0_error_pixel;
	pixel32 cc1_pixel;
	unsigned long seed;
	real self_illumination_phase;
	real self_illumination_animation_fraction;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
		671,
		global_d3d_device);

	if (rasterizer_debug_options.draw_models)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			675,
			local_parameters);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
			676,
			shader);

		if (local_parameters->effect.shader)
		{
			intensity_exponent_source = NONE;

			if (local_parameters->effect.shader->base.type ==
				_shader_type_transparent_plasma)
			{
				plasma = (struct shader_transparent_plasma_definition const *)
					shader_get_and_verify_type(
						local_parameters->effect.shader,
						_shader_type_transparent_plasma);
				intensity_exponent_source = plasma->intensity_exponent_source;
			}

			if (intensity_exponent_source < 1 ||
				intensity_exponent_source >
					NUMBER_OF_SHADER_ANIMATION_FUNCTIONS ||
				!local_parameters->effect.animation.values ||
				local_parameters->effect.animation.values[
					intensity_exponent_source-1] != 0.0f)
			{
				group = _rasterizer_model_transparent_geometry_submit(
					local_parameters->effect.shader,
					shader_permutation_index,
					triangle_buffer,
					dynamic_triangle_buffer_index,
					triangle_count,
					vertex_buffer,
					dynamic_vertex_buffer_index,
					&local_parameters->centroid,
					NULL);

				if (group)
				{
					group->animation = rasterizer_memory_alloc(
						&local_parameters->effect.animation,
						sizeof(struct render_animation));
				}
			}
		}

		if (local_model_effect_type == _render_model_effect_type_active_camouflage)
		{
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				729,
				shader->base.type==_shader_type_model);

			rasterizer_model_transparent_geometry_submit(
				shader,
				shader_permutation_index,
				triangle_buffer,
				dynamic_triangle_buffer_index,
				triangle_count,
				vertex_buffer,
				dynamic_vertex_buffer_index,
				&local_parameters->centroid,
				NULL);
			rasterizer_active_camouflage_set_visibility(TRUE);
		}
		else if (local_model_effect_type == _render_model_effect_type_none)
		{
			if (shader->base.type == _shader_type_environment)
			{
				rasterizer_model_draw_environment_shader(
					shader,
					shader_permutation_index,
					triangle_buffer,
					dynamic_triangle_buffer_index,
					triangle_count,
					vertex_buffer,
					dynamic_vertex_buffer_index);
			}
			else
			{
				shader_model = (struct shader_model_definition *)
					shader_get_and_verify_type(shader, _shader_type_model);
				alpha_blended_decal = (boolean)TEST_FLAG(
					shader_model->model.flags,
					_shader_model_alpha_blended_decal_bit);

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					771,
					local_model_effect_type==_render_model_effect_type_none);

				vector_from_points3d(
					&global_window_parameters.camera.position,
					&local_parameters->centroid,
					&camera_to_model);
				camera_distance = dot_product3d(
					&camera_to_model,
					&global_window_parameters.camera.forward);

				if (shader_model->model.reflection_cutoff_distance != 0.0f)
				{
					reflection_fraction = PIN(
						(camera_distance -
							shader_model->model.reflection_cutoff_distance)/
						(shader_model->model.reflection_falloff_distance -
							shader_model->model.reflection_cutoff_distance),
						0.0f,
						1.0f);
				}
				else
				{
					reflection_fraction = 1.0f;
				}

				if (TEST_FLAG(
						local_parameters->geometry_flags,
						_rasterizer_geometry_no_zbuffer_bit))
				{
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZENABLE,
						FALSE);
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZBIAS,
						0);
				}
				else
				{
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZENABLE,
						TRUE);
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZWRITEENABLE,
						!alpha_blended_decal);
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZFUNC,
						D3DCMP_LESSEQUAL);
					IDirect3DDevice8_SetRenderState(
						global_d3d_device,
						D3DRS_ZBIAS,
						alpha_blended_decal ?
							rasterizer_debug_options.zbias :
							0);
				}

				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_CULLMODE,
					D3DCULL_CCW);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_RED |
					D3DCOLORWRITEENABLE_GREEN |
					D3DCOLORWRITEENABLE_BLUE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHABLENDENABLE,
					alpha_blended_decal);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_SRCBLEND,
					D3DBLEND_SRCALPHA);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_DESTBLEND,
					D3DBLEND_INVSRCALPHA);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_BLENDOP,
					D3DBLENDOP_ADD);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHATESTENABLE,
					!alpha_blended_decal &&
					!TEST_FLAG(
						shader_model->model.flags,
						_shader_model_not_alpha_tested_bit));
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHAREF,
					0x7F);

				rasterizer_set_texture(
					0,
					0,
					1,
					shader_model->model.base_map.index,
					shader_permutation_index);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

				rasterizer_set_texture(
					1,
					0,
					2,
					shader_model->model.detail_map.index,
					shader_permutation_index);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device, 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

				rasterizer_set_texture(
					2,
					0,
					1,
					shader_model->model.multipurpose_map.index,
					shader_permutation_index);
				SetTextureStageStateSmart(
					2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
				SetTextureStageStateSmart(
					2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
				SetTextureStageStateSmart(
					2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
				SetTextureStageStateSmart(
					2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
				SetTextureStageStateSmart(
					2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

				rasterizer_set_texture(
					3,
					2,
					0,
					shader_model->model.reflection_cube_map.index,
					shader_permutation_index);
				SetTextureStageStateSmart(
					3, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
				SetTextureStageStateSmart(
					3, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
				SetTextureStageStateSmart(
					3, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
				SetTextureStageStateSmart(
					3, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
				SetTextureStageStateSmart(
					3, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
				SetTextureStageStateSmart(
					3, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);

				seed = local_parameters->unique_identifier;
				if (TEST_FLAG(
						shader_model->model.self_illumination_flags,
						_shader_model_self_illumination_no_random_phase_bit))
				{
					self_illumination_phase = 0.0f;
				}
				else
				{
					self_illumination_phase = real_seed_random(&seed);
				}

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					864,
					shader_model->model.self_illumination_animation_period!=0.0f);

				self_illumination_animation_fraction =
					periodic_function_evaluate(
						shader_model->model.
							self_illumination_animation_function,
						global_frame_parameters.game_time_sec/
							shader_model->model.
								self_illumination_animation_period +
							self_illumination_phase);
				self_illumination_color.red =
					shader_model->model.
						self_illumination_animation_color_lower_bound.red +
					(shader_model->model.
							self_illumination_animation_color_upper_bound.red -
						shader_model->model.
							self_illumination_animation_color_lower_bound.red) *
					self_illumination_animation_fraction;
				self_illumination_color.green =
					shader_model->model.
						self_illumination_animation_color_lower_bound.green +
					(shader_model->model.
							self_illumination_animation_color_upper_bound.green -
						shader_model->model.
							self_illumination_animation_color_lower_bound.green) *
					self_illumination_animation_fraction;
				self_illumination_color.blue =
					shader_model->model.
						self_illumination_animation_color_lower_bound.blue +
					(shader_model->model.
							self_illumination_animation_color_upper_bound.blue -
						shader_model->model.
							self_illumination_animation_color_lower_bound.blue) *
					self_illumination_animation_fraction;

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					876,
					self_illumination_color.red >=0.0f && self_illumination_color.red <=1.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					877,
					self_illumination_color.green>=0.0f && self_illumination_color.green<=1.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					878,
					self_illumination_color.blue >=0.0f && self_illumination_color.blue <=1.0f);

				if (shader_model->model.self_illumination_color_source>0 &&
					shader_model->model.self_illumination_color_source<
						NUMBER_OF_SHADER_ANIMATION_SOURCES)
				{
					external_color = &local_parameters->animation.colors[
						shader_model->model.self_illumination_color_source-1];

					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						887,
						external_color->red >=0.0f && external_color->red <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						888,
						external_color->green>=0.0f && external_color->green<=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						889,
						external_color->blue >=0.0f && external_color->blue <=1.0f);

					self_illumination_color.red *= external_color->red;
					self_illumination_color.green *= external_color->green;
					self_illumination_color.blue *= external_color->blue;
				}

				if (shader_model->model.change_color_source>0 &&
					shader_model->model.change_color_source<
						NUMBER_OF_SHADER_ANIMATION_SOURCES)
				{
					diffuse_change_color = local_parameters->animation.colors[
						shader_model->model.change_color_source-1];

					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						904,
						diffuse_change_color.red >=0.0f && diffuse_change_color.red <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						905,
						diffuse_change_color.green>=0.0f && diffuse_change_color.green<=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						906,
						diffuse_change_color.blue >=0.0f && diffuse_change_color.blue <=1.0f);
				}
				else
				{
					diffuse_change_color = *global_real_rgb_white;
				}

				vertex_shader_work =
					rasterizer_frame_statistics.vertex_shader_work;

				if (!TEST_FLAG(
						shader_model->model.flags,
						_shader_model_true_atmospheric_fog_bit))
				{
					if (local_planar_fog_flag)
					{
						vertex_shader_permutation =
							_model_vertex_shader_permutation_planar_fog;
					}
					else if (local_parameters->lighting.point_light_count>0)
					{
						vertex_shader_permutation =
							_model_vertex_shader_permutation_lights;
					}
					else if (local_parameters->skinning.node_matrix_count<=1 &&
						(shader_model->model.multipurpose_map.index==NONE ||
						(shader_model->model.detail_mask==
								_shader_detail_mask_none &&
							self_illumination_color.red==0.0f &&
							self_illumination_color.green==0.0f &&
							self_illumination_color.blue==0.0f &&
							diffuse_change_color.red==1.0f &&
							diffuse_change_color.green==1.0f &&
							diffuse_change_color.blue==1.0f)) &&
						(shader_model->model.reflection_cube_map.index==NONE ||
							!(reflection_fraction>0.0f)))
					{
						vertex_shader_permutation =
							_model_vertex_shader_permutation_reflection_one_node;
					}
					else
					{
						vertex_shader_permutation =
							_model_vertex_shader_permutation_reflection;
					}
				}
				else
				{
					vertex_shader_permutation =
						_model_vertex_shader_permutation_reflection;
				}

				rasterizer_set_vertex_shader_permutation(
					_rasterizer_vertex_shader_model,
					vertex_buffer ?
						vertex_buffer->type :
						rasterizer_dynamic_vertices_get_type(
							dynamic_vertex_buffer_index),
					vertex_shader_permutation);

				if (rasterizer_debug_options.statistics_mode >=
					_rasterizer_statistics_mode_summary)
				{
					rasterizer_frame_statistics.vertex_shader_work_accumulated +=
						rasterizer_frame_statistics.vertex_shader_work -
						vertex_shader_work;
					rasterizer_frame_statistics.vertices_by_permutation[
						vertex_shader_permutation] += vertex_buffer->count;
				}

				perpendicular.alpha = reflection_fraction*
					shader_model->model.perpendicular_brightness*
					local_parameters->lighting.reflection_tint_color.alpha;
				perpendicular.red =
					shader_model->model.perpendicular_tint_color.red*
					local_parameters->lighting.reflection_tint_color.red;
				perpendicular.green =
					shader_model->model.perpendicular_tint_color.green*
					local_parameters->lighting.reflection_tint_color.green;
				perpendicular.blue =
					shader_model->model.perpendicular_tint_color.blue*
					local_parameters->lighting.reflection_tint_color.blue;
				parallel.alpha = reflection_fraction*
					shader_model->model.parallel_brightness*
					local_parameters->lighting.reflection_tint_color.alpha;
				parallel.red =
					shader_model->model.parallel_tint_color.red*
					local_parameters->lighting.reflection_tint_color.red;
				parallel.green =
					shader_model->model.parallel_tint_color.green*
					local_parameters->lighting.reflection_tint_color.green;
				parallel.blue =
					shader_model->model.parallel_tint_color.blue*
					local_parameters->lighting.reflection_tint_color.blue;

				vertex_constants[0].n[0] = shader_model->model.detail_map_scale;
				vertex_constants[0].n[1] = shader_model->model.detail_map_scale*
					shader_model->model.detail_map_v_scale;
				vertex_constants[0].n[2] = 1.0f;
				vertex_constants[0].n[3] = 1.0f;
				vertex_constants[1].n[0] = 1.0f;
				vertex_constants[1].n[1] = 0.0f;
				vertex_constants[1].n[2] = 0.0f;
				vertex_constants[1].n[3] = 0.0f;
				vertex_constants[2].n[0] = 0.0f;
				vertex_constants[2].n[1] = 1.0f;
				vertex_constants[2].n[2] = 0.0f;
				vertex_constants[2].n[3] = 0.0f;
				specular_constants[0][0] = perpendicular.red - parallel.red;
				specular_constants[0][1] = perpendicular.green - parallel.green;
				specular_constants[0][2] = perpendicular.blue - parallel.blue;
				specular_constants[0][3] = perpendicular.alpha - parallel.alpha;
				specular_constants[1][0] = parallel.red;
				specular_constants[1][1] = parallel.green;
				specular_constants[1][2] = parallel.blue;
				specular_constants[1][3] = parallel.alpha;

				shader_texture_animation_evaluate(
					&shader_model->model.texture_animation,
					&local_parameters->animation,
					local_parameters->base_map_scale.i*
						shader_model->model.map_u_scale,
					local_parameters->base_map_scale.j*
						shader_model->model.map_v_scale,
					0.0f,
					0.0f,
					0.0f,
					global_frame_parameters.game_time_sec,
					&vertex_constants[1],
					&vertex_constants[2]);
				vertex_constants[2].n[2] = shader_model->model.translucency;

				IDirect3DDevice8_SetVertexShaderConstant(
					global_d3d_device,
					-84,
					vertex_constants,
					3);
				IDirect3DDevice8_SetVertexShaderConstant(
					global_d3d_device,
					-81,
					specular_constants,
					2);

				if (global_rasterizer_model_ambient_reflection_tint &&
					(global_rasterizer_model_ambient_reflection_tint->alpha>0.0f ||
					global_rasterizer_model_ambient_reflection_tint->red>0.0f ||
					global_rasterizer_model_ambient_reflection_tint->green>0.0f ||
					global_rasterizer_model_ambient_reflection_tint->blue>0.0f))
				{
					specular_constants[0][0] = 0.0f;
					specular_constants[0][1] = 0.0f;
					specular_constants[0][2] = 0.0f;
					specular_constants[0][3] = 0.0f;
					specular_constants[1][0] =
						global_rasterizer_model_ambient_reflection_tint->alpha;
					specular_constants[1][1] =
						global_rasterizer_model_ambient_reflection_tint->red;
					specular_constants[1][2] =
						global_rasterizer_model_ambient_reflection_tint->green;
					specular_constants[1][3] =
						global_rasterizer_model_ambient_reflection_tint->blue;

					IDirect3DDevice8_SetVertexShaderConstant(
						global_d3d_device,
						-81,
						specular_constants,
						2);
				}

				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					1074,
					global_window_parameters.fog.atmospheric_maximum_distance>global_window_parameters.fog.atmospheric_minimum_distance);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					1075,
					global_window_parameters.fog.atmospheric_maximum_distance>0.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					1076,
					global_window_parameters.fog.planar_maximum_distance>0.0f);
				match_assert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
					1077,
					global_window_parameters.fog.planar_maximum_depth>0.0f);

				if (rasterizer_debug_options.fog &&
					!TEST_FLAG(
						local_parameters->geometry_flags,
						_rasterizer_geometry_no_fog_bit))
				{
					real_plane3d const *fog_plane =
						&global_window_parameters.fog.plane;
					real camera_plane_distance;
					real planar_fog_fraction;
					real fog_density;
					real_argb_color cc0;
					real_rgb_color cc1;
					real_rgb_color cc0_error;

					camera_plane_distance = plane3d_distance_to_point(
						fog_plane,
						&global_window_parameters.camera.position);
					planar_fog_fraction = PIN(
						camera_plane_distance/
						global_window_parameters.fog.atmospheric_maximum_distance,
						0.0f,
						1.0f);
					fog_density = PIN(
						(camera_distance -
						global_window_parameters.fog.atmospheric_minimum_distance)/
						(global_window_parameters.fog.atmospheric_maximum_distance -
						global_window_parameters.fog.atmospheric_minimum_distance),
						0.0f,
						1.0f) *
						global_window_parameters.fog.atmospheric_maximum_density;

					if (TEST_FLAG(
							global_window_parameters.fog.fog_definition_flags,
						_fog_definition_atmosphere_dominant_bit))
					{
						planar_fog_fraction = 1.0f;
					}

					cc0.alpha = 1.0f - fog_density;
					cc0.red = global_window_parameters.fog.planar_color.red -
						fog_density *
						(global_window_parameters.fog.atmospheric_color.red *
							(1.0f - planar_fog_fraction) +
						global_window_parameters.fog.planar_color.red *
							planar_fog_fraction);
					cc0.green = global_window_parameters.fog.planar_color.green -
						fog_density *
						(global_window_parameters.fog.atmospheric_color.green *
							(1.0f - planar_fog_fraction) +
						global_window_parameters.fog.planar_color.green *
							planar_fog_fraction);
					cc0.blue = global_window_parameters.fog.planar_color.blue -
						fog_density *
						(global_window_parameters.fog.atmospheric_color.blue *
							(1.0f - planar_fog_fraction) +
						global_window_parameters.fog.planar_color.blue *
							planar_fog_fraction);

					cc0_error.red = PIN(-cc0.red, 0.0f, 1.0f);
					cc0_error.green = PIN(-cc0.green, 0.0f, 1.0f);
					cc0_error.blue = PIN(-cc0.blue, 0.0f, 1.0f);

					cc0.red = PIN(cc0.red, 0.0f, 1.0f);
					cc0.green = PIN(cc0.green, 0.0f, 1.0f);
					cc0.blue = PIN(cc0.blue, 0.0f, 1.0f);

					cc1.red = global_window_parameters.fog.atmospheric_color.red *
						fog_density;
					cc1.green = global_window_parameters.fog.atmospheric_color.green *
						fog_density;
					cc1.blue = global_window_parameters.fog.atmospheric_color.blue *
						fog_density;

					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1115,
						cc0.red >=0.0f && cc0.red <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1116,
						cc0.green>=0.0f && cc0.green<=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1117,
						cc0.blue >=0.0f && cc0.blue <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1118,
						cc1.red >=0.0f && cc1.red <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1119,
						cc1.green>=0.0f && cc1.green<=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1120,
						cc1.blue >=0.0f && cc1.blue <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1121,
						cc0_error.red >=0.0f && cc0_error.red <=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1122,
						cc0_error.green>=0.0f && cc0_error.green<=1.0f);
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1123,
						cc0_error.blue >=0.0f && cc0_error.blue <=1.0f);

					cc0_pixel = real_argb_color_to_pixel32(&cc0);
					cc0_error_pixel = real_rgb_color_to_pixel32(&cc0_error);
					cc1_pixel = real_rgb_color_to_pixel32(&cc1);
				}
				else
				{
					cc1_pixel = 0xFF000000;
					cc0_error_pixel = 0xFF000000;
					cc0_pixel = 0xFF000000;
				}

				set_environment_shader_pixel_shader(
					real_rgb_color_to_pixel32(&self_illumination_color),
					shader_model->model.detail_function,
					shader_model->model.detail_mask,
					real_rgb_color_to_pixel32(&diffuse_change_color),
					cc0_pixel,
					cc0_error_pixel,
					cc1_pixel,
					(boolean)TEST_FLAG(
						shader_model->model.flags,
						_shader_model_detail_after_reflection_bit),
					(boolean)TEST_FLAG(
						shader_model->model.flags,
						_shader_model_true_atmospheric_fog_bit));

				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_CULLMODE,
					D3DCULL_CCW);

				rasterizer_draw(
					triangle_buffer,
					dynamic_triangle_buffer_index,
					0,
					triangle_count,
					vertex_buffer,
					dynamic_vertex_buffer_index);

				if (rasterizer_debug_options.statistics_mode ==
					_rasterizer_statistics_mode_enabled)
				{
					rasterizer_frame_statistics.model_draw_count++;
					rasterizer_frame_statistics.model_triangle_count +=
						triangle_count;
					rasterizer_frame_statistics.model_vertex_count +=
						rasterizer_frame_statistics_count_static_vertices(
							triangle_buffer,
							vertex_buffer);
				}

				if (TEST_FLAG(
						shader_model->model.flags,
						_shader_model_two_sided_bit))
				{
					vertex_constants[0].n[0] =
						shader_model->model.detail_map_scale;
					vertex_constants[0].n[1] =
						shader_model->model.detail_map_scale*
						shader_model->model.detail_map_v_scale;
					vertex_constants[0].n[2] = 1.0f;
					vertex_constants[0].n[3] = -1.0f;
					vertex_constants[1].n[0] = 1.0f;
					vertex_constants[1].n[1] = 0.0f;
					vertex_constants[1].n[2] = 0.0f;
					vertex_constants[1].n[3] = 0.0f;
					vertex_constants[2].n[0] = 0.0f;
					vertex_constants[2].n[1] = 1.0f;
					vertex_constants[2].n[2] = 0.0f;
					vertex_constants[2].n[3] = 0.0f;

					shader_texture_animation_evaluate(
						&shader_model->model.texture_animation,
						&local_parameters->animation,
						local_parameters->base_map_scale.i*
							shader_model->model.map_u_scale,
						local_parameters->base_map_scale.j*
							shader_model->model.map_v_scale,
						0.0f,
						0.0f,
						0.0f,
						global_frame_parameters.game_time_sec,
						&vertex_constants[1],
						&vertex_constants[2]);
					vertex_constants[2].n[2] =
						shader_model->model.translucency;

					IDirect3DDevice8_SetVertexShaderConstant(
						global_d3d_device,
						-84,
						vertex_constants,
						3);
					SetRenderStateSmart(D3DRS_CULLMODE, D3DCULL_CW);

					rasterizer_draw(
						triangle_buffer,
						dynamic_triangle_buffer_index,
						0,
						triangle_count,
						vertex_buffer,
						dynamic_vertex_buffer_index);

					if (rasterizer_debug_options.statistics_mode ==
						_rasterizer_statistics_mode_enabled)
					{
						rasterizer_frame_statistics.model_draw_count++;
						rasterizer_frame_statistics.model_triangle_count +=
							triangle_count;
						rasterizer_frame_statistics.model_vertex_count +=
							rasterizer_frame_statistics_count_static_vertices(
								triangle_buffer,
								vertex_buffer);
					}
				}
			}

			if (local_environment_fog_screen_flag)
			{
				rasterizer_environment_fog_screen_model_submit(
					shader,
					shader_permutation_index,
					triangle_buffer,
					dynamic_triangle_buffer_index,
					triangle_count,
					vertex_buffer,
					dynamic_vertex_buffer_index);
			}
		}
		else
		{
			error(
				_error_silent,
				"### ERROR model effect type #%d can't render opaque shader",
				local_model_effect_type);
		}
	}

	return;
}

struct transparent_geometry_group *_rasterizer_model_transparent_geometry_submit(
	struct shader *shader,
	short shader_permutation_index,
	struct triangle_buffer const *triangle_buffer,
	long dynamic_triangle_buffer_index,
	long triangle_count,
	struct vertex_buffer const *vertex_buffer,
	long dynamic_vertex_buffer_index,
	real_point3d const *centroid,
	struct render_sort_filth *sort_filth)
{
	struct transparent_geometry_group *submitted_group = NULL;
	struct transparent_geometry_group *group;
	unsigned long geometry_flags;
	real_vector3d relative_centroid;
	real_plane3d plane;
	boolean alpha_blended_decal;
	boolean submit_decals;

	if (rasterizer_debug_options.draw_models &&
		rasterizer_debug_options.draw_transparent_models)
	{
		alpha_blended_decal =
			shader &&
			shader->base.type==_shader_type_model &&
			TEST_FLAG(
				((struct shader_model_definition *)shader_get_and_verify_type(
					shader,
					_shader_type_model))->model.flags,
				_shader_model_alpha_blended_decal_bit);

		if (local_model_effect_type!=_render_model_effect_type_active_camouflage ||
			(shader &&
			shader->base.type==_shader_type_model &&
			((struct shader_model_definition *)shader_get_and_verify_type(
				shader,
				_shader_type_model))->model.flags))
		{
			submit_decals = TRUE;
		}
		else
		{
			submit_decals = FALSE;
		}

		if (!alpha_blended_decal)
		{
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				1301,
				shader);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				1302,
				shader_type_is_valid_for_model(shader->base.type));
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				1303,
				centroid);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
				1304,
				local_parameters);

			group = NULL;
			geometry_flags = local_parameters->geometry_flags;

			if (submit_decals)
			{
				if (shader_is_decal(shader))
				{
					SET_FLAG(
						geometry_flags,
						_rasterizer_geometry_no_sort_bit,
						TRUE);
					SET_FLAG(
						geometry_flags,
						_rasterizer_geometry_no_queue_bit,
						TRUE);
				}

				if (TEST_FLAG(
						geometry_flags,
						_rasterizer_geometry_no_queue_bit))
				{
					group = &immediate_transparent_geometry_group;
					group->sorted_index = NONE;
				}
			}

			if (!group)
			{
				if (local_model_effect_type==
						_render_model_effect_type_active_camouflage &&
					shader->base.type!=_shader_type_model)
				{
					group = rasterizer_transparent_geometry_new_group2();
				}
				else
				{
					submitted_group =
						rasterizer_transparent_geometry_new_group();
					group = submitted_group;
				}

				if (sort_filth)
				{
					sort_filth->group_index =
						rasterizer_transparent_geometry_get_group_presorted_index(
							group);
					sort_filth->previous_group_presorted_index_reference =
						&group->previous_group_presorted_index;
					sort_filth->next_group_presorted_index_reference =
						&group->next_group_presorted_index;
				}
			}

			if (group)
			{
				plane.n.i = 0.0f;
				plane.n.j = 0.0f;
				plane.n.k = 0.0f;
				plane.d = 0.0f;

				group->geometry_flags = geometry_flags;
				group->object_index = local_parameters->unique_identifier;
				if (local_parameters->effect.type==_render_model_effect_type_none)
				{
					group->source_object_index = 0;
					group->centroid = *centroid;
				}
				else
				{
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1356,
						local_parameters->effect.source_object_index!=0);
					group->source_object_index =
						local_parameters->effect.source_object_index;
					group->centroid = local_parameters->effect.centroid;
				}
				group->shader = shader;
				group->shader_permutation_index = shader_permutation_index;
				group->effect = local_parameters->effect;
				group->dynamic_triangle_buffer_index = dynamic_triangle_buffer_index;
				group->triangle_buffer = triangle_buffer;
				group->first_triangle_index = 0;
				group->triangle_count = triangle_count;
				group->dynamic_vertex_buffer_index = dynamic_vertex_buffer_index;
				group->vertex_buffer = vertex_buffer;
				group->lightmap = NULL;
				vector_from_points3d(
					&global_window_parameters.camera.position,
					&group->centroid,
					&relative_centroid);
				group->z_sort = -dot_product3d(
					&global_window_parameters.camera.forward,
					&relative_centroid);
				group->plane = plane;
				group->model_base_map_scale.i = local_parameters->base_map_scale.i;
				group->model_base_map_scale.j = local_parameters->base_map_scale.j;
				group->previous_group_presorted_index = NONE;
				group->next_group_presorted_index = NONE;
				if (local_model_effect_type==
						_render_model_effect_type_active_camouflage &&
					shader->base.type!=_shader_type_model)
				{
					group->active_camouflage_transparent_source_object_index =
						local_parameters->effect.source_object_index;
					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_models.c",
						1389,
						group->active_camouflage_transparent_source_object_index);
				}
				else
				{
					group->active_camouflage_transparent_source_object_index = 0;
				}
				group->cortana_hack = rasterizer_model_cortana_hack;

				if (TEST_FLAG(
						geometry_flags,
						_rasterizer_geometry_no_queue_bit))
				{
					group->node_matrices =
						local_parameters->skinning.node_matrices;
					group->node_matrix_count =
						local_parameters->skinning.node_matrix_count;
					group->lighting = &local_parameters->lighting;
					group->animation = &local_parameters->animation;

					rasterizer_transparent_geometry_groups_begin();
					rasterizer_transparent_geometry_group_draw(group, FALSE);
					rasterizer_transparent_geometry_groups_end();
					local_pixel_shader_dirty_flag = TRUE;
				}
				else
				{
					if (!local_parameters_queued_flag)
					{
						transparent_geometry_cached_node_matrices =
							rasterizer_memory_alloc_const(
								local_parameters->skinning.node_matrices,
								local_parameters->skinning.node_matrix_count*
									sizeof(real_matrix4x3));
						transparent_geometry_cached_node_matrix_count =
							local_parameters->skinning.node_matrix_count;
						transparent_geometry_cached_lighting =
							rasterizer_memory_alloc_const(
								&local_parameters->lighting,
								sizeof(struct render_lighting));
						transparent_geometry_cached_animation =
							rasterizer_memory_alloc_const(
								&local_parameters->animation,
								sizeof(struct render_animation));
						local_parameters_queued_flag = TRUE;
					}

					group->node_matrices = transparent_geometry_cached_node_matrices;
					group->node_matrix_count =
						transparent_geometry_cached_node_matrix_count;
					group->lighting = transparent_geometry_cached_lighting;
					group->animation = transparent_geometry_cached_animation;
				}

				if (rasterizer_debug_options.statistics_mode==
					_rasterizer_statistics_mode_enabled)
				{
					rasterizer_frame_statistics.transparent_model_submit_count++;
					rasterizer_frame_statistics.transparent_model_triangle_count +=
						triangle_count;
					if (triangle_count>
						rasterizer_frame_statistics.transparent_model_maximum_triangle_count)
					{
						rasterizer_frame_statistics.transparent_model_maximum_triangle_count =
							triangle_count;
					}
					rasterizer_frame_statistics.transparent_model_vertex_count +=
						rasterizer_frame_statistics_count_static_vertices(
							triangle_buffer,
							vertex_buffer);
				}
			}
			else if (!local_reported_too_many_transparent_geometry_groups)
			{
				error(
					_error_silent,
					"### ERROR too many transparent geometry groups");
				local_reported_too_many_transparent_geometry_groups = TRUE;
			}
		}
		else if (sort_filth)
		{
			sort_filth->group_index = NONE;
			sort_filth->previous_group_presorted_index_reference = NULL;
			sort_filth->next_group_presorted_index_reference = NULL;
		}
	}

	return submitted_group;
}
