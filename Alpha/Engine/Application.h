#pragma once

#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Graphics/Graphics.h"

class Application
{
protected:
    Application(HINSTANCE hInstance, std::unique_ptr<Window> window,
        std::unique_ptr<Graphics> graphics);

public:
    Application(const Application& application) = delete;
    Application(Application&& application) = delete;

    Application& operator=(const Application& application) = delete;
    Application& operator=(Application&& application) = delete;

    ~Application() = default;

private:
    LRESULT HandleWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;

public:
    HINSTANCE HInstance() const;
    Window* Window() const;
    Keyboard* Keyboard() const;
    Mouse* Mouse() const;
    Graphics* Graphics() const;

protected:
    HINSTANCE mHInstance;
    std::unique_ptr<::Window> mWindow;
    std::unique_ptr<::Keyboard> mKeyboard;
    std::unique_ptr<::Mouse> mMouse;
    std::unique_ptr<::Graphics> mGraphics;

private:
    friend ::Window;
};
