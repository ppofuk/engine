#ifndef SERIALIZE_HEADER_H
#define SERIALIZE_HEADER_H

#include <array>
#include <cstdio>

namespace serialize {
struct SerializableHeaderField {
  unsigned int element_size = 0;
  unsigned int size = 0;
};

class SerializableHeaderBase {
 public:
  virtual unsigned int number_of_fields() const = 0;
  virtual SerializableHeaderField* mutable_fields() = 0;
  virtual bool DeserializeHeader(std::FILE* file) = 0;
  virtual bool SerializeHeader(std::FILE* file) = 0; 
};

template <typename... R>
class SerializableHeader : public SerializableHeaderBase {
 public:
  unsigned int number_of_fields() const { return number_of_fields_; }
  SerializableHeaderField* mutable_fields() { return fields_.data(); }

  bool DeserializeHeader(std::FILE* file) {
    std::fread(reinterpret_cast<void*>(&number_of_fields_),
               sizeof(unsigned int), 1, file);
    std::fread(reinterpret_cast<void*>(fields_.data()),
               sizeof(SerializableHeaderField), number_of_fields_, file);
    return true;
  }

  bool SerializeHeader(std::FILE* file) {
    std::fwrite(reinterpret_cast<void*>(&number_of_fields_),
               sizeof(unsigned int), 1, file);
    std::fwrite(reinterpret_cast<void*>(fields_.data()),
               sizeof(SerializableHeaderField), number_of_fields_, file);
    return true;
  }

 private:
  unsigned int number_of_fields_ = sizeof...(R);
  std::array<SerializableHeaderField, sizeof...(R)> fields_;
};


}  // namespace serialize

#endif  // SERIALIZE_HEADER_H
