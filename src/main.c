#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/cpu.h"
#include "../include/ram.h"
#include "../include/isa.h"

int main(void) {
    cpu_t cpu;
    memory_t mem;

    cpu_init(&cpu);
    mem_init(&mem);

    // Manually preload registers
    cpu.regs[REG_B] = 0x12;
    cpu.regs[REG_C] = 0x34;

    /*
        Program:
        MOV A, B
        MOV B, C
        HLT
    */
    uint8_t program[] = {
        0x78, // MOV A,B
        0x41, // MOV B,C
        0x76  // HLT
    };

    memcpy(mem.cells, program, sizeof(program));

    while (!cpu.halt) {
        step(&cpu, &mem);
    }

    printf("A = %02X (expected 12)\n", cpu.regs[REG_A]);
    printf("B = %02X (expected 34)\n", cpu.regs[REG_B]);
    printf("C = %02X (expected 34)\n", cpu.regs[REG_C]);
    printf("PC = %04X\n", cpu.pc);

    return 0;
}

git init
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/orlandolorenzomk/8080_emulator.git
git push -u origin main