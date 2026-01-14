#include "chip8.hpp"
#include "display.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

// Palettes de couleurs (FG, BG)
const uint32_t COLOR_SCHEMES[][2] = {
    {0xFFFFFFFF, 0x000000FF}, // Blanc sur noir
    {0x00FF00FF, 0x000000FF}, // Vert sur noir (retro)
    {0xFFAA00FF, 0x442200FF}, // Ambre
    {0x00FFFFFF, 0x000066FF}, // Cyan sur bleu
    {0xFF6600FF, 0x000000FF}, // Orange
};
const int NUM_SCHEMES = 5;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <rom_file>" << std::endl;
    return 1;
  }

  std::string romPath = argv[1];
  std::string romName = std::filesystem::path(romPath).stem().string();

  Chip8 chip8;
  Display display;

  if (!display.init(10)) {
    std::cerr << "Erreur d'initialisation de l'affichage" << std::endl;
    return 1;
  }

  if (!chip8.loadROM(romPath)) {
    std::cerr << "Erreur de chargement de la ROM" << std::endl;
    return 1;
  }

  display.setTitle("CHIP-8 - " + romName);

  // Timing
  int instructionsPerSecond = 500;
  int cycleCount = 0;
  bool running = true;
  bool paused = false;
  int colorScheme = 0;

  uint8_t keypad[16] = {0};

  while (running) {
    auto cycleDelay =
        std::chrono::microseconds(1000000 / instructionsPerSecond);
    auto start = std::chrono::high_resolution_clock::now();

    InputEvent event = display.processEvents(keypad);

    switch (event) {
    case InputEvent::Quit:
      running = false;
      break;
    case InputEvent::Pause:
      paused = !paused;
      if (paused) {
        display.setTitle("CHIP-8 - " + romName + " [PAUSE]");
      } else {
        display.setTitle("CHIP-8 - " + romName);
      }
      break;
    case InputEvent::Reset:
      chip8.initialize();
      chip8.loadROM(romPath);
      std::fill(keypad, keypad + 16, 0);
      break;
    case InputEvent::ColorNext:
      colorScheme = (colorScheme + 1) % NUM_SCHEMES;
      display.setColors(COLOR_SCHEMES[colorScheme][0],
                        COLOR_SCHEMES[colorScheme][1]);
      chip8.drawFlag = true;
      break;
    case InputEvent::ColorPrev:
      colorScheme = (colorScheme - 1 + NUM_SCHEMES) % NUM_SCHEMES;
      display.setColors(COLOR_SCHEMES[colorScheme][0],
                        COLOR_SCHEMES[colorScheme][1]);
      chip8.drawFlag = true;
      break;
    case InputEvent::SpeedUp:
      instructionsPerSecond = std::min(2000, instructionsPerSecond + 100);
      std::cout << "Vitesse: " << instructionsPerSecond << " Hz" << std::endl;
      break;
    case InputEvent::SpeedDown:
      instructionsPerSecond = std::max(100, instructionsPerSecond - 100);
      std::cout << "Vitesse: " << instructionsPerSecond << " Hz" << std::endl;
      break;
    default:
      break;
    }

    if (!paused) {
      for (int i = 0; i < 16; ++i) {
        chip8.setKey(i, keypad[i] != 0);
      }

      chip8.cycle();

      if (++cycleCount >= instructionsPerSecond / 60) {
        chip8.updateTimers();
        cycleCount = 0;
      }
    }

    if (chip8.drawFlag) {
      display.render(chip8.display.data());
      chip8.drawFlag = false;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (elapsed < cycleDelay) {
      std::this_thread::sleep_for(cycleDelay - elapsed);
    }
  }

  return 0;
}
