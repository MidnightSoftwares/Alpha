#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(::GraphicsDevice* graphicsDevice,
    UINT elementSize, UINT elementCount, ID3D11Buffer* handle) :
    mGraphicsDevice{graphicsDevice},
    mElementSize{elementSize},
    mElementCount{elementCount},
    mHandle{handle}
{
}

GraphicsDevice* VertexBuffer::GraphicsDevice() const
{
    return mGraphicsDevice;
}
