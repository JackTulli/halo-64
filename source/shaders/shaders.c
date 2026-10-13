/*
SHADERS.C

symbols in this file:
0017FF60 0120:
	_shader_get_vertex_shader_permutation (0000)
00180080 0050:
	_shader_is_mirror (0000)
001800D0 0010:
	_shader_is_destructable (0000)
001800E0 0010:
	_shader_is_double_sided (0000)
001800F0 0090:
	_shader_is_decal (0000)
00180180 0050:
	_shader_is_water_decal (0000)
001801D0 0050:
	_shader_ignores_effect (0000)
00180220 0020:
	_shader_type_is_transparent (0000)
00180240 0020:
	_shader_type_is_lightmapped (0000)
00180260 0020:
	_shader_type_is_vertex_lit (0000)
00180280 0020:
	_shader_type_is_valid_for_environment (0000)
001802A0 0020:
	_shader_type_is_valid_for_model (0000)
001802C0 0020:
	_shader_type_is_valid_for_modifier (0000)
001802E0 0150:
	_shader_environment_texture_animation_evaluate (0000)
00180430 0020:
	_numeric_countdown_timer_set (0000)
00180450 0190:
	_numeric_countdown_timer_get (0000)
001805E0 0010:
	_numeric_countdown_timer_stop (0000)
001805F0 0010:
	_numeric_countdown_timer_restart (0000)
00180600 0060:
	_numeric_countdown_timer_update (0000)
00180660 02e0:
	_shader_texture_animation_evaluate (0000)
002A0878 0021:
	??_C@_0CB@MIIPNOLL@c?3?2halo?2SOURCE?2shaders?2shaders?4c@ (0000)
002A089C 0022:
	??_C@_0CC@DMMMPAKI@diffuse?9?$DOv_animation_period?$CB?$DN0?40@ (0000)
002A08C0 0022:
	??_C@_0CC@JCBEGPL@diffuse?9?$DOu_animation_period?$CB?$DN0?40@ (0000)
002A08E4 0009:
	??_C@_08NPKNFIIJ@v_offset?$AA@ (0000)
002A08F0 0009:
	??_C@_08OGCAGEEM@u_offset?$AA@ (0000)
002A08FC 0016:
	??_C@_0BG@PELELMOE@v_transform_reference?$AA@ (0000)
002A0914 0016:
	??_C@_0BG@MNMMBBKE@u_transform_reference?$AA@ (0000)
002A0930 0063:
	??_C@_0GD@NAFENCB@texture_animation?9?$DOr_source?$DO?$DN0?5?$CG@ (0000)
002A0998 0063:
	??_C@_0GD@FJMMEFHI@texture_animation?9?$DOv_source?$DO?$DN0?5?$CG@ (0000)
002A0A00 0063:
	??_C@_0GD@PNPOEBPC@texture_animation?9?$DOu_source?$DO?$DN0?5?$CG@ (0000)
002A0A64 0012:
	??_C@_0BC@KMIJDGHK@texture_animation?$AA@ (0000)
004C0898 000c:
	_numeric_countdown_timer_milliseconds (0000)
	_numeric_countdown_timer_on (0004)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"
#include "periodic_functions.h"
#include "game.h"
#include "objects.h"
#include "shader_definitions.h"
#include "shaders.h"

/* ---------- constants */

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
	_shader_transparent_decal_bit = 1,
	_shader_transparent_lit_bit = 2,
	_shader_transparent_first_map_is_in_screenspace_bit = 3,
	_shader_transparent_draw_before_water_bit = 4,
	_shader_transparent_ignore_effect_bit = 5,
	_shader_meter_decal_bit = 0,
	_shader_environment_dynamic_mirror_bit = 0,
	_shader_glass_reflection_type_mirror = 2
};

/* ---------- macros */

#define SHADER_GET_EFFECT(shader) \
	((struct shader_effect_permutation_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_effect))
#define SHADER_GET_ENVIRONMENT(shader) \
	((struct shader_environment_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_environment))
#define SHADER_GET_MODEL(shader) \
	((struct shader_model_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_model))
#define SHADER_GET_TRANSPARENT_GENERIC(shader) \
	((struct shader_transparent_generic_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_transparent_generic))
