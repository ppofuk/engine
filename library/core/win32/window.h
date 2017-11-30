#ifndef OBSIDIAN_DEFAULT_WINDOW_H_
#define OBSIDIAN_DEFAULT_WINDOW_H_

// TODO: move from util namespace to core namespace.
// TODO: Window::CheckForEvents() should return WindowEventType.
//       See [../window.xlib.h]
// TODO: Add WindowEventType current_event_ to Window, so Window::OnResize,
//       Window::OnFocus... set current_event_.

#include "glad/glad.h"

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

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

class Window : public util::HasLog {
 public:
  Window(void);
  Window(HINSTANCE hinstance);
  ~Window(void);

  // Create a win32 window and a rendering context for OpenGL
  // Before calling Init(), instance_ should be set with set_instance function.
  // Private HINSTANCE instance_ is normally a WinMain argument.
  // Alternatively you can pass HINSTNCE in the constructor.
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

  // Set title
  void Title(const char* title);

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
  bool get_is_init() const { return is_init_; }
  bool is_init() const { return is_init_; }
  void set_instance(HINSTANCE instance) { instance_ = instance; }
  void set_window_name(const char window_name);
  HWND get_hwnd() const { return window_handle_; }
  bool is_fullscreen() const { return is_fullscreen_; }
  bool is_focused() const { return is_focused_; }
  bool is_active() const { return is_active_; }

  f32 width() {
    update_temp_rect();
    return temp_rect_.right;
  }

  f32 height() {
    update_temp_rect();
    return temp_rect_.bottom;
  }

  f32 width() const { return temp_rect_.right; }
  f32 height() const { return temp_rect_.bottom; }

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
