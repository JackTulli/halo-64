/*
N64_MENU.C

The N64's own menu, split screen multiplayer only, over the scene behind
Halo's main menu (tools/n64/ui.py): the space sky and the ring, seen from
the menu's "multiplayer" camera point, slowly drifting. Halo's fonts and
pictures (the logo, the maps' and the game types'). The menu runs at 640x480
interlaced (n64_render_set_menu_display; the game at 320x240), the size
Halo's menus were made for: text and pictures at their own size.

The title offers MULTIPLAYER, FIREFIGHT, CONTROLS and SETTINGS. Controls:
each controller chooses its own scheme, crouch held or toggled, look
inverted or not, fire and grenades swapped or not, and the stick's and the
C buttons' look speeds, on its own panel (n64_controls, xdk_input.c). Multiplayer is the game's
setup: the players join with A on their controller (B leaves, Z changes
team), any of them chooses the game type, the map and the score to win;
START (or A on "START GAME") plays. Settings: particles on or off, the 3D's
resolution (standard; medium, the game's 3D drawn at four fifths
of the width and widened; chunky, half as wide, its pixels twice as wide:
n64_render_set_chunky), the display
(240p, or 480i where the map leaves the memory: n64_render_set_480i), the
anamorphic widescreen picture (n64_render_set_anamorphic), the view
models' quality (n64_render_set_view_model_quality), the animation rate
(full, or half: n64_half_animation_rate), the HUD's inset from
the screen's edges for a television's overscan (n64_hud_set_inset), the FPS
counter (off unless turned on); the settings and controls are kept in the
cartridge's EEPROM (settings_load, settings_save). After a
game, its results first. n64_menu_run returns the choices; the
game side (port/n64/game/n64_game.c) makes the game.
*/

#include <libdragon.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "n64.h"

/* Settings: ANIMATIONS HALF RATE, read by the game's object updates
(source/objects/objects.c n64_object_unmoved) */
int n64_half_animation_rate;

/* ---------- what can be played */

static const struct
{
	const char *folder, *name;
	int picture; /* in ui/mp_map_grafix's order */
} maps[] = {
	{"beavercreek", "BATTLE CREEK", 0},
	{"sidewinder", "SIDEWINDER", 1},
	{"damnation", "DAMNATION", 2},
	{"ratrace", "RAT RACE", 3},
	{"prisoner", "PRISONER", 4},
	{"hangemhigh", "HANG 'EM HIGH", 5},
	{"chillout", "CHILL OUT", 6},
	{"carousel", "DERELICT", 7},
	{"boardingaction", "BOARDING ACTION", 8},
	{"bloodgulch", "BLOOD GULCH", 9},
	{"wizard", "WIZARD", 10},
	{"putput", "CHIRON TL-34", 11},
	{"longest", "LONGEST", 12},
};
#define MAP_COUNT ((int)(sizeof(maps) / sizeof(maps[0])))

static const struct
{
	const char *variant, *name;
	int teams;
	int picture; /* in ui/game_type_grafix's order */
	const char *unit;
	int scores[5];
	int default_score;
} game_types[] = {
	{"slayer", "SLAYER", 0, 2, "KILLS", {5, 10, 15, 25, 50}, 2},
	{"team_slayer", "TEAM SLAYER", 1, 2, "KILLS", {10, 25, 50, 75, 100}, 2},
	/* (slayer with everyone starting with a pistol and an assault rifle:
	port/n64/game/n64_game.c) */
	{"slayer_pro", "SLAYER PRO", 0, 2, "KILLS", {5, 10, 15, 25, 50}, 2},
	{"team_slayer_pro", "TEAM SLAYER PRO", 1, 2, "KILLS", {10, 25, 50, 75, 100}, 2},
	{"ctf", "CAPTURE THE FLAG", 1, 0, "FLAGS", {1, 3, 5, 7, 10}, 1},
	{"king", "KING OF THE HILL", 0, 1, "MINUTES", {1, 2, 3, 5, 10}, 1},
	{"team_king", "TEAM KING", 1, 1, "MINUTES", {1, 2, 3, 5, 10}, 1},
	{"oddball", "ODDBALL", 0, 3, "MINUTES", {1, 2, 3, 5, 10}, 1},
	{"team_oddball", "TEAM ODDBALL", 1, 3, "MINUTES", {1, 2, 3, 5, 10}, 1},
	{"race", "RACE", 0, 4, "LAPS", {1, 3, 5, 7, 10}, 1},
	{"team_race", "TEAM RACE", 1, 4, "LAPS", {1, 3, 5, 7, 10}, 1},
};
#define GAME_TYPE_COUNT ((int)(sizeof(game_types) / sizeof(game_types[0])))

/* the players' colours (Halo's colour indices: red, blue, green, yellow)
and how the menu shows them */
static const int player_colors[4] = {2, 3, 6, 5};
static const unsigned long player_rgb[4] = {0xC82020, 0x2050D8, 0x20A040, 0xE0C820};
static const unsigned long team_rgb[2] = {0xC82020, 0x2050D8};

const char *n64_menu_map_folder(int map) { return map >= 0 && map < MAP_COUNT ? maps[map].folder : "wizard"; }
const char *n64_menu_map_name(int map) { return map >= 0 && map < MAP_COUNT ? maps[map].name : ""; }
const char *n64_menu_game_variant(int type) { return type >= 0 && type < GAME_TYPE_COUNT ? game_types[type].variant : "slayer"; }
int n64_menu_game_teams(int type) { return type >= 0 && type < GAME_TYPE_COUNT && game_types[type].teams; }

int n64_menu_map_from_folder(const char *folder)
{
	for (int i = 0; i < MAP_COUNT; i++)
	{
		if (!strcmp(maps[i].folder, folder))
			return i;
	}
	return -1;
}

/* ---------- the menu's things */

#define TEXT_COLOR 0xFF8CC0FFu
#define TEXT_BRIGHT 0xFFFFFFFFu
#define TEXT_DIM 0xC05880B0u

/* the map's or the game type's picture on the screen (one of each is kept:
the one before is freed once the RDP is done with it) */
struct shown_picture
{
	sprite_t *sprite;
	int number;
};

static struct
{
	int loaded;
	struct n64_font *large, *small;
	sprite_t *logo;
	struct shown_picture map_picture, type_picture;
	/* the settings (particles: on unless turned off); the 3D's width: 0
	standard, 1 medium (256 of 320), 2 chunky (half) */
	int chunky;
	/* the game in 480i (n64_render_set_480i), the anamorphic picture */
	int interlaced;
	int widescreen;
	/* the models' poses at half their rate (n64_half_animation_rate) */
	int half_animation;
	int no_particles;
	/* 0 low, 1 medium, 2 high */
	int view_model;
	/* pixels, HUD_INSET_STEP at a time up to HUD_INSET_MAXIMUM */
	int hud_inset;
	int fps_counter;
	/* the CONTROLS page: each controller's chosen row (CONTROL_*), and the
	controller whose row the help line explains */
	int control_row[4];
	int control_pad;
	/* the scene */
	int sky_model, ring_model;
	float ring[13];
	/* the ui scenario's camera points: position, forward, up, horizontal
	field of view */
	char point_names[16][32];
	float points[16][10];
	int point_count;
	float frame;
} menu;

/* the title's camera, as Halo's: the ui scenario's script ring_loop sets
the camera to uicam, then to point_2 ... point_11 in turn, round and round,
each with a 400-tick transition (13 s: whole seconds) and the next one 200
ticks (6.7 s) later, so that the camera never quite arrives; Halo's observer
glides it (camera/observer.c: from where it is, with its velocity and
acceleration, a quintic to rest at the point in the time left, planned again
every frame). In real time, whatever the frame rate */
static const char *const camera_path[] = {
	"uicam", "point_2", "point_3", "point_4", "point_5", "point_6",
	"point_7", "point_8", "point_9", "point_10", "point_11",
};
#define CAMERA_PATH_LENGTH ((int)(sizeof(camera_path) / sizeof(camera_path[0])))
#define CAMERA_INTERVAL (200.f / 30.f)
#define CAMERA_TRANSITION 13.f

