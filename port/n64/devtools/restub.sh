#!/bin/bash
# Regenerate the N64 build's stand-ins for left-out code: link without them,
# collect the undefined names, write port/n64/game/n64_stubs_generated.c.
cd "$(dirname "$(readlink -f "$0")")/../../.." || exit 1
mv port/n64/game/n64_stubs_generated.c /tmp/n64_stubs_old.c 2>/dev/null
python3 tools/n64_build.py > /dev/null
systemd-run --user --scope --quiet -p MemoryMax=9G -p MemorySwapMax=0 ninja -j 8 -f build/n64/build.ninja -k 0 build/n64/halo.elf > build/n64/link.log 2>&1
grep -o "undefined reference to \`[^']*'" build/n64/link.log | awk '{print $NF}' | tr -d "\`'" | sort -u > build/n64/undef.txt
python3 tools/n64_link_check.py build/n64/objects.txt >> build/n64/undef.txt
wc -l < build/n64/undef.txt
python3 tools/n64_stubs.py build/n64/undef.txt > port/n64/game/n64_stubs_generated.c
grep "no declaration" port/n64/game/n64_stubs_generated.c
