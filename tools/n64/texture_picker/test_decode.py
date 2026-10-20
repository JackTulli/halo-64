"""checks make_picker's sprite decoder against images whose texels are known"""
import sys
import tempfile
from pathlib import Path

from PIL import Image

sys.path.insert(0, str(Path(__file__).parent))
import make_picker as mp  # noqa: E402

work = Path(tempfile.mkdtemp())
failures = 0

# RGBA16: 5-bit channels survive exactly
img = Image.new("RGB", (32, 16))
img.putdata([((x * 8) & 0xF8, (y * 16) & 0xF8, ((x + y) * 8) & 0xF8) for y in range(16) for x in range(32)])
out = mp.mksprite(img.convert("RGBA"), "RGBA16", work)
bad = sum(1 for a, b in zip(img.getdata(), out.getdata()) if tuple(c & 0xF8 for c in b[:3]) != a)
print("RGBA16", out.size, "mismatches", bad)
failures += bad != 0

# I4: 16 levels exactly
grey = Image.new("L", (16, 8))
grey.putdata([(x % 16) * 17 for y in range(8) for x in range(16)])
out = mp.mksprite(grey, "I4", work)
bad = sum(1 for a, b in zip(grey.getdata(), out.getdata()) if b[0] != a)
print("I4", out.size, "mismatches", bad)
failures += bad != 0

# IA4: 8 levels + cut-out
la = Image.merge("LA", (grey.point(lambda v: (v // 32) * 255 // 7), grey.point(lambda v: 255 if v >= 128 else 0)))
out = mp.mksprite(la, "IA4", work)
bad = sum(1 for (l, a), b in zip(la.getdata(), out.getdata()) if abs(b[0] - l) > 1 or (b[3] >= 128) != (a >= 128))
print("IA4", out.size, "mismatches", bad)
failures += bad != 0

# CI4: a 16-colour image comes back with the same colours
colours = [(i * 16, 255 - i * 16, (i * 37) % 256) for i in range(16)]
ci = Image.new("RGB", (32, 32))
ci.putdata([colours[(x // 8 + y // 8 * 4) % 16] for y in range(32) for x in range(32)])
out = mp.mksprite(ci, "CI4", work)
bad = sum(1 for a, b in zip(ci.getdata(), out.getdata()) if any(abs(p - q) > 8 for p, q in zip(a, b[:3])))
print("CI4", out.size, "mismatches", bad)
failures += bad != 0
print("FAILED" if failures else "all decoded right")
