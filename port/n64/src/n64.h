#ifdef HALO_N64_PC_EXPANSION
#include "n64_pc_content.h"
#endif
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
/* the bytes the uncached blocks' allocator asks memalign for (n64_main.c) */
unsigned long n64_heap_uncached_asked(int align, unsigned long size);
/* x / 255 for x up to 255 x 255, exactly, in four simple operations (GCC
makes a division by 255 a 64-bit multiply and shifts: the multiply and its
result's wait are most of a dozen cycles) */
static inline unsigned n64_div255(unsigned x)
{
	return (x + 1 + (x >> 8)) >> 8;
}
unsigned long n64_heap_describe(char *text, unsigned long size);
int n64_heap_pieces(unsigned long *sizes, int max, unsigned long least);
unsigned long n64_heap_top(char **start);
void *n64_uncached_block(int align, unsigned long size, int reserve);
int n64_heap_has(unsigned long size);
/* n64_heap_has and n64_heap_free without the bytes the map gave the heap
(n64_map_given) */
int n64_heap_has_held(unsigned long size);
unsigned long n64_heap_free_held(void);
/* whether realloc(block, size) grows block where it is */
int n64_heap_grows_in_place(void *block, unsigned long size);
int n64_heap_has_pieces(unsigned long first, int count, unsigned long each);
int n64_heap_window(void *const *blocks, int count, unsigned long size, unsigned char *chosen);
void n64_heap_dump(const char *tag, const void *start);
/* the maps' home (n64_game.c) and the display buffers lent from it
(n64_main.c, n64_render.c) */
void n64_map_home_reserve(void);
void *n64_map_home_free_part(unsigned long *size);
void n64_map_give_back(void);
void n64_map_take_back(int last);
void *n64_map_given(unsigned long *size);
void n64_display_set_home(void *start, unsigned long size);
void n64_display_lend(void *start, unsigned long size);
void n64_display_set_room(void **starts, const unsigned long *sizes, const char *whole, int count);
void n64_display_lend_room(int on);
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
	N64_SCHEME_DPAD,          /* the stick looks, the D-pad moves, the C buttons do the D-pad's */
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
/* Settings: DECALS -- the game's own (bullet holes, blood, scorch marks) on
or off; paintball's paint is the game setup's */
void n64_paint_set_decals(int on);
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
	/* an obstacle -- scenery, a machine, a control or a light fixture with
	a collision model: its definition's absolute tag index + 1, its stand-in
	drawn where its model is not (levels/<map>.sti); 0: none */
	unsigned short obstacle_definition;
	/* its model's tier for the model cache (n64_render.c load_model), most
	wanted first: N64_TIER_FIRST_PERSON the local player's own view models,
	N64_TIER_ACTOR bipeds, vehicles, projectiles and what a unit carries,
	N64_TIER_ITEM weapons, equipment and garbage on the ground (nothing
	shows them where their model has no room), N64_TIER_OBSTACLE scenery and
	devices that collide (their stand-ins show them where their model has no
	room; one with no stand-in to show is drawn at N64_TIER_ACTOR,
	n64_render.c), N64_TIER_DECORATION scenery and devices that do not
	collide, and the rest */
	unsigned char tier;
};
#define N64_TIER_FIRST_PERSON 0
#define N64_TIER_ACTOR 1
#define N64_TIER_ITEM 2
#define N64_TIER_OBSTACLE 3
#define N64_TIER_DECORATION 4

