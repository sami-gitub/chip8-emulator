#ifndef MENU_HPP
#define MENU_HPP

#include <SDL2/SDL.h>
#include <filesystem>
#include <string>
#include <vector>

class Menu {
public:
  Menu();

  bool init(SDL_Renderer *renderer);
  void scanRoms(const std::string &directory);
  int run(SDL_Renderer *renderer); // Retourne l'index selectionne, -1 si quit

  const std::string &getSelectedRom() const;

private:
  std::vector<std::string> romFiles;
  int selectedIndex = 0;
  SDL_Texture *fontTexture = nullptr;

  void render(SDL_Renderer *renderer);
  void drawText(SDL_Renderer *renderer, const std::string &text, int x, int y,
                bool selected);
};

#endif // MENU_HPP
