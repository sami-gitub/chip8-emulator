#include "chip8.hpp"
#include <cstring>
#include <fstream>
#include <iostream>

// Fontset standard CHIP-8 (caractères 0-F, 5 bytes chacun)
constexpr uint8_t FONTSET[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
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
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() : rng(std::random_device{}()), randByte(0, 255) { initialize(); }

void Chip8::initialize() {
  pc = START_ADDRESS;
  I = 0;
  sp = 0;
  delayTimer = 0;
  soundTimer = 0;
  drawFlag = false;

  // Vider la mémoire, les registres, l'écran, etc.
  memory.fill(0);
  V.fill(0);
  stack.fill(0);
  display.fill(0);
  keypad.fill(0);

  // Charger le fontset en mémoire
  loadFontset();
}

void Chip8::loadFontset() {
  for (size_t i = 0; i < 80; ++i) {
    memory[FONTSET_START + i] = FONTSET[i];
  }
}

bool Chip8::loadROM(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (!file.is_open()) {
    std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename
              << std::endl;
    return false;
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  if (size > (MEMORY_SIZE - START_ADDRESS)) {
    std::cerr << "Erreur: ROM trop grande pour la mémoire" << std::endl;
    return false;
  }

  file.read(reinterpret_cast<char *>(&memory[START_ADDRESS]), size);
  file.close();

  std::cout << "ROM chargée: " << filename << " (" << size << " bytes)"
            << std::endl;
  return true;
}

void Chip8::cycle() {
  // Fetch: lire l'opcode (2 bytes, big-endian)
  uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];

  // Incrémenter PC avant l'exécution
  pc += 2;

  // Decode & Execute
  executeOpcode(opcode);
}

void Chip8::updateTimers() {
  if (delayTimer > 0) {
    --delayTimer;
  }
  if (soundTimer > 0) {
    if (soundTimer == 1) {
      // TODO: Émettre un beep
    }
    --soundTimer;
  }
}

void Chip8::setKey(int key, bool pressed) {
  if (key >= 0 && key < NUM_KEYS) {
    keypad[key] = pressed ? 1 : 0;
  }
}

bool Chip8::isKeyPressed(int key) const {
  if (key >= 0 && key < NUM_KEYS) {
    return keypad[key] != 0;
  }
  return false;
}

void Chip8::executeOpcode(uint16_t opcode) {
  // Extraire les nibbles
  uint8_t x = (opcode >> 8) & 0x0F; // 2ème nibble
  uint8_t y = (opcode >> 4) & 0x0F; // 3ème nibble
  uint8_t n = opcode & 0x0F;        // 4ème nibble
  uint8_t nn = opcode & 0xFF;       // Dernier byte
  uint16_t nnn = opcode & 0x0FFF;   // Derniers 12 bits

  switch (opcode & 0xF000) {
  case 0x0000:
    switch (opcode) {
    case 0x00E0: // CLS - Clear screen
      display.fill(0);
      drawFlag = true;
      break;
    case 0x00EE: // RET - Return from subroutine
      --sp;
      pc = stack[sp];
      break;
    }
    break;

  case 0x1000: // JP addr - Jump to nnn
    pc = nnn;
    break;

  case 0x2000: // CALL addr - Call subroutine at nnn
    stack[sp] = pc;
    ++sp;
    pc = nnn;
    break;

  case 0x3000: // SE Vx, byte - Skip if Vx == nn
    if (V[x] == nn)
      pc += 2;
    break;

  case 0x4000: // SNE Vx, byte - Skip if Vx != nn
    if (V[x] != nn)
      pc += 2;
    break;

  case 0x5000: // SE Vx, Vy - Skip if Vx == Vy
    if (V[x] == V[y])
      pc += 2;
    break;

  case 0x6000: // LD Vx, byte - Set Vx = nn
    V[x] = nn;
    break;

  case 0x7000: // ADD Vx, byte - Set Vx = Vx + nn
    V[x] += nn;
    break;

  case 0x8000:
    switch (n) {
    case 0x0:
      V[x] = V[y];
      break; // LD Vx, Vy
    case 0x1:
      V[x] |= V[y];
      break; // OR Vx, Vy
    case 0x2:
      V[x] &= V[y];
      break; // AND Vx, Vy
    case 0x3:
      V[x] ^= V[y];
      break;    // XOR Vx, Vy
    case 0x4: { // ADD Vx, Vy
      uint16_t sum = V[x] + V[y];
      V[0xF] = (sum > 255) ? 1 : 0;
      V[x] = sum & 0xFF;
      break;
    }
    case 0x5: { // SUB Vx, Vy
      V[0xF] = (V[x] > V[y]) ? 1 : 0;
      V[x] -= V[y];
      break;
    }
    case 0x6: { // SHR Vx
      V[0xF] = V[x] & 0x1;
      V[x] >>= 1;
      break;
    }
    case 0x7: { // SUBN Vx, Vy
      V[0xF] = (V[y] > V[x]) ? 1 : 0;
      V[x] = V[y] - V[x];
      break;
    }
    case 0xE: { // SHL Vx
      V[0xF] = (V[x] >> 7) & 0x1;
      V[x] <<= 1;
      break;
    }
    }
    break;

  case 0x9000: // SNE Vx, Vy - Skip if Vx != Vy
    if (V[x] != V[y])
      pc += 2;
    break;

  case 0xA000: // LD I, addr - Set I = nnn
    I = nnn;
    break;

  case 0xB000: // JP V0, addr - Jump to V0 + nnn
    pc = V[0] + nnn;
    break;

  case 0xC000: // RND Vx, byte - Set Vx = random AND nn
    V[x] = randByte(rng) & nn;
    break;

  case 0xD000: { // DRW Vx, Vy, n - Draw sprite
    uint8_t xPos = V[x] % DISPLAY_WIDTH;
    uint8_t yPos = V[y] % DISPLAY_HEIGHT;
    V[0xF] = 0;

    for (unsigned int row = 0; row < n; ++row) {
      uint8_t spriteByte = memory[I + row];

      for (unsigned int col = 0; col < 8; ++col) {
        uint8_t spritePixel = (spriteByte >> (7 - col)) & 0x1;
        uint32_t screenX = (xPos + col) % DISPLAY_WIDTH;
        uint32_t screenY = (yPos + row) % DISPLAY_HEIGHT;
        uint32_t idx = screenY * DISPLAY_WIDTH + screenX;

        if (spritePixel) {
          if (display[idx]) {
            V[0xF] = 1; // Collision
          }
          display[idx] ^= 1;
        }
      }
    }
    drawFlag = true;
    break;
  }

  case 0xE000:
    switch (nn) {
    case 0x9E: // SKP Vx - Skip if key Vx is pressed
      if (keypad[V[x]])
        pc += 2;
      break;
    case 0xA1: // SKNP Vx - Skip if key Vx is not pressed
      if (!keypad[V[x]])
        pc += 2;
      break;
    }
    break;

  case 0xF000:
    switch (nn) {
    case 0x07:
      V[x] = delayTimer;
      break;     // LD Vx, DT
    case 0x0A: { // LD Vx, K - Wait for key
      bool keyPressed = false;
      for (int i = 0; i < NUM_KEYS; ++i) {
        if (keypad[i]) {
          V[x] = i;
          keyPressed = true;
          break;
        }
      }
      if (!keyPressed)
        pc -= 2; // Répéter cette instruction
      break;
    }
    case 0x15:
      delayTimer = V[x];
      break; // LD DT, Vx
    case 0x18:
      soundTimer = V[x];
      break; // LD ST, Vx
    case 0x1E:
      I += V[x];
      break; // ADD I, Vx
    case 0x29:
      I = FONTSET_START + (V[x] * 5);
      break;     // LD F, Vx
    case 0x33: { // LD B, Vx - BCD
      memory[I] = V[x] / 100;
      memory[I + 1] = (V[x] / 10) % 10;
      memory[I + 2] = V[x] % 10;
      break;
    }
    case 0x55: { // LD [I], Vx
      for (int i = 0; i <= x; ++i) {
        memory[I + i] = V[i];
      }
      break;
    }
    case 0x65: { // LD Vx, [I]
      for (int i = 0; i <= x; ++i) {
        V[i] = memory[I + i];
      }
      break;
    }
    }
    break;

  default:
    std::cerr << "Opcode inconnu: 0x" << std::hex << opcode << std::endl;
    break;
  }
}
