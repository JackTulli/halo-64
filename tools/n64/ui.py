"""The N64 menu's assets from the Xbox game's ui.map (port/n64/src/n64_menu.c).

The menu itself is the N64's own (split screen multiplayer only); from
Halo it takes:

- the scene behind Halo's main menu: the space sky (sky\\sky_ui) and the
  ring (scenery\\halo\\halo) as models (levels/ui.mdl, models.py), the ring's
  placement and the menu's camera points (ui/scene.bin);
- its fonts (fonts/<name>.font: an I8 atlas and the glyphs' places);
- pictures: the Halo logo, the multiplayer maps' and game types' pictures
  (ui/<name>.sprite).

    python3 -m tools.n64.ui <ui.map> --filesystem <file tree>
"""

import argparse
import math
import os
import struct
import subprocess
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageFont

from . import toolpaths
from .bitmaps import bitmap_images, decode
from .level import MKSPRITE, MATERIAL_TWO_SIDED
from .models import ModelConverter, strip_triangles, transform_point
from .repack import Repacker
from .tagdefs import TagDefinitions
from .xboxmap import XboxMap

FONTS = {"ui\\large_ui": "large", "ui\\small_ui": "small", "ui\\interstate": "interstate"}
SKY = "sky\\sky_ui\\sky_ui"
RING = "scenery\\halo\\halo"
# the sky is drawn as a sphere of this radius around the eye (it is
# ~27,000 units across; the menu's scene is ~40)
SKY_RADIUS = 30.0
# a model batch drawn first, without depth (models.py, n64_render.c)
MATERIAL_SKY = 64
# the sky's backdrop baked at this many texels a side (in 64-texel windows,
# models.py's atlas_windows), and about how many stars it keeps
SKY_ATLAS = 256
SKY_STARS = 1800
# the sky's planets, Threshold and Basis: left out (as cut-outs of their
# transparent shaders they looked crude)
SKY_LEFT_OUT = ("\\threshold", "\\basis")
# the ring's triangles: all of them (1,894)
RING_BUDGET = 4000
# the ring's inner surface, its landscapes (a 2048x128 base map six times
# round the ring; it was one 64x4 texture): at RING_ATLAS texels, in cells
# of CELL texels, each cell a texture of its own with a texel of margin all
# round (its neighbours': no seams with the filtering), the triangles cut
# along the cells' edges (UIModels.ring_cells); the six repeats share them
RING_INNER = "\\halo inner ring"
RING_ATLAS = (992, 124)
CELL = 62
# its shadow (a multiply pass over the landscapes: lit in the middle of
# its run, dark at its ends; it drew as white cut-outs): baked into the
# landscapes' vertices instead, the landscapes self-lit (their multipurpose
# map's self-illumination is full), the shadow's own part left out
RING_SHADOW = "\\halo inner ring shadow"
# the ring's outer surface (dark green plating, 1024x512, 48 times round,
# blotchy close up as CI4 64x32): greyscale I4 HULL_SIZE a side, its greys
# stretched to the full sixteen, with mipmaps (the far side) and Halo's
# detail map as the RDP's detail texture (close up: rdpq_sprite_upload turns
# it on), the plating's green on the vertices
RING_HULL = "\\halo outer ring"
HULL_SIZE = 64
HULL_DETAIL_SIZE = 32
HULL_DETAIL_FACTOR = 0.5
# the title's logo: HALO with a 64 under it (logo_64), in Will Turnbow's
# fan-made Halo font (dafont.com "Halo", free: ~/n64/fonts/Halo.ttf; without
# it the logo is Halo's own alone)
HALO_FONT = Path(os.environ.get("HALO_FONT", Path.home() / "n64" / "fonts" / "Halo.ttf"))
PICTURES = {
    # (for the menu's 640x480 screen: the logo cropped to what it draws --
    # its left 640x252 of 1024x256, with the 64 under it, 640x272 -- across
    # most of the screen, its soft edges and faint lines kept; the maps' at
    # their own 256x128, the game types' at 192x96)
    "ui\\shell\\main_menu\\halo_logo": ("logo", 560, "RGBA32"),
    "ui\\shell\\bitmaps\\mp_map_grafix": ("map", 256, "RGBA16"),
    "ui\\shell\\bitmaps\\game_type_grafix": ("gametype", 192, "RGBA16"),
}


