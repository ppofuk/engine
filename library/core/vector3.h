// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_VECTOR3_H_
#define OBSIDIAN_VECTOR3_H_

#include "vector2.h"

#include "glm/glm.hpp"

namespace core {

template <typename T>
class Vector3 {
 public:
  Vector3() {}
  Vector3(T x, T y, T z) : x_(x), y_(y), z_(z) {}
  Vector3(T* ptr) : x_(ptr[0]), y_(ptr[1]), z_(ptr[2]) {}
  Vector3(const glm::vec3& vector) : x_(vector.x), y_(vector.y), z_(vector.z) {}


  explicit Vector3(const Vector2<T>& other)
      : x_(other.x()), y_(other.y()), z_(0) {}

  // Returns true if all components are 0.
  bool IsZero() const { return x() == y() == z() == 0; }

  // Add the components of the |Vector3| to the current vector.
  void Add(const Vector3<T>& other) {
    x_ += other.x();
    y_ += other.y();
    z_ += other.z();
  }

  // Subtract the components of the |Vector3| to the current vector.
  void Subtract(const Vector3<T>& other) {
    x_ -= other.x();
    y_ -= other.y();
    z_ -= other.z();
  }

  void operator+=(const Vector3<T>& other) { Add(other); }
  void operator-=(const Vector3<T>& other) { Subtract(other); }

  T x() const { return x_; }
  T y() const { return y_; }
  T z() const { return z_; }

  void set_x(T x) { x_ = x; }
  void set_y(T y) { y_ = y; }
  void set_z(T z) { z_ = z; }

  // Unsafe but it works.
  operator T*() { return &x_; }
  operator Vector2<T>() { return Vector2<T>(x_, y_); }

  Vector2<T> ToVector2() const { return Vector2<T>(x_, y_); }
  T* type_array() { return &x_; }

 private:
  T x_;
  T y_;
  T z_;
};

template <typename T>
inline bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& v) {
  return Vector3<T>(-v.x(), -v.y(), -v.z());
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  Vector3<T> result = lhs;
  result.Add(rhs);
  return result;
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  Vector3<T> result = lhs;
  result.Subtract(rhs);
  return result;
}

typedef Vector3<float> Vector3f;
typedef Vector3<u32> Vector3ui;
typedef Vector3<i32> Vector3i;

}  // namespace core

#endif  // ifndef OBSIDIAN_VECTOR3_H_
