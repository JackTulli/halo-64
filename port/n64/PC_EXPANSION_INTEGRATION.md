# PC expansion integration on Halo 64 v0.2.0

**PC expansion BUILT / FROZEN: STATIC PASS and bounded native PASS; SD and hardware pending.**
The 19-map expansion ROM is 65,945,600 bytes, with 1,163,264 bytes of headroom
under 64 MiB. The private installer's packaged worker checks also pass within
the boundaries below. One ordinary menu boot and a separate 172.221-second
Timberland run with one human and three bots now pass bounded native checks.
Observed FPS remains below the 20 FPS target; no physical stability is established.
The separate vanilla build has one bounded menu boot; that evidence
does not transfer to the expansion. This document records merge decisions,
artifact identities and remaining review gates. No new upstream-based ROM is
installed on a flashcart. Preserve the existing main build, separately named
development build and saves.

## Pinned history and recovery

| Role | Exact ref / identity |
|---|---|
| Built code checkpoint | `d330422cdf9a3146f02613c66dabd77a6273689a` |
| Installer parity checkpoint | `e2ab7d013358bb45d608002af2979c4816d0cc3a` |
| Initial integration checkpoint | `51e405d047e44530b7ba9e1143075d21a0135e2d` |
| Integration base | Upstream `v0.2.0`, `a3f98e6484b488c1ef8638a99fba0a2b33adf28b` |
| Earlier release ancestor | `v0.1.0`, `cc1e0913c1ef3cebab3259519bcaba64c58dc1f5` |
| Preserved PC expansion | `94f3e24cd6dd6ec45ce1b0a5ddff9f0849e99348`, branch `pc-expansion` |
| Preserved annotated tag | `backup/pc-expansion-pre-upstream-20261015-1952` (peels to the preserved commit) |
| Original expansion base | `264da177203b82cdff7b8a38645af4e52aabb229` |
| Fresh vanilla checkout | `upstream-20261015/vanilla-a3f98e6` in the enclosing project |
| Integration checkout / branch | `upstream-20261015/pc-integration-a3f98e6` / `pc-expansion-upstream-a3f98e6` |

The enclosing project's recovery archive is
`version-control/pre-upstream-20261015-1952/`. Use
**`pc-expansion-source-complete.bundle`**, SHA-256
`5c3d28465e17e12ab84149a9d654b9d0e46869069b3ce5630d11a6d17f7268d0`.
`restore-verification.json` records an actual standalone offline clone and
`git fsck --full`, both exit 0, restoring the preserved commit. The earlier
`pc-expansion-source.bundle` passed `git bundle verify` but failed standalone
clone because the source was shallow. It is retained as superseded evidence and
must not be described or used as the verified recovery bundle.

Recovery example (PowerShell; choose a new, empty destination):

```powershell
$projectRoot = 'C:\path\to\Halo64-PC-Expansion'
$bundle = Join-Path $projectRoot 'version-control\pre-upstream-20261015-1952\pc-expansion-source-complete.bundle'
$restorePath = 'C:\path\to\a-new-recovery-checkout'
Get-FileHash -Algorithm SHA256 -LiteralPath $bundle
git bundle verify "$bundle"
git clone --no-checkout --branch pc-expansion "$bundle" "$restorePath"
git -C "$restorePath" rev-parse HEAD
git -C "$restorePath" fsck --full
# When a checked-out copy is wanted:
git -C "$restorePath" switch --detach 94f3e24cd6dd6ec45ce1b0a5ddff9f0849e99348
```

That complete bundle restores the **pre-upstream** preserved commit; it does not
contain the later integration checkpoints. The separate `c5f2954` checkpoint
bundle/restore proofs retain their historical commit scope; they do not imply
that subsequent P8/compression/FX-pool/ROM-guard commits are backed up by the same
bytes. After committing the final guide, root will create the enclosing project's
`upstream-20261015/handoff/pc-expansion-v0.2.0-final-integration.bundle`,
`final-integration-patches/` and `final-integration-verification.json`. These are
planned artifacts until their verification receipt exists. Require an actual
standalone bundle clone plus `git fsck --full`, and a clean v0.2.0-base checkout
with the patch series applied by `git am` and a matching complete source tree.
The final guide commit follows the built code checkpoint without changing game
code; do not substitute a historical bundle's successful test for these gates.

The bundle preserves source history, not ignored dependencies, retail input data,
SDK working changes or generated ROMs. `backup-manifest.json`,
`libdragon-working.patch`, `artifacts/` and `support/` cover the additional
recovery material. The SDK baseline is
`39d0d6096130836a65710da6730290701d060272`; reconcile the saved patch with the
chosen upstream SDK rather than applying it blindly. Keep user-owned Xbox/PC
inputs in their existing private location.

## Cartridge and input boundaries

| Cartridge | Default / input | Map and program boundary |
|---|---|---|
| Xbox multiplayer | Existing default, owned Xbox ISO or maps | Original 13 maps and upstream multiplayer payload |
| Campaign | Separate upstream cartridge and payload | `a30`, campaign UI/checkpoints/difficulty; never the PC multiplayer payload |
| PC expansion | Explicit opt-in plus owned retail Halo PC source | Original 13 plus six PC maps, separate expansion program/filesystem/ROM |

