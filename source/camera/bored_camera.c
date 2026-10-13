/*
BORED_CAMERA.C

symbols in this file:
00072FE0 0020:
	_bored_camera_new (0000)
00073000 0020:
	_code_00073000 (0000)
00073020 0020:
	_code_00073020 (0000)
00073040 0010:
	_is_bored (0000)
00073050 0010:
	_is_still_bored (0000)
00073060 0020:
	_real_local_random_range (0000)
00073080 0060:
	_valid_real_vector3d (0000)
000730E0 0070:
	_valid_real_vector3d_axes2 (0000)
00073150 0500:
	_bored_camera_update (0000)
002569B8 0087:
	??_C@_0IH@COKLNKEO@Invalid?5camera?5command?4?6F?3?5?$CI?$CFf?0?5@ (0000)
00256A40 0004:
	__real@45610000 (0000)
00256A44 0004:
	__real@459c4000 (0000)
00256A48 0004:
	__real@c59c4000 (0000)
00256A4C 0007:
	??_C@_06FNKMPIBD@camera?$AA@ (0000)
00256A54 0025:
	??_C@_0CF@KEFAILDC@c?3?2halo?2SOURCE?2camera?2bored_came@ (0000)
*/

/* ---------- headers */

#include "bored_camera.h"
#include "camera/static_camera.h"
#include "cseries/cseries_windows.h"
#include "game/player_control.h"
#include "math/real_math.h"
#include "units/unit_definitions.h"
#include "units/units.h"
#include "observer.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct camera_action
{
	short local_player_index;
};

struct unit_camera_track
{
	struct tag_reference track;
	long unused[3];
};

typedef char unit_camera_track_size_assert[
	sizeof(struct unit_camera_track) == 0x1C ? 1 : -1];

/* ---------- prototypes */

unsigned long *get_global_local_random_seed_address(
	void);
float real_seed_random_range(
	unsigned long *seed,
	float lower_bound,
	float upper_bound);
long player_control_get_aiming_unit_index(
	short local_player_index);

static long bored_camera_shot_threshold_milliseconds(
	long boredom_count);
static long bored_camera_shot_duration_milliseconds(
	long boredom_count);

/* ---------- globals */

/* ---------- public code */

void bored_camera_new(
	struct bored_camera *camera)
{
	camera->boredom_count = 0;
	camera->last_update_milliseconds = system_milliseconds();
	camera->timer_milliseconds = 0;
	return;
}

void bored_camera_update(
	struct bored_camera *camera,
	struct camera_action const *action,
	struct camera_command *result)
{
	unsigned long now;

	now = system_milliseconds();
	match_assert("c:\\halo\\SOURCE\\camera\\bored_camera.c", 51, camera);
	match_assert("c:\\halo\\SOURCE\\camera\\bored_camera.c", 52, result);
	camera->timer_milliseconds += camera->last_update_milliseconds - now;
	camera->last_update_milliseconds = now;

	if (camera->timer_milliseconds < bored_camera_shot_threshold_milliseconds(camera->boredom_count))
	{
		struct player_control_unit_camera_info camera_info;
		long aiming_unit_index;

		aiming_unit_index = player_control_get_aiming_unit_index(
			action->local_player_index);
		player_control_get_unit_camera_info(
			action->local_player_index,
			&camera_info);
		result->position = camera_info.position;

		if (aiming_unit_index != NONE)
		{
			long timer_milliseconds;
			real_point3d camera_position;
			real_euler_angles2d angles;
			real field_of_view;

			if (camera_info.camera->unit_camera_tracks.count)
			{
				TAG_BLOCK_GET_ELEMENT(
					&camera_info.camera->unit_camera_tracks,
					0,
					struct unit_camera_track);
			}

			angles = *player_control_get_facing_angles(
				action->local_player_index);
			unit_get_camera_position(aiming_unit_index, &camera_position);
			angles.pitch = real_local_random_range(
				-DEGREES_TO_RADIANS(63.f),
				DEGREES_TO_RADIANS(22.5f));
			angles.yaw += real_local_random_range(
				-DEGREES_TO_RADIANS(45.f),
				DEGREES_TO_RADIANS(45.f)) + _pi;
			vector3d_from_euler_angles2d(&result->forward, &angles);
			observer_up_from_forward(&result->forward, &result->up);
			field_of_view = real_local_random_range(
				DEGREES_TO_RADIANS(30.f),
				DEGREES_TO_RADIANS(80.f));
			result->field_of_view = field_of_view;
			result->depth = real_local_random_range(1.f, 6.f);
			result->velocity = *global_zero_vector3d;

			timer_milliseconds = bored_camera_shot_duration_milliseconds(camera->boredom_count);
			camera->timer_milliseconds = timer_milliseconds;
			result->flags = FLAG(0);
			result->timer = (real)timer_milliseconds;
			camera->boredom_count++;

			/* January can set timer to 10000..30000 here, while the
			 * validation below accepts at most 3600. Preserve that bug. */
			if (!(
				!(result->flags & FLAG(0)) ||
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
					valid_real(result->timer) && result->timer>=0.f && result->timer<=3600.f)))
			{
				long flags = result->flags;

				display_assert(
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
						flags),
					"c:\\halo\\SOURCE\\camera\\bored_camera.c",
					95,
					TRUE);
				system_exit(-1);
			}
		}
	}
	return;
}

/* ---------- private code */

static long bored_camera_shot_threshold_milliseconds(
	long boredom_count)
{
	return MIN(boredom_count, 3) * 1000;
}

static long bored_camera_shot_duration_milliseconds(
	long boredom_count)
{
	return MIN(boredom_count + 1, 3) * 10000;
}

boolean is_bored(
	void)
{
	return FALSE;
}

boolean is_still_bored(
	void)
{
	return FALSE;
}
