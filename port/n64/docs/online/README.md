# Halo 64 Online

Online play for the Nintendo 64, with the players of ChupathingyCE
(v0.7.5d: network version 29, wire `chupa-29a`) on the computers: their
game list (halo.milenko.org), their games and their dedicated servers. The
N64 runs a cartridge of its own, `halo-online.z64`; a computer beside it
runs **Halo 64 Link**, which plays the real game for it; a flashcart's USB
cable joins the two.

    N64 (halo-online.z64)  --USB-->  Halo 64 Link (computer)  --UDP-->  ChupathingyCE games
      plays its own player,           the game itself, headless            halo.milenko.org
      draws the rest

**State (October 2026).** In ares with an emulated cart USB port it lists
the internet's and the local network's games, joins them and plays them:
the player's own unit moved, aimed and fired on the N64 at once and
corrected by the Link; the other players, vehicles, weapons and
projectiles; their effects, sounds and the announcer; hits taken; aim
assist (the Link's autoaim on the shots, its magnetism on the N64's aim);
the HUD's crosshair target names, state message, goals and motion sensor
blips; the
kill feed; the lobby; the scoreboard; the player's name. Not yet: vehicles
driven by the N64's player (the view is the Link's; no vehicle HUD),
looping sounds (engines), the pickup prompt, the final scoreboard, hosting
from the N64. The SummerCart64 bridge has been tested only against a
simulated cart, not on a real one yet.

## What you need

| What | Where it comes from |
|---|---|
| An N64 with the Expansion Pak | |
| The online cartridge, `halo-online.z64` | built from this repository, from your own Xbox disc's maps (below, step 1) |
| A computer for Halo 64 Link: Linux on x86-64, or Windows 10/11 with WSL 2 | |
| Halo 64 Link | [JackTulli/halo64-link](https://github.com/JackTulli/halo64-link), built from its sources (step 2) |
| The **same** Xbox maps: `ui.map` and the 13 multiplayer maps | the ones the cartridge was converted from (`build/xbox/disc/maps`): tag `i` of an Xbox map is tag `i` of its converted image (`tools/n64/repack.py`), and the Link names everything by tag index |
| The cable | a **SummerCart64** (firmware 2.x) and its USB cable, with the bridge (halo64-link's `link/sc64/`); or, without a flashcart, **ares** built with halo64-link's `link/ares/ares-usb.patch` |
| An internet connection | for the internet's games; a test game on one computer needs none |

## The easy way: the ROM Builder (Windows)

Halo 64's ROM Builder (this repository's releases) makes the online
cartridge too: tick **Online**. From your Xbox disc it writes
`halo-online.z64` and, beside it, a `Halo64Online` folder with everything
else a Windows PC with a SummerCart64 needs: Halo 64 Link and a test server
(static Linux programs that run in WSL 2: nothing to install in Ubuntu),
the maps the Link plays (the very ones the cartridge was made from), the
SummerCart64 bridge, launchers and the licences. Then:

1. Put `halo-online.z64` on the SummerCart64's SD card and start it from
   the flashcart menu (save type EEPROM 4 kbit, which its header asks for).
2. Plug the cart's USB port into the PC and close sc64deployer and
   UNFLoader.
3. Run `Halo64Online\play-sc64.bat` (or `play-sc64-local.bat` for a test
   game on the PC alone, a local server and a bot).

The folder's `README.txt` is the player's guide, with what to do when
something goes wrong. It holds your maps: keep it to yourself.

## Building

### 1. The cartridge

Build this repository first as the main README says ("From the sources":
the toolchain, libdragon, tiny3d, Invader, and the maps in
`build/xbox/disc/maps`), then the online cartridge, which is not built by
default:

```sh
cd ~/halo-64
python3 tools/n64_build.py
ninja -f build/n64/build.ninja online     # build/n64/halo-online.z64
```

It is its own build of the game (port.json "online", `HALO_N64_ONLINE`)
with the multiplayer cartridge's maps, so once the multiplayer cartridge has
been built only the program is compiled again.

### 2. Halo 64 Link

On the same Linux, or in WSL 2's Ubuntu on Windows (halo64-link's
`link/README.md` has the details):

```sh
sudo apt install git python3 ninja-build clang lld gcc-multilib libc6-dev-i386
git clone https://github.com/JackTulli/halo64-link ~/halo64-link
cd ~/halo64-link
python3 configure.py --release --pgo=off --lto=off
ninja link-x86      # build/link-x86/halo64-link: the Link
ninja server-x86    # build/server-x86/chupathingyce-server: a local test game's server
```

It is the 32-bit build of ChupathingyCE (the one that plays the Xbox's
maps); the first configure downloads SDL's headers once.

### 3. The cable

- **SummerCart64**: the bridge, `link/sc64/sc64_bridge.py` in halo64-link,
  runs on Windows with Python 3 alone, or made a program with PyInstaller
  (`py -3 -m PyInstaller --onefile --console --name sc64_bridge
  link\sc64\sc64_bridge.py`), so the PC needs no Python.
