# Optional PC expansion integration

This source contribution adds a separate, opt-in multiplayer cartridge while
preserving the original Xbox and campaign build routes. It requires the user's
own input files. It supplies no retail maps, extracted game assets, ROM or SDK
binaries. Start with the repository [build guide](../../README.md); this page
describes the expansion-specific choices and review boundaries.

## Three separate milestones

1. **Additions implemented in source:** the optional original-map pickup and
   vehicle routes, campaign planning notes, and focused conversion/runtime fixes
   are present. This does not establish native access, balance or device quality.
2. **PR reviewed and ready:** requires a frozen source reference, exact public
   inclusion/history review, passing relevant regressions, dependency/art notices
   and reviewer disposition of open issues. A source-only PR may explicitly leave
   device qualification open. It must not describe a not-yet-built ROM as tested.
3. **New DEV ready for SD:** requires fresh compatible programs/assets, a valid
   cartridge within 64 MiB, bounded native checks, final hashes and a separate
   version-named staged file. Actual card installation then requires flushed
   copy/readback verification. Console boot/play and save tests remain distinct
   from a successful file copy.

At this documentation snapshot, a fresh cartridge for the latest original-map
and audio/safety changes is not yet qualified. Earlier installed cartridges and
their results are historical baselines, not substitutes for those gates.

## Player-facing behavior

The expansion uses Multiplayer's existing Content screen and menu style.

- WEAPONS **PC PICKUPS** adds optional Flamethrower and PC Fuel Rod world pickups
  to the original thirteen multiplayer maps. Starting equipment follows MAP
  DEFAULT, including the Pistol + AR rule in Pro variants. Other weapon choices
  keep their existing indices and behavior.
- The existing Derelict Flamethrower keeps its authored modes. Its additional
  optional row applies only to CTF, avoiding a simultaneous duplicate.
- The new pickups use native item renewal: 45 seconds for Flamethrower and
  60 seconds for Fuel Rod, with the weapon's existing ammunition and interaction
  systems. No separate pickup respawn manager is introduced.
- On Blood Gulch, VEHICLES **ALL MAP** and **ALL + ROCKET** enable two additional
  Rocket Hogs. MAP DEFAULT and ROCKET HOGS retain their previous populations and
  replacement behavior. Original vehicles are not moved or removed.
- Blood Gulch, Sidewinder and Ice Fields gain a Banshee near each team's base
  under ALL MAP and ALL + ROCKET. Missing vertical aircraft limits are derived
  from the supplied BSP and pads; these are native soft height limits. See
  [Banshee details](docs/pc-expansion/banshees.md) for limitations and device checks.
  Sidewinder ground-vehicle additions remain deferred. Compact maps get no forced
  vehicles; Death Island, Gephyrophobia and Infinity keep their authored aircraft.

The carried expansion also contains optional controls, bots, team setup,
modifiers and effects. Read the in-game options rather than assuming a new mode
changes the default. High-load bot combinations remain Experimental; available
content is not a guarantee of acceptable frame rate or physical stability.

## Input and build boundaries

Xbox inputs provide the original thirteen maps and base game/menu/audio data.
PC inputs provide Danger Canyon, Death Island, Gephyrophobia, Ice Fields,
Infinity, Timberland and their UI thumbnails. These sources remain explicit and
separate; merely placing a sibling folder beside the repository does not enable
the expansion.

PC source selection accepts an installed retail Maps directory, its game parent
or a supported loose-map ISO9660 disc. It does not launch media installers or
scripts. A CAB/MSI installer disc requires a separate normal installation before
selecting its installed files. PC Custom Edition and Xbox caches are rejected
in the PC field.

Use Python 3.11 or later. Follow the repository's toolchain setup for libdragon,
tiny3d, the MIPS compiler and Ninja. Supply matching Invader definitions, FourCC
header and P8 palette. Do not mix older programs with the current MP32/CP16 map
formats or the N64F shared-effect reader.

For an explicit developer build after preparing local normalized Xbox-v5 PC
inputs and the extracted menu bitmap:

```sh
python3 tools/n64_build.py --pc-expansion \
  --pc-maps /path/to/normalized-xbox-v5/maps \
  --pc-ui-bitmap /path/to/mp_map_grafix.bitmap
ninja -f build/n64/build.ninja pc-expansion
```

The default invocation without `--pc-expansion` retains the original build
route. It does not require PC inputs. Shared decoder and safety fixes still
apply wherever their common code is used; separate variants are not a claim of
byte-identical output across changed converter versions.

The SDK stability source patch is in
[`devtools/pc-expansion-libdragon-stability.patch`](devtools/pc-expansion-libdragon-stability.patch).
Apply the existing RSPQ patch first, then this patch, to libdragon base
`39d0d6096130836a65710da6730290701d060272`, and rebuild into an isolated SDK.
It covers pending-restoration buffer growth, display VI ownership/zeroed surface
descriptors and the missing external-inline emission declaration. Do not replace
the user's working SDK or bundle compiled dependencies in the source PR.

## ROM Builder route

The GUI remains the existing application:

```powershell
python -m tools.n64.rombuilder_main
```

