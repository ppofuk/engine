#include "logger.h"
#include "static-compile-options.h"

namespace util {

Logger::Logger() { file_ = fopen(log_path_file, "a"); }

Logger::~Logger() { fclose(file_); }

void Logger::Log(const char* msg) const {
  fputs(msg, file_);
  fputs(msg, stdout);
  fflush(file_);
}

void Logger::Log(int msg) const {
  fprintf(file_, "%d", msg);
  fprintf(stdout, "%d", msg);
}

void Logger::Log(float msg) const {
  fprintf(file_, "%f", msg);
  fprintf(stdout, "%f", msg);
}

const Logger& Logger::operator<<(const char* rhs) const {
  Log(rhs);
  return *this;
}

const Logger& Logger::operator<<(int rhs) const {
  Log(rhs);
  return *this;
}

const Logger& Logger::operator<<(float rhs) const {
  Log(rhs);
  return *this;
}

const Logger& Logger::operator<<(size_t rhs) const {
  Log((int)rhs);
  return *this;
}

const Logger& Logger::operator<<(LoggerType rhs) const {
  if (rhs == kLogDateTime) {
    char time_str[48];
    time_t raw_time;
    time(&raw_time);

    strftime(time_str, 48, "%c", localtime(&raw_time));
    Log(time_str);
  }

  return *this;
}

HasLog::HasLog() : log(Singleton<Logger>::Instance()) {}

}  // namespace util
