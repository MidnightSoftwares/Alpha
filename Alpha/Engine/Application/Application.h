#pragma once

#include "Window.h"
#include "Input.h"

class Application
{
protected:
    Application(HINSTANCE hInstance, std::unique_ptr<Window> window);

public:
    Application(const Application& application) = delete;
    Application(Application&& application) = delete;

    Application& operator=(const Application& application) = delete;
    Application& operator=(Application&& application) = delete;

    virtual ~Application() = default;

protected:
    virtual LRESULT HandleWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;

public:
    HINSTANCE HInstance() const;
    Window* Window() const;
    Input* Input() const;

protected:
    HINSTANCE mHInstance;
    std::unique_ptr<::Window> mWindow;
    std::unique_ptr<::Input> mInput;

private:
    friend ::Window;
};
