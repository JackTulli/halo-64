"""Halo 64 ROM Builder: the Windows program around tools/n64/rombuilder.py.

One window: drop Halo's Xbox disc image (or its maps folder) on it, tick
the cartridges (and add Custom Edition maps for a cartridge of them: drop
their .map or .zip files on it), choose where they go, Build. The build runs on a thread of
its own (each conversion a process: rombuilder.py), its progress and log
posted to the window through a queue. tkinterdnd2 gives the window its
drag and drop (without it, the buttons still choose); sv_ttk its look.
"""

import json
import os
import queue
import subprocess
import sys
import threading
import time
import tkinter as tk
from pathlib import Path
from tkinter import filedialog, messagebox, ttk

from .rombuilder import BuildError, Cancelled, CUSTOM_MAP_LIMIT, MapSource, RomBuilder, payload_root

try:
    from tkinterdnd2 import DND_FILES, TkinterDnD
except ImportError:  # (no drag and drop: the buttons alone)
    TkinterDnD = None

TITLE = "Halo 64 ROM Builder"
CARTRIDGE_TEXT = {
    "multiplayer": ("Multiplayer", "halo-n64.z64", "Blood Gulch and twelve more maps, split screen for four, bots, Firefight"),
    "campaign": ("Campaign: Halo", "halo-campaign.z64", "the level \"Halo\" on a cartridge of its own"),
    "pc_expansion": ("PC expansion (optional)", "halo-pc-expansion.z64",
                     "six more PC maps, weapons and vehicles on a separate cartridge; also needs your Halo PC copy"),
    "custom": ("Custom Edition maps", "halo-custom-edition.z64",
               f"Halo Custom Edition multiplayer maps you add (up to {CUSTOM_MAP_LIMIT}) on a cartridge of their own"),
}
# what a Custom Edition map comes as
CE_FILES = (".map", ".zip")


def custom_included():
    """whether this copy can make the Custom Edition cartridge: run from the
    repository (its tools chosen in the window), or a package carrying the
    Custom Edition tools and program"""
    root = payload_root()
    return root is None or ((root / "ce-tools").is_dir() and (root / "elf" / "custom").is_dir())


def pc_expansion_included():
    """whether this copy can make the PC expansion cartridge: run from the
    repository (its PC tools chosen in the window), or a package carrying the
    PC conversion tools and the PC program (package_rombuilder.py's PC
    options); a package without them shows the cartridge greyed out"""
    root = payload_root()
    return root is None or ((root / "pc-tools").is_dir() and (root / "elf" / "pc-expansion").is_dir())


def settings_path():
    local = os.environ.get("LOCALAPPDATA")
    base = Path(local) / "Halo64RomBuilder" if local else Path.home() / ".config" / "halo64-rombuilder"
    return base / "settings.json"


def load_settings():
    try:
        return json.loads(settings_path().read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def save_settings(values):
    try:
        path = settings_path()
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(values, indent=1), encoding="utf-8")
    except OSError:
        pass


def default_output():
    documents = Path.home() / "Documents"
    return str((documents if documents.is_dir() else Path.home()) / "Halo 64")


def open_folder(path):
    if sys.platform == "win32":
        os.startfile(str(path))  # noqa: S606 (the user's own folder)
    else:
        subprocess.Popen(["xdg-open", str(path)])


