// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-uniform.h"

#include <string.h>
#include <assert.h>

namespace render {

GLUniform::GLUniform() : location_(-1), program_(0) {
}

void GLUniform::Locate(const render::GLProgram& program,
                       const char* uniform_name) {
  program_ = program.get_program();
  assert(strlen(uniform_name) < kUniformNameSize - 1);
  memcpy(uniform_name_, uniform_name, kUniformNameSize);
  location_ = glGetUniformLocation(program_, uniform_name_);
#ifdef _DEBUG
  if (location_ == -1) {
    util::Log << util::kLogDateTime << ": Warning! " << __FILE__ << ": "
              << __LINE__ << " location_ is -1 for " << uniform_name << " \n";
  }
#endif  // _DEBUG
}

void GLUniform::Pass(GLfloat value) {
  glUniform1f(location_, value);
}

void GLUniform::Pass(GLint value) {
  glUniform1i(location_, value);
}

void GLUniform::Pass(GLuint value) {
  glUniform1ui(location_, value);
}

void GLUniform::Pass(GLfloat* values, size_t count) {
  glUniform1fv(location_, count, values);
}

void GLUniform::Pass(GLint* values, size_t count) {
  glUniform1iv(location_, count, values);
}

}  // namespace render
