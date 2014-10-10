// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "test-gl-sprite-shader.h"

namespace app {

void TestGLSpriteShader::Init() {
  actor_texture_.Load("resources/actor.png");
  actor_.On(&actor_texture_);

  util::Log << util::kLogDateTime << " actor.png loaded\n";

  sprite_shader_.Init();
  util::Log << util::kLogDateTime << " Sprite shader init'ed\n";
  sprite_shader_.set_view_frustum(
      core::Frustum::Calculate(0.0, 1920.0, 1680.0, 0.0, -1.0, 1.0));
}

void TestGLSpriteShader::Render() {
  sprite_shader_.Draw(actor_);
}

void TestGLSpriteShader::Destroy() {
  sprite_shader_.Destroy();
  actor_texture_.Destroy();
}

}  // namespace app
