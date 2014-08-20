// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_MEMORY_H_
#define OBSIDIAN_MEMORY_H_

#include "basic-types.h"
#include "static-compile-options.h"
#include <stdlib.h>

namespace core {

class MallocMemoryAllocator {
 public:
  static void* New(size_t size) { return malloc(size); }
  static void Delete(void* ptr) { free(ptr); }
};

class MallocMemoryAllocatorObject {
  // Default allocator is malloc so we don't need to explicitly define new
  // operator.
};

}  // namespace core

#endif  // ifndef OBSIDIAN_MEMORY_H_
