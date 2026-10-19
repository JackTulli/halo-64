"""A stand-in for Halo 64 Link, for testing the online cartridge's menu: it
answers the cartridge (port/n64/src/n64_link.c) over a socket that carries
what a flashcart's USB port would (an emulator's emulated port), with a
made-up game list.

    python3 -m tools.n64.link_sim --connect 127.0.0.1:6464   (or --listen 6464)

The socket carries UNFLoader's USB packets as the cart's USB does: "DMA@",
a header (the datatype in its top byte, the size in the rest; big-endian),
the data, "CMPH". The computer's are padded to an even length before
"CMPH"; the N64's (libdragon usb.c on the EverDrive) have the pad byte after
it. Each link packet (n64_link.h) is one USB packet of datatype 0x48.
"""

import argparse
import socket
import struct
import sys
import time

DATATYPE = 0x48
VERSION = 1
HELLO, INPUT, LIST, JOIN, HOST, LEAVE = 1, 2, 3, 4, 5, 6
WELCOME, STATUS, GAMES, LOAD, FRAME, ENDED = 64, 65, 66, 67, 68, 69
NAMES = {HELLO: "HELLO", INPUT: "INPUT", LIST: "LIST", JOIN: "JOIN", HOST: "HOST", LEAVE: "LEAVE"}

GAMES_LIST = [
    (1, 3, 16, "Milenko's server", "bloodgulch", "slayer"),
    (2, 7, 16, "Ranked Slayer", "hangemhigh", "team_slayer"),
    (3, 1, 8, "CTF night", "sidewinder", "ctf"),
]


def usb_packet(datatype, data):
    """an UNFLoader USB packet"""
    body = b"DMA@" + struct.pack(">I", (datatype << 24) | len(data)) + data
    if len(data) % 2:
        body += b"\0"
    return body + b"CMPH"


def link_packet(kind, payload, sequence):
    return b"H6" + bytes([VERSION, kind]) + struct.pack(">HH", len(payload), sequence & 0xFFFF) + payload


def string(text):
    data = text.encode("ascii", "replace")[:255]
    return bytes([len(data)]) + data


class Reader:
    """the USB packets in a byte stream"""

    def __init__(self):
        self.buffer = bytearray()

    def feed(self, data):
        self.buffer += data
        packets = []
        while True:
            start = self.buffer.find(b"DMA@")
            if start < 0:
                del self.buffer[:max(0, len(self.buffer) - 3)]
                return packets
            del self.buffer[:start]
            if len(self.buffer) < 8:
                return packets
            header, = struct.unpack_from(">I", self.buffer, 4)
            datatype, size = header >> 24, header & 0xFFFFFF
            padded = size + (size % 2)
            if len(self.buffer) < 8 + size + 4:
                return packets
            # ("CMPH" right after the data: the N64's, its pad byte after it,
            # which the next search passes over; else after the pad)
            if self.buffer[8 + size:8 + size + 4] == b"CMPH":
                end = 8 + size + 4
            elif len(self.buffer) < 8 + padded + 4:
                return packets
            elif self.buffer[8 + padded:8 + padded + 4] == b"CMPH":
                end = 8 + padded + 4
            else:
                # (out of step: look for the next header)
                del self.buffer[:4]
                continue
            packets.append((datatype, bytes(self.buffer[8:8 + size])))
            del self.buffer[:end]


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--connect", help="host:port of the emulator's USB socket")
    group.add_argument("--listen", type=int, help="a port to wait on for the emulator")
    parser.add_argument("--seconds", type=float, default=0, help="stop after this long (0: never)")
    args = parser.parse_args()
    if args.connect:
        host, port = args.connect.rsplit(":", 1)
        sock = socket.create_connection((host, int(port)))
    else:
        server = socket.socket()
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind(("127.0.0.1", args.listen))
        server.listen(1)
        print(f"link_sim: waiting on {args.listen}", flush=True)
        sock, _ = server.accept()
    sock.settimeout(0.2)
    print("link_sim: connected", flush=True)
    reader = Reader()
    sequence = 0
    started = last_status = time.time()
    welcomed = False
    inputs = 0

    def send(kind, payload=b""):
        nonlocal sequence
        sock.sendall(usb_packet(DATATYPE, link_packet(kind, payload, sequence)))
        sequence += 1

    while not args.seconds or time.time() - started < args.seconds:
        try:
            data = sock.recv(65536)
            if not data:
                print("link_sim: the emulator closed the socket", flush=True)
                return 0
        except socket.timeout:
            data = b""
        for datatype, packet in reader.feed(data):
            if datatype != DATATYPE:
                print(f"link_sim: USB datatype {datatype:#x}, {len(packet)} bytes: {packet[:60]!r}", flush=True)
                continue
            if len(packet) < 8 or packet[:2] != b"H6":
                print(f"link_sim: not a link packet: {packet[:16]!r}", flush=True)
                continue
            version, kind = packet[2], packet[3]
            length, seq = struct.unpack_from(">HH", packet, 4)
            payload = packet[8:8 + length]
            if kind == INPUT:
                inputs += 1
                continue
            print(f"link_sim: {NAMES.get(kind, kind)} #{seq} (version {version}) {payload!r}", flush=True)
            if kind == HELLO:
                send(WELCOME, bytes([VERSION]) + string("link_sim 1"))
                send(STATUS, string("CONNECTED TO THE TEST LINK"))
                welcomed = True
            elif kind == LIST:
                body = bytes([len(GAMES_LIST)])
                for game_id, players, most, name, map_name, game_type in GAMES_LIST:
                    body += struct.pack(">IBB", game_id, players, most) + string(name) + string(map_name) + string(game_type)
                send(GAMES, body)
            elif kind == JOIN:
                game_id, = struct.unpack_from(">I", payload)
                name = next((g[3] for g in GAMES_LIST if g[0] == game_id), "?")
                send(STATUS, string(f"JOINING {name} (TEST LINK: NOT A REAL GAME)"))
        if welcomed and time.time() - last_status > 1.0:
            send(STATUS, string(f"TEST LINK UP {int(time.time() - started)} S, {inputs} INPUTS"))
            last_status = time.time()
    return 0


if __name__ == "__main__":
    sys.exit(main())
