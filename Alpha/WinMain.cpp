#include "Engine/Application/Application.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Utils/FileSystem.h"
#include "Engine/Utils/Debug.h"

#include <DirectXMath.h>

struct Vertex final
{
    DirectX::XMFLOAT2 Position;
    DirectX::XMFLOAT3 Color;
};

// ReSharper disable once CppInconsistentNaming
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /* hPrevInstance */,
    _In_ LPWSTR /* lpCmdLine */,
    _In_ int /* nShowCmd */)
{
    const auto windowClass = Application::CreateWindowClass(hInstance, L"MainWindowClass");
    const auto window = Application::CreateWindow(windowClass.get(), L"Alpha", 1280, 720);

    const auto graphicsDevice = Graphics::CreateGraphicsDevice(window.get());

    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.f;
    rasterizerDesc.DepthClipEnable = false;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.SlopeScaledDepthBias = 0.f;

    const auto rasterizerState = Graphics::CreateRasterizerState(
        graphicsDevice.get(), &rasterizerDesc);

    const auto& shadersDirectory = FileSystemUtils::ExecutableDirectory();

    const D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    const auto vertexShader = Graphics::CreateVertexShader(graphicsDevice.get(),
        vertexLayoutDesc, ARRAYSIZE(vertexLayoutDesc),
        shadersDirectory + L"\\VertexShader.cso");

    const auto pixelShader = Graphics::CreatePixelShader(graphicsDevice.get(),
        shadersDirectory + L"\\PixelShader.cso");

    const Vertex vertices[] =
    {
        {{-0.5f, -0.5f}, {1.f, 0.f, 0.f}},
        {{0.0f, 0.5f}, {0.f, 1.f, 0.f}},
        {{0.5f, -0.5f}, {0.f, 0.f, 1.f}}
    };

    const auto vertexBuffer = Graphics::CreateVertexBuffer(graphicsDevice.get(),
        D3D11_USAGE_DEFAULT, sizeof(Vertex), ARRAYSIZE(vertices), vertices);

    while (!window->CloseRequested())
    {
        if (Application::PeekMessage(window.get()))
        {
            continue;
        }

        DEBUG_ONLY(
            if (window->KeyPressed(VK_ESCAPE))
            {
                window->SetCloseRequested(true);
            }
        )

        Graphics::TestRender(graphicsDevice.get(),
            rasterizerState.get(),
            vertexShader.get(), pixelShader.get(),
            vertexBuffer.get());

        Sleep(16);
    }

    return 0;
}
