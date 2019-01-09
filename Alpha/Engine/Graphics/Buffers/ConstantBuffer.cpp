#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(::GraphicsDevice* graphicsDevice, UINT size, ID3D11Buffer* handle) :
    mGraphicsDevice{graphicsDevice},
    mSize{size},
    mHandle{handle}
{
}

GraphicsDevice* ConstantBuffer::GraphicsDevice() const
{
    return mGraphicsDevice;
}
