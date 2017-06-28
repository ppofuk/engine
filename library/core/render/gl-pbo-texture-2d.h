// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_GL_PBO_TEXTURE_H_
#define OBSIDIAN_GL_PBO_TEXTURE_H_

#include "gl-texture-2d.h"
#include "gl-buffer.h"
#include "../logger.h"

namespace render {

class GLPixelBufferObjectTexture : public GLTexture {
 public:
  // Create or recreate a empty texture and PBO.
  // In the case of recreation (if texture was created before)
  // texture is just respecified with new width and height.
  // PBO size is presumed as |width| * |height| * 4 bytes.
  void Generate(f32 width, f32 height, GLenum format = GL_RGBA);

  // Returns a pointer for PBO mapping.
  void* Map(GLenum access = GL_WRITE_ONLY);

  // Unmaps the PBO and respecifies the texture.
  void Unmap();

  // void set_width(f32 width) { width_ = width; }
  // void set_height(f32 height) { height_ = height; }
  f32 width() { return width_; }
  f32 height() { return height_; }
  GLenum format() { return format_; }

 private:
  f32 width_, height_ = 0.0f;
  GLBuffer<void> pbo_;
  bool first_ = true;
  GLenum format_;
};

}  // namespace render

#endif  // define OBSIDIAN_GL_PBO_TEXTURE_H_
