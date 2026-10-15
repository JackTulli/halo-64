# Optional PC content integration

The PC expansion is a separate cartridge build. The original Xbox multiplayer
and campaign programs retain their own source inputs and outputs.

## Existing menu flow

Use Multiplayer's existing Content screen. Under WEAPONS, select **PC PICKUPS**
to add Flamethrower and PC Fuel Rod world pickups on the original thirteen maps.
Starting weapons follow MAP DEFAULT, including the Pistol + AR override in Pro
variants. The other four weapon choices keep their indices and behavior.

The original Derelict Flamethrower remains in its authored modes. Its new
optional Flamethrower row applies only to CTF, avoiding a simultaneous duplicate.
New Flamethrower and Fuel Rod pickups use native item renewal, with 45-second
and 60-second spawn intervals and the weapon's existing default ammunition.
Pickup messages, button icons, reticles and interaction use the existing game
systems.

On Blood Gulch, VEHICLES **ALL MAP** or **ALL + ROCKET** enables two additional
Rocket Hogs. Original vehicles remain in place. MAP DEFAULT and ROCKET HOGS
retain their previous populations; ROCKET HOGS continues its existing
replacement behavior. Native scenario vehicle lifetime is retained, without
adding a periodic vehicle spawn manager.

Blood Gulch, Sidewinder and Ice Fields also gain an optional Banshee near each
base under ALL MAP and ALL + ROCKET. Their previously unset vertical aircraft
limits are derived conservatively from the supplied map bounds and spawn pads.
See [Banshee placement and device checks](banshees.md). These are native soft
height limits, not an absolute containment or flight-quality guarantee.
Sidewinder ground-vehicle additions remain deferred. Compact maps receive no forced vehicles.
The six PC maps retain their own authored placements, including Banshees where
authored and allowed for the selected game type.

## Implementation and review

The converter appends marked rows without moving the original rows. Recipes
contain anchor indices and original offset choices, rather than extracted
retail XYZ, BSP geometry or model bounds. The converter derives placement from
the user's supplied maps and checks default-pose collision/physics bounds,
sampled ground support and a complete BSP body box. Incompatible geometry
fails conversion instead of silently omitting an item or forcing it into a wall.

Only the new rows carry markers: `0x40000000` for equipment and `0x4000` for
scenario vehicles. Runtime gates affect those rows only. The PC build is an
explicit opt-in; default builds require no PC donor or placement imports.

A separate fix checks the six PC maps' vehicle allowed-game-type flags before
initial automatic allocation. It preserves original Xbox zero-mask behavior,
campaign, scripted placement, Firefight and Race. It does not implement new
per-team vehicle counts or replace the game's existing vehicle-type selection.

Portable regressions cover append-only blocks, references, marker refusal,
native placement gates and build variants. Static geometry checks do not prove
native access, settling, suspension sweeps or gameplay balance. Full ROM size,
native interaction, split screen, memory pressure, sustained performance and
device tests remain acceptance gates for a newly built candidate.

## Campaign reference

[Future campaign planning](campaign-planning.md) covers every mission and thirty
Flamethrower, Fuel Rod and Rocket Hog decisions. These are proposals, including
explicit deferrals; they do not claim implementation of additional campaign
missions. The full extracted reference is retained privately for its owner.

[Contribution provenance](PROVENANCE.md) records original art and dependency
notices. Retail inputs, extracted data, ROMs and private tool binaries must
remain outside the source contribution.

## PC input and audio conversion

The optional PC cartridge accepts an installed retail Halo PC Maps folder, its
parent game folder, or an ISO exposing those loose map/resource files. An
installer-only disc with its files packed in CAB/MSI is rejected with a request
to supply installed files; the builder does not run setup or autorun programs.
The original Xbox input remains required. Work and generated outputs remain
separate from the supplied files.

Recipe version 2 converts embedded Vorbis audio to lossless PCM intermediates
before the existing N64 audio conversion. This bypasses the external Xbox
ADPCM encoder and preserves source frame counts, including partial blocks.
Native Xbox ADPCM is decoded as a header predictor followed by 63 used nibbles
per channel: 64 frames, with the final high nibble ignored as padding. Effects
and menu music follow that same contract. The sound-cache version is bumped
so old decoded results are not reused. The independent format reference is
[vgmstream's Xbox IMA decoder](https://github.com/vgmstream/vgmstream/blob/master/src/coding/ima_decoder.c).

Input hardening bounds compressed/inflated Xbox cache sizes, checks stream
termination and rejects cyclic or invalid BSP traversal paths. These checks
do not execute disc installers and do not establish a general security
warranty for external tools or arbitrary retail formats.
