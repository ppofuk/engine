#include "window.h"
using namespace util;

namespace core {

WindowHandles Window::window_handles_;

LRESULT CALLBACK Window::DefaultWin32Proc(HWND window_handle,
                                          UINT umsg,
                                          WPARAM wparam,
                                          LPARAM lparam) {
  core::Window* window = window_handles_.Get(window_handle);

  switch (umsg) {
    case WM_CLOSE:
      PostQuitMessage(0);
      return 0;

    case WM_SETFOCUS:
      if (window)
        window->OnFocus();
      break;

    case WM_KILLFOCUS:
      if (window)
        window->OnLostFocus();
      break;

    case WM_ACTIVATE:
      if (window) {
        if (LOWORD(wparam) == WA_ACTIVE)
          window->OnActivate();
        else
          window->OnInactivate();
      }
      break;

    case WM_SIZE:
      if (window)
        window->OnResize();
      break;

    case WM_MOUSEWHEEL:
      if (window)
        window->OnMouseWheel(wparam);

      break;
  }

  return DefWindowProc(window_handle, umsg, wparam, lparam);
}

Window::Window(void)
    : is_init_(false),
      window_handle_(0),
      instance_(0),
      is_fullscreen_(false),
      is_focused_(true),
      is_active_(true),
      window_event_type_(kNone) {
}

Window::Window(HINSTANCE hinstance)
    : is_init_(false),
      window_handle_(0),
      instance_(hinstance),
      is_fullscreen_(false),
      is_focused_(true),
      is_active_(true),
      window_event_type_(kNone) {
}


Window::~Window(void) {
}

bool Window::Init(const char* window_title, const char* class_name) {
  if (is_init_)
    return false;

  ZeroMemory(&window_class_, sizeof(WNDCLASSEX));
  window_class_.cbSize = sizeof(WNDCLASSEX);
  window_class_.style = CS_HREDRAW | CS_VREDRAW ;
  window_class_.lpfnWndProc = DefaultWin32Proc;
  window_class_.cbClsExtra = 0;
  window_class_.cbWndExtra = 0;
  window_class_.hInstance = instance_;
  window_class_.hIcon = LoadIcon(instance_, MAKEINTRESOURCE(IDI_APPLICATION));
  window_class_.hCursor = LoadCursor(NULL, IDC_ARROW);
  window_class_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  window_class_.lpszMenuName = NULL;
  window_class_.lpszClassName = class_name;
  window_class_.hIconSm =
      LoadIcon(window_class_.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

  if (!RegisterClassEx(&window_class_)) {
    log << util::kLogDateTime << ": RegisterClassEx in CreateWindowClass failed!\n";
    return false;
  }

  window_handle_ = CreateWindow(class_name,
                                window_title,
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                800,
                                600,
                                NULL,
                                NULL,
                                instance_,
                                NULL);
  if (!window_handle_) {
    log << util::kLogDateTime << ": CreateWindow failed!\n";
    return false;
  }

  saved_style_ = GetWindowLongPtr(window_handle_, GWL_STYLE);
  saved_ex_style_ = GetWindowLongPtr(window_handle_, GWL_EXSTYLE);
  GetClientRect(window_handle_, &saved_rect_);

  window_handles_.Add(window_handle_, this);
  is_init_ = true;
  return true;
}

void Window::Destroy() {
  // Change display setting to thoes stored in registery.
  ChangeDisplaySettings(NULL, 0);

  if (window_handle_) {
    window_handles_.Remove(window_handle_);
    DestroyWindow(window_handle_);
    window_handle_ = 0;
  }

  if (window_class_.hInstance) {
    UnregisterClass(window_class_.lpszClassName, window_class_.hInstance);
    ZeroMemory(&window_class_, sizeof(WNDCLASSEX));
  }

  is_init_ = false;
}

void Window::Prerender() {
}

void Window::Postrender() {
}

WindowEventType Window::CheckForEvents() {
  // First process windows vital events. This should be _realtime_
  if (PeekMessage(&message_, 0, 0, 0, PM_REMOVE)) {
    if (message_.message == WM_QUIT)
      window_event_type_ = kDestroyNotify;

    if (is_init_) {
      TranslateMessage(&message_);
      DispatchMessage(&message_);
    }
  }

  WindowEventType before_event_type = window_event_type_;
  window_event_type_ = kNone;
  return before_event_type;
}

void Window::Fullscreen(bool fullscreen) {
  if (!is_init_)
    return;

  if (!fullscreen) {
    // Change display setting to thoes stored in registery.
    ChangeDisplaySettings(NULL, 0);

    // Restore to saved style
    SetWindowLongPtr(window_handle_, GWL_STYLE, saved_style_);
    SetWindowLongPtr(window_handle_, GWL_EXSTYLE, saved_ex_style_);
    meta::Rectangle rect(saved_rect_);
    SetWindowPos(window_handle_,
                 NULL,
                 rect.x,
                 rect.y,
                 rect.width,
                 rect.height,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    is_fullscreen_ = false;

  } else {
    saved_style_ = GetWindowLongPtr(window_handle_, GWL_STYLE);
    saved_ex_style_ = GetWindowLongPtr(window_handle_, GWL_EXSTYLE);
    GetClientRect(get_hwnd(), &saved_rect_);

    // Send restore message before going into full screen mode
    SendMessage(window_handle_, WM_SYSCOMMAND, SC_RESTORE, 0);

    // Prepare window to fullscreen
    SetWindowLongPtr(window_handle_,
                     GWL_STYLE,
                     saved_style_ & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowLongPtr(
        window_handle_,
        GWL_EXSTYLE,
        saved_ex_style_ & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE |
                            WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    MONITORINFO monitor_info;
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(MonitorFromWindow(window_handle_, MONITOR_DEFAULTTONEAREST),
                   &monitor_info);

    meta::Rectangle rect(monitor_info.rcMonitor);
    SetWindowPos(window_handle_,
                 NULL,
                 rect.x,
                 rect.y,
                 rect.width,
                 rect.height,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    is_fullscreen_ = true;
  }
}

void Window::Show(int show_param) {
  if (is_init_) {
    ShowWindow(window_handle_, show_param);
    UpdateWindow(window_handle_);
  }
}

// Event handlings
void Window::OnActivate() {
  is_active_ = true;
  window_event_type_ = kExpose;
}

void Window::OnFocus() {
  is_focused_ = true;
  window_event_type_ = kExpose;
}

void Window::OnInactivate() {
  is_active_ = false;
}

void Window::OnLostFocus() {
  is_focused_ = false;
}

void Window::OnResize() {
  window_event_type_ = kExpose;
}

void Window::OnMouseWheel(int distance) {
  window_event_type_ = kMouseWheel;
  mouse_wheel_distance_ = distance;
}

short Window::AsyncIsKeyPressed(int virtual_key) {
  if (!is_focused_)
    return 0;

  short status = GetAsyncKeyState(virtual_key);
  if (status & 0x8000) {
    return 2;
  } else if (status) {
    return 1;
  } else
    return 0;
}

}  //  namespace util
