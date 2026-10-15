"""Reading files out of an Xbox disc image (XDVDFS), for the ROM builder.

An Xbox game disc's file system starts 32 sectors (of 2 KB) into its game
partition with a volume descriptor: "MICROSOFT*XBOX*MEDIA", the root
directory's first sector and size, and the magic again at its end. A
directory is a table of entries, each a node of a binary tree by name: the
offsets (in 4-byte words, from the table's start) of its left and right
subtrees, its first sector, its size, its attributes (0x10: a directory),
its name's length and the name; 0xFF fills a sector's end. The game
partition is the image's start in a trimmed image (extract-xiso's .iso),
0x18300000 bytes in for a whole disc's (Redump's: its video partition
first), and a few other places on other discs.

    python3 -m tools.n64.xiso <image> [<path in it>...]   (lists, or extracts)
"""

import struct
import sys
import os
from pathlib import Path

SECTOR = 2048
MAGIC = b"MICROSOFT*XBOX*MEDIA"
PARTITIONS = (0, 0x18300000, 0x0FD90000, 0x02080000)
DIRECTORY = 0x10
MAX_DEPTH = 32
MAX_DIRECTORY_SIZE = 16 << 20
MAX_DIRECTORY_BYTES = 64 << 20
MAX_ENTRIES = 100000


class XboxDisc:
    """an Xbox disc image's files: {lower-case path: (first sector, size)}"""

    def __init__(self, path):
        self.path = Path(path)
        self.file = open(self.path, "rb")
        self.partition = None
        try:
            self._image_size = os.fstat(self.file.fileno()).st_size
            for offset in PARTITIONS:
                if offset + 33 * SECTOR > self._image_size:
                    continue
                self.file.seek(offset + 32 * SECTOR)
                head = self.file.read(SECTOR)
                if len(head) == SECTOR and head[:20] == MAGIC and head[0x7EC:0x800] == MAGIC:
                    self.partition = offset
                    root_sector, root_size = struct.unpack_from("<II", head, 20)
                    break
            if self.partition is None:
                raise ValueError(f"{self.path.name} is not a complete Xbox disc image")
            self.files = {}
            self.names = {}
            self._directories = set()
            self._paths = set()
            self._directory_bytes = 0
            self._entries = 0
            if root_size < 14:
                raise ValueError("Xbox disc root directory is missing or truncated")
            self._directory(root_sector, root_size, "")
        except BaseException:
            self.file.close()
            raise

    def close(self):
        self.file.close()

    def _read(self, sector, size):
        self._extent(sector, size)
        self.file.seek(self.partition + sector * SECTOR)
        data = self.file.read(size)
        if len(data) != size:
            raise ValueError("Xbox disc ends inside a directory table")
        return data

    def _extent(self, sector, size):
        offset = self.partition + sector * SECTOR
        if (sector < 0 or size < 0 or offset < self.partition or
                offset + size > min(self._image_size, os.fstat(self.file.fileno()).st_size)):
            raise ValueError("Xbox disc file or directory extent is outside the image")

    def _directory(self, sector, size, prefix, depth=0):
        self._extent(sector, size)
        if depth > MAX_DEPTH or size > MAX_DIRECTORY_SIZE:
            raise ValueError("Xbox disc directory depth or table size exceeds the safety limit")
        if size == 0:
            return
        if sector in self._directories:
            raise ValueError("Xbox disc directory cycle or shared-directory alias")
        self._directories.add(sector)
        self._directory_bytes += size
        if self._directory_bytes > MAX_DIRECTORY_BYTES:
            raise ValueError("Xbox disc total directory bytes exceed the safety limit")
        table = self._read(sector, size)
        todo = [0]
        seen = set()
        occupied = bytearray(size)
        children = []
        while todo:
            offset = todo.pop()
            if offset in seen:
                raise ValueError("Xbox disc directory tree contains a cycle or shared pointer")
            if offset + 14 > len(table):
                raise ValueError("Xbox disc directory tree pointer or entry header is truncated")
            seen.add(offset)
            left, right, start, length, attributes, name_length = struct.unpack_from("<HHIIBB", table, offset)
            if left == 0xFFFF and right == 0xFFFF:
                raise ValueError("Xbox disc directory tree points at padding")
            end = offset + 14 + name_length
            if not name_length or end > len(table):
                raise ValueError("Xbox disc directory entry name is empty or truncated")
            if any(occupied[offset:end]):
                raise ValueError("Xbox disc directory tree entries overlap")
            occupied[offset:end] = b"\x01" * (end - offset)
            self._entries += 1
            if self._entries > MAX_ENTRIES:
                raise ValueError("Xbox disc entry count exceeds the safety limit")
            name = table[offset + 14:offset + 14 + name_length].decode("latin-1")
            if (name in (".", "..") or name.endswith((".", " ")) or
                    any(c in "/\\:" or ord(c) < 32 or 127 <= ord(c) <= 159 for c in name)):
                raise ValueError(f"Xbox disc contains an unsafe entry name: {name!r}")
            path = prefix + name
            key = path.lower()
            if key in self._paths:
                raise ValueError(f"Xbox disc contains a case-insensitive path collision: {path}")
            self._paths.add(key)
            self._extent(start, length)
            if attributes & DIRECTORY:
                children.append((start, length, path + "/", depth + 1))
            else:
                self.files[key] = (start, length)
                self.names[key] = path
            if left:
                todo.append(left * 4)
            if right:
                todo.append(right * 4)
        for child in children:
            self._directory(*child)

    def extract(self, name, target, progress=None):
        """copies a file (its path in the disc, any case) to target"""
        start, length = self.files[name.lower()]
        self._extent(start, length)
        target = Path(target)
        if target.resolve() == self.path.resolve() or (target.exists() and os.path.samefile(target, self.path)):
            raise ValueError("Xbox disc extraction target must differ from the source image")
        self.file.seek(self.partition + start * SECTOR)
        done = 0
        with open(target, "wb") as out:
            while done < length:
                chunk = self.file.read(min(4 << 20, length - done))
                if not chunk:
                    raise IOError(f"{self.path.name} ends inside {name}")
                out.write(chunk)
                done += len(chunk)
                if progress:
                    progress(done, length)


def main():
    disc = XboxDisc(sys.argv[1])
    if len(sys.argv) == 2:
        for key in sorted(disc.files):
            print(f"{disc.files[key][1]:12d}  {disc.names[key]}")
    for name in sys.argv[2:]:
        disc.extract(name, Path(name).name)
    disc.close()


if __name__ == "__main__":
    main()
