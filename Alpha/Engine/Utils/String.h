#pragma once

#include <string>

class StringUtils final
{
public:
    StringUtils() = delete;

public:
    static std::wstring ToWide(const std::string& str);
};
