#include "Engine/Application/WindowClass.h"
#include "Engine/Application/Window.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/Utils/FileSystem.h"

#include <DirectXMath.h>

struct Vertex final
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Color;
};

// ReSharper disable once CppInconsistentNaming
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /* hPrevInstance */,
    _In_ LPWSTR /* lpCmdLine */,
    _In_ int /* nShowCmd */)
{
    // Create window
    const auto windowClass = WindowClass::Create(hInstance, L"MainWindowClass");
    const auto window = Window::Create(windowClass.get(), L"Alpha", 1280, 720);

    // Create graphics device
    const auto graphicsDevice = Graphics::CreateGraphicsDevice(window.get());

    // Create vertex buffer
    const Vertex vertices[] =
    {
        {{-0.75f, -0.75f, 1.f}, {1.f, 0.f, 0.f}},
        {{0.0f, 0.75f, 1.f}, {0.f, 1.f, 0.f}},
        {{0.75f, -0.75f, 1.f}, {0.f, 0.f, 1.f}}
    };

    const auto vertexBuffer = Graphics::CreateVertexBuffer(graphicsDevice.get(),
        D3D11_USAGE_DEFAULT, sizeof(Vertex), ARRAYSIZE(vertices), vertices);

    // Create second vertex buffer
    const Vertex vertices2[] =
    {
        {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}},
        {{0.0f, 0.5f, 0.f}, {1.f, 0.f, 0.f}},
        {{0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}}
    };

    const auto vertexBuffer2 = Graphics::CreateVertexBuffer(graphicsDevice.get(),
        D3D11_USAGE_DEFAULT, sizeof(Vertex), ARRAYSIZE(vertices2), vertices2);

    // Create shaders
    const auto& shadersDirectory = FileSystemUtils::ExecutableDirectory();

    const D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
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

    // Create pipeline
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

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    const auto pipeline = Graphics::CreatePipeline(graphicsDevice.get(),
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        vertexShader.get(),
        &rasterizerDesc,
        pixelShader.get(),
        &depthStencilDesc);

    // Set pipeline
    Graphics::TestSetPipeline(graphicsDevice.get(), pipeline.get());

    // Application loop
    while (!window->CloseRequested())
    {
        if (window->ProcessMessage())
        {
            continue;
        }

        while (!window->Keyboard()->KeyEventsEmpty())
        {
            const auto keyEvent = window->Keyboard()->PopKeyEvent();

            if (keyEvent.Type == KeyEvent::Release && keyEvent.KeyCode == VK_ESCAPE)
            {
                window->SetCloseRequested(true);
            }
        }

        while (!window->Keyboard()->CharsInputsEmpty())
        {
            window->Keyboard()->PopCharInput();
        }

        while (!window->Mouse()->EventsEmpty())
        {
            window->Mouse()->PopEvent();
        }

        Graphics::TestClear(graphicsDevice.get());
        Graphics::TestRender(graphicsDevice.get(), vertexBuffer.get());
        Graphics::TestRender(graphicsDevice.get(), vertexBuffer2.get());
        Graphics::SwapBuffers(graphicsDevice.get());

        Sleep(16);
    }

    return 0;
}
