"""Compress a complete H64M sequential stream and verify its LZ4 round trip."""
import argparse
import os
from pathlib import Path
import shutil
import sys
import tempfile


def decode(data):
    if data[:4] == b"H64M":
        return data
    if len(data) < 5 or data[:4] != b"DCA5" or ((data[4] >> 4) & 3) != 1:
        raise ValueError("Only libdragon DCA5 LZ4 maps are accepted")
    cursor = 5
    values = []
    for _ in range(3):
        value = shift = 0
        while True:
            if cursor >= len(data):
                raise ValueError("Truncated DCA length")
            b = data[cursor]
            cursor += 1
            value |= (b & 127) << shift
            if not b & 128:
                break
            shift += 7
            if shift > 28:
                raise ValueError("Invalid DCA length")
        values.append(value)
    cursor = (cursor + 1) & ~1
    size, original, _ = values
    block = data[cursor:cursor + size]
    if len(block) != size or original > 16 * 1024**2:
        raise ValueError("Invalid map compression sizes")
    out, p = bytearray(), 0
    def length(base):
        nonlocal p
        if base == 15:
            while True:
                if p >= len(block):
                    raise ValueError("Truncated LZ4 length")
                extra = block[p]
                p += 1
                base += extra
                if extra < 255:
                    break
        return base
    while p < len(block):
        token = block[p]
        p += 1
        literals = length(token >> 4)
        if p + literals > len(block) or len(out) + literals > original:
            raise ValueError("Invalid LZ4 literal length")
        out += block[p:p + literals]
        p += literals
        if p == len(block):
            break
        if p + 2 > len(block):
            raise ValueError("Truncated LZ4 match")
        offset = int.from_bytes(block[p:p + 2], "little")
        p += 2
        count = length(token & 15) + 4
        if not offset or offset > len(out) or len(out) + count > original:
            raise ValueError("Invalid LZ4 match")
        for _ in range(count):
            out.append(out[-offset])
    if len(out) != original or out[:4] != b"H64M":
        raise ValueError("Map decompression did not match its declared format/size")
    return bytes(out)


def compress(path, backup=None, sdk=None):
    path = Path(path)
    original = path.read_bytes()
    if original[:4] != b"H64M":
        raise ValueError("Expected an uncompressed freshly converted map")
    if backup is not None:
        backup = Path(backup)
        backup.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, backup)
    from .toolpaths import tool, run
    asset_tool = str(Path(sdk) / "bin" / ("mkasset.exe" if sys.platform == "win32" else "mkasset")) if sdk else tool("mkasset")
    with tempfile.TemporaryDirectory(dir=path.parent) as work:
        run([asset_tool, "-c", "1", "-w", "64", "-o", Path(work).as_posix(), path.as_posix()],
                       check=True, capture_output=True)
        made = Path(work) / path.name
        packed = made.read_bytes()
        if decode(packed) != original:
            raise ValueError("Compressed map failed byte-for-byte round-trip verification")
        os.replace(made, path)
    print(f"{path.stem} compressed: {len(original)} -> {len(packed)} bytes; verified")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("map", type=Path)
    parser.add_argument("--backup", type=Path)
    parser.add_argument("--sdk", type=Path)
    args = parser.parse_args()
    compress(args.map, args.backup, args.sdk)
