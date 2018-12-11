#include "SDL.h"
#include "gameData.h"
#include "drawable.h"
#include "ioMod.h"

class HealthBar : public Drawable {
public:
  HealthBar(const std::string&);
  void draw() const;
  void update(Uint32);
  void reset() { currentLength = barRect.w; }

  HealthBar(const HealthBar&) = delete;
  HealthBar& operator=(const HealthBar&) = delete;

  virtual int getScaledWidth() const  { return barRect.w; }
  virtual int getScaledHeight() const { return barRect.h; }
  virtual const Image* getImage() const {
    throw std::string("No image in HealthBar");
  }
  virtual const SDL_Surface* getSurface() const {
    throw std::string("No image in HealthBar");
  }

  void notify(int, int) { }
private:
  SDL_Renderer* renderer;
  SDL_Rect barRect;
  int currentLength;
  int increment;
  SDL_Color barColor;
  SDL_Color backColor;
  IoMod& io;
};
