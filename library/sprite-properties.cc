// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "sprite-properties.h"
namespace resource {

SpriteProperties::SpriteProperties()
    : sheet_rows_(0),
      sheet_columns_(0),
      x(0),
      y(0),
      z(0),
      width(0),
      height(0),
      opacity(1),
      u1(0),
      u2(1),
      v1(0),
      v2(1),
      rx(0),
      ry(0),
      rz(0),
      scale(1),
      visiable(true) {}

void SpriteProperties::Set(TextureAbstract* texture) {
  width = texture->get_width();
  height = texture->get_height();
}

void SpriteProperties::SetSheetPosition(u16 row, u16 column, bool inverse) {
  if (inverse) {
    u2 = (f32)column / sheet_columns_;
    u1 = (f32)(column + 1) / sheet_columns_;
  } else {
    u1 = (f32)column / sheet_columns_;
    u2 = (f32)(column + 1) / sheet_columns_;
  }
  v1 = 1 - ((f32)row / sheet_rows_);
  v2 = 1 - ((f32)(row + 1)) / sheet_rows_;
}

}  // namespace resource
