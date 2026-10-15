"""Halo 64's ROM builder for players: Halo's Xbox maps in, the cartridges
out, without the N64 toolchain.

The game's program comes ready-made (the payload: the prebuilt, compressed
ELF of each cartridge and its symbols -- code, no Halo data); the maps of
the player's own copy of Halo: Combat Evolved for Xbox, from its disc image
(tools/n64/xiso.py) or a folder of them, are converted here as
tools/n64_build.py's ninja build converts them (tools/n64/convert.py, ui.py,
music.py, soundbank.py, prune.py), and libdragon's mkdfs, n64tool and
ed64romconfig put each cartridge together. The Windows program
(rombuilder_gui.py, packaged by tools/n64/package_rombuilder.py) runs this;
so does a command line:

    python3 -m tools.n64.rombuilder <disc image or maps folder> -o <folder>
        [--multiplayer] [--campaign] [--jobs N] [--work <folder>] [--font <Halo.ttf>]

    # optional, separate PC expansion (also requires Xbox data):
    python3 -m tools.n64.rombuilder <Xbox maps> -o <folder> --pc-expansion
        --pc-source <retail PC ISO or Maps folder> --pc-tools <trusted tools>

Each conversion runs in a process of its own (the converters take ~800 MB
each): this program again, "--worker <module> <arguments>", its output in a
log file.
"""

import argparse
import ctypes
import json
import os
import shutil
import subprocess
import sys
import threading
import tempfile
import struct
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent.parent

# the cartridges (port/n64/port.json's: the multiplayer one's maps, the
# campaign's), the files a player's ROM is called
CARTRIDGES = {
    "multiplayer": {"rom": "halo-n64.z64", "elf": "multiplayer"},
    "campaign": {"rom": "halo-campaign.z64", "elf": "campaign"},
    "pc_expansion": {"rom": "halo-pc-expansion.z64", "elf": "pc-expansion"},
}
# what a conversion costs, roughly (the progress bar's shares)
WEIGHTS = {"map": 10, "campaign map": 60, "ui": 6, "music": 4, "sounds": 2, "pack": 2, "rom": 1}
# a converter's memory at most (three at a time took ~2.4 GB)
CONVERSION_BYTES = 900 << 20
MAX_ROM_BYTES = 64 << 20


class BuildError(Exception):
    pass


class Cancelled(Exception):
    pass


# ---------- the payload: what the builder carries besides Python

def payload_root():
    """the packaged builder's payload folder, or None (running from the
    repository: the toolchain's tools, build/n64's ELFs)"""
    if getattr(sys, "frozen", False):
        return Path(getattr(sys, "_MEIPASS", Path(sys.executable).parent)) / "payload"
    folder = os.environ.get("HALO64_PAYLOAD")
    return Path(folder) if folder else None


def port_config():
    root = payload_root()
    path = root / "port.json" if root else REPO / "port" / "n64" / "port.json"
    return json.loads(path.read_text(encoding="utf-8"))


def elf_files(cartridge):
    """the cartridge's prebuilt program: (compressed stripped ELF, symbols)"""
    root = payload_root()
    if root:
        folder = root / "elf" / CARTRIDGES[cartridge]["elf"]
    else:
        folder = REPO / "build" / "n64" / ({"campaign": "campaign", "pc_expansion": "pc-expansion"}.get(cartridge, ""))
    return folder / "halo.elf.stripped", folder / "halo.elf.sym"


def worker_environment(font=None):
    env = dict(os.environ)
    root = payload_root()
    if root:
        env["N64_TOOLS"] = str(root / "bin")
        env["INVADER_DEFINITIONS"] = str(root / "invader" / "src" / "tag" / "hek" / "definition")
    env["PYTHONUTF8"] = "1"
    env["PYTHONIOENCODING"] = "utf-8"
    if font:
        env["HALO_FONT"] = str(font)
    return env


def tool(name):
    root = payload_root()
    if root:
        return str(root / "bin" / (name + ".exe" if sys.platform == "win32" else name))
    from .toolpaths import tool as toolchain_tool
    return toolchain_tool(name)


