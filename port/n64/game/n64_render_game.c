/*
N64_RENDER_GAME.C

The renderer as the game sees it (render/ and rasterizer/ are left out of
the N64 build): the globals other subsystems read, with the Xbox build's
defaults, the lifecycle calls, and the frame entry points main.c calls
(render_frame, render_frame_present). The drawing itself is the N64
renderer's (port/n64/src/n64_render.c).
*/

#include "cseries.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "game/game.h"
#include "game/players.h"
#include "game/local_players.h"
#include "interface/first_person_weapons.h"
#include "interface/interface.h"
#include "main/main.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_debug_options.h"
#include "render/render.h"
#include "objects/objects.h"
#include "objects/object_definitions.h"
#include "models/model_definitions.h"
#include "items/weapon_definitions.h"
#include "tag_files/tag_groups.h"
#include "scenario/scenario.h"
#include "structures/structures.h"
#include "structures/structure_bsp_definitions.h"
#include "scenario/sky_definitions.h"

#include "../src/n64.h"

typedef char verify_render_globals_used_size[
	offsetof(struct render_globals, environment_surface_flags) == 0x430 ? 1 : -1];

/* ---------- globals (rasterizer.c) */

struct rasterizer_globals_definition rasterizer_globals =
{
	FALSE,
	0,
	0,
	{ 0 },
	0,
	0,
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0, 3, 0, 0 },
	FALSE,
	FALSE, /* framerate_throttle: the N64 never outruns the display */
	FALSE,
	0,
	0,
	{ 0 },
	0.0625f,
	1024.f,
	0.01171875f,
	1024.f,
	{ 0 }
};

struct rasterizer_debug_options_definition rasterizer_debug_options =
{
	FALSE, 0, 0, 0, FALSE, FALSE,
	NONE, /* debug_model_lod */
	FALSE, FALSE,
	TRUE, TRUE, TRUE, TRUE, /* models, model_transparents, draw_first_person_weapon_first, stencil_mask */
	2,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE,
	TRUE, /* dynamic_screen_geometry */
	TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
	{ 0 },
	1.0f,
	{ 0 },
	0,
	TRUE, TRUE,
	{ 0 },
	0.0f,
	TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE,
	TRUE, TRUE, FALSE, FALSE, FALSE, FALSE,
	{ 0 },
	0.4f,
	8,
	1.0f / 256.0f, /* zoffset */
	FALSE, FALSE, 0, TRUE, TRUE, TRUE, FALSE, FALSE,
	{ 0 },
	0.0f,
	1.0f,
	{ 0 },
	FALSE, FALSE,
	{ 0 },
};

real_argb_color *global_rasterizer_model_ambient_reflection_tint;

static void (*vertical_blank_callback)(unsigned long context);

/* ---------- lifecycle */

boolean rasterizer_initialize(
	void)
{
	n64_render_initialize();
	/* the logical screen the game lays its views and HUD out on; the N64
	renderer scales it to its 320x240 frame */
	rasterizer_globals.reserved04.screen_bounds.x0 = 0;
	rasterizer_globals.reserved04.screen_bounds.y0 = 0;
	rasterizer_globals.reserved04.screen_bounds.x1 = 640;
	rasterizer_globals.reserved04.screen_bounds.y1 = 480;
	rasterizer_globals.reserved04.frame_bounds.x0 = 16;
	rasterizer_globals.reserved04.frame_bounds.y0 = 12;
	rasterizer_globals.reserved04.frame_bounds.x1 = 624;
	rasterizer_globals.reserved04.frame_bounds.y1 = 468;
	rasterizer_globals.initialized = TRUE;
	return TRUE;
}

void rasterizer_dispose(void) { return; }
void rasterizer_preinitialize__fill_you_up_with_the_devils_cock(void) { return; }

void rasterizer_set_vblank_callback(
	void (*callback)(unsigned long context))
{
	vertical_blank_callback = callback;
	return;
}

/* ---------- maps */

char const *n64_loaded_map_name(void);

void render_initialize(void) { return; }
void render_dispose(void) { return; }
void render_dispose_from_old_map(void) { return; }

/* debugging (port.json flag "debug_weapon=<weapon tag name>"): players
spawn with that weapon first (game_engine.c's starting equipment) */
static long debug_weapon = NONE;