#define SHADER_GET_TRANSPARENT_CHICAGO(shader) \
	((struct shader_transparent_chicago_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_transparent_chicago))
#define SHADER_GET_GLASS(shader) \
	((struct shader_glass_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_transparent_glass))
#define SHADER_GET_METER(shader) \
	((struct shader_meter_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_transparent_meter))

/* ---------- structures */

struct shader_transparent_generic_definition
{
	struct shader shader;
	byte numeric_counter_limit;
	byte flags;
	short first_map_type;
};

struct shader_transparent_chicago_definition
{
	struct shader shader;
	byte numeric_counter_limit;
	byte flags;
	short first_map_type;
};

struct shader_glass_definition
{
	struct shader shader;
	word flags;
	byte reserved_before_reflection_type[0x60];
	short reflection_type;
};

struct shader_meter_definition
{
	struct shader shader;
	word flags;
};

struct shader_effect_permutation_definition
{
	struct shader shader;
	byte reserved_before_secondary_map[0x24];
	struct tag_reference secondary_map;
	short secondary_map_anchor;
};

struct shader_model_definition
{
	struct shader shader;
	word flags;
	short type;
	byte reserved_before_translucency[0xC];
	real translucency;
};

struct shader_environment_diffuse_properties
{
	byte reserved_before_texture_animation[0xE4];
	short u_animation_function;
	short pad_u_animation;
	real u_animation_period;
	real u_animation_scale;
	short v_animation_function;
	short pad_v_animation;
	real v_animation_period;
	real v_animation_scale;
	byte reserved_after_texture_animation[0x18];
};

struct shader_environment_definition
{
	struct shader shader;
	word flags;
	short type;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
	byte reserved_before_diffuse[0x2C];
	struct shader_environment_diffuse_properties diffuse;
	byte reserved_before_reflection[0x150];
	word reflection_flags;
};

struct numeric_countdown_timer_state
{
	boolean on;
	long previous_game_time;
};

typedef char shader_transparent_flags_offset[
	offsetof(struct shader_transparent_generic_definition, flags) == 0x29 ? 1 : -1];
typedef char shader_glass_reflection_type_offset[
	offsetof(struct shader_glass_definition, reflection_type) == 0x8A ? 1 : -1];
typedef char shader_effect_secondary_map_offset[
	offsetof(struct shader_effect_permutation_definition, secondary_map) == 0x4C ? 1 : -1];
typedef char shader_effect_secondary_map_anchor_offset[
	offsetof(struct shader_effect_permutation_definition, secondary_map_anchor) == 0x5C ? 1 : -1];
typedef char shader_model_translucency_offset[
	offsetof(struct shader_model_definition, translucency) == 0x38 ? 1 : -1];
typedef char shader_environment_diffuse_offset[
	offsetof(struct shader_environment_definition, diffuse) == 0x6C ? 1 : -1];
typedef char shader_environment_u_animation_period_offset[
	offsetof(struct shader_environment_definition, diffuse.u_animation_period) == 0x154 ? 1 : -1];
typedef char shader_environment_v_animation_period_offset[
	offsetof(struct shader_environment_definition, diffuse.v_animation_period) == 0x160 ? 1 : -1];
typedef char shader_environment_reflection_flags_offset[
	offsetof(struct shader_environment_definition, reflection_flags) == 0x2D0 ? 1 : -1];
typedef char shader_texture_animation_size[
	sizeof(struct shader_texture_animation) == 0x38 ? 1 : -1];
