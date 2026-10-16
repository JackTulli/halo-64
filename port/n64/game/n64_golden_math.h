/* Independently implemented response model from the documented US timings.
   No reconstructed GoldenEye implementation is copied here. */
#ifndef N64_GOLDEN_MATH_H
#define N64_GOLDEN_MATH_H
#include <math.h>
struct n64_golden_motion { float speed[2], cursor[2]; };
static float golden_clamp(float x, float lo, float hi) { return x < lo ? lo : x > hi ? hi : x; }
static float golden_square(float x)
{
	if (fabsf(x) <= 5.f) return 0.f;
	x = copysignf(golden_clamp((fabsf(x) - 5.f) / 70.f, 0.f, 1.f), x);
	return x * fabsf(x);
}
static float golden_approach(float value, float target, float ticks)
{
	float step = (value * target < 0.f || fabsf(target) < fabsf(value) ? 0.05f : 0.0125f) * ticks;
	return value < target ? fminf(target, value + step) : fmaxf(target, value - step);
}
static float golden_speed_integral(float *value, float target, float ticks)
{
	float travel = 0.f;
	/* A reversal crosses from fast braking to slow acceleration exactly at
	   zero, including when that happens inside a fractional clock tick. */
	if (*value * target < 0.f)
	{
		float stop = fabsf(*value) / 0.05f;
		float braking = fminf(ticks, stop);
		float next = braking >= stop ? 0.f : *value - copysignf(0.05f * braking, *value);
		travel += (*value + next) * 0.5f * braking;
		*value = next;
		ticks -= braking;
	}
	if (ticks > 0.f)
	{
		float rate = fabsf(target) < fabsf(*value) ? 0.05f : 0.0125f;
		float moving = fminf(ticks, fabsf(target - *value) / rate);
		float next = golden_approach(*value, target, ticks);
		travel += (*value + next) * 0.5f * moving + target * (ticks - moving);
		*value = next;
	}
	return travel;
}
static void golden_motion_step(struct n64_golden_motion *state, float x, float y, float digital_pitch,
	int manual, float dt, float fov, float stick_speed, float c_speed, const float hip_target[2], float delta[2])
{
	/* Integrate in US clock ticks, even when Halo advances at 30 Hz. */
	float ticks = golden_clamp(dt * 60.f, 0.f, 6.f);
	delta[0] = delta[1] = 0.f;
	while (ticks > 0.00001f)
	{
		float step = fminf(ticks, 1.f);
		float target[2];
		float travel[2];
		float cursor_target[2] = {manual ? golden_clamp(x, -80.f, 80.f) * (0.65f / 80.f) : hip_target[0],
			manual ? golden_clamp(y, -80.f, 80.f) * (0.65f / 80.f) : hip_target[1]};
		for (int a = 0; a < 2; a++)
		{
			float raw = a ? y : x;
			target[a] = manual ? copysignf(golden_clamp((fabsf(raw) - 60.f) / 10.f, 0.f, 1.f) * 0.7f, raw) : (a ? digital_pitch * 0.7f : 0.f);
			target[a] *= fov / 60.f;
			travel[a] = golden_speed_integral(&state->speed[a], target[a], step);
			state->cursor[a] += (cursor_target[a] - state->cursor[a]) * (1.f - powf(manual ? 0.8f : 0.86f, step));
		}
		delta[0] -= (manual ? travel[0] : golden_square(x) * fov / 60.f * step) * 3.5f * (3.141592654f / 180.f) * stick_speed;
		delta[1] += travel[1] * 3.5f * (3.141592654f / 180.f) * (manual ? stick_speed : c_speed);
		ticks -= step;
	}
}
#endif
