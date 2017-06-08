#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "cef/cef-render-handler.h"

namespace render {
namespace cef {

bool RenderHandler::Init(WindowImpl* window) {
  window_ = window;
  if (window_->is_init()) {
    width_ = window_->width();
    height_ = window_->height();
    is_init_ = true;
    return true;
  }

  return false;
}


bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  if (window_ && window_->is_init()) {
    rect = CefRect(0, 0, window_->width(), window_->height());
    if (width_ != window_->width() || height_ != window_->height()) {
      width_ = window_->width();
      height_ = window_->height();
      texture_.Generate(width_, height_, GL_BGRA);
    }
    return true;
  } else {
    return false;
  }
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height) {
  if (texture_.width() != width || texture_.height() != height)
    texture_.Generate(width, height, GL_BGRA);
  void* ptr = texture_.Map();
  if (ptr) {
    memcpy(ptr, buffer, width * height * 4);
    texture_.Unmap();
  } else {
    util::Log << util::kLogDateTime
              << ": PBO mapped nullptr in RenderHandler::OnPaint(...)\n";
  }
}


void RenderHandler::Destroy() {
  is_init_ = false;
}

}  // namespace cef
}  // namespace render
