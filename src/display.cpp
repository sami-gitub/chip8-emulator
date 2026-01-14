#include "display.hpp"
#include <iostream>

Display::Display() : scale(10) {}

Display::~Display() { cleanup(); }

bool Display::init(int scale) {
  this->scale = scale;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH * scale,
                            HEIGHT * scale, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cerr << "Window Error: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
    return false;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  if (!texture) {
    std::cerr << "Texture Error: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void Display::render(const uint8_t *framebuffer) {
  uint32_t pixels[WIDTH * HEIGHT];

  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    // Blanc si pixel allumé, noir sinon
    pixels[i] = framebuffer[i] ? 0xFFFFFFFF : 0x000000FF;
  }

  SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void Display::cleanup() {
  if (texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  SDL_Quit();
}

int Display::getChip8Key(SDL_Keycode key) {
  // Mapping clavier:
  // 1 2 3 4  ->  1 2 3 C
  // Q W E R  ->  4 5 6 D
  // A S D F  ->  7 8 9 E
  // Z X C V  ->  A 0 B F
  switch (key) {
  case SDLK_1:
    return 0x1;
  case SDLK_2:
    return 0x2;
  case SDLK_3:
    return 0x3;
  case SDLK_4:
    return 0xC;
  case SDLK_q:
    return 0x4;
  case SDLK_w:
    return 0x5;
  case SDLK_e:
    return 0x6;
  case SDLK_r:
    return 0xD;
  case SDLK_a:
    return 0x7;
  case SDLK_s:
    return 0x8;
  case SDLK_d:
    return 0x9;
  case SDLK_f:
    return 0xE;
  case SDLK_z:
    return 0xA;
  case SDLK_x:
    return 0x0;
  case SDLK_c:
    return 0xB;
  case SDLK_v:
    return 0xF;
  default:
    return -1;
  }
}

bool Display::processEvents(uint8_t *keypad) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return false;

    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        return false;
      }
      int key = getChip8Key(event.key.keysym.sym);
      if (key >= 0) {
        keypad[key] = 1;
      }
      break;
    }

    case SDL_KEYUP: {
      int key = getChip8Key(event.key.keysym.sym);
      if (key >= 0) {
        keypad[key] = 0;
      }
      break;
    }
    }
  }

  return true;
}
