#include "GraphicsDevice.h"

GraphicsDevice::GraphicsDevice(::Window* window, ID3D11Device* device,
    ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain,
    ID3D11Texture2D* scBackBuffer, ID3D11RenderTargetView* renderTargetView,
    ID3D11Texture2D* depthStencilBuffer, ID3D11DepthStencilView* depthStencilView) :
    mWindow{window},
    mDevice{device},
    mDeviceContext{deviceContext},
    mSwapChain{swapChain},
    mSCBackBuffer{scBackBuffer},
    mRenderTargetView{renderTargetView},
    mDepthStencilBuffer{depthStencilBuffer},
    mDepthStencilView{depthStencilView}
{
}

Window* GraphicsDevice::Window() const
{
    return mWindow;
}
