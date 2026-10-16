"""Derive Xbox v5 converter intermediates from owned retail PC v7 maps.

Only the explicitly supplied tool directory is trusted. Every invocation gets a
new workspace; original maps and tools are read-only inputs. This module never
executes an installer, downloads tools, or reuses a size-based conversion cache.
"""

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import zlib

from .pc_input import PCMapSource

EXTRAS = ("dangercanyon", "deathisland", "gephyrophobia", "icefields", "infinity", "timberland")
TOOL_NAMES = tuple(f"invader-{name}.exe" for name in
                   ("extract", "convert", "refactor", "bludgeon", "edit", "sound", "build")) + ("ffmpeg.exe",)
UI_BITMAP = Path("ui/shell/bitmaps/mp_map_grafix.bitmap")
RECIPE = {
    "id": "halo-retail-pc-to-xbox-intermediate-v2",
    "input_version": 7, "output_version": 5, "maps": EXTRAS,
    "conversions": [["gbxmodel", "model"],
                    ["shader_transparent_chicago_extended", "shader_transparent_chicago"]],
    "repairs": ["invalid-indices", "invalid-enums", "scorpion-headlight-index-0"],
    "hud": "clear use_high_res_scale and halve explicit width/height (zero defaults to one)",
    "audio": "embedded complete Vorbis streams -> ffmpeg pcm_s16le -> lossless Invader 16-bit_pcm, jobs=2",
    "build": ["-g", "xbox-ntsc", "-E", "-H"],
    "ui": str(UI_BITMAP).replace("\\", "/"),
}
_MAX_SOUND = 128 << 20
_MAX_MAP = (2 << 30) - 1
_MAX_ITEMS = 65536


class PCPrepareError(ValueError):
    """PC normalization failed; suitable for the installer's error display."""


def validate_tools(tools):
    """Validate the explicit trusted tool directory without running executables."""
    if tools is None:
        raise PCPrepareError("Select a trusted PC conversion tools directory containing Invader and ffmpeg.")
    folder = Path(tools).expanduser().resolve()
    missing = [name for name in TOOL_NAMES if not (folder / name).is_file()]
    if missing:
        raise PCPrepareError("Missing PC conversion tools in the selected trusted tools directory: " + ", ".join(missing))
    return folder


def _digest(path):
    with path.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"file": path.name, "bytes": path.stat().st_size, "sha256": digest}


def _component(value):
    """An extracted tag's names must never become absolute/escaping paths."""
    if (not value or len(value) > 128 or value in (".", "..") or
            value != value.strip() or value.endswith(".") or
            any(ord(c) < 32 or c in '<>:"/\\|?*' for c in value) or
            re.fullmatch(r"(?i)(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\..*)?", value)):
        raise PCPrepareError(f"Unsafe sound pitch/permutation name: {value!r}")
    return value


def _tag_files(tags, extension):
    for path in sorted(tags.rglob("*." + extension)):
        if path.is_symlink() or not path.is_file() or not path.resolve().is_relative_to(tags.resolve()):
            raise PCPrepareError(f"Extracted tag escapes the work directory: {path}")
        yield path


def _ogg_streams(data):
    """Read bounded complete, unmultiplexed Ogg streams from a sound tag."""
    if len(data) > _MAX_SOUND:
        raise PCPrepareError("Sound tag exceeds the supported 128 MiB size.")
    streams, pages, cursor, serial, sequence = [], bytearray(), 0, None, None
    while True:
        start = data.find(b"OggS", cursor)
        if start < 0:
            break
        if start + 27 > len(data) or data[start + 4] != 0:
            raise PCPrepareError("Invalid Ogg page header in sound tag.")
        flags = data[start + 5]
        table_end = start + 27 + data[start + 26]
        if table_end > len(data):
            raise PCPrepareError("Truncated Ogg lacing table in sound tag.")
        end = table_end + sum(data[start + 27:table_end])
        if end > len(data):
            raise PCPrepareError("Truncated Ogg page in sound tag.")
        page_serial, page_sequence = struct.unpack_from("<II", data, start + 14)
        if not pages:
            if not flags & 2 or flags & 1:
                raise PCPrepareError("Ogg stream is missing its beginning page.")
            serial, sequence = page_serial, page_sequence
        elif start != cursor or flags & 2 or page_serial != serial or page_sequence != (sequence + 1) & 0xFFFFFFFF:
            raise PCPrepareError("Split or multiplexed Ogg stream requires manual conversion.")
        if flags & ~7:
            raise PCPrepareError("Invalid Ogg page flags.")
        sequence = page_sequence
        pages.extend(data[start:end])
        if flags & 4:
            streams.append(bytes(pages))
            pages.clear()
            if len(streams) > _MAX_ITEMS:
                raise PCPrepareError("Too many Ogg streams in sound tag.")
        cursor = end
    if pages:
        raise PCPrepareError("Unterminated Ogg stream in sound tag.")
    return streams


