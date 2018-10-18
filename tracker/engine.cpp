#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() {
  for(auto i : sprites) {
    delete i;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  gdata(Gamedata::getInstance()),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  front("front", Gamedata::getInstance().getXmlInt("front/factor") ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  viewport( Viewport::getInstance() ),
  sprites {
    new TwoWayMultiSprite("Skeleton"),
    new Sprite("Ghost"),
    new MultiSprite("Penor")
  },
  currentSprite(0),
  makeVideo( false ),
  hudX(gdata.getXmlInt("HUD/x")),
  hudY(gdata.getXmlInt("HUD/y"))
{
  for(int i = 0; i < static_cast<TwoWayMultiSprite*>(sprites[0])->number - 1; i++) {
    sprites.emplace_back(new TwoWayMultiSprite("Skeleton"));
  }

  for(int i = 0; i < static_cast<Sprite*>(sprites[1])->number - 1; i++) {
    sprites.emplace_back(new Sprite("Ghost"));
  }

  Viewport::getInstance().setObjectToTrack(sprites[0]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::printFps() const {
  std::stringstream fpsout;
  fpsout << "FPS: " << clock.getFps();
  //Data driven info placement
  IoMod::getInstance().writeText(fpsout.str(), hudX, hudY);
  IoMod::getInstance().writeText("Brett Probert", hudX, hudY + 30,
    {0xff, 0, 200, 200} );
}

void Engine::draw() const {
  back.draw();
  front.draw();

  for(auto i : sprites) {
    i->draw();
  }

  viewport.draw();
  printFps();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto i : sprites) {
    i->update(ticks);
  }

  back.update();
  front.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }


    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
