"""Reading the Xbox game's cache files (maps/*.map).

An Xbox cache file is a 2 KB header followed by one zlib stream. The tag
data (tag_data_offset/size) loads at TAG_BASE (0x803A6000); it begins with
the tag header (cache_files.c struct cache_file_tag_header) whose tag
instance array names each tag's group, name and address. The structure BSP
loads separately at the address its scenario reference gives
(scenario_structure_bsp_reference: file_offset, file_size, base_address).
"""

import os
import re
import struct
import zlib
from pathlib import Path

TAG_BASE = 0x803A6000


# Supported offline conversion ceiling, not a console RAM limit. The owned
# 19-map expansion + UI + ten Xbox campaigns peak at 279,763,456 bytes.
MAX_MAP_BYTES = 512 << 20
# Permit zlib framing/deflate overhead and the cache's 4096-byte zero padding.
MAX_MAP_FILE_BYTES = MAX_MAP_BYTES + (1 << 20) + 4096
_MAP_CHUNK = 1 << 20


def read_xbox_map(path):
    """Read one v5 cache with bounded inflation and exact stream/padding checks.

    Accept header-declared zero padding of 0..4095 bytes after the zlib stream.
    Reject changed inputs; open the supplied file read-only throughout.
    """
    with Path(path).open('rb') as stream:
        before = os.fstat(stream.fileno())
        if not 2048 < before.st_size <= MAX_MAP_FILE_BYTES:
            raise ValueError('Xbox cache compressed size exceeds supported bounds')
        header = stream.read(2048)
        if len(header) != 2048 or header[:4] != b'daeh' or header[-4:] != b'toof':
            raise ValueError('Invalid Xbox cache header/footer')
        version, declared, padding, tag_offset, tag_size = struct.unpack_from('<5I', header, 4)
        if version != 5:
            raise ValueError('Expected Xbox cache version 5')
        if (not 2048 < declared <= MAX_MAP_BYTES or padding > 4095 or
                tag_offset < 2048 or tag_size < 36 or tag_offset + tag_size > declared):
            raise ValueError('Invalid Xbox cache declared size, padding or tag bounds')
        raw_name = header[0x20:0x40]
        name = raw_name.split(b'\0', 1)[0]
        if (b'\0' not in raw_name or not re.fullmatch(rb'[A-Za-z0-9][A-Za-z0-9_-]{0,30}', name) or
                re.fullmatch(rb'(?i:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])', name)):
            raise ValueError('Unsafe or unterminated Xbox cache name')
        decompressor = zlib.decompressobj()
        data = bytearray(header)
        remaining = before.st_size - len(header)
        trailing = 0
        try:
            while remaining:
                block = stream.read(min(_MAP_CHUNK, remaining))
                if not block:
                    raise ValueError('Truncated Xbox compressed stream')
                remaining -= len(block)
                if not decompressor.eof:
                    while block:
                        # The extra byte detects overflow without inflating the
                        # rest of a forged stream or allocating declared size.
                        chunk = decompressor.decompress(block, min(_MAP_CHUNK, declared - len(data) + 1))
                        if len(data) + len(chunk) > declared:
                            raise ValueError('Xbox cache exceeds its declared decompressed size')
                        data.extend(chunk)
                        if decompressor.eof:
                            block = decompressor.unused_data
                            break
                        block = decompressor.unconsumed_tail
                if decompressor.eof:
                    trailing += len(block)
                    if trailing > padding or any(block):
                        raise ValueError('Invalid Xbox cache trailing padding')
        except zlib.error as error:
            raise ValueError('Invalid Xbox compressed stream') from error
        after = os.fstat(stream.fileno())
        if stream.read(1) or (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns):
            raise ValueError('Xbox cache changed while reading')
        if not decompressor.eof or len(data) != declared or trailing != padding:
            raise ValueError('Incomplete Xbox stream or inconsistent length/padding')
    tag_header = data[tag_offset:tag_offset + 36]
    address, scenario, _, count = struct.unpack_from('<4I', tag_header)
    relative = address - 0x803A6000
    if (tag_header[32:] != b'sgat' or not 0 < count <= 65535 or relative < 36 or
            relative + count * 32 > tag_size or (scenario & 0xffff) >= count):
        raise ValueError('Invalid Xbox tag table bounds')
    return bytes(data)


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
        self.data = read_xbox_map(self.path)
        self.header = self.data[:0x800]
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