def _count(value, field):
    try:
        number = int(value)
    except ValueError as error:
        raise PCPrepareError(f"Invalid count for {field}: {value!r}") from error
    if not 0 <= number <= _MAX_ITEMS:
        raise PCPrepareError(f"Count exceeds the supported limit for {field}.")
    return number


def _normalize_hud(tags, invoke):
    kinds = (("unit_hud_interface", "UnitHUDInterface"),
             ("weapon_hud_interface", "WeaponHUDInterface"),
             ("grenade_hud_interface", "GrenadeHUDInterface"))
    files = [(list(_tag_files(tags, extension)), structure) for extension, structure in kinds]
    if not any(paths for paths, _ in files):
        return []
    from .tagdefs import TagDefinitions
    definitions = TagDefinitions()
    memo, changes = {}, []

    def contains(name):
        if name not in memo:
            memo[name] = False
            memo[name] = any(field.type == "HUDInterfaceScalingFlags" or
                             (field.kind in ("struct", "TagReflexive") and contains(field.struct))
                             for _, field in definitions.fields(name))
        return memo[name]

    for paths, structure in files:
        for path in paths:
            tag = str(path.relative_to(tags)).replace("/", "\\")

            def visit(name, prefix="", depth=0):
                if depth > 64:
                    raise PCPrepareError("HUD definition nesting exceeds the supported limit.")
                for _, field in definitions.fields(name):
                    key = prefix + field.name.replace(" ", "_")
                    if field.type == "HUDInterfaceScalingFlags":
                        bit = key + ".use_high_res_scale"
                        flag = invoke("edit", "-t", tags, tag, "-G", bit)
                        if flag not in ("0", "1"):
                            raise PCPrepareError(f"Invalid HUD flag {tag}: {bit}={flag!r}")
                        if flag == "1":
                            stem = key[:-len("scaling_flags")]
                            edits = ["-S", bit, "0"]
                            for axis in ("width", "height"):
                                key_scale = stem + axis + "_scale"
                                old = float(invoke("edit", "-t", tags, tag, "-G", key_scale))
                                if not math.isfinite(old):
                                    raise PCPrepareError(f"Non-finite HUD scale in {tag}.")
                                edits.extend(("-S", key_scale, str((old or 1.0) * 0.5)))
                            invoke("edit", "-t", tags, tag, *edits)
                            changes.append({"tag": tag, "flag": bit, "scale": "half"})
                    elif field.kind in ("struct", "TagReflexive") and contains(field.struct):
                        if field.kind == "struct":
                            visit(field.struct, key + ".", depth + 1)
                        else:
                            count = _count(invoke("edit", "-t", tags, tag, "-C", key), key)
                            for index in range(count):
                                visit(field.struct, key + f"[{index}].", depth + 1)
            visit(structure)
    return changes


def _normalize_vorbis(tags, audio, invoke, run_ffmpeg):
    changed = []
    for path in _tag_files(tags, "sound"):
        if path.stat().st_size > _MAX_SOUND:
            raise PCPrepareError(f"Sound tag exceeds the supported size: {path.name}")
        data = path.read_bytes()
        if b"OggS" not in data:
            continue
        streams = _ogg_streams(data)
        tag = str(path.relative_to(tags)).replace("/", "\\")
        count = _count(invoke("edit", "-t", tags, tag, "-C", "pitch_ranges"), "pitch_ranges")
        permutations = []
        for pitch in range(count):
            prefix = f"pitch_ranges[{pitch}]"
            name = _component(invoke("edit", "-t", tags, tag, "-G", prefix + ".name"))
            total = _count(invoke("edit", "-t", tags, tag, "-C", prefix + ".permutations"), prefix)
            if len(permutations) + total > _MAX_ITEMS:
                raise PCPrepareError("Sound permutation count exceeds the supported limit.")
            for permutation in range(total):
                pn = _component(invoke("edit", "-t", tags, tag, "-G", prefix + f".permutations[{permutation}].name"))
                permutations.append((name, pn))
        identities = [(a.casefold(), b.casefold()) for a, b in permutations]
        if len(streams) != len(permutations) or len(set(identities)) != len(identities):
            raise PCPrepareError(f"Split or mixed-codec sound requires manual conversion: {tag}")
        target = audio / path.relative_to(tags).with_suffix("")
        for (pitch, name), ogg in zip(permutations, streams):
            destination = target / pitch / (name + ".wav")
            if not destination.resolve().is_relative_to(audio.resolve()):
                raise PCPrepareError("Sound output escapes the work directory.")
            destination.parent.mkdir(parents=True, exist_ok=True)
            run_ffmpeg(ogg, destination)
        # These are converter intermediates: keep the decoded samples lossless
        # and bypass the external Xbox ADPCM encoder entirely.
        invoke("sound", "-t", tags, "-d", audio, "-F", "16-bit_pcm", "-j", "2", tag[:-6])
        changed.append(tag)
    return changed


