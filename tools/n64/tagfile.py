"""Halo tag files (the HEK's .sound, .bitmap, ...) read and written whole.

A tag file is a 64-byte header, its group's struct (big-endian, but for the
fields Invader's definitions mark little-endian) and then, field by field,
what the struct points at: a reflexive's elements (all of their structs, then
each element's own data in turn), a data field's bytes, a dependency's path
and its null. ce_prepare.py repairs with it what Invader's tools can't edit:
a sound's samples, a bitmap's pixels. The layouts are Invader's definitions
(tagdefs.py).
"""

import struct
import zlib

from .tagdefs import TagDefinitions

HEADER = 64
_FORMATS = {"int8": "b", "uint8": "B", "int16": "h", "uint16": "H", "Index": "H", "int32": "i", "uint32": "I",
            "float": "f", "Angle": "f", "Fraction": "f", "TagID": "I", "TagFourCC": "I", "Pointer": "I",
            "Rectangle2D": "h", "Point2DInt": "h"}
_LAYOUTS = {}


class TagFileError(ValueError):
    """a tag file that doesn't match its definition"""


class Block:
    """one struct of a tag file: its bytes as stored, and what follows it per
    field (by the field's offset in the struct): a list of Blocks (a
    reflexive), bytes (a data field, a dependency's path)"""

    def __init__(self, defs, name, raw):
        self.defs, self.name, self.raw = defs, name, bytearray(raw)
        self.tails = {}
        if name not in _LAYOUTS:
            _LAYOUTS[name] = {f.name: (offset, f) for offset, f in defs.fields(name)}
        self.layout = _LAYOUTS[name]

    def _field(self, name):
        try:
            return self.layout[name]
        except KeyError:
            raise TagFileError(f"{self.name} has no field {name!r}") from None

    def _format(self, f, count=1):
        if f.kind != "prim":
            raise TagFileError(f"{self.name}.{f.name} is not a number")
        code = _FORMATS.get(f.type, "f" if f.swap == 4 and f.size > 4 else {1: "B", 2: "H", 4: "I"}.get(f.size))
        return ("<" if f.raw.get("endian") == "little" else ">") + code * count

    def get(self, name):
        offset, f = self._field(name)
        return struct.unpack_from(self._format(f), self.raw, offset)[0]

    def values(self, name):
        """all the numbers of a field (a point's, a bounds' from and to)"""
        offset, f = self._field(name)
        return struct.unpack_from(self._format(f, f.size * f.count // f.swap), self.raw, offset)

    def path(self, name):
        """a dependency's tag path (no extension), or """""
        offset, f = self._field(name)
        if f.kind != "TagDependency":
            raise TagFileError(f"{self.name}.{name} is not a dependency")
        return bytes(self.tails.get(offset, b"\0"))[:-1].decode("latin1")

    def set(self, name, value):
        offset, f = self._field(name)
        struct.pack_into(self._format(f), self.raw, offset, value)

    def blocks(self, name):
        """a reflexive's elements"""
        offset, f = self._field(name)
        if f.kind != "TagReflexive":
            raise TagFileError(f"{self.name}.{name} is not a reflexive")
        return self.tails.get(offset, [])

    def data(self, name):
        offset, f = self._field(name)
        if f.kind != "TagDataOffset":
            raise TagFileError(f"{self.name}.{name} is not a data field")
        return bytes(self.tails.get(offset, b""))

    def set_data(self, name, value):
        offset, f = self._field(name)
        if f.kind != "TagDataOffset":
            raise TagFileError(f"{self.name}.{name} is not a data field")
        self.tails[offset] = bytes(value)
        struct.pack_into(">I", self.raw, offset, len(value))


def _read_tails(block, struct_name, base, data, position):
    defs = block.defs
    for offset, f in defs.fields(struct_name):
        for i in range(f.count):
            at = base + offset + i * f.size
            if f.kind == "struct":
                position = _read_tails(block, f.struct, at, data, position)
            elif f.kind == "TagReflexive":
                count = struct.unpack_from(">I", block.raw, at)[0]
                size = defs.size(f.struct)
                if position + count * size > len(data):
                    raise TagFileError(f"{struct_name}.{f.name}: {count} elements past the end of the file")
                elements = [Block(defs, f.struct, data[position + k * size:position + (k + 1) * size])
                            for k in range(count)]
                position += count * size
                for element in elements:
                    position = _read_tails(element, f.struct, 0, data, position)
                block.tails[at] = elements
            elif f.kind == "TagDataOffset":
                size = struct.unpack_from(">I", block.raw, at)[0]
                if position + size > len(data):
                    raise TagFileError(f"{struct_name}.{f.name}: {size} bytes past the end of the file")
                block.tails[at] = data[position:position + size]
                position += size
            elif f.kind == "TagDependency":
                size = struct.unpack_from(">I", block.raw, at + 8)[0]
                if size:
                    if position + size + 1 > len(data) or data[position + size] != 0:
                        raise TagFileError(f"{struct_name}.{f.name}: a bad path")
                    block.tails[at] = data[position:position + size + 1]
                    position += size + 1
    return position


def _write_tails(block, struct_name, base, out):
    for offset, f in block.defs.fields(struct_name):
        for i in range(f.count):
            at = base + offset + i * f.size
            if f.kind == "struct":
                _write_tails(block, f.struct, at, out)
            elif f.kind == "TagReflexive":
                elements = block.tails.get(at, [])
                for element in elements:
                    _count(element, f.struct, 0)
                    out += element.raw
                for element in elements:
                    _write_tails(element, f.struct, 0, out)
            elif at in block.tails:
                out += block.tails[at]


def _count(block, struct_name, base):
    """the reflexives' counts in a struct made its elements'"""
    for offset, f in block.defs.fields(struct_name):
        for i in range(f.count):
            at = base + offset + i * f.size
            if f.kind == "struct":
                _count(block, f.struct, at)
            elif f.kind == "TagReflexive":
                struct.pack_into(">I", block.raw, at, len(block.tails.get(at, [])))


class TagFile:
    """a tag file: its header and its root Block"""

    def __init__(self, path, defs=None):
        self.path = path
        self.defs = defs or TagDefinitions()
        data = path.read_bytes()
        if len(data) < HEADER or data[60:64] != b"blam":
            raise TagFileError(f"{path.name} is not a tag file")
        self.header = bytearray(data[:HEADER])
        fourcc = struct.unpack_from(">I", data, 36)[0]
        name = self.defs.fourcc_struct(fourcc)
        if name is None:
            raise TagFileError(f"{path.name}: an unknown tag group")
        size = self.defs.size(name)
        self.root = Block(self.defs, name, data[HEADER:HEADER + size])
        end = _read_tails(self.root, name, 0, data, HEADER + size)
        if end != len(data):
            raise TagFileError(f"{path.name}: {len(data) - end} bytes left after the tag's data")

    def save(self, path=None):
        out = bytearray()
        body = bytearray()
        _count(self.root, self.root.name, 0)
        _write_tails(self.root, self.root.name, 0, body)
        out += self.root.raw
        out += body
        # (the header's checksum: the inverse of the data's CRC-32, as Invader writes it)
        struct.pack_into(">I", self.header, 40, ~zlib.crc32(out) & 0xFFFFFFFF)
        (path or self.path).write_bytes(bytes(self.header) + bytes(out))
