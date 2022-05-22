#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include "config.h"

struct chip8;

struct chip8Stack {
    unsigned short stack[CHIP8_STACK_DEPTH];
};

void chip8StackPush(struct chip8 *chip8, unsigned short val);
unsigned short chip8StackPop(struct chip8 *chip8);

#endif