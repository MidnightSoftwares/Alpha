#include "Window.h"
#include "Application.h"
#include "Core/Logger.h"

Window::Window(HINSTANCE hInstance, std::wstring className,
    std::wstring title, int width, int height, HWND handle) :
    mHInstance{hInstance},
    mClassName{std::move(className)},
    mTitle{std::move(title)},
    mWidth{width},
    mHeight{height},
    mHandle{handle}
{
    // Point window user data to class instance
    SetWindowLongPtr(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

Window::~Window()
{
    DestroyWindow(mHandle);
    UnregisterClass(mClassName.c_str(), mHInstance);
}

Expected<std::unique_ptr<Window>, Window::Error> Window::Create(HINSTANCE hInstance,
    std::wstring className, std::wstring title, int width, int height)
{
    // Register window class
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = className.c_str();
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    if (!RegisterClassEx(&wc))
    {
        LOG_ERROR(L"RegisterClassEx: " + DebugUtils::COMErrorMessage(GetLastError()));
        return RegisterClassFailed;
    }

    // Create window
    const auto handle = CreateWindowEx(0, className.c_str(), title.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr);

    if (!handle)
    {
        // Unregister window class
        UnregisterClass(className.c_str(), hInstance);

        LOG_ERROR(L"CreateWindowEx: " + DebugUtils::COMErrorMessage(GetLastError()));
        return CreateWindowFailed;
    }

    // Register raw input devices
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = handle;

    if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
    {
        // Destroy window
        DestroyWindow(handle);
        // Unregister window class
        UnregisterClass(className.c_str(), hInstance);

        LOG_ERROR(L"RegisterRawInputDevices: " + DebugUtils::COMErrorMessage(GetLastError()));
        return RegisterRawInputDeviceFailed;
    }

    // Show and focus window
    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);

    // Create window instance
    return std::unique_ptr<Window>{
        new Window{
            hInstance,
            std::move(className),
            std::move(title),
            width,
            height,
            handle
        }
    };
}

bool Window::ProcessMessage() const
{
    MSG msg;
    if (!PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
    {
        return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    return true;
}

LRESULT Window::WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    if (window)
    {
        if (msg == WM_CLOSE)
        {
            window->mCloseRequested = true;
            return 0;
        }

        if (window->mApplication)
        {
            return window->mApplication->HandleWindowMessage(msg, wParam, lParam);
        }
    }

    return DefWindowProc(handle, msg, wParam, lParam);
}

HINSTANCE Window::HInstance() const
{
    return mHInstance;
}

const std::wstring& Window::ClassName() const
{
    return mClassName;
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

HWND Window::Handle() const
{
    return mHandle;
}

bool Window::CloseRequested() const
{
    return mCloseRequested;
}

void Window::SetCloseRequested(bool closeRequested)
{
    mCloseRequested = closeRequested;
}

Application* Window::Application() const
{
    return mApplication;
}
