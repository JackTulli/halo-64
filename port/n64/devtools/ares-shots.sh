#!/bin/bash
# Boot a ROM in ares on the hidden display :97 and take a screenshot every
# INTERVAL seconds, COUNT times (cropped to the N64 picture).
# usage: ares-shots.sh rom.z64 start_delay interval count outprefix
ROM=$(readlink -f "$1"); START=${2:-20}; INTERVAL=${3:-5}; COUNT=${4:-4}; OUT=${5:-$HOME/n64/ares-test/shot}
export DISPLAY=:97
if ! xdpyinfo -display :97 >/dev/null 2>&1; then
	Xvfb :97 -screen 0 1024x768x24 -nolisten tcp >/dev/null 2>&1 &
	sleep 1
fi
systemd-run --user --scope --quiet -p MemoryMax=4G -p MemorySwapMax=0 flatpak run --die-with-parent dev.ares.ares --settings-file $HOME/n64/ares-test/settings.bml \
	--system "Nintendo 64" --no-file-prompt \
	--setting Audio/Driver=None --setting General/HomebrewMode=true \
	--setting Nintendo64/ExpansionPak=true --setting Video/Blocking=false \
	"$ROM" > "$OUT.log" 2>&1 &
PID=$!
sleep "$START"
for i in $(seq 1 $COUNT); do
	ffmpeg -loglevel error -y -f x11grab -video_size 1024x768 -i :97 -frames:v 1 -vf "crop=750:576:118:85" "$OUT-$i.png"
	[ $i -lt $COUNT ] && sleep "$INTERVAL"
done
kill $PID 2>/dev/null; sleep 0.5; kill -9 $PID 2>/dev/null
wait $PID 2>/dev/null
python3 - "$OUT" "$COUNT" <<'PY'
import sys
from PIL import Image
out, count = sys.argv[1], int(sys.argv[2])
imgs = [Image.open(f"{out}-{i}.png") for i in range(1, count + 1)]
w, h = imgs[0].size
sheet = Image.new("RGB", (w * 2, h * ((count + 1) // 2)))
for i, im in enumerate(imgs):
    sheet.paste(im.resize((w, h)), ((i % 2) * w, (i // 2) * h))
sheet = sheet.resize((sheet.width // 2, sheet.height // 2))
sheet.save(f"{out}-sheet.png")
PY
