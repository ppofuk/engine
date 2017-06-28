// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_RENDER_TEXTURE_H_
#define OBSIDIAN_RENDER_TEXTURE_H_

#include "texture-loader.h"

namespace render {
// TextureType is something like GLTexture from gl-texture-2d.h.
// In order for Texture class to function TextureType must implement
// void Generate(), void set_texture_loader(TextureLoaderAbstract*),
// TextureLoaderAbstract* get_texture_loader() methods.
// TextureLoaderType must be a inherited type of TextureLoaderAbstract.
template <typename TextureType, typename TextureLoaderType = TextureLoader>
class Texture {
 public:
  void Load(const char* path) {
    if (texture_loader_.Load(path)) {
      texture_.set_texture_loader(&texture_loader_);
      texture_.Generate();
    } else {
      util::Log << util::kLogDateTime << ": Can't load " << path << "!\n";
    }
  }

  void Destroy() {
    texture_loader_.Destroy();
  }

  TextureLoaderType* texture_loader() { return &texture_loader_; }
  TextureType* texture() { return &texture_; }
 private:
  TextureType texture_;
  TextureLoaderType texture_loader_;
};

}  // namespace render

#endif  // ifndef OBSIDIAN_RENDER_TEXTURE_H_
