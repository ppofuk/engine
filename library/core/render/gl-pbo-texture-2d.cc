#include "gl-pbo-texture-2d.h"

namespace render {

void GLPixelBufferObjectTexture::Generate(f32 width,
                                          f32 height,
                                          GLenum format) {
  width_ = width;
  height_ = height;
  first_ = true;
  format_ = format; 

  // TODO(ppofuk): Add GL_DYNAMIC_DRAW as arg.
  // TODO(ppofuk): Add width * height * mult, where mult is based on format
  pbo_.Create(GL_PIXEL_UNPACK_BUFFER, GL_STREAM_DRAW, nullptr,
              width * height * 4);

  pbo_.Bind();

  if (!gl_texture_)
    glGenTextures(1, &gl_texture_);

  glBindTexture(GL_TEXTURE_2D, gl_texture_);
  ApplyFilters();

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format_,
                  GL_UNSIGNED_BYTE, 0);
  // first_ = false;
}

void* GLPixelBufferObjectTexture::Map(GLenum access) {
  Bind();
  pbo_.Bind();

  return pbo_.Map(access);
}

void GLPixelBufferObjectTexture::Unmap() {
  pbo_.Unmap();
  pbo_.Bind();
  Bind();
  ApplyFilters();

  if (first_) {
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width_, height_, 0, format_,
                 GL_UNSIGNED_BYTE, nullptr);
    // first_ = false;
  } else {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_,
                    GL_UNSIGNED_BYTE, nullptr);
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

}  // namespace render
