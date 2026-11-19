#!/usr/bin/env python3
"""HUD STYLE 64's reticles: Halo's crosshairs redrawn as 1-bit pixel art for
the N64's 320x240 grid, made by this code alone (no image file is read).
tools/n64/hud64/hud64_gen.py imports it and stores the art with the rest of
the design's textures.

Why redrawn: Halo's reticle art is drawn for 640x480. The converter halves it
with a 2x2 mean, so a 1-texel line becomes a half-transparent one, the art
loses its mirror symmetry where its axis runs through a texel, and split
screen halves it again with the RDP's filter, which skips texels. Here every
reticle is drawn for the grid it is shown on: 1-pixel lines at full opacity,
odd sizes with a centre pixel, and a separate small version for 2-4 players
that is drawn 1:1.

Units: N64 pixels of the 320x240 HUD (one pixel = 2x2 of Halo's 640x480
layout), x right, y down, from the aim pixel: the pixel whose top-left
corner is the view's projection centre. Sets:
  full      1 player (Halo's 1-player size on the screen)
  split     2-4 players, about half size, its own drawing
  full_ws   WIDESCREEN 1 player: the same drawing with x times 0.75
  split_ws  WIDESCREEN 2-4 players
HUD RESOLUTION HIGH DETAIL draws the 320 art 2x across; the 480-line modes
2x both ways.
"""
import math
import sys

import numpy as np

sys.dont_write_bytecode = True

WS = 0.75
SETS = ("full", "split", "full_ws", "split_ws")

# the zoom labels: HUD 64's 7-row face (hud64_design.py FONT7's "2", "1", "0")
# and a 5-row "x"
GLYPHS = {
    "2": [".####.", "##..##", "....##", "...##.", "..##..", ".##...", "######"],
    "1": [".##", "###", ".##", ".##", ".##", ".##", ".##"],
    "0": [".###.", "##.##", "##.##", "##.##", "##.##", "##.##", ".###."],
    "x": ["......", "......", "##..##", ".####.", "..##..", ".####.", "##..##"],
}

# ---------------------------------------------------------------- the drawings
# Primitives, coordinates from the aim pixel, angles in degrees (0 right, 90 up):
#   ["arc", cx, cy, r, a0, a1]      1-pixel arc (WIDESCREEN: an ellipse), the diamond rule
#   ["ring", cx, cy, r(, rx_ws)]    the whole circle (rx_ws: its WIDESCREEN radius across, where
#                                   r x 0.75 rounded half down is not the one)
#   ["line", x0, y0, x1, y1]        1-pixel line, the diamond rule
#   ["rect", x0, y0, x1, y1]        filled, inclusive
#   ["poly", [[x, y], ...]]         filled polygon (8x8 samples a pixel, half covered)
#   ["lune", [cx, cy, r], [cx, cy, r]]  inside the first circle, outside the second
#   ["wedge", tx, ty, bx, by, w]    a spike from its tip to its base (w wide) and its axis
#   ["box3", cx, cy]                the 3x3 ring
#   ["px", [[x, y], ...]]           pixels (x squeezed in WIDESCREEN); ["clear", ...] unset
#   ["pxa", [[x, y], ...]]          pixels at exactly these places (WIDESCREEN art)
# sym: "xy" mirrored both ways about the aim pixel (stored as a quarter, drawn
# as 4 rectangles), "x" left-right only (a half, 2 rectangles)


def arcs4(r, half):
    return [["arc", 0, 0, r, c - half, c + half] for c in (0, 90, 180, 270)]


DOTS = [["px", [[x, -1] for x in (-6, -2, 2, 6)] + [[x + d, 0] for x in (-6, -2, 2, 6) for d in (-1, 0, 1)]]]

