// Copyright (C) 2017 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_CEF_RENDER_HANDLER_H_
#define OBSIDIAN_CEF_RENDER_HANDLER_H_

#include "cef_app.h"
#include "cef_client.h"
#include "cef_render_handler.h"


#if defined(OS_LINUX)
#include "window-xlib.h"

// Represents default window class on linux platform
typedef core::WindowXlib WindowImpl;// TODO(ppofuk): test defined(OS_WINDOWS)

#elif defined(OS_WINDOWS)
// TODO(ppofuk): test defined(OS_WINDOWS)
//               Also, on windows it should be glxcontext
#include "win32/window.h"
typedef core::Window WindowImpl;

#endif // if defined(OS_LINUX)


#include "cef-ui-shader.h"
#include "logger.h"

#include "stb_image_write.h"

namespace render {
namespace cef {

class RenderHandler : public CefRenderHandler, public util::HasLog {
 public:
  bool Init(WindowImpl* window);
  void Destroy(); 
  
  bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
  void OnPaint(CefRefPtr<CefBrowser> browser,
               PaintElementType type,
               const RectList& dirtyRects,
               const void* buffer,
               int width,
               int height) OVERRIDE;
  
  i32 width() { return width_; }
  i32 height() { return height_; }
  
  render::GLPixelBufferObjectTexture* get_texture() { return &texture_; }
 private:
  i32 width_, height_ = 0;
  render::GLPixelBufferObjectTexture texture_; 
  bool should_draw_ = false;
  WindowImpl* window_ = nullptr;
  bool is_init_; 

 public:
  IMPLEMENT_REFCOUNTING(RenderHandler); 
};

}  // namespace cef
}  // namespace render

#endif  // define OBSIDIAN_CEF_RENDER_HANDLER_H_
