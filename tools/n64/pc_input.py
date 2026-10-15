"""Read-only retail Halo PC map input; never runs a disc's installer.

Supports installed Maps directories and loose files on plain ISO9660 media.
Joliet/Rock Ridge names and packed installers are deliberately not interpreted.
All output files are copied, hashed and atomically replaced on every request.
"""

from dataclasses import dataclass
import hashlib
import os
from pathlib import Path
import re
import struct
import tempfile


DEFAULT_REQUIRED = ("dangercanyon", "deathisland", "gephyrophobia", "icefields", "infinity", "timberland")
PC_MAPS = DEFAULT_REQUIRED
_NAME = re.compile(r"[a-z0-9][a-z0-9_-]{0,30}\Z")
_ISO_NAME = re.compile(r"[A-Za-z0-9_][A-Za-z0-9_.-]{0,127}\Z")
_SECTOR = 2048
_MAX_MAP_SIZE = 2 << 30


class PCInputError(ValueError):
    """Unsupported, missing or malformed PC map source, suitable for a UI."""


def _name(value):
    value = str(value).lower()
    if value.endswith(".map"):
        value = value[:-4]
    if not _NAME.fullmatch(value):
        raise PCInputError(f"Unsafe map name {value!r}; use a plain Halo map name.")
    return value


@dataclass(frozen=True)
class _Member:
    name: str
    path: Path
    size: int
    offset: int = 0
    member: str = ""


def _header(header, size, name):
    if name in ("bitmaps", "sounds"):
        if size < 16 or len(header) < 16 or size > _MAX_MAP_SIZE:
            raise PCInputError(f"{name}.map resource cache is truncated or oversized.")
        kind, paths, index, count = struct.unpack_from("<4I", header)
        if (kind != (1 if name == "bitmaps" else 2) or
                not 16 <= paths <= index <= size or
                count < 1 or count > 1000000 or index + count * 12 != size or
                index - paths > 32 << 20):
            raise PCInputError(f"{name}.map has an invalid retail PC resource-cache header or bounds.")
        return
    if len(header) != 0x800 or not 0x800 <= size <= _MAX_MAP_SIZE:
        raise PCInputError(f"{name}.map is truncated or exceeds the supported map size.")
    if header[:4] != b"daeh" or header[0x7FC:] != b"toof":
        raise PCInputError(f"{name}.map has an invalid retail PC cache header/footer.")
    version, declared, _, tag_offset, tag_size = struct.unpack_from("<5I", header, 4)
    if version != 7:
        edition = {5: "Xbox", 609: "Custom Edition"}.get(version, "unsupported")
        raise PCInputError(f"{name}.map is {edition} cache version {version}; select retail Halo PC version 7 maps.")
    if declared != size or tag_offset < 0x800 or tag_size < 0x28 or tag_offset + tag_size > size:
        raise PCInputError(f"{name}.map has invalid declared length or tag-data bounds.")
    raw_name = header[0x20:0x40]
    if b"\0" not in raw_name:
        raise PCInputError(f"{name}.map has an unterminated map name in its header.")
    try:
        actual = raw_name.split(b"\0", 1)[0].decode("ascii").lower()
    except UnicodeDecodeError as error:
        raise PCInputError(f"{name}.map has a non-ASCII map name in its header.") from error
    if actual != name:
        raise PCInputError(f"{name}.map identifies itself as {actual!r}; select unrenamed retail PC maps.")


def _validate_member(stream, member):
    stream.seek(member.offset)
    header = stream.read(min(0x800, member.size))
    _header(header, member.size, member.name)
    if member.name not in ("bitmaps", "sounds"):
        return
    _, paths, index, count = struct.unpack_from("<4I", header)
    stream.seek(member.offset + paths)
    names = stream.read(index - paths)
    entries = stream.read(count * 12)
    if len(names) != index - paths or len(entries) != count * 12 or not names.endswith(b"\0"):
        raise PCInputError(f"{member.name}.map resource cache is truncated.")
    # Only declared name starts are interpreted; resource names never become paths.
    for name_offset, size, data_offset in struct.iter_unpack("<III", entries):
        if (name_offset >= len(names) - 1 or (name_offset and names[name_offset - 1] != 0) or
                data_offset < 16 or data_offset + size > paths):
            raise PCInputError(f"{member.name}.map has an out-of-bounds resource index.")