def rotation(yaw, pitch, roll):
    """Halo's matrix4x3_rotation_from_angles: forward, left, up"""
    cr, sr, cp, sp, cy, sy = math.cos(roll), math.sin(roll), math.cos(pitch), math.sin(pitch), math.cos(yaw), math.sin(yaw)
    forward = (cy * cp, sy * cr - cy * sp * sr, cy * sp * cr + sy * sr)
    left = (-(sy * cp), sy * sp * sr + cy * cr, cy * sr - sy * sp * cr)
    up = (-sp, -(cp * sr), cp * cr)
    return forward, left, up


def export_font(xmap, defs, tag, out):
    """the font's glyphs (Halo's own 8-bit coverage) in one I8 atlas, and
    their places: 'H64F', ascent, descent, glyph count, then per glyph its
    character, atlas x and y, width, height, origin x and y, advance"""
    f = {fl.name: o for o, fl in defs.fields("Font")}
    cf = {fl.name: o for o, fl in defs.fields("FontCharacter")}
    csize = defs.size("FontCharacter")
    a = tag.address
    ascent, descent = struct.unpack("<hh", xmap.read(a + f["ascending height"], 4))
    count, pointer = struct.unpack("<iI", xmap.read(a + f["characters"], 8))
    psize, _, _, ppointer, _ = struct.unpack("<iIIII", xmap.read(a + f["pixels"], 20))
    pixels = xmap.read(ppointer, psize)
    glyphs = []
    for i in range(count):
        raw = xmap.read(pointer + i * csize, csize)
        code, advance, width, height, ox, oy = struct.unpack_from("<hhhhhh", raw, 0)
        offset = struct.unpack_from("<I", raw, cf["pixels offset"])[0]
        if code < 32 or code > 126:
            continue
        width, height = max(width, 0), max(height, 0)
        data = pixels[offset:offset + width * height] if width and height else b""
        glyphs.append((code, advance, width, height, ox, oy, data))
    # shelves 128 texels wide
    atlas_width = 128
    x = y = shelf = 0
    places = []
    for code, advance, width, height, ox, oy, data in glyphs:
        if x + width + 1 > atlas_width:
            x = 0
            y += shelf + 1
            shelf = 0
        places.append((x, y))
        x += width + 1
        shelf = max(shelf, height)
    atlas_height = 1
    while atlas_height < y + shelf + 1:
        atlas_height *= 2
    atlas = Image.new("L", (atlas_width, atlas_height), 0)
    for (code, advance, width, height, ox, oy, data), (px, py) in zip(glyphs, places):
        if width and height and len(data) == width * height:
            atlas.paste(Image.frombytes("L", (width, height), data), (px, py))
    out.mkdir(parents=True, exist_ok=True)
    name = FONTS[tag.name]
    png = out / f"{name}.png"
    atlas.save(png)
    toolpaths.run([MKSPRITE, "--format", "I8", "--compress", "1", "-o", str(out), str(png)], check=True,
                   capture_output=True)
    png.unlink()
    table = b"H64F" + struct.pack(">hhH", ascent, descent, len(glyphs))
    for (code, advance, width, height, ox, oy, data), (px, py) in zip(glyphs, places):
        table += struct.pack(">HHHBBbbB", code, px, py, width, height, ox, oy, max(0, min(255, advance)))
        table += b"\0"
    (out / f"{name}.glyphs").write_bytes(table)
    return len(glyphs)


