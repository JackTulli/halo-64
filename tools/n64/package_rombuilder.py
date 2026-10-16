"""Packages the Halo 64 ROM Builder for Windows (PyInstaller), run with a
Windows Python that has pillow, tkinterdnd2, sv-ttk and pyinstaller:

    python tools/n64/package_rombuilder.py --tools <folder> --out <folder>
        [--elf build/n64] [--invader ~/n64/invader]
        [--pc-expansion-elf build/n64/pc-expansion --pc-tools <trusted tools>
         --pc-tool-notices <matching licence/source-offer files>]

What goes in, beside Python and the converters (tools/n64): the payload --
libdragon's tools for Windows (tools/n64/build_windows_tools.sh), each
cartridge's prebuilt program (the compressed stripped ELF and its symbols
from the N64 build: code, no Halo data), Invader's tag definitions (the
converters read them) and port/n64/port.json -- and the licences of what
it carries. The result is a folder (Halo64RomBuilder.exe beside its
_internal folder) and a zip of it.
"""

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent
NAME = "Halo64RomBuilder"
TOOLS = ("mksprite", "audioconv64", "mkdfs", "n64tool", "ed64romconfig")
CONVERTERS = ("convert", "ui", "music", "soundbank", "prune", "pc_prepare", "pc_thumbnails", "paint_masks", "decal_bank", "compress_map", "fx_pool")
PC_ASSET_TOOLS = ("mkasset",)
PC_TOOLS = tuple("invader-" + name for name in ("extract", "convert", "refactor", "bludgeon", "edit", "sound", "build")) + ("ffmpeg",)

NOTICES = """Halo 64 ROM Builder
===================

Makes the Halo 64 cartridges (Nintendo 64 ROMs) from your own copy of
Halo: Combat Evolved for the original Xbox. No Halo files come with it.

https://github.com/JackTulli/halo-64

It carries:

- the Halo 64 program for each cartridge, built from the halo-64 sources;
- libdragon's mksprite, audioconv64, mkdfs, n64tool and ed64romconfig
  (https://github.com/DragonMinded/libdragon), built for Windows
  (libdragon-LICENSE.md);
- Invader's Halo tag definitions and fixed P8 palette (https://github.com/SnowyMouse/invader,
  GPL-3.0: invader-LICENSE.md; the source is at that address);
- Python (PSF licence), Pillow (MIT-CMU), tkinterdnd2 and tkdnd (MIT),
  sv-ttk (MIT), packaged with PyInstaller.
"""


