"""One line from an N64 debug log: frame rate, phases, tick, counts, how
fast game time ran (game seconds per second: 1.00 is real time), the
heap's low point and the stack's deepest use."""
import re, sys
text = open(sys.argv[1], errors="replace").read()
frames = re.findall(r"^frames: ([\d.]+) fps; per frame: draw (\d+) us \(level (\d+), objects (\d+), first person (\d+), hud (\d+)\), tick (\d+)", text, re.M)
renders = re.findall(r"^render: frame \d+, per frame (\d+) clusters, (\d+) batches, (\d+) objects, (\d+) triangles; heap (\d+) KB free, stack (\d+) KB", text, re.M)
ticks = re.findall(r"^tick: ([\d.]+) ms average, ([\d.]+) ms worst; 150 ticks \(5 s of game\) took (\d+) ms", text, re.M)
crash = len(re.findall(r"CPU EXCEPTION|ASSERT|panic|FATAL", text))
f = frames[2:] or frames
if not f:
    print(f"{sys.argv[2]:24s} no frames (crashes {crash})"); sys.exit()
avg = lambda i: sum(float(x[i]) for x in f) / len(f)
r = renders[2:] or renders
ravg = lambda i: sum(float(x[i]) for x in r) / len(r) if r else 0
t = ticks[1:] or ticks
tavg = sum(float(x[0]) for x in t) / len(t) if t else 0
speed = 5000.0 * len(t) / sum(float(x[2]) for x in t) if t else 0
print(f"{sys.argv[2]:24s} {avg(0):5.1f} fps (min {min(float(x[0]) for x in f):4.1f}) draw {avg(1)/1000:5.1f} level {avg(2)/1000:5.1f} "
      f"obj {avg(3)/1000:4.1f} fp {avg(4)/1000:4.1f} hud {avg(5)/1000:4.1f} tick {tavg:5.1f} ms game x{speed:4.2f} | {ravg(1):4.0f} batches "
      f"{ravg(3):6.0f} tris {ravg(2):3.0f} objs heap {min((float(x[4]) for x in r), default=0):4.0f} KB min "
      f"stack {max((int(x[5]) for x in renders), default=0)} KB | n={len(f)} crash={crash}")
