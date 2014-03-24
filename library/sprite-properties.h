// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OBSIDIAN_SPRITE_PROPERTIES_H_
#define OBSIDIAN_SPRITE_PROPERTIES_H_
#include "basic-types.h"
#include "texture-abstract.h"

namespace resource {
class Sprite;

class SpriteProperties {
 private:
  u16 sheet_rows_, sheet_columns_;

 public:
  friend class Sprite;

  SpriteProperties();

  f32 x, y, z, width, height;
  f32 opacity;
  f32 u1, u2, v1, v2;
  f32 rx, ry, rz;
  f32 scale;
  bool visiable;

  // Copies the values from |texture.width_| and |texture.height_| to
  // corresponding |width| and |height|
  void Set(TextureAbstract* texture);

  // Applies |u1|, |u2|, |v1| and |v2| based on given |row| and |column|.
  // |sheet_columns_| and |sheet_rows_| define sheet splits.
  void SetSheetPosition(u16 row, u16 column, bool inverse = false);

  // Simple setters and getters.
  u16 get_sheet_rows() const { return sheet_rows_; }
  u16 get_sheet_columns() const { return sheet_columns_; }
  void set_sheet_rows(u16 sheet_rows) {
    sheet_rows_ = sheet_rows;
  }
  void set_sheet_columns(u16 sheet_columns) {
    sheet_columns_ = sheet_columns;
  }
};

}  // namespace resource

#endif  // OBSIDIAN_SPRITE_PROPERTIES_H_
