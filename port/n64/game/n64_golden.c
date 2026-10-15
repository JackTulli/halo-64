/* Four local sidecars; existing ABI-asserted actions/control structs stay intact. */
#include "cseries.h"
#include <math.h>
#include <string.h>
#include "game/players.h"
#include "game/local_players.h"
#include "game/player_control.h"
#include "game/aim_assist.h"
#include "items/weapons.h"
#include "items/weapon_definitions.h"
#include "units/units.h"
#include "render/render.h"
#include "camera/director.h"
#include "physics/collisions.h"
#include "../src/n64.h"
#include "n64_golden_math.h"

static struct golden_player
{
	struct n64_golden_motion motion;
	long unit, target, candidate;
	float candidate_time;
	float aim_offset[2]; /* right/up tangents relative to the view */
} golden[4];
static int scope_level[4], scope_held[4];
static long scope_weapon[4];
static unsigned short scope_buttons[4];
boolean weapon_reloading(long weapon_index);

void n64_golden_reset(void)
{
	memset(golden, 0, sizeof(golden));
	memset(scope_level, 0, sizeof(scope_level));
	memset(scope_held, 0, sizeof(scope_held));
	memset(scope_buttons, 0, sizeof(scope_buttons));
	for (int i = 0; i < 4; i++) golden[i].unit = golden[i].target = golden[i].candidate = NONE;
	for (int i = 0; i < 4; i++) scope_weapon[i] = NONE;
}

static struct unit_datum *golden_unit(int local)
{
	long player;
	if (local < 0 || local >= 4 || !n64_golden_selected(local)) return NULL;
	player = local_player_get_player_index(local);
	if (player == NONE || player_get(player)->unit_index == NONE) return NULL;
	{
		struct unit_datum *unit = unit_get(player_get(player)->unit_index);
		if (unit->object.parent_object_index != NONE || !unit_controllable(player_get(player)->unit_index)) return NULL;
		return unit;
	}
}

int n64_golden_active(int local)
{
	float x, y; int aim;
	if (local < 0 || local >= 4) return 0;
	if (!player_control_camera_control_is_active() || !golden_unit(local) ||
		director_get_perspective(local) != _director_perspective_first_person ||
		!n64_golden_raw(local, &x, &y, &aim))
	{
		memset(&golden[local], 0, sizeof(golden[local]));
		golden[local].unit = golden[local].target = golden[local].candidate = NONE;
		scope_level[local] = scope_held[local] = scope_buttons[local] = 0;
		scope_weapon[local] = NONE;
		return 0;
	}
	return 1;
}

int n64_golden_manual(int local)
{
	float x, y; int aim;
	return n64_golden_active(local) && n64_golden_raw(local, &x, &y, &aim) && aim;
}

int n64_golden_zoom(int local, long weapon_index)
{
	unsigned short buttons, pressed;
	int count;
	if (!n64_golden_active(local)) return NONE;
	buttons = n64_golden_buttons(local);
	pressed = buttons & ~scope_buttons[local];
	scope_buttons[local] = buttons;
	if (!n64_golden_manual(local) || weapon_index == NONE)
	{
		scope_held[local] = scope_level[local] = 0;
		scope_weapon[local] = NONE;
		return NONE;
	}
	if (!scope_held[local] || scope_weapon[local] != weapon_index) scope_level[local] = 0;
	scope_held[local] = 1; scope_weapon[local] = weapon_index;
	count = weapon_definition_get(weapon_get(weapon_index)->definition_index)->weapon.zoom_level_count;
	if (count <= 0 || weapon_reloading(weapon_index)) return NONE;
	if (pressed & N64_BUTTON_C_UP) scope_level[local]++;
	if (pressed & N64_BUTTON_C_DOWN) scope_level[local]--;
	scope_level[local] = PIN(scope_level[local], 0, count - 1);
	return scope_level[local];
}

void n64_golden_update(int local, float dt, float *yaw, float *pitch, float *forward, float *strafe)
{
	struct player_control *control = player_control_get(local);
	struct golden_player *state = &golden[local];
	struct unit_datum *unit = golden_unit(local);
	float x, y, speeds[2], delta[2], hip[2] = {0.f, 0.f};
	float fov = player_control_get_field_of_view(local);
	int manual;
	long player = local_player_get_player_index(local);
	long unit_index = player_get(player)->unit_index;
	if (!unit || !n64_golden_raw(local, &x, &y, &manual)) return;
	if (state->unit != unit_index)
	{
		memset(state, 0, sizeof(*state));
		state->unit = unit_index;
		state->target = state->candidate = NONE;
	}
	control->autoaim_level = control->magnetism_level = 0.f;
	control->target_object_index = NONE;
	if (!manual)
	{
		real_euler_angles2d position, velocity;
		float autoaim, magnetism;
		long target = local_player_aim_assist(local, &autoaim, &magnetism, &position, &velocity);
		n64_golden_move(local, forward, strafe);
		/* Preserve Halo's existing team/LOS/weapon eligibility, but move a
		   hip cursor rather than pull the camera or secretly bend a shot. */
		if (target != state->candidate) { state->candidate = target; state->candidate_time = 0.f; }
		else state->candidate_time += dt;
		if (target == NONE) state->target = NONE; /* no tracking through cover */
		else if (state->target == NONE || state->target == target || state->candidate_time >= 0.5f) state->target = target;
		if (target != NONE && target == state->target)
		{
			float yaw_error = position.yaw - control->desired_angles.yaw;
			while (yaw_error > 3.141592654f) yaw_error -= 6.283185307f;
			while (yaw_error < -3.141592654f) yaw_error += 6.283185307f;
			hip[0] = golden_clamp(-tanf(yaw_error) / tanf(fov * 0.5f), -0.3f, 0.3f);
			hip[1] = golden_clamp(tanf(position.pitch - control->desired_angles.pitch) / (tanf(fov * 0.5f) * 0.75f), -0.3f, 0.3f);
			control->autoaim_level = autoaim;
			control->target_object_index = target;
		}
	}
	else
	{
		state->target = state->candidate = NONE;
		state->candidate_time = 0.f;
		*forward = *strafe = 0.f;
	}
	n64_look_speed(local, &speeds[0], &speeds[1]);
	/* GoldenEye's rate factor is vertical FOV; Halo's authored weapon FOV
	   is horizontal at its reference 4:3 view. Keep that distinction here. */
	golden_motion_step(&state->motion, x, y, manual ? 0.f : y / 80.f, manual, dt,
		2.f * atanf(tanf(fov * 0.5f) * 0.75f) * (180.f / 3.141592654f), speeds[0], speeds[1], hip, delta);
	*yaw = delta[0]; *pitch = delta[1];
	/* normalized cursor requests become tangents, keeping FOV/scopes honest. */
	state->aim_offset[0] = state->motion.cursor[0] * tanf(fov * 0.5f);
	state->aim_offset[1] = state->motion.cursor[1] * tanf(fov * 0.5f) * 0.75f;
}

