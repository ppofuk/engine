#include "core/render/gl-texture.h"

namespace render {
bool GLTexture::Init(const DDSImage& image) {
  if (is_init()) {
    return false;
  }

  if (image.four_cc() == DDSImage::FourCC::kUknownType) {
    return false;
  }

  // Clear all errors before.
  while (glGetError() != GL_NO_ERROR)
    ;

  glGenTextures(1, &texture_);
  Bind();

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": " << (const char*)gluErrorString(err) << "\n";
  }

  u32 block_size = 0;
  if (image.four_cc() == DDSImage::FourCC::kDXT1) {
    format_ = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    block_size = 8;
  }

  if (image.four_cc() == DDSImage::FourCC::kDXT3) {
    format_ = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    block_size = 16;
  }

  if (image.four_cc() == DDSImage::FourCC::kDXT5) {
    format_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    block_size = 16;
  }

  u32 width = image.width();
  u32 height = image.height();

  for (u32 level = 0, offset = 0;
       level < image.mip_map_count() && (width || height); ++level) {
    u32 size = std::max((u32)1, ((width + 3) / 4)) *
               std::max((u32)1, ((height + 3) / 4)) * block_size;

    glCompressedTexImage2D(GL_TEXTURE_2D, level, format_,
                           std::max((u32)1, width), std::max((u32)1, height), 0,
                           size, (char*)image.data() + offset);

    err = glGetError();
    if (err != GL_NO_ERROR) {
      util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
                << ": " << (const char*)gluErrorString(err) << "\n";
    }

    offset += size;
    width /= 2;
    height /= 2;
  }

  is_init_ = true;
  return true;
}

void GLTexture::Destroy() {
  if (is_init()) {
    pixel_buffer_object_.Destroy();
    GLuint delete_texture[] = {texture_};
    glDeleteTextures(1, delete_texture);
    is_init_ = false;
  }
}

void GLTexture::Bind() {
  glBindTexture(texture_type(), texture_);
}

}  // namespace render
