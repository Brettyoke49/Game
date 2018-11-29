#include <iostream>
#include <sstream>
#include <cmath>
#include "bulletPool.h"
#include "ioMod.h"
#include "gameData.h"

BulletPool::~BulletPool() {
  delete strategy;
}

BulletPool::BulletPool(const std::string& n) :
  name(n),
  strategy( new PerPixelCollisionStrategy),
  bulletSpeed( Gamedata::getInstance().getXmlInt(name+"/speedY")),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{ }

BulletPool::BulletPool(const BulletPool& b) :
  name(b.name),
  strategy(b.strategy),
  bulletSpeed(b.bulletSpeed),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList),
  freeList(b.freeList)
{ }

bool BulletPool::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPool::shoot(const Vector2f& position) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      Bullet b(name);
      b.setPosition( position );
      b.setVelocity( Vector2f(0, bulletSpeed) );
      bulletList.push_back( b );
    }
    else {
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity( Vector2f(0, bulletSpeed) );
      b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
	}
}

void BulletPool::draw() const {
  std::stringstream stream;
  stream << "Active bullets: " << bulletList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 30);
  stream.clear();
  stream.str("");
  stream << "Bullet pool: " << freeList.size();
  IoMod::getInstance().
    writeText(stream.str(), 500, 60);
  for ( Bullet bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) {
	timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
