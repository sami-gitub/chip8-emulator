#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>
#include <cstdint>
#include <string>

// Evenements speciaux retournes par processEvents
enum class InputEvent {
  None,
  Quit,
  Pause,
  Reset,
  ColorNext,
  ColorPrev,
  SpeedUp,
  SpeedDown
};

class Display {
public:
  Display();
  ~Display();

  bool init(int scale = 10);
  void render(const uint8_t *framebuffer);
  void cleanup();
  InputEvent processEvents(uint8_t *keypad);
  void setTitle(const std::string &title);
  void setColors(uint32_t fg, uint32_t bg);

private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
  int scale;
  uint32_t fgColor = 0xFFFFFFFF;
  uint32_t bgColor = 0x000000FF;

  static constexpr int WIDTH = 64;
  static constexpr int HEIGHT = 32;

  int getChip8Key(SDL_Keycode key);
};

#endif // DISPLAY_HPP
