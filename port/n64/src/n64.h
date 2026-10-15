/*
N64.H

The bridge between the two halves of the N64 platform layer. Units compiled
with the game's ABI and the XDK declarations (xdk_*.c, the game itself)
cannot include libdragon's headers (both declare the same names), so the
libdragon-facing units (n64_*.c) export what the game side needs through
these plain-C declarations.
*/

#ifndef __HALO_N64_H
#define __HALO_N64_H

/* min and max of two floats, inline: newlib's fminf and fmaxf ask
__fpclassifyf about both arguments, calls in per-particle and per-object
loops (no NaNs come here) */
static inline float n64_min_f(float a, float b) { return a < b ? a : b; }
static inline float n64_max_f(float a, float b) { return a > b ? a : b; }

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- system (n64_main.c) */

/* debug output, one line per call (no newline needed) */
void n64_log(const char *text);
/* a fatal error: shows the message on screen and stops */
void n64_fatal(const char *text) __attribute__((noreturn));
/* prints the call stack to the debug output */
void n64_backtrace(void);
/* the CPU's count register: 46.875 MHz, wraps every 91 s */
unsigned long n64_ticks(void);
/* the same counter in 64 bits, never wrapping */
unsigned long long n64_ticks64(void);
#define N64_TICKS_PER_SECOND 46875000
/* microseconds since boot, 64 bits */
unsigned long long n64_microseconds(void);
/* busy-waits */
void n64_wait_microseconds(unsigned long microseconds);
/* memory left on the heap, and the largest free block */
unsigned long n64_heap_free(void);
/* x / 255 for x up to 255 x 255, exactly, in four simple operations (GCC
makes a division by 255 a 64-bit multiply and shifts: the multiply and its
result's wait are most of a dozen cycles) */
static inline unsigned n64_div255(unsigned x)
{
	return (x + 1 + (x >> 8)) >> 8;
}
unsigned long n64_heap_describe(char *text, unsigned long size);
int n64_heap_has(unsigned long size);
void n64_heap_dump(const char *tag, const void *start);
/* the maps' home (n64_game.c) and the display buffers lent from it
(n64_main.c, n64_render.c) */
void n64_map_home_reserve(void);
void *n64_map_home_free_part(unsigned long *size);
void n64_display_set_home(void *start, unsigned long size);
void n64_display_lend(void *start, unsigned long size);
int n64_render_home_in_use(void);
int n64_render_view_models_hidden(void);
/* a reserve for libdragon's uncached blocks (n64_main.c) */
#define N64_UNCACHED_RESERVE (64 * 1024)
int n64_uncached_reserve_take(void);
void n64_uncached_reserve_release(void);
void n64_uncached_reserve_settle(void);
int n64_uncached_reserve_missing(void);
int n64_uncached_reserve_spent(void);

/* TRUE when the cartridge carries the file rom:/<name> (debug switches,
port.json "flags"); looked up once per name */
int n64_debug_flag(const char *name);
/* a debug setting's value (port.json flag "name=value"): 0 when unset */
int n64_debug_text(const char *name, char *buffer, int size);
/* the profiler samples only inside a zone with the flag profile_zone */
extern volatile int n64_profile_zone;

/* ---------- cartridge files (n64_files.c): DragonFS, read-only */

/* returns a handle (>= 0) or -1 */
long n64_file_open(const char *path);
long n64_file_size(long handle);
long n64_file_read(long handle, void *buffer, long size);
long n64_file_seek(long handle, long offset);
void n64_file_close(long handle);

/* ---------- animation frames streamed from the cartridge (n64_anim.c) */

void n64_animation_open(const char *map_name);
void *n64_animation_frame(long data_offset, long data_size, short frame_index, short frame_size);

/* ---------- controllers (n64_input.c) */

struct n64_pad
{
	unsigned char connected;
	unsigned char pad[3];
	signed char stick_x, stick_y;
	unsigned short buttons; /* N64_BUTTON_* */
};

