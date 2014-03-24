#include "logger.h"

namespace util {

Logger::Logger(void) {
  file_ = fopen("log.txt", "a");
}

Logger::~Logger(void) {
  fclose(file_);
}

void Logger::Log(const char* msg) {
  FILE* file = Singleton<Logger>::Instance().get_file();
  fputs(msg, file);
  fputs(msg, stdout);
  fflush(file);
}

void Logger::Log(int msg) {
  FILE* file = Singleton<Logger>::Instance().get_file();
  fprintf(file, "%d", msg);
  fprintf(stdout, "%d", msg);
}

void Logger::Log(float msg) {
  FILE* file = Singleton<Logger>::Instance().get_file();
  fprintf(file, "%f", msg);
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

} // namespace util
