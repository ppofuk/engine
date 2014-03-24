// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "texture-png.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

namespace resource {

bool TexturePng::Load(const char* file) {
  // This code should be refactored to texture_png() method, or something
  png_structp png_pointer;
  png_infop png_info_pointer;
  unsigned int sig_read = 0;

  FILE* file_pointer;

  if ((file_pointer = fopen(file, "rb")) == NULL)
    return false;

  // Some init crap from tutorial.
  png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_pointer == NULL) {
    fclose(file_pointer);
    return false;
  }

  png_info_pointer = png_create_info_struct(png_pointer);
  if (png_info_pointer == NULL) {
    fclose(file_pointer);
    png_destroy_read_struct(&png_pointer, (png_infopp)NULL, (png_infopp)NULL);
    return false;
  }

  if (setjmp(png_jmpbuf(png_pointer))) {
    png_destroy_info_struct(png_pointer, &png_info_pointer);
    fclose(file_pointer);
    return false;
  }

  // Setup the output control to standard C streams
  // Todo(pero): Maybe some class methods hook to setup object output control?
  png_init_io(png_pointer, file_pointer);

  png_set_sig_bytes(png_pointer, sig_read);
  png_read_png(
      png_pointer,
      png_info_pointer,
      PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
      (png_voidp)NULL);

  this->width_ = png_get_image_width(png_pointer, png_info_pointer);
  this->height_ = png_get_image_height(png_pointer, png_info_pointer);
  png_get_color_type(png_pointer, png_info_pointer);

  if (png_get_color_type(png_pointer, png_info_pointer) ==
      PNG_COLOR_TYPE_RGBA) {
    has_alpha_ = true;
    gl_alpha_ = 4;
  } else if (png_get_color_type(png_pointer, png_info_pointer) ==
             PNG_COLOR_TYPE_RGB) {
    has_alpha_ = false;
    gl_alpha_ = 3;
  } else {
    // TODO(pero): error handler implementation
    fprintf(stderr,
            "TexturePng at %p, ::Load, reports color type is not supported\n",
            (void*)this);
    fclose(file_pointer);
    // Check for png_ structs deallocation
    return false;
  }

  unsigned int row_bytes = png_get_rowbytes(png_pointer, png_info_pointer);
  // TODO(pero): Replace malloc with faster allocation method.
  this->texture_data_ = (unsigned char*)malloc(row_bytes * this->height_);
  png_bytepp row_pointers = png_get_rows(png_pointer, png_info_pointer);

  for (int i = 0; i < this->height_; ++i)
    memcpy((this->texture_data_ + (row_bytes * (this->height_ - 1 - i))),
           row_pointers[i],
           row_bytes);

  png_destroy_read_struct(&png_pointer, &png_info_pointer, (png_infopp)NULL);
  return true;
}

void TexturePng::Destroy() {
  if (texture_data_) {
    free(texture_data_);
    texture_data_ = 0;
  }
}

}  // namespace render
