/*
EDITOR_FLYING_CAMERA.C

symbols in this file:
00075E70 00d0:
	_editor_camera_new (0000)
00075F40 0080:
	_editor_camera_get_focus (0000)
00075FC0 0080:
	_editor_camera_set_focus (0000)
00076040 00a0:
	_editor_camera_set_position (0000)
000760E0 0040:
	_editor_camera_bump_speed (0000)
00076120 0010:
	_editor_camera_get_speed (0000)
00076130 0030:
	_editor_camera_use_roll (0000)
00076160 0010:
	_editor_camera_get_unit_focus (0000)
00076170 00e0:
	_editor_camera_set_mode (0000)
00076250 0010:
	_editor_camera_get_mode (0000)
00076260 0010:
	_editor_camera_get_scripted (0000)
00076270 0070:
	_code_00076270 (0000)
000762E0 0010:
	_editor_camera_get_field_of_view (0000)
000762F0 0080:
	_editor_camera_move_to_point (0000)
00076370 01b0:
	_editor_camera_set_position_and_roll (0000)
00076520 0070:
	_editor_camera_set_unit_focus (0000)
00076590 0130:
	_editor_camera_update (0000)
000766C0 01b0:
	_editor_camera_set_scripted (0000)
00076870 0060:
	_code_00076870 (0000)
000768D0 0580:
	_code_000768d0 (0000)
00076E50 0460:
	_code_00076e50 (0000)
00256C64 0030:
	_rdata_00256c64 (0000)
00256C94 0008:
	??_C@_07NACDKLFL@exiting?$AA@ (0000)
00256C9C 0010:
	??_C@_0BA@NDFANOEE@orbiting?5camera?$AA@ (0000)
00256CAC 000e:
	??_C@_0O@GNANFALA@flying?5camera?$AA@ (0000)
00256CBC 0007:
	??_C@_06GNEAMJOC@angles?$AA@ (0000)
00256CC4 0009:
	??_C@_08OAGMDKAF@position?$AA@ (0000)
00256CD0 002d:
	??_C@_0CN@ELKCHNLL@c?3?2halo?2SOURCE?2camera?2editor_fly@ (0000)
00256D00 0011:
	??_C@_0BB@JCBBBJE@speed?5is?5now?5x?$CFf?$AA@ (0000)
00256D14 0025:
	??_C@_0CF@ONCNFIFO@translate_funcs?$FLmode?$FN?$FL_translate@ (0000)
00256D3C 002e:
	??_C@_0CO@LELHEFFL@translate_funcs?$FLcamera_mode?$FN?$FL_tr@ (0000)
00256D6C 001a:
	??_C@_0BK@JFLLMCBA@update_funcs?$FLcamera_mode?$FN?$AA@ (0000)
00256D88 0018:
	??_C@_0BI@CHKFHLBI@?$CFs?5scripted?5camera?5mode?$AA@ (0000)
00256DA0 0004:
	__real@3fc8a8ea (0000)
00256DA4 0004:
	__real@bfc8a8ea (0000)
00256DA8 0009:
	??_C@_08JLGBNAMD@controls?$AA@ (0000)
00256DB4 0004:
	__real@3fa0d97c (0000)
00256DB8 0004:
	__real@bfa0d97c (0000)
002DCC28 0034:
	_data_002dcc28 (0000)
	_editor_custom_render (0008)
0031D438 007c:
	_bss_0031d438 (0000)
*/

/* ---------- headers */

#include "editor_flying_camera.h"
#include "flying_camera.h"
#include "camera_scripting.h"
#include "director.h"
#include "game/player_control.h"
#include "main/console.h"
#include "observer.h"
#include "static_camera.h"
#include "render/render.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"

/* ---------- constants */

enum
{
	NUMBER_OF_EDITOR_CAMERA_SPEED_STEPS = 5
};

enum editor_camera_mode
{
	_editor_camera_mode_flying,
	_editor_camera_mode_orbiting,
	NUMBER_OF_EDITOR_CAMERA_MODES
};

enum editor_camera_translation
{
	_translate_from,
	_translate_to,
	NUMBER_OF_EDITOR_CAMERA_TRANSLATIONS
};

enum editor_camera_persisted_camera_slot
{
	_editor_camera_persisted_flying,
	_editor_camera_persisted_orbiting,
	NUMBER_OF_EDITOR_CAMERA_PERSISTED_CAMERA_SLOTS
};

