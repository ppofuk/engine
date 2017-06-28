// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "texture-loader-abstract.h"

#ifndef OBSIDIAN_TEXTURE_LOADER_H_
#define OBSIDIAN_TEXTURE_LOADER_H_

namespace render {

class TextureLoader : public TextureLoaderAbstract {
 public:
  bool Load(const char* file_name);
  bool Load(const char* file_name, bool has_alpha);
  void Destroy();
};

}  // namespace render

#endif  // ifndef OBSIDIAN_TEXTURE_LOADER_H_
