"""Gameplay sounds for the N64 (port/n64/game/n64_sound_game.c).

Halo's own sound code is left out of the N64 build; the game's calls into
it (object_impulse_sound_new and the rest) are the N64's. The sounds it
plays are the map's sound tags ('snd!') and looping sounds ('lsnd'), kept
here in a compact form of the N64's own that the tag instance's address
points at (the Policy's custom_tag, convert.py):

    sound (40 bytes, then its pitch ranges and their permutations)
        u8 class, flags (1: its files loop), pitch range count,
           skip fraction (/255)
        f32 minimum distance, maximum distance (the class's where the tag
            has none), random pitch bounds (2), gain modifier
        u16 zero/one skip fraction, zero/one gain, zero/one pitch
            modifiers (/4096)
        u32 longest permutation length (ms)
    pitch range (20 bytes)
        f32 bend bounds (2), playback rate
        u32 played permutations (the game's), u16 first permutation,
        u8 permutation count, s8 previous permutation (the game's)
    permutation (4 bytes)
        u16 file (the map's sound list), u8 gain (/255), u8 skip fraction

    looping sound (8 bytes, then 28 a track)
        u8 track count, flags, u16 pad, f32 maximum distance
        track: u32 start, loop, end sounds (tag indices or NONE),
            f32 gain, fade in, fade out (s), u32 flags

Left out: the environment (ambience, devices, music) and anything only a
left-out sound needs. Each permutation the game can choose (Halo's
"actual" ones: the others continue a long sound and are joined to it) is
decoded from Xbox ADPCM and compressed to mono 22 kHz VADPCM by
audioconv64, shared by content: build/n64/sound-cache/<hash>.wav64; the
map's list of them is build/n64/sound/<map>.slist, which soundbank.py
packs with every other map's into the cartridge's one bank file.
"""

import hashlib
import os
import struct
import subprocess
import tempfile
from pathlib import Path

from .music import AUDIOCONV, pcm_wav

NONE = 0xFFFFFFFF
RATE = 22050
SAMPLE_RATES = {0: 22050, 1: 44100}
# the cache's version: a change to the conversion makes new files
CACHE_VERSION = b"n64 sound 2"

# Halo's sound classes the N64 plays (sound_classes.c): weapons, impacts,
# explosions, footsteps, vehicles, dialogue, the announcer and the HUD's
# (shield, health) -- not the environment (devices 26-30, music 32,
# ambience 33-35)
KEPT_CLASSES = {0, 1, 4, 5, 6, 7, 8, 9, 10, 13, 14, 15, 18, 19, 22, 23, 39, 44, 45, 46, 47, 50}
UNIT_DIALOG = 19
OBJECT_IMPACTS = 13
# at most this many of a dialogue sound's lines (the Covenant's have up to
# 24 a vocalization)
DIALOG_PERMUTATIONS = 3

# the classes' own distances (sound_classes.c), where a tag has none
CLASS_DISTANCES = {
    0: (1.4, 8.0), 1: (8.0, 120.0), 4: (4.0, 70.0), 5: (1.0, 9.0), 6: (1.0, 9.0), 7: (1.0, 9.0),
    8: (1.0, 9.0), 9: (1.0, 9.0), 10: (1.0, 9.0), 13: (0.5, 3.0), 14: (0.5, 3.0), 15: (0.5, 3.0),
    18: (0.9, 10.0), 19: (3.0, 20.0), 22: (1.4, 8.0), 23: (1.4, 8.0), 26: (0.9, 5.0), 27: (0.9, 5.0),
    28: (0.9, 5.0), 29: (0.9, 5.0), 30: (0.5, 3.0), 32: (0.9, 5.0), 33: (0.9, 5.0), 34: (0.9, 5.0),
    35: (0.5, 3.0), 39: (0.5, 3.0), 44: (3.0, 20.0), 45: (2.0, 5.0), 46: (3.0, 20.0), 47: (3.0, 20.0),
    50: (3.0, 20.0),
}

