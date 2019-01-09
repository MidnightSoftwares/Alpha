#include "Graphics.h"
#include "../Application/Window.h"
#include "../Utils/Logger.h"

#include <d3dcompiler.h>

GraphicsDevicePtr Graphics::CreateGraphicsDevice(Window* window)
{
    // Graphics device deleter
    static const auto GRAPHICS_DEVICE_DELETER = [](GraphicsDevice* graphicsDevice)
    {
        graphicsDevice->mRenderTargetView->Release();
        graphicsDevice->mSwapChain->Release();
        graphicsDevice->mDeviceContext->Release();
        graphicsDevice->mDevice->Release();

        delete graphicsDevice;
    };

    // Create device, device context and swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    scd.BufferDesc.Width = window->mWidth;
    scd.BufferDesc.Height = window->mHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferCount = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window->mHandle;
    scd.Windowed = true;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;

    auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &swapChain,
        &device, nullptr, &deviceContext);

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3D11CreateDeviceAndSwapChain: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // Retrieve swap chain back buffer
    ID3D11Texture2D* scBackBuffer;

    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&scBackBuffer));

    if (FAILED(hr))
    {
        swapChain->Release();
        deviceContext->Release();
        device->Release();

        LOG_ERROR(L"IDXGISwapChain::GetBuffer: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // TODO: Maybe move this outside of GraphicsDevice
    // Create and set render target view
    ID3D11RenderTargetView* renderTargetView;

    hr = device->CreateRenderTargetView(scBackBuffer, nullptr, &renderTargetView);

    scBackBuffer->Release();

    if (FAILED(hr))
    {
        swapChain->Release();
        deviceContext->Release();
        device->Release();

        LOG_ERROR(L"ID3D11Device::CreateRenderTargetView: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

    // TODO: Move this to other method.
    // Create and set the viewport
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.f;
    viewport.TopLeftY = 0.f;
    viewport.Width = static_cast<FLOAT>(window->mWidth);
    viewport.Height = static_cast<FLOAT>(window->mHeight);
    viewport.MinDepth = 0.f;
    viewport.MaxDepth = 0.f;

    deviceContext->RSSetViewports(1, &viewport);

    // Create graphics device instance
    return GraphicsDevicePtr{
        new GraphicsDevice{
            window,
            device,
            deviceContext,
            swapChain,
            renderTargetView
        },
        GRAPHICS_DEVICE_DELETER
    };
}

RasterizerStatePtr Graphics::CreateRasterizerState(GraphicsDevice* graphicsDevice,
    const D3D11_RASTERIZER_DESC* description)
{
    // Rasterizer state deleter
    static const auto RASTERIZER_STATE_DELETER = [](RasterizerState* rasterizerState)
    {
        rasterizerState->mHandle->Release();

        delete rasterizerState;
    };

    // Create rasterizer state
    ID3D11RasterizerState* handle;

    const auto hr = graphicsDevice->mDevice->CreateRasterizerState(description, &handle);

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3D11Device::CreateRasterizerState: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // Create rasterizer state instance
    return RasterizerStatePtr{
        new RasterizerState{
            graphicsDevice,
            handle
        },
        RASTERIZER_STATE_DELETER
    };
}

VertexShaderPtr Graphics::CreateVertexShader(GraphicsDevice* graphicsDevice,
    const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements, const std::wstring& filePath)
{
    // Vertex shader deleter
    static const auto VERTEX_SHADER_DELETER = [](VertexShader* vertexShader)
    {
        vertexShader->mHandle->Release();
        vertexShader->mInputLayout->Release();

        delete vertexShader;
    };

    // Read file to buffer
    ID3DBlob* fileBuffer;

    auto hr = D3DReadFileToBlob(filePath.c_str(), &fileBuffer);

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3DReadFileToBlob: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Vertex shader file path: " + filePath);

        return nullptr;
    }

    // Create input layout
    ID3D11InputLayout* inputLayout;

    hr = graphicsDevice->mDevice->CreateInputLayout(layoutDesc, layoutElements,
        fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), &inputLayout);

    if (FAILED(hr))
    {
        fileBuffer->Release();

        LOG_ERROR(L"ID3D11Device::CreateInputLayout: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Vertex shader file path: " + filePath);

        return nullptr;
    }

    // Create vertex shader
    ID3D11VertexShader* handle;

    hr = graphicsDevice->mDevice->CreateVertexShader(
        fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), nullptr, &handle);

    fileBuffer->Release();

    if (FAILED(hr))
    {
        inputLayout->Release();

        LOG_ERROR(L"ID3D11Device::CreateVertexShader: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Vertex shader file path: " + filePath);

        return nullptr;
    }

    // Create vertex shader instance
    return VertexShaderPtr{
        new VertexShader{
            graphicsDevice,
            inputLayout,
            handle
        },
        VERTEX_SHADER_DELETER
    };
}

PixelShaderPtr Graphics::CreatePixelShader(GraphicsDevice* graphicsDevice,
    const std::wstring& filePath)
{
    // Pixel shader deleter
    static const auto PIXEL_SHADER_DELETER = [](PixelShader* pixelShader)
    {
        pixelShader->mHandle->Release();

        delete pixelShader;
    };

    // Read file to buffer
    ID3DBlob* fileBuffer;

    auto hr = D3DReadFileToBlob(filePath.c_str(), &fileBuffer);

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3DReadFileToBlob: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Pixel shader file path: " + filePath);

        return nullptr;
    }

    // Create pixel shader
    ID3D11PixelShader* handle;

    hr = graphicsDevice->mDevice->CreatePixelShader(
        fileBuffer->GetBufferPointer(), fileBuffer->GetBufferSize(), nullptr, &handle);

    fileBuffer->Release();

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateVertexShader: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Pixel shader file path: " + filePath);

        return nullptr;
    }

    // Create pixel shader instance
    return PixelShaderPtr{
        new PixelShader{
            graphicsDevice,
            handle
        },
        PIXEL_SHADER_DELETER
    };
}

