#include <sstream>
#include "hud.h"

Hud& Hud::getInstance() {
  static Hud hud;
  return hud;
}

Hud::Hud() :
  renderer(RenderContext::getInstance().getRenderer() ),
  gdata(Gamedata::getInstance()),
  ioMod(IoMod::getInstance()),
  hudPos{ gdata.getXmlInt("HUD/x"),
          gdata.getXmlInt("HUD/y"),
          gdata.getXmlInt("HUD/width"),
          gdata.getXmlInt("HUD/height") },
  colo{ (Uint8)gdata.getXmlInt("HUD/r"),
        (Uint8)gdata.getXmlInt("HUD/g"),
        (Uint8)gdata.getXmlInt("HUD/b"),
        255 },
  show(true)
{
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Hud::draw(int FPS) const {
  if(show) {
    SDL_Rect outline = { hudPos[0], hudPos[1], hudPos[2], hudPos[3] };

    //Outline
    SDL_SetRenderDrawColor( renderer, colo.r, colo.g, colo.b, 255);
    SDL_RenderDrawRect( renderer, &outline );

    //Fill (Transparent)
    SDL_SetRenderDrawColor( renderer, colo.r, colo.g, colo.b, 100);
    SDL_RenderFillRect( renderer, &outline );

    printFps(FPS);

    std::vector<string> menuItems = {
      "F1 - Toggle HUD",
      "P - Pause",
      "WAD - Move/Jump",
      "Space - Shoot",
      "F4 - Record"
    };
    int space = 2;
    for(string s : menuItems) {
      ioMod.writeText(s, hudPos[0] + 10, hudPos[1] + (25 * space),
                      s.length() * 10, 25);
      space++;
    }
  }

  ioMod.writeText("Brett Probert", 10,
                  gdata.getXmlInt("view/height") - 25,
                  {0xff, 0, 200, 200} );
}

void Hud::printFps(int FPS) const {
  std::stringstream fpsout;
  fpsout << "FPS: " << FPS;

  ioMod.writeText(fpsout.str(), hudPos[0] + 10, hudPos[1] + 10);
}
