#ifndef OBSIDIAN_LOG_H_
#define OBSIDIAN_LOG_H_

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "singleton.h"

namespace util {

// Enums useed in conjuction with Loggers << operator.
// For example, to log current date/time do the following:
//  Logger log;
//  log << Type::kDateTime << " <- this is the time. ";
enum LoggerType {
    kLogDateTime
};

// A class that enables basic logging to simple text.
// It should be extended in future.
class Logger {
 public:
  // Constructor opens the file, and destructor closes it.
  Logger();

  ~Logger();

  // Stream to file.
  void Log(const char* msg);

  void Log(int msg);

  void Log(float msg);

  // Getters and setters.
  FILE* get_file() { return file_; }

  // operators.
  Logger& operator<<(const char* rhs);

  Logger& operator<<(int rhs);

  Logger& operator<<(float rhs);

  Logger& operator<<(LoggerType rhs);

  Logger& operator<<(size_t rhs);

 private:
  FILE* file_;
};

class HasLog {
 public:
  HasLog();
  Logger& log;
};

} //  namespace util
#endif