VertexBufferPtr Graphics::CreateVertexBuffer(GraphicsDevice* graphicsDevice, D3D11_USAGE usage,
    UINT elementSize, UINT elementCount, const void* data)
{
    // Vertex buffer deleter
    static const auto VERTEX_BUFFER_DELETER = [](VertexBuffer* vertexBuffer)
    {
        vertexBuffer->mHandle->Release();

        delete vertexBuffer;
    };

    // Create buffer
    D3D11_BUFFER_DESC description;
    description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    description.ByteWidth = elementSize * elementCount;
    description.CPUAccessFlags = 0;
    description.MiscFlags = 0;
    description.StructureByteStride = 0;
    description.Usage = usage;

    D3D11_SUBRESOURCE_DATA sbData;
    sbData.SysMemPitch = 0;
    sbData.SysMemSlicePitch = 0;
    sbData.pSysMem = data;

    ID3D11Buffer* handle;

    const auto hr = graphicsDevice->mDevice->CreateBuffer(&description, &sbData, &handle);

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateBuffer: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // Create vertex buffer instance
    return VertexBufferPtr{
        new VertexBuffer{
            graphicsDevice,
            elementSize,
            elementCount,
            handle
        },
        VERTEX_BUFFER_DELETER
    };
}

IndexBufferPtr Graphics::CreateIndexBuffer(GraphicsDevice* graphicsDevice, D3D11_USAGE usage,
    UINT elementCount, const unsigned* data)
{
    // Index buffer deleter
    static const auto INDEX_BUFFER_DELETER = [](IndexBuffer* indexBuffer)
    {
        indexBuffer->mHandle->Release();

        delete indexBuffer;
    };

    // Create buffer
    D3D11_BUFFER_DESC description;
    description.BindFlags = D3D11_BIND_INDEX_BUFFER;
    description.ByteWidth = sizeof(unsigned int) * elementCount;
    description.CPUAccessFlags = 0;
    description.MiscFlags = 0;
    description.StructureByteStride = 0;
    description.Usage = usage;

    D3D11_SUBRESOURCE_DATA sbData;
    sbData.SysMemPitch = 0;
    sbData.SysMemSlicePitch = 0;
    sbData.pSysMem = data;

    ID3D11Buffer* handle;

    const auto hr = graphicsDevice->mDevice->CreateBuffer(&description, &sbData, &handle);

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateBuffer: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // Create index buffer instance
    return IndexBufferPtr{
        new IndexBuffer{
            graphicsDevice,
            elementCount,
            handle
        },
        INDEX_BUFFER_DELETER
    };
}

ConstantBufferPtr Graphics::CreateConstantBuffer(GraphicsDevice* graphicsDevice, UINT size,
    const void* data)
{
    // Constant buffer deleter
    static const auto CONSTANT_BUFFER_DELETER = [](ConstantBuffer* constantBuffer)
    {
        constantBuffer->mHandle->Release();

        delete constantBuffer;
    };

    // Create buffer
    D3D11_BUFFER_DESC description;
    description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    description.ByteWidth = size;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    description.MiscFlags = 0;
    description.StructureByteStride = size + 16 - size % 16;
    description.Usage = D3D11_USAGE_DYNAMIC;

    D3D11_SUBRESOURCE_DATA sbData;
    sbData.SysMemPitch = 0;
    sbData.SysMemSlicePitch = 0;
    sbData.pSysMem = data;

    ID3D11Buffer* handle;

    const auto hr = graphicsDevice->mDevice->CreateBuffer(&description, &sbData, &handle);

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateBuffer: " + DebugUtils::COMErrorMessage(hr));

        return nullptr;
    }

    // Create constant buffer instance
    return ConstantBufferPtr{
        new ConstantBuffer{
            graphicsDevice,
            size,
            handle
        },
        CONSTANT_BUFFER_DELETER
    };
}

void Graphics::TestRender(const GraphicsDevice* graphicsDevice,
    const RasterizerState* rasterizerState, const VertexShader* vertexShader,
    const PixelShader* pixelShader, const VertexBuffer* vertexBuffer)
{
    static const FLOAT CLEAR_COLOR[] = {0.f, 0.f, 1.f, 1.f};
    graphicsDevice->mDeviceContext->ClearRenderTargetView(
        graphicsDevice->mRenderTargetView, CLEAR_COLOR);

    graphicsDevice->mDeviceContext->RSSetState(rasterizerState->mHandle);

    graphicsDevice->mDeviceContext->VSSetShader(vertexShader->mHandle, nullptr, 0);
    graphicsDevice->mDeviceContext->IASetInputLayout(vertexShader->mInputLayout);

    graphicsDevice->mDeviceContext->PSSetShader(pixelShader->mHandle, nullptr, 0);

    UINT offset = 0;
    graphicsDevice->mDeviceContext->IASetVertexBuffers(0, 1,
        &vertexBuffer->mHandle, &vertexBuffer->mElementSize, &offset);
    graphicsDevice->mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    graphicsDevice->mDeviceContext->Draw(vertexBuffer->mElementCount, 0);

    graphicsDevice->mSwapChain->Present(1, 0);
}
