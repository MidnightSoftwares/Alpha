#pragma once

#include <string>

class StringConverter final
{
public:
    StringConverter() = delete;

public:
    static std::wstring ToWide(const std::string& str);
};
