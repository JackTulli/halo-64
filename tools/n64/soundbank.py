"""The cartridge's one sound bank (port/n64/src/n64_sound.c).

Every map's sounds (sounds.py: its list build/n64/sound/<map>.slist of
wav64 files in build/n64/sound-cache) packed into one file, each file once,
16-byte aligned, with each map's table of where its list's files are:

    snd/sounds.bank     the wav64 files
    snd/<map>.ofs       u32 (big-endian) a list entry: the file's offset

One file keeps DragonFS's lookup table (kept in RAM) small; a sound plays
from its offset (wav64_loadfd).

    python3 -m tools.n64.soundbank <build dir> <file tree> <map>...
"""

import sys
from pathlib import Path


def main():
    build = Path(sys.argv[1])
    root = Path(sys.argv[2])
    maps = sys.argv[3:]
    lists = {}
    for m in maps:
        listing = build / "sound" / f"{m}.slist"
        lists[m] = [line.strip() for line in listing.read_text().splitlines() if line.strip()] \
            if listing.exists() else []
    names = sorted({n for files in lists.values() for n in files})
    out = root / "snd"
    out.mkdir(parents=True, exist_ok=True)
    offsets = {}
    bank = bytearray()
    for name in names:
        bank += b"\0" * ((-len(bank)) % 16)
        offsets[name] = len(bank)
        bank += (build / "sound-cache" / f"{name}.wav64").read_bytes()
    made = out / ".sounds.bank"
    made.write_bytes(bank)
    made.replace(out / "sounds.bank")
    for m, files in lists.items():
        (out / f"{m}.ofs").write_bytes(b"".join(offsets[n].to_bytes(4, "big") for n in files))
    # earlier builds' tables
    for path in out.glob("*.ofs"):
        if path.stem not in lists:
            path.unlink()
    print(f"sound bank: {len(names)} files, {len(bank) / 1e6:.1f} MB")


if __name__ == "__main__":
    main()
