"""Halo: Custom Edition maps made Xbox maps for the custom cartridge.

A Custom Edition multiplayer map (cache version 609) becomes the Xbox cache
(version 5) tools/n64/convert.py reads (with --custom), in a workspace of
its own:

1. its tag table is read: a multiplayer map, its tags not hidden by its
   author ("protected" maps can't be taken apart, and aren't);
2. its tags whose data live in Custom Edition's shared resource maps
   (bitmaps.map, sounds.map, loc.map: not needed here) are taken from the
   Xbox maps' tags of the same names (the multiplayer maps first, then the
   menu's and a30's): Custom Edition's are mostly the Xbox game's own;
3. invader-extract: the Xbox build's menu tags (bloodgulch.map's), the
   map's own tags, those of step 2;
4. the PC maps' normalization (pc_prepare.py): models and chicago shaders
   made the Xbox's kinds, bludgeoned indices, HUD scales, Vorbis sounds made
   16-bit PCM (decoded by ogg2wav, tools/n64/invader_lite, or ffmpeg);
5. tags only Custom Edition has: the PC-only vehicles' and weapons' from the
   Xbox tags nearest them (STANDINS), any other bitmap white and any other
   sound silent (the build loop);
6. invader-build -g xbox-ntsc, the map named for the cartridge (-N), each
   error it stops on that has a known repair repaired (fixes.txt) and the
   build run again.

    python3 -m tools.n64.ce_prepare <map> --xbox-maps <folder> --tools <folder> --work <folder> [--name <name>]

writes <work>/<map's workspace>/maps/<name>.map and <work>/result.json (the
map, its cartridge name, the menu's name for it). Only the explicitly given
tool folder is trusted (Invader's tools as tools/n64/invader_lite builds
them, ogg2wav or ffmpeg); the input maps are only read.
"""

import argparse
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import wave

from . import pc_prepare

INVADER_TOOLS = ("extract", "convert", "refactor", "bludgeon", "edit", "sound", "build")
CE_VERSION = 609
CE_TAG_BASE = 0x40440000
# tag groups Custom Edition's resource maps hold, and their files' extensions
RESOURCE_EXTENSIONS = {"bitm": "bitmap", "snd!": "sound", "ustr": "unicode_string_list", "font": "font",
                       "hmt ": "hud_message_text"}
# where Xbox tags of a resource tag's name are looked for, in order
XBOX_ORDER = ("bloodgulch", "beavercreek", "boardingaction", "carousel", "chillout", "damnation", "hangemhigh",
              "longest", "prisoner", "putput", "ratrace", "sidewinder", "wizard", "ui", "a30", "a10", "a50", "b30",
              "b40", "c10", "c20", "c40", "d20", "d40")
# the Xbox build's menu tags (invader-build -g xbox-ntsc imports them), taken
# first so that a Custom Edition map's PC menus never replace them
XBOX_UI = ("ui/shell/multiplayer.ui_widget_collection", "ui/shell/bitmaps/white.bitmap",
           "ui/multiplayer_game_text.unicode_string_list")
