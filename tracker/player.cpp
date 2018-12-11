#include <sstream>
#include "player.h"
#include "gameData.h"
#include "imageFactory.h"
#include "sprite.h"
#include "boss.h"
#include "ioMod.h"

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if(dead) {
    if(currentFrame < deathFrames - 1 && timeSinceLastFrame / currentFrame > deathFrameInterval) {
      currentFrame += 1;
    }
  }
  else if(invincibilityPeriod > 0) {
    if(timeSinceLastFrame > invFrameInterval) {
      currentFrame = (currentFrame+1) % invFrames;
      timeSinceLastFrame = 0;
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
        if(currentFrame == 0) {
          currentFrame = walkFrames - 1;
        }
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
  shooter("Trumpet"),
  imagesLeft( ImageFactory::getInstance().getImages(name + "Left") ),
  imagesRight( ImageFactory::getInstance().getImages(name + "Right") ),
  imagesDeath( ImageFactory::getInstance().getImages(name + "Death") ),
  imagesInvincible( ImageFactory::getInstance().getImages(name + "Invincible") ),
  images{ imagesLeft, imagesRight, imagesDeath, imagesInvincible },
  observers(),
  boss(nullptr),

  jumping( false ),
  dead( false ),
  godMode( false ),
  scale( Gamedata::getInstance().getXmlFloat(name + "/scale") ),
  action(0),
  lives( Gamedata::getInstance().getXmlInt(name + "/lives") ),
  currentFrame(0),
  walkFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  deathFrames( Gamedata::getInstance().getXmlInt(name + "Death/frames") ),
  invFrames( Gamedata::getInstance().getXmlInt(name + "Invincible/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  deathFrameInterval( Gamedata::getInstance().getXmlInt(name + "Death/frameInterval")),
  invFrameInterval( Gamedata::getInstance().getXmlInt(name + "Invincible/frameInterval")),
  timeSinceLastFrame( 0 ),
  invincibilityPeriod( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  viewHeight(Gamedata::getInstance().getXmlInt("view/height")),
  baseY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  initialVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
         Gamedata::getInstance().getXmlInt(name+"/speedY"))
{
  setScale(scale);
}

Player::Player(const Player& s) :
  Drawable(s),
  collDetector(s.collDetector),
  shooter(s.shooter),
  imagesLeft(s.imagesLeft),
  imagesRight(s.imagesRight),
  imagesDeath(s.imagesDeath),
  imagesInvincible(s.imagesInvincible),
  images(s.images),
  observers(s.observers),
  boss(s.boss),
  jumping(s.jumping),
  dead(s.dead),
  godMode( s.godMode ),
  scale(s.scale),
  action(s.action),
  lives(s.lives),
  currentFrame(s.currentFrame),
  walkFrames( s.walkFrames ),
  deathFrames( s.deathFrames ),
  invFrames( s.invFrames ),
  frameInterval( s.frameInterval ),
  deathFrameInterval( s.deathFrameInterval ),
  invFrameInterval( s.invFrameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  invincibilityPeriod( s.invincibilityPeriod ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  viewHeight( s.viewHeight ),
  baseY( s.baseY ),
  initialVelocity( s.initialVelocity )
{
  setScale(scale);
}

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  collDetector = (s.collDetector);
  imagesLeft = (s.imagesLeft);
  imagesRight = (s.imagesRight);
  imagesDeath = (s.imagesDeath);
  imagesInvincible = (s.imagesInvincible);
  images = (s.images);
  observers = (s.observers);
  boss = (s.boss);
  jumping = (s.jumping);
  dead = (s.dead);
  godMode = (s.godMode);
  scale = (s.scale);
  action = (s.action);
  lives = (s.lives);
  currentFrame = (s.currentFrame);
  walkFrames = ( s.walkFrames );
  deathFrames = ( s.deathFrames );
  invFrames = ( s.invFrames );
  frameInterval = ( s.frameInterval );
  deathFrameInterval = ( s.deathFrameInterval );
  invFrameInterval = ( s.invFrameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  invincibilityPeriod = ( s.invincibilityPeriod );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  viewHeight = ( s.viewHeight );
  baseY = ( s.baseY );
  initialVelocity = ( s.initialVelocity );
  setScale(scale);
  return *this;
}

void Player::draw() const {
    images[action][currentFrame]->draw(getX(), getY(), getScale());
    shooter.draw();

    std::stringstream livesLeft;
    livesLeft << "Lives: " << lives;
    IoMod::getInstance().writeText(livesLeft.str(), 1100, viewHeight-80, {135,206,250,0});

    if(godMode)
    {
      IoMod::getInstance().writeText("GODMODE ACTIVE", 10, 10);
    }
}

void Player::attach(Drawable* obs) {
  observers.push_back(obs);
}

void Player::attachBoss(Drawable* b) {
  boss = b;
}

void Player::notifyObservers() {
  for(auto o : observers) {
    o->notify(getX(), getY());
  }
}

void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.75*getVelocityX() );

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
  if(!dead && invincibilityPeriod <= 0) {
    if(!jumping) { //Don't want to stack jumps
      jumping = true;
      setVelocityY( -initialVelocity[1] );
    }
  }
}

void Player::down() {
  if(jumping) {
    setVelocityY( getVelocityY() + 200 );
  }
}

void Player::shoot() {
  if(!dead && invincibilityPeriod <= 0) {
    Vector2f shootSpot(getX() + 30, getY() + 20);
    shooter.shoot(shootSpot);
  }
}

bool Player::collided(){
  for(auto o : observers) {
    if(collDetector.execute(*this, *o) && !dynamic_cast<Sprite*>(o)->isDead()) {
      return true;
    }
  }
  return false;
}

void Player::checkBullets() {
  for(auto o : observers) {
    if(!dynamic_cast<Sprite*>(o)->isDead() && shooter.collidedWith(o)) {
      dynamic_cast<Sprite*>(o)->explode();
    }
  }
  if(shooter.collidedWith(boss)) {
    dynamic_cast<Boss*>(boss)->hit();
  }
}

void Player::toggleGodMode() {
  godMode = !godMode;
}

void Player::update(Uint32 ticks) {
  if(dead)
    action = 2;
  else if(invincibilityPeriod > 0)
    action = 3;
  else if(getVelocityX() > 0)
    action = 1;
  else
    action = 0;

  checkBullets();

  if(invincibilityPeriod > 0) { invincibilityPeriod -= ticks; }
  if(!dead && !godMode && !(invincibilityPeriod > 0) && collided()) {
    lives -= 1;
    dead = true;
    currentFrame = 0;
  }
  if(dead) {
    timeSinceLastFrame += ticks;
    if(timeSinceLastFrame > 5000 && lives != 0) {
      dead = false;
      currentFrame = 0;
      invincibilityPeriod = 3000;
    }
  }

  shooter.update(ticks);

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if (getX() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }


  notifyObservers();
  stop();

  //This is an ugly band-aid fix to an issue with displaying a bad image and seg faulting.
  if(action == 3 && currentFrame > 1) { currentFrame = 0; }
}
