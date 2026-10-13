"""Removes the shared files no map of the cartridge uses any more.

The converter shares textures, HUD sprites, animation graphs and model data
between maps by content (tex/, hud/, anim/, mdl/ in the cartridge's file
tree, each map's list of what it uses beside them); conversions of earlier
builds leave files behind. Run before the file system is packed:

    python3 -m tools.n64.prune <file tree> <map>...
"""

import shutil
import sys
from pathlib import Path

SHARED = (("tex", ".sprite", "tex/{map}.list"), ("hud", ".sprite", "hud/{map}.list"),
          ("anim", ".anim", "maps/{map}.anims"), ("mdl", ".bin", "mdl/{map}.list"))


def main():
    root = Path(sys.argv[1])
    maps = sys.argv[2:]
    for directory, suffix, list_name in SHARED:
        used = set()
        # (each map's lighter models of 3-4 player games list theirs as
        # <map>-low, the sets with better view models <map>[-low]-fp<n>)
        names = maps + [f"{m}-low" for m in maps]
        names += [f"{m}-fp{q}" for m in names for q in (1, 2)]
        for m in names:
            listing = root / list_name.format(map=m)
            if listing.exists():
                used.update(line.strip() for line in listing.read_text().splitlines() if line.strip())
        folder = root / directory
        if not folder.exists():
            continue
        for path in folder.iterdir():
            if path.is_dir():
                # the per-map folders of earlier builds, and work folders
                shutil.rmtree(path)
            elif path.suffix == suffix and path.stem not in used:
                path.unlink()
            elif path.suffix == ".list" and path.stem not in names:
                path.unlink()
    # earlier builds' per-map animation files
    for path in (root / "maps").glob("*.anim"):
        path.unlink()


if __name__ == "__main__":
    main()