WHITE = "ui/shell/bitmaps/white.bitmap"
# Halo PC's own vehicles' and weapons' tags (Custom Edition's resource maps
# have them, the Xbox's maps don't): the Xbox tags nearest them
STANDINS = {
    "vehicles/rwarthog/bitmaps/rwarthog.bitmap": "vehicles/warthog/bitmaps/warthog.bitmap",
    "vehicles/rwarthog/bitmaps/rwarthog_tires.bitmap": "vehicles/warthog/bitmaps/warthog tires.bitmap",
    "vehicles/rwarthog/bitmaps/rwarthog_rl.bitmap": "weapons/rocket launcher/bitmaps/rocket launcher.bitmap",
    "vehicles/rwarthog/bitmaps/rwarthog_rl_multipurpose.bitmap":
        "weapons/rocket launcher/bitmaps/rocket launcher multipurpose.bitmap",
    "sound/sfx/weapons/rocket launcher/reload.sound": "sound/sfx/weapons/weapon_anims/rocket_reload_f.sound",
    "sound/sfx/impulse/melee/fuelrod_impact.sound": "sound/sfx/impulse/melee/plasma_impact.sound",
    "sound/sfx/weapons/weapon_anims/plasma_cannon_overheat.sound": "sound/sfx/weapons/weapon_anims/plasrifle_overheat.sound",
    "sound/sfx/weapons/weapon_anims/fuelrod_melee.sound": "sound/sfx/weapons/weapon_anims/plasrifle_melee.sound",
    "sound/sfx/weapons/plasma_cannon/plasma_cannon_explosion.sound": "sound/sfx/weapons/fuel rod gun/explosion.sound",
    "sound/sfx/weapons/plasma_cannon/overheat.sound": "sound/sfx/weapons/weapon_anims/plasrifle_overheat.sound",
    "sound/sfx/weapons/flamethrower/burnout.sound": "sound/sfx/weapons/flamethrower/light.sound",
    "sound/sfx/weapons/flamethrower/impact.sound": "sound/sfx/impulse/melee/flame_impact.sound",
    "sound/sfx/weapons/flamethrower/empty.sound": "sound/sfx/weapons/weapon_anims/flamethrower_posing.sound",
}
# names a custom map can't have on the cartridge (the files beside it)
RESERVED_NAMES = {"ui", "a30", "bitmaps", "sounds", "loc"}
MAX_NAME = 24
MAX_DISPLAY = 23
MAX_BUILDS = 300
_MAX_MAP = (2 << 30) - 1


class CEPrepareError(ValueError):
    """a Custom Edition map that can't be made an Xbox map, said plainly"""


# ---------- the Custom Edition map

def read_ce_map(path):
    """(header name, scenario tag name, [(group, name, in a resource map)])
    of a Custom Edition multiplayer map, bounds checked (it is the player's
    file, from anywhere)"""
    path = Path(path)
    if path.is_symlink() or not path.is_file() or not 0x800 < path.stat().st_size <= _MAX_MAP:
        raise CEPrepareError(f"{path.name} is not a Halo map file")
    size = path.stat().st_size
    with path.open("rb") as stream:
        header = stream.read(0x800)
        if header[:4] != b"daeh" or header[-4:] != b"toof":
            raise CEPrepareError(f"{path.name} is not a Halo map file")
        version = struct.unpack_from("<I", header, 4)[0]
        if version != CE_VERSION:
            kind = {5: "an Xbox map", 7: "a retail Halo PC map"}.get(version, f"cache version {version}")
            raise CEPrepareError(f"{path.name} is {kind}, not a Halo Custom Edition map")
        offset, length = struct.unpack_from("<II", header, 0x10)
        if struct.unpack_from("<H", header, 0x60)[0] != 1:
            raise CEPrepareError(f"{path.name} is not a multiplayer map (the custom cartridge plays multiplayer maps)")
        if offset < 0x800 or length < 40 or offset + length > size:
            raise CEPrepareError(f"{path.name} has invalid tag data bounds")
        stream.seek(offset)
        data = stream.read(length)
    name = header[0x20:0x40].split(b"\0", 1)[0].decode("latin1")
    array, scenario_id, _, count = struct.unpack_from("<IIII", data, 0)
    first = array - CE_TAG_BASE
    if not 0 < count <= 65535 or first < 0 or first + count * 32 > len(data):
        raise CEPrepareError(f"{path.name} has an invalid tag table")
    tags = []
    for i in range(count):
        group, _, _, _, name_pointer, _, indexed, _ = struct.unpack_from("<4s4s4sIIIII", data, first + i * 32)
        start = name_pointer - CE_TAG_BASE
        end = data.find(b"\0", start) if 0 <= start < len(data) else -1
        if end < 0 or end - start > 255:
            raise CEPrepareError(f"{path.name} has an invalid tag name")
        tags.append((group[::-1].decode("latin1"), data[start:end].decode("latin1"), bool(indexed)))
    scenario = tags[scenario_id & 0xFFFF] if (scenario_id & 0xFFFF) < count else None
    hidden = sum(1 for _, n, _ in tags if n == "<protected>" or not n)
    if hidden or not scenario or scenario[0] != "scnr":
        raise CEPrepareError(f"{path.name}'s author protected it against being taken apart: it can't be converted")
    return name, scenario[1], tags


