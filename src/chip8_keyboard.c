#include "chip8_keyboard.h"
#include <assert.h>

static void chip8KeyboardInBounds(int key) {
    assert(key >= 0 && key < CHIP8_KEYS);
}

int chip8KeyboardMap(const char *map, char key) {
    for(int i = 0; i < CHIP8_KEYS; i++) {
        if(map[i] == key) {
            return i;
        }
    }
    return -1;
}

void chip8KeyDown(struct chip8Keyboard *keyboard, int key) {
    keyboard->keyboard[key] = true;
}

void chip8KeyUp(struct chip8Keyboard *keyboard, int key) {
    keyboard->keyboard[key] = false;
}

bool chip8IsKeyDown(struct chip8Keyboard *keyboard, int key) {
    return keyboard->keyboard[key];
}