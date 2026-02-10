//
// Created by dev on 2/9/26.
//

#ifndef INC_8080_EMULATOR_CPU_H
#define INC_8080_EMULATOR_CPU_H

#include <stdint.h>
#include <stdbool.h>

#include "ram.h"

// 8080 registers (A is accumulator). M represents memory reference at (H<<8|L).
typedef enum {
    REG_B = 0,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_M, // pseudo-register that refers to memory at (H,L)
    REG_A,
    REG_COUNT
} reg_index_t;

typedef struct {
    // 8-bit registers: B,C,D,E,H,L,M/A layout where M is not stored but encoded
    uint8_t regs[REG_COUNT];

    // Program counter and stack pointer
    uint16_t pc;
    uint16_t sp;

    // Individual flags (8080): Zero, Sign, Parity, Carry, Auxiliary Carry
    struct {
        bool z;  // zero
        bool s;  // sign
        bool p;  // parity
        bool cy; // carry
        bool ac; // auxiliary carry
    } flags;

    // Interrupt enable and HALT state
    bool interrupt_enable;
    bool halt;
} cpu_t;

// Initialize CPU to a known state
void cpu_init(cpu_t *cpu);

// Execute a single instruction (fetch & execute)
void step(cpu_t *cpu, memory_t *mem);

#endif //INC_8080_EMULATOR_CPU_H

