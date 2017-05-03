// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_WINDOW_XLIB_H_
#define OBSIDIAN_WINDOW_XLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "GL/glew.h"
#include <GL/glx.h>

#include "basic-types.h"
#include "logger.h"

namespace core {

enum WindowEventType {
  kExpose,
  kDestroyNotify,
  kKeyPress,
  kWindowDelete,
  kNone
};

class WindowXlib : public util::HasLog {
 public:
  WindowXlib();

  bool Init(const char* title,
            i32 x = 0,
            i32 y = 0,
            i32 width = 800,
            i32 height = 600);

  void Destroy();

  // Should be called after rendering is complete.
  void Postrender();

  // Should be called before rendering.
  void Prerender(); 

  // Returns a pending event from internal event queue.
  WindowEventType CheckForEvents();

  // Rereads the x and y mouse coordinates. You can access them with method
  // |cursor_x| and |cursor_y|.
  void UpdateCursorPosition();

  // Maps a window to screen. Note, that this is done in |Init|. 
  // show_param is ignored (it's just for compatibility with win32 version). 
  void Show(int show_param = 0);

  // Unmaps a window from screen. 
  void Hide();

  // Sets window title. In X it's called store name. 
  void Title(const char* title);

  // 
  short AsyncIsKeyPressed(KeySym virtual_key);

  bool is_init() const { return is_init_; }
  XEvent& get_x_event() { return x_event_; }
  XWindowAttributes& get_glx_window_attributes() {
    return glx_window_attributes_;
  }

  f32 x() { return glx_window_attributes_.x; }
  f32 y() { return glx_window_attributes_.y; }
  f32 width() { return glx_window_attributes_.width; }
  f32 height() { return glx_window_attributes_.height; }
  f32 cursor_x() { return win_x_; }
  f32 cursor_y() { return win_y_; }

 private:
  Display* display_;
  Window root_window_;
  GLint attributes_[5];
  XVisualInfo* visual_info_;
  Colormap color_map_;
  XSetWindowAttributes set_window_attributes_;
  Window window_;
  GLXContext glx_context_;
  XWindowAttributes glx_window_attributes_;
  XEvent x_event_;
  Atom window_delete_;
  bool is_init_;

  i32 root_x_, root_y_, win_x_, win_y_;
  unsigned int mask_;
  Window pointer_root_, pointer_child_;
};

}  // namespace core

#endif  // OBSIDIAN_WINDOW_XLIB_H_