/* ---------- macros */

/* ---------- structures */

struct editor_camera_focus_definition
{
	real_point3d position;
	real_euler_angles2d angles;
};

struct persisted_camera_data
{
	struct flying_camera camera_data;
	boolean saved;
};

struct editor_camera_player_starting_location
{
	real_point3d position;
	real facing;
	short team_index;
	short structure_bsp_reference_index;
	byte _unknown14[0x20];
};

typedef char editor_camera_player_starting_location_size_assert[
	sizeof(struct editor_camera_player_starting_location) == 0x34 ? 1 : -1];

typedef void (*editor_camera_update_function)(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result);
typedef void (*editor_camera_translate_function)(
	struct flying_camera *camera);

/* ---------- prototypes */

static void editor_camera_flying_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result);
static void editor_camera_orbiting_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result);
static void translate_flying_to_orbiting(
	struct flying_camera *camera);
static void translate_orbiting_to_flying(
	struct flying_camera *camera);

/* ---------- globals */

/* January kept these as file-scope scalars and arrays; VC7 hoists loads of
   non-address-taken statics across pointer stores where it reloads aggregates.
   The .data and .bss statics follow the definition order and names recorded for
   this translation unit in the HCEX PDB; January's .bss offsets (+0x00 .. +0x78)
   and its camera_mode assertion text agree. The speed scalar is the .data
   anchor; is_scripted is a private .bss anchor as in January. The 124-byte
   .bss reserves local_player_index at +0x30. */
static real editor_camera_speed = 1.f;
static long unit_focus = NONE;
static boolean is_scripted = FALSE;
static boolean use_roll = FALSE;
static boolean initialized = FALSE;
static struct editor_camera_focus_definition editor_camera_focus = { 0 };
static struct flying_camera *editor_camera = NULL;
static boolean reset_all = FALSE;
static real_vector3d unit_offset = { 0 };
static short camera_mode = _editor_camera_mode_flying;
static short local_player_index = 0;
static boolean last_scripted = FALSE;
static struct persisted_camera_data persisted_cameras[NUMBER_OF_EDITOR_CAMERA_PERSISTED_CAMERA_SLOTS] = { 0 };
static unsigned long speed_step = 0;
static long const editor_camera_speed_steps[NUMBER_OF_EDITOR_CAMERA_SPEED_STEPS] = { 1, 5, 20, 40, 60 };
static real const orbiting_camera_field_of_view = DEGREES_TO_RADIANS(70.f);
static real const orbiting_camera_scale = 1.f;
static real const orbiting_camera_timer = 0.5f;
static real const orbiting_camera_vertical_offset = 0.52f;
static real const orbiting_camera_default_distance = 1.f;
static real const editor_camera_field_of_view[NUMBER_OF_EDITOR_CAMERA_MODES] =
{
	DEGREES_TO_RADIANS(70.f),
	DEGREES_TO_RADIANS(70.f)
};

struct render_globals *editor_custom_render = &render;
static editor_camera_update_function update_funcs[NUMBER_OF_EDITOR_CAMERA_MODES] =
{
	editor_camera_flying_update,
	editor_camera_orbiting_update
};
static editor_camera_translate_function translate_funcs[NUMBER_OF_EDITOR_CAMERA_MODES][NUMBER_OF_EDITOR_CAMERA_TRANSLATIONS] =
{
	{ NULL, NULL },
	{ translate_orbiting_to_flying, translate_flying_to_orbiting }
};
static char const *mode_names[NUMBER_OF_EDITOR_CAMERA_MODES] =
{
	"flying camera",
	"orbiting camera"
};
static char const *scripted_names[2] =
{
	"exiting",
	"entering"
};

/* ---------- public code */

void editor_camera_new(
	struct flying_camera *camera,
	short local_player_index)
{
	real_vector3d forward;

	if (!initialized)
	{
		if (global_scenario_get()->players.count &&
			global_scenario_get()->players.address)
		{
			struct editor_camera_player_starting_location *starting_location =
				TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->players,
					0,
					struct editor_camera_player_starting_location);

			editor_camera_focus.position = starting_location->position;
			editor_camera_focus.angles.yaw = starting_location->facing;
		}
		else
		{
			csmemset(
				&editor_camera_focus,
				0,
				sizeof(editor_camera_focus));
		}
	}

	initialized = TRUE;
	vector3d_from_euler_angles2d(
		&forward,
		&editor_camera_focus.angles);
	flying_camera_new_from_point_and_vector(
		camera,
		&editor_camera_focus.position,
		&forward);

	if (!local_player_index)
		editor_camera = camera;
	if (camera_mode)
	{
		translate_funcs[camera_mode][_translate_to](
			camera);
	}

	return;
}

