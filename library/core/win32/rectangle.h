#ifndef OBSIDIAN_RECTANGLE_H_
#define OBSIDIAN_RECTANGLE_H_

#include <Windows.h>

namespace meta {
class Rectangle {
 public:
   Rectangle();
   Rectangle(long x,
             long y,
             long width,
             long height);

   Rectangle(RECT &rect);

   long x, y, width, height;
};

} // namespace meta
#endif
