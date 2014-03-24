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

void GLProgram::AttachShader(GLShader& shader, bool delete_shader) {
  glAttachShader(program_, shader.get_shader());
  if (delete_shader)
    shader.Delete();
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

void GLProgram::DetachShader(GLShader& shader) {
  glDetachShader(program_, shader.get_shader());
}

template<typename Type>
GLUniform<Type> GLProgram::Uniform(Type& attribute, const char* name) {
  GLUniform<Type> uniform(attribute, name, program_);
  return uniform;
}

template<typename Type>
GLAttribute<Type> GLProgram::Attribute(Type& attribute, const char* name) {
  GLUniform<Type> ret_attribute(attribute, name, program_);
  return ret_attribute;
}

} //  namespace render
