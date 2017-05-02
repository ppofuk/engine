// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_APP_TEST_SHADER_H_
#define OBSIDIAN_APP_TEST_SHADER_H_

#include "render/gl-buffer.h"
#include "render/gl-program.h"
#include "render/gl-texture-2d.h"
#include "render/gl-attribute.h"
#include "render/gl-uniform.h"
#include "render/texture-loader.h"
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
  void Destroy();

  void set_aspect_ratio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
  void set_fov(int fov) { fov_ = fov; }
  int fov() const { return fov_; }

  GLfloat vertex_buffer_data[16];
  const render::GLBufferBase& vertex_buffer() const {
    return vertex_buffer_.buffer_base();
  }

 private:
  render::GLBuffer<GLfloat> vertex_buffer_;
  render::GLBuffer<GLushort> element_buffer_;
  render::GLTexture texture_;
  render::GLShaderObject vertex_shader_;
  render::GLShaderObject pixel_shader_;
  render::GLProgram program_;
  render::GLAttribute<render::GLBuffer<GLfloat> > position_attribute_;
  render::GLUniform texture_uniform_;
  render::GLUniform aspect_uniform_;
  render::GLUniform fov_uniform_;
  render::TextureLoader texture_loader_;
  char vertex_shader_source_[4096];
  char pixel_shader_source_[4096];
  bool all_init_;

  float aspect_ratio_;
  int fov_;
};

}  // namespace app

#endif  // OBSIDIAN_APP_TEST_SHADER_H_
