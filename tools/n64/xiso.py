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
from pathlib import Path

SECTOR = 2048
MAGIC = b"MICROSOFT*XBOX*MEDIA"
PARTITIONS = (0, 0x18300000, 0x0FD90000, 0x02080000)
DIRECTORY = 0x10


class XboxDisc:
    """an Xbox disc image's files: {lower-case path: (first sector, size)}"""

    def __init__(self, path):
        self.path = Path(path)
        self.file = open(self.path, "rb")
        self.partition = None
        for offset in PARTITIONS:
            self.file.seek(offset + 32 * SECTOR)
            head = self.file.read(SECTOR)
            if len(head) == SECTOR and head[:20] == MAGIC and head[0x7EC:0x800] == MAGIC:
                self.partition = offset
                root_sector, root_size = struct.unpack_from("<II", head, 20)
                break
        if self.partition is None:
            self.file.close()
            raise ValueError(f"{self.path.name} is not an Xbox disc image")
        self.files = {}
        self.names = {}
        self._directory(root_sector, root_size, "")

    def close(self):
        self.file.close()

    def _read(self, sector, size):
        self.file.seek(self.partition + sector * SECTOR)
        return self.file.read(size)

    def _directory(self, sector, size, prefix):
        if size <= 0:
            return
        table = self._read(sector, size)
        todo = [0]
        seen = set()
        while todo:
            offset = todo.pop()
            if offset in seen or offset + 14 > len(table):
                continue
            seen.add(offset)
            left, right, start, length, attributes, name_length = struct.unpack_from("<HHIIBB", table, offset)
            if left == 0xFFFF and right == 0xFFFF:
                # (a sector's padding)
                continue
            name = table[offset + 14:offset + 14 + name_length].decode("latin-1")
            path = prefix + name
            if attributes & DIRECTORY:
                self._directory(start, length, path + "/")
            else:
                self.files[path.lower()] = (start, length)
                self.names[path.lower()] = path
            if left:
                todo.append(left * 4)
            if right:
                todo.append(right * 4)

    def extract(self, name, target, progress=None):
        """copies a file (its path in the disc, any case) to target"""
        start, length = self.files[name.lower()]
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
