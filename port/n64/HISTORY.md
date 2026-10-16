# Halo 64: development history

The running log of the N64 port's state, newest first, as it was kept in
`README.md` while the port was being written (the README now covers
building and playing). Each entry says what changed and why; the
technical detail (functions, files, measurements) is here.

16 October: the PC expansion merged, its crashes fixed, a VEHICLES
setting, and the campaign level tuned.

- kaboggs' PC expansion (pull request 1): an optional third cartridge with
  the six PC maps, Content options (BIG HEAD, PAINTBALL, IMPACT FX), the
  GOLDEN STYLE controls, Halo's own bot names, shared effect records and
  compressed maps (the multiplayer ROM 63.8 -> 43.4 MB), and safer
  converters. Three crashes followed on the cartridges it was not tested
  on. The multiplayer cartridge could not start a game ("no memory to
  resize map arena: 2664 -> 3072 KB"): the arena was made at least 3 MB at
  load, past the largest image (2.6 MB) reserved at boot -- it now never
  grows past the boot reservation. The campaign stopped at boot in
  effects_initialize: the game state ceiling had come down to the
  multiplayer pools' 0x70000 -- the campaign keeps 1 MiB. And the campaign
  level could not load ("3072 -> 3145 KB"): the arena trimmed to 3 MB after
  boot, the title left a block in the freed tail -- the campaign no longer
  trims.
- VEHICLES on every cartridge's Content screen: MAP DEFAULT or NONE (the
  PC cartridge's ROCKET HOGS, ALL MAP and ALL + ROCKET besides). NONE is
  Halo's own game variant vehicle set 1, so no map vehicle is placed (Race
  has its own "none"); Firefight keeps its own.