static struct
{
	int started;
	unsigned long long last_ticks;
	float time;
	float position[3], velocity[3], acceleration[3];
	float forward[3], up[3], spin_velocity[3], spin_acceleration[3];
	float field_of_view;
} camera;

static sprite_t *picture(struct shown_picture *shown, const char *kind, int number)
{
	char path[48];

	if (number < 0 || number >= 16)
		return NULL;
	if (shown->sprite && shown->number == number)
		return shown->sprite;
	if (shown->sprite)
		rdpq_call_deferred((void (*)(void *))sprite_free, shown->sprite);
	snprintf(path, sizeof(path), "rom:/ui/%s%d.sprite", kind, number);
	shown->sprite = sprite_load(path);
	shown->number = number;
	return shown->sprite;
}

static void load_scene(void)
{
	FILE *file = fopen("rom:/ui/scene.bin", "rb");
	uint8_t header[8];
	uint32_t count;

	menu.sky_model = menu.ring_model = -1;
	if (!file)
		return;
	if (fread(header, 1, 8, file) == 8 && !memcmp(header, "H64S", 4))
	{
		menu.sky_model = (header[4] << 8) | header[5];
		menu.ring_model = (header[6] << 8) | header[7];
		fread(menu.ring, 4, 13, file);
		fread(&count, 4, 1, file);
		/* the camera points (the title's camera goes from one to the next) */
		menu.point_count = 0;
		for (uint32_t i = 0; i < count && menu.point_count < 16; i++)
		{
			if (fread(menu.point_names[menu.point_count], 1, 32, file) != 32 ||
				fread(menu.points[menu.point_count], 4, 10, file) != 10)
				break;
			menu.point_names[menu.point_count][31] = 0;
			menu.point_count++;
		}
	}
	fclose(file);
}

static void menu_load(void)
{
	if (!menu.loaded)
	{
		menu.large = n64_font_load("large");
		menu.small = n64_font_load("small");
		menu.logo = sprite_load("rom:/ui/logo.sprite");
		load_scene();
		menu.loaded = 1;
	}
	/* the title music (going on where it was if the menu is back) */
	n64_music_start();
	/* the game's level and models give way to the scene's */
	n64_render_load_scene("ui");
	n64_render_set_clear_color(0x000000);
	n64_render_set_fog(0, 0.f, 0.f);
}

/* the menu's things give their memory back to the game (once the RSP and
the RDP are done with them) */
static void menu_unload(void)
{
	rspq_wait();
	n64_font_free(menu.large);
	n64_font_free(menu.small);
	menu.large = menu.small = NULL;
	if (menu.logo)
		sprite_free(menu.logo);
	menu.logo = NULL;
	if (menu.map_picture.sprite)
		sprite_free(menu.map_picture.sprite);
	if (menu.type_picture.sprite)
		sprite_free(menu.type_picture.sprite);
	menu.map_picture.sprite = menu.type_picture.sprite = NULL;
	menu.loaded = 0;
	n64_render_load_scene(NULL);
}

/* ---------- drawing */

static const float *camera_point(const char *name)
{
	for (int i = 0; i < menu.point_count; i++)
	{
		if (!strcmp(menu.point_names[i], name))
			return menu.points[i];
	}
	return menu.point_count ? menu.points[0] : NULL;
}

/* one value's step toward its target (observer.c's polynomial): the
displacement to go, the velocity and acceleration (updated), the time left
and the step's; returns how far to move */
static float glide(float displacement, float *velocity, float *acceleration, float remaining, float dt)
{
	float i1, i2, i3, i4, i5, v, a, b, c, r, r2, delta;

	if (remaining <= dt || remaining <= 0.f)
	{
		*velocity = *acceleration = 0.f;
		return displacement;
	}
	i1 = 1.f / remaining;
	i2 = i1 * i1;
	i3 = i2 * i1;
	i4 = i3 * i1;
	i5 = i4 * i1;
	/* (Halo's velocity runs against the time left) */
	v = -*velocity;
	a = *acceleration * i3 * 0.5f - (v * i4 * 3.f + displacement * i5 * 6.f);
	b = v * i3 * 7.f + displacement * i4 * 15.f - *acceleration * i2;
	c = *acceleration * i1 * 0.5f - (v * i2 * 4.f + displacement * i3 * 10.f);
	r = remaining - dt;
	r2 = r * r;
	delta = a * r2 * r2 * r + b * r2 * r2 + c * r2 * r + displacement;
	*velocity = -(5.f * a * r2 * r2 + 4.f * b * r2 * r + 3.f * c * r2);
	*acceleration = 20.f * a * r2 * r + 12.f * b * r2 + 6.f * c * r;
	return delta;
}

static void cross(const float *a, const float *b, float *out)
{
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];
}

static void normalize(float *v)
{
	float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	if (length > 1e-6f)
	{
		v[0] /= length;
		v[1] /= length;
		v[2] /= length;
	}
}

/* v turned about the unit axis by the angle (Rodrigues) */
static void rotate(float *v, const float *axis, float angle)
{
	float k_v[3], s = sinf(angle), c = cosf(angle), dot = axis[0] * v[0] + axis[1] * v[1] + axis[2] * v[2];

	cross(axis, v, k_v);
	for (int i = 0; i < 3; i++)
		v[i] = v[i] * c + k_v[i] * s + axis[i] * dot * (1.f - c);
}

/* the rotation (axis times angle) from the frame forward, up to the point's */
static void rotation_to(const float *forward, const float *up, const float *point, float *out)
{
	float left[3], target_left[3], m[3][3];
	const float *target_forward = point + 3, *target_up = point + 6;
	float angle, s;

	cross(up, forward, left);
	cross(target_up, target_forward, target_left);
	/* target frame times the current one's transpose (frames as columns:
	forward, left, up) */
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			m[i][j] = target_forward[i] * forward[j] + target_left[i] * left[j] + target_up[i] * up[j];
	}
	angle = acosf(fmaxf(-1.f, fminf(1.f, (m[0][0] + m[1][1] + m[2][2] - 1.f) * 0.5f)));
	s = sinf(angle);
	if (angle < 1e-4f || s < 1e-4f)
	{
		out[0] = out[1] = out[2] = 0.f;
		return;
	}
	out[0] = (m[2][1] - m[1][2]) / (2.f * s) * angle;
	out[1] = (m[0][2] - m[2][0]) / (2.f * s) * angle;
	out[2] = (m[1][0] - m[0][1]) / (2.f * s) * angle;
}

