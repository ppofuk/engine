// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "window-xlib.h"

namespace core {

WindowXlib::WindowXlib() : is_init_(false) {
  attributes_[0] = GLX_RGBA;
  attributes_[1] = GLX_DEPTH_SIZE;
  attributes_[2] = 24;
  attributes_[3] = GLX_DOUBLEBUFFER;
  attributes_[4] = None;
}

bool WindowXlib::Init(const char* title, i32 x, i32 y, i32 width, i32 height) {
  display_ = XOpenDisplay(NULL);

  if (!display_) {
    return false;
  }

  root_window_ = XDefaultRootWindow(display_);
  visual_info_ = glXChooseVisual(display_, 0, attributes_);

  if (!visual_info_) {
    XCloseDisplay(display_);
    return false;
  }

  color_map_ =
      XCreateColormap(display_, root_window_, visual_info_->visual, AllocNone);

  set_window_attributes_.colormap = color_map_;
  set_window_attributes_.event_mask = ExposureMask | KeyPressMask |
                                      PointerMotionMask | ButtonReleaseMask |
                                      ButtonPressMask;

  window_ =
      XCreateWindow(display_, root_window_, x, y, width, height, 0,
                    visual_info_->depth, InputOutput, visual_info_->visual,
                    CWColormap | CWEventMask, &set_window_attributes_);
  XMapWindow(display_, window_);
  XStoreName(display_, window_, title);

  // Add X event for window destroy
  window_delete_ = XInternAtom(display_, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(display_, window_, &window_delete_, 1);

  glx_context_ = glXCreateContext(display_, visual_info_, NULL, GL_TRUE);
  glXMakeCurrent(display_, window_, glx_context_);

  is_init_ = true;

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    log << util::kLogDateTime << ": " << (char*)glewGetErrorString(err) << "\n";
    Destroy();
    return false;
  }

  glEnable(GL_DEPTH_TEST);  // TODO(ppofuk): Move?

  char* gl_version = (char*)glGetString(GL_VERSION);
  char* gl_vendor = (char*)glGetString(GL_VENDOR);
  char* gl_renderer = (char*)glGetString(GL_RENDERER);
  log << util::kLogDateTime << ": "
      << "GL Context created\n"
      << util::kLogDateTime << ": "
      << "GL version: " << gl_version << "\n"
      << util::kLogDateTime << ": "
      << "GL vendor: " << gl_vendor << "\n"
      << util::kLogDateTime << ": "
      << "GL renderer: " << gl_renderer << "\n";

  return true;
}

void WindowXlib::Destroy() {
  if (is_init_) {
    glXMakeCurrent(display_, None, NULL);
    glXDestroyContext(display_, glx_context_);
    XDestroyWindow(display_, window_);
    XCloseDisplay(display_);
    is_init_ = false;
  }
}

void WindowXlib::Show(int show_param) {
  XMapWindow(display_, window_);
}

void WindowXlib::Hide() {
  XUnmapWindow(display_, window_);
}

void WindowXlib::Title(const char* title) {
  XStoreName(display_, window_, title);
}

WindowEventType WindowXlib::CheckForEvents() {
  if (XCheckTypedEvent(display_, Expose, &x_event_)) {
    XGetWindowAttributes(display_, window_, &glx_window_attributes_);
    return kExpose;
  } else if (XCheckTypedEvent(display_, KeyPress, &x_event_)) {
    return kKeyPress;
  } else if (XCheckTypedEvent(display_, DestroyNotify, &x_event_)) {
    return kDestroyNotify;
  } else if (XCheckTypedEvent(display_, ClientMessage, &x_event_)) {
    return kWindowDelete;
  } else if (XCheckTypedEvent(display_, ButtonPress, &x_event_)) {
    // Mouse button was pressed
    return kMouseButtonPressed;
    
  } else if (XCheckTypedEvent(display_, ButtonRelease, &x_event_)) {
    // Mouse button was released
    return kMouseButtonReleased;
  }
  return kNone;
}

void WindowXlib::Prerender() {}

void WindowXlib::Postrender() {
  glXSwapBuffers(display_, window_);
}

void WindowXlib::UpdateCursorPosition() {
  XQueryPointer(display_, window_, &pointer_root_, &pointer_child_, &root_x_,
                &root_y_, &win_x_, &win_y_, &mask_);
}

short WindowXlib::AsyncIsKeyPressed(KeySym virtual_key) {
  char key_status[32];
  int keycode = XKeysymToKeycode(display_, virtual_key);

  XQueryKeymap(display_, key_status);
  return key_status[keycode / 8] & (1 << (keycode % 8));
}

}  // namespace core
