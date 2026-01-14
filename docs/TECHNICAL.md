# 📐 CHIP-8 Technical Reference

## Spécifications

| Composant | Description |
|-----------|-------------|
| **Mémoire** | 4 Ko (4096 bytes), adresses `0x000` à `0xFFF` |
| **Registres** | 16 registres 8-bit (`V0` à `VF`) |
| **Index** | Registre I (16-bit) pour adresses mémoire |
| **PC** | Program Counter (16-bit), démarre à `0x200` |
| **Stack** | 16 niveaux pour les appels de sous-routines |
| **Timers** | Delay timer + Sound timer (60 Hz) |
| **Écran** | 64×32 pixels monochromes |
| **Input** | Clavier hexadécimal 16 touches (0-F) |

## Mapping mémoire
```
0x000-0x1FF : Interpréteur (fonts ici)
0x200-0xFFF : Programme ROM + RAM
```

## Opcodes

| Opcode | Description |
|--------|-------------|
| `00E0` | Clear screen |
| `00EE` | Return from subroutine |
| `1NNN` | Jump to address NNN |
| `2NNN` | Call subroutine at NNN |
| `3XNN` | Skip if VX == NN |
| `4XNN` | Skip if VX != NN |
| `5XY0` | Skip if VX == VY |
| `6XNN` | Set VX = NN |
| `7XNN` | Add NN to VX |
| `8XY0` | Set VX = VY |
| `8XY1` | VX OR VY |
| `8XY2` | VX AND VY |
| `8XY3` | VX XOR VY |
| `8XY4` | VX += VY (carry) |
| `8XY5` | VX -= VY (borrow) |
| `8XY6` | VX >>= 1 |
| `8XY7` | VX = VY - VX |
| `8XYE` | VX <<= 1 |
| `9XY0` | Skip if VX != VY |
| `ANNN` | Set I = NNN |
| `BNNN` | Jump to V0 + NNN |
| `CXNN` | VX = random AND NN |
| `DXYN` | Draw sprite |
| `EX9E` | Skip if key pressed |
| `EXA1` | Skip if key not pressed |
| `FX07` | VX = delay timer |
| `FX0A` | Wait for key |
| `FX15` | delay timer = VX |
| `FX18` | sound timer = VX |
| `FX1E` | I += VX |
| `FX29` | I = font addr |
| `FX33` | Store BCD |
| `FX55` | Store registers |
| `FX65` | Load registers |

## Ressources
- [Cowgod's Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Test Suite](https://github.com/Timendus/chip8-test-suite)