enum
{
	N64_BUTTON_A = 1 << 0,
	N64_BUTTON_B = 1 << 1,
	N64_BUTTON_Z = 1 << 2,
	N64_BUTTON_START = 1 << 3,
	N64_BUTTON_D_UP = 1 << 4,
	N64_BUTTON_D_DOWN = 1 << 5,
	N64_BUTTON_D_LEFT = 1 << 6,
	N64_BUTTON_D_RIGHT = 1 << 7,
	N64_BUTTON_L = 1 << 8,
	N64_BUTTON_R = 1 << 9,
	N64_BUTTON_C_UP = 1 << 10,
	N64_BUTTON_C_DOWN = 1 << 11,
	N64_BUTTON_C_LEFT = 1 << 12,
	N64_BUTTON_C_RIGHT = 1 << 13,
};

/* what is kept from the first map on, made at boot, at the heap's bottom
(n64_main.c): the animation cache, the HUD's meters, blip and font, the
explosions' fireball */
void n64_animation_reserve(void);
void n64_hud_reserve(void);
void n64_particles_reserve(void);
void n64_hud_release(void);
void n64_particles_release(void);

void n64_input_poll(void);
void n64_input_get(int port, struct n64_pad *pad);
/* the buttons pressed on a port since this was last asked (N64_BUTTON_*:
START, A, B and D-pad, gathered at every n64_input_poll, so that a press between
two frames is not missed), and all of them forgotten */
unsigned short n64_input_take_pressed(int port);
void n64_input_clear_pressed(void);
/* the game's gamepads read as untouched (xdk_input.c): while the pause
menu is up */
extern int n64_input_blocked;
/* the Rumble Pak (n64_input.c): a frame's motor speeds (0-1, Halo's heavy
and light motors), from XInputSetState; whether a port has one; the
menu's buzz (frames) and its frame (every motor off but that) */
void n64_input_rumble(int port, float left, float right);
int n64_input_rumble_present(int port);
void n64_input_rumble_pulse(int port, int frames);
void n64_input_rumble_menu_frame(void);

/* each controller port's controls, chosen on the menu's CONTROLS page and
read by xdk_input.c (all zero: the standard scheme, crouch held, look not
inverted) */
enum
{
	N64_SCHEME_STANDARD,      /* the stick looks, the C buttons move */
	N64_SCHEME_BUMPER_JUMPER, /* the standard one, R jumping and A throwing grenades */
	N64_SCHEME_GOLDENEYE,     /* the stick turns and walks, C left and right strafe, C up and down look */
	N64_SCHEME_NORTH,         /* the stick walks and strafes, the C buttons look */
	N64_SCHEME_GOLDEN_STYLE,  /* optional free aim, held R and threshold turning */
	N64_SCHEME_COUNT
};

struct n64_controls
{
	unsigned char scheme;        /* N64_SCHEME_* */
	unsigned char toggle_crouch; /* crouch toggled by its button, not held */
	unsigned char inverted;      /* pushing up looks down */
	unsigned char swap_fire;     /* fire on the grenade's button, grenades on Z */
	/* look speeds, the stick's and the C buttons': the menu's 1-10 less 5
	(all zero: 5), 1.25 times faster a step */
	signed char stick_speed, c_speed;
	/* (xdk_input.c's) the toggled crouch */
	unsigned char crouched;
	/* the Rumble Pak turned off on the CONTROLS page (n64_input.c) */
	unsigned char no_rumble;
	/* (xdk_input.c's) the buttons last read */
	unsigned short last_buttons;
};

extern struct n64_controls n64_controls[4];

/* Shared by title Controls and the in-game, per-port Controls panels. */
enum { CONTROL_SCHEME, CONTROL_CROUCH, CONTROL_LOOK, CONTROL_FIRE, CONTROL_STICK_SPEED, CONTROL_C_SPEED,
	CONTROL_RUMBLE, CONTROL_COUNT };
void n64_controls_step(int port, int row, int step, int cycle);
void n64_controls_save(void);

