"""Tag layouts for the converter, from Invader's tag definitions.

Invader (https://github.com/SnowyMouse/invader, GPL-3.0) describes every
Halo CE tag group field by field, Xbox details included. The converter reads
its JSON at run time from INVADER_DEFINITIONS (default ~/n64/invader/src/tag/
hek/definition) rather than carrying a copy.
"""

import json
import os
import re
from pathlib import Path

DEFINITIONS = Path(os.environ.get(
    "INVADER_DEFINITIONS", Path.home() / "n64/invader/src/tag/hek/definition"))
FOURCC_HEADER = DEFINITIONS.parent.parent.parent.parent / "include/invader/hek/fourcc.hpp"

# primitive types: (size, element size for byte swapping)
PRIMITIVES = {
    "int8": (1, 1), "uint8": (1, 1),
    "int16": (2, 2), "uint16": (2, 2), "Index": (2, 2),
    "int32": (4, 4), "uint32": (4, 4), "float": (4, 4), "Angle": (4, 4),
    "Fraction": (4, 4), "TagID": (4, 4), "TagFourCC": (4, 4),
    "Pointer": (4, 4), "ColorARGBInt": (4, 4),
    "Point2D": (8, 4), "Point3D": (12, 4), "Vector2D": (8, 4), "Vector3D": (12, 4),
    "Euler2D": (8, 4), "Euler3D": (12, 4), "ColorRGB": (12, 4), "ColorARGB": (16, 4),
    "Quaternion": (16, 4), "Plane2D": (12, 4), "Plane3D": (16, 4), "Matrix": (36, 4),
    "Rectangle2D": (8, 2), "Point2DInt": (4, 2),
    "TagString": (32, 1),
    # the script node's value: its width follows the node's type, so the
    # converter swaps script syntax data itself
    "ScenarioScriptNodeValue": (4, 0),
}
COMPOUND = {"TagDependency": 16, "TagReflexive": 12, "TagDataOffset": 20}


def _camel_to_snake(name):
    return re.sub(r"(?<!^)(?=[A-Z])", "_", name).lower()


class Field:
    __slots__ = ("name", "type", "size", "count", "struct", "swap", "kind", "raw")

    def __repr__(self):
        return f"<{self.kind} {self.type} {self.name!r} {self.size}x{self.count}>"


class TagDefinitions:
    def __init__(self, directory=None):
        directory = Path(directory if directory is not None else
                         os.environ.get("INVADER_DEFINITIONS", DEFINITIONS)).expanduser().resolve()
        fourcc_header = directory.parent.parent.parent.parent / "include/invader/hek/fourcc.hpp"
        self.structs = {}
        self.enums = {}
        self.bitfields = {}
        self.class_struct = {}
        for path in sorted(directory.glob("*.json")):
            for d in json.load(open(path, encoding="utf-8")):
                kind = d["type"]
                if kind == "struct":
                    self.structs[d["name"]] = d
                    if "class" in d:
                        self.class_struct[d["class"]] = d["name"]
                elif kind == "enum":
                    self.enums[d["name"]] = d
                elif kind == "bitfield":
                    self.bitfields[d["name"]] = d
        self.fourcc_class = {}
        text = fourcc_header.read_text(encoding="utf-8")
        for name, value in re.findall(r"TAG_FOURCC_([A-Z_]+) = (0x[0-9A-Fa-f]+)", text):
            self.fourcc_class[int(value, 16)] = name.lower()
        self._fields = {}

    # ---------- layouts

    def fourcc_struct(self, fourcc):
        """the root struct name of a tag group, or None"""
        cls = self.fourcc_class.get(fourcc)
        return self.class_struct.get(cls) if cls else None

    def size(self, struct):
        return self.structs[struct]["size"]

    def field_size(self, f):
        t = f["type"]
        n = f.get("count", 1)
        if t == "pad":
            return f["size"]
        if t in PRIMITIVES:
            return PRIMITIVES[t][0] * n
        if t in COMPOUND:
            return COMPOUND[t] * n
        if t in self.enums:
            return 2 * n
        if t in self.bitfields:
            return self.bitfields[t]["width"] // 8 * n
        if t in self.structs:
            return self.structs[t]["size"] * n
        raise KeyError(f"unknown field type {t}")

    def fields(self, struct):
        """the flattened field list of a struct (inherited fields first), as
        (offset, Field) pairs"""
        if struct in self._fields:
            return self._fields[struct]
        d = self.structs[struct]
        out = []
        offset = 0
        if "inherits" in d:
            out = list(self.fields(d["inherits"]))
            offset = self.size(d["inherits"])
        for f in d.get("fields", []):
            fld = Field()
            fld.raw = f
            fld.name = f.get("name", "")
            fld.type = f["type"]
            fld.count = f.get("count", 1) * (2 if f.get("bounds") else 1)
            t = fld.type
            if t == "pad":
                fld.kind, fld.size, fld.count, fld.swap = "pad", f["size"], 1, 1
            elif t in PRIMITIVES:
                fld.kind = "prim"
                fld.size, fld.swap = PRIMITIVES[t]
            elif t in COMPOUND:
                fld.kind = t
                fld.size, fld.swap = COMPOUND[t], 4
                fld.struct = f.get("struct")
            elif t in self.enums:
                fld.kind, fld.size, fld.swap = "prim", 2, 2
            elif t in self.bitfields:
                w = self.bitfields[t]["width"] // 8
                fld.kind, fld.size, fld.swap = "prim", w, w
            elif t in self.structs:
                fld.kind, fld.size, fld.struct, fld.swap = "struct", self.size(t), t, 0
            else:
                raise KeyError(f"{struct}.{fld.name}: unknown type {t}")
            out.append((offset, fld))
            offset += fld.size * fld.count
        if offset != d["size"]:
            raise ValueError(f"{struct}: fields add up to {offset}, declared {d['size']}")
        self._fields[struct] = out
        return out


if __name__ == "__main__":
    defs = TagDefinitions()
    bad = 0
    for name in defs.structs:
        try:
            defs.fields(name)
        except Exception as e:  # noqa: BLE001
            bad += 1
            print("BAD", e)
    print(len(defs.structs), "structs,", bad, "bad;", len(defs.fourcc_class), "groups,",
          sum(1 for c in defs.fourcc_class.values() if c in defs.class_struct), "with a struct")
    print([c for c in defs.fourcc_class.values() if c not in defs.class_struct])
