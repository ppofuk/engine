// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "render/gl-buffer.h"
#include "render/gl-program.h"
#include "render/gl-texture-2d.h"
#include "render/gl-attribute.h"
#include "render/gl-uniform.h"
#include "render/texture-loader.h"
#include "vector4.h"

#include <vector>

namespace app {

class AppShader {
 public:
  AppShader() : is_init_(false) {}

  void Init() { program_.Init(); }

  void Destroy() { program_.Delete(); }

  template <typename... Args>
  void LoadShaders(const char* path, Args... paths) {
    if (!is_init_) {
      Init();
    }

    static char shader_source[8192];
    static util::ByteReader reader;
    reader.set_data(shader_source);
    reader.set_path(path);
    reader.Open();
    if (reader.Read()) {
      render::GLShaderObject shader;
      size_t len = strlen(path);

      if (!strcmp(".vert", &path[len - 6])) {
        shader.CreateVertexShader(path);
      } else
        shader.CreateFragmentShader(path);

      if (shader.is_vaild()) {
        program_.AttachShader(shader);
        shaders_.push_back(shader.get_shader());
      } else {
        util::Log << util::kLogDateTime << ": " << __FILE__ << ": " << __LINE__
                  << ": Shader compilation error: "
                  << "\n" << shader.InfoLog() << "\n";
      }
    }

    reader.Close();
    LoadShaders(paths...);
  }

 protected:
  void LoadShaders() {
    program_.Link();
    if (!program_.is_vaild()) {
      util::Log << util::kLogDateTime << ": " << __FILE__ << ": " << __LINE__
                << ": Program link error: "
                << "\n" << program_.InfoLog() << "\n";
    } else {
      for (; !shaders_.empty();) {
        render::GLShaderObject::DeleteShader(shaders_.back());
        shaders_.pop_back();
      }
    }
  }

  bool is_init_;
  std::vector<GLuint> shaders_;
  render::GLProgram program_;
};

void TestAppShader() {
  AppShader app_shader;
  app_shader.LoadShaders("1.vert", "2.vert", "3.vert");

}

}  // namespace app
