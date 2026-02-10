//
// Created by dev on 2/9/26.
//

#ifndef INC_8080_EMULATOR_RAM_H
#define INC_8080_EMULATOR_RAM_H

#include <stdint.h>

#define RAM_SIZE 65536
#define START_ADDRESS 0x0000
#define END_ADDRESS   0xFFFF

typedef struct {
    uint8_t cells[RAM_SIZE];
} memory_t;

void mem_init(memory_t *mem);
bool mem_write(memory_t *mem, uint16_t addr, uint8_t value);
uint8_t mem_read(memory_t *mem, uint16_t addr);

#endif //INC_8080_EMULATOR_RAM_H