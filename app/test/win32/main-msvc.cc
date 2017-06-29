// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "core/win32/window.h"
#include "core/win32/wgl-context.h"
#include "core/reader-inl.h"
#include "core/time-ticker.h"
#include "test-gl-sprite-shader.h"

#include "cef/cef-render-handler.h"
#include <string>
#include <clocale>
#include <cstdlib>
#include <sstream>

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
  std::stringstream s;
  s << "Fatal: Unhandled exception 0x"
    << std::hex
    << pExceptionInfo->ExceptionRecord->ExceptionCode 
    << std::endl;

  util::HasLog log;
  log.log << util::kLogDateTime << ": " << s.str().c_str() << "\n"; 
  exit(1);

  return EXCEPTION_CONTINUE_SEARCH;
}

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



INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  SetUnhandledExceptionFilter(TopLevelExceptionHandler);
  
  core::Window window(instance);
  core::WGLContext context;
  
  core::TimeTicker ticker;
  app::TestGLSpriteShader test_shader;
  render::cef::RenderHandler render_handler;

  core::Matrix4f perspective =
      core::Perspective::Calculate(45, 4.0f / 3, 0.1, 10000);

  core::Matrix4f ortho =
      core::Ortho::Calculate(0.0, 1920.0, -1680.0, 0.0, -100000, 100000);

  CefMainArgs cef_args(instance);
  CefSettings cef_settings;
  
  char fps_string[16];

  if (!CheckResourceExistance()) {
    return 0;
  }
  
  // Init CEF
  cef_settings.multi_threaded_message_loop = 0;
  cef_settings.single_process = 1;
  // CefString(&cef_settings.resources_dir_path)
  //     .FromASCII("third-party/cef-linux/Resources/");
  cef_settings.single_process = true;
  
  if (!CefInitialize(cef_args, cef_settings, nullptr, nullptr)) {
    util::HasLog log;
    log.log << util::kLogDateTime << ": Unable to intialize CEF! Quiting.\n";
    return 0;
  }
  
  if (!window.Init()) {
    return 0;
  }

  if (!context.Init(window)) {
    return 0;
  }

  window.Show();
  glViewport(0, 0, window.width(), window.height());

  test_shader.Init();
  render_handler.Init(&window);

  // Create browser-window
  CefRefPtr<CefBrowser> browser;
  CefRefPtr<BrowserClient> browser_client;
  CefWindowInfo window_info;
  CefBrowserSettings browser_settings;

  // std::string path = realpath("resources/main.html", nullptr);
  char path_buf[512];
  GetFullPathName("resources/main.html", 512, path_buf, nullptr); 
  std::string path = path_buf; 
  path = "file://" + path;

  window_info.SetAsWindowless(window.get_hwnd());
  browser_client = new BrowserClient(&render_handler);
  browser = CefBrowserHost::CreateBrowserSync(window_info, browser_client.get(),
                                              "http://www.google.com",
                                              browser_settings, nullptr);

  CefMouseEvent mouse_event;
  render::GLSpriteShader* sprite_shader = test_shader.get_sprite_shader();

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
    if (event == core::kDestroyNotify) {
      test_shader.Destroy();
      // simple_shader_test.Destroy();
      context.Destroy();
      window.Destroy();
      return 0;
    }

    if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
      browser->GetHost()->CloseBrowser(false);
      browser->Release();

      CefShutdown(); 
      test_shader.Destroy();
      context.Destroy();
      window.Destroy();      
    }

    if (window.AsyncIsKeyPressed(VK_F8)) {
      window.Fullscreen(!window.is_fullscreen());
    }

    if (window.AsyncIsKeyPressed(VK_W)) {
      test_shader.get_actor()->position().Add(core::Vector4f(0, 1, 0, 0));
    }

    if (window.AsyncIsKeyPressed(VK_S)) {
      test_shader.get_actor()->position().Add(core::Vector4f(0, -1, 0, 0));
    }

    if (window.AsyncIsKeyPressed(VK_A)) {
      test_shader.get_actor()->position().Add(core::Vector4f(-1, 0, 0, 0));
    }

    if (window.AsyncIsKeyPressed(VK_D)) {
      test_shader.get_actor()->position().Add(core::Vector4f(1, 0, 0, 0));
    }

    if (event == core::kMouseWheel) {
      if (window.mouse_wheel_distance() < 0) {
        test_shader.get_background()->position().Add(
            core::Vector4f(0, 0, 1000, 0));
      } else {
        test_shader.get_background()->position().Add(
            core::Vector4f(0, 0, -1000, 0));
      }
    }

    if (ticker.Tick(41666666)) {
      // 30hz tick
      test_shader.IncreaseFrame();
    }
    
    CefDoMessageLoopWork();
    
    window.Prerender();

    glClearColor(0.173, 0.251, 0.349, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    test_shader.Render();

    glDisable(GL_DEPTH_TEST);
    sprite_shader->set_projection(ortho);
    sprite_shader->Draw(*render_handler.get_texture(), window.width() / 2,
                        window.height() / 2,
                        glm::vec3(window.width() / 2, -window.height() / 2, 0));
    glEnable(GL_DEPTH_TEST);
    
    context.Postrender();

    // We haven't implemented fonts yet, so fps is drawn as window title.
    sprintf(
        fps_string, "%lld fps", (i64)1000000 / ticker.passed_since_update());
    window.Title(fps_string);

    Sleep(1);
  }

  return 0;
}
