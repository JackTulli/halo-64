"""Repacking an Xbox map's tags into one compact, big-endian image.

Every struct instance the kept tags reach (tag roots, block elements, data
blobs) is copied into a new image with its multi-byte fields swapped to big
endian (the VR4300's byte order) using the Invader layouts (tagdefs.py).
Pointers become offsets into the image, listed in a relocation table that
the N64 adds the image's load address to (port/n64/src/n64_cache.c). Tag
indices are kept: tag i of the Xbox map is tag i of the image, so every
datum index in the data stays valid; tags that are left out keep their
instance with group NONE and no address.

What is kept, dropped or replaced is decided by a Policy (convert.py).
"""

import struct

from .tagdefs import TagDefinitions
from .walk import Walker

NONE = 0xFFFFFFFF


def swap_array(data, element):
    """byte-swaps a buffer of `element`-byte integers in place (bytes)"""
    if element <= 1 or not data:
        return bytes(data)
    if element == 2:
        fmt_in, fmt_out = "<%dH", ">%dH"
    elif element == 4:
        fmt_in, fmt_out = "<%dI", ">%dI"
    else:
        raise ValueError(element)
    count = len(data) // element
    tail = data[count * element:]
    return struct.pack(fmt_out % count, *struct.unpack(fmt_in % count, data[:count * element])) + bytes(tail)


class Image:
    """the output image: bytes, relocations (offsets of pointer fields)"""

    def __init__(self):
        self.data = bytearray()
        self.relocs = []

    def alloc(self, size, align=4):
        pad = (-len(self.data)) % align
        self.data += b"\0" * pad
        offset = len(self.data)
        self.data += b"\0" * size
        return offset

    def put(self, offset, raw):
        self.data[offset:offset + len(raw)] = raw

    def put_u32(self, offset, value):
        struct.pack_into(">I", self.data, offset, value & 0xFFFFFFFF)

    def put_pointer(self, offset, target):
        """a pointer field at offset that points to image offset target
        (None = null)"""
        if target is None:
            self.put_u32(offset, 0)
        else:
            self.put_u32(offset, target)
            self.relocs.append(offset)


class External:
    """a data blob the policy put in a file of its own: the tag data keeps
    its size, flag 1 in its pad field and the offset in that file"""

    def __init__(self, offset, size):
        self.offset, self.size = offset, size


class Policy:
    """what the repacker keeps; convert.py subclasses it"""

    def keep_tag(self, tag):
        return True

    def keep_block(self, tag, path, count):
        """how many elements of a block to keep (0 drops it)"""
        return count

    def data(self, tag, path, parent, raw):
        """the bytes to store for a data blob (already copied from the map,
        little-endian), swapped; None drops it"""
        return None

    def pointer(self, tag, path, value):
        """an image offset for a raw Pointer field, or None"""
        return None

    def override(self, tag, struct_name, field_name, address):
        """a value (u32) to store in a 4-byte field instead of the map's,
        or None; address is the Xbox address of the struct instance"""
        return None

    def custom_tag(self, tag):
        """the bytes of a kept tag the N64 keeps in a form of its own (no
        pointers), or None to repack it"""
        return None


