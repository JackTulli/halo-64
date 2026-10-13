#!/bin/bash
# bench-run.sh seconds label... : runs bench ROMs one after another on the
# hidden display :97 (never :0) and summarises each log. One ares at a time:
# parallel instances exhaust the GPU memory of the software Vulkan.
SECS=$1; shift
export DISPLAY=:97
xdpyinfo -display :97 >/dev/null 2>&1 || { Xvfb :97 -screen 0 1024x768x24 -nolisten tcp >/dev/null 2>&1 & sleep 1; }
[ -f $HOME/n64/ares-test/settings-1.bml ] || sed "s/Port: 9123/Port: 9124/" $HOME/n64/ares-test/settings.bml > $HOME/n64/ares-test/settings-1.bml
for L in "$@"; do
	systemd-run --user --scope --quiet -p MemoryMax=4G -p MemorySwapMax=0 flatpak run --die-with-parent dev.ares.ares --settings-file $HOME/n64/ares-test/settings-1.bml \
		--system "Nintendo 64" --no-file-prompt --setting Audio/Driver=None --setting General/HomebrewMode=true \
		--setting Nintendo64/ExpansionPak=true --setting Video/Blocking=false \
		"$HOME/n64/ares-test/roms/bench-$L.z64" > "$HOME/n64/ares-test/bench-$L.log" 2>&1 &
	PID=$!
	sleep "$SECS"
	ffmpeg -loglevel error -y -f x11grab -video_size 1024x768 -i :97 -frames:v 1 -vf "crop=750:576:118:85" "$HOME/n64/ares-test/bench-$L.png"
	kill $PID 2>/dev/null; sleep 0.5; kill -9 $PID 2>/dev/null; wait $PID 2>/dev/null
	python3 "$(dirname "$0")/bench-summary.py" "$HOME/n64/ares-test/bench-$L.log" "$L"
done