static void update_camera(void)
{
	unsigned long long now = get_ticks();
	const float *point;
	float dt, since, remaining, turn[3], step[3], angle;
	int command;

	if (!menu.point_count)
		return;
	if (!camera.started)
	{
		/* (testing, port.json flag "debug_title_time=<seconds>": the path
		from there, the camera at that command's point) */
		char text[16];
		float start = n64_debug_text("debug_title_time", text, sizeof(text)) ? (float)atof(text) : 0.f;

		point = camera_point(camera_path[(int)(start / CAMERA_INTERVAL) % CAMERA_PATH_LENGTH]);
		memset(&camera, 0, sizeof(camera));
		memcpy(camera.position, point, sizeof(camera.position));
		memcpy(camera.forward, point + 3, sizeof(camera.forward));
		memcpy(camera.up, point + 6, sizeof(camera.up));
		camera.field_of_view = point[9];
		camera.time = start;
		camera.last_ticks = now;
		camera.started = 1;
	}
	dt = (float)(now - camera.last_ticks) / (float)TICKS_PER_SECOND;
	camera.last_ticks = now;
	/* (a long stall, a game in between: no leap) */
	dt = fminf(fmaxf(dt, 0.f), 0.1f);
	camera.time += dt;
	command = (int)(camera.time / CAMERA_INTERVAL);
	since = camera.time - command * CAMERA_INTERVAL;
	remaining = CAMERA_TRANSITION - since;
	point = camera_point(camera_path[command % CAMERA_PATH_LENGTH]);
	for (int k = 0; k < 3; k++)
		camera.position[k] += glide(point[k] - camera.position[k], &camera.velocity[k], &camera.acceleration[k],
			remaining, dt);
	/* the orientation: the rotation still to go, glided as a vector */
	rotation_to(camera.forward, camera.up, point, turn);
	for (int k = 0; k < 3; k++)
		step[k] = glide(turn[k], &camera.spin_velocity[k], &camera.spin_acceleration[k], remaining, dt);
	angle = sqrtf(step[0] * step[0] + step[1] * step[1] + step[2] * step[2]);
	if (angle > 1e-6f)
	{
		float axis[3] = {step[0] / angle, step[1] / angle, step[2] / angle};
		float left[3];

		rotate(camera.forward, axis, angle);
		rotate(camera.up, axis, angle);
		/* (kept square) */
		normalize(camera.forward);
		cross(camera.up, camera.forward, left);
		cross(camera.forward, left, camera.up);
		normalize(camera.up);
	}
	if (point[9] > 0.1f)
		camera.field_of_view = point[9];
}

static void draw_scene(void)
{
	struct n64_view view;
	float c[3];

	update_camera();
	memcpy(c, camera.position, sizeof(c));
	memset(&view, 0, sizeof(view));
	for (int k = 0; k < 3; k++)
	{
		view.position[k] = camera.position[k];
		view.forward[k] = camera.forward[k];
		view.up[k] = camera.up[k];
	}
	/* the camera point's field of view is horizontal */
	view.vertical_field_of_view = camera.field_of_view > 0.1f ?
		2.f * atanf(tanf(camera.field_of_view * 0.5f) * 0.75f) : 0.9f;
	view.z_near = 0.02f;
	view.z_far = 200.f;
	/* the whole screen (the menu's 640x480; 320x240 for the loading frame
	once the game's display is back) */
	view.x0 = 0;
	view.y0 = 0;
	view.x1 = (short)n64_render_screen_width();
	view.y1 = (short)n64_render_screen_height();
	view.local_player_index = 0;
	view.visible_clusters = NULL;
	n64_render_view(&view);
	if (menu.sky_model >= 0)
	{
		/* the sky about the eye */
		static const unsigned char permutations[16];
		float matrix[13] = {1.f, 1, 0, 0, 0, 1, 0, 0, 0, 1, c[0], c[1], c[2]};
		struct n64_object sky = {
			.model_tag_index = (short)menu.sky_model, .node_count = 1, .node_matrices = matrix,
			.region_permutations = permutations, .region_count = 0,
			.center = {c[0], c[1], c[2]}, .radius = 1000.f, .tint = {0xFF, 0xFF, 0xFF, 0xFF},
		};

		n64_render_object(&sky);
	}
	if (menu.ring_model >= 0)
	{
		static const unsigned char permutations[16];
		struct n64_object ring = {
			.model_tag_index = (short)menu.ring_model, .node_count = 1, .node_matrices = menu.ring,
			.region_permutations = permutations, .region_count = 0,
			.center = {menu.ring[10], menu.ring[11], menu.ring[12]}, .radius = 40.f, .tint = {0xFF, 0xFF, 0xFF, 0xFF},
		};

		n64_render_object(&ring);
	}
	/* the scene before the menu over it */
	n64_render_flush();
}

/* The menu's screens are laid out on its 640x480 interlaced screen
(n64_render_set_menu_display), the size Halo's fonts and pictures were made
for (the Xbox's menus): text at its own size. What matters stays inside
what every television shows, ~5% in from the edges; lines are two pixels
thick (a one-pixel line is in one field of the two and flickers on an
interlaced television). */
#define SCREEN_W 640
#define SCREEN_H 480
#define SAFE_LEFT 32
#define SAFE_RIGHT (SCREEN_W - 32)
#define SAFE_BOTTOM (SCREEN_H - 24)
#define LINE 2
#define FRAME_COLOR 0xFF8CC0FFu
#define CHOSEN_COLOR 0xA0306CB8u

/* With the anamorphic widescreen setting (a 16:9 picture squeezed into the
4:3 one, the television stretching it back) the menu's 2D is squeezed by
3/4 about the screen's middle: it keeps its shape, in a 4:3 area in the
middle of the wide picture, over the scene drawn wide; what spans the whole
screen (the pages' backdrops) still does */
static float squeeze(void)
{
	return menu.widescreen ? 0.75f : 1.f;
}

static float squeezed_x(float x)
{
	float middle = n64_render_screen_width() * 0.5f;

	return middle + (x - middle) * squeeze();
}

static void menu_text(struct n64_font *font, int x, int y, unsigned long color, const char *text)
{
	if (squeeze() == 1.f)
		n64_text_draw(font, x, y, color, text);
	else
		n64_text_draw_scaled(font, squeezed_x(x), y, color, text, squeeze());
}

static void panel(int x0, int y0, int x1, int y1, unsigned long argb)
{
	int whole = x0 <= 0 && x1 >= n64_render_screen_width();

	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_set_prim_color(RGBA32((argb >> 16) & 0xFF, (argb >> 8) & 0xFF, argb & 0xFF, argb >> 24));
	if (whole || squeeze() == 1.f)
		rdpq_fill_rectangle(x0, y0, x1, y1);
	else
		rdpq_fill_rectangle(squeezed_x(x0), y0, squeezed_x(x1), y1);
}

/* a frame's four sides (not overlapping: they are blended) */
static void frame_line(int x0, int y0, int x1, int y1, unsigned long argb)
{
	panel(x0, y0, x1, y0 + LINE, argb);
	panel(x0, y1 - LINE, x1, y1, argb);
	panel(x0, y0 + LINE, x0 + LINE, y1 - LINE, argb);
	panel(x1 - LINE, y0 + LINE, x1, y1 - LINE, argb);
}

/* the chosen row's box */
static void chosen(int x0, int y0, int x1, int y1)
{
	panel(x0, y0, x1, y1, CHOSEN_COLOR);
	frame_line(x0, y0, x1, y1, FRAME_COLOR);
}

static void blit(sprite_t *sprite, int x, int y, float scale)
{
	surface_t surface;

	if (!sprite)
		return;
	surface = sprite_get_pixels(sprite);
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(1);
	/* (at its own size texel for pixel: filtering would only blur it) */
	rdpq_mode_filter(scale == 1.f && squeeze() == 1.f ? FILTER_POINT : FILTER_BILINEAR);
	if (sprite_get_format(sprite) == FMT_RGBA16 || sprite_get_format(sprite) == FMT_RGBA32)
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_tex_blit(&surface, squeezed_x(x), y, &(rdpq_blitparms_t){ .scale_x = scale * squeeze(), .scale_y = scale });
}

static void text_centered(struct n64_font *font, int center, int y, unsigned long color, const char *text)
{
	menu_text(font, center - n64_text_width(font, text) / 2, y, color, text);
}

static void text_right(struct n64_font *font, int right, int y, unsigned long color, const char *text)
{
	menu_text(font, right - n64_text_width(font, text), y, color, text);
}