static void n64_debug_starting_weapon(
	void)
{
	char name[128];

	debug_weapon = NONE;
	if (n64_debug_text("debug_weapon", name, sizeof(name)))
		debug_weapon = tag_loaded(WEAPON_DEFINITION_TAG, name);
	return;
}

long n64_debug_spawn_weapon(
	long definition_index,
	boolean first_weapon)
{
	return first_weapon && debug_weapon != NONE ? debug_weapon : definition_index;
}

/* ---------- how far the views see

The N64 draws the world to a distance that shrinks with the number of
views, into fog of the map's own colour (its first sky's outdoor fog;
nearer where Halo's own fog is opaque nearer); past it, nothing. */

#define N64_DRAW_DISTANCE_1 55.f
#define N64_DRAW_DISTANCE_2 45.f
#define N64_DRAW_DISTANCE_4 32.f
/* the fog starts at this share of the distance */
#define N64_FOG_START 0.5f

/* Maps whose sky the N64 draws (n64_render_set_sky), as Halo's looks from
the horizon up, the fog the sky's colour at the horizon. Halo's own fog is
a light haze (Blood Gulch's: pale peach, at most 40% dense, 3 to 100 units:
a fifth at the N64's 55); the N64's must be opaque at its draw distance,
where the level ends, and in that peach over a peach frame Blood Gulch
looked like a dust storm under a peach sky. Blood Gulch's sky, "clear
afternoon": a dome whose colour is a radial gradient seen from above -- deep
blue overhead, (0, 0, 143), to pale blue at the rim, (180, 202, 224), the
radius the cosine of the height -- under light grey puffy clouds; here each
stop is a third of the way to the clouds' colour. The fog starts later than
elsewhere: the valley's middle distance clear, as in Halo */
static const struct n64_sky
{
	const char *map;
	unsigned long colors[N64_SKY_STOPS];
	real fog_start;
} n64_skies[] = {
	{"bloodgulch", {0xBBCCDD, 0xADC1DA, 0x90AAD3, 0x778CC6, 0x474AB1}, 0.6f},
};

static struct
{
	unsigned long color;
	real opaque_distance;
	real start;
} n64_fog;

static void n64_fog_for_new_map(
	void)
{
	struct sky *sky = global_scenario_get() ? scenario_get_sky(0) : NULL;
	char line[128];
	int i;

	n64_fog.color = 0x60708C;
	n64_fog.opaque_distance = 0.f;
	n64_fog.start = N64_FOG_START;
	n64_render_set_sky(NULL);
	if (sky)
	{
		real_rgb_color const *color = &sky->outdoor_fog.color;
		unsigned long r = (unsigned long)(PIN(color->red, 0.f, 1.f) * 255.f);
		unsigned long g = (unsigned long)(PIN(color->green, 0.f, 1.f) * 255.f);
		unsigned long b = (unsigned long)(PIN(color->blue, 0.f, 1.f) * 255.f);

		/* (a sky without fog has a black colour: the default then) */
		if (r + g + b > 48)
			n64_fog.color = (r << 16) | (g << 8) | b;
		if (sky->outdoor_fog.maximum_density >= 1.f && sky->outdoor_fog.opaque_distance > 1.f)
			n64_fog.opaque_distance = sky->outdoor_fog.opaque_distance;
		snprintf(line, sizeof(line), "n64: fog %06lx (Halo's: density %.2f, %.1f to %.1f)", n64_fog.color,
			sky->outdoor_fog.maximum_density, sky->outdoor_fog.start_distance, sky->outdoor_fog.opaque_distance);
		n64_log(line);
	}
	for (i = 0; i < (int)NUMBEROF(n64_skies); i++)
	{
		if (!strcmp(n64_loaded_map_name(), n64_skies[i].map))
		{
			n64_fog.color = n64_skies[i].colors[0];
			n64_fog.start = n64_skies[i].fog_start;
			n64_render_set_sky(n64_skies[i].colors);
		}
	}
}

static void n64_fog_for_views(
	short view_count)
{
	real distance = view_count <= 1 ? N64_DRAW_DISTANCE_1 : view_count == 2 ? N64_DRAW_DISTANCE_2 :
		N64_DRAW_DISTANCE_4;

	if (n64_fog.opaque_distance > 0.f && n64_fog.opaque_distance < distance)
		distance = n64_fog.opaque_distance;
	n64_render_set_fog(n64_fog.color, distance * n64_fog.start, distance);
}

