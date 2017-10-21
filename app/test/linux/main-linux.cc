// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GL/glew.h"
#include "core/window-sdl.h"
#include "core/reader-inl.h"
#include "core/time-ticker.h"
#include "../gui.h"
#include "../simple-shape.h"

#include <string>

#include <clocale>
#include <cstdlib>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void OpenGLDebug(GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam) {
  static const char* sources[] = {
      "API", "Window System", "Shader Compiler", "3rd party",
      "App", "Other",         "Uknown"};
  const char* source_human = sources[MIN(
      source - GL_DEBUG_SOURCE_API, sizeof(sources) / sizeof(const char*))];

  static const char* types[] = {"Error",       "Deprecated",  "Undefined",
                                "Portability", "Performance", "Other",
                                "Uknown"};
  const char* type_human = types[MIN(type - GL_DEBUG_SOURCE_API,
                                     sizeof(types) / sizeof(const char*))];

  static const char* severities[] = {"High", "Medium", "Low", "Uknown"};
  const char* severity_human =
      severities[MIN(severity - GL_DEBUG_SOURCE_API,
                     sizeof(severities) / sizeof(const char*))];
  util::Log << util::kLogDateTime
            << ": glDebugMessage [Source: " << source_human
            << ", Type: " << type_human << ", Severity: " << severity_human
            << "]: " << message << "\n";
}


int main(int argc, char* argv[]) {
  core::WindowSDL window;
  core::TimeTicker ticker;
  toolkit::Gui gui;
  app::SimpleShape simple_shape;


  char fps_string[16];
  fps_string[0] = '\0';

  if (!window.Init("test_shader")) {
    return 0;
  }

#ifndef NDEBUG
  util::Log << util::kLogDateTime
            << ": Enabling OpenGL debug message control.\n";
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                        GL_TRUE);
  glDebugMessageCallback((GLDEBUGPROC)OpenGLDebug, nullptr);
#endif  // NDEBUG

  window.Show();
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, window.width(), window.height());

  if (!gui.Init(&window)) {
    window.Destroy();
    return 0;
  }

  simple_shape.set_aspect_ratio((f32)window.width() / window.height());
  if (!simple_shape.Init()) {
    simple_shape.Destroy();
    gui.Shutdown();
    window.Destroy();
    return 0;
  }

  while (window.is_init()) {
    ticker.Update();
    while (window.CheckForEvents()) {
      SDL_Event* event = window.event();
      if (event->type == SDL_WINDOWEVENT &&
          event->window.event == SDL_WINDOWEVENT_EXPOSED) {
        glViewport(0, 0, window.width(), window.height());
        simple_shape.set_aspect_ratio((f32)window.width() / window.height());
      }

      gui.ProcessEvent(event);

      if (event->type == SDL_QUIT) {
        gui.Shutdown();
        window.Destroy();
        return 0;
      }
    }

    gui.NewFrame();
    {
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Window width: %d, height: %d", window.width(),
                  window.height());
    }
    simple_shape.Gui();

    window.Prerender();

    glClearColor(0.173, 0.251, 0.349, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    simple_shape.Render();
    ImGui::Render();

    window.Postrender();
    usleep(100);
  }

  return 0;
}