/* a local player's look speeds across and up and down (xdk_input.c; Halo's
turning rates times them, source/game/player_control.c): the sensitivity of
whichever turns that way in the player's scheme, the stick or the C buttons */
void n64_look_speed(int local_player_index, float *yaw, float *pitch);
int n64_golden_raw(int local_player_index, float *x, float *y, int *aim);
int n64_golden_selected(int local_player_index);
int n64_golden_active(int local_player_index);
int n64_golden_manual(int local_player_index);
void n64_golden_update(int local, float dt, float *yaw, float *pitch, float *forward, float *strafe);
void n64_golden_reset(void);
void n64_golden_ray(int local_player_index, float *direction);
void n64_golden_move(int local, float *forward, float *strafe);
void n64_golden_weapon_axes(int local, float *forward, float *up);
int n64_golden_projectile(long player_index, const float *position, float *direction, long *target_object_index);
unsigned short n64_golden_buttons(int local);
int n64_golden_zoom(int local, long weapon_index);
void n64_golden_cursor(int local_player_index, float *x, float *y);
void n64_modifiers_set(int big_head, int paintball);
int n64_big_head_enabled(void);
int n64_paint_enabled(void);
struct n64_paint_vertex { float position[3], uv[2]; };
#define N64_PAINT_VERTICES 12
void n64_paint_submit(const struct n64_paint_vertex *vertices, int count, int pattern);
void n64_paint_reset(const char *map_name);
void n64_decal_submit(const struct n64_paint_vertex *vertices, int count, const char *name,
    float lifetime, float fade, const unsigned char color[4]);
void n64_paint_update(float dt);
struct n64_particle_view;
void n64_paint_draw(const struct n64_particle_view *view);
/* Shared immutable ink art and palette; no per-view mark search. */
const unsigned char *n64_paint_mask(int pattern);
const unsigned char *n64_paint_color(int color);
int n64_paint_next_color(void);
int n64_paint_effect(const char *name);
void n64_paint_splash(const float position[3], float radius, int color);


/* ---------- the renderer (n64_render.c)

A frame is n64_render_frame_begin, one n64_render_view per player's view,
then n64_render_frame_end, which shows it. Positions are the game's world
units; the views' rectangles are pixels of the screen: the game's 320x240,
the menu's 640x480 (n64_render_set_menu_display). */

struct n64_view
{
	float position[3];
	float forward[3];
	float up[3];
	float vertical_field_of_view;
	float z_near, z_far;
	short x0, y0, x1, y1;
	short local_player_index;
	/* the level's clusters seen from the eye's (Halo's potentially visible
	set: a bit per cluster), or NULL for all of them */
	short cluster_count;
	const unsigned long *visible_clusters;
};

/* an object to draw in the current view: its model (the model tag's index),
its node matrices as the game keeps them (real_matrix4x3: scale, forward,
left, up, position; world units), the permutation of each region, and its
bounding sphere */
struct n64_object
{
	short model_tag_index;
	short node_count;
	const float *node_matrices;
	const unsigned char *region_permutations;
	short region_count;
	short head_node; /* -1: unchanged; renderer only, collision never changes */
	unsigned long head_mask[2];
	float center[3];
	float radius;
	/* a player's colour (where the model's textures take it), else white */
	unsigned char tint[4];
	/* the object (non-zero, the same in every view of a frame: its node
	matrices are converted once a frame), or 0 */
	unsigned long key;
};

