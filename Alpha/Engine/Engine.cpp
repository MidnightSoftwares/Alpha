#include "Engine.h"

Engine::Engine(HINSTANCE hInstance, std::unique_ptr<::Window> window) :
    Application{hInstance, std::move(window)}
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

    // Create engine instance
    return std::unique_ptr<Engine>{
        new Engine{
            hInstance,
            std::move(window)
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

        while (!mKeyboard->KeyQueueEmpty())
        {
            const auto keyEvent = mKeyboard->PopKey();

            DEBUG_ONLY(
                if (keyEvent.KeyCode == VK_ESCAPE && keyEvent.Type == KeyEvent::Release)
                {
                    mWindow->SetCloseRequested(true);
                }
            );

            //
        }

        while (!mKeyboard->CharQueueEmpty())
        {
            const auto ch = mKeyboard->PopChar();

            //
        }

        while (!mMouse->EventQueueEmpty())
        {
            const auto mouseEvent = mMouse->PopEvent();

            //
        }

        Sleep(16);
    }
}
