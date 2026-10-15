#!/bin/bash
# libdragon's tools the ROM builder runs (tools/n64/rombuilder.py), built for
# Windows with MinGW-w64 (apt install mingw-w64), statically: mksprite,
# audioconv64, mkdfs, n64tool, ed64romconfig. From a copy of libdragon's
# tools, so that its own build (Linux objects in the same folders) is left
# as it is.
#
#     tools/n64/build_windows_tools.sh <libdragon> <output folder>
set -e
LIBDRAGON=${1:-$HOME/n64/libdragon}
OUT=${2:-build/windows-tools}
WORK=$(mktemp -d)
trap '/bin/rm -rf "$WORK"' EXIT
/bin/cp -r "$LIBDRAGON/tools" "$LIBDRAGON/include" "$LIBDRAGON/src" "$WORK/"
cd "$WORK/tools"
find . \( -name "*.o" -o -name "*.d" -o -name "*.a" \) -delete
make -j"$(nproc)" CC=x86_64-w64-mingw32-gcc CXX=x86_64-w64-mingw32-g++ AR=x86_64-w64-mingw32-ar \
	LDFLAGS="-pthread -static -static-libgcc -static-libstdc++" \
	mksprite audioconv64 mkdfs n64tool ed64romconfig
cd - > /dev/null
mkdir -p "$OUT"
for tool in mksprite/mksprite.exe audioconv64/audioconv64.exe mkdfs/mkdfs.exe n64tool.exe ed64romconfig.exe; do
	/bin/cp "$WORK/tools/$tool" "$OUT/"
done
/bin/cp "$LIBDRAGON/LICENSE.md" "$OUT/libdragon-LICENSE.md" 2>/dev/null || true
ls -la "$OUT"