The six additions are Danger Canyon, Death Island, Gephyrophobia, Ice Fields,
Infinity and Timberland. A sibling `timberland.map` must never silently enable
imports in the Xbox or campaign build. The PC option extends the existing builder
cartridge selection and progress/error flow. It is not a new mandatory installer.
The integration branch's Xbox cartridge shares common port repairs/features; it
is not a byte-identical vanilla build. The separate pinned vanilla checkout is
the reference for an untouched upstream build.
Unselected PC support must leave the ordinary Xbox/campaign path working without
PC files or conversion tools.

The implemented high-level interface is:

```sh
# Existing Xbox/campaign path; ordinary defaults stay intact.
python -m tools.n64.rombuilder /path/to/owned/xbox.iso -o /path/to/output
# Explicit additional PC input; creates a separate expansion cartridge.
python -m tools.n64.rombuilder /path/to/owned/xbox.iso -o /path/to/output \
  --pc-expansion --pc-source /path/to/owned/pc/Maps --pc-tools /path/to/trusted/tools
```

These examples document the implemented interface, not an end-to-end successful
build claim. The GUI's optional PC-expansion checkbox exposes PC ISO/maps-folder
selection within the existing screen. Source GUI, CLI and packaging details are in
[ROMBUILDER_PC.md](../../tools/n64/ROMBUILDER_PC.md). A packaged builder needs a separately
rebuilt expansion ELF/SYM payload; reusing the Xbox multiplayer or campaign
program for the expansion is invalid. Default selections must remain Xbox
multiplayer/campaign, with PC expansion off unless explicitly selected. ELF
compression is per cartridge: Xbox multiplayer and campaign use upstream mode 1;
the PC expansion retains the selected direct-load mode 0.

PC source reading is read-only. Supported raw input is retail PC cache version 7
from installed Maps or loose files on a supported plain ISO9660 disc. Packed
installers, unsupported editions/formats and malformed/path-escaping inputs must
fail with a useful message; never execute an installer from user input. The
normalizer uses an explicitly trusted Invader/FFmpeg tools directory, fresh work
space and hashed manifests to derive Xbox-v5 intermediates and the PC UI bitmap.
The conversion handles model/shader forms, HUD scale and embedded audio. No
retail maps, extracted tags, textures, sounds, input ISOs or resulting ROM data
belong in the source/PR. No automatic tool download is implied.

The focused installer fixtures can be repeated from the checkout (they do not
replace real conversion, packaging or runtime checks):

```sh
python -m unittest discover -s tests -p test_n64_pc_input.py -v
python -m unittest discover -s tests -p test_n64_pc_prepare.py -v
python -m unittest discover -s tests -p test_n64_rombuilder_pc.py -v
python -m unittest tools.n64.test_buildgraph tools.n64.test_portable_assets
```

The low-level source build accepts explicitly prepared intermediates:

```sh
python tools/n64_build.py --pc-expansion --pc-maps /path/to/normalized/maps \
  --pc-ui-bitmap /path/to/owned/mp_map_grafix.bitmap
ninja -f build/n64/build.ninja pc-expansion
```

Use the owning pipeline/build lane's final command receipt and tool versions for
reproduction. The graph uses POSIX/MSYS shell commands on Windows, forward-slash
paths and shell quoting for spaces; generation is not an MSVC/PowerShell-only
build. `-mfix4300` is applied consistently to game, platform, musl and link flags.
The Windows graph serializes map conversions to avoid shared-sprite replacement
races. Game optimization remains `-Os`; the separate `platform_optimisation`
setting restores platform `-O2`. The first merged graph accidentally inherited
game `-Os` for the platform; the corrected graph preserves the prior/upstream
`-O2` policy and has an explicit profile regression assertion. No machine-specific `local/` diagnostic helper is a build dependency. Source-interface inspection alone does not prove the GUI package,
normalization or ROM conversion has completed successfully.

Helper comparison found that the old `map13.sprite` was an Xbox UI placeholder,
not the PC Danger Canyon image. The explicit PC bitmap produces its actual PC
thumbnail; its difference from that placeholder is intentional. The graph must
order PC thumbnails after UI scene generation, including UI rebuilds, so a later
placeholder cannot overwrite them. Tested paint-mask output and the 69-material
Wizard/Timberland decal bank are byte-identical to their earlier helper outputs;
this is not whole-map or whole-asset equivalence.

## Converted-map ABI: rebuild data and code together

`tools/n64/convert.py` emits the 64-byte big-endian `H64M` header **version 2**.
The format-kind word at byte 56 distinguishes `MP32` (`0x4d503332`, multiplayer
collision/native cluster-only leaves) from `CP16` (`0x43503136`, compact campaign
collision). `n64_game.c` must reject the wrong version/kind rather than relocating
it into a differently compiled game. Reserved fields remain checked.

Rebuild every affected converted map and each cartridge's executable/payload.
Old version-1 maps, campaign CP16 images and multiplayer MP32 images cannot be
mixed merely because filenames match. Use separate fresh build/work directories;
do not delete legal input data or overwrite the preserved known build. Keep
upstream campaign v5 streamed render chunks distinct from the multiplayer v4
low-memory geometry path. Validate converter/loader contracts for both kinds,
negative wrong-kind/version cases, relocation bounds and the map-home reservation
before calling either cartridge compatible.

## Semantic merge choices and file groups