typedef char render_animation_size[
	sizeof(struct render_animation) == 0x8 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

long numeric_countdown_timer_milliseconds = 0;
struct numeric_countdown_timer_state numeric_countdown_timer_on = { 0 };

/* ---------- public code */

short shader_get_vertex_shader_permutation(
	struct shader const *shader)
{
	short permutation;

	match_assert("c:\\halo\\SOURCE\\shaders\\shaders.c", 20, shader);

	if (shader != (struct shader const *)NONE)
	{
		switch (shader->base.type)
		{
		case _shader_type_model:
		{
			struct shader_model_definition *model = SHADER_GET_MODEL(shader);
			if (model->translucency > 0.0f)
			{
				permutation = 1;
			}
			else
			{
				permutation = 0;
			}
			break;
		}

		case _shader_type_effect:
		{
			struct shader_effect_permutation_definition *effect =
				SHADER_GET_EFFECT(shader);
			if (effect->secondary_map.index != NONE)
			{
				effect = SHADER_GET_EFFECT(shader);
				permutation = effect->secondary_map_anchor + 1;
			}
			else
			{
				permutation = 0;
			}
			break;
		}

		case _shader_type_transparent_generic:
		{
			struct shader_transparent_generic_definition *generic =
				SHADER_GET_TRANSPARENT_GENERIC(shader);
			permutation = generic->first_map_type + 1;
			if (permutation != 1)
			{
				if (TEST_FLAG(shader->base.radiosity.flags, _shader_transparent_lit_bit))
				{
					permutation = 5;
				}
				break;
			}
			generic = SHADER_GET_TRANSPARENT_GENERIC(shader);
			if (!TEST_FLAG(
					generic->flags,
					_shader_transparent_first_map_is_in_screenspace_bit))
			{
				permutation = 0;
			}
			goto test_lit;
		}

		case _shader_type_transparent_chicago:
		{
			struct shader_transparent_chicago_definition *chicago =
				SHADER_GET_TRANSPARENT_CHICAGO(shader);
			permutation = chicago->first_map_type + 1;
			if (permutation != 1)
			{
				goto test_lit;
			}
			chicago = SHADER_GET_TRANSPARENT_CHICAGO(shader);
			if (!TEST_FLAG(
					chicago->flags,
					_shader_transparent_first_map_is_in_screenspace_bit))
			{
				permutation = 0;
			}

		test_lit:
			if (TEST_FLAG(shader->base.radiosity.flags, _shader_transparent_lit_bit))
			{
				permutation = 5;
			}
			break;
		}

		default:
			permutation = 0;
			break;
		}
	}
	else
	{
		permutation = 0;
	}

	return permutation;
}

boolean shader_is_mirror(
	struct shader *shader)
{
	boolean result = FALSE;

	if (shader)
	{
		switch (shader->base.type)
		{
		case _shader_type_environment:
			result = TEST_FLAG(
				SHADER_GET_ENVIRONMENT(shader)->reflection_flags,
				_shader_environment_dynamic_mirror_bit);
			break;

		case _shader_type_transparent_glass:
			result = SHADER_GET_GLASS(shader)->reflection_type ==
				_shader_glass_reflection_type_mirror;
			break;
		}
	}

	return result;
}

boolean shader_is_decal(
	struct shader *shader)
{
	boolean result = FALSE;

	if (shader)
	{
		switch (shader->base.type)
		{
		case _shader_type_transparent_generic:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_GENERIC(shader)->flags,
				_shader_transparent_decal_bit);
			break;

		case _shader_type_transparent_chicago:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_CHICAGO(shader)->flags,
				_shader_transparent_decal_bit);
			break;

		case _shader_type_transparent_glass:
			result = TEST_FLAG(
				SHADER_GET_GLASS(shader)->flags,
				_shader_transparent_decal_bit);
			break;

		case _shader_type_transparent_meter:
			result = TEST_FLAG(
				SHADER_GET_METER(shader)->flags,
				_shader_meter_decal_bit);
			break;
		}
	}

	return result;
}

boolean shader_is_water_decal(
	struct shader *shader)
{
	boolean result = FALSE;

	if (shader)
	{
		switch (shader->base.type)
		{
		case _shader_type_transparent_generic:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_GENERIC(shader)->flags,
				_shader_transparent_draw_before_water_bit);
			break;

		case _shader_type_transparent_chicago:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_CHICAGO(shader)->flags,
				_shader_transparent_draw_before_water_bit);
			break;
		}
	}

	return result;
}

boolean shader_ignores_effect(
	struct shader *shader)
{
	boolean result = FALSE;

	if (shader)
	{
		switch (shader->base.type)
		{
		case _shader_type_transparent_generic:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_GENERIC(shader)->flags,
				_shader_transparent_ignore_effect_bit);
			break;

		case _shader_type_transparent_chicago:
			result = TEST_FLAG(
				SHADER_GET_TRANSPARENT_CHICAGO(shader)->flags,
				_shader_transparent_ignore_effect_bit);
			break;
		}
	}

	return result;
}

