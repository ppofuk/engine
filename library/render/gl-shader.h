// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_RENDER_GL_SHADER_H_
#define OBSIDIAN_RENDER_GL_SHADER_H_

#include "static-compile-options.h"

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

  void set_view_frustum(core::Matrix4f view_frustum) {
    view_frustum_ = view_frustum;
  }

  inline void PassFrustumUniform() {
    view_frustum_uniform_.Pass(view_frustum_);
  }

  GLProgram& get_program() { return program_; }
  bool is_init() const { return is_init_; }

 protected:
  bool is_init_ = false;
  size_t shader_count_ = 0;
  GLuint shader_objects_[kGLShaderMaxNumberOfShaders];
  GLProgram program_;
  core::Matrix4f view_frustum_;
  GLUniform view_frustum_uniform_;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_GL_SHADER_H_
