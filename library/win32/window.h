#ifndef OBSIDIAN_WINDOW_H_
#define OBSIDIAN_WINDOW_H_

// TODO: move from util namespace to core namespace.
// TODO: Window::CheckForEvents() should return WindowEventType.
//       See [../window.xlib.h]
// TODO: Add WindowEventType current_event_ to Window, so Window::OnResize,
//       Window::OnFocus... set current_event_.

#include "GL/glew.h"

#include <windows.h>
#include <stdio.h>

#include "rectangle.h"
#include "window-handles.h"
#include "../logger.h"
#include "../basic-types.h"

namespace core {

class Window : public util::HasLog {
 public:
  Window(void);
  ~Window(void);

  // Create a win32 window and a rendering context for OpenGL
  // Before calling Init(), instance_ should be set with set_instance function.
  // Private HINSTANCE instance_ is normaly a WinMain argumment.
  bool Init(const char* window_title = "Obsidian",
            const char* class_name = "ObsidianEngine");

  // Cleanup the mess made with win32 api.
  void Destroy();

  // Implementation of prerender.
  void Prerender();

  // Implementation of postrender.
  void Postrender();

  // Implementation of WindowInterface check for events.
  // It should be called in main loop.
  // Calls win32 api PeekMessage in depending on WM message type it
  // activates a coresponding base::Event.
  void CheckForEvents();

  // Set fullscreen mode
  void Fullscreen();

  // Events
  virtual void OnFocus();
  virtual void OnLostFocus();
  virtual void OnActivate();
  virtual void OnInactivate();
  virtual void OnResize();

  short AsyncIsKeyPressed(int virtual_key);

  // Setters and geters.
  bool get_is_init() { return is_init_; }
  bool is_init() { return is_init_; }
  void set_instance(HINSTANCE instance) { instance_ = instance; }
  void set_window_name(const char window_name);
  HWND get_window_handle() { return window_handle_; }
  bool IsFullscreen() { return is_fullscreen_; }
  bool IsFocused() { return is_focused_; }
  bool IsActive() { return is_active_; }

  f32 width() {
    update_temp_rect();
    return temp_rect_.right;
  }

  f32 height() {
    update_temp_rect();
    return temp_rect_.bottom;
  }

 private:
  void update_temp_rect() { GetClientRect(window_handle_, &temp_rect_); }

  RECT temp_rect_;

  bool is_init_;

  HWND window_handle_;
  HDC gdi_device_context_;
  HGLRC opengl_render_context_;
  WNDCLASSEX window_class_;
  HINSTANCE instance_;
  MSG message_;

  LONG saved_style_;
  LONG saved_ex_style_;
  RECT saved_rect_;
  BOOL saved_is_max_;

  bool is_fullscreen_;
  bool is_focused_;
  bool is_active_;
  char window_name_[16];
};

} //  namespace util

#endif