| Group | Principal files | Behavior to retain / review |
|---|---|---|
| Optional inputs/build | `tools/n64_build.py`, `tools/n64/{rombuilder,rombuilder_gui,package_rombuilder,pc_input,pc_prepare,pc_thumbnails,convert,merge}.py`, `port/n64/port.json` | Explicit PC input, default Xbox 13, optional PC 19, separate campaign/payloads, no bundled retail data; validate per-variant compression and caches |
| Map layout/collision | `tools/n64/{convert,level,repack}.py`, `source/structures/*`, `port/n64/game/n64_game.c` | Version-2 MP32/CP16 agreement and rebuild, upstream campaign compact representation, safe MP relocation/load guards |
| Menu/control flow | `port/n64/src/{n64_menu,n64_input,xdk_input,n64_menu_memory}.c`, `n64.h`, `port/n64/game/n64_golden*`, camera/aim/HUD hooks | Existing Halo menu style, additive options, per-player controls, reticles and non-inverted default |
| Bots and rosters | `port/n64/game/n64_bots.c`, `n64_bot_{names,ghosts}.h`, `source/units/bipeds.c`, `source/objects/objects.c` | Our Halo names/teams/Ghosts plus upstream bot LOD/thought cadence/ground movement; no new Ghost menu |
| Vehicles and content | `port/n64/game/n64_game.c`, `source/game/game_engine.c`, `source/units/vehicles.c`, conversion/import code | Authored Banshee placements and existing ALL MAP selection, PC weapons/vehicles remain additive, campaign policy retained |
| Renderer/assets/VFX | `port/n64/src/{n64_render,n64_hud,n64_particles,n64_paint}.c`, `n64_sniper_ribbon.h`, game-render/HUD hooks and bitmap/model/effect/decal exporters | Upstream ground-colour tiles/Blood Gulch sky/campaign streaming plus our material, pose, colour, normal decals, paintball, Big Head and brief sniper ribbon |
| Allocation/SDK stability | reviewed SDK patches, `source/memory/memory_pool.c`, `source/game/game_engine.c`, `source/physics/breakable_surfaces.c`, map/renderer ownership paths | RDP growth/restoration, VI ownership/partial descriptors, model-release cleanup, map allocation/read guards, gap fallback, eligible expired-item reclaim and N64 flags alignment |

### Menu, controls and content

The merged menu retains upstream campaign pages and reusable map-picture
load/wait/reuse/free ownership. Xbox multiplayer stays 13 maps with its ordinary
weapon choices; expansion adds the six maps and Fuel Rod/Flamethrower loadouts,
and its vehicle row, in the existing flow. Campaign remains `a30`, one human,
difficulty/checkpoints, and skips multiplayer bot/score/content rows.

Controls stay per physical controller, including split-screen pause access.
Retain the upstream presets plus optional Golden Style; changing a scheme must
not silently invert look. Up looks up by default; inversion is explicit opt-in.
Keep each equipped weapon's actual reticle, including Golden Style, and the
stick-move/C-button-aim option. Preserve ordinary impact/blood/scorch/bullet
decals outside Paintball, silhouette paint splats and matching impact colours
inside Paintball, grenade/weapon/impact feedback, slight sniper trail ribbon,
Big Head, weapon textures and the Flamethrower's dragon art. Integration presence
is not visual certification; user-reported weapon, colour, death, turret, wheel,
foliage alpha and pickup regressions remain explicit runtime review cases.

### Bots, teams and Ghosts

Keep the 40 Halo CE profile names and deterministic, unique seeded roster shared
by lobby preview and the actual match. Humans choose Red/Blue with left/right on
their own controller; bots balance/fill the same roster as players move. Exercise
one to four humans and the requested two-humans/two-bots-per-team arrangement.
Retain the ordinary bot choices 0–4 and eight-bot option marked Experimental until
controlled full-lobby evidence meets the user's **at least 20 FPS** threshold.
No inherited upstream optimization is itself evidence of meeting that target.

Preserve upstream near/seen LOD, firing-pose handling, timestamped thoughts,
population/distance burst pauses and bot-only grounded two-tick movement.
The bot owner reconciled first-tick over-advance, stale held pose across maps
and off-phase lost jumps, with focused actual-C control tests. The merged bot
stride is 268 bytes versus the old 248; twelve records add 240 bytes versus
the preserved expansion and match new upstream size. The held movement table
remains 4,096 bytes. Those layout checks do not establish native collision-sweep
equivalence or a frame-rate improvement. Retain our Ghost names,
abilities and FFA colours (Chaotic 6, Kode 12), with native per-life camo chances
10%/25%. Ghost match selection changes to **1/32 per named profile per match**,
not per bot/life, and adds no new option screen. This is not wireless/network AI.

### Banshee and existing vehicle policy

Use the existing multiplayer ALL MAP / ALL+ROCKET selection path. Authored
Banshee placements exist on Death Island (4), Gephyrophobia (4) and Infinity (8),
with authored game-type masks retained. Do not invent placements on the other
16 maps or globally substitute Banshees. The asset, driver seat, type 5 and
two-powered-point flight physics are present; upstream alien-fighter functions
remain the reference. Seat entry, flight, collision, multiplayer and performance
still need native/hardware validation.

An initial direct helper test suggested a campaign vehicle override regression.
The vehicle owner corrected that interpretation: the actual sole wrapper bypasses
remapping without an engine. Do not add a new guard based on the bypassed-wrapper
test. Final full-wrapper actual-C policy tests pass 321 non-N64 and 347
N64/campaign cases (668 total), including campaign bypass. They use bounded
engine/tag models; actual spawning, seat entry and flight remain untested.

### Renderer and stability boundaries

