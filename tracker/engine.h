#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext& rc;
  const IoMod& io;
  const Gamedata& gdata;
  Clock& clock;

  SDL_Renderer * const renderer;
  World front;
  World back;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;

  int hudX;
  int hudY;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();

  void printFps() const;
};
