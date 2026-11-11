#!/usr/bin/env python3
"""HUD STYLE 64's design: every texture, glyph and meter shape of the HUD,
made by this code alone (no image file is read). tools/n64/hud64/hud64_gen.py
imports it and writes the game's tables and texture files from it.

The design is Halo CE's HUD composition redrawn as N64 pixel art for 320x240,
640x240 and 640x480 (this file; hud64_gen.py applies the later changes to
it as overrides). Every player count is the 1-player
HUD at its own scale in each view's own corners. 3-4 players drop the
sensor's range label. The pickup prompt keeps HOLD and the verb; the rounds
are whole 2-pixel ticks. WIDESCREEN squeezes everything, with 2-pixel text
strokes in the 640-wide modes (1-pixel only at 320x240).

What everything is made from:
  - text: FONT7 and FONT7N below, 7-row letters drawn by hand as rows of '#';
    at 480 lines FONT7 doubled by EPX (face_large14);
  - numerals: numeral(), seven segments drawn from a few stroke sizes;
  - icons (times, round, grenades, health cross, B button): rows of '#' and
    shapes drawn by code (ellipse, poly);
  - panels, plate, meters, ticks, the motion sensor's disc: polygons and
    ellipses (poly, ellipse) filled at 8x8 samples a pixel;
  - colours: the palette below, Halo CE's HUD colours as numbers.

Sets (HUD STYLE "64": these textures only; ORIGINAL keeps Halo's own):
  240 set:     drawn 1:1 at 320x240; at 640x240 the graphics doubled across and
               the text from its own 2x-wide glyphs with a 1-pixel outline;
  240 WS set:  the 240 set redrawn 75% wide for WIDESCREEN (320x240: condensed
               1-pixel face; 640x240: the same face 2x across, 2-pixel strokes);
  480 set:     640x480i: the 240 face doubled (14-row capitals), flat fills
               shared with the 240 set at 2x;
  480 WS set:  the 480 set 75% wide: its face squeezed by column removal
               (strokes >= 2 px), flat fills shared with the 240 WS set at 2x.
"""
import sys

import numpy as np
from PIL import Image, ImageDraw

sys.dont_write_bytecode = True

# ---------------------------------------------------------------- palette (Halo CE tag colours)
BLUE = (0x28, 0x96, 0xFF)
OUTLINE = (0xAA, 0xD5, 0xFF)
NUMBER = (0xE6, 0xF0, 0xFF)
HELP = (0xD8, 0xEC, 0xFF)
SHADOW = (0x00, 0x14, 0x30)
TICK_ON = (0xE6, 0xF0, 0xFF)
TICK_OFF = (0x14, 0x28, 0x46)
PLATE = (0x00, 0x10, 0x28)
SENSOR = (0x8C, 0xAA, 0xC8)    # cone and marker (Halo #6986A0, lifted: a lit tracker)
SENSOR_BG = (0x3C, 0x64, 0x8C)  # the disc's lit fill
SENSOR_FILL = 0.26             # the disc's lit fill
RETICLE = (0x46, 0xA5, 0xFF)
PLASMA = (0x78, 0xC8, 0xFF)
RED = (0xFF, 0x3C, 0x28)
YELLOW = (0xFF, 0xDF, 0x00)
BUTTON = (0x28, 0xB4, 0x50)
WHITE = (0xFF, 0xFF, 0xFF)
LEADER = (0xA0, 0xB4, 0xC8)

PANEL_ALPHA = (0.63, 0.46)     # Halo's weapon panel
EDGE_ALPHA = 0.86
GLOW = (0.0, 0.6)              # 320-wide text: haze (intensity navy -> glyph, alpha); dark and thin, so
                               # that it covers little of the picture and the letters keep their contrast
NUM_GLOW = (0.0, 0.75)
OUT_ALPHA = 1.0                # 640-wide modes: 1-pixel dark outline (opaque: IA4)
WS = 0.75                      # WIDESCREEN: every element drawn this wide
WS_GLOW = (0.10, 1.0)          # the condensed face's 1-px strokes need a darker, opaque surround
ELEMENTS = ("ammo", "grenades", "shield", "sensor", "score", "messages", "reticle")


def c01(rgb):
    return np.array(rgb, np.float32) / 255.0


# ---------------------------------------------------------------- formats
BPP = {"IA4": 4, "IA8": 8, "IA16": 16, "I4": 4, "I8": 8, "RGBA16": 16}


def quantise(fmt, I, A):
    if fmt == "IA4":
        return np.round(I * 7) / 7, (A >= 0.5).astype(np.float32)
    if fmt == "IA8":
        return np.round(I * 15) / 15, np.round(A * 15) / 15
    if fmt == "IA16":
        return np.round(I * 255) / 255, np.round(A * 255) / 255
    if fmt == "I4":
        q = np.round(I * 15) / 15
        return q, q
    if fmt == "I8":
        q = np.round(I * 255) / 255
        return q, q
    raise ValueError(fmt)


def two_rows(I, A):
    I, A = I.copy(), A.copy()
    m = A > 0
    H, W = m.shape
    for x in range(W):
        y = 0
        while y < H:
            if not m[y, x]:
                y += 1
                continue
            y0 = y
            while y < H and m[y, x]:
                y += 1
            if y - y0 == 1:
                t = y0 + 1 if y0 + 1 < H else y0 - 1
                if t >= 0:
                    I[t, x], A[t, x] = I[y0, x], A[y0, x]
                    m[t, x] = True
                    y = max(y, t + 1)
    return I, A


class Tex:
    def __init__(self, name, fmt, I, A, sets, note=""):
        I = np.clip(np.asarray(I, np.float32), 0, 1)
        if fmt in ("I4", "I8"):
            A = I
        A = np.clip(np.asarray(A, np.float32), 0, 1)
        if sets == "480":
            I, A = two_rows(I, A)
        self.name, self.fmt, self.sets, self.note = name, fmt, sets, note
        self.I, self.A = quantise(fmt, I, A)
        self.h, self.w = self.I.shape

    def bytes(self):
        return self.w * self.h * BPP[self.fmt] // 8


class Scaled:
    """a 240-line texture drawn 2x at 480i (shared, stored once)"""

    def __init__(self, base, s):
        self.base, self.fmt, self.sets = base, base.fmt, None
        self.I = np.kron(base.I, np.ones((s, s), np.float32))
        self.A = np.kron(base.A, np.ones((s, s), np.float32))
        self.h, self.w = self.I.shape


