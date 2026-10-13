#!/bin/bash
# Builds the ROM with the given flags (plus autotest) and prints the counter
# line from three screenshots: ab.sh name [flags...]
NAME=$1; shift
cd "$(dirname "$(readlink -f "$0")")/../../.."
python3 - "$@" <<'PY'
import json, sys
p = 'port/n64/port.json'
d = json.load(open(p))
d["flags"] = ["autotest"] + sys.argv[1:]
json.dump(d, open(p, 'w'), indent=2); open(p, 'a').write('\n')
PY
$HOME/n64/tools/build.sh > /dev/null || exit 1
$HOME/n64/tools/ares-shots.sh build/n64/halo.z64 30 3 3 $HOME/n64/ares-test/ab-$NAME
python3 - $NAME <<'PY'
import sys
from PIL import Image
n = sys.argv[1]
ims = [Image.open(f'/home/jack/n64/ares-test/ab-{n}-{i}.png').crop((0, 0, 750, 36)) for i in range(1, 4)]
s = Image.new('RGB', (750, 108))
for i, im in enumerate(ims): s.paste(im, (0, i * 36))
s.save(f'/home/jack/n64/ares-test/ab-{n}-counter.png')
PY
