#!/bin/bash
# Builds the two ROMs for playing/watching into ~/n64/roms:
#   halo-n64.z64       real controllers (ports 1 and 2)
#   halo-n64-bots.z64  scripted players (the "autotest" flag file)
set -e
HERE="$(dirname "$(readlink -f "$0")")"
cd "$HERE/../../.."
set_flags() {
	python3 - "$@" <<'PY'
import json, sys
p = 'port/n64/port.json'
d = json.load(open(p))
d["flags"] = sys.argv[1:]
json.dump(d, open(p, 'w'), indent=2); open(p, 'a').write('\n')
PY
	/bin/rm -f build/n64/filesystem/autotest build/n64/filesystem/debug_overview build/n64/filesystem/debug_untextured
}
set_flags
"$HERE/build.sh"
/bin/cp -f build/n64/halo.z64 $HOME/n64/roms/halo-n64.z64
set_flags autotest
"$HERE/build.sh"
/bin/cp -f build/n64/halo.z64 $HOME/n64/roms/halo-n64-bots.z64
ls -la $HOME/n64/roms
