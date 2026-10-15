"""The main menu's music for the N64 (port/n64/src/n64_menu.c).

Halo's title music is a looping sound in ui.map (sound\\music\\title1\\
title1): a track whose start sound ("in", the monks) plays once and whose
loop sound ("loops") then plays for ever, one of its three versions at a
time. Each version is a chain of permutations (next permutation index)
whose samples are Xbox ADPCM in the cache file: 4-bit IMA ADPCM in
36-byte blocks a channel (a 4-byte header with the first sample and step
index, then 63 used sample nibbles and one padding nibble, interleaved
between channels 4 bytes at a time). Each block contains 64 frames. The chains
are joined (each block starts afresh), decoded here, and resampled and
compressed to VADPCM wav64 files with libdragon's audioconv64 (its
libsamplerate; ffmpeg did the resampling, which the Windows ROM builder
would have had to carry), which the menu streams from the cartridge:

    music/title_in.wav64, music/title_loop0..2.wav64

    python3 -m tools.n64.music <ui.map> --filesystem <file tree>
"""

import argparse
import struct
import subprocess
import tempfile
from pathlib import Path

from . import toolpaths
from .tagdefs import TagDefinitions
from .toolpaths import tool
from .xboxmap import XboxMap

TITLE_MUSIC = "sound\\music\\title1\\title1"
# the N64's playback rate (the menu's mixer, n64_menu.c)
RATE = 22050
AUDIOCONV = Path(tool("audioconv64"))
SAMPLE_RATES = {0: 22050, 1: 44100}


def chains(xmap, defs, sound_tag):
    """the sound's versions: each actual permutation's chain of samples,
    joined; and its sample rate and channel count"""
    sf = {f.name: o for o, f in defs.fields("Sound")}
    pf = {f.name: o for o, f in defs.fields("SoundPitchRange")}
    mf = {f.name: o for o, f in defs.fields("SoundPermutation")}
    msize = defs.size("SoundPermutation")
    a = sound_tag.address
    rate = SAMPLE_RATES[struct.unpack("<H", xmap.read(a + sf["sample rate"], 2))[0]]
    channels = 2 if struct.unpack("<H", xmap.read(a + sf["channel count"], 2))[0] == 1 else 1
    fmt = struct.unpack("<H", xmap.read(a + sf["format"], 2))[0]
    if fmt != 1:
        raise ValueError(f"{sound_tag.name}: format {fmt}, not Xbox ADPCM")
    count, pointer = struct.unpack("<iI", xmap.read(a + sf["pitch ranges"], 8))
    out = []
    for r in range(max(count, 0)):
        base = pointer + r * defs.size("SoundPitchRange")
        pc, pp = struct.unpack("<iI", xmap.read(base + pf["permutations"], 8))
        actual = struct.unpack("<H", xmap.read(base + pf["actual permutation count"], 2))[0]
        for first in range(min(actual, pc)):
            data = bytearray()
            index, seen = first, set()
            while 0 <= index < pc and index not in seen:
                seen.add(index)
                raw = xmap.read(pp + index * msize, msize)
                size, flags, offset, _, _ = struct.unpack_from("<iIIII", raw, mf["samples"])
                data += xmap.data[offset:offset + size]
                index = struct.unpack_from("<h", raw, mf["next permutation index"])[0]
            out.append(bytes(data))
    return out, rate, channels


INDEX_TABLE = (-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8)
STEP_TABLE = (
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
    107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428,
    4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767)


def decode_xbox_adpcm(data, channels):
    """16-bit PCM (interleaved) from Xbox ADPCM: 64 samples a block"""
    block = 36 * channels
    blocks = len(data) // block
    out = [0] * (blocks * 64 * channels)
    for b in range(blocks):
        base = b * block
        frame = b * 64 * channels
        for c in range(channels):
            predictor = struct.unpack_from("<h", data, base + 4 * c)[0]
            index = min(data[base + 4 * c + 2], 88)
            out[frame + c] = predictor
            for group in range(8):
                nibbles = base + 4 * channels + (group * channels + c) * 4
                for k in range(7 if group == 7 else 8):
                    nibble = (data[nibbles + (k >> 1)] >> (4 * (k & 1))) & 15
                    step = STEP_TABLE[index]
                    difference = step >> 3
                    if nibble & 1:
                        difference += step >> 2
                    if nibble & 2:
                        difference += step >> 1
                    if nibble & 4:
                        difference += step
                    if nibble & 8:
                        difference = -difference
                    predictor = max(-32768, min(32767, predictor + difference))
                    index = max(0, min(88, index + INDEX_TABLE[nibble]))
                    out[frame + (group * 8 + k + 1) * channels + c] = predictor
    return struct.pack(f"<{len(out)}h", *out)


def pcm_wav(pcm, rate, channels):
    """a WAV file around 16-bit PCM"""
    fmt = struct.pack("<HHIIHH", 1, channels, rate, rate * 2 * channels, 2 * channels, 16)
    body = b"fmt " + struct.pack("<I", len(fmt)) + fmt + b"data" + struct.pack("<I", len(pcm)) + pcm
    return b"RIFF" + struct.pack("<I", 4 + len(body)) + b"WAVE" + body


def convert(xmap, defs, filesystem):
    music = filesystem / "music"
    music.mkdir(parents=True, exist_ok=True)
    lf = {f.name: o for o, f in defs.fields("SoundLooping")}
    tf = {f.name: o for o, f in defs.fields("SoundLoopingTrack")}
    found = [t for t in xmap.tags if t.group_name == "lsnd" and t.name == TITLE_MUSIC]
    if not found:
        raise KeyError(f"{xmap.name} has no {TITLE_MUSIC}")
    count, pointer = struct.unpack("<iI", xmap.read(found[0].address + lf["tracks"], 8))
    raw = xmap.read(pointer, defs.size("SoundLoopingTrack"))
    start = xmap.tag(struct.unpack_from("<I", raw, tf["start"] + 12)[0])
    loop = xmap.tag(struct.unpack_from("<I", raw, tf["loop"] + 12)[0])
    written = []
    with tempfile.TemporaryDirectory() as work:
        work = Path(work)
        for name, tag in (("title_in", start), ("title_loop", loop)):
            if tag is None:
                continue
            versions, rate, channels = chains(xmap, defs, tag)
            for i, data in enumerate(versions):
                out_name = name if name == "title_in" else f"{name}{i}"
                pcm = decode_xbox_adpcm(data, channels)
                if channels == 1:
                    # (the menu's music is stereo)
                    samples = struct.unpack(f"<{len(pcm) // 2}h", pcm)
                    pcm = struct.pack(f"<{2 * len(samples)}h", *(s for s in samples for _ in range(2)))
                # (audioconv64 names its output after its input)
                decoded = work / f"{out_name}.wav"
                decoded.write_bytes(pcm_wav(pcm, rate, 2))
                toolpaths.run([str(AUDIOCONV), "--wav-resample", str(RATE), "--wav-compress", "1", "-o", str(music),
                                str(decoded)], check=True, capture_output=True)
                seconds = len(data) / (36 * channels) * 64 / rate
                written.append(f"{out_name} ({seconds:.0f} s)")
    return written


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("map", type=Path)
    parser.add_argument("--filesystem", type=Path, required=True)
    args = parser.parse_args()
    xmap = XboxMap(args.map)
    written = convert(xmap, TagDefinitions(), args.filesystem)
    print("music:", ", ".join(written))


if __name__ == "__main__":
    main()
