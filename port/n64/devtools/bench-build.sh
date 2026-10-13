#!/bin/bash
# bench-build.sh label flag... : builds the ROM with these port.json flags
# into ~/n64/ares-test/roms/bench-<label>.z64 (port.json's flags are left
# set: put them back to [] before committing or packaging)
LABEL=$1; shift
cd "$(dirname "$(readlink -f "$0")")/../../.." || exit 1
python3 - "$@" <<'PY'
import json, sys
p = 'port/n64/port.json'
d = json.load(open(p)); d["flags"] = sys.argv[1:]
json.dump(d, open(p, 'w'), indent=2); open(p, 'a').write('\n')
PY
$HOME/n64/tools/build.sh > /dev/null || { echo "build failed"; grep -E "error|FAILED" build/n64/last-build.log | head; exit 1; }
mkdir -p $HOME/n64/ares-test/roms
/bin/cp -f build/n64/halo.z64 $HOME/n64/ares-test/roms/bench-$LABEL.z64