def make_icon(folder):
    """the program's icon: a "64" in a ring (no Halo art)"""
    from PIL import Image, ImageDraw, ImageFont
    size = 256
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    draw.ellipse((8, 8, size - 8, size - 8), fill=(18, 40, 30, 255), outline=(108, 203, 95, 255), width=14)
    try:
        font = ImageFont.truetype("segoeuib.ttf", 120)
    except OSError:
        font = ImageFont.load_default()
    box = draw.textbbox((0, 0), "64", font=font)
    draw.text(((size - (box[2] - box[0])) / 2 - box[0], (size - (box[3] - box[1])) / 2 - box[1]), "64",
              font=font, fill=(220, 240, 220, 255))
    png = folder / "rombuilder.png"
    img.resize((64, 64), Image.LANCZOS).save(png)
    ico = folder / "rombuilder.ico"
    img.save(ico, sizes=[(16, 16), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)])
    return png, ico


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--tools", type=Path, required=True, help="libdragon's tools for Windows (.exe)")
    parser.add_argument("--elf", type=Path, default=REPO / "build" / "n64", help="the N64 build folder")
    parser.add_argument("--invader", type=Path, default=Path.home() / "n64" / "invader")
    parser.add_argument("--pc-expansion-elf", type=Path, help="optional distinct PC expansion program folder")
    parser.add_argument("--pc-tools", type=Path, help="trusted Windows Invader + FFmpeg executables and required DLLs")
    parser.add_argument("--pc-tool-notices", type=Path, help="licence/source-offer notices matching your supplied PC tool builds")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    palette = args.invader / "src" / "bitmap" / "p8" / "p8_palette"
    if not palette.is_file() or palette.stat().st_size != 1024:
        parser.error("Invader src/bitmap/p8/p8_palette must contain exactly 1024 bytes")
    selected = (args.pc_expansion_elf, args.pc_tools, args.pc_tool_notices)
    if any(selected) and not all(selected):
        parser.error("PC expansion packaging requires --pc-expansion-elf, --pc-tools and --pc-tool-notices together")
    if args.pc_expansion_elf:
        for name in PC_ASSET_TOOLS:
            if not (args.tools / f"{name}.exe").is_file():
                parser.error(f"missing optional PC asset tool: {name}.exe (build Windows tools with --pc-expansion)")
        for name in PC_TOOLS:
            if not (args.pc_tools / f"{name}.exe").is_file():
                parser.error(f"missing supplied PC tool: {name}.exe")
        if not args.pc_tool_notices.is_dir() or not any(p.is_file() for p in args.pc_tool_notices.iterdir()):
            parser.error("--pc-tool-notices must contain the supplied tools' licence/source-offer notices")
    stage = Path(tempfile.mkdtemp(prefix="halo64-package-"))
    try:
        # the converters, a regular package (tools/ is a namespace package
        # in the repository)
        source = stage / "src"
        (source / "tools").mkdir(parents=True)
        (source / "tools" / "__init__.py").write_text("", encoding="utf-8")
        shutil.copytree(REPO / "tools" / "n64", source / "tools" / "n64",
                        ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.sh"))
        # the payload
        payload = stage / "payload"
        (payload / "bin").mkdir(parents=True)
        for name in TOOLS:
            shutil.copy2(args.tools / f"{name}.exe", payload / "bin" / f"{name}.exe")
        if args.pc_expansion_elf:
            for name in PC_ASSET_TOOLS:
                shutil.copy2(args.tools / f"{name}.exe", payload / "bin" / f"{name}.exe")
        programs = [("multiplayer", args.elf), ("campaign", args.elf / "campaign")]
        if args.pc_expansion_elf:
            programs.append(("pc-expansion", args.pc_expansion_elf))
        for cartridge, folder in programs:
            target = payload / "elf" / cartridge
            target.mkdir(parents=True)
            for name in ("halo.elf.stripped", "halo.elf.sym"):
                shutil.copy2(folder / name, target / name)
        definitions = payload / "invader" / "src" / "tag" / "hek" / "definition"
        shutil.copytree(args.invader / "src" / "tag" / "hek" / "definition", definitions)
        # bitmaps._p8_palette resolves this relative to the definitions tree.
        palette_target = payload / "invader" / "src" / "bitmap" / "p8" / "p8_palette"
        palette_target.parent.mkdir(parents=True)
        shutil.copy2(palette, palette_target)
        fourcc = payload / "invader" / "include" / "invader" / "hek"
        fourcc.mkdir(parents=True)
        shutil.copy2(args.invader / "include" / "invader" / "hek" / "fourcc.hpp", fourcc / "fourcc.hpp")
        shutil.copy2(REPO / "port" / "n64" / "port.json", payload / "port.json")
        art = payload / "art"
        art.mkdir()
        shutil.copy2(REPO / "assets" / "paintball" / "paint-splatter-original.png", art / "paint-splatter-original.png")
        if args.pc_tools:
            pc_tools = payload / "pc-tools"
            pc_tools.mkdir()
            supplied = [args.pc_tools / f"{name}.exe" for name in PC_TOOLS]
            supplied += sorted(p for p in args.pc_tools.iterdir() if p.suffix.lower() == ".dll" and p.is_file())
            inventory = []
            for binary in supplied:
                target_binary = pc_tools / binary.name
                shutil.copy2(binary, target_binary)
                inventory.append({"name": binary.name, "bytes": binary.stat().st_size,
                                  "sha256": hashlib.sha256(binary.read_bytes()).hexdigest()})
            (payload / "pc-tools.json").write_text(json.dumps(inventory, indent=2), encoding="utf-8")
        png, ico = make_icon(payload)
        notices = stage / "notices"
        notices.mkdir()
        (notices / "README.txt").write_text(NOTICES, encoding="utf-8")
        shutil.copy2(args.invader / "LICENSE.md", notices / "invader-LICENSE.md")
        if args.pc_tool_notices:
            shutil.copytree(args.pc_tool_notices, notices / "pc-tools")
            with (notices / "README.txt").open("a", encoding="utf-8") as readme:
                readme.write("\nOptional PC expansion: includes supplied libdragon mkasset, Invader executables and FFmpeg. "
                             "See pc-tools/ for their matching licences and source information. "
                             "Only code, tools and project-original paint art are bundled; supply your own Halo PC maps.\n")
        libdragon_license = args.tools / "libdragon-LICENSE.md"
        if libdragon_license.is_file():
            shutil.copy2(libdragon_license, notices / "libdragon-LICENSE.md")

        command = [
            sys.executable, "-m", "PyInstaller", "--noconfirm", "--clean", "--windowed",
            "--name", NAME, "--icon", str(ico),
            "--paths", str(source),
            "--distpath", str(stage / "dist"), "--workpath", str(stage / "build"), "--specpath", str(stage),
            "--add-data", f"{payload}{os.pathsep}payload",
            "--collect-all", "tkinterdnd2", "--collect-all", "sv_ttk",
            "--collect-submodules", "tools.n64",
            *[f"--hidden-import=tools.n64.{m}" for m in CONVERTERS],
            str(source / "tools" / "n64" / "rombuilder_main.py"),
        ]
        subprocess.run(command, check=True)
        built = stage / "dist" / NAME
        for path in notices.iterdir():
            if path.is_dir():
                shutil.copytree(path, built / path.name)
            else:
                shutil.copy2(path, built / path.name)
        args.out.mkdir(parents=True, exist_ok=True)
        target = args.out / NAME
        if target.is_symlink() or not target.resolve().is_relative_to(args.out.resolve()):
            raise ValueError("package output must remain inside the selected output folder")
        if target.exists():
            shutil.rmtree(target)
        shutil.copytree(built, target)
        archive = shutil.make_archive(str(args.out / NAME), "zip", args.out, NAME)
        print(f"packaged: {target} and {archive}")
    finally:
        shutil.rmtree(stage, ignore_errors=True)


if __name__ == "__main__":
    main()
