// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_MATRIX_UTILITY_H_
#define OBSIDIAN_MATRIX_UTILITY_H_

#include "matrix4.h"

namespace core {

class Frustum {
 public:
  Frustum();
  Frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

  Matrix4f mat4() const { return mat4_; }
  void Set(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

  static Matrix4f Calculate(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

 private:
  f32 l_;
  f32 r_;
  f32 b_;
  f32 t_;
  f32 n_;
  f32 f_;
  Matrix4f mat4_;
};

class Perspective {
 public:
  Perspective();
  Perspective(f32 angle_of_view, f32 aspect_ratio, f32 z_near, f32 z_far);
  static Matrix4f Calculate(f32 angle_of_view,
                            f32 aspect_ratio,
                            f32 z_near,
                            f32 z_far);

  void Set(f32 angle_of_view, f32 aspect_ratio, f32 z_near, f32 z_far);
  Matrix4f mat4() const { return mat4_; }

 private:
  f32 angle_of_view_;
  f32 aspect_ratio_;
  f32 z_near_;
  f32 z_far_;

  Matrix4f mat4_;
};

class Ortho {
 public:
  Ortho();
  Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

  Matrix4f mat4() const { return mat4_; }
  void Set(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

  static Matrix4f Calculate(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);

 private:
  f32 l_;
  f32 r_;
  f32 b_;
  f32 t_;
  f32 n_;
  f32 f_;
  Matrix4f mat4_;
};

class Identity {
 public:
  static Matrix4f Calculate() {
    return Matrix4f(Vector4f(1, 0, 0, 0),
                    Vector4f(0, 1, 0, 0),
                    Vector4f(0, 0, 1, 0),
                    Vector4f(0, 0, 0, 1));
  }
};

}  // namespace core

#endif  // ifndef OBSIDIAN_MATRIX_UTILITY_H_
