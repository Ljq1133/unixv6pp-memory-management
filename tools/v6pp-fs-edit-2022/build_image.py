#!/usr/bin/env python3
import math
import os
import struct
import sys
from pathlib import Path

SECTOR_SIZE = 512
TOTAL_SECTORS = 20160
SUPER_BLOCK_SECTOR = 200
INODE_ZONE_START = 202
INODE_ZONE_SIZE = 1024 - 202
DATA_ZONE_START = 1024
DATA_ZONE_END = 18000 - 1
DIRSIZ = 28

IALLOC = 0x8000
IFDIR = 0x4000
IFCHR = 0x2000
IREAD = 0x100
IWRITE = 0x80
IEXEC = 0x40
PERM_777 = IREAD | IWRITE | IEXEC | (IREAD >> 3) | (IWRITE >> 3) | (IEXEC >> 3) | (IREAD >> 6) | (IWRITE >> 6) | (IEXEC >> 6)
PERM_755 = IREAD | IWRITE | IEXEC | (IREAD >> 3) | (IEXEC >> 3) | (IREAD >> 6) | (IEXEC >> 6)


class ImageBuilder:
    def __init__(self, workspace: Path, output: Path):
        self.workspace = workspace
        self.output = output
        self.image = bytearray(TOTAL_SECTORS * SECTOR_SIZE)
        self.next_inode = 1
        self.next_block = DATA_ZONE_START
        self.free_blocks = set(range(DATA_ZONE_START, DATA_ZONE_END + 1))

    def sector_offset(self, sector: int) -> int:
        return sector * SECTOR_SIZE

    def write_sector(self, sector: int, data: bytes) -> None:
        offset = self.sector_offset(sector)
        self.image[offset:offset + len(data)] = data

    def alloc_inode(self) -> int:
        inode = self.next_inode
        self.next_inode += 1
        if inode >= INODE_ZONE_SIZE * 8:
            raise RuntimeError("inode zone exhausted")
        return inode

    def alloc_block(self) -> int:
        while self.next_block not in self.free_blocks:
            self.next_block += 1
        if self.next_block > DATA_ZONE_END:
            raise RuntimeError("data zone exhausted")
        block = self.next_block
        self.free_blocks.remove(block)
        self.next_block += 1
        return block

    def write_inode(self, inode: int, mode: int, nlink: int, size: int, addrs, uid: int = 0, gid: int = 0) -> None:
        addrs = list(addrs)[:10] + [0] * (10 - len(addrs))
        data = struct.pack("<IiHHI10iII", mode, nlink, uid, gid, size, *addrs, 0, 0)
        if len(data) != 64:
            raise AssertionError(len(data))
        sector = INODE_ZONE_START + inode // 8
        offset = self.sector_offset(sector) + (inode % 8) * 64
        self.image[offset:offset + 64] = data

    def directory_entry(self, inode: int, name: str) -> bytes:
        raw_name = name.encode("ascii")
        if len(raw_name) > DIRSIZ:
            raise RuntimeError(f"directory name too long: {name}")
        return struct.pack("<I", inode) + raw_name.ljust(DIRSIZ, b"\0")

    def add_small_file(self, data: bytes, mode: int, nlink: int = 1) -> int:
        inode = self.alloc_inode()
        data_blocks = []
        for offset in range(0, len(data), SECTOR_SIZE):
            block = self.alloc_block()
            data_blocks.append(block)
            self.write_sector(block, data[offset:offset + SECTOR_SIZE])

        addrs = data_blocks[:6]
        remaining = data_blocks[6:]
        while remaining:
            index_block = self.alloc_block()
            chunk = remaining[:128]
            remaining = remaining[128:]
            table = struct.pack("<" + "i" * 128, *(chunk + [0] * (128 - len(chunk))))
            self.write_sector(index_block, table)
            addrs.append(index_block)

        if len(addrs) > 8:
            raise RuntimeError("file too large for single-indirect image builder")

        self.write_inode(inode, mode, nlink, len(data), addrs)
        return inode

    def add_directory(self, entries) -> int:
        inode = self.alloc_inode()
        data = b"".join(self.directory_entry(child, name) for child, name in entries)
        block = self.alloc_block()
        self.write_sector(block, data)
        self.write_inode(inode, IALLOC | IFDIR | PERM_777, 2, len(data), [block])
        return inode

    def write_directory(self, inode: int, entries) -> None:
        data = b"".join(self.directory_entry(child, name) for child, name in entries)
        block = self.alloc_block()
        self.write_sector(block, data)
        self.write_inode(inode, IALLOC | IFDIR | PERM_777, 2, len(data), [block])

    def write_superblock(self) -> None:
        s_free = sorted(self.free_blocks, reverse=True)[:100]
        s_inode = list(range(INODE_ZONE_SIZE * 8 - 1, self.next_inode - 1, -1))[:100]
        values = [
            INODE_ZONE_SIZE,
            TOTAL_SECTORS,
            len(s_free),
            *s_free,
            len(s_inode),
            *s_inode,
            0,
            0,
            1,
            0,
            0,
            *([0] * 47),
        ]
        packed = struct.pack("<" + "i" * len(values), *values)
        if len(packed) != 1024:
            raise AssertionError(len(packed))
        self.write_sector(SUPER_BLOCK_SECTOR, packed[:SECTOR_SIZE])
        self.write_sector(SUPER_BLOCK_SECTOR + 1, packed[SECTOR_SIZE:])

    def build(self) -> None:
        boot = (self.workspace / "boot.bin").read_bytes()
        kernel = (self.workspace / "kernel.bin").read_bytes()
        if len(boot) != SECTOR_SIZE:
            raise RuntimeError("boot.bin must be exactly one sector")
        if math.ceil(len(kernel) / SECTOR_SIZE) > 180:
            raise RuntimeError("kernel.bin is larger than boot.s KERNEL_SIZE")

        self.write_sector(0, boot)
        self.image[SECTOR_SIZE:SECTOR_SIZE + len(kernel)] = kernel

        root_inode = self.alloc_inode()
        bin_inode = self.alloc_inode()
        dev_inode = self.alloc_inode()

        tty_inode = self.alloc_inode()
        self.write_inode(tty_inode, IALLOC | IFCHR | PERM_777, 1, 0, [0])

        program_dir = self.workspace / "programs"
        shell_inode = self.add_small_file((program_dir / "Shell.exe").read_bytes(), IALLOC | PERM_755)

        bin_entries = [(bin_inode, "."), (root_inode, "..")]
        for program in sorted((program_dir / "bin").iterdir(), key=lambda p: p.name.lower()):
            if program.is_file():
                inode = self.add_small_file(program.read_bytes(), IALLOC | PERM_755)
                bin_entries.append((inode, program.name))

        dev_entries = [(dev_inode, "."), (root_inode, ".."), (tty_inode, "tty1")]
        root_entries = [
            (root_inode, "."),
            (root_inode, ".."),
            (dev_inode, "dev"),
            (bin_inode, "bin"),
            (shell_inode, "Shell.exe"),
        ]

        self.write_directory(bin_inode, bin_entries)
        self.write_directory(dev_inode, dev_entries)
        self.write_directory(root_inode, root_entries)
        self.write_superblock()

        self.output.parent.mkdir(parents=True, exist_ok=True)
        self.output.write_bytes(self.image)


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: build_image.py <workspace> <output-image>", file=sys.stderr)
        return 2
    ImageBuilder(Path(sys.argv[1]), Path(sys.argv[2])).build()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
