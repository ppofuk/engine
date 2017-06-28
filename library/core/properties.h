// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string.h>
#include "c-json.h"
#include "basic-types.h"
#include "static-compile-options.h"

#include "default-properties-json.h"

namespace core {

class Properties {
 public:
  Properties(const char* json = default_properties_json);
  ~Properties();

  void Load(const char* json);
  void Destroy();

  char* RenderJSON() { return cJSON_Print(json_root_); }

  const char* working_dir() const { return working_dir_->valuestring; }
  void set_working_dir(char* working_dir) {
    working_dir_->valuestring = working_dir;
  }

  const char* json() { return cJSON_Print(json_root_); }

 private:
  cJSON* json_root_;
  cJSON* working_dir_;

  DISALLOW_COPY_AND_ASSIGN(Properties);
};

}  // namespace core
