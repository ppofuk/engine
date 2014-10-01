// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_MATRIX_UTILITY_H_
#define OBSIDIAN_MATRIX_UTILITY_H_

#include "matrix4.h"

namespace core {

class Frustrum {
 public:
  Frustrum();
  Frustrum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

  Matrix4f mat4() const { return mat4_; }

  static Matrix4f Calculate(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

 private:
  f32 l;
  f32 r;
  f32 b;
  f32 t;
  f32 n;
  f32 f;
  Matrix4f mat4_;
};

}  // namespace core

#endif  // ifndef OBSIDIAN_MATRIX_UTILITY_H_
