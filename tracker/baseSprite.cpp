#include <cmath>
#include <random>
#include <functional>
#include "baseSprite.h"
#include "gameData.h"
#include "renderContext.h"

Vector2f BaseSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

BaseSprite::BaseSprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frame( ImageFactory::getInstance().getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

BaseSprite::BaseSprite(const BaseSprite& s) :
  Drawable(s),
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

BaseSprite& BaseSprite::operator=(const BaseSprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void BaseSprite::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  frame->draw(getX(), getY(), getScale());
}

void BaseSprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScale()*frame->getHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScale()*frame->getWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }
}
