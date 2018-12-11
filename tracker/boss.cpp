#include "boss.h"
#include "gameData.h"
#include "imageFactory.h"

void Boss::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Boss::Boss( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  subject(nullptr),
  healthBar("HealthBar"),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  hitCount(0),
  alive(true)
{
  setScale(3.0);
}

Boss::Boss( const std::string& name, Player* subject) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  subject(subject),
  healthBar("HealthBar"),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  hitCount(0),
  alive(true)
{
  subject->attachBoss(this);
  setScale(3.0);
}

Boss::Boss(const Boss& s) :
  Drawable(s),
  images(s.images),
  subject(s.subject),
  healthBar("HealthBar"),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  hitCount( s.hitCount ),
  alive(true)
  { }

Boss& Boss::operator=(const Boss& s) {
  Drawable::operator=(s);
  images = (s.images);
  subject = (s.subject);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  hitCount = ( s.hitCount );
  alive = ( s.alive );
  return *this;
}

void Boss::draw() const {
  if(alive){
    images[currentFrame]->draw(getX(), getY(), getScale());
    healthBar.draw();
  }
}

void Boss::hit() {
  hitCount += 1;
  if(hitCount >= 10) {
    alive = false;
    return;
  }
  healthBar.update(hitCount);
}

bool Boss::update(Uint32 ticks, bool ghostsAlive) {
  if(ghostsAlive)
    advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);


  if ( getX() >= worldWidth-getScaledWidth()) {
    setX(0);
  }

  return alive;
}
