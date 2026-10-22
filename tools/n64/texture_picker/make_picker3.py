"""The third texture picker: every texture the converter can store in more
than one format -- the levels' (make_picker.py), the objects' seen in third
person and the terrain's grain and overlays (make_picker2.py) -- chosen map
by map. The first-person view models (weapons held, the hands) are left
out. The page saves texture-choices-per-map.json:

    {"version": 2,
     "maps": {map: {"textures": {bitmap name: {"option": id, "tint": [r, g, b] for -tint}},
                    "objects":  {...}, "grain": {...}, "overlays": {...}}}}

Each map's every texture is in it; NOW on the page is what the cartridge
holds today (tools/n64/texture_choices.json, else the converter's own
format). Transparent level textures stay in colour (level.py first_map):
they are offered 16 colours or full colour only.

    python3 tools/n64/texture_picker/make_picker3.py <the Xbox maps folder> <output folder>

The page holds the game's textures: keep it to yourself.
"""

import functools
import json
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
import make_picker as mp  # noqa: E402
import make_picker2 as mp2  # noqa: E402
from make_picker import MAPS, data_uri  # noqa: E402
from tools.n64.level import texture_choices  # noqa: E402

# the levels' options, in the objects' order: (label, what it is); IA4 for a cut-out
LEVEL_OPTIONS = (
    ("ci4-64x64", "16 colours 64×64", "CI4 · 2 KB"),
    ("colour-32x64", "Full colour 32×64", "RGBA16 · 4 KB"),
    ("grey-64x64", "Greyscale 64×64", "{fmt} · 2 KB"),
    ("grey-64x64-tint", "Greyscale 64×64, tinted", "{fmt} · 2 KB + tint"),
    ("grey-128x64", "Greyscale 128×64", "{fmt} · 4 KB (8192 texels, the shape that keeps most)"),
    ("grey-128x64-tint", "Greyscale 128×64, tinted", "{fmt} · 4 KB + tint"),
)
TRANSPARENT_OPTIONS = ("ci4-64x64", "colour-32x64")
MAP_IDS = {display: name for name, display in MAPS}
# the options "more dithering" applies to: 16 colours and the 16- and 8-grey
# ones (tools/n64/dither.py; not full colour or the 256-grey I8)
DITHERABLE = {"ci4-64x64", "grey-64x64", "grey-64x64-tint", "grey-128x64", "grey-128x64-tint",
              "now", "grain-i4-64x64", "grain-i4-64x64-micro", "overlay-i4-32x32", "overlay-i4-128x64"}


def with_dithered(options, dithered):
    """options (make_picker2.option) with their dithered images beside them
    ("dither"), where dithering applies and changes something"""
    by_id = {o["id"]: o["img"] for o in dithered}
    for o in options:
        image = by_id.get(o["id"])
        if o["id"] in DITHERABLE and image is not None and image.tobytes() != o["img"].tobytes():
            o["dither"] = image
    return options


def options_json(options):
    return [{"id": o["id"], "label": o["label"], "sub": o["sub"], "size": list(o["img"].size),
             "src": data_uri(o["img"]),
             **({"dither": {"src": data_uri(o["dither"])}} if o.get("dither") is not None else {})}
            for o in options]


@functools.lru_cache(maxsize=None)
def choices_of(section, map_id):
    """texture_choices, read once a section and map"""
    return texture_choices(section, map_id)


def now_of(section, name, default, options, maps):
    """what the cartridge holds: (the shared choice's option, else the
    converter's own; {map: [option, dithered]} where a map's own choice --
    texture_choices.json's "maps" -- is another)"""
    ids = {o["id"] for o in options}
    choice = choices_of(section, None).get(name.lower())
    now = choice.option if choice and choice.option in ids else default
    by_map = {}
    for display in maps:
        map_id = MAP_IDS.get(display)
        choice = choices_of(section, map_id).get(name.lower()) if map_id else None
        if choice and choice.option in ids and (choice.option, choice.dither) != (now, False):
            by_map[map_id] = [choice.option, choice.dither]
    return now, by_map


