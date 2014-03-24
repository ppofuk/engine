// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_ATTRIBUTE_H_
#define OBSIDIAN_GL_ATTRIBUTE_H_

#include "GL/glew.h"
#include "gl-buffer.h"

namespace render {

// TODO: description
// NOTE: when using attribute for an vertex array use typename of vertices,
// e.g for GLBuffer<GLfloat>, use GLAttribute<GLfloat>.
template <typename Type>
class GLAttribute {
 public:
  GLAttribute(Type& variable, const char* name, GLuint program)
    : variable_(variable), name_(name), program_(program) {
    location_ = glGetAttribLocation(program_, name);
  }

  Type& variable() { return variable_; }
  void value(Type value) { variable_ = value; }
  const char* name() { return name; }
  GLuint program() const { return program_; }
  GLint location() { return location_; }

  // TODO: offset not working for now
  void VertexAttribPointer(GLBuffer<Type>& buffer,
                           size_t pair_size = 4,
                           GLenum type = GL_FLOAT,
                           GLenum normalized = GL_FALSE,
                           size_t offset = 0) {
    glVertexAttribPointer(location_,
                          pair_size,
                          type,
                          normalized,
                          (sizeof(Type) * buffer.get_size()) / pair_size,
                          (void*)(offset * sizeof(Type)));
  }

  void EnableVertexAttrib() {
    glEnableVertexAttribArray(location_);
  }

  void DisableVertexAttrib() {
    glDisableVertexAttribArray(location_);
  }

 private:
  GLint location_;
  Type& variable_;
  const char* name_;
  GLuint program_;
};

} //  namespace render

#endif // OBSIDIAN_GL_ATTRIBUTE_H_