void render_initialize_for_new_map(
	void)
{
	extern short player_spawn_count;

	n64_fog_for_new_map();
	/* three or four views: the level's lighter version */
	n64_render_load_level(n64_loaded_map_name(), player_spawn_count > 2);
	n64_hud_set_map(n64_loaded_map_name());
	n64_particles_load(n64_loaded_map_name());
	n64_debug_starting_weapon();
	return;
}

/* ---------- objects */

/* the eye's potentially visible set, for the objects in a view */
static unsigned long const *n64_view_clusters;
static short n64_view_cluster_count;

/* every visible object but the viewer's own body (the view is first person) */
/* the frame's drawable objects, gathered once for all its views (each view
walked every object and looked up its definition, model and owner again) */
#define MAXIMUM_FRAME_OBJECTS 256

static struct frame_object
{
	long object_index;
	long ultimate_parent;
	short cluster;
	short pad;
	struct n64_object n64;
} frame_objects[MAXIMUM_FRAME_OBJECTS];
static short frame_object_count;
static long frame_objects_frame = -1;

static void gather_frame_objects(
	void)
{
	struct object_iterator iterator;
	struct object_datum *object;

	frame_object_count = 0;
	frame_objects_frame = render.frame_index;
	object_iterator_new(&iterator, _object_mask_all, FLAG(_object_header_visible_bit));
	while ((object = object_iterator_next(&iterator)) != NULL && frame_object_count < MAXIMUM_FRAME_OBJECTS)
	{
		struct frame_object *entry = &frame_objects[frame_object_count];
		struct object_definition *definition;
		struct n64_object *n64 = &entry->n64;
		struct object_datum *owner;

		if (TEST_FLAG(object->object.flags, _object_invisible_bit))
			continue;
		definition = object_definition_get(object->definition_index);
		if (definition->object.model.index == NONE)
			continue;
		entry->object_index = iterator.index;
		entry->ultimate_parent = object_get_ultimate_parent(iterator.index);
		entry->cluster = object->object.location.cluster_index;
		n64->model_tag_index = (short)DATUM_INDEX_TO_ABSOLUTE_INDEX(definition->object.model.index);
		n64->node_count = (short)model_definition_get(definition->object.model.index)->nodes.count;
		n64->node_matrices = (const float *)object_get_node_matrices(iterator.index);
		n64->key = (unsigned long)iterator.index;
		n64->region_permutations = object->object.region_permutations;
		n64->region_count = MAXIMUM_REGIONS_PER_OBJECT;
		n64->center[0] = object->object.bounding_sphere_center.x;
		n64->center[1] = object->object.bounding_sphere_center.y;
		n64->center[2] = object->object.bounding_sphere_center.z;
		n64->radius = object->object.bounding_sphere_radius;
		n64->tint[0] = n64->tint[1] = n64->tint[2] = n64->tint[3] = 0xFF;
		owner = entry->ultimate_parent == iterator.index ? object : object_get(entry->ultimate_parent);
		if (TEST_FLAG(_object_mask_unit, object->object.type) || TEST_FLAG(_object_mask_unit, owner->object.type))
		{
			/* a player (or what a player holds): the armour colour, where the
			model's textures take it */
			real_rgb_color const *color = &owner->object.base_change_colors[_object_change_color_a];

			n64->tint[0] = (unsigned char)PIN(color->red * 255.f, 0.f, 255.f);
			n64->tint[1] = (unsigned char)PIN(color->green * 255.f, 0.f, 255.f);
			n64->tint[2] = (unsigned char)PIN(color->blue * 255.f, 0.f, 255.f);
		}
		frame_object_count++;
	}
}

/* the frame each object was last in a view, by its absolute index (a
collision of two objects in a slot only keeps one's updates at full rate):
objects.c updates a Covenant no view drew every other tick */
#define N64_SEEN_SLOTS 512
static unsigned short n64_object_seen[N64_SEEN_SLOTS];

