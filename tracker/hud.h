#pragma once
#include "gameData.h"
#include "ioMod.h"

class Hud {
public:
  static Hud& getInstance();
  void draw();
  void toggle();

private:
  Vector2f hudPos;

  Hud();
}
