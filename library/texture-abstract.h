// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_TEXTURE_H_
#define OBSIDIAN_TEXTURE_H_

namespace resource {

class TextureAbstract {
 public:
  typedef unsigned char byte;

  TextureAbstract() : has_alpha_(false), width_(0), height_(0) {}

  // Virtual function definition that should be implemented
  virtual bool Load(const char* file) = 0;

  // Deallocate all junk, mostly texture data
  virtual void Destroy() = 0;

  inline bool has_alpha() { return has_alpha_; }

  inline int get_width() { return width_; }

  inline int get_height() { return height_; }

  inline byte* get_texture_data() { return texture_data_; }

  // For internal use with opengl renderer
  inline int get_gl_alpha() { return gl_alpha_; }

 protected:
  bool has_alpha_;
  int width_, height_;
  byte* texture_data_;

  int gl_alpha_;
};

}  // namespace resource

#endif  // OBSIDIAN_TEXTURE_H_