Retain upstream natural-ground tiles, Blood Gulch sky/material and campaign v5
streaming while preserving MP v4's 64 KiB DMA geometry cache. Reconcile our
first-person/player pose/colour/death/Big Head paths, normal decals and reviewed
model-release fallback. The renderer owner source-reviewed tile/model-table allocation, short reads,
malformed data and flying-slot failure paths. The full renderer TU compiles;
56 extracted actual-C fault cases pass and 22,040 real valid strip batches
retain byte-identical output in the host check. Stubbed I/O/GPU/cache services
and host metadata do not qualify actual DragonFS/GPU pressure or campaign chunks. Held sky06 and beam10
proposals are excluded; a math/proposal receipt is not an integration admission.

Old Stability06/07 checks explain the rationale for retained repairs. They do
**not** qualify this new upstream-based code. Even the older flags-alignment
Prisoner run did not exercise destruction writes or prove causal elimination of
all earlier handled exceptions. SDK patch application, archive identity and actual
loaded ELF/ROM payloads require fresh checks against the selected upstream SDK.
The admitted dependency repair and completed static link/freeze are recorded below.

## Validation status and required matrix

This table records the final static freeze and bounded native02 results at source
checkpoint d330422. Broader runtime qualification remains pending. The enclosing audit
handoff map records source/evidence hashes and pending owner receipts. A row's
PASS is limited to its stated scope. MISSING means no accepted evidence for this
new integration, not an invented implementation defect.

| Gate | Current evidence / status | Remaining |
|---|---|---|
| Source recovery | PASS: actual complete-bundle clone and `fsck --full` at preserved commit | Keep archive/manifest correction together |
| Upstream base / checkpoint | v0.2.0 = a3f98e6; current source checkpoint d330422, initial integration 51e405d | Final source/guide review; older backup proofs keep their original checkpoint scope |
| Menu logic and four macro variants | PASS: extracted actual-C tables/controls/EEPROM/team/bot-limit and picture ownership checks; whole menu TU compiled MIPS without warnings | Payloads linked; rendered menus, quick map-picture scrolling and reentry under memory pressure |
| Bot source/control checks | PASS: source/Astra review, roster/LOD/movement actual-C controls and three target TUs compiled; 65,536 seeded rosters tested | Linked payloads ready; native LOD/cadence/collision/pose/ghost/team behavior MISSING |
| Renderer TU / guards | PASS: independent source review, strict-warning full MIPS TU compile, 56 actual-C fault cases and 22,040 valid strip-batch byte comparisons | Linked payloads ready; campaign fixture, actual DragonFS/GPU pressure, runtime/hardware checks MISSING |
| Optional PC input/normalization/GUI | PASS actual normalization and 73 current installer tests, including P8 resource, PC compression, pool routing and ROM publication guard | Private packaged workers PASS; full 19-map installer ROM, rendered packaged GUI, clean-machine, game runtime and hardware qualification MISSING |
| ABI version/kind / authored starts | PASS all 19 new H64M-v2 MP32 conversions and recursive/static validation; all 1,299 authored starts retain identical logical fields; 96 guard cases retained | Static/native-layout evidence is not dynamic respawn/collision clearance; campaign/gameplay acceptance MISSING |
| Banshee selection | PASS authored asset/placement inventory and 668 actual-C wrapper/policy checks | Native placement masks, seat entry, flight/combat and performance MISSING |
| Build graph / helpers | Earlier eleven-test/28-module closure checks retained; exact FX-pool helper/runtime and portable tests admitted | Corrected read-only PC graph admitted at d330422; final ROM static/readback and scoped packaged-worker gates PASS; native checks remain open |
| Program compilation/link and payload isolation | Three matching fresh post-pool program ELF/SYM/stripped payload sets linked, 353 objects each; distinct MP32/CP16 variants | Final PC ROM static freeze/readback and bounded native02 PASS; broader runtime qualification MISSING |
| Runtime | PASS bounded native02: one ordinary menu boot and separate 172.221 s Timberland 1H3B; observed human/bot respawn evidence | All-map/campaign/split/content coverage, menu navigation/reentry, repeated ordinary boots, long-term reliability and physical acceptance remain open |
| Splits/controls | MISSING for new upstream integration | 1/2/4 humans, per-port scheme/inversion/sensitivity/reticles, pause controls, EEPROM round trip |
| Bots/teams | Native02 verifies actual 1-human/3-bot identities and four live units in six sparse snapshots | Bounded full8 result recorded below; team/split, 2 humans + 2 bots per team, 4-human-plus-bot case and Ghost rarity/abilities/FFA colours remain open |
| Content/visuals | MISSING for new upstream integration | PC weapon pickups, Flamethrower dragon/fire, Rocket Hog pose/turret/wheels, deaths, alpha foliage, decals/paint/VFX/ribbon |
| ROM capacity | PASS: final ROM 65,945,600 bytes; 1,163,264 bytes below 64 MiB. Packed DFS 64,469,184 bytes, lossless saving 2,646,342 | Final ROM/header/TOC/load-segment/readback checks PASS; no runtime or performance inference |
| Performance/memory | Bounded Timberland 1H3B: 43 logged FPS windows, 10.2–18.1 FPS, median 15.2; lowest logged free heap 94 KiB | Does not meet 20 FPS. Debugger perturbation; no universal malloc-failure counter/true heap low-water guarantee. Bounded full8 is also below target; worst maps/splits/vehicles/modifiers and hardware remain open; Experimental retained |
| Flashcarts/hardware | MISSING | New exact package readback, SummerCart/EverDrive X7, physical cold boot/repeated gameplay; preserve named saves and main ROM |
| Networking | MISSING / separate research scope | No native M64 Wi-Fi or SummerCart USB matchmaking acceptance from this merge |

