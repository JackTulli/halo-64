#!/bin/bash
# bench-set.sh label [seconds] [scene...] : builds the benchmark scenes'
# ROMs (bench-<label>-<scene>.z64) and runs them one after another on the
# hidden display (bench-run.sh), one summary line each. Scenes: w1 w2 w4
# (Wizard, scripted players), b1 b2 b4 (Blood Gulch), f1 f4 (Blood Gulch
# Firefight, normal). Extra port.json flags: BENCH_FLAGS="flag ...".
# port.json's flags are put back to [] afterwards.
LABEL=$1; SECS=${2:-70}; shift 2 2>/dev/null
SCENES=${*:-w1 w2 w4 b1 b2 b4 f1 f4}
cd "$(dirname "$(readlink -f "$0")")/../../.." || exit 1
DIR=$(dirname "$0")
for S in $SCENES; do
	case $S in
		w*) FLAGS="autotest debug_map=wizard debug_players=${S#w}" ;;
		b*) FLAGS="autotest debug_map=bloodgulch debug_players=${S#b}" ;;
		f*) FLAGS="autotest debug_map=bloodgulch debug_firefight=1 debug_players=${S#f}" ;;
	esac
	"$DIR/bench-build.sh" "$LABEL-$S" $FLAGS $BENCH_FLAGS || exit 1
done
python3 - <<'PY'
import json
p = 'port/n64/port.json'
d = json.load(open(p)); d["flags"] = []
json.dump(d, open(p, 'w'), indent=2); open(p, 'a').write('\n')
PY
RUNS=""
for S in $SCENES; do RUNS="$RUNS $LABEL-$S"; done
"$DIR/bench-run.sh" "$SECS" $RUNS
for S in $SCENES; do /bin/rm -f "$HOME/n64/ares-test/roms/bench-$LABEL-$S.z64"; done
