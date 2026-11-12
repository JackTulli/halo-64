/*
N64_PROJECTILES_MODULE.H

The projectiles' module (rom:/projectiles.dso: port/n64/src/n64_projectiles.c)
and the program: the one symbol the module exports, n64_projectiles_entries,
its entry points. The program (n64_particles.c) loads the module only while
the setting is LIGHT or FULL and the heap has room for it, and gives it back
first when anything else needs memory; while it is out, each call is
skipped and no tracer or glow is drawn (the dots, the program's, still are).
*/

#ifndef N64_PROJECTILES_MODULE_H
#define N64_PROJECTILES_MODULE_H

#define N64_PROJECTILES_ENTRIES "n64_projectiles_entries"

struct n64_particle_view;

struct n64_projectiles_entries
{
	/* the module's segments and looks new (after its load), the map's mode
	(1 LIGHT, 2 FULL) and the frame time (n64_particles_update's, kept
	across the module's loads) */
	void (*begin)(int map_mode, float frame_time);
	/* a projectile's move of this tick, from from to to (source/items/
	projectiles.c): its definition, its owner (a player, or NONE), whether
	the round is a tracer */
	void (*moved)(long projectile_index, long definition_index, long owner_player_index, int tracer,
		const float *from, const float *to);
	/* a frame of dt seconds: the segments one frame older, the trails' life
	from the frame time */
	void (*update)(float dt, float frame_time);
	/* a view's segments: views the frame's views; *mode the RDP's mode set
	(the program's dots may have set it) */
	void (*draw)(const struct n64_particle_view *view, int views, int *mode);
};

/* the module's entry points while it is loaded, else NULL (n64_particles.c) */
extern const struct n64_projectiles_entries *n64_projectiles_entry;


/* (inside the module: n64_projectiles.c's and n64_projectiles_game.c's; the
map's mode, 1 LIGHT, 2 FULL) */
extern int n64_projectiles_map_mode;
void n64_projectiles_segment_record(const float *from, const float *to, unsigned long color, int radius, int opacity,
	int key);
void n64_projectiles_looks_clear(void);
void n64_projectiles_moved(long projectile_index, long definition_index, long owner_player_index, int tracer,
	const float *from, const float *to);

#endif
