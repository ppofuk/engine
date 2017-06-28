// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "gl-shader-object.h"
#include "../static-compile-options.h"

namespace render {

GLShaderObject::GLShaderObject()
    : shader_source_(0), vaild_(false), type_(GL_VERTEX_ARRAY) {}

void GLShaderObject::Compile(GLenum type) {
  type_ = type;
  vaild_ = false;
  if (!shader_source_) {
    log << util::kLogDateTime
        << ": shader_source not supplied in GLShaderObject at "
        << (size_t)((void*)this) << "\n";
    return;
  }

  GLint length = strlen(shader_source_);

#ifdef _DEBUG
  if (kOnDebugOutputShaderSource) {
    log << util::kLogDateTime << ": Compiling (" << length << "b):\n"
        << shader_source_ << "\n";
  }
#endif  // _DEBUG

  shader_ = glCreateShader(type);
  glShaderSource(shader_, 1, (const GLchar**)&shader_source_, &length);
  glCompileShader(shader_);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__ << ": "
        << (char*)gluErrorString(err) << "\n";
  }

  GLint ok;
  glGetShaderiv(shader_, GL_COMPILE_STATUS, &ok);

  if (ok == GL_TRUE) {
    vaild_ = true;
  }
}

char* GLShaderObject::InfoLog() {
  GLint length;
  glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &length);
  glGetShaderInfoLog(shader_, length, NULL, info_log_);
  info_log_[length] = 0;
  return info_log_;
}

void GLShaderObject::CreateVertexShader(const char* shader_source) {
  set_shader_source((char*)shader_source);
  Compile(GL_VERTEX_SHADER);
}

void GLShaderObject::CreateFragmentShader(const char* shader_source) {
  set_shader_source((char*)shader_source);
  Compile(GL_FRAGMENT_SHADER);
}

void GLShaderObject::Delete() {
  glDeleteShader(shader_);
  vaild_ = false;
}

}  //  namespace render