void n64_golden_ray(int local, float *direction)
{
	real_vector3d forward, right, up, aim;
	if (!n64_golden_active(local) || golden[local].unit != player_get(local_player_get_player_index(local))->unit_index) return;
	memcpy(&forward, direction, sizeof(forward));
	cross_product3d(&forward, global_up3d, &right);
	if (normalize3d(&right) < 0.001f) return;
	cross_product3d(&right, &forward, &up);
	aim.i = forward.i + right.i * golden[local].aim_offset[0] + up.i * golden[local].aim_offset[1];
	aim.j = forward.j + right.j * golden[local].aim_offset[0] + up.j * golden[local].aim_offset[1];
	aim.k = forward.k + right.k * golden[local].aim_offset[0] + up.k * golden[local].aim_offset[1];
	normalize3d(&aim);
	memcpy(direction, &aim, sizeof(aim));
}

void n64_golden_weapon_axes(int local, float *forward, float *up)
{
	struct unit_datum *unit = golden_unit(local);
	real_vector3d right, corrected_up;
	if (!unit || !n64_golden_active(local)) return;
	cross_product3d(&unit->unit.aiming_vector, (const real_vector3d *)up, &right);
	if (normalize3d(&right) < 0.001f) return;
	cross_product3d(&right, &unit->unit.aiming_vector, &corrected_up);
	memcpy(forward, &unit->unit.aiming_vector, sizeof(real_vector3d));
	memcpy(up, &corrected_up, sizeof(real_vector3d));
}

static long golden_guidance_target(int local, long unit_index)
{
	real_euler_angles2d position, velocity;
	float autoaim, magnetism;
	long target;
	/* Keep the acquired hip target, but recheck current weapon eligibility,
	   teams and line of sight at the shot. Never guide toward a stale target
	   during switch hysteresis, after respawn, or while R is held. */
	if (n64_golden_manual(local) || golden[local].unit != unit_index ||
		golden[local].target == NONE || golden[local].target != golden[local].candidate) return NONE;
	target = local_player_aim_assist(local, &autoaim, &magnetism, &position, &velocity);
	return target == golden[local].target ? target : NONE;
}

int n64_golden_projectile(long player_index, const float *position, float *direction, long *target_object_index)
{
	struct player_datum *player = player_get(player_index);
	struct unit_datum *unit;
	struct collision_result hit;
	real_point3d eye, target;
	real_vector3d ray, aimed;
	*target_object_index = NONE;
	if (player->local_player_index == NONE || !n64_golden_active(player->local_player_index)) return 0;
	unit = unit_get(player->unit_index);
	unit_get_camera_position(player->unit_index, &eye);
	scale_vector3d(&unit->unit.aiming_vector, 128.f, &ray);
	point_from_line3d(&eye, &unit->unit.aiming_vector, 128.f, &target);
	if (collision_test_vector(_collision_test_for_projectiles_flags, &eye, &ray, player->unit_index, &hit)) target = hit.point;
	vector_from_points3d((const real_point3d *)position, &target, &aimed);
	if (normalize3d(&aimed) > 0.001f) memcpy(direction, &aimed, sizeof(aimed));
	*target_object_index = golden_guidance_target(player->local_player_index, player->unit_index);
	return 1;
}

void n64_golden_cursor(int local, float *x, float *y)
{
	struct unit_datum *unit = golden_unit(local);
	real_vector3d right, up;
	float z, vertical, aspect;
	*x = *y = 0.f;
	if (!unit || !n64_golden_active(local)) return;
	cross_product3d(&render.camera.forward, &render.camera.up, &right);
	if (normalize3d(&right) < 0.001f) return;
	cross_product3d(&right, &render.camera.forward, &up);
	z = dot_product3d(&unit->unit.aiming_vector, &render.camera.forward);
	if (z < 0.1f) return;
	vertical = tanf(render.camera.vertical_field_of_view * 0.5f);
	aspect = (float)(render.camera.viewport_bounds.x1 - render.camera.viewport_bounds.x0) /
		(render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0);
	*x = golden_clamp(dot_product3d(&unit->unit.aiming_vector, &right) / (z * vertical * aspect), -0.9f, 0.9f);
	*y = golden_clamp(dot_product3d(&unit->unit.aiming_vector, &up) / (z * vertical), -0.9f, 0.9f);
}
