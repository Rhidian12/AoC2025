#pragma once

#include <string_view>

enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Success,
    Error
};

struct Logger
{
    LogLevel logLevel = LogLevel::Info;

    void LogTrace(std::string_view message) const;
    void LogDebug(std::string_view message) const;
    void LogInfo(std::string_view message) const;
    void LogSuccess(std::string_view message) const;
    void LogError(std::string_view message) const;
};