#pragma once
#include <iostream>
#include "baseSprite.h"
#include "gameData.h"

class Bullet : public BaseSprite {
public:
  explicit Bullet(const string& name) :
    BaseSprite(name),
    distance(0),
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
    tooFar(false)
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  virtual const Image* getImage() const { return frame; }
  virtual const SDL_Surface* getSurface() const {
    return frame->getSurface();
  }
  int getScaledWidth()  const { return getScale()*frame->getWidth();  }
  int getScaledHeight() const { return getScale()*frame->getHeight(); }

  void notify(int, int) { }

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
