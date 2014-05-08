#include "logger.h"
#include "static-compile-options.h"

namespace util {

Logger::Logger() { file_ = fopen(log_path_file, "a"); }

Logger::~Logger() { fclose(file_); }

void Logger::Log(const char* msg) {
  fputs(msg, file_);
  fputs(msg, stdout);
  fflush(file_);
}

void Logger::Log(int msg) {
  fprintf(file_, "%d", msg);
  fprintf(stdout, "%d", msg);
}

void Logger::Log(float msg) {
  fprintf(file_, "%f", msg);
  fprintf(stdout, "%f", msg);
}

Logger& Logger::operator<<(const char* rhs) {
  Log(rhs);
  return *this;
}

Logger& Logger::operator<<(int rhs) {
  Log(rhs);
  return *this;
}

Logger& Logger::operator<<(float rhs) {
  Log(rhs);
  return *this;
}

Logger& Logger::operator<<(size_t rhs) {
  Log((int)rhs);
  return *this;
}

Logger& Logger::operator<<(LoggerType rhs) {
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