def cartridge_name(text):
    """a map's name on the cartridge (its file's and the menu's folder):
    lower-case letters, digits and underscores"""
    name = re.sub(r"[^a-z0-9]+", "_", text.lower()).strip("_")[:MAX_NAME].strip("_") or "custom"
    return name + "_ce" if name in RESERVED_NAMES else name


def display_name(text):
    """the menu's name for a map"""
    name = re.sub(r"[_\s]+", " ", text).strip().upper()
    return re.sub(r"[^A-Z0-9 '!&.-]", "", name)[:MAX_DISPLAY].strip() or "CUSTOM MAP"


# ---------- the tools

def find_tools(folder):
    """{tool: path} from the explicit folder: Invader's (invader-<name>) and
    an Ogg Vorbis decoder (ogg2wav, else ffmpeg)"""
    if folder is None:
        raise CEPrepareError("the Custom Edition conversion needs its tools folder (Invader's tools and ogg2wav)")
    folder = Path(folder).expanduser().resolve()
    suffix = ".exe" if sys.platform == "win32" else ""
    tools, missing = {}, []
    for name in INVADER_TOOLS:
        path = folder / f"invader-{name}{suffix}"
        if path.is_file():
            tools[name] = path
        else:
            missing.append(path.name)
    for decoder in ("ogg2wav", "ffmpeg"):
        path = folder / f"{decoder}{suffix}"
        if path.is_file():
            tools["decoder"], tools["decoder_kind"] = path, decoder
            break
    else:
        missing.append(f"ogg2wav{suffix}")
    if missing:
        raise CEPrepareError("missing Custom Edition conversion tools in " + str(folder) + ": " + ", ".join(missing))
    return tools


class Workspace:
    """one map's conversion: its folders, its tools, its logs"""

    def __init__(self, root, tools):
        self.root = root
        self.tools = tools
        self.tags, self.audio, self.maps, self.logs = (root / n for n in ("tags", "sound-source", "maps", "logs"))
        for folder in (self.tags, self.audio, self.maps, self.logs):
            folder.mkdir(parents=True, exist_ok=True)
        self.count = 0
        self.fixes = (root / "fixes.txt").open("a", encoding="utf-8")

    def run(self, command, data=None, check=True):
        self.count += 1
        command = [str(c) for c in command]
        options = {"capture_output": True, "cwd": str(self.root)}
        if os.name == "nt":
            options["creationflags"] = subprocess.CREATE_NO_WINDOW
        if data is None:
            options["stdin"] = subprocess.DEVNULL
        else:
            options["input"] = data
        result = subprocess.run(command, **options)
        # (the Windows tools end their lines with CR LF: the repairs read
        # lines)
        text = (result.stdout + result.stderr).decode("utf-8", "replace").replace("\r\n", "\n")
        (self.logs / f"{self.count:04d}-{Path(command[0]).stem}.log").write_text(" ".join(command) + "\n" + text,
                                                                                  encoding="utf-8")
        if result.returncode and check:
            raise CEPrepareError(f"{Path(command[0]).name} failed ({result.returncode}): " + text.strip()[-1500:])
        return result.returncode, text

    def invoke(self, tool, *args):
        """pc_prepare.py's way of running a tool: its output, stripped"""
        if tool == "sound" and os.sep == "/":
            # (invader-sound joins the tag's path to the data folder)
            args = (*args[:-1], str(args[-1]).replace("\\", "/"))
        _, text = self.run([self.tools[tool], *args])
        return text.strip()

    def decode_ogg(self, ogg, destination):
        if self.tools["decoder_kind"] == "ogg2wav":
            self.run([self.tools["decoder"], destination], ogg)
        else:
            self.run([self.tools["decoder"], "-hide_banner", "-loglevel", "error", "-y", "-i", "pipe:0",
                      "-acodec", "pcm_s16le", destination], ogg)

    def fixed(self, text):
        self.fixes.write(text + "\n")
        self.fixes.flush()

    def edit(self, tag, *args):
        self.invoke("edit", "-t", self.tags, *args, tag)
        self.fixed("edit " + " ".join(str(a) for a in args) + " " + tag)


