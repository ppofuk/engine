// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_PROGRAM_H_
#define OBSIDIAN_GL_PROGRAM_H_

#include "gl-shader.h"
#include "gl-uniform.h"
#include "gl-attribute.h"
#include <string.h>

namespace render {

// Abstraction for OpenGL program.
class GLProgram {
 public:
  GLProgram();

  // Should be called before anything is done with program.
  void Init();

  // Attach a specified shader to program.
  // If |delete_shader| is true, then shader.Delete() will be called after
  // attachment.
  void AttachShader(GLShader& shader, bool delete_shader = false);

  // Remove shader from program.
  void DetachShader(GLShader& shader);

  // Link program with all shaders. You should check is_vaild() after and if it
  // yields false you can check the error with InfoLog.
  // Delete() should be also called if is_vaild() yield false.
  void Link();

  // Delete() a program. This should be always called when we don't need out
  // program anymore.
  void Delete();

  // Returns a pointer to string describing link error in program.
  // Use it if is_vaild() yields false.
  char* InfoLog();

  // Use the program.
  void Use();

  // Creates and returns an uniform variable for this program.
  template<typename Type>
  GLUniform<Type> Uniform(Type& attribute, const char* name);

  // Creates and returns an attribute variable for this program.
  template<typename Type>
  GLAttribute<Type> Attribute(Type& attribute, const char* name);

  bool is_vaild() const { return vaild_; }
  GLuint get_program() const { return program_; }

 private:
  GLuint program_;
  bool vaild_;
  char info_log_[1024];
};

} //  namespace render

#endif
