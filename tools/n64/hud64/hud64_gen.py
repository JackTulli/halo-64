#!/usr/bin/env python3
"""HUD STYLE 64: the data the game draws the HUD with.

    python3 -B tools/n64/hud64/hud64_gen.py --fs assets --header port/n64/src/n64_hud64_data.h

It imports the design, hud64_design.py beside it (every texture, glyph and
meter shape, made by code: no image file is read), applies the changes
below to it (the overrides), and writes:

  --fs      <dir>/hud64/c<set><640x240><3-4 players>.bin: the textures in N64
            formats, a file a context, so that a game loads only what it
            draws (assets/hud64 in the repository; tools/n64_build.py copies
            them into the ROM's file system);
  --header  port/n64/src/n64_hud64_data.h: the C tables -- colours, faces and
            glyph cells, icon cells, each set's texture references, meters and
            the layout's distances in pixels (n64_hud64.c draws from them).

Both carry the same tables id (HUD64_TABLES_ID), and the game refuses a
texture file whose id is not its tables': write both from one run. A
different design can replace this one by writing the same tables; the C
code holds no position of its own. Needs numpy and Pillow (not the ROM
Builder: its output is in the repository).
"""
import argparse
import copy
import hashlib
import json
import struct
import sys
from pathlib import Path

import numpy as np

sys.dont_write_bytecode = True

# ---------------------------------------------------------------- the design


def mirror(order):
    """a meter lit from its right end instead of its left: each order value
    reflected within the meter's own range (its lowest and highest), so a
    value lights as many columns or segments as before, from the other end.
    The part lit at any value (order 1/255: the shield's triangle, which
    Halo's meter also lights at any shield) is kept"""
    fixed = order == np.float32(1 / 255)
    part = (order > 0) & ~fixed
    lo, hi = float(order[part].min()), float(order[part].max())
    return np.where(part, np.float32(lo + hi) - order, order).astype(np.float32)