# ---------- the steps

def xbox_tag_names(path):
    """the (group, name) pairs of an Xbox map's tags"""
    from .xboxmap import XboxMap, TAG_BASE
    xmap = XboxMap(path)
    first = xmap.instances_address - TAG_BASE
    names = set()
    for i in range(xmap.tag_count):
        group, = struct.unpack_from("<I", xmap.tag_data, first + i * 32)
        pointer = struct.unpack_from("<I", xmap.tag_data, first + i * 32 + 16)[0] - TAG_BASE
        name = xmap.tag_data[pointer:xmap.tag_data.index(b"\0", pointer)].decode("latin1")
        names.add((group.to_bytes(4, "big").decode("latin1"), name))
    return names


def resource_sources(tags, xbox_maps, log):
    """{(group, name): Xbox map} for the map's resource tags"""
    wanted = {(g, n) for g, n, indexed in tags if indexed and g in RESOURCE_EXTENSIONS}
    found = {}
    for name in XBOX_ORDER:
        path = xbox_maps / f"{name}.map"
        if len(found) == len(wanted):
            break
        if not path.is_file():
            continue
        names = xbox_tag_names(path)
        for tag in wanted - found.keys():
            if tag in names:
                found[tag] = path
    log(f"{len(found)} of {len(wanted)} shared Custom Edition tags found in the Xbox maps")
    return found


def tag_file(name, group):
    return name.replace("\\", "/") + "." + RESOURCE_EXTENSIONS[group]


def extract(space, ce_map, xbox_maps, found):
    def queries(map_path, names):
        for i in range(0, len(names), 60):
            args = ["-t", space.tags, "-r"]
            for query in names[i:i + 60]:
                args += ["-s", query]
            space.invoke("extract", *args, map_path)

    queries(xbox_maps / "bloodgulch.map", list(XBOX_UI))
    space.invoke("extract", "-t", space.tags, ce_map)
    by_map = {}
    for (group, name), path in found.items():
        by_map.setdefault(path, []).append(tag_file(name, group))
    for path, names in by_map.items():
        queries(path, sorted(names))


def apply_standins(space):
    for target, source in STANDINS.items():
        t, s = space.tags / target, space.tags / source
        if not t.exists() and s.is_file():
            t.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(s, t)
            space.fixed(f"stand-in: {source} copied to {target}")


def silent_sound(space, name):
    """a silent sound tag at name (no extension): a sound only Custom
    Edition has"""
    folder = space.audio / "silent" / Path(name.replace("\\", "/"))
    folder.mkdir(parents=True, exist_ok=True)
    with wave.open(str(folder / "silence.wav"), "wb") as out:
        out.setnchannels(1)
        out.setsampwidth(2)
        out.setframerate(22050)
        out.writeframes(bytes(2 * 2205))
    space.invoke("sound", "-t", space.tags, "-d", space.audio / "silent", "-F", "16-bit_pcm", "-c", "scripted_effect",
                 name.replace("/", "\\"))
    space.fixed(f"stand-in: a silent sound for {name}.sound")


