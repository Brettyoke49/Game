#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "collisionStrategy.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[action][currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[action][currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[action][currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[action][currentFrame]->getSurface();
  }
  void attach(Drawable* obs);
  bool collided();
  void notify(int, int) { }


  void right();
  void left();
  void jump();
  void down();
  void stop();

private:
  PerPixelCollisionStrategy collDetector;

  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesDeath; //TODO
  std::vector<std::vector<Image *>> images;
  std::vector<Drawable*> observers;

  bool jumping;
  bool dead;
  int scale;
  int action;
  unsigned currentFrame;
  unsigned walkFrames;
  unsigned deathFrames; //TODO

  unsigned frameInterval;
  unsigned deathFrameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  int baseY;
  Vector2f initialVelocity;

  void notifyObservers();
  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
