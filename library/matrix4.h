// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_MATRIX4_H_
#define OBSIDIAN_MATRIX4_H_

#include "vector4.h"
namespace core {

template <typename T>
class Matrix4 {
 public:
  Matrix4() {}
  Matrix4(Vector4<T> x, Vector4<T> y, Vector4<T> z, Vector4<T> w) {
    rows_[0] = x;
    rows_[1] = y;
    rows_[2] = z;
    rows_[3] = w;
  }

  T* operator[](size_t i) { return rows_[i].type_array(); }
  const T* type_array() const { return rows_[0].type_array(); }

  Matrix4<T> Transpose() {
    Matrix4<T> transposed = *this;
    transposed[0][1] = *this[1][0];
    transposed[0][2] = *this[2][0];
    transposed[0][3] = *this[3][0];

    transposed[1][0] = *this[0][1];
    transposed[1][2] = *this[2][1];
    transposed[1][3] = *this[3][1];

    transposed[2][0] = *this[0][2];
    transposed[2][1] = *this[1][2];
    transposed[2][3] = *this[3][2];

    transposed[3][0] = *this[0][3];
    transposed[3][1] = *this[1][3];
    transposed[3][2] = *this[2][3];

    return transposed;
  }

  size_t size() const { return sizeof(T) * 4 * 4; }

  static Matrix4<f32> Idenitity() {
    return Matrix4<f32>(Vector4f(1, 0, 0, 0),
                        Vector4f(0, 1, 0, 0),
                        Vector4f(0, 0, 1, 0),
                        Vector4f(0, 0, 0, 1));
  }



 private:
  Vector4<T> rows_[4];
};

typedef Matrix4<f32> Matrix4f;

}  // namespace core

#endif  // ifndef OBSIDIAN_MATRIX4_H_
