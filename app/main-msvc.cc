// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "win32/window.h"
#include "win32/wgl-context.h"
#include "reader-inl.h"
#include "time-ticker.h"

#include "test-shader.h"

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

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  core::Window window(instance);
  core::WGLContext context;
  core::TimeTicker ticker, ticker_on_30_fps;

  app::SimpleShaderTest simple_shader_test;

  char fps_string[16];

  if (!CheckResourceExistance()) {
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

  simple_shader_test.ReadResources("resources/simple-vertex.vs",
                                   "resources/simple-fragment.vs",
                                   "resources/actor.png");
  simple_shader_test.InitBuffersAndTextures();
  simple_shader_test.InitShaders();
  simple_shader_test.InitProgram();

  simple_shader_test.set_fov(-75);
  simple_shader_test.set_aspect_ratio(window.width() / window.height());

  while (window.is_init()) {
    ticker.Update();
    core::WindowEventType event = window.CheckForEvents();

    if (event == core::kExpose) {
      glViewport(0, 0, window.width(), window.height());
    }
    if (event == core::kDestroyNotify) {
      context.Destroy();
      window.Destroy();
      simple_shader_test.Destroy();
      return 0;
    }

    if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
      window.Destroy();
    }

    if (window.AsyncIsKeyPressed(VK_F8)) {
      window.Fullscreen(!window.is_fullscreen());
    }

    if (event == core::kMouseWheel) {
      if (window.mouse_wheel_distance() < 0) {
        if (simple_shader_test.fov() + 1 < 180)
          simple_shader_test.set_fov(simple_shader_test.fov() + 1);
      } else {
        if (simple_shader_test.fov() - 1 > -180)
          simple_shader_test.set_fov(simple_shader_test.fov() - 1);
      }
    }

    if (ticker_on_30_fps.Tick(41666666)) {
      // 30hz tick
      for (size_t i = 0; i < 4; ++i) {
        float& x = simple_shader_test.vertex_buffer_data[4*i];
        float& y = simple_shader_test.vertex_buffer_data[4*i + 1];
        float& z = simple_shader_test.vertex_buffer_data[4*i + 2];
        float& w = simple_shader_test.vertex_buffer_data[4*i + 3];

        // Translate by x', y', z'; aka x_, y_, z_
        float x_ = 0.01f, y_ = 0.01f, z_ = 0.0f;
        // w = w + (x * x_) + (y * y_) + (z * z_);
        x += x_;
        y += y_;
      }

      simple_shader_test.vertex_buffer().Bind();
      glBufferData(simple_shader_test.vertex_buffer().get_type(),
                   sizeof(GLfloat) * 16,
                   simple_shader_test.vertex_buffer_data,
                   GL_STATIC_DRAW);

      // glBindBuffer(type, buffer_);
      // glBufferData(type, size_, data_, usage_);
    }

    window.Prerender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    simple_shader_test.Render();

    context.Postrender();

    Sleep(0);

    // We haven't implemented fonts yet, so fps is drawn as window title.
    sprintf(
        fps_string, "%lld fps", (i64)1000000 / ticker.passed_since_update());
    window.Title(fps_string);
  }

  return 0;
}
