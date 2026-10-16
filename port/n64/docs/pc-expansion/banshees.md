# Optional Banshee additions

The PC expansion adds two multiplayer Banshees on Blood Gulch, Sidewinder and
Ice Fields, one near each team's base. Enable them through the existing
CONTENT OPTIONS -> VEHICLES -> ALL MAP or ALL + ROCKET selection. MAP DEFAULT
and ROCKET HOGS retain their previous populations. Blood Gulch keeps its two
optional Rocket Hogs as well. These additions apply only to the PC build.

The converter imports the multiplayer Banshee and its dependencies from the
explicit user-supplied Death Island map. Recipes store only anchor indices and
relative offsets. It appends vehicle/palette/global rows without moving the
original spawns; native vehicle lifetime and game-type rules remain in use.
Each pad must pass the real aircraft body's float32 BSP box and nine ground
support checks. No retail coordinates or game assets accompany this source.

These three maps have no authored aircraft floor/ceiling. The converter fills
those fields from the supplied BSP bounds and resolved spawn heights, preserving
any nonzero authored values. It requires three units of headroom above every
new aircraft. Native spring-based flight limits are vertical soft limits, not
an absolute horizontal containment guarantee or evidence of balanced flight.

Death Island, Gephyrophobia and Infinity keep their authored Banshees and limits.
Timberland did not yield a paired pad under the conservative placement checks;
Danger Canyon and compact maps get no forced aircraft. Do not infer Banshee bot
piloting, race support, collision-free flight or physical hardware acceptance
from conversion alone. Device checks should cover approach/prompt, boarding,
takeoff, ceiling behavior, weapons, landing, exit and vehicle destruction/renewal.

The device-test filename for this iteration is `Halo64-DEV-v0.2.1.z64`.
This is a local DEV subversion of the upstream v0.2.0 integration, not a claim
that upstream released v0.2.1. Keep main, vanilla and older DEV ROMs/saves.

On the six PC maps, ALL MAP and ALL + ROCKET keep the first game-type-eligible
pad of each vehicle definition per team. This avoids filling the N64 object
pool with alternate PC pads (Ice Fields otherwise creates 22 vehicles including
the added aircraft). Original rows, MAP DEFAULT, Xbox-map policy, scripted
placements and Race are preserved. Selection uses authored order, not live
object counts, so destruction and renewal do not enable duplicate pads.
