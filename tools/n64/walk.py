"""Walking a tag's structures with the Invader layouts (tagdefs.py).

walk() visits every struct instance reachable from a tag's root: the tag's
own struct, the elements of its blocks (TagReflexive), recursively. The
visitor receives (address, struct name, path) for each instance; data
blobs (TagDataOffset) and dependencies (TagDependency) are reported to
on_data/on_dependency.
"""

import struct

from .tagdefs import TagDefinitions


class Walker:
    def __init__(self, xmap, defs=None):
        self.map = xmap
        self.defs = defs or TagDefinitions()

    def walk(self, tag, on_struct=None, on_data=None, on_dependency=None):
        root = self.defs.fourcc_struct(tag.group)
        if root is None:
            raise KeyError(f"no layout for {tag}")
        self._struct(tag, tag.address, root, root, on_struct, on_data, on_dependency)

    def _struct(self, tag, address, name, path, on_struct, on_data, on_dependency):
        if on_struct:
            on_struct(tag, address, name, path)
        size = self.defs.size(name)
        blob = self.map.read(address, size)
        for offset, f in self.defs.fields(name):
            for i in range(f.count):
                o = offset + i * f.size
                a = address + o
                if f.kind == "struct":
                    self._struct(tag, a, f.struct, f"{path}.{f.name}", on_struct, on_data, on_dependency)
                elif f.kind == "TagReflexive":
                    count, pointer, _ = struct.unpack_from("<iII", blob, o)
                    if count <= 0:
                        continue
                    esize = self.defs.size(f.struct)
                    for e in range(count):
                        self._struct(tag, pointer + e * esize, f.struct, f"{path}.{f.name}[]",
                                     on_struct, on_data, on_dependency)
                elif f.kind == "TagDataOffset":
                    if on_data:
                        size_, flags, file_offset, pointer, _ = struct.unpack_from("<iIIII", blob, o)
                        on_data(tag, a, f, f"{path}.{f.name}", size_, flags, pointer)
                elif f.kind == "TagDependency":
                    if on_dependency:
                        group, name_ptr, nlen, datum = struct.unpack_from("<IIiI", blob, o)
                        on_dependency(tag, a, f, f"{path}.{f.name}", group, datum)


if __name__ == "__main__":
    import collections
    import sys
    from .xboxmap import XboxMap
    m = XboxMap(sys.argv[1])
    m.load_bsp(0)
    w = Walker(m)
    per_group = collections.Counter()
    per_group_data = collections.Counter()
    per_path = collections.Counter()
    covered = bytearray(0)
    errors = 0
    seen = set()

    def on_struct(tag, address, name, path):
        size = w.defs.size(name)
        per_group[tag.group_name] += size
        per_path[(tag.group_name, path)] += size

    def on_data(tag, address, f, path, size, flags, pointer):
        if size > 0:
            per_group_data[tag.group_name] += size
            per_path[(tag.group_name, path)] += size

    for t in m.tags + [m.bsps[0][0]]:
        if t.group == 0x73627370 and t is not m.bsps[0][0]:
            continue
        try:
            w.walk(t, on_struct, on_data)
        except Exception as e:  # noqa: BLE001
            errors += 1
            if errors < 15:
                print("ERROR", t, e)
    print("errors", errors)
    tot_s = sum(per_group.values()); tot_d = sum(per_group_data.values())
    print(f"structs {tot_s/1e6:.2f} MB, data {tot_d/1e6:.2f} MB")
    for g, s in sorted(per_group.items(), key=lambda x: -(x[1] + per_group_data[x[0]]))[:30]:
        print(f"  {g} structs {s/1024:7.0f} KB data {per_group_data[g]/1024:7.0f} KB")
    print("biggest paths:")
    for (g, p), s in per_path.most_common(30):
        print(f"  {s/1024:7.0f} KB {g} {p}")
