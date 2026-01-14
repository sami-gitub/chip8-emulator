#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>
#include <cstdint>

class Display {
public:
  Display();
  ~Display();

  bool init(int scale = 10);
  void render(const uint8_t *framebuffer);
  void cleanup();
  bool processEvents(uint8_t *keypad);

private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
  int scale;

  static constexpr int WIDTH = 64;
  static constexpr int HEIGHT = 32;

  // Mapping clavier CHIP-8
  int getChip8Key(SDL_Keycode key);
};

#endif // DISPLAY_HPP
