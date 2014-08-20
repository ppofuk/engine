// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <windows.h>
#include "logger.h"
#include "memory-pool.h"
#include "type-traits.h"
#include "time-ticker.h"

static int tmp = 0;

class vec3_non {
 public:
  vec3_non(int a = 0, int b = 0, int c = 0) : x(a), y(b), z(c) {}
  int x, y, z;
};

class vec3_fast : public core::PooledObject<SizeTrait<vec3_non>::size> {
 public:
  vec3_fast(int a = 0, int b = 0, int c = 0) : x(a), y(b), z(c) {}
  int x, y, z;
};

void TestPure() {
  for (size_t i = 0; i < kDefaultPoolObjectCount; i++) {
    vec3_non* vec = new vec3_non;
    vec->x = rand();
    delete vec;
  }
}


void TestFastObject() {
  for (size_t i = 0; i < kDefaultPoolObjectCount; i++) {
    vec3_fast* vec = new vec3_fast;
    vec->x = rand();
    delete vec;
  }
}

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  srand(time(0));

  core::TimeTicker time_ticker;
  util::HasLog logger;

  // Benchmark pure
  time_ticker.Update();
  i64 time_now = time_ticker.time_now();

  TestPure();
  time_ticker.Update();
  logger.log << util::kLogDateTime
             << " TestPure count: " << time_ticker.time_now() - time_now
             << "\n";

  // Benchmark fast pool

  time_ticker.Update();
  time_now = time_ticker.time_now();

  TestFastObject();
  time_ticker.Update();
  logger.log << util::kLogDateTime
             << " TestFast count: " << time_ticker.time_now() - time_now
             << "\n";

  return 0;
}