# ---------------------------------------------------------------- mask tools
def shift(m, dy, dx):
    H, W = m.shape
    out = np.zeros_like(m)
    ys, yd = (slice(0, H - dy), slice(dy, H)) if dy >= 0 else (slice(-dy, H), slice(0, H + dy))
    xs, xd = (slice(0, W - dx), slice(dx, W)) if dx >= 0 else (slice(-dx, W), slice(0, W + dx))
    out[yd, xd] = m[ys, xs]
    return out


def grow(m, ry, rx):
    out = m.copy()
    for dy in range(-ry, ry + 1):
        for dx in range(-rx, rx + 1):
            out |= shift(m, dy, dx)
    return out


def dilate(m, r):
    return grow(m, r, r)


def erode(m, r):
    out = m.copy()
    for dy in range(-r, r + 1):
        for dx in range(-r, r + 1):
            out &= shift(m, dy, dx)
    return out


def poly(points, w, h, s=1, sx=1.0, ss=8):
    """a polygon in 240-set units at scale s, squeezed across by sx"""
    W, H = int(round(w * s * sx)), h * s
    img = Image.new("L", (W * ss, H * ss), 0)
    ImageDraw.Draw(img).polygon([(x * s * sx * ss, y * s * ss) for x, y in points], fill=255)
    return np.asarray(img.resize((W, H), Image.BOX), np.float32) / 255 >= 0.5


def ellipse(cx, cy, rx, ry, W, H, ss=8):
    img = Image.new("L", (W * ss, H * ss), 0)
    ImageDraw.Draw(img).ellipse([(cx - rx) * ss, (cy - ry) * ss, (cx + rx) * ss, (cy + ry) * ss], fill=255)
    return np.asarray(img.resize((W, H), Image.BOX), np.float32) / 255


def squeeze(m, sx):
    """a 1-bit picture resampled to sx of its width (box filter, >= 0.5)"""
    if sx == 1.0:
        return m
    h, w = m.shape
    W = max(1, int(round(w * sx)))
    img = Image.fromarray((m * 255).astype(np.uint8)).resize((W, h), Image.BOX)
    return np.asarray(img) >= 128


def epx(m):
    H, W = m.shape
    p = np.pad(m, 1)
    P = m
    A, B, C, D = p[0:H, 1:W + 1], p[1:H + 1, 2:W + 2], p[1:H + 1, 0:W], p[2:H + 2, 1:W + 1]
    e0 = np.where((C == A) & (C != D) & (A != B), A, P)
    e1 = np.where((A == B) & (A != C) & (B != D), B, P)
    e2 = np.where((D == C) & (D != B) & (C != A), C, P)
    e3 = np.where((B == D) & (B != A) & (D != C), D, P)
    out = np.zeros((2 * H, 2 * W), bool)
    out[0::2, 0::2], out[0::2, 1::2], out[1::2, 0::2], out[1::2, 1::2] = e0, e1, e2, e3
    return out


def bits(rows):
    return np.array([[c == "#" for c in r] for r in rows], bool)


