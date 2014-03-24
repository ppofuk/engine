// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GL/glew.h"
#include "window-xlib.h"
#include "simple-shader.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  core::WindowXlib window;
  app::SimpleShader simple_shader;

  window.Init("app test");

  if (window.is_init()) {
    util::Reader<char>::Chdir("../../");

    glewInit();
    simple_shader.Init();
  }

  while (window.is_init()) {
    core::WindowEventType event_type = window.CheckForEvents();

    if (event_type == core::kDestroyNotify ||
        event_type == core::kWindowDelete || event_type == core::kKeyPress) {
      window.Destroy();
    } else {
      if (event_type == core::kExpose)
        glViewport(window.x(), window.y(), window.width(), window.height());

      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      simple_shader.Render();

      window.Postrender();
      usleep(10);
    }
  }

  return 0;
}
