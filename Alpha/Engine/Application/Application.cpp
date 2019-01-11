#include "Application.h"
#include "../Utils/Logger.h"

WindowClassPtr Application::CreateWindowClass(HINSTANCE hInstance, std::wstring name)
{
    // Window class deleter
    static const auto WINDOW_CLASS_DELETER = [](WindowClass* windowClass)
    {
        UnregisterClass(windowClass->mName.c_str(), windowClass->mHInstance);

        delete windowClass;
    };

    // Register window class
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof wc;
    wc.cbWndExtra = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = HandleMessage;
    wc.lpszClassName = name.c_str();
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    if (!RegisterClassEx(&wc))
    {
        LOG_ERROR(L"RegisterClassEx: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Create window class instance
    return WindowClassPtr{
        new WindowClass{hInstance, std::move(name)},
        WINDOW_CLASS_DELETER
    };
}

WindowPtr Application::CreateWindow(WindowClass* windowClass, std::wstring title, int width,
    int height)
{
    // Window deleter
    static const auto WINDOW_DELETER = [](Window* window)
    {
        DestroyWindow(window->mHandle);

        delete window;
    };

    // Window style
    const auto windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    // Adjust window width and height and center its position
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
    const auto handle = CreateWindowEx(0, windowClass->mName.c_str(), title.c_str(),
        windowStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, windowClass->mHInstance, nullptr);

    if (!handle)
    {
        LOG_ERROR(L"CreateWindowEx: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Register raw input devices
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = handle;

    if (!RegisterRawInputDevices(&rid, 1, sizeof rid))
    {
        DestroyWindow(handle);

        LOG_ERROR(L"RegisterRawInputDevices: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Show and focus window
    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);

    // Create window instance
    auto window = WindowPtr{
        new Window{windowClass, std::move(title), width, height, handle},
        WINDOW_DELETER
    };

    // Set window user data to window instance
    SetLastError(0);

    if (!SetWindowLongPtr(window->mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.get())))
    {
        const auto lastError = GetLastError();

        if (lastError)
        {
            LOG_ERROR(L"SetWindowLongPtr: " + DebugUtils::COMErrorMessage(lastError));

            return nullptr;
        }
    }

    return window;
}

bool Application::PeekMessage(const Window* window)
{
    // NOTE: Static may be problematic if this function is called during message dispatch.
    // But this should not happens.
    static MSG msg{};

#pragma pop_macro("PeekMessage")
    if (!PeekMessage(&msg, window->mHandle, 0, 0, PM_REMOVE))
#pragma push_macro("PeekMessage")
#undef PeekMessage
    {
        return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    return true;
}

LRESULT Application::HandleMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    const auto window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));

    if (window)
    {
        switch (msg)
        {
            case WM_KEYDOWN:
            {
                if (window->AutoRepeatKeys() || !(lParam & 0x40000000))
                {
                    window->PushKeyPress(static_cast<unsigned char>(wParam));
                }

                return 0;
            }

            case WM_KEYUP:
            {
                window->PushKeyRelease(static_cast<unsigned char>(wParam));

                return 0;
            }

            case WM_CHAR:
            {
                if (window->AutoRepeatChars() || !(lParam & 0x40000000))
                {
                    window->PushCharInput(static_cast<wchar_t>(wParam));
                }

                return 0;
            }

            case WM_MOUSEMOVE:
            {
                window->PushMouseMove(LOWORD(lParam), HIWORD(lParam));

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
                    window->PushMouseRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }

                break;
            }

            case WM_LBUTTONDOWN:
            {
                window->PushMouseLeftPress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_LBUTTONUP:
            {
                window->PushMouseLeftRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_RBUTTONDOWN:
            {
                window->PushMouseRightPress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_RBUTTONUP:
            {
                window->PushMouseRightRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MBUTTONDOWN:
            {
                window->PushMouseMiddlePress(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MBUTTONUP:
            {
                window->PushMouseMiddleRelease(LOWORD(lParam), HIWORD(lParam));

                return 0;
            }

            case WM_MOUSEWHEEL:
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
                {
                    window->PushMouseWheelUp(LOWORD(lParam), HIWORD(lParam));
                }
                else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                {
                    window->PushMouseWheelDown(LOWORD(lParam), HIWORD(lParam));
                }

                return 0;
            }

            case WM_CLOSE:
            {
                window->SetCloseRequested(true);

                return 0;
            }

            default:
                break;
        }
    }

    return DefWindowProc(handle, msg, wParam, lParam);
}
