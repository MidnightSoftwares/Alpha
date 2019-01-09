#include "PixelShader.h"

PixelShader::PixelShader(::GraphicsDevice* graphicsDevice, ID3D11PixelShader* handle):
    mGraphicsDevice{graphicsDevice},
    mHandle{handle}
{
}

GraphicsDevice* PixelShader::GraphicsDevice() const
{
    return mGraphicsDevice;
}
