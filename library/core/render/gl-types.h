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

namespace intern {
static GLenum gl_texture_enums[] = {GL_TEXTURE0,
                                    GL_TEXTURE1,
                                    GL_TEXTURE2,
                                    GL_TEXTURE3,
                                    GL_TEXTURE4,
                                    GL_TEXTURE5,
                                    GL_TEXTURE6,
                                    GL_TEXTURE7,
                                    GL_TEXTURE8,
                                    GL_TEXTURE9,
                                    GL_TEXTURE10,
                                    GL_TEXTURE11,
                                    GL_TEXTURE12,
                                    GL_TEXTURE13,
                                    GL_TEXTURE14,
                                    GL_TEXTURE15,
                                    GL_TEXTURE16,
                                    GL_TEXTURE17,
                                    GL_TEXTURE18,
                                    GL_TEXTURE19,
                                    GL_TEXTURE20,
                                    GL_TEXTURE21,
                                    GL_TEXTURE22,
                                    GL_TEXTURE23,
                                    GL_TEXTURE24,
                                    GL_TEXTURE25,
                                    GL_TEXTURE26,
                                    GL_TEXTURE27,
                                    GL_TEXTURE28,
                                    GL_TEXTURE29,
                                    GL_TEXTURE30,
                                    GL_TEXTURE31};
}  // namespace intern

static inline GLenum GLActiveTextureIndex(size_t index) {
  if (index < 32)
    return intern::gl_texture_enums[index];
  return GL_TEXTURE0;
}

}  // namespace render
#endif  // OBSIDIAN_GL_TYPES_H_
