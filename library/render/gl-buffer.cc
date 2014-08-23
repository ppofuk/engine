// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-buffer.h"
namespace render {

GLBufferBase::GLBufferBase(GLenum usage, void* data, size_t size)
    : data_(data), size_(size), usage_(usage), buffer_(0) {}

GLBufferBase::GLBufferBase() : data_(NULL), size_(0), buffer_(0) {}

void GLBufferBase::Destroy() {
  if (buffer_) {
    glDeleteBuffers(1, &buffer_);
    buffer_ = 0;
  }
}

void GLBufferBase::Create(GLenum type) {
  Destroy();
  glGenBuffers(1, &buffer_);
  glBindBuffer(type, buffer_);
  glBufferData(type, size_, data_, usage_);
  type_ = type;
}

void GLBufferBase::Create(GLenum type, GLenum usage, void* data, size_t size) {
  data_ = data;
  size_ = size;
  usage_ = usage;
  Create(type);
}

void GLBufferBase::Bind() const { glBindBuffer(type_, buffer_); }

}  // namespace render
