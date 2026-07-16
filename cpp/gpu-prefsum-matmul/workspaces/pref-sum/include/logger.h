#pragma once
#include <cstdio>

namespace logg {

enum LogLevel {
  INFO,
  ERROR,
};

extern LogLevel globalLogLevel;

template <typename... Args>
static void log(LogLevel logLevel = INFO, Args... args) {
  if (logLevel < globalLogLevel) {
    return;
  }
  // ((std::cout << args << " "), ...);
  // std::cout << std::endl;
  std::printf(args...);
}

template <typename... Args>
static void log(Args... args) {
  if (INFO < globalLogLevel) {
    return;
  }

  std::printf(args...);
  // ((std::cout << args << " "), ...);
  // std::cout << std::endl;
}

} // namespace logg