void n64_render_initialize(void);
/* loads rom:/levels/<map>.lvl and its textures (with low, the lighter
<map>-low.lvl where there is one); FALSE if there is none */
int n64_render_load_level(const char *map_name, int low);
void n64_render_frame_begin(void);
void n64_render_view(const struct n64_view *view);
/* after n64_render_view: objects in that view */
/* returns whether it is in the view (not culled) */
int n64_render_object(const struct n64_object *object);
/* the menu: a scene's models without a level (levels/<name>.mdl; NULL:
none), the colour a frame is cleared to (0xRRGGBB) */
void n64_render_load_scene(const char *name);
void n64_render_unload_level(void);
void n64_render_set_clear_color(unsigned long rgb);
/* how far the views see (world units): fog from start, the colour (0xRRGGBB)
reached at end, nothing drawn past it (the frame cleared to the colour);
end 0: no fog, the views' own far planes */
void n64_render_set_fog(unsigned long rgb, float start, float end);
/* the sky behind the views, while there is fog: its colours (0xRRGGBB) at
0, 15, 30, 50 and 90 degrees above the horizon (N64_SKY_STOPS), or NULL:
the fog's colour alone */
#define N64_SKY_STOPS 5
void n64_render_set_sky(const unsigned long *colors);
/* the FPS counter's line of the game's own (n64_campaign.c), 0 for none */
int n64_game_debug_line(char *text, int size);
/* the views this frame: small views draw objects' far versions sooner */
void n64_render_set_view_count(int count);
/* then the first-person weapon: its own near plane, drawn over the world */
void n64_render_first_person_begin(void);
void n64_render_frame_end(void);
/* draws the objects queued so far (n64_render_object queues them, sorted
by texture): before 2D drawn over them */
void n64_render_flush(void);
/* chunky mode (the menu's settings): from the next frame on, the views'
3D is drawn half as wide and its pixels doubled across (the HUD and text
stay at full width) */
void n64_render_set_chunky(int level); /* 0 off, 1 medium (4/5 width), 2 chunky (half) */
/* the view models' quality (the menu's settings), for the next level
loaded: 0 low, 1 medium, 2 high -- the first-person weapons and arms at
1.25 or 2 times the triangles (levels/<map>-fp1.mdl, -fp2) */
void n64_render_set_view_model_quality(int quality);
/* the FPS counter in the corner (off in the menu) */
void n64_render_show_counter(int on);
/* the menu's display, 640x480 interlaced (two buffers and a z-buffer of
that size), or back to the game's 320x240: the old display's memory freed
first, the game's buffers where they were before the menu */
void n64_render_set_menu_display(int on);
int n64_render_screen_width(void);
/* Settings: the game in high detail -- 480i where the map leaves room
(640x480, else 512x480), else 640x240 at 240p (Perfect Dark's high
resolution), else 320x240 and a word on the screen -- tried once the map is
in; the anamorphic picture (16:9 squeezed into 4:3: a wider view across, the
HUD squeezed about each view's middle) */
void n64_render_set_high_detail(int on);
void n64_render_set_anamorphic(int on);
/* the N64's pixels for one of the game's units (it lays the views and the
HUD out on a 640x480 screen), across and down; the HUD's squeeze */
float n64_render_layout_scale_x(void);
float n64_render_layout_scale_y(void);
float n64_render_hud_squeeze(void);
int n64_render_screen_height(void);

/* ---------- sound (n64_sound.c): the main menu's music, the game's
sounds (port/n64/game/n64_sound_game.c) */

void n64_sound_initialize(void);
void n64_sound_reserve(void);
/* the mixer fed (every frame's end, and between ticks and views) and the
music's next track started */
void n64_sound_poll(void);
/* the title music from its start (not again while it plays), and off */
void n64_music_start(void);
void n64_music_stop(void);
/* the map's sounds: rom:/snd/<map>.ofs, the offsets in the sound bank of
the files of the map's sound list (FALSE: none) */
int n64_sound_load_map(const char *map_name);
void n64_sound_unload_map(void);
/* plays file `file` of the map's list on a voice (a free one, or the
cheapest of no higher priority, 0-255): its volume 0-1, pan -1 (left) to 1
(right), pitch (a rate: 1 as recorded), looping or once; returns the voice
(>= 0) or -1 */
long n64_voice_play(int file, float volume, float pan, float pitch, int priority, int loop);
void n64_voice_set(long voice, float volume, float pan, float pitch);
void n64_voice_stop(long voice);
/* FALSE once it has ended, been stopped or been stolen */
int n64_voice_playing(long voice);
void n64_voice_stop_all(void);
/* the game's voices silent while the game is paused (they play on
unheard), and heard again */
void n64_sound_pause(int paused);
void n64_sound_statistics(unsigned long *started, unsigned long *stolen, unsigned long *refused);

