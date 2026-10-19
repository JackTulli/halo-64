"""Compressed animations made plain before a map is converted.

The stock Xbox maps' animations are uncompressed (convert.py streams their
frames as they are); a Custom Edition map's may be keyframe-compressed
(flags bit 0, "compressed data": Halo's own codec, decoded by
source/models/model_animations.c animation_get_keyframe_rotation and the
rest). decompress_animations() samples each compressed animation at every
frame exactly as the game would and writes it back in the plain layout:

    frame data      per frame, per node in order: its rotation (4 shorts,
                    i j k w / 32767) if the node's rotation flag is set, its
                    translation (3 floats) if its translation flag is set,
                    its scale (float) if its scale flag is set
    default data    per node, the channels whose flags are clear, in the same
                    order (compressed: the header's default rotation and
                    translation of the node, scale 1)

The node flags stay as they were; the animation's flags lose bit 0, its
frame size and offset to compressed data are set, and its frame data and
default data point at a region of their own added to the map (an XboxMap,
changed in place, like merge.py's imports).
"""

import math
import struct

from .tagdefs import TagDefinitions

DECOMPRESSED_BASE = 0x70000000
KEYFRAME_COUNT_BITS = 12
QMAX = 32767.0


def _short(value):
    value &= 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def quaternion_6byte(data, offset):
    """quaternion_decompress_6byte"""
    w0, w1, w2 = struct.unpack_from("<3H", data, offset)
    i = _short((w0 >> 12) | (w0 & 0xFFF0))
    j = _short(((w1 >> 4) & 0x0FF0) | (w0 & 0x000F) | (w0 << 12))
    k = _short(((((w2 >> 4) & 0x0F00) | (w1 & 0x00F0)) >> 4) | (w1 << 8))
    w = _short(((w2 >> 8) & 0x000F) | (w2 << 4))
    return (i / QMAX, j / QMAX, k / QMAX, w / QMAX)


def normalize(q):
    m = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]
    if m > 0.0:
        s = 1.0 / math.sqrt(m)
        return tuple(c * s for c in q)
    return (0.0, 0.0, 0.0, 1.0)


def interpolate(q0, q1, t):
    """quaternions_interpolate_and_normalize"""
    v = 1.0 - t
    if sum(a * b for a, b in zip(q0, q1)) < 0.0:
        t = -t
    return normalize(tuple(a * v + b * t for a, b in zip(q0, q1)))


def compress_8byte(q):
    """quaternion_compress_8byte (C's truncating float to short)"""
    return struct.pack("<4h", *(max(-32768, min(32767, int(c * QMAX))) for c in q))


def _keyframes(data, header_word, indices_offset, values_offset, value_size):
    first = header_word >> KEYFRAME_COUNT_BITS
    count = header_word & ((1 << KEYFRAME_COUNT_BITS) - 1)
    frames = [struct.unpack_from("<H", data, indices_offset + 2 * (first + n))[0] for n in range(count)]
    values = [values_offset + value_size * (first + n) for n in range(count)]
    return frames, values


def _sample(frame, frames, values, default, decode, lerp, final):
    """the game's keyframe lookup at an integer frame: before the first
    keyframe from the default (at frame 0); at the last keyframe its value;
    else between the two keyframes around it (decode: a keyframe's raw
    value; final: what the game does to a value taken as it is -- a
    rotation is normalized; lerp normalizes its own result)"""
    if not frames:
        return final(default)
    if frame < frames[0]:
        this_frame, this, next_frame, nxt = 0, default, frames[0], decode(values[0])
    elif frame == frames[-1]:
        return final(decode(values[-1]))
    else:
        k = 0
        while not (frames[k] <= frame < frames[k + 1]):
            k += 1
        this_frame, this, next_frame, nxt = frames[k], decode(values[k]), frames[k + 1], decode(values[k + 1])
    if frame == this_frame:
        return final(this)
    return lerp(this, nxt, (frame - this_frame) / (next_frame - this_frame))


