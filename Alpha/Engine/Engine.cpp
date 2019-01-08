#include "Engine.h"

Engine::Engine(HINSTANCE hInstance, std::unique_ptr<::Window> window,
    std::unique_ptr<::Graphics> graphics) :
    Application{hInstance, std::move(window)},
    mGraphics{std::move(graphics)}
{
}

Expected<std::unique_ptr<Engine>, Engine::Error> Engine::Create(HINSTANCE hInstance,
    std::wstring windowClassName, std::wstring windowTitle, int windowWidth, int windowHeight)
{
    // Create window
    auto windowE = Window::Create(hInstance,
        std::move(windowClassName), std::move(windowTitle), windowWidth, windowHeight);

    if (!windowE.Valid())
    {
        return CreateWindowFailed;
    }

    auto window = std::move(windowE.Value());

    // Create graphics
    auto graphicsE = Graphics::Create(window.get());

    if (!graphicsE.Valid())
    {
        return CreateGraphicsFailed;
    }

    auto graphics = std::move(graphicsE.Value());

    // Create engine instance
    return std::unique_ptr<Engine>{
        new Engine{
            hInstance,
            std::move(window),
            std::move(graphics)
        }
    };
}

void Engine::Run() const
{
    while (!mWindow->CloseRequested())
    {
        if (mWindow->ProcessMessage())
        {
            continue;
        }

        while (!mInput->KeyEventQueueEmpty())
        {
            const auto keyEvent = mInput->PopKeyEvent();

            DEBUG_ONLY(
                if (keyEvent.KeyCode == VK_ESCAPE && keyEvent.Type == KeyEvent::Release)
                {
                    mWindow->SetCloseRequested(true);
                }
            );

            //
        }

        while (!mInput->CharQueueEmpty())
        {
            const auto ch = mInput->PopChar();

            //
        }

        while (!mInput->MouseEventQueueEmpty())
        {
            const auto mouseEvent = mInput->PopMouseEvent();

            //
        }

        //

        mGraphics->RenderFrame();

        Sleep(16);
    }
}

Graphics* Engine::Graphics() const
{
    return mGraphics.get();
}
