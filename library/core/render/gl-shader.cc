// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-shader.h"

namespace render {

void GLShader::Init() {
  program_.Init();
  OnInit();

  is_init_ = true;
}

void GLShader::Destroy() {
  OnDestroy();

  for (; shader_count_; shader_count_--) {
    render::GLShaderObject::DeleteShader(shader_objects_[shader_count_ - 1]);
  }

  program_.Delete();
  is_init_ = false;
}

bool GLShader::AddShader(const char* path, ShaderType shader_type) {
  bool success = true;
  static char shader_source[kGLShaderShaderSourceSize];
  static util::ByteReader reader;
  reader.set_data(shader_source);
  reader.set_path(path);
  reader.Open();
  if (reader.Read()) {
    render::GLShaderObject shader;

    if (shader_type == kVertexShader) {
      shader.CreateVertexShader(shader_source);
    } else if (shader_type == kFragmentShader)
      shader.CreateFragmentShader(shader_source);

    if (shader.is_vaild()) {
      program_.AttachShader(shader);

      shader_objects_[shader_count_++] = shader.get_shader();
    } else {
      util::Log << util::kLogDateTime << ": " << __FILE__ << ": " << __LINE__
                << ": Shader compilation error in " << path << ": "
                << "\n"
                << shader.InfoLog() << "\n";
      success = false;
    }
  } else {
    success = false;
  }

  reader.Close();
  return success;
}

void GLShader::Compile() {
  program_.Link();
  if (!program_.is_vaild()) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ": " << __LINE__
              << ": Program link error: "
              << "\n"
              << program_.InfoLog() << "\n";
  }
  projection_uniform_.Locate(program_, "projection");
  view_uniform_.Locate(program_, "view");
  model_uniform_.Locate(program_, "model");

  PostCompile();
}

void GLShader::ReallocateModelUnifrom(const char* location) {
  model_uniform_.Locate(program_, location);
}

void GLShader::ReallocateModelUnifrom(GLint location) {
  model_uniform_.set_location(location);
}

void GLShader::ReallocateViewUnifrom(const char* location) {
  view_uniform_.Locate(program_, location);
}

void GLShader::ReallocateViewUnifrom(GLint location) {
  view_uniform_.set_location(location);
}

void GLShader::ReallocateProjectionUnifrom(const char* location) {
  projection_uniform_.Locate(program_, location);
}

void GLShader::ReallocateProjectionUnifrom(GLint location) {
  projection_uniform_.set_location(location);
}

}  // namespace render
