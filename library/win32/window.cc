#include "window.h"
using namespace util;

namespace core {

LRESULT CALLBACK
DefaultWin32Proc(HWND window_handle, UINT umsg, WPARAM wparam, LPARAM lparam) {
  core::Window* window =
      Singleton<WindowHandles>::Instance().Get(window_handle);

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
  }

  return DefWindowProc(window_handle, umsg, wparam, lparam);
}

Window::Window(void)
    : is_init_(false),
      window_handle_(0),
      gdi_device_context_(0),
      opengl_render_context_(0),
      instance_(0),
      is_fullscreen_(false),
      is_focused_(true),
      is_active_(true) {}

Window::~Window(void) {}

bool Window::Init(const char* window_title, const char* class_name) {

  if (instance_ && !is_init_) {
    ZeroMemory(&window_class_, sizeof(WNDCLASSEX));

    window_class_.cbSize = sizeof(WNDCLASSEX);
    window_class_.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class_.lpfnWndProc = (WNDPROC)DefaultWin32Proc;
    window_class_.hInstance = instance_;
    window_class_.hbrBackground = (HBRUSH)COLOR_WINDOW;

    // Set cursor to default arrow.
    window_class_.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class_.lpszClassName = class_name;

    if (RegisterClassEx(&window_class_)) {
      window_handle_ = CreateWindowEx(
          WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
          class_name,
          window_title,
          WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
          0,
          0,
          800,
          600,
          NULL,
          NULL,
          instance_,
          NULL);

      if (window_handle_) {
        static PIXELFORMATDESCRIPTOR pixel_format_desc = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,  // Color Depth
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
            24,  // Depth Buffer
            8, // Stencil buffer
            0,
            PFD_MAIN_PLANE,
            0,
            0,
            0,
            0};

        gdi_device_context_ = GetDC(window_handle_);
        if (gdi_device_context_) {
          unsigned int pixel_format =
              ChoosePixelFormat(gdi_device_context_, &pixel_format_desc);

          if (SetPixelFormat(
                  gdi_device_context_, pixel_format, &pixel_format_desc)) {

            opengl_render_context_ = wglCreateContext(gdi_device_context_);

            if (opengl_render_context_)
              if (wglMakeCurrent(gdi_device_context_, opengl_render_context_)) {
                // Load up extensions.
                GLenum err = glewInit();
                if (err != GLEW_OK) {
                  log << util::kLogDateTime << ": "
                      << (char*) glewGetErrorString(err) << "\n";
                  Destroy();
                  return false;
                }

                ShowWindow(window_handle_, SW_SHOW);
                SetForegroundWindow(window_handle_);
                SetFocus(window_handle_);

                glEnable(GL_DEPTH_TEST);

                is_init_ = true;

                // Store style infromations for fullscreen switching
                saved_style_ = GetWindowLongPtr(window_handle_, GWL_STYLE);
                saved_ex_style_ = GetWindowLongPtr(window_handle_, GWL_EXSTYLE);
                GetClientRect(get_window_handle(), &saved_rect_);

                // For optimization wise, we'll register handle only if we
                // have GL context. So we can presume, some functions called
                // are able to execute.
                Singleton<WindowHandles>::Instance().Add(window_handle_, this);

                // Log the success with version
                char* gl_version = (char*)glGetString(GL_VERSION);
                char* gl_vendor = (char*)glGetString(GL_VENDOR);
                char* gl_renderer = (char*)glGetString(GL_RENDERER);

                log << kLogDateTime << ": "
                    << "GL Context created\n" << kLogDateTime << ": "
                    << "GL version: " << gl_version << "\n" << kLogDateTime
                    << ": "
                    << "GL vendor: " << gl_vendor << "\n" << kLogDateTime
                    << ": "
                    << "GL renderer: " << gl_renderer << "\n";

              } else
                log << kLogDateTime << ": Failed wglMakeCurrent\n";
            else
              log << kLogDateTime << ": Failed wglCreateContext\n";
          } else
            log << kLogDateTime << ": Failed SetPixelFormat\n";
        } else
          log << kLogDateTime << ": Failed GetDC\n";
      } else
        log << kLogDateTime << ": Failed CreateWindowEx\n";
    } else
      log << kLogDateTime << ": Failed RegisterClassEx\n";
  }

  return is_init_;
}

void Window::Destroy() {
  // Change display setting to thoes stored in registery.
  ChangeDisplaySettings(NULL, 0);

  if (opengl_render_context_) {
    wglMakeCurrent(0, 0);
    wglDeleteContext(opengl_render_context_);
    opengl_render_context_ = 0;
  }

  if (gdi_device_context_) {
    ReleaseDC(window_handle_, gdi_device_context_);
    gdi_device_context_ = 0;
  }

  if (window_handle_) {
    Singleton<WindowHandles>::Instance().Remove(window_handle_);
    DestroyWindow(window_handle_);
    window_handle_ = 0;
  }

  if (window_class_.hInstance) {
    UnregisterClass(window_class_.lpszClassName, window_class_.hInstance);
    ZeroMemory(&window_class_, sizeof(WNDCLASSEX));
  }

  is_init_ = false;
}

void Window::Prerender() {}

void Window::Postrender() { SwapBuffers(gdi_device_context_); }

void Window::CheckForEvents() {
  // First process windows vital events. This should be _realtime_
  if (PeekMessage(&message_, 0, 0, 0, PM_REMOVE)) {
    if (message_.message == WM_QUIT)
      Destroy();

    if (is_init_) {
      TranslateMessage(&message_);
      DispatchMessage(&message_);
    }
  }
}

void Window::Fullscreen() {
  if (!is_init_)
    return;

  if (is_fullscreen_) {
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
    GetClientRect(get_window_handle(), &saved_rect_);

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

// Event handlings
void Window::OnActivate() { is_active_ = true; }

void Window::OnFocus() { is_focused_ = true; }

void Window::OnInactivate() { is_active_ = false; }

void Window::OnLostFocus() { is_focused_ = false; }

void Window::OnResize() {
  RECT rc;
  GetClientRect(get_window_handle(), &rc);
  meta::Rectangle rect(rc);

  // We assume the hardcodness of opengl now :D
  glViewport(0, 0, rect.width, rect.height);
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