class App:
    def __init__(self, root):
        self.root = root
        self.settings = load_settings()
        self.source = None
        self.pc_source = None
        self.pc_tools = tk.StringVar(value=self.settings.get("pc_tools", ""))
        self.ce_tools = tk.StringVar(value=self.settings.get("ce_tools", os.environ.get("N64_CE_TOOLS", "")))
        self.custom_maps = [p for p in self.settings.get("custom_maps", []) if Path(p).is_file()]
        self.builder_thread = None
        self.cancel = threading.Event()
        self.messages = queue.Queue()
        self.started = None
        self.outputs = []

        root.title(TITLE)
        root.minsize(720, 640)
        root.geometry(self.settings.get("geometry", "760x700"))
        try:
            import sv_ttk
            sv_ttk.set_theme("dark")
        except ImportError:
            ttk.Style().theme_use("clam")
        self.style = ttk.Style()
        self.style.configure("Title.TLabel", font=("Segoe UI Semibold", 20))
        self.style.configure("Subtitle.TLabel", font=("Segoe UI", 10))
        self.style.configure("Section.TLabel", font=("Segoe UI Semibold", 12))
        self.style.configure("Small.TLabel", font=("Segoe UI", 9))
        self.style.configure("Build.TButton", font=("Segoe UI Semibold", 12), padding=(24, 8))

        outer = ttk.Frame(root, padding=(20, 16, 20, 12))
        outer.pack(fill="both", expand=True)
        outer.columnconfigure(0, weight=1)

        # the header
        ttk.Label(outer, text="HALO 64", style="Title.TLabel").grid(row=0, column=0, sticky="w")
        ttk.Label(outer, style="Subtitle.TLabel", wraplength=700, justify="left",
                  text="Makes the Halo 64 cartridges for your Nintendo 64 or emulator from your own copy of "
                       "Halo: Combat Evolved for the original Xbox, and Halo Custom Edition maps you add."
                  ).grid(row=1, column=0, sticky="w", pady=(0, 12))

        # 1. Halo
        ttk.Label(outer, text="1   Your Halo", style="Section.TLabel").grid(row=2, column=0, sticky="w")
        drop = tk.Frame(outer, highlightthickness=2, highlightbackground="#5a5a5a", bd=0, height=110,
                        cursor="hand2")
        drop.grid(row=3, column=0, sticky="ew", pady=(6, 4))
        drop.columnconfigure(0, weight=1)
        self.drop = drop
        self.drop_text = tk.Label(drop, text="Drag your Halo Xbox disc image (.iso) or its maps folder here",
                                  font=("Segoe UI", 11), pady=10)
        self.drop_text.grid(row=0, column=0, pady=(18, 4))
        buttons = ttk.Frame(drop)
        buttons.grid(row=1, column=0, pady=(0, 14))
        ttk.Button(buttons, text="Choose disc image…", command=self.choose_image).pack(side="left", padx=6)
        ttk.Button(buttons, text="Choose maps folder…", command=self.choose_folder).pack(side="left", padx=6)
        # (a plain label: the theme keeps a ttk label's colour its own)
        self.source_label = tk.Label(outer, text="", font=("Segoe UI", 10), wraplength=700, justify="left",
                                     anchor="w")
        self.source_label.grid(row=4, column=0, sticky="w", pady=(0, 10))
        self.match_drop_colours()

        # 2. the cartridges
        ttk.Label(outer, text="2   Cartridges", style="Section.TLabel").grid(row=5, column=0, sticky="w")
        choices = ttk.Frame(outer)
        choices.grid(row=6, column=0, sticky="ew", pady=(6, 10))
        self.choice = {}
        pc_included = pc_expansion_included()
        rows = {}
        row = 0
        for key, (name, file, description) in CARTRIDGE_TEXT.items():
            included = (key != "pc_expansion" or pc_included) and (key != "custom" or custom_included())
            var = tk.BooleanVar(value=included and self.settings.get(key, key not in ("pc_expansion", "custom")))
            self.choice[key] = var
            ttk.Checkbutton(choices, text=f"{name}   ({file})", variable=var, command=self.toggle_pc_inputs,
                            state="normal" if included else "disabled").grid(row=row, column=0, sticky="w")
            if not included:
                description += (" (not in this download: it carries no PC conversion tools)" if key == "pc_expansion"
                                else " (not in this download: it carries no Custom Edition conversion tools)")
            ttk.Label(choices, text=description, style="Small.TLabel", wraplength=670).grid(row=row + 1, column=0, sticky="w",
                                                                            padx=(28, 0), pady=(0, 6))
            # (each optional cartridge's own inputs under it)
            rows[key] = row + 2
            row += 3

        # The PC source is part of the optional cartridge's existing flow.
        self.pc_panel = ttk.Frame(choices, padding=(28, 4, 0, 6))
        self.pc_panel.grid(row=rows["pc_expansion"], column=0, sticky="ew")
        ttk.Label(self.pc_panel, text="Your Halo: Combat Evolved for PC", style="Section.TLabel").grid(
            row=0, column=0, sticky="w")
        ttk.Label(self.pc_panel, text="Choose a PC retail ISO with loose maps, or the installed game's maps folder. "
                  "Installer-only discs: install your copy first, then choose its maps folder.",
                  style="Small.TLabel", wraplength=660).grid(row=1, column=0, sticky="w", pady=(2, 6))
        pc_buttons = ttk.Frame(self.pc_panel)
        pc_buttons.grid(row=2, column=0, sticky="w")
        ttk.Button(pc_buttons, text="Choose PC disc image…", command=self.choose_pc_image).pack(side="left", padx=(0, 12))
        ttk.Button(pc_buttons, text="Choose PC maps folder…", command=self.choose_pc_folder).pack(side="left")
        self.pc_source_label = ttk.Label(self.pc_panel, text="No PC source selected", style="Small.TLabel", wraplength=660)
        self.pc_source_label.grid(row=3, column=0, sticky="w", pady=(6, 0))
        if payload_root() is None:
            tools_row = ttk.Frame(self.pc_panel)
            tools_row.grid(row=4, column=0, sticky="ew", pady=(6, 0))
            ttk.Label(tools_row, text="Local PC conversion tools:", style="Small.TLabel").pack(side="left")
            ttk.Entry(tools_row, textvariable=self.pc_tools, width=36).pack(side="left", padx=6)
            ttk.Button(tools_row, text="Browse…", command=self.choose_pc_tools).pack(side="left")

        # the Custom Edition maps: a list, added by button or dropped
        self.custom_panel = ttk.Frame(choices, padding=(28, 4, 0, 6))
        self.custom_panel.grid(row=rows["custom"], column=0, sticky="ew")
        self.custom_panel.columnconfigure(0, weight=1)
        ttk.Label(self.custom_panel, text="Your Custom Edition maps", style="Section.TLabel").grid(
            row=0, column=0, sticky="w")
        ttk.Label(self.custom_panel, style="Small.TLabel", wraplength=660,
                  text="Add Halo Custom Edition multiplayer maps (.map files, or the .zip files maps come in), or "
                       "drag them onto this window. Each is made an Xbox map with your Xbox maps' tags filling in "
                       "Custom Edition's shared ones; maps their authors protected can't be converted.").grid(
            row=1, column=0, sticky="w", pady=(2, 6))
        listing = ttk.Frame(self.custom_panel)
        listing.grid(row=2, column=0, sticky="ew")
        listing.columnconfigure(0, weight=1)
        self.custom_list = tk.Listbox(listing, height=4, selectmode="extended", activestyle="none",
                                      font=("Segoe UI", 9), bd=0, highlightthickness=1)
        self.custom_list.grid(row=0, column=0, sticky="ew")
        # (in the theme's colours: a plain tk widget keeps its own)
        background = self.style.lookup("TFrame", "background") or "#1c1c1c"
        foreground = self.style.lookup("TLabel", "foreground") or "#ffffff"
        self.custom_list.configure(bg=background, fg=foreground, selectbackground="#2f5f86",
                                   selectforeground=foreground, highlightbackground="#5a5a5a")
        custom_buttons = ttk.Frame(listing)
        custom_buttons.grid(row=0, column=1, sticky="n", padx=(8, 0))
        ttk.Button(custom_buttons, text="Add maps…", command=self.choose_custom_maps).pack(fill="x")
        ttk.Button(custom_buttons, text="Remove", command=self.remove_custom_maps).pack(fill="x", pady=(4, 0))
        ttk.Button(custom_buttons, text="Clear", command=self.clear_custom_maps).pack(fill="x", pady=(4, 0))
        self.custom_label = ttk.Label(self.custom_panel, text="", style="Small.TLabel", wraplength=660)
        self.custom_label.grid(row=3, column=0, sticky="w", pady=(6, 0))
        if payload_root() is None:
            ce_row = ttk.Frame(self.custom_panel)
            ce_row.grid(row=4, column=0, sticky="ew", pady=(6, 0))
            ttk.Label(ce_row, text="Local Custom Edition tools:", style="Small.TLabel").pack(side="left")
            ttk.Entry(ce_row, textvariable=self.ce_tools, width=36).pack(side="left", padx=6)
            ttk.Button(ce_row, text="Browse…", command=self.choose_ce_tools).pack(side="left")
        self.show_custom_maps()
        self.toggle_pc_inputs()

        # 3. where they go
        ttk.Label(outer, text="3   Save the ROMs in", style="Section.TLabel").grid(row=7, column=0, sticky="w")
        where = ttk.Frame(outer)
        where.grid(row=8, column=0, sticky="ew", pady=(6, 12))
        where.columnconfigure(0, weight=1)
        self.output = tk.StringVar(value=self.settings.get("output", default_output()))
        ttk.Entry(where, textvariable=self.output).grid(row=0, column=0, sticky="ew")
        ttk.Button(where, text="Browse…", command=self.choose_output).grid(row=0, column=1, padx=(8, 0))

        # build, progress, details
        actions = ttk.Frame(outer)
        actions.grid(row=9, column=0, sticky="ew")
        actions.columnconfigure(2, weight=1)
        self.build_button = ttk.Button(actions, text="Build ROMs", style="Build.TButton", command=self.build)
        try:
            self.build_button.configure(style="Accent.TButton")
        except tk.TclError:
            pass
        self.build_button.grid(row=0, column=0, sticky="w")
        self.cancel_button = ttk.Button(actions, text="Cancel", command=self.cancel_build, state="disabled")
        self.cancel_button.grid(row=0, column=1, padx=(10, 0))
        self.open_button = ttk.Button(actions, text="Open folder", command=self.open_output)
        self.open_button.grid(row=0, column=3, sticky="e")
        self.open_button.grid_remove()
        self.progress = ttk.Progressbar(outer, mode="determinate", maximum=1000)
        self.progress.grid(row=10, column=0, sticky="ew", pady=(12, 4))
        status = ttk.Frame(outer)
        status.grid(row=11, column=0, sticky="ew")
        status.columnconfigure(0, weight=1)
        self.step = ttk.Label(status, text="Choose your cartridges and source files, then build.", style="Small.TLabel")
        self.step.grid(row=0, column=0, sticky="w")
        self.clock = ttk.Label(status, text="", style="Small.TLabel")
        self.clock.grid(row=0, column=1, sticky="e")
        self.details_shown = tk.BooleanVar(value=False)
        ttk.Checkbutton(outer, text="Show details", variable=self.details_shown,
                        command=self.toggle_details).grid(row=12, column=0, sticky="w", pady=(6, 0))
        logs = ttk.Frame(outer)
        logs.grid(row=13, column=0, sticky="nsew", pady=(4, 0))
        logs.columnconfigure(0, weight=1)
        logs.rowconfigure(0, weight=1)
        self.log_frame = logs
        self.log = tk.Text(logs, height=10, wrap="none", font=("Consolas", 9), state="disabled", bd=0)
        scroll = ttk.Scrollbar(logs, orient="vertical", command=self.log.yview)
        self.log.configure(yscrollcommand=scroll.set)
        self.log.grid(row=0, column=0, sticky="nsew")
        scroll.grid(row=0, column=1, sticky="ns")
        logs.grid_remove()
        outer.rowconfigure(13, weight=1)
        ttk.Label(outer, style="Small.TLabel", wraplength=700, justify="left",
                  text="No Halo files come with this program: it converts your own copy.").grid(
            row=14, column=0, sticky="w", pady=(10, 0))

        # drag and drop: on the drop box and anywhere on the window
        if TkinterDnD is not None:
            for widget in (root, drop, self.drop_text):
                widget.drop_target_register(DND_FILES)
                widget.dnd_bind("<<Drop>>", self.dropped)
                widget.dnd_bind("<<DropEnter>>", lambda e: self.highlight(True))
                widget.dnd_bind("<<DropLeave>>", lambda e: self.highlight(False))
        else:
            self.drop_text.configure(text="Choose your Halo Xbox disc image (.iso) or its maps folder")
        for widget in (drop, self.drop_text):
            widget.bind("<Button-1>", lambda e: self.choose_image())

        root.protocol("WM_DELETE_WINDOW", self.close)
        if self.settings.get("source") and Path(self.settings["source"]).exists():
            self.set_source(self.settings["source"], quiet=True)
        if self.settings.get("pc_source") and self.choice["pc_expansion"].get():
            self.set_pc_source(self.settings["pc_source"], quiet=True)
        self.root.after(100, self.poll)

    # ---------- the source

    def match_drop_colours(self):
        background = self.style.lookup("TFrame", "background") or "#1c1c1c"
        foreground = self.style.lookup("TLabel", "foreground") or "#ffffff"
        self.drop.configure(bg=background)
        self.drop_text.configure(bg=background, fg=foreground)
        self.source_label.configure(bg=background, fg=foreground)

    def highlight(self, on):
        self.drop.configure(highlightbackground="#4cc2ff" if on else "#5a5a5a")
        return "copy"

    def dropped(self, event):
        self.highlight(False)
        paths = self.root.tk.splitlist(event.data)
        # (Custom Edition maps and their .zip files: the custom cartridge's;
        # anything else: Halo's Xbox disc image or maps folder)
        custom = [p for p in paths if Path(p).is_file() and Path(p).suffix.lower() in CE_FILES]
        if custom:
            self.add_custom_maps(custom)
        elif paths:
            self.set_source(paths[0])
        return "copy"

    def choose_image(self):
        if self.busy():
            return
        path = filedialog.askopenfilename(title="Your Halo: Combat Evolved Xbox disc image",
                                          filetypes=[("Xbox disc images", "*.iso *.xiso"), ("All files", "*.*")])
        if path:
            self.set_source(path)

    def choose_folder(self):
        if self.busy():
            return
        path = filedialog.askdirectory(title="Halo's maps folder (from the Xbox disc)")
        if path:
            self.set_source(path)

    def set_source(self, path, quiet=False):
        if self.busy():
            return
        try:
            source = MapSource(path)
            try:
                source.validate(["ui", "a30", "bloodgulch"])
            finally:
                source.close()
        except (BuildError, ValueError, OSError) as error:
            self.source = None
            if not quiet:
                self.show_source(f"✖  {error}", False)
            return
        needed = {"ui", "a30", "bloodgulch"}
        missing = source.missing(sorted(needed))
        source.close()
        if missing:
            self.source = None
            self.show_source(f"✖  {Path(path).name}: no {', '.join(missing)} map -- Halo 64 needs Halo: Combat "
                             "Evolved for the original Xbox", False)
            return
        self.source = str(path)
        self.show_source(f"✔  {Path(path).name}: {source.describe()}", True)
        self.drop_text.configure(text=Path(path).name)

    def show_source(self, text, good):
        self.source_label.configure(text=text, fg="#6ccb5f" if good else "#ff6b6b")

    def toggle_pc_inputs(self):
        if not hasattr(self, "pc_panel") or not hasattr(self, "custom_panel"):
            return
        shown = 0
        for key, panel in (("pc_expansion", self.pc_panel), ("custom", self.custom_panel)):
            if self.choice[key].get():
                panel.grid()
                shown += 1
            else:
                panel.grid_remove()
        if shown:
            # Keep the optional source controls and Build button visible.
            height = min(760 + 170 * shown, self.root.winfo_screenheight() - 80)
            self.root.geometry(f"{max(760, self.root.winfo_width())}x{height}")

    # ---------- the Custom Edition maps

    def choose_custom_maps(self):
        if self.busy():
            return
        paths = filedialog.askopenfilenames(title="Halo Custom Edition maps",
                                            filetypes=[("Custom Edition maps", "*.map *.zip"), ("All files", "*.*")])
        if paths:
            self.add_custom_maps(paths)

    def add_custom_maps(self, paths):
        if self.busy():
            return
        known = {str(Path(p).resolve()).lower() for p in self.custom_maps}
        for path in paths:
            if str(Path(path).resolve()).lower() not in known:
                self.custom_maps.append(str(path))
                known.add(str(Path(path).resolve()).lower())
        if self.choice["custom"].get() is False and custom_included():
            self.choice["custom"].set(True)
            self.toggle_pc_inputs()
        self.show_custom_maps()

    def remove_custom_maps(self):
        if self.busy():
            return
        for index in reversed(self.custom_list.curselection()):
            del self.custom_maps[index]
        self.show_custom_maps()

    def clear_custom_maps(self):
        if self.busy():
            return
        self.custom_maps = []
        self.show_custom_maps()

    def show_custom_maps(self):
        """the list, each map checked as it is shown (a Custom Edition
        multiplayer map, not protected)"""
        from .ce_prepare import CEPrepareError, read_ce_map
        from .rombuilder import zip_maps
        self.custom_list.delete(0, "end")
        problems = 0
        for path in map(Path, self.custom_maps):
            try:
                if path.suffix.lower() == ".zip":
                    count = len(zip_maps(path))
                    text = f"✔  {path.name}  ({count} map{'s' if count != 1 else ''} inside)"
                else:
                    text = f"✔  {path.name}  (\"{read_ce_map(path)[0]}\")"
            except (BuildError, CEPrepareError, OSError) as error:
                text = f"✖  {path.name}: {error}"
                problems += 1
            self.custom_list.insert("end", text)
        if not self.custom_maps:
            self.custom_label.configure(text="No Custom Edition maps added yet")
        else:
            self.custom_label.configure(text=f"{len(self.custom_maps)} file{'s' if len(self.custom_maps) != 1 else ''}"
                                        + (f"; {problems} can't be used (remove {'it' if problems == 1 else 'them'})"
                                           if problems else ""))

    def choose_ce_tools(self):
        if self.busy():
            return
        path = filedialog.askdirectory(title="Invader's tools and ogg2wav (tools/n64/invader_lite)")
        if path:
            self.ce_tools.set(path)

    def choose_pc_image(self):
        if self.busy():
            return
        path = filedialog.askopenfilename(title="Your Halo: Combat Evolved PC retail disc image",
                                         filetypes=[("PC disc images", "*.iso"), ("All files", "*.*")])
        if path:
            self.set_pc_source(path)

    def choose_pc_folder(self):
        if self.busy():
            return
        path = filedialog.askdirectory(title="Halo PC's installed maps folder")
        if path:
            self.set_pc_source(path)

    def choose_pc_tools(self):
        if self.busy():
            return
        path = filedialog.askdirectory(title="Trusted local Invader and FFmpeg tools")
        if path:
            self.pc_tools.set(path)

    def set_pc_source(self, path, quiet=False):
        if self.busy():
            return
        from .pc_input import PCMapSource, PC_MAPS
        try:
            source = PCMapSource(path)
            try:
                source.validate([*PC_MAPS, "ui"])
                description = f"{Path(path).name}: {len(source.names())} PC maps"
            finally:
                source.close()
        except (ValueError, OSError) as error:
            self.pc_source = None
            self.pc_source_label.configure(text=f"✖  {error}")
            return
        self.pc_source = str(path)
        self.pc_source_label.configure(text=f"✔  {description}")

    # ---------- where the ROMs go

    def choose_output(self):
        path = filedialog.askdirectory(title="Save the ROMs in", initialdir=self.output.get() or None)
        if path:
            self.output.set(path)

    def open_output(self):
        target = Path(self.output.get())
        if target.is_dir():
            open_folder(target)

    # ---------- the build

    def busy(self):
        return self.builder_thread is not None and self.builder_thread.is_alive()

    def build(self):
        if self.busy():
            return
        if not self.source:
            messagebox.showinfo(TITLE, "First choose your Halo Xbox disc image, or its maps folder.")
            return
        cartridges = [key for key, var in self.choice.items() if var.get()]
        if not cartridges:
            messagebox.showinfo(TITLE, "Tick at least one cartridge to build.")
            return
        if "pc_expansion" in cartridges and not self.pc_source:
            messagebox.showinfo(TITLE, "Choose your Halo PC retail disc image or installed maps folder for the PC expansion.")
            return
        if "custom" in cartridges and not self.custom_maps:
            messagebox.showinfo(TITLE, "Add your Custom Edition maps (.map or .zip files) for the Custom Edition cartridge.")
            return
        output = self.output.get().strip()
        if not output:
            messagebox.showinfo(TITLE, "Choose a folder for the ROMs.")
            return
        self.remember()
        self.cancel.clear()
        self.outputs = []
        self.clear_log()
        self.progress.configure(value=0)
        self.open_button.grid_remove()
        self.build_button.configure(state="disabled")
        self.cancel_button.configure(state="normal")
        self.started = time.time()
        self.builder_thread = threading.Thread(target=self.run_build, args=(self.source, output, cartridges, self.pc_source if "pc_expansion" in cartridges else None,
                                                     self.pc_tools.get().strip() or None,
                                                     list(self.custom_maps) if "custom" in cartridges else None,
                                                     self.ce_tools.get().strip() or None),
                                               daemon=True)
        self.builder_thread.start()

    def run_build(self, source, output, cartridges, pc_source=None, pc_tools=None, custom_maps=None, ce_tools=None):
        def log(text):
            self.messages.put(("log", text))

        def progress(fraction, step):
            self.messages.put(("progress", (fraction, step)))

        try:
            builder = RomBuilder(source, output, cartridges, log=log, progress=progress, cancel=self.cancel, pc_source=pc_source, pc_tools=pc_tools,
                                 custom_maps=custom_maps, ce_tools=ce_tools)
            made = builder.run()
            self.messages.put(("done", (made, list(builder.notes))))
        except Cancelled:
            self.messages.put(("cancelled", None))
        except BuildError as error:
            self.messages.put(("failed", str(error)))
        except Exception as error:  # noqa: BLE001 (shown to the player)
            import traceback
            log(traceback.format_exc())
            self.messages.put(("failed", f"{type(error).__name__}: {error}"))

    def cancel_build(self):
        if self.busy():
            self.cancel.set()
            self.step.configure(text="Stopping…")
            self.cancel_button.configure(state="disabled")

    def poll(self):
        try:
            while True:
                kind, value = self.messages.get_nowait()
                if kind == "log":
                    self.append_log(value)
                elif kind == "progress":
                    fraction, step = value
                    self.progress.configure(value=int(fraction * 1000))
                    if not self.cancel.is_set():
                        self.step.configure(text=step)
                elif kind == "done":
                    made, notes = value
                    self.finished(True, "Your ROMs are ready: " + ", ".join(Path(p).name for p in made))
                    self.outputs = made
                    self.open_button.grid()
                    if notes:
                        messagebox.showwarning(TITLE, "\n\n".join(notes)[:1500])
                elif kind == "cancelled":
                    self.finished(False, "Stopped.")
                elif kind == "failed":
                    self.finished(False, "The build failed.")
                    self.details_shown.set(True)
                    self.toggle_details()
                    self.append_log(value)
                    messagebox.showerror(TITLE, value[:1500])
        except queue.Empty:
            pass
        if self.busy() and self.started:
            elapsed = int(time.time() - self.started)
            self.clock.configure(text=f"{elapsed // 60}:{elapsed % 60:02d}")
        self.root.after(150, self.poll)

    def finished(self, success, text):
        self.build_button.configure(state="normal")
        self.cancel_button.configure(state="disabled")
        self.step.configure(text=text)
        if success:
            self.progress.configure(value=1000)

    # ---------- the log

    def toggle_details(self):
        if self.details_shown.get():
            self.log_frame.grid()
        else:
            self.log_frame.grid_remove()

    def clear_log(self):
        self.log.configure(state="normal")
        self.log.delete("1.0", "end")
        self.log.configure(state="disabled")

    def append_log(self, text):
        self.log.configure(state="normal")
        self.log.insert("end", text.rstrip() + "\n")
        self.log.see("end")
        self.log.configure(state="disabled")

    # ---------- closing

    def remember(self):
        values = {"output": self.output.get(), "geometry": self.root.geometry()}
        if self.source:
            values["source"] = self.source
        if self.pc_tools.get().strip():
            values["pc_tools"] = self.pc_tools.get().strip()
        if self.pc_source:
            values["pc_source"] = self.pc_source
        if self.ce_tools.get().strip():
            values["ce_tools"] = self.ce_tools.get().strip()
        values["custom_maps"] = list(self.custom_maps)
        for key, var in self.choice.items():
            values[key] = var.get()
        save_settings(values)

    def close(self):
        if self.busy():
            if not messagebox.askyesno(TITLE, "A build is running. Stop it and quit?"):
                return
            self.cancel.set()
            self.builder_thread.join(timeout=10)
        self.remember()
        self.root.destroy()


def main():
    root = TkinterDnD.Tk() if TkinterDnD is not None else tk.Tk()
    icon = (payload_root() or Path(__file__).parent) / "rombuilder.png"
    if icon.is_file():
        try:
            root.iconphoto(True, tk.PhotoImage(file=str(icon)))
        except tk.TclError:
            pass
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
