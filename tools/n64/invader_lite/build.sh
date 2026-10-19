#!/bin/bash
# invader-lite: Invader (https://github.com/SnowyMouse/invader, GPLv3) built
# without Qt, SDL2, Rust/riat, libsquish, libFLAC and libvorbis, for Linux or
# (cross-compiled with MinGW-w64, statically linked) Windows; plus ogg2wav,
# a small Ogg Vorbis -> 16-bit PCM WAV decoder (stb_vorbis).
#
#     tools/n64/invader_lite/build.sh <invader-source> <build-dir> linux|windows
#
# <build-dir>/invader-lite  symlink farm of <invader-source> with this folder's
#                           stubs and edited copies of three CMake files
# <build-dir>/build         the CMake/Ninja build (invader-* tools, ogg2wav)
# <build-dir>/deps          windows: libsamplerate built for MinGW-w64
# <build-dir>/dist          the finished tools (+ NOTICE.txt and the GPL text)
#
# The source tree is only read: everything the build writes lands in
# <build-dir>, and the script fails if anything in the source changed.
#
# Needs: cmake (3.18+), ninja, python3 (Invader generates code at build time),
# a C/C++ compiler; linux: zlib and libsamplerate development packages
# (apt install zlib1g-dev libsamplerate0-dev); windows: apt install mingw-w64
# libz-mingw-w64-dev, git (libsamplerate is cloned at a pinned tag unless
# LIBSAMPLERATE_SOURCE names a checkout).
set -euo pipefail

TOOLS=(build extract convert refactor bludgeon edit sound info dependency strip crc)
LIBSAMPLERATE_URL=https://github.com/libsndfile/libsamplerate.git
LIBSAMPLERATE_TAG=0.2.2
LIBSAMPLERATE_COMMIT=c96f5e3de9c4488f4e6c97f59f5245f22fda22f7
MINGW=x86_64-w64-mingw32
MINGW_ROOT=/usr/$MINGW

usage() { echo "Usage: $0 <invader-source> <build-dir> linux|windows" >&2; exit 2; }
die() { echo "build.sh: $*" >&2; exit 1; }
[ $# -eq 3 ] || usage
TARGET=$3
case "$TARGET" in linux|windows) ;; *) usage ;; esac

HERE=$(dirname "$(readlink -f "$0")")
SRC=$(readlink -f "$1")
[ -f "$SRC/CMakeLists.txt" ] && [ -f "$SRC/src/invader.cmake" ] && [ -f "$SRC/dependencies.cmake" ] ||
	die "$SRC is not an Invader source tree"
OUT=$(readlink -m "$2")
case "$OUT/" in "$SRC/"*) die "the build directory must be outside the source tree" ;; esac
case "$SRC/" in "$OUT/"*) die "the source tree must not be inside the build directory" ;; esac
mkdir -p "$OUT"
FARM=$OUT/invader-lite
BUILD=$OUT/build
DEPS=$OUT/deps
DIST=$OUT/dist
JOBS=${JOBS:-$(nproc)}
export PYTHONDONTWRITEBYTECODE=1   # no __pycache__ next to the generators

# run a command with its output in a log; on failure show the log's tail
run_logged() {
	local log=$1
	shift
	if ! "$@" > "$log" 2>&1; then
		tail -40 "$log" >&2
		die "failed: $* (log: $log)"
	fi
}

STAMP=$OUT/.build-started
touch "$STAMP"

