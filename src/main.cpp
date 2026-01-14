#include "chip8.hpp"
#include "display.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <rom_file>" << std::endl;
    return 1;
  }

  Chip8 chip8;
  Display display;

  if (!display.init(10)) {
    std::cerr << "Erreur d'initialisation de l'affichage" << std::endl;
    return 1;
  }

  if (!chip8.loadROM(argv[1])) {
    std::cerr << "Erreur de chargement de la ROM" << std::endl;
    return 1;
  }

  // Timing: ~500 Hz pour les instructions, 60 Hz pour les timers
  const int INSTRUCTIONS_PER_SECOND = 500;
  const auto cycleDelay =
      std::chrono::microseconds(1000000 / INSTRUCTIONS_PER_SECOND);

  int cycleCount = 0;
  bool running = true;

  while (running) {
    auto start = std::chrono::high_resolution_clock::now();

    // Gérer les événements (clavier, fermeture)
    // Accès direct au keypad via un tableau temporaire
    uint8_t keypad[16] = {0};
    running = display.processEvents(keypad);

    // Mettre à jour l'état des touches
    for (int i = 0; i < 16; ++i) {
      chip8.setKey(i, keypad[i] != 0);
    }

    // Exécuter un cycle CPU
    chip8.cycle();

    // Mettre à jour les timers (60 Hz)
    if (++cycleCount >= INSTRUCTIONS_PER_SECOND / 60) {
      chip8.updateTimers();
      cycleCount = 0;
    }

    // Rafraîchir l'affichage si nécessaire
    if (chip8.drawFlag) {
      display.render(chip8.display.data());
      chip8.drawFlag = false;
    }

    // Attendre pour maintenir le timing
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (elapsed < cycleDelay) {
      std::this_thread::sleep_for(cycleDelay - elapsed);
    }
  }

  return 0;
}
