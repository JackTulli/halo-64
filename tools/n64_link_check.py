#!/usr/bin/env python3
"""Lists the weak references of the N64 link that nothing defines.

The N64 build marks the game's inline functions `#pragma weak` (tools/
linux_msvc_semantics.py), so a call to one that no linked unit defines (its
body lived in a unit the N64 leaves out) is a weak undefined symbol, which
the linker silently resolves to address 0. This prints those names, one per
line, for tools/n64_stubs.py; with --check it fails instead.

usage: n64_link_check.py <file listing the objects, one per line> [--check]
"""

import shlex
import subprocess
import sys
from pathlib import Path

NM = str(Path.home() / "n64/toolchain/bin/mips64-elf-nm")


def main():
    objects = [o for o in Path(sys.argv[1]).read_text().splitlines() if o]
    defined, weak_undefined = set(), set()
    output = subprocess.run([NM, "-A"] + objects, capture_output=True, text=True).stdout
    for line in output.splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue
        kind, name = parts[-2], parts[-1]
        if kind in ("w", "v"):
            weak_undefined.add(name)
        elif kind not in ("U",) and len(parts) >= 3:
            defined.add(name)
    missing = sorted(weak_undefined - defined)
    if "--check" in sys.argv:
        if missing:
            sys.exit("weak references with no definition (they would be address 0): " + " ".join(missing))
        return
    for name in missing:
        print(name)


if __name__ == "__main__":
    main()
