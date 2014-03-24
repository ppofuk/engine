// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_TEXTURE_PNG_H_
#define OBSIDIAN_TEXTURE_PNG_H_

#include "texture-abstract.h"
#include "png.h"
#include <stdio.h>

namespace resource {

class TexturePng : public resource::TextureAbstract {
 public:
  bool Load(const char* file);

  // Clear texture data from heap
  void Destroy();
};

}  // namespace resource
#endif  // OBSIDIAN_TEXTURE_PNG_H_
