/* Optional visual modifiers. No object, projectile or damage data is changed. */
#include "cseries.h"
#include <math.h>
#include <string.h>
#include "physics/collisions.h"
#include "physics/collision_bsp_definitions.h"
#include "structures/structures.h"
#include "structures/structure_bsp_definitions.h"
#include "scenario/scenario.h"
#include "tag_files/tag_groups.h"
#include "../src/n64.h"

static int big_head, paintball;
static unsigned long paint_sequence;
static int impact_budget;

int n64_big_head_enabled(void) { return big_head; }
int n64_paint_enabled(void) { return paintball; }
void n64_modifiers_set(int heads, int paint)
{
	big_head = !!heads;
	paintball = !!paint;
	paint_sequence = 0;
	impact_budget = 8;
}

void n64_paint_budget_reset(void) { impact_budget = 8; }

/* Clip the actual convex collision face into the decal's UV square. Geometry
   outside the supporting surface is never painted across a portal or corner. */
static int clip_uv(struct n64_paint_vertex *v, int count, int axis, float bound, int positive)
{
	struct n64_paint_vertex out[N64_PAINT_VERTICES];
	int found = 0;
	for (int i = 0; i < count; i++)
	{
		const struct n64_paint_vertex *a = &v[i], *b = &v[(i + 1) % count];
		float da = (a->uv[axis] - bound) * positive, db = (b->uv[axis] - bound) * positive;
		if (da >= 0.f)
		{
			if (found == N64_PAINT_VERTICES) return 0;
			out[found++] = *a;
		}
		if ((da < 0.f) != (db < 0.f))
		{
			float t = da / (da - db);
			if (found == N64_PAINT_VERTICES) return 0;
			for (int k = 0; k < 3; k++) out[found].position[k] = a->position[k] + (b->position[k] - a->position[k]) * t;
			for (int k = 0; k < 2; k++) out[found].uv[k] = a->uv[k] + (b->uv[k] - a->uv[k]) * t;
			found++;
		}
	}
	memcpy(v, out, found * sizeof(*v));
	return found;
}

static float decal_float(const unsigned char *definition, int offset)
{
	float value;
	memcpy(&value, definition + offset, sizeof(value));
	return value;
}

static unsigned char decal_channel(float value)
{
	if (!isfinite(value) || value <= 0.f) return 0;
	return value >= 1.f ? 255 : (unsigned char)(value * 255.f + .5f);
}

