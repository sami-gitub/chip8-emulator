#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <string>
#include <array>
#include <random>

class Chip8 {
public:
    // Constantes
    static constexpr int MEMORY_SIZE = 4096;
    static constexpr int NUM_REGISTERS = 16;
    static constexpr int STACK_SIZE = 16;
    static constexpr int NUM_KEYS = 16;
    static constexpr int DISPLAY_WIDTH = 64;
    static constexpr int DISPLAY_HEIGHT = 32;
    static constexpr uint16_t START_ADDRESS = 0x200;
    static constexpr uint16_t FONTSET_START = 0x50;

    // État public pour l'affichage
    std::array<uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> display{};
    bool drawFlag = false;

    // Constructeur
    Chip8();

    // Méthodes principales
    void initialize();
    bool loadROM(const std::string& filename);
    void cycle();
    void updateTimers();

    // Input
    void setKey(int key, bool pressed);
    bool isKeyPressed(int key) const;

private:
    // Mémoire et registres
    std::array<uint8_t, MEMORY_SIZE> memory{};
    std::array<uint8_t, NUM_REGISTERS> V{};  // Registres V0-VF
    uint16_t I = 0;                           // Registre Index
    uint16_t pc = START_ADDRESS;              // Program Counter
    
    // Stack
    std::array<uint16_t, STACK_SIZE> stack{};
    uint8_t sp = 0;                           // Stack Pointer

    // Timers
    uint8_t delayTimer = 0;
    uint8_t soundTimer = 0;

    // Input
    std::array<uint8_t, NUM_KEYS> keypad{};

    // Random
    std::mt19937 rng;
    std::uniform_int_distribution<uint8_t> randByte;

    // Fontset
    void loadFontset();

    // Opcodes (déclarations)
    void executeOpcode(uint16_t opcode);
};

#endif // CHIP8_HPP
