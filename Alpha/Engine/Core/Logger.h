#pragma once

#include "../Utils/Debug.h"

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Logger final
{
public:
    Logger() = delete;

public:
    static void Log(LogLevel level, const std::string& message,
        const char* file, unsigned long line);
    static void Log(LogLevel level, const std::wstring& message,
        const char* file, unsigned long line);
};

#define LOG_INFO(message) ::Logger::Log(::LogLevel::Info, message, __FILE__, __LINE__)
#define LOG_WARNING(message) ::Logger::Log(::LogLevel::Warning, message, __FILE__, __LINE__)
#define LOG_ERROR(message) ::Logger::Log(::LogLevel::Error, message, __FILE__, __LINE__)

#define LOG_DEBUG(message) DEBUG_ONLY(LOG_INFO(message))
