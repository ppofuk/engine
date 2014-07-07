#ifndef OBSIDIAN_LOG_H_
#define OBSIDIAN_LOG_H_

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "singleton.h"

namespace util {

// Enums useed in conjunction with Loggers << operator.
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
  void Log(const char* msg) const;

  void Log(int msg) const;

  void Log(float msg) const;

  // Getters and setters.
  FILE* get_file() const { return file_; }

  template<typename Type> char* Address(Type& var) const {
    static char address[18];
    sprintf(address, "%p", (void*)var);
    return address;
  }

  // operators.
  const Logger& operator<<(const char* rhs) const;

  const Logger& operator<<(int rhs) const;

  const Logger& operator<<(float rhs) const;

  const Logger& operator<<(LoggerType rhs) const;

  const Logger& operator<<(size_t rhs) const;

 private:
  FILE* file_;
};

// Best way to safely use the Log class is by inheriting HasLog class to your
// class.
// For example:
// class MyClass : public HasLog {
//  public:
//   MyClass() { log << "Hello from MyClass()\n"; }
// }
class HasLog {
 public:
  HasLog();
  const Logger& log;
};

} //  namespace util
#endif
