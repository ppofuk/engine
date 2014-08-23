// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_VECTOR2_H_
#define OBSIDIAN_VECTOR2_H_

#include "memory.h"
#include "memory-pool.h"
#include "logger.h"

namespace core {

template <typename T>
class Vector2 {
 public:
  Vector2() {}
  Vector2(T x, T y) : x_(x), y_(y) {}
  Vector2(T* ptr) : x_(ptr[0]), y_(ptr[1])) {}

  // Returns true if all components are 0.
  bool IsZero() const { return x() == y() == 0; }

  // Add the components of the |Vector2| to the current vector.
  void Add(const Vector2<T>& other) {
    x_ += other.x();
    y_ += other.y();
  }

  // Subtract the components of the |Vector2| to the current vector.
  void Subtract(const Vector2<T>& other) {
    x_ -= other.x();
    y_ -= other.y();
  }

  void operator+=(const Vector2<T>& other) { Add(other); }
  void operator-=(const Vector2<T>& other) { Subtract(other); }

  T x() const { return x_; }
  T y() const { return y_; }

  void set_x(T x) { x_ = x; }
  void set_y(T y) { y_ = y; }

  // Unsafe but it works.
  operator T*() { return &x_; }
  T* type_array() { return &x_; }

 private:
  T x_;
  T y_;
};

template <typename T>
inline bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& v) {
  return Vector2<T>(-v.x(), -v.y());
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  Vector2<T> result = lhs;
  result.Add(rhs);
  return result;
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  Vector2<T> result = lhs;
  result.Subtract(rhs);
  return result;
}

typedef Vector2<float> Vector2f;
typedef Vector2<u32> Vector2ui;
typedef Vector2<i32> Vector2i;

}  // namespace core

#endif  // ifndef OBSIDIAN_VECTOR2_H_
