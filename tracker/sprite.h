#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"
#include "player.h"

class ExplodingSprite;

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, Player* subject);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel,
         const Image*);
  Sprite(const Sprite&);
  Sprite& operator=(const Sprite&);
  ~Sprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  }
  int getScaledHeight() const { return getScale()*image->getHeight(); }
  void notify(int, int);

  static bool bossAlive;

  virtual void explode();
  bool isDead();

  int number;

private:
  const Image * image;
  ExplodingSprite* explosion;
  static int count;

  Player* subject;
  int subjPos[2];
  bool attacking;
  bool resetting;
  bool dead;
  int startPos[2];

  int worldWidth;
  int worldHeight;

  void attack();
  int getDistance(const Sprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