void editor_camera_get_focus(
	real_point3d *position,
	real_euler_angles2d *angles)
{
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 120, position);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 121, angles);

	*position = editor_camera_focus.position;
	*angles = editor_camera_focus.angles;

	return;
}

void editor_camera_set_focus(
	real_point3d const *position,
	real_euler_angles2d const *angles)
{
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 129, position);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 130, angles);

	editor_camera_focus.position = *position;
	editor_camera_focus.angles = *angles;

	return;
}

void editor_camera_move_to_point(
	real_point3d const *point)
{
	struct flying_camera *camera;
	real_vector3d forward;

	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 139, point);

	vector3d_from_euler_angles2d(
		&forward,
		&editor_camera->facing);
	camera = editor_camera;
	camera->position.x = point->x - forward.i * 2.5f;
	camera->position.y = point->y - forward.j * 2.5f;
	camera->position.z = point->z - forward.k * 2.5f;

	return;
}

void editor_camera_set_position(
	real_point3d const *point,
	real_euler_angles2d const *angles)
{
	struct flying_camera *camera;

	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 148, point);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 149, angles);

	camera = editor_camera;
	if (!camera)
	{
		editor_camera_set_focus(point, angles);
		initialized = TRUE;
	}
	else
	{
		camera->position = *point;
		camera->facing.yaw = angles->yaw;
		camera->facing.pitch = angles->pitch;
	}

	return;
}

void editor_camera_set_position_and_roll(
	real_point3d const *point,
	real_euler_angles3d const *angles)
{
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 169, point);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 170, angles);

	if (!editor_camera)
	{
		/* the yaw/pitch prefix of the 3d angles is the focus orientation */
		editor_camera_set_focus(point, (real_euler_angles2d const *)angles);
		initialized = TRUE;
	}
	else
	{
		real_matrix4x3 matrix;
		real_vector3d forward;
		real_vector3d left;
		real_vector3d up;
		real_vector3d diff;
		real_euler_angles2d hack_angles;

		editor_camera->position = *point;
		matrix4x3_rotation_from_angles(
			&matrix,
			angles->yaw,
			angles->pitch,
			angles->roll);
		euler_angles2d_from_vector3d(
			&editor_camera->facing,
			&matrix.forward);
		vector3d_from_euler_angles2d(
			&forward,
			&editor_camera->facing);

		hack_angles = editor_camera->facing;
		hack_angles.pitch += _pi / 2.f;
		vector3d_from_euler_angles2d(
			&up,
			&hack_angles);
		normalize3d(&forward);
		normalize3d(&up);
		/* left is never read; the later first-party build still computes and normalizes it */
		cross_product3d(
			&up,
			&forward,
			&left);
		normalize3d(&left);
		cross_product3d(
			&up,
			&matrix.up,
			&diff);
		normalize3d(&diff);
		editor_camera->roll = angle_between_vectors3d(&up, &matrix.up) *
			dot_product3d(&forward, &diff);

		if (unit_focus != NONE)
		{
			unit_offset.i = point->x;
			unit_offset.j = point->y;
			unit_offset.k = point->z;
		}
	}

	reset_all = TRUE;

	return;
}

void editor_camera_set_unit_focus(
	long unit_index)
{
	struct flying_camera *camera = editor_camera;

	unit_focus = unit_index;
	if (!camera)
		return;

	if (unit_index != NONE)
	{
		struct object_datum *unit = object_get(unit_index);

		vector_from_points3d(
			&unit->object.bounding_sphere_center,
			&camera->position,
			&unit_offset);
	}
	else
	{
		unit_offset = *global_zero_vector3d;
	}

	return;
}

