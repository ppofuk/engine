// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "test-shader.h"
#include "reader-inl.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace app {

SimpleShaderTest::SimpleShaderTest() : all_init_(false) {
}

bool SimpleShaderTest::ReadResources(const char* vertex_shader_path,
                                     const char* pixel_shader_path,
                                     const char* texture_png_path) {
  util::ByteReader reader;

  reader.set_data(vertex_shader_source_);
  reader.set_path(vertex_shader_path);
  reader.Open();
  if (!reader.Read()) {
    log << util::kLogDateTime << "Unable to read " << vertex_shader_path
        << "\n";
    return false;
  }
  reader.Close();

  reader.set_data(pixel_shader_source_);
  reader.set_path(pixel_shader_path);
  reader.Open();
  if (!reader.Read()) {
    log << util::kLogDateTime << "Unable to read " << pixel_shader_path << "\n";
    return false;
  }
  reader.Close();

  if (!texture_loader_.Load(texture_png_path)) {
    log << util::kLogDateTime << "Unable to read " << texture_png_path << "\n";
    return false;
  }

  return true;
}

void SimpleShaderTest::InitBuffersAndTextures() {
  static GLfloat vertex_buffer_data_[] = {-1.0f,
                                          -1.0f,
                                          0.0f,
                                          1.0f,
                                          1.0f,
                                          -1.0f,
                                          0.0f,
                                          1.0f,
                                          -1.0f,
                                          1.0f,
                                          0.0f,
                                          1.0f,
                                          1.0f,
                                          1.0f,
                                          0.0f,
                                          1.0f};

  memcpy(vertex_buffer_data, vertex_buffer_data_, sizeof(vertex_buffer_data_));

  static GLushort element_buffer_data[] = {0, 1, 2, 3};

  render::CreateVertexBuffer(
      &vertex_buffer_, GL_STATIC_DRAW, vertex_buffer_data, 16);

  render::CreateElementBuffer(
      &element_buffer_, GL_STATIC_DRAW, element_buffer_data, 4);

  texture_.set_texture_loader(&texture_loader_);
  // Some texture experiments
  texture_.set_gl_mag_filter(GL_NEAREST);
  texture_.set_gl_min_filter(GL_NEAREST);

  texture_.Generate();
}

void SimpleShaderTest::InitShaders() {
  vertex_shader_.CreateVertexShader(vertex_shader_source_);
  if (!vertex_shader_.is_vaild()) {
    log << util::kLogDateTime << vertex_shader_.InfoLog() << "\n";
    return;
  }

  pixel_shader_.CreateFragmentShader(pixel_shader_source_);
  if (!pixel_shader_.is_vaild()) {
    log << util::kLogDateTime << pixel_shader_.InfoLog() << "\n";
    return;
  }
}

void SimpleShaderTest::InitProgram() {
  program_.Init();
  program_.AttachShader(vertex_shader_);
  program_.AttachShader(pixel_shader_);
  program_.Link();
  if (!program_.is_vaild()) {
    log << util::kLogDateTime << program_.InfoLog() << "\n";
    return;
  }

  position_attribute_.Locate(program_, "coord");
  texture_uniform_.Locate(program_, "texture");
  view_frustum_uniform_.Locate(program_, "view_frustum");

  log << util::kLogDateTime << " Program linked successfuly!\n";

  set_view_frustum(core::Ortho::Calculate(0.0, 1920.0, 1680.0, 0.0, -1.0, 1.0));

  glActiveTexture(GL_TEXTURE0);
  texture_.Bind();
}

void SimpleShaderTest::Render() {
  program_.Use();
  render::GLTexture::EnableBlending();

  view_frustum_uniform_.Pass(view_frustum_);

  texture_uniform_.Pass(static_cast<GLint>(0));

  vertex_buffer_.Bind();

  position_attribute_.PassVertexPointer(vertex_buffer_, 4);

  position_attribute_.Enable();

  element_buffer_.Bind();

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

  position_attribute_.Disable();
}

void SimpleShaderTest::Destroy() {
  texture_loader_.Destroy();
}

}  // namespace app
