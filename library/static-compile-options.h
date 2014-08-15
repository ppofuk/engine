// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_STATIC_COMPILE_OPTIONS_H_
#define OBSIDIAN_STATIC_COMPILE_OPTIONS_H_
#include "limits.h"

// Options like static array sizes and such should be defined here along
// with corresponding comment on what they effect.
enum StaticArraySizes {
  kUniformNameSize = 24,  // size of char[] for storing uniform variable names
  kAttribNameSize = 24,   // size of char[] for storing attrib varibale names
};

enum GLShaderCompileOptions { kOnDebugOutputShaderSource = false };

// Enumerations of some used types for RTTI or templating.
enum Types {
  kUknownType,
};


#define kDefaultPoolObjectCount 4096*4096

namespace util {
static const char* log_path_file = "log.txt";
}  // namespace util

#endif  // OBSIDIAN_STATIC_COMPILE_OPTIONS_H_
