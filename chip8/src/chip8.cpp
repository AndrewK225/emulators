#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include "chip8.h"

unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    for (int i = 0; i < 2048; ++i) {
        gfx[i] = 0;
    }
    for (int i = 0; i < 16; ++i) {
        stack[i] = 0;
        keypad[i] = 0;
        V[i] = 0;
    }
    for(int i = 0; i < 4096; ++i) {
        memory[i] = 0;
    }
    for (int i = 0; i < 80; ++i) {
        memory[i] = chip8_fontset[i];
    }

    delayTimer = 0;
    soundTimer = 0;
    srand(time(NULL));
}
Chip8::~Chip8() {}

bool Chip8::loadROM(const char *rom) {
    std::basic_ifstream<unsigned char> romFile(rom,std::ios::in | std::ios::binary);
    if(romFile) {
        std::streamsize romSize = romFile.tellg();
        romFile.seekg(0,std::ios::beg);
        if (romSize < (4096-512)) {
            romFile.read(&memory[512],romSize);
            return true;
        } else {
            std::cerr << "ROM file too big for memory" << std::endl;
            return false;
        }
    }

}

void Chip8::cycle() {
    opcode = memory[pc] << 8 | memory[pc+1]; //bitwise math to pull 2bytes
    switch(opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    for (int i = 0; i < 2048; ++i) {
                        gfx[i] = 0;
                    }
                    drawFlag = true;
                    pc+=2;
                    break;
                case 0x000E:
                    --sp;
                    pc = stack[sp];
                    pc += 2;
                    break;
                default:
                    std::cerr << "invalid opcode: " << opcode << std::endl;
                    exit(1);
            }
            break;

        case 0x1000:
            pc = opcode & 0x0FFF;
            break;

        case 0x2000:
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;
        


        default:
            std::cerr << "invalid opcode: " << opcode << std::endl;
            exit(3);    
    }
    

}
