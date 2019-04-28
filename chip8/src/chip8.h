#ifndef CHIP_8_H
#define CHIP_8_H
#include <cstdint>
#include <vector>
class Chip8 {
    private:
        std::vector<uint8_t> memory = std::vector<uint8_t>(4096);
        std::vector<uint8_t> V = std::vector<uint8_t>(16);
        uint16_t I;

        std::vector<uint16_t> stack = std::vector<uint16_t>(16);
        uint16_t sp;

        uint16_t pc;
        uint16_t opcode;
        
        uint8_t delayTimer;
        uint8_t soundTimer;


    public:
        std::vector<uint8_t> gfx = std::vector<uint8_t>(2048);  //64x32 pixel resolution
        std::vector<uint8_t> keypad = std::vector<uint8_t>(16);
        bool drawFlag;

        Chip8();
        ~Chip8();

        void cycle();
        bool loadROM(const char *rom);


};

#endif
