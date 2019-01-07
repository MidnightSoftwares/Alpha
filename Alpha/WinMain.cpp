#include "Engine/Engine.h"

// ReSharper disable once CppInconsistentNaming
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /* hPrevInstance */,
    _In_ LPWSTR /* lpCmdLine */,
    _In_ int /* nShowCmd */)
{
    auto engineE = Engine::Create(hInstance, L"MainWindowClass", L"Alpha", 1280, 720);
    if (!engineE.Valid())
    {
        DEBUG_BREAK();
        return -1;
    }

    const auto engine = std::move(engineE.Value());
    engine->Run();

    return 0;
}
