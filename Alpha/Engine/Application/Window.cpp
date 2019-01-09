#include "Window.h"

Window::Window(::WindowClass* windowClass, std::wstring title, int width, int height, HWND handle):
    mWindowClass{windowClass},
    mTitle{std::move(title)},
    mWidth{width},
    mHeight{height},
    mHandle{handle}
{
}

WindowClass* Window::WindowClass() const
{
    return mWindowClass;
}

const std::wstring& Window::Title() const
{
    return mTitle;
}

int Window::Width() const
{
    return mWidth;
}

int Window::Height() const
{
    return mHeight;
}

bool Window::CloseRequested() const
{
    return mCloseRequested;
}

void Window::SetCloseRequested(bool closeRequested)
{
    mCloseRequested = closeRequested;
}
