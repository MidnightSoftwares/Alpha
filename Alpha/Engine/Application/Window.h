#pragma once

#include "Input.h"

#include <Windows.h>
#include <functional>

class WindowClass;

class Window final : public Input
{
private:
    Window(WindowClass* windowClass, std::wstring title, int width, int height, HWND handle);

public:
    WindowClass* WindowClass() const;
    const std::wstring& Title() const;
    int Width() const;
    int Height() const;

    bool CloseRequested() const;
    void SetCloseRequested(bool closeRequested);

private:
    ::WindowClass* mWindowClass;
    std::wstring mTitle;
    int mWidth;
    int mHeight;
    HWND mHandle;
    bool mCloseRequested = false;

private:
    friend class Application;
    friend class Graphics;
};

using WindowDeleter = std::function<void(Window*)>;
using WindowPtr = std::unique_ptr<Window, WindowDeleter>;
