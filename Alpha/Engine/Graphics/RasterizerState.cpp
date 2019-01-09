#include "RasterizerState.h"

RasterizerState::RasterizerState(::GraphicsDevice* graphicsDevice, ID3D11RasterizerState* handle):
    mGraphicsDevice{graphicsDevice},
    mHandle{handle}
{
}

GraphicsDevice* RasterizerState::GraphicsDevice() const
{
    return mGraphicsDevice;
}
