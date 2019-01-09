#include "WindowClass.h"

WindowClass::WindowClass(HINSTANCE hInstance, std::wstring name):
    mHInstance{hInstance},
    mName{std::move(name)}
{
}
