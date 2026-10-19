# Halo 64 Online (design, in progress)

Online play for the Nintendo 64, with the players of ChupathingyCE
(v0.7.5d: network version 29, wire `chupa-29a`) on the computers: their
game list (halo.milenko.org), their games and their dedicated servers.

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
controllers for its players. It lists, joins and hosts games exactly as
ChupathingyCE does, so it is compatible by construction. After each tick it
sends the N64 what to draw; the N64 draws it with the renderer it has and
sends its controllers back.

    N64 (halo-online.z64)  --USB-->  Halo 64 Link (computer)  --UDP-->  ChupathingyCE games
      plays its own player,           the game itself, headless            halo.milenko.org
      draws the rest

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
players' projectiles -- is drawn as the Link sends it.

## The parts

### The online cartridge (`halo-online.z64`)

A cartridge of its own (port.json "online", `HALO_N64_ONLINE`), built from
the same tree as the others, with the multiplayer cartridge's maps
(converted from the same Xbox maps the Link plays: tag `i` of an Xbox map is
tag `i` of its converted image, `tools/n64/repack.py`, so the Link names
everything by tag index).

- Menu: ONLINE (the game list the Link fetches: the local network's games
  and the internet's, halo.milenko.org's, games on maps the cartridge lacks
  marked NOT ON CART; join; hosting later), CONTROLS, SETTINGS.
- In a game: the cartridge's own game of the map (above); each frame the
  Link's latest frame's objects are drawn into its views
  (`port/n64/src/n64_online.c`). Start and Z together leave.
- Control: each tick, CONTROL (the control the local unit played), to the
  Link; each drawn frame an INPUT, the pace of the Link's frames.

### Halo 64 Link (the computer)

Its source is [JackTulli/halo64-link](https://github.com/JackTulli/halo64-link)
(how to build and run it: its `link/README.md`). ChupathingyCE v0.7.5d with:

- the dedicated server's headless platform (no window, no sound);
- `link_input.c` in place of the server's input: the N64's controllers as
  its local players (up to four: split screen);
- `link_export.c`: after each tick, the frame packet;
- `link_control.c`: the N64's control of each tick, played as the Link's
  player's;
- `link_director.c`: the N64's menu requests (list, join, leave);
- `link_cable.c`: the cable: for an emulator a TCP socket; a flashcart's
  USB (UNFLoader's device layer: 64drive, EverDrive, SummerCart64) later.

### The cable

UNFLoader's USB packets (libdragon `usb.c` on the N64), one datatype of the
game's own, carrying the link packets below. The Link sends at most one
frame packet a tick (30 a second, a few KB each: well within USB's speed).

## The link protocol (version 2)

Header: magic `"H6"`, version (1), type (1), length (2), sequence (2),
big-endian. `port/n64/src/n64_link.h` and the Link's `link_export.c` give
the layouts.

N64 to Link: HELLO (protocol version, build), INPUT (a frame drawn; the
pace of the Link's frames), CONTROL (each tick: the control the local unit
played, for the Link's to play), LIST, JOIN (a listed game's id), HOST
(later), LEAVE.

Link to N64: WELCOME, STATUS (the menu's line), GAMES (id, players, most,
name, map, game type), LOAD (map, game type, teams, the player's team),
FRAME (the tick; per local player its camera, HUD and own unit: the N64
tick last played, alive, in a vehicle, position, velocity, facing, the
respawn's ticks, team, weapons and rounds), OBJECTS (everything else to
draw: model, tint, regions, bounding sphere, node transforms), FRAME_END,
ENDED.

Still to come: other players' sounds and effects, the scoreboard, the
motion sensor's blips for other players, hosting from the N64.

## Testing without a flashcart

No emulator has a flashcart's USB port (ares has only the IS-Viewer's
one-way text). ares is given one: an emulated cart USB port whose bytes go
to and come from a TCP socket on the computer (a small patch:
halo64-link's `link/ares/`), and the Link speaks to that socket as it would
to the cable. A real SummerCart64 is reached the same way, through a small
bridge on the computer (halo64-link's `link/sc64/`, not yet tried on a
cart).
The whole chain then runs on one computer:

    ares (patched) + halo-online.z64  <--TCP-->  Halo 64 Link  <--UDP-->  ChupathingyCE dedicated server (Xbox maps)

## Stages

1. ares with the emulated USB port; an echo ROM to prove it.
2. The Link: ChupathingyCE v0.7.5d headless, joining a local dedicated
   server, logging what it would send.
3. The online cartridge: the menu, the game list, drawing frame packets,
   sending input.
4. The player's own unit played on the N64 (its first-person weapon,
   HUD, sounds and effects), corrected by the Link's: done. Other players'
   sounds and effects, the scoreboard: next.
5. Internet games: halo.milenko.org's list and its invites: done; hosting
   from the N64: later.
