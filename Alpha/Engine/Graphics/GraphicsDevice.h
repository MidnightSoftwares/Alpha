#pragma once

#include <d3d11.h>
#include <functional>

class Window;

class GraphicsDevice final
{
private:
    GraphicsDevice(Window* window,
        ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain,
        ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);

public:
    Window* Window() const;

private:
    ::Window* mWindow;
    ID3D11Device* mDevice;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11DepthStencilView* mDepthStencilView;

private:
    friend class Graphics;
};

using GraphicsDeviceDeleter = std::function<void(GraphicsDevice*)>;
using GraphicsDevicePtr = std::unique_ptr<GraphicsDevice, GraphicsDeviceDeleter>;
