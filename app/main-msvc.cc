// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"
#include <windows.h>

#include "win32/window.h"
#include "reader-inl.h"

#include "simple-shader.h"

void TestReader() {
  util::Reader<char> reader;
  util::Logger& log = Singleton<util::Logger>::Instance();
  log << util::kLogDateTime << " " << util::Reader<char>::Cwd() << "\n";
  util::Reader<char>::Chdir("..");
  log << util::kLogDateTime << " " << util::Reader<char>::Cwd() << "\n";

  char data[2048];
  reader.set_data(data);
  reader.set_path("resources/simple-vertex.glsl");
  reader.Open();
  if (reader.Read()) {
    log << "Reader: " << reader.get_file_size() << "b\n" << data << "\n---\n";
  }
  reader.Close();
}

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  util::Logger& log = Singleton<util::Logger>::Instance();

  util::Window window;
  app::SimpleShader simple_shader;

  window.set_instance(instance);
  if (!window.Init()) {
    log << util::kLogDateTime << ": Win32 Window initialization failed\n";
  } else {
    util::Reader<char>::Chdir("..");

    glewInit();
    simple_shader.Init();

    RECT rect;
    GetClientRect(window.get_window_handle(), &rect);
    glViewport(0, 0, rect.right, rect.bottom);

    window.Fullscreen();
  }

  while (window.get_is_init()) {
    window.Prerender();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    simple_shader.Render();

    window.Postrender();

    Sleep(10);
    window.CheckForEvents();

    if (window.AsyncIsKeyPressed(VK_ESCAPE)) {
      window.Destroy();
    }
  }

  simple_shader.Destroy();
  window.Destroy();
  return 0;
}
