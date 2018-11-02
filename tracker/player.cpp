#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval && (getVelocityX() > 5 || getVelocityX() < -5)) {
    if(getVelocityX() > 5) {
      currentFrame = (currentFrame+1) % numberOfFrames;
      timeSinceLastFrame = 0;
    }
    else { //If moving left, play the frames going backwards so he doesn't moonwalk
      if(currentFrame == 0)
        currentFrame = numberOfFrames - 1;
      else
        currentFrame = currentFrame - 1;

      timeSinceLastFrame = 0;
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

  imagesLeft( ImageFactory::getInstance().getImages(name + "Left") ),
  imagesRight( ImageFactory::getInstance().getImages(name + "Right") ),
  images{ imagesLeft, imagesRight },
  observers(),

  jumping( false ),
  scale( Gamedata::getInstance().getXmlFloat(name + "/scale") ),
  leftOrRight(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
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
  imagesLeft(s.imagesLeft),
  imagesRight(s.imagesRight),
  images(s.images),
  observers(s.observers),
  jumping(s.jumping),
  scale(s.scale),
  leftOrRight(s.leftOrRight),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
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
  imagesLeft = (s.imagesLeft);
  imagesRight = (s.imagesRight);
  images = (s.images);
  observers = (s.observers),
  jumping = (s.jumping);
  scale = (s.scale);
  leftOrRight = (s.leftOrRight);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  baseY = ( s.baseY );
  initialVelocity = ( s.initialVelocity );
  setScale(scale);
  return *this;
}

void Player::draw() const {
  images[leftOrRight][currentFrame]->draw(getX(), getY(), getScale());
}

void Player::attach(Drawable* obs) {
  observers.push_back(obs);
}

void Player::notifyObservers() {
  for(auto o : observers) {
    o->notify(getX(), getY());
  }
}

bool collided(const std::vector<Drawable*>) {
  std::cout << "test" << std::endl;
  return false;
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
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::jump()    {
  if(!jumping) { //Don't want to stack jumps
    jumping = true;
    setVelocityY( -initialVelocity[1] );
  }
}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
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

  notifyObservers();
  stop();
}