def decompress_one(raw, blob):
    """(frame size, frame data, default data) of one compressed animation:
    raw is its 180-byte block element, blob its frame data (the compressed
    data at offset 0)"""
    frame_count, = struct.unpack_from("<H", raw, 0x22)
    node_count, = struct.unpack_from("<H", raw, 0x2C)
    offset, = struct.unpack_from("<I", raw, 0x88)
    data = blob[offset:]
    trans_flags = struct.unpack_from("<2I", raw, 0x5C)
    rot_flags = struct.unpack_from("<2I", raw, 0x6C)
    scale_flags = struct.unpack_from("<2I", raw, 0x7C)

    def flag(bits, n):
        return (bits[n >> 5] >> (n & 31)) & 1

    (rot_indices, default_rots, rot_keys, trans_headers, trans_indices, default_trans, trans_keys,
     scale_headers, scale_indices, default_scales, scale_keys) = struct.unpack_from("<11i", data, 0)
    point = lambda o: struct.unpack_from("<3f", data, o)  # noqa: E731
    lerp3 = lambda a, b, t: tuple(x * (1.0 - t) + y * t for x, y in zip(a, b))  # noqa: E731
    channels = []  # per node: (rotation, translation, scale) samplers or None
    default = bytearray()
    ri = ti = si = 0
    for n in range(node_count):
        node = []
        rot_default = quaternion_6byte(data, default_rots + 6 * n)
        if flag(rot_flags, n):
            header, = struct.unpack_from("<I", data, 0x2C + 4 * ri)
            ri += 1
            frames, values = _keyframes(data, header, rot_indices, rot_keys, 6)
            node.append(("r", frames, values, rot_default))
        else:
            default += compress_8byte(normalize(rot_default))
        trans_default = point(default_trans + 12 * n)
        if flag(trans_flags, n):
            header, = struct.unpack_from("<I", data, trans_headers + 4 * ti)
            ti += 1
            frames, values = _keyframes(data, header, trans_indices, trans_keys, 12)
            node.append(("t", frames, values, trans_default))
        else:
            default += struct.pack("<3f", *trans_default)
        if flag(scale_flags, n):
            header, = struct.unpack_from("<I", data, scale_headers + 4 * si)
            # (the game reads a scale's default by its place among the scaled
            # nodes, not by node)
            scale_default, = struct.unpack_from("<f", data, default_scales + 4 * si)
            si += 1
            frames, values = _keyframes(data, header, scale_indices, scale_keys, 4)
            node.append(("s", frames, values, scale_default))
        else:
            default += struct.pack("<f", 1.0)
        channels.append(node)
    same = lambda v: v  # noqa: E731
    decoders = {
        "r": (lambda o: quaternion_6byte(data, o), interpolate, normalize, compress_8byte),
        "t": (point, lerp3, same, lambda p: struct.pack("<3f", *p)),
        "s": (lambda o: struct.unpack_from("<f", data, o)[0], lambda a, b, t: a * (1.0 - t) + b * t, same,
              lambda s: struct.pack("<f", s)),
    }
    frames_out = bytearray()
    frame_size = 0
    for f in range(frame_count):
        start = len(frames_out)
        for node in channels:
            for kind, frames, values, dflt in node:
                decode, lerp, final, encode = decoders[kind]
                frames_out += encode(_sample(f, frames, values, dflt, decode, lerp, final))
        frame_size = len(frames_out) - start
    return frame_size, bytes(frames_out), bytes(default)


def decompress_animations(xmap, defs=None):
    """makes every compressed animation of the map plain; returns how many"""
    defs = defs or TagDefinitions()
    go = {f.name: o for o, f in defs.fields("ModelAnimations")}
    asize = defs.size("ModelAnimationsAnimation")
    base = DECOMPRESSED_BASE
    while any(b <= base < b + max(len(d), 0x04000000) for b, d in xmap.regions):
        base += 0x04000000
    added = bytearray()
    patches = []  # (address, bytes)
    count = 0
    for t in xmap.tags:
        if t.group_name != "antr":
            continue
        n, pointer = struct.unpack("<iI", xmap.read(t.address + go["animations"], 8))
        for i in range(max(n, 0)):
            address = pointer + i * asize
            raw = xmap.read(address, asize)
            flags, = struct.unpack_from("<H", raw, 0x3A)
            if not flags & 1:
                continue
            size, _, _, data_pointer, _ = struct.unpack_from("<iIIII", raw, 0xA0)
            blob = xmap.read(data_pointer, size)
            frame_size, frames, default = decompress_one(raw, blob)
            if frame_size > 0xFFFF:
                raise ValueError(f"{t.name}: animation {i}'s frame is {frame_size} bytes")
            new = bytearray(raw)
            struct.pack_into("<H", new, 0x3A, flags & ~1)
            struct.pack_into("<H", new, 0x24, frame_size)
            struct.pack_into("<I", new, 0x88, 0)
            for at, blob_out in ((0x8C, default), (0xA0, frames)):
                added += b"\0" * ((-len(added)) % 16)
                where = base + len(added) if blob_out else 0
                added += blob_out
                struct.pack_into("<iIIII", new, at, len(blob_out), 0, 0, where, 0)
            patches.append((address, bytes(new)))
            count += 1
    if not count:
        return 0
    for i, (start, region) in enumerate(xmap.regions):
        mine = [(a, b) for a, b in patches if start <= a < start + len(region)]
        if not mine:
            continue
        rewritten = bytearray(region)
        for a, b in mine:
            rewritten[a - start:a - start + len(b)] = b
        xmap.regions[i] = (start, bytes(rewritten))
        if start == xmap.regions[0][0] and i == 0:
            xmap.tag_data = xmap.regions[0][1]
    xmap.regions.append((base, bytes(added)))
    print(f"{xmap.name}: {count} compressed animations made plain, {len(added) / 1e6:.2f} MB of frames")
    return count
