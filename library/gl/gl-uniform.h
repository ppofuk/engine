// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_UNIFORM_H_
#define OBSIDIAN_GL_UNIFORM_H_

#include "GL/glew.h"
namespace render {

template <typename Type>
class GLUniform {
 public:
  GLUniform(Type& variable, const char* name, GLuint program)
      : variable_(variable), name_(name), program_(program) {
    location_ = glGetUniformLocation(program_, name);
  }

  Type& variable() { return variable_; }
  void value(Type value) { variable_ = value; }
  const char* name() { return name; }
  GLuint program() const { return program_; }
  GLint location() { return location_; }

  void Uniform1f() { glUniform1f(location_, variable_); }
  void Uniform1i() { glUniform1i(location_, variable_); }

 private:
  Type& variable_;
  GLint location_;
  const char* name_;
  GLuint program_;
};

}  //  namespace render

#endif  // OBSIDIAN_GL_UNIFORM_H_
