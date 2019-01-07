#pragma once

#include <string>

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)

class StringUtils final
{
public:
    StringUtils() = delete;

public:
    static std::wstring ToWide(const std::string& str);
};
