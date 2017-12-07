#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#ifndef _NDEBUG
#include <iostream>
#include <typeinfo>
#endif  // _NDEBUG

#include <array>
#include <cstdio>
#include <vector>

#include "serialize_header.h"
#include "serialize_pack.h"

namespace serialize {

template <typename BaseType>
class Serializable {
 public:
  template <typename... R>
  bool Serialize(const char* filename, R... packs) {
    std::FILE* file = std::fopen(filename, "wb");
    if (!file) {
      return false;
    }

    SerializableHeader<R...> header;
    SerializeHeader(&header, 0, packs...);

    // TODO(ppofuk): check if fwrite returns sizeof(header)
    // std::fwrite(reinterpret_cast<void*>(&header), sizeof(header), 1, file);
    header.SerializeHeader(file); 

    SerializePack(&header, 0, file, packs...);
    // TODO: Write every pack
    std::fclose(file);
    return true; 
  }

  template <typename... R>
  bool Deserialize(const char* filename, R&... packs) {
    std::FILE* file = std::fopen(filename, "rb");
    if (!file) {
      return false;
    }

    SerializableHeader<R...> header;
    // TODO(ppofuk): check if fwrite returns sizeof(header)
    // std::fread(reinterpret_cast<void*>(&header), sizeof(header), 1, file);
    header.DeserializeHeader(file); 

    DeserializePack(&header, 0, file, packs...);
    // TODO: Write every pack
    std::fclose(file);
    return true; 
  }

 protected:
  template <typename T, typename... R>
  bool SerializeHeader(SerializableHeaderBase* header,
                       unsigned int offset,
                       T pack,
                       R... packs) {
    SerializeHeader(header, offset, pack);
    SerializeHeader(header, offset + 1, packs...);
  }

  template <typename T>
  bool SerializeHeader(SerializableHeaderBase* header,
                       unsigned int offset,
                       T pack) {
    header->mutable_fields()[offset] = CreateHeaderField(pack);
  }

  template <typename T, typename... R>
  bool SerializePack(SerializableHeaderBase* header,
                     unsigned int offset,
                     std::FILE* file,
                     T pack,
                     R... packs) {
    SerializePack(header, offset, file, pack);
    SerializePack(header, offset + 1, file, packs...);
  }

  template <typename T>
  bool SerializePack(SerializableHeaderBase* header,
                     unsigned int offset,
                     std::FILE* file,
                     T pack) {
    Pack(file, header->mutable_fields()[offset], &pack);
  }

  template <typename T, typename... R>
  bool DeserializePack(SerializableHeaderBase* header,
                       unsigned int offset,
                       std::FILE* file,
                       T& pack,
                       R&... packs) {
    DeserializePack(header, offset, file, pack);
    DeserializePack(header, offset + 1, file, packs...);
  }

  template <typename T>
  bool DeserializePack(SerializableHeaderBase* header,
                       unsigned int offset,
                       std::FILE* file,
                       T& pack) {
    UnPack(file, header->mutable_fields()[offset], &pack);
  }


 private:
};

}  // namespace serialize

#endif  // SERIALIZE_H_
