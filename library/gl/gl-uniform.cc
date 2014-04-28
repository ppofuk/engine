// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-uniform.h"

#include <string.h>
#include <assert.h>

namespace render {
namespace intern {

GLUniformBase::GLUniformBase() : location_(-1), program_(0) {}

void GLUniformBase::Locate(const render::GLProgram& program,
                           const char* uniform_name) {
  program_ = program.get_program();
  assert(strlen(uniform_name) < kUniformNameSize - 1);
  memcpy(uniform_name_, uniform_name, kUniformNameSize);
  location_ = glGetUniformLocation(program_, uniform_name_);
}

}  // namespace intern
}  // namespace render
