// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-texture-2d.h"
#include <assert.h>

namespace render {

GLTexture::GLTexture()
    : gl_alpha_(0),
      gl_texture_(0),
      gl_min_filter_(GL_LINEAR),
      gl_mag_filter_(GL_LINEAR),
      gl_wrap_s_(GL_CLAMP),
      gl_wrap_t_(GL_CLAMP),
      texture_abstract_(0) {}

void GLTexture::Generate() {
  assert(texture_abstract_ != NULL);
  assert(texture_abstract_->get_texture_data() != NULL);

  glGenTextures(1, &gl_texture_);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap_s_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap_t_);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               texture_abstract_->get_gl_alpha(),
               texture_abstract_->get_width(),
               texture_abstract_->get_height(),
               0,
               texture_abstract_->has_alpha() ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE,
               texture_abstract_->get_texture_data());
}

void GLTexture::Bind() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, gl_texture_);
}

}  // namespace render
