#!/bin/bash
# Regenerate the N64 ninja file and build the ROM; prints errors or the ROM.
# The build runs in a memory-capped scope (9 GB, no swap) with 8 jobs: past
# the cap the build is killed instead of the machine freezing (unbounded,
# thirteen map conversions and a 16-job LTO link froze a 15 GB machine).
cd "$(dirname "$(readlink -f "$0")")/../../.." || exit 1
# Invader (tag definitions, P8 palette): tools/n64_build.py defaults to the
# repository's dependencies/invader since the PC expansion PR; the converter's own
# default (tools/n64/tagdefs.py) is ~/n64/invader
export INVADER_DEFINITIONS="${INVADER_DEFINITIONS:-$HOME/n64/invader/src/tag/hek/definition}"
python3 tools/n64_build.py > /dev/null || exit 1
if ! systemd-run --user --scope --quiet -p MemoryMax=9G -p MemorySwapMax=0 \
	ninja -j 8 -f build/n64/build.ninja "$@" > build/n64/last-build.log 2>&1; then
	grep -E "error|FAILED|undefined|Killed|oom" build/n64/last-build.log | head -30
	exit 1
fi
ls -la --time-style=+%H:%M:%S build/n64/halo.z64 | awk '{print $5, $6, $7}'
