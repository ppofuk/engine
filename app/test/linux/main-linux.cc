// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GL/glew.h"
#include "core/linux/window-xlib.h"
#include "core/linux/window-xlib-utility.h"
#include "core/reader-inl.h"
#include "core/time-ticker.h"
#include "test-gl-sprite-shader.h"
// #include "test-shader.h"

#include "cef/cef-render-handler.h"
#include <string>

#include <clocale>
#include <cstdlib>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

bool CheckResourceExistance() {
  // This is only for testing.
  util::HasLog has_log;
  if (!util::ByteReader::IsReadable("resources/actor.png")) {
    util::ByteReader::Chdir("../../");
    if (!util::ByteReader::IsReadable("resources/actor.png")) {
      has_log.log << util::kLogDateTime << ": resources/actor.png or "
                  << "../../resources.actor.png do not exist!\n";
      return false;
    }
  }
  return true;
}

class BrowserClient : public CefClient {
 public:
  BrowserClient(render::cef::RenderHandler* render_handler)
      : render_handler_(render_handler){};

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return render_handler_;
  }

 private:
  CefRefPtr<CefRenderHandler> render_handler_;
  IMPLEMENT_REFCOUNTING(BrowserClient);
};

void OpenGLDebug(GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam) {
  static const char* sources[] = {
      "API", "Window System", "Shader Compiler", "3rd party",
      "App", "Other",         "Uknown"};
  const char* source_human = sources[MIN(
      source - GL_DEBUG_SOURCE_API, sizeof(sources) / sizeof(const char*))];

  static const char* types[] = {"Error",       "Deprecated",  "Undefined",
                                "Portability", "Performance", "Other",
                                "Uknown"};
  const char* type_human = types[MIN(type - GL_DEBUG_SOURCE_API,
                                     sizeof(types) / sizeof(const char*))];

  static const char* severities[] = {"High", "Medium", "Low", "Uknown"};
  const char* severity_human =
      severities[MIN(severity - GL_DEBUG_SOURCE_API,
                     sizeof(severities) / sizeof(const char*))];
  util::Log << util::kLogDateTime
            << ": glDebugMessage [Source: " << source_human
            << ", Type: " << type_human << ", Severity: " << severity_human
            << "]: " << message << "\n";
}


int main(int argc, char* argv[]) {
  core::WindowXlib window;
  core::TimeTicker ticker;
  app::TestGLSpriteShader test_shader;
  render::cef::RenderHandler render_handler;

  core::Matrix4f perspective =
      core::Perspective::Calculate(45, 4.0f / 3, 0.1, 10000);

  core::Matrix4f ortho =
      core::Ortho::Calculate(0.0, 1920.0, -1680.0, 0.0, -100000, 100000);

  CefMainArgs cef_args(argc, argv);
  CefSettings cef_settings;

  // app::SimpleShaderTest simple_shader_test;

  char fps_string[16];

  if (!CheckResourceExistance()) {
    return 0;
  }

  // Init CEF
  // cef_settings.multi_threaded_message_loop = 0;
  // cef_settings.single_process = 1;
  // CefString(&cef_settings.resources_dir_path)
  //     .FromASCII("third-party/cef-linux/Resources/");
  // cef_settings.single_process = true;


  if (!CefInitialize(cef_args, cef_settings, nullptr, nullptr)) {
    util::HasLog log;
    log.log << util::kLogDateTime << ": Unable to intialize CEF! Quiting.\n";
    return 0;
  }

  if (!window.Init("test_shader")) {
    return 0;
  }

#ifndef NDEBUG
  util::Log << util::kLogDateTime
            << ": Enabling OpenGL debug message control.\n";
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                        GL_TRUE);
  glDebugMessageCallback((GLDEBUGPROC)OpenGLDebug, nullptr);
