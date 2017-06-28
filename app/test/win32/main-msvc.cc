// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "core/win32/window.h"
#include "core/win32/wgl-context.h"
#include "core/reader-inl.h"
#include "core/time-ticker.h"
#include "test-gl-sprite-shader.h"

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
  
  core::TimeTicker ticker;
  app::TestGLSpriteShader test_shader;
  // app::SimpleShaderTest simple_shader_test;

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

  //
  test_shader.Init();
  // simple_shader_test.ReadResources("resources/simple-vertex.vs",
  //                                 "resources/simple-fragment.vs",
  //                                  "resources/actor.png");
  // simple_shader_test.InitBuffersAndTextures();
  // simple_shader_test.InitShaders();
  // simple_shader_test.InitProgram();
  //

  while (window.is_init()) {
    ticker.Update();
    core::WindowEventType event = window.CheckForEvents();

    if (event == core::kExpose) {
      glViewport(0, 0, window.width(), window.height());
    }
    if (event == core::kDestroyNotify) {
      test_shader.Destroy();
      // simple_shader_test.Destroy();
      context.Destroy();
      window.Destroy();
      return 0;
    }

    if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
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

    window.Prerender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    test_shader.Render();
    // simple_shader_test.Render();
    context.Postrender();

    // We haven't implemented fonts yet, so fps is drawn as window title.
    sprintf(
        fps_string, "%lld fps", (i64)1000000 / ticker.passed_since_update());
    window.Title(fps_string);

    Sleep(1);
  }

  return 0;
}