# ---------------------------------------------------------------- text faces (240 lines)
# small_ui letterforms at 7 rows, 2-pixel stems; look-alike pairs kept apart
# (no two letters with the same shape); 3-pixel word spaces and 5-pixel C and
# K, so the longest 4p prompt ("[B] PICK UP ROCKET LAUNCHER") fits one line of
# a quadrant.
FONT7 = {
    "A": ["..##..", ".####.", "##..##", "##..##", "######", "##..##", "##..##"],
    "B": ["#####.", "##..##", "##..##", "#####.", "##..##", "##..##", "#####."],
    "C": [".####", "##...", "##...", "##...", "##...", "##...", ".####"],
    "D": ["#####.", "##..##", "##..##", "##..##", "##..##", "##..##", "#####."],
    "E": ["#####", "##...", "##...", "####.", "##...", "##...", "#####"],
    "F": ["#####", "##...", "##...", "####.", "##...", "##...", "##..."],
    "G": [".####.", "##..##", "##....", "##.###", "##..##", "##..##", ".#####"],
    "H": ["##..##", "##..##", "##..##", "######", "##..##", "##..##", "##..##"],
    "I": ["##", "##", "##", "##", "##", "##", "##"],
    "J": ["....##", "....##", "....##", "....##", "##..##", "##..##", ".####."],
    "K": ["##..##", "##.##.", "####..", "###...", "####..", "##.##.", "##..##"],
    "L": ["##...", "##...", "##...", "##...", "##...", "##...", "#####"],
    "M": ["##....##", "###..###", "########", "##.##.##", "##....##", "##....##", "##....##"],
    "N": ["##..##", "###.##", "######", "##.###", "##..##", "##..##", "##..##"],
    "O": [".####.", "##..##", "##..##", "##..##", "##..##", "##..##", ".####."],
    "P": ["#####.", "##..##", "##..##", "#####.", "##....", "##....", "##...."],
    "Q": [".####.", "##..##", "##..##", "##..##", "##.###", "##..##", ".##.##"],
    "R": ["#####.", "##..##", "##..##", "#####.", "##.##.", "##..##", "##..##"],
    "S": [".####.", "##..##", "##....", ".####.", "....##", "##..##", ".####."],
    "T": ["######", "..##..", "..##..", "..##..", "..##..", "..##..", "..##.."],
    "U": ["##..##", "##..##", "##..##", "##..##", "##..##", "##..##", ".####."],
    "V": ["##..##", "##..##", "##..##", "##..##", "##..##", ".####.", "..##.."],
    "W": ["##....##", "##....##", "##....##", "##.##.##", "########", "###..###", "##....##"],
    "X": ["##..##", "##..##", ".####.", "..##..", ".####.", "##..##", "##..##"],
    "Y": ["##..##", "##..##", ".####.", "..##..", "..##..", "..##..", "..##.."],
    "Z": ["######", "....##", "...##.", "..##..", ".##...", "##....", "######"],
    "0": [".###.", "##.##", "##.##", "##.##", "##.##", "##.##", ".###."],
    "1": [".##", "###", ".##", ".##", ".##", ".##", ".##"],
    "2": [".####.", "##..##", "....##", "...##.", "..##..", ".##...", "######"],
    "3": [".####.", "##..##", "....##", "..###.", "....##", "##..##", ".####."],
    "4": ["...##.", "..###.", ".####.", "##.##.", "######", "...##.", "...##."],
    "5": ["######", "##....", "#####.", "....##", "....##", "##..##", ".####."],
    "6": ["..###.", ".##...", "##....", "#####.", "##..##", "##..##", ".####."],
    "7": ["######", "....##", "...##.", "..##..", "..##..", "..##..", "..##.."],
    "8": [".####.", "##..##", ".####.", "##..##", "##..##", "##..##", ".####."],
    "9": [".####.", "##..##", "##..##", ".#####", "....##", "...##.", ".###.."],
    "-": ["....", "....", "....", "####", "....", "....", "...."],
    ".": ["..", "..", "..", "..", "..", "##", "##"],
    ":": ["..", "##", "##", "..", "##", "##", ".."],
    "!": ["##", "##", "##", "##", "##", "..", "##"],
    "m": ["#######.", "##.##.##", "##.##.##", "##.##.##", "##.##.##"],
}
# WIDESCREEN's condensed face: 4 pixels a letter (1-pixel stems), so a line
# is 0.71-0.75 of the normal face's width. 2-pixel stems cannot be kept at
# this width on a 320-wide picture.
FONT7N = {
    "A": [".##.", "#..#", "#..#", "####", "#..#", "#..#", "#..#"],
    "B": ["###.", "#..#", "#..#", "###.", "#..#", "#..#", "###."],
    "C": [".###", "#...", "#...", "#...", "#...", "#...", ".###"],
    "D": ["###.", "#..#", "#..#", "#..#", "#..#", "#..#", "###."],
    "E": ["####", "#...", "#...", "###.", "#...", "#...", "####"],
    "F": ["####", "#...", "#...", "###.", "#...", "#...", "#..."],
    "G": [".###", "#...", "#...", "#.##", "#..#", "#..#", ".###"],
    "H": ["#..#", "#..#", "#..#", "####", "#..#", "#..#", "#..#"],
    "I": ["###", ".#.", ".#.", ".#.", ".#.", ".#.", "###"],
    "J": ["..#", "..#", "..#", "..#", "..#", "#.#", ".#."],
    "K": ["#..#", "#.#.", "##..", "#...", "##..", "#.#.", "#..#"],
    "L": ["#..", "#..", "#..", "#..", "#..", "#..", "###"],
    "M": ["#...#", "##.##", "#.#.#", "#...#", "#...#", "#...#", "#...#"],
    "N": ["#..#", "##.#", "##.#", "#.##", "#.##", "#..#", "#..#"],
    "O": [".##.", "#..#", "#..#", "#..#", "#..#", "#..#", ".##."],
    "P": ["###.", "#..#", "#..#", "###.", "#...", "#...", "#..."],
    "Q": [".##.", "#..#", "#..#", "#..#", "#.#.", "#..#", ".#.#"],
    "R": ["###.", "#..#", "#..#", "###.", "#.#.", "#..#", "#..#"],
    "S": [".###", "#...", "#...", ".##.", "...#", "...#", "###."],
    "T": ["###", ".#.", ".#.", ".#.", ".#.", ".#.", ".#."],
    "U": ["#..#", "#..#", "#..#", "#..#", "#..#", "#..#", ".##."],
    "V": ["#..#", "#..#", "#..#", "#..#", "#..#", ".##.", ".##."],
    "W": ["#...#", "#...#", "#...#", "#.#.#", "#.#.#", "##.##", "#...#"],
    "X": ["#..#", "#..#", ".##.", ".##.", ".##.", "#..#", "#..#"],
    "Y": ["#.#", "#.#", "#.#", ".#.", ".#.", ".#.", ".#."],
    "Z": ["####", "...#", "..#.", ".#..", "#...", "#...", "####"],
    "0": [".##.", "#..#", "#.##", "#.##", "##.#", "##.#", ".##."],
    "1": [".#", "##", ".#", ".#", ".#", ".#", ".#"],
    "2": [".##.", "#..#", "...#", "..#.", ".#..", "#...", "####"],
    "3": ["###.", "...#", "...#", ".##.", "...#", "...#", "###."],
    "4": ["..#.", ".##.", "#.#.", "#.#.", "####", "..#.", "..#."],
    "5": ["####", "#...", "###.", "...#", "...#", "#..#", ".##."],
    "6": [".##.", "#...", "#...", "###.", "#..#", "#..#", ".##."],
    "7": ["####", "...#", "..#.", "..#.", ".#..", ".#..", ".#.."],
    "8": [".##.", "#..#", ".##.", "#..#", "#..#", "#..#", ".##."],
    "9": [".##.", "#..#", "#..#", ".###", "...#", "..#.", ".#.."],
    "-": ["...", "...", "...", "###", "...", "...", "..."],
    ".": [".", ".", ".", ".", ".", ".", "#"],
    ":": [".", "#", ".", ".", ".", "#", "."],
    "!": ["#", "#", "#", "#", "#", ".", "#"],
    "m": ["##.#.", "#.#.#", "#.#.#", "#.#.#", "#.#.#"],
}
CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-.:!m"


def treat(m, kind):
    """glyph + edge as one IA texture (I 1 = PRIM, 0 = ENV): glow (320),
    out (640x240: 1-px outline), out2 (480i: 1 px x 2 rows)"""
    if kind in ("glow", "numglow", "glowfull"):
        gi, ga = GLOW if kind == "glow" else NUM_GLOW if kind == "numglow" else WS_GLOW
        f = np.pad(m, ((1, 1), (1, 1)))
        sh = (shift(f, 0, 1) | shift(f, 1, 0) | shift(f, 1, 1)) & ~f
        g = dilate(f, 1) & ~f & ~sh
        return np.where(f, 1.0, np.where(g, gi, 0.0)), np.where(f | sh, 1.0, np.where(g, ga, 0.0)), (1, 1)
    if kind in ("soft", "soft2"):
        # the 640-wide modes: the glyph alone; a soft offset shadow
        # is drawn from the same cells (draw_cell), as Halo draws its text
        f = m.astype(np.float32)
        return f, f, (0, 0)
    ry = 2 if kind == "out2" else 1
    f = np.pad(m, ((ry, ry), (1, 1)))
    o = grow(f, ry, 1) & ~f
    return f.astype(np.float32), np.where(f, 1.0, np.where(o, OUT_ALPHA, 0.0)), (1, ry)


class Face:
    def __init__(self, name, masks, oys, cap, space, gap, kind, unit):
        self.name, self.masks, self.oy, self.cap = name, masks, oys, cap
        self.space, self.gap, self.kind, self.unit = space, gap, kind, unit
        self.cells, self.pad = {}, (0, 0)
        for ch, m in masks.items():
            I, A, pad = treat(m, kind)
            self.cells[ch] = (I, A)
            self.pad = pad
        self.fmt = "IA8" if kind in ("glow", "numglow") else "IA4"

    def width(self, ch):
        return self.masks[ch].shape[1]