The last verified development-card install is the named Stability06 ROM
(FFC5), distinct from upstream integration. Older 19-map and ten-boot evidence
belongs to that executable; the later alignment07's single ordinary boot and
bounded Prisoner run are also older-code evidence. Neither transfers to this
merge. Do not claim blanket performance, hardware stability, all-map visual
quality, networking or 100% project completion.

## Dependency SDK patch and current build progress

The repository now carries
[`devtools/pc-expansion-libdragon-stability.patch`](devtools/pc-expansion-libdragon-stability.patch),
SHA-256 `f19f262580c6358a2acfe411db132a624e289ec4613c4ff3e3f4590d6bcfdc46`.
Use a **fresh libdragon checkout at**
`39d0d6096130836a65710da6730290701d060272`: apply upstream's existing
[`libdragon-rspq-buffer.patch`](devtools/libdragon-rspq-buffer.patch) **first**,
then this expansion patch. The expansion patch deliberately excludes the official
RSPQ change, avoiding a duplicate application. Keep the old and vanilla SDKs and
archives intact; rebuild/install into an isolated SDK destination.

The added patch contains the reviewed pending-restoration RDP buffer-growth fix,
VI reference ownership/zeroed display descriptors, and one missing external-inline
emission declaration for `rdpq_mode_zmode`. Both the old and vanilla SDK archives
lacked that exported definition despite identical headers. Actual `-Os` game code
emits an external call; the `-O2` form inlines it. The repair adds the existing
header function to the SDK's `extern inline` list; it does not introduce new
z-mode semantics or replace the independent platform-optimization correction.

Astra/root admitted isolated `libdragon.a` SHA-256
`2757caf61018a146e7e5ce094fa0c85c784d38df0fbc629e4e8d81248d327aa5`
for the shared integration link. The enclosing project's receipt is
`audit/integration-sdk-a3f98e6-01/primary-link-admission.json` (outside
`upstream-20261015/audit`). Headers and converter-tool bytes remain identical;
only link library/dependency paths use the new isolated SDK. This preserves asset
commands and does not qualify a linked ROM or hardware.

Current progress is a snapshot, not completion:

- Root's combined source suite passes **64 tests**; the frozen installer suite
  records **60 tests** with source/mock native tools, real withdrawn Tk optional
  controls/main-thread handoff and trusted-Python cancellation. The current
  installer suite has 73 tests and private packaged-worker checks pass. Rendered
  packaged GUI and clean-machine acceptance remain open.
- The link-checker has four tests plus an actual **350-native-object** comparison:
  one relative-path invocation and four bounded absolute-path batches yield
  identical empty weak/missing sets. This validates checker batching, not that
  final game programs link successfully.
- The initial links exposed the missing SDK export. After the admitted repair
  and profile correction, all three fresh post-pool ELF/SYM/stripped payload sets
  now link. The final PC ROM is assembled, frozen and independently rechecked;
  bounded ordinary-menu/Timberland native02 checks now pass. Broader runtime
  qualification remains pending.
- Actual frozen PC normalization now completed in its new serial work directory:
  **725 trusted Invader/FFmpeg calls**, 34 HUD scale edits and 39 re-encoded sound
  tags. All six v5 compressed-stream/tag-bounds validators and explicit converter
  preflight checks pass; all nine owned input hashes remain unchanged. The
  345,308-byte PC UI bitmap was extracted and six 256x128 images decoded; the
  packaged PC UI worker later passes all six authored images. Native rendered
  thumbnail acceptance remains open. A schema walk covered
  1,653,242 struct instances and 43,074 dependency edges with zero walk/bounds
  errors. Qualification retains the old identical dropped Soul layout and four
  unused zero Meter fields per map as documented exceptions. New compressed
  outputs differ from old ones, so no byte-identity claim is made. **This receipt qualifies normalization only. Later nineteen-map conversion,
  ROM static freeze and packaged-worker results are separately bound below;
  hardware acceptance remains open.**
- The separate vanilla multiplayer build now completed and froze all 13 maps /
  1,038 assets. One ordinary emulator menu boot with two matching completed frames
  is recorded below. This is not PC expansion, campaign, sustained gameplay or
  physical acceptance.
- All 19 PC maps now converted with H64M-v2 MP32 recursive/static validation.
  The corrected pool graph is admitted at d330422. The final 19-map ROM is
  frozen at 65,945,600 bytes; packaged-worker checks pass separately. Bounded
  native02 results below cover one ordinary boot and one Timberland 1H3B run.

Root owns source curation and subsequent runtime/physical qualification. The
completed static freeze does not qualify gameplay or hardware. No PR, push or
maintainer contact follows from this handoff.

## Current completed repairs: maps, exact FX pooling and installer parity

The current source checkpoint is `d330422cdf9a3146f02613c66dabd77a6273689a`;
installer parity was checkpointed immediately before it at `e2ab7d0`.
Source milestones include the P8 packaging repair (`b5a470d`), PC-only map
compression (`a2ccde5`), exact FX pooling (`a9cc834`), its portable tests
(`f0da099`), and installer pool/ROM-publication guards (`e2ab7d0`). These are source
and focused-test milestones, not a released or hardware-qualified PC ROM.

### Fresh map and spawn data

