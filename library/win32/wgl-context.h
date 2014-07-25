// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_WGL_CONTEXT_H_
#define OBSIDIAN_WGL_CONTEXT_H_
// This is only intended for usage where an win32 api window exists.

#include "GL/glew.h"
#include "window.h"

namespace core {

class WGLContext : util::HasLog {
 public:
  WGLContext();
  WGLContext(HWND hwnd);

  HWND get_hwnd() const { return window_handle_; }

  virtual PIXELFORMATDESCRIPTOR pixel_format_desc(u8 color_depth = 32,
                                                  u8 depth_buffer = 24,
                                                  u8 stencil_buffer = 8);
  void set_pixel_format_desc(u8 color_depth = 32,
                             u8 depth_buffer = 24,
                             u8 stencil_buffer = 8) {
    pixel_format_ = pixel_format_desc();
  }

  bool Init();
  bool Init(HWND hwnd);

  void Destroy();

  virtual void Postrender() const { SwapBuffers(gdi_device_context_); }

  // This method is called after context successful creation and GLEW init.
  virtual void OnGLEWInit() const; 

 protected:
  HWND window_handle_;
  HDC gdi_device_context_;
  HGLRC opengl_render_context_;
  PIXELFORMATDESCRIPTOR pixel_format_;
};

}  // namespace core

#endif  // ifndef OBSIDIAN_WGL_CONTEXT_H_
