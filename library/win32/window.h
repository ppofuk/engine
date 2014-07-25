#ifndef OBSIDIAN_DEFAULT_WINDOW_H_
#define OBSIDIAN_DEFAULT_WINDOW_H_

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

enum WindowEventType {
  kExpose,
  kDestroyNotify,
  kKeyPress,
  kWindowDelete,
  kMouseWheel,
  kNone
};

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

  // Show/hide etc.
  void Show(int show_param = SW_SHOWNORMAL);

  // Implementation of WindowInterface check for events.
  // It should be called in main loop.
  // It returns the current event type.
  WindowEventType CheckForEvents();

  // Set to fullscreen if |fullscreen| is true or to windowed if it's false
  void Fullscreen(bool fullscreen);

  // Events
  virtual void OnFocus();
  virtual void OnLostFocus();
  virtual void OnActivate();
  virtual void OnInactivate();
  virtual void OnResize();
  virtual void OnMouseWheel(int distance);

  short AsyncIsKeyPressed(int virtual_key);

  // Setters and geters.
  bool get_is_init() { return is_init_; }
  bool is_init() { return is_init_; }
  void set_instance(HINSTANCE instance) { instance_ = instance; }
  void set_window_name(const char window_name);
  HWND get_hwnd() { return window_handle_; }
  bool is_fullscreen() { return is_fullscreen_; }
  bool is_focused() { return is_focused_; }
  bool is_active() { return is_active_; }

  f32 width() {
    update_temp_rect();
    return temp_rect_.right;
  }

  f32 height() {
    update_temp_rect();
    return temp_rect_.bottom;
  }

  i32 mouse_wheel_distance() { return mouse_wheel_distance_; }

 private:
  void update_temp_rect() { GetClientRect(window_handle_, &temp_rect_); }

  RECT temp_rect_;

  bool is_init_;

  HWND window_handle_;
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

  int mouse_wheel_distance_;

  WindowEventType window_event_type_;

 protected:
  static LRESULT CALLBACK DefaultWin32Proc(HWND window_handle,
                                           UINT umsg,
                                           WPARAM wparam,
                                           LPARAM lparam);
  static WindowHandles window_handles_; 
};

}  //  namespace util

#endif
