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
  gdata( Gamedata::getInstance() ),
  hud( Hud::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  front("front", Gamedata::getInstance().getXmlInt("front/factor") ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Skeleton")),
  sprites {
    new Sprite("Ghost", player)
  },
  currentSprite(0),
  makeVideo( false ),
  hudX(gdata.getXmlInt("HUD/x")),
  hudY(gdata.getXmlInt("HUD/y"))
{
  for(int i = 0; i < static_cast<Sprite*>(sprites[0])->number - 1; i++) {
    sprites.emplace_back(new Sprite("Ghost", player));
  }

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  back.draw();
  front.draw();

  for(auto i : sprites) {
    i->draw();
  }

  player->draw();
  hud.draw(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  player->update(ticks);

  for(auto i : sprites) {
    i->update(ticks);
  }

  back.update();
  front.update();
  viewport.update(); // always update viewport last
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
        if ( keystate[SDL_SCANCODE_F1] ) {
          hud.toggle();
        }
        if ( keystate[SDL_SCANCODE_W] ) {
          player->jump();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
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
      if ( keystate[SDL_SCANCODE_A] ) {
        player->left();
      }
      if ( keystate[SDL_SCANCODE_S] ) {
        //No function... for now.
      }
      if ( keystate[SDL_SCANCODE_D] ) {
        player->right();
      }
    }
  }
}
