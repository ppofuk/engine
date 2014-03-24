// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_BUFFER_H_
#define OBSIDIAN_GL_BUFFER_H_

#include "GL/glew.h"
#include <string.h>

namespace render {

template <typename Type>
class GLBuffer {
 public:
  GLBuffer() : data_(0), buffer_(0) {};

  void set_data(Type* data) { data_ = data; }
  Type* get_data() const { return data_; }
  void set_size(size_t size) { size_ = size; }
  size_t get_size() const { return size_; }
  GLuint get_buffer() const { return buffer_; }
  GLenum get_type() const { return type_; }

  void Create(GLenum type) {
    glGenBuffers(1, &buffer_);
    glBindBuffer(type, buffer_);
    glBufferData(type,
                 size_ * sizeof(Type),
                 data_,
                 GL_STATIC_DRAW);
    type_ = type;
  }

  void CreateVertexBuffer(Type* data, size_t size) {
    set_data(data);
    set_size(size);
    Create(GL_ARRAY_BUFFER);
  }

  void CreateElementBuffer(Type* data, size_t size) {
    set_data(data);
    set_size(size);
    Create(GL_ELEMENT_ARRAY_BUFFER);
  }

  void Bind() {
    glBindBuffer(type_, buffer_);
  }

 private:
  Type* data_;
  size_t size_;
  GLuint buffer_;
  GLenum type_;
};
} //  namespace render

#endif
