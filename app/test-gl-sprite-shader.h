// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_APP_TEST_GL_SPRITE_SHADER_H_
#define OBSIDIAN_APP_TEST_GL_SPRITE_SHADER_H_

#include "render/gl-sprite-shader.h"
#include "render/gl-texture-2d.h"
#include "render/texture.h"

namespace app {

class TestGLSpriteShader {
 public:
  void Init();
  void Render();
  void Destroy();
 private:
  render::DefaultGLTexture actor_texture_;
  render::Sprite<render::GLTexture> actor_;
  render::GLSpriteShader sprite_shader_;
};

}  // namespace app

#endif  // ifndef OBSIDIAN_APP_TEST_GL_SPRITE_SHADER_H_
