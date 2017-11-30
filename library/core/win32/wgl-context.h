// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_WGL_CONTEXT_H_
#define OBSIDIAN_WGL_CONTEXT_H_
// This is only intended for usage where an win32 api window exists.

// TODO(ppofuk): remove GLEW
#include "glad/glad.h"
#include "GL/wglew.h"
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

  void set_pixel_fomat_desc(PIXELFORMATDESCRIPTOR desc) {
    pixel_format_ = desc;
  }

  bool Init();
  bool Init(HWND hwnd);
  bool Init(const Window& window) { return Init(window.get_hwnd()); }

  void Destroy();

  // Vsync or swap Interval is a means of synchronizing the swapping of the
  // front and back frame buffers with vertical blanks (v-blank): the hardware
  // event where the screen image is updated with data from the front
  // framebuffer . It is a very common means of preventing frame "tearing,"
  // (seeing half of one frame and half of another) as often seen in
  // high-motion-content graphics.
  //
  // If |enable| is true, vsync will be enabled.
  //
  // Recent GL drivers implement a new WGL/GLX extension called
  // EXT_swap_control_tear. This extension brings "adaptive vsync" as featured
  // in modern gaming consoles to the PC.
  // Adaptive vsync enables v-blank synchronization when the frame rate is
  // higher than the sync rate, but disables synchronisation when the frame rate
  // drops below the sync rate. Disabling the synchronisation on low frame rates
  // prevents the common problem where the frame rate syncs to a integer
  // fraction of the screen's refresh rate in a complex scene.
  //
  // If |adaptive| is ture, adaptive vsync will be enabled.
  void Vsync(bool enable, bool adaptive = false);

  virtual void Postrender() const { SwapBuffers(gdi_device_context_); }

  // This method is called after context successful creation and GLEW init.
  virtual void OnGLEWInit() const;

 protected:
  HWND window_handle_;
  HDC gdi_device_context_;
  HGLRC opengl_render_context_;
  PIXELFORMATDESCRIPTOR pixel_format_;
};

typedef WGLContext GLContext;

}  // namespace core

#endif  // ifndef OBSIDIAN_WGL_CONTEXT_H_
