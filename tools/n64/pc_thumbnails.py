"""Read the owned PC map-picture tag and append its six original menu images."""
import argparse
from pathlib import Path
import struct
import sys
from types import SimpleNamespace

from .paint_masks import configure
from .toolpaths import tool, run


def export(bitmap, output, *, definitions=None, sdk=None):
    data = Path(bitmap).read_bytes()
    defs = configure(definitions)
    from .bitmaps import BitmapImage, decode
    from .repack import swap_array
    cursor = 64 + defs.size("Bitmap")
    blocks = {}
    def visit(address, name, path):
        nonlocal cursor
        for offset, field in defs.fields(name):
            location, key = address + offset, path + "." + field.name
            if field.kind == "struct":
                visit(location, field.struct, key)
            elif field.kind in ("TagDataOffset", "TagReflexive"):
                count = struct.unpack_from(">i", data, location)[0]
                if count < 0:
                    raise ValueError("Invalid HEK block count")
                start = cursor
                if field.kind == "TagDataOffset":
                    cursor += count
                    blocks[key] = (start, count)
                else:
                    cursor += count * defs.size(field.struct)
                    if cursor > len(data):
                        raise ValueError("Truncated HEK array")
                    blocks[key] = (start, count)
                    for index in range(count):
                        visit(start + index * defs.size(field.struct), field.struct, key + f"[{index}]")
                if cursor > len(data):
                    raise ValueError("Truncated HEK data")
            elif field.kind == "TagDependency":
                size = struct.unpack_from(">i", data, location + 8)[0]
                if size < 0:
                    raise ValueError("Invalid HEK dependency path length")
                cursor += size + 1 if size else 0
                if cursor > len(data):
                    raise ValueError("Truncated HEK dependency path")
    visit(64, "Bitmap", "Bitmap")
    if cursor != len(data):
        raise ValueError("Unexpected HEK bitmap serialization")
    pixels, length = blocks["Bitmap.processed pixel data"]
    start, count = blocks["Bitmap.bitmap data"]
    if count != 21:
        raise ValueError("PC menu picture count changed; verify ordering before using it")
    memory = SimpleNamespace(data=data[pixels:pixels + length])
    out = Path(output)
    out.mkdir(parents=True, exist_ok=True)
    bitmap_size = defs.size("BitmapData")
    for index in range(13, 19):
        raw = bytearray(data[start + index * bitmap_size:start + (index + 1) * bitmap_size])
        for offset, field in defs.fields("BitmapData"):
            size = field.size * field.count
            if field.swap > 1:
                raw[offset:offset + size] = swap_array(raw[offset:offset + size], field.swap)
        image = decode(memory, BitmapImage(raw), 256).convert("RGBA")
        if image.size != (256, 128):
            raise ValueError("Unexpected menu picture dimensions")
        png = out / f"map{index}.png"
        image.save(png)
        run([str(Path(sdk) / "bin" / ("mksprite.exe" if sys.platform == "win32" else "mksprite")) if sdk else tool("mksprite"), "--format", "RGBA16",
                        "--compress", "1", "-o", out.as_posix(), png.as_posix()], check=True, capture_output=True)
        png.unlink()
    print("PC map thumbnails appended at existing 256x128 menu size")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--bitmap", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--definitions", type=Path)
    parser.add_argument("--sdk", type=Path)
    args = parser.parse_args()
    export(args.bitmap, args.output, definitions=args.definitions, sdk=args.sdk)


if __name__ == "__main__":
    main()
