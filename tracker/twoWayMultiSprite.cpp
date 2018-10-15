#include <stdlib.h>
#include <time.h>
#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include <iostream>

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :

  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),

  number(Gamedata::getInstance().getXmlInt(name+"/number")),
  imagesLeft( ImageFactory::getInstance().getImages(name + "Left") ),
  imagesRight( ImageFactory::getInstance().getImages(name + "Right") ),
  images{ imagesLeft, imagesRight },

  leftOrRight(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
  {
  if(count == 0) {
    srand(time(NULL));
    count++;
  }
  Vector2f vel = Drawable::getVelocity();
  vel[0] += (rand() % 401) - 200;
  vel[1] += (rand() % 401) - 200;
  Drawable::setVelocity(vel);
  }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s),
  number(s.number),
  imagesLeft(s.imagesLeft),
  imagesRight(s.imagesRight),
  images(s.images),
  leftOrRight(s.leftOrRight),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  number = (s.number),
  imagesLeft = (s.imagesLeft);
  imagesRight = (s.imagesRight);
  images = (s.images);
  leftOrRight = (s.leftOrRight),
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwoWayMultiSprite::draw() const {
  images[leftOrRight][currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  if(getVelocityX() > 0)
    leftOrRight = 1;
  else
    leftOrRight = 0;

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

}

int TwoWayMultiSprite::count = 0;