All **19** new maps are converted as **H64M version 2 / MP32** and pass recursive
serialized-layout validation. The authored-start regression preserves all
**1,299** logical starts, including 470 on the six added maps. Static/native-layout
and lifecycle checks establish no concrete new spawn/respawn problem. They do not
prove full capsule clearance, dynamic obstruction, every pickup/respawn sequence,
or runtime collision-sweep equivalence. No speculative spawn fix was added.
Campaign CP16 remains a distinct program/data ABI; MP data must not be substituted.

### Exact FX record pooling without content reduction

The unpooled DFS is **67,115,526 bytes**. Combined with the pre-pool program/SYM,
the ROM projection was **68,599,808 bytes**, above the 67,108,864-byte cartridge
limit. The admitted repair changes storage of identical FX record bodies; it does
not lower texture quality, omit effects or remove content.

`tools/n64/fx_pool.py` stages a separate packed filesystem. Per-map **N64F version
3** metadata references `fx/records.bank` with an identity CRC; the runtime keeps
legacy N64F version 1/2 support. This FX format version is separate from H64M-v2
map ABI and level-render v4/v5 chunk versions. The verified packed DFS is
**64,469,184 bytes**, saving **2,646,342 bytes**. Evidence preserves all **6,035**
logical FX bodies, all **19** per-map metadata records and all **1,740** non-FX
files. There are 898 unique shared record bodies. The exporter/actual-C reader,
malformed-input checks, target compilation and portable tests have scoped passes;
no rendered/GPU sequence or hardware acceptance follows.

Runtime storage changes are explicit: persistent globals add **8 bytes**;
loader frame size adds **432 bytes** (184→616); there is **no bank-resident heap
allocation** or persistent heap-payload growth. The existing 12-entry cache is
retained. CRC work at map load is unmeasured. This is a ROM-space repair, not an
FPS or hardware-latency claim. The later actual post-pool ROM is 65,945,600 bytes;
its independently checked headroom is 1,163,264 bytes.

An additional actual-C legacy-reader check exported only the real owned campaign
`a30` FX data: all 274 legacy-v2 logical record bodies and DMA padding, index
entries, 4,096 atlas bytes, 128 colour bytes and flame/explosion metadata match.
The harness substitutes SDK file/DMA calls; CRC is not used by legacy v2. This
qualifies legacy reader/cache/metadata compatibility for that fixture, not full
campaign conversion, native timing, rendering or campaign gameplay.

The raw filesystem is pruned **before** pooling. `PACKFS` is a verified output:
never prune, edit or overwrite it after pool verification. The initial Ninja
proposal was held because a real prune-after-pack pass could
mutate verified output. That regression is now corrected and admitted in
`d330422`: the PC DFS rule consumes verified `PACKFS` **read-only**, while the
main/campaign legacy raw-prune path stays unchanged. Existing verified packed
data remains untouched. A different packed output must establish its own
identity rather than modifying the verified tree.

Owner validation records 12 source/unit tests, nine graph checks and nine actual
Ninja incremental scenarios; Astra/root reviewed admission, and root ran seven
source tests. The Ninja scenario sinks are hash writers, not actual SDK ROM
packaging or executable ROMs. Proposal receipts whose statuses still say
"pending admission" retain that earlier historical state; the later root
admission/source checkpoint does not rewrite them.

### Supported source/package flow and publication safeguards

Use the normal optional-PC builder flow described earlier and in
[ROMBUILDER_PC.md](../../tools/n64/ROMBUILDER_PC.md); no machine-specific helper or
private input path is required. The source pipeline is:

1. Read the owned Xbox source and explicitly selected retail PC source.
2. Normalize the PC source in a fresh work area and convert explicit 19-map inputs.
3. Apply the existing lossless `compress_map.compress` worker to **PC** converted
   maps only. Default/campaign installer paths add no new compression calls.
4. Prune the raw PC filesystem, run the exact admitted pool helper into a new
   packed tree, and pass that immutable packed tree to `mkdfs`. Pool manifests
   remain outside DFS. Default/campaign raw filesystem routing stays unchanged.
5. Package each variant with its own matching fresh executable/SYM payload.
   Build the ROM under a unique owned temporary filename; complete `n64tool` and
   cartridge configuration, then reject a ROM over **64 MiB before publication**.
   Only then atomically replace the requested final output. Failure preserves
   prior ROMs and unrelated temporary files.

Packaging now includes the required selected P8 palette and validates its shape;
real palette lookup is covered by the package regression. Optional PC packaging
also includes `mkasset` for the existing compression helper; this is not a new
vanilla-tool requirement. The source/worker compression round trip was checked
against an actual H64M-v2 Blood Gulch input with exact decoded bytes. Current
installer tests total **73**, including real helper dispatch, pool routing,
unique-temp ownership and exact-limit/oversized-ROM guard cases.

The private executable is now assembled with all six matching program/SYM
payloads bound. Its packaged workers pass actual freshly normalized Timberland
conversion, lossless compression, one-map FX pooling, six authored PC UI images
and a separate synthetic P8 palette probe. These ran with a Windows-only PATH,
cleared tool overrides and a working directory outside the source checkout.
The one-map pool covers 328 logical records / 274 unique bodies and preserves the
source snapshot. Package file hashes remain unchanged after these checks.

Of 605 generated Timberland files, 604 match the Ninja raw output exactly. The
decompressed H64M images differ only in the four preserved source tag-header
checksum bytes at `0x2e0644`–`0x2e0647`; all other 3,072,160 bytes match. The
separately normalized inputs have distinct source checksums. Original network
join code consumes this checksum: retain its provenance and do not describe it
as unused or universally inert. No checksum bytes were rewritten to force parity.