/* ---------- particles (n64_particles.c): what effects make, simply */

/* an effect's particle emitter, as tools/n64/effects.py writes it */
struct n64_fx_emitter
{
	unsigned char location;
	unsigned char flags; /* camera mode (0-1), attached (2), additive (3) */
	unsigned char cell, distribution;
	unsigned char count_min, count_max, violence;
	signed char gravity; /* world units/s^2, x8 */
	unsigned short life_min, life_max, fade_in, fade_out; /* ms */
	float velocity_min, velocity_max, cone_angle;
	float radius_min, radius_max;
	float distribution_min, distribution_max;
	float offset[3], direction[3];
	unsigned char tint_lower[4], tint_upper[4]; /* RGBA */
	unsigned char drag; /* per second, /16 */
	unsigned char end_scale; /* /32 */
	unsigned char pad[2];
};

#define N64_FX_ADDITIVE 0x08

enum
{
	N64_PARTICLE_FIRST_PERSON = 1, /* in its local player's camera space */
	N64_PARTICLE_THIRD_PERSON_ONLY = 2, /* not in its local player's view */
	N64_PARTICLE_ADDITIVE = 4, /* its colour's alpha left out */
	N64_PARTICLE_FLAME = 8, /* source cloud-fire shape, hot core and warm edge */
};

struct n64_particle_spawn
{
	/* world units; first person: along the camera's right, up, forward */
	float position[3];
	float velocity[3]; /* per second, the same way */
	float radius, end_scale;
	float life, fade_in, fade_out; /* seconds */
	unsigned long color; /* RGBA */
	float gravity, drag;
	unsigned char cell, flags;
	signed char local_player; /* -1: none */
	unsigned char pad;
};

/* a view's camera, for drawing (n64_render.c) */
struct n64_particle_view
{
	float eye[3], right[3], up[3], forward[3];
	float pixels_per_unit, center_x, center_y;
	/* across: x scale of the pixels per unit (a pixel wider than tall:
	512x480, the anamorphic picture) */
	float x_scale;
	float x0, y0, x1, y1;
	float z_near, z_far; /* world units, this pass's */
	float fog_start, fog_end; /* 0: no fog */
	int local_player, first_person, chunky;
	/* chunky's or medium's: the drawn width over the view's (1/2, 4/5) */
	float chunky_scale;
};

/* the map's particles: rom:/fx/<map>.fx */
void n64_particles_load(const char *map_name);
/* an effect's event's emitters (by the effect's tag index), or NULL; valid
until the next call */
const struct n64_fx_emitter *n64_fx_event(int effect_tag_index, int event_index, int *count);
void n64_particle_spawn(const struct n64_particle_spawn *spawn);
void n64_flame_trail(const float *position, const float *velocity);
/* Short visual-only segment along the actual sniper projectile movement. */
void n64_sniper_ribbon(const float *start, const float *end);
/* explosions: the N64's own flash, fireball and smoke for the effects the
converter marks (tools/n64/effects.py EXPLOSIONS), started by the effect
code (source/effects/effects.c, HALO_N64) where the effect begins */
enum
{
	N64_EXPLOSION_FRAG,
	N64_EXPLOSION_PLASMA,
	N64_EXPLOSION_ROCKET,
	N64_EXPLOSION_SHELL,
	N64_EXPLOSION_NEEDLER,
	N64_EXPLOSION_FUEL_ROD,
	N64_EXPLOSION_KINDS
};
/* the kind of explosion an effect is (by its tag index), or -1 */
int n64_explosion_kind(int effect_tag_index);
void n64_explosion(const float position[3], int kind);
void n64_particles_update(float dt);
/* the menu's setting: none made when off */
void n64_particles_set_enabled(int on);
void n64_particles_set_enhanced(int on);
int n64_particles_enhanced(void);
/* the views drawn (more views: fewer particles made) */
void n64_particles_set_view_count(int views);
void n64_particles_draw(const struct n64_particle_view *view);
void n64_particles_statistics(unsigned long *spawned, unsigned long *drawn, unsigned long *reads, int *live);
/* the view's 3D done (n64_render_game.c, after the first-person weapon):
its particles drawn */
void n64_render_view_particles(void);

