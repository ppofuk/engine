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

  // For Debugging

  while (glGetError() != GL_NO_ERROR)
    ;

  glGenBuffers(1, &buffer_);
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": " << (const char*)gluErrorString(err) << "\n";
  }

  glBindBuffer(type, buffer_);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": " << (const char*)gluErrorString(err) << "\n";
  }

  // glNamedBufferDataEXT(buffer_, size_, data_, usage_);
  glBufferData(type, size_, data_, usage_);
  err = glGetError();
  if (err != GL_NO_ERROR) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": " << (const char*)gluErrorString(err) << "\n";
  }

  type_ = type;
}

void GLBufferBase::Create(GLenum type, GLenum usage, void* data, size_t size) {
  data_ = data;
  size_ = size;
  usage_ = usage;
  Create(type);
}

void GLBufferBase::Bind() const {
  glBindBuffer(type_, buffer_);
}

void* GLBufferBase::Map(GLenum access) const {
  Bind();

  void* ptr = glMapBuffer(type_, access);
  if (ptr == nullptr) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": " << (const char*)gluErrorString(glGetError()) << "\n";
  }

  return ptr;
}

bool GLBufferBase::Unmap() const {
  if (glUnmapBuffer(type_) == GL_TRUE)
    return true;
  else
    return false;
}

void GLBufferBase::UnbindAll() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace render