void n64_render_initialize(void);
/* loads rom:/levels/<map>.lvl and its textures (with low, the lighter
<map>-low.lvl where there is one); FALSE if there is none */
/* the game's local players (before its level loads: the model cache's tiers) */
void n64_render_set_players(int count);
int n64_render_load_level(const char *map_name, int low);
void n64_render_frame_begin(void);
void n64_render_view(const struct n64_view *view);
/* after n64_render_view: objects in that view */
/* returns whether it is in the view (not culled) */
int n64_render_object(const struct n64_object *object);
/* the model tags of the objects in the world, each once (n64_render_game.c) */
int n64_render_game_world_models(short *tags, int most);
/* n64_render_object for an object whose node matrices are in another order:
node n's matrix is node_matrices[node_remap[n]] (a first-person model's,
which are its animation graph's) */
int n64_render_object_remapped(const struct n64_object *object, const short *node_remap);
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
int n64_game_debug_line_2(char *text, int size);
/* the views this frame: small views draw objects' far versions sooner */
void n64_render_set_view_count(int count);
/* then the first-person weapon: its own near plane, drawn over the world */
void n64_render_first_person_begin(void);
void n64_render_frame_end(void);
/* draws the objects queued so far (n64_render_object queues them, sorted
by texture): before 2D drawn over them */
void n64_render_flush(void);
/* n64_render_flush, then the 2D over the views (the HUD's phase of the counter) */
void n64_render_hud_phase(void);
/* a view's HUD drawn: the next view's 3D may follow (HUD RESOLUTION HIGH
DETAIL) */
void n64_render_hud_end(void);
/* a view's rectangle, laid out in the 3D's pixels, in the pixels its HUD
is drawn in: the same, but twice as wide where the HUD is at 640x240 over
the 3D's 320x240 (n64_render_set_hud_detail) */
void n64_render_hud_rect(int *x0, int *y0, int *x1, int *y1);
/* chunky mode (the menu's settings): from the next frame on, the views'
3D is drawn half as wide and its pixels doubled across (the HUD and text
stay at full width) */
void n64_render_set_chunky(int level); /* 0 off, 1 medium (4/5 width), 2 chunky (half) */
/* the view models' quality (the menu's settings), for the next level
loaded: 0 low, 1 medium, 2 high -- the first-person weapons and arms at
1.25 or 2 times the triangles (levels/<map>-fp1.mdl, -fp2) */
void n64_render_set_view_model_quality(int quality);
/* (the custom cartridge) the first-person model handed to the renderer
next is the arms, on: they keep the model cache's whole margin, the weapon
a smaller one (n64_render.c load_model) */
void n64_render_view_model_arms(int on);
/* the FPS counter in the corner (off in the menu) */
void n64_render_show_counter(int on);
/* the menu's display, 640x480 interlaced (two buffers and a z-buffer of
that size), or back to the game's 320x240: the old display's memory freed
first, the game's buffers where they were before the menu */
void n64_render_set_menu_display(int on);
int n64_render_screen_width(void);
/* Settings: the game in more detail, tried once the map is in -- level 1,
HIGH DETAIL: 640x240 at 240p (Perfect Dark's high resolution), else 520x240
or 480x240 where the map leaves less room; level 2, ULTRA DETAIL: 480i
(640x480, else 512x480), else as high detail; else 320x240 and a word on
the screen. 0: off. The anamorphic picture (16:9 squeezed into 4:3: a wider
view across, the HUD squeezed about each view's middle) */
#define N64_DETAIL_HIGH 1
#define N64_DETAIL_ULTRA 2
void n64_render_set_high_detail(int level);
/* Settings: HUD RESOLUTION HIGH DETAIL -- the HUD and text at 640x240 at
240p, as high detail draws them, over the 3D's 320x240 picture (medium and
chunky widen theirs to it first; each of its pixels shown twice across),
tried once the map is in; else as without it (no word on the screen, only a
line in the debug log). Nothing with high or ultra detail: their HUD is at
their own resolution already. 0 (STANDARD): the HUD 320 across, as before */
void n64_render_set_hud_detail(int on);
void n64_render_set_anamorphic(int on);
/* Settings: ANTI-ALIASING (the RDP's coverage on opaque surfaces, the VI's
filter smoothing their edges) and DEDITHER (the VI rebuilding colour from
the dither: from the next display made) */
void n64_render_set_antialias(int on);
void n64_render_set_dedither(int on);
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

/* the CONTENT page's IMPACT FX (n64_menu_choice.enhanced_fx): the weapons',
grenades' and impacts' particles enhanced, as they were, or none at all */
enum n64_impact_fx
{
	N64_IMPACT_FX_ORIGINAL = 0,
	N64_IMPACT_FX_ENHANCED = 1,
	N64_IMPACT_FX_OFF = 2
};

