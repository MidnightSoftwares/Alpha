#pragma once

#include "../Utils/Expected.h"

class Application;

class Window final
{
public:
    enum Error
    {
        RegisterClassFailed,
        CreateWindowFailed,
        RegisterRawInputDeviceFailed
    };

private:
    Window(HINSTANCE hInstance, std::wstring className,
        std::wstring title, int width, int height, HWND handle);

public:
    Window(const Window& window) = delete;
    Window(Window&& window) = delete;

    Window& operator=(const Window& window) = delete;
    Window& operator=(Window&& window) = delete;

    ~Window();

public:
    static Expected<std::unique_ptr<Window>, Error> Create(HINSTANCE hInstance,
        std::wstring className, std::wstring title, int width, int height);

public:
    bool ProcessMessage() const;

private:
    static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    HINSTANCE HInstance() const;

    const std::wstring& ClassName() const;

    const std::wstring& Title() const;
    //void SetTitle(const std::wstring& title);

    int Width() const;
    //void SetWidth(int width);

    int Height() const;
    //void SetHeight(int height);

    HWND Handle() const;

    bool CloseRequested() const;
    void SetCloseRequested(bool closeRequested);

    Application* Application() const;

private:
    HINSTANCE mHInstance;
    std::wstring mClassName;
    std::wstring mTitle;
    int mWidth;
    int mHeight;
    HWND mHandle;
    bool mCloseRequested = false;
    ::Application* mApplication = nullptr;

private:
    friend ::Application;
};