def face240(table, name, kind, cap, space, gap=1):
    masks = {ch: bits(table[ch]) for ch in CHARS if ch in table}
    return Face(name, masks, {ch: (cap if ch != "m" else len(table["m"])) for ch in masks}, cap, space, gap, kind, 1)


def widen(face, name):
    masks = {ch: np.repeat(m, 2, axis=1) for ch, m in face.masks.items()}
    return Face(name, masks, dict(face.oy), face.cap, face.space * 2, face.gap * 2, "soft", 2)


def face_large14():
    """the 480-line message face: FONT7 doubled by EPX (14-row capitals, 4-pixel
    stems, the diagonals' steps smoothed: 14 rows read at 480 lines as 7 do at
    240), then no 1-pixel run left (widen_thin); no two letters share a shape,
    as in FONT7"""
    masks, oys = {}, {}
    for ch in CHARS:
        if ch in FONT7:
            masks[ch] = widen_thin(epx(bits(FONT7[ch])))
            oys[ch] = 2 * (7 if ch != "m" else len(FONT7["m"]))
    return Face("large14", masks, oys, 14, 6, 2, "soft2", 2)


def widen_thin(m):
    """every 1-pixel horizontal run of ink widened to 2 (toward the
    side with ink above or below), so no part of a 2-pixel face is 1 pixel"""
    m = m.copy()
    H, W = m.shape
    for y in range(H):
        x = 0
        while x < W:
            if m[y, x] and (x == 0 or not m[y, x - 1]) and (x + 1 >= W or not m[y, x + 1]):
                near = lambda c: 0 <= c < W and ((y > 0 and m[y - 1, c]) or (y + 1 < H and m[y + 1, c]))
                if x + 1 < W and (near(x + 1) or x == 0):
                    m[y, x + 1] = True
                elif x > 0:
                    m[y, x - 1] = True
                x += 2
            else:
                x += 1
    return m


def condense(m, sx, minrun=2):
    """a glyph squeezed to sx of its width by removing whole columns: every
    horizontal stroke keeps >= minrun pixels (or its own width if thinner),
    no counter closes, no stroke breaks. Columns that repeat a neighbour go
    first (no shape lost), the widest repeated runs before narrow ones."""
    def rowruns(a):
        out = []
        for row in a:
            ink, gaps, n, cur = [], [], 0, None
            for v in row:
                if v == cur:
                    n += 1
                    continue
                if cur is not None:
                    (ink if cur else gaps).append(n)
                cur, n = v, 1
            if cur is not None:
                (ink if cur else gaps).append(n)
            out.append((len(ink), min(ink) if ink else 99))
        return out
    ref = rowruns(m)
    target = max(1, int(round(m.shape[1] * sx)))
    while m.shape[1] > target:
        best = None
        w = m.shape[1]
        for c in range(w):
            m2 = np.delete(m, c, axis=1)
            ok = all(n2 == n0 and (n0 == 0 or k2 >= min(minrun, k0))
                     for (n2, k2), (n0, k0) in zip(rowruns(m2), ref))
            if not ok:
                continue
            nb = [m[:, c - 1]] if c > 0 else []
            nb += [m[:, c + 1]] if c < w - 1 else []
            cost = min(int((m[:, c] != q).sum()) for q in nb)
            run = 1
            for dx in (-1, 1):
                k = c + dx
                while 0 <= k < w and (m[:, k] == m[:, c]).all():
                    run, k = run + 1, k + dx
            key = (cost, -run, abs(c - (w - 1) / 2))
            if best is None or key < best[0]:
                best = (key, c)
        if best is None:
            break
        m = np.delete(m, best[1], axis=1)
    return m


def face_condensed(face, sx, name, gap, space):
    masks = {ch: condense(mk, sx) for ch, mk in face.masks.items()}
    return Face(name, masks, dict(face.oy), face.cap, space, gap, face.kind, face.unit)


# ---------------------------------------------------------------- numerals
SEGS = {"0": "abcdef", "2": "abged", "3": "abgcd", "4": "fgbc", "5": "afgcd",
        "6": "afgecd", "7": "abc", "8": "abcdefg", "9": "abcdfg"}


# numeral cells: width, stroke across, stroke down, lean, slash width and
# stroke, the 1's offset and flag, corner cut. WIDESCREEN (sx < 1): 320x240
# fallback 1-pixel strokes; 480i keeps 2-pixel strokes (a 1-pixel line
# flickers in an interlaced picture)
NUMSPEC = {(1, False): (7, 2, 1, 2, 5, 2, 1, 2, 1), (1, True): (5, 1, 1, 1, 4, 1, 1, 1, 1),
           (2, False): (13, 2, 2, 4, 9, 2, 2, 4, 2), (2, True): (10, 2, 2, 3, 7, 2, 1, 3, 2)}


