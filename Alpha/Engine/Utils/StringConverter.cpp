#include "StringConverter.h"

std::wstring StringConverter::ToWide(const std::string& str)
{
    return {str.begin(), str.end()};
}
