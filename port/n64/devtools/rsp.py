#!/usr/bin/env python3
"""Minimal GDB remote-protocol client for ares' debug server: halts the CPU,
prints pc/ra/sp and a stack scan symbolized with addr2line.
usage: rsp.py elf [port] [stack_words]"""
import os, socket, sys, subprocess, struct

def checksum(s): return sum(s.encode()) & 0xff

class RSP:
    def __init__(self, port):
        self.s = socket.create_connection(("127.0.0.1", port), timeout=10)
        self.buf = b""
    def send(self, cmd):
        pkt = f"${cmd}#{checksum(cmd):02x}".encode()
        self.s.sendall(pkt)
        return self.recv()
    def recv(self):
        while True:
            while b"$" not in self.buf or b"#" not in self.buf[self.buf.index(b"$"):]:
                self.buf += self.s.recv(65536)
            start = self.buf.index(b"$")
            end = self.buf.index(b"#", start)
            if len(self.buf) < end + 3:
                self.buf += self.s.recv(65536); continue
            data = self.buf[start+1:end].decode()
            self.buf = self.buf[end+3:]
            self.s.sendall(b"+")
            return data
    def interrupt(self):
        self.s.sendall(b"\x03")
        return self.recv()

def main():
    elf = sys.argv[1]; port = int(sys.argv[2]) if len(sys.argv) > 2 else 9123
    words = int(sys.argv[3]) if len(sys.argv) > 3 else 256
    r = RSP(port)
    r.s.sendall(b"+")
    print("stop:", r.interrupt())
    g = r.send("g")
    n = len(g)
    # registers are 64-bit (16 hex digits) in ares' mips64 target
    regs = [int(g[i:i+16], 16) for i in range(0, n, 16)]
    names = ["zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7",
             "s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","s8","ra",
             "sr","lo","hi","bad","cause","pc"]
    R = {names[i]: regs[i] & 0xffffffff for i in range(min(len(names), len(regs)))}
    print(" ".join(f"{k}={R[k]:08x}" for k in ("pc","ra","sp","a0","a1","v0","cause","bad","sr") if k in R))
    addrs = [R["pc"], R["ra"]]
    sp = R["sp"]
    mem = r.send(f"m{sp:x},{words*4:x}")
    stack = bytes.fromhex(mem) if mem and not mem.startswith("E") else b""
    for i in range(0, len(stack) - 3, 4):
        v = struct.unpack(">I", stack[i:i+4])[0]
        if 0x80000400 <= v < 0x80400000:
            addrs.append(v)
    out = subprocess.run([os.path.join(os.environ.get("N64_INST", os.path.expanduser("~/n64/toolchain")), "bin", "mips64-elf-addr2line"), "-f", "-C", "-e", elf] +
                         [f"{a:x}" for a in addrs], capture_output=True, text=True).stdout.split("\n")
    seen = set()
    for i, a in enumerate(addrs):
        fn, loc = out[2*i], out[2*i+1] if 2*i+1 < len(out) else ""
        if fn == "??" or (fn, loc) in seen:
            continue
        seen.add((fn, loc))
        tag = "pc" if i == 0 else "ra" if i == 1 else "stack"
        print(f"  {tag:5s} {a:08x} {fn} {loc.split('/halo-ce-universal/')[-1]}")
    r.s.close()

main()
