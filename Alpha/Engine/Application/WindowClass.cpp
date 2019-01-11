#include "WindowClass.h"
#include "../Utils/Logger.h"

WindowClass::WindowClass(HINSTANCE hInstance, std::wstring name) :
    mHInstance{hInstance},
    mName{std::move(name)}
{
}

WindowClass::~WindowClass()
{
    UnregisterClass(mName.c_str(), mHInstance);
}

std::unique_ptr<WindowClass> WindowClass::Create(HINSTANCE hInstance,
    std::wstring name, WNDPROC wndProc)
{
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
    wc.lpfnWndProc = wndProc;
    wc.lpszClassName = name.c_str();
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    if (!RegisterClassEx(&wc))
    {
        LOG_ERROR(L"RegisterClassEx: " + DebugUtils::COMErrorMessage(GetLastError()));

        return nullptr;
    }

    // Create window class instance
    return std::unique_ptr<WindowClass>{new WindowClass{hInstance, std::move(name)}};
}

HINSTANCE WindowClass::HInstance() const
{
    return mHInstance;
}

const std::wstring& WindowClass::Name() const
{
    return mName;
}
