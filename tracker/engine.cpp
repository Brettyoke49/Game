#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
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
  sound(),
  renderer( rc.getRenderer() ),
  front("front", Gamedata::getInstance().getXmlInt("front/factor") ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Skeleton")),
  boss(new Boss("Boss", player)),
  sprites {
    new Sprite("Ghost", player)
  },
  currentSprite(0),
  bossAlive( true ),
  makeVideo( false ),
  gameOver( false ),
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

  if(gameOver) {
    endGame();
  }

  player->draw();
  boss->draw();
  hud.draw(clock.getFps());
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  player->update(ticks);
  bossAlive = boss->update(ticks, true);
  bool anyAlive = false;

  for(auto i : sprites) {
    i->update(ticks);
    if(!bossAlive)
      Sprite::bossAlive = false;
    if(!dynamic_cast<Sprite*>(i)->isDead()) {
      anyAlive = true;
    }
  }

  if(!anyAlive) {
    gameOver = true;
  }
  if(player->deadForGood()) {
    gameOver = true;
  }

  back.update();
  front.update();
  viewport.update(); // always update viewport last
}

void Engine::endGame() const {
  if(player->deadForGood()) {
    std::string endText = "Oh dear, you are dead! Press R to restart, or ESC to quit.";
    io.writeText(endText, 300, 500, {255, 0, 0, 0});
  }
  else {
    std::string endText = "Congratulations! You have defeated all the ghosts! Press R to"
      " restart, or ESC to quit.";
    io.writeText(endText, 150, 500, {255, 50, 50, 0});
  }
}

bool Engine::play() {
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
        if (keystate[SDL_SCANCODE_ESCAPE]){
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
          if(!player->deadForGood()) {
          sound[0];
          player->shoot();
          }
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          player->toggleGodMode();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          std::cout << "Restarting Game..." << std::endl;
          return true;
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
      if ( keystate[SDL_SCANCODE_D] ) {
        player->right();
      }
      if ( keystate[SDL_SCANCODE_S] ) {
        player->down();
      }
    }
  }
  return false;
}
