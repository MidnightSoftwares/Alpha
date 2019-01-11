#pragma once

#include "KeyboardInput.h"
#include "MouseInput.h"

#include <Windows.h>

class WindowClass;

class Window final
{
private:
    Window(WindowClass* windowClass, std::wstring title, int width, int height, HWND handle);

public:
    Window(const Window& window) = delete;
    Window(Window&& window) = delete;

    Window& operator=(const Window& window) = delete;
    Window& operator=(Window&& window) = delete;

    ~Window();

public:
    static std::unique_ptr<Window> Create(WindowClass* windowClass,
        std::wstring title, int width, int height);

public:
    bool ProcessMessage() const;

private:
    static LRESULT CALLBACK HandleMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    WindowClass* WindowClass() const;

    const std::wstring& Title() const;
    // SetTitle

    int Width() const;
    // SetWidth

    int Height() const;
    // SetHeight

    HWND Handle() const;

    KeyboardInput* Keyboard() const;
    MouseInput* Mouse() const;

    bool CloseRequested() const;
    void SetCloseRequested(bool closeRequested);

private:
    ::WindowClass* mWindowClass;
    std::wstring mTitle;
    int mWidth;
    int mHeight;
    HWND mHandle;
    mutable KeyboardInput mKeyboard;
    mutable MouseInput mMouse;
    bool mCloseRequested = false;
};