This is scoped private worker qualification. Full 19-map installer ROM generation,
a rendered packaged top-level GUI, clean-machine operation and public binary
redistribution/source-licence closure remain unqualified. The public repository/PR
contains source, tests, format docs, original project art and dependency patches
only; it supplies no retail maps/FX/UI assets, generated ROMs or private
tool-binary payloads.

### Frozen static artifacts and remaining runtime gates

All three matching **post-pool** program payload sets are now linked: Xbox
multiplayer, campaign and PC expansion, each with its own ELF/stripped/SYM data.
The PC ELF identity is
`abc07f5bd7d67a1f0df27f567c8f3b462bbbbc0853db8376ccd5ee3a59881179`.
The program receipt is in the enclosing project's
`audit/integration-upstream-a3f98e6-build01/installer-programs-receipt.json`.
These program payloads contain no Halo retail assets. The final cartridge,
assembled from the admitted matching program/data set, is frozen with these
identities:

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| PC expansion ROM | 65,945,600 | `33247b81ca0d11b93c62b847a928bad8777b45eb8896a853bc8711f3965b7ee9` |
| PC ELF | 9,036,516 | `abc07f5bd7d67a1f0df27f567c8f3b462bbbbc0853db8376ccd5ee3a59881179` |
| Packaged SYM | 199,030 | `f235173490fd0d81c1a85cc4b0b4edf35be577ebddb4bf0ec5b37f7cbc6db8f8` |
| Packed DFS | 64,469,184 | `ab9f04dc7103f851e4e2ac212b22a2d3f393b7195e429587cf5ce16bcac286ab` |
| Private installer ZIP | 208,943,266 | `e1053d4ffe20d590c3f48ea1045d353e454579e6afaed3ea7b4dd8b305d78d6a` |

The private EXE SHA-256 is
`2d5443bb3a6b61bd7804401a103734b2585acca238cb976cbddf4ae7e6de6848`.
These identities document local evidence; they do not authorize distributing
retail-derived ROM/DFS data or privately packaged dependencies.

The enclosing project's `audit/integration-upstream-a3f98e6-build01/` contains
`receipt.json`, `final-readback.json` and `primary-frozen-pair-recheck.json`.
The independent recheck verifies the ROM header/TOC, exact frozen payload hashes
and identical full/stripped ELF load segments. The actual ROM is 1,163,264 bytes
below the 67,108,864-byte limit. Old protected local files remain unchanged.
Private worker qualification is separately recorded under
`upstream-20261015/audit/installer/private-package-postpool01-qualification.json`.

### Bounded native02 result and local cartridge stages

The enclosing project's
`audit/hardware-repair/integration-upstream-a3f98e6-runtime01/runtime-receipt02.json`
records exactly **one ordinary unflagged boot** of the frozen ROM with a default
512-byte EEPROM and two actual completed 640x480 menu frames (native progression
1→2). A separate audit-only diagnostic ROM changes only five root flags: autotest,
Timberland map, one human, three bots and debug statistics. Its loaded code,
stripped program/SYM and all **1,760 non-root assets** match the frozen cartridge.
The gameplay start bypasses menu settings loading; this is not automated menu
navigation or a per-player-controls acceptance test.

The Ares v148 gameplay run lasted **172.221 seconds**. Six sparse RAM snapshots
verify actual bot/player identities, all four live native units, all 328 FX index
entries, the pooled bank identity and published cache bytes. FX record reads
advance from 19 to 30. One same-player salted-unit replacement with an increased
death counter corroborates a bot respawn; periodic native logs separately show
human and bot dead-to-alive transitions. These observations do not establish exact
spawn time, full health at spawn, controller responsiveness or every respawn path.

There are **43 native FPS windows**, **10.2–18.1 FPS**, median **15.2 FPS**.
The lowest logged free-heap sample is **94 KiB**. Debugger pauses affect timing;
these are bounded emulator observations, not hardware performance. Logged fatal,
object-exhaustion, collision-reference exhaustion and GC-critical/warning matches
are absent. There is **no comprehensive malloc-failure counter**: absence of
allocation-failure log matches cannot exclude silent or recovered failures, and
sparse heap/reserve samples are not true low-water/peak measurements. Nonfatal
CACHE/CreateThread diagnostics remain recorded. The 20 FPS target is not met;
**Experimental remains**. The separate full-eight-bot result is recorded below.

Root and the runtime lane viewed the completed 320x240 gameplay capture: coherent
pistol/hand, circular reticle and readable HUD/world. Its dark foreground and
single view do not qualify terrain shading, trees, every bot/model, all maps or
all weapon/VFX states. The earlier observer-only preflight NameError is retained
as preparation history; it occurred before launch and was not a game failure.
All owned native02 emulator children stopped, the debug port cleared and frozen
inputs remained unchanged.

`upstream-20261015/pc-integration-ready-manifest.json` records **three local** exact
copies of `Halo64-DEV-v0.2.0-PC-Integration01.z64`: the ROM folder, SummerCart stage
and EverDrive X7 stage. These are prepared on the PC, not installed on removable
media. No SD was mounted at the last root check (2026-10-15 21:07:35 UTC); no saves
were copied and no old builds replaced. The vanilla stages remain separate.

