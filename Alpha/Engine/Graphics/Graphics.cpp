#include "Graphics.h"
#include "../Window.h"
#include "../Core/Logger.h"

Graphics::Graphics(::Window* window,
    Microsoft::WRL::ComPtr<ID3D11Device> device,
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain,
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView,
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState,
    std::unique_ptr<Pipeline> pipeline) :
    mWindow{window},
    mDevice{std::move(device)},
    mDeviceContext{std::move(deviceContext)},
    mSwapChain{std::move(swapChain)},
    mRenderTargetView{std::move(renderTargetView)},
    mRasterizerState{std::move(rasterizerState)},
    mPipeline{std::move(pipeline)}
{
}

Expected<std::unique_ptr<Graphics>, Graphics::Error> Graphics::Create(::Window* window)
{
    // Create device and swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    scd.BufferDesc.Width = window->Width();
    scd.BufferDesc.Height = window->Height();
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferCount = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window->Handle();
    scd.Windowed = true;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

    auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, swapChain.GetAddressOf(),
        device.GetAddressOf(), nullptr, deviceContext.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3D11CreateDeviceAndSwapChain: " + DebugUtils::COMErrorMessage(hr));
        return CreateDeviceAndSwapChainFailed;
    }

    // Retrieve swap chain back buffer
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(backBuffer.GetAddressOf()));

    if (FAILED(hr))
    {
        LOG_ERROR(L"IDXGISwapChain::GetBuffer: " + DebugUtils::COMErrorMessage(hr));
        return GetSwapChainBackBufferFailed;
    }

    // Create render target view
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateRenderTargetView: " + DebugUtils::COMErrorMessage(hr));
        return CreateRenderTargetViewFailed;
    }

    // Set render target
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    // Create and set the viewport
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.f;
    viewport.TopLeftY = 0.f;
    viewport.Width = static_cast<FLOAT>(window->Width());
    viewport.Height = static_cast<FLOAT>(window->Height());
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 0.f;

    deviceContext->RSSetViewports(1, &viewport);

    // Create rasterizer state
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

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

    hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3D11Device::CreateRasterizerState: " + DebugUtils::COMErrorMessage(hr));
        return CreateRasterizerStateFailed;
    }

    // Create pipeline
    const auto& shadersDirectory = FileSystemUtils::ExecutableDirectory();

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            0,
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

    auto pipelineE = Pipeline::Create(device.Get(),
        layoutDesc, ARRAYSIZE(layoutDesc),
        shadersDirectory + L"\\VertexShader.cso", shadersDirectory + L"\\PixelShader.cso");

    if (!pipelineE.Valid())
    {
        return CreatePipelineFailed;
    }

    auto pipeline = std::move(pipelineE.Value());

    // Create graphics instance
    return std::unique_ptr<Graphics>{
        new Graphics{
            window,
            std::move(device),
            std::move(deviceContext),
            std::move(swapChain),
            std::move(renderTargetView),
            std::move(rasterizerState),
            std::move(pipeline)
        }
    };
}

void Graphics::RenderFrame() const
{
    static const float BG_COLOR[] = {0.f, 0.f, 0.f, 1.f};
    mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), BG_COLOR);

    //

    mSwapChain->Present(1, 0);
}

Window* Graphics::Window() const
{
    return mWindow;
}