def build(space, scenario, name, log):
    """invader-build until it builds, a known repair made after each stop"""
    output = space.maps / f"{name}.map"
    last = None
    for attempt in range(MAX_BUILDS):
        code, text = space.run([space.tools["build"], "-g", "xbox-ntsc", "-E", "-H", "-N", name, "-t", space.tags,
                                "-o", output, scenario], check=False)
        if code == 0:
            log(f"built after {attempt} repairs")
            return output
        flat = re.sub(r"\n(?!\.\.\.in |FATAL|ERROR|WARNING|Failed)", " ", text)
        # (the same error again after its repair: the repair didn't take)
        errors = "\n".join(line for line in flat.splitlines() if "ERROR" in line or line.startswith("Failed"))
        if errors != last and repair(space, flat, log):
            last = errors
            continue
        lines = [line for line in text.splitlines() if "ERROR" in line or line.startswith(("...in", "Failed"))]
        raise CEPrepareError("the Xbox map could not be built: " + " / ".join(lines[-6:])[-1200:])
    raise CEPrepareError(f"the Xbox map could not be built after {MAX_BUILDS} repairs")


def repair(space, flat, log):
    """one known repair of invader-build's error (True), else False"""
    missing = re.search(r"Failed to find (.*)\.bitmap\n", flat)
    if missing:
        # a bitmap only Custom Edition's bitmaps.map has: white
        target = space.tags / (missing.group(1).replace("\\", "/") + ".bitmap")
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(space.tags / WHITE, target)
        space.fixed(f"stand-in: white bitmap for {missing.group(1)}.bitmap")
        return True
    missing = re.search(r"Failed to find (.*)\.sound\n", flat)
    if missing:
        silent_sound(space, missing.group(1))
        return True
    m = re.search(r"FATAL ERROR: (.*)\n\.\.\.in (\S.*)", flat)
    if not m:
        return False
    message, tag = m.group(1).strip(), m.group(2).strip()
    log(f"repairing: {message} ({tag})")
    s = re.match(r"Sequence #(\d+) in '(.*)' referenced by (.*) is out of bounds", message)
    if s:
        field = s.group(3).strip()
        key = field.replace(" ", "_") + "_sequence_index"
        indexed = re.match(r"(.*) #(\d+)$", field)
        if indexed:
            key = f"{indexed.group(1).replace(' ', '_')}[{indexed.group(2)}].sequence_index"
        space.edit(tag, "-S", key, "0")
        return True
    # a crosshair overlay past the Xbox bitmap's sequences (PC added some to
    # the shared reticles): the rocket hog the rocket launcher's (9), the
    # rest the first
    c = re.match(r"Sequence #(\d+) in '(.*)' referenced in overlay #(\d+) of crosshair #(\d+) is out of bounds",
                 message)
    if c:
        value = "9" if "rwarthog" in tag and c.group(2).endswith("hud_reticles.bitmap") else "0"
        space.edit(tag, "-S", f"crosshairs[{c.group(4)}].crosshair_overlays[{c.group(3)}].sequence_index", value)
        return True
    # a string list's text that Invader won't take (a stray null in it):
    # Invader's own repair
    if "null terminator" in message:
        space.invoke("bludgeon", "-t", space.tags, "-T", "invalid-strings", tag)
        space.fixed(f"bludgeon invalid-strings {tag}")
        return True
    # a value past the range the Xbox build allows (a Custom Edition HUD's
    # opacity above 1): the end of the range
    v = re.match(r"\w+::(\w+) must (?:be )?(no more than|no less than|at least|at most) (-?[\d.]+)", message)
    if v:
        space.edit(tag, "-S", v.group(1), v.group(3))
        return True
    # a lens flare reflection's image past its bitmap: the first
    r = re.match(r"Bitmap #(\d+) in '(.*)' referenced in reflection #(\d+) is out of bounds", message)
    if r:
        space.edit(tag, "-S", f"reflections[{r.group(3)}].bitmap_index", "0")
        return True
    # (particle systems: the N64 converter drops them)
    p = re.match(r"Particle type #(\d+) uses complex sprite render mode 'rotational'", message)
    if p:
        space.edit(tag, "-S", f"particle_types[{p.group(1)}].complex_sprite_render_mode", "simple")
        return True
    b = re.match(r"\S+ references (\w+) bitmap '(.*)' where a (\w+) was expected", message)
    if b and b.group(3) == "cube_maps" and message.startswith("map references") and \
            "shader_transparent_chicago" in tag:
        # (a chicago shader's first map as a cube map, its stand-in a 2D
        # image: drawn as a 2D map)
        space.edit(tag, "-S", "first_map_type", "2d_map")
        return True
    if b and b.group(3) == "cube_maps":
        # (a 2D image can't be a cube map: the reference cleared)
        space.edit(tag, "-S", message.split()[0], "")
        return True
    if b:
        space.edit(b.group(2), "-S", "type", b.group(3))
        return True
    # a scope with fewer images than the weapon's zoom levels
    z = re.match(r"Weapon has (\d+) zoom levels?, but the sequence referenced in crosshair overlay #\d+ of "
                 r"crosshair #\d+ only has (\d+)", message)
    if z:
        space.edit(tag, "-S", "zoom_levels", z.group(2))
        return True
    return False


