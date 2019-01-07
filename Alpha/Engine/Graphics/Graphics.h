#pragma once

#include "../Core/Expected.h"

#include <d3d11.h>
#include <wrl/client.h>

class Window;

class Graphics final
{
public:
    enum Error
    {
        CreateDeviceAndSwapChainFailed,
        GetSwapChainBackBufferFailed,
        CreateRenderTargetViewFailed,
        CreateRasterizerStateFailed
    };

private:
    Graphics(Window* window,
        Microsoft::WRL::ComPtr<ID3D11Device> device,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain,
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView,
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState);

public:
    Graphics(const Graphics& graphics) = delete;
    Graphics(Graphics&& graphics) = delete;

    Graphics& operator=(const Graphics& graphics) = delete;
    Graphics& operator=(Graphics&& graphics) = delete;

    ~Graphics() = default;

public:
    static Expected<std::unique_ptr<Graphics>, Error> Create(Window* window);

public:
    void RenderFrame() const;

public:
    Window* Window() const;

private:
    ::Window* mWindow;
    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
};