# Xbox ADPCM (IMA ADPCM, 36-byte blocks a channel: a 4-byte header with the
# predictor and step index, then 64 samples' nibbles, interleaved between
# the channels 4 bytes at a time; port/linux/src/dsound_sdl.c)
INDEX_TABLE = (-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8)
STEP_TABLE = (
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
    107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428,
    4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767)


def _tables():
    diff, step_next = [], []
    for index in range(89):
        step = STEP_TABLE[index]
        for nibble in range(16):
            d = step >> 3
            if nibble & 1:
                d += step >> 2
            if nibble & 2:
                d += step >> 1
            if nibble & 4:
                d += step
            diff.append(-d if nibble & 8 else d)
            step_next.append(min(88, max(0, index + INDEX_TABLE[nibble])) * 16)
    return diff, step_next


DIFF, NEXT = _tables()


def decode_xbox_adpcm(data, channels):
    """16-bit PCM (interleaved) from Xbox ADPCM: 64 samples a block"""
    block = 36 * channels
    blocks = len(data) // block
    out = [0] * (blocks * 64 * channels)
    diff, step_next = DIFF, NEXT
    for b in range(blocks):
        base = b * block
        frame = b * 64 * channels
        for c in range(channels):
            predictor = struct.unpack_from("<h", data, base + 4 * c)[0]
            state = min(data[base + 4 * c + 2], 88) * 16
            o = frame + c
            for group in range(8):
                p = base + 4 * channels + (group * channels + c) * 4
                word = data[p] | data[p + 1] << 8 | data[p + 2] << 16 | data[p + 3] << 24
                for _ in range(8):
                    k = state + (word & 15)
                    word >>= 4
                    predictor += diff[k]
                    if predictor > 32767:
                        predictor = 32767
                    elif predictor < -32768:
                        predictor = -32768
                    state = step_next[k]
                    out[o] = predictor
                    o += channels
    return struct.pack(f"<{len(out)}h", *out)


def _fields(defs, name):
    return {f.name: o for o, f in defs.fields(name)}


