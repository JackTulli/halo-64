#!/usr/bin/env python3
"""Attributes the N64 profiler's samples (port/n64/src/n64_profile.c) to
functions and source files.

    python3 tools/n64_profile.py <log with "prof:" lines> build/n64/halo.elf
"""

import collections
import subprocess
import sys
from pathlib import Path

ADDR2LINE = str(Path.home() / "n64/toolchain/bin/mips64-elf-addr2line")


def main():
    log, elf = sys.argv[1], sys.argv[2]
    pcs = []
    for line in open(log, errors="replace"):
        if line.startswith("prof: ") and not line.startswith("prof: s") and "end" not in line and "samples" not in line:
            pcs += [int(x, 16) for x in line[6:].split()]
    if not pcs:
        sys.exit("no samples")
    unique = sorted(set(pcs))
    out = subprocess.run([ADDR2LINE, "-f", "-e", elf] + [f"{p:x}" for p in unique], capture_output=True, text=True).stdout.splitlines()
    where = {p: (out[2 * i], out[2 * i + 1]) for i, p in enumerate(unique)}
    functions = collections.Counter()
    files = collections.Counter()
    for p in pcs:
        fn, loc = where[p]
        functions[fn] += 1
        path = loc.split(":")[0]
        for key in ("/source/", "/port/", "libdragon/", "newlib", "tiny3d"):
            if key in path:
                path = path[path.index(key):]
                break
        files[path.rsplit("/", 1)[0] if "/" in path else path] += 1
    total = len(pcs)
    print(f"{total} samples")
    print("functions:")
    for fn, n in functions.most_common(45):
        print(f"  {100 * n / total:5.1f}% {fn}")
    print("directories:")
    for f, n in files.most_common(20):
        print(f"  {100 * n / total:5.1f}% {f}")


if __name__ == "__main__":
    main()