boolean n64_object_recently_seen(
	long object_index)
{
	return (unsigned short)((unsigned short)render.frame_index -
		n64_object_seen[DATUM_INDEX_TO_ABSOLUTE_INDEX(object_index) & (N64_SEEN_SLOTS - 1)]) <= 2;
}

/* whether any cluster an object's bounding sphere reaches (Halo's cluster
partitions: its ultimate parent's) is in the eye's potentially visible set:
Halo draws an object from every cluster it touches. By its own cluster
alone, players, vehicles and scenery near a portal vanished from places
that see only the cluster beside theirs (Sidewinder's clusters see 7-17 of
its 35) */
static boolean object_reaches_visible_cluster(
	long object_index)
{
	struct object_cluster_iterator iterator;
	short cluster;

	for (cluster = object_get_first_cluster(&iterator, object_index); cluster != NONE;
		cluster = object_get_next_cluster(&iterator, object_index))
	{
		if (cluster < 0 || cluster >= n64_view_cluster_count ||
			(n64_view_clusters[cluster >> 5] & (1UL << (cluster & 31))))
			return TRUE;
	}
	return FALSE;
}

static void render_objects_for_view(
	short local_player_index,
	boolean third_person)
{
	long viewer_unit = NONE;
	long player_index = local_player_get_player_index(local_player_index);
	short i;

	if (player_index != NONE && !third_person)
		viewer_unit = player_get(player_index)->unit_index;
	if (frame_objects_frame != render.frame_index)
		gather_frame_objects();
	for (i = 0; i < frame_object_count; i++)
	{
		struct frame_object const *entry = &frame_objects[i];
		short cluster = entry->cluster;

		/* in a cluster the eye cannot see, and reaching into none it can */
		if (n64_view_clusters && cluster >= 0 && cluster < n64_view_cluster_count &&
			!(n64_view_clusters[cluster >> 5] & (1UL << (cluster & 31))) &&
			!object_reaches_visible_cluster(entry->object_index))
			continue;
		if (viewer_unit != NONE && entry->ultimate_parent == viewer_unit)
			continue;
		if (n64_render_object(&entry->n64))
			n64_object_seen[DATUM_INDEX_TO_ABSOLUTE_INDEX(entry->object_index) & (N64_SEEN_SLOTS - 1)] =
				(unsigned short)render.frame_index;
	}
}

/* ---------- the first-person weapon (interface/first_person_weapons.c poses
it; its draw hands each model here) */

static boolean first_person_begun;

void n64_first_person_model_draw(
	long model_index,
	struct real_matrix4x3 const *node_matrices,
	real_rgb_color const *tint)
{
	static unsigned char const no_permutations[MAXIMUM_REGIONS_PER_OBJECT] = { 0 };
	struct n64_object n64;
	/* (measuring, "debug_fp_nodraw": the first-person weapon posed but not
	drawn -- its animation's cost apart from its drawing's) */
	static int no_draw = -1;

	if (no_draw < 0)
		no_draw = n64_debug_flag("debug_fp_nodraw");
	/* (Settings: VIEW MODEL "off in split", with three or four views) */
	if (model_index == NONE || no_draw || n64_render_view_models_hidden())
		return;
	if (!first_person_begun)
	{
		n64_render_first_person_begin();
		first_person_begun = TRUE;
	}
	n64.model_tag_index = (short)DATUM_INDEX_TO_ABSOLUTE_INDEX(model_index);
	n64.node_count = (short)model_definition_get(model_index)->nodes.count;
	n64.node_matrices = (const float *)node_matrices;
	n64.key = 0;
	n64.region_permutations = no_permutations;
	n64.region_count = 0;
	n64.center[0] = render.camera.position.x;
	n64.center[1] = render.camera.position.y;
	n64.center[2] = render.camera.position.z;
	n64.radius = 4.f;
	/* the player's colour, where the model's textures take it */
	n64.tint[0] = tint ? (unsigned char)PIN(tint->red * 255.f, 0.f, 255.f) : 0xFF;
	n64.tint[1] = tint ? (unsigned char)PIN(tint->green * 255.f, 0.f, 255.f) : 0xFF;
	n64.tint[2] = tint ? (unsigned char)PIN(tint->blue * 255.f, 0.f, 255.f) : 0xFF;
	n64.tint[3] = 0xFF;
	n64_render_object(&n64);
}

/* ---------- frames */

static boolean frame_begun;