boolean shader_is_destructable(
	struct shader *shader)
{
	return FALSE;
}

boolean shader_is_double_sided(
	struct shader *shader)
{
	return FALSE;
}

boolean shader_type_is_transparent(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type==1 || (type>4 && type<=10))
	{
		result = TRUE;
	}

	return result;
}

boolean shader_type_is_lightmapped(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type>=3 && (type<=4 || type==8))
	{
		result = TRUE;
	}

	return result;
}

boolean shader_type_is_vertex_lit(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type==4 || type==8)
	{
		result = TRUE;
	}

	return result;
}

boolean shader_type_is_valid_for_environment(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type==3 || (type>4 && type<=9))
	{
		result = TRUE;
	}

	return result;
}

boolean shader_type_is_valid_for_model(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type>=3 && type<=10)
	{
		result = TRUE;
	}

	return result;
}

boolean shader_type_is_valid_for_modifier(
	short shader_type)
{
	long type = shader_type;
	boolean result = FALSE;

	if (type==1 || (type>4 && type<=10))
	{
		result = TRUE;
	}

	return result;
}

void shader_texture_animation_evaluate(
	struct shader_texture_animation const *texture_animation,
	struct render_animation const *render_animation,
	real u_scale,
	real v_scale,
	real u_offset,
	real v_offset,
	real r_offset,
	real time_value,
	real_vector4d *u_transform_reference,
	real_vector4d *v_transform_reference)
{
	real u_period;
	real v_period;
	real r_period;
	real u_input;
	real v_input;
	real r_input;
	real angle_degrees;
	real cosine_value;
	real sine_value;
	real const *values;

	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		275,
		texture_animation);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		276,
		texture_animation->u_source>=0 &&
		texture_animation->u_source<NUMBER_OF_OBJECT_FUNCTION_REFERENCES);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		277,
		texture_animation->v_source>=0 &&
		texture_animation->v_source<NUMBER_OF_OBJECT_FUNCTION_REFERENCES);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		278,
		texture_animation->r_source>=0 &&
		texture_animation->r_source<NUMBER_OF_OBJECT_FUNCTION_REFERENCES);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		279,
		u_transform_reference);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		280,
		v_transform_reference);

	u_period = texture_animation->u_period == 0.0f ?
		1.0f : texture_animation->u_period;
	v_period = texture_animation->v_period == 0.0f ?
		1.0f : texture_animation->v_period;
	r_period = texture_animation->r_period == 0.0f ?
		1.0f : texture_animation->r_period;

	if (render_animation)
	{
		values = render_animation->values;
		if (texture_animation->u_source == 0)
		{
			u_input = 1.0f;
		}
		else
		{
			u_input = values[texture_animation->u_source - 1];
		}
		if (texture_animation->v_source == 0)
		{
			v_input = 1.0f;
		}
		else
		{
			v_input = values[texture_animation->v_source - 1];
		}
		if (texture_animation->r_source == 0)
		{
			r_input = 1.0f;
		}
		else
		{
			r_input = values[texture_animation->r_source - 1];
		}
	}
	else
	{
		v_input = 1.0f;
		u_input = 1.0f;
		r_input = 1.0f;
	}

	u_input = periodic_function_evaluate(
		texture_animation->u_function,
		(texture_animation->u_phase + time_value) / u_period) *
		texture_animation->u_scale * u_input;
	v_input = periodic_function_evaluate(
		texture_animation->v_function,
		(texture_animation->v_phase + time_value) / v_period) *
		texture_animation->v_scale * v_input;
	r_input = periodic_function_evaluate(
		texture_animation->r_function,
		(texture_animation->r_phase + time_value) / r_period) *
		texture_animation->r_scale * r_input;

	u_input = (u_offset - texture_animation->r_center.x) + u_input;
	v_input = (v_offset - texture_animation->r_center.y) + v_input;
	angle_degrees = r_input + r_offset;

	if (angle_degrees != 0.0f)
	{
		real radians = DEGREES_TO_RADIANS(angle_degrees);
		cosine_value = cosine(radians);
		sine_value = sine(radians);
	}
	else
	{
		cosine_value = 1.0f;
		sine_value = 0.0f;
	}

	u_transform_reference->n[2] = 0.0f;
	u_transform_reference->n[0] = cosine_value * u_scale;
	u_transform_reference->n[1] = -(sine_value * v_scale);
	u_transform_reference->n[3] =
		cosine_value * u_input - sine_value * v_input +
		texture_animation->r_center.x;

	v_transform_reference->n[0] = sine_value * u_scale;
	v_transform_reference->n[1] = cosine_value * v_scale;
	v_transform_reference->n[2] = 0.0f;
	v_transform_reference->n[3] =
		cosine_value * v_input + sine_value * u_input +
		texture_animation->r_center.y;

	return;
}

