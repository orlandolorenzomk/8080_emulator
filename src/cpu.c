//
// Created by dev on 2/9/26.
//

#include "../include/cpu.h"

#include <stdio.h>

#include "../include/ram.h"
#include "../include/isa.h"

#include <string.h>

// Fetch one byte from memory at PC and advance PC
static uint8_t fetch8(cpu_t *cpu, memory_t *mem) {
    uint8_t value = mem_read(mem, cpu->pc);
    cpu->pc++;
    return value;
}

// Fetch two bytes (little-endian) and advance PC by 2
static uint16_t fetch16(cpu_t *cpu, memory_t *mem) {
    uint8_t lo = fetch8(cpu, mem);
    uint8_t hi = fetch8(cpu, mem);
    return (uint16_t)lo | ((uint16_t)hi << 8);
}

// Helper: get HL combined address
static uint16_t get_hl(cpu_t *cpu) {
    uint8_t h = cpu->regs[REG_H];
    uint8_t l = cpu->regs[REG_L];
    return ((uint16_t)h << 8) | l;
}

// Helper: read register or memory (for REG_M)
static uint8_t reg_read(cpu_t *cpu, memory_t *mem, reg_index_t idx) {
    if (idx == REG_M) {
        return mem_read(mem, get_hl(cpu));
    }
    return cpu->regs[idx];
}

// Helper: write register or memory (for REG_M)
static void reg_write(cpu_t *cpu, memory_t *mem, reg_index_t idx, uint8_t value) {
    if (idx == REG_M) {
        mem_write(mem, get_hl(cpu), value);
        return;
    }
    cpu->regs[idx] = value;
}

// Flag helpers (parity calc)
static bool even_parity(uint8_t x) {
    uint8_t count = 0;
    for (; x; x &= x - 1) count++;
    return (count % 2) == 0;
}

void cpu_init(cpu_t *cpu) {
    memset(cpu->regs, 0, sizeof(cpu->regs));
    cpu->pc = 0;
    cpu->sp = 0;
    cpu->flags.z = cpu->flags.s = cpu->flags.p = cpu->flags.cy = cpu->flags.ac = false;
    cpu->interrupt_enable = false;
    cpu->halt = false;
}

void step(cpu_t *cpu, memory_t *mem) {
    if (cpu->halt) return;

    uint8_t opcode = fetch8(cpu, mem);
    printf("Executing opcode %u\n", opcode);

    switch (opcode) {

        case ISA_NOP:
            // No operation, continue
            break;
        case ISA_HLT:
            cpu->halt = true;
            printf("Halted\n");
            break;
        default:
            // MOV r,r : 0x40–0x7F except 0x76
            if ((opcode & 0xC0) == 0x40) {
                reg_index_t dest = (opcode >> 3) & 0x7;
                reg_index_t src  = opcode & 0x7;

                uint8_t value = reg_read(cpu, mem, src);
                reg_write(cpu, mem, dest, value);
                break;
            }

            // unhandled opcode
            break;
    }
}

// NOP
// HLT
// MOV
// MVI
// INR
// DCR
// JMP
// JZ
// JNZ
// LDA
// STA
// ADD
