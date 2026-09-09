#!/usr/bin/env python3
import json
import socket
import struct
import sys


FLASH_BASE = 0x08000000
FAULT_REGS = {
    "cfsr": 0xE000ED28,
    "hfsr": 0xE000ED2C,
    "dfsr": 0xE000ED30,
    "afsr": 0xE000ED3C,
    "mmfar": 0xE000ED34,
    "bfar": 0xE000ED38,
    "abfsr": 0xE000EFA8,
}


def checksum(s):
    return sum(s.encode("ascii")) & 0xff


def packet(s):
    return "$%s#%02x" % (s, checksum(s))


def parse_packet(conn):
    while True:
        c = conn.recv(1)
        if c == b"$":
            break
        if c == b"\x03":
            return "\x03"
    data = bytearray()
    while True:
        c = conn.recv(1)
        if c == b"#":
            conn.recv(2)
            conn.sendall(b"+")
            return data.decode("ascii")
        data += c


def symbols(path):
    syms = {}
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    for line in lines:
        fields = line.split()
        if len(fields) >= 2 and fields[0].startswith("0x"):
            syms[fields[-1]] = int(fields[0], 16)
    return syms


def value(v, syms):
    if isinstance(v, int):
        return v & 0xffffffff
    s = str(v).replace(" ", "")
    for op in ("+", "-"):
        if op in s:
            name, off = s.split(op, 1)
            n = int(off, 0)
            return (syms[name] + n if op == "+" else syms[name] - n) & 0xffffffff
    if s.startswith("0x"):
        return int(s, 16) & 0xffffffff
    return syms[s] & 0xffffffff


def readmem(mem, addr, size):
    data = bytearray()
    for i in range(size):
        b = mem.get(addr + i)
        if b is None:
            return None
        data.append(b)
    return data.hex()


def hex_decode(s):
    return bytes.fromhex(s).decode("ascii")


def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 3333
    with open("crash.json", "r", encoding="utf-8") as f:
        crash = json.load(f)
    image = crash.get("image", "crash.elf")
    syms = symbols(crash.get("map", image + ".map"))
    with open(crash.get("binary", image.rsplit(".", 1)[0] + ".bin"), "rb") as f:
        flash = f.read()

    regs = crash["regs"]
    stack = crash["stack"]
    mem = {}
    for i, b in enumerate(flash):
        mem[FLASH_BASE + i] = b
    addr = value(stack["addr"], syms)
    for i, word in enumerate(stack["words"]):
        w = value(word, syms)
        for j, b in enumerate(struct.pack("<I", w)):
            mem[addr + i * 4 + j] = b
    for name, addr in FAULT_REGS.items():
        if name in crash.get("fault", {}):
            w = value(crash["fault"][name], syms)
            for j, b in enumerate(struct.pack("<I", w)):
                mem[addr + j] = b

    order = ["r%d" % n for n in range(13)] + ["sp", "lr", "pc", "xpsr"]
    regvals = [value(regs[name], syms) for name in order]
    regpkt = b"".join(struct.pack("<I", r) for r in regvals).hex()

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("127.0.0.1", port))
        s.listen(1)
        print("fake target listening on 127.0.0.1:%d" % port, flush=True)
        conn, _ = s.accept()
        with conn:
            while True:
                req = parse_packet(conn)
                if req.startswith("qSupported"):
                    rep = "PacketSize=4000"
                elif req.startswith("qRcmd,"):
                    print("monitor %s" % hex_decode(req[6:]), flush=True)
                    rep = "OK"
                elif req in ("?", "vStopped", "\x03", "c", "s") or \
                        req.startswith(("C", "S", "vCont;c", "vCont;s")):
                    rep = "T05thread:1;"
                elif req == "vCont?":
                    rep = "vCont;c;s"
                elif req == "qC":
                    rep = "QC1"
                elif req == "qfThreadInfo":
                    rep = "m1"
                elif req == "qsThreadInfo":
                    rep = "l"
                elif req == "qAttached":
                    rep = "1"
                elif req.startswith("T"):
                    rep = "OK"
                elif req.startswith(("Hg", "Hc", "QStartNoAckMode")):
                    rep = "OK"
                elif req == "g":
                    rep = regpkt
                elif req.startswith("p"):
                    n = int(req[1:], 16)
                    rep = struct.pack("<I", regvals[n]).hex() if n < len(regvals) else "00000000"
                elif req.startswith("m"):
                    a, n = req[1:].split(",", 1)
                    data = readmem(mem, int(a, 16), int(n, 16))
                    rep = data if data is not None else "E01"
                elif req.startswith(("q", "v", "Z", "z")):
                    rep = ""
                elif req in ("D", "k"):
                    conn.sendall(packet("OK").encode("ascii"))
                    break
                else:
                    rep = ""
                conn.sendall(packet(rep).encode("ascii"))


if __name__ == "__main__":
    main()