R = {
    # four arcs of +-34 degrees with gaps on the diagonals, 2-pixel ticks out;
    # split: a closed ring with 1-pixel ticks
    "assault_rifle": dict(sym="xy",
                          full=arcs4(11, 34) + [["line", 12, 0, 13, 0], ["line", 0, -12, 0, -13],
                                                ["line", -12, 0, -13, 0], ["line", 0, 12, 0, 13]],
                          split=arcs4(5, 38) + [["line", 6, 0, 6, 0], ["line", 0, -6, 0, -6],
                                                ["line", -6, 0, -6, 0], ["line", 0, 6, 0, 6]]),
    # ring with three 1-pixel ticks in and the 2-pixel bottom tick
    "pistol": dict(sym="x",
                   full=[["ring", 0, 0, 6], ["px", [[0, -5], [-5, 0], [5, 0], [0, 5], [0, 4]]]],
                   split=[["ring", 0, 0, 3], ["px", [[0, 2]]]],
                   split_ws=[["ring", 0, 0, 3]]),
    # the small ring and its dot: not halved (Halo's "don't scale size"), not squeezed
    "sniper_rifle": dict(sym="xy", ws_unsqueezed=True,
                         full=[["ring", 0, 0, 2], ["px", [[0, 0]]]],
                         split=[["ring", 0, 0, 2], ["px", [[0, 0]]]]),
    # (the Wraith's too) ring, dashed line, ladder ticks; Halo's tiny labels left out
    "rocket_launcher": dict(sym="xy",
                            full=[["ring", 0, 0, 16], ["rect", -3, 0, 3, 0], ["rect", -10, 0, -7, 0],
                                  ["rect", 7, 0, 10, 0], ["rect", -16, 0, -13, 0], ["rect", 13, 0, 16, 0],
                                  ["rect", -3, -5, 3, -5], ["rect", -3, 5, 3, 5], ["rect", -3, -11, 3, -11],
                                  ["rect", -3, 11, 3, 11]],
                            split=[["ring", 0, 0, 8], ["rect", -1, 0, 1, 0], ["rect", -8, 0, -5, 0],
                                   ["rect", 5, 0, 8, 0], ["rect", -1, -4, 1, -4], ["rect", -1, 4, 1, 4]]),
    # (the PC flamethrower's too) a plain ring
    "shotgun": dict(sym="xy", full=[["ring", 0, 0, 14]], split=[["ring", 0, 0, 7]]),
    # four bold wedges on the diagonals pointing in
    "plasma_rifle": dict(sym="xy",
                         full=[["wedge", -4.6, -4.6, -10.4, -10.4, 3.0]],
                         split=[["wedge", -2.2, -2.2, -5.4, -5.4, 2.6]],
                         full_ws=[["wedge", -4.6, -4.6, -10.0, -10.0, 3.0]]),
    # three wedges pointing in: a 3-pixel head over a stem on top, two at 210 and 330 degrees
    "plasma_pistol": dict(sym="x",
                          full=[["wedge", 0, -6.6, 0, -14.4, 3.4], ["wedge", -5.8, 3.35, -11.9, 6.9, 2.8],
                                ["clear", [[-1, -13], [1, -13], [-1, -12], [1, -12], [0, -7]]]],
                          full_ws=[["wedge", 0, -6.6, 0, -14.4, 3.4], ["wedge", -5.8, 3.35, -11.9, 6.9, 3.4],
                                   ["clear", [[-1, -13], [1, -13], [-1, -12], [1, -12], [0, -7], [0, -8]]]],
                          split=[["wedge", 0, -3.2, 0, -7.2, 2.2], ["wedge", -2.9, 1.7, -5.6, 3.3, 2.2],
                                 ["clear", [[0, -3], [0, -4]]]]),
    # two crescents with a small triangle inside each
    "needler": dict(sym="xy",
                    full=[["lune", [-9.47, 0, 4.53], [-5.94, 0, 6.06]],
                          ["px", [[-10, -1], [-10, 0], [-10, 1], [-9, 0]]]],
                    full_ws=[["lune", [-9.47, 0, 4.53], [-5.94, 0, 6.06]], ["px", [[-9.4, -1], [-9.4, 0], [-9.4, 1]]]],
                    split=[["px", [[-6, -2], [-7, -1], [-7, 0], [-7, 1], [-6, 2], [-5, 0]]]],
                    split_ws=[["pxa", [[-4, -2], [-5, -1], [-5, 0], [-5, 1], [-4, 2], [-3, 0]]]]),
    # a 3x3 X in every set
    "unarmed": dict(sym="xy", ws_unsqueezed=True,
                    full=[["px", [[0, 0], [-1, -1], [1, -1], [-1, 1], [1, 1]]]],
                    split=[["px", [[0, 0], [-1, -1], [1, -1], [-1, 1], [1, 1]]]]),
    # two rings and four spokes between them
    "warthog_gunner": dict(sym="xy",
                           full=[["ring", 0, 0, 13], ["ring", 0, 0, 9], ["rect", -12, 0, -10, 0],
                                 ["rect", 10, 0, 12, 0], ["rect", 0, -12, 0, -10], ["rect", 0, 10, 0, 12]],
                           split=[["ring", 0, 0, 6, 5], ["ring", 0, 0, 4], ["px", [[-5, 0], [5, 0], [0, -5], [0, 5]]]]),
    # a spike down, two small rings, two crescents
    "ghost": dict(sym="x",
                  full=[["wedge", 0, -4.6, 0, -11.2, 3.4], ["ring", -5, 1, 3],
                        ["lune", [-9.47, 1, 4.53], [-7.1, 1, 4.9]]],
                  split=[["px", [[0, -6], [0, -5], [0, -4], [0, -3]]], ["box3", -3, 0],
                         ["px", [[-6, -2], [-7, -1], [-7, 0], [-7, 1], [-6, 2]]]],
                  split_ws=[["pxa", [[0, -6], [0, -5], [0, -4], [0, -3], [-3, 0], [-1, 0], [-2, -1], [-2, 1],
                                     [-4, -2], [-5, -1], [-5, 0], [-5, 1], [-4, 2]]]]),
    # brackets with a gap, inward triangles, a ladder outside each
    "scorpion": dict(sym="xy",
                     full=[["rect", -7, -12, -7, -4], ["rect", -7, -12, -2, -12],
                           ["poly", [[-7.5, -2.6], [-7.5, 2.6], [-3.6, 0]]]] +
                          [["rect", -12, y, -10, y] for y in (-9, -6, -3, 0, 3, 6, 9)],
                     split=[["rect", -3, -6, -3, -2], ["rect", -3, -6, -2, -6],
                            ["px", [[-2, -1], [-2, 0], [-2, 1], [-1, 0]]]] +
                           [["rect", -6, y, -5, y] for y in (-3, 0, 3)],
                     split_ws=[["rect", -2.67, -6, -2.67, -2], ["px", [[-2.67, -6], [-1.33, -6], [-1.33, 0]]]] +
                              [["px", [[-5.33, y]]] for y in (-3, 0, 3)]),
    # a spike down, two rings with thick tails
    "banshee": dict(sym="x",
                    full=[["wedge", 0, -4.0, 0, -10.4, 3.4], ["ring", -5, 1, 3],
                          ["poly", [[-6.6, 2.8], [-8.0, 4.5], [-11.9, 8.0], [-11.2, 4.9]]], ["px", [[-7, 2], [-6, 1]]]],
                    split=[["wedge", 0, -2.0, 0, -4.6, 2.2], ["box3", -3, 1], ["line", -4, 2, -5, 3]],
                    split_ws=[["wedge", 0, -2.0, 0, -4.6, 2.2], ["pxa", [[-3, 1], [-1, 1], [-2, 0], [-2, 2], [-4, 2]]]]),
    # the Warthog driver's mark: a 9-pixel "^", not halved
    "warthog_driver": dict(sym="x",
                           full=[["px", [[-1, -1], [0, -1], [1, -1], [-2, 0], [-3, 0], [2, 0], [3, 0], [-4, 1], [4, 1]]]],
                           split=[["px", [[-1, -1], [0, -1], [1, -1], [-2, 0], [-3, 0], [2, 0], [3, 0], [-4, 1],
                                          [4, 1]]]]),
    # the overheat row: four small triangles every 4 pixels (k dots: the first 4k - 1
    # columns), not halved, not squeezed
    "dots": dict(sym="whole", ws_unsqueezed=True, full=DOTS, split=DOTS),
    # PC: the fuel rod gun: two crescents and a ladder of lines
    "fuel_rod": dict(sym="xy",
                     full=[["lune", [1.02, 0, 16.02], [15.54, 0, 27.54]],
                           ["rect", -1, -9, 1, -9], ["rect", -3, -6, 3, -6], ["rect", -4, -3, 4, -3], ["rect", -4, 0, 4, 0]],
                     full_ws=[["px", [[-10.67, -11]]], ["lune", [1.02, 0, 16.02], [15.54, 0, 27.54]],
                              ["rect", -1, -9, 1, -9], ["rect", -3, -6, 3, -6], ["rect", -4, -3, 4, -3],
                              ["rect", -4, 0, 4, 0]],
                     split=[["arc", 2, 0, 9, 145, 215], ["rect", -1, -4, 1, -4], ["rect", -2, -2, 2, -2],
                            ["rect", -2, 0, 2, 0]]),
    # PC: the rocket Warthog's gunner, at its tag's 0.8 size: two rings, spokes, a dash and ticks
    "rocket_warthog": dict(sym="xy",
                           full=[["ring", 0, 0, 12], ["ring", 0, 0, 8], ["rect", -11, 0, -9, 0], ["rect", 9, 0, 11, 0],
                                 ["rect", 0, -11, 0, -9], ["rect", 0, 9, 0, 11], ["rect", -1, 0, 1, 0],
                                 ["rect", -2, -4, 2, -4], ["rect", -2, 4, 2, 4]],
                           split=[["ring", 0, 0, 6, 5], ["ring", 0, 0, 4],
                                  ["px", [[-5, 0], [5, 0], [0, -5], [0, 5], [0, 0]]]]),
    # the zoom labels, centred on Halo's place for them, not squeezed
    "label_2x": dict(sym="whole", ws_unsqueezed=True, text="2x"),
    "label_10x": dict(sym="whole", ws_unsqueezed=True, text="10x"),
}
ART = tuple(R)

