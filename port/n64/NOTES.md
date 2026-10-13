# N64 port: working notes

Findings that steer the port (from reading the game's code, October 2026).
`port/n64/README.md` will describe the finished design; this file is the
running notebook. Paths are relative to `source/`.

## Starting a split-screen game without the menus

- Xbox split screen is a real network game (server + client over loopback).
  The N64 does not need that: `_game_connection_local` supports up to 4
  local players, and the game engine runs whenever
  `game_variant_global.game_engine_index != 0`.
- Before `main_setup_connection` (main.c `main_loop` prologue, after
  `game_initialize`/`console_startup`): `game_set_game_variant_from_name`
  ("slayer", "team_slayer", "ctf", ...; game.c:1087), `player_spawn_count = N`,
  `main_set_map_name("levels\\test\\wizard\\wizard")` (clears
  want_to_be_at_main_menu), then `main_disallow_persistent_storage()`
  (main_set_map_name turns it on). `main_menu_scenario_loaded` must be FALSE.
- `create_local_players` (main.c:752): local player index = controller
  index, players in ascending controller order; names empty (NULL network
  player). Team games: round robin; FFA under HALO_LINUX: team = datum.
- Engine table: 1 ctf, 2 slayer, 3 oddball, 4 king, 5 race.
- Local time cap: dt capped at 1/15 s (main.c:2195), at most 7 ticks; the
  game slows down below 15 fps.
- Postgame on a local connection jumps to state 3; the loop must notice
  `game_engine_force_single_screen()` and `main_reset_map()`.
- TRAP: `scenario_switch_structure_bsp` (every map load) does
  `main_stop_time`/`main_start_time`; the end of `main_loop`'s frame resets
  `halt_time_scale`. Keep the game's own `main_loop` (main.c), hook it with
  `#ifdef HALO_N64`.
- Nothing draws before game time 3 when an engine runs (main.c:3297).

## What the game needs from the (excluded) renderer

- No simulation reads render results. Required anyway:
  - `render.frame_index` incremented per frame; particles die when
    `frame_index - last_rendered_frame_index > 15` (the particle renderer
    must refresh it).
  - `render.camera`/`render.frustum` per window (`render_camera_build_frustum`,
    keep render/render_cameras.c: pure maths), `render.local_player_index`.
  - `first_person_weapon_render_update()` per window (builds the FP node
    matrices effects attach to).
  - `interface_draw_screen()` per window: motion sensor, HUD sounds,
    pickup prompts, message timers, target-name hold all advance inside it.
  - `rasterizer_globals`: screen_bounds 0,0,640,480 / frame_bounds
    48,36,592,444 (compute_window_bounds builds the views from them),
    near/far clip 0.0625/1024, the vblank callback
    (`rasterizer_set_vblank_callback`) incrementing
    `frame_and_vertical_blank_index` (main_rasterizer_throttle spins on it).
  - `rasterizer_debug_options` defaults: debug_model_lod (0x08) = NONE,
    draw_first_person_weapon_first (0x0E) = TRUE, dynamic_screen_geometry
    (0x22) = TRUE, zoffset (0x58) = 1/256.
  - `_texture_cache_bitmap_get_hardware_format` returning NULL skips a HUD
    element.
- Keep: render/render_cameras.c, rasterizer/rasterizer_geometry.c (lightmap
  sampling for unit illumination), rasterizer/rasterizer_swizzle.c,
  shaders/*.c, text/draw_string.c.
- HUD primitive: `rasterizer_psuedo_dynamic_screen_quad_draw(params, v[4])`;
  text: `draw_string`/`draw_unicode_string` with a glyph callback.
- Node matrices: computed on the tick (objects.c `object_compute_node_matrices`),
  read with `object_get_node_matrices`. Skinning:
  `node_matrices[i] * node->runtime_default_inverse_matrix` (models.c:818).
  LOD in `render_model` (models.c:838): detail_cutoff_pixels.

## Map data (tools/n64)

- Xbox MP map tag data ~7 MB; 2.9 MB of it is Xbox model geometry.
  Converted Chill Out: 4.13 MB (antr 1.74 MB, effe 0.57 MB, sbsp 0.34 MB,
  coll 0.21 MB). All 345 animations uncompressed (layout per node:
  rotation 4 x int16, translation 3 floats, scale float; animated ones per
  frame, the rest in default data).
- Bitmap pixels and sound samples are outside the tag data (in the file).
- Layouts from Invader's definitions (~/n64/invader, GPL-3, read at run
  time). The sbsp tag instance must have no address and the BSP header
  signature 'sbsp' (cache_files.c asserts).

## The HUD (October 2026)

- The HUD's bitmaps are premultiplied: Halo blends them as colour +
  framebuffer * (1 - alpha) (`_shader_framebuffer_blend_function_alpha_multiply_add`).
  The RDP's additive blending wraps around (libdragon says so; it drew dark
  boxes over bright walls), so the sprites are stored straight and a mostly
  transparent quad colour (a glow) blends with its brightest channel as
  alpha.
- Meter bitmaps hold the fill order in their colour and the shape in alpha.
  Halo's meter shader (decoded from the register combiner words in
  `rasterizer_xbox_dynavobgeom.c`): texels with order < flash colour alpha
  are full, coloured min to max colour by order (+ flash colour where order
  >= min colour alpha); the rest take the background; alpha kill outside the
  shape; the framebuffer keeps tint alpha (full) or background alpha
  (empty).
- Damage indicators are rotated 128x128 bitmaps: more than TMEM, drawn in
  strips.
- Slayer looks up "ui\\multiplayer_game_text" by name every frame:
  `tag_loaded` remembers recent answers on the N64.
- The stack: Halo needs ~97 KB of stack; libdragon keeps 64 KB above the
  heap, so the heap's top was being overwritten (seen as z-buffer values in
  pointers once the HUD's allocations reached that far). The game now runs
  on its own 256 KB stack.

## Simplified models (October 2026)

- Each part of a model (one per shader: glove, hand, gun body...) is
  simplified on its own, so a vertex on an open edge must not move: the
  first versions let part edges and the seams between rigidly skinned nodes
  drift, which opened holes in the first-person hands and weapons (the
  plasma pistol went from 20 open edges to 167). Locked now, and a collapse
  is also refused when it turns a triangle more than 60 degrees (a fold
  faces away and is culled) or breaks the link condition. Checked with a
  script that counts open edges and same-direction edges per model.
- Model shaders can be two-sided (shader_model flags bit 1; transparent
  shaders bit 2, meters bit 1): those batches draw without back-face
  culling. In Wizard only the needler's core is.

## First-person textures (October 2026)

- A weapon's first-person texture is one 512x512 atlas; squeezed into one
  64x64 CI4 texture it turned to coloured noise. First-person parts are now
  cut into windows of the atlas at 256x256 (models.py texture_windows),
  each its own 64x64-or-smaller CI4 texture with its own palette.
- The simplifier must not move a vertex on a texture seam either: a
  triangle could take its corner's texture coordinate from another island
  of the atlas.
- Halo doubles a model's colour against half-intensity lighting; the baked
  sky light here is whole, so model textures are no longer doubled (they
  saturated to white).
- Colour change (shader_model "change color source", the multipurpose map's
  blue on the Xbox): the texture's 16 colours are split, 0-7 outside the
  mask, 8-15 inside (level.py split_palette; mksprite keeps a paletted PNG's
  palette), and the renderer uploads a palette with 8-15 times the player's
  colour. No second RDP cycle for players any more.
- Transparent model shaders: meters (the plasma rifle's heat gauge, which
  drew as white noise) and numeric displays are left out; the rest are
  cut-out RGBA16 glows, unlit.
- Models and their textures are read from the cartridge when first drawn
  (the models file keeps per-model data after its tables) and dropped
  least recently drawn first past 256 KB / 192 KB: the heap went from
  145 KB free to ~770 KB.
- Debugging: port.json flag "debug_weapon=<weapon tag>" makes it the
  spawn weapon (n64_files.c n64_debug_text reads name=value settings).

## Level lighting from the lightmap (October 2026)

- The level's vertex colours were the materials' lighting summaries (ambient
  + two distant lights, clamped), which blew the centre out to white. They
  are now Halo's lightmap: each lightmap vertex (8 bytes after the 32-byte
  rendered vertices: a compressed normal, then u and v over 32767) samples
  its page of the BSP's lightmap bitmap (r5g6b5), averaging the texels
  within a texel and a half and skipping the fill (pure yellow and pure
  red) that pads the pages. The level textures stay doubled (Halo's
  lightmaps are half intensity).
- Triangles are split where the lightmap changes across them (an edge over
  2 texels whose middle differs from its ends by more than 0.06, up to 3
  times; decisions shared per edge so no crack opens): Wizard goes from
  2,972 to 5,090 triangles. N64_LIGHTMAP_SPLIT_DEPTH=0 turns it off.
- The teleporters' portals are a scenery model (teleporter_shield) with a
  transparent shader whose first map is a white energy mask: a glow whose
  map is grey takes the shader's emitted light colour, solid (green).
- Debugging: "debug_spin=x y z [yaw]" stands the first view there, turning
  or facing yaw degrees. Without objects Wizard's centre draws at ~30 fps:
  the objects (first-person models included) are the larger cost.

## Sound (October 2026)

- Xbox ADPCM is IMA ADPCM in 36-byte blocks a channel: a 4-byte header
  (predictor, step index) that is not a sample, then 64 samples. ffmpeg's
  IMA WAV decoder (0x69) outputs the header as a 65th sample: decode it
  ourselves (tools/n64/sounds.py, table-driven).
- audioconv64's VADPCM is Huffman-packed by default; libdragon undoes that
  on the CPU and reads such files synchronously. With a few voices it was
  a quarter of the CPU in ares: `--wav-compress vadpcm,huffman=false`
  (12% bigger) streams plain frames by DMA for the RSP to decode.
- DragonFS keeps a lookup table of every file in RAM (16 bytes each): a
  thousand sound files would cost 16 KB, so they are one bank file read at
  offsets (libdragon's `wav64_loadfd` takes an open file at its position;
  declared in wav64_internal.h only).
- A stereo wav64 takes two mixer channels (the music: 0 and 1).
- `mixer_ch_alloc` plans a voice: a free one, else the lowest priority,
  quietest, oldest; `mixer_ch_play` resets the priority, so set it after.
- Halo's code that reads sound definitions directly: the announcer's queue
  and unit speech (longest permutation length: patched to ask
  n64_sound_definition_length), predicted resources (off on the N64).
- The Covenant's dialogue tags are 4 KB each (209 vocalization
  references); their animation graphs reference dialogue too (sleeping,
  retreating), which merge.py follows.

## Performance (October 2026, the deep round)

- Measure first: `devtools/bench-set.sh <label> [seconds] [scenes]`;
  `profile` (+`profile_zone` with `profile_tick`/`profile_sound`, or the
  HUD alone) and `tools/n64_profile.py`; `profile_stack` adds rough call
  chains (MIPS stack scanning misses many frames); `debug_stage_type=<n>`
  splits an object type's update by stage and part; `debug_ray_callers`
  counts collision_test_vector by caller; `debug_gpu_time` waits for the
  RSP/RDP after each phase.
- rspq_next_buffer near the top of a profile means the CPU is waiting for
  the RSP: the low-priority queue's buffers were 512 words; at 2048 the CPU
  queues a whole frame and goes on to the ticks.
- `n64_debug_flag` must be cheap: it is called per object, per ray, per
  sound. Its cache (64 names, a pointer slot in front) once held 16 and
  every other flag opened a file each call.
- The decompiled code's square_root/sine/cosine were double precision
  (cvt + sqrt.d), its fmod(x, 1) a double library call; on the N64 they are
  float (no lockstep netplay to keep bit-identical).
- Halo updates and poses every object every tick: parked vehicles, plants
  and far AI bipeds now skip what cannot have changed or is not seen.
- Nothing draws particles, contrails or widgets on the N64: their
  creators return NONE (Halo's callers already handle a full pool).
- What is left at four players is the game itself: bipeds (~3 ms each a
  tick, a third of it biped_update_physics), projectiles (~0.4 ms each:
  a ray and a map reconnect a tick), Halo's HUD code (~5 ms a recording).
