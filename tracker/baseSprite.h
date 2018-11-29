#ifndef BaseSprite__H
#define BaseSprite__H
#include <string>
#include "drawable.h"

class BaseSprite : public Drawable {
public:
  BaseSprite(const std::string&);
  BaseSprite(const BaseSprite&);
  virtual ~BaseSprite() { }
  BaseSprite& operator=(const BaseSprite&);

  virtual const Image* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);

protected:
  Image* frame;

private:
  int worldWidth;
  int worldHeight;

  int getDistance(const BaseSprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