- The campaign level (a30), after the user's play: its trees and boulders
  thinned in the scenario's order (`tools/n64/campaign.py`
  SCENERY_THINNING: one pine in four, one large boulder in four, one
  gigantic in two, no small ones -- 293 scenery placements -> about 80);
  the Covenant alone (no Marine squads or placed Marines,
  CAMPAIGN_ALLIES); no squad that comes in a vehicle the N64 has not (the
  Banshee pilots stood where their Banshees would be, outside the map;
  DROPPED_SQUAD_WORDS) and each squad only at its starting locations inside
  the map; one warthog, moved beside the Marine base's squad on flat
  ground with no ledge near (CAMPAIGN_WARTHOG_BESIDE; a first try near the
  start went over a cliff); its rivers and waterfalls (chicago shaders, a
  mask their first map) drawn as Battle Creek's water is, and blue (a30's
  water tints are grey: Halo's blue is the sky in its reflection); and an
  actor 55 units from every player, not fighting, put away -- its squad
  comes again when a player is back near (everything placed had kept
  running after the players moved on, each biped ~3 ms of a tick).
  `tools/n64/repack.py` gained override_bytes for a field of any size (the
  moved warthog's position).

15 October, late: libdragon's RSP crash screen given room. On the user's
console a game stopped at "Out of memory" in display_init, under
console_init, under __rsp_crash, under display_get: the CPU had waited
200 ms for a frame buffer (the RSP or the RDP stuck on the frame before),
and the crash screen, which makes a 640x240 display of its own (600 KB),
found none of the memory free -- the model cache fills the heap and the
display's buffers are lent from the map's home -- so the screen showed its
own failure instead of the RSP's and RDP's state. The link now wraps
console_init (only the crash screen calls it; `port.json`'s ldflags):
the map's whole home is lent to the display and the uncached reserve
freed first. The hang itself is not found yet: the new terrain tiles are
16-byte aligned (the RDP hangs loading from an address 1-7 bytes into a
16-byte line, which emulators don't model) and the sky draws in the
standard mode; the next crash screen will say more.

15 October, night, later: a bot's movement every other tick with four
bots or more (the user's ask, after the profile of the 14th: with 2
players and 4 bots on Blood Gulch the tick was 22 ms against 6.4 alone,
~10% of the frame collision and physics, ~13% posing, ~20% Halo's object
bookkeeping, ~3% the bots' own thinking, 0.6% projectiles -- so no hitscan
for their shots). bipeds.c biped_update runs a bot's biped_update_moving
(its physics with it) on every other tick, staggered by the biped's index,
with n64_biped_step 2: collision_move_pill sweeps the pill the whole two
ticks' way (so it stops at walls as before), the acceleration limits and
gravity count both ticks, the velocity it keeps is a tick's; between, the
last animation state is held (by the biped's absolute index, one each; an
off tick with none held moves a tick's way, so the distance is always
right). In the air or landing, and dead, every tick. n64_bots.c
n64_bot_move_interval says which bipeds (bots, four or more).

15 October, night: bots by their level of detail (the user: optimise the
bots, large counts especially). A bot's own thinking was already cheap
(staggered, its rays and searches rationed); each biped's tick (~2.7 ms,
its pose the dearest part) is what grows with the count, and a bot's pose
rate came from the count alone (every tick, every other with 4 to 6, every
third past) whether any view showed it. n64_bots.c bot_detail now works
out each bot's level once a tick: full in a view lately
(n64_object_recently_seen) and within 15 units of a player, least out of
view and 30 or more from every player, reduced between. Its pose
(objects.c n64_far_ai_biped -> n64_bot_pose_interval): the count's rate at
full, one tick more at reduced (every other at least), twice it at least
(every fourth at least) -- Firefight's AI rule, now for bots of any count
-- and the count's rate whatever the level while it fires (a trigger, a
grenade, a melee: its shots come from its weapon's markers). Its thoughts:
each bot on a clock of its own (last_think, next_think; its timers count
the ticks since its last thought), twice as far apart at least
(n64_bots_examine_turn asks that clock). Its shots: the pauses between
bursts by the count (x1 up to 3 bots, x1.5 to 6, x2 to 8, x2.5 to 10, x3
past; half again at least detail) rather than by the think step, and
bursts a quarter shorter past 8 bots -- each shot is an object with a ray
a tick.

15 October, evening: the Halo 64 ROM Builder, a Windows program for
players (the user: drag and drop the Halo ISO or the maps folder, build
the ROM, no build process). tools/n64/rombuilder.py runs what
tools/n64_build.py's ninja build runs for each cartridge's files -- the
maps (convert.py, three at a time, each in a process of its own: ~800 MB
each), the menu (ui.py), the title music (music.py, once for both
cartridges), the sound bank, prune, mkdfs -- and makes the ROM with the
cartridge's prebuilt program (the build's compressed, stripped ELF and
its symbols: code, no Halo data), n64tool and ed64romconfig.
tools/n64/xiso.py reads an Xbox disc image (XDVDFS: the volume descriptor
32 sectors into the game partition -- at 0 in a trimmed image, 0x18300000
in a whole disc's -- and each directory a binary tree of entries); the maps
are copied out of it to the work folder (%LOCALAPPDATA%\Halo64RomBuilder)
and deleted after, the sounds' cache kept. tools/n64/rombuilder_gui.py is
the window (tkinter, tkinterdnd2's drag and drop, sv-ttk's look): drop
zone, the two cartridges, the output folder, Build, progress, the log.
libdragon's mksprite, audioconv64, mkdfs, n64tool and ed64romconfig are
built for Windows with MinGW-w64, statically
(tools/n64/build_windows_tools.sh); tools/n64/package_rombuilder.py
packages it all with PyInstaller (~27 MB zipped), the converters running
as the program itself ("--worker <module>", their output in log files: a
windowed program has no console). For Windows the converters call the
tools through tools/n64/toolpaths.py: their folder (N64_TOOLS, else
$N64_INST/bin), .exe, paths with forward slashes (mksprite names its
output after the input's part past the last /: with backslashes it wrote
"<folder>/C:\\...") and no console window (one each, from a windowed
parent: slow, flashing, and now and then 0xC0000142). The title music is
resampled by audioconv64 (libsamplerate) instead of ffmpeg. Checked: from
the Xbox disc image, in WSL, both cartridges in 16.7 minutes with cold
caches, their files byte for byte the ninja build's but the title music's
four (the resampler).

15 October, afternoon: the campaign's Covenant, memory and warthog (the
user: barely any enemies spawn, out-of-memory errors, a warthog belongs by
the base, the "dropships" are invisible but solid). Halo places a level's
objects at the start; a30's half on the cartridge has ~435, of them ~300
trees and boulders, ~230 KB of the 256 KB of object memory the N64 gives
Halo, and the encounters' actors (a biped ~7 KB with its nodes) found no
room: object_new failed ("OUT OF OBJECTS" on the console, which Halo prints
on the screen) and n64_campaign.c counted each such actor as placed and
down, so whole squads and waves never came. Now the scenery and ambient
sounds are left out of the start's placing (tools/n64/campaign.py
STREAMED_PLACEMENTS: "not placed" bit 0 set) and n64_campaign.c
stream_scenery makes them with Halo's object_new_from_scenario within 58
units of a player (past the fog's end) and deletes them past 66; an actor
with no room is tried again two seconds later instead of being counted
down (and one whose weapon found no room is not placed without it); four
bodies are kept, not twelve; the campaign's object memory is 288 KB. The
converter keeps only placements in the open space of the BSP on the
cartridge (campaign.py Placements, its collision's test_point), leaves out
Halo's particle emitters (objects with nothing else to them on the N64),
the cutscenes' props and -- at the user's word -- a30's crashed lifepods:
the "invisible dropships" were these, their insides' model at the highest
level of detail finding no room in the models' cache once the ground's
tiles took ~85 KB of the campaign's heap (~0.67 MB beside the 3.2 MB map
image). The campaign has no colour tiles (the ground's vertex colours with
the detail maps' tint), and its encounter table only the squads that can
start on the cartridge's BSP (31 of 78). The warthog: the vehicles' blocks
are kept, and a vehicle of CAMPAIGN_VEHICLES (a30's warthog by the marines'
base, at (-132, -31)) is placed at the start; the image grows ~75 KB for
it, ~30 KB less for the lifepods. The FPS counter has two more lines on a
campaign level: MODELS (the cache's KB, the models that found no room and
were drawn as nothing) and the game's objects, the share of their memory
in use, the actors and the scenery made.

15 October, later: Blood Gulch's sky and fog as the original's (the user:
the reddish-brown fog looked like a dust storm). No sky was drawn in a
game: past the draw distance the frame was cleared to the fog's colour,
the sky's outdoor fog, Blood Gulch's pale peach (1.0, 0.90, 0.77) -- in
Halo a light haze (at most 40% dense, 3 to 100 units: a fifth at the
N64's 55), here opaque where the level ends, over a peach sky. Its sky,
"clear afternoon", is a dome under a radial gradient seen from above,
deep blue overhead (0, 0, 143) to pale blue at the rim (180, 202, 224),
the radius the cosine of the height, under light grey puffy clouds.
n64_render.c draw_sky now fills each view from the horizon up with
strips whose rows take the sky's colour at their height (five stops,
0-90 degrees: n64_render_set_sky), the fog the sky's colour at the
horizon, so the level fades into it; n64_render_game.c's n64_skies table
gives Blood Gulch's colours (each stop a third of the way to the clouds')
and its fog a later start (0.6 of the draw distance, from 0.5). Other
maps are as they were.

15 October: the natural ground's colour map in tiles, a megatexture (the
user: Blood Gulch's ground was not as vibrant as the original's). Halo
lays one colour map over its natural ground (Blood Gulch's 512x512 spans
the map) under detail maps repeating every metre or two; the RDP samples
two textures a pixel and the detail map's mipmaps took both, so the
colour went on the vertices, 64x64 seven units apart -- grass, dirt and
paths blurred to beige. tools/n64/terrain_tiles.py gives each terrain
batch of a level (the 1-2 player levels and the campaign's; the 3-4
player levels keep their vertex colours) its own RGBA16 tile of the map,
at most 1024 texels and no finer than the map, the light baked in from
the lightmap under each texel (the terrain's triangles in the map's
coordinates), addressed with the batch's own detail coordinates shifted
right and wrapped (a texel stored at its coordinate mod the tile's
width): rom:/levels/<map>.tiles. Tiles over 256 texels are read when
their batch comes within 18 units (24 slots of 2 KB, 2 reads a frame,
the least recently drawn first), a copy of at most 8x8 staying in memory
for the distance. n64_render.c (MATERIAL_TILED) draws tile x (0.5 +
detail) in two cycles: the detail map's 32x32 level faded to grey by the
LOD fraction, which the RDP works out for a combiner that asks for it
without mipmapping -- with the triangles' max level 1 (SOMX_NUMLODS, which
tiny3d copies into its triangles) it rises from 0 to 1 as two detail
texels come to share a pixel, a few units away on flat ground. TERRAIN
DETAIL is now four levels (the user's ask): full (the ground's detail at
every distance, the default), high (faded), medium (high without rock and
earth's overlays), low (no detail maps); the smudge hunt's modes (every
mipmap, no mipmaps) are gone.
Blood Gulch: 103 tiles, 16 KB in memory, 176 KB on the cartridge;
checked offline by drawing the converted files top-down as the RDP
addresses them. The first build doubled colour x light as the level's
other textures are, and the user found it blown out: Halo's detail
functions do the doubling (twice the detail texel), and Blood Gulch's
grass detail is green, so the tiles (and the vertex colours of the
untiled ground) take the colour map times its detail maps' average
colours (level.py detail_tint: the primary's where the map's alpha is
high, the secondary's where low, then the micro detail's) times the
light. PIL scales RGBA premultiplied, which had blackened the map where
its alpha is 0 (the grass): colour and alpha are scaled apart.

14 October, afternoon: the powerups in their colours (the user: they were
all grey). The overshield and active camouflage are a metal frame
("powerup edge"), a few glass panels and an energy orb, a transparent
generic shader of grey noise maps (the jackal shield's) whose colour comes
from its seven combiner stages -- red for one, blue-green for the other,
added to the frame -- while the converter drew its first map alone as a
grey cut-out, and the glass as a cut-out of its grey reflection cube.
(Bungie's names are crossed: the overshield equipment uses the "active
camoflage" model, red, and active camouflage the "over shield" model, blue-green.)
tools/n64/stages.py evaluates a transparent generic shader's stages texel
by texel (maps where they lie, the objects' functions 0 -- neither
equipment exports any -- muxes on scratch alpha 0, output scale and bias,
-1..1 clamps); models.py stages_texture makes the energy's 32x32 RGBA16 of
it, brightened 2.25 times (the blend's 112 of 255 for Halo's addition),
texels under 20 transparent, drawn MATERIAL_BLEND; the "powerup glass"
panels (near clear in Halo) are left out. MODEL_MODE_BLEND now takes the
texture's alpha times GLOW_ALPHA (the other blended textures are opaque:
unchanged). Previews of the evaluated orbs were checked as images, not in
ares.

14 October, midday: a pause menu and named bots (the user's ask: the
scoreboard with up to 12 players, 4 on the controllers and 8 bots; one
button back to the main menu; bots named at random from Mario, Zelda,
GoldenEye and Rare N64 characters).

Pause. Halo's own pause UI is not in the N64 build, so START reached the
game and did nothing. n64_game.c's n64_game_pause_frame, run by
render_frame_present before each frame is shown: any port's START (the
presses gathered at every n64_input_poll, n64_input_take_pressed, so a
quick press between two slow frames is not lost) stops the game's clock
with game_time_set_paused -- main_loop goes on drawing a paused game, and
Halo's own code already holds the rumble, the motion sensor and player
control while paused. Meanwhile the game's gamepads read as untouched
(xdk_input.c n64_input_blocked), the game's voices are muted
(n64_sound_pause: they play on unheard, their volumes kept) and the Rumble
Paks are off. No pause at a game's end (n64_game_engine_postgame_state,
game_engine.c) or in its first ticks. START or B goes back to the game;
A, MAIN MENU, unpauses and calls main_goto_main_menu with
n64_quit_to_title set: n64_game_results gives no results and the menu
opens on its title (n64_menu_choice.to_title; FIREFIGHT chosen if that
was the game), the players still joined. The gamepads stay untouched until
the next game starts, so the A is not also a jump.

Scoreboard. n64_score_rows: every player (the controllers' as PLAYER n,
the bots by name) with Halo's own formatted score
(game_engine->format_player_score), kills and deaths, sorted by the
leading team (team games), then score (get_player_score), kills, fewest
deaths. n64_hud_pause (n64_hud.c) draws it over the frame: the picture
darkened, a panel in the menu's colours with the title, a note (team
scores in Slayer and CTF, who leads in the timed games, Firefight's wave
line, else the map), the columns sized to their widest text, a team square
per row, the controllers' players on a band, then MAIN MENU and "START
RESUME". Halo's small font: its capitals are 10 pixels (the 14-pixel
ascent leaves room for accents, n64_font_cap_height), so rows 12 apart fit
12 rows in 240 lines; the widest bot name (HUMBA WUMBA, 110 pixels) makes
a 299-pixel panel. Squeezed like the HUD with the anamorphic setting. The
results after a game use the same rows (sorted now, the bots by name, up
to 12: a taller panel past 8).

