#ifndef SERIALIZE_PACK_H
#define SERIALIZE_PACK_H

#include "serialize_header.h"
#include <vector>

namespace serialize {

template <typename T>
SerializableHeaderField CreateHeaderField(const T& field) {
  SerializableHeaderField header_field;
  header_field.element_size = sizeof(T);
  header_field.size = 1;
  return header_field;
};

template <typename T>
SerializableHeaderField CreateHeaderField(const std::vector<T>& field) {
  SerializableHeaderField header_field;
  header_field.element_size = sizeof(T);
  header_field.size = field.size();
  return header_field;
};

template <typename T>
bool Pack(std::FILE* file, const SerializableHeaderField& header, T* field) {
  std::fwrite(reinterpret_cast<void*>(field), header.element_size, header.size,
              file);
}

template <typename T>
bool Pack(std::FILE* file,
          const SerializableHeaderField& header,
          std::vector<T>* field) {
  std::fwrite(reinterpret_cast<void*>(field->data()), header.element_size,
              header.size, file);
}

template <typename T>
bool UnPack(std::FILE* file, const SerializableHeaderField& header, T* field) {
  std::fread(reinterpret_cast<void*>(field), header.element_size, header.size,
             file);
}

template <typename T>
bool UnPack(std::FILE* file,
            const SerializableHeaderField& header,
            std::vector<T>* field) {
  field->reserve(header.size);
  for (unsigned int i = 0; i < header.size; ++i) {
    T element;
    std::fread(reinterpret_cast<void*>(&element), header.element_size, 1, file);
    field->push_back(element);
  }
}


}  // serialize

#endif  // SERIALIZE_PACK_H
