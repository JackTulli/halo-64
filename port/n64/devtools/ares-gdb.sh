#!/bin/bash
# Boot a ROM in ares (hidden display :97) with its GDB server, wait, then
# attach mips64-elf-gdb and print where every thread is.
# usage: ares-gdb.sh rom.z64 elf seconds [extra gdb commands file]
ROM=$(readlink -f "$1"); ELF=$(readlink -f "$2"); SECS=${3:-10}; OUT=$HOME/n64/ares-test/gdb
export DISPLAY=:97
if ! xdpyinfo -display :97 >/dev/null 2>&1; then
	Xvfb :97 -screen 0 1024x768x24 -nolisten tcp >/dev/null 2>&1 &
	sleep 1
fi
flatpak run --die-with-parent dev.ares.ares --settings-file $HOME/n64/ares-test/settings.bml \
	--system "Nintendo 64" --no-file-prompt \
	--setting Audio/Driver=None --setting General/HomebrewMode=true \
	--setting Nintendo64/ExpansionPak=true --setting Video/Blocking=false \
	--setting DebugServer/Enabled=true --setting DebugServer/Port=9123 --setting DebugServer/UseIPv4=true \
	"$ROM" > "$OUT.log" 2>&1 &
PID=$!
sleep "$SECS"
CMDS=${4:-/dev/null}
python3 $HOME/n64/tools/rsp.py "$ELF" 9123 512 > "$OUT.gdb.txt" 2>&1
kill $PID 2>/dev/null; sleep 0.5; kill -9 $PID 2>/dev/null
wait $PID 2>/dev/null
cat "$OUT.gdb.txt"
