// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_SPRITE_H_
#define OBSIDIAN_SPRITE_H_

#include "texture-abstract.h"
#include "sprite-properties.h"

namespace resource {

// Represents a renderable sprite.
// Before calling any rendering operation of sprite be sure to apply an
// instanced implementation of TextureAbstract class (e.g. TexturePng defined in
// texure-png.h). Also don't forget to set at least |properties.width| and
// |properties.height|. You can do that by calling |properties.Set| method, for
// example.
//   TexturePng texture;
//   texture.Load("texture.png");
//   Sprite sprite;
//   Sprite.set_texture(&texture);
//   Sprite.properties.Set(&texture);
class Sprite {
 public:
  Sprite();
  SpriteProperties properties;

  TextureAbstract* get_texture() const { return texture_; }
  void set_texture(TextureAbstract* texture) { texture_ = texture; }

 private:
  TextureAbstract* texture_;
};

}  // namespace resource
#endif  // OBSIDIAN_SPRITE_H_