class Repacker:
    def __init__(self, xmap, policy, defs=None):
        self.map = xmap
        self.defs = defs or TagDefinitions()
        self.walker = Walker(xmap, self.defs)
        self.policy = policy
        self.image = Image()
        self.names = {}
        self.stats = {}

    def name_offset(self, text):
        if text not in self.names:
            raw = text.encode("latin-1") + b"\0"
            o = self.image.alloc(len(raw), 1)
            self.image.put(o, raw)
            self.names[text] = o
        return self.names[text]

    def count(self, tag, size):
        self.stats[tag.group_name] = self.stats.get(tag.group_name, 0) + size

    # ---------- structs

    def struct_array(self, tag, address, name, count, path):
        """copies `count` instances of struct `name` from the map address;
        returns the image offset"""
        size = self.defs.size(name)
        offset = self.image.alloc(size * count)
        self.count(tag, size * count)
        raw = self.map.read(address, size * count)
        for i in range(count):
            self.struct_into(tag, address + i * size, raw[i * size:(i + 1) * size], name, offset + i * size, path)
        return offset

    def struct_into(self, tag, address, raw, name, out, path):
        for o, f in self.defs.fields(name):
            fsize = f.size * f.count
            src = raw[o:o + fsize]
            dst = out + o
            if f.kind == "pad":
                self.image.put(dst, src)
            elif f.kind == "prim":
                value = self.policy.override(tag, name, f.name, address) if f.size == 4 else None
                if value is not None:
                    self.image.put_u32(dst, value)
                else:
                    self.image.put(dst, swap_array(src, f.swap))
            elif f.kind == "struct":
                for i in range(f.count):
                    self.struct_into(tag, address + o + i * f.size, src[i * f.size:(i + 1) * f.size],
                                     f.struct, dst + i * f.size, f"{path}.{f.name}")
            elif f.kind == "TagReflexive":
                for i in range(f.count):
                    s = src[i * 12:(i + 1) * 12]
                    count, pointer, _ = struct.unpack("<iII", s)
                    sub = f"{path}.{f.name}[]"
                    keep = self.policy.keep_block(tag, sub, count) if count > 0 else 0
                    target = None
                    if keep > 0:
                        target = self.struct_array(tag, pointer, f.struct, keep, sub)
                    self.image.put_u32(dst + i * 12, max(keep, 0))
                    self.image.put_pointer(dst + i * 12 + 4, target)
                    self.image.put_u32(dst + i * 12 + 8, 0)
            elif f.kind == "TagDataOffset":
                size, flags, file_offset, pointer, unused = struct.unpack("<iIIII", src[:20])
                sub = f"{path}.{f.name}"
                blob = None
                if size > 0 and pointer:
                    try:
                        source = self.map.read(pointer, size)
                    except KeyError:
                        source = None
                    if source is not None:
                        blob = self.policy.data(tag, sub, (address, name, raw), source)
                target = None
                if isinstance(blob, External):
                    self.image.put(dst, struct.pack(">iIIII", blob.size, 1, blob.offset, 0, 0))
                    continue
                if blob is not None:
                    target = self.image.alloc(len(blob), 8)
                    self.image.put(target, blob)
                    self.count(tag, len(blob))
                self.image.put(dst, struct.pack(">iIIII", len(blob) if blob is not None else 0, 0, 0, 0, 0))
                self.image.put_pointer(dst + 12, target)
            elif f.kind == "TagDependency":
                for i in range(f.count):
                    group, name_ptr, nlen, datum = struct.unpack("<IIiI", src[i * 16:(i + 1) * 16])
                    target = self.map.tag(datum)
                    if target is None or not self.policy.keep_tag(target):
                        group, datum, nlen, name_off = (group if target is None else group), NONE, 0, None
                    else:
                        name_off = self.name_offset(target.name)
                    self.image.put(dst + i * 16, struct.pack(">IIiI", group, 0, nlen, datum))
                    self.image.put_pointer(dst + i * 16 + 4, name_off)
            else:
                raise ValueError(f.kind)
            if f.type == "Pointer":
                for i in range(f.count):
                    value = struct.unpack_from("<I", src, i * 4)[0]
                    target = self.policy.pointer(tag, f"{path}.{f.name}", value)
                    self.image.put_pointer(dst + i * 4, target)
            elif f.type == "TagID":
                for i in range(f.count):
                    datum = struct.unpack_from("<I", src, i * 4)[0]
                    t = self.map.tag(datum)
                    if t is not None and not self.policy.keep_tag(t):
                        self.image.put_u32(dst + i * 4, NONE)

    # ---------- tags

    def repack(self, tags):
        """repacks the given tags; returns the image offsets of their roots"""
        roots = {}
        for tag in tags:
            if not self.policy.keep_tag(tag):
                continue
            blob = self.policy.custom_tag(tag)
            if blob is not None:
                roots[tag.index] = self.image.alloc(len(blob), 8)
                self.image.put(roots[tag.index], blob)
                self.count(tag, len(blob))
                continue
            root = self.defs.fourcc_struct(tag.group)
            roots[tag.index] = self.struct_array(tag, tag.address, root, 1, root)
        return roots
