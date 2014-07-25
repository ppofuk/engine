// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "win32/window.h"
#include "win32/wgl-context.h"
#include "test-shader.h"
#include "reader-inl.h"
#include "time-ticker.h"

bool CheckResourceExistance() {
  // This is only for testing.
  util::HasLog has_log;
  if (!util::Reader<char>::IsReadable("resources/actor.png")) {
    util::Reader<char>::Chdir("../../");
    if (!util::Reader<char>::IsReadable("resources/actor.png")) {
      has_log.log << util::kLogDateTime << ": resources/actor.png or "
                  << "../../resources.actor.png do not exist!\n";
      return false;
    }
  }
  return true;
}

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  core::Window window;
  core::WGLContext wgl_context;
  core::TimeTicker ticker;
  app::SimpleShaderTest simple_shader;

  if (!CheckResourceExistance()) {
    return 0; 
  }

  window.set_instance(instance);
  if (!window.Init()) {
    return 0;
  }

  wgl_context.set_pixel_format_desc();
  if (!wgl_context.Init(window.get_hwnd())) {
    return 0;
  }
  window.Show();

  // Init test shader here
  simple_shader.ReadResources("resources/base-vertex.vs",
                              "resources/simple-vertex.vs",
                              "resources/simple-fragment.vs",
                              "resources/actor.png");
  simple_shader.InitBuffersAndTextures();
  simple_shader.InitShaders();
  simple_shader.InitProgram();

  glViewport(0, 0, window.width(), window.height());
  simple_shader.set_aspect_ratio(window.width() / window.height());
  simple_shader.set_fov(45);

  while (window.is_init()) {
    core::WindowEventType event = window.CheckForEvents();

    if (event == core::kExpose) {
      glViewport(0, 0, window.width(), window.height());
      simple_shader.set_aspect_ratio(window.width() / window.height());
    } else if (event == core::kDestroyNotify) {
      window.Destroy();
    } else {
      if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
        window.Destroy();
      }

      if (window.AsyncIsKeyPressed(VK_F8)) {
        window.Fullscreen(!window.is_fullscreen());
      }

      if (event == core::kMouseWheel) {
        if (window.mouse_wheel_distance() < 0) {
          if (simple_shader.fov() + 1 < 180)
            simple_shader.set_fov(simple_shader.fov() + 1);
        } else {
          if (simple_shader.fov() - 1 > -180)
            simple_shader.set_fov(simple_shader.fov() - 1);
        }
      }

      if (ticker.Tick(41666666)) {
        // 30hz tick
      }

      window.Prerender();

      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      simple_shader.Render();
      wgl_context.Postrender();
      Sleep(10);
    }
  }

  simple_shader.Destroy();
  wgl_context.Destroy();
  window.Destroy();
  return 0;
}
