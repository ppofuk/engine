// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_MEMORY_POOL_H_
#define OBSIDIAN_MEMORY_POOL_H_

#include <string.h>
#include <stdlib.h>
#include "static-compile-options.h"
#include "type-traits.h"
#include "linked-list.h"
#include <new>

namespace core {

namespace intern {

template <typename T>
class MemoryPoolObject : public LinkNode<MemoryPoolObject<T> > {
 public:
  friend class MemoryPool;
  MemoryPoolObject() {}

 private:
  template <typename ...Params>
  T* New(Params... params) {
    return new (memory_ptr_) T(params...);
  }

  T* New() {
    return new (memory_ptr_) T;
  }

  char memory_[SizeTrait<T>::size];
};

}  // namespace intern


template <typename T>
class MemoryPool {
 public:
  MemoryPool() : pool_(0), pool_count_(0) {}

  // Allocates |pool_|.
  void AllocatePool(size_t pool_count) {
    pool_count_ = pool_count;
    pool_ = new intern::MemoryPoolObject[pool_count];

  }

  void AllocatePool() { AllocatePool(kDefaultPoolObjectCount); }

  void DeallocatePool() {
    if (pool_) {
      delete[] pool_;
      pool_ = 0;
      pool_count_ = 0;
    }
  }


  // Returns the pool maximum size.
  size_t pool_count() const { return pool_count_; }

 protected:
  intern::MemoryPoolObject<T>* pool_;
  size_t pool_count_;
  LinkedList<intern::MemoryPoolObject<T> > free_list_;
};

}  // namespace core

#endif  // ifndef OBSIDIAN_MEMORY_POOL_H _