def prepare_ce_map(ce_map, xbox_maps, tools, work, name=None, log=print):
    """one Custom Edition map made an Xbox map: {"map", "name", "display"}"""
    ce_map, xbox_maps, work = (Path(p).expanduser().resolve() for p in (ce_map, xbox_maps, work))
    header_name, scenario, tags = read_ce_map(ce_map)
    tools = find_tools(tools)
    if not (xbox_maps / "bloodgulch.map").is_file():
        raise CEPrepareError("the Custom Edition conversion needs the Xbox maps (bloodgulch.map and the rest)")
    name =cartridge_name(name or header_name or ce_map.stem)
    display = display_name(header_name or ce_map.stem)
    work.mkdir(parents=True, exist_ok=True)
    space = Workspace(Path(tempfile.mkdtemp(prefix=f"ce-{name}-", dir=work)), tools)
    log(f"{ce_map.name}: Custom Edition map \"{header_name}\" ({len(tags)} tags), on the cartridge as {name}")
    found = resource_sources(tags, xbox_maps, log)
    extract(space, ce_map, xbox_maps, found)
    for old, new in pc_prepare.RECIPE["conversions"]:
        if any(space.tags.rglob(f"*.{old}")):
            space.invoke("convert", "-O", "-t", space.tags, "-g", old, new, "-b", f"*.{old}")
            space.invoke("refactor", "-t", space.tags, "-M", "no-move", "-g", old, new)
    for kind in ("invalid-indices", "invalid-enums"):
        space.invoke("bludgeon", "-t", space.tags, "-T", kind, "-b", "*")
    hud = pc_prepare._normalize_hud(space.tags, space.invoke)
    sounds = pc_prepare._normalize_vorbis(space.tags, space.audio, space.invoke, space.decode_ogg)
    log(f"normalized: {len(hud)} HUD scales, {len(sounds)} Vorbis sounds made 16-bit PCM")
    apply_standins(space)
    output = build(space, scenario, name, log)
    try:
        pc_prepare.validate_xbox_map(output, name)
    except pc_prepare.PCPrepareError as error:
        raise CEPrepareError(str(error)) from error
    result = {"map": str(output), "name": name, "display": display, "source": ce_map.name}
    (work / "result.json").write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("map", type=Path, help="a Halo Custom Edition multiplayer map")
    parser.add_argument("--xbox-maps", type=Path, required=True, help="the Xbox game's maps (bloodgulch.map, ...)")
    parser.add_argument("--tools", type=Path, required=True, help="Invader's tools and ogg2wav (or ffmpeg)")
    parser.add_argument("--work", type=Path, required=True, help="a folder for the conversion")
    parser.add_argument("--name", help="the map's name on the cartridge (default: its own)")
    options = parser.parse_args(argv)
    try:
        result = prepare_ce_map(options.map, options.xbox_maps, options.tools, options.work, options.name)
    except CEPrepareError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    print(result["map"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
