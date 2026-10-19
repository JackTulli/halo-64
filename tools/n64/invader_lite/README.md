# invader-lite and ogg2wav

The command-line tools the Halo 64 ROM Builder runs to turn Halo Custom
Edition maps into Xbox-format cache files, built from source for Linux and
(cross-compiled with MinGW-w64) Windows:

- **invader-lite**: [Invader](https://github.com/SnowyMouse/invader), the Halo:
  Combat Evolved modding toolkit, built without Qt, SDL2, Rust/Rat In a Tube,
  libsquish, libFLAC and libvorbis. It keeps the tools the converter uses
  (`invader-extract`, `invader-build -g xbox-ntsc`, `invader-convert`,
  `invader-refactor`, `invader-bludgeon`, `invader-edit`, `invader-sound`)
  plus `invader-info`, `invader-dependency`, `invader-strip` and
  `invader-crc`. Its only libraries are zlib and libsamplerate, so the
  Windows `.exe` files are small enough to ship and need no DLLs.
- **ogg2wav**: decodes one Ogg Vorbis stream from standard input to a 16-bit
  PCM WAV file, in place of
  `ffmpeg -hide_banner -loglevel error -y -i pipe:0 -acodec pcm_s16le OUT.wav`
  (Custom Edition sound tags carry Ogg Vorbis, which invader-lite cannot
  decode). Built on [stb_vorbis](https://github.com/nothings/stb).

## Upstream Invader

Built from Invader **0.55.0**, upstream commit
`696830ff80af227e84e7237c2ef26eb2301ed110` (2026-10-03, "we don't need this
flag after all", branch `master` of https://github.com/SnowyMouse/invader).
The `ext/riat` submodule is not needed. `build.sh` never writes into the
Invader checkout: it builds from a symlink farm and fails if anything in the
source tree changed during the build.

## What is stubbed out, and why

| Upstream dependency | Used upstream for | In invader-lite |
|---|---|---|
| Qt 6, SDL2 | `invader-edit-qt` (GUI editor, sound playback) | not built |
| Rust + Corrosion, Rat In a Tube (`riatc`) | compiling scenario scripts in `invader-build` | `riat.h`, `riat.hpp`: a compiler that yields no scripts, so a built scenario has no scripts (the N64 port does not run Halo scripts) |
| libsquish | DXT encoding/decoding of bitmaps | `squish.h`: decoding through Invader's bundled bcdec; encoding throws an error |
| libFLAC, libvorbis, libogg | FLAC / Ogg Vorbis encoding and decoding (`invader-sound`, Ogg sample counts) | `sound_lite_stubs.cpp`: encoding and decoding throw an error; `ogg_vorbis_sample_count` reads the Ogg pages itself (last granule position x channels); `vorbisenc.h` is an empty header. Decode Ogg Vorbis with `ogg2wav` first |
| libarchive, libtiff, FreeType, git | `invader-archive`, `invader-bitmap`, `invader-font`, version string | those tools are not built; the version string has no commit |

Tools not built: `invader-index`, `-bitmap`, `-resource`, `-archive`, `-font`,
`-string`, `-script`, `-collection`, `-compare`, `-scan`, `-model`,
`-recover`, `-lightmap`, `-edit-qt`.

The edits `build.sh` makes to Invader's build (all in the farm's copies):

- `dependencies.cmake` is replaced: Python 3 (Invader generates C++ from its
  tag definitions at build time), threads, zlib and libsamplerate only.
- `CMakeLists.txt`: the `include(src/<tool>/<tool>.cmake)` lines of the tools
  not built are commented out.
- `src/invader.cmake`: the FLAC/Ogg encoder and reader sources are replaced
  by `lite/sound_lite_stubs.cpp`; `riatc` is not linked; the stub headers in
  `lite/include` are on the include path.

## Files

| File | |
|---|---|
| `build.sh` | the build (farm, edits, CMake + Ninja, ogg2wav, `dist/`) |
| `mingw-w64-x86_64.cmake` | CMake toolchain file for the Windows cross-build |
| `riat.h`, `riat.hpp`, `squish.h`, `vorbisenc.h`, `sound_lite_stubs.cpp` | the stubs |
| `ogg2wav.c` | the decoder CLI |
| `stb_vorbis.c` | stb_vorbis v1.22, vendored unmodified |
| `NOTICE` | licence notices to ship next to the programs (`NOTICE.txt`) |

`stb_vorbis.c` is the file from https://github.com/nothings/stb at commit
`a7d4c2175630e1ea5b6f8dd00ab96fed0b97b96d` (fetched 2026-10-19; the file's
own header says v1.22), SHA-256
`4c7cb2ff1f7011e9d67950446b7eb9ca044f2e464d76bfbb0b84dd2e23e65636`.

## Building

```sh
tools/n64/invader_lite/build.sh <invader-source> <build-dir> linux|windows
```

for example (`JOBS=n` sets the parallel jobs, default `nproc`):

```sh
git clone https://github.com/SnowyMouse/invader.git ~/invader
git -C ~/invader checkout 696830ff80af227e84e7237c2ef26eb2301ed110
tools/n64/invader_lite/build.sh ~/invader ~/invader-lite-linux linux
tools/n64/invader_lite/build.sh ~/invader ~/invader-lite-win windows
```

`<build-dir>` gets:

- `invader-lite/`: the symlink farm (links into the source, plus the edited
  CMake files, the stubs, and the headers Invader's code generator writes)
- `build/`: the CMake/Ninja build and its logs (`cmake.log`, `ninja.log`)
- `deps/` (windows): libsamplerate built for MinGW-w64
- `dist/`: the 11 `invader-*` tools, `ogg2wav`, `NOTICE.txt` and the GPL
  text (`GPL-3.0.txt`) - the folder to ship

Requirements (Ubuntu 24.04 package names): `cmake` (3.18 or newer), `ninja-build`,
`python3`, `git`, and

- linux: `g++`, `zlib1g-dev`, `libsamplerate0-dev`. The tools link the system
  zlib and libsamplerate dynamically, like the earlier hand-made Linux build.
- windows: `mingw-w64` (the `x86_64-w64-mingw32-*-posix` compilers, for
  `std::thread` through winpthreads) and `libz-mingw-w64-dev` (static zlib
  1.3.1). libsamplerate 0.2.2 (commit `c96f5e3de9c4488f4e6c97f59f5245f22fda22f7`)
  is cloned from https://github.com/libsndfile/libsamplerate into
  `deps/src/` (or set `LIBSAMPLERATE_SOURCE` to a checkout) and built as a
  static library. Everything is linked statically (`-static -static-libgcc
  -static-libstdc++`) and stripped; the script checks that the `.exe` files
  import only Windows system DLLs. Invader's code generators run with the
  build host's `python3`.

Invader's `CMakeLists.txt` handles MinGW itself (no wildcard expansion of
arguments, 8 MB stack, `-Wa,-mbig-obj`, the version resources and the
manifest that turns on UTF-8 paths and long paths).

## ogg2wav

```
ogg2wav OUT.wav < stream.ogg
```

Reads one Ogg Vorbis stream (all of standard input, binary mode on Windows)
and writes a canonical 44-byte-header WAV (`WAVE_FORMAT_PCM`, 16-bit, the
stream's own sample rate and channel count; more than two channels stay in
Vorbis channel order). The output path may be any Unicode path on Windows.
Exit status 0 on success; otherwise a message on standard error, the partial
output removed, and status 1 (usage), 2 (input or decoding) or 3 (output).
Only one logical stream is decoded: split chained or multiplexed Ogg files
first (the ROM builder already passes one stream per call).

Compared with ffmpeg 6.1 (its native Vorbis decoder) on seven streams (the
three in h3_foundry.map and four made with libvorbis: mono and stereo,
22.05 to 44.1 kHz, 0.05 to 12.7 s): same format and length, and at most
0.05 % of the samples differ, by 1 LSB (float rounding in the two
decoders). The length follows the stream's final granule position, as
Invader's `ogg_vorbis_sample_count` does; on a 1600-frame single-page stream
ffmpeg gave 1664 frames (untrimmed), ogg2wav 1600. ffmpeg's WAV also carries
a `LIST` chunk that ogg2wav does not write.

## Verification

Checked on 2026-10-19 (Windows 11 host running the `.exe` files natively,
Linux build in WSL Ubuntu 24.04):

- The `linux` target is byte-identical to the earlier hand-made Linux build
  of the same recipe (all 11 tools).
- `invader-extract` on a Custom Edition map (h3_foundry.map, 2182 tags):
  the Windows and Linux tag trees are byte-identical.
- `invader-build -g xbox-ntsc -E -H` of a normalised Custom Edition tag tree
  (complex): the Windows and Linux cache files are byte-identical (no
  timestamps in the Xbox map), with the same warnings.
- `pc_prepare.py`'s Ogg Vorbis step (`ogg2wav`, `invader-edit`,
  `invader-sound -F 16-bit_pcm`): identical WAVs and sound tags on both.
- `ogg2wav` on Windows and Linux gives identical WAVs; against ffmpeg see
  above.

## Licences

- **Invader** is licensed under the GNU General Public License, version 3 only
  (`gplv3.txt` and `LICENSE.md` in its source). The stubs, the edits made by
  `build.sh` and the toolchain file in this folder are the changes to it and
  are under the same licence (GPL-3.0-only). Corresponding source: upstream
  Invader at the commit above plus this folder. Invader bundles ADPCM-XQ
  (BSD 3-clause), Reclaimer's Xbox ADPCM changes (MIT), bcdec (MIT or
  Unlicense) and stb_image (public domain).
- **libsamplerate** (linked into the tools): BSD 2-clause licence.
- **zlib** (linked into the tools): zlib licence.
- **stb_vorbis** (in ogg2wav): public domain (Unlicense) or MIT, at your
  option. `ogg2wav.c` is under the same terms.
- Windows builds also link the MinGW-w64 runtime and winpthreads (MIT and
  BSD-style licences) and GCC's runtime libraries (GPL-3.0 with the GCC
  Runtime Library Exception).

`NOTICE` carries the notices and licence texts these require; `build.sh`
copies it, with Invader's GPL text, into `dist/`.
