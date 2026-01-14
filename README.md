# Emulateur CHIP-8

Un emulateur CHIP-8 ecrit en C++17 moderne avec SDL2.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL-2.0-green.svg)
![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)

## Fonctionnalites

- Jeu d'instructions CHIP-8 complet (35 opcodes)
- Affichage 64x32 pixels avec mise a l'echelle
- Menu de selection de ROMs integre
- Pause/Resume et Reset
- 5 palettes de couleurs
- Vitesse ajustable

## Capture d'ecran

![CHIP-8 Test Suite](docs/premiertest.png)

## Prerequis

- CMake 3.16+
- SDL2
- Compilateur compatible C++17

## Compilation

```bash
# Installer SDL2 (Ubuntu/Debian)
sudo apt install libsdl2-dev

# Compiler
mkdir build && cd build
cmake ..
make

# Lancer avec menu
./chip8

# Lancer une ROM directement
./chip8 ../roms/pong.ch8
```

## Controles

### Controles de l'emulateur

| Touche | Action |
|--------|--------|
| Espace | Pause / Resume |
| F5 | Reset |
| F1 / F2 | Changer palette de couleurs |
| + / - | Ajuster la vitesse |
| Echap | Quitter |

### Mapping clavier CHIP-8

```
CHIP-8:          Clavier:
1 2 3 C          1 2 3 4
4 5 6 D    ->    Q W E R
7 8 9 E          A S D F
A 0 B F          Z X C V
```

## Structure du projet

```
chip8-emulator/
├── CMakeLists.txt       # Configuration CMake
├── README.md
├── src/
│   ├── main.cpp         # Point d'entree et boucle principale
│   ├── chip8.hpp/cpp    # CPU et opcodes
│   ├── display.hpp/cpp  # Rendu SDL2
│   └── menu.hpp/cpp     # Menu de selection
├── roms/                # ROMs de test
└── docs/                # Documentation
```

## Ressources

- [Reference CHIP-8 de Cowgod](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Suite de tests CHIP-8](https://github.com/Timendus/chip8-test-suite)

## Licence

MIT