def validate_xbox_map(path, name):
    """Validate actual v5 compressed data and tag bounds without a huge allocation."""
    if path.is_symlink() or not path.is_file() or not 0x800 < path.stat().st_size <= _MAX_MAP:
        raise PCPrepareError(f"Missing or invalid Xbox intermediate: {path}")
    with path.open("rb") as stream:
        header = stream.read(0x800)
        if header[:4] != b"daeh" or header[-4:] != b"toof":
            raise PCPrepareError(f"Invalid Xbox header/footer: {path.name}")
        version, declared, padding, offset, size = struct.unpack_from("<5I", header, 4)
        if version != 5:
            raise PCPrepareError(f"{path.name}: expected Xbox cache version 5, got {version}.")
        raw_name = header[0x20:0x40]
        if b"\0" not in raw_name or raw_name.split(b"\0", 1)[0] != name.encode("ascii"):
            raise PCPrepareError(f"Xbox intermediate has the wrong map name: {path.name}")
        if not 0x800 < declared <= _MAX_MAP or offset < 0x800 or size < 36 or offset + size > declared or padding > 4095:
            raise PCPrepareError(f"Xbox intermediate has invalid tag-data bounds: {path.name}")
        decompressor, actual, tag_header, trailing = zlib.decompressobj(), 0x800, bytearray(), 0
        try:
            for block in iter(lambda: stream.read(1 << 20), b""):
                if decompressor.eof:
                    trailing += len(block)
                    continue
                while block:
                    output = decompressor.decompress(block, 1 << 20)
                    low, high = max(actual, offset), min(actual + len(output), offset + 36)
                    if low < high:
                        tag_header.extend(output[low - actual:high - actual])
                    actual += len(output)
                    if actual > declared:
                        raise PCPrepareError(f"Xbox intermediate exceeds its declared length: {path.name}")
                    if decompressor.eof:
                        trailing += len(decompressor.unused_data)
                        break
                    block = decompressor.unconsumed_tail
        except zlib.error as error:
            raise PCPrepareError(f"Invalid Xbox compressed stream: {path.name}") from error
        if not decompressor.eof or actual != declared or trailing != padding or len(tag_header) != 36:
            raise PCPrepareError(f"Truncated or inconsistent Xbox intermediate: {path.name}")
        address, scenario, _, count = struct.unpack_from("<4I", tag_header)
        relative = address - 0x803A6000
        if (tag_header[32:] != b"sgat" or not 0 < count <= 65535 or relative < 36 or
                relative + count * 32 > size or (scenario & 0xFFFF) >= count):
            raise PCPrepareError(f"Invalid Xbox tag table bounds: {path.name}")


