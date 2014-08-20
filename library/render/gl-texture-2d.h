// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_TEXTURE_H_
#define OBSIDIAN_GL_TEXTURE_H_

#include "GL/glew.h"
#include "texture-loader-abstract.h"

namespace render {
class GLTexture {
 public:
  GLTexture();

  GLint get_gl_alpha() const { return gl_alpha_; }
  GLuint get_gl_texture() const { return gl_texture_; }
  GLint get_gl_min_filter() const { return gl_min_filter_; }
  GLint get_gl_mag_filter() const { return gl_mag_filter_; }
  GLint get_gl_wrap_s() const { return gl_wrap_s_; }
  GLint get_gl_wrap_t() const { return gl_wrap_t_; }

  void set_gl_min_filter(GLint gl_min_filter) {
    gl_min_filter_ = gl_min_filter;
  }
  void set_gl_mag_filter(GLint gl_mag_filter) {
    gl_mag_filter_ = gl_mag_filter;
  }
  void set_gl_wrap_s(GLint gl_wrap_s) { gl_wrap_s_ = gl_wrap_s; }
  void set_gl_wrap_t(GLint gl_wrap_t) { gl_wrap_t_ = gl_wrap_t; }

  TextureLoaderAbstract* get_texture_loader() const {
    return texture_abstract_;
  }
  void set_texture_loader(TextureLoaderAbstract* texture_abstract) {
    texture_abstract_ = texture_abstract;
  }

  // Create a OpenGL texture object (referenced by gl_texture_) based on class
  // gl_* parameters.
  void Generate();

  void Bind();

 private:
  GLint gl_alpha_;
  GLuint gl_texture_;
  GLint gl_min_filter_;
  GLint gl_mag_filter_;
  GLint gl_wrap_s_;
  GLint gl_wrap_t_;

  TextureLoaderAbstract* texture_abstract_;
};

}  // namespace render

#endif  // OBSIDIAN_GL_TEXTURE_H_