void editor_camera_bump_speed(
	void)
{
	speed_step = (speed_step + 1) % NUMBER_OF_EDITOR_CAMERA_SPEED_STEPS;
	editor_camera_speed =
		(real)editor_camera_speed_steps[speed_step];
	terminal_printf(
		global_real_argb_white,
		"speed is now x%f",
		editor_camera_speed);

	return;
}

long editor_camera_get_speed(
	void)
{
	return (long)editor_camera_speed;
}

boolean editor_camera_use_roll(
	boolean new_use_roll)
{
	boolean previous_use_roll = use_roll;

	use_roll = new_use_roll;
	if (!new_use_roll && editor_camera)
		editor_camera->roll = 0.0f;

	return previous_use_roll;
}

long editor_camera_get_unit_focus(
	void)
{
	return unit_focus;
}

short editor_camera_get_mode(
	void)
{
	return camera_mode;
}

boolean editor_camera_get_scripted(
	void)
{
	return is_scripted;
}

real editor_camera_get_field_of_view(
	void)
{
	return editor_camera_field_of_view[camera_mode];
}

void editor_camera_set_mode(
	short mode)
{
	if (editor_camera && camera_mode != mode)
	{
		if (camera_mode)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
				302,
				translate_funcs[camera_mode][_translate_from],
				"translate_funcs[camera_mode][_translate_from]");
			translate_funcs[camera_mode][_translate_from](
				editor_camera);
		}
		if (mode)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
				308,
				translate_funcs[mode][_translate_to],
				"translate_funcs[mode][_translate_to]");
			translate_funcs[mode][_translate_to](
				editor_camera);
		}
	}

	camera_mode = mode;
	console_printf(FALSE, mode_names[mode]);

	return;
}

void editor_camera_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result)
{
	match_dassert(
		"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
		340,
		update_funcs[camera_mode],
		"update_funcs[camera_mode]");

	if (is_scripted)
	{
		if (!controls->active)
		{
			scripted_camera_update(NULL, controls, result);
			return;
		}

		editor_camera->position = editor_custom_render->camera.position;
		euler_angles2d_from_vector3d(
			&editor_camera->facing,
			&editor_custom_render->camera.forward);
		editor_camera_set_unit_focus(unit_focus);
		if (camera_mode)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
				356,
				translate_funcs[camera_mode][_translate_from],
				"translate_funcs[camera_mode][_translate_from]");
			translate_funcs[camera_mode][_translate_to](
				editor_camera);
		}
	}

	update_funcs[camera_mode](camera, controls, result);
	if (is_scripted)
	{
		long flags = result->flags;

		SET_FLAG(flags, _observer_command_valid_bit, TRUE);
		SET_FLAG(flags, _observer_command_force_time_bit, TRUE);
		result->timer = 0.f;
		result->flags = flags;
	}

	return;
}

void editor_camera_set_scripted(
	boolean scripted)
{
	if (scripted)
	{
		real_euler_angles2d angles;

		if (camera_mode)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
				387,
				translate_funcs[camera_mode][_translate_from],
				"translate_funcs[camera_mode][_translate_from]");
			translate_funcs[camera_mode][_translate_from](
				editor_camera);
		}

		euler_angles2d_from_vector3d(
			&angles,
			&editor_custom_render->camera.forward);
		editor_camera_set_position(
			&editor_custom_render->camera.position,
			&angles);

		if (unit_focus != NONE)
		{
			scripted_camera_set_camera_point_relative(
				(real_point3d const *)&unit_offset,
				&editor_custom_render->camera.forward,
				&editor_custom_render->camera.up,
				DEGREES_TO_RADIANS(70.f),
				0,
				unit_focus);
		}
		else
		{
			scripted_camera_set_camera_point_relative(
				&editor_custom_render->camera.position,
				&editor_custom_render->camera.forward,
				&editor_custom_render->camera.up,
				DEGREES_TO_RADIANS(70.f),
				0,
				NONE);
		}
	}
	else
	{
		editor_camera->position = editor_custom_render->camera.position;
		euler_angles2d_from_vector3d(
			&editor_camera->facing,
			&editor_custom_render->camera.forward);
		editor_camera_set_unit_focus(unit_focus);
		if (camera_mode)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
				414,
				translate_funcs[camera_mode][_translate_from],
				"translate_funcs[camera_mode][_translate_from]");
			translate_funcs[camera_mode][_translate_to](
				editor_camera);
		}
	}

	last_scripted = is_scripted;
	is_scripted = scripted;
	console_printf(
		FALSE,
		"%s scripted camera mode",
		scripted_names[scripted]);

	return;
}

