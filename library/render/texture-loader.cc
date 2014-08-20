// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "texture-loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace render {

bool TextureLoader::Load(const char* file_name) {
  texture_data_ = stbi_load(file_name, &width_, &height_, &gl_alpha_, 4);
  if (texture_data_) {
    has_alpha_ = true;
    return true;
  }
  return false;
}

bool TextureLoader::Load(const char* file_name, bool has_alpha) {
  if (has_alpha) {
    return Load(file_name);
  }

  texture_data_ = stbi_load(file_name, &width_, &height_, &gl_alpha_, 3);
  if (texture_data_) {
    has_alpha_ = true;
    return true;
  }

  return false;
}

void TextureLoader::Destroy() {
  if (texture_data_)
    stbi_image_free(texture_data_);
}

}  // namespace render
