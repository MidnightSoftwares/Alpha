#pragma once

#include <string>

class FileSystemUtils final
{
public:
    FileSystemUtils() = delete;

public:
    static const std::wstring& ExecutableDirectory();
};
