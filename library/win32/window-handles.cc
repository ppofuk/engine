#include "window-handles.h"

namespace core {

WindowHandles::WindowHandles() : size_(0) {
}

void WindowHandles::Add(HWND hwnd, Window* window) {
  bool not_collected = true;
  // Check if hwnd is still in collection. If it is, just change window.
  for (size_t i = 0; i < size_ && not_collected; ++i) {
    if (collection_[i].hwnd == hwnd) {
      collection_[i].window = window;
      not_collected = false;
    }
  }

  if (not_collected) {
    collection_[size_].hwnd = hwnd;
    collection_[size_++].window = window;
  }
}

Window* WindowHandles::Get(HWND hwnd) {
  for (size_t i = 0; i < size_; ++i)
    if (collection_[i].hwnd == hwnd)
      return collection_[i].window;

  return NULL;
}

void WindowHandles::Remove(HWND hwnd) {
  for (size_t i = 0; i < size_; ++i)
    if (collection_[i].hwnd == hwnd) {
      if (i != size_ - 1)
        collection_[i] = collection_[size_ - 1];
      size_--;
    }
}

} //  namespace util
