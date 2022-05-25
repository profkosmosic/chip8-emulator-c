#include "chip8_memory.h"
#include <assert.h>

static void chip8MemoryInBounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void chip8MemorySet(struct chip8Memory *memory, int index, unsigned char val) {
    chip8MemoryInBounds(index);
    memory->memory[index] = val;
}

unsigned char chip8MemoryGet(struct chip8Memory *memory, int index) {
    chip8MemoryInBounds(index);
    return memory->memory[index];
}

unsigned short chip8MemoryGetShort(struct chip8Memory *memory, int index) {
    unsigned char byte1 = chip8MemoryGet(memory, index);
    unsigned char byte2 = chip8MemoryGet(memory, index+1);
    return byte1 << 8 | byte2;
}