// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_EMPTY_TEXTURE_H_
#define OBSIDIAN_EMPTY_TEXTURE_H_

#include "memory.h"
#include "texture-loader-abstract.h"

namespace render {

// EmptyTextureLoader can be used as an TextureLoader to an extarnal
// texture data. 
class EmptyTextureLoader : public TextureLoaderAbstract {
 public:
  // This is just to satisfy the TextureLoaderAbstract.
  bool Load(const char* file_name) { return false; }

  // Emulate texture load, where |ptr| is texture data. 
  bool Load(size_t width, size_t height, byte* ptr, bool has_alpha = true) {
    if (has_alpha)
      gl_alpha_ = 4;
    else
      gl_alpha_ = 3;

    texture_data_ = ptr;

    width_ = width;
    height_ = height;
    has_alpha_ = has_alpha;

    return true;
  }

  void Destroy() {
    
  }
};

}  // namespace resource

#endif  // ifndef OBSIDIAN_EMPTY_TEXTURE_H_
