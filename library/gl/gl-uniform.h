// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_UNIFORM_H_
#define OBSIDIAN_GL_UNIFORM_H_

#include "GL/glew.h"

#include "gl-program.h"
#include "../static-compile-options.h"

namespace render {

namespace intern {

class GLUniformBase {
 public:
  GLUniformBase();
  // Locate a new or existing uniform variable of uniform_name in program.
  // This method should be always called.
  void Locate(const render::GLProgram& program, const char* uniform_name);

  GLint get_location() const { return location_; }
  void set_location(const GLint location) { location_ = location; }
  const char* get_uniform_name() const { return uniform_name_; }

  virtual ~GLUniformBase() {}

  // Pass method is a wraper around glUniform... function.


 protected:
  char uniform_name_[kUniformNameSize];
  GLint location_;
  GLuint program_;
};

}  // namespace intern

template <typename T>
class GLUniform : public intern::GLUniformBase {
 public:
  typedef void Type;
  GLUniform() : intern::GLUniformBase() {}
};

template <>
class GLUniform<GLfloat> : public intern::GLUniformBase {
 public:
  typedef GLfloat Type;
  GLUniform() : intern::GLUniformBase() {}
  void Pass(GLfloat value) { glUniform1f(location_, value); }
};

template <>
class GLUniform<GLint> : public intern::GLUniformBase {
 public:
  typedef GLint Type;
  GLUniform() : intern::GLUniformBase() {}
  void Pass(GLint value) { glUniform1i(location_, value); }
};

template <>
class GLUniform<GLuint> : public intern::GLUniformBase {
 public:
  typedef GLint Type;
  GLUniform() : intern::GLUniformBase() {}
  void Pass(GLuint value) { glUniform1ui(location_, value); }
};

template <>
class GLUniform<GLfloat*> : public intern::GLUniformBase {
 public:
  typedef GLfloat* Type;
  GLUniform() : intern::GLUniformBase() {}
  void Pass(GLfloat* values, size_t count) {
    glUniform1fv(location_, count, values);
  }
};

template <>
class GLUniform<GLint*> : public intern::GLUniformBase {
 public:
  typedef GLint* Type;
  GLUniform() : intern::GLUniformBase() {}
  void Pass(GLint* values, size_t count) {
    glUniform1iv(location_, count, values);
  }
};

}  //  namespace render

#endif  // OBSIDIAN_GL_UNIFORM_H_
