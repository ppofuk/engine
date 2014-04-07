// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "win32/window.h"
#include "simple-shader.h"

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  core::Window window;
  app::SimpleShader simple_shader;


  window.set_instance(instance);
  window.Init();
  window.Destroy();
  window.set_instance(instance);

  if (window.Init()) {
    // This is only for testing.
    if (!util::Reader<char>::IsReadable("resources/actor.png")) {
      util::Reader<char>::Chdir("../../");
      if (!util::Reader<char>::IsReadable("resources/actor.png")) {
        window.log << util::kLogDateTime
                   << "resources/actor.png or "
                   << "../../resources.actor.png do not exist!\n";
        window.Destroy();
        return 0;
      }
    }

    simple_shader.Init();

    glViewport(0, 0, window.width(), window.height());
  }

  while (window.is_init()) {
    window.CheckForEvents();
    if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
      window.Destroy();
    }

    if (window.AsyncIsKeyPressed(VK_F8)) {
      window.Fullscreen();
    }

    window.Prerender();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    simple_shader.Render();
    window.Postrender();
    Sleep(10);
  }

  simple_shader.Destroy();
  window.Destroy();
  return 0;
}