enum n64_vehicle_selection
{
	N64_VEHICLES_MAP_DEFAULT = 0,
	N64_VEHICLES_ROCKET_HOGS = 1,
	N64_VEHICLES_ALL_MAP = 2,
	N64_VEHICLES_ALL_ROCKET = 3,
	/* none at all (every cartridge: Halo's own game variant vehicle set 1) */
	N64_VEHICLES_NONE = 4,
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
	int enhanced_fx; /* enum n64_impact_fx */
	int big_head;
	int paintball;
	/* an online game (the online cartridge, HALO_N64_ONLINE): the Link's
	game played, the map its; whether it has teams and the player's team.
	to_online: the next menu opens on ONLINE */
	int online;
	int online_teams, online_team;
	int to_online;
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
/* the HUD's sprites for a display 640 across (on): the full-width set
(tools/n64/hud.py) where the map has one and room is not 0 (the renderer's
word that the models keep their room: n64_render.c's hud_sprite_room), else
the half-size set */
void n64_hud_set_wide(int on, unsigned long room);
/* the views' HUD recordings dropped (the display changed size) */
void n64_hud_forget_recordings(void);
/* room on the heap for count blocks of size bytes at once, models dropped
from the cache for it; 0 if it can't be made (n64_render.c; not while
recording a block) */
int n64_render_make_room(unsigned long size, int count);
/* room on the heap for need bytes in one piece, for a HUD image: on the PC
Expansion cartridge with one or two players, item, obstacle and decoration
models pushed out for it (n64_render.c; not while recording a block). 1:
room; 0: none, nothing pushed out; -1: none, though models were pushed out */
int n64_render_push_out_for_hud(unsigned long image, int pieces, unsigned long piece);
/* room for size bytes in one block that still leaves margin bytes of the
heap free, models dropped from the cache for it if short; 1 if there is
(n64_render.c; HUD STYLE 64's module and textures) */
int n64_render_room_leaving(unsigned long size, unsigned long margin);
/* the RSP's RDP output moved off every block's buffer, before a block is
freed with rdpq_call_deferred (n64_render.c; not while recording a block) */
void n64_render_park_rdp_output(void);
/* the RSP held after a SYNC_FULL until the CPU has taken its interrupt, so
that no buffer switch restarts the RDP too soon (n64_render.c; after
rdpq_detach, rdpq_fence and every rspq_wait: n64_main.c wraps it) */
void n64_render_wait_sync_full(void);
/* TERRAIN DETAIL (SETTINGS): 0 full, 1 no overlays, 2 nearest mipmaps, 3
none (n64_render.c) */
void n64_render_set_terrain_detail(int mode);
/* OBSTACLE STAND-INS (SETTINGS): off, no map's stand-ins are read; from the
next map's load (n64_render.c) */
void n64_render_set_standins(int on);
/* the HUD's inset from each view's edges in pixels, for a television's
overscan (Settings: HUD INSET; port.json flag "debug_hud_inset=<pixels>") */
void n64_hud_set_inset(int pixels);
int n64_hud_inset(void);
void n64_hud_quad(const struct n64_hud_quad *quad);
void n64_hud_text(int x, int y, unsigned long color, const char *text);
float n64_hud_text_width(const char *text);
int n64_hud_text_height(void);
int n64_hud_ensure_sprite(int number);
int n64_hud_ensure_sprite_at(int number, float s, float t);
void n64_hud_mark_decoration_at(int number, float s, float t);
void n64_hud_mark_carried_at(int number, float s, float t);
void n64_hud_critical_pass(int on);
/* whether this map's HUD is packed (rom:/hud/<map>.subs): its HUD runs the critical pass */
int n64_hud_packed(void);
/* (the custom cartridge) the free bytes the critical HUD sprites wanted and
missing need, of those that need at most limit: the room the local
player's view models leave them; and a view model gave way for them */
unsigned long n64_hud_view_model_room(unsigned long limit);
int n64_hud_view_models_wait(void);
void n64_hud_view_models_gave_way(void);
/* a motion sensor blip: centre in pixels, position and size in pixels */
void n64_hud_blip(float x, float y, float width, float height, unsigned long color);
/* the view's rectangle (n64_hud_begin's), x0 y0 x1 y1 (HUD STYLE 64's module) */
void n64_hud_view_rect(int rect[4]);

/* ---------- HUD STYLE 64 (Settings; n64_hud64.c): the HUD redrawn as N64
pixel art from tables, the game's HUD code handing over its values
(port/n64/game/n64_hud64_game.c). 0 ORIGINAL (the game's own HUD), 1 64 */
void n64_hud_set_style(int style);
int n64_hud_style(void);
#define N64_HUD64_MESSAGES 4
enum { N64_HUD64_ROUNDS, N64_HUD64_BATTERY, N64_HUD64_NO_AMMO };
/* (the reticle and the motion tracker are Halo's own with either style:
the player keeps the aim and the tracker they know) */
struct n64_hud64_state
{
	unsigned char show_ammo, show_grenades, show_shield, show_sensor, show_score;
	unsigned char ammo_kind; /* N64_HUD64_* */
	/* unlimited: a weapon no player holds (a vehicle's), which Halo spends
	no rounds or battery of -- a lemniscate in the count's place, no round
	grid */
	unsigned char ammo_infinite;
	short total;             /* the number: rounds in reserve, or the battery's percent */
	short loaded, loaded_max; /* the magazine's rounds, of 60 (the grid's ticks) */
	/* a battery weapon's heat (0-1) and whether it shows one, red from the
	weapon HUD's flash cutoff (or overheated) */
	unsigned char show_heat, heat_hot;
	float heat;
	short frags, plasmas;
	signed char grenade_type; /* the selected type: 0 frag, 1 plasma (-1 none) */
	/* the grenade panel's fill and rim as Halo colours its own for the
	selected type's state: blue and rim, flashing at its flash cutoff,
	Halo's warning colour with none left (0: the design's) */
	unsigned long grenade_fill, grenade_edge;
	/* the slots lit white now in a double flash: 1 frag, 2 plasma (a type
	ran out, a switch to it or a throw with none: n64_hud64_game.c); 4 the
	whole panel too (a switch or a throw with both types out) */
	unsigned char grenade_flash;
	/* the seat's secondary function in the grenade panel's place: a seat
	whose weapon has "secondary trigger overrides grenades" (the grenade
	button fires its second trigger -- the Banshee's
	fuel rod, the Scorpion's machine gun); its icon (0 a charge: the trigger
	draws from a magazine; 1 a round: it does not), the part of its recharge
	gone (0-255, 255 ready) and the grenade button's letter in the player's
	controls */
	unsigned char show_secondary, secondary_icon, secondary_fill;
	char secondary_button;
	/* an unlimited weapon whose main trigger draws from a magazine (the
	Scorpion's cannon, the rocket Warthog's gun, the Multiplayer Ghost's)
	shows its recharge under its card, in Halo's blue, filling to ready: the
	part gone, 0-255, 255 when it can fire */
	unsigned char show_cooldown, cooldown_fill;
	/* shield 0-1, above 1 the overshield (Halo's overcharge levels, to 5);
	health 0-1 */
	float shield, health;
	/* the health's colour as Halo picks it (its unit HUD's cutoffs: the
	middle colour, the low one), 0xRRGGBB with the top byte set; 0: the
	design's */
	unsigned long health_colour;
	/* (Halo's own flashes, the factor its get_flash_color blends by,
	0-255, and the tag's flashing colour: HUD 64 blends its own colours
	toward that colour by the factor) the health panel's (1 player's tag
	only: the unit's shield-depleted flag) */
	unsigned char health_flash;
	unsigned long health_flash_colour;
	/* the shield panel's low-shield blink (under 25%: its background, the
	frame's outline and tray here) */
	unsigned char shield_flash;
	unsigned long shield_flash_colour;
	/* the damage ghost: the shield value Halo holds after a drop
	(last_shield_vitality), the factor its flash colour adds by (0: none),
	and that colour (the meter's) */
	float shield_held;
	unsigned char shield_ghost;
	unsigned long shield_ghost_colour;
	short score, other, limit; /* yours, the best other's (team games: the teams'), the game's */
	unsigned char other_leads;
	/* the pickup prompt: the weapon's name in capitals ("" none), SWAP FOR */
	char prompt[28];
	unsigned char prompt_swap;
	/* another prompt the game shows (to enter a vehicle...): as the
	messages' text, '\2' the action button */
	char state_text[72];
	/* the game's messages, oldest first: capitals, '\1' before and after a
	name (drawn in the numbers' colour), their fade (0-255) */
	short message_count;
	struct { char text[72]; unsigned char alpha; } messages[N64_HUD64_MESSAGES];
};
/* the display the HUD is drawn in (n64_render.c): with HUD RESOLUTION
HIGH DETAIL 640x240, not the 3D's, which the screen is until the HUD's phase */
void n64_render_hud_size(int *width, int *height);
/* before a view's HUD, not while it is recorded: 1 if the style is 64 and
the textures it draws with this many views are in memory (else the game
draws its own HUD) */
/* (n64_hud64_stub.c: the module, rom:/hud64.dso, loaded by the first of
these in a map with the style at 64; given back with the map) */
void n64_hud64_module_end(void);
int n64_hud64_prepare(int views);
/* the next draws kept in a rectangle (the module's own scissor) */
void n64_hud64_scissor(int x0, int y0, int x1, int y1);
/* a view's HUD, its rectangle in the HUD's pixels (after n64_hud_begin) */
void n64_hud64_draw(const struct n64_hud64_state *state, int x0, int y0, int x1, int y1, int views);
/* (HUD STYLE 64's module) the start of Halo's struct unit_hud_state
(hud_unit.c, which checks that the two match): the view's unit HUD state
the style reads and sets as Halo's own drawing would -- the shield's value
held after a drop and its fade, the panels' flash start ticks */
struct n64_hud64_unit_state
{
	float last_shield_vitality, last_body_vitality, fade_time;
	long last_shield_hit_time, last_shield_flash_time, last_health_flash_time;
};
/* (n64_hud.c: its kept render state dropped, after HUD STYLE 64 drew) */
void n64_hud_state_lost(void);
/* the textures given back (n64_hud_release); their bytes in memory */
void n64_hud64_release(void);
unsigned long n64_hud64_resident_bytes(void);
/* the heap HUD STYLE 64 leaves free when it loads its module and its
textures; without that room, the game's own HUD on that map
(n64_hud64_stub.c) */
unsigned long n64_hud64_heap_margin(void);

/* a module of code and data in the ROM (n64_module.c; libdragon's DSO
format, n64dso -c 0) loaded and linked: its memory, given back with
n64_module_free (libc's free: in the game's units free is cseries.h's
debug_free); in *address the value of its export name. NULL without the
file, an import the program lacks, or the room for it that leaves margin
bytes of the heap free (n64_render_room_leaving) */
void *n64_module_load(const char *path, const char *name, unsigned long margin, void **address);
void *n64_module_load_into(const char *path, const char *name, unsigned long margin, void **address, void *home,
	unsigned long size, unsigned long *resident);
/* HUD STYLE 64's borrowed meter slots and the memory after its module in them (n64_hud.c, n64_hud64_stub.c) */
void *n64_hud_lend_slots(int keep, unsigned long *bytes);
void n64_hud_return_slots(void);
void *n64_hud64_arena_take(unsigned long size);
void n64_uncached_arena_begin(void);
void n64_uncached_arena_end(void);
int n64_hud64_arena_owns(const void *block);
void n64_hud64_arena_give_back(void *block);
void n64_module_free(void *module);
/* as n64_module_load, but only into room the heap has: nothing is dropped
or given back for it (the projectiles' module, which gives way to all) */
void *n64_module_load_spare(const char *path, const char *name, unsigned long margin, void **address);

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

/* ---------- online games (HALO_N64_ONLINE; port/n64/docs/online): the
cartridge plays its player's own unit in a local game of the Link's map --
moving, aiming, firing, its first-person weapon and HUD at once -- and Halo
64 Link plays the same control in the real game (n64_online_send_control
each tick), whose state of that unit (n64_online_self) corrects the local
one (port/n64/game/n64_online_game.c); everything else in the game is drawn
as the Link sends it (n64_online.c) */

/* the Link's state of a local player's own unit, as of its latest frame */
struct n64_online_weapon
{
	int definition; /* the weapon's tag index, -1: none in the slot */
	int loaded, total;
	float age;
};

struct n64_online_self
{
	unsigned long serial; /* one more each frame from the Link */
	/* the N64 tick whose control the Link last played (a local tick's
	predicted state is compared with this), 0xFFFFFFFF: none yet */
	unsigned long applied_tick;
	int alive, in_vehicle, action_target;
	float position[3], velocity[3];
	float yaw, pitch;
	int respawn_ticks, team;
	float health, shields;
	int frags, plasmas;
	int current_weapon; /* the slot, -1 none */
	struct n64_online_weapon weapons[4];
};

/* a tick's control of a local player's unit, to the Link */
struct n64_online_control
{
	unsigned long tick;
	int local_player;
	unsigned long flags;
	float yaw, pitch, throttle[2], trigger;
	int weapon, grenade, zoom;
};

/* an online game begins (its map loading; the player's controller port),
and ends */
void n64_online_begin(int port);
void n64_online_end(void);
int n64_online_active(void);
/* each frame: the cable read, the Link told a frame was drawn; 0 when the
game is over or was left (Start and Z together) */
int n64_online_update(void);
void n64_online_send_control(const struct n64_online_control *control);
/* the Link's state of a local player's unit (NULL: none yet) */
const struct n64_online_self *n64_online_self(int local_player_index);
/* the Link's camera for a local player (its vehicle's) put in view: 1, else 0 */
int n64_online_view(int local_player_index, struct n64_view *view);
/* the Link's objects, in the current view */
void n64_online_draw_objects(void);
/* a message the Link's game printed on a local player's HUD, the oldest not
yet taken (UTF-16, ended by a 0): 1, else 0 */
int n64_online_take_message(int *local_player_index, unsigned short *text, int size);

/* the game the Link joined: its players, about once a second (its lobby's
before it begins), the best first (n64_link.c, SCORES) */
struct n64_online_scores
{
	int valid, joined, teams, in_game;
	long red, blue;
	char game[24], map[32], game_type[24];
	int count, total; /* the rows kept, the players in all */
	struct n64_score_row rows[N64_SCORE_ROWS];
	unsigned long serial;
};

const struct n64_online_scores *n64_online_scores(void);

/* what happens in the Link's game, made again by the N64's
(n64_online_game.c): an effect begun, a sound (or the announcer's), a hit
the player takes; each taken once, the oldest first */
struct n64_online_effect
{
	int definition; /* the tag's absolute index */
	float point[3], forward[3];
	float scale_a, scale_b;
};

struct n64_online_sound
{
	int definition; /* the tag's absolute index; the announcer's: its sound */
	int announcer;
	float point[3];
	float scale;
};

struct n64_online_damage
{
	int local_player_index;
	int definition; /* the damage effect's tag's absolute index */
	float direction[3];
	float scale, total;
	int has_attacker;
	float attacker[3];
};

int n64_online_take_effect(struct n64_online_effect *effect);
int n64_online_take_sound(struct n64_online_sound *sound);
int n64_online_take_damage(struct n64_online_damage *damage);

/* the HUD's view of the Link's game, a local player's, as of its latest
frame: the autoaim level (the crosshair red at 1), the name the crosshair
shows ("" none) and whether an enemy's, the state message (UTF-16; empty
none), how goals are shown (the variant's goal_radar), the goals the player
sees, the motion sensor's blips, and the aim's magnetism (0-1) with its
target's motion across the view (yaw and pitch, radians a tick; a Link
before v0.6.2 sends none: 0) */
#define N64_ONLINE_GOALS 16
#define N64_ONLINE_BLIPS 16
/* the target's motion on the cable: s16 over +-this many radians a tick
(the Link's link_export.c has the same) */
#define N64_ONLINE_ANGULAR_RANGE 0.5f

struct n64_online_hud
{
	unsigned long serial;
	float autoaim;
	char target[16];
	int target_enemy;
	unsigned short state[64];
	int goal_radar;
	int goal_count;
	struct { float position[3]; short nav_point; } goals[N64_ONLINE_GOALS];
	int blip_count;
	struct { float position[3]; signed char type, size; } blips[N64_ONLINE_BLIPS];
	float magnetism;
	float target_velocity[2];
};

const struct n64_online_hud *n64_online_hud(int local_player_index);

#ifdef __cplusplus
}
#endif

#endif /* __HALO_N64_H */
