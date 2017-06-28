// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "matrix-utility.h"
#include <math.h>

namespace core {

Matrix4f Frustum::Calculate(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
  return Matrix4f(
      Vector4f(2 * n / (r - l), 0.0, 0.0, 0.0),
      Vector4f(0.0, 2 * n / (t - b), 0.0, 0.0),
      Vector4f((r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1),
      Vector4f(0.0, 0.0, -2 * f * n / (f - n), 0));
}

Frustum::Frustum(){};
Frustum::Frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
    : l_(l),
      r_(r),
      b_(b),
      t_(t),
      n_(n),
      f_(f),
      mat4_(Calculate(l, r, b, t, n, f)) {
}

void Frustum::Set(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
  l_ = l;
  r_ = r;
  b_ = b;
  t_ = t;
  n_ = n;
  f_ = f;
  mat4_ = Calculate(l, r, b, t, n, f);
}

Matrix4f Perspective::Calculate(f32 fov, f32 a, f32 n, f32 f) {
  float d = 1 / tan(fov / 2);
  return Matrix4f(Vector4f(d / a, 0, 0, 0),
                  Vector4f(0, d, 0, 0),
                  Vector4f(0, 0, (n + f) / (n - f), (2 * n * f) / (n - f)),
                  Vector4f(0, 0, -1, 0));
}

Perspective::Perspective() {
}

Perspective::Perspective(f32 angle_of_view,
                         f32 aspect_ratio,
                         f32 z_near,
                         f32 z_far)
    : angle_of_view_(angle_of_view),
      aspect_ratio_(aspect_ratio),
      z_near_(z_near),
      z_far_(z_far),
      mat4_(Calculate(angle_of_view, aspect_ratio, z_near, z_far)) {
}

void Perspective::Set(f32 angle_of_view,
                      f32 aspect_ratio,
                      f32 z_near,
                      f32 z_far) {
  angle_of_view_ = angle_of_view;
  aspect_ratio_ = aspect_ratio;
  z_near_ = z_near;
  z_far = z_far_;
  mat4_ = Calculate(angle_of_view, aspect_ratio, z_near, z_far);
}

Matrix4f Ortho::Calculate(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
  return Matrix4f(Vector4f(2.0 / (r - l), 0.0, 0.0, (l + r) / (l - r)),
                  Vector4f(0.0, 2.0 / (t - b), 0.0, (b + t) / (b - t)),
                  Vector4f(0.0, 0.0, 2.0 / (n - f), (n + f) / (n - f)),
                  Vector4f(0.0, 0.0, 0.0, 1.0));
}

Ortho::Ortho(){};
Ortho::Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
    : l_(l),
      r_(r),
      b_(b),
      t_(t),
      n_(n),
      f_(f),
      mat4_(Calculate(l, r, b, t, n, f)) {
}

void Ortho::Set(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
  l_ = l;
  r_ = r;
  b_ = b;
  t_ = t;
  n_ = n;
  f_ = f;
  mat4_ = Calculate(l, r, b, t, n, f);
}

}  // namespace core
