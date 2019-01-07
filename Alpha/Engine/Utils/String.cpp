#include "String.h"

std::wstring StringUtils::ToWide(const std::string& str)
{
    return {str.begin(), str.end()};
}