Bot names. n64_bots.c bot_names: 302 names of 11 letters at most (a
player's name holds 11), each bot a different one drawn with the bots'
random number (seeded at each game's start); kept across a bot's lives
(bot.name_index). Halo shows them in its kill messages too.

14 October, overnight (the user asked for a night of optimisations and
fixes, every map tested each time; runs in ares on a hidden display, the
scripts in WSL's /root/n64/overnight): numbers are ares's, one player
unless said.

Testing. Port.json flags for unattended runs: `debug_cycle=<seconds>`
(after so many seconds a game goes back to the menu, which starts the next
map by itself: every map in turn through the menu, as a player goes),
`debug_high_detail`, `debug_heap_dump` (every block in use past the maps'
home at the menu, its size and first words: what a game left behind),
`debug_rdpq` (libdragon's RDP validator), `debug_fp_nodraw`.

Memory. The HUD's recording ran out on Blood Gulch in high detail (its
sprites, loaded while it recorded, took the room it began with): a 64 KB
reserve for libdragon's uncached blocks, given back when one cannot be had
and taken again the next frame (and settled where the heap has room at a
game's end); the HUD's sprites leave a recording its room. The maps' home
made the menu run out after Sidewinder and Prisoner: freeing the image gave
nothing back, and the old level and its models were still loaded when the
menu loaded its logo -- n64_game_free_map unloads them. What each game and
the menu left behind, found by diffing the heap dumps from game to game,
is made at boot instead: the mixer's channel rings (made on a channel's
first play: n64_sound_reserve plays and stops a sound on each), the sound
bank's open file, libdragon's EEPROM cache, the models' render-mode blocks,
and newlib's float-printing big numbers (dtoa keeps them on lists of its
own for good). Now nothing is left: 74 blocks at every menu.

The CPU. Two hidden costs found in the profiles: the FPU trapped on NaNs
(the port emulates the instruction, thousands of cycles each) -- units.c's
aiming change divided 0 by 0 for every unit with no aiming limit, every
tick, and the HUD's flash colour took fmod by a zero period for every
coloured element; both give x86's answers now without the NaN. Firefight
on Blood Gulch: tick 17.1 -> 11.6 ms, 28.2 -> 29.7 fps (its lows 21.9 ->
27.5); two players and six bots on Wizard 10.5 -> 12.1 fps. Then: the
aiming change's angle from the chord (no arc cosine); Firefight's Covenant
unseen and over 30 units from every player posed one tick in four; a level
batch's fog test in its own 16-bit units (level 9.3 -> 8.2 ms with four
views); the depth reset by an RDP fill held to the scissor instead of
tiny3d's whole-buffer RSP clear behind an RDP fence (at the frame's start
and in each view's first person); debug flags looked up once where they were
asked for on every ray, effect, sound and AI biped; the HUD's unturned
bitmaps without a sine and cosine, its meters' colour sums without
divisions, and the multiplayer "player under the crosshair" casting its
rays only to units within its angle and range. Four players on Blood Gulch
16.4 -> 16.8 fps: that case waits on the RSP/RDP, and the first-person
models were found its dearest part (16.8 fps drawn, 24.6 posed but not
drawn): VIEW MODEL has a fourth value, OFF IN SPLIT (the low models, not
drawn with three or four views; still posed, the game takes a player's shots
from its weapon's markers).

The console's smudges: the RDP validator showed the mipmapped detail
terrain (any mipmapping is two cycles in rdpq, nearest too) and the
doubling materials with rdpq's fog alone in the blender's second pass, where
a console reads the shade's alpha -- the fog -- of the next pixel (a
hardware bug the validator names; no emulator shows it), and TERRAIN
DETAIL OFF, the one setting without smudges, was the one-cycle case. Those
materials now have a second pass of their own (the colour as it comes, in x
0 + in x 1: in x a + in x (1 - a) encodes as all zeros, which
rdpq_mode_blender takes for no blender), so rdpq keeps the fog in the first:
the validator's warnings on Blood Gulch 15499 -> 0, the picture unchanged.
ALL MIPS keeps the old way for the comparison. Firefight on Sidewinder in
high detail ran 25 minutes under the validator without an RDP hang.

The final code went round every map through the menu again and again: four
players (37 maps loaded), two players and six bots (56), Firefight (36),
high detail with two players (29) -- nothing went wrong, and no FPU
instruction needed emulating. (Rarely, the breakable surfaces' bit vector,
at an odd address, is still read a long at a time: emulated, correct.)

13 October, night: on a console, TERRAIN DETAIL "no overlay" took the
black smudges off the cliffs but not the ground, and "off" took them all:
the detail maps themselves, sampled far away, are the trouble. They are I4,
64x64, with mipmaps down to 4x4, and the RDP was told to sample down to
8x8 -- a level whose rows are half a TMEM word, loaded as I8 into padded
rows, the one odd case on that path and only reached in the distance (on
the ground at a glance, on the cliffs further off). FULL now uploads the
levels down to 16x16 only (detail_upload) and blends them in two cycles,
the overlay too; ALL MIPS is the old way, for the comparison on a console,
and NO MIPS samples the first level alone. High detail tries 520x240 and
480x240 after 640x240, and the FPS counter shows the 3D's resolution under
it (the screen's too when medium or chunky draw it narrower).

13 October, late: the DISPLAY setting (240p or 480i) is gone into 3D
RESOLUTION as its highest value, HIGH DETAIL (chunky, medium, standard,
high detail; right is higher). High detail is 480i where the map leaves the
memory (640x480, else 512x480, as before), else Perfect Dark's high
resolution: 640x240 shown at 240p, twice the pixels across and the same
lines (three buffers, else two: 1.2 MB with the z-buffer, where 320x240
took 600 KB), else 320x240 and a word on the screen (try_high_detail). The
buffers go in the maps' home past a smaller map's image where they fit.
A saved 480i (byte 9) reads as high detail.

13 October, late: those fixes were not enough -- the heap's map on the
console's screen showed 3973 KB free in four pieces (150, 1097, 521 and
2189 KB), split by a 40-byte block (libdragon's list of display buffers,
made when the game's display came back after the menu) and 44 small blocks
(19 KB) at the top of what the menu had used, with the rest of the menu's
memory free around them. Chasing each block left behind would never be
sure, so the maps' images now have a home of their own: made at boot from
the heap's bottom (n64_map_home_reserve, n64_game.c), the size of the
largest map the menu offers (Sidewinder's 2.8 MB), never freed; every image
is loaded into it and nothing else is made there, so that nothing can split
it. Between games it is lent to the menu's display (its two 640x480 buffers
and its own z-buffer, 1.8 MB, which were the heap's), and in a game the part
past a smaller map's image to the 480i buffers: while a loan is open the
surfaces libdragon makes come from it (the link wraps
malloc_uncached_aligned; freeing them is nothing). Smaller maps leave the
game the heap Sidewinder does (up to 1 MB less than before).

13 October, night: Sidewinder and Blood Gulch stopped with "no memory
for the map" (2811 and 2686 KB needed, 3947 KB free) when started from the
menu, never when loaded directly. The screen's "3931 KB in one piece" was
false: the halving probe's malloc, freed unused, was dropped by the
compiler, which then took every probe for a success -- and the HUD's room
checks (a sprite's, an upload's) had been dropped the same way and never
looked. What split the memory: the sound's buffers and mixer were made with
the title's music, after the menu's 640x480 buffers (1.2 MB), and stayed in
the middle once the menu was gone (about 1.4 MB under them, 2.4 MB over);
the mixer's VADPCM decoder states (malloc_uncached, made again for each
sound started) stayed wherever the last ones went; and a game's HUD
sprites, recordings and uploads and its effects' cache and atlas were only
given back when the next map was set up, after its image. Now the sound is
made at boot (n64_main.c, with the reserves); blocks of 64 bytes or fewer
from malloc_uncached come from a pool of their own (the link wraps
libdragon's malloc_uncached and free_uncached: port.json); the HUD's and
the effects' per-map memory is given back when a game ends
(n64_hud_release, n64_particles_release, from n64_game_free_map); the room
checks go through n64_heap_has (the block kept in a volatile). The failure
screen walks newlib's heap instead (n64_heap_describe): the largest piece,
and each free piece of 64 KB or more with the blocks in use between them,
so that if it happens again it says what stood in the way.

13 October, evening: the console's black smudges are on terrain drawn
with two textures (a colour pass, then the detail overlay darkening it).
Nothing like it is reported for tiny3d or libdragon, and neither ares nor
anything else here reproduces it (mupen64plus with angrylion-rdp-plus
cannot boot libdragon's IPL3; CEN64, with the user's PIF ROM and three
local patches, boots but hangs silently). The overlay pass is made as
ordinary as it can be: rdpq's multiply blender (black at an alpha of
1 - texel, the fog fading it) instead of a formula with the framebuffer as
the blender's first input, its mipmaps chosen (MIPMAP_NEAREST: one cycle)
instead of blended in two cycles, no alpha dither; and the converter's
overlay texels never darker than 5/15 (OVERLAY_FLOOR_LEVEL), so that
whatever the console does there, it cannot smear black. TERRAIN DETAIL
stays, to tell which part it was if the smudges remain.

13 October, later: two reports from a real console. (1) Sidewinder
stopped with "no memory for the map" after another game had been played:
the animation cache (48 blocks of 4 KB), the HUD's meter slots (96 KB),
its blip and font and the explosions' fireball were made with the first
map, above its image, and kept: with that map gone they split the heap,
and Sidewinder's image (2.8 MB, the largest) found no piece big enough.
They are made at boot now (n64_animation_reserve, n64_hud_reserve,
n64_particles_reserve from n64_main.c), at the heap's bottom; the message
now gives the size needed, the heap free and the largest piece. (2) Black
smudges on the natural terrain with detail textures, away from the eye,
on the console only (not in ares). Not reproducible here, so SETTINGS has
TERRAIN DETAIL to find the part at fault on the console: FULL (as before),
NO OVERLAY (the rock and earth overlays left out), NEAREST MIPS (the detail
and overlay maps' mipmaps chosen, not blended: MIPMAP_NEAREST), OFF (the
terrain's light alone, no overlays); kept in the EEPROM record's byte 14.

13 October: Rumble Pak. Halo's rumble (player_rumble.c, two motors at any
speed) reached XInputSetState through input_flush_rumble on a thread the
N64 never runs: on HALO_N64 input_vertical_blank_interrupt (called by
render_frame_present after each frame) flushes it instead. XInputSetState
hands the speeds to n64_input_rumble (n64_input.c), which makes one
strength (the heavy motor's, or three quarters of the light one's) and
switches the pak's motor on for that share of the frames (a running sum:
on in a frame where it passes 1), off below 0.08; libdragon finds the pak
(joypad_get_rumble_supported) and the motor is only told of changes. The
CONTROLS page has a RUMBLE row (on by default; a 12-frame buzz when turned
on; the rows 17 pixels apart to fit seven), kept in the EEPROM record's
byte 13 as a bit a controller with it off (records from before read as
on). In the menu every motor is held off (n64_input_rumble_menu_frame).

12 October, later (built, not yet measured): a crash out of memory while
the HUD recorded a view (`__rdpq_block_next_buffer`): the model cache fills
the heap down to its margin and what is allocated after it (HUD recordings
and upload blocks, sprites, the bots' paths) ran out.
`n64_render_make_room` drops the models drawn longest ago until the room
can be had in one piece; the HUD makes room before it records a view, or
draws it unrecorded that frame. Then a round of CPU savings for bots and
split screen, found by reading the code (nothing measured yet):

- Bots and Covenant: Halo's path search no longer clears its 47 KB state
  each time, has a smaller hash table and nodes, smooths from the end, ends
  when its node list is full, is weighted (the distance to go counts 1.5
  times: far fewer surfaces opened around obstacles), is bounded (2.5 times
  the straight line plus 15) and runs a slice a tick (40 surfaces) instead
  of all at once; a straight walk to the target is tested first and needs
  no search where it's clear. Sight rays are skipped where the clusters'
  potentially visible set rules the pair out, and stop at the level when
  it is in the way. A bot looks over the objects around it only before a
  tick it thinks. Firefight's per-Covenant log is a debug flag.
- Objects: a holstered weapon has its full update every fourth tick (its
  weapon update every tick: a holstered plasma weapon picked up at rest
  cooled at a quarter speed); bullets and bolts without a model stay out
  of the cluster partitions; a shot's pellets share one test that keeps
  them inside the level, the shooter left out of it; bodies settle one a
  tick, unseen ones not at all; nothing makes dynamic lights (nothing
  drew them); a kept biped pose keeps its held weapon's pose and doesn't
  put its IK on again; ray and pill tests reject object types they don't
  test by the object's header.
- Effects made of nothing but sounds, particles, decals and lights are not
  made farther from every player than they can be seen or heard. A sound
  started again reuses its file loaded in a stopped voice.
- Drawing: a model's batches alike in a row are one draw; big level
  clusters are tested in groups of 16 batches before their batches; the
  vehicle and death cameras cast 4 bisection rays, not 10; aim assist
  looks the eye's leaf up once and skips rays the PVS rules out.
- The motion sensor steps every tick: with three or four views it stepped
  only when view 0's HUD was recorded, and its scan seldom ran.
- Firefight with two or more players ended as it began (the end-of-game
  camera, one view): Halo ends a game when only one team is alive, and
  since the morning's team fix Firefight's players are all red.
  game_engine_should_end_game leaves Firefight to end its own game.

12 October: the game keeps to real time below 15 frames a second. The
Xbox's main_update_time let a frame run two ticks at most (it never fell
below 15), so on the N64 three or four views, Firefight and bots slowed
the whole game down; HALO_N64 lets a frame run up to 7 (game_time_update's
limit for a local game): real time down to ~4 frames a second, fewer frames
where a tick is dear. With three or four views every biped's pose (the
players' too) is made every other tick, staggered (objects.c; a living
biped's camera is its sight position, not a node's). SLAYER PRO and TEAM
SLAYER PRO: slayer with everyone starting with a pistol (in hand) and an
assault rifle (n64_game.c `n64_game_pro_weapons`, called from
game_engine_postspawn_player_update). Up to 8 bots whatever the players. With 4 to 6 bots each thinks and has
its pose made every other tick, with 7 or 8 every third, staggered
(`n64_bots_update_interval`): between thoughts a bot's last action is held
(stick, aim, a held trigger) without its presses (jump, grenade, melee,
reload, action, swap); its timers, turning and grenade chances count by the
interval, so it plays the same, a little slower to react.

