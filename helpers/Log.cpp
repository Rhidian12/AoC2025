#include "Log.h"

#include <iostream>

void Logger::LogInfo(std::string_view message) const
{
  if (logLevel > LogLevel::Info) return;
  std::cout << message << std::endl;
}

void Logger::LogTrace(std::string_view message) const
{
    if (logLevel > LogLevel::Trace) return;
    std::cout << message << std::endl;
}

void Logger::LogDebug(std::string_view message) const
{
    if (logLevel > LogLevel::Debug) return;
    std::cout << message << std::endl;
}

void Logger::LogSuccess(std::string_view message) const
{
    if (logLevel > LogLevel::Success) return;
    std::cout << "\033[42mSuccess: " << message << "\033[m" << std::endl;
}

void Logger::LogError(std::string_view message) const
{
    if (logLevel > LogLevel::Error) return;
  std::cerr << "\033[41mError: " << message << "\033[m" << std::endl;
}