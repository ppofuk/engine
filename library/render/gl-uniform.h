// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_UNIFORM_H_
#define OBSIDIAN_GL_UNIFORM_H_

#include "GL/glew.h"

#include "gl-program.h"
#include "../static-compile-options.h"

#include "../matrix-utility.h"

namespace render {

class GLUniform {
 public:
  GLUniform();
  // Locate a new or existing uniform variable of uniform_name in program.
  // This method should be always called.
  void Locate(const render::GLProgram& program, const char* uniform_name);

  GLint get_location() const { return location_; }
  void set_location(const GLint location) { location_ = location; }
  const char* get_uniform_name() const { return uniform_name_; }

  // Pass method is a wraper around glUniform... function.
  void Pass(GLfloat value);
  void Pass(GLint value);
  void Pass(GLuint value);
  void Pass(GLfloat* values, size_t count);
  void Pass(GLint* values, size_t count);
  void Pass(const core::Matrix4f& values, bool transpose = true);
  void Pass(const glm::mat4x4& values, bool transpose = true); 

 protected:
  char uniform_name_[kUniformNameSize];
  GLint location_;
  GLuint program_;
};

}  //  namespace render

#endif  // OBSIDIAN_GL_UNIFORM_H_