/* ---------- Halo's fonts (n64_font.c): rom:/fonts/<name>.* from
tools/n64/ui.py (large, small, interstate) */

struct n64_font;
struct n64_font *n64_font_load(const char *name);
void n64_font_free(struct n64_font *font);
int n64_font_height(const struct n64_font *font);
int n64_font_ascent(const struct n64_font *font);
int n64_font_cap_height(const struct n64_font *font);
int n64_text_width(const struct n64_font *font, const char *text);
/* text with its baseline at y; color 0xAARRGGBB */
void n64_text_draw(const struct n64_font *font, int x, int y, unsigned long color, const char *text);
/* text squeezed across (x_scale, the anamorphic picture's 3/4), filtered */
void n64_text_draw_scaled(const struct n64_font *font, float x, int y, unsigned long color, const char *text,
	float x_scale);

/* ---------- the menu (n64_menu.c): split screen multiplayer's setup */

enum n64_vehicle_selection
{
	N64_VEHICLES_MAP_DEFAULT = 0,
	N64_VEHICLES_ROCKET_HOGS = 1,
	N64_VEHICLES_ALL_MAP = 2,
	N64_VEHICLES_ALL_ROCKET = 3,
	N64_VEHICLES_COUNT
};

struct n64_menu_choice
{
	int game_type; /* n64_menu_game_variant */
	int map; /* n64_menu_map_folder */
	int score; /* to win: kills, flags, minutes or laps */
	/* the players: each one's controller port, team (0 red, 1 blue) and
	Halo colour index */
	int player_count;
	int ports[4], teams[4], colors[4];
	/* Firefight (port/n64/game/n64_firefight.c) instead of multiplayer:
	the map, the difficulty (0 easy to 3 legendary) */
	int firefight;
	int difficulty;
	/* multiplayer's bots (port/n64/game/n64_bots.c): how many, their
	skill (0 easy to 3 legendary) */
	int bots;
	int bot_skill;
	/* the next menu opens on its title (the pause menu's MAIN MENU), not
	the game's setup; cleared there */
	int to_title;
	/* a campaign level (the campaign cartridge, HALO_N64_CAMPAIGN): the
	map, the difficulty */
	int campaign;
	/* Session-only expansion choices; EEPROM v1 remains unchanged. */
	int weapon_set;
	int vehicle_set; /* enum n64_vehicle_selection; session only */
	int enhanced_fx;
	int big_head;
	int paintball;
};
void n64_menu_memory_begin(void);
void n64_map_boot_complete(void);
void n64_render_prepare_map_image(void);
void n64_render_finish_map_image(void);
void n64_menu_memory_end(void);
void *n64_menu_alloc(unsigned long size);
void n64_menu_free(void *p);
void *n64_menu_sprite_load(const char *path);
void n64_menu_sprite_free(void *p);

/* runs the menu until a game is chosen (after a game, its results first:
lines of text); the choice keeps what was chosen last time */
int n64_menu_run(struct n64_menu_choice *choice, const char *const *results, int result_count);
/* One stable automatic bot roster is shared by lobby previews and spawn. */
void n64_bots_prepare_lobby(void);
const char *n64_bot_lobby_name(int index, int bot_count);
int n64_bot_lobby_team(int index, const int *human_teams, int human_count);
const char *n64_bots_display_name(long player_index);
/* World-pose and movement cadence; non-bot units keep native single ticks. */
int n64_bot_pose_interval(long unit_index);
int n64_bot_move_interval(long unit_index);
const char *n64_menu_map_folder(int map);
int n64_menu_map_count(void);
const char *n64_menu_map_name(int map);
int n64_menu_map_from_folder(const char *folder);
const char *n64_menu_game_variant(int game_type);
int n64_menu_game_teams(int game_type);
int n64_menu_game_bots(int game_type);

