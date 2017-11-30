#ifndef CORE_RENDER_DDS_IMAGE_H_
#define CORE_RENDER_DDS_IMAGE_H_

#include "core/basic-types.h"
#include "core/reader-inl.h"
#include "core/logger.h"
#include "core/make-four-cc-inl.h"
#include <string>
#include <memory>
#include <stdlib.h>

namespace render {

struct DDSPixelFormat {
  u32 size;
  u32 flags;
  u32 four_cc;
  u32 rgb_bit_count;
  u32 r_bit_mask;
  u32 g_bit_mask;
  u32 b_bit_mask;
  u32 a_bit_mask;
};

// Used with DDSPixelFormat::flags.
// Values which indicate what type of data is in the surface.
enum DDSPixelFormatFlags {
  kDDSPixelFormatAlphaPixels = 0x1,  // Texture contains alpha data
  kDDSPixelFormatAlpha = 0x2,  // Used in some older DDS files for alpha channel
                               // only uncompressed data (rgb_bit_count contains
                               // the alpha channel bitcount; a_bit_mask
                               // contains valid data)
  kDDSPixelFormatFourCC =
      0x4,  // Texture contains compressed RGB data; four_cc contains valid data
  kDDSPixelFormatRGB = 0x40,  // Texture contains uncompressed RGB data;
                              // rgb_bit_count and RGB masks (r_bit_mask,
                              // g_bit_mask, b_bit_mask) contain valid data
  kDDSPixelFormatYUV =
      0x200,  // Used in some older DDS files for YUV uncompressed data
              // (rgb_bit_count contains YUV bit count; r_bit_mask contains Y
              // mask, g_bit_mask contains U mask, b_bit_mask contains V mask)
  kDDSPixelFormatLuminance =
      0x20000  // Used in some older DDS files for single channel color
               // uncompressed data (rgb_bit_count contains the luminance
               // channel bit count; r_bit_mask contains the channel mask)
};

struct DDSHeader {
  u32 size;
  u32 flags;
  u32 height;
  u32 width;
  u32 pitch_or_linear_size;
  u32 depth;
  u32 mip_map_count;
  u32 reserved[11];
  DDSPixelFormat pixel_format;
  u32 caps;
  u32 caps2;
  u32 caps3;
  u32 caps4;
  u32 reserved2;
};

// Used with DDSHeader::flags.
// Flags to indicate which members contain valid data.
enum DDSHeaderFlags {
  kDDSHeaderCaps = 0x1,    // Required in every .dds file
  kDDSHeaderHeight = 0x2,  // Required in every .dds file
  kDDSHeaderWidth = 0x4,   // Required in every .dds file
  kDDSHeaderPitch =
      0x8,  // Required when pitch is provided for an uncompressed texture
  kDDSHeaderPixelFormat = 0x1000,   // Required in every .dds file
  kDDSHeaderMipMapCount = 0x20000,  // Required in mipmapped texture
  kDDSHeaderLinearSize =
      0x80000,  // Required when pitch is provided for compressed texture
  kDDSHeaderDepth = 0x800000  // Required in a depth texture
};


class DDSImage {
 public:
  enum FourCC {
    kDXT1 = 0x31545844,
    kDXT3 = 0x33545844,
    kDXT5 = 0x35545844,
    kUknownType
  };

  // TODO: memory management extension
  bool Load(const char* filename);

  void Destroy();

  // Surface height in pixels
  u32 width() const { return dds_header_.width; }

  // Surface width in pixels
  u32 height() const { return dds_header_.height; }

  // The pitch or number of bytes per scan line in an uncompressed texture; the
  // total number of bytes in the top level texture for a compressed texture.
  u32 linear_size() const { return dds_header_.pitch_or_linear_size; }

  // Number of mipmap levels
  u32 mip_map_count() const { return dds_header_.mip_map_count; }
  const DDSHeader& header() const { return dds_header_; }
  FourCC four_cc() const;

  bool contains_alpha_data() const {
    return dds_header_.pixel_format.flags &
           DDSPixelFormatFlags::kDDSPixelFormatAlpha;
  }

  void* data() const { return data_; }
  size_t data_size() const { return data_size_; }

 private:
  DDSHeader dds_header_;
  void* data_ = nullptr;
  size_t data_size_ = 0;
};

}  // namespace render

#endif  // CORE_RENDER_DDS_IMAGE_H_
