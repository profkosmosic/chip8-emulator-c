#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include "config.h"
#include <stdbool.h>

struct chip8Keyboard {
    bool keyboard[CHIP8_KEYS];
};

int chip8KeyboardMap(const char *map, char key);
void chip8KeyDown(struct chip8Keyboard *keyboard, int key);
void chip8KeyUp(struct chip8Keyboard *keyboard, int key);
bool chip8IsKeyDown(struct chip8Keyboard *keyboard, int key);

#endif