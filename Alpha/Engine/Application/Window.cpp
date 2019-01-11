#include "Window.h"
#include "WindowClass.h"
#include "../Utils/Logger.h"

Window::Window(::WindowClass* windowClass, std::wstring title, int width, int height, HWND handle) :
    mWindowClass{windowClass},
    mTitle{std::move(title)},
    mWidth{width},
    mHeight{height},
    mHandle{handle}
{
}

Window::~Window()
{
    DestroyWindow(mHandle);
}

std::unique_ptr<Window> Window::Create(::WindowClass* windowClass,
    std::wstring title, int width, int height)
{
    // Window style
    const auto windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    // Adjust window width and height with centered position
    RECT rc;
    rc.left = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    rc.top = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
    rc.right = rc.left + width;
    rc.bottom = rc.top + height;

    if (!AdjustWindowRectEx(&rc, windowStyle, false, 0))
    {
        LOG_ERROR(L"AdjustWindowRectEx: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Create window
    const auto handle = CreateWindowEx(0, windowClass->Name().c_str(), title.c_str(),
        windowStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, windowClass->HInstance(), nullptr);

    if (!handle)
    {
        LOG_ERROR(L"CreateWindowEx: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Create window instance
    auto window = std::unique_ptr<Window>{
        new Window{windowClass, std::move(title), width, height, handle}
    };

    // Register raw input devices
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = window->mHandle;

    if (!RegisterRawInputDevices(&rid, 1, sizeof rid))
    {
        LOG_ERROR(L"RegisterRawInputDevices: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Set window user data to window instance
    SetLastError(0);

    SetWindowLongPtr(window->mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.get()));
    SetWindowLongPtr(window->mHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessage));

    const auto lastError = GetLastError();
    if (lastError)
    {
        LOG_ERROR(L"SetWindowLongPtr: " + DebugUtils::COMErrorMessage(lastError));

        return nullptr;
    }

    // Show and focus window
    ShowWindow(window->mHandle, SW_SHOW);
    SetForegroundWindow(window->mHandle);
    SetFocus(window->mHandle);

    // Return window class instance
    return window;
}

bool Window::ProcessMessage() const
{
    // NOTE: Static may be problematic if this function is called during message dispatch.
    // But this should not happens.
    static MSG msg{};

    if (!PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
    {
        return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    return true;
}

LRESULT Window::HandleMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));

    if (window)
    {
        switch (msg)
        {
            case WM_KEYDOWN:
            {
                if (window->mKeyboard.AutoRepeatKeys() || !(lParam & 0x40000000))
                {
                    window->mKeyboard.PushKeyPress(static_cast<unsigned char>(wParam));
                }

                return 0;
            }

            case WM_KEYUP:
            {
                window->mKeyboard.PushKeyRelease(static_cast<unsigned char>(wParam));

                return 0;
            }

            case WM_CHAR:
            {
                if (window->mKeyboard.AutoRepeatChars() || !(lParam & 0x40000000))
                {
                    window->mKeyboard.PushCharInput(static_cast<wchar_t>(wParam));
                }

                return 0;
            }

            case WM_MOUSEMOVE:
            {
                window->mMouse.PushMove(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_INPUT:
            {
                static BYTE rawData[40];

                UINT dataSize = 40;
                GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                    rawData, &dataSize, sizeof(RAWINPUTHEADER));

                const auto raw = reinterpret_cast<RAWINPUT*>(rawData);

                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    window->mMouse.PushRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }

                break;
            }

            case WM_LBUTTONDOWN:
            {
                window->mMouse.PushLeftPress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_LBUTTONUP:
            {
                window->mMouse.PushLeftRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_RBUTTONDOWN:
            {
                window->mMouse.PushRightPress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_RBUTTONUP:
            {
                window->mMouse.PushRightRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MBUTTONDOWN:
            {
                window->mMouse.PushMiddlePress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MBUTTONUP:
            {
                window->mMouse.PushMiddleRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MOUSEWHEEL:
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                {
                    window->mMouse.PushWheelUp(LOWORD(lParam), HIWORD(lParam));
                }
                else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                {
                    window->mMouse.PushWheelDown(LOWORD(lParam), HIWORD(lParam));
                }

                return 0;
            }

            case WM_CLOSE:
            {
                window->mCloseRequested = true;

                return 0;
            }

            default:
                break;
        }
    }

    return DefWindowProc(handle, msg, wParam, lParam);
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

HWND Window::Handle() const
{
    return mHandle;
}

KeyboardInput* Window::Keyboard() const
{
    return &mKeyboard;
}

MouseInput* Window::Mouse() const
{
    return &mMouse;
}

bool Window::CloseRequested() const
{
    return mCloseRequested;
}

void Window::SetCloseRequested(bool closeRequested)
{
    mCloseRequested = closeRequested;
}