# Halo's items the art stands for: (bitmap, sequence, first frame, last frame,
# Halo's offset of the item in its tag: every stock reticle's is 0, 0) ->
# (art, the art's offset from the aim pixel). Every other item -- a Custom
# Edition map's own reticles among them, and a stock sequence a map moved
# away from the aim point -- is drawn as Halo draws it.
RETICLES = "ui\\hud\\bitmaps\\combined\\hud_reticles"
WARNINGS = "ui\\hud\\bitmaps\\combined\\hud_reticle_warnings"
SCOPE = "ui\\hud\\bitmaps\\sniper\\hud_reticles_scope"
LINES = "ui\\hud\\bitmaps\\sniper\\sniper_scope_crosshairs2"
LINES_SM = "ui\\hud\\bitmaps\\sniper\\sniper_scope_crosshairs_sm"
PISTOL_MASK = "ui\\hud\\bitmaps\\pistol\\pistol_scope_mask"
PISTOL_MASK_SPLIT = "ui\\hud\\bitmaps\\pistol\\pistol_scope_mask split"
SNIPER_MASK = "ui\\hud\\bitmaps\\sniper\\sniper_scope_mask2"
SNIPER_MASK_SPLIT = "ui\\hud\\bitmaps\\sniper\\sniper_scope_mask_sm"
BITMAPS = (RETICLES, WARNINGS, SCOPE, LINES, LINES_SM, PISTOL_MASK, PISTOL_MASK_SPLIT, SNIPER_MASK, SNIPER_MASK_SPLIT)
# (other names of the same art: on the PC Expansion cartridge a weapon HUD
# imported from the PC maps keeps the PC's own reticle sheet, its sequences
# 0-14 the same and 15-16 added, under this name -- tools/n64/merge.py)
ALIASES = {RETICLES + "\\pc_expansion": RETICLES}
ITEMS = [
    (RETICLES, 0, 0, 0, (0, 0), "assault_rifle", (0, 0)),
    (RETICLES, 1, 0, 0, (0, 0), "banshee", (0, 0)),
    (RETICLES, 2, 0, 0, (0, 0), "warthog_gunner", (0, 0)),
    (RETICLES, 3, 0, 0, (0, 0), "plasma_pistol", (0, 0)),
    (RETICLES, 4, 0, 0, (0, 0), "plasma_rifle", (0, 0)),
    (RETICLES, 6, 0, 0, (0, 0), "needler", (0, 0)),
    (RETICLES, 7, 0, 0, (0, 0), "unarmed", (0, 0)),
    (RETICLES, 8, 0, 0, (0, 0), "pistol", (0, 0)),
    (RETICLES, 9, 0, 0, (0, 0), "rocket_launcher", (0, 0)),       # the Wraith's too
    (RETICLES, 10, 0, 0, (0, 0), "shotgun", (0, 0)),              # the PC flamethrower's too
    (RETICLES, 11, 0, 0, (0, 0), "scorpion", (0, 0)),
    (RETICLES, 13, 0, 0, (0, 0), "ghost", (0, 0)),
    (RETICLES, 14, 0, 0, (0, -40), "warthog_driver", (0, -21)),  # (a seat's HUD, hud_unit.c)
    (RETICLES, 15, 0, 0, (0, 0), "fuel_rod", (0, 0)),             # (PC Expansion)
    (RETICLES, 16, 0, 0, (0, 0), "rocket_warthog", (0, 0)),       # (PC Expansion)
    (SCOPE, 0, 0, 0, (0, 0), "sniper_rifle", (0, 0)),
    # the magnification: frame 0 2x, frame 1 10x, at Halo's place (its offset
    # halved, toward zero): pistol and rocket launcher, sniper rifle; split screen
    (SCOPE, 1, 0, 0, (105, 105), "label_2x", (52, 52)),
    (SCOPE, 1, 0, 0, (67, 67), "label_2x", (33, 33)),
    (SCOPE, 1, 0, 0, (150, 104), "label_2x", (75, 52)),
    (SCOPE, 1, 1, 1, (150, 104), "label_10x", (75, 52)),
    (SCOPE, 1, 0, 0, (95, 60), "label_2x", (47, 30)),
    (SCOPE, 1, 1, 1, (95, 60), "label_10x", (47, 30)),
    # the overheat dots (frame k: k dots): Scorpion, Banshee, rocket Warthog
    (WARNINGS, 9, 0, 4, (0, 30), "dots", (0, 15)),
    (WARNINGS, 9, 0, 4, (0, 15), "dots", (0, 8)),
    (WARNINGS, 9, 0, 4, (0, 32), "dots", (0, 16)),
]
# (the sniper's dark lines -- LINES, LINES_SM -- are drawn with its zoom
# window, by the scope's mask: zoom_pieces)

