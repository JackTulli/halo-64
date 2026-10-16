/* Visual-only sniper trail geometry; no game state or combat RNG. */
#ifndef N64_SNIPER_RIBBON_H
#define N64_SNIPER_RIBBON_H
#include <math.h>
#include <string.h>
#define N64_SNIPER_RIBBONS 8
#define N64_SNIPER_LIFE 0.24f
#define N64_SNIPER_LENGTH 12.f
#define N64_SNIPER_CLIP 10

/* Retain the last short piece of the actual collision-resolved trajectory. */
static int sniper_ribbon_segment(const float a[3], const float b[3], float start[3], float end[3])
{
	float d[3], length2 = 0.f;
	for (int k = 0; k < 3; k++)
	{
		if (!isfinite(a[k]) || !isfinite(b[k])) return 0;
		d[k] = b[k] - a[k]; length2 += d[k] * d[k];
	}
	if (!isfinite(length2) || length2 < 0.000001f) return 0;
	float scale = length2 > N64_SNIPER_LENGTH * N64_SNIPER_LENGTH ? N64_SNIPER_LENGTH / sqrtf(length2) : 1.f;
	for (int k = 0; k < 3; k++) { end[k] = b[k]; start[k] = b[k] - d[k] * scale; }
	return 1;
}

static int sniper_ribbon_clip(float in[][5], int count, const float plane[4], float out[][5])
{
	int n = 0;
	for (int i = 0; i < count; i++)
	{
		float *a = in[i], *b = in[(i + 1) % count];
		float da = a[0] * plane[0] + a[1] * plane[1] + a[2] * plane[2] + plane[3];
		float db = b[0] * plane[0] + b[1] * plane[1] + b[2] * plane[2] + plane[3];
		if (!isfinite(da) || !isfinite(db)) return 0;
		if (da >= 0.f) { if (n == N64_SNIPER_CLIP) return 0; memcpy(out[n++], a, 5 * sizeof(float)); }
		if ((da < 0.f) != (db < 0.f))
		{
			float t = da / (da - db);
			if (n == N64_SNIPER_CLIP) return 0;
			for (int k = 0; k < 5; k++) out[n][k] = a[k] + (b[k] - a[k]) * t;
			n++;
		}
	}
	return n;
}

/* Output RDPQ XYZ/ST/INV_W. All geometry clips before perspective division;
   thin screen-space width stays readable in small and anamorphic viewports. */
static int sniper_ribbon_geometry(const struct n64_particle_view *view, const float start[3], const float end[3], float out[N64_SNIPER_CLIP][6])
{
	float line[2][3], xy[2][2], camera[N64_SNIPER_CLIP][5], temp[N64_SNIPER_CLIP][5];
	float px = view->pixels_per_unit * view->x_scale, py = view->pixels_per_unit;
	float near = view->z_near * 1.01f;
	if (!(px > 0.f && py > 0.f && near > 0.f && view->z_far > near)) return 0;
	for (int i = 0; i < 2; i++)
	{
		const float *point = i ? end : start;
		for (int k = 0; k < 3; k++)
		{
			const float *basis = k == 0 ? view->right : k == 1 ? view->up : view->forward;
			line[i][k] = 0.f;
			for (int j = 0; j < 3; j++) line[i][k] += (point[j] - view->eye[j]) * basis[j];
			if (!isfinite(line[i][k])) return 0;
		}
	}
	/* Trim the center line against depth before constructing its width. */
	for (int side = 0; side < 2; side++)
	{
		float depth = side ? view->z_far : near;
		float da = side ? depth - line[0][2] : line[0][2] - depth;
		float db = side ? depth - line[1][2] : line[1][2] - depth;
		if (da < 0.f && db < 0.f) return 0;
		if ((da < 0.f) != (db < 0.f))
		{
			float t = da / (da - db), point[3];
			for (int k = 0; k < 3; k++) point[k] = line[0][k] + (line[1][k] - line[0][k]) * t;
			memcpy(line[da < 0.f ? 0 : 1], point, sizeof(point));
		}
	}
	for (int i = 0; i < 2; i++)
	{
		xy[i][0] = line[i][0] * px / line[i][2];
		xy[i][1] = line[i][1] * py / line[i][2];
	}
	float dx = xy[1][0] - xy[0][0], dy = xy[1][1] - xy[0][1];
	float length = sqrtf(dx * dx + dy * dy);
	/* Looking exactly along the shot produces no artificial glowing dot. */
	if (!isfinite(length) || length < 0.8f) return 0;
	float normal[2] = {-dy / length, dx / length};
	for (int v = 0; v < 4; v++)
	{
		int endpoint = v == 1 || v == 2;
		float z = line[endpoint][2], width = py * 0.009f / z;
		if (width < 0.35f) width = 0.35f;
		if (width > 1.1f) width = 1.1f;
		float side = v < 2 ? -1.f : 1.f;
		camera[v][0] = line[endpoint][0] + normal[0] * width * side * z / px;
		camera[v][1] = line[endpoint][1] + normal[1] * width * side * z / py;
		camera[v][2] = z;
		camera[v][3] = endpoint ? 15.f : 0.f;
		camera[v][4] = v < 2 ? 0.f : 3.f;
	}
	const float planes[4][4] = {
		{px, 0.f, view->center_x - view->x0, 0.f},
		{-px, 0.f, view->x1 - view->center_x, 0.f},
		{0.f, -py, view->center_y - view->y0, 0.f},
		{0.f, py, view->y1 - view->center_y, 0.f}
	};
	int count = 4;
	for (int p = 0; p < 4 && count >= 3; p++)
	{
		count = sniper_ribbon_clip(camera, count, planes[p], temp);
		memcpy(camera, temp, count * 5 * sizeof(float));
	}
	float m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	float m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);
	for (int v = 0; v < count; v++)
	{
		float inv = 1.f / camera[v][2];
		out[v][0] = view->center_x + camera[v][0] * px * inv;
		out[v][1] = view->center_y - camera[v][1] * py * inv;
		if (view->chunky) out[v][0] = view->x0 + (out[v][0] - view->x0) * view->chunky_scale;
		out[v][2] = fmaxf(0.f, fminf(1.f, (-m22 + m32 * inv) * .5f + .5f));
		out[v][3] = camera[v][3]; out[v][4] = camera[v][4]; out[v][5] = inv;
		for (int k = 0; k < 6; k++) if (!isfinite(out[v][k])) return 0;
	}
	return count >= 3 ? count : 0;
}
#endif
