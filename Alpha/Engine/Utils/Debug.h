#pragma once

#include <Windows.h>

#include <string>

#ifdef _DEBUG
#    define DEBUG_ONLY(x) x
#else
#    define DEBUG_ONLY(x)
#endif

#define DEBUG_BREAK() DEBUG_ONLY(DebugBreak())

#define ASSERT(condition) \
    DEBUG_ONLY( \
        if (condition) \
        { \
        } \
        else \
        { \
            DEBUG_BREAK(); \
        } \
    )

class DebugUtils final
{
public:
    DebugUtils() = delete;

public:
    static std::wstring COMErrorMessage(HRESULT hr);
};