# ---------------------------------------------------------------- the rasteriser
CONT = {"arc", "ring", "line"}


def rnd(v):
    """round half away from zero: the same for v and -v (mirror-safe)"""
    return int(math.floor(abs(v) + 0.5)) * (1 if v >= 0 else -1)


def diamond(points):
    """pixels whose diamond (|dx| + |dy| <= 0.5) a point falls in: thin 8-connected curves"""
    out = set()
    for x, y in points:
        px, py = rnd(x), rnd(y)
        if abs(x - px) + abs(y - py) <= 0.5 + 1e-9:
            out.add((px, py))
    return out


def arc_pixels(cx, cy, r, a0, a1, sx, rx_ws=None):
    """WIDESCREEN: the ellipse's radius across r x 0.75 rounded half down (or rx_ws), its centre rounded"""
    rx, cxs = (r, cx) if sx == 1 else (rx_ws or math.floor(r * sx + 0.5 - 1e-6), rnd(cx * sx))
    n = max(64, int(abs(a1 - a0) / 360 * 2 * math.pi * r * 64))
    ts = np.radians(np.linspace(a0, a1, n))
    return diamond(zip(cxs + rx * np.cos(ts), cy - r * np.sin(ts)))


def line_pixels(x0, y0, x1, y1, sx):
    x0, x1 = rnd(x0 * sx), rnd(x1 * sx)
    n = int(max(abs(x1 - x0), abs(y1 - y0)) * 32) + 2
    ts = np.linspace(0, 1, n)
    return diamond(zip(x0 + (x1 - x0) * ts, y0 + (y1 - y0) * ts))


