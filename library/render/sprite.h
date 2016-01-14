// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_RENDER_SPRITE_META_H_
#define OBSIDIAN_RENDER_SPRITE_META_H_

#include "../basic-types.h"
#include "../static-compile-options.h"
#include "../vector4.h"

#include "texture-loader-abstract.h"
#include "texture.h"

namespace render {

// Represents the meta informations on sprite.
// Position vector is trivial, the x, y, z represent position maps.
// In rotation vector x, y, z represent rotation angles, aka. x = rx and so on.

#include "texture.h"

template <typename TextureType>
class Sprite {
 public:
  core::Vector3f& position() { return position_; }
  core::Vector3f& rotation() { return rotation_; }
  core::Vector3f position() const { return position_; }
  core::Vector3f rotation() const { return rotation_; }
  void set_position(core::Vector3f position) { position_ = position; }
  void set_rotation(core::Vector3f rotation) { rotation_ = rotation; }
  f32 opacity() const { return opacity_; }
  void set_opacity(f32 opacity) { opacity_ = opacity; }
  f32 width() const { return width_; }
  void set_width(f32 width) { width_ = width; }
  f32 height() const { return height_; }
  void set_height(f32 height) { height_ = height; }

  TextureType* get_texture() const { return texture_; }
  TextureType* set_texture(TextureType* texture) { texture_ = texture; }

  core::Vector4f& texcoords() { return texcoords_; }
  core::Vector4f texcoords() const { return texcoords_; }
  void set_texcoords(core::Vector4f texcoords) { texcoords_ = texcoords; }

  template <typename TextureLoaderType>
  void On(Texture<TextureType, TextureLoaderType>* texture) {
    texture_ = texture->texture();
    width_ = texture_->get_texture_loader()->get_width();
    height_ = texture_->get_texture_loader()->get_height();
  }

  // Presumes that texture is a sheet texture with row_count rows and
  // column_count columns.
  // Set's the uv textures for the given row, column.
  // If inverse is true, the sheet will be inversed.
  void SetSheetPosition(u16 row_count,
                        u16 row,
                        u16 column_count,
                        u16 column,
                        bool inverse) {
    row = row_count - row;

    if (inverse) {
      texcoords_.set_u2((f32)column / column_count);
      texcoords_.set_u1((f32)(column + 1) / column_count);
    } else {
      texcoords_.set_u1((f32)column / column_count);
      texcoords_.set_u2((f32)(column + 1) / column_count);
    }
    texcoords_.set_v2(1 - ((f32)row / row_count));
    texcoords_.set_v1(1 - ((f32)(row + 1)) / row_count);
  }

 private:
  core::Vector3f position_ = core::Vector3f(0, 0, 0);
  core::Vector3f rotation_ = core::Vector3f(0, 0, 0);
  core::Vector4f texcoords_ = core::Vector4f(0, 1, 0, 1);
  f32 opacity_ = 1.0f;
  f32 width_ = 0.0f;
  f32 height_ = 0.0f;

  TextureType* texture_ = 0;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_SPRITE_META_H_
