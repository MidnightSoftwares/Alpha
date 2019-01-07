#pragma once

#include "String.h"

#define W_FILE WIDE1(__FILE__)

class FileSystemUtils final
{
public:
    FileSystemUtils() = delete;

public:
    static const std::wstring& ExecutableDirectory();
};