def logo_64(logo):
    """Halo's logo (RGBA) with a 64 centred under the word, as tall as its
    letters, in HALO_FONT, dressed like them: their chrome (each row's mean
    colour over the letters, row for row down the 64), a light rim just
    inside it, their soft blue glow. Without the font, the logo as it is"""
    if not HALO_FONT.is_file():
        print(f"ui: no {HALO_FONT}: the title's logo without its 64")
        return logo
    alpha = logo.getchannel("A")
    width = min(logo.width, 640)
    # the letters: rows with many opaque texels (the drawing's lines are
    # fainter), then the columns opaque in those rows
    rows = [y for y in range(logo.height) if sum(1 for x in range(width) if alpha.getpixel((x, y)) > 230) > 20]
    top, bottom = rows[0], rows[-1]
    columns = [x for x in range(width) if sum(1 for y in range(top, bottom + 1) if alpha.getpixel((x, y)) > 230) > 3]
    left, right = columns[0], columns[-1]
    gradient = []
    for y in range(top, bottom + 1):
        px = [logo.getpixel((x, y)) for x in range(left, right + 1) if logo.getpixel((x, y))[3] > 230]
        gradient.append(tuple(sum(p[k] for p in px) // len(px) for k in range(3)) if px else gradient[-1])
    height = bottom - top + 1
    supersample = 4
    font = ImageFont.truetype(str(HALO_FONT), height * supersample * 2)
    box = font.getbbox("64")
    glyph = Image.new("L", (box[2] - box[0] + 8 * supersample, box[3] - box[1] + 8 * supersample), 0)
    ImageDraw.Draw(glyph).text((4 * supersample - box[0], 4 * supersample - box[1]), "64", font=font, fill=255)
    glyph = glyph.crop(glyph.getbbox())
    glyph = glyph.resize((round(glyph.width * height / glyph.height), height), Image.LANCZOS)
    x0, y0 = (left + right + 1) // 2 - glyph.width // 2, bottom + 16
    canvas = Image.new("RGBA", (logo.width, max(logo.height, y0 + height + 12)), (0, 0, 0, 0))
    canvas.paste(logo, (0, 0))
    mask = Image.new("L", canvas.size, 0)
    mask.paste(glyph, (x0, y0))
    glow = Image.new("RGBA", canvas.size, (150, 200, 255, 0))
    glow.putalpha(mask.filter(ImageFilter.GaussianBlur(6)).point(lambda v: int(v * 0.55)))
    fill = Image.new("RGBA", canvas.size, (0, 0, 0, 0))
    for y in range(y0, y0 + height):
        fill.paste(gradient[min(len(gradient) - 1, (y - y0) * len(gradient) // height)] + (255,),
                   (0, y, canvas.width, y + 1))
    fill.putalpha(mask)
    rim = Image.new("RGBA", canvas.size, (205, 228, 255, 0))
    rim.putalpha(ImageChops.subtract(mask, mask.filter(ImageFilter.MinFilter(3))).point(lambda v: int(v * 0.85)))
    for layer in (glow, fill, rim):
        canvas = Image.alpha_composite(canvas, layer)
    return canvas


def export_pictures(xmap, defs, out):
    out.mkdir(parents=True, exist_ok=True)
    written = []
    for tag in xmap.tags:
        if tag.group_name != "bitm" or tag.name not in PICTURES:
            continue
        name, width, fmt = PICTURES[tag.name]
        for i, image in enumerate(bitmap_images(xmap, tag, defs)):
            img = decode(xmap, image, max(image.width, image.height))
            if name == "logo":
                img = logo_64(img.convert("RGBA"))
                img = img.crop(img.getchannel("A").getbbox() or (0, 0, img.width, img.height))
            height = max(1, round(img.height * width / img.width))
            img = img.resize((width, height), Image.LANCZOS)
            png = out / (f"{name}.png" if name == "logo" else f"{name}{i}.png")
            img.save(png)
            toolpaths.run([MKSPRITE, "--format", fmt, "--compress", "1", "-o", str(out), str(png)], check=True,
                           capture_output=True)
            png.unlink()
            written.append(png.stem)
    return written


def export_scene(xmap, defs, out):
    """ui/scene.bin: 'H64S', the sky's and the ring's model tag indices
    (u16 each), the ring's node matrix (scale, forward, left, up, position:
    Halo's real_matrix4x3), the camera points' count, then each one's
    name[32], position, forward, up and vertical field of view"""
    scenario = xmap.scenario
    f = {fl.name: o for o, fl in defs.fields("Scenario")}
    a = scenario.address
    sc, sp = struct.unpack("<iI", xmap.read(a + f["scenery"], 8))
    sf = {fl.name: o for o, fl in defs.fields("ScenarioScenery")}
    ring = (1.0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0)
    if sc > 0:
        raw = xmap.read(sp, defs.size("ScenarioScenery"))
        position = struct.unpack_from("<3f", raw, sf["position"])
        yaw, pitch, roll = struct.unpack_from("<3f", raw, sf["rotation"])
        forward, left, up = rotation(yaw, pitch, roll)
        ring = (1.0,) + forward + left + up + position
    cc, cp = struct.unpack("<iI", xmap.read(a + f["cutscene camera points"], 8))
    cf = {fl.name: o for o, fl in defs.fields("ScenarioCutsceneCameraPoint")}
    csize = defs.size("ScenarioCutsceneCameraPoint")
    sky = [t.index for t in xmap.tags if t.group_name == "mode" and t.name == SKY]
    ring_model = [t.index for t in xmap.tags if t.group_name == "mode" and t.name == RING]
    out_data = b"H64S" + struct.pack(">HH", sky[0] if sky else 0xFFFF, ring_model[0] if ring_model else 0xFFFF)
    out_data += struct.pack(">13f", *ring) + struct.pack(">I", max(cc, 0))
    for i in range(max(cc, 0)):
        raw = xmap.read(cp + i * csize, csize)
        name = raw[cf["name"]:cf["name"] + 32].split(b"\0")[0]
        position = struct.unpack_from("<3f", raw, cf["position"])
        yaw, pitch, roll = struct.unpack_from("<3f", raw, cf["orientation"])
        fov = struct.unpack_from("<f", raw, cf["field of view"])[0]
        forward, left, up = rotation(yaw, pitch, roll)
        out_data += name + b"\0" * (32 - len(name)) + struct.pack(">10f", *position, *forward, *up, fov)
    out.mkdir(parents=True, exist_ok=True)
    (out / "scene.bin").write_bytes(out_data)
    return cc


def chicago_maps(xmap, defs, shader_tag):
    """a chicago shader's maps: (bitmap tag, colour function, u scale, v
    scale)"""
    f = {fl.name: o for o, fl in defs.fields("ShaderTransparentChicago")}
    mf = {fl.name: o for o, fl in defs.fields("ShaderTransparentChicagoMap")}
    size = defs.size("ShaderTransparentChicagoMap")
    count, pointer = struct.unpack("<iI", xmap.read(shader_tag.address + f["maps"], 8))
    maps = []
    for i in range(max(count, 0)):
        raw = xmap.read(pointer + i * size, size)
        datum = struct.unpack_from("<I", raw, mf["map"] + 12)[0]
        function = struct.unpack_from("<H", raw, mf["color function"])[0]
        u, v = struct.unpack_from("<2f", raw, mf["map u scale"])[0], struct.unpack_from("<f", raw, mf["map v scale"])[0]
        maps.append((xmap.tag(datum), function, u or 1.0, v or 1.0))
    return maps


def tiled(xmap, defs, tag, u_scale, v_scale, size, pool=False):
    """a map tiled u_scale by v_scale times over size x size texels: averaged,
    or with pool the brightest of the texels each one covers"""
    img = decode(xmap, bitmap_images(xmap, tag, defs)[0], 4096).convert("RGB")
    tiles_u, tiles_v = max(1, round(u_scale)), max(1, round(v_scale))
    sheet = Image.new("RGB", (img.width * tiles_u, img.height * tiles_v))
    for y in range(tiles_v):
        for x in range(tiles_u):
            sheet.paste(img, (x * img.width, y * img.height))
    if not pool:
        return sheet.resize((size, size), Image.BICUBIC)
    step_x, step_y = sheet.width / size, sheet.height / size
    result = Image.new("RGB", (size, size))
    for dy in range(max(1, int(step_y))):
        for dx in range(max(1, int(step_x))):
            sample = sheet.transform((size, size), Image.AFFINE, (step_x, 0, dx, 0, step_y, dy), Image.NEAREST)
            result = ImageChops.lighter(result, sample)
    return result


def space_backdrop(xmap, defs, shader_tag, size=SKY_ATLAS, stars=SKY_STARS):
    """the menu sky's space shader (a chicago shader: star mask times stars,
    plus space, added to a black framebuffer) baked into one image. The
    stars are tiled far finer than any N64 texture shows: the brightest in
    each place are kept, spaced out, about as many as the screen shows"""
    maps = chicago_maps(xmap, defs, shader_tag)
    # (the Xbox's order: current map, then the next by the colour function)
    layers = [tiled(xmap, defs, tag, u, v, size, pool=(max(u, v) * 64 > size)) for tag, _, u, v in maps]
    result = layers[0]
    for (tag, function, u, v), layer in zip(maps, layers[1:]):
        if function == 2:  # multiply
            result = ImageChops.multiply(result, layer)
        elif function == 4:  # add
            result = ImageChops.add(result, layer)
        else:
            result = layer
    # the stars alone (what the backdrop adds to the darkest map), thinned
    # to the local maxima above the level that keeps about 'stars' of them
    base = min(layers, key=lambda layer: sum(layer.convert("L").getextrema()))
    glow = ImageChops.subtract(result, base).convert("L")
    peaks = glow.filter(ImageFilter.MaxFilter(5))
    values = list(glow.getdata())
    is_peak = [g == p and g > 0 for g, p in zip(values, peaks.getdata())]
    ranked = sorted((g for g, peak in zip(values, is_peak) if peak), reverse=True)
    level = ranked[min(len(ranked) - 1, stars)] if ranked else 255
    kept = Image.new("L", glow.size)
    kept.putdata([g if peak and g > level else 0 for g, peak in zip(values, is_peak)])
    return ImageChops.add(base, Image.merge("RGB", (kept, kept, kept)))


class UIModels(ModelConverter):
    """the menu's scene: the sky, baked small, and the ring"""

    def shader_texture(self, shader_datum):
        # the sky's star field is its backdrop: drawn whole, not cut out, in
        # windows of a bigger image (texture_windows)
        tag = self.map.tag(shader_datum)
        if tag is not None and tag.name.endswith("\\space"):
            image = space_backdrop(self.map, self.defs, tag)
            whole = self.textures.write_texture(image, tag.name)
            return whole, 0, ("backdrop", image, whole, tag.name)
        if tag is not None and tag.name.endswith(SKY_LEFT_OUT + (RING_SHADOW,)):
            return None, 0, None
        if tag is not None and tag.name.endswith(RING_HULL):
            return self.hull_texture(tag)
        if tag is not None and tag.name.endswith(RING_INNER):
            texture, flags, (base, _) = super().shader_texture(shader_datum)
            image = decode(self.map, bitmap_images(self.map, self.map.tag(base), self.defs)[0], 4096)
            image = image.convert("RGB").resize(RING_ATLAS, Image.LANCZOS)
            return texture, flags, ("ring", image, texture, tag.name)
        return super().shader_texture(shader_datum)

    def texture_windows(self, bitmap, vertices, triangles):
        if bitmap is not None and bitmap[0] == "backdrop":
            _, image, whole, name = bitmap
            return self.atlas_windows(image, None, whole, name, vertices, triangles)
        if bitmap is not None and bitmap[0] == "ring":
            _, image, whole, name = bitmap
            groups = self.ring_cells(image, name, vertices, triangles)
            # self-lit, under the shadow (RING_SHADOW)
            shadow = self.ring_shadow()
            for i in {i for _, _, tris in groups for tri in tris for i in tri}:
                v = vertices[i]
                vertices[i] = (v[0], v[1], v[2], shadow(v[1]), v[4])
            return groups
        if bitmap is not None and bitmap[0] == "hull":
            # the plating's colour on the vertices, times their light; the
            # texture whole (it repeats round the ring)
            tint = bitmap[1]
            for i in {i for tri in triangles for i in tri}:
                v = vertices[i]
                vertices[i] = (v[0], v[1], v[2], tuple(min(1.0, c * v[3]) for c in tint), v[4])
            return None
        return super().texture_windows(bitmap, vertices, triangles)

    def convert_part(self, part_address, nodes_inverse, u_scale, v_scale, *rest, **options):
        # (the ring's shadow is read in the same space as the part)
        self.part_space = (nodes_inverse, u_scale, v_scale)
        return super().convert_part(part_address, nodes_inverse, u_scale, v_scale, *rest, **options)

    def hull_texture(self, tag):
        """the ring's outer surface (RING_HULL): (texture, flags, ('hull',
        tint)) -- the tint the vertices' colour"""
        so = self.f("ShaderModel")
        read = lambda field, fmt: struct.unpack(fmt, self.map.read(tag.address + so[field], struct.calcsize(fmt)))[0]
        base = struct.unpack("<I", self.map.read(tag.address + so["base map"] + 12, 4))[0]
        detail = struct.unpack("<I", self.map.read(tag.address + so["detail map"] + 12, 4))[0]
        image = decode(self.map, bitmap_images(self.map, self.map.tag(base), self.defs)[0], 4096).convert("RGB")
        colour = image.resize((1, 1), Image.BOX).getpixel((0, 0))
        grey = image.convert("L").resize((HULL_SIZE, HULL_SIZE), Image.LANCZOS)
        top = max(grey.getdata()) or 1
        grey = grey.point(lambda v: min(255, int(round(v * 255 / top))))
        mean = sum(grey.getdata()) / (HULL_SIZE * HULL_SIZE) / 255.0
        # the colour per unit of grey, the stretch undone: the vertices'
        luminance = (0.299 * colour[0] + 0.587 * colour[1] + 0.114 * colour[2]) or 1.0
        tint = tuple(min(1.0, c / luminance * top / 255.0) for c in colour)
        work = self.textures.out_dir / "tex" / f".{self.textures.map_name}"
        work.mkdir(parents=True, exist_ok=True)
        index = len(self.textures.textures)
        png = work / f"{self.textures.texture_prefix}hull{index:02d}.png"
        grey.save(png)
        command = [MKSPRITE, "--format", "I4", "--mipmap", "BOX", "--compress", "1"]
        detail_tag = self.map.tag(detail)
        if detail_tag is not None:
            # the detail about the plating's mean grey (the RDP blends toward
            # it, close up), repeating Halo's times per repeat of the base
            # map: texels of it per texel of the base, as a power of two
            d = decode(self.map, bitmap_images(self.map, detail_tag, self.defs)[0], 4096).convert("L")
            d = d.resize((HULL_DETAIL_SIZE, HULL_DETAIL_SIZE), Image.LANCZOS)
            d_mean = (sum(d.getdata()) / (HULL_DETAIL_SIZE * HULL_DETAIL_SIZE)) or 1.0
            d = d.point(lambda v: min(255, int(round(mean * 255 * v / d_mean))))
            detail_png = work / f"{self.textures.texture_prefix}hulldetail{index:02d}.png"
            d.save(detail_png)
            repeats = read("detail map scale", "<f") or 1.0
            scale_log = -max(0, round(math.log2(max(1.0, repeats * HULL_DETAIL_SIZE / HULL_SIZE))))
            command += ["--detail", f"{detail_png.as_posix()},I4,{HULL_DETAIL_FACTOR}",
                        "--detail-texparms", f"0,{scale_log},2048,0"]
        toolpaths.run(command + ["-o", str(png.parent), str(png)], check=True, capture_output=True)
        png.unlink()
        if detail_tag is not None:
            detail_png.unlink()
        from .level import Texture, share_file
        name = share_file(png.with_suffix(".sprite"), self.textures.out_dir / "tex", ".sprite")
        self.textures.used_textures.add(name)
        self.textures.textures.append(Texture(name, HULL_SIZE, HULL_SIZE, tuple(c / 255.0 for c in colour)))
        model_flags = struct.unpack("<H", self.map.read(tag.address + so["shader model flags"], 2))[0]
        return index, MATERIAL_TWO_SIDED if model_flags & 2 else 0, ("hull", tint)

    def ring_shadow(self):
        """a function of a point of the ring (its part's space) to the
        shadow's brightness there (RING_SHADOW): its texel where the
        shadow's surface lies, 1 where it does not"""
        nodes_inverse, u_scale, v_scale = self.part_space
        ring = next(t for t in self.map.tags if t.group_name == "mode" and t.name == RING)
        mf, gf, pf = self.f("Model"), self.f("ModelGeometry"), self.f("ModelGeometryPart")
        sc, sp = self.block(ring.address + mf["shaders"])
        shader_names = [self.map.tag(struct.unpack("<I", self.map.read(sp + i * 32 + 12, 4))[0]) for i in range(sc)]
        gc, gp = self.block(ring.address + mf["geometries"])
        pc, pp = self.block(gp + gf["parts"])
        psize = self.defs.size("ModelGeometryPart")
        triangles, image = [], None
        for p in range(pc):
            raw = self.map.read(pp + p * psize, psize)
            shader = struct.unpack_from("<h", raw, pf["shader index"])[0]
            tag = shader_names[shader] if 0 <= shader < len(shader_names) else None
            if tag is None or not tag.name.endswith(RING_SHADOW):
                continue
            image = decode(self.map, bitmap_images(self.map, chicago_maps(self.map, self.defs, tag)[0][0], self.defs)[0],
                           4096).convert("L")
            count = struct.unpack_from("<I", raw, pf["triangle count"])[0]
            pointer = struct.unpack_from("<I", raw, pf["triangle offset"])[0]
            vertex_count = struct.unpack_from("<I", raw, pf["vertex count"])[0]
            entry = struct.unpack_from("<I", raw, pf["vertex offset"])[0]
            data = self.map.read(struct.unpack("<I", self.map.read(entry + 4, 4))[0], vertex_count * 32)
            points = []
            for v in range(vertex_count):
                px, py, pz, _, _, _, tu, tv = struct.unpack_from("<3fIIIhh", data, v * 32)
                position = tuple(c * self.current_scale for c in transform_point(nodes_inverse[0], (px, py, pz)))
                points.append((position, (tu / 32767.0 * u_scale, tv / 32767.0 * v_scale)))
            indices = struct.unpack(f"<{count + 2}H", self.map.read(pointer, 2 * (count + 2)))
            for a, b, c in strip_triangles(indices):
                if max(a, b, c) < vertex_count:
                    triangles.append((points[a], points[b], points[c]))
        if image is None:
            return lambda point: 1.0
        width, height = image.size
        pixels = image.load()

        def sample(uv):
            x = (uv[0] - math.floor(uv[0])) * width - 0.5
            y = min(height - 1.0, max(0.0, uv[1] * height - 0.5))
            x0, y0 = math.floor(x), int(y)
            fx, fy = x - x0, y - y0
            y1 = min(height - 1, y0 + 1)
            value = 0.0
            for yy, wy in ((y0, 1 - fy), (y1, fy)):
                for xx, wx in ((x0 % width, 1 - fx), ((x0 + 1) % width, fx)):
                    value += pixels[xx, yy] * wx * wy
            return value / 255.0

        sub = lambda a, b: (a[0] - b[0], a[1] - b[1], a[2] - b[2])
        dot = lambda a, b: a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

        def brightness(point):
            best = None
            for (pa, ua), (pb, ub), (pc_, uc) in triangles:
                e1, e2, w = sub(pb, pa), sub(pc_, pa), sub(point, pa)
                d00, d01, d11, d20, d21 = dot(e1, e1), dot(e1, e2), dot(e2, e2), dot(w, e1), dot(w, e2)
                denominator = d00 * d11 - d01 * d01
                if denominator <= 1e-12:
                    continue
                s = (d11 * d20 - d01 * d21) / denominator
                t = (d00 * d21 - d01 * d20) / denominator
                if s < -0.02 or t < -0.02 or s + t > 1.02:
                    continue
                closest = tuple(pa[k] + e1[k] * s + e2[k] * t for k in range(3))
                distance = math.sqrt(dot(sub(point, closest), sub(point, closest)))
                if distance < 0.5 and (best is None or distance < best[0]):
                    uv = (ua[0] + (ub[0] - ua[0]) * s + (uc[0] - ua[0]) * t,
                          ua[1] + (ub[1] - ua[1]) * s + (uc[1] - ua[1]) * t)
                    best = (distance, uv)
            return sample(best[1]) if best else 1.0

        return brightness

    def ring_cells(self, image, name, vertices, triangles):
        """the ring's inner surface by cell of its base map (RING_ATLAS):
        each triangle brought into the map's first repeat, cut along the
        cells' edges, each piece drawn with its cell's texture: [(texture,
        uv -> texels, triangles)]. New vertices (the cuts', the repeats'
        copies) are added to vertices"""
        aw, ah = image.size
        columns, rows = aw // CELL, ah // CELL
        # the map with a texel more all round: wrapped across (the map
        # repeats round the ring), clamped along
        edged = Image.new("RGB", (aw + 2, ah + 2))
        edged.paste(image, (1, 1))
        edged.paste(image.crop((aw - 1, 0, aw, ah)), (0, 1))
        edged.paste(image.crop((0, 0, 1, ah)), (aw + 1, 1))
        edged.paste(edged.crop((0, 1, aw + 2, 2)), (0, 0))
        edged.paste(edged.crop((0, ah, aw + 2, ah + 1)), (0, ah + 1))
        textures = {}
        groups = {}

        def lerp(a, b, t):
            va, vb = vertices[a], vertices[b]
            mix = lambda x, y: tuple(p + (q - p) * t for p, q in zip(x, y))
            n = mix(va[4], vb[4])
            length = math.sqrt(sum(c * c for c in n)) or 1.0
            vertices.append((va[0], mix(va[1], vb[1]), mix(va[2], vb[2]), va[3] + (vb[3] - va[3]) * t,
                             tuple(c / length for c in n)))
            return len(vertices) - 1

        cuts = {}

        def cut_point(a, b, axis, value):
            key = (min(a, b), max(a, b), axis, value)
            if key not in cuts:
                pa, pb = vertices[a][2][axis], vertices[b][2][axis]
                cuts[key] = lerp(a, b, (value - pa) / (pb - pa))
            return cuts[key]

        def split(polygon, axis, value):
            """a convex polygon (vertex indices) cut by the line uv[axis] =
            value: (below, above)"""
            below, above = [], []
            for i, a in enumerate(polygon):
                b = polygon[(i + 1) % len(polygon)]
                pa, pb = vertices[a][2][axis], vertices[b][2][axis]
                (below if pa < value else above).append(a)
                if (pa < value) != (pb < value):
                    point = cut_point(a, b, axis, value)
                    below.append(point)
                    above.append(point)
            return below, above

        shifted = {}

        def shift(index, du):
            """a copy of the vertex du repeats along"""
            if not du:
                return index
            key = (index, du)
            if key not in shifted:
                v = vertices[index]
                vertices.append((v[0], v[1], (v[2][0] + du, v[2][1]), v[3], v[4]))
                shifted[key] = len(vertices) - 1
            return shifted[key]

        for tri in triangles:
            du = -math.floor(min(vertices[i][2][0] for i in tri))
            pieces = [[shift(i, du) for i in tri]]
            for axis, count, size in ((0, columns * 2, aw), (1, rows, ah)):
                for k in range(1, count):
                    value = k * CELL / size
                    next_pieces = []
                    for polygon in pieces:
                        coordinates = [vertices[i][2][axis] for i in polygon]
                        if min(coordinates) < value < max(coordinates):
                            next_pieces.extend(p for p in split(polygon, axis, value) if len(p) >= 3)
                        else:
                            next_pieces.append(polygon)
                    pieces = next_pieces
            for polygon in pieces:
                u = sum(vertices[i][2][0] for i in polygon) / len(polygon)
                v = sum(vertices[i][2][1] for i in polygon) / len(polygon)
                column, row = int(u * aw // CELL), min(rows - 1, max(0, int(v * ah // CELL)))
                # (past the first repeat: the next one's copy)
                if column >= columns:
                    polygon = [shift(i, -1) for i in polygon]
                    column -= columns
                group = groups.setdefault((column, row), [])
                for k in range(1, len(polygon) - 1):
                    group.append((polygon[0], polygon[k], polygon[k + 1]))
        result = []
        for (column, row), tris in sorted(groups.items()):
            if (column, row) not in textures:
                crop = edged.crop((column * CELL, row * CELL, column * CELL + CELL + 2, row * CELL + CELL + 2))
                textures[(column, row)] = self.textures.write_texture(crop, f"{name} cell")
            x0, y0 = column * CELL - 1, row * CELL - 1
            result.append((textures[(column, row)], lambda uv, x0=x0, y0=y0: (uv[0] * aw - x0, uv[1] * ah - y0), tris))
        return result

    def object_models(self):
        found = []
        self.budgets = {}
        self.first_person = set()
        self.hands = set()
        self.units = set()
        for tag in self.map.tags:
            if tag.group_name == "mode" and tag.name in (SKY, RING):
                found.append(tag)
                self.budgets[tag.index] = 100000 if tag.name == SKY else RING_BUDGET
                # (for texture_windows: the backdrop's windows, the ring's
                # cells)
                self.first_person.add(tag.index)
        # the sky first: drawn first (n64_menu.c)
        found.sort(key=lambda t: t.name != SKY)
        return found


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("map", type=Path)
    parser.add_argument("--filesystem", type=Path, required=True)
    args = parser.parse_args()
    xmap = XboxMap(args.map)
    defs = TagDefinitions()
    fonts = [export_font(xmap, defs, t, args.filesystem / "fonts") for t in xmap.tags
             if t.group_name == "font" and t.name in FONTS]
    pictures = export_pictures(xmap, defs, args.filesystem / "ui")
    cameras = export_scene(xmap, defs, args.filesystem / "ui")
    models = UIModels(xmap, Repacker(xmap, None).defs, "ui", args.filesystem)
    sky = [t for t in xmap.tags if t.group_name == "mode" and t.name == SKY]
    if sky:
        # the sky's vertices at SKY_RADIUS, its batches drawn first and
        # without depth
        models.position_scales = {sky[0].index: SKY_RADIUS / 27000.0}
        models.extra_flags = {sky[0].index: MATERIAL_SKY | MATERIAL_TWO_SIDED}
        models.no_far = {t.index for t in xmap.tags if t.group_name == "mode"}
    stats = models.write(args.filesystem / "levels" / "ui.mdl")
    from .level import write_list
    write_list(args.filesystem / "tex" / "ui.list", sorted(models.textures.used_textures))
    print(f"ui: {len(fonts)} fonts, {len(pictures)} pictures, {cameras} camera points, models {stats}")


if __name__ == "__main__":
    main()