def proportional(order):
    """the shield's meters lit in proportion to the value, from the right as
    Halo fills its shield meter, the fill edge parallel to the bar's right end: on each row the
    texel d places from the row's right end (the highlight's step texel included -- the bar's own outline) lights
    at (d + 0.5) / L, the rightmost at 1/255 (any value above 0); L, one length for every row, is the body's
    middle row's. Every row lights the same count from its right end, so the fill edge steps as the bar's right
    end does -- no lone texel where rows differ in length; a row shorter than L is whole before 100%, one longer
    lights its last texels at 100%. The part lit at any value (1/255: the triangle) is kept. (The design's own
    ramp, mirrored, or column_ramp would light about 0.125 to 0.965 of the value -- an empty bar under 12.5%, and
    less than the value up to the middle.)"""
    fixed = order == np.float32(1 / 255)
    whole = order > 0
    above = np.zeros_like(whole)
    above[1:] = whole[:-1]
    hl = whole & ~above                       # (the generator's highlight: shape & ~shift(shape, 1, 0))
    shape = whole & ~fixed
    rows = [r for r in range(shape.shape[0]) if shape[r].any()]
    if not rows:
        return order
    own = {r: int((shape[r] & ~hl[r]).sum()) for r in rows}
    longest = max(own.values())
    body = [r for r in rows if own[r] >= 0.6 * longest]
    L = float(shape[body[len(body) // 2]].sum())
    out = np.zeros(order.shape, np.float32)
    for r in rows:
        for d, x in enumerate(np.nonzero(shape[r])[0][::-1]):
            out[r, x] = 1 / 255 if d == 0 else min((d + 0.5) / L, 1.0)
    return np.where(fixed, np.float32(1 / 255), out).astype(np.float32)


# the vitals' 3-tone ramp on the shield meter (a tint row, the body, shade rows): per order
# map (texels; a canvas pixel at 640x240, a pixel at 480 lines)
RAMP_MIN_W = 4          # (a foot row's right-edge shade shows from this lit width in the level's colour)


# the vitals' health pips: a framed gauge of upright cells parted by 1-pixel notches,
# Halo's count (8 cells at 1-2 players, 10 at 3-4), in the shield panel's own coordinates (its origin, set pixels):
# (first cell's left column, cells, cell width, rows). WIDESCREEN 3-4 players reaches 6 columns left of the panel's
# origin (Halo's 10 cells need the room there): its textures start there (offset -6).
GAUGE = {("s240", 0): (12, 8, 3, (10, 11, 12, 13, 14)), ("s240ws", 0): (10, 8, 2, (10, 11, 12, 13, 14)),
         ("s240", 1): (6, 10, 2, (6, 7, 8)), ("s240ws", 1): (-5, 10, 2, (6, 7, 8))}
PIP_EMPTY = (0x00, 0x14, 0x30)          # (the empty slots' dark blue, drawn at half alpha under the cells)


def apply_gauge(make, sets):
    """the health meter, its frame and its slots replaced by the gauge in the design's 240-line sets (their 480-line
    doubles share them): each cell's texels at order (j + 1) / 255, j counted from the right -- the module lights
    exactly Halo's count (n64_hud64.c shield); the frame (outline and notches) an IA8 texture in the outline at the
    edge alpha; the slots an I4 mask. Returns {(set key, split): columns left of the origin}"""
    pads = {}
    for k240, k480, sk in ((1, 2, "s240"), ("ws", "ws2", "s240ws")):
        S, S2 = sets[k240], sets[k480]
        for split in (0, 1):
            hk, fk, slk = ("health_s", "health_s_frame", "health_s_slots") if split else \
                ("health", "health_frame", "health_slots")
            x0, n, w, rows = GAUGE[(sk, split)]
            H, W = S.order[hk].shape
            pad = max(0, 1 - x0)
            W2 = W + pad
            order = np.zeros((H, W2), np.float32)
            slots = np.zeros((H, W2), bool)
            frame = np.zeros((H, W2), bool)
            for i in range(n):
                c0 = x0 + pad + (w + 1) * i
                for r in rows:
                    order[r, c0:c0 + w] = (n - i) / 255.0
                    slots[r, c0:c0 + w] = True
            x1 = x0 + pad + (w + 1) * n - 1
            frame[rows[0] - 1, x0 + pad - 1:x1 + 1] = True
            frame[rows[-1] + 1, x0 + pad - 1:x1 + 1] = True
            for i in range(n + 1):
                frame[list(rows), x0 + pad - 1 + (w + 1) * i] = True
            S.order[hk] = order
            tag = f"{S.sfx}_{S.tag}"
            S.T[fk] = make.Tex(f"gauge{'s' if split else ''}_frame{tag}", "IA8", frame.astype(np.float32),
                               np.where(frame, make.EDGE_ALPHA, 0.0), S.tag, "the pips' gauge")
            S.T[slk] = make.Tex(f"gauge{'s' if split else ''}_slots{tag}", "I4", slots.astype(np.float32), None, S.tag,
                                "the pips' empty slots")
            S2.T[fk] = make.Scaled(S.T[fk], 2)
            S2.T[slk] = make.Scaled(S.T[slk], 2)
            pads[(sk, split)] = pad
    return pads


def gauge_ramp(order):
    """the cells' tint (their top row where a cell has 4 rows or more) and shade (their bottom row where it has 3
    or more) -- a cell too short for both keeps the shade; the 480-line doubles 2 pixels"""
    cells = order > 0
    rows = np.nonzero(cells.any(axis=1))[0]
    tint = np.zeros_like(cells)
    shade = np.zeros_like(cells)
    if len(rows) >= 4:
        tint[rows[0]] = cells[rows[0]]
    if len(rows) >= 3:
        shade[rows[-1]] = cells[rows[-1]]
    return tint, shade

def shield_ramp(order, hl, rows_px=1, decide=None):
    """(tint, static shade, diagonal shade, the diagonal's thresholds as order values, the rules kept) for one shield
    order map. The top run (the body's long rows, k of them) and the foot (the rows under it):
      tint    the top run's first row (its first 2 at 480 lines: rows_px 2) -- where k >= 4 (as the cells' tint);
      shade   the top run's last row(s) where the foot is not under them (k >= 4, or k = 3); the foot's last row(s);
              the foot's right diagonal inside (each foot row's rightmost texel of the colour, and the pixels a
              2-pixel step leaves between two rows: an unbroken line) -- the diagonal dropped first, then the foot's
              last rows, while the foot would keep under 2/3 pure colour;
      the diagonal's texels shade only once their row's lit width in the colour (the outline's step texels at the
      right end not counted) reaches RAMP_MIN_W, or the line's pixels in the row plus 2: below that, base colour.
    decide: a 240-line face's rules for its 480-line double (the same parts, rows_px 2)."""
    shape = order > 0
    own = shape & ~hl
    rows = [r for r in range(own.shape[0]) if own[r].any()]
    cnt = {r: int(own[r].sum()) for r in rows}
    top = max(cnt.values())
    run = [r for r in rows if cnt[r] >= 0.6 * top]
    foot = [r for r in rows if r > max(run)]
    k = len(run)
    rule = dict(decide) if decide else {"tint": k >= 4, "run_shade": k >= 4 or k == 3}
    tint = np.zeros_like(own)
    static = np.zeros_like(own)
    if rule["tint"]:
        for r in run[:rows_px]:
            tint[r] = own[r]
    if rule["run_shade"]:
        last = run[-1]
        under = shape[last + 1] if last + 1 < own.shape[0] else np.zeros(own.shape[1], bool)
        for r in run[-rows_px:]:
            static[r] = own[r] & ~under
    diag = np.zeros_like(own)
    if foot:
        F = np.zeros_like(own)
        for r in foot:
            F[r] = own[r]
        bottom = np.zeros_like(own)
        for r in foot[-rows_px:]:
            bottom[r] = own[r]
        dg = np.zeros_like(own)
        prev = None
        for r in foot:
            if bottom[r].any():
                break
            x = int(np.nonzero(own[r])[0][-1])
            dg[r, x] = True
            if prev is not None and x - prev >= 2:
                dg[r, prev + 1:x] = own[r, prev + 1:x]
            prev = x
        if decide is None:
            n = F.sum()
            use_d = 1 - (bottom | dg).sum() / n >= 2 / 3
            use_b = use_d or 1 - bottom.sum() / n >= 2 / 3
            rule.update({"foot_bottom": bool(use_b), "foot_diag": bool(use_d)})
        if rule.get("foot_bottom"):
            static |= bottom
        if rule.get("foot_diag"):
            diag = dg & ~static
    # (the diagonal's later order: that of the texel making its row's colour width RAMP_MIN_W, or its line + 2)
    delayed = np.zeros(order.shape, np.float32)
    for r in foot:
        if not diag[r].any():
            continue
        xs = np.nonzero(shape[r])[0][::-1]
        step = 0
        while step < len(xs) and hl[r, xs[step]]:
            step += 1
        need = step + max(RAMP_MIN_W, int(diag[r].sum()) + 2)
        o_need = order[r, xs[need - 1]] if len(xs) >= need else 1.0
        for x in np.nonzero(diag[r])[0]:
            delayed[r, x] = max(order[r, x], o_need)
    return tint, static, diag, delayed, rule


# ---------------------------------------------------------------- overrides: text, grids, segments
# Changes to the design made here (hud64_design.py stays as it is):
# - the kill readout's parentheses ("YOU KILLED BLOOBER (1)"): ( and ) in
#   the 240-line faces (Halo's large_ui has them for 480i);
# - one tick a round at every player count and in WIDESCREEN, as ORIGINAL
#   (1-2 players 3 rows of 20, WIDESCREEN 4 of 15; 3-4 players 4 rows of 15,
#   WIDESCREEN 6 of 10), not a tick for 2 or 3 rounds;
# - the plate under the ticks as far past the last tick as before the
#   first: a pixel round every row of ticks, at every size;
# - every health segment the same width in whole pixels: WIDESCREEN 3-4
#   players' 10 segments 1 pixel wide a pixel apart (a 0.75 squeeze of the
#   4:3 segments gives runs of 1 and 2 that merge).
GRID = {  # (players' arrangement, WIDESCREEN): rows, columns, tick width, height, pitch across, down
    ("full", False): (3, 20, 2, 3, 3, 4), ("full", True): (4, 15, 2, 3, 3, 4),
    ("split", False): (4, 15, 2, 2, 3, 3), ("split", True): (6, 10, 2, 2, 3, 3)}
PLATE_MARGIN = 1  # texels round the ticks, every side


def grid_textures(m, S, arr):
    """a round grid at the 240-line scale: the order map (round j of n at
    j / n, rows first), the spent-round slots and the plate under them --
    a step left each row, as the ticks lean, PLATE_MARGIN round every row
    of ticks"""
    rows, cols, tw, th, px, py = GRID[(arr, S.sx != 1.0)]
    M = PLATE_MARGIN
    lean = rows - 1                              # each row a pixel left of the row above
    x00 = M + lean                               # the first row's first tick
    W = x00 + (cols - 1) * px + tw + M
    H = M + (rows - 1) * py + th + M
    order = np.zeros((H, W), np.float32)
    ticks = np.zeros((H, W), bool)
    plate = np.zeros((H, W), bool)
    for r in range(rows):
        y0 = M + r * py
        xr = x00 - r
        for i in range(cols):
            ticks[y0:y0 + th, xr + i * px:xr + i * px + tw] = True
            order[y0:y0 + th, xr + i * px:xr + i * px + tw] = (r * cols + i + 1) / (rows * cols)
        # (the row's band of the plate: the gap under a row's ticks is its own)
        b0 = 0 if r == 0 else M + (r - 1) * py + th + (py - th + 1) // 2
        b1 = H if r == rows - 1 else M + r * py + th + (py - th + 1) // 2
        plate[b0:b1, xr - M:xr + (cols - 1) * px + tw + M] = True
    return order, ticks, plate


def text_and_grid_overrides(m):
    m.FONT7["("] = [".##", "##.", "##.", "##.", "##.", "##.", ".##"]
    m.FONT7[")"] = ["##.", ".##", ".##", ".##", ".##", ".##", "##."]
    m.FONT7N["("] = [".#", "#.", "#.", "#.", "#.", "#.", ".#"]
    m.FONT7N[")"] = ["#.", ".#", ".#", ".#", ".#", ".#", "#."]
    if "(" not in m.CHARS:
        m.CHARS += "()"

    def plate(self):
        return  # (the plate comes with its grid: ticks)

    def ticks(self):
        order, tk, pl = grid_textures(m, self, "full")
        self.order["ticks"] = order
        self.T["ticks_slots"] = m.Tex(f"ticks_slots{self.sfx}_{self.tag}", "I4", np.where(tk, 0.9, 0), None, self.tag,
                                      "spent-round slots (alpha)")
        self.T["plate"] = m.Tex(f"plate{self.sfx}_{self.tag}", "I4", np.where(pl, 0.50, 0), None, self.tag,
                                "round plate (alpha)")

    def ticks4(self):
        order, tk, pl = grid_textures(m, self, "split")
        self.order["ticks4"] = order
        self.T["plate4"] = m.Tex(f"plate4{self.sfx}_{self.tag}", "I4", np.where(pl, 0.50, 0), None, self.tag,
                                 "round plate, 3-4 players (alpha)")
        self.T["ticks4_slots"] = m.Tex(f"ticks4_slots{self.sfx}_{self.tag}", "I4", np.where(tk, 0.9, 0), None,
                                       self.tag, "spent-round slots, 3-4 players (alpha)")

    shield_split0 = m.Set.shield_split

    def shield_split(self):
        shield_split0(self)
        if self.sx == 1.0 or self.s != 1:
            return
        # WIDESCREEN, 240 lines: the 10 health segments 1 pixel wide, a pixel
        # apart, where the design's segments are (the 480i set doubles them)
        old = self.order["health_s"] > 0
        ys, xs = np.nonzero(old)
        hm = np.zeros(old.shape, bool)
        order = np.zeros(old.shape, np.float32)
        for i in range(10):
            x = int(xs.min()) + 2 * i
            hm[ys.min():ys.max() + 1, x] = True
            order[ys.min():ys.max() + 1, x] = (i + 1) / 10
        self.order["health_s"] = order
        self.frame("health_s_frame", hm, np.zeros_like(hm), "healths_frame", 0.18)

    m.Set.plate, m.Set.ticks, m.Set.ticks4, m.Set.shield_split = plate, ticks, ticks4, shield_split


# A weapon of unlimited
# ammunition (a vehicle's: Halo spends rounds and battery only of a weapon a
# player holds) shows a lemniscate in its count's place: "~" in the numeral
# faces. Drawn by hand for each face, so it cannot read as "00": wider than
# tall, about two thirds of the numerals' height, round ends, the loops'
# inner sides diagonal and crossing in a single pixel. Upright (a numeral's
# lean shears the crossing apart). The 640-wide faces double the 240-line
# glyph across, as they double the numerals (make.widen).
INFINITY = "~"
INFINITY_ART = {
    # 240 lines, 4:3 (n8, n7): the ends 2 pixels across, as the numerals' strokes
    (1, False): ["..###...###..",
                 ".#...#.#...#.",
                 "##....#....##",
                 ".#...#.#...#.",
                 "..###...###.."],
    # 240 lines, WIDESCREEN (n8n, n7n): 1-pixel strokes, the ends 2
    (1, True): ["..##...##..",
                ".#..#.#..#.",
                "##...#...##",
                ".#..#.#..#.",
                "..##...##.."],
    # 480i, 4:3: 2-pixel strokes; 1 player (16 rows), 3-4 players (14 rows). An even
    # height: the crossing a pixel across and 2 rows down (no part of a 480i
    # glyph 1 row: it would flicker between the fields)
    (2, False, 16): ["....#####.....#####....",
                     "..#######.....#######..",
                     ".##.....##...##.....##.",
                     "##.......##.##.......##",
                     "##.........#.........##",
                     "##.........#.........##",
                     "##.......##.##.......##",
                     ".##.....##...##.....##.",
                     "..#######.....#######..",
                     "....#####.....#####...."],
    (2, False, 14): ["...#####.....#####...",
                     ".#######.....#######.",
                     "##.....##...##.....##",
                     "##......##.##......##",
                     "##........#........##",
                     "##........#........##",
                     "##......##.##......##",
                     "##.....##...##.....##",
                     ".#######.....#######.",
                     "...#####.....#####..."],
    # 480i, WIDESCREEN (condensed faces)
    (2, True, 16): ["...####.....####...",
                    ".######.....######.",
                    "##....##...##....##",
                    "##.....##.##.....##",
                    "##.......#.......##",
                    "##.......#.......##",
                    "##.....##.##.....##",
                    "##....##...##....##",
                    ".######.....######.",
                    "...####.....####..."],
    (2, True, 14): ["..####.....####..",
                    ".#####.....#####.",
                    "##...##...##...##",
                    "##....##.##....##",
                    "##......#......##",
                    "##......#......##",
                    "##....##.##....##",
                    "##...##...##...##",
                    ".#####.....#####.",
                    "..####.....####.."],
}


def infinity_mask(m, H, s, sx):
    """the face's lemniscate in a cell the numerals' height (H rows), centred down"""
    art = INFINITY_ART.get((s, sx != 1.0)) or INFINITY_ART[(s, sx != 1.0, H)]
    g = np.array([[c == "#" for c in r] for r in art], bool)
    assert len({len(r) for r in art}) == 1 and g.shape[0] < H
    out = np.zeros((H, g.shape[1]), bool)
    top = (H - g.shape[0]) // 2
    out[top:top + g.shape[0]] = g
    return out


# A battery's count is its percent, so it carries the unit: "%" after a
# battery weapon's count in the numeral faces, drawn by hand
# for each face, the digits' height: two rings and a slash between them, apart
# (three pieces, the rings' holes open), leaned as the numerals lean. The
# 640-wide faces double the 240-line glyph across, as for the numerals.
PERCENT = "%"
PERCENT_ART = {
    # 240 lines, 4:3 (n8, n7): two 3x3 rings and a 1-pixel slash, leaned as the numerals
    (1, False, 8): ["..###....#",
                    "..#.#...#.",
                    ".###..##..",
                    ".....#....",
                    "....#.....",
                    "...#..###.",
                    ".##..#.#..",
                    "#....###.."],
    (1, False, 7): ["..###.....#",
                    "..#.#...##.",
                    ".###..##...",
                    ".....#.....",
                    "...##..###.",
                    ".##...#.#..",
                    "#.....###.."],
    # 240 lines, WIDESCREEN (n8n, n7n): 1-pixel strokes
    (1, True, 8): [".###....#",
                   ".#.#...#.",
                   ".###..#..",
                   ".....#...",
                   "....#....",
                   "...#..###",
                   "..#...#.#",
                   ".#....###"],
    (1, True, 7): ["###.....#",
                   "#.#....#.",
                   "###..##..",
                   "....#....",
                   "...#..###",
                   ".##...#.#",
                   "#.....###"],
    # 480i: 6x6 rings and a slash 2 pixels across (4:3, then WIDESCREEN)
    (2, False, 16): [".....####...........##",
                     "....######.........##.",
                     "...##..##........##...",
                     "...##..##.......##....",
                     "...######......##.....",
                     "....####.....##.......",
                     "............##........",
                     "...........##.........",
                     ".........##...........",
                     "........##............",
                     ".......##.....####....",
                     ".....##......######...",
                     "....##.......##..##...",
                     "...##........##..##...",
                     ".##.........######....",
                     "##...........####....."],
    (2, False, 14): [".....####...........##",
                     "....######........##..",
                     "...##..##........##...",
                     "...##..##......##.....",
                     "...######.....##......",
                     "....####....##........",
                     "...........##.........",
                     ".........##...........",
                     "........##....####....",
                     "......##.....######...",
                     ".....##......##..##...",
                     "...##........##..##...",
                     "..##........######....",
                     "##...........####....."],
    (2, True, 16): ["....####........##",
                    "...######......##.",
                    "...##..##.....##..",
                    "...##..##....##...",
                    "..######....##....",
                    "...####....##.....",
                    "..........##......",
                    ".........##.......",
                    ".......##.........",
                    "......##..........",
                    ".....##....####...",
                    "....##....######..",
                    "...##....##..##...",
                    "..##.....##..##...",
                    ".##......######...",
                    "##........####...."],
    (2, True, 14): ["....####........##",
                    "...######......##.",
                    "..##..##......##..",
                    "..##..##....##....",
                    "..######...##.....",
                    "...####...##......",
                    ".........##.......",
                    ".......##.........",
                    "......##...####...",
                    ".....##...######..",
                    "....##...##..##...",
                    "..##.....##..##...",
                    ".##......######...",
                    "##........####...."],
}


def percent_mask(H, s, sx):
    """the face's percent sign: the numerals' height (H rows)"""
    art = PERCENT_ART[(s, sx != 1.0, H)]
    g = np.array([[c == "#" for c in r] for r in art], bool)
    assert g.shape[0] == H, (s, sx, H, g.shape)
    return g


# A vehicle's secondary function in the grenade slot shows its button's letter and its recharge as a fill: a
# charge bolt for a secondary that recharges (its trigger draws from a magazine: the Banshee's fuel rod) --
# "*" in the numeral faces, drawn by hand as the lemniscate: a rod with a rounded nose and a broken trail,
# about two thirds of the numerals' height. The 640-wide faces double it across (make.widen); the 480-line
# faces take it doubled both ways (2-pixel strokes, no part 1 row).
CHARGE = "*"
CHARGE_ART = {
    (1, False): ["....######.",
                 "#..########",
                 "##.########",
                 "#..########",
                 "....######."],
    (1, True): ["...#####.",
                "#.#######",
                "#.#######",
                "#.#######",
                "...#####."],
}


def charge_mask(H, s, sx):
    """the face's charge bolt in a cell the numerals' height (H rows), centred down"""
    g = np.array([[c == "#" for c in r] for r in CHARGE_ART[(1, sx != 1.0)]], bool)
    if s > 1:
        g = np.kron(g, np.ones((s, s), bool))
    assert g.shape[0] < H
    out = np.zeros((H, g.shape[1]), bool)
    top = (H - g.shape[0]) // 2
    if s > 1:
        top -= top % 2          # (480 lines: on an even row, as the strokes)
    out[top:top + g.shape[0]] = g
    return out


def numeral_glyph_overrides(m):
    numface0 = m.numface

    def numface(H, s, kind, name, sx=1.0):
        f = numface0(H, s, kind, name, sx)
        mask = infinity_mask(m, H * s, s, sx)
        f.masks[INFINITY] = mask
        f.oy[INFINITY] = H * s
        I, A, pad = m.treat(mask, kind)
        f.cells[INFINITY] = (I, A)
        pm = percent_mask(H * s, s, sx)
        f.masks[PERCENT] = pm
        f.oy[PERCENT] = H * s
        I, A, pad = m.treat(pm, kind)
        f.cells[PERCENT] = (I, A)
        cm = charge_mask(H * s, s, sx)
        f.masks[CHARGE] = cm
        f.oy[CHARGE] = H * s
        I, A, pad = m.treat(cm, kind)
        f.cells[CHARGE] = (I, A)
        return f
    m.numface = numface


# The tracker's range label a size step smaller than the messages' face, so
# that it stays off the tracker's circle -- a 5-row face (10 rows at 480i: the
# 240-line glyphs doubled, strokes 2 pixels) of the digits and "m", in the
# numerals' treatment at 320 (WIDESCREEN: their 1-pixel strokes' full
# haze), outlined at 640 (doubled across, as the other faces)
LABEL_ART = {
    "0": ["###", "#.#", "#.#", "#.#", "###"], "1": [".#.", "##.", ".#.", ".#.", "###"],
    "2": ["###", "..#", "###", "#..", "###"], "3": ["###", "..#", ".##", "..#", "###"],
    "4": ["#.#", "#.#", "###", "..#", "..#"], "5": ["###", "#..", "###", "..#", "###"],
    "6": ["###", "#..", "###", "#.#", "###"], "7": ["###", "..#", "..#", ".#.", ".#."],
    "8": ["###", "#.#", "###", "#.#", "###"], "9": ["###", "#.#", "###", "..#", "###"],
    "m": [".....", ".....", "##.#.", "#.#.#", "#.#.#"],
}
LABEL_ROWS = 5


def label_face(m, S):
    """the range label's face of a set"""
    s, ws = S.s, S.sx != 1.0
    masks = {}
    for ch, rows in LABEL_ART.items():
        g = np.array([[c == "#" for c in r] for r in rows], bool)
        masks[ch] = np.kron(g, np.ones((s, s), bool)) if s > 1 else g
    kind = "soft2" if s > 1 else "glowfull" if ws else "numglow"
    cap = LABEL_ROWS * s
    return m.Face("lbln" if ws else "lbl", masks, {ch: cap for ch in masks}, cap, 3 * s, s, kind, s)


# "100%" at 3-4 players: the 3-4p ammunition
# panel a character wider for every weapon, so "100%" ends the margin after it
# that the 1-2p panel's last glyph has ("240 x [round]"); the grenade panel
# beside it moves right as much, once. The panel's middle columns repeated,
# its ends kept (the inverse of the lemniscate's shortened panel). In the
# 240-line sets' pixels (the 480i sets draw them doubled)
AMMO4_WIDER = {False: 9, True: 9}       # 4:3, WIDESCREEN
# The grenade panel's spacing relaxed a little: its gaps wider -- frag icon
# to count (a), frag count to plasma icon (b), plasma icon to count (c) --
# in 240-set units, per arrangement
# (full: 1-2 players; split: 3-4) and WIDESCREEN; the grenade panel as many
# pixels wider (its middle columns repeated), so the plasma count keeps its
# margin. Sized so no icon and count touch
GREN_EXTRA = {("full", False): (1, 0, 0), ("full", True): (2, 0, 0),
              ("split", False): (2, 1, 0), ("split", True): (3, 2, 0)}
# At 3-4 players "100%" would come close to the frag icon: the ammunition text
# to the frag icon a unit wider at 3-4 players, 4:3 and WIDESCREEN -- the frag
# icon a unit further into its panel, the panel as much wider
GREN_LEAD = {"full": 0, "split": 1}
# ... unless that brings the grenade panel closer to the shield panel than
# the panels' gap (1 pixel at 240 lines, 640 across drawn doubled; 2 at

# 480): at 3-4 players a view narrower than split_gren_room (HUD INSET 20,
# 4:3) draws the spacing without that unit instead -- the frag icon at
# split_frag_dx_e, its panel gren4_e (a unit narrower).
# The range label stays at Halo's place for it, clear of the motion tracker's
# circle (the style's text must not lie over the circle): the range
# label's text where Halo's small label is, then moved the shortest way down
# or left off Halo's tracker circle (n64_hud64.c range_label). The circle, the
# plate's edge as drawn, from the tracker box's unrounded edges: its centre the
# box's middle plus (cx, cy), its radii half the box plus (gx, gy) -- the plate
# fitted in ORIGINAL's layers of every 1-2p view (320x240,
# 640x240, 640x480, 512x480; 4:3, WIDESCREEN; HUD INSET 0, 10, 19, 20) and
# half a pixel wider, the mean per set, player count and frame, so
# the program's place stays within a pixel of the shortest move measured there
# (the stop condition of that search). Per set: 1
# player variants 0-2, then 2 players -- variant 0 the full frame, 1 the
# 640x240 canvas (240-line sets) or a narrow frame (480-line sets: ULTRA's
# 512x480), 2 a narrow WIDESCREEN frame; each (cx, cy, gx, gy) in eighths of a
# canvas pixel
LBL_INK_LEN = 2 + 2 * 16
TRK_PLATE = {
    "s240": ((0, -6, 7, 5), (4, -5, 6, 6), (4, -5, 6, 6), (-1, -7, 9, 7), (0, -5, 10, 9), (0, -5, 10, 9)),
    "s240ws": ((1, -7, 6, 7), (3, -7, 7, 7), (3, -7, 7, 7), (-1, -10, 9, 9), (0, -6, 10, 9), (0, -6, 10, 9)),
    "s480": ((4, -7, 6, 8), (4, -7, 6, 8), (4, -7, 6, 8), (0, -7, 10, 14), (0, -7, 10, 14), (0, -7, 10, 14)),
    "s480ws": ((3, -11, 7, 10), (-2, -10, 9, 11), (-1, -11, 7, 11), (0, -11, 9, 15), (-1, -12, 14, 14), (-1, -12, 9, 14))}


def gren_extra(arr, s, sx, lead=True):
    """(a, b, c) and the panel's growth (with GREN_LEAD's, or without it: gren4_e), in this set's pixels"""
    a, b, c = GREN_EXTRA[(arr, sx != 1.0)]
    X = lambda v: int(round(v * s * sx))
    base = {"full": (8, 4, 12), "split": (7, 2, 9)}[arr]
    frag = {"full": 6, "split": 4}[arr]
    df = X(frag + GREN_LEAD[arr]) - X(frag) if lead else 0
    da, db, dc = X(base[0] + a) - X(base[0]), X(base[1] + b) - X(base[1]), X(base[2] + b + c) - X(base[2])
    return da, db, dc, df + da + dc


def widen_panel(t, extra):
    """a panel texture extra columns wider: its middle column repeated"""
    mid = t.w // 2
    t.I = np.concatenate([t.I[:, :mid], np.repeat(t.I[:, mid:mid + 1], extra, axis=1), t.I[:, mid:]], axis=1)
    t.A = np.concatenate([t.A[:, :mid], np.repeat(t.A[:, mid:mid + 1], extra, axis=1), t.A[:, mid:]], axis=1)
    t.h, t.w = t.I.shape


def label_and_panel_overrides(m):
    init0 = m.Set.__init__

    def init(self, s, base=None, sx=1.0):
        init0(self, s, base, sx)
        if s == 1:
            extra = AMMO4_WIDER[sx != 1.0]
            widen_panel(self.T["ammo4"], extra)
            widen_panel(self.T["ammo4_edge"], extra)
            self.panels["ammo4"] = (self.T["ammo4"].w, self.T["ammo4"].h)
            # (the 3-4p panel without GREN_LEAD's unit, for the narrow views: a copy before the widening)
            for k in ("gren4", "gren4_edge"):
                t = copy.copy(self.T[k])
                t.name = t.name.replace("gren4", "gren4e", 1)
                self.T[k.replace("gren4", "gren4_e")] = t
            grow = gren_extra("split", s, sx, lead=False)[3]
            for k in ("gren4_e", "gren4_e_edge"):
                widen_panel(self.T[k], grow)
            self.panels["gren4_e"] = (self.T["gren4_e"].w, self.T["gren4_e"].h)
            for arr, key in (("full", "grenade"), ("split", "gren4")):
                grow = gren_extra(arr, s, sx)[3]
                if grow:
                    widen_panel(self.T[key], grow)
                    widen_panel(self.T[key + "_edge"], grow)
                    self.panels[key] = (self.T[key].w, self.T[key].h)
        elif base is not None:
            for k in ("gren4_e", "gren4_e_edge"):
                base.T[k].sets = "both"
                self.T[k] = m.Scaled(base.T[k], s)
            self.panels["gren4_e"] = (self.T["gren4_e"].w, self.T["gren4_e"].h)
        f = label_face(m, self)
        self.faces["lbl"] = f
        self.atlas(f.name, f.cells, f.fmt)
        if self.wide is not None:
            w = m.widen(f, f.name + "w")
            self.wide["lbl"] = w
            self.atlas(w.name, w.cells, w.fmt)
    m.Set.__init__ = init


# the vitals' shield badge (one solid pointed shield): per set, its fill's rows
# {row: (left, right)} in the shield panel's coordinates and its outline's width; the outline the fill grown that
# many pixels (square), the crest its top row. It takes the place of the design's triangle left of the bar.
BADGE = {"s240": ({1: (-3, 3), 2: (-3, 3), 3: (-3, 3), 4: (-2, 2), 5: (-1, 1), 6: (0, 0)}, 1),
         "s240ws": ({1: (-3, 1), 2: (-3, 1), 3: (-3, 1), 4: (-2, 0), 5: (-2, 0), 6: (-1, -1)}, 1),
         "s480": ({**{r: (-6, 7) for r in range(2, 8)}, 8: (-5, 6), 9: (-4, 5), 10: (-3, 4), 11: (-2, 3), 12: (-1, 2),
                   13: (0, 1)}, 2),
         "s480ws": ({**{r: (-6, 3) for r in range(2, 8)}, 8: (-5, 2), 9: (-5, 2), 10: (-4, 1), 11: (-4, 1),
                     12: (-3, 0), 13: (-2, -1)}, 2)}
BADGE_CREST = 0.25      # (the crest: the fill colour a quarter of the way to white)
BADGE_SLOT_ALPHA = 0.13  # (the unlit badge: the bar's slot alpha)


def badge_masks(skey):
    """(the badge's left column in the panel's coordinates, outline, fill, crest) as bool arrays from row 0"""
    spec, s = BADGE[skey]
    x0, H = -12, max(spec) + s + 1
    W = 32
    fill = np.zeros((H, W), bool)
    for r, (l, rr) in spec.items():
        fill[r, l - x0:rr - x0 + 1] = True
    grown = fill.copy()
    for dy in range(-s, s + 1):
        for dx in range(-s, s + 1):
            grown |= np.roll(np.roll(fill, dy, 0), dx, 1)
    edge = grown & ~fill
    crest = np.zeros_like(fill)
    crest[min(spec)] = fill[min(spec)]
    cols = np.nonzero((edge | fill).any(axis=0))[0]
    c0, c1 = int(cols.min()), int(cols.max()) + 1
    return x0 + c0, edge[:, c0:c1], (fill & ~crest)[:, c0:c1], crest[:, c0:c1]


def apply_badge(make, sets):
    """the badge's textures in every set (its own art at 480 lines): the outline (where the bar's frame is not: it
    is drawn over the frame), the slot (fill and crest, the unlit badge), the fill, the crest -- I4 masks
    the module colours (n64_hud64.c shield); placed at their left column (ref offsets, _set)"""
    for key, sk in ((1, "s240"), (2, "s480"), ("ws", "s240ws"), ("ws2", "s480ws")):
        S = sets[key]
        bx0, edge, fill, crest = badge_masks(sk)
        frame = S.T["shield_frame"].A > 0
        edge = edge.copy()
        for r in range(edge.shape[0]):
            for c in range(edge.shape[1]):
                x = bx0 + c
                if edge[r, c] and 0 <= x < frame.shape[1] and r < frame.shape[0] and frame[r, x]:
                    edge[r, c] = False
        tag = f"{S.sfx}_{S.tag}"
        for k, mask in (("badge_edge", edge), ("badge_slot", fill | crest), ("badge_fill", fill), ("badge_crest", crest)):
            S.T[k] = make.Tex(f"{k}{tag}", "I4", mask.astype(np.float32), None, S.tag, "the shield badge")
        S.badge_x0 = bx0


def badge_overrides(m):
    """the design's 1-2p shield panel without its triangle (order and frame): the badge takes its place"""
    full0 = m.Set.shield_full

    def shield_full(self):
        full0(self)
        s, sx = self.s, self.sx
        w, h = 62, 16
        bar = m.poly([(7, 1), (54, 1), (61, 15), (49, 15), (45.5, 7), (9, 7)], w, h, s, sx)
        tri = m.poly([(0.5, 1.5), (5.5, 1.5), (5.5, 7.5)], w, h, s, sx) & ~bar
        self.order["shield"] = np.where(tri, 0, self.order["shield"]).astype(np.float32)
        self.frame("shield_frame", bar, m.dilate(bar, s) & ~bar, "shield_frame", 0.13)
    m.Set.shield_full = shield_full

def load_design(design):
    sys.path.insert(0, str(Path(design).resolve()))
    import hud64_design as make  # noqa: E402  (the design, read only)
    text_and_grid_overrides(make)
    numeral_glyph_overrides(make)
    label_and_panel_overrides(make)
    badge_overrides(make)
    sets = make.build_sets()
    apply_badge(make, sets)
    return make, sets


SET_KEYS = {"s240": 1, "s480": 2, "s240ws": "ws", "s480ws": "ws2"}
GROUP_OF_SET = {"s240": "g240", "s480": "g480", "s240ws": "g240ws", "s480ws": "g480ws"}
GROUPS = ("g240", "g240ws", "g480", "g480ws")
FMT_CODE = {"I4": 0, "IA4": 1, "I8": 2, "IA8": 3}
ICON_KEYS = ("times", "round", "frag", "plasma", "cross")
METER_KEYS = ("shield", "health", "shield_s", "health_s", "ticks", "ticks4", "bar", "heat", "heat4", "shield_w",
              "shield_s_w")
# the shield's meters at 640 across (HIGH DETAIL 640x240, the 240-line sets
# doubled across): a column of the order map to each screen column, the
# design's ramp at twice the columns, so a recharge advances a pixel at a
# time, not 2, as smooth as ORIGINAL's recharge; the 480-line sets draw theirs at 480 already
WIDE_METERS = {"shield_w": "shield", "shield_s_w": "shield_s"}
HIGHLIGHT_METERS = ("shield", "shield_s")
# the shield's meters, every face: lit in proportion to the value (proportional)
PROPORTIONAL_METERS = ("shield", "shield_s")
# the meters that light from the right, as Halo's own do (ORIGINAL's shield,
# overcharge and health light from their right end; its rounds from the left,
# as the design's ticks already do); the design lights every meter from the left
MIRRORED_METERS = ("shield", "shield_s")
# (the health meters are the gauge's cells, their order already counted from the right -- apply_gauge)
# (the reticle and the motion tracker
# are Halo's own in either style -- their art ORIGINAL's -- so the design's
# reticle, tracker and blips are not in the tables)
REFS = ("weapon", "weapon_edge", "grenade", "grenade_edge", "ammo4", "ammo4_edge", "gren4", "gren4_edge", "plate",
        "plate4", "shield_frame", "health_frame", "shield_s_frame", "health_s_frame", "bar_slot", "button", "gren4_e",
        "gren4_e_edge", "health_slots", "health_s_slots", "badge_edge", "badge_slot", "badge_fill", "badge_crest")
FACE_KEYS = ("msg", "n8", "n7", "lbl")
# the heat bar's colour along its length, a gradient like Halo's own heat
# meter: blue to purple to

# red by position along the bar, as ORIGINAL's plasma weapons' heat meter
# runs, not by the heat (the same colours at heat 0.4 and 0.95) -- 16 even
# steps from the design's blue to its warning red, mixed in RGB
HEAT_STOPS = 16


def heat_gradient(blue, red):
    out = []
    for k in range(HEAT_STOPS):
        t = k / (HEAT_STOPS - 1)
        r, g, b = (int(round(c0 + (c1 - c0) * t)) for c0, c1 in zip(blue, red))
        out.append(r << 16 | g << 8 | b)
    return tuple(out)
OVERCHARGE = ((0xFF, 0x00, 0x00), (0x00, 0xFF, 0x00), (0xFF, 0xFF, 0x00), (0x7F, 0x00, 0xFF))
KIND_CODE = {"glow": 0, "numglow": 0, "glowfull": 0, "soft": 1, "soft2": 2}


class Texture:
    """one stored texture: N64 format, the design's quantised I and A"""

    def __init__(self, name, fmt, I, A, group):
        self.name, self.fmt, self.group = name, fmt, group
        self.I = np.asarray(I, np.float32)
        self.A = np.asarray(A, np.float32) if A is not None else self.I
        self.h, self.w = self.I.shape

    def stride(self):
        bpp = 4 if self.fmt in ("I4", "IA4") else 8
        return ((self.w * bpp + 7) // 8 + 7) & ~7

    def pixels(self):
        """N64 bytes, rows of stride() bytes (8-byte aligned)"""
        st = self.stride()
        out = bytearray(st * self.h)
        if self.fmt == "I4":
            v = np.round(self.I * 15).astype(np.uint8)
        elif self.fmt == "IA4":
            v = (np.round(self.I * 7).astype(np.uint8) << 1) | (self.A >= 0.5).astype(np.uint8)
        elif self.fmt == "IA8":
            v = (np.round(self.I * 15).astype(np.uint8) << 4) | np.round(self.A * 15).astype(np.uint8)
        else:
            v = np.round(self.I * 255).astype(np.uint8)
        for y in range(self.h):
            row = v[y]
            if self.fmt in ("I4", "IA4"):
                for x in range(0, self.w, 2):
                    hi = int(row[x]) & 15
                    lo = int(row[x + 1]) & 15 if x + 1 < self.w else 0
                    out[y * st + x // 2] = hi << 4 | lo
            else:
                out[y * st:y * st + self.w] = bytes(row.tolist())
        return bytes(out)

    def decoded(self):
        """I and A as the RDP reads them back (for the reference renderer)"""
        if self.fmt == "I4":
            q = np.round(self.I * 15) / 15
            return q, q
        if self.fmt == "IA4":
            return np.round(self.I * 7) / 7, (self.A >= 0.5).astype(np.float32)
        if self.fmt == "IA8":
            return np.round(self.I * 15) / 15, np.round(self.A * 15) / 15
        q = np.round(self.I * 255) / 255
        return q, q


class Data:
    def __init__(self, design):
        make, sets = load_design(design)
        self.make, self.sets = make, sets
        self.heat = heat_gradient(make.BLUE, make.RED)
        self.tables_id = 0          # (main: from the header's text and the textures)
        self.gauge_pad = apply_gauge(make, sets)        # (the health pips)
        self.textures = []          # Texture, in id order
        self.by_name = {}
        self.faces = []             # dict per face
        self.face_index = {}
        self.setdata = {}
        self._index_design_textures()
        for skey in SET_KEYS:
            self.setdata[skey] = self._set(skey)
        self.arrangements = self._arrangements()
        self._compact()

    def _compact(self):
        """the textures no table draws -- the parts stacked into one (_merge) -- dropped, the rest numbered
        again in order: HUD64_TEXTURE_COUNT, and the module's arrays sized by it, as small as the tables need"""
        keep = sorted(self.used())
        new = {old: i for i, old in enumerate(keep)}
        if len(keep) == len(self.textures):
            return
        self.textures = [self.textures[i] for i in keep]
        self.by_name = {t.name: i for i, t in enumerate(self.textures)}
        for f in self.faces:
            for g in f["glyphs"].values():
                g["tex"] = new[g["tex"]]
        part = lambda r: (new[r[0]],) + tuple(r[1:]) if r and r[0] != 0xFFFF else r
        for d in self.setdata.values():
            d["refs"] = {k: (new[t], sc) for k, (t, sc) in d["refs"].items()}
            d["icons"] = {k: (new[c[0]],) + tuple(c[1:]) for k, c in d["icons"].items()}
            d["meters"] = {k: (part(b), part(h)) for k, (b, h) in d["meters"].items()}
            d["meter_ramp"] = {k: tuple(part(r) for r in v) for k, v in d.get("meter_ramp", {}).items()}

    def _arrangements(self):
        """each player count's arrangement (the design's compose: 1-2 players
        compose_full, 3-4 compose_4p): which variant of each part, so that a
        design with other arrangements needs other tables, not other code"""
        out = {}
        for players in (1, 2, 3, 4):
            split = players >= 3
            out[players] = {
                "split": int(split),                      # ammo_split / shield_s / band of messages
                "sensor_label": 0 if split else 1,        # "15m" (not in 3-4p: no room beside the tracker)
                "score_words": 0 if split else 1,         # YOU / 1ST (1-2p)
                "prompt_first_form": 1 if split else 0,   # 3-4p: HOLD [B] PICK UP <short name>
                # (3-4p: the lines beside the tracker, as many as
                # stay below the aim zone -- n64_hud64.c split_lines)
                "message_lines": 3 if split else 2,
            }
        return out

    # ---- textures
    def used(self):
        """the textures the tables reference (the design's spent-round
        slots are drawn from the rounds' order maps instead: the same pixels,
        and only as many slots as the magazine holds)"""
        out = set()
        for f in self.faces:
            out |= {g["tex"] for g in f["glyphs"].values()}
        for d in self.setdata.values():
            out |= {t for t, _ in d["refs"].values()}
            out |= {c[0] for c in d["icons"].values()}
            for body, hl in d["meters"].values():
                if body[0] == 0xFFFF:
                    continue
                out.add(body[0])
                if hl:
                    out.add(hl[0])
            for ramp in d.get("meter_ramp", {}).values():
                out |= {r[0] for r in ramp if r}
        return out

    def add(self, tex):
        if tex.name in self.by_name:
            return self.by_name[tex.name]
        self.by_name[tex.name] = len(self.textures)
        self.textures.append(tex)
        return self.by_name[tex.name]

    def _index_design_textures(self):
        m = self.make
        for skey, k in SET_KEYS.items():
            S = self.sets[k]
            for name, t in sorted(S.T.items()):
                if isinstance(t, m.Scaled):
                    continue
                self.add(Texture(t.name, t.fmt, t.I, t.A, GROUP_OF_SET[skey]))

    def ref(self, S, key):
        """(texture id, scale) of the design's texture S.T[key]"""
        m = self.make
        t = S.T[key]
        if isinstance(t, m.Scaled):
            return self.by_name[t.base.name], t.h // t.base.h
        return self.by_name[t.name], 1

    def _atlas_of(self, S, arr):
        """the atlas texture a cell array is a view of, and the cell's x, y"""
        base = arr.base if arr.base is not None else arr
        for name, t in S.T.items():
            if isinstance(t, self.make.Scaled):
                continue
            if t.I is base or np.shares_memory(t.I, arr):
                off = (arr.__array_interface__["data"][0] - t.I.__array_interface__["data"][0]) // t.I.itemsize
                y, x = divmod(off, t.I.shape[1])
                assert np.array_equal(t.I[y:y + arr.shape[0], x:x + arr.shape[1]], arr)
                return self.by_name[t.name], x, y
        raise KeyError("cell not in an atlas")

    # ---- faces
    def face(self, S, f):
        key = (S.tag, S.sfx, f.name)
        if key in self.face_index:
            return self.face_index[key]
        glyphs = {}
        for ch, (I, A) in f.cells.items():
            tid, x, y = self._atlas_of(S, I)
            glyphs[ch] = {"tex": tid, "x": int(x), "y": int(y), "w": int(I.shape[1]), "h": int(I.shape[0]),
                          "adv": int(f.width(ch)), "oy": int(f.oy[ch])}
        ry = 2 if f.kind == "soft2" else 1
        d = {"name": f.name, "kind": KIND_CODE[f.kind], "pad": [int(f.pad[0]), int(f.pad[1])], "space": int(f.space),
             "gap": int(f.gap), "cap": int(f.cap), "ry": ry, "glyphs": glyphs,
             "ring": self._ring(f, ry) if KIND_CODE[f.kind] else []}
        self.face_index[key] = len(self.faces)
        self.faces.append(d)
        return self.face_index[key]

    @staticmethod
    def _ring(f, ry):
        """the outline's offsets (the design: the glyph drawn at every
        neighbour offset, dx -1..1, dy -ry..ry): the fewest that give every
        glyph of the face the same outline pixels (a face without 1-pixel
        strokes needs fewer draws). Checked glyph by glyph"""
        full = [(dx, dy) for dy in range(-ry, ry + 1) for dx in (-1, 0, 1) if dx or dy]
        masks = [(A > 0.5) for _, A in f.cells.values()]

        def ring(m, offs):
            H, W = m.shape
            pad = np.pad(m, ((ry, ry), (1, 1)))
            out = np.zeros_like(pad)
            for dx, dy in offs:
                out[ry + dy:ry + dy + H, 1 + dx:1 + dx + W] |= m
            return out & ~pad

        want = [ring(m, full) for m in masks]
        offs = list(full)
        # (the inner offsets first: the corners and sides cover them)
        for o in sorted(full, key=lambda o: (abs(o[0]) + abs(o[1]), abs(o[1]))):
            trial = [x for x in offs if x != o]
            if all(np.array_equal(ring(m, trial), w) for m, w in zip(masks, want)):
                offs = trial
        return [[int(dx), int(dy)] for dx, dy in offs]

    # ---- one set
    @staticmethod
    def column_ramp(order):
        """a shield order map lit a column at a time: the design's ramp
        (column / width) over the map's own columns; the part lit at any
        value (1/255) kept. The 480-line sets' maps are the 240-line ones
        doubled (the design's SHARED_ORDERS), 2 columns a step; WIDE_METERS"""
        fixed = order == np.float32(1 / 255)
        shape = order > 0
        H, W = shape.shape
        ramp = np.clip(np.mgrid[0:H, 0:W][1] / W, 1 / 255, 1)
        return np.where(fixed, 1 / 255, np.where(shape, ramp, 0)).astype(np.float32)

    @classmethod
    def wide_order(cls, order):
        """a shield order map at twice the columns (WIDE_METERS)"""
        return cls.column_ramp(np.repeat(order, 2, axis=1))

    @staticmethod
    def heat_order(order):
        """a battery weapon's heat (as ORIGINAL's plasma weapons show their
        heat under the battery): a bar
        over the round grid's first row -- from its first tick's left to its
        last tick's right, the ticks' height -- filled from the left; and
        where it sits on the grid (x, y)"""
        vals = np.unique(order[order > 0])
        boxes = []
        for v in vals:
            ys, xs = np.nonzero(order == v)
            boxes.append((int(xs.min()), int(ys.min()), int(xs.max()) + 1, int(ys.max()) + 1))
        first = [b for b in boxes if b[1] == boxes[0][1]]
        x0, y0, x1, y1 = first[0][0], first[0][1], first[-1][2], first[0][3]
        w = x1 - x0
        xx = np.mgrid[0:y1 - y0, 0:w][1]
        return np.clip((xx + 1) / w, 1 / 255, 1).astype(np.float32), (x0, y0)

    def _set(self, skey):
        m, S = self.make, self.sets[SET_KEYS[skey]]
        if S.s == 1:
            for k, src in (("heat", "ticks"), ("heat4", "ticks4")):
                S.order[k], S.__dict__.setdefault("heat_at", {})[k] = self.heat_order(S.order[src])
        s, sx = S.s, S.sx
        base = self.sets[1] if skey == "s480" else self.sets["ws"] if skey == "s480ws" else S
        d = {"s": s, "ws": int(sx != 1.0), "refs": {}, "faces": {}, "wfaces": {}, "icons": {},
             "meters": {}, "ref_off": {}, "meter_ramp": {}}
        for k in REFS:
            d["refs"][k] = self.ref(S, k)
        for k in ("badge_edge", "badge_slot", "badge_fill", "badge_crest"):
            d["ref_off"][k] = (S.badge_x0, 0)
        for split, keys in ((0, ("health_frame", "health_slots")), (1, ("health_s_frame", "health_s_slots"))):
            pad = self.gauge_pad[(skey.replace("480", "240"), split)]
            for k in keys:
                if pad:
                    d["ref_off"][k] = (-pad, 0)
        for k in FACE_KEYS:
            d["faces"][k] = self.face(S, S.faces[k])
            if S.wide:
                d["wfaces"][k] = self.face(S, S.wide[k])
        # icons: views of this set's atlas, or (480i WIDESCREEN) the base's at 2x
        src = S if not isinstance(S.T["icons0"], m.Scaled) else base
        for k in ICON_KEYS:
            I, A = src.icons[k]
            tid, x, y = self._atlas_of(src, I)
            d["icons"][k] = (tid, x, y, I.shape[1], I.shape[0], s if src is not S else 1)
        # meters: each order map as I8, 0 outside the shape and 256 - order*255
        # inside (the RDP's alpha compare keeps texels >= its threshold); the
        # shield's top edge apart (drawn in its highlight colour)
        for k in METER_KEYS:
            if k in WIDE_METERS:
                continue
            shared = s == 2 and k not in HIGHLIGHT_METERS
            if shared:
                src = self.setdata[skey.replace("480", "240")]
                body, hl = src["meters"][k]
                d["meters"][k] = ((body[0], 2) + tuple(body[2:]), (hl[0], 2) if hl else None)
                # (the 240-line ramp doubled: its rows 2 pixels at 480 lines)
                d["meter_ramp"][k] = tuple((r[0], 2) + tuple(r[2:]) if r else None
                                           for r in src["meter_ramp"].get(k, (None, None)))
                continue
            order = S.order[k]
            if k in PROPORTIONAL_METERS:
                order = proportional(order)           # (lit in proportion, from the right; 480 lines a column a step)
            else:
                if s == 2 and k in WIDE_METERS.values():
                    order = self.column_ramp(order)   # (480 lines: a column a step)
                order = mirror(order) if k in MIRRORED_METERS else order
            shape = order > 0
            hlm = shape & ~m.shift(shape, 1, 0) if k in HIGHLIGHT_METERS else np.zeros_like(shape)
            name = f"order_{k}{S.sfx}_{S.tag}"
            body_mask = shape & ~hlm
            gpad = 0
            if k in PROPORTIONAL_METERS:
                body_mask = self._ramp(d, skey, k, name, order, hlm, s)
            elif k in ("health", "health_s"):
                # (the gauge's cells, their tint and shade rows; WIDESCREEN 3-4p reaching left)
                gpad = self.gauge_pad[(skey, int(k == "health_s"))]
                tint, shade = gauge_ramp(order)
                parts = []
                for nm, mask in (("_tn", tint), ("_sh", shade)):
                    if not mask.any():
                        parts.append(None)
                        continue
                    rows = np.nonzero(mask.any(axis=1))[0]
                    y0, y1 = int(rows.min()), int(rows.max()) + 1
                    tid = self.add(Texture(name + nm, "I8", self._threshold(order, mask)[y0:y1], None,
                                           GROUP_OF_SET[skey]))
                    parts.append((tid, 1, -gpad, y0))
                d["meter_ramp"][k] = tuple(parts)
                body_mask = shape & ~tint & ~shade
            body_id = self.add(Texture(name, "I8", self._threshold(order, body_mask), None, GROUP_OF_SET[skey]))
            hl_id = None
            if hlm.any():
                hl_id = self.add(Texture(name + "_hl", "I8", self._threshold(order, hlm), None, GROUP_OF_SET[skey]))
            d["meters"][k] = ((body_id, 1, -gpad, 0) if gpad else (body_id, 1), (hl_id, 1) if hl_id is not None else None)
        for k, src in WIDE_METERS.items():
            if s != 1:
                d["meters"][k] = ((0xFFFF, 0), None)
                continue
            order = proportional(np.repeat(S.order[src], 2, axis=1))   # (a column a canvas pixel, in proportion)
            shape = order > 0
            hlm = shape & ~m.shift(shape, 1, 0)
            name = f"order_{k}{S.sfx}_{S.tag}"
            body_mask = self._ramp(d, skey, k, name, order, hlm, s)
            body_id = self.add(Texture(name, "I8", self._threshold(order, body_mask), None, GROUP_OF_SET[skey]))
            hl_id = self.add(Texture(name + "_hl", "I8", self._threshold(order, hlm), None, GROUP_OF_SET[skey]))
            d["meters"][k] = ((body_id, 1), (hl_id, 1))
        d["panels"] = {k: [int(v) for v in S.panels[k]] for k in S.panels}
        d["shield_size"] = [int(v) for v in S.shield_size]
        d["shield_s_size"] = [int(v) for v in S.shield_s_size]
        d["layout"] = self._layout(S)
        # the range label's rows of ink, its own face and (240 lines) the 640-wide one (its place off the tracker)
        d["layout"]["lbl_ink"] = self._label_ink(d["faces"]["lbl"], 1)
        d["layout"]["lbl_wink"] = self._label_ink(d["wfaces"]["lbl"], 2) if "lbl" in d["wfaces"] else [0] * LBL_INK_LEN
        # the lemniscate's card: the panel texels left out of its middle
        d["layout"]["full_inf_dx"] = self._inf_dx(d, "weapon", "n8", d["layout"]["full_num_dx"], d["layout"]["full_num_dy"])
        d["layout"]["split_inf_dx"] = self._inf_dx(d, "ammo4", "n7", d["layout"]["split_num_dx"],
                                                   d["layout"]["split_num_dy"])
        d["layout"]["trk_plate"] = [v for e in TRK_PLATE[skey] for v in e]
        hb = base if S.s == 2 else S
        d["layout"]["full_heat"] = [int(v) for v in hb.heat_at["heat"]]
        d["layout"]["split_heat"] = [int(v) for v in hb.heat_at["heat4"]]
        d["layout"]["full_plate"] = self._plate(S, base, "plate", "ticks", self.sets[1])
        d["layout"]["split_plate"] = self._plate(S, base, "plate4", "ticks4", self.sets[1])
        self._crop(d)
        self._merge(d, skey)
        return d

    # (the vitals panel's textures cropped to their ink -- the gauge's frame and slots, the shield's frame, the
    # badge, the shield's and the health's order maps and their tint, shade and top-edge parts -- not stored at the
    # panel's full size, mostly empty, which would take heap for nothing. Each texture keeps its id; every
    # reference and meter part that draws it moves by the cut's origin (struct hud64_ref's dx, dy: texels times the
    # scale; a 640-wide meter's, a texel a screen column). The drawn pixels are the same.)
    CROP_REFS = ("health_frame", "health_s_frame", "health_slots", "health_s_slots", "shield_frame", "shield_s_frame",
                 "badge_edge", "badge_slot", "badge_fill", "badge_crest")
    CROP_METERS = ("shield", "shield_s", "health", "health_s", "shield_w", "shield_s_w")

    def _crop_texture(self, tid):
        """the texture cut to its ink (once; later calls give the same origin): its (x0, y0)"""
        if tid in self.crop_origin:
            return self.crop_origin[tid], False
        t = self.textures[tid]
        ink = (t.A > 0) | (t.I > 0) if t.fmt in ("IA4", "IA8") else (t.I > 0)
        ys, xs = np.nonzero(ink)
        if not len(xs):
            self.crop_origin[tid] = (0, 0)
            return (0, 0), True
        x0, y0, x1, y1 = int(xs.min()), int(ys.min()), int(xs.max()) + 1, int(ys.max()) + 1
        A = t.A[y0:y1, x0:x1] if t.A is not t.I else None
        self.textures[tid] = Texture(t.name, t.fmt, t.I[y0:y1, x0:x1], A, t.group)
        self.crop_origin[tid] = (x0, y0)
        return (x0, y0), True

    def _crop(self, d):
        if not hasattr(self, "crop_origin"):
            self.crop_origin = {}
        for k in self.CROP_REFS:
            if k not in d["refs"]:
                continue
            tid, sc = d["refs"][k]
            (x0, y0), _ = self._crop_texture(tid)
            dx, dy = d["ref_off"].get(k, (0, 0))
            d["ref_off"][k] = (dx + x0, dy + y0)
        for k in self.CROP_METERS:
            if k not in d["meters"]:
                continue
            body, hl = d["meters"][k]
            tint, shade = d["meter_ramp"].get(k, (None, None))
            parts = []
            for r in (body, hl, tint, shade):
                if not r or r[0] == 0xFFFF:
                    parts.append(r)
                    continue
                (x0, y0), new = self._crop_texture(r[0])
                dx, dy = (r[2], r[3]) if len(r) > 2 else (0, 0)
                # (a 480-line set's meter that draws the 240-line texture copies its parts, offsets already moved)
                parts.append((r[0], r[1], dx + x0, dy + y0) + tuple(r[4:]) if new else r)
            d["meters"][k] = (parts[0], parts[1])
            if k in d["meter_ramp"]:
                d["meter_ramp"][k] = (parts[2], parts[3])

    # (a meter's parts -- its body, top edge, tint and shade -- and the badge's four, each one texture stacked
    # into one ("atlas_..."), drawn by its rows (struct hud64_ref's t0, h): each texture's upload is a block on the
    # heap, about 0.6 KB however small the texture, so one block for the parts, not four. Only where the parts share
    # a format and the stack fits one TMEM load (4,096 bytes); the drawn pixels are the same)
    MERGE_METERS = ("shield", "shield_s", "health", "health_s", "shield_w", "shield_s_w")
    MERGE_REFS = (("badge", ("badge_edge", "badge_slot", "badge_fill", "badge_crest")),)

    def _stack(self, name, tids, group):
        """one texture of the given ones stacked (left-aligned, the widest's width): its id, each one's (t0, h)"""
        ts = [self.textures[t] for t in tids]
        fmt = ts[0].fmt
        if any(t.fmt != fmt for t in ts):
            return None
        W = max(t.w for t in ts)
        rows = sum(t.h for t in ts)
        probe = Texture(name, fmt, np.zeros((rows, W), np.float32), None, group)
        if probe.stride() * rows > 4096:
            return None
        I = np.zeros((rows, W), np.float32)
        A = np.zeros((rows, W), np.float32)
        at, y = [], 0
        for t in ts:
            I[y:y + t.h, :t.w] = t.I
            A[y:y + t.h, :t.w] = t.A
            at.append((y, t.h))
            y += t.h
        aid = self.add(Texture(name, fmt, I, A if fmt in ("IA4", "IA8") else None, group))
        return aid, at

    def _merge(self, d, skey):
        if not hasattr(self, "atlases"):
            self.atlases = set()
        S = self.sets[SET_KEYS[skey]]
        d.setdefault("ref_part", {})
        for k in self.MERGE_METERS:
            if k not in d["meters"]:
                continue
            body, hl = d["meters"][k]
            tint, shade = d["meter_ramp"].get(k, (None, None))
            parts = [body, hl, tint, shade]
            live = [i for i, r in enumerate(parts) if r and r[0] != 0xFFFF]
            # (a 480-line set's meter drawing the 240-line parts copies them: merged there already)
            if len(live) < 2 or any(parts[i][0] in self.atlases for i in live):
                continue
            got = self._stack(f"atlas_{k}{S.sfx}_{S.tag}", [parts[i][0] for i in live], GROUP_OF_SET[skey])
            if not got:
                continue
            aid, at = got
            self.atlases.add(aid)
            for i, (t0, h) in zip(live, at):
                r = parts[i]
                dx, dy = (r[2], r[3]) if len(r) > 2 else (0, 0)
                parts[i] = (aid, r[1], dx, dy, t0, h)
            d["meters"][k] = (parts[0], parts[1])
            if k in d["meter_ramp"]:
                d["meter_ramp"][k] = (parts[2], parts[3])
        for nm, keys in self.MERGE_REFS:
            if not all(k in d["refs"] for k in keys):
                continue
            tids = [d["refs"][k][0] for k in keys]
            if any(t in self.atlases for t in tids) or len({d["refs"][k][1] for k in keys}) != 1:
                continue
            got = self._stack(f"atlas_{nm}{S.sfx}_{S.tag}", tids, GROUP_OF_SET[skey])
            if not got:
                continue
            aid, at = got
            self.atlases.add(aid)
            for k, (t0, h) in zip(keys, at):
                d["refs"][k] = (aid, d["refs"][k][1])
                d["ref_part"][k] = (t0, h)

    @staticmethod
    def _plate_ends(inside, order):
        """the plate's right end and the last tick's along the grid's first
        row (texels, exclusive): the plate's rightmost column over the first
        row's lines, the first row's last tick's right column"""
        vals = np.unique(order[order > 0])
        corners = []
        for v in vals:
            ys, xs = np.nonzero(order == v)
            corners.append((int(xs.min()), int(ys.min()), int(xs.max()) + 1, int(ys.max()) + 1))
        first = [c for c in corners if c[1] == corners[0][1]]
        y0, y1 = first[0][1], first[0][3]
        plate_right = max(int(np.nonzero(inside[y])[0].max()) + 1 for y in range(y0, y1))
        return plate_right, first[-1][2]

    @staticmethod
    def _plate_left_margin(inside, order):
        """the plate's columns before the first tick, along the grid's first
        row (texels): the first row's first tick's left column less the
        plate's leftmost column over its lines"""
        vals = np.unique(order[order > 0])
        ys, xs = np.nonzero(order == vals[0])
        y0, y1 = int(ys.min()), int(ys.max()) + 1
        plate_left = min(int(np.nonzero(inside[y])[0].min()) for y in range(y0, y1))
        return int(xs.min()) - plate_left

    def _plate(self, S, base, plate_key, order_key, normal):
        """the plate under a round grid, sized to the slots the magazine
        draws (a 2-round rocket launcher must not get the assault rifle's
        plate). Read from the design's own
        plate and grid, in the plate texture's texels: [ticks, columns, rows,
        the ticks' pitch across, their pitch down, the seam]. The game draws
        the full plate's top rows (a row of ticks fewer: a pitch down
        fewer); when the slots fill less than a row, the plate's left part
        up to the seam and its right end moved left by the unused columns
        (a pitch across each), so the plate keeps its lean ends and its
        margins round the slots"""
        m = self.make
        t = S.T[plate_key]
        src_set = base if isinstance(t, m.Scaled) else S
        src = t.base if isinstance(t, m.Scaled) else t
        inside = src.A > 0
        order = src_set.order[order_key]
        assert order.shape == inside.shape, (plate_key, order.shape, inside.shape)
        q = np.clip(np.floor(order * 255 + 1e-4), 1, 255)
        vals = np.unique(order[order > 0])
        n = len(vals)
        corners = []
        for j, v in enumerate(vals, 1):
            ys, xs = np.nonzero(order == v)
            # (the game counts the ticks drawn as draw_meter keeps them:
            # tick j of n stored at 256 - j * 255 / n)
            assert int(q[ys[0], xs[0]]) == j * 255 // n, (plate_key, j)
            corners.append((int(xs.min()), int(ys.min())))
        cols = sum(1 for c in corners if c[1] == corners[0][1])
        rows = n // cols
        assert cols * rows == n
        px = corners[1][0] - corners[0][0]
        py = corners[cols][1] - corners[0][1] if rows > 1 else 0
        H, W = inside.shape
        h1 = H - (rows - 1) * py
        dmax = (cols - 1) * px
        # the seam: inside the plate on both sides for every row the
        # shortest plate keeps, at every shift (each row is one run)
        # the plate's right end as far past the last tick drawn as its left
        # end is before the first (the plate balanced: not 3 pixels past the
        # last tick with 0-1 before the first); dx0 the
        # shift that gives it with the grid's every column
        right, last = self._plate_ends(inside, order)
        margin = self._plate_left_margin(inside, order)
        dx0 = right - last - margin
        assert dx0 >= 0, (plate_key, S.tag, S.sfx, dx0)
        dmax = dx0 + (cols - 1) * px
        cut = next(c for c in range(1, W - dmax)
                   if all(inside[y, c - 1] and inside[y, c + dmax] for y in range(h1)))
        return [n, cols, rows, px, py, cut, dx0, margin]

    def _ramp(self, d, skey, k, name, order, hlm, s):
        """the shield meter's tint and shade parts (shield_ramp), each an I8 order map cropped to its rows; returns
        the body's mask (the order map less the top edge, the tint and the static shade -- the diagonal's texels stay
        in the body, its shade drawn over them once its later order passes). 480 lines: the 240-line face's rules,
        its rows 2 pixels (a 1-line detail flickers in 480i)"""
        shape = order > 0
        decide = None
        if s == 2:
            decide = self.__dict__.setdefault("ramp_rules", {}).get((skey.replace("480", "240"), k))
        tint, static, diag, delayed, rule = shield_ramp(order, hlm, 2 if s == 2 else 1, decide)
        if s == 1:
            self.__dict__.setdefault("ramp_rules", {})[(skey, k)] = rule
        parts = []
        for nm, mask, src in (("_tn", tint, order), ("_sh", static | diag, np.where(diag, delayed, order))):
            if not mask.any():
                parts.append(None)
                continue
            rows = np.nonzero(mask.any(axis=1))[0]
            y0, y1 = int(rows.min()), int(rows.max()) + 1
            tid = self.add(Texture(name + nm, "I8", self._threshold(src, mask)[y0:y1], None, GROUP_OF_SET[skey]))
            parts.append((tid, 1, 0, y0))
        d["meter_ramp"][k] = tuple(parts)
        d.setdefault("ramp_rules", {})[k] = {kk: bool(v) for kk, v in rule.items()}
        return shape & ~hlm & ~tint & ~static

    @staticmethod
    def _threshold(order, mask):
        q = np.clip(np.floor(order * 255 + 1e-4), 1, 255)  # (the C code: value * 255 + 1e-3, floored)
        t = np.where(mask, 256 - q, 0)
        return t / 255.0

    def _inf_dx(self, d, panel, fkey, num_dx, num_dy):
        """the texels n64_hud64.c infinite_panel leaves out of the panel's middle under unlimited ammunition, so that
        the card's blue is visually balanced about the lemniscate.
        The card is balanced when its blue is the same on each side of the glyph: split at the middle of the glyph's
        ink (its core and its outline), the panel's fill pixels that the outline and the ink leave, counted on each
        side over the rows between the outlines -- the slanted ends included. The card's box and the glyph share a
        middle when the panel is the glyph's advance and its margin before it twice; of the lengths within a texel
        of that one that keep the glyph's ink off the card's outline, the one whose blue balances best (a tie: the
        longer; none: the longest). The panel's staircase ends are not
        quite symmetric, which moves the best length off the box's middle by up to a texel (at 480 lines, where
        a texel is 2 pixels and the glyphs are an odd number of pixels wide, the box's middle falls between two
        texels). (The slant's run -- a third of the panel's height -- added after the glyph instead would leave more
        blue on the right.) In this set's pixels (640x240 draws the same texels, doubled)"""
        def mask(key):
            tid, sc = d["refs"][key]
            a = self.textures[tid].A > 0
            return (np.kron(a, np.ones((sc, sc), bool)) if sc > 1 else a), sc
        (fill, sc), (edge, _) = mask(panel), mask(panel + "_edge")
        pw = fill.shape[1]
        f = self.faces[d["faces"][fkey]]
        g = f["glyphs"][INFINITY]
        A = self.textures[g["tex"]].A[g["y"]:g["y"] + g["h"], g["x"]:g["x"] + g["w"]]
        want, cut = 2 * num_dx + g["adv"], pw // 3 // sc
        O = 8
        best = None
        lo, hi = (pw - want) // sc, -(-(pw - want) // sc)
        cands = sorted({max(0, v) for v in range(lo - 1, hi + 2)})
        for dx in cands:
            # (the short panel as draw_ref_short draws it: its columns to the cut, then its right end moved left)
            keep = np.r_[0:cut * sc, (cut + dx) * sc:pw]
            H, W = fill.shape[0] + 2 * O, pw + 2 * O
            F, E, ink = (np.zeros((H, W), bool) for _ in range(3))
            F[O:O + fill.shape[0], O:O + len(keep)] = fill[:, keep]
            E[O:O + edge.shape[0], O:O + len(keep)] = edge[:, keep[keep < edge.shape[1]]]
            # (the glyph as glyph_draw / outline_draw place it: a 320-wide face's cell holds its outline)
            if f["kind"] == 0:
                y, x = O + num_dy - f["pad"][1], O + num_dx - f["pad"][0]
                ink[y:y + g["h"], x:x + g["w"]] |= A > 0
            else:
                for rx, ry in f["ring"]:
                    ink[O + num_dy + ry:O + num_dy + ry + g["h"], O + num_dx + rx:O + num_dx + rx + g["w"]] |= A > 0.5
                ink[O + num_dy:O + num_dy + g["h"], O + num_dx:O + num_dx + g["w"]] |= A > 0
            card = F | E
            ink &= card
            blue = F & ~E & ~ink
            # (the glyph's ink may not cover the card's outline -- on a row between the outlines, the card's first or
            # last pixel ink: a length that cramps the glyph there is out)
            inner = np.nonzero((blue | ink).any(axis=1))[0]
            ends = [(y, np.nonzero(card[y])[0]) for y in inner]
            if any(ink[y, c.min()] or ink[y, c.max()] for y, c in ends) and dx != cands[0]:
                continue
            xs = np.nonzero(ink.any(axis=0))[0]
            cx = (xs.min() + xs.max()) / 2
            bx = np.nonzero(blue)[1]
            rows = int((blue | ink).any(axis=1).sum())
            D = ((bx > cx).sum() - (bx < cx).sum()) / rows
            if best is None or abs(D) < abs(best[0]) - 1e-9:
                best = (D, dx)
        return int(best[1])

    def _label_ink(self, fi, xs):
        """the range label "15m" in face fi as n64_hud64.c draws it (a soft face's cells with their haze; an
        outlined face's outline cell and glyphs): [first ink row from the text's top, rows, then each row's first
        and last ink column from the text's left (an empty row: 1, 0)], in the canvas's pixels (640x240: the
        640-wide face, its text drawn from 2 x the label's x): the label's test against Halo's
        tracker circle"""
        f = self.faces[fi]
        H, W, OX, OY = 64, 160, 16, 16
        ink = np.zeros((H, W), bool)
        x = 0
        for ch in "15m":
            g = f["glyphs"][ch]
            A = self.textures[g["tex"]].A[g["y"]:g["y"] + g["h"], g["x"]:g["x"] + g["w"]]
            if f["kind"] == 0:
                ink[OY - f["pad"][1]:OY - f["pad"][1] + g["h"], OX + x - f["pad"][0]:OX + x - f["pad"][0] + g["w"]] |= A > 0
            else:
                m = A > 0.5
                for dx, dy in f["ring"]:
                    ink[OY + dy:OY + dy + g["h"], OX + x + dx:OX + x + dx + g["w"]] |= m
                ink[OY:OY + g["h"], OX + x:OX + x + g["w"]] |= A > 0
            x += g["adv"] + f["gap"]
        ys = np.nonzero(ink.any(axis=1))[0]
        out = [int(ys.min() - OY), int(ys.max() - ys.min() + 1)]
        assert out[1] <= (LBL_INK_LEN - 2) // 2
        for y in range(ys.min(), ys.max() + 1):
            c = np.nonzero(ink[y])[0]
            out += [int(c.min() - OX), int(c.max() - OX)] if len(c) else [1, 0]
        return out + [1, 0] * ((LBL_INK_LEN - len(out)) // 2)

    @staticmethod
    def _readout_gap(S, ammo, gren, gren_dx):
        """the readout gap (the panels kept apart, not shrunk, where WIDESCREEN 3-4p would make them touch): how
        far the grenade panel must move right from gren_dx so that on every row both panels
        ink, the ammunition panel's last inked column and the grenade panel's first keep the protected panel gap
        (s pixels: 1 at 240 lines, drawn 2 wide at 640x240; 2 at 480). The panels are slanted, so their rectangles
        overlap by design; the gap is a row's. 0 where the design keeps it (every 4:3 and 1-2 player layout)"""
        T = S.T

        def ink(keys):
            return np.logical_or.reduce([T[k].A > 0 for k in keys])
        am, gm = ink(ammo), ink(gren)
        worst = None
        for r in range(min(am.shape[0], gm.shape[0])):
            if am[r].any() and gm[r].any():
                g = gren_dx + int(np.nonzero(gm[r])[0].min()) - int(np.nonzero(am[r])[0].max()) - 1
                worst = g if worst is None else min(worst, g)
        return 0 if worst is None or worst >= S.s else S.s - worst

    @staticmethod
    def _gren_room(S, gren_dx):
        """the narrow views' fallback (gren4_e): the narrowest view (R - L, this set's
        pixels) in which the wider 3-4p grenade panel keeps the panels' gap (s
        pixels) to the shield panel -- the grenade panel's last inked column
        from the view's left, the shield panel's first from its right"""
        T = S.T
        ink = (T["gren4"].A > 0) | (T["gren4_edge"].A > 0)
        right = int(np.nonzero(ink.any(axis=0))[0].max())
        sh = (T["shield_s_frame"].A > 0) | (S.order["shield_s"] > 0)
        left = int(np.nonzero(sh.any(axis=0))[0].min())
        hs = T["health_s_frame"].A > 0          # (the gauge's frame, round its cells, may reach left of the
        pad = hs.shape[1] - sh.shape[1]        # panel's origin: its texture is wider by that much)
        left = min(left, int(np.nonzero(hs.any(axis=0))[0].min()) - pad)
        return gren_dx + right + 1 + S.s + S.shield_s_size[0] - left

    def _layout(self, S):
        """every distance compose_full / compose_4p / draw_score use, in this
        set's pixels (the design's own formulas, its own rounding)"""
        m, s, X = self.make, S.s, S.X
        f = S.faces["msg"]
        pw, ph = S.panels["weapon"]
        p4w, p4h = S.panels["ammo4"]
        gw, gh = S.panels["gren4"]
        lean = m.NUMSPEC[(s, S.sx != 1.0)][3]
        plasma_h = S.icons["plasma"][0].shape[0]
        # (the readout gap: the grenade panel moved right where a row of it would come within the panels' gap of
        # the ammunition panel -- WIDESCREEN 3-4 players; gren4 and gren4_e share the left end)
        sdx = p4w - X(2)
        sdx += max(self._readout_gap(S, ("ammo4", "ammo4_edge"), ("gren4", "gren4_edge"), sdx),
                   self._readout_gap(S, ("ammo4", "ammo4_edge"), ("gren4_e", "gren4_e_edge"), sdx))
        # (and at 480 lines the 3-4p plate and its ticks or heat bar 1 pixel lower: there the frag icon, set a pixel
        # down in its panel (split_frag_dy), would come down onto the plate's top row: the icon keeps its place,
        # the readout gains a pixel of room below)
        fdx = pw - X(3)
        fdx += self._readout_gap(S, ("weapon", "weapon_edge"), ("grenade", "grenade_edge"), fdx)
        return {
            # 1-2 players (compose_full)
            "full_num_dx": X(6), "full_num_dy": 2 * s, "full_times_dx": X(3), "full_round_dx": X(10),
            "full_icon_dy": 2 * s, "full_ticks_dy": (ph // s + 1) * s, "full_gren_dx": fdx,
            "full_frag_dx": X(6 + GREN_LEAD["full"]), "full_gnum_dx": X(8 + GREN_EXTRA[("full", S.sx != 1.0)][0]),
            "full_plasma_dx": X(4 + GREN_EXTRA[("full", S.sx != 1.0)][1]), "full_plasma_dy": s,
            "full_pnum_dx": X(12 + sum(GREN_EXTRA[("full", S.sx != 1.0)][1:])), "full_cross_dx": X(3), "full_cross_dy": 9 * s, "full_label_h": f.cap + 3 * s,
            "full_label_base": 2 * s, "full_msg_dx": X(5), "full_msg_gap": X(8),
            "full_pips": GAUGE[("s240", 0)][1], "split_pips": GAUGE[("s240", 1)][1],
            # 3-4 players (compose_4p, top_left with "ammo4")
            "split_num_dx": X(4), "split_num_dy": (p4h - 7 * s) // 2, "split_num_gap": s - lean,
            "split_ticks_dy": p4h + (1 if s == 2 else 0), "split_gren_dx": sdx, "split_frag_dx": X(4 + GREN_LEAD["split"]),
            "split_frag_dy": (s // 2 if s > 1 else 0), "split_gnum_dx": X(7 + GREN_EXTRA[("split", S.sx != 1.0)][0]),
            "split_plasma_dx": X(2 + GREN_EXTRA[("split", S.sx != 1.0)][1]),
            "split_plasma_dy": (gh - plasma_h) // 2 + s // 2, "split_pnum_dx": X(9 + sum(GREN_EXTRA[("split", S.sx != 1.0)][1:])), "split_sensor_dy": s,
            "split_frag_dx_e": X(4), "split_gren_room": self._gren_room(S, sdx),
            "split_msg_dx": s, "split_msg_pad": 2 * s,
            # score (draw_score)
            "score_row_h": f.cap + 4 * s, "score_dx": s, "score_base_dy": 4 * s, "score_row_gap": s,
            "score_bar_dy": s,
            # messages (message_lines, line_pitch, band_for)
            "pitch": f.cap + 2 * s, "msg_base_dy": s,
            "lift": s if S.T["button"].h - s > f.cap else 0,
            # Halo's motion tracker (drawn by Halo with either style): its
            # box, as the port places Halo's HUD -- measured in ORIGINAL's
            # layers at every display mode and inset: 41 pixels a side
            # (240 lines) for 1 player, 31 from 2; its left edge 8 pixels in
            # from a screen edge, 2 from a view's inner edge; its bottom 6
            # up from a screen edge, 2 from an inner edge; HUD INSET in from
            # every edge of a view; WIDESCREEN squeezes it to 0.75 about the
            # view's middle. The design's text keeps out of it
            "trk_size": [41 * s, 31 * s], "trk_edge": [8 * s, 2 * s, 6 * s, 2 * s],
            # the aim zone the 3-4p lines stay below (the view's reticle
            # box, 16% of its height, and 8 lines of 240 under it)
            "aim_clear": 8 * s,
            "button_adv": S.T["button"].w - s, "button_h": S.T["button"].h,
            "gap": m.GAP * s,
        }


# ---------------------------------------------------------------- outputs

# what each arrangement draws (n64_hud64.c: ammo_full / ammo_split, shield,
# sensor, score, messages, reticle): a game loads only its context's file
PARTS = {
    "full": {"refs": ("weapon", "weapon_edge", "grenade", "grenade_edge", "plate", "shield_frame", "health_frame",
                      "bar_slot", "button", "health_slots", "badge_edge", "badge_slot", "badge_fill", "badge_crest"),
             "faces": ("msg", "n8", "lbl"), "icons": ICON_KEYS, "meters": ("ticks", "shield", "health", "bar", "heat")},
    "split": {"refs": ("ammo4", "ammo4_edge", "gren4", "gren4_edge", "plate4", "shield_s_frame", "health_s_frame",
                       "bar_slot", "button", "gren4_e", "gren4_e_edge", "health_s_slots"),
              "faces": ("msg", "n7"), "icons": ("frag", "plasma"),
              "meters": ("ticks4", "shield_s", "health_s", "bar", "heat4")},
}
SET_ORDER = ("s240", "s480", "s240ws", "s480ws")  # n64_hud64_data.h's HUD64_SET_* order


def contexts(data):
    """(set, 640x240, split) -> the texture ids it draws; 640x240 only for
    the 240-line sets (its text from the 640-wide faces)"""
    out = {}
    for si, skey in enumerate(SET_ORDER):
        d = data.setdata[skey]
        for wide in ((0, 1) if d["s"] == 1 else (0,)):
            for split, arr in enumerate(("full", "split")):
                P = PARTS[arr]
                ids = {d["refs"][k][0] for k in P["refs"]}
                for k in P["faces"]:
                    f = data.faces[d["wfaces"][k] if wide else d["faces"][k]]
                    ids |= {g["tex"] for g in f["glyphs"].values()}
                ids |= {d["icons"][k][0] for k in P["icons"]}
                for k in P["meters"] + ((("shield_w",) if arr == "full" else ("shield_s_w",)) if wide else ()):
                    body, hl = d["meters"][k]
                    ids.add(body[0])
                    if hl:
                        ids.add(hl[0])
                    ids |= {r[0] for r in d.get("meter_ramp", {}).get(k, (None, None)) if r}
                out[(si, wide, split)] = sorted(ids)
    return out


def context_file(si, wide, split):
    return f"c{si}{wide}{split}.bin"


def write_fs(data, fs):
    """hud64/c<set><640x240><split>.bin, a file per context: "H64T",
    version, count, then per texture: id (u16), format, (pad), width,
    height, stride (u16 each), offset (u32 from the file's start); the
    pixels 8-byte aligned"""
    out = Path(fs) / "hud64"
    out.mkdir(parents=True, exist_ok=True)
    for old in out.glob("*.bin"):
        old.unlink()
    sizes = {}
    for (si, wide, split), ids in contexts(data).items():
        g = context_file(si, wide, split)[:-4]
        texs = [(i, data.textures[i]) for i in ids]
        head = 8 + 16 * len(texs)
        off = (head + 7) & ~7
        entries, blobs = [], []
        for i, t in texs:
            px = t.pixels()
            entries.append(struct.pack(">HBBHHHHI", i, FMT_CODE[t.fmt], 0, t.w, t.h, t.stride(), 0, off))
            blobs.append(px)
            off += (len(px) + 7) & ~7
        body = b"H64T" + struct.pack(">HH", data.tables_id, len(texs)) + b"".join(entries)
        body += b"\0" * (((len(body) + 7) & ~7) - len(body))
        for px in blobs:
            body += px + b"\0" * (((len(px) + 7) & ~7) - len(px))
        (out / f"{g}.bin").write_bytes(body)
        sizes[g] = len(body)
    return sizes


def c_rgb(rgb):
    return "0x%02X%02X%02X" % tuple(rgb)


TABLES_ID_LINE = "@TABLES_ID@"


def header_lines(data):
    """the header's lines, its tables id's line a placeholder (main)"""
    m = data.make
    L = []
    w = L.append
    w("/* n64_hud64_data.h: HUD STYLE 64's tables, written by tools/n64/hud64/hud64_gen.py")
    w("from the design (tools/n64/hud64/hud64_design.py). Do not edit: regenerate (with assets/hud64). */")
    w("#ifndef N64_HUD64_DATA_H\n#define N64_HUD64_DATA_H\n")
    w(f"#define HUD64_TEXTURE_COUNT {len(data.textures)}")
    w(f"#define HUD64_FACE_COUNT {len(data.faces)}")
    w("/* the texture files' tables id (their header's version field): a file from other tables is refused */")
    w(TABLES_ID_LINE)
    pal = {"BLUE": m.BLUE, "OUTLINE": m.OUTLINE, "NUMBER": m.NUMBER, "HELP": m.HELP, "SHADOW": m.SHADOW,
           "TICK_ON": m.TICK_ON, "TICK_OFF": m.TICK_OFF, "PLATE": m.PLATE, "PLASMA": m.PLASMA, "RED": m.RED,
           "BUTTON": m.BUTTON, "WHITE": m.WHITE, "LEADER": m.LEADER,
           # the grenade type not selected (both types shown, the selected
           # one lit): the design's grey for the other, the score's other bar
           "DIM": m.LEADER}
    pal["PIP_EMPTY"] = PIP_EMPTY       # (the pips' empty slots)
    for k, v in pal.items():
        w(f"#define HUD64_{k} {c_rgb(v)}")
    # the overshield: Halo's own
    # overcharge meters, one a level over the shield in these colours
    # (source/interface/hud_unit.c, hud_render_unit_interface: red, green,
    # yellow, purple); the design shows no overshield
    w("/* the overshield's levels 1-4 (Halo's hud_unit.c overcharge colours) */")
    w("static const uint32_t hud64_overcharge[5] = {0, " + ", ".join(c_rgb(v) for v in OVERCHARGE) + "};")
    w("/* the heat bar's colours along its length, the design's blue to its red (hud64_gen.py heat_gradient) */")
    w(f"#define HUD64_HEAT_STOPS {len(data.heat)}")
    w("static const uint32_t hud64_heat_gradient[HUD64_HEAT_STOPS] = {" +
      ", ".join("0x%06X" % v for v in data.heat) + "};")
    # alphas (0-255) the design draws with
    alphas = {"EDGE": m.EDGE_ALPHA, "SHIELD": 0.80, "HEALTH": 0.85, "BAR": 0.9, "PIP_EMPTY": 0.5,
              "BADGE_SLOT": BADGE_SLOT_ALPHA}
    for k, v in alphas.items():
        w(f"#define HUD64_ALPHA_{k} {int(round(v * 255))}")
    # the ticks' slots: the design's I4 0.9, as stored
    w(f"#define HUD64_ALPHA_SLOTS {int(round(np.round(0.9 * 15) / 15 * 255))}")
    w("")
    w("/* a context's file: its set (HUD64_SET_*), 640x240 (the 640-wide faces), 3-4 players' arrangement */")
    w('#define HUD64_CONTEXT_FILE "rom:/hud64/c%d%d%d.bin"')
    w("")
    w("struct hud64_glyph { uint16_t tex; uint8_t x, y, w, h, adv; int8_t oy; };")
    w("struct hud64_face { uint8_t kind, padx, pady, space, gap, cap, ry; const uint8_t *map; const struct hud64_glyph *glyphs;")
    w("\t/* the outline's offset draws (the fewest that give the full ring's pixels) */")
    w("\tuint8_t ring_count; int8_t ring[14][2]; uint8_t glyph_count; };")
    for i, f in enumerate(data.faces):
        chars = list(f["glyphs"].keys())
        w(f"static const struct hud64_glyph hud64_glyphs_{i}[{len(chars)}] = {{ /* {f['name']} */")
        for ch in chars:
            g = f["glyphs"][ch]
            w(f"\t{{{g['tex']}, {g['x']}, {g['y']}, {g['w']}, {g['h']}, {g['adv']}, {g['oy']}}}, /* {ch} */")
        w("};")
    # (memory: a face's character map -- 96 bytes, a glyph's index a character -- shared by every face with the
    # same characters, not one a face: 24 faces, a few maps)
    maps, map_of = [], []
    for f in data.faces:
        mp = [0] * 96
        for j, ch in enumerate(f["glyphs"].keys()):
            mp[ord(ch) - 32] = j + 1
        if mp not in maps:
            maps.append(mp)
        map_of.append(maps.index(mp))
    for k, mp in enumerate(maps):
        w(f"static const uint8_t hud64_map_{k}[96] = {{{', '.join(map(str, mp))}}};")
    w("static const struct hud64_face hud64_faces[HUD64_FACE_COUNT] = {")
    for i, f in enumerate(data.faces):
        ring = ", ".join("{%d, %d}" % tuple(o) for o in f["ring"]) or "{0, 0}"
        w(f"\t{{{f['kind']}, {f['pad'][0]}, {f['pad'][1]}, {f['space']}, {f['gap']}, {f['cap']}, {f['ry']}, "
          f"hud64_map_{map_of[i]}, hud64_glyphs_{i}, {len(f['ring'])}, {{{ring}}}, {len(f['glyphs'])}}}, "
          f"/* {f['name']} */")
    w("};")
    w("")
    w("/* a texture drawn dx, dy of its own texels from where it is placed (a meter's cropped ramp rows, a part")
    w("   that reaches left of its panel's origin) */")
    w("struct hud64_ref { uint16_t tex; uint8_t scale; int8_t dx, dy; uint8_t t0, h; };")
    w("struct hud64_cell { uint16_t tex; uint8_t x, y, w, h, scale; };")
    w("/* a meter's parts, drawn in turn at one threshold: the body in the meter's colour, the tint (30% toward white)")
    w("   and the shade (65% of it) -- the vitals' 3-tone ramp; each its texels' own order -- and the top edge in")

    w("   its highlight colour */")
    w("struct hud64_meter { struct hud64_ref body, highlight, tint, shade; };")
    w("enum { " + ", ".join(f"HUD64_REF_{k.upper()}" for k in REFS) + ", HUD64_REF_COUNT };")
    w("enum { " + ", ".join(f"HUD64_ICON_{k.upper()}" for k in ICON_KEYS) + ", HUD64_ICON_COUNT };")
    w("enum { " + ", ".join(f"HUD64_METER_{k.upper()}" for k in METER_KEYS) + ", HUD64_METER_COUNT };")
    lay_keys = list(data.setdata["s240"]["layout"].keys())
    w("struct hud64_layout {")
    for k in lay_keys:
        v = data.setdata["s240"]["layout"][k]
        if isinstance(v, list):
            w(f"\tint16_t {k}[{len(v)}];")
        else:
            w(f"\tint16_t {k};")
    w("};")
    w("struct hud64_set {")
    w("\tuint8_t s, ws;")
    w(f"\tuint8_t face[{len(FACE_KEYS)}], wface[{len(FACE_KEYS)}]; /* {', '.join(FACE_KEYS)}; the 640x240 faces (0xFF: none) */")
    w("\tstruct hud64_ref ref[HUD64_REF_COUNT];")
    w("\tstruct hud64_cell icon[HUD64_ICON_COUNT];")
    w("\tstruct hud64_meter meter[HUD64_METER_COUNT];")
    w("\tint16_t panel[4][2]; /* weapon, grenade, ammo4, gren4 */")
    w("\tint16_t shield_size[2], shield_s_size[2];")
    w("\tstruct hud64_layout layout;")
    w("};")
    w("enum { " + ", ".join(f"HUD64_SET_{k[1:].upper()}" for k in SET_KEYS) + " };")
    w("static const struct hud64_set hud64_sets[4] = {")
    for skey in SET_KEYS:
        d = data.setdata[skey]
        refs = ", ".join(f"{{{t}, {sc}, {d['ref_off'].get(k, (0, 0))[0]}, {d['ref_off'].get(k, (0, 0))[1]}, "
                         f"{d.get('ref_part', {}).get(k, (0, 0))[0]}, {d.get('ref_part', {}).get(k, (0, 0))[1]}}}"
                         for k, (t, sc) in ((k, d["refs"][k]) for k in REFS))
        faces = ", ".join(str(d["faces"][k]) for k in FACE_KEYS)
        wfaces = ", ".join(str(d["wfaces"].get(k, 255)) for k in FACE_KEYS)
        icons = ", ".join("{%d, %d, %d, %d, %d, %d}" % d["icons"][k] for k in ICON_KEYS)
        meters = []
        for k in METER_KEYS:
            body, hl = d["meters"][k]
            tint, shade = d["meter_ramp"].get(k, (None, None))
            part = lambda r: (f"{{{r[0]}, {r[1]}, {r[2] if len(r) > 2 else 0}, {r[3] if len(r) > 3 else 0}, "
                              f"{r[4] if len(r) > 4 else 0}, {r[5] if len(r) > 5 else 0}}}") if r \
                else "{0xFFFF, 0, 0, 0, 0, 0}"
            meters.append(f"{{{part(body)}, {part(hl)}, {part(tint)}, {part(shade)}}}")
        pan = ", ".join("{%d, %d}" % tuple(d["panels"][k]) for k in ("weapon", "grenade", "ammo4", "gren4"))
        lay = []
        for k in lay_keys:
            v = d["layout"][k]
            lay.append("{" + ", ".join(map(str, v)) + "}" if isinstance(v, list) else str(int(v)))
        w(f"\t{{ /* {skey} */ {d['s']}, {d['ws']}, {{{faces}}}, {{{wfaces}}},")
        w(f"\t\t{{{refs}}},")
        w(f"\t\t{{{icons}}},")
        w(f"\t\t{{{', '.join(meters)}}},")
        w(f"\t\t{{{pan}}}, {{{d['shield_size'][0]}, {d['shield_size'][1]}}}, "
          f"{{{d['shield_s_size'][0]}, {d['shield_s_size'][1]}}},")
        w(f"\t\t{{{', '.join(lay)}}}}},")
    w("};")
    w("/* each player count's arrangement: which variant of each part (make.py's compose) */")
    w("struct hud64_arrangement { uint8_t split, sensor_label, score_words, prompt_first_form, message_lines; };")
    w("static const struct hud64_arrangement hud64_arrangements[5] = {")
    keys = ("split", "sensor_label", "score_words", "prompt_first_form", "message_lines")
    for players, ar in data.arrangements.items():
        w(f"\t[{players}] = {{{', '.join(str(ar[k]) for k in keys)}}},")
    w("};")
    # the weapons' names: the short form of the prompt
    w("struct hud64_weapon_name { const char *name, *short_name; };")
    w("static const struct hud64_weapon_name hud64_weapon_names[] = {")
    for full, short in m.SHORT_NAME.items():
        w(f'\t{{"{full}", "{short}"}},')
    w("};")
    w("\n#endif")
    return L


def tables_id(data, lines):
    """16 bits of the SHA-1 of the header's text and every texture's pixels"""
    h = hashlib.sha1("\n".join(lines).encode())
    for t in data.textures:
        h.update(t.fmt.encode() + struct.pack(">HH", t.w, t.h) + t.pixels())
    return int(h.hexdigest()[:4], 16)


def dump_json(data, path):
    m = data.make
    alphas = {"EDGE": m.EDGE_ALPHA, "SHIELD": 0.80, "HEALTH": 0.85, "BAR": 0.9,
              "SLOTS": float(np.round(0.9 * 15) / 15)}
    out = {"tables_id": data.tables_id, "faces": data.faces, "sets": data.setdata, "alphas": alphas,
           "heat_gradient": list(data.heat),
           "arrangements": data.arrangements,
           "textures": [{"name": t.name, "fmt": t.fmt, "w": t.w, "h": t.h, "group": t.group} for t in data.textures]}
    Path(path).write_text(json.dumps(out, indent=1, default=lambda o: o if not isinstance(o, np.integer) else int(o)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--design", default=str(Path(__file__).resolve().parent),
                    help="the folder holding hud64_design.py (default: this script's)")
    ap.add_argument("--fs")
    ap.add_argument("--header")
    ap.add_argument("--json")
    a = ap.parse_args()
    data = Data(a.design)
    lines = header_lines(data)
    data.tables_id = tables_id(data, lines)
    if a.fs:
        sizes = write_fs(data, a.fs)
        print("hud64 groups:", ", ".join(f"{g} {n} B" for g, n in sizes.items()))
    if a.header:
        Path(a.header).write_text("\n".join(f"#define HUD64_TABLES_ID 0x{data.tables_id:04X}" if x == TABLES_ID_LINE
                                            else x for x in lines) + "\n")
        print("header:", a.header)
    if a.json:
        dump_json(data, a.json)
    print(f"{len(data.textures)} textures, {len(data.faces)} faces, tables id 0x{data.tables_id:04X}")


if __name__ == "__main__":
    main()