/* ---------- private code */

static void editor_camera_flying_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result)
{
	real_vector3d right;
	real_vector3d translation;
	real_point3d position;

	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 448, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 449, controls);
	match_assert("c:\\halo\\SOURCE\\camera\\editor_flying_camera.c", 450, result);

	if (controls->active)
	{
		camera->facing.yaw += controls->facing_delta.yaw;
		camera->facing.pitch = PIN(
			camera->facing.pitch + controls->facing_delta.pitch,
			-1.56765485f,
			1.56765485f);
		if (use_roll)
			camera->roll += controls->facing_delta.roll;
		else
			camera->roll = 0.f;
	}

	result->timer = 0.3f;
	vector3d_from_euler_angles2d(&result->forward, &camera->facing);
	right.i = result->forward.j;
	right.j = -result->forward.i;
	right.k = 0.f;
	if (normalize3d(&right) == 0.f)
	{
		right.i = 1.f;
		right.j = right.k = 0.f;
	}
	cross_product3d(&right, &result->forward, &result->up);
	rotate_vector_about_axis(
		&result->up,
		&result->forward,
		sine(camera->roll),
		cosine(camera->roll));

	{
		real cosine_yaw = cosine(camera->facing.yaw);
		real sine_yaw = sine(camera->facing.yaw);

		set_real_vector3d(
			&translation,
			cosine_yaw*controls->translation.i - sine_yaw*controls->translation.j,
			cosine_yaw*controls->translation.j + sine_yaw*controls->translation.i,
			controls->translation.k);
		scale_vector3d(&translation, editor_camera_speed, &translation);
	}

	if (unit_focus != NONE && object_try_and_get(unit_focus))
	{
		struct object_datum *unit;

		unit_offset.i += translation.i;
		unit_offset.j += translation.j;
		unit_offset.k += translation.k;
		unit = object_get(unit_focus);
		point_from_line3d(
			&unit->object.bounding_sphere_center,
			&unit_offset,
			1.f,
			&position);
	}
	else
	{
		point_from_line3d(
			&camera->position,
			&translation,
			1.f,
			&position);
	}
	camera->position = position;
	result->position = position;
	result->offset = *global_zero_vector3d;
	result->depth = 0.f;
	result->field_of_view = 1.2217305f;
	result->flags = FLAG(_observer_command_valid_bit);

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
		518,
		!TEST_FLAG(result->flags, _observer_command_valid_bit) ||
		(valid_real_vector3d_axes2(&result->forward, &result->up) &&
			valid_real(result->position.x) && result->position.x>=-5000.f && result->position.x<=5000.f &&
			valid_real(result->position.y) && result->position.y>=-5000.f && result->position.y<=5000.f &&
			valid_real(result->position.z) && result->position.z>=-5000.f && result->position.z<=5000.f &&
			valid_real(result->offset.i) && result->offset.i>=-5000.f && result->offset.i<=5000.f &&
			valid_real(result->offset.j) && result->offset.j>=-5000.f && result->offset.j<=5000.f &&
			valid_real(result->offset.k) && result->offset.k>=-5000.f && result->offset.k<=5000.f &&
			valid_real_vector3d(&result->velocity) &&
			valid_real(result->depth) && result->depth>=0.f && result->depth<=5000.f &&
			valid_real(result->field_of_view) && result->field_of_view>=0.001f && result->field_of_view<=_pi / 2.f &&
			valid_real(result->timer) && result->timer>=0.f && result->timer<=3600.f),
		csprintf(
			temporary,
			"Invalid camera command.\nF: (%f, %f, %f) U: (%f, %f, %f)\nP: (%f, %f, %f) O: (%f, %f, %f)\nD: %f V: (%f, %f, %f), FOV: %f, T: %f, FL: %ld",
			result->forward.i,
			result->forward.j,
			result->forward.k,
			result->up.i,
			result->up.j,
			result->up.k,
			result->position.x,
			result->position.y,
			result->position.z,
			result->offset.i,
			result->offset.j,
			result->offset.k,
			result->depth,
			result->velocity.i,
			result->velocity.j,
			result->velocity.k,
			result->field_of_view,
			result->timer,
			result->flags));

	return;
}

