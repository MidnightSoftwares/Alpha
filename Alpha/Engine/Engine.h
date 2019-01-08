#pragma once

#include "Application/Application.h"
#include "Graphics/Graphics.h"

class Graphics;

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
        std::unique_ptr<Graphics> graphics);

public:
    static Expected<std::unique_ptr<Engine>, Error> Create(HINSTANCE hInstance,
        std::wstring windowClassName, std::wstring windowTitle, int windowWidth, int windowHeight);

public:
    void Run() const;

public:
    Graphics* Graphics() const;

private:
    std::unique_ptr<::Graphics> mGraphics;
};