static void render_frame_begin(
	void)
{
	if (!frame_begun)
	{
		n64_render_frame_begin();
		frame_begun = TRUE;
	}
}

void render_frame(
	struct render_window *windows,
	short window_count,
	point2d const *screenshot_page_index,
	point2d const *screenshot_index,
	struct bitmap_data *screenshot_bitmap,
	real time_delta_since_tick_sec)
{
	short window_index;

	(void)screenshot_page_index;
	(void)screenshot_index;
	(void)screenshot_bitmap;
	{
		extern void n64_game_cycle_frame(void);

		n64_game_cycle_frame();
	}
	render.frame_index++;
	render.time_delta_since_tick_sec = time_delta_since_tick_sec;
	{
		short views = 0;

		for (window_index = 0; window_index < window_count; window_index++)
			views += !windows[window_index].console_window;
		n64_fog_for_views(views);
		n64_hud_set_view_count(views);
		n64_render_set_view_count(views);
	}
	render_frame_begin();
	for (window_index = 0; window_index < window_count; window_index++)
	{
		struct render_window *window = &windows[window_index];
		struct render_camera const *camera = &window->render_camera;
		struct n64_view view;

		if (window->console_window)
			continue;
		view.position[0] = camera->position.x;
		view.position[1] = camera->position.y;
		view.position[2] = camera->position.z;
		view.forward[0] = camera->forward.i;
		view.forward[1] = camera->forward.j;
		view.forward[2] = camera->forward.k;
		view.up[0] = camera->up.i;
		view.up[1] = camera->up.j;
		view.up[2] = camera->up.k;
		view.vertical_field_of_view = camera->vertical_field_of_view;
		view.z_near = camera->z_near;
		view.z_far = 200.f;
		{
			/* (measuring, "debug_far=<units>") */
			static float debug_far = -1.f;
			char text[16];

			if (debug_far < 0.f)
				debug_far = n64_debug_text("debug_far", text, sizeof(text)) ? (float)atof(text) : 0.f;
			if (debug_far > 0.f)
				view.z_far = debug_far;
		}
		/* the game lays the views out on a 640x480 screen */
		{
			/* (half at 320x240; 480i's 640x480 or 512x480) */
			float scale_x = n64_render_layout_scale_x(), scale_y = n64_render_layout_scale_y();

			view.x0 = (short)(camera->viewport_bounds.x0 * scale_x);
			view.y0 = (short)(camera->viewport_bounds.y0 * scale_y);
			view.x1 = (short)(camera->viewport_bounds.x1 * scale_x);
			view.y1 = (short)(camera->viewport_bounds.y1 * scale_y);
		}
		view.local_player_index = window->local_player_index;
		view.visible_clusters = NULL;
		view.cluster_count = 0;
		if (window_index == 0 && n64_debug_flag("debug_chase"))
		{
			/* debugging: the first view follows its player from behind */
			long player_index = local_player_get_player_index(window->local_player_index);

			if (player_index != NONE && player_get(player_index)->unit_index != NONE)
			{
				struct object_datum *unit = object_get(player_get(player_index)->unit_index);
				real_vector3d forward = unit->object.forward;

				view.position[0] = unit->object.position.x - forward.i * 1.6f;
				view.position[1] = unit->object.position.y - forward.j * 1.6f;
				view.position[2] = unit->object.position.z + 0.9f;
				view.forward[0] = forward.i * 0.94f;
				view.forward[1] = forward.j * 0.94f;
				view.forward[2] = -0.34f;
				view.up[0] = 0.f;
				view.up[1] = 0.f;
				view.up[2] = 1.f;
			}
		}
		{
			/* debugging (port.json flag "debug_spin=x y z [yaw]"): the first
			view stands there, turning once every 16 seconds; with a yaw,
			every view stands there facing yaw degrees (and 180 more for each
			further view) -- the same frames for measurements */
			static int spin = -1;
			static float spin_at[4];
			char text[64];

			if (spin < 0)
			{
				spin = n64_debug_text("debug_spin", text, sizeof(text)) ?
					sscanf(text, "%f %f %f %f", &spin_at[0], &spin_at[1], &spin_at[2], &spin_at[3]) : 0;
				if (spin < 3)
					spin = 0;
			}
			if (spin == 4 || (spin && window_index == 0))
			{
				/* (port.json flag "debug_spin_period=<ticks>": another turning speed) */
				static long period = -1;
				float angle;

				if (period < 0)
					period = n64_debug_text("debug_spin_period", text, sizeof(text)) ? atol(text) : 480;
				if (period <= 0)
					period = 480;
				angle = spin == 4 ? (spin_at[3] + 180.f * window_index) * (6.2831853f / 360.f) :
					(float)(game_time_get() % period) * (6.2831853f / (float)period);

				view.position[0] = spin_at[0];
				view.position[1] = spin_at[1];
				view.position[2] = spin_at[2];
				view.forward[0] = cosf(angle);
				view.forward[1] = sinf(angle);
				view.forward[2] = -0.15f;
				view.up[0] = 0.f;
				view.up[1] = 0.f;
				view.up[2] = 1.f;
			}
		}
		{
			/* the clusters the eye can see (Halo's potentially visible set) */
			struct location location;
			struct structure_bsp *bsp = global_structure_bsp_get();
			real_point3d eye;

			eye.x = view.position[0];
			eye.y = view.position[1];
			eye.z = view.position[2];
			scenario_location_from_point(&location, &eye);
			if (bsp && location.cluster_index != NONE && bsp->cluster_data.size > 0 && !n64_debug_flag("debug_no_pvs"))
			{
				view.visible_clusters = structure_bsp_get_cluster_pvs(bsp, location.cluster_index);
				view.cluster_count = (short)bsp->clusters.count;
			}
			n64_view_clusters = view.visible_clusters;
			n64_view_cluster_count = view.cluster_count;
		}
		render.local_player_index = window->local_player_index;
		render.window_index = window_index;
		render.camera = *camera;
		n64_render_view(&view);
		render_objects_for_view(window->local_player_index, window_index == 0 && n64_debug_flag("debug_chase"));
		if (!(window_index == 0 && n64_debug_flag("debug_chase")) && !n64_debug_flag("debug_no_first_person"))
		{
			first_person_begun = FALSE;
			first_person_weapon_render_update();
			first_person_weapon_draw();
		}
		/* the view's particles (the world's, if the first-person weapon did
		not already; then the first-person ones) */
		n64_render_view_particles();
		if (!n64_debug_flag("debug_no_hud"))
		{
			/* the game's own HUD (port/n64/game/n64_hud_game.c) */
			if (n64_hud_begin(window_index, view.x0, view.y0, view.x1, view.y1))
			{
				extern int n64_firefight_status(char *text, int size);
				char status[64];
				/* the HUD's inset (Settings: HUD INSET, for a television's
				overscan): Halo lays its HUD out in the window's bounds (its
				corners, messages, scores, nav points' reach), so they shrink
				by it on every side meanwhile; the game's units are twice
				the N64's pixels */
				rectangle2d window_bounds = render.camera.window_bounds;
				int inset = n64_hud_inset();

				render.camera.window_bounds.x0 += inset * 2;
				render.camera.window_bounds.y0 += inset * 2;
				render.camera.window_bounds.x1 -= inset * 2;
				render.camera.window_bounds.y1 -= inset * 2;
				interface_draw_screen();
				render.camera.window_bounds = window_bounds;
				/* Firefight's wave, lives and score, low in the middle */
				if (n64_firefight_status(status, sizeof(status)))
					n64_hud_text((view.x0 + view.x1) / 2 - 62, view.y1 - 16 - inset, 0xFFC0E0FF, status);
			}
			n64_hud_end();
		}
	}
	return;
}

void render_frame_pregame(
	struct render_window const *window,
	struct bitmap_data *bitmap)
{
	(void)window;
	(void)bitmap;
	render.frame_index++;
	render_frame_begin();
	return;
}

void render_frame_present(
	const point2d *screenshot_index,
	struct bitmap_data *bitmap)
{
	(void)screenshot_index;
	(void)bitmap;
	render_frame_begin();
	{
		/* START: the pause menu, over the frame (n64_game.c) */
		extern void n64_game_pause_frame(void);

		n64_game_pause_frame();
	}
	n64_render_frame_end();
	frame_begun = FALSE;
	if (vertical_blank_callback)
		vertical_blank_callback(0);
	return;
}
