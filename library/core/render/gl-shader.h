// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_RENDER_GL_SHADER_H_
#define OBSIDIAN_RENDER_GL_SHADER_H_

#include "../static-compile-options.h"

#include "gl-program.h"
#include "gl-uniform.h"
#include <glm/glm.hpp>

namespace render {

class GLShader {
 public:
  enum ShaderType { kFragmentShader, kVertexShader };

  // Initializes shader program for linkage and compilation. Also calls virtual
  // method |OnInit|.
  void Init();

  // Destroys all added shaders and destroys the shader program.
  void Destroy();

  // Reads shader from file specified with |path|, compiles it and attaches it
  // to program (contained in this class).
  // |shader_type| is either kFragmentShader or kVertexShader.
  // Returns true on success.
  bool AddShader(const char* path, ShaderType shader_type);

  // TODO(ppofuk): Rename this to link, because AddShader is shader compilation.
  //               This is actually a link process.
  // Links all compiled shaders to a program.
  void Compile();

  // Overloadable method that is called on initialization, just after |Init|
  // method.
  virtual void OnInit() {}

  // Overloadable method that is called on |Destroy|. This method is called
  // before program or any added shaders are deleted.
  virtual void OnDestroy() {}

  // TODO(ppofuk): Rename this to PostLink. See TODO for |Compile|.
  // Overloadble method that is called after linking all shaders in one program.
  virtual void PostCompile() {}

  // Reallocate model uniform to different location.
  // Location is found and set via |GLUniform.Locate| method.
  void ReallocateModelUnifrom(const char* location);

  // Reallocate model uniform to different location.
  // Location is set via |GLUniform.set_location| method. It's a simple setter
  // in GLUniform.
  void ReallocateModelUnifrom(GLint location);

  // Reallocate view uniform to different location.
  // Location is found and set via |GLUniform.Locate| method.
  void ReallocateViewUnifrom(const char* location);

  // Reallocate view uniform to different location.
  // Location is set via |GLUniform.set_location| method. It's a simple setter
  // in GLUniform.
  void ReallocateViewUnifrom(GLint location);

  // Reallocate projection uniform to different location.
  // Location is found and set via |GLUniform.Locate| method.
  void ReallocateProjectionUnifrom(const char* location);

  // Reallocate projection uniform to different location.
  // Location is set via |GLUniform.set_location| method. It's a simple setter
  // in GLUniform.
  void ReallocateProjectionUnifrom(GLint location);

  inline void model_uniform_pass(const glm::mat4& model) {
    model_uniform_.Pass(model);
  }

  inline void view_uniform_pass(const glm::mat4& view) {
    view_uniform_.Pass(view);
  }

  inline void projection_uniform_pass(const glm::mat4& projection) {
    projection_uniform_.Pass(projection);
  }

  size_t shader_count() const { return shader_count_; }

  GLProgram* get_program_unsafe() { return &program_; }
  const GLProgram& get_program() const { return program_; }
  bool is_init() const { return is_init_; }

 protected:
  bool is_init_ = false;
  size_t shader_count_ = 0;
  GLuint shader_objects_[kGLShaderMaxNumberOfShaders];
  GLProgram program_;

  GLUniform model_uniform_;
  GLUniform view_uniform_;
  GLUniform projection_uniform_;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_GL_SHADER_H_
