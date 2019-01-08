#pragma once

#include "Debug.h"
#include "FileSystem.h"

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
    static void Log(LogLevel level, const std::wstring& message,
        const wchar_t* file, unsigned long line);
};

#define LOG_INFO(message) ::Logger::Log(::LogLevel::Info, message, W_FILE, __LINE__)
#define LOG_WARNING(message) ::Logger::Log(::LogLevel::Warning, message, W_FILE, __LINE__)
#define LOG_ERROR(message) ::Logger::Log(::LogLevel::Error, message, W_FILE, __LINE__)

#define LOG_DEBUG(message) DEBUG_ONLY(LOG_INFO(message))
