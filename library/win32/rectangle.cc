#include "rectangle.h"
namespace meta {

Rectangle::Rectangle() : x(0), y(0), width(0), height(0) {
}

Rectangle::Rectangle(long x, 
                     long y, 
                     long width,
                     long height) {
  this->x = x;
  this->y = y; 
  this->width = width; 
  this->height = height; 
}

Rectangle::Rectangle(RECT &rect) {
  x = rect.left;
  y = rect.top;
  width = rect.right - rect.left;
  height = rect.bottom - rect.top; 
}

} // namespace meta