def numeral(d, H, s, sx=1.0):
    W, SW, BT, lean, slw, slsw, one_off, fl, cutw = NUMSPEC[(s, sx != 1.0)]
    rows = H // s
    g0 = (rows - 1) // 2 * s if rows == 7 else 3 * s
    if d == "/":
        W = slw
        m = np.zeros((H, W), bool)
        sw = slsw
        for y in range(H):
            x = round((H - 1 - y) * (W - sw) / (H - 1))
            m[y, x:x + sw] = True
    elif d == "1":
        m = np.zeros((H, W), bool)
        x0 = (W - SW) // 2 + one_off
        m[:, x0:x0 + SW] = True
        m[0:BT * (2 if s == 1 else 1), max(0, x0 - fl):x0] = True
    else:
        m = np.zeros((H, W), bool)
        seg = SEGS[d]
        if "a" in seg: m[0:BT, :] = True
        if "d" in seg: m[H - BT:H, :] = True
        if "g" in seg: m[g0:g0 + BT, :] = True
        if "f" in seg: m[0:g0 + BT, 0:SW] = True
        if "b" in seg: m[0:g0 + BT, W - SW:W] = True
        if "e" in seg: m[g0:H, 0:SW] = True
        if "c" in seg: m[g0:H, W - SW:W] = True
        ch = cutw

        def cut(y0, x0):
            m[y0:y0 + ch, x0:x0 + ch] = False
        if "a" in seg and "f" in seg: cut(0, 0)
        if "a" in seg and "b" in seg: cut(0, W - ch)
        if "d" in seg and "e" in seg: cut(H - ch, 0)
        if "d" in seg and "c" in seg: cut(H - ch, W - ch)
        if "g" in seg and ("f" in seg) != ("e" in seg): cut(g0 if "e" in seg else g0 + BT - ch, 0)
        if "g" in seg and ("b" in seg) != ("c" in seg): cut(g0 if "c" in seg else g0 + BT - ch, W - ch)
    out = np.zeros((H, W + lean), bool)
    for y in range(H):
        off = round((rows - 1 - y // s) * lean / (rows - 1))
        out[y, off:off + W] = m[y]
    return out


def numface(H, s, kind, name, sx=1.0):
    masks = {d: numeral(d, H * s, s, sx) for d in "0123456789"}
    gap = 2 * s if sx == 1.0 else (2 if s == 1 else 3)
    return Face(name, masks, {d: H * s for d in masks}, H * s, 4 * s, gap, kind, s)


# ---------------------------------------------------------------- icons (240 lines)
ICONS = {
    "times": ["##..##", ".####.", "..##..", ".####.", "##..##"],
    "round": ["####.##..", "####.###.", "####.####", "####.###.", "####.##.."],
    "frag": ["..##..", ".####.", "......", ".####.", "######", "######", "######", ".####."],
    "plasma": [".####.", "######", "######", "######", "######", ".####."],
    "cross": ["..##..", "..##..", "######", "######", "..##..", "..##.."],
}


# ---------------------------------------------------------------- weapon pickup icons
# none: the pickup prompt names the weapon in words (its short name where
# the long one does not fit, then wrapped at its words); Halo's pickup icons
# are its own art, which this design does not copy


# WIDESCREEN: the grenades drawn 5 pixels wide (a squeezed frag lost its
# fuse and read as an "i"; the plasma became a sliver)
ICONS_WS = {
    "frag": ["..#..", ".###.", ".....", ".###.", "#####", "#####", "#####", ".###."],
    "plasma": [".###.", "#####", "#####", "#####", "#####", ".###."],
}


class Set:
    def __init__(self, s, base=None, sx=1.0):
        self.s, self.sx = s, sx
        tag = self.tag = "240" if s == 1 else "480"
        sfx = self.sfx = "_ws" if sx != 1.0 else ""
        T = self.T = {}
        self.order = {}
        if s == 1 and sx == 1.0:
            t7 = face240(FONT7, "t7", "glow", 7, 3)
            self.faces = {"msg": t7, "n8": numface(8, 1, "numglow", "n8"), "n7": numface(7, 1, "numglow", "n7")}
            self.wide = {k: widen(f, f.name + "w") for k, f in self.faces.items()}
        elif s == 1:
            t7n = face240(FONT7N, "t7n", "glowfull", 7, 3)
            self.faces = {"msg": t7n, "n8": numface(8, 1, "glowfull", "n8n", sx), "n7": numface(7, 1, "glowfull", "n7n", sx)}
            # HIGH DETAIL (640x240): the condensed face at 2x across, so its
            # strokes are 2 pixels (no 1-pixel strokes where the display has
            # the pixels for 2), outlined like the 4:3 text
            self.wide = {k: widen(f, f.name + "w") for k, f in self.faces.items()}
        elif sx != 1.0:
            # 480i WIDESCREEN: large_ui squeezed to 75% by column removal,
            # strokes kept at >= 2 pixels (a 1-pixel line flickers in 480i)
            self.faces = {"msg": face_condensed(face_large14(), 0.72, "large14n", 2, 5),
                          "n8": numface(8, 2, "soft2", "n8n", sx), "n7": numface(7, 2, "soft2", "n7n", sx)}
            self.wide = None
        else:
            self.faces = {"msg": face_large14(), "n8": numface(8, 2, "soft2", "n8"), "n7": numface(7, 2, "soft2", "n7")}
            self.wide = None
        done = set()
        for group in (self.faces, self.wide or {}):
            for f in group.values():
                if f.name not in done:
                    done.add(f.name)
                    self.atlas(f.name, f.cells, f.fmt)
        cells = {}
        for k, rows in ICONS.items():
            hand = sx != 1.0 and k in ICONS_WS
            m = bits(ICONS_WS[k] if hand else rows)
            if s == 2:
                m = np.kron(m, np.ones((2, 2), bool)) if k in ("cross", "skull", "crown") else epx(m)
            if not hand:
                m = squeeze(m, sx)
            f = np.pad(m, ((0, s), (0, s)))
            sh = np.zeros_like(f)
            for oy in range(s + 1):
                for ox in range(s + 1):
                    if oy or ox:
                        sh |= shift(f, oy, ox)
            cells[k] = (f.astype(np.float32), (f | (sh & ~f)).astype(np.float32))
        I, A = cells["plasma"]
        h, w = I.shape
        yy, xx = np.mgrid[0:h, 0:w]
        r = np.hypot((yy - (h - 1) / 2) / h, (xx - (w - 1) / 2) / w)
        cells["plasma"] = (np.where(I > 0, np.clip(1.2 - 1.2 * r, 0.6, 1), 0), A)
        self.icons = cells
        self.atlas("icons", cells, "IA4")
        # the B button icon: a green disc, a dark B (9 rows)
        n = 9
        bw = int(round(n * sx))
        d = ellipse(bw / 2, n / 2, bw / 2, n / 2, bw, n) >= 0.5
        b = np.zeros_like(d)
        # WIDESCREEN: the same 5-pixel B, unsqueezed, in the 7-pixel
        # disc (a 4-pixel B's 1-pixel counters vanished: it read as "3")
        bm = bits(["####.", "##.##", "####.", "##.##", "####."])
        oy, ox = (n - bm.shape[0]) // 2, (bw - bm.shape[1] + 1) // 2
        b[oy:oy + bm.shape[0], ox:ox + bm.shape[1]] = bm
        if s == 2:
            d, b = epx(d), epx(b)
        fd, fb = np.pad(d, ((0, s), (0, s))), np.pad(b, ((0, s), (0, s)))
        sh = (shift(fd, s, s) | shift(fd, 0, s) | shift(fd, s, 0)) & ~fd
        T["button"] = Tex(f"button{sfx}_{tag}", "IA4", (fd & ~fb).astype(np.float32), fd | sh, tag, "B button icon")
        # panels and frames
        self.panels = {}
        for key, (w, h, right) in {"weapon": (58, 12, False), "grenade": (47, 12, False),
                                   "ammo4": (31, 9, False), "gren4": (40, 9, False)}.items():
            self.panel(key, w, h, right)
        self.plate()
        self.shield_full()
        self.shield_split()
        self.ticks()
        self.ticks4()
        self.bars()
        for k, r in (("ms", 21), ("mss", 14)):
            if base is None:
                self.sensor(k, r)
            else:                               # 480 lines: the 240 disc's size, doubled
                bw, bh, btop, _ = base.sensor_geo[k]
                self.sensor(k, r, (bw * s, bh * s))
                self.sensor_geo[k] = (bw * s, bh * s, btop * s, r)
        for k, args in RETICLE_ARGS[sx != 1.0].items():
            self.reticle(k, *args)
        bl = np.ones((3 * s, max(1, int(round(3 * s * sx)))), bool)
        fb = np.pad(bl, ((0, s), (0, s)))
        sh = (shift(fb, s, s) | shift(fb, 0, s) | shift(fb, s, 0)) & ~fb
        T["blip"] = Tex(f"blip{sfx}_{tag}", "IA4", fb, fb | sh, tag, "sensor blip")
        if base is not None:
            share = SHARED + tuple(k for k in base.T if k.startswith("weapons"))
            share += ("ms_fill", "ms_cone", "mss_fill", "mss_cone", "ret", "rets")
            # the reticle: Halo's 1-pixel ring at 240 lines, doubled (its arcs
            # and ticks keep their proportions in WIDESCREEN)
            self.ret_full = {k: np.kron(m, np.ones((s, s), bool)) for k, m in base.ret_full.items()}
            if sx != 1.0:
                # 480i WIDESCREEN: only text and numerals are drawn at 480
                # lines (2-pixel strokes); every graphic is the 240 WIDESCREEN
                # set at 2x (graphics drawn again at 480 lines would make this
                # set's file about twice as large: about 30 KB, not 16)
                share += ("ms_rim", "mss_rim", "button", "blip") + \
                         tuple(k for k in base.T if k.startswith("icons"))
                self.icons = {k: (np.kron(I, np.ones((s, s), np.float32)), np.kron(A, np.ones((s, s), np.float32)))
                              for k, (I, A) in base.icons.items()}
            for k in share:
                base.T[k].sets = "both"
                T[k] = Scaled(base.T[k], s)
            for k in SHARED_ORDERS:
                self.order[k] = np.kron(base.order[k], np.ones((s, s), np.float32))
            for k in self.panels:
                self.panels[k] = (T[k].w, T[k].h)
            self.shield_size = (T["shield_frame"].w, T["shield_frame"].h)
            self.shield_s_size = (T["shield_s_frame"].w, T["shield_s_frame"].h)

    def X(self, v):
        """a horizontal distance in 240-set units, at this set's scale and squeeze"""
        return int(round(v * self.s * self.sx))

    def atlas(self, name, cells, fmt):
        """cells packed in rows; the sheet width (a multiple of 16, at most
        128) that needs the fewest bytes, each sheet within TMEM's 4 KB"""
        tag = self.tag
        best = None
        for width in (128, 112, 96, 80, 64, 48, 32):
            if max(I.shape[1] for I, _ in cells.values()) > width:
                continue
            maxh = 4096 * 8 // (BPP[fmt] * width)
            sheets, x, y, rowh, cur = [], 0, 0, 0, []
            for ch, (I, A) in cells.items():
                h, w = I.shape
                if x + w > width:
                    x, y, rowh = 0, y + rowh, 0
                if y + h > maxh:
                    sheets.append(cur)
                    cur, x, y, rowh = [], 0, 0, 0
                cur.append((ch, x, y, I, A))
                x += w
                rowh = max(rowh, h)
            sheets.append(cur)
            area = sum(max(y + I.shape[0] for _, _, y, I, _ in sh) *
                       ((max(x + I.shape[1] for _, x, _, I, _ in sh) + 15) // 16 * 16) for sh in sheets)
            if best is None or (area, len(sheets)) < best[0]:
                best = ((area, len(sheets)), sheets)
        sheets = best[1]
        for i, sheet in enumerate(sheets):
            H = max(y + I.shape[0] for _, _, y, I, _ in sheet)
            W = (max(x + I.shape[1] for _, x, _, I, _ in sheet) + 15) // 16 * 16
            SI, SA = np.zeros((H, W), np.float32), np.zeros((H, W), np.float32)
            for ch, x, y, I, A in sheet:
                SI[y:y + I.shape[0], x:x + I.shape[1]] = I
                SA[y:y + I.shape[0], x:x + I.shape[1]] = A
            t = Tex(f"{name}{i}{self.sfx if not name.startswith(('t7n', 'n8n', 'n7n')) else ''}_{tag}", fmt, SI, SA,
                    tag, f"{name} atlas")
            self.T[f"{name}{i}"] = t
            for ch, x, y, I, A in sheet:
                cells[ch] = (t.I[y:y + I.shape[0], x:x + I.shape[1]], t.A[y:y + I.shape[0], x:x + I.shape[1]])

    def panel(self, key, w, h, lean_right=False):
        s, sx, tag = self.s, self.sx, self.tag
        lean = min(h, 12) / 3.0
        pts = [(0, 0), (w - lean, 0), (w, h), (lean, h)] if lean_right else [(lean, 0), (w, 0), (w - lean, h), (0, h)]
        m = poly(pts, w, h, s, sx)
        inner = erode(np.pad(m, s), s)[s:-s, s:-s] & m
        edge = m & ~inner
        yy = np.mgrid[0:m.shape[0], 0:m.shape[1]][0] / (m.shape[0] - 1)
        A = np.where(inner, PANEL_ALPHA[0] + (PANEL_ALPHA[1] - PANEL_ALPHA[0]) * yy, 0)
        self.T[key] = Tex(f"{key}_fill{self.sfx}_{tag}", "I4", A, None, tag, "panel fill (alpha)")
        self.T[key + "_edge"] = Tex(f"{key}_edge{self.sfx}_{tag}", "IA4", edge, edge, tag, "panel edge")
        self.panels[key] = (m.shape[1], m.shape[0])

    def plate(self):
        s, sx = self.s, self.sx
        w, h = 66, 13
        lean = 13 / 3.0
        m = poly([(lean, 0), (w, 0), (w - lean, h), (0, h)], w, h, s, sx)
        self.T["plate"] = Tex(f"plate{self.sfx}_{self.tag}", "I4", np.where(m, 0.50, 0), None, self.tag,
                              "round plate (alpha)")

    def frame(self, key, shape, edge, name, slot_alpha):
        A = np.where(edge, EDGE_ALPHA, np.where(shape, slot_alpha, 0))
        self.T[key] = Tex(f"{name}{self.sfx}_{self.tag}", "IA8", edge.astype(np.float32), A, self.tag, "meter frame")

    def shield_full(self):
        s, sx = self.s, self.sx
        w, h = 62, 16
        bar = poly([(7, 1), (54, 1), (61, 15), (49, 15), (45.5, 7), (9, 7)], w, h, s, sx)
        tri = poly([(0.5, 1.5), (5.5, 1.5), (5.5, 7.5)], w, h, s, sx)
        shape = bar | tri
        xx = np.mgrid[0:shape.shape[0], 0:shape.shape[1]][1] / shape.shape[1]
        self.order["shield"] = np.where(tri, 1 / 255, np.where(bar, np.clip(xx, 1 / 255, 1), 0)).astype(np.float32)
        self.frame("shield_frame", shape, dilate(shape, s) & ~shape, "shield_frame", 0.13)
        hm = np.zeros(shape.shape, bool)
        for i in range(8):
            x0 = 11 + i * 4
            hm |= poly([(x0, 9), (x0 + 3, 9), (x0 + 5, 15), (x0 + 2, 15)], w, h, s, sx)
        xi = np.mgrid[0:shape.shape[0], 0:shape.shape[1]][1] / (s * sx)
        self.order["health"] = np.where(hm, (np.clip((xi - 11) // 4, 0, 7) + 1) / 8.0, 0).astype(np.float32)
        self.frame("health_frame", hm, np.zeros_like(hm), "health_frame", 0.18)
        self.shield_size = (shape.shape[1], shape.shape[0])

    def shield_split(self):
        """cyborg_mp's thin bar with 10 flat health segments (4p)"""
        s, sx = self.s, self.sx
        w, h = 46, 10
        bar = poly([(4, 1), (40, 1), (45, 9), (37, 9), (35, 4), (5, 4)], w, h, s, sx)
        xx = np.mgrid[0:bar.shape[0], 0:bar.shape[1]][1] / bar.shape[1]
        self.order["shield_s"] = np.where(bar, np.clip(xx, 1 / 255, 1), 0).astype(np.float32)
        self.frame("shield_s_frame", bar, dilate(bar, s) & ~bar, "shields_frame", 0.13)
        hm = np.zeros(bar.shape, bool)
        order = np.zeros(bar.shape, np.float32)
        for i in range(10):
            x0 = 5 + 3 * i
            seg = poly([(x0, 6), (x0 + 2, 6), (x0 + 2, 8), (x0, 8)], w, h, s, sx)
            hm |= seg
            order[seg] = (i + 1) / 10
        self.order["health_s"] = order
        self.frame("health_s_frame", hm, np.zeros_like(hm), "healths_frame", 0.18)
        self.shield_s_size = (bar.shape[1], bar.shape[0])

    def ticks(self):
        """the 60 rounds under the weapon panel, 3 rows of 20 (Halo's grid).
        Each round an upright 2-pixel tick on a 3-pixel pitch, 3 rows tall,
        each row one pixel left of the row above, so the grid leans as Halo's
        does while every tick stays whole (ticks leaning within themselves
        come out as a zigzag at this size and blur into a grey band). WIDESCREEN:
        the same ticks, one for 2 rounds, 3 rows of 10"""
        s, sx = self.s, self.sx
        w, h = 66, 13
        W = int(round(w * s * sx))
        m = np.zeros((h * s, W), bool)
        order = np.zeros((h * s, W), np.float32)
        cols = 20 if sx == 1.0 else 10
        for row in range(3):
            for i in range(cols):
                x0, y0 = (4 - row + 3 * i) * s, (1 + row * 4) * s
                m[y0:y0 + 3 * s, x0:x0 + 2 * s] = True
                order[y0:y0 + 3 * s, x0:x0 + 2 * s] = (row * cols + i + 1) / (3 * cols)
        self.order["ticks"] = order
        self.T["ticks_slots"] = Tex(f"ticks_slots{self.sfx}_{self.tag}", "I4", np.where(m, 0.9, 0), None, self.tag,
                                    "spent-round slots (alpha)")

    def ticks4(self):
        """3-4 players: 2-pixel ticks on a 3-pixel pitch, one tick
        per 2 rounds (2 rows of 15; WIDESCREEN one per 3 rounds, 2 rows of
        10), at the 1p brightness (1-pixel dots blur into grey).
        6 rows, so the grid ends above the aim zone at HUD INSET 20"""
        s, sx = self.s, self.sx
        cols = 15 if sx == 1.0 else 10
        rows = 2
        pad_r = 1 if sx == 1.0 else 3          # WIDESCREEN: the plate 0.72 of the 4:3 one
        w = 1 + (rows - 1) + 3 * (cols - 1) + 2 + pad_r
        h = 6
        m = np.zeros((h * s, w * s), bool)
        order = np.zeros((h * s, w * s), np.float32)
        for row in range(rows):
            for i in range(cols):
                x0, y0 = (1 + (rows - 1 - row) + 3 * i) * s, (1 + 3 * row) * s
                m[y0:y0 + 2 * s, x0:x0 + 2 * s] = True
                order[y0:y0 + 2 * s, x0:x0 + 2 * s] = (row * cols + i + 1) / (rows * cols)
        lean = 1.0
        plate = poly([(lean, 0), (w, 0), (w - lean, h), (0, h)], w, h, s, 1.0) | m
        self.order["ticks4"] = order
        self.T["plate4"] = Tex(f"plate4{self.sfx}_{self.tag}", "I4", np.where(plate, 0.50, 0), None, self.tag,
                               "round plate, 3-4 players (alpha)")
        self.T["ticks4_slots"] = Tex(f"ticks4_slots{self.sfx}_{self.tag}", "I4", np.where(m, 0.9, 0), None, self.tag,
                                     "spent-round slots, 3-4 players (alpha)")

    def bars(self):
        s, sx = self.s, self.sx
        for key, w in (("bar", 13),):
            W = int(round(w * s * sx))
            m = np.ones((2 * s, W), bool)
            xx = np.mgrid[0:2 * s, 0:W][1] / W
            self.order[key] = np.clip(xx + 1 / 255, 0, 1).astype(np.float32)
            self.T[key + "_slot"] = Tex(f"{key}_slot{self.sfx}_{self.tag}", "I4", np.where(m, 0.3, 0), None, self.tag,
                                        "score bar slot (alpha)")

    def sensor(self, key, r, size=None):
        """a lit disc (Halo's tracker reads as a lit disc), its cone
        and marker; an ellipse 75% wide in WIDESCREEN. The fill and
        the cone are flat enough to share at 2x between the 240 and 480 sets;
        only the rim is drawn at 480 lines (1-bit IA4, drawn at 0.85)"""
        s, sx, tag = self.s, self.sx, self.tag
        n = 2 * r
        if size:
            W, H = size
        else:
            W, H = int(round(n * s * sx)), n * s
            W += W % 2
        inside = ellipse(W / 2, H / 2, W / 2 - 0.5 * s * sx, H / 2 - 0.5 * s, W, H) >= 0.5
        rim = inside & ~erode(np.pad(inside, 2 * s), 2 * s)[2 * s:-2 * s, 2 * s:-2 * s]
        qx, qy = W // 2, H // 2
        self.T[key + "_fill"] = Tex(f"{key}_fill{self.sfx}_{tag}", "I4", np.where(inside, SENSOR_FILL, 0)[:qy, :qx], None,
                                    tag, "sensor disc fill quarter (alpha; mirrored 4 ways)")
        self.T[key + "_rim"] = Tex(f"{key}_rim{self.sfx}_{tag}", "IA4", rim[:qy, :qx], rim[:qy, :qx], tag,
                                   "sensor rim quarter (mirrored 4 ways)")
        yy, xx = np.mgrid[0:H, 0:W]
        cy, cx = (H - 1) / 2, (W - 1) / 2
        ang = np.degrees(np.arctan2(np.abs(xx - cx) / sx, cy - yy))
        dist = np.hypot((xx - cx) / sx, yy - cy) / (r * s)
        cone = (ang < 33) & (yy <= cy) & (dist < 0.88)
        Ac = np.where(cone, np.clip(0.70 - 0.60 * dist, 0.10, 1), 0)
        mark = ellipse(W / 2, H / 2, max(2.5, r * 0.2) * s * sx, max(2.5, r * 0.2) * s, W, H) >= 0.5
        Ac = np.where(mark, 0.9, Ac)
        rows = np.nonzero((Ac > 0).any(axis=1))[0]
        top, bot = int(rows.min()), int(rows.max()) + 1
        self.T[key + "_cone"] = Tex(f"{key}_cone{self.sfx}_{tag}", "I4", Ac[top:bot, :qx], None, tag,
                                    "sensor cone + marker, left half (alpha)")
        self.__dict__.setdefault("sensor_geo", {})[key] = (W, H, top, r)

    def reticle(self, key, r, lw, tick, half=34.0):
        """Halo CE's reticle (hud_reticles 0): four arcs of +-34 degrees, broken
        at the diagonals, each with a 1-pixel tick pointing out (so that the
        ring reads as broken, not closed, on a TV). One odd-sized picture with the aim

        point on its centre pixel; r, lw, tick in 240-line pixels. Rasterised
        here (8x8 samples a pixel), so the four arcs are mirror images."""
        s, sx, tag = self.s, self.sx, self.tag
        ext = r + lw / 2 + tick
        H = 2 * int(np.ceil(ext * s)) + 1
        W = 2 * int(np.ceil(ext * s * sx)) + 1
        k = 8
        o = (np.arange(k) + 0.5) / k - 0.5
        ys = (np.arange(H)[:, None] + o[None, :] - H // 2).reshape(-1)
        xs = (np.arange(W)[:, None] + o[None, :] - W // 2).reshape(-1)
        Y, X = np.meshgrid(ys, xs / sx, indexing="ij")          # unsqueezed units (pixels at this scale)
        R = r * s
        d = np.hypot(X, Y)
        ang = np.degrees(np.arctan2(np.abs(Y), np.abs(X)))      # 0 = horizontal axis, 90 = vertical
        arc = (np.abs(d - R) <= lw * s / 2) & ((ang <= half) | (ang >= 90 - half))
        t0, t1 = R + lw * s / 2 - 0.25, R + lw * s / 2 + tick * s
        thin = 0.5 * max(1.0, s / 2.0)                            # 1 pixel at 240 and at 480
        tick_m = ((np.abs(Y) < thin) & (np.abs(X) >= t0) & (np.abs(X) <= t1)) | \
                 ((np.abs(X) < thin / sx * (sx if s == 1 else 1)) & (np.abs(Y) >= t0) & (np.abs(Y) <= t1))
        cov = (arc | tick_m).reshape(H, k, W, k).mean(axis=(1, 3))
        m = cov >= 0.4
        q = m[:H // 2 + 1, :W // 2 + 1]                         # a quarter with the centre row and column
        self.T[key] = Tex(f"{key}{self.sfx}_{tag}", "IA4", q, q, tag, "reticle quarter (Halo CE's broken ring)")
        self.__dict__.setdefault("ret_full", {})[key] = m


# reticle radius, line, tick (240-line pixels) and arc half-angle: Halo's
# +-34 degrees; the 1p ring 36. The WIDESCREEN arcs are chosen so each arc's
# box is 0.70-0.80 of its 4:3 width, found by a search over these four.
RETICLE_ARGS = {False: {"ret": (12.0, 1, 2.0, 36.0), "rets": (6.5, 1, 2.0, 34.0)},
                True: {"ret": (11.5, 1, 2.0, 38.0), "rets": (6.5, 1, 2.0, 34.0)}}

SHARED = ("weapon", "weapon_edge", "grenade", "grenade_edge", "ammo4", "ammo4_edge", "gren4", "gren4_edge",
          "plate", "ticks_slots", "plate4", "ticks4_slots", "shield_frame", "health_frame", "shield_s_frame",
          "health_s_frame", "bar_slot")
SHARED_ORDERS = ("ticks", "ticks4", "shield", "health", "shield_s", "health_s", "bar")


# ---------------------------------------------------------------- look
# one look in every mode (a lighter look at 640 wide made the numerals read
# hollow; Halo's own numerals are solid white on the plate)
LOOK_320 = {"panel": 1.0, "panel_rgb": BLUE, "num": NUMBER, "plate": PLATE, "plate_a": 1.0, "tick_on": TICK_ON,
            "tick_off": TICK_OFF}
# ---------------------------------------------------------------- drawing

SHORT_NAME = {"PISTOL": "PISTOL", "ROCKET LAUNCHER": "ROCKET", "ASSAULT RIFLE": "RIFLE", "SHOTGUN": "SHOTGUN",
              "SNIPER RIFLE": "SNIPER", "PLASMA PISTOL": "PLASMA PISTOL", "PLASMA RIFLE": "PLASMA RIFLE",
              "NEEDLER": "NEEDLER", "FLAMETHROWER": "FLAMER"}

GAP = 5                                   # 10 px between two players' HUDs



def build_sets():
    s1, ws = Set(1), Set(1, sx=WS)
    return {1: s1, 2: Set(2, s1), "ws": ws, "ws2": Set(2, ws, sx=WS)}

