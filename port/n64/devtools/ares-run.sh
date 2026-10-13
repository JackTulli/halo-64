#!/bin/bash
# Boot a ROM in ares on the hidden Xvfb display :97 (never :0).
# usage: ares-run.sh rom.z64 [seconds] [outprefix]
# Writes <outprefix>.log (ares stdout/stderr incl. ISViewer debugf) and <outprefix>.png.
ROM=$(readlink -f "$1"); SECS=${2:-10}; OUT=${3:-$HOME/n64/ares-test/run}
export DISPLAY=:97
if ! xdpyinfo -display :97 >/dev/null 2>&1; then
	Xvfb :97 -screen 0 1024x768x24 -nolisten tcp >/dev/null 2>&1 &
	echo $! > $HOME/n64/ares-test/xvfb.pid
	sleep 1
fi
flatpak run --die-with-parent dev.ares.ares --settings-file $HOME/n64/ares-test/settings.bml \
	--system "Nintendo 64" --no-file-prompt \
	--setting Audio/Driver=None --setting General/HomebrewMode=true \
	--setting Nintendo64/ExpansionPak=true --setting Video/Blocking=false \
	"$ROM" > "$OUT.log" 2>&1 &
PID=$!
sleep "$SECS"
ffmpeg -loglevel error -y -f x11grab -video_size 1024x768 -i :97 -frames:v 1 "$OUT.png"
kill $PID 2>/dev/null; sleep 0.5; kill -9 $PID 2>/dev/null
wait $PID 2>/dev/null
exit 0
