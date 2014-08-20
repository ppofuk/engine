// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_EMPTY_TEXTURE_H_
#define OBSIDIAN_EMPTY_TEXTURE_H_

#include "memory.h"
#include "texture-loader-abstract.h"

namespace render {

template <typename MemoryAllocator = core::MallocMemoryAllocator>
class EmptyTextureLoader : public TextureLoaderAbstract {
 public:
  // This is just to satisfy the TextureLoaderAbstract.
  bool Load(const char* file_name) { return false; }

  bool Load(size_t width, size_t height, bool has_alpha = true) {
    if (texture_data_)
      Destory();

    if (has_alpha)
      gl_alpha_ = 4;
    else
      gl_alpha = 3;

    texture_data_ = MemoryAllocator::New(width * height * gl_alpha_);
    if (!texture_data_)
      return false;

    width_ = width;
    height_ = height;
    has_alpha_ = has_alpha;

    return true;
  }

  void Destroy() {
    MemoryAllocator::Delete(texture_data_);
    width_ = height_ = 0;
  }
};

typedef EmptyTextureLoader<> DefaultEmptyTextureLoader;

}  // namespace resource

#endif  // ifndef OBSIDIAN_EMPTY_TEXTURE_H_