- **ares, no flashcart**: build ares (commit `9408cb43`) with
  `link/ares/ares-usb.patch` (halo64-link's `link/ares/README.md`): an
  emulated EverDrive USB port wired to a TCP socket.

## Playing

### A SummerCart64 on Windows: the kit

halo64-link's `link/sc64/make-kit.sh` packs everything into one folder, the
same one the ROM Builder writes: the cartridge, the Link and the test server
(static, or with the 32-bit libraries they need: Ubuntu needs nothing
installed), the maps, the bridge and launchers:

```sh
cd ~/halo64-link
link/sc64/make-kit.sh --rom ~/halo-64/build/n64/halo-online.z64 \
    --maps ~/halo-64/build/xbox/disc/maps --out ~/Halo64Online --zip \
    [--bridge-exe /mnt/c/<folder>/dist/sc64_bridge.exe]
```

On the PC: unpack it, put `halo-online.z64` on the SummerCart64's SD card
(save type EEPROM 4 kbit, which its header asks for), start it from the
flashcart menu, plug in the cart's USB port and run `play-sc64.bat`
(`play-sc64-local.bat`: a test game on the PC alone, a local server and a
bot). It starts the Link in WSL, then the bridge; closing the bridge stops
both. The kit's `README.txt` is the player's guide, with what to do when
something goes wrong. The kit holds Halo's maps and a cartridge made from
them: keep it to yourself.

### By hand

1. Start the Link waiting for the cable on port 6466, for the internet's
   games (halo64-link's `link/README.md`, "Playing on the N64", gives the
   whole command: a data folder with `maps/`, `HALO_NET_ONLINE=true`,
   `HALO_LINK_CABLE=listen:6466`, and what a player's machine would publish
   or report turned off), or for a test game on one computer
   (`link/run-local-server.sh` and `link/run-local.sh`).
2. Connect the N64:
   - SummerCart64: the cartridge on the cart, the USB cable to the PC,
     sc64deployer and UNFLoader closed, then
     `py -3 sc64_bridge.py --link 127.0.0.1:6466` (a Link in WSL 2 is
     reached at 127.0.0.1);
   - ares: `ARES_N64_USB=connect:127.0.0.1:6466 ares --system "Nintendo 64"
     --setting Nintendo64/ExpansionPak=true halo-online.z64`.

### On the N64

The title's **ONLINE** shows whether the Link answers, then the games it
finds: the local network's, then the internet's (games on maps the
cartridge lacks are marked NOT ON CART). **R** sets your player name (kept
on the cartridge), **Z** asks for the list again, **A** joins a game (its
lobby shows the players until the game starts; **B** there leaves it),
**B** goes back to the title. In a game the
controls are the cartridge's (the main README's "Controls"); **START**
shows the players and the scores (the game goes on; MAIN MENU leaves it),
and **START and Z** together leave at once. After a game the cartridge is
back on ONLINE.

## Why the computer does the work

ChupathingyCE's native builds play the "distributed" netcode
(`port/linux/NETCODE.md` in its sources): every machine ticks the whole
game at 30 Hz on its own clock, predicts its own players, drives the others
from the input the host relays, and the host corrects them. A machine in
such a game simulates every unit, vehicle and projectile of a game of up to
128 players (PC hosts), with the computer builds' pools. The N64 holds 16 at
most, simulates a few bipeds a tick at 30 Hz, and has 8 MB; it can't be a
machine of that game.

The N64 needs a computer beside it anyway: it reaches the network through
its flashcart's USB port, as buu342's
[N64-NetLib](https://github.com/buu342/N64-NetLib) does (UNFLoader's USB
library on the N64, a program on the computer at the other end of the
cable). So the computer is the machine: **Halo 64 Link** is ChupathingyCE
v0.7.5d (CC0) built without a window or sound, as its dedicated server is
(`HALO_SERVER`, `server/platform/sdl_headless.c`), with the N64's
controllers for its players. It lists and joins games exactly as
ChupathingyCE does, so it is compatible by construction. After each tick it
sends the N64 what to draw; the N64 draws it with the renderer it has and
sends its controllers back.

Only the player's own unit is the N64's to play: drawing the Link's view of
it lagged by the whole round trip through the cable (the controller read,
the Link's tick, the frame back). So the cartridge plays a game of its own
on the same map -- slayer or team slayer with its player alone in it, no
bots, vehicles or map weapons -- in which that unit moves, aims and fires at
once, with its first-person weapon, sounds, effects and the game's own HUD,
as in any game of the cartridge. Each tick's control (facing, throttle,
trigger, buttons, the weapon, grenade and zoom wanted: what
`player_control.c` made of the controller) goes to the Link, whose player
plays exactly that control in the real game; the Link's state of the unit
comes back with each frame and corrects the N64's
(`port/n64/game/n64_online_game.c`):