/* the chosen item's text, pulsing */
static unsigned long pulsing(void)
{
	int pulse = (int)(200 + 55 * sinf(menu.frame * 0.1f));

	return ((unsigned long)pulse << 24) | (TEXT_BRIGHT & 0xFFFFFF);
}

/* a page's darkened backdrop, its title (and a note on the right) over a
rule */
static void page(const char *title, const char *note)
{
	panel(0, 0, SCREEN_W, SCREEN_H, 0x40000818);
	menu_text(menu.large, SAFE_LEFT + 8, 60, TEXT_BRIGHT, title);
	if (note)
		text_right(menu.small, SAFE_RIGHT - 8, 60, TEXT_DIM, note);
	panel(SAFE_LEFT, 68, SAFE_RIGHT, 68 + LINE, 0xC08CC0FF);
}

/* ---------- input: any controller, with the stick's repeats */

struct pad
{
	joypad_buttons_t pressed;
	int up, down, left, right; /* this frame, from the d-pad or the stick */
	int stick_held, stick_timer;
};

static struct pad pads[4];

/* testing (port.json flag "debug_menu=<script>"): a press every 20 frames,
one character each: u d l r (d-pad), a b z s (A, B, Z, START) on port 1,
upper case on port 2, '.' none */
static char script[64];
static int script_position = -1;

static void scripted_press(void)
{
	char c;
	struct pad *pad;

	if (script_position < 0)
		script_position = n64_debug_text("debug_menu", script, sizeof(script)) ? 0 : (int)sizeof(script);
	if (script_position >= (int)sizeof(script) || !script[script_position] || (int)menu.frame % 20)
		return;
	c = script[script_position++];
	pad = &pads[c >= 'A' && c <= 'Z' ? 1 : 0];
	switch (c | 0x20)
	{
	case 'u': pad->up = 1; break;
	case 'd': pad->down = 1; break;
	case 'l': pad->left = 1; break;
	case 'r': pad->right = 1; break;
	case 'a': pad->pressed.a = 1; break;
	case 'b': pad->pressed.b = 1; break;
	case 'z': pad->pressed.z = 1; break;
	case 's': pad->pressed.start = 1; break;
	}
}

static void read_pads(void)
{
	joypad_poll();
	for (int p = 0; p < 4; p++)
	{
		struct pad *pad = &pads[p];
		joypad_inputs_t inputs = joypad_get_inputs(p);
		int direction = 0;

		pad->pressed = joypad_get_buttons_pressed(p);
		pad->up = pad->pressed.d_up;
		pad->down = pad->pressed.d_down;
		pad->left = pad->pressed.d_left;
		pad->right = pad->pressed.d_right;
		if (inputs.stick_y > 50)
			direction = 1;
		else if (inputs.stick_y < -50)
			direction = 2;
		else if (inputs.stick_x < -50)
			direction = 3;
		else if (inputs.stick_x > 50)
			direction = 4;
		if (!direction)
		{
			pad->stick_held = 0;
			continue;
		}
		/* a step when pushed, then every 8 frames after 20 */
		if (direction != pad->stick_held)
		{
			pad->stick_held = direction;
			pad->stick_timer = 20;
		}
		else if (--pad->stick_timer > 0)
			continue;
		else
			pad->stick_timer = 8;
		pad->up |= direction == 1;
		pad->down |= direction == 2;
		pad->left |= direction == 3;
		pad->right |= direction == 4;
	}
	scripted_press();
}

/* ---------- the screens */

enum { SCREEN_RESULTS, SCREEN_TITLE, SCREEN_SETUP, SCREEN_SETTINGS, SCREEN_CONTROLS };
enum { ROW_GAME_TYPE, ROW_MAP, ROW_SCORE, ROW_BOTS, ROW_START, ROW_COUNT };
/* bots (port/n64/game/n64_bots.c): at most this many, whatever the
players (each biped costs ~2.7 ms of a tick: many will be slow) */
#define MAXIMUM_BOTS 8
enum { TITLE_MULTIPLAYER, TITLE_FIREFIGHT, TITLE_CONTROLS, TITLE_SETTINGS, TITLE_COUNT };
enum { CONTROL_SCHEME, CONTROL_CROUCH, CONTROL_LOOK, CONTROL_FIRE, CONTROL_STICK_SPEED, CONTROL_C_SPEED,
	CONTROL_COUNT };
static const char *const difficulties[4] = {"EASY", "NORMAL", "HEROIC", "LEGENDARY"};
enum { SETTING_CHUNKY, SETTING_ANIMATIONS, SETTING_PARTICLES, SETTING_VIEW_MODEL, SETTING_DISPLAY,
	SETTING_WIDESCREEN, SETTING_HUD_INSET, SETTING_COUNTER, SETTING_BACK, SETTING_COUNT };
#define HUD_INSET_STEP 4
#define HUD_INSET_MAXIMUM 20

/* ---------- the settings kept: the SETTINGS page and each controller's
CONTROLS, in the cartridge's EEPROM (4 kbit: the ROM's header asks for it,
tools/n64_build.py ed64romconfig --savetype eeprom4k; ares keeps it in a
save file). Read when the menu first runs; written when the SETTINGS or
CONTROLS page is left and when a game starts, if anything changed. A
missing or damaged record leaves the defaults. */

#define SAVED_BYTES 48 /* six of the EEPROM's 8-byte blocks */
#define SAVED_VERSION 1
#define SAVED_CONTROLS 16 /* where the controllers' start, 6 bytes each */

static unsigned char saved[SAVED_BYTES];
static int saved_read, saved_present;

static unsigned char saved_checksum(const unsigned char *bytes)
{
	unsigned sum = 0x5A;

	for (int i = 0; i < SAVED_BYTES - 1; i++)
		sum = sum * 31 + bytes[i];
	return (unsigned char)(sum ^ (sum >> 8) ^ (sum >> 16));
}

static void settings_pack(unsigned char *bytes)
{
	memset(bytes, 0, SAVED_BYTES);
	memcpy(bytes, "H64S", 4);
	bytes[4] = SAVED_VERSION;
	bytes[5] = (unsigned char)menu.chunky;
	bytes[6] = (unsigned char)menu.half_animation;
	bytes[7] = (unsigned char)menu.no_particles;
	bytes[8] = (unsigned char)menu.view_model;
	bytes[9] = (unsigned char)menu.interlaced;
	bytes[10] = (unsigned char)menu.widescreen;
	bytes[11] = (unsigned char)menu.hud_inset;
	bytes[12] = (unsigned char)menu.fps_counter;
	for (int p = 0; p < 4; p++)
	{
		unsigned char *controls = bytes + SAVED_CONTROLS + p * 6;

		controls[0] = n64_controls[p].scheme;
		controls[1] = n64_controls[p].toggle_crouch;
		controls[2] = n64_controls[p].inverted;
		controls[3] = n64_controls[p].swap_fire;
		controls[4] = (unsigned char)n64_controls[p].stick_speed;
		controls[5] = (unsigned char)n64_controls[p].c_speed;
	}
	bytes[SAVED_BYTES - 1] = saved_checksum(bytes);
}

