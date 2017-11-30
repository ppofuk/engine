#ifndef CORE_RENDER_GL_TEXTURE_H_
#define CORE_RENDER_GL_TEXTURE_H_

#include "glad/glad.h"
#include "core/render/gl-types.h"
#include "core/render/gl-buffer.h"
#include "core/render/dds-image.h"
#include <algorithm>

namespace render {

class GLTexture {
 public:
  void Bind();
  void Destroy();

  bool Init(const DDSImage& image);

  GLBuffer<void>* mutable_pixel_buffer_object() {
    return &pixel_buffer_object_;
  }

  bool is_init() const { return is_init_; }
  GLuint texture() const { return texture_; }
  GLuint format() const { return format_; }
  GLenum texture_type() const { return texture_type_; }

 private:
  bool is_init_ = false;
  GLuint texture_ = 0;
  GLuint format_ = 0;
  GLenum texture_type_ = GL_TEXTURE_2D;
  mutable GLBuffer<void> pixel_buffer_object_;
};


}  // namespace render

#endif  // #ifndef CORE_RENDER_GL_TEXTURE_H_
