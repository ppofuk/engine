// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-program.h"

namespace render {

GLProgram::GLProgram() : program_(0), vaild_(false) {}

void GLProgram::Init() {
  // TODO: maybe fail check
  program_ = glCreateProgram();
}

void GLProgram::AttachShader(const GLShader& shader) {
  glAttachShader(program_, shader.get_shader());
}

void GLProgram::Link() {
  vaild_ = false;
  glLinkProgram(program_);

  GLint ok;
  glGetProgramiv(program_, GL_LINK_STATUS, &ok);
  if (ok) {
    vaild_ = true;
  }
}

char* GLProgram::InfoLog() {
  // PFNGLGETSHADERIVPROC glGet__iv = glGetProgramiv;
  PFNGLGETSHADERINFOLOGPROC glGet__InfoLog = glGetProgramInfoLog;
  glGet__InfoLog(program_, 1024, NULL, (GLchar* )&info_log_);
  return info_log_;
}

void GLProgram::Delete() {
  if (vaild_)
    glDeleteProgram(program_);
  vaild_ = false;
}

void GLProgram::Use() {
  if (vaild_) {
    glUseProgram(program_);
  }
}

void GLProgram::DetachShader(const GLShader& shader) {
  glDetachShader(program_, shader.get_shader());
}

} //  namespace render
