// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_APP_SIMPLE_SHADER_H_
#define OBSIDIAN_APP_SIMPLE_SHADER_H_

#include <windows.h>
#include "win32/window.h"
#include "texture-png.h"
#include "gl/gl-texture.h"
#include "gl/gl-buffer.h"
#include "gl/gl-program.h"
#include "logger.h"
#include "singleton.h"
#include "reader-inl.h"

namespace app {

// This is a simple shader test class.
class SimpleShader {
 public:
  SimpleShader();

  void Init();
  void Render();
  void Destroy();

  bool is_init() const { return init_; }
 private:
  util::Logger& log_;
  resource::TexturePng texture_png_;
  render::GLTexture texture_;
  render::GLShader vertex_shader_;
  render::GLShader fragment_shader_;
  render::GLBuffer<GLfloat> vertex_buffer_;
  render::GLBuffer<GLushort> element_buffer_;
  render::GLProgram program_;

  render::GLUniform<GLuint> *texture_uniform_;
  render::GLAttribute<GLfloat> *vertex_attribute_;
  bool init_;
};

} // namespace app

#endif // OBSIDIAN_APP_SIMPLE_SHADER_H_
