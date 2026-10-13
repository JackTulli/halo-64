/*
OVERHEAD_MAP.C

symbols in this file:
000CFB20 0010:
	_overhead_map_initialize (0000)
000CFB30 0010:
	_overhead_map_initialize_for_new_map (0000)
000CFB40 0010:
	_overhead_map_dispose_from_old_map (0000)
000CFB50 00e0:
	_overhead_map_post_rasterize (0000)
00270BD0 000c:
	_rdata_00270bd0 (0000)
00453CD0 0030:
	_bss_00453cd0 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bitmaps/bitmap_group.h"
#include "game/game.h"
#include "math/integer_math.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct overhead_map_globals
{
	boolean enabled;
	byte alignment_pad;
	rectangle2d bitmap_bounds;
	boolean throttle_updates;
	boolean render_target_dirty;
	rectangle2d render_bounds;
	real_point2d viewport_size;
	volatile point2d bitmap_origin;
	long last_render_bounds_update_time;
	long reserved24;
	struct bitmap_data *bitmap;
	long reserved2c;
};

typedef char verify_overhead_map_globals_size[sizeof(struct overhead_map_globals) == 0x30 ? 1 : -1];
typedef char verify_overhead_map_enabled_offset[offsetof(struct overhead_map_globals, enabled) == 0x00 ? 1 : -1];
typedef char verify_overhead_map_bitmap_bounds_offset[offsetof(struct overhead_map_globals, bitmap_bounds) == 0x02 ? 1 : -1];
typedef char verify_overhead_map_throttle_updates_offset[offsetof(struct overhead_map_globals, throttle_updates) == 0x0A ? 1 : -1];
typedef char verify_overhead_map_render_target_dirty_offset[offsetof(struct overhead_map_globals, render_target_dirty) == 0x0B ? 1 : -1];
typedef char verify_overhead_map_render_bounds_offset[offsetof(struct overhead_map_globals, render_bounds) == 0x0C ? 1 : -1];
typedef char verify_overhead_map_viewport_size_offset[offsetof(struct overhead_map_globals, viewport_size) == 0x14 ? 1 : -1];
typedef char verify_overhead_map_bitmap_origin_offset[offsetof(struct overhead_map_globals, bitmap_origin) == 0x1C ? 1 : -1];
typedef char verify_overhead_map_last_update_offset[offsetof(struct overhead_map_globals, last_render_bounds_update_time) == 0x20 ? 1 : -1];
typedef char verify_overhead_map_bitmap_offset[offsetof(struct overhead_map_globals, bitmap) == 0x28 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

real const overhead_map_constants[3] = { 0.5f, 84.0f, 80.0f };

static struct overhead_map_globals bss_00453cd0;
#define overhead_map_globals bss_00453cd0

/* ---------- public code */

void overhead_map_initialize(void)
{
}

void overhead_map_initialize_for_new_map(void)
{
}

void overhead_map_dispose_from_old_map(void)
{
	overhead_map_globals.enabled = FALSE;
}

void overhead_map_post_rasterize(
	void *unused0,
	void *unused1,
	real_point2d const *viewport_size)
{
	if (overhead_map_globals.enabled)
	{
		rectangle2d render_bounds;
		struct bitmap_data *bitmap;

		overhead_map_globals.viewport_size = *viewport_size;
		bitmap = overhead_map_globals.bitmap;
		render_bounds.x0 = ((short)(long)viewport_size->x >> 1) - bitmap->registration_point.x;
		render_bounds.y0 = ((short)(long)viewport_size->y >> 1) - bitmap->registration_point.y;

		render_bounds.x1 = overhead_map_globals.bitmap_bounds.x1 + render_bounds.x0;
		render_bounds.y1 = overhead_map_globals.bitmap_bounds.y1 + render_bounds.y0;
		overhead_map_globals.bitmap_origin.y = render_bounds.y0;
		render_bounds.y0 += overhead_map_globals.bitmap_bounds.y0;
		overhead_map_globals.bitmap_origin.x = render_bounds.x0;
		render_bounds.x0 += overhead_map_globals.bitmap_bounds.x0;

		if (!overhead_map_globals.throttle_updates ||
			local_time_get() - overhead_map_globals.last_render_bounds_update_time > 7)
		{
			overhead_map_globals.render_bounds = render_bounds;
			overhead_map_globals.last_render_bounds_update_time = local_time_get();
			overhead_map_globals.render_target_dirty = FALSE;
		}
	}
}

/* ---------- private code */
