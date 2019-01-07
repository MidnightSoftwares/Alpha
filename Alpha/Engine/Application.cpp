#include "Application.h"

Application::Application(HINSTANCE hInstance, std::unique_ptr<::Window> window,
    std::unique_ptr<::Graphics> graphics) :
    mHInstance{hInstance},
    mWindow{std::move(window)},
    mKeyboard{std::make_unique<::Keyboard>()},
    mMouse{std::make_unique<::Mouse>()},
    mGraphics{std::move(graphics)}
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
            if (mKeyboard->AutoRepeatKeys() || !(lParam & 0x40000000))
            {
                mKeyboard->PushKeyPress(static_cast<unsigned char>(wParam));
            }

            return 0;
        }

        case WM_KEYUP:
        {
            mKeyboard->PushKeyRelease(static_cast<unsigned char>(wParam));

            return 0;
        }

        case WM_CHAR:
        {
            if (mKeyboard->AutoRepeatChars() || !(lParam & 0x40000000))
            {
                mKeyboard->PushChar(static_cast<unsigned char>(wParam));
            }

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            mMouse->PushMove(LOWORD(lParam), HIWORD(lParam));

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
                        mMouse->PushRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                    }
                }
            }

            return DefWindowProc(mWindow->mHandle, msg, wParam, lParam);
        }

        case WM_LBUTTONDOWN:
        {
            mMouse->PushLeftPress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_LBUTTONUP:
        {
            mMouse->PushLeftRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            mMouse->PushRightPress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_RBUTTONUP:
        {
            mMouse->PushRightRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MBUTTONDOWN:
        {
            mMouse->PushMiddlePress(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MBUTTONUP:
        {
            mMouse->PushMiddleRelease(LOWORD(lParam), HIWORD(lParam));

            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
            {
                mMouse->PushWheelUp(LOWORD(lParam), HIWORD(lParam));
            }
            else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
            {
                mMouse->PushWheelDown(LOWORD(lParam), HIWORD(lParam));
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

Keyboard* Application::Keyboard() const
{
    return mKeyboard.get();
}

Mouse* Application::Mouse() const
{
    return mMouse.get();
}

Graphics* Application::Graphics() const
{
    return mGraphics.get();
}
