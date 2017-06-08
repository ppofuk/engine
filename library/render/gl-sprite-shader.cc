// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-sprite-shader.h"

namespace render {

void GLSpriteShader::OnInit() {
  CreateVertexBuffer(
      &positions_buffer_, GL_STATIC_DRAW, intern::sprite_positions, 16);
  CreateElementBuffer(
      &indices_buffer_, GL_STATIC_DRAW, intern::sprite_indices, 4);
  CreateVertexBuffer(
      &texcoords_buffer_, GL_DYNAMIC_DRAW, intern::sprite_texcoords, 8);

  AddShader(gl_sprite_shader_vert_source, GLShader::kVertexShader);
  AddShader(gl_sprite_shader_frag_source, GLShader::kFragmentShader);
}

void GLSpriteShader::OnDestroy() {
}

void GLSpriteShader::PostCompile() {
  positions_attrib_.Locate(program_, "coords");
  texcoords_attrib_.Locate(program_, "texcoords");
  x_uniform_.Locate(program_, "x");
  y_uniform_.Locate(program_, "y");
  z_uniform_.Locate(program_, "z");
  rx_uniform_.Locate(program_, "rx");
  ry_uniform_.Locate(program_, "ry");
  rz_uniform_.Locate(program_, "rz");
  width_uniform_.Locate(program_, "width");
  height_uniform_.Locate(program_, "height");
  opacity_uniform_.Locate(program_, "opacity");
  texture_uniform_.Locate(program_, "texture");
}

void GLSpriteShader::PassDimensionUniforms(f32 x,
                                           f32 y,
                                           f32 z,
                                           f32 rx,
                                           f32 ry,
                                           f32 rz,
                                           f32 width,
                                           f32 height,
                                           f32 opacity) {
  x_uniform_.Pass(x);
  y_uniform_.Pass(y);
  z_uniform_.Pass(z);
  rx_uniform_.Pass(rx);
  ry_uniform_.Pass(ry);
  rz_uniform_.Pass(rz);
  width_uniform_.Pass(width);
  height_uniform_.Pass(height);
  opacity_uniform_.Pass(opacity);
}

void GLSpriteShader::PassActiveTexture(u8 texture) {
  texture_uniform_.Pass(static_cast<GLint>(texture));
}

void GLSpriteShader::MapTexcoordBuffer(f32 u1, f32 u2, f32 v1, f32 v2) {
  f32* data = texcoords_buffer_.Map(GL_WRITE_ONLY);
  data[0] = u1;
  data[1] = v1;
  data[2] = u2;
  data[3] = v1;
  data[4] = u1;
  data[5] = v2;
  data[6] = u2;
  data[7] = v2;
  texcoords_buffer_.Unmap();
}

void GLSpriteShader::MapTexcoordBuffer(core::Vector4f uv) {
  MapTexcoordBuffer(uv.x(), uv.y(), uv.z(), uv.w());
}

void GLSpriteShader::MapTexcoordBuffer(f32 width,
                                       f32 height,
                                       core::Vector4f rect) {
  MapTexcoordBuffer(
      rect.x() / width, rect.y() / width, rect.z() / height, rect.w() / height);
}

void GLSpriteShader::MapTexcoordBuffer(f32 width,
                                       f32 height,
                                       f32 x1,
                                       f32 x2,
                                       f32 y1,
                                       f32 y2) {
  MapTexcoordBuffer(x1 / width, x2 / width, y1 / height, y2 / height);
}

void GLSpriteShader::PassAndEnableAttributes() {
  positions_buffer_.Bind();
  positions_attrib_.PassVertexPointer(positions_buffer_, 4);
  positions_attrib_.Enable();

  texcoords_buffer_.Bind();
  texcoords_attrib_.PassVertexPointer(texcoords_buffer_, 2);
  texcoords_attrib_.Enable();
}

void GLSpriteShader::DisableAttributes() {
  positions_attrib_.Disable();
  texcoords_attrib_.Disable();
}

void GLSpriteShader::Draw(const Sprite<GLTexture>& sprite) {
  MapTexcoordBuffer(sprite.texcoords());
  Render(sprite);
}

void GLSpriteShader::Draw(const Sprite<GLTexture>& sprite,
                          core::Vector4f rect) {
  MapTexcoordBuffer(
      sprite.get_texture()->width(), sprite.get_texture()->height(), rect);
  Render(sprite);
}

void GLSpriteShader::Render(const Sprite<GLTexture>& sprite) {
  glEnable(GL_DEPTH_TEST);
  program_.Use();

  PassProjectionViewUnifroms();
  PassDimensionUniforms(sprite.position().x(),
                        sprite.position().y(),
                        sprite.position().z(),
                        sprite.rotation().x(),
                        sprite.rotation().y(),
                        sprite.rotation().z(),
                        sprite.width(),
                        sprite.height(),
                        sprite.opacity());

  // TODO: smarter texture binding
  GLTexture::EnableBlending();

  sprite.get_texture()->Bind(active_texture_);  // This bind should change.
  PassActiveTexture(active_texture_);

  PassAndEnableAttributes();

  indices_buffer_.Bind();
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

  DisableAttributes();
}

void GLSpriteShader::Draw(const GLTexture& texture,
                          f32 width,
                          f32 height,
                          glm::vec3 position,
                          glm::vec4 texcoords,
                          f32 opacity,
                          glm::vec3 rotation) {

  MapTexcoordBuffer(texcoords);
  
  glEnable(GL_DEPTH_TEST);
  program_.Use();

  PassProjectionViewUnifroms();
  PassDimensionUniforms(position.x,
                        position.y,
                        position.z,
                        rotation.x,
                        rotation.y,
                        rotation.z,
                        width,
                        height,
                        opacity);

  // TODO: smarter texture binding
  GLTexture::EnableBlending();

  texture.Bind(active_texture_);  // This bind should change.
  PassActiveTexture(active_texture_);

  PassAndEnableAttributes();

  indices_buffer_.Bind();
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

  DisableAttributes();
}


}  // namespace render
