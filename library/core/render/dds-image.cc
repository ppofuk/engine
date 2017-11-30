#include "core/render/dds-image.h"
namespace render {

// TODO(ppofuk): extend support for custom memory buffer (malloc)
// TODO(ppofuk): extend support for noncompressed textures
bool DDSImage::Load(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (file == nullptr) {
    util::Log << util::kLogDateTime << ": DDSImage::Load -> Unable to open "
              << filename << ".\n";
    return false;
  }

  u32 magic_number;
  fread(reinterpret_cast<void*>(&magic_number), 1, sizeof(u32), file);
  // magic_number must containt the four character code value 'DDS '
  // (0x20534444).
  if (magic_number != 0x20534444) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": wrong magic number for " << filename << "\n";
    return false;
  }

  fread(reinterpret_cast<void*>(&dds_header_), sizeof(render::DDSHeader), 1,
        file);

  if (dds_header_.size != 124) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": warning, DDS header size is not 124 in " << filename
              << "\n";
  }

  if (dds_header_.pixel_format.size != 32) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": warning, DDS headers pixel format size is not 32 in "
              << filename << "\n";
  }

  if (dds_header_.pixel_format.four_cc == MAKE_FOUR_CC('D', 'X', '1', '0')) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": DXT10 is not supported; in " << filename << "\n";
    return false;
  }

  if (four_cc() == DDSImage::FourCC::kUknownType) {
    util::Log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__
              << ": DXT1, DXT3, DXT5 are currently supported. " << filename
              << " is none.\n";
    return false;
  }

  data_size_ = mip_map_count() > 1 ? linear_size() * 2 : linear_size();
  data_ = malloc(data_size_);

  // TODO: check data_ != nullptr
  fread(data_, 1, data_size_, file);

  fclose(file);
  return true;
}

void DDSImage::Destroy() {
  if (data_) {
    free(data_);
  }
}

DDSImage::FourCC DDSImage::four_cc() const {
  if (dds_header_.pixel_format.four_cc == DDSImage::FourCC::kDXT1) {
    return DDSImage::FourCC::kDXT1;
  }

  if (dds_header_.pixel_format.four_cc == DDSImage::FourCC::kDXT3) {
    return DDSImage::FourCC::kDXT3;
  }

  if (dds_header_.pixel_format.four_cc == DDSImage::FourCC::kDXT5) {
    return DDSImage::FourCC::kDXT5;
  }

  return DDSImage::FourCC::kUknownType;
}

}  // namespace render
