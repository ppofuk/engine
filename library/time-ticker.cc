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

void TimeTicker::Save() {
  time_saved_ = time_on_update();
}

i64 TimeTicker::time_between() {
  i64 increment = time_updated_ - time_saved_ < 0 ? 1000000000 : 0;
  return increment + time_updated_ - time_saved_;
}

void TimeTicker::Update() {
  timespec timespec_buffer;
  clock_gettime(CLOCK_REALTIME, &timespec_buffer);
  time_updated_ = timespec_buffer.tv_nsec;
}

i64 TimeTicker::passed_since_update() {
  timespec timespec_buffer;
  clock_gettime(CLOCK_REALTIME, &timespec_buffer);
  i64 increment = timespec_buffer.tv_nsec - time_updated_ < 0 ? 1000000000 : 0;
  return increment + timespec_buffer.tv_nsec - time_updated_;
}

bool TimeTicker::Tick(i64 tick_time) {
  Update();
  i64 increment = time_between() - tick_time < 0 ? 1000000000 : 0;
  tick_treshold_ = increment + time_between() - tick_time;
  if (time_between() >= tick_time) {
    Save();
    return true;
  }
  return false;
}


#endif  // defined(OS_LINUX) || defined(OS_WINDOWS)

}  // namespace core
