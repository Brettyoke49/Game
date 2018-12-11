#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "collisionStrategy.h"
#include "bulletPool.h"

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
  void attach(Drawable*);
  void attachBoss(Drawable*);
  bool collided();
  void notify(int, int) { }
  void checkBullets();
  void toggleGodMode();


  void right();
  void left();
  void jump();
  void down();
  void stop();
  void shoot();

private:
  PerPixelCollisionStrategy collDetector;
  BulletPool shooter;

  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesDeath;
  std::vector<Image *> imagesInvincible;
  std::vector<std::vector<Image *>> images;
  std::vector<Drawable*> observers;
  Drawable* boss;

  bool jumping;
  bool dead;
  bool godMode;
  int scale;
  int action;
  int lives;
  unsigned currentFrame;
  unsigned walkFrames;
  unsigned deathFrames;
  unsigned invFrames;

  unsigned frameInterval;
  unsigned deathFrameInterval;
  unsigned invFrameInterval;
  float timeSinceLastFrame;
  int invincibilityPeriod;
  int worldWidth;
  int worldHeight;

  int baseY;
  Vector2f initialVelocity;

  void notifyObservers();
  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
