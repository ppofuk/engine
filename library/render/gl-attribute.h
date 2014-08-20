// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_ATTRIBUTE_H_
#define OBSIDIAN_GL_ATTRIBUTE_H_

#include "GL/glew.h"

#include "gl-program.h"
#include "gl-buffer.h"
#include "gl-types.h"

#include "../static-compile-options.h"

namespace render {

namespace intern {

class GLAttributeBase {
 public:
  GLAttributeBase();

  // Locate a new attribute variable location of attrib_name in program.
  // This method should be always called.
  void Locate(const render::GLProgram& program, const char* attrib_name);

  GLint get_location() const { return location_; }
  void set_location(const GLint location) { location_ = location; }
  const char* get_attrib_name() const { return attrib_name_; }

 protected:
  char attrib_name_[kAttribNameSize];
  GLint location_;
  GLuint program_;
};

}  // namespace intern

template <typename T>
class GLAttribute : public intern::GLAttributeBase {
 public:
  typedef void Type;
  GLAttribute() : intern::GLAttributeBase() {}
};

template <typename T>
class GLAttribute<GLBuffer<T> > : public intern::GLAttributeBase {
 public:
  typedef GLBuffer<T> Type;
  GLAttribute() : intern::GLAttributeBase() {}

  // Creates an vertex attribute pointer to currently bound buffer.
  // Before calling |PassVertexPointer| on |buffer| you should call
  // |buffer.Bind()|
  void PassVertexPointer(const GLBuffer<T>& buffer,
                         size_t pair_size,
                         size_t offset = 0,
                         GLenum normalized = GL_FALSE) {
    // TODO: check if buffer is OK to pass.
    glVertexAttribPointer(location_,
                          pair_size,
                          GLTypeEnum<T>::type,
                          normalized,
                          buffer.buffer_base().get_size() / pair_size,
                          buffer.get_offset(offset));
  }

  void Enable() { glEnableVertexAttribArray(location_); }

  void Disable() { glDisableVertexAttribArray(location_); }
};

}  //  namespace render

#endif  // OBSIDIAN_GL_ATTRIBUTE_H_
