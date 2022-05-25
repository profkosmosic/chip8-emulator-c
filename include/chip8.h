#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>
#include "config.h"
#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"
#include "chip8_keyboard.h"
#include "chip8_screen.h"

struct chip8 {
    struct chip8Memory memory;
    struct chip8Registers registers;
    struct chip8Stack stack;
    struct chip8Keyboard keyboard;
    struct chip8Screen screen;
};

void chip8Init(struct chip8 *chip8);
void chip8Exec(struct chip8 *chip8, unsigned short opcode);
void chip8Load(struct chip8 *chip8, const char *buffer, size_t size);

#endif