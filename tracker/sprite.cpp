#include <cmath>
#include <random>
#include <functional>
#include "sprite.h"
#include "gameData.h"
#include "imageFactory.h"

Vector2f Sprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel),
  number( Gamedata::getInstance().getXmlInt(n+"/number")),
  image( img ),
  subject(nullptr),
  attacking(false),
  resetting(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  number( Gamedata::getInstance().getXmlInt(name+"/number")),
  image( ImageFactory::getInstance().getImage(name) ),
  subject(nullptr),
  attacking(false),
  resetting(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
  if(count == 0) {
    srand(time(NULL));
    count++;
  }
  //Some initial randomization, works with data driven xml stuff
  Vector2f vel = getVelocity();
  vel[0] += (rand() % 201) - 100;
  if(rand() % 2 == 0) vel[0] = -vel[0];
  setVelocity(vel);

  Vector2f pos = getPosition();
  pos[0] = (worldWidth - (rand() % worldWidth) + 50);
  pos[1] += (rand() % 251) - 125;
  startPos[0] = pos[0];
  startPos[1] = pos[1];
  setPosition(pos);
}

Sprite::Sprite(const std::string& name, Player* subject) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  number( Gamedata::getInstance().getXmlInt(name+"/number")),
  image( ImageFactory::getInstance().getImage(name) ),
  subject(subject),
  attacking(false),
  resetting(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
  subject->attach(this);
  if(count == 0) {
    srand(time(NULL));
    count++;
  }
  //Some initial randomization, works with data driven xml stuff
  Vector2f vel = getVelocity();
  vel[0] += (rand() % 201) - 100;
  if(rand() % 2 == 0) vel[0] = -vel[0];
  setVelocity(vel);

  Vector2f pos = getPosition();
  pos[0] = (worldWidth - (rand() % worldWidth) + 50);
  pos[1] += (rand() % 251) - 125;
  startPos[0] = pos[0];
  startPos[1] = pos[1];
  setPosition(pos);
}

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  number(s.number),
  image(s.image),
  subject(s.subject),
  attacking(s.attacking),
  resetting(s.resetting),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  number = rhs.number;
  image = rhs.image;
  subject = rhs.subject;
  attacking = rhs.attacking;
  resetting = rhs.resetting;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale());
}

void Sprite::notify(int x, int y) {
  subjPos[0] = x;
  subjPos[1] = y;
}

void Sprite::attack() {
  if(getVelocityY() == 0) {
    setVelocityY(200);
  }
  if(getY() >= worldHeight - 50) {
    resetting = true;
    attacking = false;
  }
}

void Sprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if((attacking || rand() % 500 == 0) && !resetting) {
    attacking = true;
    attack();
  }
  if(resetting) { //to reset, float back to highground and move left/right
    if(getVelocityY() != -200) {
      setVelocityY(-150);
      setVelocityX(0);
    }
    if(getY() <= startPos[1]) {
      setVelocityY(0);
      setVelocityX(Gamedata::getInstance().getXmlInt(name+"/speedX") + rand() % 201 - 100);
      resetting = false;
    }
  }

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }
}

int Sprite::count = 0;
