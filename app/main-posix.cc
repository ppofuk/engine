// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GL/glew.h"
#include "window-xlib.h"
#include "reader-inl.h"
#include "time-ticker.h"
#include "test-gl-sprite-shader.h"
#include "test-shader.h"

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

int main(int argc, char* argv[]) {
  core::WindowXlib window;
  core::TimeTicker ticker;
  app::TestGLSpriteShader test_shader;
  // app::SimpleShaderTest simple_shader_test;

  char fps_string[16];

  if (!CheckResourceExistance()) {
    return 0;
  }

  if (!window.Init("test_shader")) {
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
    if (event == core::kDestroyNotify && event == core::kWindowDelete) {
      test_shader.Destroy();
      // simple_shader_test.Destroy();
      window.Destroy();
      return 0;
    }

    if (window.AsyncIsKeyPressed(XK_Escape)) {
      test_shader.Destroy(); 
      window.Destroy();
      return 0;
    }

    // if (window.AsyncIsKeyPressed(VK_F8)) {
    //   window.Fullscreen(!window.is_fullscreen());
    // }
    
    if (window.AsyncIsKeyPressed(XK_W)) {
      test_shader.get_actor()->position().Add(core::Vector4f(0, 1, 0, 0));
    }

    if (window.AsyncIsKeyPressed(XK_S)) {
      test_shader.get_actor()->position().Add(core::Vector4f(0, -1, 0, 0));
    }

    if (window.AsyncIsKeyPressed(XK_A)) {
      test_shader.get_actor()->position().Add(core::Vector4f(-1, 0, 0, 0));
    }

    if (window.AsyncIsKeyPressed(XK_D)) {
      test_shader.get_actor()->position().Add(core::Vector4f(1, 0, 0, 0));
    }

    // if (event == core::kMouseWheel) {
    //   if (window.mouse_wheel_distance() < 0) {
    //     test_shader.get_background()->position().Add(
    //         core::Vector4f(0, 0, 1000, 0));
    //   } else {
    //     test_shader.get_background()->position().Add(
    //         core::Vector4f(0, 0, -1000, 0));
    //   }
    // }

    if (ticker.Tick(41666666)) {
      // 30hz tick
      test_shader.IncreaseFrame();
    }

    window.Prerender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    test_shader.Render();
    // simple_shader_test.Render();
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
