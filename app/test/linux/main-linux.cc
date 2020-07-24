// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "glad/glad.h"
#include "core/window-sdl.h"
#include "core/reader-inl.h"
#include "core/time-ticker.h"
#include "../gui.h"
#include "../simple-shape.h"

#include "core/render/dds-image.h"

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
  render::Camera camera;
  toolkit::Gui gui;
  app::SimpleShape simple_shape;
  app::SimpleShape simple_shape_b;
  glm::vec4 ambient_light =
      glm::vec4(0.98823529f, 0.83137254f, 0.95098039f, 0.04f);
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
  glEnable(GL_MULTISAMPLE);
  glViewport(0, 0, window.width(), window.height());

  if (!gui.Init(&window)) {
    window.Destroy();
    return 0;
  }

  int mouse_x = 0;
  int mouse_y = 0;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  camera.LookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  simple_shape.set_camera(&camera);
  simple_shape.set_aspect_ratio((f32)window.width() / window.height());
  simple_shape_b.set_camera(&camera);
  simple_shape_b.set_aspect_ratio((f32)window.width() / window.height());
  simple_shape_b.set_translate_preinit(glm::vec3(-3.0f, 1.0f, -3.0f));
  if (!simple_shape.Init()) {
    simple_shape.Destroy();
    gui.Shutdown();
    window.Destroy();
    return 0;
  }

  if (!simple_shape_b.Init()) {
    simple_shape_b.Destroy();
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
        simple_shape_b.set_aspect_ratio((f32)window.width() / window.height());
      }

      gui.ProcessEvent(event);

      if (event->type == SDL_QUIT) {
        gui.Shutdown();
        simple_shape.Destroy();
        simple_shape_b.Destroy();
        window.Destroy();
        return 0;
      }
    }

    const Uint8* key_states = SDL_GetKeyboardState(nullptr);
    if (key_states[SDL_SCANCODE_W]) {
      // camera.position_add(glm::vec3(0.0f, 0.0f, -0.20f));
      // camera.target_add(glm::vec3(0.0f, 0.0f, -0.20f));
      // camera.RecalculateViewMatrix();
      camera.Move(glm::vec3(0.0f, 0.0f, 0.20f));
    }

    if (key_states[SDL_SCANCODE_S]) {
      // camera.position_add(glm::vec3(0.0f, 0.0f, 0.20f));
      // camera.target_add(glm::vec3(0.0f, 0.0f, 0.20f));
      // camera.RecalculateViewMatrix();
      camera.Move(glm::vec3(0.0f, 0.0f, -0.20f));
    }

    if (key_states[SDL_SCANCODE_A]) {
      // camera.position_add(glm::vec3(-0.20f, 0.0f, 0.0f));
      // camera.target_add(glm::vec3(-0.20f, 0.0f, 0.0f));
      // camera.RecalculateViewMatrix();
      camera.Move(glm::vec3(0.20f, 0.0f, 0.0f));
    }

    if (key_states[SDL_SCANCODE_D]) {
      // camera.position_add(glm::vec3(0.20f, 0.0f, 0.0f));
      // camera.target_add(glm::vec3(0.20f, 0.0f, 0.0f));
      // camera.RecalculateViewMatrix();
      camera.Move(glm::vec3(-0.20f, 0.0f, 0.0f));
    }

    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      int mouse_dt_x = 0;
      int mouse_dt_y = 0;
      SDL_GetMouseState(&mouse_dt_x, &mouse_dt_y);
      mouse_dt_x -= mouse_x;
      mouse_dt_y -= mouse_y;

      camera.Move(glm::vec3((float)mouse_dt_x / 100.0f,
                            -(float)mouse_dt_y / 100.0f, 0.0f));
    }

    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
      int mouse_dt_x = 0;
      int mouse_dt_y = 0;
      SDL_GetMouseState(&mouse_dt_x, &mouse_dt_y);
      mouse_dt_x -= mouse_x;
      mouse_dt_y -= mouse_y;
      // camera.ApplyRotationAroundPosition(glm::vec3(
      //     -(float)mouse_dt_x / 10.0f, (float)mouse_dt_y / 10.0f, 0.0f));
      camera.Rotate(glm::vec3((float)mouse_dt_x / 10.0f,
                              (float)mouse_dt_y / 10.0f, 0.0f));
    }


    gui.NewFrame();
    {
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Window width: %d, height: %d", window.width(),
                  window.height());
    }

    simple_shape.Gui();
    simple_shape_b.Gui("Simple Shape B");

    ImGui::Begin("Camera");
    ImGui::Text("Eye: (%f, %f, %f)", -camera.view()[3].x, -camera.view()[3].y,
                -camera.view()[3].z);
    ImGui::End();

    ImGui::Begin("Light");
    ImGui::Text("Ambient Light:");
    if (ImGui::ColorEdit3("Ambient light color:",
                          glm::value_ptr(ambient_light))) {
      simple_shape.set_ambient_light(ambient_light);
      simple_shape_b.set_ambient_light(ambient_light);
    }

    if (ImGui::SliderFloat("Intensity", &ambient_light.w, 0, 1)) {
      simple_shape.set_ambient_light(ambient_light);
      simple_shape_b.set_ambient_light(ambient_light);
    }
    
    ImGui::End();

    if (camera.HasViewUpdated()) {
      simple_shape.ForceUpdateModelViewProjection();
      simple_shape_b.ForceUpdateModelViewProjection();
      simple_shape.set_camera_uniforms_changed_true();
      simple_shape_b.set_camera_uniforms_changed_true();
    }

    window.Prerender();

    // glClearColor(0.173, 0.251, 0.349, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    simple_shape.Render();
    simple_shape_b.Render();
    ImGui::Render();

    window.Postrender();

    SDL_GetMouseState(&mouse_x, &mouse_y);
    usleep(100);
  }

  return 0;
}
