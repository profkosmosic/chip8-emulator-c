#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"

struct chip8Registers {
    unsigned char V[CHIP8_DATA_REGISTERS];
    unsigned short I;
    unsigned char delayTimer;
    unsigned char soundTimer;
    unsigned short programCounter;
    unsigned char stackPointer;
};

#endif