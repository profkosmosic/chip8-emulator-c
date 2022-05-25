#include "chip8_screen.h"
#include <assert.h>
#include <memory.h>

static void chip8ScreenInBounds(int x, int y) {
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void chip8ScreenSet(struct chip8Screen *screen, int x, int y) {
    chip8ScreenInBounds(x, y);
    screen->screenPixels[y][x] = true;
}

bool chip8ScreenIsSet(struct chip8Screen *screen, int x, int y) {
    chip8ScreenInBounds(x, y);
    return screen->screenPixels[y][x];
}

bool chip8ScreenDrawSprite(struct chip8Screen *screen, int x, int y, const char *sprite, int num) {
    bool pixelHit = false;
    for(int ly = 0; ly < num; ly++) {
        char c = sprite[ly];
        for(int lx = 0; lx < 8; lx++) {
            if((c & (0b10000000 >> lx)) == 0) continue;
            if(screen->screenPixels[(ly + y) % CHIP8_HEIGHT][(lx + x) % CHIP8_WIDTH]) pixelHit = true;
            screen->screenPixels[(ly + y) % CHIP8_HEIGHT][(lx + x) % CHIP8_WIDTH] ^= true;
        }
    }
    return pixelHit;
}

void chip8ScreenClear(struct chip8Screen *screen) {
    memset(screen->screenPixels, 0, sizeof(screen->screenPixels));
}