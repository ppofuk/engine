// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_RENDER_GL_SPRITE_SHADER_H_
#define OBSIDIAN_RENDER_GL_SPRITE_SHADER_H_

#include "gl-program.h"
#include "gl-texture-2d.h"
#include "gl-shader.h"
#include "gl-buffer.h"
#include "gl-attribute.h"

#include "sprite.h"

#include "../static-compile-options.h"

namespace render {

namespace intern {
static GLfloat sprite_positions[] = {-1.0f,
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
static GLushort sprite_indices[] = {0, 1, 2, 3};
static GLfloat sprite_texcoords[] =
    {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

}  // namespace intern

class GLSpriteShader : public GLShader {
 public:
  void OnInit();
  void OnDestroy();
  void PostCompile();

  // Sprite drawing. Texture mapping is read from sprite texcoords vector.
  void Draw(const Sprite<GLTexture>& sprite);

  // Sprite texcoord vector is ignored.
  // rect is a rectangle bounded inside (0, 0, texture width, texture height).
  // void Draw(const Sprite<GLTexture>& sprite, core::Vector4f rect);
  // void Draw(const Sprite<GLTexture>& sprite, f32 u1, f32 u2, f32 v1, f32 v2);

  u8 get_active_texture() const { return active_texture_; }
  void set_active_texture(u8 active_texture) {
    active_texture_ = active_texture;
  }

 private:
  // Pass the x, y, z, width, height uniforms, aka. the sprite dimension
  // properties.
  inline void PassDimensionUniforms(f32 x,
                                    f32 y,
                                    f32 z,
                                    f32 rx,
                                    f32 ry,
                                    f32 rz,
                                    f32 width,
                                    f32 height,
                                    f32 opacity);

  inline void PassActiveTexture(u8 texture);

  void Render(const Sprite<GLTexture>& sprite);

  // Maps the u, v texcoords to appropriate values in texcoords buffer.
  void MapTexcoordBuffer(f32 u1, f32 u2, f32 v1, f32 v2);
  void MapTexcoordBuffer(core::Vector4f uv);
  // |width| and |height| are texture sizes, and |rect| must be in bounds of (0,
  // 0, width, height).
  // In the |rect| vector x, y, z, w translates to x1, x2, y1, y2 by the given
  // order. (!)
  void MapTexcoordBuffer(f32 width, f32 height, core::Vector4f rect);
  // x1, x2, y1, y2 are the bound of an rectangle that must reside inside 0, 0,
  // width, height.
  void MapTexcoordBuffer(f32 width, f32 height, f32 x1, f32 x2, f32 y1, f32 y2);

  void PassAndEnableAttributes();
  void DisableAttributes();

  u8 active_texture_ = 0;

  GLBuffer<GLfloat> positions_buffer_;
  GLBuffer<GLushort> indices_buffer_;
  GLBuffer<GLfloat> texcoords_buffer_;
  GLAttribute<GLBuffer<GLfloat> > positions_attrib_;
  GLAttribute<GLBuffer<GLfloat> > texcoords_attrib_;
  GLUniform x_uniform_;
  GLUniform y_uniform_;
  GLUniform z_uniform_;
  GLUniform rx_uniform_;
  GLUniform ry_uniform_;
  GLUniform rz_uniform_;
  GLUniform width_uniform_;
  GLUniform height_uniform_;
  GLUniform opacity_uniform_;
  GLUniform texture_uniform_;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_GL_SPRITE_SHADER_H_
