# Xbox SDK declarations for the native ports

The game's sources were written against the Xbox Development Kit (XDK):
they include `xtl.h` and a few other SDK headers, and use its Win32 subset,
Direct3D 8, DirectSound, Xbox networking and debug APIs. The native ports
(Linux, Windows, Android) implement those APIs themselves
(`port/linux/src`), so from the SDK they only need the *declarations*: the
types, structure layouts, constants and function prototypes. The SDK cannot
be redistributed, so these headers stand in for it. They were written
without the SDK's headers, from the sources below, and the native builds
need no part of the XDK.

## What is here

Only what the game and the platform layer use:

| File | Contents | Written from |
| --- | --- | --- |
| `xdk_pdb.h` | types, structures, enumerations and function prototypes | the January build's debug information (generated, see below) |
| `xdk_win32.h` | Win32 constants and macros | public documentation, public-domain mingw-w64 headers |
| `xdk_winsock.h` | Winsock and Xbox network constants and macros | public documentation, public-domain mingw-w64 headers, the January build |
| `xdk_xbox.h` | Xbox system and controller constants | the January build, public Xbox documentation |
| `xdk_d3d8.h` | Direct3D 8 (the Xbox's) constants, macros and the C interface wrappers | the January build, public Xbox and DirectX documentation |
| `xdk_dsound.h` | DirectSound (the Xbox's) constants and wrappers | the January build, public documentation |
| `xdk_xbdm.h`, `xdk_xkbd.h`, `xdk_d3d8perf.h` | debug monitor, debug keyboard, Direct3D performance counters | the January build, the platform layer's implementation |

and, under the names the sources include, headers that bring them in as the
SDK's did: `xtl.h` (everything; `NOD3D` and `NODSOUND` leave out Direct3D
and DirectSound), `windef.h`, `winbase.h`, `xbox.h` (which also gives the
debug keyboard to a unit that defines `DEBUG_KEYBOARD`), `xkbd.h`, `xbdm.h`
and `d3d8perf.h`. Each hand-written header says, per group of declarations,
where it came from.

## Where they came from

- **The January build's debug information.** `cachebeta.pdb`, the debug
  information of the build the decompilation reproduces, records every type,
  structure layout, enumeration and library function prototype the game was
  compiled with. `tools/xdk_headers.py` (with `tools/pdb200_types.py`, a
  reader for its PDB 2.00 format) writes the ones named in `pdb_names.txt`,
  and what they depend on, into `xdk_pdb.h`, then checks every structure's
  size and member offsets against the PDB for each port's target. To add a
  name: add it to `pdb_names.txt` and rerun
  `python tools/xdk_headers.py --pdb path/to/cachebeta.pdb`.
- **The January build's code.** Macros (`#define` constants) leave no trace
  in debug information, but their values do in the compiled game: where the
  game's source uses one, the original machine code shows its value, and most
  of `source/` compiles to exactly that code.
- **Public documentation and headers.** Microsoft's public Win32, Winsock and
  DirectX documentation; the public-domain headers of mingw-w64; the Xbox Dev
  Wiki and nxdk for Xbox-specific facts.
- **Our own code.** The C interface wrappers (`IDirect3DDevice8_...` and the
  like) and anything else the SDK defined inline are written here, forwarding
  to the functions the platform layer implements.

The hand-written headers were written by agents given only these sources
and the list of names our code uses, never the SDK's headers or anything
derived from them; the generator reads only the PDB. The few additions made
while integrating them (`APIENTRY`, `<ctype.h>`, the C++ linkage of the
Direct3D tables) are public Win32 facts or C plumbing.

## How they are checked

- `tools/xdk_headers.py` checks each generated structure's size and member
  offsets against the PDB, for Linux, Windows and Android.
- The game's units were compiled with the original compiler and these
  headers, and compared with the January build (`build/split`, from the
  byte-matching setup): every function that matched the original with the
  SDK's headers calls the same functions with the same constants and data
  with these. Where the code differs, it is only in form: the inline
  functions here are written differently from the SDK's (a switch where it
  had a chain of comparisons, a loop where it had sixteen stores).
- The native builds compile for all three targets, and the game runs.

Some values leave no trace in the January build, because nothing in the
game tests them; those were chosen to fit the platform layer, and say so
where they are defined: `XBDM_NOERR`, the individual `DSMIXBIN_*` bits
(the game only uses their combinations, which the build fixes), which of
the `XNET_GET_XNADDR_*` bits means Ethernet and which DHCP,
`XMO_STREAMF_*`, `DSSTREAMPAUSE_*`, and the field names of the debug
monitor's module records beyond those the game reads.

## Changing them

They are ordinary headers, maintained by hand apart from `xdk_pdb.h`. When
code starts using an SDK name that is not here: if the PDB describes it, add
it to `pdb_names.txt` and regenerate `xdk_pdb.h`; otherwise add it to the
topic's header with a comment saying where its value comes from (the
January build, public documentation, or the platform layer's own choice).
Never copy from the SDK's headers.