class _ISO9660:
    """Bounded primary-volume reader. Records never become output paths."""

    def __init__(self, path):
        self.path = path
        self.size = path.stat().st_size
        self.limit = self.size
        self.maps = {}
        self._directories = set()
        self._paths = set()
        self._entries = 0
        self._directory_bytes = 0
        self.packed = False
        with path.open("rb") as stream:
            primary = None
            for sector in range(16, 80):
                block = self._read(stream, sector * _SECTOR, _SECTOR)
                if block[1:6] != b"CD001" or block[6] != 1:
                    raise PCInputError("Not a supported plain ISO9660 PC disc image; select an installed Halo PC Maps folder.")
                if block[0] == 1:
                    if primary is not None:
                        raise PCInputError("ISO has duplicate primary volume descriptors.")
                    primary = block
                if block[0] == 255:
                    break
            else:
                raise PCInputError("ISO volume descriptor terminator is missing.")
            if primary is None:
                raise PCInputError("ISO primary volume descriptor is missing.")
            volume_size = self._dual32(primary, 80) * _SECTOR
            block_size = self._dual16(primary, 128)
            if block_size != _SECTOR or volume_size > self.size or volume_size < (sector + 1) * _SECTOR:
                raise PCInputError("ISO volume size or logical block size is invalid.")
            self.limit = volume_size
            root = self._record(primary, 156)
            if root[3] != b"\0" or not root[2] & 2:
                raise PCInputError("ISO root directory record is invalid.")
            self._directory(stream, root[0], root[1], "", 0)

    @staticmethod
    def _dual32(data, offset):
        little = struct.unpack_from("<I", data, offset)[0]
        if little != struct.unpack_from(">I", data, offset + 4)[0]:
            raise PCInputError("ISO has inconsistent endian copies of a directory value.")
        return little

    @staticmethod
    def _dual16(data, offset):
        little = struct.unpack_from("<H", data, offset)[0]
        if little != struct.unpack_from(">H", data, offset + 2)[0]:
            raise PCInputError("ISO has inconsistent endian copies of a volume value.")
        return little

    def _read(self, stream, offset, size):
        if offset < 0 or size < 0 or offset + size > self.limit:
            raise PCInputError("ISO extent is out of bounds or the image is truncated.")
        stream.seek(offset)
        data = stream.read(size)
        if len(data) != size:
            raise PCInputError("ISO ends inside a file or directory.")
        return data

    def _record(self, data, offset):
        length = data[offset]
        if length < 34 or offset + length > len(data) or (offset % _SECTOR) + length > _SECTOR:
            raise PCInputError("ISO contains a truncated or crossing directory record.")
        record = data[offset:offset + length]
        name_length = record[32]
        if not name_length or 33 + name_length > length:
            raise PCInputError("ISO directory identifier length is invalid.")
        extent = self._dual32(record, 2) * _SECTOR
        size = self._dual32(record, 10)
        if record[1] or record[26] or record[27] or record[25] & (0x80 | 0x04):
            raise PCInputError("ISO uses unsupported extended, interleaved, associated or multi-extent files; select installed maps.")
        if self._dual16(record, 28) != 1 or extent + size > self.limit:
            raise PCInputError("ISO file extent is out of bounds or spans another volume.")
        return extent, size, record[25], record[33:33 + name_length]

    def _directory(self, stream, offset, size, prefix, depth):
        if depth > 16 or size <= 0 or size > 16 << 20:
            raise PCInputError("ISO directory depth or size exceeds the supported safety limit.")
        if offset in self._directories:
            raise PCInputError("ISO directory cycle or shared-directory alias detected.")
        self._directories.add(offset)
        self._directory_bytes += size
        if self._directory_bytes > 64 << 20:
            raise PCInputError("ISO directory data exceeds the supported safety limit.")
        data = self._read(stream, offset, size)
        cursor = 0
        children = []
        while cursor < len(data):
            length = data[cursor]
            if not length:
                end = min(((cursor // _SECTOR) + 1) * _SECTOR, len(data))
                if any(data[cursor:end]):
                    raise PCInputError("ISO directory padding is malformed.")
                cursor = end
                continue
            extent, count, flags, raw_name = self._record(data, cursor)
            cursor += length
            self._entries += 1
            if self._entries > 100000:
                raise PCInputError("ISO entry count exceeds the supported safety limit.")
            if raw_name in (b"\0", b"\1"):
                if not flags & 2:
                    raise PCInputError("ISO special directory entry is not a directory.")
                continue
            try:
                name = raw_name.decode("ascii")
            except UnicodeDecodeError as error:
                raise PCInputError("ISO primary directory has an unsupported non-ASCII name.") from error
            if ";" in name:
                name, _, version = name.partition(";")
                if flags & 2 or not version.isascii() or not version.isdigit() or not 1 <= int(version) <= 32767:
                    raise PCInputError("ISO file version suffix is invalid.")
            if not _ISO_NAME.fullmatch(name) or name in (".", "..") or name.endswith(".") and flags & 2:
                raise PCInputError(f"ISO contains an unsafe directory name: {name!r}.")
            member = prefix + name
            key = member.lower()
            if key in self._paths:
                raise PCInputError(f"ISO contains a case-insensitive path collision: {member}.")
            self._paths.add(key)
            if flags & 2:
                children.append((extent, count, member + "/", depth + 1))
            elif name.lower().endswith(".map"):
                map_name = _name(name)
                if map_name in self.maps:
                    raise PCInputError(f"ISO contains duplicate map name {map_name}.map.")
                minimum = 16 if map_name in ("bitmaps", "sounds") else 0x800
                if not minimum <= count <= _MAX_MAP_SIZE:
                    raise PCInputError(f"ISO map {member} has an invalid size.")
                self.maps[map_name] = _Member(map_name, self.path, count, extent, member)
            elif name.lower().endswith((".cab", ".msi", ".exe")):
                self.packed = True
        for child in children:
            self._directory(stream, *child)


class PCMapSource:
    """Installed retail PC maps or loose maps in a read-only ISO9660 image.

    ``validate`` returns the selected normalized map names; ``provide`` returns
    the destination Path. ``progress`` receives (map_name, bytes_done, size).
    ``last_manifest`` describes only the most recent successful provide call.
    """

    def __init__(self, path):
        self.path = Path(path).expanduser().resolve()
        self.last_manifest = None
        self._members = self._scan()

    def _scan(self):
        if self.path.is_file() and self.path.suffix.lower() == ".iso":
            iso = _ISO9660(self.path)
            if not iso.maps:
                detail = " Packed CAB/MSI/installer extraction is unsupported." if iso.packed else " No loose .map files were found."
                raise PCInputError("This PC ISO has no supported loose retail maps." + detail + " Select an installed Halo PC Maps folder instead; installers are never executed.")
            return iso.maps
        if not self.path.is_dir():
            raise PCInputError("Select an installed Halo PC Maps folder, game folder, or .iso image.")
        candidates = [self.path]
        candidates.extend(p for p in self.path.iterdir() if p.name.lower() == "maps" and p.is_dir())
        members = {}
        for folder in candidates:
            if folder.is_symlink():
                raise PCInputError("Select a real installed Maps directory rather than a symbolic link.")
            for path in folder.iterdir():
                if path.suffix.lower() != ".map" or not path.is_file():
                    continue
                if path.is_symlink():
                    raise PCInputError(f"Map symbolic links are unsupported: {path.name}.")
                name = _name(path.name)
                if name in members:
                    raise PCInputError(f"Duplicate or case-colliding map {name}.map; select one Maps folder.")
                members[name] = _Member(name, path, path.stat().st_size, member=str(path))
        if not members:
            raise PCInputError("No .map files found; select the installed retail Halo PC Maps folder.")
        return members

    def names(self):
        self._members = self._scan()
        return sorted(self._members)

    def list(self):
        return self.names()

    def missing(self, names):
        available = set(self.names())
        return [name for name in map(_name, names) if name not in available]

    def validate(self, required=None):
        self._members = self._scan()
        names = list(dict.fromkeys(map(_name, DEFAULT_REQUIRED if required is None else required)))
        missing = [name + ".map" for name in names if name not in self._members]
        if missing:
            raise PCInputError("Missing required retail Halo PC maps: " + ", ".join(missing) + ". Select the installed Maps folder or an ISO with loose maps.")
        for name in names:
            member = self._members[name]
            with member.path.open("rb") as stream:
                _validate_member(stream, member)
        return names

    def provide(self, names, folder, progress=None):
        self.last_manifest = None
        names = self.validate(names)
        folder = Path(folder)
        # Never overwrite source maps, including through destination symlinks.
        for name in names:
            target = folder / (name + ".map")
            if target.resolve() == self._members[name].path.resolve():
                raise PCInputError("The output folder must differ from the read-only source Maps folder.")
        folder.mkdir(parents=True, exist_ok=True)
        manifest = {"source": str(self.path), "source_kind": "iso9660" if self.path.is_file() else "folder", "maps": []}
        for name in names:
            member = self._members[name]
            before = member.path.stat()
            temporary = None
            try:
                with member.path.open("rb") as source, tempfile.NamedTemporaryFile(dir=folder, prefix=".pc-map-", suffix=".tmp", delete=False) as output:
                    temporary = Path(output.name)
                    source.seek(member.offset)
                    digest = hashlib.sha256()
                    done = 0
                    while done < member.size:
                        block = source.read(min(4 << 20, member.size - done))
                        if not block:
                            raise PCInputError(f"Source ends inside {name}.map; select a complete retail map source.")
                        output.write(block)
                        digest.update(block)
                        done += len(block)
                        if progress:
                            progress(name, done, member.size)
                after = member.path.stat()
                if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns):
                    raise PCInputError(f"Source changed while copying {name}.map; retry with an unchanged source.")
                # Check written bytes too; size alone must never validate a cache.
                written = hashlib.sha256()
                with temporary.open("rb") as copied:
                    _validate_member(copied, _Member(name, temporary, member.size))
                    copied.seek(0)
                    for block in iter(lambda: copied.read(4 << 20), b""):
                        written.update(block)
                if written.digest() != digest.digest():
                    raise PCInputError(f"Output verification failed for {name}.map.")
                os.replace(temporary, folder / (name + ".map"))
                temporary = None
                entry = {"name": name, "size": member.size, "sha256": digest.hexdigest(), "member": member.member}
                if self.path.is_file():
                    entry["iso_offset"] = member.offset
                manifest["maps"].append(entry)
            finally:
                if temporary is not None:
                    temporary.unlink(missing_ok=True)
        self.last_manifest = manifest
        return folder

    def close(self):
        """Compatibility with other source adapters; no handles are retained."""
