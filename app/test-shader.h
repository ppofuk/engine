// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_APP_TEST_SHADER_H_
#define OBSIDIAN_APP_TEST_SHADER_H_

#include "gl/gl-buffer.h"
#include "gl/gl-program.h"
#include "gl/gl-texture.h"
#include "gl/gl-attribute.h"
#include "gl/gl-uniform.h"
#include "texture-png.h"
#include "logger.h"

namespace app {

class SimpleShaderTest : util::HasLog {
 public:
  SimpleShaderTest();
  bool ReadResources(const char* vertex_shader_path,
                     const char* pixel_shader_path,
                     const char* texture_png_path);

  void InitShaders();
  void InitBuffersAndTextures();
  void InitProgram();
  void Render();

 private:
  render::GLBuffer<GLfloat> vertex_buffer_;
  render::GLBuffer<GLushort> element_buffer_;
  render::GLTexture texture_;
  render::GLShader vertex_shader_;
  render::GLShader pixel_shader_;
  render::GLProgram program_;
  render::GLAttribute<render::GLBuffer<GLfloat> > position_attribute_;
  render::GLUniform texture_uniform_;
  resource::TexturePng texture_loader_;
  char vertex_shader_source_[4096];
  char pixel_shader_source_[4096];
  bool all_init_;
};

}  // namespace app

#endif  // OBSIDIAN_APP_TEST_SHADER_H_
