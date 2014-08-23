// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_VECTOR4_H_
#define OBSIDIAN_VECTOR4_H_

#include "vector2.h"
#include "vector3.h"

namespace core {

template <typename T>
class Vector4 {
 public:
  Vector4() {}
  Vector4(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}
  Vector4(T* ptr) : x_(ptr[0]), y_(ptr[1]), z_(ptr[2]), w_(ptr[3]) {}

  explicit Vector4(const Vector2<T>& other)
      : x_(other.x()), y_(other.y()), z_(0), w_(0) {}

  explicit Vector4(const Vector3<T>& other)
      : x_(other.x()), y_(other.y()), z_(other.z()), w_(0) {}

  // Returns true if all components are 0.
  bool IsZero() const { return x() == y() == z() == w() == 0; }

  // Add the components of the |vector4| to the current vector.
  void Add(const Vector4<T>& other) {
    x_ += other.x();
    y_ += other.y();
    z_ += other.z();
    w_ += other.w();
  }

  // Subtract the components of the |vector4| to the current vector.
  void Subtract(const Vector4<T>& other) {
    x_ -= other.x();
    y_ -= other.y();
    z_ -= other.z();
    w_ -= other.w();
  }

  void operator+=(const Vector4<T>& other) { Add(other); }
  void operator-=(const Vector4<T>& other) { Subtract(other); }

  T x() const { return x_; }
  T y() const { return y_; }
  T z() const { return z_; }
  T w() const { return w_; }

  void set_x(T x) { x_ = x; }
  void set_y(T y) { y_ = y; }
  void set_z(T z) { z_ = z; }
  void set_w(T w) { w_ = w; }

  // Unsafe but it works.
  operator T*() { return &x_; }
  operator Vector2<T>() { return Vector2<T>(x_, y_); }
  operator Vector3<T>() { return Vector3<T>(x_, y_, z_); }

  Vector2<T> ToVector2() const { return Vector2<T>(x_, y_); }
  Vector3<T> ToVector3() const { return Vector3<T>(x_, y_, z_); }

  T* type_array() { return &x_; }

 private:
  T x_;
  T y_;
  T z_;
  T w_;
};

template <typename T>
inline bool operator==(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z() &&
         lhs.w() == rhs.w();
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T>& v) {
  return Vector4<T>(-v.x(), -v.y(), -v.z(), -v.w());
}

template <typename T>
inline Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  Vector4<T> result = lhs;
  result.Add(rhs);
  return result;
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  Vector4<T> result = lhs;
  result.Subtract(rhs);
  return result;
}

typedef Vector4<f32> Vector4f;
typedef Vector4<u32> Vector4ui;
typedef Vector4<i32> Vector4i;

typedef Vector4<u8> Color4;
typedef Vector4<f32> Coord;

}  // namespace core

#endif  // ifndef OBSIDIAN_VECTOR4_H_
