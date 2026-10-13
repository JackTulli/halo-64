#!/bin/bash
# bench-wsl.sh label seconds scene... : the benchmark scenes on the WSL
# machine with the shared agent tools -- flags-rom.sh builds each scene's ROM
# (the build lock), run-rom.sh runs it on the hidden display (the ares lock)
# for <seconds> of real time, bench-summary.py prints one line per scene.
# Scenes: w1 w2 w4 (Wizard, scripted players), b1 b2 b4 (Blood Gulch), f1 f2
# f4 (Blood Gulch Firefight), c4 (Chill Out). More port.json flags:
# BENCH_FLAGS="flag ...". Logs: /root/n64/ares-test/perf/<label>-<scene>.log.
# BUILD=0 runs ROMs built before.
REPO=${REPO:-$(cd "$(dirname "$0")/../../.." && pwd)}
LABEL=$1; SECS=$2; shift 2
OUT=/root/n64/ares-test/perf
mkdir -p "$OUT"
flags_for() {
	case $1 in
		w*) echo "autotest debug_map=wizard debug_players=${1#w}" ;;
		b*) echo "autotest debug_map=bloodgulch debug_players=${1#b}" ;;
		f*) echo "autotest debug_map=bloodgulch debug_firefight=1 debug_players=${1#f}" ;;
		c*) echo "autotest debug_map=chillout debug_players=${1#c}" ;;
	esac
}
if [ "${BUILD:-1}" != 0 ]; then
	for S in "$@"; do
		REPO=$REPO /root/n64/agent-tools/flags-rom.sh "perf-$LABEL-$S" $(flags_for "$S") $BENCH_FLAGS > /dev/null ||
			{ echo "build of $S failed"; exit 1; }
	done
fi
for S in "$@"; do
	/root/n64/agent-tools/run-rom.sh "/root/n64/ares-test/roms/perf-$LABEL-$S.z64" "$SECS" 1 1 "$OUT/$LABEL-$S" > /dev/null
	python3 "$REPO/port/n64/devtools/bench-summary.py" "$OUT/$LABEL-$S.log" "$LABEL-$S"
done
