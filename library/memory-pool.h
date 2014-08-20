// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <new>
#include <stdlib.h>
#include "basic-types.h"
#include "static-compile-options.h"

namespace core {
namespace intern {

template <size_t chunk_size>
struct PoolChunk {
  PoolChunk()
      : next(reinterpret_cast<PoolChunk<chunk_size>*>(
            (char*)this + sizeof(PoolChunk*) + chunk_size)) {}
  PoolChunk* next;
  char memory[chunk_size];
};

}  // namespace intern

template <size_t chunk_size>
class MemoryPoolHolder {
 public:
  static void AllocateChunkRegionUnsafe(
      size_t chunk_count = kDefaultPoolObjectCount) {
    chunk_count_ = chunk_count;
    chunks_ = new intern::PoolChunk<chunk_size>[chunk_count];
    chunks_[--chunk_count].next = 0;
    head_ = &chunks_[0];
  }

  static void DestroyChunkRegionUnsafe() {
    head_ = 0;
    chunk_count_ = 0;
    delete[] chunks_;
  }

  static intern::PoolChunk<chunk_size>* head() { return head_; }
  static void append(intern::PoolChunk<chunk_size>* chunk) {
    chunk->next = head_;
    head_ = chunk;
  }
  static void set_head(intern::PoolChunk<chunk_size>* chunk) { head_ = chunk; }

  static bool is_null() { return chunks_ == 0; }

 private:
  static intern::PoolChunk<chunk_size>* head_;
  static intern::PoolChunk<chunk_size>* chunks_;
  static size_t chunk_count_;
};

template <size_t chunk_size>
intern::PoolChunk<chunk_size>* MemoryPoolHolder<chunk_size>::chunks_ = 0;

template <size_t chunk_size>
intern::PoolChunk<chunk_size>* MemoryPoolHolder<chunk_size>::head_ = 0;

template <size_t chunk_size>
size_t MemoryPoolHolder<chunk_size>::chunk_count_ = 0;

template <size_t chunk_size,
          typename StaticPoolType = MemoryPoolHolder<chunk_size> >
class PooledObject {
 public:
  void* operator new(size_t size) {
    if (StaticPoolType::is_null()) {
      StaticPoolType::AllocateChunkRegionUnsafe();
    }

    intern::PoolChunk<chunk_size>* chunk = StaticPoolType::head();
    if (chunk == 0) {
      return NULL;
    }

    StaticPoolType::set_head(chunk->next);
    return (void*)(chunk->memory);
  }

  void operator delete(void* ptr) {
    intern::PoolChunk<chunk_size>* chunk =
        reinterpret_cast<intern::PoolChunk<chunk_size>*>(
            (char*)ptr - sizeof(intern::PoolChunk<chunk_size>*));
    StaticPoolType::append(chunk);
  }
};


}  // namespace core
