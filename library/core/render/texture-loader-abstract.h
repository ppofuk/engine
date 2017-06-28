// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_TEXTURE_H_
#define OBSIDIAN_TEXTURE_H_

#include "../logger.h"
#include "../basic-types.h"

namespace render {

typedef i16 TextureHandleType;

class TextureLoaderAbstract : public util::HasLog {
 public:
  typedef unsigned char byte;

  TextureLoaderAbstract()
      : has_alpha_(false), width_(0), height_(0), handle_(-1) {}

  virtual bool Load(const char* file_name) = 0;
  // Deallocate all junk, mostly texture data
  virtual void Destroy() = 0;

  inline bool has_alpha() { return has_alpha_; }

  inline int get_width() { return width_; }

  inline int get_height() { return height_; }

  inline byte* get_texture_data() { return texture_data_; }

  // For internal use with opengl renderer
  inline int get_gl_alpha() { return gl_alpha_; }

  inline TextureHandleType get_handle() { return handle_; }

  inline void set_handle(TextureHandleType handle) { handle_ = handle; }

 protected:
  bool has_alpha_;
  int width_, height_;
  byte* texture_data_;

  int gl_alpha_;
  TextureHandleType handle_;
};

}  // namespace resource

#endif  // OBSIDIAN_TEXTURE_H_