A separate **one-human/eight-bot** Timberland stress run is sealed in
`audit/hardware-repair/integration-upstream-a3f98e6-runtime01/full8-runtime-receipt03.json`.
It lasts **82.237 seconds** under a 90-second cap including reserved cleanup.
Compared with native02, only the `debug_bots=3` root flag changes to `debug_bots=8`;
loaded code, SYM and all 1,760 non-root assets remain identical. Four snapshots
verify nine actual native player identities, eight bots, and all nine spawned,
live, weapon-equipped/player-bound units. FX index/bank/cache identity passes;
record reads progress 21→30→35→35. These are native state observations, not nine
actors all visible together in the captured image.

Its **14 logged FPS windows** are **7.3–10.1 FPS**, median **8.15 FPS**; the separate
completed-image overlay reads 7.1 FPS. The lowest logged heap sample is **90 KiB**.
No fatal/object-exhaustion/collision-reference/GC-warning match was logged; no
general malloc-failure counter exists. Debugger timing and sparse sample limits
still apply. This bounded run passes roster/FX/stress-observation checks but
**fails the 20 FPS performance target**, so eight bots remain **Experimental**.
Root and the runtime lane viewed a coherent pistol/arms/reticle/HUD/outdoor scene;
that single view does not accept all bots, maps, shading, animation or VFX.
The owned emulator stopped, its port cleared and protected inputs were unchanged.

The finalized local-stage manifest binds both native receipts. All-map/split/
control/content gameplay, repeated cold boots, sustained reliability and
controlled 20 FPS remain open. Bounded full8 observation is complete; broad
full8 performance/reliability acceptance is not. The native/worker passes do not
close full 19-map installer ROM generation, rendered packaged GUI, clean-machine
operation, SD installation or physical hardware acceptance.

## Earlier checkpoint: dependency closure and vanilla readiness

The initial integration source checkpoint is
`51e405d047e44530b7ba9e1143075d21a0135e2d`. Its earlier build-graph suite passes
**11 tests**, including a 28-module static conversion dependency closure. The
narrow dependency split excludes installer/parser/test modules from conversion
edges but retains every reachable converter module, function-local static import,
schema JSON, P8 palette, SDK tool and explicit normalized map/HEK input. No dynamic
import/exec/eval is found in that conversion closure. This avoids unnecessary
reconversion without dropping real asset dependencies. Earlier graph receipts
remain historical; the final split is in
`upstream-20261015/audit/buildgraph/nonconverter-dependencies-review.json` in the
enclosing project.

The only root formatting change to `pc_input.py` removed an extra EOF newline.
The full AST, including node positions, is unchanged. The frozen installer and
actual normalization receipts retain their original source hashes; the explicit
`root-eof-format-bridge.json` bridges to the committed formatting hash instead of
silently relabeling their provenance.

The **separate untouched-upstream multiplayer** build is frozen as
`Halo64-Vanilla-v0.2.0-a3f98e6-20261015.z64`, 63,733,760 bytes, with all 13 maps
and 1,038 assets. Exact identities:

| Artifact | SHA-256 |
|---|---|
| Vanilla ROM | `a59444a6583275641aea354ee7525c834a216bdb5e0884aa54659b9a23b0cd21` |
| Vanilla ELF | `2344236f4d4bf6c40e9d13f79bdd27b86bd66ad4b46ee72eaa0c72c7639b28e1` |
| Packaged SYM | `7f381f057022cd883571fd5d10ebefcad08b49596abd457cd6aad96af8f12e53` |

`audit/vanilla-upstream-a3f98e6-build01/receipt.json` records the build/freeze.
The later boot receipt, separately under
`audit/hardware-repair/vanilla-upstream-a3f98e6-boot01/ordinary01/`, records **one**
ordinary emulator boot, no retry, and two actual matching submitted/completed
640x480 menu frames with native menu progression 1→2. Root viewed frame001:
readable HALO, Multiplayer, Firefight, Controls and Settings. Native default EEPROM
and controls/graphics were checked. Startup CACHE diagnostics and an unimplemented
CreateThread message remain recorded as nonfatal diagnostics; do not claim a
warning-free run. This does not test multiplayer/bots/maps, sustained reliability,
FPS, campaign or physical hardware.

`upstream-20261015/vanilla-ready-manifest.json` records **local** version-named
ROM/SummerCart/EverDrive stage copies. They are ready for a later card return,
not installed on removable media. No card is presently available for this new
copy. Original MAIN/DEV ROMs and saves are preserved; the synthetic Ares default
EEPROM is not copied into card stages. A later hardware preparation must keep the
new versioned save separate and record the actual copy/readback. PC19 integration
now has a separate static freeze and bounded native02 result; broader native and
hardware qualification remain open.

## Review and commit plan

Built game code is checkpointed at `d330422`; root owns the next reviewed guide
commit and subsequent runtime/package/physical qualification. Review later
changes against that checkpoint and v0.2.0. Keep recoverable source history and the
verified archive; do not replace old reports with new-code claims. Split review
into input/build/ABI, menu/controls/bots, renderer/stability, and focused docs/tests
where boundaries permit a buildable series. Root owns the Git index and the final
commit structure; this handoff does not stage, commit, push or open a PR.

Keep local audit captures, hardware identifiers, paths, legal input assets, build
caches, SDK binaries, emulator snapshots and generated ROMs out of the PR. Curate
`local/` helper files explicitly; copied local tools and stale HOLD documents are
not automatically publication-ready. Ship necessary reproducible converter/build
code and documented dependency patches, with tests relevant to their behavior.
A reviewer-ready PR draft and detailed requirements/evidence map live outside the
checkout under `upstream-20261015/audit/handoff/` while integration is unfinished.
