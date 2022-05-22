#include "chip8_stack.h"
#include "chip8.h"
#include <assert.h>

static void chip8StackInBounds(struct chip8 *chip8) {
    assert(chip8->registers.stackPointer < CHIP8_STACK_DEPTH);
}

void chip8StackPush(struct chip8 *chip8, unsigned short val) {
    chip8StackInBounds(chip8);
    chip8->stack.stack[chip8->registers.stackPointer] = val; // Accessing chip8 stack under the index of stack pointer and assigning value to it
    ++(chip8->registers.stackPointer);
}

unsigned short chip8StackPop(struct chip8 *chip8) {
    --(chip8->registers.stackPointer);
    chip8StackInBounds(chip8);
    unsigned short val = chip8->stack.stack[chip8->registers.stackPointer];
    return val;
}