/* the record's settings taken, each checked (out of range: its default) */
static int settings_unpack(const unsigned char *bytes)
{
	if (memcmp(bytes, "H64S", 4) || bytes[4] != SAVED_VERSION || bytes[SAVED_BYTES - 1] != saved_checksum(bytes))
		return 0;
	menu.chunky = bytes[5] <= 2 ? bytes[5] : 0;
	menu.half_animation = bytes[6] != 0;
	menu.no_particles = bytes[7] != 0;
	menu.view_model = bytes[8] <= 2 ? bytes[8] : 0;
	menu.interlaced = bytes[9] != 0;
	menu.widescreen = bytes[10] != 0;
	menu.hud_inset = bytes[11] <= HUD_INSET_MAXIMUM && !(bytes[11] % HUD_INSET_STEP) ? bytes[11] : 0;
	menu.fps_counter = bytes[12] != 0;
	for (int p = 0; p < 4; p++)
	{
		const unsigned char *controls = bytes + SAVED_CONTROLS + p * 6;
		signed char stick = (signed char)controls[4], c_speed = (signed char)controls[5];

		n64_controls[p].scheme = controls[0] < N64_SCHEME_COUNT ? controls[0] : 0;
		n64_controls[p].toggle_crouch = controls[1] != 0;
		n64_controls[p].inverted = controls[2] != 0;
		n64_controls[p].swap_fire = controls[3] != 0;
		/* (the 1-10 levels less 5) */
		n64_controls[p].stick_speed = stick >= -4 && stick <= 5 ? stick : 0;
		n64_controls[p].c_speed = c_speed >= -4 && c_speed <= 5 ? c_speed : 0;
	}
	return 1;
}

static void settings_load(void)
{
	if (saved_read)
		return;
	saved_read = 1;
	saved_present = eeprom_present() != EEPROM_NONE;
	if (!saved_present)
	{
		debugf("n64: no EEPROM, the settings are not kept\n");
		return;
	}
	eeprom_read_bytes(saved, 0, SAVED_BYTES);
	debugf("n64: settings %s\n", settings_unpack(saved) ? "read" : "not kept yet (defaults)");
}

static void settings_save(void)
{
	unsigned char now[SAVED_BYTES];

	if (!saved_present)
		return;
	settings_pack(now);
	if (!memcmp(now, saved, SAVED_BYTES))
		return;
	eeprom_write_bytes(now, 0, SAVED_BYTES);
	memcpy(saved, now, SAVED_BYTES);
}

/* a choice in a list: brighter, framed and pulsing when chosen (y its
baseline) */
static void list_item(int center, int y, int width, int selected, const char *text)
{
	if (selected)
		chosen(center - width / 2, y - 22, center + width / 2, y + 10);
	text_centered(menu.large, center, y, selected ? pulsing() : TEXT_COLOR, text);
}

/* a setting: its name on the left, its value on the right (between arrows
when chosen) */
static void setting_row(int x0, int x1, int y, int selected, struct n64_font *font, const char *label,
	const char *value)
{
	char arrows[48];

	if (selected)
	{
		chosen(x0, y - 22, x1, y + 10);
		snprintf(arrows, sizeof(arrows), "<  %s  >", value);
		value = arrows;
	}
	menu_text(font, x0 + 14, y, selected ? TEXT_BRIGHT : TEXT_COLOR, label);
	text_right(font, x1 - 14, y, selected ? pulsing() : TEXT_COLOR, value);
}

static void draw_title(int item)
{
	static const char *items[TITLE_COUNT] = {"MULTIPLAYER", "FIREFIGHT", "CONTROLS", "SETTINGS"};

	/* Halo's logo (with its drawing's lines) across the top, centred */
	if (menu.logo)
		blit(menu.logo, (SCREEN_W - menu.logo->width) / 2, 30, 1.f);
	for (int i = 0; i < TITLE_COUNT; i++)
		list_item(SCREEN_W / 2, 306 + i * 36, 280, i == item, items[i]);
	text_centered(menu.small, SCREEN_W / 2, 450, TEXT_DIM, "SPLIT SCREEN   -   UP TO 4 PLAYERS");
}

/* the CONTROLS page: a panel for each controller (two by two), its own rows
chosen and changed with its own buttons; each row says what it is */
static void draw_controls(void)
{
	static const char *const schemes[N64_SCHEME_COUNT] = {"STANDARD", "BUMPER JUMPER", "GOLDENEYE", "NORTH"};
	static const char *const labels[CONTROL_COUNT] = {
		"SCHEME", "CROUCH", "LOOK", "FIRE BUTTON", "STICK SPEED", "C BUTTON SPEED",
	};
	static const char *const scheme_help[N64_SCHEME_COUNT] = {
		"THE STICK LOOKS, THE C BUTTONS MOVE",
		"STANDARD, BUT R JUMPS AND A THROWS GRENADES",
		"THE STICK WALKS AND TURNS, C LEFT AND RIGHT STRAFE",
		"THE STICK WALKS AND STRAFES, THE C BUTTONS LOOK",
	};
	const struct n64_controls *chosen_controls = &n64_controls[menu.control_pad & 3];
	const char *help;
	char text[24], help_text[48];

	page("CONTROLS", "B  BACK");
	for (int p = 0; p < 4; p++)
	{
		const struct n64_controls *controls = &n64_controls[p];
		int x = SAFE_LEFT + (p & 1) * 292, y = 84 + (p >> 1) * 176, w = 284, h = 166, center = x + w / 2;
		int connected = joypad_is_connected(p);
		/* (fire on the grenade's button when swapped) */
		const char *fire = !controls->swap_fire ? "Z" : controls->scheme == N64_SCHEME_BUMPER_JUMPER ? "A" : "R";

		panel(x, y, x + w, y + h, connected ? 0xA0102040 : 0x60000000);
		frame_line(x, y, x + w, y + h, connected ? FRAME_COLOR : 0x806080A0);
		snprintf(text, sizeof(text), "PLAYER %d", p + 1);
		text_centered(menu.large, center, y + 28, connected ? TEXT_BRIGHT : TEXT_DIM, text);
		if (!connected)
		{
			text_centered(menu.small, center, y + 96, TEXT_DIM, "NO CONTROLLER");
			continue;
		}
		panel(x + 14, y + 38, x + w - 14, y + 38 + LINE, 0x808CC0FF);
		for (int row = 0; row < CONTROL_COUNT; row++)
		{
			int row_y = y + 62 + row * 19, selected = menu.control_row[p] == row;
			const char *value = text;

			switch (row)
			{
			case CONTROL_SCHEME: value = schemes[controls->scheme]; break;
			case CONTROL_CROUCH: value = controls->toggle_crouch ? "TOGGLE" : "HOLD"; break;
			case CONTROL_LOOK: value = controls->inverted ? "INVERTED" : "NORMAL"; break;
			case CONTROL_FIRE: value = fire; break;
			case CONTROL_STICK_SPEED: snprintf(text, sizeof(text), "%d", 5 + controls->stick_speed); break;
			default: snprintf(text, sizeof(text), "%d", 5 + controls->c_speed); break;
			}
			if (selected)
			{
				char arrows[32];

				chosen(x + 6, row_y - 15, x + w - 6, row_y + 5);
				snprintf(arrows, sizeof(arrows), "<  %s  >", value);
				menu_text(menu.small, x + 14, row_y, TEXT_BRIGHT, labels[row]);
				text_right(menu.small, x + w - 14, row_y, TEXT_BRIGHT, arrows);
			}
			else
			{
				menu_text(menu.small, x + 14, row_y, TEXT_DIM, labels[row]);
				text_right(menu.small, x + w - 14, row_y, TEXT_COLOR, value);
			}
		}
	}
	/* what the last controller to move chose */
	switch (menu.control_row[menu.control_pad & 3])
	{
	case CONTROL_SCHEME:
		help = scheme_help[chosen_controls->scheme];
		break;
	case CONTROL_CROUCH:
		help = chosen_controls->toggle_crouch ? "D-PAD DOWN CROUCHES OR STANDS" : "HOLD D-PAD DOWN TO CROUCH";
		break;
	case CONTROL_LOOK:
		help = chosen_controls->inverted ? "PUSH UP TO LOOK DOWN" : "PUSH UP TO LOOK UP";
		break;
	case CONTROL_FIRE:
		snprintf(help_text, sizeof(help_text), "%s FIRES, %s THROWS GRENADES",
			!chosen_controls->swap_fire ? "Z" : chosen_controls->scheme == N64_SCHEME_BUMPER_JUMPER ? "A" : "R",
			chosen_controls->swap_fire ? "Z" : chosen_controls->scheme == N64_SCHEME_BUMPER_JUMPER ? "A" : "R");
		help = help_text;
		break;
	case CONTROL_STICK_SPEED:
		help = "HOW FAST THE STICK TURNS YOU (1-10)";
		break;
	default:
		help = "HOW FAST THE C BUTTONS TURN YOU (1-10)";
		break;
	}
	snprintf(text, sizeof(text), "PLAYER %d:", (menu.control_pad & 3) + 1);
	{
		int width = n64_text_width(menu.small, text) + 8 + n64_text_width(menu.small, help);
		int x = (SCREEN_W - width) / 2;

		menu_text(menu.small, x, 448, TEXT_DIM, text);
		menu_text(menu.small, x + n64_text_width(menu.small, text) + 8, 448, TEXT_COLOR, help);
	}
}

