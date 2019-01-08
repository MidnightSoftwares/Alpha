#include "Application.h"

Application::Application(HINSTANCE hInstance, std::unique_ptr<::Window> window) :
    mHInstance{hInstance},
    mWindow{std::move(window)},
    mInput{std::make_unique<::Input>()}
{
    // Point window application to class instance
    mWindow->mApplication = this;
}

LRESULT Application::HandleWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) const
{
    switch (msg)
    {
        case WM_KEYDOWN:
        {
            if (mInput->AutoRepeatKeys() || !(lParam & 0x40000000))
            {
                mInput->PushKeyPress(static_cast<unsigned char>(wParam));
            }

            return 0;
        }

        case WM_KEYUP:
        {
            mInput->PushKeyRelease(static_cast<unsigned char>(wParam));

            return 0;
        }

        case WM_CHAR:
        {
            if (mInput->AutoRepeatChars() || !(lParam & 0x40000000))
            {
                mInput->PushChar(static_cast<wchar_t>(wParam));
            }

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            mInput->PushMouseMove(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_INPUT:
        {
            UINT dataSize;
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                nullptr, &dataSize, sizeof(RAWINPUTHEADER));

            if (dataSize)
            {
                auto rawData = std::make_unique<BYTE[]>(dataSize);
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                    rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)))
                {
                    const auto raw = reinterpret_cast<RAWINPUT*>(rawData.get());
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        mInput->PushMouseRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                    }
                }
            }

            return DefWindowProc(mWindow->mHandle, msg, wParam, lParam);
        }

        case WM_LBUTTONDOWN:
        {
            mInput->PushMouseLeftPress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_LBUTTONUP:
        {
            mInput->PushMouseLeftRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            mInput->PushMouseRightPress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_RBUTTONUP:
        {
            mInput->PushMouseRightRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MBUTTONDOWN:
        {
            mInput->PushMouseMiddlePress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MBUTTONUP:
        {
            mInput->PushMouseMiddleRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
            {
                mInput->PushMouseWheelUp(LOWORD(lParam), HIWORD(lParam));
            }
            else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
            {
                mInput->PushMouseWheelDown(LOWORD(lParam), HIWORD(lParam));
            }

            return 0;
        }

        default:
        {
            return DefWindowProc(mWindow->mHandle, msg, wParam, lParam);
        }
    }
}

HINSTANCE Application::HInstance() const
{
    return mHInstance;
}

Window* Application::Window() const
{
    return mWindow.get();
}

Input* Application::Input() const
{
    return mInput.get();
}
