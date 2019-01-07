#pragma once

#include "Application.h"

class Engine final : public Application
{
public:
    enum Error
    {
        CreateWindowFailed,
        CreateGraphicsFailed
    };

private:
    Engine(HINSTANCE hInstance, std::unique_ptr<::Window> window,
        std::unique_ptr<::Graphics> graphics);

public:
    static Expected<std::unique_ptr<Engine>, Error> Create(HINSTANCE hInstance,
        std::wstring windowClassName, std::wstring windowTitle, int windowWidth, int windowHeight);

public:
    void Run() const;
};
