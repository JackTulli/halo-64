"""Where the converters find libdragon's tools (mksprite, audioconv64, ...):
in N64_TOOLS, a folder of them (the ROM builder's, tools/n64/rombuilder.py),
else in $N64_INST/bin (the toolchain's), else ~/n64/toolchain/bin. On
Windows they are .exe files, and run() gives them paths with forward
slashes: mksprite names its output after its input's part past the last
/, and with backslashes wrote to "<folder>/C:/<the whole input path>"."""

import os
import subprocess
import sys
from pathlib import Path


def tool(name):
    """the path of one of libdragon's tools"""
    folder = os.environ.get("N64_TOOLS")
    if folder:
        base = Path(folder)
    else:
        base = Path(os.environ.get("N64_INST", Path.home() / "n64" / "toolchain")) / "bin"
    return str(base / (name + ".exe" if sys.platform == "win32" else name))


def run(command, **options):
    """subprocess.run for a tool's command, its paths with forward
    slashes on Windows, and no console window of its own (the ROM
    builder's window has no console: each tool's run made one -- a flash
    on the screen, slow, and now and then one failed to start,
    0xC0000142)"""
    if sys.platform == "win32":
        command = [part.replace("\\", "/") if isinstance(part, str) else part for part in command]
        options.setdefault("creationflags", subprocess.CREATE_NO_WINDOW)
        options.setdefault("stdin", subprocess.DEVNULL)
    return subprocess.run(command, **options)
