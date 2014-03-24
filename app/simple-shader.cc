// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "simple-shader.h"
namespace app {

SimpleShader::SimpleShader()
  : log_(Singleton<util::Logger>::Instance()),
    texture_uniform_(0),
    vertex_attribute_(0) {}

void SimpleShader::Init() {
  if (!texture_png_.Load("resources/actor.png")) {
    log_ << util::kLogDateTime << ": resources/actor.png can't be loaded!\n";
    return;
  }

  GLfloat vertex_data[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f
  };
  GLushort element_data[] = { 0, 1, 2, 3};

  vertex_buffer_.CreateVertexBuffer(vertex_data, 4*4);
  element_buffer_.CreateElementBuffer(element_data, 4);

  texture_.set_texture_abstract(&texture_png_);
  texture_.Generate();

  char vertex_shader[4098];
  char fragment_shader[4098];
  util::Reader<char> reader;

  reader.set_data(vertex_shader);
  reader.set_path("resources/simple-vertex.vs");
  reader.Open();
  if (!reader.Read()) {
    log_ << util::kLogDateTime << ": Reader I/O error!\n";
    return;
  }
  reader.Close();

  reader.set_data(fragment_shader);
  reader.set_path("resources/simple-fragment.vs");
  reader.Open();
  if (!reader.Read()) {
    log_ << util::kLogDateTime << ": Reader I/O error!\n";
    return;
  }
  reader.Close();

  vertex_shader_.CreateVertexShader(vertex_shader);
  if (!vertex_shader_.is_vaild()) {
    log_ << util::kLogDateTime << ": Vertex shader errors:\n"
         << vertex_shader_.InfoLog() << "\n";
    return;
  }

  fragment_shader_.CreateFragmentShader(fragment_shader);
  if (!fragment_shader_.is_vaild()) {
    log_ << util::kLogDateTime << ": Fragment shader errors: "
         << fragment_shader_.InfoLog() << "\n";
    return;
  }

  program_.Init();
  program_.AttachShader(vertex_shader_);
  program_.AttachShader(fragment_shader_);
  program_.Link();
  if (!program_.is_vaild()) {
    log_ << util::kLogDateTime << ": Program error:\n"
         << program_.InfoLog() << "\n";
    return;
  }

  static GLuint zero = 0;
  static GLfloat zerof;
  texture_uniform_ = new render::GLUniform<GLuint>(zero,
                                                   "texture",
                                                   program_.get_program());
  vertex_attribute_ = new render::GLAttribute<GLfloat>(zerof,
                                                       "position",
                                                       program_.get_program());

  init_ = true;
}

void SimpleShader::Render() {
  if (!init_)
    return;

  program_.Use();

  glActiveTexture(GL_TEXTURE0);
  texture_.Bind();
  texture_uniform_->Uniform1f();

  vertex_buffer_.Bind();
  vertex_attribute_->VertexAttribPointer(vertex_buffer_);
  vertex_attribute_->EnableVertexAttrib();

  element_buffer_.Bind();

  glDrawElements(GL_TRIANGLE_STRIP,
                 4,
                 GL_UNSIGNED_SHORT,
                 (void*)0);

  vertex_attribute_->DisableVertexAttrib();
}

void SimpleShader::Destroy() {
  program_.Delete();
  if (vertex_attribute_) {
    delete vertex_attribute_;
    vertex_attribute_ = 0;
  }
  if (texture_uniform_) {
    delete texture_uniform_;
    texture_uniform_ = 0;
  }
}

} // namespace app
