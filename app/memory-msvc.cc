// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include <windows.h>
#include "logger.h"
#include "memory-pool.h"
#include "type-traits.h"
#include "time-ticker.h"

#include "vector4.h"

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show_num) {
  using namespace core;
  Vector2f vec2(1, 2);
  Vector4f vec4(vec2);

  return 0;
}
