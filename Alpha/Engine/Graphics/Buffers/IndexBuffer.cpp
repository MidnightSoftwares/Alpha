#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(::GraphicsDevice* graphicsDevice,
    UINT elementCount, ID3D11Buffer* handle) :
    mGraphicsDevice{graphicsDevice},
    mElementCount{elementCount},
    mHandle{handle}
{
}

GraphicsDevice* IndexBuffer::GraphicsDevice() const
{
    return mGraphicsDevice;
}