Then the user found bots far slower since the real-time change: a frame
could run 7 ticks, and where a tick costs more than a thirtieth of a
second (many bots) the frame rate sank (35 ms ticks: ~4 frames a second at
90% speed). A frame runs 4 ticks at most now (real time down to 7.5 frames
a second; 6.5 at 86% there). The bots and the Covenant spend less: a path
(the dearest thing an AI does) is found at most every 3 ticks among the
bots and every 2 among the Covenant, kept 5 s and found again when the
target has moved 7 units (3 s, 4); sight every 10 ticks for a bot (6) and
none past 50 units, every 12 for a Covenant (8); a bot looks for weapons
every 45 ticks (20) with two lines of sight at most; with 4 to 6 bots
their pauses between bursts are half as long again, with 7 or 8 twice
(fewer shots in the air: each projectile an object updated every tick);
the Covenant think every third tick with five or more up or three or four
players (every second), turning as fast a tick.

12 October: vehicles' physics every other tick, every fourth with
ANIMATIONS HALF RATE (the user's ask; physics.c physics_update,
n64_vehicle_physics_turn): a vehicle's forces are worked out and its
movement made on its turn only, staggered; the rest of vehicle_update runs
every tick (controls, seats, timers, animations keep time). Each step
covers the ticks since the vehicle's last (at most the interval,
n64_physics_step): the worked-out and control forces are scaled to that
many ticks' impulse, the velocity carries it that far, the turning that
much (both the new and the old physics paths); what other objects pushed it
with meanwhile counts once. Coarser steps: the suspension and contacts are
worked out half or a quarter as often, so vehicles may bounce or settle
differently, most at a quarter; a driver's view moves in those steps.
The first build crashed (the user's screenshot: effect_set_event from
create_slipping_effects): physics_update returned early on the off ticks,
but vehicle_update's mass points are a stack array the step fills, and the
slipping, suspension, crashing and airborne updates (and the Ghost's hover)
read it uninitialised. Now vehicle_update asks n64_vehicle_physics_turn and
leaves out the whole physics block on the off ticks; the crash test takes
the velocity change per tick and the airborne/on-ground counts add the
step's ticks (n64_vehicle_physics_step). With ANIMATIONS HALF RATE the bots
and Firefight's Covenant also think every fourth tick (n64_bots_update_
interval; the bots' pose keeps n64_bots_pose_interval; think_interval, the
turn over four ticks 0.84).