static void draw_settings(int item)
{
	/* what the chosen row does, under the list */
	static const char *const help[SETTING_COUNT][2] = {
		{"MEDIUM: THE 3D DRAWN 256 ACROSS, WIDENED; CHUNKY:", "HALF AS WIDE, EACH PIXEL DOUBLED (HUD SHARP)"},
		{"HALF: POSES, YOUR GUN'S TOO, AT HALF THE RATE", "(HALVED AGAIN WITH 3 OR 4 PLAYERS)"},
		{"MUZZLE FLASHES, SPARKS, DUST AND BLOOD", ""},
		{"YOUR GUN AND ARMS: HIGHER HAS MORE TRIANGLES", ""},
		{"480I: TWICE THE LINES, ON MAPS WITH THE", "MEMORY FOR IT (THE BIGGEST STAY 240P)"},
		{"ANAMORPHIC: A 16:9 PICTURE, FOR A SCREEN", "THAT STRETCHES THE IMAGE TO 16:9"},
		{"MOVES THE HUD IN FROM THE SCREEN'S EDGES, FOR", "TELEVISIONS THAT CUT THEM OFF"},
		{"FRAMES A SECOND AND TIMINGS AT THE TOP", ""},
		{"BACK TO THE TITLE", ""},
	};
	char value[32];

	page("SETTINGS", NULL);
	setting_row(112, 528, 100, item == SETTING_CHUNKY, menu.large, "3D RESOLUTION",
		menu.chunky == 2 ? "CHUNKY" : menu.chunky == 1 ? "MEDIUM" : "STANDARD");
	setting_row(112, 528, 128, item == SETTING_ANIMATIONS, menu.large, "ANIMATIONS",
		menu.half_animation ? "HALF RATE" : "FULL RATE");
	setting_row(112, 528, 156, item == SETTING_PARTICLES, menu.large, "PARTICLES", menu.no_particles ? "OFF" : "ON");
	setting_row(112, 528, 184, item == SETTING_VIEW_MODEL, menu.large, "VIEW MODEL",
		menu.view_model == 2 ? "HIGH" : menu.view_model == 1 ? "MEDIUM" : "LOW");
	if (menu.hud_inset)
		snprintf(value, sizeof(value), "%d PIXELS", menu.hud_inset);
	else
		snprintf(value, sizeof(value), "OFF");
	setting_row(112, 528, 212, item == SETTING_DISPLAY, menu.large, "DISPLAY", menu.interlaced ? "480I" : "240P");
	setting_row(112, 528, 240, item == SETTING_WIDESCREEN, menu.large, "WIDESCREEN",
		menu.widescreen ? "ANAMORPHIC" : "OFF");
	setting_row(112, 528, 268, item == SETTING_HUD_INSET, menu.large, "HUD INSET", value);
	setting_row(112, 528, 296, item == SETTING_COUNTER, menu.large, "FPS COUNTER", menu.fps_counter ? "ON" : "OFF");
	list_item(SCREEN_W / 2, 332, 180, item == SETTING_BACK, "BACK");
	panel(112, 352, 528, 404, 0x80102040);
	frame_line(112, 352, 528, 404, 0x806080A0);
	for (int line = 0; line < 2; line++)
	{
		if (help[item][line][0])
			text_centered(menu.small, SCREEN_W / 2, (help[item][1][0] ? 374 : 383) + line * 18, TEXT_COLOR,
				help[item][line]);
	}
	text_centered(menu.small, SCREEN_W / 2, 448, TEXT_DIM, "A, LEFT, RIGHT  CHANGE        B  BACK");
}

static void draw_setup(const struct n64_menu_choice *c, int row, int score_index)
{
	static const char *labels[ROW_COUNT] = {"GAME TYPE", "MAP", "SCORE TO WIN", "BOTS        A  SKILL", "START GAME"};
	char value[48];
	/* (Firefight: the difficulty in the game type's row, no score; the
	players one team) */
	int teams = !c->firefight && n64_menu_game_teams(c->game_type);

	page(c->firefight ? "FIREFIGHT" : "MULTIPLAYER", "SPLIT SCREEN");
	/* the rows on the left: what it is above, its value below */
	for (int r = 0; r < ROW_COUNT; r++)
	{
		int x0 = SAFE_LEFT, x1 = 336, y = 80 + r * 50;
		int selected = r == row;

		if (c->firefight && (r == ROW_SCORE || r == ROW_BOTS))
			continue;
		if (selected)
			chosen(x0, y, x1, y + 46);
		else
		{
			panel(x0, y, x1, y + 46, 0x80102040);
			frame_line(x0, y, x1, y + 46, 0x606080A0);
		}
		if (r == ROW_START)
		{
			text_centered(menu.large, (x0 + x1) / 2, y + 30,
				c->player_count ? (selected ? pulsing() : TEXT_COLOR) : TEXT_DIM, labels[r]);
			continue;
		}
		menu_text(menu.small, x0 + 14, y + 18, selected ? TEXT_COLOR : TEXT_DIM,
			c->firefight && r == ROW_GAME_TYPE ? "DIFFICULTY" : labels[r]);
		if (r == ROW_GAME_TYPE && c->firefight)
			snprintf(value, sizeof(value), "%s", difficulties[c->difficulty & 3]);
		else if (r == ROW_GAME_TYPE)
			snprintf(value, sizeof(value), "%s", game_types[c->game_type].name);
		else if (r == ROW_MAP)
			snprintf(value, sizeof(value), "%s", maps[c->map].name);
		else if (r == ROW_BOTS)
		{
			if (c->bots > 0)
				snprintf(value, sizeof(value), "%d  %s", c->bots, difficulties[c->bot_skill & 3]);
			else
				snprintf(value, sizeof(value), "NONE");
		}
		else
			snprintf(value, sizeof(value), "%d %s", game_types[c->game_type].scores[score_index],
				game_types[c->game_type].unit);
		text_centered(menu.large, (x0 + x1) / 2, y + 38, selected ? TEXT_BRIGHT : TEXT_COLOR, value);
		if (selected)
		{
			menu_text(menu.large, x0 + 14, y + 38, pulsing(), "<");
			text_right(menu.large, x1 - 14, y + 38, pulsing(), ">");
		}
	}
	/* the map's picture (Halo's, 256x128) and the game type's */
	panel(352, 84, SAFE_RIGHT, 212, 0xC0000000);
	blit(picture(&menu.map_picture, "map", maps[c->map].picture), 352, 84, 1.f);
	frame_line(350, 82, SAFE_RIGHT + 2, 214, FRAME_COLOR);
	if (c->firefight)
	{
		text_centered(menu.large, 480, 256, TEXT_COLOR, "COVENANT IN WAVES");
		text_centered(menu.small, 480, 282, TEXT_DIM, "SHARED LIVES");
	}
	else
	{
		panel(384, 226, 576, 322, 0xC0000000);
		blit(picture(&menu.type_picture, "gametype", game_types[c->game_type].picture), 384, 226, 1.f);
		frame_line(382, 224, 578, 324, FRAME_COLOR);
	}
	/* the players */
	for (int p = 0; p < 4; p++)
	{
		int x = SAFE_LEFT + p * 146, y = 338, w = 138, h = 92, center = x + w / 2;
		int slot = -1;

		for (int i = 0; i < c->player_count; i++)
		{
			if (c->ports[i] == p)
				slot = i;
		}
		panel(x, y, x + w, y + h, slot >= 0 ? 0xA0102040 : 0x60000000);
		frame_line(x, y, x + w, y + h, slot >= 0 ? FRAME_COLOR : 0x806080A0);
		if (slot < 0)
		{
			if (joypad_is_connected(p))
			{
				text_centered(menu.large, center, y + 44, TEXT_COLOR, "PRESS A");
				text_centered(menu.small, center, y + 66, TEXT_DIM, "TO JOIN");
			}
			else
				text_centered(menu.small, center, y + 52, TEXT_DIM, "NO CONTROLLER");
			continue;
		}
		snprintf(value, sizeof(value), "PLAYER %d", p + 1);
		text_centered(menu.large, center, y + 26, TEXT_BRIGHT, value);
		panel(x + 12, y + 36, x + w - 12, y + 62, 0xFF000000 | (teams ? team_rgb[c->teams[slot]] : player_rgb[slot & 3]));
		if (teams)
			text_centered(menu.small, center, y + 55, TEXT_BRIGHT, c->teams[slot] ? "BLUE TEAM" : "RED TEAM");
		text_centered(menu.small, center, y + 82, TEXT_DIM, teams ? "Z  TEAM" : "B  LEAVE");
	}
	text_centered(menu.small, SCREEN_W / 2, 452, TEXT_DIM, "A  JOIN        B  LEAVE        Z  TEAM        START  PLAY");
}

