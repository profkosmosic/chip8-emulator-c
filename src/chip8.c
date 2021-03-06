#include "chip8.h"
#include "memory.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"

const char chip8DefaultCharacterSet[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

void chip8Init(struct chip8 *chip8) {
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8DefaultCharacterSet, sizeof(chip8DefaultCharacterSet));
}

static void chip8ExecExtendedEight(struct chip8* chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char finalFourBits = opcode & 0x000f;
    unsigned short tmp = 0;
    switch(finalFourBits)
    {
        // 8xy0 - LD Vx, Vy. Vx = Vy
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
            break;

        // 8xy1 - OR Vx, Vy. Performs a bitwise OR on Vx and Vy stores the result in Vx
        case 0x01:
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
            break;

        // 8xy2 - AND Vx, Vy. Performs a bitwise AND on Vx and Vy stores the result in Vx
        case 0x02:
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
            break;

        // 8xy3 - XOR Vx, Vy. Performs a bitwise XOR on Vx and Vy stores the result in Vx
        case 0x03:
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
            break;

        // 8xy4 - ADD Vx, Vy. Set Vx = Vx + Vy, set VF = carry
        case 0x04:
            tmp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = false;
            if(tmp > 0xff) chip8->registers.V[0x0f] = true;
            chip8->registers.V[x] = tmp;
            break;

        // 8xy5 - SUB Vx, Vy. Set vx = Vx - Vy, set VF = Not borrow
        case 0x05:
            chip8->registers.V[0x0f] = false;
            if(chip8->registers.V[x] > chip8->registers.V[y]) chip8->registers.V[0x0f] = true;
            chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
            break;

        // 8xy6 - SHR Vx {, Vy}
        case 0x06:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] = chip8->registers.V[x] / 2;
            break;

        // 8xy7 - SUBN Vx, Vy
        case 0x07:
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
            break;

        // 8xyE - SHL Vx {, Vy}
        case 0x0E:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] = chip8->registers.V[x] * 2;
            break;
    }
}

static char chip8WaitForKeyPress(struct chip8* chip8)
{
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        if(event.type != SDL_KEYDOWN) continue;
        char c = event.key.keysym.sym;
        char chip8Key = chip8KeyboardMap(&chip8->keyboard, c);
        if(chip8Key != -1) return chip8Key;
    }
    return -1;
}

static void chip8ExecExtendedF(struct chip8* chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff)
    {
        // fx07 - LD Vx, DT. Set Vx to the delay timer value
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delayTimer;
            break;

        // fx0a - LD Vx, K
        case 0x0A:
            {char pressedKey = chip8WaitForKeyPress(chip8);
            chip8->registers.V[x] = pressedKey;}
            break; 

        // fx15 - LD DT, Vx, set the delay timer to Vx
        case 0x15:
            chip8->registers.delayTimer = chip8->registers.V[x];
            break;

        // fx18 - LD ST, Vx, set the sound timer to Vx
        case 0x18:
            chip8->registers.soundTimer = chip8->registers.V[x];
            break;

        // fx1e - Add I, Vx
        case 0x1e:
            chip8->registers.I += chip8->registers.V[x];
            break;

        // fx29 - LD F, Vx
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
            break;

        // fx33 - LD B, Vx
        case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char units = chip8->registers.V[x] % 10;
            chip8MemorySet(&chip8->memory, chip8->registers.I, hundreds);
            chip8MemorySet(&chip8->memory, chip8->registers.I+1, tens);
            chip8MemorySet(&chip8->memory, chip8->registers.I+2, units);
        }
        break;

        // fx55 - LD [I], Vx
        case 0x55:
            {for (int i = 0; i <= x; i++)
            {
                chip8MemorySet(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }}
            break;

        // fx65 - LD Vx, [I]
        case 0x65:
            {for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = chip8MemoryGet(&chip8->memory, chip8->registers.I+i);
            }}
            break;
    }
}

static void chip8ExecExtended(struct chip8 *chip8, unsigned short opcode) {
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;
    switch(opcode & 0xf000) {
        // JP addr, 1nnn Jump to location nnn's
        case 0x1000:
            chip8->registers.programCounter = nnn;
            break;

        // CALL addr, 2nnn Call subroutine at location nnn
        case 0x2000:
            chip8StackPush(chip8, chip8->registers.programCounter);
            chip8->registers.programCounter = nnn;
            break;

        // SE Vx, byte - 3xkk Skip next instruction if Vx=kk
        case 0x3000:
            if(chip8->registers.V[x] == kk) chip8->registers.programCounter += 2;
            break;

         // SNE Vx, byte - 3xkk Skip next instruction if Vx!=kk
        case 0x4000:
            if(chip8->registers.V[x] != kk) chip8->registers.programCounter += 2;
            break;

        // 5xy0 - SE, Vx, Vy, skip the next instruction if Vx = Vy
        case 0x5000:
            if(chip8->registers.V[x] == chip8->registers.V[y]) chip8->registers.programCounter += 2;
            break;

        // 6xkk - LD Vx, byte, Vx = kk
        case 0x6000:
            chip8->registers.V[x] = kk;
            break;

        // 7xkk - ADD Vx, byte. Set Vx = Vx + kk
        case 0x7000:
            chip8->registers.V[x] += kk;
            break;

        case 0x8000:
            chip8ExecExtendedEight(chip8, opcode);
            break;

        // 9xy0 - SNE Vx, Vy. Skip next instruction if Vx != Vy
        case 0x9000:
            if(chip8->registers.V[x] != chip8->registers.V[y]) chip8->registers.programCounter += 2;
            break;

        // Annn - LD I, addr. Sets the I register to nnn
        case 0xA000:
            chip8->registers.I = nnn;
            break;

        // bnnn - Jump to location nnn + V0
        case 0xB000:
            chip8->registers.programCounter = nnn + chip8->registers.V[0x00];
            break;

        // Cxkk - RND Vx, byte
        case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
            break;

        // Dxyn - DRW Vx, Vy, nibble. Draws sprite to the screen
        case 0xD000:
            {const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = chip8ScreenDrawSprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);}
            break;

        // Keyboard operations
        case 0xE000:
            {switch(opcode & 0x00ff)
            {
                // Ex9e - SKP Vx, Skip the next instruction if the key with the value of Vx is pressed
                case 0x9e:
                    if(chip8IsKeyDown(&chip8->keyboard, chip8->registers.V[x])) chip8->registers.programCounter += 2;
                    break;

                // Exa1 - SKNP Vx - Skip the next instruction if the key with the value of Vx is not pressed
                case 0xa1:
                    if(!chip8IsKeyDown(&chip8->keyboard, chip8->registers.V[x])) chip8->registers.programCounter += 2;
                    break;
            }}
            break;
        
        case 0xF000:
            chip8ExecExtendedF(chip8, opcode);
            break;
    }
}

void chip8Exec(struct chip8 *chip8, unsigned short opcode) {
    switch(opcode) {
        // CLS - clear display
        case 0x00E0:
            chip8ScreenClear(&chip8->screen);
            break;
        // RET - return from subroutine
        case 0x00EE:
            chip8->registers.programCounter = chip8StackPop(chip8);
            break;
        default:
            chip8ExecExtended(chip8, opcode);
    }
}

void chip8Load(struct chip8 *chip8, const char *buffer, size_t size) {
    assert(size + CHIP8_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_LOAD_ADDRESS], buffer, size);
    chip8->registers.programCounter = CHIP8_LOAD_ADDRESS;
}