def halo_font():
    """the fan-made Halo font for the menu logo's 64 (ui.py), if this PC has
    it installed (Windows' fonts folders) or as ~/n64/fonts/Halo.ttf"""
    places = [Path.home() / "n64" / "fonts" / "Halo.ttf"]
    windir = os.environ.get("WINDIR")
    if windir:
        places.append(Path(windir) / "Fonts" / "Halo.ttf")
    local = os.environ.get("LOCALAPPDATA")
    if local:
        places.append(Path(local) / "Microsoft" / "Windows" / "Fonts" / "Halo.ttf")
    return next((p for p in places if p.is_file()), None)


def free_memory():
    """physical memory free now (bytes), or None"""
    try:
        if sys.platform == "win32":
            class Status(ctypes.Structure):
                _fields_ = [("length", ctypes.c_ulong), ("load", ctypes.c_ulong), ("total", ctypes.c_ulonglong),
                            ("available", ctypes.c_ulonglong), ("page_total", ctypes.c_ulonglong),
                            ("page_available", ctypes.c_ulonglong), ("virtual_total", ctypes.c_ulonglong),
                            ("virtual_available", ctypes.c_ulonglong), ("extended", ctypes.c_ulonglong)]
            status = Status()
            status.length = ctypes.sizeof(Status)
            ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(status))
            return status.available
        return os.sysconf("SC_AVPHYS_PAGES") * os.sysconf("SC_PAGE_SIZE")
    except (AttributeError, OSError, ValueError):
        return None