static void draw_results(const char *const *lines, int count)
{
	panel(112, 92, 528, 400, 0xC0001028);
	frame_line(112, 92, 528, 400, FRAME_COLOR);
	text_centered(menu.large, SCREEN_W / 2, 136, TEXT_BRIGHT, "GAME OVER");
	panel(152, 150, 488, 150 + LINE, 0x808CC0FF);
	/* (the winner, then each player and bot: up to eight) */
	for (int i = 0; i < count && i < 9; i++)
	{
		if (i == 0)
			text_centered(menu.large, SCREEN_W / 2, 186, TEXT_BRIGHT, lines[i]);
		else
			text_centered(menu.small, SCREEN_W / 2, 200 + i * (count > 6 ? 20 : 26), TEXT_COLOR, lines[i]);
	}
	text_centered(menu.small, SCREEN_W / 2, 388, TEXT_DIM, "PRESS A");
}

/* the loading screen: on the menu's screen, and once more on the game's
320x240 one (shown while the map loads) */
static void draw_loading(const struct n64_menu_choice *c)
{
	char text[64];
	int width = n64_render_screen_width(), middle = n64_render_screen_height() / 2;
	int high = width >= SCREEN_W;

	snprintf(text, sizeof(text), "%s ON %s", c->firefight ? "FIREFIGHT" : game_types[c->game_type].name, maps[c->map].name);
	panel(0, middle - (high ? 40 : 24), width, middle + (high ? 40 : 24), 0xC0000818);
	text_centered(menu.large, width / 2, middle - (high ? 4 : 2), TEXT_BRIGHT, "LOADING");
	text_centered(menu.small, width / 2, middle + (high ? 24 : 16), TEXT_COLOR, text);
}

/* ---------- the menu */

