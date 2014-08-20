// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_TYPES_H_
#define OBSIDIAN_GL_TYPES_H_

#include "GL/glew.h"

namespace render {

// Get GL type enumeration.
// Example: GLTypeEnum<GLFloat>::type will yield GL_FLOAT
template <typename T>
struct GLTypeEnum {};

template <>
struct GLTypeEnum<GLfloat> {
  enum {
    type = GL_FLOAT
  };
};

template <>
struct GLTypeEnum<GLint> {
  enum {
    type = GL_INT
  };
};

template <>
struct GLTypeEnum<GLuint> {
  enum {
    type = GL_UNSIGNED_INT
  };
};

template <>
struct GLTypeEnum<GLbyte> {
  enum {
    type = GL_BYTE
  };
};

// Get enumeration for bool false or true.
// Example: GLBoolValue<true>::value
template <bool Value>
struct GLBoolValue {};

template <>
struct GLBoolValue<true> {
  enum {
    value = GL_TRUE
  };
};

template <>
struct GLBoolValue<false> {
  enum {
    value = GL_FALSE
  };
};

}  // namespace render
#endif  // OBSIDIAN_GL_TYPES_H_