def default_jobs():
    cpus = os.cpu_count() or 2
    jobs = max(1, min(3, cpus // 2 or 1))
    memory = free_memory()
    if memory:
        jobs = max(1, min(jobs, int((memory - (1 << 30)) // CONVERSION_BYTES)))
    return jobs


# ---------- the maps

class MapSource:
    """Halo's Xbox maps: a disc image's (extracted as needed) or a folder's
    (the folder itself, or its maps folder)"""

    def __init__(self, path):
        self.path = Path(path)
        self.disc = None
        self.folder = None
        if self.path.is_file():
            from .xiso import XboxDisc
            self.disc = XboxDisc(self.path)
            self.names = sorted(Path(n).stem for n in self.disc.files
                                if n.startswith("maps/") and n.endswith(".map") and n.count("/") == 1)
        elif self.path.is_dir():
            for candidate in (self.path, self.path / "maps", self.path / "Maps"):
                if candidate.is_dir() and any(p.is_file() and p.suffix.lower() == ".map" for p in candidate.iterdir()):
                    self.folder = candidate
                    break
            if self.folder is None:
                raise BuildError(f"no Halo maps (.map files) in {self.path}")
            self.names = sorted(p.stem.lower() for p in self.folder.iterdir() if p.is_file() and p.suffix.lower() == ".map")
            if len(self.names) != len(set(self.names)):
                raise BuildError("ambiguous map names differing only in case")
        else:
            raise BuildError(f"{self.path} is neither a disc image nor a folder")

    def close(self):
        if self.disc:
            self.disc.close()

    def missing(self, names):
        return [n for n in names if n not in self.names]

    def size(self, name):
        if self.disc:
            return self.disc.files[f"maps/{name}.map"][1]
        return self.map_path(name).stat().st_size

    def map_path(self, name):
        if self.folder:
            for p in self.folder.iterdir():
                if p.is_file() and p.suffix.lower() == ".map" and p.stem.lower() == name:
                    return p
        raise KeyError(name)

    def provide(self, names, folder, progress=None):
        """the maps in one folder (copied from the disc, linked or copied
        from a folder elsewhere): the converter looks for a30.map beside
        the map it converts (Firefight's Covenant)"""
        folder.mkdir(parents=True, exist_ok=True)
        for name in names:
            target = folder / f"{name}.map"
            if self.disc:
                self.disc.extract(f"maps/{name}.map", target,
                                  (lambda done, total, n=name: progress(n, done, total)) if progress else None)
            else:
                shutil.copyfile(self.map_path(name), target)
        return folder

    def validate(self, names):
        """Reject PC/Custom Edition data in the Xbox input before converters run."""
        missing = self.missing(names)
        if missing:
            raise BuildError("missing Xbox maps: " + ", ".join(missing))
        for name in names:
            size = self.size(name)
            if self.disc:
                sector, _ = self.disc.files[f"maps/{name}.map"]
                self.disc.file.seek(self.disc.partition + sector * 2048)
                header = self.disc.file.read(2048)
            else:
                with self.map_path(name).open("rb") as stream:
                    header = stream.read(2048)
            if (len(header) != 2048 or header[:4] != b"daeh" or
                    struct.unpack_from("<I", header, 4)[0] != 5 or header[-4:] != b"toof"):
                raise BuildError(f"{name}.map is not a Halo Xbox version-5 cache file; choose Xbox data here")
            declared, _, tag_offset, tag_size = struct.unpack_from("<4I", header, 8)
            # Xbox files are compressed and 4096-byte padded; the length in
            # the cache header describes the decompressed stream.
            if (size < 2048 or not 2048 <= declared <= (2 << 30) or
                    tag_offset < 2048 or tag_size < 36 or tag_offset + tag_size > declared):
                raise BuildError(f"{name}.map has invalid Xbox tag-data bounds")
            actual_name = header[0x20:0x40].split(b"\0", 1)[0]
            if actual_name.lower() != name.encode("ascii"):
                raise BuildError(f"{name}.map identifies itself as another map")

    def describe(self):
        """a line on what was found: is it Halo for Xbox?"""
        kind = "disc image" if self.disc else "maps folder"
        return f"Halo: Combat Evolved (Xbox) {kind}, {len(self.names)} maps"


# ---------- the build

class RomBuilder:
    """builds the chosen cartridges into an output folder; log(text) and
    progress(fraction, step) report, cancel (a threading.Event) stops it"""

    def __init__(self, source, output, cartridges, work=None, jobs=None, font=None, log=print, progress=None,
                 cancel=None, pc_source=None, pc_tools=None):
        self.source = MapSource(source)
        self.pc_source = Path(pc_source) if pc_source is not None else None
        self.pc_tools = Path(pc_tools) if pc_tools is not None else None
        self.pc_maps = None
        self.pc_bitmap = None
        self.output = Path(output)
        self.cartridges = list(cartridges)
        self.work = Path(work) if work else default_work_folder()
        self.jobs = default_jobs() if jobs is None else jobs
        self.font = font or halo_font()
        self.log = log
        self.report = progress or (lambda fraction, step: None)
        self.cancel = cancel or threading.Event()
        self.config = port_config()
        self.done = 0
        self.total = 1
        self.processes = set()
        self.lock = threading.Lock()
        # (the title music, the same on every cartridge: converted once)
        self.music = None

    # (what each cartridge needs)
    def maps_for(self, cartridge):
        if cartridge in ("campaign", "pc_expansion"):
            return list(self.config[cartridge]["maps"])
        return list(self.config["maps"])

    def needed_maps(self):
        names = {"ui"}
        for cartridge in self.cartridges:
            names.update(self.config["maps"] if cartridge == "pc_expansion" else self.maps_for(cartridge))
            if cartridge in ("multiplayer", "pc_expansion"):
                # (Firefight's Covenant come from a30, convert.py)
                names.add("a30")
        return sorted(names)

    def check(self):
        if not isinstance(self.jobs, int) or self.jobs < 1:
            raise BuildError("conversion jobs must be a positive integer")
        if not self.cartridges or any(c not in CARTRIDGES for c in self.cartridges):
            raise BuildError("choose at least one supported cartridge")
        if "pc_expansion" in self.cartridges:
            if not self.pc_source:
                raise BuildError("PC expansion also needs your Halo PC retail disc image or installed maps folder")
            if "pc_expansion" not in self.config:
                raise BuildError("this builder has no PC expansion configuration")
            if not Path(tool("mkasset")).is_file():
                raise BuildError("PC expansion needs the trusted libdragon mkasset tool; include it in N64_TOOLS")
            from .pc_input import PCMapSource, PC_MAPS
            from .pc_prepare import validate_tools
            try:
                pc = PCMapSource(self.pc_source)
                try:
                    pc.validate([*PC_MAPS, "ui"])
                finally:
                    pc.close()
                self.pc_tools = validate_tools(self.pc_tools or ((payload_root() / "pc-tools") if payload_root() else None))
            except (ValueError, OSError) as error:
                raise BuildError(str(error)) from error
        missing = self.source.missing(self.needed_maps())
        if missing:
            raise BuildError("these maps are not in " + self.source.path.name + ": " + ", ".join(missing) +
                             " (Halo 64 needs Halo: Combat Evolved for the original Xbox)")
        self.source.validate(self.needed_maps())
        for cartridge in self.cartridges:
            for path in elf_files(cartridge):
                if not path.is_file():
                    raise BuildError(f"the builder is missing its {cartridge} program ({path.name})")

    def step(self, weight, text):
        with self.lock:
            self.done += weight
            self.report(min(1.0, self.done / self.total), text)

    def run(self):
        try:
            return self._run()
        except (ValueError, OSError) as error:
            raise BuildError(str(error)) from error
        finally:
            self.source.close()

    def _run(self):
        started = time.time()
        self.check()
        self.output.mkdir(parents=True, exist_ok=True)
        self.work.mkdir(parents=True, exist_ok=True)
        self.total = 0
        for cartridge in self.cartridges:
            maps = self.maps_for(cartridge)
            self.total += len(maps) * WEIGHTS["campaign map" if cartridge == "campaign" else "map"]
            self.total += WEIGHTS["ui"] + WEIGHTS["music"] + WEIGHTS["sounds"] + WEIGHTS["pack"] + WEIGHTS["rom"]
        if self.source.disc:
            self.total += 10
        self.log(f"{self.source.describe()}; {self.jobs} conversion(s) at a time; work folder {self.work}")
        if not self.font:
            self.log("(no Halo.ttf font installed: the menu's logo is drawn without its 64)")
        made = []
        # Each run owns a new input stage; equal-size data from a previous disc
        # can never be reused. Only this generated temporary folder is removed.
        self.input_stage = Path(tempfile.mkdtemp(prefix="inputs-", dir=self.work))
        self.receipts = self.work / "receipts" / self.input_stage.name
        self.receipts.mkdir(parents=True)
        try:
            maps = self.provide_maps()
            if "pc_expansion" in self.cartridges:
                self.prepare_pc()
            for cartridge in self.cartridges:
                made.append(self.build(cartridge, maps))
        finally:
            shutil.rmtree(self.input_stage, ignore_errors=True)
        self.report(1.0, "Done")
        self.log(f"done in {(time.time() - started) / 60:.1f} minutes: " + ", ".join(str(m) for m in made))
        return made

    def provide_maps(self):
        maps = self.input_stage / "xbox"
        names = self.needed_maps()
        def progress(name, done, size):
            self.check_cancel()
            self.report(min(1.0, self.done / self.total), f"Copying {name}.map")
        self.source.provide(names, maps, progress)
        if self.source.disc:
            self.step(10, "Maps copied from the disc")
        return maps

    def prepare_pc(self):
        from .pc_input import PCMapSource
        source = PCMapSource(self.pc_source)
        try:
            from .pc_input import PC_MAPS
            available = set(source.names())
            needed = [*PC_MAPS, "ui", *[name for name in ("bitmaps", "sounds") if name in available]]
            raw = source.provide(needed, self.input_stage / "pc-retail",
                                 lambda *_: self.check_cancel())
            (self.receipts / "pc-inputs.json").write_text(json.dumps(source.last_manifest, indent=2), encoding="utf-8")
        finally:
            source.close()
        destination = self.input_stage / "pc-preparation"
        self.run_worker("Preparing your PC expansion maps", "pc_prepare",
                        [str(raw), "--work", str(destination), "--tools", str(self.pc_tools)])
        result = json.loads((destination / "result.json").read_text(encoding="utf-8"))
        self.pc_maps = Path(result["maps"])
        self.pc_bitmap = Path(result["ui_bitmap"])
        # The worker must return generated paths inside this run's own stage.
        for path in (self.pc_maps, self.pc_bitmap):
            if not path.resolve().is_relative_to(destination.resolve()):
                raise BuildError("PC preparation returned a path outside its work folder")
        shutil.copy2(self.pc_maps.parent / "manifest.json", self.receipts / "pc-normalization.json")

    def build(self, cartridge, maps):
        config = self.config[cartridge] if cartridge in ("campaign", "pc_expansion") else self.config
        names = self.maps_for(cartridge)
        variant = self.input_stage / cartridge
        fsdir = variant / "filesystem"
        fsdir.mkdir(parents=True, exist_ok=True)
        # (the flags a developer's build may carry: files at the root)
        for path in fsdir.iterdir():
            if path.is_file():
                path.unlink()
        label = {"campaign": "the campaign", "pc_expansion": "PC expansion"}.get(cartridge, "multiplayer")
        weight = WEIGHTS["campaign map" if cartridge == "campaign" else "map"]
        # the maps, a few at a time
        tasks = []
        for name in names:
            source_maps = self.pc_maps if cartridge == "pc_expansion" and name not in self.config["maps"] else maps
            arguments = [str(source_maps / f"{name}.map"), "-o", str(fsdir / "maps" / f"{name}.map"),
                         "--filesystem", str(fsdir)]
            if cartridge == "campaign":
                arguments.append("--campaign")
            elif cartridge == "pc_expansion":
                arguments += ["--pc-expansion", "--pc-maps", str(self.pc_maps)]
            tasks.append((f"Converting {name} ({label})", "convert", arguments, weight))
        self.run_parallel(tasks)
        if cartridge == "pc_expansion":
            for name in names:
                self.run_worker(f"Compressing {name} ({label})", "compress_map",
                                [str(fsdir / "maps" / f"{name}.map")])
        self.run_worker(f"Converting the menu ({label})", "ui", [str(maps / "ui.map"), "--filesystem", str(fsdir)])
        if cartridge == "pc_expansion":
            self.run_worker("Adding the PC map thumbnails", "pc_thumbnails",
                            ["--bitmap", str(self.pc_bitmap), "--output", str(fsdir / "ui")])
        self.step(WEIGHTS["ui"], f"Menu converted ({label})")
        if self.music:
            shutil.copytree(self.music, fsdir / "music", dirs_exist_ok=True)
        else:
            self.run_worker(f"Converting the title music ({label})", "music",
                            [str(maps / "ui.map"), "--filesystem", str(fsdir)])
            self.music = fsdir / "music"
        self.step(WEIGHTS["music"], f"Music converted ({label})")
        self.run_worker(f"Packing the sounds ({label})", "soundbank", [str(variant), str(fsdir), *names])
        self.step(WEIGHTS["sounds"], f"Sounds packed ({label})")
        # Derive decal pixels from the selected legal maps; the paint mask's
        # outline is project-original art, packaged separately from Halo data.
        paint_map = self.pc_maps / "timberland.map" if cartridge == "pc_expansion" else maps / "a30.map"
        root = payload_root()
        paint_art = root / "art" / "paint-splatter-original.png" if root else REPO / "assets" / "paintball" / "paint-splatter-original.png"
        self.run_worker(f"Preparing paint ({label})", "paint_masks",
                        ["--map", str(paint_map), "--output", str(fsdir / "paint" / "masks.i4"), "--art", str(paint_art)])
        decal_maps = [str((self.pc_maps if cartridge == "pc_expansion" and n not in self.config["maps"] else maps) / f"{n}.map") for n in names]
        decal_args = ["--maps", *decal_maps, "--output", str(fsdir / "decals" / "materials.bank")]
        if cartridge == "pc_expansion":
            decal_args += ["--pc-expansion", "--pc-maps", str(self.pc_maps)]
        elif cartridge == "campaign":
            decal_args += ["--campaign"]
        self.run_worker(f"Preparing impact decals ({label})", "decal_bank", decal_args)
        # the cartridge's file system
        self.run_worker(f"Tidying the files ({label})", "prune", [str(fsdir), *names, "ui"])
        packed_fs = fsdir
        if cartridge == "pc_expansion":
            packed_fs = variant / "packed-filesystem"
            self.run_worker("Sharing exact PC effect records", "fx_pool",
                            ["--filesystem", str(fsdir), "--output", str(packed_fs), "--maps", *names])
            manifest = packed_fs.with_name(packed_fs.name + ".fx-pool.json")
            shutil.copy2(manifest, self.receipts / "pc-fx-pool.json")
        # The pooled tree is already final; do not prune or modify it here.
        dfs = variant / "halo.dfs"
        self.run_tool(f"Packing the files ({label})", [tool("mkdfs"), str(dfs), str(packed_fs)])
        self.step(WEIGHTS["pack"], f"Files packed ({label})")
        # the ROM: the program, its symbols, the files
        elf, symbols = elf_files(cartridge)
        rom = self.output / CARTRIDGES[cartridge]["rom"]
        # Reserve a unique owned temporary file so rejection/cleanup cannot
        # remove another build's file or the previously published ROM.
        with tempfile.NamedTemporaryFile(prefix=rom.stem + "-", suffix=".tmp", dir=self.output, delete=False) as reserved:
            temporary = Path(reserved.name)
        title = config.get("title", "HALO N64")
        try:
            self.run_tool(f"Making the ROM ({label})", [tool("n64tool"), "--toc", "--title", title, "--output",
                                                       str(temporary), "--align", "256", str(elf), str(symbols), str(dfs)])
            self.run_tool(f"Making the ROM ({label})", [tool("ed64romconfig"), "--expansionpak", "required", "--savetype",
                                                       "eeprom4k", "--regionfree", str(temporary)])
            size = temporary.stat().st_size
            if size > MAX_ROM_BYTES:
                raise BuildError(f"{rom.name} would be {size / (1 << 20):.2f} MiB; maximum ROM size is 64 MiB")
            os.replace(temporary, rom)
        finally:
            temporary.unlink(missing_ok=True)
        self.step(WEIGHTS["rom"], f"{rom.name} made")
        self.log(f"{rom}: {rom.stat().st_size / (1 << 20):.1f} MB")
        return rom

    # ---------- processes

    def check_cancel(self):
        if self.cancel.is_set():
            raise Cancelled()

    def worker_command(self, module, arguments, log_path):
        if getattr(sys, "frozen", False):
            command = [sys.executable]
        else:
            command = [sys.executable, "-m", "tools.n64.rombuilder"]
        return command + ["--worker", module, "--log", str(log_path), *arguments]

    def start(self, command, capture):
        """a process (no console window); its output piped back, or not
        (a worker writes its own log)"""
        flags = getattr(subprocess, "CREATE_NO_WINDOW", 0)
        cwd = None if getattr(sys, "frozen", False) else str(REPO)
        process = subprocess.Popen(command, cwd=cwd, env=worker_environment(self.font),
                                   stdout=subprocess.PIPE if capture else subprocess.DEVNULL,
                                   stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, creationflags=flags)
        with self.lock:
            self.processes.add(process)
        return process

    def wait(self, process):
        while True:
            try:
                code = process.wait(timeout=0.25)
                break
            except subprocess.TimeoutExpired:
                if self.cancel.is_set():
                    self.stop_all()
        with self.lock:
            self.processes.discard(process)
        self.check_cancel()
        return code

    def stop_all(self):
        with self.lock:
            processes = list(self.processes)
        for process in processes:
            try:
                if sys.platform == "win32" and process.poll() is None:
                    # A converter can own an Invader/FFmpeg child. Stop that
                    # process tree rather than leaving a writer behind.
                    taskkill = Path(os.environ.get("SystemRoot", r"C:\Windows")) / "System32" / "taskkill.exe"
                    subprocess.run([str(taskkill), "/PID", str(process.pid), "/T", "/F"],
                                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                                   creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0), check=False)
                if process.poll() is None:
                    process.kill()
            except OSError:
                pass

    def run_worker(self, text, module, arguments):
        self.check_cancel()
        self.report(min(1.0, self.done / self.total), text)
        logs = self.work / "logs"
        logs.mkdir(parents=True, exist_ok=True)
        log_path = logs / f"{module}-{abs(hash(tuple(arguments))) % 100000}.log"
        process = self.start(self.worker_command(module, arguments, log_path), capture=False)
        code = self.wait(process)
        text_out = log_path.read_text(encoding="utf-8", errors="replace") if log_path.exists() else ""
        for line in text_out.splitlines():
            if line.strip():
                self.log(line)
        if code != 0:
            tail = "\n".join(text_out.splitlines()[-12:])
            raise BuildError(f"{text} failed (code {code}):\n{tail}")

    def run_parallel(self, tasks):
        errors = []
        queue = list(tasks)
        lock = threading.Lock()

        def runner():
            while True:
                with lock:
                    if not queue or errors or self.cancel.is_set():
                        return
                    text, module, arguments, weight = queue.pop(0)
                try:
                    self.run_worker(text, module, arguments)
                    self.step(weight, text.replace("Converting", "Converted"))
                except Cancelled:
                    return
                except Exception as error:  # noqa: BLE001 (reported; the queue stops)
                    with lock:
                        errors.append(error)

        threads = [threading.Thread(target=runner, daemon=True) for _ in range(min(self.jobs, len(tasks)))]
        for t in threads:
            t.start()
        for t in threads:
            t.join()
        self.check_cancel()
        if errors:
            raise errors[0]

    def run_tool(self, text, command):
        self.check_cancel()
        self.report(min(1.0, self.done / self.total), text)
        process = self.start(command, capture=True)
        while True:
            try:
                output, _ = process.communicate(timeout=0.25)
                break
            except subprocess.TimeoutExpired:
                if self.cancel.is_set():
                    self.stop_all()
        code = self.wait(process)
        if code != 0:
            raise BuildError(f"{text} failed: {Path(command[0]).name} said:\n" +
                             output.decode("utf-8", "replace")[-1500:])


def default_work_folder():
    local = os.environ.get("LOCALAPPDATA")
    if local:
        return Path(local) / "Halo64RomBuilder" / "work"
    return Path.home() / ".cache" / "halo64-rombuilder"


# ---------- a worker: one converter, its output in a log file

def run_worker(arguments):
    module, log_path, rest = arguments[0], None, arguments[1:]
    if rest[:1] == ["--log"]:
        log_path, rest = rest[1], rest[2:]
    if log_path:
        log = open(log_path, "w", encoding="utf-8", buffering=1)
        sys.stdout = sys.stderr = log
    # (the payload's tools and tag definitions, before the converters read
    # where they are)
    for key, value in worker_environment().items():
        os.environ.setdefault(key, value)
    import importlib
    import traceback
    allowed = {"convert", "ui", "music", "soundbank", "prune", "pc_prepare", "pc_thumbnails", "paint_masks", "decal_bank", "compress_map", "fx_pool"}
    if module not in allowed:
        print(f"no such converter: {module}")
        return 2
    try:
        if module == "compress_map":
            if len(rest) != 1:
                print("compress_map worker expects exactly one generated map path")
                return 2
            # Keep the existing Ninja CLI/helper unchanged: it is a global
            # converter dependency. The installer only needs this function.
            from .compress_map import compress
            compress(Path(rest[0]))
            return 0
        target = importlib.import_module(f"tools.n64.{module}")
        sys.argv = [f"tools.n64.{module}", *rest]
        result = target.main()
        return result if isinstance(result, int) else 0
    except SystemExit as exit:
        return exit.code if isinstance(exit.code, int) else 1
    except BaseException:  # noqa: BLE001 (the worker's own report)
        traceback.print_exc()
        return 1
    finally:
        sys.stdout.flush()


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--worker":
        sys.exit(run_worker(sys.argv[2:]))
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("source", type=Path, help="Halo's Xbox disc image, or its maps folder")
    parser.add_argument("-o", "--output", type=Path, required=True)
    parser.add_argument("--multiplayer", action="store_true")
    parser.add_argument("--campaign", action="store_true")
    parser.add_argument("--pc-expansion", action="store_true", help="separate 19-map PC expansion cartridge")
    parser.add_argument("--pc-source", type=Path, help="your PC retail ISO or installed maps folder (PC expansion only)")
    parser.add_argument("--pc-tools", type=Path, help="trusted local Invader and FFmpeg tools (source runs only)")
    parser.add_argument("--jobs", type=int)
    parser.add_argument("--work", type=Path)
    parser.add_argument("--font", type=Path)
    args = parser.parse_args()
    cartridges = [c for c in ("multiplayer", "campaign", "pc_expansion") if getattr(args, c)] or ["multiplayer", "campaign"]

    def progress(fraction, step):
        print(f"[{fraction * 100:5.1f}%] {step}", flush=True)

    try:
        RomBuilder(args.source, args.output, cartridges, work=args.work, jobs=args.jobs, font=args.font,
                   pc_source=args.pc_source, pc_tools=args.pc_tools, log=lambda text: print("   ", text, flush=True), progress=progress).run()
    except BuildError as error:
        print(f"error: {error}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
