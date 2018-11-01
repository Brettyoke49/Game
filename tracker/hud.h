#ifndef HUD__H
#define HUD__H
#include "gameData.h"
#include "ioMod.h"
#include "SDL.h"
#include "renderContext.h"

class Hud {
public:
  static Hud& getInstance();
  void draw(int) const;
  void toggle() const { show = !show; }

private:
  SDL_Renderer* renderer;
  const Gamedata& gdata;
  const IoMod& ioMod;

  //x, y, width, height
  std::vector<int> hudPos;
  SDL_Color colo;
  mutable bool show;

  void printFps(int) const;

  Hud();
  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;
};
#endif
