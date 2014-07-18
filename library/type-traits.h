// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_TYPE_TRAITS_H_
#define OBSIDIAN_TYPE_TRAITS_H_

// If you need to define a types instance default size, you can specialize the
// SizeTrait template or you can use the OBSIDIAN_DEFINE_SIZE macro.
template <typename T>
struct SizeTrait {
  enum Size { size = sizeof(T) };
};

// Wraps the SizeTrait template struct into macro, where T is type and S is
// size.
#define OBSIDIAN_DEFINE_SIZE(T, S) \
  template <>                      \
  struct SizeTrait<T> {            \
    enum Size { size = S; };       \
  };

#endif  // ifndef OBSIDIAN_TYPE_TRAITS_H_