int n64_menu_run(struct n64_menu_choice *c, const char *const *results, int result_count)
{
	static int score_index = -1;
	int screen = result_count > 0 ? SCREEN_RESULTS : (c->player_count ? SCREEN_SETUP : SCREEN_TITLE);
	int row = ROW_GAME_TYPE, item = TITLE_MULTIPLAYER, setting = SETTING_CHUNKY;

	/* the menu at 640x480 interlaced (the game's display given back at the
	start of a game) */
	n64_render_set_menu_display(1);
	menu_load();
	settings_load();
	/* the menu itself always at full width */
	n64_render_set_chunky(0);
	n64_render_show_counter(0);
	if (c->map < 0 || c->map >= MAP_COUNT)
		c->map = n64_menu_map_from_folder("wizard");
	if (c->game_type < 0 || c->game_type >= GAME_TYPE_COUNT)
		c->game_type = 0;
	if (score_index < 0)
		score_index = game_types[c->game_type].default_score;
	for (;;)
	{
		int start = 0;
		int previous_screen = screen;

		menu.frame += 1.f;
		read_pads();
		if (screen == SCREEN_RESULTS)
		{
			for (int p = 0; p < 4; p++)
			{
				if (pads[p].pressed.a || pads[p].pressed.start)
					screen = SCREEN_SETUP;
			}
		}
		else if (screen == SCREEN_TITLE)
		{
			for (int p = 0; p < 4 && screen == SCREEN_TITLE; p++)
			{
				if (pads[p].up)
					item = (item + TITLE_COUNT - 1) % TITLE_COUNT;
				if (pads[p].down)
					item = (item + 1) % TITLE_COUNT;
				if (!pads[p].pressed.start && !pads[p].pressed.a)
					continue;
				if (item == TITLE_SETTINGS)
				{
					screen = SCREEN_SETTINGS;
					setting = SETTING_CHUNKY;
					continue;
				}
				if (item == TITLE_CONTROLS)
				{
					screen = SCREEN_CONTROLS;
					menu.control_pad = p;
					continue;
				}
				c->firefight = item == TITLE_FIREFIGHT;
				{
					/* (normal, the first time) */
					static int difficulty_chosen;

					if (c->firefight && !difficulty_chosen)
					{
						c->difficulty = 1;
						difficulty_chosen = 1;
					}
				}
				screen = SCREEN_SETUP;
				row = ROW_GAME_TYPE;
				/* the player who pressed is in */
				if (!c->player_count)
				{
					c->ports[0] = p;
					c->teams[0] = 0;
					c->player_count = 1;
				}
			}
		}
		else if (screen == SCREEN_SETTINGS)
		{
			for (int p = 0; p < 4 && screen == SCREEN_SETTINGS; p++)
			{
				struct pad *pad = &pads[p];

				if (pad->up)
					setting = (setting + SETTING_COUNT - 1) % SETTING_COUNT;
				if (pad->down)
					setting = (setting + 1) % SETTING_COUNT;
				/* (standard, medium, chunky) */
				if (setting == SETTING_CHUNKY && pad->left)
					menu.chunky = (menu.chunky + 2) % 3;
				else if (setting == SETTING_CHUNKY && (pad->right || pad->pressed.a))
					menu.chunky = (menu.chunky + 1) % 3;
				if (setting == SETTING_DISPLAY && (pad->left || pad->right || pad->pressed.a))
					menu.interlaced = !menu.interlaced;
				if (setting == SETTING_ANIMATIONS && (pad->left || pad->right || pad->pressed.a))
					menu.half_animation = !menu.half_animation;
				if (setting == SETTING_WIDESCREEN && (pad->left || pad->right || pad->pressed.a))
					menu.widescreen = !menu.widescreen;
				if (setting == SETTING_PARTICLES && (pad->left || pad->right || pad->pressed.a))
					menu.no_particles = !menu.no_particles;
				if (setting == SETTING_VIEW_MODEL && pad->left)
					menu.view_model = (menu.view_model + 2) % 3;
				else if (setting == SETTING_VIEW_MODEL && (pad->right || pad->pressed.a))
					menu.view_model = (menu.view_model + 1) % 3;
				if (setting == SETTING_HUD_INSET && pad->left)
					menu.hud_inset = (menu.hud_inset + HUD_INSET_MAXIMUM) % (HUD_INSET_MAXIMUM + HUD_INSET_STEP);
				else if (setting == SETTING_HUD_INSET && (pad->right || pad->pressed.a))
					menu.hud_inset = (menu.hud_inset + HUD_INSET_STEP) % (HUD_INSET_MAXIMUM + HUD_INSET_STEP);
				if (setting == SETTING_COUNTER && (pad->left || pad->right || pad->pressed.a))
					menu.fps_counter = !menu.fps_counter;
				if (pad->pressed.b || pad->pressed.start || (setting == SETTING_BACK && pad->pressed.a))
					screen = SCREEN_TITLE;
			}
		}
		else if (screen == SCREEN_CONTROLS)
		{
			for (int p = 0; p < 4 && screen == SCREEN_CONTROLS; p++)
			{
				struct pad *pad = &pads[p];
				struct n64_controls *controls = &n64_controls[p];
				int *row = &menu.control_row[p];
				int step = pad->left ? -1 : (pad->right || pad->pressed.a) ? 1 : 0;

				if (pad->up || pad->down || step)
					menu.control_pad = p;
				if (pad->up)
					*row = (*row + CONTROL_COUNT - 1) % CONTROL_COUNT;
				if (pad->down)
					*row = (*row + 1) % CONTROL_COUNT;
				if (step && *row == CONTROL_SCHEME)
					controls->scheme = (controls->scheme + N64_SCHEME_COUNT + step) % N64_SCHEME_COUNT;
				else if (step && *row == CONTROL_CROUCH)
					controls->toggle_crouch = !controls->toggle_crouch;
				else if (step && *row == CONTROL_LOOK)
					controls->inverted = !controls->inverted;
				else if (step && *row == CONTROL_FIRE)
					controls->swap_fire = !controls->swap_fire;
				else if (step && (*row == CONTROL_STICK_SPEED || *row == CONTROL_C_SPEED))
				{
					/* 1-10 (kept less 5): left and right step, A goes round */
					signed char *speed = *row == CONTROL_STICK_SPEED ? &controls->stick_speed : &controls->c_speed;
					int level = 5 + *speed;

					if (pad->left)
						level = level > 1 ? level - 1 : 1;
					else if (pad->right)
						level = level < 10 ? level + 1 : 10;
					else
						level = level % 10 + 1;
					*speed = (signed char)(level - 5);
				}
				if (pad->pressed.b || pad->pressed.start)
					screen = SCREEN_TITLE;
			}
		}
		else
		{
			for (int p = 0; p < 4; p++)
			{
				struct pad *pad = &pads[p];
				int slot = -1;

				for (int i = 0; i < c->player_count; i++)
				{
					if (c->ports[i] == p)
						slot = i;
				}
				if (slot < 0)
				{
					/* A joins (on the START row it starts the game, once
					someone has) */
					if (pad->pressed.a && !(row == ROW_START && c->player_count) && c->player_count < 4)
					{
						c->ports[c->player_count] = p;
						c->teams[c->player_count] = c->player_count & 1;
						c->player_count++;
						continue;
					}
					if (pad->pressed.b && !c->player_count)
						screen = SCREEN_TITLE;
				}
				else
				{
					if (pad->pressed.b)
					{
						for (int i = slot; i + 1 < c->player_count; i++)
						{
							c->ports[i] = c->ports[i + 1];
							c->teams[i] = c->teams[i + 1];
						}
						c->player_count--;
						if (!c->player_count)
							screen = SCREEN_TITLE;
						continue;
					}
					if ((pad->pressed.z || pad->pressed.r) && !c->firefight)
						c->teams[slot] ^= 1;
					/* (A on the bots' row: their skill) */
					if (pad->pressed.a && row == ROW_BOTS && !c->firefight)
						c->bot_skill = (c->bot_skill + 1) & 3;
				}
				if (pad->up)
					row = (row + ROW_COUNT - 1) % ROW_COUNT;
				if (pad->down)
					row = (row + 1) % ROW_COUNT;
				/* (Firefight has no score or bots rows) */
				while (c->firefight && (row == ROW_SCORE || row == ROW_BOTS))
					row = (row + (pad->up ? ROW_COUNT - 1 : 1)) % ROW_COUNT;
				if (pad->left || pad->right)
				{
					int step = pad->right ? 1 : -1;

					if (row == ROW_GAME_TYPE && c->firefight)
						c->difficulty = (c->difficulty + 4 + step) % 4;
					else if (row == ROW_GAME_TYPE)
					{
						c->game_type = (c->game_type + GAME_TYPE_COUNT + step) % GAME_TYPE_COUNT;
						score_index = game_types[c->game_type].default_score;
					}
					else if (row == ROW_MAP)
						c->map = (c->map + MAP_COUNT + step) % MAP_COUNT;
					else if (row == ROW_SCORE)
						score_index = (score_index + 5 + step) % 5;
					else if (row == ROW_BOTS)
						c->bots = c->bots + step < 0 ? 0 : c->bots + step > MAXIMUM_BOTS ?
							MAXIMUM_BOTS : c->bots + step;
				}
				if (pad->pressed.start || (pad->pressed.a && row == ROW_START && slot >= 0))
					start = c->player_count > 0;
			}
		}
		c->score = game_types[c->game_type].scores[score_index];
		for (int i = 0; i < c->player_count; i++)
			c->colors[i] = c->firefight ? player_colors[i & 3] :
				n64_menu_game_teams(c->game_type) ? (c->teams[i] ? 3 : 2) : player_colors[i & 3];

		/* (the settings kept when their page is left) */
		if (screen != previous_screen && (previous_screen == SCREEN_SETTINGS || previous_screen == SCREEN_CONTROLS))
			settings_save();
		n64_render_frame_begin();
		n64_render_set_anamorphic(menu.widescreen);
		draw_scene();
		if (screen == SCREEN_RESULTS)
			draw_results(results, result_count);
		else if (screen == SCREEN_TITLE)
			draw_title(item);
		else if (screen == SCREEN_SETTINGS)
			draw_settings(setting);
		else if (screen == SCREEN_CONTROLS)
			draw_controls();
		else if (start)
			draw_loading(c);
		else
			draw_setup(c, row, score_index);
		n64_render_frame_end();
		if (start)
		{
			/* the game's 320x240 display back (its buffers where they were
			before the menu), the loading screen on it while the map loads */
			n64_render_set_menu_display(0);
			n64_render_frame_begin();
			n64_render_set_anamorphic(menu.widescreen);
			draw_scene();
			draw_loading(c);
			n64_render_frame_end();
			/* the game in chunky mode or not from its first frame */
			n64_render_set_chunky(menu.chunky);
			n64_particles_set_enabled(!menu.no_particles);
			n64_render_set_view_model_quality(menu.view_model);
			n64_hud_set_inset(menu.hud_inset);
			n64_render_show_counter(menu.fps_counter);
			n64_render_set_480i(menu.interlaced);
			n64_half_animation_rate = menu.half_animation;
			settings_save();
			n64_render_set_anamorphic(menu.widescreen);
			/* everyone starts standing */
			for (int p = 0; p < 4; p++)
			{
				n64_controls[p].crouched = 0;
				n64_controls[p].last_buttons = 0;
			}
			n64_render_set_clear_color(0x60708C);
			menu_unload();
			n64_music_stop();
			return 1;
		}
	}
}
