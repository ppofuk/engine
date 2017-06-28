// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-attribute.h"

#include <string.h>
#include <assert.h>

namespace render {
namespace intern {

GLAttributeBase::GLAttributeBase() : location_(-1), program_(0) {
}

void GLAttributeBase::Locate(const render::GLProgram& program,
                             const char* attrib_name) {
  program_ = program.get_program();
  assert(strlen(attrib_name) < kAttribNameSize - 1);
  memcpy(attrib_name_, attrib_name, kAttribNameSize);
  location_ = glGetAttribLocation(program_, attrib_name_);

#ifdef _DEBUG
  if (location_ == -1) {
    util::Log << util::kLogDateTime << ": Warning! " << __FILE__ << ": "
              << __LINE__ << " location_ is -1 for " << attrib_name << " \n";
  }
#endif  // _DEBUG
}

void GLAttributeBase::BindLocation(const render::GLProgram& program,
                                   GLuint index,
                                   const char* attrib_name) {
  glBindAttribLocation(program.get_program(), index, attrib_name);
  location_ = index;
  program_ = program.get_program();
}

}  // namespace intern
}  // namespace render
