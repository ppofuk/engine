// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "properties.h"

namespace core {

Properties::~Properties() {
  Destroy();
}

Properties::Properties(const char* json) : json_root_(0), working_dir_(0) {
  Load(json);
}

void Properties::Load(const char* json) {
  json_root_ = cJSON_Parse(json);
  working_dir_ = cJSON_GetObjectItem(json_root_, "working_dir");
}

void Properties::Destroy() {
  if (json_root_) {
    cJSON_Delete(json_root_);
    json_root_ = 0;
  }
}


}  // namespace core
