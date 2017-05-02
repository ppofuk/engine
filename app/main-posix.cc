// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GL/glew.h"
#include "window-xlib.h"
#include "reader-inl.h"

#include "test-shader.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  core::WindowXlib window;
  app::SimpleShaderTest simple_shader_test;

  window.Init("app test");

  if (window.is_init()) {
    if (!util::Reader<char>::IsReadable("resources/actor.png")) {
      util::Reader<char>::Chdir("../../");
      if (!util::Reader<char>::IsReadable("resources/actor.png")) {
        window.log << util::kLogDateTime << "resources/actor.png or "
                   << "../../resources/actor.png don not exist!\n";
        window.Destroy();
        return 0;
      }
    }

    glewInit();

    simple_shader_test.ReadResources("resources/simple-vertex.vs",
                                     "resources/simple-fragment.vs",
                                     "resources/actor.png");
    simple_shader_test.InitBuffersAndTextures();
    simple_shader_test.InitShaders();
    simple_shader_test.InitProgram();
  }

  while (window.is_init()) {
    core::WindowEventType event_type = window.CheckForEvents();

    if (event_type == core::kDestroyNotify ||
        event_type == core::kWindowDelete || event_type == core::kKeyPress) {
      window.Destroy();
    } else {
      if (event_type == core::kExpose) {
        glViewport(0, 0, window.width(), window.height());
        simple_shader_test.set_aspect_ratio(window.width() / window.height());
      }

      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      simple_shader_test.Render();

      window.Postrender();
      usleep(1);
    }
  }

  simple_shader_test.Destroy();

  return 0;
}