# ---- the symlink farm --------------------------------------------------------
/bin/rm -rf "$FARM" "$BUILD" "$DIST"
mkdir -p "$FARM"
shopt -s dotglob nullglob
for entry in "$SRC"/*; do
	[ "$(basename "$entry")" = .git ] && continue
	/bin/cp -as "$entry" "$FARM/"
done
shopt -u dotglob nullglob

# files replaced with real (edited) copies: remove their symlinks first
for f in CMakeLists.txt dependencies.cmake src/invader.cmake; do
	/bin/rm -f "$FARM/$f"
	/bin/cp "$SRC/$f" "$FARM/$f"
done
# headers the code generator writes into the "source" include tree: never
# links into the real source (it would write through them)
/bin/rm -f "$FARM/include/invader/tag/hek/definition.hpp" "$FARM/include/invader/tag/parser/parser.hpp"

# dependencies: only Python (code generators), threads, zlib and libsamplerate
cat > "$FARM/dependencies.cmake" <<'EOF'
# invader-lite: Python, threads, zlib and libsamplerate only
find_package(Python3 REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_path(SAMPLERATE_INCLUDE_DIR samplerate.h REQUIRED)
find_library(SAMPLERATE_LIBRARY NAMES samplerate REQUIRED)
include_directories(${SAMPLERATE_INCLUDE_DIR})
set(GIT_FOUND FALSE)
set(DEP_AUDIO_LIBRARIES ${SAMPLERATE_LIBRARY})
set(DEP_SQUISH_LIBRARIES "")
EOF

# the tools: only the ones the ROM builder uses (+ info, dependency, strip, crc)
python3 - "$FARM/CMakeLists.txt" "${TOOLS[@]}" <<'EOF'
import re
import sys
path, keep = sys.argv[1], {"invader", *sys.argv[2:]}
out = []
for line in open(path).read().splitlines():
    m = re.match(r"include\(src/(\w+)/(?:\w+/)?(\w+)\.cmake\)", line)
    if m and m.group(2) not in keep:
        line = "# invader-lite: " + line
    out.append(line)
open(path, "w").write("\n".join(out) + "\n")
EOF

# libinvader: no FLAC/Vorbis sources (the stubs instead), no riatc, stub headers
python3 - "$FARM/src/invader.cmake" <<'EOF'
import sys
path = sys.argv[1]
s = open(path).read()
for name in ("src/sound/sound_encoder_flac.cpp", "src/sound/sound_encoder_ogg_vorbis.cpp",
             "src/sound/sound_reader_flac.cpp", "src/sound/sound_reader_ogg.cpp"):
    assert "    " + name + "\n" in s, name
    s = s.replace("    " + name + "\n", "")
assert "    src/error.cpp\n" in s
s = s.replace("    src/error.cpp\n", "    src/error.cpp\n    lite/sound_lite_stubs.cpp\n", 1)
assert " ${DEP_SQUISH_LIBRARIES} riatc)" in s
s = s.replace(" ${DEP_SQUISH_LIBRARIES} riatc)", " ${DEP_SQUISH_LIBRARIES})")
old = "include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIRS} ext/riat/riatc/include)"
assert old in s
s = s.replace(old, "include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIRS} lite/include src/bitmap)")
assert "riatc" not in s
open(path, "w").write(s)
EOF

mkdir -p "$FARM/lite/include/riat" "$FARM/lite/include/vorbis"
/bin/cp "$HERE/riat.h" "$HERE/riat.hpp" "$FARM/lite/include/riat/"
/bin/cp "$HERE/squish.h" "$FARM/lite/include/"
/bin/cp "$HERE/vorbisenc.h" "$FARM/lite/include/vorbis/vorbisenc.h"
/bin/cp "$HERE/sound_lite_stubs.cpp" "$FARM/lite/"

# ---- configure ---------------------------------------------------------------
mkdir -p "$BUILD"
CMAKE_ARGS=(-S "$FARM" -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF)
if [ "$TARGET" = windows ]; then
	command -v "$MINGW-g++-posix" > /dev/null || die "$MINGW-g++-posix not found (apt install mingw-w64)"
	[ -f "$MINGW_ROOT/lib/libz.a" ] || die "$MINGW_ROOT/lib/libz.a not found (apt install libz-mingw-w64-dev)"

	# libsamplerate (BSD-2-Clause) for MinGW, static, into <build-dir>/deps
	LSR=${LIBSAMPLERATE_SOURCE:-$DEPS/src/libsamplerate}
	if [ ! -f "$LSR/CMakeLists.txt" ]; then
		mkdir -p "$(dirname "$LSR")"
		git -c advice.detachedHead=false clone --quiet --depth 1 --branch "$LIBSAMPLERATE_TAG" "$LIBSAMPLERATE_URL" "$LSR"
	fi
	commit=$(git -C "$LSR" rev-parse HEAD 2> /dev/null || echo unknown)
	[ "$commit" = "$LIBSAMPLERATE_COMMIT" ] ||
		echo "build.sh: warning: libsamplerate at $commit, expected $LIBSAMPLERATE_TAG ($LIBSAMPLERATE_COMMIT)" >&2
	/bin/rm -rf "$DEPS/build" "$DEPS/include" "$DEPS/lib"
	mkdir -p "$DEPS/build"
	run_logged "$DEPS/build/libsamplerate-cmake.log" cmake -S "$LSR" -B "$DEPS/build/libsamplerate" -G Ninja \
		-DCMAKE_TOOLCHAIN_FILE="$HERE/mingw-w64-x86_64.cmake" -DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX="$DEPS" -DCMAKE_INSTALL_LIBDIR=lib -DBUILD_SHARED_LIBS=OFF \
		-DBUILD_TESTING=OFF -DLIBSAMPLERATE_EXAMPLES=OFF -DLIBSAMPLERATE_INSTALL=ON
	run_logged "$DEPS/build/libsamplerate-build.log" cmake --build "$DEPS/build/libsamplerate" -j "$JOBS" --target install
	[ -f "$DEPS/lib/libsamplerate.a" ] || die "libsamplerate did not install $DEPS/lib/libsamplerate.a"
	/bin/cp "$LSR/COPYING" "$DEPS/libsamplerate-COPYING.txt"

	CMAKE_ARGS+=(-DCMAKE_TOOLCHAIN_FILE="$HERE/mingw-w64-x86_64.cmake"
		-DPython3_EXECUTABLE="$(command -v python3)"
		-DZLIB_LIBRARY="$MINGW_ROOT/lib/libz.a" -DZLIB_INCLUDE_DIR="$MINGW_ROOT/include"
		-DSAMPLERATE_LIBRARY="$DEPS/lib/libsamplerate.a" -DSAMPLERATE_INCLUDE_DIR="$DEPS/include")
fi
run_logged "$BUILD/cmake.log" cmake "${CMAKE_ARGS[@]}"
echo "configured $TARGET in $BUILD"

# ---- build -------------------------------------------------------------------
run_logged "$BUILD/ninja.log" cmake --build "$BUILD" -j "$JOBS"

if [ "$TARGET" = windows ]; then
	EXE=.exe
	run_logged "$BUILD/ogg2wav.log" "$MINGW-gcc-posix" -O2 -municode -static -s -o "$BUILD/ogg2wav.exe" "$HERE/ogg2wav.c"
else
	EXE=
	run_logged "$BUILD/ogg2wav.log" cc -O2 -o "$BUILD/ogg2wav" "$HERE/ogg2wav.c" -lm
fi

# ---- the finished tools --------------------------------------------------------
mkdir -p "$DIST"
for tool in "${TOOLS[@]}"; do
	[ -f "$BUILD/invader-$tool$EXE" ] || die "invader-$tool$EXE was not built"
	/bin/cp "$BUILD/invader-$tool$EXE" "$DIST/"
done
/bin/cp "$BUILD/ogg2wav$EXE" "$DIST/"
if [ "$TARGET" = windows ]; then
	# Windows line endings for the text files
	sed 's/\r*$/\r/' "$HERE/NOTICE" > "$DIST/NOTICE.txt"
	sed 's/\r*$/\r/' "$SRC/gplv3.txt" > "$DIST/GPL-3.0.txt"
else
	/bin/cp "$HERE/NOTICE" "$DIST/NOTICE.txt"
	/bin/cp "$SRC/gplv3.txt" "$DIST/GPL-3.0.txt"
fi

if [ "$TARGET" = windows ]; then
	# nothing but Windows system DLLs may be imported
	for exe in "$DIST"/*.exe; do
		extra=$("$MINGW-objdump" -p "$exe" | sed -n 's/^\s*DLL Name: //p' |
			grep -viE '^(kernel32|msvcrt|advapi32|user32|shell32|ws2_32|ole32|bcrypt|api-ms-win-.*)\.dll$' || true)
		[ -z "$extra" ] || die "$(basename "$exe") imports $extra"
	done
fi

# the source tree must be untouched
changed=$(find "$SRC" -path "$SRC/.git" -prune -o -newer "$STAMP" -print | head -5)
[ -z "$changed" ] || die "the build wrote into the source tree: $changed"

echo "invader-lite ($TARGET) in $DIST:"
ls -l "$DIST" | tail -n +2
