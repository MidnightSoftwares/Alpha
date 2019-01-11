#pragma once

#include <Windows.h>
#include <string>

class WindowClass final
{
private:
    WindowClass(HINSTANCE hInstance, std::wstring name);

public:
    WindowClass(const WindowClass& windowClass) = delete;
    WindowClass(WindowClass&& windowClass) = delete;

    WindowClass& operator=(const WindowClass& windowClass) = delete;
    WindowClass& operator=(WindowClass&& windowClass) = delete;

    ~WindowClass();

public:
    static std::unique_ptr<WindowClass> Create(HINSTANCE hInstance,
        std::wstring name, WNDPROC wndProc = DefWindowProc);

public:
    HINSTANCE HInstance() const;
    const std::wstring& Name() const;

private:
    HINSTANCE mHInstance;
    std::wstring mName;
};
