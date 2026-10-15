"""Particles for the N64 (port/n64/src/n64_particles.c).

The N64 does not keep Halo's effect particle lists (Effect.events[].
particles[]: 232 bytes an entry, ~1,900 of them a map) nor its particle
tags; the effect code asks for an effect's particles in a form of the
N64's own, kept on the cartridge and read when an effect plays:

    fx/<map>.fx
        header (32 bytes): 'N64F', u32 version, u16 entry count, cell
            count, u32 index offset, atlas offset
        index: u16 effect tag index, u16 record size, u32 record offset
            (sorted by tag index)
        atlas: 128x64 I4, 32 cells of 16x16 (the particles' bitmaps'
            first sprites, cell 0 a soft round blob), then each cell's
            colour (RGBA, the bitmap's own hue at full brightness)
        records: u16 event count, u16 emitter count, u16 first emitter of
            each event and one past the last, then 80-byte emitters:
            u8 location, flags (camera mode 0-1, attached 2, additive 3),
               cell, distribution function, count min, max, violence mode,
               s8 gravity (wu/s^2 x8)
            u16 lifespan min, max, fade in, fade out (ms)
            f32 velocity min, max, cone angle, radius min, max,
                distribution radius min, max, offset x y z, direction x y z
            u8 tint lower rgba, upper rgba, drag (/16 per s), end scale
               (/32), pad x2

    python3 -m tools.n64.effects <map> --filesystem <file tree>
"""

import math
import struct
from pathlib import Path

from PIL import Image

from .bitmaps import bitmap_images, decode

MAGIC = b"N64F"
VERSION = 2
# the explosions the N64 draws its own way (n64_particles.c n64_explosion:
# a flash, a fireball, smoke): the effect's tag name, its kind (n64.h
# N64_EXPLOSION_*); listed after the records, the header's bytes 20-25 the
# list's offset (u32) and count (u16), each entry u16 effect tag index, u8
# kind, u8 pad
EXPLOSIONS = {
    "weapons\\frag grenade\\effects\\explosion": 0,
    "weapons\\plasma grenade\\effects\\explosion": 1,
    "weapons\\rocket launcher\\effects\\rocket explosion": 2,
    "vehicles\\scorpion\\shell explosion": 3,
    "weapons\\needler\\effects\\explosion": 4,
    "weapons\\plasma_cannon\\effects\\explosion": 5,
}
CELL = 16
COLUMNS, ROWS = 8, 4
CELLS = COLUMNS * ROWS
EMITTER = struct.Struct(">BBBBBBBbHHHH13f8BBBBB")
assert EMITTER.size == 80
# at most this many emitters an event (the game counts them in a byte array
# of 32), and this many bytes a record
MAXIMUM_EMITTERS = 16
MAXIMUM_RECORD = 2048
GRAVITY = 3.2  # world units/s^2 (~9.8 m/s^2)


def _fields(defs, name):
    return {f.name: o for o, f in defs.fields(name)}


def _blob():
    img = Image.new("L", (CELL, CELL))
    for y in range(CELL):
        for x in range(CELL):
            d = math.hypot(x + 0.5 - CELL / 2, y + 0.5 - CELL / 2) / (CELL / 2)
            img.putpixel((x, y), int(max(0.0, 1.0 - d) ** 1.5 * 255))
    return img