- position: the Link's for the N64 tick it last played, against where the
  N64's unit was after that same tick (a history of the ticks): a small
  difference is taken off a part a frame, a large one at once;
- health and shields are the Link's (the local unit takes no damage);
- weapons, rounds and grenades become the Link's when they have differed
  for a few frames (a pickup there; rounds the local firing hasn't heard);
- the unit dies when the Link's does and spawns where the Link's does;
- in a vehicle the view is the Link's, and the unit waits where the Link's
  sits.

Everything else -- other players, vehicles, weapons on the ground, other
players' projectiles -- is drawn as the Link sends it, and what happens in
the Link's game (effects, sounds, the announcer, hits taken, HUD messages,
the HUD's view of the game) is made again in the N64's.

## The parts

### The online cartridge (`halo-online.z64`)

A cartridge of its own (port.json "online", `HALO_N64_ONLINE`), built from
the same tree as the others, with the multiplayer cartridge's maps.

- Menu: ONLINE (the game list the Link fetches, the lobby, the player's
  name), CONTROLS, SETTINGS.
- In a game: the cartridge's own game of the map (above); each frame the
  Link's latest frame's objects are drawn into its views
  (`port/n64/src/n64_online.c`), and its events replayed
  (`port/n64/game/n64_online_game.c`). START shows the Link's scores; START
  and Z together leave.
- Control: each tick, CONTROL (the control the local unit played), to the
  Link; each drawn frame an INPUT, the pace of the Link's frames.

### Halo 64 Link (the computer)

Its source is [JackTulli/halo64-link](https://github.com/JackTulli/halo64-link)
(how to build and run it: its `link/README.md`). ChupathingyCE v0.7.5d with:

- the dedicated server's headless platform (no window, no sound);
- `link_input.c` in place of the server's input: the N64's controllers as
  its local players;
- `link_export.c`: after each tick, the frame packet and the events;
- `link_control.c`: the N64's control of each tick, played as the Link's
  player's;
- `link_director.c`: the N64's menu requests (list, join, leave, name), the
  lobby and the scores;
- `link_cable.c`: the cable, a TCP socket: ares' emulated port connects to
  it, or the SummerCart64 bridge does.

### The cable

UNFLoader's USB packets (libdragon `usb.c` on the N64), one datatype of the
game's own (0x48), carrying the link packets below. The Link sends at most
one frame a tick (30 a second, a few KB each: well within USB's speed). No
emulator has a flashcart's USB port (ares has only the IS-Viewer's one-way
text), so halo64-link's `link/ares/` gives ares one whose bytes go to and
come from a TCP socket; a real SummerCart64 is reached the same way, through
the bridge (`link/sc64/`), so the Link can't tell the two apart.

## The link protocol (version 2)

Header: magic `"H6"`, version (1), type (1), length (2), sequence (2),
big-endian. `port/n64/src/n64_link.h` and the Link's `link_export.c` give
the layouts.

N64 to Link: HELLO (protocol version, build), INPUT (a frame drawn; the
pace of the Link's frames), LIST, JOIN (a listed game's id), HOST (later),
LEAVE, CONTROL (each tick: the control the local unit played, for the
Link's to play), NAME (the player's name).

Link to N64: WELCOME, STATUS (the menu's line), GAMES (id, players, most,
name, map, game type), LOAD (map, game type, teams, the player's team),
FRAME (the tick; per local player its camera, HUD and own unit: the N64
tick last played, alive, in a vehicle, position, velocity, facing, the
respawn's ticks, team, weapons and rounds), OBJECTS (everything else to
draw: model, tint, regions, bounding sphere, node transforms), FRAME_END,
ENDED, MESSAGE (a HUD message: kills, joins, scores), SCORES (the lobby or
game: teams' scores, each player's name, score, kills, deaths, team; once a
second), EFFECTS (effects begun: weapons fired, impacts, explosions, with
their sounds), SOUNDS (sounds that are no effect's, and the announcer),
DAMAGE (a hit the player takes, and from where), HUD (each frame: the
autoaim level, the crosshair target's name, the state message, the goals,
the motion sensor's blips, and the aim's magnetism with its target's motion
across the view, which the N64's look plays as Halo's magnetism does: the
targets are the Link's).

## Stages

1. ares with the emulated USB port; an echo ROM to prove it: done.
2. The Link: ChupathingyCE v0.7.5d headless, joining a local dedicated
   server: done.
3. The online cartridge: the menu, the game list, drawing frame packets,
   sending input: done.
4. The player's own unit played on the N64 (its first-person weapon,
   HUD, sounds and effects), corrected by the Link's; other players'
   effects and sounds, the scoreboard, the lobby, the HUD's view: done.
   Vehicles, looping sounds, the pickup prompt, the final scoreboard: next.
5. Internet games: halo.milenko.org's list and its invites: done; hosting
   from the N64: later.
6. A real flashcart: the SummerCart64 bridge and the Windows kit: made, not
   yet tried on a cart.