12 October: the settings are kept (the user's ask): the SETTINGS page and
each controller's CONTROLS in the cartridge's EEPROM (the ROM's header now
declares a 4 kbit one: ed64romconfig --savetype eeprom4k, header byte 0x3F
0x1E; ares keeps it in a save file). A 48-byte record (n64_menu.c
settings_pack: "H64S", version 1, each value, a checksum) read when the menu
first runs (settings_load; a missing or damaged one leaves the defaults,
each value range-checked) and written when the SETTINGS or CONTROLS page is
left and when a game starts, only if it changed (settings_save). The game
setup (type, map, score, bots) is not kept. And CHUNKY MODE became 3D
RESOLUTION: STANDARD, MEDIUM (new: the 3D drawn at four fifths of each
view's width, 256 of 320, and widened by a quarter in place, point sampled)
or CHUNKY (half, as before; n64_render_set_chunky takes the level,
chunky_width, stretch_view widens by the ratio, particles squash by
n64_particle_view.chunky_scale).

12 October: Settings ANIMATIONS FULL RATE / HALF RATE (the user's ask).
Half: a parentless biped's, vehicle's, scenery's or device's pose (node
matrices, and a biped's function values) is made on its every other tick
only, staggered, and the existing rules count their turns on a clock that
runs at half speed on those ticks (objects.c n64_pose_clock): what was every
tick is every other, three or four views' every other is every fourth,
unseen AI's every fourth, bots' every second or third every fourth or
sixth. Held weapons and riders still follow their parent every tick;
projectiles and items keep the full rate. n64_half_animation_rate
(n64_menu.c). Then the first-person gun too (the user's ask): its pose's
orientations (keyframes, overlays, sway) are worked out every other frame,
the views taking turns, a new weapon's at once; in between the last ones are
placed at the camera as it is now, so the gun stays in the view's hand
(first_person_weapons.c first_person_weapon_build_node_matrices).
Settings rows are 28 pixels apart now (eight and BACK).

12 October, three of the user's asks (untested here). The HUD is recorded
every sixth frame with three or four views (was fourth), the views' turns
spread over the six (n64_hud_begin). Settings has two rows more: DISPLAY
240P/480I and WIDESCREEN OFF/ANAMORPHIC. 480i in the game
(n64_render_set_480i, try_480i): once the map is in, the 320x240 display is
closed, the model cache emptied (it refills), and 640x480 tried, then
512x480 (stretched to the same 4:3 by the VI), each only if two buffers, the
z-buffer and 448 KB for the models fit (display_fits allocates and frees
them); else 320x240 again and "NOT ENOUGH MEMORY FOR 480I ON THIS MAP" for
five seconds. Shown as both fields per frame (INTERLACE_FULL, two buffers,
as the menu). Memory decides: at level load the small maps leave 2.1-2.4 MB
(Wizard, Chill Out, Prisoner, Hang 'em High, Rat Race, Longest, Carousel),
the middle ones ~1.8 MB, Blood Gulch 1.05-1.1 and Sidewinder 1.3 -- 640x480
needs 1.8 MB of display where 320x240 took 0.6, so in practice 512x480 on
the small maps and 240p on the big ones. The game's 640x480 layout is now
scaled by n64_render_layout_scale_x/_y (views, HUD quads, text, blips; half
at 320x240, to the bit as before), the 3D's aspect by the pixel's shape
(display_pixel_aspect), the far versions' pixel threshold by the lines,
HUD sprites point sampled at the screen's whole multiple (2 at 640x480).
The anamorphic picture: a 16:9 view squeezed into the 4:3 one (the 3D's
aspect a third wider at the same vertical field of view; particles
squeezed across, n64_particle_view.x_scale); the HUD squeezed by 3/4 about
each view's middle (n64_render_hud_squeeze), so it keeps its shape in a
4:3 area in the middle and the game's own screen projections (nav points)
land where the wider 3D puts them; a scope's mask leaves the sides open.
Then the menu too (the user's ask): with WIDESCREEN on, its scene is drawn
wide and its 2D (panels, frames, pictures, text) squeezed by 3/4 about the
screen's middle (n64_menu.c squeeze, squeezed_x, menu_text), the pages'
full-screen backdrops still full width; toggling the setting shows at once.
Text squeezed across is drawn filtered (n64_text_draw_scaled, the HUD's
text too); at 1 it is drawn as before.

12 October, the HUD and whatever else could be found, without runs (the
user tests), from the 8-9 October profiles, the disassembly and a research
pass on the VR4300 and ares (ares charges the CPU for instructions, FPU and
divide latencies and cache misses: a data line 40 cycles, a code line 48;
it does not seem to model the RDP's memory traffic, so the CPU is what
counts). The HUD: each sprite rectangle a quad uploads is kept as a
recording of its own, made after the HUD's recording and called from then
on (rdpq_tex_upload_sub's work on the CPU every quad: 48 of them, freed with
the map); motion sensor blips upload the dot once for all of them and draw
plain scaled rectangles (rdpq_tex_blit's planning per blip); text loads its
atlas a band of 32 rows at a time and draws each glyph from it (a load per
character before) -- the same commands, texels and order. The maths: on
the N64 the game's sin, cos, atan2, acos, pow and exp are single precision
(port/include/halo_math.h: musl's double ones existed for lockstep network
play, which split screen doesn't have; bipeds, vehicles and projectiles
called them every tick); fast_ftol, fast_ftol_C and colour packing round by
the VR4300's conversion (cvt.w.s/cvt.w.d: rint on a double was a library
call); the animations' keyframe floors and the objects' function value
floor and fmods are float ones, to the bit (n64_floorf, n64_fraction,
fmodf); bots and Covenant use square_root. fminf/fmaxf/floorf/ceilf in the
HUD, particles and fog tests are inline (newlib's asked __fpclassifyf).
Teleporter checks (every player, every tick) read only the map's flags of
their type (game_engine.c find_netgame_flags, listed once a map). A
debug flag name at a second address is cached as well (it compared names
on every call). The periodic statistics (every 30, 120 and 240 frames: float
formatting, a heap walk, a scan of the 192 KB stack) print only with
`debug_stats`. Code generation (tools/n64_build.py CODE_FLAGS): -mno-fix4300
(the toolchain's default -mfix4300 put a nop after each of 8,179 floating
point multiplies, for a multiplier bug of the first consoles' CPUs that
ares does not have -- a real early console could get rare wrong products),
no 8-byte padding of loops, jumps and labels; error(), display_assert and
halt_and_catch_fire are cold. 43 KB less code (1.30 MB). Tried and left
out: libdragon with NDEBUG (it also takes out its crash screen and debug
channel) and without its pattern-initialised locals (two instructions in
the texture loader).

12 October, the bipeds' movement physics, again without runs (about 1 ms
a biped a tick, measured 9 October: one collision_move_pill each, which
gathers the level's geometry around the pill -- a walk down the collision
BSP, every nearby surface's edges -- then runs the pill's move against the
features). Each biped keeps the level around it: a list of the vertices,
edges and surfaces (and their clusters) of a sphere 0.25 units wider than
its pill's, and while the sphere it needs stays inside, the features come
from the list -- vertices and edges by the walk's own sphere tests,
surfaces by bounding spheres, objects gathered fresh from the kept clusters
-- so a running biped walks the BSP about one tick in four and a standing
one not at all (collisions.c `n64_pill_features_in_sphere`; 12 lists, 23
KB; dropped with the map, a BSP switch or a broken surface; where the
level is too busy for a list the margin halves, down to 0.1, then the walk
for a second; `debug_no_pill_cache` turns it off). The walk itself
(collision_bsp.c): its lists of vertices, edges and surfaces found an entry
by reading the whole list (a few thousand compares for a pill), now a
hash; a surface reached again from another leaf is not walked again (it
lists nothing new); the lists come out the same. The move: a prism facing
away from the move is passed over before its test, whose hit would have
been dropped (collision_features.c). The same features but for their
order, which only decides between two at exactly the same distance.
Unmeasured.

12 October, an optimisation round without runs (the user tests the
builds), from the 9 October measurements: idle objects -- an item lying at
rest, an empty vehicle at rest, scenery that does not animate -- are
updated every fourth tick (objects.c `n64_object_idle`; on Blood Gulch they
were ~4 ms of a 21 ms 4-player tick: 15 weapons 1.9, 19 equipment 0.8, 45
scenery 1.8, 2 vehicles 1.2; pickups are the players' side, item physics
waits for whatever clears the at-rest flag); a bot no view drew (or far
from every player) has its pose and function values made every other
tick, as the Covenant; the object header lookups are inline (objects.h:
datum_get was 6-7% of the processor, mostly object lookups); sine, cosine,
atan2 and acos are Cephes' single precision routines (real_math.c; newlib's
sinf reduced every angle past pi/4 through __ieee754_rem_pio2f; within
3e-7 of libm's). +7 KB of code. Next, with measurements: the bipeds'
movement physics (collision_move_pill, ~1.3 ms a biped a tick, a quarter
of the tick) and the HUD's recordings (5-8 ms a frame with 2-4 views).

12 October: bots in multiplayer (`port/n64/game/n64_bots.c`; the setup's
BOTS row: left/right how many, A their skill; players and bots at most 8).
A bot is a player of the game like the ones on the controllers --
`player_new` on a machine of its own (a machine holds four), spawned by the
game engine in the player biped, scored, respawned, on a team -- whose
action (stick, aim, buttons) comes from here: `update_client_dequeue`
(HALO_N64) hands each tick's actions to `n64_bots_actions`, and
`players_update_before_game` applies them as it does a controller's, so the
action button picks weapons up, swaps them and gets out of vehicles, and
the rules are the players' own. The fighting is the Firefight Covenant's:
the nearest enemy (another team's player in a team game), Halo's paths
while out of sight, the weapon's range and strafing in sight, bursts (held
for automatic weapons, pressed shot by shot for the rest, explosives never
up close), aim error by skill, grenades, melee, unsticking; a better weapon
lying near is picked up when no fight is in sight; reload and swap when
empty. No shot without a clear line (teammates block it). Objectives (flag,
hill, ball, race) are not played: bots fight. Team games now keep the team
each player chose (the menu's Z): the game engine dealt teams out in turn
(`game_engine_player_added`, HALO_N64). Flags: `debug_bots=<n>`,
`debug_bot_skill=<0-3>`, `debug_game_type=<n>`. Built, not tested by me
(the user tests the builds).

11 October: the menu runs at 640x480 interlaced, the game at 320x240 as
before. `n64_render_set_menu_display` (`n64_render.c`) closes libdragon's
display and opens the other: the menu's is two 640x480 16-bit buffers shown
as both fields (`INTERLACE_FULL`) and a 640x480 z-buffer (1.8 MB), the
game's three 320x240 buffers; while the menu runs a block of the game
buffers' size holds their place, so a game's heap is laid out as before.
`n64_menu_run` switches on entry and, once a game is chosen, back to
320x240, where it draws the loading screen once more (shown while the map
loads). After a game, `main_menu_load` (`source/main/main.c`, HALO_N64)
unloads the old map before the menu, not after it, and frees its image
(`n64_game_free_map`): the menu's display needs the memory, and the next
game reads its map again. Every screen is laid out on 640x480 with Halo's
fonts and pictures at their own size, as on the Xbox's menus (text half as
big against the screen as before), inside a ~5% safe area, lines two
pixels thick (a one-pixel line flickers on an interlaced television); the
logo is exported 560 pixels wide (RGBA32), the maps' pictures at 256x128
and the game types' at 192x96 (`tools/n64/ui.py` PICTURES); one map
picture and one game type picture are kept loaded at a time. CONTROLS and
SETTINGS rows are a name on the left and the value on the right. Heap in a
game (one player, Battle Creek): 1845 -> 1843 KB after the map, 1626 ->
1623 KB after the level. Flag `debug_game_ticks=<n>`: a game ends after n
ticks (testing the way back to the menu).

11 October, memory and the CPU (built, not run here: the user tests):

- The heap has ~0.8 MB more in a game. Halo's game state was the 1 MB
  ceiling the port began with (`HALO_PORT_GAME_STATE_CPU_SIZE`), of which
  the N64's pools took ~640 KB; when the first map locks it, the buffer is
  shrunk in place to what was allocated (`n64_game_state_trim`,
  `port/n64/game/n64_game.c`; the log says "n64: game state N KB of 1024 KB
  used, M KB given back", and an allocation after the lock stops the game
  with a message rather than overrunning). The pools of what the N64 never
  makes are tokens: decals (2048, 112 KB -- none are made now,
  `decals_enabled`: nothing drew them, and every bullet impact cast a ray
  and clipped a decal against the level's surfaces), glows (512 particles,
  50 KB), Halo's own particles, particle systems and contrails, the other
  widgets (flags, antennas, light volumes, lightning): ~404 KB used, ~620 KB
  given back (counted from the allocation calls in the ELF). The program
  image is 134 KB smaller: the render globals' 128 rendered clusters (one
  now: the Xbox renderer's visibility pass filled them, 52 KB), the
  profiler's 150 tick timers in each of its nine frames (eight, 32 KB), the
  decal clipping buffer (30 KB), and the code (below). The game runs on
  libdragon's own 64 KB main stack made 192 KB deep by lowering the heap's
  top under it (`sbrk_top`, as the display does for the Z buffer, which
  stays in the last megabyte): 128 KB of heap instead of a 192 KB stack of
  its own (`port/n64/src/n64_main.c`). Firefight's heap low point (~90 KB)
  was under the model cache's margin (96 KB): a model load there first
  dropped others and waited for the RSP to finish with them.
- Small data: the game and the platform layer are compiled with `-G8`
  (port.json `small_data`; libdragon's own default): globals of up to 8
  bytes in `.sdata`/`.sbss`, read from `$gp` in one instruction instead of
  `lui` and a load. 2.3% fewer instructions in the program (306,788 ->
  299,715; `lui` 13,446 -> 8,824), 28 KB less code; in ares an instruction
  is a cycle and an instruction-cache line refill 48.
- A view's object draws are sorted by a merge sort written out (newlib's
  `qsort` called its comparison through a pointer and swapped two-byte
  entries a byte at a time, twice a view), to the same order (checked
  against a stable reference sort on the host).
- The "free half megabyte" (Kaze Emanuar, December 2024) is RDRAM's ninth
  bits: 0.5 MB per 4 MB that only the RDP and VI use (coverage, the Z
  buffer's slopes). The CPU reaches them only in the MIPS interface's EBus
  test mode, uncached, four bits per 32-bit word, and any ordinary write to
  a byte resets its ninth bit; ares v148 (the user's) does not emulate that
  access at all (its hidden-bit support came in July 2026), so data kept
  there would read back as garbage. Not used; the game state above is the
  half megabyte this ROM had idle.
- `tools/n64_build.py`: `game_optimisation` (the game's own level, e.g.
  `-Os` -- not used: an earlier -Os build crashed, untested since),
  `small_data`, and `source_flags` patterns (`source/physics/*.c`). GCC
  keeps each function's own -O level through LTO.
11 October, Sidewinder ("completely bugged, horrible artifacting and
missing models"), found from the converter's data:

- Its snow pines' needles were solid cards, as were Blood Gulch's shrubs,
  ferns and broadleaf plants: Halo's model shaders test the base map's
  alpha (at 0x7F) unless marked "not alpha tested", and blend alpha-blended
  decals by it. The converter ignored both, and mksprite's CI4 quantizer
  drops alpha anyway. Such textures now keep a transparent palette entry of
  their own (`tools/n64/level.py` cutout_palette; split_palette for the
  colour-change ones, as the needles are), their batches carry
  MATERIAL_ALPHA_TEST and the renderer draws them cut out at half alpha
  (MODEL_MODE_CUTOUT). The weapons' decals (rocket launcher, sniper rifle,
  shotgun, frag grenade, flamethrower) and the ammo boxes are cut out too.
  Far versions keep cut-out parts textured and leave decals out; one with
  cut-out parts no lighter than the near version is dropped (the pines are
  cards the simplifier cannot merge: 15 KB a pine, not 30).
- Holes in the ground, cliffs and base walls: a surface several BSP
  clusters list (it crosses between them) went to the first of them, and
  Sidewinder's potentially visible sets are tight (a cluster sees 7-17 of
  its 35): from where another of them was visible and the first was not,
  the surface was not drawn (258 surfaces; Blood Gulch 189). level.py
  surface_homes gives it to one of them that every eye seeing any of them
  sees too, else to a group drawn when any of them is visible: the level
  file (version 4) lists each cluster's owners. Clusters: Sidewinder 35 ->
  43, Battle Creek 29 -> 48, Blood Gulch 30 -> 37 (a few more batches).
- Objects were drawn by their own cluster alone: a player, vehicle or tree
  near a portal vanished from places that see only the cluster beside it.
  Now any cluster the object's sphere reaches (Halo's cluster partitions,
  object_get_first_cluster) in the eye's set draws it
  (`port/n64/game/n64_render_game.c`).
- The teleporters' energy (one emitted colour on a grey mask) was an opaque
  green slab: translucent now (MATERIAL_BLEND, MODEL_MODE_BLEND at
  GLOW_ALPHA, no depth written).
- Smeared trims: a level batch's texture coordinates started at its lowest
  whole repeat, and a lone 1,229-texel trim triangle passed s10.5's 1,024
  (clamped). Level and model batches are centred on 0 now, and a model
  part spanning more than s10.5 holds is batched within 1,792 texels
  (Blood Gulch's trees: 21 clamped vertices -> 4). Damnation, Boarding
  Action and Hang 'em High still have single level triangles too long for
  s10.5 (63, 57 and 6; were 107, 180 and 52): they would need splitting.

9 October, afternoon (on the user's Windows PC, in WSL):

- Water (Battle Creek's creek, Halo's `swat`): one flat colour, blended
  (`MATERIAL_WATER`, `tools/n64/level.py` water_colour; the renderer's
  `WATER_ALPHA`): untextured, its vertices the shader's perpendicular and
  parallel tints' mean x0.7. Before, its base map's ~0.2 alpha was lost in
  CI4's one-bit palette alpha and, with no lightmap and no light of its own,
  the surface drew opaque black over the creek bed -- and cost ~6 ms of
  draw in view of it.
- The HUD's black pixels: its bitmaps were halved with Lanczos (its
  undershoot, divided by a small alpha when unpremultiplied, left dark
  texels along every thin outline), their transparent texels black
  (bilinear filtering mixed them in), every quad filtered at 1:1 (half a
  texel of blur), and meters' edge texels darkened by their coverage, opaque.
  Now `tools/n64/hud.py` halves by box filter and bleeds the edges' colour
  into the transparent texels; quads at their sprite's size are point
  sampled; meter texels of at least half the meter's own strongest opacity
  are drawn at least three quarters as bright, the rest cut out.
- Settings: HUD INSET (0-20 pixels, by 4: Halo's window bounds shrink by it
  while the HUD lays out -- corners, messages, scores, nav points; flag
  `debug_hud_inset=<pixels>`) and FPS COUNTER (off unless turned on; a
  `debug_map` game still shows it).
- CONTROLS (the title's new entry): per controller, the scheme, crouch held
  or toggled (a jump stands back up) and look inverted
  (`n64_controls`, n64.h; `xdk_input.c`).
- Aim assist stronger (HALO_N64): the weapons' autoaim and magnetism cones
  x1.75, their reach x1.25 (`source/game/aim_assist.c`); magnetism friction
  0.6 -> 0.75 and adhesion 0.7 -> 0.95 (`source/game/player_control.c`).

9 October: particles again, simply (`port/n64/src/n64_particles.c`): what
effects make -- muzzle flashes, sparks, dust, blood, smoke, explosions --
as a pool of 128 soft glows in each particle's colour, flying straight
(gravity and drag) and fading out within a third of a second (a tenth on
your own gun; anything Halo meant to linger -- smoke, debris -- is left
out, it hung in the air), drawn as
z-tested rectangles facing the camera with the depth
tiny3d gives its triangles; first-person ones (a muzzle flash on your own
gun) stay in the camera's space and are drawn after the weapon.
`tools/n64/effects.py` writes each map's effect particle lists as compact
emitter records on the cartridge (`fx/<map>.fx`, ~160 KB; read when an
effect plays) with a 32-cell 16x16 I4 atlas of the particles' pictures and
each cell's colour; `source/effects/effects.c` (HALO_N64) counts and places
them as Halo did, but one particle each from an event's two largest
emitters, drawn half again as large (up to 20 pixels a half): fewer,
larger. A budget (~300 new a second, half with 3-4 views), looping effects
thinned; Settings turns them off. The
`part`/`pctl`/`cont`/`ant!`/`flag`/`glw!` tag groups are no longer kept
(~50 KB of RAM). Also: the 1- and 3-4-player views see Halo's full 70
degrees (the Xbox narrowed every view by 0.85 for TV overscan; the wide
2-player views are unchanged), and Firefight takes a fallen Covenant's
body away five seconds later.

Level textures from the larger original images (`tools/n64/level.py`), on
every map. A texture without colour of its own (`needs_colour`: mean
saturation under 38, its mean colour within 25 of grey -- the Human and
Forerunner metal, Sidewinder's snow cliffs) is greyscale (`grey_texture`):
I4 (IA4 cut out) with no palette fills all 4 KB of TMEM, 8192 texels --
128x64 or 64x128, whichever keeps more of the picture -- against a colour
texture's 64x64, its greys stretched to the full sixteen, the stretch undone
on the vertices and Halo's doubling of texture x light in a second combiner
cycle (in the texture it turned bright textures white, on the vertices
bright light dark). The Covenant's purples and blues, tinted metal, rock and
earth keep their colour. Natural terrain -- an environment shader with
detail maps whose base map spans 40 units or more, Blood Gulch's, Battle
Creek's, Sidewinder's ground (`terrain_materials`) -- has the base map's
colour on its vertices (sampled 64 a side, edges split where it changes)
and Halo's own detail map as the texture: greyscale 64x64 with mipmaps
(they fade it to grey with distance), drawn light x (0.5 + detail), the
map Halo's blend picks by the base map's alpha (sand or grass) chosen per
triangle. Batches of these may span 832 texels.

The level's triangles are tiny3d strips now (`strip_chunks`, read by DMA
into the vertex cache's free end: batches are 64 vertices), not recorded
rspq blocks (8 bytes a triangle, and each block's chunks): Sidewinder's
level cost 590 KB of heap and the game ran out of memory looking across the
map; ~340 KB now. Heap after the level: Sidewinder 699 -> 918 KB, Damnation
1084 -> 1486, Blood Gulch 1096 -> 1223.

Rock, earth, snow and wood (Halo's material types; `overlay_material`) with
a detail map have it drawn over them in a second pass: one pass cannot
take a colour texture, a detail texture and the vertices' light (three
products, two combiner cycles). The overlay is the same batches (the same
culling) compared as decals, its mipmapped I4 texel darkening the
framebuffer (the RDP's blender wraps brighter); the material's own pass is
brightened by the overlay's mean (0.88: at 0.75 bright rock saturated and
went grey). With fog it fades away: the shade's alpha is the fog's
visibility, 1 near (rdpq's fog blends IN by it). Not in the lighter level
of 3-4 player games.

The black slabs on many maps were Halo's additive light volumes (green
lights, "black lights", cone and space lights: black textures, invisible
added) drawn blended; glass was drawn with its dark reflection cube. The
N64 cannot add (the blender wraps), so additive transparent level shaders
and glass are left out (`dropped_shader`), the light cones with them; only
alpha-blended ones (Damnation's waterfall) and water are drawn.

Settings: VIEW MODEL LOW / MEDIUM / HIGH -- the first-person weapons and
arms at their triangle budget, 1.25 or 2 times it (`levels/<map>-fp1.mdl`,
`-fp2`, made beside the plain set; `n64_render_set_view_model_quality`,
flag `debug_view_model=<0-2>`).

Firefight: a jackal cost ~1 ms of a tick more than a grunt (its longer
skeleton and its shield's function values). A Covenant no view drew in the
last frames (or far from every player) has its pose and function values
made every other tick. The HUD's line of sight to a teammate's nav point is
tested again every six ticks, not every recording; with two views each
view's HUD is recorded every third frame. Levels draw a cluster wholly in
view and in the fog's reach without a test per batch. The rspq buffer is
4096 words (with the strips 2048 kept the CPU waiting on the RSP).
Flags: `debug_firefight_kind=<0-2>` (every Covenant a grunt, jackal or
elite), `debug_firefight_wave=<n>`, `debug_no_overlay`,
`debug_overlay_alpha` (the overlay's fog alpha as grey). The emulated
float exceptions are counted in the render statistics (~15 a frame).

A build dropped only debug flags it knew by name, so new ones lingered in
later builds (a benchmark ran at wave 8): every unrequested file at the
cartridge's root is removed now.

Builds are memory-capped: thirteen map conversions (~800 MB each) beside
a 16-job LTO link froze a 15 GB machine. Map conversions run three at a
time (a ninja pool), the link with four jobs, and `devtools/build.sh` runs
ninja inside `systemd-run --user --scope -p MemoryMax=9G` (the test
scripts cap ares at 4 GB): call it, not ninja.

8 October, a deep optimisation round (`devtools/bench-set.sh`: eight
scenes, scripted players; ares, fps before -> after):

| scene | 1 player | 2 players | 4 players |
|---|---|---|---|
| Wizard | 29.9 -> 30.0 | 26.2 -> 29.2 | 10.7 -> 18.3 |
| Blood Gulch | 29.8 -> 29.9 | 16.6 -> 26.9 | 8.6 -> 14.0 |
| Firefight, Blood Gulch | 23.4 -> 25.2 | | 5.9 -> 8.4 |

What did it, largest first: the debug flag cache held 16 names and the
rest opened a cartridge file on every call, one of them per object per
view; the RSP command queue's buffers at 2048 words (libdragon patched:
the CPU waited for the RSP mid-frame, now it runs the next ticks while
the RSP draws); link-time optimisation of the game; single precision
square root and trigonometry, fmod(x, 1) as an exact float fraction,
matrix4x3_multiply written out; parked vehicles keep their node matrices,
far Covenant pose every other tick, swaying plants too; the N64 makes no
particles, particle systems, contrails or widgets (nothing draws them;
their point physics cast ~1,500 rays a second in Firefight); the 3-4
player models' first-person weapons from one 64-texel atlas; the unit and
biped vector diagnostics off; HUD quads without rdpq_tex_blit where their
rectangle fits TMEM; tag block element access inline; the drawable
objects gathered once a frame for all views. At 4 players the game's
ticks are the limit now (a biped's movement, biped_update_physics, is
~1 ms of its ~3 ms a tick); Firefight with four players still runs below
real time. -Os and -O3 do not fit: the bigger code leaves too little
heap, -Os crashed.

8 October, the game's sounds: weapons (fire, reload, ready, charge,
overheat), impacts and explosions, footsteps, vehicles (their engines'
loops follow the vehicle's speed), the shields' and health's HUD sounds,
pickups, the announcer, the players' and the Covenant's pain and death
cries, and the Covenant's lines in Firefight (spotting a player, throwing
a grenade, taunts, melee, a player killed, a grunt panicking when its
elite falls). Halo's sound code stays left out: the calls the game makes
into it are `port/n64/game/n64_sound_game.c`'s, after Halo's sound manager
(random permutation not played lately, pitch and gain from the scale,
pitch ranges, skip fractions; the nearest player hears a sound, full
within its minimum distance, then minimum / distance, nothing past its
maximum; panned to the camera's left or right; a class's limits of the
same sound at once and on one object). Sound tags are kept as records of
the N64's own (`tools/n64/sounds.py`; 27 KB for Blood Gulch), the
environment's (ambience, devices, music) left out; each choosable
permutation is decoded from Xbox ADPCM, made mono 22 kHz VADPCM without
Huffman packing (that was undone by the CPU: a quarter of it) and packed
with every map's into one bank, `snd/sounds.bank` (`tools/n64/
soundbank.py`, 12 MB, 1005 sounds; a map's `snd/<map>.ofs` says where its
sounds are), streamed from the cartridge on 12 voices
(`port/n64/src/n64_sound.c`; a new sound takes a free voice or the
cheapest of no higher priority). Firefight's Covenant bring their
dialogue from a30 (`merge.py`: the vocalizations Firefight plays, three
lines each at most; not their animations' foley). The mixer is fed between
ticks and views as well as at the frame's end (150 ms of audio queued).
ROM 57 MB. In ares sound costs ~8% of the CPU (Blood Gulch Firefight,
one player); the heap's low point there is ~90 KB (was ~118).

8 October, sound starts: the main menu plays Halo's title music
(`tools/n64/music.py`: ui.map's title1 looping sound, its chains of Xbox
ADPCM permutations decoded -- 64 samples a 36-byte block, as
`port/linux/src/dsound_sdl.c`; ffmpeg's IMA WAV decoder adds a 65th --
resampled to 22 kHz and compressed to VADPCM wav64 by audioconv64, 5 MB;
`port/n64/src/n64_sound.c`: libdragon's audio and mixer, the start once
then a random loop version after another, streamed from the cartridge;
the mixer fed at every frame's end). Halo's `profile_dump` is renamed on
the N64 (libdragon's audio brings its own).

8 October, Firefight (`port/n64/game/n64_firefight.c`): the title's
FIREFIGHT entry, 1-4 players, a map and a difficulty, against waves of
the campaign's grunts, jackals and elites (imported into every
multiplayer map from a30 by `tools/n64/merge.py`, at their lowest level
of detail, their bitmaps read from a30's file). Their AI is the N64's
own, through `unit_control()`, thinking at 15 Hz (half of them each
tick); they find their way with Halo's own pathfinding
(`source/ai/path*.c`, one search a tick at most, obstacle avoidance off:
it needs the AI's object records). Units must be kept active
(`object_set_automatic_deactivation`) and actively controlled
(`unit_set_actively_controlled`), or Halo freezes them out of sight and
resets their throttles. Shared lives, waves of five a set, results on
the menu. ares: Wizard ~28 fps with one player, Blood Gulch ~21, Wizard
four players ~9 (each Covenant biped costs ~3.4 ms of a tick, like a
player).

8 October, performance: the views see 55 world units (one player), 45
(two) or 32 (three or four) into fog of the map's own colour (its sky's
outdoor fog), nothing wholly past it (left out by distance); three or
four players get lighter levels
(`<map>-low.lvl`, no lightmap splitting) and models (`<map>-low.mdl`: the
objects at half their triangles, first-person views at 350); the level's
visible batches are drawn a material at a time and the objects' batches
sorted by mode and texture (switching between rdpq's and tiny3d's RSP code
was costly); each level material's mode and texture are recorded once (the
CPU worked them out every change, ~7 ms a frame with four views); objects
whose node matrices cannot have changed keep them (scenery too: half of
Blood Gulch's tick); with three or four views each view's HUD code runs
every fourth frame. In ares: Wizard 30 / 27 / 11 fps with 1 / 2 / 4
players, Blood Gulch 30 / 18 / 9 (were 30 / 19 / 8 and 27 / 12 / 6). Four
players also crashed in the motion sensor (a decompiled array sized for
two players), fixed.

Distant terrain flashing in and out (worse on big maps) was a race: the
viewports were tiny3d's unbuffered kind, one copy of the camera and
projection matrices overwritten by every attach, and once the CPU ran
ahead of the RSP the world's later batches and objects were drawn with the
first-person weapon's (16-unit far plane). They are buffered now (two
attaches a frame for each frame in flight). The frustum's far plane is
also made from the eye and its direction: tiny3d's, the difference of two
projection rows equal to four digits, moved about in floats. Measuring:
`debug_spin_period=<ticks>`, `debug_far`, `debug_no_pvs`,
`debug_fog_test=<1-5>`.

8 October: the menu and all 13 multiplayer maps (Battle Creek, Sidewinder,
Damnation, Rat Race, Prisoner, Hang 'em High, Chill Out, Derelict, Boarding
Action, Blood Gulch, Wizard, Chiron TL-34, Longest) in Slayer, Team Slayer,
CTF, King of the Hill, Team King, Oddball, Team Oddball, Race and Team
Race; 10-25 fps in ares depending on the map and the view. Chunky mode
draws each view's 3D at half width and doubles its pixels in place before
the HUD (the RDP's fill halves; ares hardly charges for fill, so there it
looks chunkier more than it runs faster).

Wizard, 2 players: level, players, weapons, first-person view, Halo's own
HUD (shields, health, ammo, motion sensor, crosshairs, messages). 13-20 fps
in ares; a game tick costs ~16 ms. The HUD's own code costs ~7 ms a view
when it runs (every other frame per view). Text uses libdragon's debug font,
not Halo's.
