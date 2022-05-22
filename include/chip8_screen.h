#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include "config.h"
#include <stdbool.h>

struct chip8Screen {
    bool screenPixels[CHIP8_HEIGHT][CHIP8_WIDTH];
};

void chip8ScreenSet(struct chip8Screen *screen, int x, int y);
bool chip8ScreenIsSet(struct chip8Screen *screen, int x, int y);
bool chip8ScreenDrawSprite(struct chip8Screen *screen, int x, int y, const char *sprite, int num);

#endif