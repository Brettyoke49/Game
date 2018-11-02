#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if(dead) {
    if(currentFrame < deathFrames - 1 && timeSinceLastFrame / currentFrame > deathFrameInterval) {
      currentFrame += 1;
    }
  }
  else {
    if(currentFrame > walkFrames) { currentFrame = 0; }
    if (timeSinceLastFrame > frameInterval && (getVelocityX() > 5 || getVelocityX() < -5)) {
      if(getVelocityX() > 5) {
        currentFrame = (currentFrame+1) % walkFrames;
        timeSinceLastFrame = 0;
      }
      else { //If moving left, play the frames going backwards so he doesn't moonwalk
        if(currentFrame == 0)
          currentFrame = walkFrames - 1;
        else
          currentFrame = currentFrame - 1;

        timeSinceLastFrame = 0;
      }
    }
  }
}

Player::Player( const std::string& name) :
  //Only starting loc needed?
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(0,0)
          ),

  collDetector(),
  imagesLeft( ImageFactory::getInstance().getImages(name + "Left") ),
  imagesRight( ImageFactory::getInstance().getImages(name + "Right") ),
  imagesDeath( ImageFactory::getInstance().getImages(name + "Death") ),
  images{ imagesLeft, imagesRight, imagesDeath },
  observers(),

  jumping( false ),
  dead( false ),
  scale( Gamedata::getInstance().getXmlFloat(name + "/scale") ),
  action(0),
  currentFrame(0),
  walkFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  deathFrames( Gamedata::getInstance().getXmlInt(name + "Death/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  deathFrameInterval( Gamedata::getInstance().getXmlInt(name + "Death/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  baseY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
         Gamedata::getInstance().getXmlInt(name+"/speedY"))
{
  setScale(scale);
}

Player::Player(const Player& s) :
  Drawable(s),
  collDetector(s.collDetector),
  imagesLeft(s.imagesLeft),
  imagesRight(s.imagesRight),
  imagesDeath(s.imagesDeath),
  images(s.images),
  observers(s.observers),
  jumping(s.jumping),
  dead(s.dead),
  scale(s.scale),
  action(s.action),
  currentFrame(s.currentFrame),
  walkFrames( s.walkFrames ),
  deathFrames( s.deathFrames ),
  frameInterval( s.frameInterval ),
  deathFrameInterval( s.deathFrameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  baseY( s.baseY ),
  initialVelocity( s.initialVelocity )
{
  setScale(scale);
}

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  collDetector = s.collDetector;
  imagesLeft = (s.imagesLeft);
  imagesRight = (s.imagesRight);
  imagesDeath = (s.imagesDeath);
  images = (s.images);
  observers = (s.observers),
  jumping = (s.jumping);
  dead = (s.dead);
  scale = (s.scale);
  action = (s.action);
  currentFrame = (s.currentFrame);
  walkFrames = ( s.walkFrames );
  deathFrames = ( s.deathFrames );
  frameInterval = ( s.frameInterval );
  deathFrameInterval = ( s.deathFrameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  baseY = ( s.baseY );
  initialVelocity = ( s.initialVelocity );
  setScale(scale);
  return *this;
}

void Player::draw() const {
    images[action][currentFrame]->draw(getX(), getY(), getScale());
}

void Player::attach(Drawable* obs) {
  observers.push_back(obs);
}

void Player::notifyObservers() {
  for(auto o : observers) {
    o->notify(getX(), getY());
  }
}

void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.83*getVelocityX() );

  if(getY() > baseY) {
    setY(baseY);
    setVelocityY(0);
    jumping = false;
  }
  if(getY() < baseY) {
    setVelocityY( 20 + getVelocityY() );
  }
}

void Player::right() {
  if(!dead) {
    if ( getX() < worldWidth-getScaledWidth()) {
      setVelocityX(initialVelocity[0]);
    }
  }
}
void Player::left()  {
  if(!dead) {
    if ( getX() > 0) {
      setVelocityX(-initialVelocity[0]);
    }
  }
}
void Player::jump()    {
  if(!dead) {
    if(!jumping) { //Don't want to stack jumps
      jumping = true;
      setVelocityY( -initialVelocity[1] );
    }
  }
}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

bool Player::collided(){
  for(auto o : observers) {
    if(collDetector.execute(*this, *o)) {
      return true;
    }
  }
  return false;
}

void Player::update(Uint32 ticks) {
  if(dead)
    action = 2;
  else if(getVelocityX() > 0)
    action = 1;
  else
    action = 0;

  if(!dead && collided()) {
    dead = true;
    currentFrame = 0;
  }
  if(dead) {
    timeSinceLastFrame += ticks;
    if(timeSinceLastFrame > 5000) {
      dead = false;
    }
  }


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

  notifyObservers();
  stop();
}

