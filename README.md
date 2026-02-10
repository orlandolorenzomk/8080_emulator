8080-Emulator
=============

Status: Work in progress — ongoing development.

Overview
--------
This repository is an in-progress Intel 8080 CPU emulator written in C, intended for experimentation and learning. The emulator uses a compact ISA description and a small CPU core that fetches and executes opcodes from an emulated RAM.

Project structure
-----------------
(Top-level)
- CMakeLists.txt                 - CMake build manifest
- README.md                      - This file
- include/                       - Public headers
  - cpu.h                        - 8080-style CPU structure and public API
  - isa.h                        - Compact ISA enum with base-opcode hints + examples
  - ram.h                        - Simple RAM (memory_t) abstraction
- src/                           - Source files
  - cpu.c                        - CPU implementation: fetch helpers, register helpers, MOV group, NOP, HLT
  - ram.c                        - RAM implementation
  - main / main.c                - Small runner (platform/IDE integrated)
- cmake-build-debug/             - Local CLion debug build output (ignored by Git normally)
  - 8080_emulator                - Built executable (if you built in CLion)

What is implemented so far
--------------------------
- A clear 8080-like CPU model in `include/cpu.h`:
  - 8-bit registers (B,C,D,E,H,L,A) and a pseudo-register `M` (memory at HL)
  - Program counter (PC), stack pointer (SP), and individual flags (Z,S,P,CY,AC)
  - `cpu_init()` and `step()` public functions
- `src/cpu.c` provides:
  - `fetch8()` and `fetch16()` for instruction fetching (little-endian)
  - `reg_read()` / `reg_write()` helpers that map `REG_M` to memory at HL
  - `MOV` instruction group (opcodes 0x40..0x7F, excluding 0x76 which is HLT)
  - NOP (0x00) and HLT (0x76) handling
- `include/isa.h` contains a compact base-opcode enum and concrete opcode examples to aid decoding and testing
- `src/ram.c` implements a simple RAM array of 65536 bytes (addressable 0x0000..0xFFFF)

Design notes and important semantics
----------------------------------
- REG_M is a pseudo-register. Use the provided `reg_read()` / `reg_write()` helpers to access `M` so reads/writes go to RAM at the (H<<8)|L address. Do not write directly to `cpu->regs[REG_M]` — that only changes an array slot and does not affect RAM.
- Immediate 16-bit values (addresses, LXI, etc.) are little-endian (low byte then high byte), consistent with the 8080.
- The stack grows downward (SP is decremented on push), matching 8080 conventions.

Building
--------
This project uses CMake (CLion defaults to a per-project build dir `cmake-build-debug`). To build from the command line using Ninja/CMake, from the repo root:

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

If you use CLion, the IDE will configure and build with the configured CMake profile automatically.

Running
-------
If you built with CLion the executable is typically at `cmake-build-debug/8080_emulator`.
From a command line build above, the executable will be under `build/`.

Usage examples and quick tests
-----------------------------
There are no automated tests yet, but you can exercise basic functionality by running the built binary (if it includes a CLI runner) or by writing a small harness that loads bytes into `memory_t` and calls `cpu_init()` and `step()` in a loop until `cpu->halt`.

Example test (manual):
- Assemble a tiny program in RAM: `MVI B,0x03; MVI A,0x05; MOV C,A; HLT`
  - Bytes: `0x06 0x03 0x3E 0x05 0x4F 0x76`
  - After executing the sequence, register C should contain 0x05.


Contributing
------------
When contributing, prefer small PRs that implement and test single opcode groups.

License
-------
This repository doesn't yet include an explicit license file.

Contact 
---------------
orlandolorenzomk@gmail.com
