#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[leftOrRight][currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[leftOrRight][currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[leftOrRight][currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[leftOrRight][currentFrame]->getSurface();
  }
  int number;

protected:
  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesRight;
  std::vector<std::vector<Image *>> images;

  static int count;
  int leftOrRight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
