#pragma once

#include "WindowClass.h"
#include "Window.h"

#pragma push_macro("CreateWindow")
#undef CreateWindow

#pragma push_macro("PeekMessage")
#undef PeekMessage

class Application final
{
public:
    Application() = delete;

public:
    static WindowClassPtr CreateWindowClass(HINSTANCE hInstance, std::wstring name);
    static WindowPtr CreateWindow(WindowClass* windowClass,
        std::wstring title, int width, int height);

public:
    static bool PeekMessage(Window* window);

private:
    static LRESULT CALLBACK HandleMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
};
