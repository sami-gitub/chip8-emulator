# 🎮 CHIP-8 Emulator

A CHIP-8 emulator written in modern C++17 with SDL2.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL-2.0-green.svg)
![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)

## 🚀 Features

- [ ] Full CHIP-8 instruction set (35 opcodes)
- [ ] 64x32 pixel display with scaling
- [ ] Keyboard input mapping
- [ ] Accurate timing (60 Hz timers)
- [ ] Sound support

## 📋 Requirements

- CMake 3.16+
- SDL2
- C++17 compatible compiler

## 🔧 Build

```bash
# Install SDL2 (Ubuntu/Debian)
sudo apt install libsdl2-dev

# Build
mkdir build && cd build
cmake ..
make

# Run
./chip8 ../roms/your_rom.ch8
```

## 🎮 Controls

```
CHIP-8:          Keyboard:
1 2 3 C          1 2 3 4
4 5 6 D    →     Q W E R
7 8 9 E          A S D F
A 0 B F          Z X C V
```

## 📸 Screenshots

*Coming soon...*

## 📚 Resources

- [Cowgod's CHIP-8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite)

## 📄 License

MIT License
