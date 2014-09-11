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
  T* operator[](size_t i) { return rows_[i].type_array(); }
  T* type_array() const { return rows_[0].type_array(); }

  Matrix4<T> Transpose() const {
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

 private:
  Vector4<T> rows_[4];
};

void TestMatrix4() {
  Matrix4<float> mat4;
  mat4[1][2] = 2;
}

}  // namespace core

#endif  // ifndef OBSIDIAN_MATRIX4_H_
