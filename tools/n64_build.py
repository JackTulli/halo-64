#!/usr/bin/env python3
"""Ninja file for the Nintendo 64 build (port/n64).

    python3 tools/n64_build.py        # writes build/n64/build.ninja
    ninja -f build/n64/build.ninja    # build/n64/halo.z64 (from the repo root)

The game sources are the matching project's own (config/config.json,
project halobetacache), less the subsystems the N64 leaves out
(port/n64/port.json "exclude_dirs"/"exclude_sources"), compiled with
libdragon's MIPS GCC for the big-endian VR4300. The platform layer
(port/n64/src) is ordinary libdragon code. See port/n64/README.md.
"""

import argparse
import fnmatch
import json
import os
import shlex
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT))
from tools.ninja_syntax import Writer, escape_path  # noqa: E402

def posix(value):
    return str(value).replace("\\", "/")


def q(value):
    """Quote one literal path/argument for the POSIX shell used by Ninja/MSYS."""
    return shlex.quote(posix(value).replace("\\", "/")).replace("$", "$$")


N64_INST = Path(os.environ.get("N64_INST", Path.home() / "n64" / "toolchain"))
PORT_DIR = Path("port/n64")
BUILD_DIR = Path("build/n64")
XDK_INCLUDE = Path("port/include/xdk")
TINY3D = Path(os.environ.get("TINY3D", Path.home() / "n64" / "tiny3d"))

# The installer and archive/PC normalization tools produce the explicit input
# files before conversion. They are not imported by the converter dependency
# closure and must not invalidate every converted level when their UI changes.
NON_CONVERTER_TOOLS = frozenset({"package_rombuilder.py", "pc_input.py", "pc_prepare.py", "xiso.py", "fx_pool.py"})


def converter_source(path):
    return path.name not in NON_CONVERTER_TOOLS and not path.name.startswith(("rombuilder", "test_"))


# The MSVC/Xbox environment of the game (see tools/linux_build.py for the
# reasons behind each): 16-bit wchar_t, C89 with common tentative
# definitions, MSVC's tolerance of aliasing, overflow and null checks, small
# structures returned in registers (hs_runtime.c), no fused multiply-adds.
# Retain the early hardware multiplier workaround for physical cartridges.
# No padding before loops, jump targets and labels: the VR4300 issues
# one instruction a cycle from its 16 KB instruction cache, so aligning them
# to 8 bytes gains nothing and the padding takes cache lines (a miss is 48
# cycles in ares).
CODE_FLAGS = [
    "-mfix4300",
    "-falign-jumps=1", "-falign-loops=1", "-falign-labels=1",
]

GAME_ABI = [
    "-march=vr4300", "-mtune=vr4300", "-mabi=o64",
    "-G0",
    "-fms-extensions",
    "-fshort-wchar",
    "-fcommon",
    "-fno-strict-aliasing",
    "-fwrapv",
    "-fno-delete-null-pointer-checks",
    "-freg-struct-return",
    "-ffp-contract=off",
    "-fno-math-errno",
    "-fsigned-char",
    "-ffunction-sections", "-fdata-sections",
    "-falign-functions=32",
    *CODE_FLAGS,
    "-g",
    *(f"-fno-builtin-{name}" for name in (
        "wcslen", "wcsnlen", "wcschr", "wcsrchr", "wcscmp", "wcsncmp", "wcscpy",
        "wcsncpy", "wcscat", "wcsncat", "wmemchr", "wmemcmp", "wmemcpy",
        "wmemmove", "wmemset",
    )),
]

GAME_FLAGS = [
    "-std=gnu89",
    "-D__STRICT_ANSI__",
    "-w",
    "-Wno-error=incompatible-pointer-types",
    "-Wno-error=int-conversion",
    "-Wno-error=implicit-function-declaration",
    "-Wno-error=implicit-int",
    "-Wno-error=return-type",
    "-Wno-error=return-mismatch",
    "-Wno-error=declaration-missing-parameter-type",
]

