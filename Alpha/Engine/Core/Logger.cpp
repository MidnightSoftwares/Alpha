#include "Logger.h"
#include "../Utils/String.h"

//#include <Shlwapi.h>

#include <sstream>

void Logger::Log(LogLevel level, const std::string& message,
    const char* file, unsigned long line)
{
    return Log(level, StringUtils::ToWide(message), file, line);
}

void Logger::Log(LogLevel level, const std::wstring& message,
    const char* /* file */, unsigned long /* line */)
{
    const auto levelStr = [level]() -> std::wstring
    {
        switch (level)
        {
            case LogLevel::Info:
                return L"INFO";
            case LogLevel::Warning:
                return L"WARNING";
            case LogLevel::Error:
                return L"ERROR";
            default:
                return L"UNKNOWN";
        }
    }();

    //const auto fileStr = [file, line]() -> std::wstring
    //{
    //    return StringUtils::ToWide(PathFindFileNameA(file))
    //        + L'(' + StringUtils::ToWide(std::to_string(line)) + L')';
    //}();

    const auto logStr = [&levelStr, &message]() -> std::wstring
    {
        std::wstringstream ss;
        ss << L'[' << levelStr << L"] " << message << L'\n';
        return ss.str();
    }();

    OutputDebugStringW(logStr.c_str());
}