Tick **PC expansion (optional)** and supply the explicit PC source and trusted
conversion-tools folder. The separate output is `halo-pc-expansion.z64`.
The Xbox-only outputs remain `halo-n64.z64` and `halo-campaign.z64`.

The command-line equivalent is:

```powershell
python -m tools.n64.rombuilder "D:\OwnedHaloXbox\maps" -o "D:\HaloROMs" --pc-expansion --pc-source "D:\OwnedHaloPC\Maps" --pc-tools "D:\TrustedHaloConversionTools"
```

These are placeholder user-owned locations, not supplied assets. Fresh matching
MIPS programs and native tools must already be prepared for a source run; the
GUI does not replace that developer step. See
[`tools/n64/ROMBUILDER_PC.md`](../../tools/n64/ROMBUILDER_PC.md) for exact tools,
packaging options, input layouts and cleanup behavior.

The PC normalization recipe uses lossless audio intermediates rather than the
unsafe external ADPCM encoding path. It does not add or distribute a modified
external encoder. New normalization requires fresh compatible cache inputs;
older normalized files are not acceptance evidence for this recipe.

Final map compression is lossless and verified by decoding and comparison.
Shared FX pooling reuses only exact record bodies and preserves logical record
indices. A pooled filesystem requires the matching reader. ROM Builder checks
the 64 MiB limit and preserves an existing output when its new candidate fails.
Input files are read-only; generated staging and receipts are separate.

## Review entry points

| Area | Source entry points | Focused checks |
| --- | --- | --- |
| Optional item/vehicle policy | `port/n64/src/n64_pc_content.h`, `port/n64/game/n64_game.c`, `source/game/game_engine.c`, `source/objects/objects.c` | Native gates and PC vehicle mode behavior |
| Additive conversion | `tools/n64/pc_additions.py`, `pc_placements.py`, `pc_placement_geometry.py` | Original rows/references preserved; repeat markers rejected; conservative bounds failures |
| Explicit PC inputs | `tools/n64/pc_input.py`, `pc_prepare.py`, `rombuilder.py`, `rombuilder_gui.py` | Hostile input, normalization failures, opt-in state and output separation |
| Cache and collision bounds | `tools/n64/xboxmap.py`, `collision.py` | Complete bounded inflation; reject malformed/cyclic BSP paths |
| Xbox audio | `tools/n64/sounds.py`, `music.py` | Correct block predictor/nibble handling, framing and playback duration |
| Cartridge formats | `tools/n64/fx_pool.py`, `port/n64/src/n64_fx_format.h`, `n64_files.c` | Logical index/bank bounds, exact sharing and linked ABI contracts |
| Public documentation | `docs/pc-expansion/README.md`, `campaign-planning.md`, `PROVENANCE.md` | User flow, thirty future campaign decisions and notices; no retail dumps |

All source entry points in the table are repository paths or siblings of the
named full path. Focused portable test groups live in `tests/`; additional build
graph and mode fixtures are in `tools/n64/test_*.py`.

Example targeted source checks:

```sh
python3 -m unittest discover -s tests -p 'test_n64_pc_additions.py'
python3 -m unittest discover -s tests -p 'test_n64_xboxmap_security.py'
python3 -m unittest discover -s tests -p 'test_n64_collision_bounds.py'
python3 -m unittest discover -s tests -p 'test_n64_xbox_adpcm.py'
python3 -m unittest discover -s tests -p 'test_n64_pc_prepare.py'
python3 -m unittest discover -s tests -p 'test_n64_rombuilder_pc.py'
```

These commands describe relevant test routes. A passing host fixture is not a
native cartridge, target compiler, hardware, playback-audibility or clean-machine
installer result. Record the exact source, test selection and outcomes when
preparing a release.

## What existing evidence does and does not establish

The earlier `33247b81…` PC cartridge predates this original-map placement and
audio/safety delta. Its bounded Timberland emulator samples and installed copy
do not establish that this new candidate builds, fits, spawns items correctly or
works on a device. The separate vanilla cartridge is also its own artifact.

For the latest candidate, acceptance still needs fresh conversion/link/package
checks, representative native pickup/renewal and vehicle behavior, multiplayer
interaction, memory/resource pressure and sustained play. A collision box or
clear launch column does not establish fair routes or flight containment.

Device testing must separately check cold boot, play, save/power-cycle behavior
and reported intermittent black boots. Keep every old ROM/save intact and use a
new output/version name for testing. Full-load bot performance and physical
stability remain qualified separately from file preparation.

Campaign proposals cover all ten missions but implement none of their new
placements. Source review does not establish actual Wi-Fi preview, matchmaking
or online multiplayer. No all-bugs-free, universal OOM, legal-certification or
whole-project-completion claim is made.

## Public source and notices

See [`docs/pc-expansion/PROVENANCE.md`](docs/pc-expansion/PROVENANCE.md) for
original generated paint-art provenance and dependency notes. Preserve the
repository's existing license/credits and third-party notices. CC0 does not
clear Halo retail content or trademarks.

The public contribution excludes map/ISO data, extracted assets, authored
coordinate/palette/script dumps, generated ROMs, saves, emulator captures,
SDK/tool binaries and private input/audit locations. Detailed private campaign
reference is retained by its owner; the public campaign page is original
landmark-relative future planning only.
