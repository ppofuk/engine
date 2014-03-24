#ifndef WINDOW_HANDLES_H_
#define WINDOW_HANDLES_H_

#include <windows.h>

#define WIN_HANDLES_SIZE 32

namespace util {
class Window;

struct WindowHandleAssoc {
  HWND hwnd;
  util::Window* window;
};

// We use a simple unsorted array to associate handles with
// different Windows.
class WindowHandles {
 public:
  WindowHandles();

  // Add association to array.
  void Add(HWND hwnd, util::Window* window);

  // Get Window that had handle hwnd.
  // Returns NULL if no window is found.
  Window* Get(HWND hwnd);

  // Remove window with given handle from list.
  void Remove(HWND hwnd);

 private:
  WindowHandleAssoc collection_[WIN_HANDLES_SIZE];
  size_t size_;
};

} // namespace util

#endif