def main():
    maps_folder, out_folder = Path(sys.argv[1]), Path(sys.argv[2])
    out_folder.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="picker3-sprites-"))
    textures = []

    def add(section, key, name, default, chips, detail, maps, tint, image, options):
        # (now: the shared choice; now_maps: each map's own where it differs)
        now, now_maps = now_of(section, key, default, options, maps)
        textures.append({"section": section, "key": key, "name": name, "now": now, "now_maps": now_maps,
                         "chips": chips, "detail": detail, "maps": [MAP_IDS[m] for m in maps if m in MAP_IDS],
                         "tint": [round(t, 3) for t in tint] if tint else None, "size": list(image.size),
                         "original": mp2.reference(image), "options": options_json(options)})

    print("Finding the levels' textures", flush=True)
    found = mp.level_textures(maps_folder)
    for i, r in enumerate(sorted(found.values(), key=lambda r: -r["surfaces"])):
        cutout = r["kind"] == "cut-out"
        made, tint = mp.options_for(r["image"], cutout, work)
        dithered, _ = mp.options_for(r["image"], cutout, work, dither=True)
        wanted = TRANSPARENT_OPTIONS if r["kind"] == "transparent" else [o[0] for o in LEVEL_OPTIONS]
        options = with_dithered([mp2.option(id_, label, sub.format(fmt="IA4" if cutout else "I4"), made[id_])
                                 for id_, label, sub in LEVEL_OPTIONS if id_ in wanted],
                                [mp2.option(id_, "", "", image) for id_, image in dithered.items()])
        add("textures", r["name"], r["name"], r["current"], [r["kind"], f"{r['surfaces']:,} surfaces"],
            ", ".join(s.split("\\")[-1] for s in sorted(r["shaders"])[:6]), r["maps"], tint, r["image"], options)
        if (i + 1) % 20 == 0:
            print(f"  {i + 1}/{len(found)}", flush=True)

    print("Finding the objects' textures (third person)", flush=True)
    objects = [r for r in mp2.object_textures(maps_folder).values() if not r["first"]]
    for i, r in enumerate(sorted(objects, key=lambda r: (-len(r["maps"]), r["name"]))):
        options, tint = mp2.object_options(r["image"], r["mask"], r["cut"], False, work)
        options = with_dithered(options, mp2.object_options(r["image"], r["mask"], r["cut"], False, work, True)[0])
        chips = ["cut-out" if r["cut"] else "opaque"] + (["player colour"] if r["mask"] is not None else []) \
            + sorted(r["roles"])
        detail = ", ".join(sorted(r["models"])[:6]) + (" …" if len(r["models"]) > 6 else "")
        add("objects", r["key"], r["name"], "ci4-64x64", chips, detail, r["maps"], tint, r["image"], options)
        if (i + 1) % 25 == 0:
            print(f"  {i + 1}/{len(objects)}", flush=True)

    print("Finding the terrain's textures", flush=True)
    grains, overlays = mp2.terrain_textures(maps_folder)
    for section, records in (("grain", grains), ("overlays", overlays)):
        for r in sorted(records.values(), key=lambda r: -r["surfaces"]):
            if section == "grain":
                options = with_dithered(
                    mp2.grain_options(r["image"], r["micro"], r["micro_ratio"], r["colour"], work),
                    mp2.grain_options(r["image"], r["micro"], r["micro_ratio"], r["colour"], work, True))
            else:
                options = with_dithered(mp2.overlay_options(r["image"], r["colour"], work),
                                        mp2.overlay_options(r["image"], r["colour"], work, True))
            add(section, r["name"], r["name"], "now", [f"{r['surfaces']:,} triangles"], ", ".join(s.split("\\")[-1] for s in sorted(r["shaders"])[:6]),
                r["maps"], None, r["image"], options)

    template = (Path(__file__).parent / "picker3_template.html").read_text(encoding="utf-8")
    data = json.dumps({"maps": [{"id": name, "name": display} for name, display in MAPS],
                       "textures": textures}).replace("</", "<\\/")
    target = out_folder / "texture-picker-per-map.html"
    target.write_text(template.replace("/*DATA*/null", data), encoding="utf-8")
    counts = {s: sum(1 for t in textures if t["section"] == s) for s in ("textures", "objects", "grain", "overlays")}
    print(f"{target}: {target.stat().st_size / 1e6:.1f} MB, {counts}", flush=True)


if __name__ == "__main__":
    main()