static void project_decal(long tag, const unsigned char *definition, real_point3d const *origin,
	real_vector3d const *velocity, real radius_modifier)
{
	struct collision_result hit;
	struct structure_bsp *structure;
	struct collision_bsp *bsp;
	struct collision_surface *surface;
	struct n64_paint_vertex vertices[N64_PAINT_VERTICES];
	real_vector3d tangent, bitangent, axis;
	float radius, lo, hi, angle, c, s;
	long edge, first;
	int count = 0;
	const char *name;
	int painted;
	if (impact_budget <= 0 || !origin || !velocity) return;
	name = tag_get_name(tag);
	if (!name) return;
	/* Paintball replaces only weapon surface ink. Blood and other source
	   materials retain their original art; all particle VFX stay independent. */
	painted = paintball && strstr(name, "bullet holes");
	if (painted && strstr(name, "glow")) return;
	for (int k = 0; k < 3; k++)
		if (!isfinite(((const float *)origin)[k]) || !isfinite(((const float *)velocity)[k])) return;
	if (!isfinite(radius_modifier) || radius_modifier < 0.f) return;
	memcpy(&lo, definition + 0x18, 4);
	memcpy(&hi, definition + 0x1c, 4);
	/* Use a local counter, never either engine random stream. A zero effect
	   modifier means 1.0 in Halo's own decal implementation. */
	paint_sequence++;
	radius = (lo + (hi - lo) * ((paint_sequence * 73u & 255u) / 255.f)) * (radius_modifier == 0.f ? 1.f : radius_modifier);
	if (!isfinite(radius) || radius <= 0.f || radius > 16.f) return;
	impact_budget--;
	if (!collision_test_vector(FLAG(_collision_test_structure_bit) | FLAG(_collision_test_front_facing_surfaces_bit),
		origin, velocity, NONE, &hit) || hit.type != _collision_result_structure) return;
	structure = global_structure_bsp_get();
	if (!structure || structure->collision_bsp.count < 1) return;
	bsp = TAG_BLOCK_GET_ELEMENT(&structure->collision_bsp, 0, struct collision_bsp);
	if (hit.surface_index < 0 || hit.surface_index >= bsp->surfaces.count) return;
	surface = TAG_BLOCK_GET_ELEMENT(&bsp->surfaces, hit.surface_index, struct collision_surface);
	axis = fabsf(hit.plane.n.k) < 0.9f ? *global_up3d : *global_forward3d;
	cross_product3d(&hit.plane.n, &axis, &tangent);
	if (normalize3d(&tangent) < 0.001f) return;
	cross_product3d(&hit.plane.n, &tangent, &bitangent);
	angle = (paint_sequence * 137u % 360u) * (6.283185307f / 360.f);
	c = cosf(angle); s = sinf(angle);
	first = edge = surface->first_edge_index;
	do
	{
		struct collision_edge *e;
		struct collision_vertex *v;
		real_vector3d d;
		int reverse;
		float u, w;
		if (count >= 8 || edge < 0 || edge >= bsp->edges.count) return;
		e = TAG_BLOCK_GET_ELEMENT(&bsp->edges, edge, struct collision_edge);
		reverse = e->surface_indices[1] == hit.surface_index;
		if (e->surface_indices[reverse] != hit.surface_index || e->vertex_indices[reverse] < 0 || e->vertex_indices[reverse] >= bsp->vertices.count) return;
		v = TAG_BLOCK_GET_ELEMENT(&bsp->vertices, e->vertex_indices[reverse], struct collision_vertex);
		vector_from_points3d(&hit.point, &v->point, &d);
		u = dot_product3d(&d, &tangent); w = dot_product3d(&d, &bitangent);
		vertices[count].position[0] = v->point.x;
		vertices[count].position[1] = v->point.y;
		vertices[count].position[2] = v->point.z;
		vertices[count].uv[0] = 0.5f + (u * c - w * s) / (2.f * radius);
		vertices[count].uv[1] = 0.5f + (u * s + w * c) / (2.f * radius);
		count++;
		edge = e->edge_indices[reverse];
	} while (edge != first);
	for (int a = 0; a < 2 && count >= 3; a++)
	{
		count = clip_uv(vertices, count, a, 0.f, 1);
		count = clip_uv(vertices, count, a, 1.f, -1);
	}
	if (count >= 3)
	{
		if (painted)
		{
			/* The event's splash and its persistent surface ink share one hue. */
			n64_paint_splash((const float *)&hit.point, radius, n64_paint_next_color());
			n64_paint_submit(vertices, count, radius >= 0.25f ? 1 + (paint_sequence & 1) : 0);
		}
		else
		{
			unsigned char rgba[4];
			float mix = ((paint_sequence * 73u & 255u) / 255.f);
			float intensity = decal_float(definition, 0x2c) * (1.f - mix) + decal_float(definition, 0x30) * mix;
			float life = decal_float(definition, 0x78) * (1.f - mix) + decal_float(definition, 0x7c) * mix;
			float fade = decal_float(definition, 0x80) * (1.f - mix) + decal_float(definition, 0x84) * mix;
			for (int k = 0; k < 3; k++) rgba[k] = decal_channel(
				decal_float(definition, 0x34 + k * 4) * (1.f - mix) + decal_float(definition, 0x40 + k * 4) * mix);
			rgba[3] = decal_channel(intensity);
			n64_decal_submit(vertices, count, name, life, fade, rgba);
		}
	}
}

/* The original 112 KB Xbox decal pool remains disabled. A bounded cosmetic
   path consumes native deca references, including chained burn/glow materials. */
void n64_paint_impact(long tag, real_point3d const *origin, real_vector3d const *velocity, real radius_modifier)
{
	long visited[4];
	for (int chain = 0; chain < 4 && tag != NONE; chain++)
	{
		const unsigned char *definition;
		long next;
		for (int i = 0; i < chain; i++) if (visited[i] == tag) return;
		if (!tag_index_is_group(tag, 'deca')) return;
		visited[chain] = tag;
		definition = (const unsigned char *)tag_get('deca', tag);
		if (!definition) return;
		project_decal(tag, definition, origin, velocity, radius_modifier);
		memcpy(&next, definition + 0x14, sizeof(next));
		tag = next;
	}
}