static void editor_camera_orbiting_update(
	struct flying_camera *camera,
	struct flying_camera_action const *controls,
	struct camera_command *result)
{
	struct player_control_unit_camera_info camera_info;

	player_control_get_unit_camera_info(
		controls->local_player_index,
		&camera_info);
	result->position = camera_info.position;

	if (controls->active)
	{
		camera->facing.yaw += controls->facing_delta.yaw;
		camera->facing.pitch = PIN(
			camera->facing.pitch + controls->facing_delta.pitch,
			-1.2566371f,
			1.2566371f);
		director_inhibit_input(controls->local_player_index);
	}

	camera->position.y = MAX(
		camera->position.y - controls->wheel_delta / 3.f,
		0.6f);

	if (camera_info.unit_index != NONE)
	{
		vector3d_from_euler_angles2d(
			&result->forward,
			&camera->facing);
		observer_up_from_forward(&result->forward, &result->up);
		object_get_velocities(
			camera_info.unit_index,
			&result->velocity,
			NULL);
		result->flags = FLAG(_observer_command_valid_bit);
	}

	result->offset = *global_zero_vector3d;
	result->depth = camera->position.y;
	result->field_of_view = orbiting_camera_field_of_view;
	result->timer = orbiting_camera_timer;

	match_vassert(
		"c:\\halo\\SOURCE\\camera\\editor_flying_camera.c",
		571,
		!TEST_FLAG(result->flags, _observer_command_valid_bit) ||
		(valid_real_vector3d_axes2(&result->forward, &result->up) &&
			valid_real(result->position.x) && result->position.x>=-5000.f && result->position.x<=5000.f &&
			valid_real(result->position.y) && result->position.y>=-5000.f && result->position.y<=5000.f &&
			valid_real(result->position.z) && result->position.z>=-5000.f && result->position.z<=5000.f &&
			valid_real(result->offset.i) && result->offset.i>=-5000.f && result->offset.i<=5000.f &&
			valid_real(result->offset.j) && result->offset.j>=-5000.f && result->offset.j<=5000.f &&
			valid_real(result->offset.k) && result->offset.k>=-5000.f && result->offset.k<=5000.f &&
			valid_real_vector3d(&result->velocity) &&
			valid_real(result->depth) && result->depth>=0.f && result->depth<=5000.f &&
			valid_real(result->field_of_view) && result->field_of_view>=0.001f && result->field_of_view<=_pi / 2.f &&
			valid_real(result->timer) && result->timer>=0.f && result->timer<=3600.f),
		csprintf(
			temporary,
			"Invalid camera command.\nF: (%f, %f, %f) U: (%f, %f, %f)\nP: (%f, %f, %f) O: (%f, %f, %f)\nD: %f V: (%f, %f, %f), FOV: %f, T: %f, FL: %ld",
			result->forward.i,
			result->forward.j,
			result->forward.k,
			result->up.i,
			result->up.j,
			result->up.k,
			result->position.x,
			result->position.y,
			result->position.z,
			result->offset.i,
			result->offset.j,
			result->offset.k,
			result->depth,
			result->velocity.i,
			result->velocity.j,
			result->velocity.k,
			result->field_of_view,
			result->timer,
			result->flags));

	return;
}


static void translate_orbiting_to_flying(
	struct flying_camera *camera)
{
	persisted_cameras[_editor_camera_persisted_orbiting].camera_data = *camera;
	persisted_cameras[_editor_camera_persisted_orbiting].saved = TRUE;

	camera->position = editor_custom_render->camera.position;
	euler_angles2d_from_vector3d(
		&camera->facing,
		&editor_custom_render->camera.forward);
	editor_camera_set_unit_focus(unit_focus);

	return;
}

static void translate_flying_to_orbiting(
	struct flying_camera *camera)
{
	persisted_cameras[_editor_camera_persisted_flying].camera_data = *camera;

	if (persisted_cameras[_editor_camera_persisted_orbiting].saved)
	{
		*camera = persisted_cameras[_editor_camera_persisted_orbiting].camera_data;
	}
	else
	{
		camera->position.x = 0.f;
		camera->position.y = orbiting_camera_default_distance;
		camera->position.z = 0.f;
		euler_angles2d_from_vector3d(
			&camera->facing,
			&editor_custom_render->camera.forward);
	}

	return;
}
