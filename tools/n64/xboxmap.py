"""Reading the Xbox game's cache files (maps/*.map).

An Xbox cache file is a 2 KB header followed by one zlib stream. The tag
data (tag_data_offset/size) loads at TAG_BASE (0x803A6000); it begins with
the tag header (cache_files.c struct cache_file_tag_header) whose tag
instance array names each tag's group, name and address. The structure BSP
loads separately at the address its scenario reference gives
(scenario_structure_bsp_reference: file_offset, file_size, base_address).
"""

import struct
import zlib
from pathlib import Path

TAG_BASE = 0x803A6000


class Tag:
    # (source: the map a tag was imported from, merge.py: its bitmaps'
    # pixels are in that map's file)
    __slots__ = ("index", "datum", "group", "parents", "name", "address", "map", "source")

    def __repr__(self):
        return f"<{self.group_name} {self.name} @{self.address:#x}>"

    @property
    def group_name(self):
        return self.group.to_bytes(4, "big").decode("latin-1")


class XboxMap:
    def __init__(self, path):
        self.path = Path(path)
        raw = self.path.read_bytes()
        self.header = raw[:0x800]
        self.data = raw[:0x800] + zlib.decompressobj().decompress(raw[0x800:])
        (self.signature, self.version, self.file_length, _, self.tag_data_offset,
         self.tag_data_size) = struct.unpack_from("<4sIIIII", self.data, 0)
        self.name = self.data[0x20:0x40].split(b"\0")[0].decode()
        self.build = self.data[0x40:0x60].split(b"\0")[0].decode()
        self.map_type = struct.unpack_from("<H", self.data, 0x60)[0]
        self.tag_data = self.data[self.tag_data_offset:self.tag_data_offset + self.tag_data_size]
        (self.instances_address, self.scenario_datum, self.tags_checksum, self.tag_count,
         self.vertex_buffer_count, self.vertex_buffers, self.index_buffer_count,
         self.index_buffers, self.tag_signature) = struct.unpack_from("<IiIiIIIII", self.tag_data, 0)
        # regions of the console's memory: (base address, bytes)
        self.regions = [(TAG_BASE, self.tag_data)]
        self.tags = []
        for i in range(self.tag_count):
            o = self.instances_address - TAG_BASE + i * 32
            group, p1, p2, datum, name, address, u1, u2 = struct.unpack_from("<IIIiIIII", self.tag_data, o)
            t = Tag()
            t.index, t.datum, t.group, t.parents = i, datum, group, (p1, p2)
            t.name = self.cstring(name)
            t.address = address
            t.map = self
            self.tags.append(t)
        self.scenario = self.tags[self.scenario_datum & 0xFFFF]
        self.bsps = []

    # ---------- memory

    def region(self, address):
        for base, data in self.regions:
            if base <= address < base + len(data):
                return base, data
        raise KeyError(f"address {address:#x} is outside the loaded data")

    def read(self, address, size):
        base, data = self.region(address)
        o = address - base
        if o + size > len(data):
            raise KeyError(f"{address:#x}+{size:#x} runs past its region")
        return data[o:o + size]

    def u32(self, address):
        return struct.unpack("<I", self.read(address, 4))[0]

    def cstring(self, address):
        base, data = self.region(address)
        o = address - base
        return data[o:data.index(b"\0", o)].decode("latin-1")

    def tag(self, datum):
        if datum in (0xFFFFFFFF, -1):
            return None
        t = self.tags[datum & 0xFFFF]
        return t

    def load_bsp(self, index=0):
        """maps the scenario's structure BSP into the address space; returns
        (tag, base address, size)"""
        scenario = self.scenario.address
        count, address = struct.unpack("<iI", self.read(scenario + 0x5A4, 8))
        if index >= count:
            raise IndexError("no such BSP")
        file_offset, file_size, base, _, group, name, nlen, datum = struct.unpack(
            "<iiIIIIiI", self.read(address + index * 32, 32))
        blob = self.data[file_offset:file_offset + file_size]
        self.regions.append((base, blob))
        tag = self.tag(datum)
        # the BSP blob begins with its header (cache_file_structure_bsp_header):
        # the address of the sbsp structure itself
        tag.address = struct.unpack_from("<I", blob, 0)[0]
        self.bsps.append((tag, base, file_size))
        return tag, base, file_size
