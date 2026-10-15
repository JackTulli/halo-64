# PC expansion integration on Halo 64 v0.2.0

**Integration source checkpoint committed; PC cartridge UNBUILT / UNQUALIFIED.**
The separate vanilla multiplayer build and one bounded menu boot are complete
within the scope recorded below; they do not qualify the PC expansion.
This document describes the merge decisions and review gates. Focused host/native
translation-unit checks are reported separately; they do not qualify the merged
ROM. No new upstream-based ROM is installed on a flashcart. Preserve the existing
main build and the separately named development build and saves.

## Pinned history and recovery

| Role | Exact ref / identity |
|---|---|
| Core source checkpoint | `51e405d047e44530b7ba9e1143075d21a0135e2d` (handoff documentation follows separately) |
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
contain the later `51e405d` integration checkpoint. Preserve the integration Git
history separately or create a newly verified checkpoint bundle before relying
on that new checkpoint for offline recovery.

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

The high-level interface being integrated is:

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
The admitted dependency repair and its remaining link gate are recorded below.

## Validation status and required matrix

This table is a snapshot after the core source checkpoint; asset/link work is
still in progress. The enclosing audit
handoff map records source/evidence hashes and pending owner receipts. A row's
PASS is limited to its stated scope. MISSING means no accepted evidence for this
new integration, not an invented implementation defect.

| Gate | Current evidence / status | Remaining |
|---|---|---|
| Source recovery | PASS: actual complete-bundle clone and `fsck --full` at preserved commit | Keep archive/manifest correction together |
| Upstream base / checkpoint | Local refs confirm v0.2.0 = a3f98e6; core integration committed as 51e405d | Root commits this handoff next and curates any later PR diff |
| Menu logic and four macro variants | PASS: extracted actual-C tables/controls/EEPROM/team/bot-limit and picture ownership checks; whole menu TU compiled MIPS without warnings | Full link; rendered menus, quick map-picture scrolling and reentry under memory pressure |
| Bot source/control checks | PASS: source/Astra review, roster/LOD/movement actual-C controls and three target TUs compiled; 65,536 seeded rosters tested | Fresh semantics/full link and native LOD/cadence/collision/pose/ghost/team behavior MISSING |
| Renderer TU / guards | PASS: independent source review, strict-warning full MIPS TU compile, 56 actual-C fault cases and 22,040 valid strip-batch byte comparisons | Full link, campaign fixture, actual DragonFS/GPU pressure, runtime/hardware checks MISSING |
| Optional PC input/normalization/GUI | PASS frozen installer suite: 60 source/mock tests and compile, real withdrawn Tk state/cancellation checks; root combined 64-test suite passes; owned Xbox ISO parsed read-only (50 files) | Actual normalization now passes within documented legacy-schema exceptions; N64 conversion, matched payloads, real package/clean-machine/rendered GUI and ROM/hardware qualification MISSING |
| ABI version/kind | PASS focused pipeline receipt: ten Python tests and 96 extracted actual-C version/kind/magic/reserved guard cases; owned-v5 explicit import/sibling isolation check | Fresh MP32/CP16 assets and linked loader/runtime coverage MISSING |
| Banshee selection | PASS authored asset/placement inventory and 668 actual-C wrapper/policy checks | Native placement masks, seat entry, flight/combat and performance MISSING |
| Build graph / helpers | PASS: eleven focused tests, 28-module actual converter dependency closure, Xbox13/campaign/PC19 graph generation and selected helper comparisons | Final profile-regenerated links and PC ROM conversion MISSING |
| Program compilation/link and payload isolation | Initial three 353-object compile sets completed; initial links exposed the SDK export omission | SDK plus platform-O2 graph regeneration/link retry remains pending; PC19 assets at 8/19 in the root progress snapshot; no PC ROM yet |
| Runtime | MISSING for new upstream integration | All 13/19 maps, campaign start/checkpoints, menu return/reentry, pickups/respawns and ordinary boots |
| Splits/controls | MISSING for new upstream integration | 1/2/4 humans, per-port scheme/inversion/sensitivity/reticles, pause controls, EEPROM round trip |
| Bots/teams | MISSING for new upstream integration | Full roster, 2 humans + 2 bots per team, 4-human-plus-bot case, 8-bot mode, Ghost rarity/abilities/FFA colours |
| Content/visuals | MISSING for new upstream integration | PC weapon pickups, Flamethrower dragon/fire, Rocket Hog pose/turret/wheels, deaths, alpha foliage, decals/paint/VFX/ribbon |
| ROM capacity | MISSING for fresh upstream assets | Recheck the 64 MiB cartridge limit with upstream ground tiles and the 19-map PC set; do not infer the old package margin transfers |
| Performance/memory | MISSING | Controlled full-lobby native runs, worst maps/splits/vehicles/modifiers, allocation/GC/collision limits; at least 20 FPS before removing Experimental |
| Flashcarts/hardware | MISSING | New exact package readback, SummerCart/EverDrive X7, physical cold boot/repeated gameplay; preserve named saves and main ROM |
| Networking | MISSING / separate research scope | No native M64 Wi-Fi or SummerCart USB matchmaking acceptance from this merge |

The previously installed card remains the named Stability06 development ROM
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
  controls/main-thread handoff and trusted-Python cancellation. Rendered GUI and
  packaged executable acceptance remain open.
- The link-checker has four tests plus an actual **350-native-object** comparison:
  one relative-path invocation and four bounded absolute-path batches yield
  identical empty weak/missing sets. This validates checker batching, not that
  final game programs link successfully.
- Root reports the three 353-object program compile sets completed. All three
  program links then failed on the missing SDK export. The isolated SDK repair
  is admitted for retry; successful fresh links/symbols/ROMs are still missing.
- Actual frozen PC normalization now completed in its new serial work directory:
  **725 trusted Invader/FFmpeg calls**, 34 HUD scale edits and 39 re-encoded sound
  tags. All six v5 compressed-stream/tag-bounds validators and explicit converter
  preflight checks pass; all nine owned input hashes remain unchanged. The
  345,308-byte PC UI bitmap was extracted and six 256x128 images decoded; native
  sprite packing of this new output is not yet qualified. A schema walk covered
  1,653,242 struct instances and 43,074 dependency edges with zero walk/bounds
  errors. Qualification retains the old identical dropped Soul layout and four
  unused zero Meter fields per map as documented exceptions. New compressed
  outputs differ from old ones, so no byte-identity claim is made. **This passes
  normalization, not N64 nineteen-map conversion, ROM assembly or packaged-binary
  smoke/hardware tests.**
- The separate vanilla multiplayer build now completed and froze all 13 maps /
  1,038 assets. One ordinary emulator menu boot with two matching completed frames
  is recorded below. This is not PC expansion, campaign, sustained gameplay or
  physical acceptance.
- Root's PC asset job snapshot is 8 of 19 maps. Three fresh integration program
  links still await SDK/profile graph regeneration and retry; no PC19 ROM exists
  at this checkpoint.

Root owns the subsequent link retry and final source curation; normalization
completion alone does not prove a successful link. Root will curate
the source commit. No PR, push or maintainer contact follows from this handoff.

## Latest checkpoint: dependency closure and vanilla readiness

Core integration source is committed as
`51e405d047e44530b7ba9e1143075d21a0135e2d`. The latest build-graph suite passes
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
assets/links and hardware qualification remain open.

## Review and commit plan

Core source is checkpointed in `51e405d`; root will commit the handoff separately
next. Review any later combined changes against that checkpoint and v0.2.0. Keep recoverable source history and the
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
