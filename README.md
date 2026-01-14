# 🎮 Émulateur CHIP-8

Un émulateur CHIP-8 écrit en C++17 moderne avec SDL2.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL-2.0-green.svg)
![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)

## 🚀 Fonctionnalités

- [ ] Jeu d'instructions CHIP-8 complet (35 opcodes)
- [ ] Affichage 64x32 pixels avec mise à l'échelle
- [ ] Mapping clavier
- [ ] Timing précis (timers 60 Hz)
- [ ] Support du son

## 📋 Prérequis

- CMake 3.16+
- SDL2
- Compilateur compatible C++17

## 🔧 Compilation

```bash
# Installer SDL2 (Ubuntu/Debian)
sudo apt install libsdl2-dev

# Compiler
mkdir build && cd build
cmake ..
make

# Lancer
./chip8 ../roms/votre_rom.ch8
```

## 🎮 Contrôles

```
CHIP-8:          Clavier:
1 2 3 C          1 2 3 4
4 5 6 D    →     Q W E R
7 8 9 E          A S D F
A 0 B F          Z X C V
```

## Capture d'ecran

![CHIP-8 Test Suite](docs/premiertest.png)

## 📚 Ressources

- [Référence CHIP-8 de Cowgod](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Suite de tests CHIP-8](https://github.com/Timendus/chip8-test-suite)

## 📄 Licence

Licence MIT