void shader_environment_texture_animation_evaluate(
	struct shader const *shader,
	real time_value,
	real *u_offset,
	real *v_offset)
{
	struct shader_environment_diffuse_properties *diffuse;

	match_assert("c:\\halo\\SOURCE\\shaders\\shaders.c", 345, shader);
	match_assert("c:\\halo\\SOURCE\\shaders\\shaders.c", 346, u_offset);
	match_assert("c:\\halo\\SOURCE\\shaders\\shaders.c", 347, v_offset);

	diffuse = &SHADER_GET_ENVIRONMENT(shader)->diffuse;

	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		352,
		diffuse->u_animation_period!=0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\shaders\\shaders.c",
		353,
		diffuse->v_animation_period!=0.0f);

	*u_offset = periodic_function_evaluate(
		diffuse->u_animation_function,
		time_value / diffuse->u_animation_period) * diffuse->u_animation_scale;
	*v_offset = periodic_function_evaluate(
		diffuse->v_animation_function,
		time_value / diffuse->v_animation_period) * diffuse->v_animation_scale;

	return;
}

void numeric_countdown_timer_set(
	long milliseconds,
	boolean auto_start)
{
	numeric_countdown_timer_milliseconds = milliseconds;
	numeric_countdown_timer_on.on = auto_start;

	return;
}

short numeric_countdown_timer_get(
	short digit_index)
{
	long digit = 0;

	switch (digit_index)
	{
	case NONE:
		digit = numeric_countdown_timer_milliseconds;
		break;
	case 0:
		digit = numeric_countdown_timer_milliseconds % 10;
		break;
	case 1:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND/100) % 10;
		break;
	case 2:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND/10) % 10;
		break;
	case 3:
		digit = numeric_countdown_timer_milliseconds / MILLISECONDS_PER_SECOND % 10;
		break;
	case 4:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND*10) % 6;
		break;
	case 5:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND*SECONDS_PER_MINUTE) % 10;
		break;
	case 6:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND*SECONDS_PER_MINUTE*10) % 6;
		break;
	case 7:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND*SECONDS_PER_MINUTE*MINUTES_PER_HOUR) % 10;
		break;
	case 8:
		digit = numeric_countdown_timer_milliseconds / (MILLISECONDS_PER_SECOND*SECONDS_PER_MINUTE*MINUTES_PER_HOUR*10) % 10;
		break;
	}

	return digit;
}

void numeric_countdown_timer_stop(
	void)
{
	numeric_countdown_timer_on.on = FALSE;

	return;
}

void numeric_countdown_timer_restart(
	void)
{
	numeric_countdown_timer_on.on = TRUE;

	return;
}

void numeric_countdown_timer_update(
	void)
{
	if (numeric_countdown_timer_on.on)
	{
		long game_time = MILLISECONDS_PER_SECOND * game_time_get() / TICKS_PER_SECOND;

		if (game_time >= numeric_countdown_timer_on.previous_game_time)
		{
			numeric_countdown_timer_milliseconds = numeric_countdown_timer_milliseconds - game_time + numeric_countdown_timer_on.previous_game_time;

			if (numeric_countdown_timer_milliseconds < 0)
			{
				numeric_countdown_timer_milliseconds = 0;
			}
		}

		numeric_countdown_timer_on.previous_game_time = game_time;
	}

	return;
}

/* ---------- private code */
