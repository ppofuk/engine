// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_BUFFER_H_
#define OBSIDIAN_GL_BUFFER_H_

#include "glad/glad.h"
#include "core/render/gl-types.h"
#include "../logger.h"
#include <string.h>

namespace render {

class GLBufferBase {
 public:
  GLBufferBase(GLenum usage, void* data, size_t size);
  GLBufferBase();

  GLenum get_type() const { return type_; }
  GLenum get_usage() const { return usage_; }
  void* get_data() const { return data_; }
  size_t get_size() const { return size_; }
  GLuint get_buffer() const { return buffer_; }

  void Create(GLenum type);
  void Create(GLenum type, GLenum usage, void* data, size_t size);

  // Destroy buffer GL object.
  void Destroy();

  // Bind the current buffer.
  void Bind() const;

  // Maps the buffer object into client's memory. Returns NULL on faliure.
  // |access| flag specifies what to do with the mapped data: read, write or
  // both. Use:
  //    GL_READ_ONLY
  //    GL_WRITE_ONLY
  //    GL_READ_WRITE
  void* Map(GLenum access) const;

  // After modifiying data in client memory with |Map|, the buffer must be
  // unmapped from client memory. If the method returns false, the contents of
  // the buffer bacame currupted and the data must be resubmited.
  bool Unmap() const;

  // Two buffer objects are logically the same if they have the same GL object
  // number.
  bool operator==(const GLBufferBase& rhs) {
    return (buffer_ == rhs.get_buffer());
  }

  inline void buffer_data() { glBufferData(type_, size_, data_, usage_); }
  
  static void UnbindAll();

 private:
  void* data_;
  size_t size_;
  GLenum type_;
  GLenum usage_;
  GLuint buffer_;
};

template <typename T>
class GLBuffer : public GLBufferBase {};

template <>
class GLBuffer<void> {
 public:
  // Destroy current buffer.
  void Destroy() { buffer_base_.Destroy(); }

  // Bind the current buffer.
  void Bind() { buffer_base_.Bind(); }

  // Create a new buffer of void* type. |size| is the memory size in bytes
  // of pointed array |data|.
  void Create(GLenum type, GLenum usage, void* data, size_t size) {
    buffer_base_.Create(type, usage, data, size);
  }

  const GLBufferBase& buffer_base() const { return buffer_base_; }

  template <typename U>
  bool Equal(const GLBuffer<U>& buffer) const {
    return (buffer_base_ == buffer.buffer_base());
  }

  inline void* Map(GLenum access) {
    buffer_base_.buffer_data(); 
    return buffer_base_.Map(access);
  }
  inline bool Unmap() const { return buffer_base_.Unmap(); }

  // Returns the actual size of pointed array in bytes.
  virtual size_t size() const { return buffer_base_.get_size(); }

  virtual inline void* get_offset(size_t offset) const {
    return (void*)(offset);
  }

 protected:
  GLBufferBase buffer_base_;
};

template <>
class GLBuffer<GLfloat> : public GLBuffer<void> {
 public:
  // Create a new buffer that holds GLfloat type.
  // |size| is the number of elements, not the memory size in bytes.
  void Create(GLenum type, GLenum usage, GLfloat* data, size_t size) {
    buffer_base_.Create(
        type, usage, static_cast<void*>(data), size * sizeof(GLfloat));
  }

  inline GLfloat* Map(GLenum access) const {
    return static_cast<GLfloat*>(buffer_base_.Map(access));
  }

  inline bool Unmap() const { return buffer_base_.Unmap(); }

  // Returns the number of elements in pointed array, not the size of array.
  // If you need to get the actual size you can use |buffer_base().get_size()|,
  // or multiply the returned value from this method by sizeof(GLfloat).
  size_t size() const { return buffer_base_.get_size() / sizeof(GLfloat); }

  inline void* get_offset(size_t offset) const {
    return (void*)(offset * sizeof(GLfloat));
  }
};

template <>
class GLBuffer<GLint> : public GLBuffer<void> {
 public:
  // Create a new buffer that holds GLint type.
  // |size| is the number of elements, not the memory size in bytes.
  void Create(GLenum type, GLenum usage, GLint* data, size_t size) {
    buffer_base_.Create(
        type, usage, static_cast<void*>(data), size * sizeof(GLint));
  }

  inline GLint* Map(GLenum access) const {
    return static_cast<GLint*>(buffer_base_.Map(access));
  }

  inline bool Unmap() const { return buffer_base_.Unmap(); }

  // Returns the number of elements in pointed array, not the size of array.
  // If you need to get the actual size you can use |buffer_base().get_size()|,
  // or multiply the returned value from this method by sizeof(GLint).
  size_t size() const { return buffer_base_.get_size() / sizeof(GLint); }

  inline void* get_offset(size_t offset) const {
    return (void*)(offset * sizeof(GLint));
  }
};

template <>
class GLBuffer<GLushort> : public GLBuffer<void> {
 public:
  // Create a new buffer that holds GLushort type.
  // |size| is the number of elements, not the memory size in bytes.
  void Create(GLenum type, GLenum usage, GLushort* data, size_t size) {
    buffer_base_.Create(
        type, usage, static_cast<void*>(data), size * sizeof(GLushort));
  }

  inline GLushort* Map(GLenum access) const {
    return static_cast<GLushort*>(buffer_base_.Map(access));
  }

  inline bool Unmap() const { return buffer_base_.Unmap(); }

  // Returns the number of elements in pointed array, not the size of array.
  // If you need to get the actual size you can use |buffer_base().get_size()|,
  // or multiply the returned value from this method by sizeof(GLushort).
  size_t size() const { return buffer_base_.get_size() / sizeof(GLushort); }

  inline void* get_offset(size_t offset) const {
    return (void*)(offset * sizeof(GLushort));
  }
};

// Helper functions that call GLBuffer<T>::Create(type, usage, data, size)
// method with type value correlated to function name (vertex buffer, element
// buffer)
template <typename T>
void CreateVertexBuffer(GLBuffer<T>* buffer,
                        GLenum usage,
                        T* data,
                        size_t size) {
  buffer->Create(GL_ARRAY_BUFFER, usage, data, size);
}

template <typename T>
void CreateElementBuffer(GLBuffer<T>* buffer,
                         GLenum usage,
                         T* data,
                         size_t size) {
  buffer->Create(GL_ELEMENT_ARRAY_BUFFER, usage, data, size);
}


}  //  namespace render

#endif  // OBSIDIAN_GL_BUFFER_H_
