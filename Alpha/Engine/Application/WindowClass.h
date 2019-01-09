#pragma once

#include <Windows.h>
#include <string>
#include <functional>

class WindowClass final
{
private:
    WindowClass(HINSTANCE hInstance, std::wstring name);

private:
    HINSTANCE mHInstance;
    std::wstring mName;

private:
    friend class Application;
};

using WindowClassDeleter = std::function<void(WindowClass*)>;
using WindowClassPtr = std::unique_ptr<WindowClass, WindowClassDeleter>;
