// Copyright (C) 2017 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_CEF_UI_SHADER_H_
#define OBSIDIAN_CEF_UI_SHADER_H_

#include "render/gl-shader.h"
#include "render/gl-buffer.h"
#include "render/gl-attribute.h"
#include "render/gl-pbo-texture-2d.h"
#include "glm/glm.hpp"

namespace render {
namespace cef {

class CefUIShader : public GLShader {
 public:
  GLPixelBufferObjectTexture* get_texture() { return &texture_; }
  void Update(i32 width, i32 height);
  void Render();
  
 private:
  void OnInit();
  void OnDestroy();
  void PostCompile();
  void PassAndEnableAttributes();
  void DisableAttributes(); 

  GLBuffer<GLfloat> positions_buffer_;
  GLBuffer<GLushort> indices_buffer_;
  GLBuffer<GLfloat> texcoords_buffer_;

  GLAttribute<GLBuffer<GLfloat> > positions_attrib_;
  GLAttribute<GLBuffer<GLfloat> > texcoords_attrib_;

  GLUniform matrix_;
  GLPixelBufferObjectTexture texture_; 

  static GLfloat positions_[]; 
  static GLushort indices_[]; 
  static GLfloat texcoords_[]; 
};


} // namespace cef
} // namespace render

#endif // ifndef OBSIDIAN_CEF_UI_SHADER_H_