class EffectParticles:
    def __init__(self, xmap, defs):
        self.map = xmap
        self.defs = defs
        self.ef = _fields(defs, "Effect")
        self.ev = _fields(defs, "EffectEvent")
        self.ep = _fields(defs, "EffectParticle")
        self.pf = _fields(defs, "Particle")
        self.bf = _fields(defs, "Bitmap")
        self.sf = _fields(defs, "BitmapGroupSequence")
        self.spf = _fields(defs, "BitmapGroupSprite")
        self.phf = _fields(defs, "PointPhysics")
        self.sprites = {}   # bitmap tag index -> 16x16 L image (or None)
        self.colours = {}   # bitmap tag index -> (r, g, b)

    def _dep(self, address):
        return self.map.tag(struct.unpack("<I", self.map.read(address + 12, 4))[0])

    def _block(self, address):
        return struct.unpack("<iI", self.map.read(address, 8))

    def sprite(self, part):
        """the particle's bitmap's first sprite, 16x16 greyscale (its
        brightness times its coverage)"""
        bitmap = part if part.group_name == "bitm" else self._dep(part.address + self.pf["bitmap"])
        if bitmap is None or bitmap.group_name != "bitm":
            return None, None
        if bitmap.index in self.sprites:
            return bitmap.index, self.sprites[bitmap.index]
        image = None
        try:
            images = bitmap_images(self.map, bitmap, self.defs)
            first = 0 if part.group_name == "bitm" else struct.unpack("<h", self.map.read(part.address + self.pf["first sequence index"], 2))[0]
            count, pointer = self._block(bitmap.address + self.bf["bitmap group sequence"])
            index, box = 0, (0.0, 0.0, 1.0, 1.0)
            sequence = first if 0 <= first < count else 0
            if count > 0:
                sc, sp = self._block(pointer + sequence * self.defs.size("BitmapGroupSequence") + self.sf["sprites"])
                if sc > 0:
                    raw = self.map.read(sp, self.defs.size("BitmapGroupSprite"))
                    index = struct.unpack_from("<h", raw, self.spf["bitmap index"])[0]
                    left, right, top, bottom = (struct.unpack_from("<f", raw, self.spf[k])[0]
                                                for k in ("left", "right", "top", "bottom"))
                    box = (left, top, right, bottom)
                else:
                    index = struct.unpack("<h", self.map.read(
                        pointer + sequence * self.defs.size("BitmapGroupSequence") + self.sf["first bitmap index"], 2))[0]
            if 0 <= index < len(images):
                full = decode(self.map, images[index], 128)
                w, h = full.size
                x0, y0 = int(box[0] * w), int(box[1] * h)
                x1, y1 = max(x0 + 1, int(math.ceil(box[2] * w))), max(y0 + 1, int(math.ceil(box[3] * h)))
                crop = full.crop((x0, y0, x1, y1)).resize((CELL, CELL), Image.LANCZOS)
                r, g, b, a = crop.split()
                luma = Image.merge("RGB", (r, g, b)).convert("L")
                # brightness times coverage
                pixels = [lv * av // 255 for lv, av in zip(luma.getdata(), a.getdata())]
                peak = max(pixels) or 1
                image = Image.new("L", (CELL, CELL))
                image.putdata([min(255, p * 255 // peak) for p in pixels])
                # its colour (the tint multiplies it: a muzzle flash's orange
                # is its bitmap's), weighted by coverage, brightest channel 255
                weights = [av for av in a.getdata()]
                total = sum(weights) or 1
                colour = [sum(c * w for c, w in zip(band.getdata(), weights)) / total for band in (r, g, b)]
                top = max(colour)
                self.colours[bitmap.index] = tuple(int(round(c * 255 / top)) for c in colour) \
                    if top >= 8 else (255, 255, 255)
        except (ValueError, KeyError, IndexError, OSError):
            image = None
        self.sprites[bitmap.index] = image
        return bitmap.index, image

    def physics(self, part):
        """gravity (wu/s^2, + falls) and drag (1/s) from the particle's point
        physics: lighter than air rises"""
        physics = self._dep(part.address + self.pf["physics"])
        if physics is None or physics.group_name != "pphy":
            return 0.0, 0.0
        raw = self.map.read(physics.address, self.defs.size("PointPhysics"))
        density = struct.unpack_from("<f", raw, self.phf["density"])[0]
        scale = struct.unpack_from("<f", raw, self.phf["air gravity scale"])[0] or 1.0
        friction = struct.unpack_from("<f", raw, self.phf["air friction"])[0]
        if density <= 0:
            gravity = 0.0
        else:
            gravity = GRAVITY * scale * max(-0.3, 1.0 - 0.0011 / density)
        return gravity, max(0.0, min(15.0, friction * 30.0))

    def build(self):
        """the records (by effect tag index), the cells' images, and which
        bitmap each record's emitters use"""
        emitters_by_effect = {}
        usage = {}
        for tag in self.map.tags:
            if tag.group_name != "effe":
                continue
            ec, eptr = self._block(tag.address + self.ef["events"])
            events = []
            for e in range(max(ec, 0)):
                base = eptr + e * self.defs.size("EffectEvent")
                pc, pp = self._block(base + self.ev["particles"])
                entries = []
                for k in range(max(pc, 0))[:MAXIMUM_EMITTERS]:
                    raw = self.map.read(pp + k * self.defs.size("EffectParticle"), self.defs.size("EffectParticle"))
                    part = self.map.tag(struct.unpack_from("<I", raw, self.ep["particle type"] + 12)[0])
                    if part is None or part.group_name != "part":
                        continue
                    bitmap_index, image = self.sprite(part)
                    if bitmap_index is not None and image is not None:
                        usage[bitmap_index] = usage.get(bitmap_index, 0) + 1
                    entries.append((raw, part, bitmap_index))
                events.append(entries)
            if any(events):
                emitters_by_effect[tag.index] = events
        # the cells: the most used bitmaps, the rest the most like one of them
        # The PC flame travels as a contrail, rather than an effect emitter.
        # Reserve its original cloud-fire sprite explicitly for the native trail.
        for tag in self.map.tags:
            if tag.group_name == "bitm" and tag.name == "weapons\\flamethrower\\bitmaps\\cloud fire":
                _, image = self.sprite(tag)
                if image is not None:
                    usage[tag.index] = usage.get(tag.index, 0) + 1
        # Keep actual flame shapes instead of replacing the Flamethrower
        # stream with the nearest generic spark when the 32-cell atlas fills.
        ranked = [b for b, _ in sorted(usage.items(), key=lambda kv:
                  (not self.map.tags[kv[0]].name.startswith("weapons\\flamethrower\\"), -kv[1]))]
        cells = [_blob()] + [self.sprites[b] for b in ranked[:CELLS - 1]]
        cell_of = {b: i + 1 for i, b in enumerate(ranked[:CELLS - 1])}
        self.flame_cell = next((cell_of[b] for b in ranked[:CELLS - 1]
                               if self.map.tags[b].name.startswith("weapons\\flamethrower\\")), 0)
        for b in ranked[CELLS - 1:]:
            img = list(self.sprites[b].getdata())
            best, best_cost = 0, None
            for i, cell in enumerate(cells):
                cost = sum((p - q) * (p - q) for p, q in zip(img, cell.getdata()))
                if best_cost is None or cost < best_cost:
                    best, best_cost = i, cost
            cell_of[b] = best
        records = {}
        for index, events in emitters_by_effect.items():
            records[index] = self.record(events, cell_of)
        colours = [(255, 255, 255)] + [self.colours.get(b, (255, 255, 255)) for b in ranked[:CELLS - 1]]
        return records, cells, colours

    def record(self, events, cell_of):
        ep, pf = self.ep, self.pf
        out = bytearray()
        firsts = [0]
        packed = []
        for entries in events:
            for raw, part, bitmap_index in entries:
                praw = self.map.read(part.address, self.defs.size("Particle"))

                def f(offset, n=1):
                    return struct.unpack_from(f"<{n}f", raw, offset)

                create = struct.unpack_from("<H", raw, ep["create"])[0] & 3
                violence = struct.unpack_from("<H", raw, ep["violence mode"])[0]
                location = struct.unpack_from("<h", raw, ep["location"])[0]
                pflags = struct.unpack_from("<I", raw, ep["flags"])[0]
                distribution = struct.unpack_from("<H", raw, ep["distribution function"])[0]
                count_min, count_max = struct.unpack_from("<2h", raw, ep["count"])
                blend = struct.unpack_from("<H", praw, pf["framebuffer blend function"])[0]
                life = struct.unpack_from("<2f", praw, pf["lifespan"])
                fade_in, fade_out = struct.unpack_from("<2f", praw, pf["fade in time"])
                animation = struct.unpack_from("<2f", praw, pf["radius animation"])
                gravity, drag = self.physics(part)
                # tint: Halo's ARGB floats
                lower = struct.unpack_from("<4f", raw, ep["tint lower bound"])
                upper = struct.unpack_from("<4f", raw, ep["tint upper bound"])

                def rgba(c):
                    a, r, g, b = (max(0, min(255, int(round(v * 255)))) for v in c)
                    return r, g, b, a

                flags = create | ((pflags & 1) << 2) | ((1 if blend in (3, 7) else 0) << 3)
                end_scale = animation[1] if animation[1] > 0 else 1.0

                def ms(v):
                    return max(0, min(65535, int(round(v * 1000))))

                packed.append(EMITTER.pack(
                    max(0, min(255, location)) if location >= 0 else 255, flags,
                    cell_of.get(bitmap_index, 0), distribution & 255,
                    max(0, min(255, count_min)), max(0, min(255, max(count_min, count_max))),
                    violence & 255, max(-127, min(127, int(round(gravity * 8)))),
                    ms(life[0]), ms(max(life)), ms(fade_in), ms(fade_out),
                    *f(ep["velocity"], 2), *f(ep["velocity cone angle"]), *f(ep["radius"], 2),
                    *f(ep["distribution radius"], 2), *f(ep["relative offset"], 3),
                    *f(ep["relative direction vector"], 3),
                    *rgba(lower), *rgba(upper),
                    max(0, min(255, int(round(drag * 16)))), max(0, min(255, int(round(end_scale * 32)))), 0, 0))
            firsts.append(len(packed))
        # (an effect's emitters past MAXIMUM_RECORD are left out, from its
        # last events)
        while 4 + 2 * len(firsts) + 2 + EMITTER.size * len(packed) > MAXIMUM_RECORD and packed:
            packed.pop()
            firsts = [min(x, len(packed)) for x in firsts]
        out += struct.pack(">HH", len(firsts) - 1, len(packed))
        out += struct.pack(f">{len(firsts)}H", *firsts)
        out += b"\0" * ((-len(out)) % 4)
        out += b"".join(packed)
        return bytes(out)

    def write(self, path):
        records, cells, colours = self.build()
        atlas = Image.new("L", (COLUMNS * CELL, ROWS * CELL))
        for i, cell in enumerate(cells):
            atlas.paste(cell, ((i % COLUMNS) * CELL, (i // COLUMNS) * CELL))
        pixels = list(atlas.getdata())
        i4 = bytes(((pixels[i] * 15 + 127) // 255) << 4 | ((pixels[i + 1] * 15 + 127) // 255)
                   for i in range(0, len(pixels), 2))
        indices = sorted(records)
        index_offset = 32
        atlas_offset = index_offset + 8 * len(indices)
        atlas_offset += (-atlas_offset) % 16
        # each cell's colour (RGBA) after the atlas
        colour_table = b"".join(bytes((r, g, b, 255)) for r, g, b in colours)
        colour_table += b"\0" * (4 * CELLS - len(colour_table))
        offset = atlas_offset + len(i4) + len(colour_table)
        table = bytearray()
        body = bytearray()
        for index in indices:
            record = records[index]
            table += struct.pack(">HHI", index, len(record), offset + len(body))
            body += record
            body += b"\0" * ((-len(body)) % 16)
        out = bytearray(MAGIC + struct.pack(">IHHII", VERSION, len(indices), len(cells), index_offset, atlas_offset))
        out += b"\0" * (index_offset - len(out))
        out += table
        out += b"\0" * (atlas_offset - len(out))
        out += i4
        out += colour_table
        out += body
        explosions = sorted((t.index, EXPLOSIONS[t.name]) for t in self.map.tags
                            if t.group_name == "effe" and t.name in EXPLOSIONS)
        out += b"\0" * ((-len(out)) % 4)
        struct.pack_into(">IH", out, 20, len(out), len(explosions))
        out[26] = self.flame_cell
        for index, kind in explosions:
            out += struct.pack(">HBB", index, kind, 0)
        Path(path).parent.mkdir(parents=True, exist_ok=True)
        Path(path).write_bytes(bytes(out))
        return {"effects": len(indices), "emitters": sum(struct.unpack_from(">H", r, 2)[0] for r in records.values()),
                "cells": len(cells), "explosions": len(explosions), "bytes": len(out)}