#endif  // NDEBUG

  window.Show();
  glViewport(0, 0, window.width(), window.height());

  test_shader.Init();
  render_handler.Init(&window);

  // Create browser-window
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browser_client;
  CefWindowInfo window_info;
  CefBrowserSettings browser_settings;

  std::string path = realpath("resources/main.html", nullptr);
  path = "file://" + path;

  window_info.SetAsWindowless(window.get_window(), true);
  browser_client = new BrowserClient(&render_handler);
  browser = CefBrowserHost::CreateBrowserSync(window_info, browser_client.get(),
                                              "http://www.google.com",
                                              browser_settings, nullptr);

  CefMouseEvent mouse_event;

  while (window.is_init()) {
    ticker.Update();
    core::WindowEventType event = window.CheckForEvents();

    if (event == core::kExpose) {
      glViewport(0, 0, window.width(), window.height());

      perspective = core::Perspective::Calculate(
          45, (f32)window.width() / window.height(), 0.1, 10000);

      ortho = core::Ortho::Calculate(0.0, window.width(), -window.height(), 0.0,
                                     -1, 1);

      browser.get()->GetHost()->WasResized();
      browser->GetHost()->SendFocusEvent(true);
    }
    if (event == core::kDestroyNotify && event == core::kWindowDelete) {
      CefShutdown();
      test_shader.Destroy();
      // simple_shader_test.Destroy();
      window.Destroy();
      return 0;
    }

    if (event == core::kMouseButtonPressed) {
      CefMouseEvent event;
      window.UpdateCursorPosition();
      event.x = window.cursor_x();
      event.y = window.cursor_y();
      browser->GetHost()->SendMouseClickEvent(event, MBT_LEFT, false, 1);
    }

    if (event == core::kMouseButtonReleased) {
      CefMouseEvent event;
      window.UpdateCursorPosition();
      event.x = window.cursor_x();
      event.y = window.cursor_y();
      browser->GetHost()->SendMouseClickEvent(event, MBT_LEFT, true, 1);
    }

    if (event == core::kMotionNotify) {
      window.UpdateCursorPosition();
      mouse_event.x = window.cursor_x();
      mouse_event.y = window.cursor_y();

      browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
    }

    if (event == core::kKeyPress) {
      CefKeyEvent key_event;
      const char* keycode_str = window.get_last_key_str();
      util::Log
          << util::kLogDateTime
          << ": kKeyPress (main-posix): "
          << keycode_str
          << "\n";

      if (window.get_last_keysym() == XK_space) {
        printf("space\n"); 
      }

      if (strlen(keycode_str) < 2) {
        wchar_t key_char[2];
        std::mbstowcs(key_char, keycode_str, 2);
        key_event.type = KEYEVENT_CHAR;
        key_event.character = key_char[0];

      } else {
        key_event.type = KEYEVENT_KEYDOWN;
      }

      key_event.windows_key_code =
          core::xlib::ConvertToKeyboardCode(window.get_last_keysym());
      browser->GetHost()->SendKeyEvent(key_event);
    }

    if (window.AsyncIsKeyPressed(XK_Escape)) {
      browser->GetHost()->CloseBrowser(false); 
      CefShutdown();
      test_shader.Destroy();
      window.Destroy();
      return 0;
    }

    // if (window.AsyncIsKeyPressed(VK_F8)) {
    //   window.Fullscreen(!window.is_fullscreen());
    // }

    // if (window.AsyncIsKeyPressed(XK_W)) {
    //   test_shader.get_actor()->position().Add(core::Vector4f(0, 1, 0, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_S)) {
    //   test_shader.get_actor()->position().Add(core::Vector4f(0, -1, 0, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_A)) {
    //   test_shader.get_actor()->position().Add(core::Vector4f(-1, 0, 0, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_D)) {
    //   test_shader.get_actor()->position().Add(core::Vector4f(1, 0, 0, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_Q)) {
    //   test_shader.get_background()->position().Add(core::Vector4f(0, 0, 10, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_E)) {
    //   test_shader.get_background()->position().Add(
    //       core::Vector4f(0, 0, -10, 0));
    // }

    // if (window.AsyncIsKeyPressed(XK_R)) {
    // }


    if (ticker.Tick(41666666)) {
      // 30hz tick
      test_shader.IncreaseFrame();
    }

    CefDoMessageLoopWork();

    render::GLSpriteShader* sprite_shader = test_shader.get_sprite_shader();

    window.Prerender();

    glClearColor(0.173, 0.251, 0.349, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sprite_shader->set_projection(perspective);

    test_shader.Render();
    // simple_shader_test.Render();

    glDisable(GL_DEPTH_TEST);
    sprite_shader->set_projection(ortho);
    sprite_shader->Draw(*render_handler.get_texture(), window.width() / 2,
                        window.height() / 2,
                        glm::vec3(window.width() / 2, -window.height() / 2, 0));
    glEnable(GL_DEPTH_TEST);

    // render_handler.shader()->Render();

    window.Postrender();

    // We haven't implemented fonts yet, so fps is drawn as window title.
    i64 watch_fps = (i64)1000000000 / ticker.passed_since_update();
    sprintf(fps_string, "%ld fps",
            (i64)1000000000 / ticker.passed_since_update());
    window.Title(fps_string);

    usleep(100);
  }

  return 0;
}
