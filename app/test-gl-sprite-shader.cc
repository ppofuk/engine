// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "test-gl-sprite-shader.h"

namespace app {

void TestGLSpriteShader::Init() {
  glEnable(GL_DEPTH_TEST);


  background_texture_.Load("resources/back.jpg");
  background_.On(&background_texture_);

  actor_texture_.Load("resources/actor.png");
  actor_.On(&actor_texture_);

  sprite_shader_.Init();
  sprite_shader_.Compile();

  // sprite_shader_.set_view_frustum(
  //     core::Ortho::Calculate(0.0, 1920, 1080, 0.0, 0.1, 10000.0));
  sprite_shader_.set_projection(
      core::Perspective::Calculate(45, 4.0f / 3, 0.1, 10000));
  sprite_shader_.set_view(core::Matrix4f::Idenitity());

  actor_.position().set_x(500);
  actor_.position().set_y(500);
  actor_.position().set_z(-1000);
  background_.position().set_z(-5000);

  actor_.set_width(actor_.get_texture()->get_texture_loader()->get_width() /
                   column_count_);
  actor_.set_height(actor_.get_texture()->get_texture_loader()->get_height() /
                    row_count_);
  actor_.SetSheetPosition(row_count_, 1, column_count_, frame_, false);
}

void TestGLSpriteShader::IncreaseFrame() {
  frame_ = (++frame_) % column_count_;
  actor_.SetSheetPosition(row_count_, 1, column_count_, frame_, false);
}

void TestGLSpriteShader::Render() {
  sprite_shader_.Draw(background_);
  sprite_shader_.Draw(actor_);
}

void TestGLSpriteShader::Destroy() {
  sprite_shader_.Destroy();
  actor_texture_.Destroy();
}

}  // namespace app
