// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "time-ticker.h"
namespace core {

#if defined(OS_WINDOWS)

void TimeTicker::Update() {
  QueryPerformanceFrequency(&frequency_);
  QueryPerformanceCounter(&performance_count_);
  time_updated_ = performance_count_.QuadPart;
}

void TimeTicker::Save() {
  time_saved_ = time_on_update();
}

i64 TimeTicker::time_between() {
  return time_updated_ - time_saved_;
}

i64 TimeTicker::passed_since_update() {
  QueryPerformanceFrequency(&frequency_);
  QueryPerformanceCounter(&performance_count_);
  return performance_count_.QuadPart - time_updated_;
}

bool TimeTicker::Tick(i64 tick_time) {
  Update();
  tick_treshold_ = (time_between() - (tick_time / 1000)) * 1000;
  if (time_between() >= tick_time / 1000) {
    Save();
    return true;
  }
  return false;
}

#elif defined(OS_LINUX)

// TODO: linux implementation

#endif  // defined(OS_LINUX) || defined(OS_WINDOWS)

}  // namespace core
