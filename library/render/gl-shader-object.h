// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_SHADER_H_
#define OBSIDIAN_GL_SHADER_H_

#include "GL/glew.h"
#include <string.h>

#include "../logger.h"
#include <stdlib.h>

#include "../reader-inl.h"

namespace render {
// This class represents a GLSL shader, defined as an char* source.
class GLShaderObject : public util::HasLog {
 public:
  GLShaderObject();

  char* get_shader_source() const { return shader_source_; }
  void set_shader_source(char* shader_source) {
    shader_source_ = shader_source;
  }
  bool is_vaild() const { return vaild_; }
  GLuint get_shader() const { return shader_; }
  GLenum get_gl_type() const { return type_; }

  // Get compile errors and etc. of GLSL shader.
  // You should check with is_vaild() method before running this method.
  char* InfoLog();

  // Compile a GLSL shader from shader_source_.
  // |type| is a GL enumeration of GLSL shader type.
  void Compile(GLenum type);

  // After linking the program it's safe to call shader deletion.
  void Delete();

  // If you wish to recompile an existing shader, use one of this methods with
  // get_shader_source() method.
  // For example:
  //   MyShader.CreateVertexShader((const char*)MyShader.get_shader_source());
  // or with Compile and get_gl_type methods:
  //   MyShader.Compile(MyShader.get_gl_type());
  void CreateVertexShader(const char* shader_source);
  void CreateFragmentShader(const char* shader_source);

 private:
  char* shader_source_;
  GLuint shader_;
  bool vaild_;
  char info_log_[4098];
  GLenum type_;
};

}  //  namespace render

#endif
