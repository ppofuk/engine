// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "GL/glew.h"

#include "win32/window.h"
#include "win32/wgl-context.h"
#include "reader-inl.h"
#include "time-ticker.h"

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
  core::WGLContext wgl_context;
  core::TimeTicker ticker;

  if (!CheckResourceExistance()) {
    return 0;
  }

  if (!window.Init()) {
    return 0;
  }

  wgl_context.set_pixel_format_desc();
  if (!wgl_context.Init(window)) {
    return 0;
  }
  window.Show();

  glViewport(0, 0, window.width(), window.height());

  while (window.is_init()) {
    core::WindowEventType event = window.CheckForEvents();

    if (event == core::kExpose) {
      glViewport(0, 0, window.width(), window.height());
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
        }
      }

      if (ticker.Tick(41666666)) {
        // 30hz tick
      }

      window.Prerender();

      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      wgl_context.Postrender();
      Sleep(10);
    }
  }

  wgl_context.Destroy();
  window.Destroy();
  return 0;
}
