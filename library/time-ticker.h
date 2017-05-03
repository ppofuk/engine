// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_TIME_TICKER_H_
#define OBSIDIAN_TIME_TICKER_H_

#include "basic-types.h"
#include <time.h>

namespace core {

#if defined(OS_WINDOWS)
#include <windows.h>
class TimeTicker {
 public:
  TimeTicker() : time_saved_(0), time_updated_(0) {}

  // Updates the current intern time to |time_updated_|.
  void Update();

  // Returns the time of last Update() call in system clock representation.
  i64 time_on_update() { return time_updated_; }

  i64 tick_treshold() const { return tick_treshold_; }

  // Returns the current passed time since last Update()
  i64 passed_since_update();

  // Tick returns true if the tick_time is greater or equel to time between two
  // Tick calls. |tick_treshold()| will contain the size of deviation. The
  // deviation is < 0 if Tick yields true
  bool Tick(i64 tick_time);

 private:
  // Saves the current updated time to |time_saved_|.
  void Save();

  // Returns the difference between updated and saved time, the difference
  // between Save() and Update() calls.
  i64 time_between();

  LARGE_INTEGER frequency_, performance_count_;
  i64 time_saved_;
  i64 time_updated_;
  i64 tick_treshold_;
};

#elif defined(OS_LINUX)

#include <time.h>

class TimeTicker {
 public:
  TimeTicker() : time_saved_(0), time_updated_(0) {}

  // Updates the current intern time to |time_updated_|.
  void Update();

  // Returns the time of last Update() call in system clock representation.
  i64 time_on_update() { return time_updated_; }

  i64 tick_treshold() const { return tick_treshold_; }

  // Returns the current passed time since last Update()
  i64 passed_since_update();

  // Tick returns true if the tick_time is greater or equel to time between two
  // Tick calls. |tick_treshold()| will contain the size of deviation. The
  // deviation is < 0 if Tick yields true
  bool Tick(i64 tick_time);

 private:
  // Saves the current updated time to |time_saved_|.
  void Save();

  // Returns the difference between updated and saved time, the difference
  // between Save() and Update() calls.
  i64 time_between();

  i64 time_saved_;
  i64 time_updated_;
  i64 tick_treshold_;
};

#else  // not OS_WINDOWS or OS_LINUX

#endif  // defined(OS_WINDOWS) elif defined(OS_LINUX)

}  // namespace core

#endif  // ifndef OBSIDIAN_TIME_TICKER_H_
