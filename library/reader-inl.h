// Copyright (C) 2014 Petar Pofuk. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef OBSIDIAN_READER_H_
#define OBSIDIAN_READER_H_

#include <string.h>
#include <stdio.h>
#include "singleton.h"
#include "logger.h"

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif

namespace util {

// A simple reader class.
// You must set a pointer to memory with set_data().
// Notice, that when using Read(binary = false), a 0 is appended to
// data[file_size].
template <typename Type>
class Reader {
 public:
  Reader() : file_(0), file_size_(0), data_(0) {}

  void set_path(const char* path) { strcpy(path_, path); }
  char* get_path() const { return path_; }
  Type* get_data() const { return data_; }
  void set_data(Type* data) { data_ = data; }

  void Close() {
    if (file_) {
      fclose(file_);
      file_ = 0;
      file_size_ = 0;
    }
  }

  void Open(bool binary = true) {
    Close();
    if (binary)
      file_ = fopen(path_, "rb");
    else
      file_ = fopen(path_, "r");

    if (!file_) {
      Logger& log = Singleton<Logger>::Instance();
      log << kLogDateTime << ": Cannot open " << path_ << "!\n";
      return;
    }
  }

  Type* Read(bool binary = false) {
    if (!file_) {
      return NULL;
    }

    get_file_size();
    if (!data_)
      return NULL;

    if (fread(data_,
              sizeof(Type),
              file_size_ / sizeof(Type),
              file_) != file_size_) {
      Logger& log = Singleton<Logger>::Instance();
      log << kLogDateTime << ": Read error on " << path_ << "!\n";
    }

    if (!binary) {
      data_[file_size_] = 0; // Presume we are using chars
    }

    return data_;
  }

  size_t get_file_size() {
    fseek(file_, 0, SEEK_END);
    file_size_ = ftell(file_);
    rewind(file_);
    return file_size_;
  }

  static void Chdir(const char* path) {
    chdir(path);
  }

  static char* Cwd() {
    static char path[2048];
    getcwd(path, 2048);
    return path;
  }

  static bool IsReadable(const char* path) {
    FILE* file = fopen(path, "r");
    if (file) {
      fclose(file);
      return true;
    } else {
      return false;
    }
  }

 private:
  char path_[2048];
  FILE* file_;
  size_t file_size_;
  Type* data_;
};

} // namespace util

#endif // OBSIDIAN_READER_H_
