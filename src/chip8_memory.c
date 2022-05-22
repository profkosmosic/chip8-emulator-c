#include "chip8_memory.h"
#include <assert.h>

static void chip8MemoryInBounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE); // checking if chip8 memory is in bounds
}

void chip8MemorySet(struct chip8Memory *memory, int index, unsigned char val) {
    chip8MemoryInBounds(index);
    memory->memory[index] = val; // pointer instance of memory struct accessing the memory array inside of the struct and setting index to val
}

unsigned char chip8MemoryGet(struct chip8Memory *memory, int index) {
    chip8MemoryInBounds(index);
    return memory->memory[index]; // returns memory under index from memory array that is inside of memory struct
}