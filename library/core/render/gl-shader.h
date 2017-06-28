// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_RENDER_GL_SHADER_H_
#define OBSIDIAN_RENDER_GL_SHADER_H_

#include "../static-compile-options.h"

#include "gl-program.h"
#include "gl-texture-2d.h"
#include "gl-uniform.h"
#include "../matrix-utility.h"

namespace render {

class GLShader {
 public:
  enum ShaderType { kFragmentShader, kVertexShader };

  void Init();
  void Destroy();

  void AddShader(const char* path, ShaderType shader_type);

  void Compile();

  virtual void OnInit() {}
  virtual void OnDestroy() {}
  virtual void PostCompile() {}

  void set_projection(core::Matrix4f projection) {
    projection_ = projection;
  }

  void set_view(core::Matrix4f view) {
    view_ = view;
  }

  inline void PassProjectionViewUnifroms() {
    projection_uniform_.Pass(projection_);
    view_uniform_.Pass(view_);

  }

  GLProgram& get_program() { return program_; }
  bool is_init() const { return is_init_; }

 protected:
  bool is_init_ = false;
  size_t shader_count_ = 0;
  GLuint shader_objects_[kGLShaderMaxNumberOfShaders];
  GLProgram program_;
  core::Matrix4f projection_;
  GLUniform projection_uniform_;
  core::Matrix4f view_;
  GLUniform view_uniform_;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_GL_SHADER_H_
