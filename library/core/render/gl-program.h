// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_GL_PROGRAM_H_
#define OBSIDIAN_GL_PROGRAM_H_

#include "gl-shader-object.h"
#include <string.h>

namespace render {

// Abstraction for OpenGL program.
class GLProgram {
 public:
  GLProgram();

  // Should be called before anything is done with program.
  void Init();

  // Attach a specified shader to program.
  void AttachShader(const GLShaderObject& shader);

  // Remove shader from program.
  void DetachShader(const GLShaderObject& shader);

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

  bool is_vaild() const { return vaild_; }
  GLuint get_program() const { return program_; }

 private:
  GLuint program_;
  bool vaild_;
  char info_log_[1024];
};

} //  namespace render

#endif
