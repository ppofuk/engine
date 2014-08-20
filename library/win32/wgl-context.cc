// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "wgl-context.h"

namespace core {

WGLContext::WGLContext()
    : window_handle_(0),
      gdi_device_context_(0),
      opengl_render_context_(0),
      pixel_format_(pixel_format_desc()) {
}

WGLContext::WGLContext(HWND hwnd)
    : window_handle_(hwnd), gdi_device_context_(0), opengl_render_context_(0) {
}

PIXELFORMATDESCRIPTOR WGLContext::pixel_format_desc(u8 color_depth,
                                                    u8 depth_buffer,
                                                    u8 stencil_buffer) {
  static PIXELFORMATDESCRIPTOR pixel_format = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      color_depth,  // Color Depth
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,

      depth_buffer,    // Depth Buffer
      stencil_buffer,  // Stencil buffer
      0,
      PFD_MAIN_PLANE,
      0,
      0,
      0,
      0};
  return pixel_format;
}

bool WGLContext::Init(HWND hwnd) {
  window_handle_ = hwnd;
  return Init();
}

bool WGLContext::Init() {
  pixel_format_ = pixel_format_desc();
  gdi_device_context_ = GetDC(window_handle_);
  if (!gdi_device_context_) {
    log << util::kLogDateTime << ": "
        << "Can't get GDI device context!\n";
    return false;
  }
  unsigned int pixel_format =
      ChoosePixelFormat(gdi_device_context_, &pixel_format_);

  if (!SetPixelFormat(gdi_device_context_, pixel_format, &pixel_format_)) {
    log << util::kLogDateTime << ": "
        << "Can't set pixel format!\n";
    return false;
  }
  opengl_render_context_ = wglCreateContext(gdi_device_context_);

  if (!opengl_render_context_) {
    log << util::kLogDateTime << ": "
        << "Can't create opengl render context!\n";
    return false;
  }
  if (!wglMakeCurrent(gdi_device_context_, opengl_render_context_)) {
    log << util::kLogDateTime << ": "
        << "wglMakeCurrent failed!\n";
    return false;
  }
  // Load up extensions.
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    log << util::kLogDateTime << ": " << (char*)glewGetErrorString(err) << "\n";
    return false;
  }

  OnGLEWInit();
  return true;
}

void WGLContext::OnGLEWInit() const {
  glEnable(GL_DEPTH_TEST);

  char* gl_version = (char*)glGetString(GL_VERSION);
  char* gl_vendor = (char*)glGetString(GL_VENDOR);
  char* gl_renderer = (char*)glGetString(GL_RENDERER);

  log << util::kLogDateTime << ": "
      << "GL Context created\n" << util::kLogDateTime << ": "
      << "GL version: " << gl_version << "\n" << util::kLogDateTime << ": "
      << "GL vendor: " << gl_vendor << "\n" << util::kLogDateTime << ": "
      << "GL renderer: " << gl_renderer << "\n";
}

void WGLContext::Vsync(bool enable, bool adaptive) {
  if (enable) {
    if (adaptive)
      wglSwapIntervalEXT(-1);
    else
      wglSwapIntervalEXT(1);
  } else {
    wglSwapIntervalEXT(0);
  }
}

void WGLContext::Destroy() {
  if (opengl_render_context_) {
    wglMakeCurrent(0, 0);
    wglDeleteContext(opengl_render_context_);
    opengl_render_context_ = 0;
  }

  if (gdi_device_context_) {
    ReleaseDC(window_handle_, gdi_device_context_);
    gdi_device_context_ = 0;
  }
}

}  // namespace core