SUB = (np.arange(8) + 0.5) / 8 - 0.5


def coverage(fn, ext):
    """pixels in [-ext, ext]^2 whose 8x8 samples of fn are at least half inside"""
    out = set()
    for py in range(-ext, ext + 1):
        Y = py + SUB[:, None] + 0 * SUB[None, :]
        for px in range(-ext, ext + 1):
            X = px + SUB[None, :] + 0 * SUB[:, None]
            if fn(X, Y).mean() >= 0.5:
                out.add((px, py))
    return out


def inside_poly(pts):
    pts = np.asarray(pts, float)

    def fn(X, Y):
        ins = np.zeros(X.shape, bool)
        n = len(pts)
        for i in range(n):
            x0, y0 = pts[i]
            x1, y1 = pts[(i + 1) % n]
            cond = (y0 > Y) != (y1 > Y)
            xin = x0 + (Y - y0) * (x1 - x0) / (y1 - y0 + 1e-12)
            ins ^= cond & (X < xin)
        return ins
    return fn


def draw(prims, sx, sym):
    """the set of pixels a drawing sets"""
    pix = set()
    for p in prims:
        k = p[0]
        if k == "arc":
            s = arc_pixels(p[1], p[2], p[3], p[4], p[5], sx)
        elif k == "ring":
            s = arc_pixels(p[1], p[2], p[3], 0, 360, sx, p[4] if len(p) > 4 else None)
        elif k == "line":
            s = line_pixels(p[1], p[2], p[3], p[4], sx)
        elif k == "rect":
            xa, xb = sorted((rnd(p[1] * sx), rnd(p[3] * sx)))
            s = {(x, y) for x in range(xa, xb + 1) for y in range(p[2], p[4] + 1)}
        elif k == "poly":
            s = coverage(inside_poly([(x * sx, y) for x, y in p[1]]), 24)
        elif k == "wedge":
            tx, ty, bx, by, w = p[1:6]
            dx, dy = bx - tx, by - ty
            n = math.hypot(dx, dy)
            qx, qy = -dy / n * w / 2, dx / n * w / 2
            s = coverage(inside_poly([(tx * sx, ty), ((bx + qx) * sx, by + qy), ((bx - qx) * sx, by - qy)]), 24)
            s |= line_pixels(tx, ty, bx, by, sx)
        elif k == "box3":
            cx, cy = rnd(p[1] * sx), p[2]
            s = {(cx + i, cy + j) for i in (-1, 0, 1) for j in (-1, 0, 1) if i or j}
        elif k == "lune":
            (ax, ay, ar), (bx, by, br) = p[1], p[2]
            s = coverage(lambda X, Y: (((X / sx - ax) ** 2 + (Y - ay) ** 2) <= ar * ar) &
                         (((X / sx - bx) ** 2 + (Y - by) ** 2) > br * br), 24)
        elif k == "px":
            s = {(rnd(x * sx), y) for x, y in p[1]}
        elif k == "pxa":
            s = {(x, y) for x, y in p[1]}
        elif k == "clear":
            pix -= {(rnd(x * sx), y) for x, y in p[1]}
            continue
        else:
            raise ValueError(k)
        if sym in ("xy", "x"):
            s = s | {(-x, y) for x, y in s}
        if sym == "xy":
            s = s | {(x, -y) for x, y in s}
        pix |= s
    return pix