/* ---------- the HUD (n64_hud.c): drawn by the game's own HUD code
(port/n64/game/n64_hud_game.c passes its quads, text and blips on).
Positions are pixels of the 320x240 frame, texture coordinates texels of
the half-size HUD sprite. */

struct n64_hud_quad
{
	short sprite;
	short pad;
	float x[4], y[4];
	float s[4], t[4];
	/* ARGB; the sprites are stored straight (not premultiplied): an opaque
	colour blends over, a transparent one adds (Halo's HUD blends
	premultiplied, colour + framebuffer * (1 - alpha)) */
	unsigned long color;
	/* a meter (Halo's meter shader, rasterizer_xbox_dynavobgeom.c): the
	sprite's intensity is each texel's fill order f, its alpha the shape.
	Texels with f under the flash colour's alpha are full, coloured from
	minimum_color to maximum_color by f (plus the flash colour where f is at
	least the minimum colour's alpha), the rest background_color; all scaled
	by the tint's intensity, over the framebuffer times the tint's alpha
	(full) or the background's (empty) */
	int meter;
	unsigned long minimum_color, maximum_color, flash_color, background_color, tint_color;
};

/* a new map: its sprites are rom:/hud/<map>/<n>.sprite */
void n64_hud_set_map(const char *map_name);
/* a view's HUD: 1 when the game should draw it now (it is recorded and
shown until the view's next one), 0 when the last one is shown again (the
game's HUD code runs for each view every other frame) */
int n64_hud_begin(int view, int x0, int y0, int x1, int y1);
void n64_hud_end(void);
/* the views this frame (more views: each one's HUD code runs less often) */
void n64_hud_set_view_count(int count);
/* the views' HUD recordings dropped (the display changed size) */
void n64_hud_forget_recordings(void);
/* room on the heap for count blocks of size bytes at once, models dropped
from the cache for it; 0 if it can't be made (n64_render.c; not while
recording a block) */
int n64_render_make_room(unsigned long size, int count);
/* TERRAIN DETAIL (SETTINGS): 0 full, 1 no overlays, 2 nearest mipmaps, 3
none (n64_render.c) */
void n64_render_set_terrain_detail(int mode);
/* the HUD's inset from each view's edges in pixels, for a television's
overscan (Settings: HUD INSET; port.json flag "debug_hud_inset=<pixels>") */
void n64_hud_set_inset(int pixels);
int n64_hud_inset(void);
void n64_hud_quad(const struct n64_hud_quad *quad);
void n64_hud_text(int x, int y, unsigned long color, const char *text);
float n64_hud_text_width(const char *text);
int n64_hud_text_height(void);
int n64_hud_ensure_sprite(int number);
/* a motion sensor blip: centre in pixels, position and size in pixels */
void n64_hud_blip(float x, float y, float width, float height, unsigned long color);

/* the pause menu (START in a game, port/n64/game/n64_game.c): the
scoreboard over the frame's views, every player and bot a row, sorted --
and its one button, MAIN MENU */
struct n64_score_row
{
	char name[16];
	char score[12];
	short kills, deaths;
	signed char team; /* 0 red, 1 blue; -1 without teams */
	unsigned char human;
};

#define N64_SCORE_ROWS 16

struct n64_pause_screen
{
	const char *title;
	const char *note; /* under the title: the teams' scores, Firefight's wave */
	const struct n64_score_row *rows;
	int row_count;
	int selected; /* 0 Controls, 1 Main Menu */
};

void n64_hud_pause(const struct n64_pause_screen *screen);
void n64_hud_pause_controls(const int *ports, int count, const int *selected_rows);

#ifdef __cplusplus
}
#endif

#endif /* __HALO_N64_H */