# libdragon's own flags (n64.mk), for the platform layer
PLATFORM_FLAGS = [
    "-march=vr4300", "-mtune=vr4300", "-mabi=o64", "-G0",
    q(f"-I{N64_INST}/mips64-elf/include/newlib_overrides") if (N64_INST / "mips64-elf/include/newlib_overrides").is_dir() else "",
    q(f"-I{N64_INST}/include"),
    q(f"-I{N64_INST}/mips64-elf/include"),
    "-include", "ktls.h",
    "-falign-functions=32",
    *CODE_FLAGS,
    "-ffunction-sections", "-fdata-sections", "-g",
    "-std=gnu17",
    "-DN64", "-DLIBDRAGON_PREVIEW=2",
    "-Wall", "-Wno-unused-function", "-Wno-unused-variable",
]


def load_projects():
    config = json.load(open(ROOT / "config" / "config.json", encoding="utf-8"))
    return {p["name"]: p for p in config["projects"]}


def main(argv=None):
    global BUILD_DIR
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--pc-expansion", action="store_true", help="also generate the separate PC multiplayer cartridge")
    parser.add_argument("--pc-maps", type=Path, help="explicit folder containing all six normalized Xbox-v5 PC maps")
    parser.add_argument("--pc-ui-bitmap", type=Path, help="owned extracted HEK mp_map_grafix.bitmap with the PC thumbnails")
    parser.add_argument("--definitions", type=Path, default=Path(os.environ.get("INVADER_DEFINITIONS", "dependencies/invader/src/tag/hek/definition")))
    parser.add_argument("--build-dir", type=Path, default=BUILD_DIR, help="generated artifacts (default build/n64; Ninja execution uses a POSIX/MSYS shell)")
    args = parser.parse_args(argv)
    os.chdir(ROOT)
    BUILD_DIR = args.build_dir
    from tools.n64.convert import validate_pc_maps, PC_MAPS
    try:
        pc_maps = validate_pc_maps(args.pc_expansion, args.pc_maps)
    except (ValueError, OSError) as error:
        parser.error(str(error))
    if args.pc_expansion:
        if args.pc_ui_bitmap is None or not args.pc_ui_bitmap.is_file():
            parser.error("--pc-expansion requires --pc-ui-bitmap with the extracted owned PC menu bitmap")
        args.pc_ui_bitmap = args.pc_ui_bitmap.resolve()
    elif args.pc_ui_bitmap is not None:
        parser.error("--pc-ui-bitmap requires --pc-expansion")
    definitions = args.definitions.resolve()
    p8_palette = posix(definitions.parents[2] / "bitmap" / "p8" / "p8_palette")
    port = json.load(open(PORT_DIR / "port.json", encoding="utf-8"))
    opt = port.get("optimisation", "-Os")
    # the game's units (and the XDK layer compiled with them) may have a
    # level of their own ("game_optimisation"). The cartridge platform
    # retains its separately selected optimisation (rendering is speed-sensitive).
    game_opt = port.get("game_optimisation", opt)
    platform_opt = port.get("platform_optimisation", opt)
    # small data ("small_data": bytes, -G): globals up to that size in
    # .sdata/.sbss, reached from $gp in one instruction instead of two
    # (libdragon itself is built with the compiler's default, 8); 0 keeps
    # every global out of it
    small = f"-G{int(port.get('small_data', 0))}"
    global GAME_ABI, PLATFORM_FLAGS
    GAME_ABI = [small if flag == "-G0" else flag for flag in GAME_ABI]
    PLATFORM_FLAGS = [small if flag == "-G0" else flag for flag in PLATFORM_FLAGS]
    projects = load_projects()
    project = projects["halobetacache"]
    options = project.get("options", {})
    exclude_dirs = [d.rstrip("/") + "/" for d in port.get("exclude_dirs", [])]
    exclude_sources = set(port.get("exclude_sources", []))

    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    out = open(BUILD_DIR / "build.ninja", "w", encoding="utf-8")
    n = Writer(out)
    n.comment("generated by tools/n64_build.py - do not edit")
    n.variable("builddir", posix(BUILD_DIR))
    n.variable("root", q(ROOT))
    n.variable("bin", q(N64_INST / "bin"))
    n.variable("n64inst", q(N64_INST))
    n.variable("cc", q(N64_INST / "bin" / "mips64-elf-gcc"))
    n.variable("python", q(os.environ.get("N64_PYTHON", sys.executable)))
    n.variable("definitions", q(definitions))
    n.newline()

    n.rule("cc", command="cd $root && $cc -MMD -MF $out.d $cflags -c $in -o $out",
           description="N64 CC $in", depfile="$out.d", deps="gcc")
    n.rule("semantics",
           command="cd $root && $python tools/linux_msvc_semantics.py --output $out $scan",
           description="N64 MSVC SEMANTICS $out", restat=True)
    n.rule("link",
           command=("cd $root && export N64_INST=$n64inst && $cc -march=vr4300 -mtune=vr4300 -mabi=o64 -g -o $out @$out.rsp "
                    "-L$libdir $ldlibs -lc -Wl,-L$libdir -Wl,-Tn64.ld -Wl,--gc-sections "
                    "-Wl,--wrap,__do_global_ctors -Wl,-Map=$out.map $ldflags "
                    "&& $python tools/n64_link_check.py $objlist --check"),
           description="N64 LINK $out", rspfile="$out.rsp", rspfile_content="$in_newline")
    n.rule("z64",
           command=("cd $root && export N64_INST=$n64inst INVADER_DEFINITIONS=$definitions && $bin/n64sym --all $in $in.sym && cp $in $in.stripped && "
                    "$bin/mips64-elf-strip -s $in.stripped && "
                    "$bin/n64elfcompress -o $outdir -c $rom_elf_compression $in.stripped && "
                    "$bin/n64tool --toc --title \"$title\" --output $out.tmp --align 256 $in.stripped $in.sym $extra && "
                    "$bin/ed64romconfig --expansionpak required --savetype eeprom4k --regionfree $out.tmp && mv $out.tmp $out"),
           description="N64 ROM $out")
    n.newline()

    semantics = BUILD_DIR / "halo_msvc_semantics.h"
    game_headers = sorted(posix(p) for p in Path("source").rglob("*") if p.suffix in (".c", ".h"))
    n.build(posix(semantics), "semantics",
            implicit=["tools/linux_msvc_semantics.py", *map(posix, sorted(XDK_INCLUDE.glob("*.h"))), *game_headers],
            variables={"scan": f"--all-inlines --exclude-static --tags source --inlines source --inlines {XDK_INCLUDE.as_posix()}"})

    base_defines = " ".join(f"-D{d}" for d in options.get("defines") or [] if d not in port.get("drop_defines", []))
    base_defines += " " + " ".join(f"-D{d}" for d in port.get("defines", []))
    includes = " ".join(
        "-I" + shlex.quote(d) for d in options.get("include_dirs") or [] if Path(d) != Path("xbox/include"))
    # link-time optimisation of the game's units (port.json "lto"): the
    # decompiled code calls small getters across files on every object
    # (datum_get, tag_get, tag_block_get_element_with_size...); fat objects
    # keep the plain symbols tools/n64_link_check.py reads
    lto = ["-flto", "-ffat-lto-objects"] if port.get("lto") else []
    # extra flags by source: a path, or a pattern ("source/physics/*.c");
    # every matching entry's flags are added, in port.json's order (a later
    # -O wins over the unit's own)
    source_flags = port.get("source_flags", {})

    def add(objects, obj_dir, source, cflags):
        name = posix(source).replace(os.sep, "/")
        extra = " ".join(flags for pattern, flags in source_flags.items()
                         if pattern == name or fnmatch.fnmatchcase(name, pattern))
        if extra:
            cflags = f"{cflags} {extra}"
        obj = obj_dir / Path(source).with_suffix(".o")
        objects.append(posix(obj))
        n.build(posix(obj), "cc", posix(source), implicit=[posix(semantics)],
                variables={"cflags": cflags})

    # the game's sin, pow and the rest (port/include/halo_math.h): the same
    # objects for every cartridge
    musl_objects = []
    musl = Path("port/third_party/musl-math")
    musl_cflags = " ".join([*GAME_ABI, "-O2", "-std=gnu11", "-w", f"-I{musl.as_posix()}/include",
                            f"-include {musl.as_posix()}/include/libm.h"])
    for source in sorted((musl / "src").glob("*.c")):
        add(musl_objects, BUILD_DIR / "obj", source, musl_cflags)

    # the cartridge's files (DragonFS): the converted maps, and the flags
    # port.json asks for (an "autotest" file drives the controllers by script)
    # the converters take ~800 MB each: three at a time (thirteen at once,
    # with the link, ran a 15 GB machine out of memory and froze it)
    n.pool("convert", 1 if sys.platform == "win32" or os.environ.get("N64_SERIAL_CONVERT") else 3)
    n.rule("convert_map",
           command="cd $root && export N64_INST=$n64inst INVADER_DEFINITIONS=$definitions && $python -m tools.n64.convert $in -o $map --filesystem $fsdir $convert_flags && $python -m tools.n64.compress_map $map --backup $map_backup",
           pool="convert",
           description="N64 MAP $out")
    n.rule("convert_ui", command="cd $root && export N64_INST=$n64inst INVADER_DEFINITIONS=$definitions && $python -m tools.n64.ui $in --filesystem $fsdir",
           pool="convert",
           description="N64 MENU $out")
    n.rule("convert_music", command="cd $root && export INVADER_DEFINITIONS=$definitions && $python -m tools.n64.music $in --filesystem $fsdir",
           pool="convert",
           description="N64 MUSIC $out")
    n.rule("sound_bank", command="cd $root && $python -m tools.n64.soundbank $variantdir $fsdir $maps",
           description="N64 SOUNDS $out")
    n.rule("touch", command="cd $root && mkdir -p $$(dirname $out) && : > $out", description="N64 FLAG $out")
    n.rule("mkdfs", command="cd $root && $python -m tools.n64.prune $fsdir $maps && $bin/mkdfs $out $fsdir > /dev/null && : $in $flags",
           description="N64 DFS $out")
    # Shared packing records the complete copied tree. Never mutate that owned
    # inventory afterward: default/campaign retain their historical pruning rule.
    n.rule("mkdfs_packed", command="cd $root && $bin/mkdfs $out $fsdir > /dev/null && : $in $flags",
           description="N64 PC DFS $out")
    # Packing is an independent, serialized PC-only step after raw assets exist.
    # Its helper must not invalidate the expensive per-map conversions.
    n.pool("pack", 1)
    n.rule("fx_pool", command="cd $root && $python -m tools.n64.fx_pool --filesystem $source_fs --output $packed_fs --maps $maps --stamp $stamp",
           pool="pack", description="N64 PC SHARED FX $packed_fs", restat=True)
    n.rule("paint_masks", command="cd $root && export INVADER_DEFINITIONS=$definitions && $python -m tools.n64.paint_masks --map $source_map --output $out", description="N64 PAINT $out")
    n.rule("decal_bank", command="cd $root && export INVADER_DEFINITIONS=$definitions && $python -m tools.n64.decal_bank --maps $sources --output $out $convert_flags", pool="convert", description="N64 DECALS $out")
    n.rule("pc_thumbnails", command="cd $root && export N64_INST=$n64inst INVADER_DEFINITIONS=$definitions && $python -m tools.n64.pc_thumbnails --bitmap $bitmap --output $outputdir", description="N64 PC MAP PICTURES")
    converter_tools = [posix(N64_INST / "bin" / (name + (".exe" if sys.platform == "win32" else ""))) for name in ("mksprite", "audioconv64", "mkasset")]
    tool_sources = sorted(posix(p) for p in Path("tools/n64").glob("*.py") if converter_source(p)) + [p8_palette]
    tool_sources += sorted(posix(p) for p in definitions.glob("*.json"))

    def variant(variant_dir, extra_defines, maps, flags, rom, title, campaign, pc_expansion=False, compression=1):
        """one cartridge: the game compiled with its defines, its files, its
        ROM"""
        if type(compression) is not int or not 0 <= compression <= 3:
            raise ValueError("rom_elf_compression must be an integer from 0 to 3")
        convert_flags = "--campaign" if campaign else ""
        if pc_expansion:
            convert_flags = "--pc-expansion --pc-maps " + q(pc_maps)
        def map_source(name):
            return pc_maps / f"{name}.map" if pc_expansion and name in PC_MAPS else Path(f"build/xbox/disc/maps/{name}.map")
        defines = base_defines + " " + " ".join(f"-D{d}" for d in extra_defines)
        game_cflags = " ".join([
            *GAME_ABI, game_opt, *lto, *GAME_FLAGS,
            f"-I{PORT_DIR.as_posix()}/include", "-Iport/linux/include", "-Iport/include",
            f"-include {PORT_DIR.as_posix()}/include/halo_n64_prefix.h",
            f"-include {q(semantics)}",
            defines, includes,
            f"-idirafter {XDK_INCLUDE.as_posix()}",
        ])
        obj_dir = variant_dir / "obj"
        objects = []
        game_objects = []
        for o in project["objects"]:
            name = o["name"].replace(os.sep, "/")
            if o["status"] == "MISSING" or not name.endswith(".c") or not Path(name).is_file():
                continue
            if name in exclude_sources or any(name.startswith(d) for d in exclude_dirs):
                continue
            add(objects, obj_dir, name, game_cflags)
            game_objects.append(name)
        for source in sorted((PORT_DIR / "game").glob("*.c")):
            extra_flags = " -std=gnu99" if source.name in {"n64_golden.c", "n64_modifiers.c", "n64_render_game.c"} else ""
            if source.name in {"n64_golden.c", "n64_modifiers.c"}:
                extra_flags += " -Werror=implicit-function-declaration -Werror=int-conversion"
            add(objects, obj_dir, source, game_cflags + extra_flags)
        for extra in port.get("extra_game_sources", []):
            add(objects, obj_dir, extra, game_cflags)

        # the XDK-facing half of the platform layer (port/n64/src/xdk_*.c) and
        # the host-independent Linux units compiled again: the game's ABI and
        # the SDK declarations, through platform.h
        xdk_cflags = " ".join([
            *GAME_ABI, game_opt, *lto,
            "-std=gnu11", "-w", "-DHALO_LINUX_PLATFORM_LAYER",
            f"-I{PORT_DIR.as_posix()}/src", f"-I{PORT_DIR.as_posix()}/include", "-Iport/linux/src",
            "-Iport/linux/include", "-Iport/include", "-Isource", "-Isource/cseries",
            f"-include {PORT_DIR.as_posix()}/include/halo_n64_prefix.h",
            f"-include {q(semantics)}",
            f"-idirafter {XDK_INCLUDE.as_posix()}",
            " ".join(f"-D{d}" for d in extra_defines),
        ])
        for source in sorted((PORT_DIR / "src").glob("xdk_*.c")):
            add(objects, obj_dir, source, xdk_cflags)
        for name in port.get("linux_platform_sources", []):
            add(objects, obj_dir, Path("port/linux/src") / name, xdk_cflags)

        # the libdragon-facing half (port/n64/src/n64_*.c)
        platform_cflags = " ".join([platform_opt, *[f for f in PLATFORM_FLAGS if f], f"-I{PORT_DIR.as_posix()}/src", f"-I{PORT_DIR.as_posix()}/include",
                                    q(f"-I{TINY3D}/src"), *(f"-D{d}" for d in extra_defines)])
        for source in sorted((PORT_DIR / "src").glob("n64_*.c")):
            add(objects, obj_dir, source, platform_cflags)
        objects += musl_objects

        variant_dir.mkdir(parents=True, exist_ok=True)
        # the object list, for tools/n64_link_check.py
        objlist = variant_dir / "objects.txt"
        objlist.write_text("\n".join(objects) + "\n", encoding="utf-8")
        elf = variant_dir / "halo.elf"
        libdir = N64_INST / "mips64-elf" / "lib"
        link_dependencies = [posix(libdir / name) for name in ("libdragon.a", "libm.a", "libdragonsys.a", "libc.a", "n64.ld", "crt0.o")]
        link_dependencies += [posix(TINY3D / "build/libt3d.a"), "tools/n64_link_check.py"]
        n.build(posix(elf), "link", objects, implicit=link_dependencies, variables={
            "libdir": q(libdir),
            "objlist": q(objlist),
            "ldlibs": " ".join(port.get("libraries", [])) + f" {q(TINY3D / 'build/libt3d.a')} -ldragon -lm -ldragonsys",
            # (four link-time jobs: each lto1 takes hundreds of MB, and sixteen
            # of them beside the map conversions froze a 15 GB machine)
            "ldflags": " ".join(port.get("ldflags", []) + (["-flto=4", game_opt, small, *CODE_FLAGS] if lto else [])),
        })

        fsdir = variant_dir / "filesystem"
        fs_files = []
        for name in maps:
            out_map = fsdir / "maps" / f"{name}.map"
            outputs = [posix(out_map), posix(fsdir / "levels" / f"{name}.lvl"), posix(fsdir / "maps" / f"{name}.anims"),
                       posix(variant_dir / "sound" / f"{name}.slist"), posix(fsdir / "fx" / f"{name}.fx")]
            if campaign:
                outputs.append(posix(fsdir / "levels" / f"{name}.enc"))
            else:
                outputs += [posix(fsdir / "levels" / f"{name}.tiles"), posix(fsdir / "levels" / f"{name}-low.lvl"),
                            posix(fsdir / "levels" / f"{name}-low.mdl")]
            sources = ["build/xbox/disc/maps/a30.map"] if not campaign else []
            if pc_expansion:
                # Both explicit donors can contribute tag dependencies. Track
                # aircraft donor changes just like the weapon/ground donor.
                sources += [posix(pc_maps / "timberland.map"),
                            posix(pc_maps / "deathisland.map")]
            n.build(outputs, "convert_map", posix(map_source(name)), implicit=tool_sources + converter_tools + sources,
                    variables={"fsdir": q(fsdir), "map": q(out_map),
                               "map_backup": q(variant_dir / "maps-uncompressed" / f"{name}.map"),
                               "convert_flags": convert_flags})
            fs_files.append(posix(out_map))
        # Variant-local material banks have explicit owned source maps.
        paint = fsdir / "paint/masks.i4"
        paint_source = pc_maps / "timberland.map" if pc_expansion else Path("build/xbox/disc/maps/a30.map")
        n.build(posix(paint), "paint_masks", posix(paint_source), implicit=tool_sources + ["assets/paintball/paint-splatter-original.png"],
                variables={"source_map": q(paint_source)})
        fs_files.append(posix(paint))
        decals = fsdir / "decals/materials.bank"
        decal_sources = [posix(map_source(name)) for name in maps]
        imports = ["build/xbox/disc/maps/a30.map"] if not campaign else []
        if pc_expansion:
            imports.append(posix(pc_maps / "timberland.map"))
        n.build(posix(decals), "decal_bank", decal_sources, implicit=tool_sources + imports,
                variables={"sources": " ".join(q(p) for p in decal_sources), "convert_flags": convert_flags})
        fs_files.append(posix(decals))
        # the maps' gameplay sounds in one bank (tools/n64/soundbank.py)
        bank = fsdir / "snd" / "sounds.bank"
        n.build([posix(bank)] + [posix(fsdir / "snd" / f"{name}.ofs") for name in maps], "sound_bank",
                [posix(variant_dir / "sound" / f"{name}.slist") for name in maps], implicit=["tools/n64/soundbank.py"],
                variables={"variantdir": q(variant_dir), "fsdir": q(fsdir), "maps": " ".join(maps)})
        fs_files.append(posix(bank))
        # the menu (port/n64/src/n64_menu.c): ui.map's scene, fonts and pictures
        ui_scene = fsdir / "ui" / "scene.bin"
        n.build([posix(ui_scene), posix(fsdir / "levels" / "ui.mdl"), posix(fsdir / "fonts" / "small.glyphs")], "convert_ui",
                "build/xbox/disc/maps/ui.map", implicit=tool_sources + converter_tools, variables={"fsdir": q(fsdir)})
        fs_files.append(posix(ui_scene))
        if pc_expansion:
            pictures = [posix(fsdir / "ui" / f"map{i}.sprite") for i in range(13, 19)]
            n.build(pictures, "pc_thumbnails", posix(args.pc_ui_bitmap), implicit=tool_sources + converter_tools + [posix(ui_scene)],
                    variables={"bitmap": q(args.pc_ui_bitmap), "outputdir": q(fsdir / "ui")})
            fs_files.extend(pictures)
        # the main menu's music (tools/n64/music.py, port/n64/src/n64_sound.c)
        music = [posix(fsdir / "music" / f"title_{name}.wav64") for name in ("in", "loop0", "loop1", "loop2")]
        n.build(music, "convert_music", "build/xbox/disc/maps/ui.map", implicit=tool_sources,
                variables={"fsdir": q(fsdir)})
        fs_files.append(music[0])
        # the debug switches the cartridge carries (n64_debug_flag) are the files
        # at its root (everything else is in folders): the ones port.json does
        # not ask for are removed, so none lingers in a build (a list of known
        # switches missed the new ones)
        flag_names = [flag.split("=", 1)[0] for flag in flags]
        if fsdir.exists():
            for path in fsdir.iterdir():
                if path.is_file() and path.name not in flag_names:
                    path.unlink()
        for flag in flags:
            name, _, value = flag.partition("=")
            out_flag = fsdir / name
            if value:
                fsdir.mkdir(parents=True, exist_ok=True)
                if not out_flag.exists() or out_flag.read_text() != value + "\n":
                    out_flag.write_text(value + "\n")
            else:
                n.build(posix(out_flag), "touch")
            fs_files.append(posix(out_flag))
        dfs = variant_dir / "halo.dfs"
        dfsdir = fsdir
        dfs_files = fs_files
        if pc_expansion:
            # Preserve the complete raw conversion tree. The helper creates a
            # distinct owned tree; pruning and DragonFS packing use only that tree.
            dfsdir = variant_dir / "filesystem-packed"
            stamp = variant_dir / "filesystem-packed.fx-pool.stamp"
            manifest = variant_dir / "filesystem-packed.fx-pool.json"
            # The helper's durable sibling .owner.json is deliberately not a
            # Ninja output: `ninja -t clean` must leave ownership proof intact.
            raw_fx = [posix(fsdir / "fx" / f"{name}.fx") for name in maps]
            copies = [posix(dfsdir / Path(path).relative_to(fsdir)) for path in fs_files]
            packed_fx = [posix(dfsdir / "fx" / f"{name}.fx") for name in maps]
            dfs_files = list(dict.fromkeys([*copies, *packed_fx, posix(dfsdir / "fx/records.bank"),
                                            posix(stamp), posix(manifest)]))
            # Validate the dynamic copied asset tree on every invocation. On
            # identical bytes the helper preserves output mtimes, so restat
            # prevents an otherwise unnecessary DragonFS/ROM rebuild.
            validate = variant_dir / "fx-pool-validate"
            n.build(posix(validate), "phony")
            n.build(dfs_files, "fx_pool", list(dict.fromkeys([*fs_files, *raw_fx, posix(validate)])),
                    implicit=["tools/n64/fx_pool.py"],
                    variables={"source_fs": q(fsdir), "packed_fs": q(dfsdir),
                               "maps": " ".join(maps), "stamp": q(stamp)})
        n.build(posix(dfs), "mkdfs_packed" if pc_expansion else "mkdfs", dfs_files, variables={"fsdir": q(dfsdir), "flags": " ".join(flags),
                                                           "maps": " ".join(maps + ["ui"])})
        target = BUILD_DIR / rom
        n.build(posix(target), "z64", posix(elf), implicit=[posix(dfs)] + [posix(N64_INST / "bin" / (name + (".exe" if sys.platform == "win32" else ""))) for name in ("n64sym", "mips64-elf-strip", "n64elfcompress", "n64tool", "ed64romconfig")],
                variables={"extra": q(dfs), "outdir": q(variant_dir), "title": title, "rom_elf_compression": str(compression)})
        return target, len(game_objects), len(objects)

    targets = []
    main_rom, game_count, object_count = variant(BUILD_DIR, [], port.get("maps", []), port.get("flags", []),
                                                 "halo.z64", "HALO N64", False, compression=port.get("rom_elf_compression", 1))
    targets.append(main_rom)
    # the campaign's cartridge (port.json "campaign"): its own build of the
    # game (HALO_N64_CAMPAIGN: the compact collision of tools/n64/
    # collision.py) and its levels converted as campaign maps
    campaign = port.get("campaign")
    if campaign:
        rom, _, _ = variant(BUILD_DIR / "campaign", campaign.get("defines", []), campaign.get("maps", []),
                            campaign.get("flags", []), campaign.get("rom", "halo-campaign.z64"),
                            campaign.get("title", "HALO N64 CAMPAIGN"), True, compression=campaign.get("rom_elf_compression", 1))
        targets.append(rom)
    n.build("campaign", "phony", [posix(t) for t in targets[1:]])
    if args.pc_expansion:
        pc = port.get("pc_expansion")
        if not pc:
            parser.error("port.json has no pc_expansion configuration")
        rom, _, _ = variant(BUILD_DIR / "pc-expansion", pc.get("defines", []), pc.get("maps", []),
                            pc.get("flags", []), pc.get("rom", "halo-pc-expansion.z64"),
                            pc.get("title", "HALO 64 PC EXPANSION"), False, True,
                            pc.get("rom_elf_compression", 0))
        targets.append(rom)
        n.build("pc-expansion", "phony", posix(rom))
    n.default([escape_path(posix(t)) for t in targets])
    out.close()
    print(f"wrote {BUILD_DIR / 'build.ninja'}: {game_count} game units, {object_count} objects a cartridge, "
          f"{len(targets)} cartridges")


if __name__ == "__main__":
    main()