def prepare_pc_maps(maps, work, tools, log=print):
    """Return a fresh ``maps`` directory; UI tags and manifest are its siblings.

    ``maps`` must be staged retail PC maps with all six expansion maps and ui.
    bitmaps.map and sounds.map, when available, remain beside them for Invader.
    ``tools`` is the explicit trusted directory containing all TOOL_NAMES.
    Failed workspaces are retained for diagnosis; no user directory is deleted.
    """
    tools = validate_tools(tools)
    maps, work = (Path(value).expanduser().resolve() for value in (maps, work))
    if not maps.is_dir():
        raise PCPrepareError("PC normalization requires a staged retail Maps directory.")
    source = PCMapSource(maps)
    source.validate((*EXTRAS, "ui"))
    # A dedicated work root is required; never create derived data in inputs.
    if work == maps or work.is_relative_to(maps) or work == tools or work.is_relative_to(tools):
        raise PCPrepareError("The PC work directory must be outside the source maps and trusted tools.")
    work.mkdir(parents=True, exist_ok=True)
    root = Path(tempfile.mkdtemp(prefix="pc-prepare-", dir=work))
    tags, audio, output, ui_tags, logs = (root / name for name in ("tags", "sound-source", "maps", "ui-tags", "logs"))
    for folder in (tags, audio, output, ui_tags, logs):
        folder.mkdir()
    manifest_path = root / "manifest.json"
    manifest = {"recipe": RECIPE, "source": str(maps), "tools_directory": str(tools),
                "inputs": [_digest(path) for path in sorted(maps.glob("*.map"))],
                "tools": [_digest(tools / name) for name in TOOL_NAMES], "commands": [], "status": "running"}

    def save():
        manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    def run(executable, args, data=None):
        command = [str(tools / executable), *map(str, args)]
        manifest["commands"].append(command)
        options = {"capture_output": True, "cwd": str(root)}
        if os.name == "nt":
            options["creationflags"] = subprocess.CREATE_NO_WINDOW
        if data is None:
            options["stdin"] = subprocess.DEVNULL
        else:
            options["input"] = data
        result = subprocess.run(command, **options)
        combined = result.stdout + result.stderr
        (logs / f"{len(manifest['commands']):04d}-{Path(executable).stem}.log").write_bytes(combined)
        if result.returncode:
            raise PCPrepareError(f"{executable} failed (exit {result.returncode}): " + combined.decode(errors="replace")[-4000:])
        return result.stdout.decode(errors="replace").strip()

    def invoke(tool, *args):
        return run(f"invader-{tool}.exe", args)

    def ffmpeg(ogg, destination):
        run("ffmpeg.exe", ("-hide_banner", "-loglevel", "error", "-y", "-i", "pipe:0", "-acodec", "pcm_s16le", destination), ogg)

    save()
    try:
        invoke("extract", "-O", "-m", maps, "-t", ui_tags, "-s", str(UI_BITMAP).replace("/", "\\"), maps / "ui.map")
        bitmap = ui_tags / UI_BITMAP
        if not bitmap.is_file() or bitmap.is_symlink() or not bitmap.resolve().is_relative_to(ui_tags.resolve()):
            raise PCPrepareError("PC ui.map did not produce the required multiplayer thumbnail bitmap.")
        for name in EXTRAS:
            invoke("extract", "-O", "-m", maps, "-t", tags, maps / f"{name}.map")
            log(f"Extracted PC expansion map {name}")
        for old, new in RECIPE["conversions"]:
            invoke("convert", "-O", "-t", tags, "-g", old, new, "-b", f"*.{old}")
            invoke("refactor", "-t", tags, "-M", "no-move", "-g", old, new)
        for kind in ("invalid-indices", "invalid-enums"):
            invoke("bludgeon", "-t", tags, "-T", kind, "-b", "*")
        invoke("edit", "-t", tags, "vehicles\\scorpion\\headlights scorpion.lens_flare", "-S", "reflections[0].bitmap_index", "0")
        manifest["hud_changes"] = _normalize_hud(tags, invoke)
        manifest["sound_changes"] = _normalize_vorbis(tags, audio, invoke, ffmpeg)
        for name in EXTRAS:
            target = output / (name + ".map")
            # These exceed Xbox cache limits before N64 downsampling/pruning;
            # they are converter intermediates, never intended to boot on Xbox.
            invoke("build", *RECIPE["build"], "-t", tags, "-o", target, f"levels\\test\\{name}\\{name}")
            validate_xbox_map(target, name)
            log(f"Built Xbox intermediate {name}")
        if manifest["inputs"] != [_digest(path) for path in sorted(maps.glob("*.map"))]:
            raise PCPrepareError("PC source maps changed during normalization; retry with unchanged inputs.")
        manifest["outputs"] = [_digest(output / (name + ".map")) for name in EXTRAS]
        manifest["ui_bitmap"] = _digest(bitmap)
        manifest["status"] = "complete"
        save()
        return output
    except Exception as error:
        manifest["status"] = "failed"
        manifest["error"] = str(error)
        save()
        raise


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("maps", type=Path, help="staged retail PC Maps directory")
    parser.add_argument("--work", required=True, type=Path, help="directory for fresh derived workspaces")
    parser.add_argument("--tools", required=True, type=Path, help="explicit trusted Invader/ffmpeg directory")
    options = parser.parse_args(argv)
    output = prepare_pc_maps(options.maps, options.work, options.tools)
    result = {"maps": str(output.resolve()), "ui_bitmap": str((output.parent / "ui-tags" / UI_BITMAP).resolve())}
    with tempfile.NamedTemporaryFile(mode="w", dir=options.work, prefix=".pc-result-", suffix=".json", encoding="utf-8", delete=False) as stream:
        json.dump(result, stream, indent=2)
        stream.write("\n")
        result_temporary = Path(stream.name)
    os.replace(result_temporary, options.work / "result.json")
    print(output)


if __name__ == "__main__":
    main()
