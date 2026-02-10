//
// Created by dev on 2/9/26.
//

#include "ram.h"

#include <string.h>

void mem_init(memory_t *mem) {
    if (mem == nullptr) return;
    memset(mem->cells, 0, RAM_SIZE);
}

bool mem_write(memory_t *mem, uint16_t addr, uint8_t value) {
    if (mem == nullptr) return false;
    mem->cells[addr] = value;
    return true;
}

uint8_t mem_read(memory_t *mem, uint16_t addr) {
    if (mem == nullptr) return 0;
    return mem->cells[addr];
}