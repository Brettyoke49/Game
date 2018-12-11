#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "player.h"
#include "healthBar.h"

class Boss : public Drawable {
public:
  Boss(const std::string&);
  Boss(const Boss&);
  Boss(const std::string&, Player* subject);

  virtual void draw() const;
  void update(Uint32) { }
  bool update(Uint32, bool);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  void hit();

  void notify(int, int) { }

protected:
  std::vector<Image *> images;

  Player* subject;
  HealthBar healthBar;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int hitCount;

  bool alive;

  void advanceFrame(Uint32 ticks);
  Boss& operator=(const Boss&);
};