class SoundConverter:
    """the map's kept sounds: compact records for the tag image, and the
    files they play"""

    def __init__(self, xmap, defs, cache_dir):
        self.map = xmap
        self.defs = defs
        self.cache = Path(cache_dir)
        self.sf = _fields(defs, "Sound")
        self.pf = _fields(defs, "SoundPitchRange")
        self.mf = _fields(defs, "SoundPermutation")
        self.lf = _fields(defs, "SoundLooping")
        self.tf = _fields(defs, "SoundLoopingTrack")
        self.files = []        # cache names, in the map's list order
        self.file_index = {}
        self.pending = {}      # cache name -> (adpcm, channels, rate, loop)
        self.seconds = 0.0
        # the sounds a looping sound loops (their files loop)
        self.looped = set()
        for t in xmap.tags:
            if t.group_name == "lsnd":
                for track in self._tracks(t):
                    if track["loop"] is not None:
                        self.looped.add(track["loop"].index)
        self._keep = {}

    # ---------- which tags

    def _class(self, tag):
        return struct.unpack("<h", self.map.read(tag.address + self.sf["sound class"], 2))[0]

    def _tracks(self, tag):
        count, pointer = struct.unpack("<iI", self.map.read(tag.address + self.lf["tracks"], 8))
        size = self.defs.size("SoundLoopingTrack")
        out = []
        for i in range(max(count, 0)):
            raw = self.map.read(pointer + i * size, size)
            track = {"raw": raw}
            for key in ("start", "loop", "end"):
                datum = struct.unpack_from("<I", raw, self.tf[key] + 12)[0]
                track[key] = self.map.tag(datum)
            out.append(track)
        return out

    def keep(self, tag):
        if tag.index in self._keep:
            return self._keep[tag.index]
        kept = False
        if tag.group_name == "snd!":
            sound_class = self._class(tag)
            kept = sound_class in KEPT_CLASSES and self._has_samples(tag)
            # (not the Covenant's animations' foley, 1.4 MB: their voices,
            # weapons and impacts)
            if getattr(tag, "source", None) is not None and sound_class == OBJECT_IMPACTS:
                kept = False
        elif tag.group_name == "lsnd":
            self._keep[tag.index] = False  # (a loop through itself)
            kept = any(track["loop"] is not None and track["loop"].group_name == "snd!" and self.keep(track["loop"])
                       for track in self._tracks(tag))
        self._keep[tag.index] = kept
        return kept

    def _has_samples(self, tag):
        count = struct.unpack("<i", self.map.read(tag.address + self.sf["pitch ranges"], 4))[0]
        return count > 0

    # ---------- records

    def blob(self, tag):
        if tag.group_name == "snd!":
            return self.sound_blob(tag)
        if tag.group_name == "lsnd":
            return self.looping_blob(tag)
        return None

    def sound_blob(self, tag):
        sf, pf, mf = self.sf, self.pf, self.mf
        a = tag.address
        raw = self.map.read(a, self.defs.size("Sound"))

        def f32(name, at=0):
            return struct.unpack_from("<f", raw, sf[name] + at)[0]

        sound_class = self._class(tag)
        rate = SAMPLE_RATES.get(struct.unpack_from("<H", raw, sf["sample rate"])[0], RATE)
        channels = 2 if struct.unpack_from("<H", raw, sf["channel count"])[0] == 1 else 1
        fmt = struct.unpack_from("<H", raw, sf["format"])[0]
        minimum, maximum = f32("minimum distance"), f32("maximum distance")
        default = CLASS_DISTANCES.get(sound_class, (1.0, 10.0))
        minimum = minimum or default[0]
        maximum = maximum or default[1]
        looped = tag.index in self.looped
        cap = DIALOG_PERMUTATIONS if sound_class == UNIT_DIALOG else 32

        count, pointer = struct.unpack_from("<iI", raw, sf["pitch ranges"])
        ranges, permutations = [], []
        psize, msize = self.defs.size("SoundPitchRange"), self.defs.size("SoundPermutation")
        for r in range(max(count, 0)):
            rraw = self.map.read(pointer + r * psize, psize)
            natural = struct.unpack_from("<f", rraw, pf["natural pitch"])[0]
            bend_lower, bend_upper = struct.unpack_from("<2f", rraw, pf["bend bounds"])
            playback = struct.unpack_from("<f", rraw, pf["playback rate"])[0]
            if playback <= 0:
                playback = 1.0 / natural if natural > 0 else 1.0
            pc, pp = struct.unpack_from("<iI", rraw, pf["permutations"])
            actual = min(struct.unpack_from("<H", rraw, pf["actual permutation count"])[0], max(pc, 0))
            first = len(permutations)
            for p in range(min(actual, cap)):
                mraw = self.map.read(pp + p * msize, msize)
                gain = struct.unpack_from("<f", mraw, mf["gain"])[0]
                skip = struct.unpack_from("<f", mraw, mf["skip fraction"])[0]
                data = self._chain(tag, pp, pc, p) if fmt in (0, 1) else None
                file = self._file(data, channels, rate, looped, fmt) if data else 0xFFFF
                permutations.append(struct.pack(">HBB", file, _u8(gain), _u8(skip)))
            ranges.append(struct.pack(">3fIHBb", bend_lower, bend_upper, playback, 0, first,
                                      len(permutations) - first, -1))
        head = struct.pack(
            ">BBBB5f6HI", sound_class, 1 if looped else 0, len(ranges), _u8(f32("skip fraction")),
            minimum, maximum, f32("random pitch bounds"), f32("random pitch bounds", 4),
            f32("random gain modifier") or 1.0,
            _u16(f32("zero skip fraction modifier")), _u16(f32("one skip fraction modifier")),
            _u16(f32("zero gain modifier")), _u16(f32("one gain modifier")),
            _u16(f32("zero pitch modifier")), _u16(f32("one pitch modifier")),
            struct.unpack_from("<I", raw, sf["longest permutation length"])[0])
        assert len(head) == 40
        return head + b"".join(ranges) + b"".join(permutations)

    def looping_blob(self, tag):
        raw = self.map.read(tag.address, self.defs.size("SoundLooping"))
        maximum = struct.unpack_from("<f", raw, self.lf["maximum distance"])[0]
        tracks = []
        for track in self._tracks(tag):
            t = track["raw"]
            refs = []
            for key in ("start", "loop", "end"):
                target = track[key]
                kept = target is not None and target.group_name == "snd!" and self.keep(target)
                refs.append(target.datum & 0xFFFFFFFF if kept else NONE)
                if kept and key != "start":
                    distance = struct.unpack("<f", self.map.read(target.address + self.sf["maximum distance"], 4))[0]
                    maximum = max(maximum, distance or CLASS_DISTANCES.get(self._class(target), (0, 10.0))[1])
            gain, fade_in, fade_out = struct.unpack_from("<3f", t, self.tf["gain"])
            flags = struct.unpack_from("<I", t, self.tf["flags"])[0]
            tracks.append(struct.pack(">3I3fI", *refs, gain, fade_in, fade_out, flags))
        flags = struct.unpack_from("<I", raw, self.lf["flags"])[0]
        return struct.pack(">BBHf", len(tracks), flags & 0xFF, 0, maximum) + b"".join(tracks)

    # ---------- samples

    def _chain(self, tag, pointer, count, first):
        """a permutation's samples, with the ones that continue it"""
        source = getattr(tag, "source", None) or self.map
        msize = self.defs.size("SoundPermutation")
        data = bytearray()
        index, seen = first, set()
        while 0 <= index < count and index not in seen:
            seen.add(index)
            raw = self.map.read(pointer + index * msize, msize)
            size, flags, offset, _, _ = struct.unpack_from("<iIIII", raw, self.mf["samples"])
            if size > 0:
                data += source.data[offset:offset + size]
            index = struct.unpack_from("<h", raw, self.mf["next permutation index"])[0]
        return bytes(data)

    def _file(self, data, channels, rate, looped, fmt):
        key = hashlib.sha1(CACHE_VERSION + bytes([channels, fmt, 1 if looped else 0])
                           + struct.pack("<I", rate) + data).hexdigest()[:16]
        if key not in self.file_index:
            self.file_index[key] = len(self.files)
            self.files.append(key)
            if not (self.cache / f"{key}.wav64").exists():
                self.pending[key] = (data, channels, rate, looped, fmt)
            block = 36 * channels
            self.seconds += (len(data) // block * 64 if fmt == 1 else len(data) // (2 * channels)) / rate
        return self.file_index[key]

    def write(self, list_path):
        """converts the files the cache lacks; writes the map's list"""
        self.cache.mkdir(parents=True, exist_ok=True)
        if self.pending:
            with tempfile.TemporaryDirectory(dir=self.cache) as work:
                work = Path(work)
                for looped in (False, True):
                    names = [k for k, v in self.pending.items() if v[3] == looped]
                    if not names:
                        continue
                    source = work / ("loop" if looped else "once")
                    out = work / ("loop-out" if looped else "once-out")
                    source.mkdir()
                    out.mkdir()
                    for key in names:
                        data, channels, rate, _, fmt = self.pending[key]
                        pcm = decode_xbox_adpcm(data, channels) if fmt == 1 else data[:len(data) // 2 * 2]
                        (source / f"{key}.wav").write_bytes(pcm_wav(pcm, rate, channels))
                    # (VADPCM without its Huffman packing: that is undone by
                    # the CPU, a quarter of it with a few voices playing,
                    # and read from the cartridge synchronously; plain
                    # frames stream by DMA and the RSP decodes them)
                    flags = ["--wav-mono", "--wav-resample", str(RATE), "--wav-compress", "vadpcm,huffman=false"]
                    if looped:
                        flags += ["--wav-loop", "true"]
                    subprocess.run([str(AUDIOCONV), *flags, "-o", str(out), str(source)], check=True,
                                   capture_output=True)
                    for key in names:
                        # (another map's conversion may make the same file:
                        # each replaces it whole)
                        os.replace(out / f"{key}.wav64", self.cache / f"{key}.wav64")
        Path(list_path).parent.mkdir(parents=True, exist_ok=True)
        Path(list_path).write_text("".join(f"{n}\n" for n in self.files))
        return len(self.files), len(self.pending)


def _u8(value):
    return max(0, min(255, int(round(value * 255))))


def _u16(value):
    return max(0, min(65535, int(round(value * 4096))))
