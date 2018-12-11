#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "boss.h"
#include "hud.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

private:
  const RenderContext& rc;
  const IoMod& io;
  const Gamedata& gdata;
  const Hud& hud;
  Clock& clock;

  SDL_Renderer * const renderer;
  World front;
  World back;
  Viewport& viewport;

  Player* player;
  Boss* boss;
  std::vector<Drawable*> sprites;
  int currentSprite;

  bool bossAlive;
  bool makeVideo;
  bool gameOver;

  int hudX;
  int hudY;

  void draw() const;
  void update(Uint32);
  void endGame() const;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