def to_array(pix):
    """the tight bitmap and the aim pixel's place in it"""
    xs = [x for x, _ in pix]
    ys = [y for _, y in pix]
    x0, x1, y0, y1 = min(xs), max(xs), min(ys), max(ys)
    a = np.zeros((y1 - y0 + 1, x1 - x0 + 1), bool)
    for x, y in pix:
        a[y - y0, x - x0] = True
    return a, (-x0, -y0)


def text(keys):
    """glyphs 1 pixel apart; the aim pixel: the middle column (rounded left), the fourth row"""
    rows = len(GLYPHS[keys[0]])
    w = sum(len(GLYPHS[k][0]) + 1 for k in keys) - 1
    a = np.zeros((rows, w), bool)
    x = 0
    for k in keys:
        g = GLYPHS[k]
        for j, row in enumerate(g):
            for i, c in enumerate(row):
                a[j, x + i] = c == "#"
        x += len(g[0]) + 1
    return a, (w // 2, 3)


def art(name, setname):
    """(bitmap, (x, y) of the aim pixel in it, sym) of a drawing in a set"""
    d = R[name]
    if "text" in d:
        a, c = text(["1", "0", "x"] if d["text"] == "10x" else ["2", "x"])
        return a, c, d["sym"]
    base = "split" if setname.startswith("split") else "full"
    prims = d.get(setname) or d.get(base)
    sx = WS if setname.endswith("_ws") and not d.get("ws_unsqueezed") else 1.0
    a, c = to_array(draw(prims, sx, d["sym"]))
    return a, c, d["sym"]


def stored(a, c, sym):
    """the part kept: the top-left quarter (centre row and column included), the left half, or all"""
    if sym in ("x", "xy"):
        assert a.shape[1] == 2 * c[0] + 1 and (a == a[:, ::-1]).all()
    if sym == "xy":
        assert a.shape[0] == 2 * c[1] + 1 and (a == a[::-1, :]).all()
        return a[:c[1] + 1, :c[0] + 1]
    if sym == "x":
        return a[:, :c[0] + 1]
    return a


# ---------------------------------------------------------------- the zoom views
# Pistol and rocket launcher (2x): a soft circle. One 32x32 I4 quarter (texel
# (0, 0) at the circle's centre), its value the darkening's alpha: 0 inside,
# 0.55 outside, 0.8 on a thin rim, the edge 8x8 samples a texel. Drawn as 4
# mirrored rectangles on the projection centre: 2x with the RDP's bilinear
# filter at 1 player (radius 61), 1:1 at 2-4 players (30.5); the rest of the
# view darkened at its outer value by plain rectangles. No blur outside.
CIRCLE_N, CIRCLE_R, CIRCLE_RIM, CIRCLE_OUT, CIRCLE_IN_RIM = 32, 30.5, 0.6, 0.55, 0.8


def circle_quarter():
    sub = (np.arange(8) + 0.5) / 8
    q = np.zeros((CIRCLE_N, CIRCLE_N))
    for j in range(CIRCLE_N):
        for i in range(CIRCLE_N):
            r = np.hypot(i + sub[None, :], j + sub[:, None])
            q[j, i] = np.where(r < CIRCLE_R, 0.0, np.where(r < CIRCLE_R + CIRCLE_RIM, CIRCLE_IN_RIM, CIRCLE_OUT)).mean()
    q = np.round(q * 15) / 15
    # (the last row and column, next to the plain rectangles, at their value: no darker notch where the rim
    # touches them)
    q[-1, :] = q[:, -1] = np.round(CIRCLE_OUT * 15) / 15
    return q


# Sniper rifle (2x, 10x): a window with rounded corners around the projection
# centre (half size at 2-4 players), the view outside it darkened to 50%, a
# 1-pixel outline around it to 30%; inside, Halo's dark lines (black at its
# alpha 0.47): posts across from 58 to 78 pixels out, thin lines to 34, a thin
# line down from 13 to 36 and a post to the window's bottom, no post above;
# and two dark side frames, a bar 4 pixels inside each side with ticks every
# 4 lines. Its blue ladders are the weapon HUD's own, drawn by Halo.
SNIPER_DARK_OUT, SNIPER_DARK_OUTLINE, SNIPER_DARK_LINES = 0.5, 0.7, 120 / 255


def sniper_geometry(s):
    """the window's half sizes and corner radius, the lines' rectangles (x0, y0, x1, y1 inclusive, from the aim
    pixel) and the side frame's strip (left side: the bar in column 0, the ticks to its right), at size s (1:
    1 player, 0.5: 2-4 players)"""
    hw, hh, r = int(round(97 * s)), int(round(60 * s)), max(3, int(round(7 * s)))
    a1, a2, a3 = int(78 * s), int(58 * s), int(34 * s)
    b0, b1, b2 = int(13 * s), int(36 * s), int(37 * s)
    lines = [(-a1, -1, -a2, 1), (a2, -1, a1, 1), (-a2 + 1, 0, -a3, 0), (a3, 0, a2 - 1, 0),
             (0, b0, 0, min(b1, b2 - 1)), (-1, b2, 1, hh)]
    top, step = int(50 * s), max(2, int(4 * s))
    strip = np.zeros((2 * top + 1, 4), bool)
    strip[:, 0] = True
    for k, y in enumerate(range(-int(48 * s), int(48 * s) + 1, step)):
        strip[y + top, 1:4 if k % 2 == 0 else 3] = True
    # (the strip is the ladder's period read 4 // step rows a line from its first row)
    ladder = sniper_ladder()
    assert (strip == (ladder[(np.arange(2 * top + 1) * (4 // step)) % 8] > 0)).all(), s
    return dict(hw=hw, hh=hh, r=r, lines=lines, strip=strip, bar=hw - 4, top=top, step=4 // step)


def squeeze(v):
    """a distance across in WIDESCREEN: 0.75 of it, rounded half up"""
    return int(math.floor(abs(v) * WS + 0.5)) * (1 if v >= 0 else -1)


def zoom_view(s, ws):
    """what the C code draws of a zoomed view at size s (1 player 1, 2-4 players 0.5), WIDESCREEN or not, in grid
    pixels from the aim pixel: the sniper window's outline rectangle (left, top, right, bottom, exclusive), its
    corner squares' size, the rectangles darkened (outline edges at the outline's alpha, dark lines at Halo's, x0 y0
    x1 y1 exclusive), the side frames' bar, the ladder's half height and rows a line; the circle's half size. In
    WIDESCREEN every distance across is squeezed as the right side's, the left side mirrors it, so the parts keep
    their widths and never overlap"""
    g = sniper_geometry(s)
    sq = squeeze if ws else (lambda v: v)
    hw, hh, n = sq(g["hw"] + 1), g["hh"] + 1, g["r"] + 1
    cw = sq(n)
    box = (-hw, -hh, hw + 1, hh + 1)
    rects = [(-hw + cw, -hh, hw + 1 - cw, -hh + 1, 1), (-hw + cw, hh, hw + 1 - cw, hh + 1, 1),
             (-hw, -hh + n, -hw + 1, hh + 1 - n, 1), (hw, -hh + n, hw + 1, hh + 1 - n, 1)]
    for x0, y0, x1, y1 in g["lines"]:
        if x0 >= 0:                   # (the right side's, squeezed; then the left's as its mirror image)
            a, b = sq(x0), sq(x1)
        elif x1 <= 0:
            a, b = -sq(-x0), -sq(-x1)
        else:                         # (across the middle: its half squeezed)
            a, b = -sq(x1), sq(x1)
        rects.append((a, y0, b + 1, y1 + 1, 2))
    lines = rects[4:]
    assert all(q[0] < q[2] and q[1] < q[3] for q in rects)
    for i in range(len(lines)):
        for j in range(i):
            p, q = lines[i], lines[j]
            assert p[2] <= q[0] or q[2] <= p[0] or p[3] <= q[1] or q[3] <= p[1], ("overlap", s, ws, p, q)
    circle = (sq(CIRCLE_N * (2 if s == 1 else 1)), CIRCLE_N * (2 if s == 1 else 1))
    return dict(box=box, corner=(cw, n), rects=rects, bar=sq(g["bar"]), top=g["top"], step=g["step"], circle=circle)


def sniper_ladder():
    """one period of the side frame (left side): the bar in column 0, a tick 3 long in row 2, 2 long in row 6"""
    p = np.zeros((8, 4))
    p[:, 0] = 1
    p[2, 1:4] = 1
    p[6, 1:3] = 1
    return p


def rounded(x, y, hw, hh, r):
    """inside the rectangle with rounded corners (pixel offsets from the aim pixel)"""
    dx = max(0, abs(x) - (hw - r))
    dy = max(0, abs(y) - (hh - r))
    return abs(x) <= hw and abs(y) <= hh and dx * dx + dy * dy <= r * r


def sniper_corner(s):
    """the window's top-left corner square, (r + 1) pixels a side, from its outline's corner: the darkening's
    alpha a texel (outside, outline, window)"""
    g = sniper_geometry(s)
    hw, hh, r = g["hw"], g["hh"], g["r"]
    n = r + 1
    c = np.zeros((n, n))
    for j in range(n):
        for i in range(n):
            x, y = -(hw + 1) + i, -(hh + 1) + j
            if rounded(x, y, hw, hh, r):
                c[j, i] = 0.0
            elif rounded(x, y, hw + 1, hh + 1, r + 1):
                c[j, i] = SNIPER_DARK_OUTLINE
            else:
                c[j, i] = SNIPER_DARK_OUT
    return c
